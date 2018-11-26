#pragma once
#include <functional>
#include <algorithm>
#include "tuple_serialization.h"

namespace mathlab
{
  // block interface
  struct block {
    virtual double eval(double input) const = 0;
    virtual void dump(std::ostream& to_stream) const = 0;
    virtual ~block() = default;
  };

  template<typename TBlock, typename TCallable, typename ...TArgs>
  struct block_with_constants : block {
    double eval(double input) const override {
      auto input_with_constants = std::tuple_cat(std::tuple<double>(input), constants_);
      return std::apply(callable_, input_with_constants);
    }
    // Serializes all constants to stream
    void dump(std::ostream& to) const override {
      tuple_serialization<sizeof...(TArgs), TArgs...>::serialize(to, constants_);
    }
    // Creates new block from stream
    // throws invalid_argument if stream cannot be converted
    static std::unique_ptr<TBlock> create_from_stream(std::istream& input_stream) {
      std::tuple<TArgs...> to;
      tuple_serialization<sizeof...(TArgs), TArgs...>::deserialize(input_stream, to);
      TBlock* block = std::apply(create<TBlock, TArgs...>, to);
      return std::unique_ptr<TBlock>(block);
    }
  protected:
    block_with_constants(TCallable callable, TArgs... args) : callable_(std::move(callable)), constants_(std::tie(args...)) {}
  private:
    std::tuple<TArgs...> constants_;
    TCallable callable_;
  };

  std::ostream& operator<<(std::ostream& stream, const block& block);

  // Supported blocks:

  struct identity final : block_with_constants<identity, std::function<double(double)>> {
    identity() : block_with_constants([](auto input) { return input; }) {}
  };

  struct addition final : block_with_constants<addition, std::plus<double>, double> {
    addition(double constant) : block_with_constants(std::plus<double>(), constant) {}
  };

  struct multiplication final : block_with_constants<multiplication, std::multiplies<double>, double> {
    multiplication(double constant) : block_with_constants(std::multiplies<double>(), constant) {}
  };

  struct power final : block_with_constants<power, std::function<double(double, double)>, double> {
    power(double constant) : block_with_constants([](double input, double other) { return std::pow(input, other); }, constant) {}
  };
  
  struct condition final : block_with_constants<condition, std::function<double(double, double)>, double> {
    condition(double constant) : 
      block_with_constants([](double input, double other) { return input < other ? -1 : (input == other ? 0 : 1); }, constant) {}
  };

  struct limit final : block_with_constants<limit, std::function<double(double, double, double)>, double, double> {
    limit(double lower, double upper) : block_with_constants(std::clamp<double>, lower, upper) {}
  };
}

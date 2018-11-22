#pragma once
#include <array>
#include <sstream>
#include <iterator>

namespace mathlab
{
  // block interface
  class block {
  public:
    virtual double eval(double input) const = 0;
    virtual void dump(std::ostream& to_stream) const = 0;
    virtual ~block() = default;
  };

  // represents block with N constants (inputs) that can be constructed from stream and be written to stream
  template<size_t N>
  class block_with_constants : public block {
  protected:
    std::array<double, N> constants_;
    std::string type_;

    // "It" represents iterator that contains (at least) N constants
    // throws invalid_argument if iterator does not contain exactly N values of type double
    template<typename It> block_with_constants(const std::string& type, It begin, It end) : type_(type) {
      // In case of istream_iterator, std::difference advances iterator => not able to do the check
      auto index = 0U;
      for (; begin != end; ++begin) {
        if (index++ < N)
          constants_[index - 1] = *begin;
        else break;
      }
      if (index != N)
        throw std::invalid_argument("Expected number of constants: " + std::to_string(N) + " but was: " + std::to_string(index));
    }

    // throws invalid_argument if initializer list does not contain exactly N values of type double
    block_with_constants(const std::string& type, std::initializer_list<double> list) : block_with_constants(type, list.begin(), list.end()) { }

  public:
    // Each block outputs: "block_type[ constant1][ constant2][...constantN]\n"
    void dump(std::ostream& stream) const override {
      stream << type_;
      std::for_each(constants_.begin(), constants_.end(), [&stream](double constant) { stream << ' ' << constant; });
      stream << std::endl;
    }
  };

  std::ostream& operator<<(std::ostream& stream, const block& block);

  struct identity final : block_with_constants<0> {
    inline static const char* my_type = "identity";
    identity() : block_with_constants(my_type, {}) {}
    double eval(double input) const override { return input; }
  };

  struct addition final : block_with_constants<1> {
    inline static const char* my_type = "addition";
    addition(double constant) : block_with_constants(my_type, { constant }) {}
    addition(std::istream& stream) : block_with_constants(my_type, std::istream_iterator<double>(stream), std::istream_iterator<double>()) {}
    double eval(double input) const override { return input + constants_[0]; }
  };

  struct multiplication final : block_with_constants<1> {
    inline static const char* my_type = "multiplication";
    multiplication(double constant) : block_with_constants(my_type, { constant }) {}
    multiplication(std::istream& stream) : block_with_constants(my_type, std::istream_iterator<double>(stream), std::istream_iterator<double>()) {}
    double eval(double input) const override { return input * constants_[0]; }
  };
  
  struct power final : block_with_constants<1> {
    inline static const char* my_type = "power";
    power(double constant) : block_with_constants(my_type, { constant }) {}
    power(std::istream& stream) : block_with_constants(my_type, std::istream_iterator<double>(stream), std::istream_iterator<double>()) {}
    double eval(double input) const override { return std::pow(input, constants_[0]); }
  };

  struct condition final : block_with_constants<1> {
    inline static const char* my_type = "condition";
    condition(double constant) : block_with_constants(my_type, { constant }) {}
    condition(std::istream& stream) : block_with_constants(my_type, std::istream_iterator<double>(stream), std::istream_iterator<double>()) {}
    double eval(double input) const override { return input < constants_[0] ? -1 : (input == constants_[0] ? 0 : 1); }
  };

  struct limit final : block_with_constants<2> {
    inline static const char* my_type = "limit";
    limit(double lower, double upper) : block_with_constants(my_type, { lower, upper }) {}
    limit(std::istream& stream) : block_with_constants(my_type, std::istream_iterator<double>(stream), std::istream_iterator<double>()) {}
    double eval(double input) const override { return std::clamp(input, constants_[0], constants_[1]); }
  };
}

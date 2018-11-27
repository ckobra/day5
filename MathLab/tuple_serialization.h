#pragma once
#include <stdexcept>
#include <typeinfo>
#include <ostream>

namespace mathlab {
  
  // Tuple serialization / de-serialization
  // Type arguments:
  //  N - number of tuple members that are considered
  //  TArgs - tuple types (e.g. double, int => tuple<double, int>
  template<size_t N, typename ...TArgs>
  struct tuple_serialization {

    // Serializes first N tuple members to output stream
    // A space is added after each member
    static void serialize(std::ostream& to, const std::tuple<TArgs...>& from) {
      tuple_serialization<N - 1, TArgs...>::serialize(to, from);
      to << std::get<N - 1>(from) << ' ';
    }

    // De-serializes first N tuple members from input stream
    // Throws std::invalid_argument when reading from stream fails
    static void deserialize(std::istream& from, std::tuple<TArgs...>& to) {
      tuple_serialization<N - 1, TArgs...>::deserialize(from, to);
      using type_n = typename std::tuple_element<N - 1, std::tuple<TArgs...>>::type;
      type_n object;
      from >> object;
      if (! from) // bad-bit or eof
        throw std::invalid_argument(std::string("Unable to read object from stream. Object type: ") + typeid(type_n).name());
      std::get<N - 1>(to) = object;
    }
  };

  // Specialization for empty tuple, doing nothing
  template<typename ...TArgs>
  struct tuple_serialization<0, TArgs...> {
    static void serialize(std::ostream& to, const std::tuple<TArgs...>& from) {}
    static void deserialize(std::istream& from, std::tuple<TArgs...>& to) {}
  };

  // Helper function for constructing an object of type TBlock, using variable constructor arguments
  template<typename T, typename ...TArgs>
  T* create(TArgs ...args) {
    return new T(args...);
  }

}
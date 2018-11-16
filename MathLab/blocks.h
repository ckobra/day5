#pragma once
#include <functional>
#include <algorithm>

namespace mathlab
{

class block {
  std::function<double(double)> fn_;
public:
  block(std::function<double(double)> fn) : fn_(fn) {  }
  double eval(double input) { return fn_(input); }
};

class identity : public block {
public:
  identity() : block([](double input) {return input; }) {}
};

class addition : public block {
public:
  addition(double constant) : block([constant](double input) {return input + constant; }) {}
};

class multiplication : public block {
public:
  multiplication(double constant) : block([constant](double input) {return input * constant; }) {}
};

class power : public block {
public:
  power(double constant) : block([constant](double input) {return std::pow(input, constant); }) {}
};

class condition : public block {
public:
  condition(double constant) : block([constant](double input) {return input < constant ? -1 : (input == constant ? 0 : 1); }) {}
};

class limit : public block {
public:
  limit(double lower, double upper) : block([lower, upper](double input) {return std::clamp(input, lower, upper); }) {}
};

}
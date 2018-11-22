#include "CppUnitTest.h"
#include "../MathLab/factory.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MathLabTests
{
  TEST_CLASS(factory_tests)
  {
  public:
    TEST_METHOD(dump_after_register)
    {
      std::ostringstream output;
      auto factory = mathlab::factory();
      factory.dump_registered(output);
      Assert::AreEqual(std::string(""), output.str());
      factory.register_block("dummy1", mathlab::factory::fn_type());
      factory.register_block("dummy2", mathlab::factory::fn_type());
      factory.dump_registered(output);
      Assert::AreEqual(std::string("dummy1 dummy2 "), output.str());
    }

    TEST_METHOD(create_for_non_registered_fails)
    {
      std::istringstream input;
      auto factory = mathlab::factory();
      const auto ptr = factory.create("dummy", input);
      Assert::IsFalse(static_cast<bool>(ptr));
    }

    TEST_METHOD(create_after_registration)
    {
      std::istringstream input;
      auto factory = mathlab::factory();
      factory.register_block("dummy", [](std::istream&) { return new mathlab::identity(); });
      const auto ptr = factory.create("dummy", input);
      Assert::AreEqual(123.45, ptr->eval(123.45));
    }

    TEST_METHOD(create_after_registration_with_wrong_number_of_constants_throws)
    {
      std::istringstream input;
      auto factory = mathlab::factory();
      factory.register_block(mathlab::power::my_type, [](std::istream& stream) { return new mathlab::power(stream); });
      Assert::ExpectException<std::invalid_argument>([&factory, &input]() { factory.create(mathlab::power::my_type, input); });
    }
  };
}
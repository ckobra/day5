#include "CppUnitTest.h"
#include "../MathLab/blocks.h"
#include <ostream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MathLabTests
{		
	TEST_CLASS(block_types_directly_constructed)
	{
	public:
		TEST_METHOD(identity_returns_input)
		{
      auto identity = mathlab::identity();
      Assert::AreEqual(3.14, identity.eval(3.14));
		}
	
    TEST_METHOD(addition_works)
    {
      auto addition = mathlab::addition(100.0);
      Assert::AreEqual(103.14, addition.eval(3.14));
    }

    TEST_METHOD(multiplication_works)
    {
      auto multiplication = mathlab::multiplication(100.0);
      Assert::AreEqual(314., multiplication.eval(3.14));
    }

    TEST_METHOD(power_works)
    {
      auto power = mathlab::power(3.);
      Assert::AreEqual(15.625, power.eval(2.5));
    }

    TEST_METHOD(condition_returns_0_if_same_as_input)
    {
      auto condition = mathlab::condition(3.14);
      Assert::AreEqual(0., condition.eval(3.14));
    }

    TEST_METHOD(condition_returns_minus_1_if_less_than_input)
    {
      auto condition = mathlab::condition(3.14);
      Assert::AreEqual(-1., condition.eval(0.5));
    }

    TEST_METHOD(condition_returns_1_if_greater_than_input)
    {
      auto condition = mathlab::condition(3.14);
      Assert::AreEqual(1., condition.eval(5.));
    }

    TEST_METHOD(limit_returns_value_if_within_limits)
    {
      auto limit = mathlab::limit(1., 100.);
      Assert::AreEqual(50., limit.eval(50.));
    }

    TEST_METHOD(limit_returns_lower_if_value_is_lower)
    {
      auto limit = mathlab::limit(1., 100.);
      Assert::AreEqual(1., limit.eval(-50.));
    }

    TEST_METHOD(limit_returns_upper_if_value_is_above)
    {
      auto limit = mathlab::limit(1., 100.);
      Assert::AreEqual(100., limit.eval(200.));
    }
	};

  TEST_CLASS(blocks_can_be_created_from_stream_and_dumped_to_stream)
  {
  public:
    TEST_METHOD(block_without_constants)
    {
      auto identity = mathlab::identity();
      std::ostringstream output_stream;
      output_stream << identity;
      Assert::AreEqual(std::string(""), output_stream.str());
    }

    TEST_METHOD(block_with_one_constant)
    {
      std::istringstream input("123.45");
      auto block = mathlab::addition::create_from_stream<mathlab::addition>(input);
      std::ostringstream output_stream;
      output_stream << *block;
      Assert::AreEqual(std::string("123.45 "), output_stream.str());
    }

    TEST_METHOD(block_with_one_constant_throws_if_no_input)
    {
      Assert::ExpectException<std::invalid_argument>([]()
      {
        std::istringstream input("");
        mathlab::multiplication::create_from_stream<mathlab::multiplication>(input);
      });
    }

    TEST_METHOD(block_with_one_constant_throws_in_case_of_wrong_input_type)
    {
      Assert::ExpectException<std::invalid_argument>([]()
      {
        std::istringstream input("Hello");
        mathlab::multiplication::create_from_stream<mathlab::multiplication>(input);
      });
    }

    TEST_METHOD(block_with_two_constants)
    {
      std::istringstream input("-123.45 666.66");
      auto limit = mathlab::limit::create_from_stream<mathlab::limit>(input);
      std::ostringstream output_stream;
      output_stream << *limit;
      Assert::AreEqual(std::string("-123.45 666.66 "), output_stream.str());
    }

    TEST_METHOD(block_with_two_constants_throws_if_no_input)
    {
      Assert::ExpectException<std::invalid_argument>([]()
      {
        std::istringstream input("");
        mathlab::limit::create_from_stream<mathlab::limit>(input);
      });
    }

    TEST_METHOD(block_with_two_constants_throws_in_case_of_wrong_input_type)
    {
      Assert::ExpectException<std::invalid_argument>([]()
      {
        std::istringstream input("Hello World");
        mathlab::multiplication::create_from_stream<mathlab::multiplication>(input);
      });
    }
  };
}
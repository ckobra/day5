#include "CppUnitTest.h"
#include "../MathLab/blocks.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MathLabTests
{		
	TEST_CLASS(block_types)
	{
	public:
		
		TEST_METHOD(identity_returns_input)
		{
      mathlab::block identity = mathlab::identity();
      Assert::AreEqual(3.14, identity.eval(3.14));
		}
	
    TEST_METHOD(addition_works)
    {
      mathlab::block block = mathlab::addition(100.0);
      Assert::AreEqual(103.14, block.eval(3.14));
    }

    TEST_METHOD(multiplication_works)
    {
      mathlab::block block = mathlab::multiplication(100.0);
      Assert::AreEqual(314., block.eval(3.14));
    }

    TEST_METHOD(power_works)
    {
      mathlab::block block = mathlab::power(3.);
      Assert::AreEqual(15.625, block.eval(2.5));
    }

    TEST_METHOD(condition_returns_0_if_same_as_input)
    {
      mathlab::block block = mathlab::condition(3.14);
      Assert::AreEqual(0., block.eval(3.14));
    }

    TEST_METHOD(condition_returns_minus_1_if_less_than_input)
    {
      mathlab::block block = mathlab::condition(3.14);
      Assert::AreEqual(-1., block.eval(0.5));
    }

    TEST_METHOD(condition_returns_1_if_greater_than_input)
    {
      mathlab::block block = mathlab::condition(3.14);
      Assert::AreEqual(1., block.eval(5.));
    }

    TEST_METHOD(limit_returns_value_if_within_limits)
    {
      mathlab::block block = mathlab::limit(1., 100.);
      Assert::AreEqual(50., block.eval(50.));
    }

    TEST_METHOD(limit_returns_lower_if_value_is_lower)
    {
      mathlab::block block = mathlab::limit(1., 100.);
      Assert::AreEqual(1., block.eval(-50.));
    }

    TEST_METHOD(limit_returns_upper_if_value_is_above)
    {
      mathlab::block block = mathlab::limit(1., 100.);
      Assert::AreEqual(100., block.eval(200.));
    }
	};
}
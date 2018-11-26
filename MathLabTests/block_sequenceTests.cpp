#include "CppUnitTest.h"
#include "../MathLab/block_sequence.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MathLabTests
{
  TEST_CLASS(block_sequence_tests)
  {
  public:
    TEST_METHOD(dump_empty_sequence)
    {
      mathlab::factory factory;
      mathlab::block_sequence sequence(factory);
      std::ostringstream output;
      sequence.dump(output, false);
      Assert::AreEqual(std::string(""), output.str());
    }

    TEST_METHOD(dump_sequence_with_line_numbers)
    {
      mathlab::factory factory;
      factory.register_block<mathlab::addition>("addition");
      mathlab::block_sequence sequence(factory);
      std::istringstream input("addition 100\n");
      sequence.append_from(input);

      std::ostringstream output;
      sequence.dump(output, true);
      Assert::AreEqual(std::string("1: addition 100 \n"), output.str());
    }

    TEST_METHOD(append_multiple_and_eval)
    {
      mathlab::factory factory;
      factory.register_block<mathlab::addition>("add");
      factory.register_block<mathlab::multiplication>("multiply");
      mathlab::block_sequence sequence(factory);
      std::istringstream input("add 100.\nmultiply 2.\n");
      auto invalid_lines = sequence.append_from(input);
      Assert::IsTrue(invalid_lines.empty());
      Assert::AreEqual(400., sequence.eval(100.));
    }

    TEST_METHOD(append_skips_invalid_input_and_returns_invalid_lines)
    {
      mathlab::factory factory;
      factory.register_block<mathlab::addition>("addition");
      factory.register_block<mathlab::multiplication>("multiplication");
      mathlab::block_sequence sequence(factory);
      std::istringstream input("dummy\naddition 100.\ndummy\nmultiplication 2.\ndummy\n");
      const auto invalid_lines = sequence.append_from(input);
      Assert::AreEqual(std::string("dummy\ndummy\ndummy\n"), invalid_lines);
      Assert::AreEqual(400., sequence.eval(100.));
    }

    TEST_METHOD(load_clears_sequence_and_appends)
    {
      mathlab::factory factory;
      factory.register_block<mathlab::addition>("addition");
      factory.register_block<mathlab::multiplication>("multiplication");
      mathlab::block_sequence sequence(factory);
      std::istringstream input1("addition 100.\n");
      sequence.append_from(input1);
      Assert::AreEqual(103., sequence.eval(3.));
      std::istringstream input2("multiplication 2.\n");
      sequence.load_from(input2);
      Assert::AreEqual(6., sequence.eval(3.));
    }

    TEST_METHOD(remove_from_sequence)
    {
      mathlab::factory factory;
      factory.register_block<mathlab::addition>("addition");
      factory.register_block<mathlab::multiplication>("multiplication");
      mathlab::block_sequence sequence(factory);
      std::istringstream input("addition 100.\nmultiplication 2.\n");
      sequence.append_from(input);
      sequence.remove_at(0);

      std::ostringstream output;
      sequence.dump(output, false);
      Assert::AreEqual(std::string("multiplication 2 \n"), output.str());
    }

    TEST_METHOD(move_to_beginning_works)
    {
      mathlab::factory factory;
      factory.register_block<mathlab::addition>("addition");
      factory.register_block<mathlab::multiplication>("multiplication");
      mathlab::block_sequence sequence(factory);
      std::istringstream input("addition 100.\nmultiplication 2.\n");
      sequence.append_from(input);
      sequence.move_to_beginning(1);

      std::ostringstream output;
      sequence.dump(output, false);
      // ReSharper restore CppExpressionWithoutSideEffects
      Assert::AreEqual(std::string("multiplication 2 \naddition 100 \n"), output.str());
    }
  };
}
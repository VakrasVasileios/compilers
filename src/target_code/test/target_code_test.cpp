#include <gtest/gtest.h>
#include "../../../util/exec/include/exec.h"

/**
 * @brief Test suite for the intermediate code
 * production.
 * 
**/
class BinaryParserSuite : public ::testing::Test
{
protected:
    std::string expected;
    std::string actual;
};

/* ------------ assign -------------- */

TEST_F(BinaryParserSuite, assign_simple)
{
    expected = "1: assign 0 0 [line 1]\n"
               "2: assign 1 0 [line 1]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/assign/simple.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(BinaryParserSuite, assign_calls_nested)
{
    expected = "1: jump 4 [line 1]\n"
               "2: enterfunc 0 [line 1]\n"
               "3: exitfunc 0 [line 2]\n"
               "4: jump 7 [line 4]\n"
               "5: enterfunc 1 [line 4]\n"
               "6: exitfunc 1 [line 5]\n"
               "7: pusharg 0 [line 7]\n"
               "8: callfunc 1 [line 7]\n"
               "9: assign 3 69420 [line 7]\n"
               "10: pusharg 3 [line 7]\n"
               "11: callfunc 0 [line 7]\n"
               "12: assign 4 69420 [line 7]\n"
               "13: assign 2 4 [line 7]\n"
               "14: assign 5 2 [line 7]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/assign/calls_nested.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(BinaryParserSuite, assign_string)
{
    expected = "1: assign 0 0 [line 1]\n"
               "2: assign 1 0 [line 1]\n"
               "3: assign 0 1 [line 2]\n"
               "4: assign 1 0 [line 2]\n"
               "5: assign 0 2 [line 3]\n"
               "6: assign 1 0 [line 3]\n"
               "7: assign 0 3 [line 4]\n"
               "8: assign 1 0 [line 4]\n"
               "9: assign 0 4 [line 5]\n"
               "10: assign 1 0 [line 5]\n"
               "11: assign 0 5 [line 6]\n"
               "12: assign 1 0 [line 6]\n"
               "13: assign 0 6 [line 7]\n"
               "14: assign 1 0 [line 7]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/assign/string.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

/* ------------ Function -------------- */

TEST_F(BinaryParserSuite, functions_single)
{
    expected = "1: jump 4 [line 1]\n"
               "2: enterfunc 0 [line 1]\n"
               "3: exitfunc 0 [line 2]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/functions/single.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(BinaryParserSuite, functions_many)
{
    expected = "1: jump 4 [line 1]\n"
               "2: enterfunc 0 [line 1]\n"
               "3: exitfunc 0 [line 2]\n"
               "4: jump 7 [line 4]\n"
               "5: enterfunc 1 [line 4]\n"
               "6: exitfunc 1 [line 5]\n"
               "7: jump 10 [line 8]\n"
               "8: enterfunc 2 [line 8]\n"
               "9: exitfunc 2 [line 9]\n"
               "10: jump 13 [line 17]\n"
               "11: enterfunc 3 [line 17]\n"
               "12: exitfunc 3 [line 18]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/functions/many.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(BinaryParserSuite, functions_nested)
{
    expected = "1: jump 16 [line 5]\n"
               "2: enterfunc 0 [line 5]\n"
               "3: jump 12 [line 6]\n"
               "4: enterfunc 1 [line 6]\n"
               "5: jump 11 [line 7]\n"
               "6: enterfunc 2 [line 7]\n"
               "7: jump 10 [line 8]\n"
               "8: enterfunc 3 [line 8]\n"
               "9: exitfunc 3 [line 9]\n"
               "10: exitfunc 2 [line 10]\n"
               "11: exitfunc 1 [line 11]\n"
               "12: jump 15 [line 12]\n"
               "13: enterfunc 4 [line 12]\n"
               "14: exitfunc 4 [line 13]\n"
               "15: exitfunc 0 [line 14]\n"
               "16: jump 19 [line 16]\n"
               "17: enterfunc 5 [line 16]\n"
               "18: exitfunc 5 [line 27]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/functions/nested.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(BinaryParserSuite, functions_shadowed)
{
    expected = "1: jump 7 [line 1]\n"
               "2: enterfunc 0 [line 1]\n"
               "3: jump 6 [line 2]\n"
               "4: enterfunc 1 [line 2]\n"
               "5: exitfunc 1 [line 3]\n"
               "6: exitfunc 0 [line 5]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/functions/shadowed.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(BinaryParserSuite, functions_anonymous)
{
    expected = "1: jump 10 [line 1]\n"
               "2: enterfunc 0 [line 1]\n"
               "3: jump 6 [line 2]\n"
               "4: enterfunc 1 [line 2]\n"
               "5: exitfunc 1 [line 3]\n"
               "6: jump 9 [line 4]\n"
               "7: enterfunc 2 [line 4]\n"
               "8: exitfunc 2 [line 5]\n"
               "9: exitfunc 0 [line 6]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/functions/anonymous.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(BinaryParserSuite, functions_callfunc_warning)
{
    expected = "1: callfunc 0 [line 1]\n"
               "2: assign 1 69420 [line 1]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/functions/call_warning.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(BinaryParserSuite, functions_callfunc_noparameters)
{
    expected = "1: jump 4 [line 1]\n"
               "2: enterfunc 0 [line 1]\n"
               "3: exitfunc 0 [line 2]\n"
               "4: callfunc 0 [line 4]\n"
               "5: assign 1 69420 [line 4]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/functions/call_no_parameters.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(BinaryParserSuite, functions_callfunc_parameters)
{
    expected = "1: jump 4 [line 1]\n"
               "2: enterfunc 0 [line 1]\n"
               "3: exitfunc 0 [line 2]\n"
               "4: pusharg 0 [line 4]\n"
               "5: callfunc 0 [line 4]\n"
               "6: assign 1 69420 [line 4]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/functions/call_parameters.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(BinaryParserSuite, functions_callfunc_with_symbol_parameters)
{
    expected =  "\x1B[33mWarning, in line: 10:\x1B[0m Too many arguments passed to function: f, defined in line: 1\n"
                "\x1B[33mWarning, in line: 11:\x1B[0m Too many arguments passed to function: f, defined in line: 1\n"
                "\x1B[33mWarning, in line: 12:\x1B[0m Too many arguments passed to function: f, defined in line: 1\n"
                "1: jump 4 [line 1]\n"
                "2: enterfunc 0 [line 1]\n"
                "3: exitfunc 0 [line 2]\n"
                "4: pusharg 1 [line 5]\n"
                "5: callfunc 0 [line 5]\n"
                "6: assign 2 69420 [line 5]\n"
                "7: pusharg 0 [line 10]\n"
                "8: pusharg 1 [line 10]\n"
                "9: callfunc 0 [line 10]\n"
                "10: assign 2 69420 [line 10]\n"
                "11: pusharg 1 [line 11]\n"
                "12: pusharg 2 [line 11]\n"
                "13: pusharg 1 [line 11]\n"
                "14: callfunc 0 [line 11]\n"
                "15: assign 2 69420 [line 11]\n"
                "16: pusharg 4 [line 12]\n"
                "17: pusharg 4 [line 12]\n"
                "18: pusharg 3 [line 12]\n"
                "19: pusharg 0 [line 12]\n"
                "20: pusharg 1 [line 12]\n"
                "21: callfunc 0 [line 12]\n"
                "22: assign 2 69420 [line 12]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/functions/call_with_symbol_parameters.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(BinaryParserSuite, functions_callfunc_less_parameters)
{
    expected = "\x1B[31mError, in line: 4:\x1B[0m Too few arguments passed to function: f, defined in line: 1\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/functions/call_less_parameters.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(BinaryParserSuite, functions_call_nested)
{
    expected = "\x1B[33mWarning, in line: 10:\x1B[0m Too many arguments passed to function: g, defined in line: 6\n"
               "\x1B[33mWarning, in line: 12:\x1B[0m Too many arguments passed to function: g, defined in line: 6\n"
               "1: jump 4 [line 2]\n"
                "2: enterfunc 0 [line 2]\n"
                "3: exitfunc 0 [line 4]\n"
                "4: jump 7 [line 6]\n"
                "5: enterfunc 1 [line 6]\n"
                "6: exitfunc 1 [line 8]\n"
                "7: pusharg 0 [line 10]\n"
                "8: callfunc 1 [line 10]\n"
                "9: assign 2 69420 [line 10]\n"
                "10: pusharg 2 [line 10]\n"
                "11: callfunc 0 [line 10]\n"
                "12: assign 3 69420 [line 10]\n"
                "13: pusharg 1 [line 12]\n"
                "14: callfunc 4 [line 12]\n"
                "15: assign 2 69420 [line 12]\n"
                "16: pusharg 2 [line 12]\n"
                "17: callfunc 1 [line 12]\n"
                "18: assign 3 69420 [line 12]\n"
                "19: pusharg 3 [line 12]\n"
                "20: callfunc 0 [line 12]\n"
                "21: assign 5 69420 [line 12]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/functions/call_nested.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(BinaryParserSuite, functions_call_more_parameters)
{
    expected = "\x1B[33mWarning, in line: 5:\x1B[0m Too many arguments passed to function: f, defined in line: 1\n"
               "1: jump 4 [line 1]\n"
               "2: enterfunc 0 [line 1]\n"
               "3: exitfunc 0 [line 3]\n"
               "4: pusharg 0 [line 5]\n"
               "5: pusharg 1 [line 5]\n"
               "6: callfunc 0 [line 5]\n"
               "7: assign 2 69420 [line 5]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/functions/call_more_parameters.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(BinaryParserSuite, functions_call_function_def)
{
    expected = "1: jump 4 [line 1]\n"
               "2: enterfunc 0 [line 1]\n"
               "3: exitfunc 0 [line 1]\n"
               "4: pusharg 0 [line 1]\n"
               "5: callfunc 0 [line 1]\n"
               "6: assign 1 69420 [line 1]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/functions/call_function_def.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(BinaryParserSuite, functions_call_nested_with_func_def)
{
    expected = "1: jump 4 [line 1]\n"
               "2: enterfunc 0 [line 1]\n"
               "3: exitfunc 0 [line 1]\n"
               "4: pusharg 2 [line 1]\n"
               "5: callfunc 0 [line 1]\n"
               "6: assign 3 69420 [line 1]\n"
               "7: pusharg 3 [line 1]\n"
               "8: callfunc 0 [line 1]\n"
               "9: assign 4 69420 [line 1]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/functions/call_nested_with_func_def.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(BinaryParserSuite, functions_call_anonymous_func_def)
{
    expected = "1: jump 4 [line 1]\n"
               "2: enterfunc 0 [line 1]\n"
               "3: exitfunc 0 [line 1]\n"
               "4: pusharg 0 [line 1]\n"
               "5: pusharg 1 [line 1]\n"
               "6: callfunc 0 [line 1]\n"
               "7: assign 1 69420 [line 1]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/functions/call_anonymous_func_def.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(BinaryParserSuite, functions_call_method_call) {
    expected =  "1: tablegetelem 2 1 0 [line 1]\n"
                "2: pusharg 0 [line 1]\n"
                "3: callfunc 2 [line 1]\n"
                "4: assign 3 69420 [line 1]\n"
                "5: tablegetelem 2 5 0 [line 3]\n"
                "6: pusharg 4 [line 3]\n"
                "7: pusharg 0 [line 3]\n"
                "8: pusharg 0 [line 3]\n"
                "9: callfunc 2 [line 3]\n"
                "10: assign 3 69420 [line 3]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/functions/call_method_call.asc");
    GTEST_ASSERT_EQ(expected, actual);            
}

/* ------------ return -------------- */

TEST_F(BinaryParserSuite, return_void)
{
    expected = "1: jump 6 [line 1]\n"
               "2: enterfunc 0 [line 1]\n"
               "3: assign 69420 [line 2]\n"
               "4: jump 5 [line 2]\n"
               "5: exitfunc 0 [line 3]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/return/return_void.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(BinaryParserSuite, return_symbol)
{
    expected = "1: jump 6 [line 5]\n"
               "2: enterfunc 0 [line 5]\n"
               "3: assign 69420 0 [line 6]\n"
               "4: jump 5 [line 6]\n"
               "5: exitfunc 0 [line 7]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/return/return_symbol.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(BinaryParserSuite, return_many_and_nested)
{
    expected = "1: jump 17 [line 1]\n"
               "2: enterfunc 0 [line 1]\n"
               "3: assign 69420 [line 2]\n"
               "4: jump 16 [line 2]\n"
               "5: assign 69420 0 [line 4]\n"
               "6: jump 16 [line 4]\n"
               "7: jump 12 [line 6]\n"
               "8: enterfunc 1 [line 6]\n"
               "9: assign 69420 [line 6]\n"
               "10: jump 11 [line 6]\n"
               "11: exitfunc 1 [line 6]\n"
               "12: callfunc 1 [line 6]\n"
               "13: assign 2 69420 [line 6]\n"
               "14: assign 69420 2 [line 6]\n"
               "15: jump 16 [line 6]\n"
               "16: exitfunc 0 [line 7]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/return/return_many_and_nested.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

/* ------------ Logical ----------------- */

TEST_F(BinaryParserSuite, logical_greater_simple)
{
    expected = "1: jgt 3 0 1 [line 1]\n"
               "2: jump 5 [line 1]\n"
               "3: assign 0 1 [line 1]\n"
               "4: jump 6 [line 1]\n"
               "5: assign 0 0 [line 1]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/logical/greater_simple.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(BinaryParserSuite, logical_equal_simple)
{
    expected = "1: jeq 3 0 1 [line 1]\n"
               "2: jump 5 [line 1]\n"
               "3: assign 0 1 [line 1]\n"
               "4: jump 6 [line 1]\n"
               "5: assign 0 0 [line 1]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/logical/equal_simple.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(BinaryParserSuite, logical_greater_equal_simple)
{
    expected = "1: jge 3 0 1 [line 1]\n"
               "2: jump 5 [line 1]\n"
               "3: assign 0 1 [line 1]\n"
               "4: jump 6 [line 1]\n"
               "5: assign 0 0 [line 1]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/logical/greater_equal_simple.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(BinaryParserSuite, logical_less_equal_simple)
{
    expected = "1: jle 3 0 1 [line 1]\n"
               "2: jump 5 [line 1]\n"
               "3: assign 0 1 [line 1]\n"
               "4: jump 6 [line 1]\n"
               "5: assign 0 0 [line 1]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/logical/less_equal_simple.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(BinaryParserSuite, logical_less_simple)
{
    expected = "1: jlt 3 0 1 [line 1]\n"
               "2: jump 5 [line 1]\n"
               "3: assign 0 1 [line 1]\n"
               "4: jump 6 [line 1]\n"
               "5: assign 0 0 [line 1]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/logical/less_simple.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(BinaryParserSuite, logical_not_equal_simple)
{
    expected = "1: jne 3 0 1 [line 1]\n"
               "2: jump 5 [line 1]\n"
               "3: assign 0 1 [line 1]\n"
               "4: jump 6 [line 1]\n"
               "5: assign 0 0 [line 1]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/logical/not_equal_simple.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(BinaryParserSuite, logical_errors)
{
    expected = "\x1B[31mError, in line: 1:\x1B[0m Invalid use of comparison operator on const string \"string\"\n"
               "\x1B[31mError, in line: 3:\x1B[0m Invalid use of comparison operator on const bool \'true\'\n"
               "\x1B[31mError, in line: 5:\x1B[0m Invalid use of comparison operator on library function print\n"
               "\x1B[31mError, in line: 9:\x1B[0m Invalid use of comparison operator on user function s\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/logical/errors.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

/* ------------- Loop ------------------- */

TEST_F(BinaryParserSuite, loop_while_simple)
{
    expected = "1: jgt 3 0 1 [line 1]\n"
               "2: jump 5 [line 1]\n"
               "3: assign 0 1 [line 1]\n"
               "4: jump 6 [line 1]\n"
               "5: assign 0 0 [line 1]\n"
               "6: jeq 8 0 1 [line 1]\n"
               "7: jump 9 [line 1]\n"
               "8: jump 1 [line 1]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/loop/while_simple.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(BinaryParserSuite, loop_while_simple_with_stms)
{
    expected = "1: jeq 3 0 1 [line 1]\n"
               "2: jump 6 [line 1]\n"
               "3: assign 0 1 [line 2]\n"
               "4: assign 1 0 [line 2]\n"
               "5: jump 1 [line 5]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/loop/while_simple_with_stmts.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(BinaryParserSuite, loop_while_many_nested)
{
    expected = "1: jlt 3 0 0 [line 1]\n"
               "2: jump 5 [line 1]\n"
               "3: assign 1 1 [line 1]\n"
               "4: jump 6 [line 1]\n"
               "5: assign 1 0 [line 1]\n"
               "6: jeq 8 1 1 [line 1]\n"
               "7: jump 17 [line 1]\n"
               "8: assign 0 0 [line 2]\n"
               "9: assign 2 0 [line 2]\n"
               "10: jeq 12 1 1 [line 3]\n"
               "11: jump 16 [line 3]\n"
               "12: jeq 14 0 1 [line 3]\n"
               "13: jump 15 [line 3]\n"
               "14: jump 12 [line 3]\n"
               "15: jump 10 [line 3]\n"
               "16: jump 1 [line 4]\n"
               "17: jeq 19 3 1 [line 9]\n"
               "18: jump 20 [line 9]\n"
               "19: jump 17 [line 10]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/loop/while_many_nested.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(BinaryParserSuite, loop_while_continue_single)
{
    expected = "1: jeq 3 0 1 [line 1]\n"
               "2: jump 5 [line 1]\n"
               "3: jump 1 [line 2]\n"
               "4: jump 1 [line 3]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/loop/while_continue_single.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(BinaryParserSuite, loop_while_continue_many_nested)
{
    expected = "1: jeq 3 0 1 [line 1]\n"
               "2: jump 29 [line 1]\n"
               "3: jump 1 [line 2]\n"
               "4: jump 1 [line 3]\n"
               "5: jump 1 [line 4]\n"
               "6: jump 1 [line 5]\n"
               "7: jump 1 [line 5]\n"
               "8: jump 1 [line 6]\n"
               "9: jump 1 [line 7]\n"
               "10: jump 1 [line 7]\n"
               "11: jump 1 [line 7]\n"
               "12: jump 1 [line 7]\n"
               "13: jump 1 [line 7]\n"
               "14: jump 1 [line 7]\n"
               "15: jump 1 [line 7]\n"
               "16: jlt 18 1 2 [line 8]\n"
               "17: jump 20 [line 8]\n"
               "18: assign 0 1 [line 8]\n"
               "19: jump 21 [line 8]\n"
               "20: assign 0 0 [line 8]\n"
               "21: jeq 23 0 1 [line 8]\n"
               "22: jump 28 [line 8]\n"
               "23: jump 16 [line 9]\n"
               "24: jump 16 [line 10]\n"
               "25: jump 16 [line 10]\n"
               "26: jump 16 [line 10]\n"
               "27: jump 16 [line 15]\n"
               "28: jump 1 [line 20]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/loop/while_continue_many_nested.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(BinaryParserSuite, loop_while_break_single)
{
    expected = "1: jeq 3 0 1 [line 1]\n"
               "2: jump 5 [line 1]\n"
               "3: jump 5 [line 2]\n"
               "4: jump 1 [line 3]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/loop/while_break_single.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(BinaryParserSuite, loop_for_simple)
{
    expected = "1: assign 0 0 [line 1]\n"
               "2: assign 1 0 [line 1]\n"
               "3: jlt 5 0 1 [line 1]\n"
               "4: jump 7 [line 1]\n"
               "5: assign 2 1 [line 1]\n"
               "6: jump 8 [line 1]\n"
               "7: assign 2 0 [line 1]\n"
               "8: jeq 13 2 1 [line 1]\n"
               "9: jump 14 [line 1]\n"
               "10: assign 3 0 [line 1]\n"
               "11: add 0 0 2 [line 1]\n"
               "12: jump 3 [line 1]\n"
               "13: jump 10 [line 2]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/loop/for_simple.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(BinaryParserSuite, loop_for_simple_with_stmts)
{
    expected = "1: assign 0 0 [line 1]\n"
               "2: assign 1 0 [line 1]\n"
               "3: jlt 5 0 1 [line 1]\n"
               "4: jump 7 [line 1]\n"
               "5: assign 2 1 [line 1]\n"
               "6: jump 8 [line 1]\n"
               "7: assign 2 0 [line 1]\n"
               "8: jeq 13 2 1 [line 1]\n"
               "9: jump 16 [line 1]\n"
               "10: assign 3 0 [line 1]\n"
               "11: add 0 0 2 [line 1]\n"
               "12: jump 3 [line 1]\n"
               "13: assign 4 3 [line 2]\n"
               "14: assign 5 4 [line 2]\n"
               "15: jump 10 [line 5]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/loop/for_simple_with_stmts.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(BinaryParserSuite, loop_for_many_nested)
{
    expected =  "1: assign 0 0 [line 1]\n"
                "2: assign 1 0 [line 1]\n"
                "3: jlt 5 0 1 [line 1]\n"
                "4: jump 7 [line 1]\n"
                "5: assign 2 1 [line 1]\n"
                "6: jump 8 [line 1]\n"
                "7: assign 2 0 [line 1]\n"
                "8: jeq 13 2 1 [line 1]\n"
                "9: jump 40 [line 1]\n"
                "10: assign 3 0 [line 1]\n"
                "11: add 0 0 2 [line 1]\n"
                "12: jump 3 [line 1]\n"
                "13: assign 0 0 [line 2]\n"
                "14: assign 4 0 [line 2]\n"
                "15: jlt 17 0 1 [line 2]\n"
                "16: jump 19 [line 2]\n"
                "17: assign 5 1 [line 2]\n"
                "18: jump 20 [line 2]\n"
                "19: assign 5 0 [line 2]\n"
                "20: jeq 25 5 1 [line 2]\n"
                "21: jump 39 [line 2]\n"
                "22: assign 6 0 [line 2]\n"
                "23: add 0 0 2 [line 2]\n"
                "24: jump 15 [line 2]\n"
                "25: assign 0 0 [line 2]\n"
                "26: assign 7 0 [line 2]\n"
                "27: jlt 29 0 1 [line 2]\n"
                "28: jump 31 [line 2]\n"
                "29: assign 8 1 [line 2]\n"
                "30: jump 32 [line 2]\n"
                "31: assign 8 0 [line 2]\n"
                "32: jeq 37 8 1 [line 2]\n"
                "33: jump 38 [line 2]\n"
                "34: assign 9 0 [line 2]\n"
                "35: add 0 0 2 [line 2]\n"
                "36: jump 27 [line 2]\n"
                "37: jump 34 [line 2]\n"
                "38: jump 22 [line 2]\n"
                "39: jump 10 [line 3]\n"
                "40: assign 0 0 [line 6]\n"
                "41: assign 10 0 [line 6]\n"
                "42: jlt 44 0 1 [line 6]\n"
                "43: jump 46 [line 6]\n"
                "44: assign 11 1 [line 6]\n"
                "45: jump 47 [line 6]\n"
                "46: assign 11 0 [line 6]\n"
                "47: jeq 52 11 1 [line 6]\n"
                "48: jump 53 [line 6]\n"
                "49: assign 12 0 [line 6]\n"
                "50: add 0 0 2 [line 6]\n"
                "51: jump 42 [line 6]\n"
                "52: jump 49 [line 6]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/loop/for_many_nested.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(BinaryParserSuite, loop_for_continue_single)
{
    expected = "1: jlt 3 0 0 [line 1]\n"
               "2: jump 5 [line 1]\n"
               "3: assign 1 1 [line 1]\n"
               "4: jump 6 [line 1]\n"
               "5: assign 1 0 [line 1]\n"
               "6: jeq 11 1 1 [line 1]\n"
               "7: jump 13 [line 1]\n"
               "8: assign 2 0 [line 1]\n"
               "9: add 0 0 1 [line 1]\n"
               "10: jump 1 [line 1]\n"
               "11: jump 8 [line 2]\n"
               "12: jump 8 [line 3]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/loop/for_continue_single.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(BinaryParserSuite, loop_for_continue_many_nested)
{
    expected =  "1: jlt 3 0 0 [line 1]\n"
                "2: jump 5 [line 1]\n"
                "3: assign 1 1 [line 1]\n"
                "4: jump 6 [line 1]\n"
                "5: assign 1 0 [line 1]\n"
                "6: jeq 11 1 1 [line 1]\n"
                "7: jump 34 [line 1]\n"
                "8: assign 2 0 [line 1]\n"
                "9: add 0 0 1 [line 1]\n"
                "10: jump 1 [line 1]\n"
                "11: jump 8 [line 2]\n"
                "12: jump 8 [line 2]\n"
                "13: jump 8 [line 3]\n"
                "14: jump 8 [line 3]\n"
                "15: jump 8 [line 3]\n"
                "16: jump 8 [line 3]\n"
                "17: jump 8 [line 3]\n"
                "18: jump 8 [line 3]\n"
                "19: jlt 21 0 0 [line 4]\n"
                "20: jump 23 [line 4]\n"
                "21: assign 3 1 [line 4]\n"
                "22: jump 24 [line 4]\n"
                "23: assign 3 0 [line 4]\n"
                "24: jeq 29 3 1 [line 4]\n"
                "25: jump 33 [line 4]\n"
                "26: assign 4 0 [line 4]\n"
                "27: add 0 0 1 [line 4]\n"
                "28: jump 19 [line 4]\n"
                "29: jump 26 [line 4]\n"
                "30: jump 26 [line 4]\n"
                "31: jump 26 [line 4]\n"
                "32: jump 26 [line 4]\n"
                "33: jump 8 [line 5]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/loop/for_continue_many_nested.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(BinaryParserSuite, loop_for_break_single)
{
    expected = "1: jlt 3 0 0 [line 1]\n"
               "2: jump 5 [line 1]\n"
               "3: assign 1 1 [line 1]\n"
               "4: jump 6 [line 1]\n"
               "5: assign 1 0 [line 1]\n"
               "6: jeq 11 1 1 [line 1]\n"
               "7: jump 13 [line 1]\n"
               "8: assign 2 0 [line 1]\n"
               "9: add 0 0 1 [line 1]\n"
               "10: jump 1 [line 1]\n"
               "11: jump 13 [line 2]\n"
               "12: jump 8 [line 3]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/loop/for_break_single.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(BinaryParserSuite, loop_for_break_many_nested)
{
    expected =  "1: jlt 3 0 0 [line 1]\n"
                "2: jump 5 [line 1]\n"
                "3: assign 1 1 [line 1]\n"
                "4: jump 6 [line 1]\n"
                "5: assign 1 0 [line 1]\n"
                "6: jeq 11 1 1 [line 1]\n"
                "7: jump 33 [line 1]\n"
                "8: assign 2 0 [line 1]\n"
                "9: add 0 0 1 [line 1]\n"
                "10: jump 1 [line 1]\n"
                "11: jump 33 [line 2]\n"
                "12: jump 33 [line 3]\n"
                "13: jump 33 [line 3]\n"
                "14: jump 33 [line 3]\n"
                "15: jump 33 [line 3]\n"
                "16: jump 33 [line 3]\n"
                "17: jump 33 [line 3]\n"
                "18: jump 33 [line 3]\n"
                "19: jgt 21 3 2 [line 4]\n"
                "20: jump 23 [line 4]\n"
                "21: assign 4 1 [line 4]\n"
                "22: jump 24 [line 4]\n"
                "23: assign 4 0 [line 4]\n"
                "24: jeq 29 4 1 [line 4]\n"
                "25: jump 32 [line 4]\n"
                "26: assign 5 3 [line 4]\n"
                "27: add 3 3 1 [line 4]\n"
                "28: jump 19 [line 4]\n"
                "29: jump 32 [line 4]\n"
                "30: jump 32 [line 4]\n"
                "31: jump 26 [line 4]\n"
                "32: jump 8 [line 5]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/loop/for_break_many_nested.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

/* ------------- Conditional ------------- */

TEST_F(BinaryParserSuite, if_simple)
{
    expected = "1: jeq 3 0 1 [line 1]\n"
               "2: jump 3 [line 1]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/conditional/if_simple.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(BinaryParserSuite, if_expr)
{
    expected = "1: jgt 3 0 0 [line 1]\n"
               "2: jump 5 [line 1]\n"
               "3: assign 1 1 [line 1]\n"
               "4: jump 6 [line 1]\n"
               "5: assign 1 0 [line 1]\n"
               "6: jeq 8 1 1 [line 1]\n"
               "7: jump 8 [line 1]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/conditional/if_expr.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(BinaryParserSuite, if_stmts)
{
    expected = "1: jlt 3 0 0 [line 1]\n"
               "2: jump 5 [line 1]\n"
               "3: assign 1 1 [line 1]\n"
               "4: jump 6 [line 1]\n"
               "5: assign 1 0 [line 1]\n"
               "6: jeq 8 1 1 [line 1]\n"
               "7: jump 10 [line 1]\n"
               "8: assign 2 3 [line 5]\n"
               "9: assign 4 2 [line 5]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/conditional/if_stmts.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(BinaryParserSuite, if_nested_many)
{
    expected = "1: jeq 3 0 1 [line 1]\n"
               "2: jump 11 [line 1]\n"
               "3: jeq 5 1 1 [line 2]\n"
               "4: jump 11 [line 2]\n"
               "5: jeq 7 2 1 [line 3]\n"
               "6: jump 9 [line 3]\n"
               "7: assign 3 0 [line 4]\n"
               "8: assign 4 3 [line 4]\n"
               "9: assign 5 1 [line 7]\n"
               "10: assign 6 5 [line 7]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/conditional/if_nested_many.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(BinaryParserSuite, if_else)
{
    expected = "1: jeq 3 0 1 [line 1]\n"
               "2: jump 4 [line 1]\n"
               "3: jump 8 [line 2]\n"
               "4: assign 1 0 [line 2]\n"
               "5: assign 2 1 [line 2]\n"
               "6: assign 0 3 [line 2]\n"
               "7: assign 2 0 [line 2]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/conditional/if_else.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(BinaryParserSuite, if_elseif)
{
    expected = "1: jeq 3 0 1 [line 1]\n"
               "2: jump 6 [line 1]\n"
               "3: assign 0 1 [line 1]\n"
               "4: assign 1 0 [line 1]\n"
               "5: jump 10 [line 2]\n"
               "6: jeq 8 0 1 [line 2]\n"
               "7: jump 10 [line 2]\n"
               "8: assign 2 2 [line 2]\n"
               "9: assign 3 2 [line 2]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/conditional/if_elseif.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(BinaryParserSuite, if_elseif_else)
{
    expected = "1: jeq 3 0 1 [line 1]\n"
               "2: jump 6 [line 1]\n"
               "3: assign 0 1 [line 1]\n"
               "4: assign 1 0 [line 1]\n"
               "5: jump 28 [line 2]\n"
               "6: jeq 8 0 1 [line 2]\n"
               "7: jump 11 [line 2]\n"
               "8: assign 2 2 [line 2]\n"
               "9: assign 3 2 [line 2]\n"
               "10: jump 28 [line 3]\n"
               "11: jeq 13 0 1 [line 3]\n"
               "12: jump 16 [line 3]\n"
               "13: assign 4 2 [line 3]\n"
               "14: assign 5 4 [line 3]\n"
               "15: jump 28 [line 4]\n"
               "16: jeq 18 0 1 [line 4]\n"
               "17: jump 21 [line 4]\n"
               "18: assign 6 2 [line 4]\n"
               "19: assign 7 6 [line 4]\n"
               "20: jump 28 [line 5]\n"
               "21: jeq 23 0 1 [line 5]\n"
               "22: jump 26 [line 5]\n"
               "23: assign 8 2 [line 5]\n"
               "24: assign 9 8 [line 5]\n"
               "25: jump 28 [line 6]\n"
               "26: assign 10 2 [line 6]\n"
               "27: assign 11 10 [line 6]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/conditional/if_elseif_else.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(BinaryParserSuite, if_else_many_nested)
{
    expected =  "1: jeq 3 0 1 [line 1]\n"
                "2: jump 16 [line 1]\n"
                "3: jeq 5 1 1 [line 1]\n"
                "4: jump 13 [line 1]\n"
                "5: jeq 7 0 2 [line 1]\n"
                "6: jump 9 [line 1]\n"
                "7: assign 1 1 [line 1]\n"
                "8: jump 10 [line 1]\n"
                "9: assign 1 0 [line 1]\n"
                "10: jeq 12 1 1 [line 1]\n"
                "11: jump 13 [line 1]\n"
                "12: jump 13 [line 1]\n"
                "13: assign 2 1 [line 1]\n"
                "14: assign 3 2 [line 1]\n"
                "15: jump 27 [line 2]\n"
                "16: jgt 18 4 2 [line 3]\n"
                "17: jump 20 [line 3]\n"
                "18: assign 5 1 [line 3]\n"
                "19: jump 21 [line 3]\n"
                "20: assign 5 0 [line 3]\n"
                "21: jeq 23 5 1 [line 3]\n"
                "22: jump 24 [line 3]\n"
                "23: jump 27 [line 4]\n"
                "24: jeq 26 4 1 [line 4]\n"
                "25: jump 27 [line 4]\n"
                "26: jump 27 [line 5]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/conditional/if_else_many_nested.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

/* ------------ Arithmetic -------------- */

TEST_F(BinaryParserSuite, arithmetic_simple)
{
    expected = "1: add 1 0 0 [line 1]\n"
               "2: sub 1 2 1 [line 2]\n"
               "3: mul 1 2 3 [line 3]\n"
               "4: mod 1 3 4 [line 4]\n"
               "5: div 1 5 5 [line 5]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/arithmetic/simple.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(BinaryParserSuite, arithmetic_spaghetti)
{
    expected = "1: add 1 0 0 [line 1]\n"
               "2: add 2 1 1 [line 1]\n"
               "3: add 3 2 0 [line 1]\n"
               "4: add 5 3 4 [line 1]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/arithmetic/spaghetti.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(BinaryParserSuite, arithmetic_errors)
{
    expected = "\x1B[31mError, in line: 4:\x1B[0m Invalid use of arithmetic operator on user function f\n"
               "\x1B[31mError, in line: 6:\x1B[0m Invalid use of arithmetic operator on const bool \'true\'\n"
               "\x1B[31mError, in line: 6:\x1B[0m Invalid use of arithmetic operator on const bool \'false\'\n"
               "\x1B[31mError, in line: 8:\x1B[0m Invalid use of arithmetic operator on const string \"a\"\n"
               "\x1B[31mError, in line: 10:\x1B[0m Invalid use of arithmetic operator on library function print\n";

    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/arithmetic/errors.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(BinaryParserSuite, arithmetic_plusplus_suffix)
{
    expected = "1: assign 1 0 [line 1]\n"
               "2: add 0 0 0 [line 1]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/arithmetic/plusplus_suffix.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(BinaryParserSuite, arithmetic_plusplus_prefix)
{
    expected = "1: add 0 0 0 [line 1]\n"
               "2: assign 1 0 [line 1]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/arithmetic/plusplus_prefix.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(BinaryParserSuite, arithmetic_uminus)
{
    expected = "1: mul 0 0 1 [line 1]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/arithmetic/uminus.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(BinaryParserSuite, arithmetic_uminus_bool) {
    expected = "\x1B[31mError, in line: 1:\x1B[0m Invalid use of arithmetic operator on const bool 'true'\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/arithmetic/uminus_bool.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(BinaryParserSuite, arithmetic_not)
{
    expected = "1: jeq 5 0 1 [line 1]\n"
               "2: jump 3 [line 1]\n"
               "3: assign 0 1 [line 1]\n"
               "4: jump 6 [line 1]\n"
               "5: assign 0 0 [line 1]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/arithmetic/not.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(BinaryParserSuite, arithmetic_minusminus_prefix)
{
    expected = "1: sub 0 0 0 [line 1]\n"
               "2: assign 1 0 [line 1]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/arithmetic/minusminus_prefix.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(BinaryParserSuite, arithmetic_minusminus_suffix)
{
    expected = "1: assign 1 0 [line 1]\n"
               "2: sub 0 0 0 [line 1]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/arithmetic/minusminus_suffix.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

/* ------------ Table -------------- */

TEST_F(BinaryParserSuite, table_tablemake_list_single)
{
    expected = "1: newtable 0 [line 1]\n"
               "2: tablesetelem 0 0 1 [line 1]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/table/tablemake_list_single.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(BinaryParserSuite, table_tablemake_list_many)
{
    expected = "1: callfunc 1 [line 1]\n"
               "2: assign 2 69420 [line 1]\n"
               "3: newtable 3 [line 1]\n"
               "4: tablesetelem 3 0 1 [line 1]\n"
               "5: tablesetelem 3 1 2 [line 1]\n"
               "6: tablesetelem 3 3 0 [line 1]\n"
               "7: tablesetelem 3 2 0 [line 1]\n"
               "8: tablesetelem 3 4 2 [line 1]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/table/tablemake_list_many.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(BinaryParserSuite, table_tablemake_pairs_single)
{
    expected = "1: newtable 1 [line 1]\n"
               "2: tablesetelem 1 0 0 [line 1]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/table/tablemake_pairs_single.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(BinaryParserSuite, table_tablemake_pairs_many)
{
    expected = "1: newtable 1 [line 1]\n"
               "2: tablesetelem 1 0 0 [line 1]\n"
               "3: tablesetelem 1 0 0 [line 1]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/table/tablemake_pairs_many.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(BinaryParserSuite, table_tablemake_pairs_macaroni)
{
    expected = "1: assign 1 0 [line 1]\n"
               "2: assign 2 1 [line 1]\n"
               "3: newtable 3 [line 1]\n"
               "4: tablesetelem 3 0 2 [line 1]\n"
               "5: tablesetelem 3 1 1 [line 1]\n"
               "6: tablesetelem 3 2 1 [line 1]\n"
               "7: tablesetelem 3 3 1 [line 1]\n"
               "8: assign 0 3 [line 1]\n"
               "9: assign 4 0 [line 1]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/table/tablemake_pairs_macaroni.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(BinaryParserSuite, table_tablemake_nested)
{
    expected = "1: newtable 0 [line 1]\n"
               "2: tablesetelem 0 0 1 [line 1]\n"
               "3: tablesetelem 0 1 2 [line 1]\n"
               "4: tablesetelem 0 2 3 [line 1]\n"
               "5: newtable 1 [line 1]\n"
               "6: tablesetelem 1 1 0 [line 1]\n"
               "7: newtable 2 [line 1]\n"
               "8: tablesetelem 2 0 1 [line 1]\n"
               "9: tablesetelem 2 1 0 [line 1]\n"
               "10: assign 3 2 [line 1]\n"
               "11: assign 4 3 [line 1]\n"
               "12: newtable 5 [line 1]\n"
               "13: tablesetelem 5 4 4 [line 1]\n"
               "14: newtable 6 [line 1]\n"
               "15: tablesetelem 6 0 0 [line 1]\n"
               "16: tablesetelem 6 1 5 [line 1]\n"
               "17: newtable 7 [line 1]\n"
               "18: tablesetelem 7 0 5 [line 1]\n"
               "19: newtable 8 [line 1]\n"
               "20: tablesetelem 8 0 0 [line 1]\n"
               "21: tablesetelem 8 1 2 [line 1]\n"
               "22: tablesetelem 8 2 6 [line 1]\n"
               "23: tablesetelem 8 3 7 [line 1]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/table/tablemake_nested.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(BinaryParserSuite, table_tablemember_item_simple) {
    expected = "1: tablegetelem 2 1 0 [line 1]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/table/tablemember_item_simple.asc");
    GTEST_ASSERT_EQ(expected, actual); 
}

TEST_F(BinaryParserSuite, table_tableelem_item_seq) {
    expected =  "1: tablegetelem 2 1 0 [line 1]\n"
                "2: tablegetelem 4 3 1 [line 1]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/table/tableelem_item_seq.asc");
    GTEST_ASSERT_EQ(expected, actual); 
}        

TEST_F(BinaryParserSuite, table_tableelem_ind_simp) {
    expected = "1: tablegetelem 2 1 0 [line 1]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/table/tableelem_ind_simp.asc");
    GTEST_ASSERT_EQ(expected, actual); 
}

TEST_F(BinaryParserSuite, table_tablelem_ind_manyexprs) {
    expected =  "1: tablegetelem 2 1 0 [line 1]\n"
                "2: tablegetelem 2 4 3 [line 2]\n"
                "3: assign 0 1 [line 3]\n"
                "4: assign 2 0 [line 3]\n"
                "5: tablegetelem 6 5 2 [line 3]\n"
                "6: pusharg 9 [line 4]\n"
                "7: callfunc 8 [line 4]\n"
                "8: assign 2 69420 [line 4]\n"
                "9: pusharg 2 [line 4]\n"
                "10: callfunc 7 [line 4]\n"
                "11: assign 6 69420 [line 4]\n"
                "12: mul 10 2 6 [line 4]\n"
                "13: add 11 3 10 [line 4]\n"
                "14: tablegetelem 13 12 11 [line 4]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/table/tableelem_ind_manyexprs.asc");
    GTEST_ASSERT_EQ(expected, actual);            
}

TEST_F(BinaryParserSuite, table_tableelem_it_call) {
    expected =  "1: callfunc 0 [line 1]\n"
                "2: assign 1 69420 [line 1]\n"
                "3: tablegetelem 3 2 0 [line 1]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/table/tableelem_it_call.asc");
    GTEST_ASSERT_EQ(expected, actual);            
}

TEST_F(BinaryParserSuite, table_tebleelem_ind_call) {
    expected =  "1: callfunc 0 [line 1]\n"
                "2: assign 1 69420 [line 1]\n"
                "3: tablegetelem 3 2 0 [line 1]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/table/tableelem_ind_call.asc");
    GTEST_ASSERT_EQ(expected, actual);              
}

TEST_F(BinaryParserSuite, table_emit_iftableitem_plusplusprefix) {
    expected =  "1: tablegetelem 3 1 0 [line 1]\n"
                "2: add 3 3 1 [line 1]\n"
                "3: tablesetelem 1 0 3 [line 1]\n";
    actual =  util::exec::exec("./d_target_code ../../test/files/phase3_tests/table/emit_iftableitem_plusplusprefix.asc");     
    GTEST_ASSERT_EQ(expected, actual);     
}

TEST_F(BinaryParserSuite, table_emit_iftableitem_plusplussufix) {
    expected =  "1: tablegetelem 3 1 0 [line 1]\n"
                "2: assign 2 3 [line 1]\n"
                "3: add 3 3 1 [line 1]\n"
                "4: tablesetelem 1 0 3 [line 1]\n";
    actual =  util::exec::exec("./d_target_code ../../test/files/phase3_tests/table/emit_iftableitem_plusplussufix.asc");     
    GTEST_ASSERT_EQ(expected, actual);            
}

TEST_F(BinaryParserSuite, table_emit_iftableitem_minusminus_prefix) {
    expected =  "1: tablegetelem 3 1 0 [line 1]\n"
                "2: sub 3 3 1 [line 1]\n"
                "3: tablesetelem 1 0 3 [line 1]\n";
    actual =  util::exec::exec("./d_target_code ../../test/files/phase3_tests/table/emit_iftableitem_minusminus_prefix.asc");     
    GTEST_ASSERT_EQ(expected, actual);            
}

TEST_F(BinaryParserSuite, table_emit_iftableitem_minusminus_suffix) {
    expected =  "1: tablegetelem 3 1 0 [line 1]\n"
                "2: assign 2 3 [line 1]\n"
                "3: sub 3 3 1 [line 1]\n"
                "4: tablesetelem 1 0 3 [line 1]\n";
    actual =  util::exec::exec("./d_target_code ../../test/files/phase3_tests/table/emit_iftableitem_minusminus_suffix.asc");     
    GTEST_ASSERT_EQ(expected, actual);            
}

TEST_F(BinaryParserSuite, table_emit_iftableitem_assignexpr) {
    expected =  "1: tablesetelem 1 0 1 [line 1]\n"
                "2: tablegetelem 2 1 0 [line 1]\n";
    actual =  util::exec::exec("./d_target_code ../../test/files/phase3_tests/table/emit_iftableitem_assignexpr.asc");     
    GTEST_ASSERT_EQ(expected, actual);            
}

TEST_F(BinaryParserSuite, table_emit_iftableitem_call) {
    expected =  "1: tablegetelem 3 1 0 [line 1]\n"
                "2: pusharg 2 [line 1]\n"
                "3: callfunc 3 [line 1]\n"
                "4: assign 4 69420 [line 1]\n";
    actual =  util::exec::exec("./d_target_code ../../test/files/phase3_tests/table/emit_iftableitem_call.asc");     
    GTEST_ASSERT_EQ(expected, actual);              
}

//----------------------- short circuit ---------------------------------------------------------------------------------

TEST_F(BinaryParserSuite, short_circuit_simple_or)
{
    expected = "1: jeq 9 0 1 [line 1]\n"
               "2: jump 3 [line 1]\n"
               "3: jeq 9 1 1 [line 1]\n"
               "4: jump 5 [line 1]\n"
               "5: jeq 7 2 1 [line 1]\n"
               "6: jump 11 [line 1]\n"
               "7: jeq 11 3 1 [line 1]\n"
               "8: jump 9 [line 1]\n"
               "9: assign 4 1 [line 1]\n"
               "10: jump 12 [line 1]\n"
               "11: assign 4 0 [line 1]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/short_circuit/simple_or.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

//-----------------------assistant tests---------------------------------------------------------------------------------

TEST_F(BinaryParserSuite, backpatch_backpatch0)
{
    expected = "1: jeq 3 1 1 [line 1]\n"
               "2: jump 7 [line 1]\n"
               "3: jeq 7 1 1 [line 1]\n"
               "4: jump 5 [line 1]\n"
               "5: assign 2 1 [line 1]\n"
               "6: jump 8 [line 1]\n"
               "7: assign 2 0 [line 1]\n"
               "8: assign 0 2 [line 1]\n"
               "9: assign 3 0 [line 1]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/backpatch/backpatch0.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(BinaryParserSuite, backpatch_backpatch1)
{
    expected = "1: jeq 7 0 1 [line 1]\n"
               "2: jump 3 [line 1]\n"
               "3: jeq 5 1 1 [line 1]\n"
               "4: jump 9 [line 1]\n"
               "5: jeq 7 2 1 [line 1]\n"
               "6: jump 9 [line 1]\n"
               "7: assign 3 1 [line 1]\n"
               "8: jump 10 [line 1]\n"
               "9: assign 3 0 [line 1]\n"
               "10: jeq 12 3 1 [line 1]\n"
               "11: jump 16 [line 1]\n"
               "12: pusharg 0 [line 2]\n"
               "13: callfunc 0 [line 2]\n"
               "14: assign 4 69420 [line 2]\n"
               "15: jump 1 [line 2]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/backpatch/backpatch1.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(BinaryParserSuite, backpatch_backpatch2)
{
    expected = "1: jeq 5 0 1 [line 1]\n"
               "2: jump 3 [line 1]\n"
               "3: jlt 5 1 0 [line 1]\n"
               "4: jump 7 [line 1]\n"
               "5: assign 2 1 [line 1]\n"
               "6: jump 8 [line 1]\n"
               "7: assign 2 0 [line 1]\n"
               "8: jeq 10 2 1 [line 1]\n"
               "9: jump 13 [line 1]\n"
               "10: pusharg 0 [line 2]\n"
               "11: callfunc 0 [line 2]\n"
               "12: assign 3 69420 [line 2]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/backpatch/backpatch2.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(BinaryParserSuite, backpatch_backpatch3)
{
    expected =  "\x1B[33mWarning, in line: 8:\x1B[0m Too many arguments passed to function: $0, defined in line: 8\n"
                "1: assign 5 0 [line 1]\n"
                "2: assign 6 5 [line 1]\n"
                "3: assign 4 6 [line 1]\n"
                "4: assign 7 4 [line 1]\n"
                "5: assign 3 7 [line 1]\n"
                "6: assign 8 3 [line 1]\n"
                "7: assign 2 8 [line 1]\n"
                "8: assign 9 2 [line 1]\n"
                "9: assign 1 9 [line 1]\n"
                "10: assign 10 1 [line 1]\n"
                "11: assign 0 10 [line 1]\n"
                "12: assign 11 0 [line 1]\n"
                "13: jlt 19 0 1 [line 3]\n"
                "14: jump 15 [line 3]\n"
                "15: jgt 17 2 3 [line 3]\n"
                "16: jump 21 [line 3]\n"
                "17: jlt 19 4 5 [line 3]\n"
                "18: jump 21 [line 3]\n"
                "19: assign 6 1 [line 3]\n"
                "20: jump 22 [line 3]\n"
                "21: assign 6 0 [line 3]\n"
                "22: assign 12 6 [line 3]\n"
                "23: assign 7 12 [line 3]\n"
                "24: jeq 28 12 1 [line 5]\n"
                "25: jump 26 [line 5]\n"
                "26: jeq 30 12 1 [line 5]\n"
                "27: jump 28 [line 5]\n"
                "28: jeq 30 0 1 [line 5]\n"
                "29: jump 32 [line 5]\n"
                "30: assign 6 1 [line 5]\n"
                "31: jump 33 [line 5]\n"
                "32: assign 6 0 [line 5]\n"
                "33: assign 13 6 [line 5]\n"
                "34: assign 7 13 [line 5]\n"
                "35: jeq 37 12 1 [line 8]\n"
                "36: jump 47 [line 8]\n"
                "37: jump 42 [line 8]\n"
                "38: enterfunc 0 [line 8]\n"
                "39: assign 69420 0 [line 8]\n"
                "40: jump 41 [line 8]\n"
                "41: exitfunc 0 [line 8]\n"
                "42: pusharg 1 [line 8]\n"
                "43: callfunc 0 [line 8]\n"
                "44: assign 6 69420 [line 8]\n"
                "45: jeq 54 6 1 [line 8]\n"
                "46: jump 47 [line 8]\n"
                "47: jump 52 [line 8]\n"
                "48: enterfunc 1 [line 8]\n"
                "49: assign 69420 1 [line 8]\n"
                "50: jump 51 [line 8]\n"
                "51: exitfunc 1 [line 8]\n"
                "52: jeq 54 1 1 [line 8]\n"
                "53: jump 56 [line 8]\n"
                "54: assign 7 1 [line 8]\n"
                "55: jump 57 [line 8]\n"
                "56: assign 7 0 [line 8]\n"
                "57: assign 12 7 [line 8]\n"
                "58: assign 8 12 [line 8]\n"
                "59: jeq 67 0 1 [line 10]\n"
                "60: jump 61 [line 10]\n"
                "61: jeq 67 1 1 [line 10]\n"
                "62: jump 63 [line 10]\n"
                "63: assign 13 12 [line 10]\n"
                "64: assign 6 13 [line 10]\n"
                "65: jeq 67 6 1 [line 10]\n"
                "66: jump 69 [line 10]\n"
                "67: assign 7 1 [line 10]\n"
                "68: jump 70 [line 10]\n"
                "69: assign 7 0 [line 10]\n"
                "70: jeq 72 7 1 [line 10]\n"
                "71: jump 97 [line 10]\n"
                "72: assign 16 2 [line 11]\n"
                "73: assign 8 16 [line 11]\n"
                "74: jlt 76 16 3 [line 11]\n"
                "75: jump 80 [line 11]\n"
                "76: jeq 78 0 1 [line 11]\n"
                "77: jump 80 [line 11]\n"
                "78: assign 9 1 [line 11]\n"
                "79: jump 81 [line 11]\n"
                "80: assign 9 0 [line 11]\n"
                "81: jeq 86 9 1 [line 11]\n"
                "82: jump 93 [line 11]\n"
                "83: add 16 16 0 [line 11]\n"
                "84: assign 10 16 [line 11]\n"
                "85: jump 74 [line 11]\n"
                "86: pusharg 16 [line 12]\n"
                "87: callfunc 0 [line 12]\n"
                "88: assign 11 69420 [line 12]\n"
                "89: jump 83 [line 13]\n"
                "90: add 16 16 0 [line 14]\n"
                "91: assign 6 16 [line 14]\n"
                "92: jump 83 [line 15]\n"
                "93: pusharg 13 [line 16]\n"
                "94: callfunc 0 [line 16]\n"
                "95: assign 6 69420 [line 16]\n"
                "96: jump 114 [line 18]\n"
                "97: jeq 101 13 1 [line 19]\n"
                "98: jump 99 [line 19]\n"
                "99: assign 6 1 [line 19]\n"
                "100: jump 102 [line 19]\n"
                "101: assign 6 0 [line 19]\n"
                "102: jeq 104 6 1 [line 19]\n"
                "103: jump 114 [line 19]\n"
                "104: jeq 108 13 1 [line 20]\n"
                "105: jump 106 [line 20]\n"
                "106: assign 7 1 [line 20]\n"
                "107: jump 109 [line 20]\n"
                "108: assign 7 0 [line 20]\n"
                "109: pusharg 7 [line 20]\n"
                "110: callfunc 0 [line 20]\n"
                "111: assign 8 69420 [line 20]\n"
                "112: jump 114 [line 21]\n"
                "113: jump 97 [line 22]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/backpatch/backpatch3.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(BinaryParserSuite, backpatch_p3t_assignments_complex)
{
    expected =  "1: assign 0 0 [line 1]\n"
                "2: assign 1 0 [line 1]\n"
                "3: assign 2 1 [line 2]\n"
                "4: assign 1 2 [line 2]\n"
                "5: assign 3 2 [line 3]\n"
                "6: assign 1 3 [line 3]\n"
                "7: assign 4 3 [line 4]\n"
                "8: assign 1 4 [line 4]\n"
                "9: assign 3 4 [line 6]\n"
                "10: assign 1 3 [line 6]\n"
                "11: assign 2 1 [line 6]\n"
                "12: assign 5 2 [line 6]\n"
                "13: assign 0 5 [line 6]\n"
                "14: assign 6 0 [line 6]\n"
                "15: add 1 3 4 [line 8]\n"
                "16: mul 5 1 4 [line 8]\n"
                "17: sub 6 0 2 [line 8]\n"
                "18: mul 7 1 6 [line 8]\n"
                "19: mul 8 7 4 [line 8]\n"
                "20: mul 9 2 4 [line 8]\n"
                "21: mod 10 7 2 [line 8]\n"
                "22: assign 2 10 [line 8]\n"
                "23: assign 11 2 [line 8]\n"
                "24: assign 0 11 [line 8]\n"
                "25: assign 12 0 [line 8]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/backpatch/p3t_assignments_complex.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(BinaryParserSuite, backpatch_p3t_assignments_objects)
{
    expected =  "1: tablesetelem 1 0 0 [line 1]\n"
                "2: tablegetelem 2 1 0 [line 1]\n"
                "3: tablegetelem 2 3 0 [line 2]\n"
                "4: tablegetelem 6 5 0 [line 2]\n"
                "5: tablesetelem 4 1 6 [line 2]\n"
                "6: tablegetelem 7 4 1 [line 2]\n"
                "7: tablegetelem 2 9 0 [line 3]\n"
                "8: tablegetelem 6 10 1 [line 3]\n"
                "9: tablegetelem 7 11 1 [line 3]\n"
                "10: tablegetelem 13 12 1 [line 3]\n"
                "11: tablesetelem 8 0 13 [line 3]\n"
                "12: tablegetelem 14 8 0 [line 3]\n"
                "13: tablegetelem 2 16 0 [line 4]\n"
                "14: tablesetelem 15 0 2 [line 4]\n"
                "15: tablegetelem 6 15 0 [line 4]\n"
                "16: tablegetelem 2 17 0 [line 5]\n"
                "17: tablegetelem 6 19 0 [line 5]\n"
                "18: tablegetelem 7 20 1 [line 5]\n"
                "19: tablesetelem 18 1 7 [line 5]\n"
                "20: tablegetelem 13 18 1 [line 5]\n"
                "21: tablegetelem 2 21 0 [line 6]\n"
                "22: tablegetelem 6 22 1 [line 6]\n"
                "23: tablegetelem 7 23 0 [line 6]\n"
                "24: tablegetelem 13 24 2 [line 6]\n"
                "25: tablesetelem 25 0 2 [line 6]\n"
                "26: tablegetelem 14 25 0 [line 6]\n"
                "27: tablegetelem 2 26 3 [line 7]\n"
                "28: tablegetelem 6 27 1 [line 7]\n"
                "29: tablegetelem 7 28 1 [line 7]\n"
                "30: callfunc 7 [line 7]\n"
                "31: assign 13 69420 [line 7]\n"
                "32: tablesetelem 31 0 3 [line 8]\n"
                "33: tablegetelem 2 31 0 [line 8]\n"
                "34: tablesetelem 30 4 2 [line 8]\n"
                "35: tablegetelem 6 30 4 [line 8]\n"
                "36: tablesetelem 29 4 6 [line 8]\n"
                "37: tablegetelem 7 29 4 [line 8]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/backpatch/p3t_assignments_objects.asc");
    GTEST_ASSERT_EQ(expected, actual);   
}

TEST_F(BinaryParserSuite, backpatch_p3t_assignments_simple)
{
    expected =  "1: assign 0 0 [line 2]\n"
                "2: assign 1 0 [line 2]\n"
                "3: add 1 0 1 [line 3]\n"
                "4: assign 0 1 [line 3]\n"
                "5: assign 2 0 [line 3]\n"
                "6: assign 3 0 [line 4]\n"
                "7: assign 1 3 [line 4]\n"
                "8: add 1 0 3 [line 5]\n"
                "9: assign 0 1 [line 5]\n"
                "10: assign 2 0 [line 5]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/backpatch/p3t_assignments_simple.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(BinaryParserSuite, backpatch_p3t_var_maths)
{
    expected =  "1: add 2 0 1 [line 1]\n"
                "2: mul 2 3 4 [line 2]\n"
                "3: sub 2 5 6 [line 3]\n"
                "4: div 2 7 8 [line 4]\n"
                "5: mod 2 9 10 [line 5]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/backpatch/p3t_var_maths.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(BinaryParserSuite, backpatch_p3t_relational)
{
    expected =  "1: assign 11 0 [line 1]\n"
                "2: assign 12 11 [line 1]\n"
                "3: assign 10 12 [line 1]\n"
                "4: assign 13 10 [line 1]\n"
                "5: assign 9 13 [line 1]\n"
                "6: assign 14 9 [line 1]\n"
                "7: assign 8 14 [line 1]\n"
                "8: assign 15 8 [line 1]\n"
                "9: assign 7 15 [line 1]\n"
                "10: assign 16 7 [line 1]\n"
                "11: assign 6 16 [line 1]\n"
                "12: assign 17 6 [line 1]\n"
                "13: assign 5 17 [line 1]\n"
                "14: assign 18 5 [line 1]\n"
                "15: assign 4 18 [line 1]\n"
                "16: assign 19 4 [line 1]\n"
                "17: assign 3 19 [line 1]\n"
                "18: assign 20 3 [line 1]\n"
                "19: assign 2 20 [line 1]\n"
                "20: assign 21 2 [line 1]\n"
                "21: assign 1 21 [line 1]\n"
                "22: assign 22 1 [line 1]\n"
                "23: assign 0 22 [line 1]\n"
                "24: assign 23 0 [line 1]\n"
                "25: jgt 47 0 1 [line 3]\n"
                "26: jump 27 [line 3]\n"
                "27: jlt 29 2 3 [line 3]\n"
                "28: jump 31 [line 3]\n"
                "29: jge 47 4 5 [line 3]\n"
                "30: jump 31 [line 3]\n"
                "31: jle 33 6 7 [line 3]\n"
                "32: jump 40 [line 3]\n"
                "33: jeq 37 8 1 [line 3]\n"
                "34: jump 35 [line 3]\n"
                "35: assign 12 1 [line 3]\n"
                "36: jump 38 [line 3]\n"
                "37: assign 12 0 [line 3]\n"
                "38: jeq 47 12 9 [line 3]\n"
                "39: jump 40 [line 3]\n"
                "40: jeq 44 10 1 [line 3]\n"
                "41: jump 42 [line 3]\n"
                "42: assign 13 1 [line 3]\n"
                "43: jump 45 [line 3]\n"
                "44: assign 13 0 [line 3]\n"
                "45: jne 47 13 11 [line 3]\n"
                "46: jump 49 [line 3]\n"
                "47: assign 14 1 [line 3]\n"
                "48: jump 50 [line 3]\n"
                "49: assign 14 0 [line 3]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/backpatch/p3t_relational.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(BinaryParserSuite, backpatch_p3t_object_creation_expr)
{
    expected =  "1: newtable 0 [line 1]\n"
                "2: newtable 0 [line 3]\n"
                "3: tablesetelem 0 0 1 [line 3]\n"
                "4: tablesetelem 0 1 2 [line 3]\n"
                "5: tablesetelem 0 2 3 [line 3]\n"
                "6: newtable 0 [line 5]\n"
                "7: newtable 1 [line 5]\n"
                "8: tablesetelem 1 0 0 [line 5]\n"
                "9: tablesetelem 1 1 1 [line 5]\n"
                "10: newtable 2 [line 5]\n"
                "11: newtable 3 [line 5]\n"
                "12: tablesetelem 3 0 0 [line 5]\n"
                "13: tablesetelem 3 1 1 [line 5]\n"
                "14: tablesetelem 3 2 0 [line 5]\n"
                "15: tablesetelem 3 3 4 [line 5]\n"
                "16: tablesetelem 3 5 1 [line 5]\n"
                "17: tablesetelem 3 4 2 [line 5]\n"
                "18: jump 22 [line 7]\n"
                "19: enterfunc 0 [line 7]\n"
                "20: newtable 0 [line 7]\n"
                "21: exitfunc 0 [line 7]\n"
                "22: newtable 0 [line 8]\n"
                "23: tablesetelem 0 0 1 [line 8]\n"
                "24: tablesetelem 0 1 2 [line 8]\n"
                "25: tablesetelem 0 2 3 [line 8]\n"
                "26: newtable 1 [line 8]\n"
                "27: tablesetelem 1 1 2 [line 8]\n"
                "28: tablesetelem 1 2 0 [line 8]\n"
                "29: tablesetelem 1 4 0 [line 8]\n"
                "30: newtable 2 [line 9]\n"
                "31: tablesetelem 2 5 4 [line 9]\n"
                "32: tablesetelem 2 2 6 [line 9]\n"
                "33: tablesetelem 2 3 0 [line 9]\n"
                "34: tablesetelem 2 4 0 [line 9]\n"
                "35: tablesetelem 2 5 1 [line 9]\n"
                "36: jump 41 [line 11]\n"
                "37: enterfunc 1 [line 11]\n"
                "38: assign 0 5 [line 11]\n"
                "39: assign 0 0 [line 11]\n"
                "40: exitfunc 1 [line 11]\n"
                "41: newtable 0 [line 11]\n"
                "42: tablesetelem 0 5 1 [line 11]\n"
                "43: newtable 1 [line 11]\n"
                "44: tablesetelem 1 5 0 [line 11]\n"
                "45: newtable 2 [line 11]\n"
                "46: tablesetelem 2 5 1 [line 11]\n"
                "47: newtable 3 [line 11]\n"
                "48: tablesetelem 3 5 2 [line 11]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/backpatch/p3t_object_creation_expr.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(BinaryParserSuite, backpatch_p3t_if_else)
{
    expected =  "1: jeq 3 1 1 [line 1]\n"
                "2: jump 5 [line 1]\n"
                "3: assign 0 0 [line 2]\n"
                "4: assign 1 0 [line 2]\n"
                "5: jeq 7 0 1 [line 4]\n"
                "6: jump 9 [line 4]\n"
                "7: assign 2 1 [line 5]\n"
                "8: assign 1 2 [line 5]\n"
                "9: jeq 11 0 1 [line 7]\n"
                "10: jump 15 [line 7]\n"
                "11: jeq 13 2 1 [line 8]\n"
                "12: jump 15 [line 8]\n"
                "13: assign 3 2 [line 9]\n"
                "14: assign 1 3 [line 9]\n"
                "15: jeq 17 0 1 [line 11]\n"
                "16: jump 20 [line 11]\n"
                "17: assign 4 0 [line 12]\n"
                "18: assign 1 4 [line 12]\n"
                "19: jump 22 [line 13]\n"
                "20: assign 3 0 [line 14]\n"
                "21: assign 1 3 [line 14]\n"
                "22: jeq 24 0 1 [line 16]\n"
                "23: jump 30 [line 16]\n"
                "24: jeq 26 2 1 [line 17]\n"
                "25: jump 29 [line 17]\n"
                "26: assign 3 1 [line 18]\n"
                "27: assign 1 3 [line 18]\n"
                "28: jump 30 [line 19]\n"
                "29: sub 1 4 1 [line 20]\n"
                "30: jeq 32 0 1 [line 22]\n"
                "31: jump 38 [line 22]\n"
                "32: jeq 34 2 1 [line 23]\n"
                "33: jump 36 [line 23]\n"
                "34: sub 1 4 3 [line 24]\n"
                "35: jump 37 [line 25]\n"
                "36: sub 1 4 0 [line 26]\n"
                "37: jump 39 [line 27]\n"
                "38: sub 1 3 4 [line 28]\n"
                "39: jeq 41 0 1 [line 30]\n"
                "40: jump 45 [line 30]\n"
                "41: jeq 43 2 1 [line 31]\n"
                "42: jump 44 [line 31]\n"
                "43: add 1 3 4 [line 32]\n"
                "44: jump 46 [line 34]\n"
                "45: sub 1 3 3 [line 35]\n"
                "46: jeq 48 0 1 [line 38]\n"
                "47: jump 50 [line 38]\n"
                "48: add 1 4 3 [line 39]\n"
                "49: jump 59 [line 40]\n"
                "50: jeq 52 2 1 [line 40]\n"
                "51: jump 54 [line 40]\n"
                "52: add 1 5 3 [line 41]\n"
                "53: jump 59 [line 42]\n"
                "54: jeq 56 3 1 [line 42]\n"
                "55: jump 58 [line 42]\n"
                "56: add 1 4 3 [line 43]\n"
                "57: jump 59 [line 44]\n"
                "58: sub 1 6 3 [line 45]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/backpatch/p3t_if_else.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(BinaryParserSuite, backpatch_p3t_funcdecl)
{
    expected =  "1: jump 8 [line 2]\n"
                "2: enterfunc 0 [line 2]\n"
                "3: add 0 0 1 [line 3]\n"
                "4: assign 69420 0 [line 4]\n"
                "5: jump 7 [line 4]\n"
                "6: sub 0 1 2 [line 5]\n"
                "7: exitfunc 0 [line 6]\n"
                "8: jump 11 [line 8]\n"
                "9: enterfunc 1 [line 8]\n"
                "10: exitfunc 1 [line 8]\n"
                "11: jump 18 [line 10]\n"
                "12: enterfunc 2 [line 10]\n"
                "13: sub 0 3 2 [line 10]\n"
                "14: assign 69420 4 [line 10]\n"
                "15: jump 17 [line 10]\n"
                "16: sub 0 5 2 [line 10]\n"
                "17: exitfunc 2 [line 10]\n"
                "18: assign 2 2 [line 10]\n"
                "19: assign 4 2 [line 10]\n"
                "20: jump 23 [line 12]\n"
                "21: enterfunc 3 [line 12]\n"
                "22: exitfunc 3 [line 12]\n"
                "23: jump 33 [line 14]\n"
                "24: enterfunc 4 [line 14]\n"
                "25: jump 32 [line 14]\n"
                "26: enterfunc 5 [line 14]\n"
                "27: pusharg 6 [line 14]\n"
                "28: pusharg 7 [line 14]\n"
                "29: callfunc 0 [line 14]\n"
                "30: assign 4 69420 [line 14]\n"
                "31: exitfunc 5 [line 14]\n"
                "32: exitfunc 4 [line 14]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/backpatch/p3t_funcdecl.asc");
    GTEST_ASSERT_EQ(expected, actual);
}


TEST_F(BinaryParserSuite, backpatch_p3t_flow_control)
{
    expected =  "1: assign 0 0 [line 1]\n"
                "2: assign 1 0 [line 1]\n"
                "3: jeq 5 1 1 [line 1]\n"
                "4: jump 10 [line 1]\n"
                "5: sub 2 1 2 [line 2]\n"
                "6: jump 10 [line 3]\n"
                "7: sub 1 3 2 [line 4]\n"
                "8: jump 1 [line 5]\n"
                "9: jump 1 [line 6]\n"
                "10: jeq 12 4 1 [line 8]\n"
                "11: jump 19 [line 8]\n"
                "12: jeq 14 5 1 [line 8]\n"
                "13: jump 18 [line 8]\n"
                "14: jeq 16 6 1 [line 8]\n"
                "15: jump 17 [line 8]\n"
                "16: jump 14 [line 8]\n"
                "17: jump 12 [line 8]\n"
                "18: jump 10 [line 8]\n"
                "19: jeq 21 7 1 [line 10]\n"
                "20: jump 27 [line 10]\n"
                "21: jeq 23 8 1 [line 10]\n"
                "22: jump 25 [line 10]\n"
                "23: sub 1 9 2 [line 10]\n"
                "24: jump 26 [line 10]\n"
                "25: sub 1 10 2 [line 10]\n"
                "26: jump 19 [line 10]\n"
                "27: jeq 29 11 1 [line 12]\n"
                "28: jump 47 [line 12]\n"
                "29: sub 1 12 2 [line 13]\n"
                "30: jeq 34 13 1 [line 13]\n"
                "31: jump 46 [line 13]\n"
                "32: sub 3 14 2 [line 13]\n"
                "33: jump 30 [line 13]\n"
                "34: jeq 36 15 1 [line 14]\n"
                "35: jump 38 [line 14]\n"
                "36: sub 4 16 2 [line 15]\n"
                "37: jump 43 [line 16]\n"
                "38: jeq 40 17 1 [line 16]\n"
                "39: jump 42 [line 16]\n"
                "40: sub 1 18 2 [line 17]\n"
                "41: jump 43 [line 18]\n"
                "42: sub 1 19 2 [line 19]\n"
                "43: jump 46 [line 20]\n"
                "44: jump 32 [line 21]\n"
                "45: jump 32 [line 22]\n"
                "46: jump 27 [line 22]\n"
                "47: sub 1 20 2 [line 24]\n"
                "48: jeq 52 21 1 [line 24]\n"
                "49: jump 58 [line 24]\n"
                "50: sub 3 22 2 [line 24]\n"
                "51: jump 48 [line 24]\n"
                "52: jeq 54 23 1 [line 25]\n"
                "53: jump 56 [line 25]\n"
                "54: jump 56 [line 26]\n"
                "55: jump 52 [line 26]\n"
                "56: jump 50 [line 27]\n"
                "57: jump 50 [line 28]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/backpatch/p3t_flow_control.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(BinaryParserSuite, backpatch_p3t_flow_control_error)
{
    expected =  "\x1B[31mError, in line: 3:\x1B[0m invalid keyword CONTINUE outside of loop\n"
                "\x1B[31mError, in line: 4:\x1B[0m invalid keyword BREAK outside of loop\n"
                "\x1B[31mError, in line: 6:\x1B[0m Invalid return, used outside a function block\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/backpatch/p3t_flow_control_error.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(BinaryParserSuite, backpatch_p3t_const_maths)
{
    expected =  "1: add 0 0 1 [line 1]\n"
                "2: mul 0 2 3 [line 2]\n"
                "3: sub 0 0 4 [line 3]\n"
                "4: div 0 5 6 [line 4]\n"
                "5: mod 0 7 5 [line 5]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/backpatch/p3t_const_maths.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(BinaryParserSuite, backpatch_p3t_basic_expr)
{
    expected =  "1: assign 0 0 [line 2]\n"
                "2: assign 1 0 [line 2]\n"
                "3: assign 2 1 [line 3]\n"
                "4: assign 1 2 [line 3]\n"
                "5: assign 3 2 [line 4]\n"
                "6: assign 1 3 [line 4]\n"
                "7: assign 4 3 [line 5]\n"
                "8: assign 1 4 [line 5]\n"
                "9: add 1 0 2 [line 7]\n"
                "10: add 6 1 3 [line 7]\n"
                "11: add 7 6 4 [line 7]\n"
                "12: assign 5 7 [line 7]\n"
                "13: assign 8 5 [line 7]\n"
                "14: add 1 3 4 [line 8]\n"
                "15: add 6 2 1 [line 8]\n"
                "16: add 7 0 6 [line 8]\n"
                "17: assign 5 7 [line 8]\n"
                "18: assign 8 5 [line 8]\n"
                "19: mul 1 0 4 [line 9]\n"
                "20: add 6 0 2 [line 9]\n"
                "21: assign 5 6 [line 9]\n"
                "22: assign 7 5 [line 9]\n"
                "23: add 1 0 2 [line 10]\n"
                "24: mul 6 1 4 [line 10]\n"
                "25: assign 5 1 [line 10]\n"
                "26: assign 7 5 [line 10]\n"
                "27: mul 1 0 2 [line 12]\n"
                "28: div 6 3 4 [line 12]\n"
                "29: mod 7 6 5 [line 12]\n"
                "30: sub 8 1 7 [line 12]\n"
                "31: assign 5 8 [line 12]\n"
                "32: assign 9 5 [line 12]\n"
                "33: mod 1 4 5 [line 13]\n"
                "34: div 6 3 1 [line 13]\n"
                "35: sub 7 2 6 [line 13]\n"
                "36: mul 8 0 7 [line 13]\n"
                "37: assign 5 8 [line 13]\n"
                "38: assign 9 5 [line 13]\n"
                "39: add 1 0 2 [line 15]\n"
                "40: mul 6 1 4 [line 15]\n"
                "41: add 7 3 4 [line 15]\n"
                "42: mul 8 7 4 [line 15]\n"
                "43: div 9 1 7 [line 15]\n"
                "44: mul 10 9 4 [line 15]\n"
                "45: mod 11 9 5 [line 15]\n"
                "46: assign 5 11 [line 15]\n"
                "47: assign 12 5 [line 15]\n"
                "48: assign 1 0 [line 17]\n"
                "49: sub 0 0 5 [line 17]\n"
                "50: add 2 2 5 [line 17]\n"
                "51: assign 6 2 [line 17]\n"
                "52: sub 7 1 6 [line 17]\n"
                "53: sub 3 3 5 [line 17]\n"
                "54: assign 8 3 [line 17]\n"
                "55: add 9 7 8 [line 17]\n"
                "56: assign 10 4 [line 17]\n"
                "57: add 4 4 5 [line 17]\n"
                "58: sub 11 9 10 [line 17]\n"
                "59: tablegetelem 6 14 0 [line 19]\n"
                "60: assign 1 6 [line 19]\n"
                "61: sub 6 6 5 [line 19]\n"
                "62: tablesetelem 14 0 6 [line 19]\n"
                "63: tablegetelem 8 15 6 [line 19]\n"
                "64: add 8 8 5 [line 19]\n"
                "65: tablesetelem 15 6 8 [line 19]\n"
                "66: sub 9 1 8 [line 19]\n"
                "67: tablegetelem 11 16 1 [line 19]\n"
                "68: sub 11 11 5 [line 19]\n"
                "69: tablesetelem 16 1 11 [line 19]\n"
                "70: add 12 9 11 [line 19]\n"
                "71: tablegetelem 19 17 2 [line 19]\n"
                "72: assign 18 19 [line 19]\n"
                "73: add 19 19 5 [line 19]\n"
                "74: tablesetelem 17 2 19 [line 19]\n"
                "75: sub 20 12 18 [line 19]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/backpatch/p3t_basic_expr.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(BinaryParserSuite, backpatch_p3t_calls)
{
    expected =  "1: assign 4 0 [line 2]\n"
                "2: assign 5 4 [line 2]\n"
                "3: assign 3 5 [line 2]\n"
                "4: assign 6 3 [line 2]\n"
                "5: assign 2 6 [line 2]\n"
                "6: assign 7 2 [line 2]\n"
                "7: assign 1 7 [line 2]\n"
                "8: assign 8 1 [line 2]\n"
                "9: assign 0 8 [line 2]\n"
                "10: assign 9 0 [line 2]\n"
                "11: tablegetelem 5 10 0 [line 4]\n"
                "12: pusharg 0 [line 4]\n"
                "13: pusharg 0 [line 4]\n"
                "14: callfunc 5 [line 4]\n"
                "15: assign 6 69420 [line 4]\n"
                "16: callfunc 4 [line 6]\n"
                "17: assign 5 69420 [line 6]\n"
                "18: callfunc 4 [line 6]\n"
                "19: assign 6 69420 [line 6]\n"
                "20: callfunc 6 [line 6]\n"
                "21: assign 7 69420 [line 6]\n"
                "22: pusharg 7 [line 6]\n"
                "23: pusharg 3 [line 6]\n"
                "24: pusharg 2 [line 6]\n"
                "25: pusharg 1 [line 6]\n"
                "26: pusharg 0 [line 6]\n"
                "27: callfunc 5 [line 6]\n"
                "28: assign 8 69420 [line 6]\n"
                "29: jump 34 [line 9]\n"
                "30: enterfunc 0 [line 9]\n"
                "31: callfunc 1 [line 9]\n"
                "32: assign 5 69420 [line 9]\n"
                "33: exitfunc 0 [line 9]\n"
                "34: callfunc 0 [line 9]\n"
                "35: assign 5 69420 [line 9]\n"
                "36: callfunc 1 [line 9]\n"
                "37: assign 6 69420 [line 9]\n"
                "38: callfunc 6 [line 9]\n"
                "39: assign 7 69420 [line 9]\n"
                "40: pusharg 7 [line 9]\n"
                "41: pusharg 5 [line 9]\n"
                "42: callfunc 0 [line 9]\n"
                "43: assign 8 69420 [line 9]\n";
    actual = util::exec::exec("./d_target_code ../../test/files/phase3_tests/backpatch/p3t_calls.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

//----------------------------------------------------------------------------------------------------------------------

#ifdef TESTING
int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
#endif
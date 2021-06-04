#include <gtest/gtest.h>
#include "../util/exec/exec.h"

/**
 * @brief Test suite for the intermediate code
 * production.
 * 
**/
class InterCodeSuite : public ::testing::Test
{
protected:
    std::string expected;
    std::string actual;
};

/* ------------ assign -------------- */

TEST_F(InterCodeSuite, assign_simple)
{
    expected = "1: assign x 2 [line 1]\n"
               "2: assign ^0 x [line 1]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/assign/simple.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(InterCodeSuite, assign_calls_nested)
{
    expected = "1: jump 4 [line 1]\n"
               "2: funcstart f [line 1]\n"
               "3: funcend f [line 2]\n"
               "4: jump 7 [line 4]\n"
               "5: funcstart g [line 4]\n"
               "6: funcend g [line 5]\n"
               "7: param 3 [line 7]\n"
               "8: call g [line 7]\n"
               "9: getretval ^0 [line 7]\n"
               "10: param ^0 [line 7]\n"
               "11: call f [line 7]\n"
               "12: getretval ^1 [line 7]\n"
               "13: assign x ^1 [line 7]\n"
               "14: assign ^2 x [line 7]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/assign/calls_nested.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(InterCodeSuite, assign_string)
{
    expected = "1: assign a \"2\" [line 1]\n"
               "2: assign ^0 a [line 1]\n"
               "3: assign a \"adsad\" [line 2]\n"
               "4: assign ^0 a [line 2]\n"
               "5: assign a \"2ads\" [line 3]\n"
               "6: assign ^0 a [line 3]\n"
               "7: assign a \"2z\" [line 4]\n"
               "8: assign ^0 a [line 4]\n"
               "9: assign a \"222\" [line 5]\n"
               "10: assign ^0 a [line 5]\n"
               "11: assign a \"21\" [line 6]\n"
               "12: assign ^0 a [line 6]\n"
               "13: assign a \"2111\" [line 7]\n"
               "14: assign ^0 a [line 7]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/assign/string.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

/* ------------ Function -------------- */

TEST_F(InterCodeSuite, functions_single)
{
    expected = "1: jump 4 [line 1]\n"
               "2: funcstart f [line 1]\n"
               "3: funcend f [line 2]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/functions/single.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(InterCodeSuite, functions_many)
{
    expected = "1: jump 4 [line 1]\n"
               "2: funcstart f [line 1]\n"
               "3: funcend f [line 2]\n"
               "4: jump 7 [line 4]\n"
               "5: funcstart g [line 4]\n"
               "6: funcend g [line 5]\n"
               "7: jump 10 [line 8]\n"
               "8: funcstart y [line 8]\n"
               "9: funcend y [line 9]\n"
               "10: jump 13 [line 17]\n"
               "11: funcstart r [line 17]\n"
               "12: funcend r [line 18]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/functions/many.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(InterCodeSuite, functions_nested)
{
    expected = "1: jump 16 [line 5]\n"
               "2: funcstart a [line 5]\n"
               "3: jump 12 [line 6]\n"
               "4: funcstart b [line 6]\n"
               "5: jump 11 [line 7]\n"
               "6: funcstart c [line 7]\n"
               "7: jump 10 [line 8]\n"
               "8: funcstart d [line 8]\n"
               "9: funcend d [line 9]\n"
               "10: funcend c [line 10]\n"
               "11: funcend b [line 11]\n"
               "12: jump 15 [line 12]\n"
               "13: funcstart e [line 12]\n"
               "14: funcend e [line 13]\n"
               "15: funcend a [line 14]\n"
               "16: jump 19 [line 16]\n"
               "17: funcstart e [line 16]\n"
               "18: funcend e [line 27]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/functions/nested.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(InterCodeSuite, functions_shadowed)
{
    expected = "1: jump 7 [line 1]\n"
               "2: funcstart lkdland [line 1]\n"
               "3: jump 6 [line 2]\n"
               "4: funcstart lkdland [line 2]\n"
               "5: funcend lkdland [line 3]\n"
               "6: funcend lkdland [line 5]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/functions/shadowed.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(InterCodeSuite, functions_anonymous)
{
    expected = "1: jump 10 [line 1]\n"
               "2: funcstart $0 [line 1]\n"
               "3: jump 6 [line 2]\n"
               "4: funcstart c [line 2]\n"
               "5: funcend c [line 3]\n"
               "6: jump 9 [line 4]\n"
               "7: funcstart $1 [line 4]\n"
               "8: funcend $1 [line 5]\n"
               "9: funcend $0 [line 6]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/functions/anonymous.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(InterCodeSuite, functions_call_warning)
{
    expected = "1: call g [line 1]\n"
               "2: getretval ^0 [line 1]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/functions/call_warning.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(InterCodeSuite, functions_call_noparameters)
{
    expected = "1: jump 4 [line 1]\n"
               "2: funcstart f [line 1]\n"
               "3: funcend f [line 2]\n"
               "4: call f [line 4]\n"
               "5: getretval ^0 [line 4]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/functions/call_no_parameters.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(InterCodeSuite, functions_call_parameters)
{
    expected = "1: jump 4 [line 1]\n"
               "2: funcstart f [line 1]\n"
               "3: funcend f [line 2]\n"
               "4: param 0 [line 4]\n"
               "5: call f [line 4]\n"
               "6: getretval ^0 [line 4]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/functions/call_parameters.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(InterCodeSuite, functions_call_with_symbol_parameters)
{
    expected = "\x1B[33mWarning, in line: 10:\x1B[0m Too many arguments passed to function: f, defined in line: 1\n"
               "\x1B[33mWarning, in line: 11:\x1B[0m Too many arguments passed to function: f, defined in line: 1\n"
               "\x1B[33mWarning, in line: 12:\x1B[0m Too many arguments passed to function: f, defined in line: 1\n"
               "1: jump 4 [line 1]\n"
               "2: funcstart f [line 1]\n"
               "3: funcend f [line 2]\n"
               "4: param c [line 5]\n"
               "5: call f [line 5]\n"
               "6: getretval ^0 [line 5]\n"
               "7: param 0 [line 10]\n"
               "8: param c [line 10]\n"
               "9: call f [line 10]\n"
               "10: getretval ^0 [line 10]\n"
               "11: param 3 [line 11]\n"
               "12: param 10 [line 11]\n"
               "13: param c [line 11]\n"
               "14: call f [line 11]\n"
               "15: getretval ^0 [line 11]\n"
               "16: param aaaaa [line 12]\n"
               "17: param aaaaa [line 12]\n"
               "18: param a [line 12]\n"
               "19: param 0 [line 12]\n"
               "20: param c [line 12]\n"
               "21: call f [line 12]\n"
               "22: getretval ^0 [line 12]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/functions/call_with_symbol_parameters.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(InterCodeSuite, functions_call_less_parameters)
{
    expected = "\x1B[31mError, in line: 4:\x1B[0m Too few arguments passed to function: f, defined in line: 1\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/functions/call_less_parameters.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(InterCodeSuite, functions_call_nested)
{
    expected = "\x1B[33mWarning, in line: 10:\x1B[0m Too many arguments passed to function: g, defined in line: 6\n"
               "\x1B[33mWarning, in line: 12:\x1B[0m Too many arguments passed to function: g, defined in line: 6\n"
               "1: jump 4 [line 2]\n"
               "2: funcstart f [line 2]\n"
               "3: funcend f [line 4]\n"
               "4: jump 7 [line 6]\n"
               "5: funcstart g [line 6]\n"
               "6: funcend g [line 8]\n"
               "7: param 1 [line 10]\n"
               "8: call g [line 10]\n"
               "9: getretval ^0 [line 10]\n"
               "10: param ^0 [line 10]\n"
               "11: call f [line 10]\n"
               "12: getretval ^1 [line 10]\n"
               "13: param 2 [line 12]\n"
               "14: call y [line 12]\n"
               "15: getretval ^0 [line 12]\n"
               "16: param ^0 [line 12]\n"
               "17: call g [line 12]\n"
               "18: getretval ^1 [line 12]\n"
               "19: param ^1 [line 12]\n"
               "20: call f [line 12]\n"
               "21: getretval ^2 [line 12]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/functions/call_nested.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(InterCodeSuite, functions_call_more_parameters)
{
    expected = "\x1B[33mWarning, in line: 5:\x1B[0m Too many arguments passed to function: f, defined in line: 1\n"
               "1: jump 4 [line 1]\n"
               "2: funcstart f [line 1]\n"
               "3: funcend f [line 3]\n"
               "4: param 2 [line 5]\n"
               "5: param a [line 5]\n"
               "6: call f [line 5]\n"
               "7: getretval ^0 [line 5]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/functions/call_more_parameters.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(InterCodeSuite, functions_call_function_def)
{
    expected = "1: jump 4 [line 1]\n"
               "2: funcstart f [line 1]\n"
               "3: funcend f [line 1]\n"
               "4: param 1 [line 1]\n"
               "5: call f [line 1]\n"
               "6: getretval ^0 [line 1]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/functions/call_function_def.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(InterCodeSuite, functions_call_nested_with_func_def)
{
    expected = "1: jump 4 [line 1]\n"
               "2: funcstart x [line 1]\n"
               "3: funcend x [line 1]\n"
               "4: param d [line 1]\n"
               "5: call x [line 1]\n"
               "6: getretval ^0 [line 1]\n"
               "7: param ^0 [line 1]\n"
               "8: call g [line 1]\n"
               "9: getretval ^1 [line 1]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/functions/call_nested_with_func_def.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(InterCodeSuite, functions_call_anonymous_func_def)
{
    expected = "1: jump 4 [line 1]\n"
               "2: funcstart $0 [line 1]\n"
               "3: funcend $0 [line 1]\n"
               "4: param 4 [line 1]\n"
               "5: param 1 [line 1]\n"
               "6: call $0 [line 1]\n"
               "7: getretval ^0 [line 1]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/functions/call_anonymous_func_def.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(InterCodeSuite, functions_call_method_call) {
    expected =  "1: tablegetelem ^0 a \"f\" [line 1]\n"
                "2: param a [line 1]\n"
                "3: call ^0 [line 1]\n"
                "4: getretval ^1 [line 1]\n"
                "5: tablegetelem ^0 a \"f\" [line 3]\n"
                "6: param s [line 3]\n"
                "7: param 9 [line 3]\n"
                "8: param a [line 3]\n"
                "9: call ^0 [line 3]\n"
                "10: getretval ^1 [line 3]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/functions/call_method_call.asc");
    GTEST_ASSERT_EQ(expected, actual);            
}

/* ------------ return -------------- */

TEST_F(InterCodeSuite, return_void)
{
    expected = "1: jump 6 [line 1]\n"
               "2: funcstart f [line 1]\n"
               "3: return [line 2]\n"
               "4: jump 5 [line 2]\n"
               "5: funcend f [line 3]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/return/return_void.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(InterCodeSuite, return_symbol)
{
    expected = "1: jump 6 [line 5]\n"
               "2: funcstart f [line 5]\n"
               "3: return x [line 6]\n"
               "4: jump 5 [line 6]\n"
               "5: funcend f [line 7]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/return/return_symbol.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(InterCodeSuite, return_many_and_nested)
{
    expected = "1: jump 17 [line 1]\n"
               "2: funcstart MPIFTEKI [line 1]\n"
               "3: return [line 2]\n"
               "4: jump 16 [line 2]\n"
               "5: return a [line 4]\n"
               "6: jump 16 [line 4]\n"
               "7: jump 12 [line 6]\n"
               "8: funcstart $0 [line 6]\n"
               "9: return [line 6]\n"
               "10: jump 11 [line 6]\n"
               "11: funcend $0 [line 6]\n"
               "12: call $0 [line 6]\n"
               "13: getretval ^0 [line 6]\n"
               "14: return ^0 [line 6]\n"
               "15: jump 16 [line 6]\n"
               "16: funcend MPIFTEKI [line 7]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/return/return_many_and_nested.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

/* ------------ Logical ----------------- */

TEST_F(InterCodeSuite, logical_greater_simple)
{
    expected = "1: if_greater 3 2 3 [line 1]\n"
               "2: jump 5 [line 1]\n"
               "3: assign ^0 'true' [line 1]\n"
               "4: jump 6 [line 1]\n"
               "5: assign ^0 'false' [line 1]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/logical/greater_simple.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(InterCodeSuite, logical_equal_simple)
{
    expected = "1: if_eq 3 2 3 [line 1]\n"
               "2: jump 5 [line 1]\n"
               "3: assign ^0 'true' [line 1]\n"
               "4: jump 6 [line 1]\n"
               "5: assign ^0 'false' [line 1]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/logical/equal_simple.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(InterCodeSuite, logical_greater_equal_simple)
{
    expected = "1: if_greatereq 3 2 3 [line 1]\n"
               "2: jump 5 [line 1]\n"
               "3: assign ^0 'true' [line 1]\n"
               "4: jump 6 [line 1]\n"
               "5: assign ^0 'false' [line 1]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/logical/greater_equal_simple.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(InterCodeSuite, logical_less_equal_simple)
{
    expected = "1: if_lesseq 3 2 3 [line 1]\n"
               "2: jump 5 [line 1]\n"
               "3: assign ^0 'true' [line 1]\n"
               "4: jump 6 [line 1]\n"
               "5: assign ^0 'false' [line 1]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/logical/less_equal_simple.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(InterCodeSuite, logical_less_simple)
{
    expected = "1: if_less 3 2 3 [line 1]\n"
               "2: jump 5 [line 1]\n"
               "3: assign ^0 'true' [line 1]\n"
               "4: jump 6 [line 1]\n"
               "5: assign ^0 'false' [line 1]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/logical/less_simple.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(InterCodeSuite, logical_not_equal_simple)
{
    expected = "1: if_noteq 3 2 3 [line 1]\n"
               "2: jump 5 [line 1]\n"
               "3: assign ^0 'true' [line 1]\n"
               "4: jump 6 [line 1]\n"
               "5: assign ^0 'false' [line 1]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/logical/not_equal_simple.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(InterCodeSuite, logical_errors)
{
    expected = "\x1B[31mError, in line: 1:\x1B[0m Invalid use of comparison operator on const string \"string\"\n"
               "\x1B[31mError, in line: 3:\x1B[0m Invalid use of comparison operator on const bool \'true\'\n"
               "\x1B[31mError, in line: 5:\x1B[0m Invalid use of comparison operator on library function print\n"
               "\x1B[31mError, in line: 9:\x1B[0m Invalid use of comparison operator on user function s\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/logical/errors.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

/* ------------- Loop ------------------- */

TEST_F(InterCodeSuite, loop_while_simple)
{
    expected = "1: if_greater 2 6 3 [line 1]\n"
               "2: jump 5 [line 1]\n"
               "3: assign ^0 'true' [line 1]\n"
               "4: jump 6 [line 1]\n"
               "5: assign ^0 'false' [line 1]\n"
               "6: if_eq ^0 'true' 8 [line 1]\n"
               "7: jump 9 [line 1]\n"
               "8: jump 1 [line 1]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/loop/while_simple.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(InterCodeSuite, loop_while_simple_with_stms)
{
    expected = "1: if_eq 1 'true' 3 [line 1]\n"
               "2: jump 6 [line 1]\n"
               "3: assign x 9 [line 2]\n"
               "4: assign ^0 x [line 2]\n"
               "5: jump 1 [line 5]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/loop/while_simple_with_stmts.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(InterCodeSuite, loop_while_many_nested)
{
    expected = "1: if_less x 0 3 [line 1]\n"
               "2: jump 5 [line 1]\n"
               "3: assign ^0 'true' [line 1]\n"
               "4: jump 6 [line 1]\n"
               "5: assign ^0 'false' [line 1]\n"
               "6: if_eq ^0 'true' 8 [line 1]\n"
               "7: jump 17 [line 1]\n"
               "8: assign x 0 [line 2]\n"
               "9: assign ^1 x [line 2]\n"
               "10: if_eq 1 'true' 12 [line 3]\n"
               "11: jump 16 [line 3]\n"
               "12: if_eq 0 'true' 14 [line 3]\n"
               "13: jump 15 [line 3]\n"
               "14: jump 12 [line 3]\n"
               "15: jump 10 [line 3]\n"
               "16: jump 1 [line 4]\n"
               "17: if_eq s 'true' 19 [line 9]\n"
               "18: jump 20 [line 9]\n"
               "19: jump 17 [line 10]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/loop/while_many_nested.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(InterCodeSuite, loop_while_continue_single)
{
    expected = "1: if_eq 1 'true' 3 [line 1]\n"
               "2: jump 5 [line 1]\n"
               "3: jump 1 [line 2]\n"
               "4: jump 1 [line 3]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/loop/while_continue_single.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(InterCodeSuite, loop_while_continue_many_nested)
{
    expected = "1: if_eq 1 'true' 3 [line 1]\n"
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
               "16: if_less 3 0 18 [line 8]\n"
               "17: jump 20 [line 8]\n"
               "18: assign ^0 'true' [line 8]\n"
               "19: jump 21 [line 8]\n"
               "20: assign ^0 'false' [line 8]\n"
               "21: if_eq ^0 'true' 23 [line 8]\n"
               "22: jump 28 [line 8]\n"
               "23: jump 16 [line 9]\n"
               "24: jump 16 [line 10]\n"
               "25: jump 16 [line 10]\n"
               "26: jump 16 [line 10]\n"
               "27: jump 16 [line 15]\n"
               "28: jump 1 [line 20]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/loop/while_continue_many_nested.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(InterCodeSuite, loop_while_break_single)
{
    expected = "1: if_eq 1 'true' 3 [line 1]\n"
               "2: jump 5 [line 1]\n"
               "3: jump 5 [line 2]\n"
               "4: jump 1 [line 3]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/loop/while_break_single.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(InterCodeSuite, loop_for_simple)
{
    expected = "1: assign i 0 [line 1]\n"
               "2: assign ^0 i [line 1]\n"
               "3: if_less i 20 5 [line 1]\n"
               "4: jump 7 [line 1]\n"
               "5: assign ^1 'true' [line 1]\n"
               "6: jump 8 [line 1]\n"
               "7: assign ^1 'false' [line 1]\n"
               "8: if_eq ^1 'true' 13 [line 1]\n"
               "9: jump 14 [line 1]\n"
               "10: assign ^2 i [line 1]\n"
               "11: add i i 1 [line 1]\n"
               "12: jump 3 [line 1]\n"
               "13: jump 10 [line 2]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/loop/for_simple.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(InterCodeSuite, loop_for_simple_with_stmts)
{
    expected = "1: assign i 0 [line 1]\n"
               "2: assign ^0 i [line 1]\n"
               "3: if_less i 20 5 [line 1]\n"
               "4: jump 7 [line 1]\n"
               "5: assign ^1 'true' [line 1]\n"
               "6: jump 8 [line 1]\n"
               "7: assign ^1 'false' [line 1]\n"
               "8: if_eq ^1 'true' 13 [line 1]\n"
               "9: jump 16 [line 1]\n"
               "10: assign ^2 i [line 1]\n"
               "11: add i i 1 [line 1]\n"
               "12: jump 3 [line 1]\n"
               "13: assign x 9 [line 2]\n"
               "14: assign ^3 x [line 2]\n"
               "15: jump 10 [line 5]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/loop/for_simple_with_stmts.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(InterCodeSuite, loop_for_many_nested)
{
    expected =  "1: assign i 0 [line 1]\n"
                "2: assign ^0 i [line 1]\n"
                "3: if_less i 20 5 [line 1]\n"
                "4: jump 7 [line 1]\n"
                "5: assign ^1 'true' [line 1]\n"
                "6: jump 8 [line 1]\n"
                "7: assign ^1 'false' [line 1]\n"
                "8: if_eq ^1 'true' 13 [line 1]\n"
                "9: jump 40 [line 1]\n"
                "10: assign ^2 i [line 1]\n"
                "11: add i i 1 [line 1]\n"
                "12: jump 3 [line 1]\n"
                "13: assign i 0 [line 2]\n"
                "14: assign ^3 i [line 2]\n"
                "15: if_less i 20 17 [line 2]\n"
                "16: jump 19 [line 2]\n"
                "17: assign ^4 'true' [line 2]\n"
                "18: jump 20 [line 2]\n"
                "19: assign ^4 'false' [line 2]\n"
                "20: if_eq ^4 'true' 25 [line 2]\n"
                "21: jump 39 [line 2]\n"
                "22: assign ^5 i [line 2]\n"
                "23: add i i 1 [line 2]\n"
                "24: jump 15 [line 2]\n"
                "25: assign i 0 [line 2]\n"
                "26: assign ^6 i [line 2]\n"
                "27: if_less i 20 29 [line 2]\n"
                "28: jump 31 [line 2]\n"
                "29: assign ^7 'true' [line 2]\n"
                "30: jump 32 [line 2]\n"
                "31: assign ^7 'false' [line 2]\n"
                "32: if_eq ^7 'true' 37 [line 2]\n"
                "33: jump 38 [line 2]\n"
                "34: assign ^8 i [line 2]\n"
                "35: add i i 1 [line 2]\n"
                "36: jump 27 [line 2]\n"
                "37: jump 34 [line 2]\n"
                "38: jump 22 [line 2]\n"
                "39: jump 10 [line 3]\n"
                "40: assign i 0 [line 6]\n"
                "41: assign ^9 i [line 6]\n"
                "42: if_less i 20 44 [line 6]\n"
                "43: jump 46 [line 6]\n"
                "44: assign ^10 'true' [line 6]\n"
                "45: jump 47 [line 6]\n"
                "46: assign ^10 'false' [line 6]\n"
                "47: if_eq ^10 'true' 52 [line 6]\n"
                "48: jump 53 [line 6]\n"
                "49: assign ^11 i [line 6]\n"
                "50: add i i 1 [line 6]\n"
                "51: jump 42 [line 6]\n"
                "52: jump 49 [line 6]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/loop/for_many_nested.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(InterCodeSuite, loop_for_continue_single)
{
    expected = "1: if_less i 0 3 [line 1]\n"
               "2: jump 5 [line 1]\n"
               "3: assign ^0 'true' [line 1]\n"
               "4: jump 6 [line 1]\n"
               "5: assign ^0 'false' [line 1]\n"
               "6: if_eq ^0 'true' 11 [line 1]\n"
               "7: jump 13 [line 1]\n"
               "8: assign ^1 i [line 1]\n"
               "9: add i i 1 [line 1]\n"
               "10: jump 1 [line 1]\n"
               "11: jump 8 [line 2]\n"
               "12: jump 8 [line 3]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/loop/for_continue_single.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(InterCodeSuite, loop_for_continue_many_nested)
{
    expected =  "1: if_less i 0 3 [line 1]\n"
                "2: jump 5 [line 1]\n"
                "3: assign ^0 'true' [line 1]\n"
                "4: jump 6 [line 1]\n"
                "5: assign ^0 'false' [line 1]\n"
                "6: if_eq ^0 'true' 11 [line 1]\n"
                "7: jump 34 [line 1]\n"
                "8: assign ^1 i [line 1]\n"
                "9: add i i 1 [line 1]\n"
                "10: jump 1 [line 1]\n"
                "11: jump 8 [line 2]\n"
                "12: jump 8 [line 2]\n"
                "13: jump 8 [line 3]\n"
                "14: jump 8 [line 3]\n"
                "15: jump 8 [line 3]\n"
                "16: jump 8 [line 3]\n"
                "17: jump 8 [line 3]\n"
                "18: jump 8 [line 3]\n"
                "19: if_less i 0 21 [line 4]\n"
                "20: jump 23 [line 4]\n"
                "21: assign ^2 'true' [line 4]\n"
                "22: jump 24 [line 4]\n"
                "23: assign ^2 'false' [line 4]\n"
                "24: if_eq ^2 'true' 29 [line 4]\n"
                "25: jump 33 [line 4]\n"
                "26: assign ^3 i [line 4]\n"
                "27: add i i 1 [line 4]\n"
                "28: jump 19 [line 4]\n"
                "29: jump 26 [line 4]\n"
                "30: jump 26 [line 4]\n"
                "31: jump 26 [line 4]\n"
                "32: jump 26 [line 4]\n"
                "33: jump 8 [line 5]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/loop/for_continue_many_nested.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(InterCodeSuite, loop_for_break_single)
{
    expected = "1: if_less i 8 3 [line 1]\n"
               "2: jump 5 [line 1]\n"
               "3: assign ^0 'true' [line 1]\n"
               "4: jump 6 [line 1]\n"
               "5: assign ^0 'false' [line 1]\n"
               "6: if_eq ^0 'true' 11 [line 1]\n"
               "7: jump 13 [line 1]\n"
               "8: assign ^1 i [line 1]\n"
               "9: add i i 1 [line 1]\n"
               "10: jump 1 [line 1]\n"
               "11: jump 13 [line 2]\n"
               "12: jump 8 [line 3]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/loop/for_break_single.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(InterCodeSuite, loop_for_break_many_nested)
{
    expected =  "1: if_less i 8 3 [line 1]\n"
                "2: jump 5 [line 1]\n"
                "3: assign ^0 'true' [line 1]\n"
                "4: jump 6 [line 1]\n"
                "5: assign ^0 'false' [line 1]\n"
                "6: if_eq ^0 'true' 11 [line 1]\n"
                "7: jump 33 [line 1]\n"
                "8: assign ^1 i [line 1]\n"
                "9: add i i 1 [line 1]\n"
                "10: jump 1 [line 1]\n"
                "11: jump 33 [line 2]\n"
                "12: jump 33 [line 3]\n"
                "13: jump 33 [line 3]\n"
                "14: jump 33 [line 3]\n"
                "15: jump 33 [line 3]\n"
                "16: jump 33 [line 3]\n"
                "17: jump 33 [line 3]\n"
                "18: jump 33 [line 3]\n"
                "19: if_greater j 0 21 [line 4]\n"
                "20: jump 23 [line 4]\n"
                "21: assign ^2 'true' [line 4]\n"
                "22: jump 24 [line 4]\n"
                "23: assign ^2 'false' [line 4]\n"
                "24: if_eq ^2 'true' 29 [line 4]\n"
                "25: jump 32 [line 4]\n"
                "26: assign ^3 j [line 4]\n"
                "27: add j j 1 [line 4]\n"
                "28: jump 19 [line 4]\n"
                "29: jump 32 [line 4]\n"
                "30: jump 32 [line 4]\n"
                "31: jump 26 [line 4]\n"
                "32: jump 8 [line 5]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/loop/for_break_many_nested.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

/* ------------- Conditional ------------- */

TEST_F(InterCodeSuite, if_simple)
{
    expected = "1: if_eq x 'true' 3 [line 1]\n"
               "2: jump 3 [line 1]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/conditional/if_simple.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(InterCodeSuite, if_expr)
{
    expected = "1: if_greater x 3 3 [line 1]\n"
               "2: jump 5 [line 1]\n"
               "3: assign ^0 'true' [line 1]\n"
               "4: jump 6 [line 1]\n"
               "5: assign ^0 'false' [line 1]\n"
               "6: if_eq ^0 'true' 8 [line 1]\n"
               "7: jump 8 [line 1]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/conditional/if_expr.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(InterCodeSuite, if_stmts)
{
    expected = "1: if_less 3 x 3 [line 1]\n"
               "2: jump 5 [line 1]\n"
               "3: assign ^0 'true' [line 1]\n"
               "4: jump 6 [line 1]\n"
               "5: assign ^0 'false' [line 1]\n"
               "6: if_eq ^0 'true' 8 [line 1]\n"
               "7: jump 10 [line 1]\n"
               "8: assign a f [line 5]\n"
               "9: assign ^1 a [line 5]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/conditional/if_stmts.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(InterCodeSuite, if_nested_many)
{
    expected = "1: if_eq a 'true' 3 [line 1]\n"
               "2: jump 11 [line 1]\n"
               "3: if_eq sz 'true' 5 [line 2]\n"
               "4: jump 11 [line 2]\n"
               "5: if_eq z 'true' 7 [line 3]\n"
               "6: jump 9 [line 3]\n"
               "7: assign s 9 [line 4]\n"
               "8: assign ^0 s [line 4]\n"
               "9: assign k 0 [line 7]\n"
               "10: assign ^0 k [line 7]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/conditional/if_nested_many.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(InterCodeSuite, if_else)
{
    expected = "1: if_eq a 'true' 3 [line 1]\n"
               "2: jump 4 [line 1]\n"
               "3: jump 8 [line 2]\n"
               "4: assign b 2 [line 2]\n"
               "5: assign ^0 b [line 2]\n"
               "6: assign a x [line 2]\n"
               "7: assign ^0 a [line 2]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/conditional/if_else.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(InterCodeSuite, if_elseif)
{
    expected = "1: if_eq 1 'true' 3 [line 1]\n"
               "2: jump 6 [line 1]\n"
               "3: assign x 2 [line 1]\n"
               "4: assign ^0 x [line 1]\n"
               "5: jump 10 [line 2]\n"
               "6: if_eq 1 'true' 8 [line 2]\n"
               "7: jump 10 [line 2]\n"
               "8: assign x 3 [line 2]\n"
               "9: assign ^0 x [line 2]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/conditional/if_elseif.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(InterCodeSuite, if_elseif_else)
{
    expected = "1: if_eq 1 'true' 3 [line 1]\n"
               "2: jump 6 [line 1]\n"
               "3: assign x 2 [line 1]\n"
               "4: assign ^0 x [line 1]\n"
               "5: jump 28 [line 2]\n"
               "6: if_eq 1 'true' 8 [line 2]\n"
               "7: jump 11 [line 2]\n"
               "8: assign x 3 [line 2]\n"
               "9: assign ^0 x [line 2]\n"
               "10: jump 28 [line 3]\n"
               "11: if_eq 1 'true' 13 [line 3]\n"
               "12: jump 16 [line 3]\n"
               "13: assign x 3 [line 3]\n"
               "14: assign ^0 x [line 3]\n"
               "15: jump 28 [line 4]\n"
               "16: if_eq 1 'true' 18 [line 4]\n"
               "17: jump 21 [line 4]\n"
               "18: assign x 3 [line 4]\n"
               "19: assign ^0 x [line 4]\n"
               "20: jump 28 [line 5]\n"
               "21: if_eq 1 'true' 23 [line 5]\n"
               "22: jump 26 [line 5]\n"
               "23: assign x 3 [line 5]\n"
               "24: assign ^0 x [line 5]\n"
               "25: jump 28 [line 6]\n"
               "26: assign x 3 [line 6]\n"
               "27: assign ^0 x [line 6]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/conditional/if_elseif_else.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(InterCodeSuite, if_else_many_nested)
{
    expected =  "1: if_eq 1 'true' 3 [line 1]\n"
                "2: jump 16 [line 1]\n"
                "3: if_eq 2 'true' 5 [line 1]\n"
                "4: jump 13 [line 1]\n"
                "5: if_eq a 0 7 [line 1]\n"
                "6: jump 9 [line 1]\n"
                "7: assign ^0 'true' [line 1]\n"
                "8: jump 10 [line 1]\n"
                "9: assign ^0 'false' [line 1]\n"
                "10: if_eq ^0 'true' 12 [line 1]\n"
                "11: jump 13 [line 1]\n"
                "12: jump 13 [line 1]\n"
                "13: assign x 2 [line 1]\n"
                "14: assign ^1 x [line 1]\n"
                "15: jump 27 [line 2]\n"
                "16: if_greater x 0 18 [line 3]\n"
                "17: jump 20 [line 3]\n"
                "18: assign ^0 'true' [line 3]\n"
                "19: jump 21 [line 3]\n"
                "20: assign ^0 'false' [line 3]\n"
                "21: if_eq ^0 'true' 23 [line 3]\n"
                "22: jump 24 [line 3]\n"
                "23: jump 27 [line 4]\n"
                "24: if_eq x 'true' 26 [line 4]\n"
                "25: jump 27 [line 4]\n"
                "26: jump 27 [line 5]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/conditional/if_else_many_nested.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

/* ------------ Arithmetic -------------- */

TEST_F(InterCodeSuite, arithmetic_simple)
{
    expected = "1: add ^0 x 9 [line 1]\n"
               "2: sub ^0 a 0 [line 2]\n"
               "3: mul ^0 8 o [line 3]\n"
               "4: mod ^0 3 6 [line 4]\n"
               "5: div ^0 1 1 [line 5]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/arithmetic/simple.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(InterCodeSuite, arithmetic_spaghetti)
{
    expected = "1: add ^0 x 9 [line 1]\n"
               "2: add ^1 ^0 8 [line 1]\n"
               "3: add ^2 ^1 9 [line 1]\n"
               "4: add ^3 ^2 u [line 1]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/arithmetic/spaghetti.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(InterCodeSuite, arithmetic_errors)
{
    expected = "\x1B[31mError, in line: 4:\x1B[0m Invalid use of arithmetic operator on user function f\n"
               "\x1B[31mError, in line: 6:\x1B[0m Invalid use of arithmetic operator on const bool \'true\'\n"
               "\x1B[31mError, in line: 6:\x1B[0m Invalid use of arithmetic operator on const bool \'false\'\n"
               "\x1B[31mError, in line: 8:\x1B[0m Invalid use of arithmetic operator on const string \"a\"\n"
               "\x1B[31mError, in line: 10:\x1B[0m Invalid use of arithmetic operator on library function print\n";

    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/arithmetic/errors.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(InterCodeSuite, arithmetic_plusplus_suffix)
{
    expected = "1: assign ^0 i [line 1]\n"
               "2: add i i 1 [line 1]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/arithmetic/plusplus_suffix.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(InterCodeSuite, arithmetic_plusplus_prefix)
{
    expected = "1: add i i 1 [line 1]\n"
               "2: assign ^0 i [line 1]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/arithmetic/plusplus_prefix.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(InterCodeSuite, arithmetic_uminus)
{
    expected = "1: uminus ^0 1 [line 1]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/arithmetic/uminus.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(InterCodeSuite, arithmetic_uminus_bool) {
    expected = "\x1B[31mError, in line: 1:\x1B[0m Invalid use of arithmetic operator on const bool 'true'\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/arithmetic/uminus_bool.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(InterCodeSuite, arithmetic_not)
{
    expected = "1: if_eq 1 'true' 5 [line 1]\n"
               "2: jump 3 [line 1]\n"
               "3: assign ^0 'true' [line 1]\n"
               "4: jump 6 [line 1]\n"
               "5: assign ^0 'false' [line 1]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/arithmetic/not.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(InterCodeSuite, arithmetic_minusminus_prefix)
{
    expected = "1: sub a a 1 [line 1]\n"
               "2: assign ^0 a [line 1]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/arithmetic/minusminus_prefix.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(InterCodeSuite, arithmetic_minusminus_suffix)
{
    expected = "1: assign ^0 a [line 1]\n"
               "2: sub a a 1 [line 1]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/arithmetic/minusminus_suffix.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

/* ------------ Table -------------- */

TEST_F(InterCodeSuite, table_tablemake_list_single)
{
    expected = "1: tablecreate ^0 [line 1]\n"
               "2: tablesetelem ^0 0 1 [line 1]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/table/tablemake_list_single.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(InterCodeSuite, table_tablemake_list_many)
{
    expected = "1: call f [line 1]\n"
               "2: getretval ^0 [line 1]\n"
               "3: tablecreate ^1 [line 1]\n"
               "4: tablesetelem ^1 0 1 [line 1]\n"
               "5: tablesetelem ^1 1 3 [line 1]\n"
               "6: tablesetelem ^1 2 s [line 1]\n"
               "7: tablesetelem ^1 3 0 [line 1]\n"
               "8: tablesetelem ^1 4 ^0 [line 1]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/table/tablemake_list_many.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(InterCodeSuite, table_tablemake_pairs_single)
{
    expected = "1: tablecreate ^0 [line 1]\n"
               "2: tablesetelem ^0 x 0 [line 1]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/table/tablemake_pairs_single.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(InterCodeSuite, table_tablemake_pairs_many)
{
    expected = "1: tablecreate ^0 [line 1]\n"
               "2: tablesetelem ^0 x 0 [line 1]\n"
               "3: tablesetelem ^0 x 0 [line 1]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/table/tablemake_pairs_many.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(InterCodeSuite, table_tablemake_pairs_macaroni)
{
    expected = "1: assign pi 3.14159 [line 1]\n"
               "2: assign ^0 pi [line 1]\n"
               "3: tablecreate ^1 [line 1]\n"
               "4: tablesetelem ^1 \"pi\" ^0 [line 1]\n"
               "5: tablesetelem ^1 \"r\" 0 [line 1]\n"
               "6: tablesetelem ^1 \"x\" 0 [line 1]\n"
               "7: tablesetelem ^1 \"y\" 0 [line 1]\n"
               "8: assign circle ^1 [line 1]\n"
               "9: assign ^2 circle [line 1]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/table/tablemake_pairs_macaroni.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(InterCodeSuite, table_tablemake_nested)
{
    expected = "1: tablecreate ^0 [line 1]\n"
               "2: tablesetelem ^0 0 1 [line 1]\n"
               "3: tablesetelem ^0 1 2 [line 1]\n"
               "4: tablesetelem ^0 2 3 [line 1]\n"
               "5: tablecreate ^1 [line 1]\n"
               "6: tablesetelem ^1 1 0 [line 1]\n"
               "7: tablecreate ^2 [line 1]\n"
               "8: tablesetelem ^2 0 ^1 [line 1]\n"
               "9: tablesetelem ^2 1 0 [line 1]\n"
               "10: assign s 2 [line 1]\n"
               "11: assign ^3 s [line 1]\n"
               "12: tablecreate ^4 [line 1]\n"
               "13: tablesetelem ^4 ^3 9 [line 1]\n"
               "14: tablecreate ^5 [line 1]\n"
               "15: tablesetelem ^5 0 \"s\" [line 1]\n"
               "16: tablesetelem ^5 1 ^4 [line 1]\n"
               "17: tablecreate ^6 [line 1]\n"
               "18: tablesetelem ^6 0 7 [line 1]\n"
               "19: tablecreate ^7 [line 1]\n"
               "20: tablesetelem ^7 0 ^0 [line 1]\n"
               "21: tablesetelem ^7 1 ^2 [line 1]\n"
               "22: tablesetelem ^7 2 ^5 [line 1]\n"
               "23: tablesetelem ^7 3 ^6 [line 1]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/table/tablemake_nested.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(InterCodeSuite, table_tablemember_item_simple) {
    expected = "1: tablegetelem ^0 a \"o\" [line 1]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/table/tablemember_item_simple.asc");
    GTEST_ASSERT_EQ(expected, actual); 
}

TEST_F(InterCodeSuite, table_tableelem_item_seq) {
    expected =  "1: tablegetelem ^0 a \"b\" [line 1]\n"
                "2: tablegetelem ^1 ^0 \"o\" [line 1]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/table/tableelem_item_seq.asc");
    GTEST_ASSERT_EQ(expected, actual); 
}        

TEST_F(InterCodeSuite, table_tableelem_ind_simp) {
    expected = "1: tablegetelem ^0 a 1 [line 1]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/table/tableelem_ind_simp.asc");
    GTEST_ASSERT_EQ(expected, actual); 
}

TEST_F(InterCodeSuite, table_tablelem_ind_manyexprs) {
    expected =  "1: tablegetelem ^0 a 1 [line 1]\n"
                "2: tablegetelem ^0 a s [line 2]\n"
                "3: assign a 0 [line 3]\n"
                "4: assign ^0 a [line 3]\n"
                "5: tablegetelem ^1 a ^0 [line 3]\n"
                "6: param c [line 4]\n"
                "7: call y [line 4]\n"
                "8: getretval ^0 [line 4]\n"
                "9: param ^0 [line 4]\n"
                "10: call f [line 4]\n"
                "11: getretval ^1 [line 4]\n"
                "12: mul ^2 9 ^1 [line 4]\n"
                "13: add ^3 s ^2 [line 4]\n"
                "14: tablegetelem ^4 a ^3 [line 4]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/table/tableelem_ind_manyexprs.asc");
    GTEST_ASSERT_EQ(expected, actual);            
}

TEST_F(InterCodeSuite, table_tableelem_it_call) {
    expected =  "1: call f [line 1]\n"
                "2: getretval ^0 [line 1]\n"
                "3: tablegetelem ^1 ^0 \"a\" [line 1]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/table/tableelem_it_call.asc");
    GTEST_ASSERT_EQ(expected, actual);            
}

TEST_F(InterCodeSuite, table_tebleelem_ind_call) {
    expected =  "1: call f [line 1]\n"
                "2: getretval ^0 [line 1]\n"
                "3: tablegetelem ^1 ^0 2 [line 1]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/table/tableelem_ind_call.asc");
    GTEST_ASSERT_EQ(expected, actual);              
}

TEST_F(InterCodeSuite, table_emit_iftableitem_plusplusprefix) {
    expected =  "1: tablegetelem ^1 a 2 [line 1]\n"
                "2: add ^1 ^1 1 [line 1]\n"
                "3: tablesetelem a 2 ^1 [line 1]\n";
    actual =  exec("./d_intermediate_code ../../test/files/phase3_tests/table/emit_iftableitem_plusplusprefix.asc");     
    GTEST_ASSERT_EQ(expected, actual);     
}

TEST_F(InterCodeSuite, table_emit_iftableitem_plusplussufix) {
    expected =  "1: tablegetelem ^1 a 2 [line 1]\n"
                "2: assign ^0 ^1 [line 1]\n"
                "3: add ^1 ^1 1 [line 1]\n"
                "4: tablesetelem a 2 ^1 [line 1]\n";
    actual =  exec("./d_intermediate_code ../../test/files/phase3_tests/table/emit_iftableitem_plusplussufix.asc");     
    GTEST_ASSERT_EQ(expected, actual);            
}

TEST_F(InterCodeSuite, table_emit_iftableitem_minusminus_prefix) {
    expected =  "1: tablegetelem ^1 a 2 [line 1]\n"
                "2: sub ^1 ^1 1 [line 1]\n"
                "3: tablesetelem a 2 ^1 [line 1]\n";
    actual =  exec("./d_intermediate_code ../../test/files/phase3_tests/table/emit_iftableitem_minusminus_prefix.asc");     
    GTEST_ASSERT_EQ(expected, actual);            
}

TEST_F(InterCodeSuite, table_emit_iftableitem_minusminus_suffix) {
    expected =  "1: tablegetelem ^1 a 2 [line 1]\n"
                "2: assign ^0 ^1 [line 1]\n"
                "3: sub ^1 ^1 1 [line 1]\n"
                "4: tablesetelem a 2 ^1 [line 1]\n";
    actual =  exec("./d_intermediate_code ../../test/files/phase3_tests/table/emit_iftableitem_minusminus_suffix.asc");     
    GTEST_ASSERT_EQ(expected, actual);            
}

TEST_F(InterCodeSuite, table_emit_iftableitem_assignexpr) {
    expected =  "1: tablesetelem a 2 9 [line 1]\n"
                "2: tablegetelem ^0 a 2 [line 1]\n";
    actual =  exec("./d_intermediate_code ../../test/files/phase3_tests/table/emit_iftableitem_assignexpr.asc");     
    GTEST_ASSERT_EQ(expected, actual);            
}

TEST_F(InterCodeSuite, table_emit_iftableitem_call) {
    expected =  "1: tablegetelem ^0 a 3 [line 1]\n"
                "2: param s [line 1]\n"
                "3: call ^0 [line 1]\n"
                "4: getretval ^1 [line 1]\n";
    actual =  exec("./d_intermediate_code ../../test/files/phase3_tests/table/emit_iftableitem_call.asc");     
    GTEST_ASSERT_EQ(expected, actual);              
}

//----------------------- short circuit ---------------------------------------------------------------------------------

TEST_F(InterCodeSuite, short_circuit_simple_or)
{
    expected = "1: if_eq a 'true' 9 [line 1]\n"
               "2: jump 3 [line 1]\n"
               "3: if_eq b 'true' 9 [line 1]\n"
               "4: jump 5 [line 1]\n"
               "5: if_eq c 'true' 7 [line 1]\n"
               "6: jump 11 [line 1]\n"
               "7: if_eq f 'true' 11 [line 1]\n"
               "8: jump 9 [line 1]\n"
               "9: assign ^0 'true' [line 1]\n"
               "10: jump 12 [line 1]\n"
               "11: assign ^0 'false' [line 1]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/short_circuit/simple_or.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

//-----------------------assistant tests---------------------------------------------------------------------------------

TEST_F(InterCodeSuite, backpatch_backpatch0)
{
    expected = "1: if_eq a 'true' 3 [line 1]\n"
               "2: jump 7 [line 1]\n"
               "3: if_eq 'true' 'true' 7 [line 1]\n"
               "4: jump 5 [line 1]\n"
               "5: assign ^0 'true' [line 1]\n"
               "6: jump 8 [line 1]\n"
               "7: assign ^0 'false' [line 1]\n"
               "8: assign x ^0 [line 1]\n"
               "9: assign ^1 x [line 1]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/backpatch/backpatch0.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(InterCodeSuite, backpatch_backpatch1)
{
    expected = "1: if_eq a 'true' 7 [line 1]\n"
               "2: jump 3 [line 1]\n"
               "3: if_eq b 'true' 5 [line 1]\n"
               "4: jump 9 [line 1]\n"
               "5: if_eq c 'true' 7 [line 1]\n"
               "6: jump 9 [line 1]\n"
               "7: assign ^0 'true' [line 1]\n"
               "8: jump 10 [line 1]\n"
               "9: assign ^0 'false' [line 1]\n"
               "10: if_eq ^0 'true' 12 [line 1]\n"
               "11: jump 16 [line 1]\n"
               "12: param \"...\" [line 2]\n"
               "13: call print [line 2]\n"
               "14: getretval ^1 [line 2]\n"
               "15: jump 1 [line 2]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/backpatch/backpatch1.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(InterCodeSuite, backpatch_backpatch2)
{
    expected = "1: if_eq a 'true' 5 [line 1]\n"
               "2: jump 3 [line 1]\n"
               "3: if_less b 2 5 [line 1]\n"
               "4: jump 7 [line 1]\n"
               "5: assign ^0 'true' [line 1]\n"
               "6: jump 8 [line 1]\n"
               "7: assign ^0 'false' [line 1]\n"
               "8: if_eq ^0 'true' 10 [line 1]\n"
               "9: jump 13 [line 1]\n"
               "10: param \"?\" [line 2]\n"
               "11: call print [line 2]\n"
               "12: getretval ^1 [line 2]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/backpatch/backpatch2.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(InterCodeSuite, backpatch_backpatch3)
{
    expected = "\x1B[33mWarning, in line: 8:\x1B[0m Too many arguments passed to function: $0, defined in line: 8\n"
               "1: assign f 1 [line 1]\n"
               "2: assign ^0 f [line 1]\n"
               "3: assign e ^0 [line 1]\n"
               "4: assign ^1 e [line 1]\n"
               "5: assign d ^1 [line 1]\n"
               "6: assign ^2 d [line 1]\n"
               "7: assign c ^2 [line 1]\n"
               "8: assign ^3 c [line 1]\n"
               "9: assign b ^3 [line 1]\n"
               "10: assign ^4 b [line 1]\n"
               "11: assign a ^4 [line 1]\n"
               "12: assign ^5 a [line 1]\n"
               "13: if_less a b 19 [line 3]\n"
               "14: jump 15 [line 3]\n"
               "15: if_greater c d 17 [line 3]\n"
               "16: jump 21 [line 3]\n"
               "17: if_less e f 19 [line 3]\n"
               "18: jump 21 [line 3]\n"
               "19: assign ^0 'true' [line 3]\n"
               "20: jump 22 [line 3]\n"
               "21: assign ^0 'false' [line 3]\n"
               "22: assign x ^0 [line 3]\n"
               "23: assign ^1 x [line 3]\n"
               "24: if_eq x 'true' 28 [line 5]\n"
               "25: jump 26 [line 5]\n"
               "26: if_eq x 'true' 30 [line 5]\n"
               "27: jump 28 [line 5]\n"
               "28: if_eq a 'true' 30 [line 5]\n"
               "29: jump 32 [line 5]\n"
               "30: assign ^0 'true' [line 5]\n"
               "31: jump 33 [line 5]\n"
               "32: assign ^0 'false' [line 5]\n"
               "33: assign y ^0 [line 5]\n"
               "34: assign ^1 y [line 5]\n"
               "35: if_eq x 'true' 37 [line 8]\n"
               "36: jump 47 [line 8]\n"
               "37: jump 42 [line 8]\n"
               "38: funcstart $0 [line 8]\n"
               "39: return 'false' [line 8]\n"
               "40: jump 41 [line 8]\n"
               "41: funcend $0 [line 8]\n"
               "42: param 2 [line 8]\n"
               "43: call $0 [line 8]\n"
               "44: getretval ^0 [line 8]\n"
               "45: if_eq ^0 'true' 54 [line 8]\n"
               "46: jump 47 [line 8]\n"
               "47: jump 52 [line 8]\n"
               "48: funcstart $1 [line 8]\n"
               "49: return 'true' [line 8]\n"
               "50: jump 51 [line 8]\n"
               "51: funcend $1 [line 8]\n"
               "52: if_eq $1 'true' 54 [line 8]\n"
               "53: jump 56 [line 8]\n"
               "54: assign ^1 'true' [line 8]\n"
               "55: jump 57 [line 8]\n"
               "56: assign ^1 'false' [line 8]\n"
               "57: assign x ^1 [line 8]\n"
               "58: assign ^2 x [line 8]\n"
               "59: if_eq a 'true' 67 [line 10]\n"
               "60: jump 61 [line 10]\n"
               "61: if_eq b 'true' 67 [line 10]\n"
               "62: jump 63 [line 10]\n"
               "63: assign y x [line 10]\n"
               "64: assign ^0 y [line 10]\n"
               "65: if_eq ^0 'true' 67 [line 10]\n"
               "66: jump 69 [line 10]\n"
               "67: assign ^1 'true' [line 10]\n"
               "68: jump 70 [line 10]\n"
               "69: assign ^1 'false' [line 10]\n"
               "70: if_eq ^1 'true' 72 [line 10]\n"
               "71: jump 97 [line 10]\n"
               "72: assign i 0 [line 11]\n"
               "73: assign ^2 i [line 11]\n"
               "74: if_less i 5 76 [line 11]\n"
               "75: jump 80 [line 11]\n"
               "76: if_eq a 'true' 78 [line 11]\n"
               "77: jump 80 [line 11]\n"
               "78: assign ^3 'true' [line 11]\n"
               "79: jump 81 [line 11]\n"
               "80: assign ^3 'false' [line 11]\n"
               "81: if_eq ^3 'true' 86 [line 11]\n"
               "82: jump 93 [line 11]\n"
               "83: add i i 1 [line 11]\n"
               "84: assign ^4 i [line 11]\n"
               "85: jump 74 [line 11]\n"
               "86: param i [line 12]\n"
               "87: call print [line 12]\n"
               "88: getretval ^5 [line 12]\n"
               "89: jump 83 [line 13]\n"
               "90: add i i 1 [line 14]\n"
               "91: assign ^0 i [line 14]\n"
               "92: jump 83 [line 15]\n"
               "93: param y [line 16]\n"
               "94: call print [line 16]\n"
               "95: getretval ^0 [line 16]\n"
               "96: jump 114 [line 18]\n"
               "97: if_eq y 'true' 101 [line 19]\n"
               "98: jump 99 [line 19]\n"
               "99: assign ^0 'true' [line 19]\n"
               "100: jump 102 [line 19]\n"
               "101: assign ^0 'false' [line 19]\n"
               "102: if_eq ^0 'true' 104 [line 19]\n"
               "103: jump 114 [line 19]\n"
               "104: if_eq y 'true' 108 [line 20]\n"
               "105: jump 106 [line 20]\n"
               "106: assign ^1 'true' [line 20]\n"
               "107: jump 109 [line 20]\n"
               "108: assign ^1 'false' [line 20]\n"
               "109: param ^1 [line 20]\n"
               "110: call print [line 20]\n"
               "111: getretval ^2 [line 20]\n"
               "112: jump 114 [line 21]\n"
               "113: jump 97 [line 22]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/backpatch/backpatch3.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(InterCodeSuite, backpatch_p3t_assignments_complex)
{
    expected =  "1: assign a 3 [line 1]\n"
                "2: assign ^0 a [line 1]\n"
                "3: assign b 6 [line 2]\n"
                "4: assign ^0 b [line 2]\n"
                "5: assign c 9 [line 3]\n"
                "6: assign ^0 c [line 3]\n"
                "7: assign d 54 [line 4]\n"
                "8: assign ^0 d [line 4]\n"
                "9: assign c d [line 6]\n"
                "10: assign ^0 c [line 6]\n"
                "11: assign b ^0 [line 6]\n"
                "12: assign ^1 b [line 6]\n"
                "13: assign a ^1 [line 6]\n"
                "14: assign ^2 a [line 6]\n"
                "15: add ^0 c d [line 8]\n"
                "16: uminus ^1 ^0 [line 8]\n"
                "17: sub ^2 a b [line 8]\n"
                "18: mul ^3 ^0 ^2 [line 8]\n"
                "19: uminus ^4 ^3 [line 8]\n"
                "20: uminus ^5 b [line 8]\n"
                "21: mod ^6 ^3 b [line 8]\n"
                "22: assign b ^6 [line 8]\n"
                "23: assign ^7 b [line 8]\n"
                "24: assign a ^7 [line 8]\n"
                "25: assign ^8 a [line 8]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/backpatch/p3t_assignments_complex.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(InterCodeSuite, backpatch_p3t_assignments_objects)
{
    expected =  "1: tablesetelem a 3 3 [line 1]\n"
                "2: tablegetelem ^0 a 3 [line 1]\n"
                "3: tablegetelem ^0 a 3 [line 2]\n"
                "4: tablegetelem ^1 a 3 [line 2]\n"
                "5: tablesetelem ^0 4 ^1 [line 2]\n"
                "6: tablegetelem ^2 ^0 4 [line 2]\n"
                "7: tablegetelem ^0 a 3 [line 3]\n"
                "8: tablegetelem ^1 ^0 4 [line 3]\n"
                "9: tablegetelem ^2 ^1 4 [line 3]\n"
                "10: tablegetelem ^3 ^2 4 [line 3]\n"
                "11: tablesetelem a \"r\" ^3 [line 3]\n"
                "12: tablegetelem ^4 a \"r\" [line 3]\n"
                "13: tablegetelem ^0 a \"r\" [line 4]\n"
                "14: tablesetelem a \"r\" ^0 [line 4]\n"
                "15: tablegetelem ^1 a \"r\" [line 4]\n"
                "16: tablegetelem ^0 a \"r\" [line 5]\n"
                "17: tablegetelem ^1 a \"r\" [line 5]\n"
                "18: tablegetelem ^2 ^1 \"q\" [line 5]\n"
                "19: tablesetelem ^0 \"q\" ^2 [line 5]\n"
                "20: tablegetelem ^3 ^0 \"q\" [line 5]\n"
                "21: tablegetelem ^0 a \"r\" [line 6]\n"
                "22: tablegetelem ^1 ^0 \"q\" [line 6]\n"
                "23: tablegetelem ^2 ^1 \"r\" [line 6]\n"
                "24: tablegetelem ^3 ^2 \"s\" [line 6]\n"
                "25: tablesetelem ^3 3 12 [line 6]\n"
                "26: tablegetelem ^4 ^3 3 [line 6]\n"
                "27: tablegetelem ^0 a \"3\" [line 7]\n"
                "28: tablegetelem ^1 ^0 \"q\" [line 7]\n"
                "29: tablegetelem ^2 ^1 4 [line 7]\n"
                "30: call ^2 [line 7]\n"
                "31: getretval ^3 [line 7]\n"
                "32: tablesetelem a \"r\" 5 [line 8]\n"
                "33: tablegetelem ^0 a \"r\" [line 8]\n"
                "34: tablesetelem a 1 ^0 [line 8]\n"
                "35: tablegetelem ^1 a 1 [line 8]\n"
                "36: tablesetelem a \"x\" ^1 [line 8]\n"
                "37: tablegetelem ^2 a \"x\" [line 8]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/backpatch/p3t_assignments_objects.asc");
    GTEST_ASSERT_EQ(expected, actual);   
}

TEST_F(InterCodeSuite, backpatch_p3t_assignments_simple)
{
    expected =  "1: assign a 4 [line 2]\n"
                "2: assign ^0 a [line 2]\n"
                "3: add ^0 a 5 [line 3]\n"
                "4: assign a ^0 [line 3]\n"
                "5: assign ^1 a [line 3]\n"
                "6: assign b a [line 4]\n"
                "7: assign ^0 b [line 4]\n"
                "8: add ^0 a b [line 5]\n"
                "9: assign a ^0 [line 5]\n"
                "10: assign ^1 a [line 5]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/backpatch/p3t_assignments_simple.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(InterCodeSuite, backpatch_p3t_var_maths)
{
    expected =  "1: add ^0 x y [line 1]\n"
                "2: mul ^0 z w [line 2]\n"
                "3: sub ^0 t s [line 3]\n"
                "4: div ^0 u v [line 4]\n"
                "5: mod ^0 r l [line 5]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/backpatch/p3t_var_maths.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(InterCodeSuite, backpatch_p3t_relational)
{
    expected =  "1: assign l 0 [line 1]\n"
                "2: assign ^0 l [line 1]\n"
                "3: assign k ^0 [line 1]\n"
                "4: assign ^1 k [line 1]\n"
                "5: assign j ^1 [line 1]\n"
                "6: assign ^2 j [line 1]\n"
                "7: assign i ^2 [line 1]\n"
                "8: assign ^3 i [line 1]\n"
                "9: assign h ^3 [line 1]\n"
                "10: assign ^4 h [line 1]\n"
                "11: assign g ^4 [line 1]\n"
                "12: assign ^5 g [line 1]\n"
                "13: assign f ^5 [line 1]\n"
                "14: assign ^6 f [line 1]\n"
                "15: assign e ^6 [line 1]\n"
                "16: assign ^7 e [line 1]\n"
                "17: assign d ^7 [line 1]\n"
                "18: assign ^8 d [line 1]\n"
                "19: assign c ^8 [line 1]\n"
                "20: assign ^9 c [line 1]\n"
                "21: assign b ^9 [line 1]\n"
                "22: assign ^10 b [line 1]\n"
                "23: assign a ^10 [line 1]\n"
                "24: assign ^11 a [line 1]\n"
                "25: if_greater a b 47 [line 3]\n"
                "26: jump 27 [line 3]\n"
                "27: if_less c d 29 [line 3]\n"
                "28: jump 31 [line 3]\n"
                "29: if_greatereq e f 47 [line 3]\n"
                "30: jump 31 [line 3]\n"
                "31: if_lesseq g h 33 [line 3]\n"
                "32: jump 40 [line 3]\n"
                "33: if_eq i 'true' 37 [line 3]\n"
                "34: jump 35 [line 3]\n"
                "35: assign ^0 'true' [line 3]\n"
                "36: jump 38 [line 3]\n"
                "37: assign ^0 'false' [line 3]\n"
                "38: if_eq ^0 j 47 [line 3]\n"
                "39: jump 40 [line 3]\n"
                "40: if_eq k 'true' 44 [line 3]\n"
                "41: jump 42 [line 3]\n"
                "42: assign ^1 'true' [line 3]\n"
                "43: jump 45 [line 3]\n"
                "44: assign ^1 'false' [line 3]\n"
                "45: if_noteq ^1 l 47 [line 3]\n"
                "46: jump 49 [line 3]\n"
                "47: assign ^2 'true' [line 3]\n"
                "48: jump 50 [line 3]\n"
                "49: assign ^2 'false' [line 3]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/backpatch/p3t_relational.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(InterCodeSuite, backpatch_p3t_object_creation_expr)
{
    expected =  "1: tablecreate ^0 [line 1]\n"
                "2: tablecreate ^0 [line 3]\n"
                "3: tablesetelem ^0 0 1 [line 3]\n"
                "4: tablesetelem ^0 1 2 [line 3]\n"
                "5: tablesetelem ^0 2 3 [line 3]\n"
                "6: tablecreate ^0 [line 5]\n"
                "7: tablecreate ^1 [line 5]\n"
                "8: tablesetelem ^1 0 \"hello\" [line 5]\n"
                "9: tablesetelem ^1 1 \"lol\" [line 5]\n"
                "10: tablecreate ^2 [line 5]\n"
                "11: tablecreate ^3 [line 5]\n"
                "12: tablesetelem ^3 0 'false' [line 5]\n"
                "13: tablesetelem ^3 1 'true' [line 5]\n"
                "14: tablesetelem ^3 2 ^0 [line 5]\n"
                "15: tablesetelem ^3 3 5 [line 5]\n"
                "16: tablesetelem ^3 4 ^1 [line 5]\n"
                "17: tablesetelem ^3 5 ^2 [line 5]\n"
                "18: jump 22 [line 7]\n"
                "19: funcstart $0 [line 7]\n"
                "20: tablecreate ^0 [line 7]\n"
                "21: funcend $0 [line 7]\n"
                "22: tablecreate ^0 [line 8]\n"
                "23: tablesetelem ^0 0 1 [line 8]\n"
                "24: tablesetelem ^0 1 2 [line 8]\n"
                "25: tablesetelem ^0 2 3 [line 8]\n"
                "26: tablecreate ^1 [line 8]\n"
                "27: tablesetelem ^1 1 2 [line 8]\n"
                "28: tablesetelem ^1 \"bool\" 'false' [line 8]\n"
                "29: tablesetelem ^1 5 print [line 8]\n"
                "30: tablecreate ^2 [line 9]\n"
                "31: tablesetelem ^2 4 5 [line 9]\n"
                "32: tablesetelem ^2 2 18 [line 9]\n"
                "33: tablesetelem ^2 \"lulz\" $0 [line 9]\n"
                "34: tablesetelem ^2 \"wot\" ^0 [line 9]\n"
                "35: tablesetelem ^2 \"wut\" ^1 [line 9]\n"
                "36: jump 41 [line 11]\n"
                "37: funcstart four [line 11]\n"
                "38: assign four 4 [line 11]\n"
                "39: assign ^0 four [line 11]\n"
                "40: funcend four [line 11]\n"
                "41: tablecreate ^0 [line 11]\n"
                "42: tablesetelem ^0 4 four [line 11]\n"
                "43: tablecreate ^1 [line 11]\n"
                "44: tablesetelem ^1 4 ^0 [line 11]\n"
                "45: tablecreate ^2 [line 11]\n"
                "46: tablesetelem ^2 4 ^1 [line 11]\n"
                "47: tablecreate ^3 [line 11]\n"
                "48: tablesetelem ^3 4 ^2 [line 11]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/backpatch/p3t_object_creation_expr.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(InterCodeSuite, backpatch_p3t_if_else)
{
    expected =  "1: if_eq 'true' 'true' 3 [line 1]\n"
                "2: jump 5 [line 1]\n"
                "3: assign a 4 [line 2]\n"
                "4: assign ^0 a [line 2]\n"
                "5: if_eq 'false' 'true' 7 [line 4]\n"
                "6: jump 9 [line 4]\n"
                "7: assign b 5 [line 5]\n"
                "8: assign ^0 b [line 5]\n"
                "9: if_eq a 'true' 11 [line 7]\n"
                "10: jump 15 [line 7]\n"
                "11: if_eq b 'true' 13 [line 8]\n"
                "12: jump 15 [line 8]\n"
                "13: assign c 6 [line 9]\n"
                "14: assign ^0 c [line 9]\n"
                "15: if_eq a 'true' 17 [line 11]\n"
                "16: jump 20 [line 11]\n"
                "17: assign d 4 [line 12]\n"
                "18: assign ^0 d [line 12]\n"
                "19: jump 22 [line 13]\n"
                "20: assign c a [line 14]\n"
                "21: assign ^0 c [line 14]\n"
                "22: if_eq a 'true' 24 [line 16]\n"
                "23: jump 30 [line 16]\n"
                "24: if_eq b 'true' 26 [line 17]\n"
                "25: jump 29 [line 17]\n"
                "26: assign c 5 [line 18]\n"
                "27: assign ^0 c [line 18]\n"
                "28: jump 30 [line 19]\n"
                "29: sub ^0 d 5 [line 20]\n"
                "30: if_eq a 'true' 32 [line 22]\n"
                "31: jump 38 [line 22]\n"
                "32: if_eq b 'true' 34 [line 23]\n"
                "33: jump 36 [line 23]\n"
                "34: sub ^0 d 0 [line 24]\n"
                "35: jump 37 [line 25]\n"
                "36: sub ^0 d 4 [line 26]\n"
                "37: jump 39 [line 27]\n"
                "38: sub ^0 0 d [line 28]\n"
                "39: if_eq a 'true' 41 [line 30]\n"
                "40: jump 45 [line 30]\n"
                "41: if_eq b 'true' 43 [line 31]\n"
                "42: jump 44 [line 31]\n"
                "43: add ^0 0 d [line 32]\n"
                "44: jump 46 [line 34]\n"
                "45: sub ^0 0 0 [line 35]\n"
                "46: if_eq a 'true' 48 [line 38]\n"
                "47: jump 50 [line 38]\n"
                "48: add ^0 8 0 [line 39]\n"
                "49: jump 59 [line 40]\n"
                "50: if_eq b 'true' 52 [line 40]\n"
                "51: jump 54 [line 40]\n"
                "52: add ^0 9 0 [line 41]\n"
                "53: jump 59 [line 42]\n"
                "54: if_eq c 'true' 56 [line 42]\n"
                "55: jump 58 [line 42]\n"
                "56: add ^0 8 0 [line 43]\n"
                "57: jump 59 [line 44]\n"
                "58: sub ^0 10 0 [line 45]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/backpatch/p3t_if_else.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(InterCodeSuite, backpatch_p3t_funcdecl)
{
    expected =  "1: jump 8 [line 2]\n"
                "2: funcstart iamafunction [line 2]\n"
                "3: add ^0 a b [line 3]\n"
                "4: return 8 [line 4]\n"
                "5: jump 7 [line 4]\n"
                "6: sub ^0 9 0 [line 5]\n"
                "7: funcend iamafunction [line 6]\n"
                "8: jump 11 [line 8]\n"
                "9: funcstart iamnothing [line 8]\n"
                "10: funcend iamnothing [line 8]\n"
                "11: jump 18 [line 10]\n"
                "12: funcstart $0 [line 10]\n"
                "13: sub ^0 10 0 [line 10]\n"
                "14: return 12 [line 10]\n"
                "15: jump 17 [line 10]\n"
                "16: sub ^0 11 0 [line 10]\n"
                "17: funcend $0 [line 10]\n"
                "18: assign ihavenoname $0 [line 10]\n"
                "19: assign ^0 ihavenoname [line 10]\n"
                "20: jump 23 [line 12]\n"
                "21: funcstart $1 [line 12]\n"
                "22: funcend $1 [line 12]\n"
                "23: jump 33 [line 14]\n"
                "24: funcstart $2 [line 14]\n"
                "25: jump 32 [line 14]\n"
                "26: funcstart $3 [line 14]\n"
                "27: param 4 [line 14]\n"
                "28: param 3 [line 14]\n"
                "29: call iamafunction [line 14]\n"
                "30: getretval ^0 [line 14]\n"
                "31: funcend $3 [line 14]\n"
                "32: funcend $2 [line 14]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/backpatch/p3t_funcdecl.asc");
    GTEST_ASSERT_EQ(expected, actual);
}


TEST_F(InterCodeSuite, backpatch_p3t_flow_control)
{
    expected =  "1: assign a 1 [line 1]\n"
                "2: assign ^0 a [line 1]\n"
                "3: if_eq ^0 'true' 5 [line 1]\n"
                "4: jump 10 [line 1]\n"
                "5: sub ^1 2 0 [line 2]\n"
                "6: jump 10 [line 3]\n"
                "7: sub ^0 3 0 [line 4]\n"
                "8: jump 1 [line 5]\n"
                "9: jump 1 [line 6]\n"
                "10: if_eq 4 'true' 12 [line 8]\n"
                "11: jump 19 [line 8]\n"
                "12: if_eq 5 'true' 14 [line 8]\n"
                "13: jump 18 [line 8]\n"
                "14: if_eq 6 'true' 16 [line 8]\n"
                "15: jump 17 [line 8]\n"
                "16: jump 14 [line 8]\n"
                "17: jump 12 [line 8]\n"
                "18: jump 10 [line 8]\n"
                "19: if_eq 7 'true' 21 [line 10]\n"
                "20: jump 27 [line 10]\n"
                "21: if_eq 8 'true' 23 [line 10]\n"
                "22: jump 25 [line 10]\n"
                "23: sub ^0 9 0 [line 10]\n"
                "24: jump 26 [line 10]\n"
                "25: sub ^0 10 0 [line 10]\n"
                "26: jump 19 [line 10]\n"
                "27: if_eq 11 'true' 29 [line 12]\n"
                "28: jump 47 [line 12]\n"
                "29: sub ^0 12 0 [line 13]\n"
                "30: if_eq 13 'true' 34 [line 13]\n"
                "31: jump 46 [line 13]\n"
                "32: sub ^1 14 0 [line 13]\n"
                "33: jump 30 [line 13]\n"
                "34: if_eq 15 'true' 36 [line 14]\n"
                "35: jump 38 [line 14]\n"
                "36: sub ^2 16 0 [line 15]\n"
                "37: jump 43 [line 16]\n"
                "38: if_eq 17 'true' 40 [line 16]\n"
                "39: jump 42 [line 16]\n"
                "40: sub ^0 18 0 [line 17]\n"
                "41: jump 43 [line 18]\n"
                "42: sub ^0 19 0 [line 19]\n"
                "43: jump 46 [line 20]\n"
                "44: jump 32 [line 21]\n"
                "45: jump 32 [line 22]\n"
                "46: jump 27 [line 22]\n"
                "47: sub ^0 20 0 [line 24]\n"
                "48: if_eq 21 'true' 52 [line 24]\n"
                "49: jump 58 [line 24]\n"
                "50: sub ^1 22 0 [line 24]\n"
                "51: jump 48 [line 24]\n"
                "52: if_eq 23 'true' 54 [line 25]\n"
                "53: jump 56 [line 25]\n"
                "54: jump 56 [line 26]\n"
                "55: jump 52 [line 26]\n"
                "56: jump 50 [line 27]\n"
                "57: jump 50 [line 28]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/backpatch/p3t_flow_control.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(InterCodeSuite, backpatch_p3t_flow_control_error)
{
    expected =  "\x1B[31mError, in line: 3:\x1B[0m invalid keyword CONTINUE outside of loop\n"
                "\x1B[31mError, in line: 4:\x1B[0m invalid keyword BREAK outside of loop\n"
                "\x1B[31mError, in line: 6:\x1B[0m Invalid return, used outside a function block\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/backpatch/p3t_flow_control_error.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(InterCodeSuite, backpatch_p3t_const_maths)
{
    expected =  "1: add ^0 4 5 [line 1]\n"
                "2: mul ^0 6 8 [line 2]\n"
                "3: sub ^0 4 9 [line 3]\n"
                "4: div ^0 3 1 [line 4]\n"
                "5: mod ^0 0 3 [line 5]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/backpatch/p3t_const_maths.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(InterCodeSuite, backpatch_p3t_basic_expr)
{
    expected =  "1: assign a 4 [line 2]\n"
                "2: assign ^0 a [line 2]\n"
                "3: assign b 5 [line 3]\n"
                "4: assign ^0 b [line 3]\n"
                "5: assign c 7 [line 4]\n"
                "6: assign ^0 c [line 4]\n"
                "7: assign d 8 [line 5]\n"
                "8: assign ^0 d [line 5]\n"
                "9: add ^0 a b [line 7]\n"
                "10: add ^1 ^0 c [line 7]\n"
                "11: add ^2 ^1 d [line 7]\n"
                "12: assign e ^2 [line 7]\n"
                "13: assign ^3 e [line 7]\n"
                "14: add ^0 c d [line 8]\n"
                "15: add ^1 b ^0 [line 8]\n"
                "16: add ^2 a ^1 [line 8]\n"
                "17: assign e ^2 [line 8]\n"
                "18: assign ^3 e [line 8]\n"
                "19: uminus ^0 a [line 9]\n"
                "20: add ^1 a b [line 9]\n"
                "21: assign e ^1 [line 9]\n"
                "22: assign ^2 e [line 9]\n"
                "23: add ^0 a b [line 10]\n"
                "24: uminus ^1 ^0 [line 10]\n"
                "25: assign e ^0 [line 10]\n"
                "26: assign ^2 e [line 10]\n"
                "27: mul ^0 a b [line 12]\n"
                "28: div ^1 c d [line 12]\n"
                "29: mod ^2 ^1 e [line 12]\n"
                "30: sub ^3 ^0 ^2 [line 12]\n"
                "31: assign e ^3 [line 12]\n"
                "32: assign ^4 e [line 12]\n"
                "33: mod ^0 d e [line 13]\n"
                "34: div ^1 c ^0 [line 13]\n"
                "35: sub ^2 b ^1 [line 13]\n"
                "36: mul ^3 a ^2 [line 13]\n"
                "37: assign e ^3 [line 13]\n"
                "38: assign ^4 e [line 13]\n"
                "39: add ^0 a b [line 15]\n"
                "40: uminus ^1 ^0 [line 15]\n"
                "41: add ^2 c d [line 15]\n"
                "42: uminus ^3 ^2 [line 15]\n"
                "43: div ^4 ^0 ^2 [line 15]\n"
                "44: uminus ^5 ^4 [line 15]\n"
                "45: mod ^6 ^4 e [line 15]\n"
                "46: assign e ^6 [line 15]\n"
                "47: assign ^7 e [line 15]\n"
                "48: assign ^0 a [line 17]\n"
                "49: sub a a 1 [line 17]\n"
                "50: add b b 1 [line 17]\n"
                "51: assign ^1 b [line 17]\n"
                "52: sub ^2 ^0 ^1 [line 17]\n"
                "53: sub c c 1 [line 17]\n"
                "54: assign ^3 c [line 17]\n"
                "55: add ^4 ^2 ^3 [line 17]\n"
                "56: assign ^5 d [line 17]\n"
                "57: add d d 1 [line 17]\n"
                "58: sub ^6 ^4 ^5 [line 17]\n"
                "59: tablegetelem ^1 t \"x\" [line 19]\n"
                "60: assign ^0 ^1 [line 19]\n"
                "61: sub ^1 ^1 1 [line 19]\n"
                "62: tablesetelem t \"x\" ^1 [line 19]\n"
                "63: tablegetelem ^3 t 3 [line 19]\n"
                "64: add ^3 ^3 1 [line 19]\n"
                "65: tablesetelem t 3 ^3 [line 19]\n"
                "66: sub ^4 ^0 ^3 [line 19]\n"
                "67: tablegetelem ^6 t \"a\" [line 19]\n"
                "68: sub ^6 ^6 1 [line 19]\n"
                "69: tablesetelem t \"a\" ^6 [line 19]\n"
                "70: add ^7 ^4 ^6 [line 19]\n"
                "71: tablegetelem ^9 t \"z\" [line 19]\n"
                "72: assign ^8 ^9 [line 19]\n"
                "73: add ^9 ^9 1 [line 19]\n"
                "74: tablesetelem t \"z\" ^9 [line 19]\n"
                "75: sub ^10 ^7 ^8 [line 19]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/backpatch/p3t_basic_expr.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(InterCodeSuite, backpatch_p3t_calls)
{
    expected =  "1: assign e 'false' [line 2]\n"
                "2: assign ^0 e [line 2]\n"
                "3: assign d ^0 [line 2]\n"
                "4: assign ^1 d [line 2]\n"
                "5: assign c ^1 [line 2]\n"
                "6: assign ^2 c [line 2]\n"
                "7: assign b ^2 [line 2]\n"
                "8: assign ^3 b [line 2]\n"
                "9: assign a ^3 [line 2]\n"
                "10: assign ^4 a [line 2]\n"
                "11: tablegetelem ^0 a \"f\" [line 4]\n"
                "12: param a [line 4]\n"
                "13: param a [line 4]\n"
                "14: call ^0 [line 4]\n"
                "15: getretval ^1 [line 4]\n"
                "16: call e [line 6]\n"
                "17: getretval ^0 [line 6]\n"
                "18: call e [line 6]\n"
                "19: getretval ^1 [line 6]\n"
                "20: call ^1 [line 6]\n"
                "21: getretval ^2 [line 6]\n"
                "22: param ^2 [line 6]\n"
                "23: param d [line 6]\n"
                "24: param c [line 6]\n"
                "25: param b [line 6]\n"
                "26: param a [line 6]\n"
                "27: call ^0 [line 6]\n"
                "28: getretval ^3 [line 6]\n"
                "29: jump 34 [line 9]\n"
                "30: funcstart $0 [line 9]\n"
                "31: call b [line 9]\n"
                "32: getretval ^0 [line 9]\n"
                "33: funcend $0 [line 9]\n"
                "34: call a [line 9]\n"
                "35: getretval ^0 [line 9]\n"
                "36: call b [line 9]\n"
                "37: getretval ^1 [line 9]\n"
                "38: call ^1 [line 9]\n"
                "39: getretval ^2 [line 9]\n"
                "40: param ^2 [line 9]\n"
                "41: param ^0 [line 9]\n"
                "42: call $0 [line 9]\n"
                "43: getretval ^3 [line 9]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/backpatch/p3t_calls.asc");
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
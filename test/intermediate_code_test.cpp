#include <gtest/gtest.h>
#include "../util/exec/exec.h"

/**
 * @brief Test suite for the intermediate code
 * production.
 * 
**/ 
class InterCodeSuite : public ::testing::Test {
protected:
    std::string expected;
    std::string actual;    
};

/*  ------------ Assign --------------   */

TEST_F(InterCodeSuite, assign_simple) {
   expected =   "1:   ASSIGN x 2 [line 1]\n"
                "2:   ASSIGN ^0 x [line 1]\n";
   actual = exec("./d_intermediate_code ../../test/files/phase3_tests/assign/simple.asc");
   GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(InterCodeSuite, assign_calls_nested) {
    expected =  "1:   JUMP 4 [line 1]\n"
                "2:   FUNCSTART f [line 1]\n"
                "3:   FUNCEND f [line 2]\n"
                "4:   JUMP 7 [line 4]\n"
                "5:   FUNCSTART g [line 4]\n"
                "6:   FUNCEND g [line 5]\n"
                "7:   PARAM 3 [line 7]\n"
                "8:   CALL g [line 7]\n"
                "9:   GETRETVAL ^0 [line 7]\n"
                "10:   PARAM ^0 [line 7]\n"
                "11:   CALL f [line 7]\n"
                "12:   GETRETVAL ^1 [line 7]\n"
                "13:   ASSIGN x ^1 [line 7]\n"
                "14:   ASSIGN ^2 x [line 7]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/assign/calls_nested.asc");
    GTEST_ASSERT_EQ(expected, actual);            
}

TEST_F(InterCodeSuite, assign_string) {
    expected =  "1:   ASSIGN a \"2\" [line 1]\n"
                "2:   ASSIGN ^0 a [line 1]\n"
                "3:   ASSIGN a \"adsad\" [line 2]\n"
                "4:   ASSIGN ^0 a [line 2]\n"
                "5:   ASSIGN a \"2ads\" [line 3]\n"
                "6:   ASSIGN ^0 a [line 3]\n"
                "7:   ASSIGN a \"2z\" [line 4]\n"
                "8:   ASSIGN ^0 a [line 4]\n"
                "9:   ASSIGN a \"222\" [line 5]\n"
                "10:   ASSIGN ^0 a [line 5]\n"
                "11:   ASSIGN a \"21\" [line 6]\n"
                "12:   ASSIGN ^0 a [line 6]\n"
                "13:   ASSIGN a \"2111\" [line 7]\n"
                "14:   ASSIGN ^0 a [line 7]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/assign/string.asc");
    GTEST_ASSERT_EQ(expected, actual);             
}

/*  ------------ Function --------------   */

TEST_F(InterCodeSuite, functions_single) {
    expected =  "1:   JUMP 4 [line 1]\n"
                "2:   FUNCSTART f [line 1]\n"
                "3:   FUNCEND f [line 2]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/functions/single.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(InterCodeSuite, functions_many) {
    expected =  "1:   JUMP 4 [line 1]\n"
                "2:   FUNCSTART f [line 1]\n"
                "3:   FUNCEND f [line 2]\n"
                "4:   JUMP 7 [line 4]\n"
                "5:   FUNCSTART g [line 4]\n"
                "6:   FUNCEND g [line 5]\n"
                "7:   JUMP 10 [line 8]\n"
                "8:   FUNCSTART y [line 8]\n"
                "9:   FUNCEND y [line 9]\n"
                "10:   JUMP 13 [line 17]\n"
                "11:   FUNCSTART r [line 17]\n"
                "12:   FUNCEND r [line 18]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/functions/many.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(InterCodeSuite, functions_nested) {
    expected =  "1:   JUMP 16 [line 5]\n"
                "2:   FUNCSTART a [line 5]\n"
                "3:   JUMP 12 [line 6]\n"
                "4:   FUNCSTART b [line 6]\n"
                "5:   JUMP 11 [line 7]\n"
                "6:   FUNCSTART c [line 7]\n"
                "7:   JUMP 10 [line 8]\n"
                "8:   FUNCSTART d [line 8]\n"
                "9:   FUNCEND d [line 9]\n"
                "10:   FUNCEND c [line 10]\n"
                "11:   FUNCEND b [line 11]\n"
                "12:   JUMP 15 [line 12]\n"
                "13:   FUNCSTART e [line 12]\n"
                "14:   FUNCEND e [line 13]\n"
                "15:   FUNCEND a [line 14]\n"
                "16:   JUMP 19 [line 16]\n"
                "17:   FUNCSTART e [line 16]\n"
                "18:   FUNCEND e [line 27]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/functions/nested.asc");
    GTEST_ASSERT_EQ(expected, actual);            
}

TEST_F(InterCodeSuite, functions_shadowed) {
    expected =  "1:   JUMP 7 [line 1]\n"
                "2:   FUNCSTART lkdland [line 1]\n"
                "3:   JUMP 6 [line 2]\n"
                "4:   FUNCSTART lkdland [line 2]\n"
                "5:   FUNCEND lkdland [line 3]\n"
                "6:   FUNCEND lkdland [line 5]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/functions/shadowed.asc");
    GTEST_ASSERT_EQ(expected, actual);            
}

TEST_F(InterCodeSuite, functions_anonymous) {
    expected =  "1:   JUMP 10 [line 1]\n"
                "2:   FUNCSTART $1 [line 1]\n"
                "3:   JUMP 6 [line 2]\n"
                "4:   FUNCSTART c [line 2]\n"
                "5:   FUNCEND c [line 3]\n"
                "6:   JUMP 9 [line 4]\n"
                "7:   FUNCSTART $2 [line 4]\n"
                "8:   FUNCEND $2 [line 5]\n"
                "9:   FUNCEND $1 [line 6]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/functions/anonymous.asc");
    GTEST_ASSERT_EQ(expected, actual);              
}

TEST_F(InterCodeSuite, functions_call_warning) {
    expected =  "1:   CALL g [line 1]\n"
                "2:   GETRETVAL ^0 [line 1]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/functions/call_warning.asc");
    GTEST_ASSERT_EQ(expected, actual);  
}

TEST_F(InterCodeSuite, functions_call_noparameters) {
    expected =  "1:   JUMP 4 [line 1]\n"
                "2:   FUNCSTART f [line 1]\n"
                "3:   FUNCEND f [line 2]\n"
                "4:   CALL f [line 4]\n"
                "5:   GETRETVAL ^0 [line 4]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/functions/call_no_parameters.asc");
    GTEST_ASSERT_EQ(expected, actual);          
}

TEST_F(InterCodeSuite, functions_call_parameters) {
    expected =  "1:   JUMP 4 [line 1]\n"
                "2:   FUNCSTART f [line 1]\n"
                "3:   FUNCEND f [line 2]\n"
                "4:   PARAM 0 [line 4]\n"
                "5:   CALL f [line 4]\n"
                "6:   GETRETVAL ^0 [line 4]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/functions/call_parameters.asc");
    GTEST_ASSERT_EQ(expected, actual);               
}

TEST_F(InterCodeSuite, functions_call_with_symbol_parameters) {
    expected =  "Warning, in line: 10: Too many arguments passed to function: f, defined in line: 1\n"
                "Warning, in line: 11: Too many arguments passed to function: f, defined in line: 1\n"
                "Warning, in line: 12: Too many arguments passed to function: f, defined in line: 1\n"
                "1:   JUMP 4 [line 1]\n"
                "2:   FUNCSTART f [line 1]\n"
                "3:   FUNCEND f [line 2]\n"
                "4:   PARAM c [line 5]\n"
                "5:   CALL f [line 5]\n"
                "6:   GETRETVAL ^0 [line 5]\n"
                "7:   PARAM 0 [line 10]\n"
                "8:   PARAM c [line 10]\n"
                "9:   CALL f [line 10]\n"
                "10:   GETRETVAL ^0 [line 10]\n"
                "11:   PARAM 3 [line 11]\n"
                "12:   PARAM 10 [line 11]\n"
                "13:   PARAM c [line 11]\n"
                "14:   CALL f [line 11]\n"
                "15:   GETRETVAL ^0 [line 11]\n"
                "16:   PARAM aaaaa [line 12]\n"
                "17:   PARAM aaaaa [line 12]\n"
                "18:   PARAM a [line 12]\n"
                "19:   PARAM 0 [line 12]\n"
                "20:   PARAM c [line 12]\n"
                "21:   CALL f [line 12]\n"
                "22:   GETRETVAL ^0 [line 12]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/functions/call_with_symbol_parameters.asc");
    GTEST_ASSERT_EQ(expected, actual);            
}

TEST_F(InterCodeSuite, functions_call_less_parameters) {
    expected = "Error, in line: 4: Too few arguments passed to function: f, defined in line: 1\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/functions/call_less_parameters.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(InterCodeSuite, functions_call_nested) {
    expected =  "Warning, in line: 10: Too many arguments passed to function: g, defined in line: 6\n"
                "Warning, in line: 12: Too many arguments passed to function: g, defined in line: 6\n"
                "1:   JUMP 4 [line 2]\n"
                "2:   FUNCSTART f [line 2]\n"
                "3:   FUNCEND f [line 4]\n"
                "4:   JUMP 7 [line 6]\n"
                "5:   FUNCSTART g [line 6]\n"
                "6:   FUNCEND g [line 8]\n"
                "7:   PARAM 1 [line 10]\n"
                "8:   CALL g [line 10]\n"
                "9:   GETRETVAL ^0 [line 10]\n"
                "10:   PARAM ^0 [line 10]\n"
                "11:   CALL f [line 10]\n"
                "12:   GETRETVAL ^1 [line 10]\n"
                "13:   PARAM 2 [line 12]\n"
                "14:   CALL y [line 12]\n"
                "15:   GETRETVAL ^0 [line 12]\n"
                "16:   PARAM ^0 [line 12]\n"
                "17:   CALL g [line 12]\n"
                "18:   GETRETVAL ^1 [line 12]\n"
                "19:   PARAM ^1 [line 12]\n"
                "20:   CALL f [line 12]\n"
                "21:   GETRETVAL ^2 [line 12]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/functions/call_nested.asc");
    GTEST_ASSERT_EQ(expected, actual);            
}

TEST_F(InterCodeSuite, functions_call_more_parameters) {
    expected =  "Warning, in line: 5: Too many arguments passed to function: f, defined in line: 1\n"
                "1:   JUMP 4 [line 1]\n"
                "2:   FUNCSTART f [line 1]\n"
                "3:   FUNCEND f [line 3]\n"
                "4:   PARAM 2 [line 5]\n"
                "5:   PARAM a [line 5]\n"
                "6:   CALL f [line 5]\n"
                "7:   GETRETVAL ^0 [line 5]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/functions/call_more_parameters.asc");
    GTEST_ASSERT_EQ(expected, actual);            
}

TEST_F(InterCodeSuite, functions_call_function_def) {
    expected =  "1:   JUMP 4 [line 1]\n"
                "2:   FUNCSTART f [line 1]\n"
                "3:   FUNCEND f [line 1]\n"
                "4:   PARAM 1 [line 1]\n"
                "5:   CALL f [line 1]\n"
                "6:   GETRETVAL ^0 [line 1]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/functions/call_function_def.asc");
    GTEST_ASSERT_EQ(expected, actual);        
}

TEST_F(InterCodeSuite, functions_call_nested_with_func_def) {
    expected =  "1:   JUMP 4 [line 1]\n"
                "2:   FUNCSTART x [line 1]\n"
                "3:   FUNCEND x [line 1]\n"
                "4:   PARAM d [line 1]\n"
                "5:   CALL x [line 1]\n"
                "6:   GETRETVAL ^0 [line 1]\n"
                "7:   PARAM ^0 [line 1]\n"
                "8:   CALL g [line 1]\n"
                "9:   GETRETVAL ^1 [line 1]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/functions/call_nested_with_func_def.asc");
    GTEST_ASSERT_EQ(expected, actual);              
}

TEST_F(InterCodeSuite, functions_call_anonymous_func_def) {
    expected =  "1:   JUMP 4 [line 1]\n"
                "2:   FUNCSTART $1 [line 1]\n"
                "3:   FUNCEND $1 [line 1]\n"
                "4:   PARAM 4 [line 1]\n"
                "5:   PARAM 1 [line 1]\n"
                "6:   CALL $1 [line 1]\n"
                "7:   GETRETVAL ^0 [line 1]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/functions/call_anonymous_func_def.asc");
    GTEST_ASSERT_EQ(expected, actual);
}
/*  ------------ Return --------------   */

TEST_F(InterCodeSuite, return_void) {
    expected =  "1:   JUMP 6 [line 1]\n"
                "2:   FUNCSTART f [line 1]\n"
                "3:   RETURN [line 2]\n"
                "4:   JUMP 5 [line 2]\n"
                "5:   FUNCEND f [line 3]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/return/return_void.asc");
    GTEST_ASSERT_EQ(expected, actual);            
}


TEST_F(InterCodeSuite, return_symbol) {
    expected =  "1:   JUMP 6 [line 5]\n"
                "2:   FUNCSTART f [line 5]\n"
                "3:   RETURN x [line 6]\n"
                "4:   JUMP 5 [line 6]\n"
                "5:   FUNCEND f [line 7]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/return/return_symbol.asc");
    GTEST_ASSERT_EQ(expected, actual);            
}

TEST_F(InterCodeSuite, return_many_and_nested) {
    expected =  "1:   JUMP 17 [line 1]\n"
                "2:   FUNCSTART MPIFTEKI [line 1]\n"
                "3:   RETURN [line 2]\n"
                "4:   JUMP 16 [line 2]\n"
                "5:   RETURN a [line 4]\n"
                "6:   JUMP 16 [line 4]\n"
                "7:   JUMP 12 [line 6]\n"
                "8:   FUNCSTART $1 [line 6]\n"
                "9:   RETURN [line 6]\n"
                "10:   JUMP 11 [line 6]\n"
                "11:   FUNCEND $1 [line 6]\n"
                "12:   CALL $1 [line 6]\n"
                "13:   GETRETVAL ^0 [line 6]\n"
                "14:   RETURN ^0 [line 6]\n"
                "15:   JUMP 16 [line 6]\n"
                "16:   FUNCEND MPIFTEKI [line 7]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/return/return_many_and_nested.asc");
    GTEST_ASSERT_EQ(expected, actual);            
}

/*  ------------ Logical -----------------   */

TEST_F(InterCodeSuite, logical_greater_simple) {
    expected =  "1:   IF_GREATER 3 2 3 [line 1]\n"
                "2:   JUMP 5 [line 1]\n"
                "3:   ASSIGN ^0 'true' [line 1]\n"
                "4:   JUMP 6 [line 1]\n"
                "5:   ASSIGN ^0 'false' [line 1]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/logical/greater_simple.asc");    
    GTEST_ASSERT_EQ(expected, actual);     
}

TEST_F(InterCodeSuite, logical_equal_simple) {
    expected =  "1:   IF_EQ 3 2 3 [line 1]\n"
                "2:   JUMP 5 [line 1]\n"
                "3:   ASSIGN ^0 'true' [line 1]\n"
                "4:   JUMP 6 [line 1]\n"
                "5:   ASSIGN ^0 'false' [line 1]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/logical/equal_simple.asc");    
    GTEST_ASSERT_EQ(expected, actual);     
}

TEST_F(InterCodeSuite, logical_greater_equal_simple) {
    expected =  "1:   IF_GREATEREQ 3 2 3 [line 1]\n"
                "2:   JUMP 5 [line 1]\n"
                "3:   ASSIGN ^0 'true' [line 1]\n"
                "4:   JUMP 6 [line 1]\n"
                "5:   ASSIGN ^0 'false' [line 1]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/logical/greater_equal_simple.asc");    
    GTEST_ASSERT_EQ(expected, actual);     
}

TEST_F(InterCodeSuite, logical_less_equal_simple) {
    expected =  "1:   IF_LESSEQ 3 2 3 [line 1]\n"
                "2:   JUMP 5 [line 1]\n"
                "3:   ASSIGN ^0 'true' [line 1]\n"
                "4:   JUMP 6 [line 1]\n"
                "5:   ASSIGN ^0 'false' [line 1]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/logical/less_equal_simple.asc");    
    GTEST_ASSERT_EQ(expected, actual);     
}

TEST_F(InterCodeSuite, logical_less_simple) {
    expected =  "1:   IF_LESS 3 2 3 [line 1]\n"
                "2:   JUMP 5 [line 1]\n"
                "3:   ASSIGN ^0 'true' [line 1]\n"
                "4:   JUMP 6 [line 1]\n"
                "5:   ASSIGN ^0 'false' [line 1]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/logical/less_simple.asc");    
    GTEST_ASSERT_EQ(expected, actual);     
}

TEST_F(InterCodeSuite, logical_not_equal_simple) {
    expected =  "1:   IF_NOTEQ 3 2 3 [line 1]\n"
                "2:   JUMP 5 [line 1]\n"
                "3:   ASSIGN ^0 'true' [line 1]\n"
                "4:   JUMP 6 [line 1]\n"
                "5:   ASSIGN ^0 'false' [line 1]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/logical/not_equal_simple.asc");    
    GTEST_ASSERT_EQ(expected, actual);     
}


/* ------------- Loop -------------------  */

TEST_F(InterCodeSuite, loop_while_simple) {
    expected =  "1:   IF_GREATER 2 6 3 [line 1]\n"
                "2:   JUMP 5 [line 1]\n"
                "3:   ASSIGN ^0 'true' [line 1]\n"
                "4:   JUMP 6 [line 1]\n"
                "5:   ASSIGN ^0 'false' [line 1]\n"
                "6:   IF_EQ ^0 'true' 8 [line 1]\n"
                "7:   JUMP 9 [line 1]\n"
                "8:   JUMP 1 [line 1]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/loop/while_simple.asc");    
    GTEST_ASSERT_EQ(expected, actual);  
}

TEST_F(InterCodeSuite, loop_while_simple_with_stms) {
    expected =  "1:   IF_EQ 1 'true' 3 [line 1]\n"
                "2:   JUMP 6 [line 1]\n"
                "3:   ASSIGN x 9 [line 2]\n"
                "4:   ASSIGN ^0 x [line 2]\n"
                "5:   JUMP 1 [line 5]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/loop/while_simple_with_stmts.asc");    
    GTEST_ASSERT_EQ(expected, actual);              
}

TEST_F(InterCodeSuite, loop_while_many_nested) {
    expected =  "1:   IF_LESS x 0 3 [line 1]\n"
                "2:   JUMP 5 [line 1]\n"
                "3:   ASSIGN ^0 'true' [line 1]\n"
                "4:   JUMP 6 [line 1]\n"
                "5:   ASSIGN ^0 'false' [line 1]\n"
                "6:   IF_EQ ^0 'true' 8 [line 1]\n"
                "7:   JUMP 17 [line 1]\n"
                "8:   ASSIGN x 0 [line 2]\n"
                "9:   ASSIGN ^1 x [line 2]\n"
                "10:   IF_EQ 1 'true' 12 [line 3]\n"
                "11:   JUMP 16 [line 3]\n"
                "12:   IF_EQ 0 'true' 14 [line 3]\n"
                "13:   JUMP 15 [line 3]\n"
                "14:   JUMP 12 [line 3]\n"
                "15:   JUMP 10 [line 3]\n"
                "16:   JUMP 1 [line 4]\n"
                "17:   IF_EQ s 'true' 19 [line 9]\n"
                "18:   JUMP 20 [line 9]\n"
                "19:   JUMP 17 [line 10]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/loop/while_many_nested.asc");    
    GTEST_ASSERT_EQ(expected, actual);            
}

TEST_F(InterCodeSuite, loop_while_continue_single) {
    expected =  "1:   IF_EQ 1 'true' 3 [line 1]\n"
                "2:   JUMP 5 [line 1]\n"
                "3:   JUMP 1 [line 2]\n"
                "4:   JUMP 1 [line 3]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/loop/while_continue_single.asc");    
    GTEST_ASSERT_EQ(expected, actual);             
}

TEST_F(InterCodeSuite, loop_while_continue_many_nested) {
    expected =  "1:   IF_EQ 1 'true' 3 [line 1]\n"
                "2:   JUMP 29 [line 1]\n"
                "3:   JUMP 1 [line 2]\n"
                "4:   JUMP 1 [line 3]\n"
                "5:   JUMP 1 [line 4]\n"
                "6:   JUMP 1 [line 5]\n"
                "7:   JUMP 1 [line 5]\n"
                "8:   JUMP 1 [line 6]\n"
                "9:   JUMP 1 [line 7]\n"
                "10:   JUMP 1 [line 7]\n"
                "11:   JUMP 1 [line 7]\n"
                "12:   JUMP 1 [line 7]\n"
                "13:   JUMP 1 [line 7]\n"
                "14:   JUMP 1 [line 7]\n"
                "15:   JUMP 1 [line 7]\n"
                "16:   IF_LESS 3 0 18 [line 8]\n"
                "17:   JUMP 20 [line 8]\n"
                "18:   ASSIGN ^0 'true' [line 8]\n"
                "19:   JUMP 21 [line 8]\n"
                "20:   ASSIGN ^0 'false' [line 8]\n"
                "21:   IF_EQ ^0 'true' 23 [line 8]\n"
                "22:   JUMP 28 [line 8]\n"
                "23:   JUMP 16 [line 9]\n"
                "24:   JUMP 16 [line 10]\n"
                "25:   JUMP 16 [line 10]\n"
                "26:   JUMP 16 [line 10]\n"
                "27:   JUMP 16 [line 15]\n"
                "28:   JUMP 1 [line 20]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/loop/while_continue_many_nested.asc");    
    GTEST_ASSERT_EQ(expected, actual);            
}

TEST_F(InterCodeSuite, loop_while_break_single) {
    expected =  "1:   IF_EQ 1 'true' 3 [line 1]\n"
                "2:   JUMP 5 [line 1]\n"
                "3:   JUMP 5 [line 2]\n"
                "4:   JUMP 1 [line 3]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/loop/while_break_single.asc");    
    GTEST_ASSERT_EQ(expected, actual);             
}

TEST_F(InterCodeSuite, loop_for_simple) {
    expected =  "1:   ASSIGN i 0 [line 1]\n"
                "2:   ASSIGN ^0 i [line 1]\n"
                "3:   IF_LESS i 20 5 [line 1]\n"
                "4:   JUMP 7 [line 1]\n"
                "5:   ASSIGN ^1 'true' [line 1]\n"
                "6:   JUMP 8 [line 1]\n"
                "7:   ASSIGN ^1 'false' [line 1]\n"
                "8:   IF_EQ ^1 'true' 13 [line 1]\n"
                "9:   JUMP 14 [line 1]\n"
                "10:   ASSIGN ^2 i [line 1]\n"
                "11:   ADD i i 1 [line 1]\n"
                "12:   JUMP 3 [line 1]\n"
                "13:   JUMP 10 [line 2]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/loop/for_simple.asc");    
    GTEST_ASSERT_EQ(expected, actual);                
}

TEST_F(InterCodeSuite, loop_for_simple_with_stmts) {
    expected =  "1:   ASSIGN i 0 [line 1]\n"
                "2:   ASSIGN ^0 i [line 1]\n"
                "3:   IF_LESS i 20 5 [line 1]\n"
                "4:   JUMP 7 [line 1]\n"
                "5:   ASSIGN ^1 'true' [line 1]\n"
                "6:   JUMP 8 [line 1]\n"
                "7:   ASSIGN ^1 'false' [line 1]\n"
                "8:   IF_EQ ^1 'true' 13 [line 1]\n"
                "9:   JUMP 16 [line 1]\n"
                "10:   ASSIGN ^2 i [line 1]\n"
                "11:   ADD i i 1 [line 1]\n"
                "12:   JUMP 3 [line 1]\n"
                "13:   ASSIGN x 9 [line 2]\n"
                "14:   ASSIGN ^3 x [line 2]\n"
                "15:   JUMP 10 [line 5]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/loop/for_simple_with_stmts.asc");    
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(InterCodeSuite, loop_for_many_nested) {
    expected =  "1:   ASSIGN i 0 [line 1]\n"
                "2:   ASSIGN ^0 i [line 1]\n"
                "3:   IF_LESS i 20 5 [line 1]\n"
                "4:   JUMP 7 [line 1]\n"
                "5:   ASSIGN ^1 'true' [line 1]\n"
                "6:   JUMP 8 [line 1]\n"
                "7:   ASSIGN ^1 'false' [line 1]\n"
                "8:   IF_EQ ^1 'true' 13 [line 1]\n"
                "9:   JUMP 40 [line 1]\n"
                "10:   ASSIGN ^2 i [line 1]\n"
                "11:   ADD i i 1 [line 1]\n"
                "12:   JUMP 3 [line 1]\n"
                "13:   ASSIGN i 0 [line 2]\n"
                "14:   ASSIGN ^3 i [line 2]\n"
                "15:   IF_LESS i 20 17 [line 2]\n"
                "16:   JUMP 19 [line 2]\n"
                "17:   ASSIGN ^4 'true' [line 2]\n"
                "18:   JUMP 20 [line 2]\n"
                "19:   ASSIGN ^4 'false' [line 2]\n"
                "20:   IF_EQ ^4 'true' 25 [line 2]\n"
                "21:   JUMP 39 [line 2]\n"
                "22:   ASSIGN ^5 i [line 2]\n"
                "23:   ADD i i 1 [line 2]\n"
                "24:   JUMP 15 [line 2]\n"
                "25:   ASSIGN i 0 [line 2]\n"
                "26:   ASSIGN ^6 i [line 2]\n"
                "27:   IF_LESS i 20 29 [line 2]\n"
                "28:   JUMP 31 [line 2]\n"
                "29:   ASSIGN ^7 'true' [line 2]\n"
                "30:   JUMP 32 [line 2]\n"
                "31:   ASSIGN ^7 'false' [line 2]\n"
                "32:   IF_EQ ^7 'true' 37 [line 2]\n"
                "33:   JUMP 38 [line 2]\n"
                "34:   ASSIGN ^8 i [line 2]\n"
                "35:   ADD i i 1 [line 2]\n"
                "36:   JUMP 27 [line 2]\n"
                "37:   JUMP 34 [line 2]\n"
                "38:   JUMP 22 [line 2]\n"
                "39:   JUMP 10 [line 3]\n"
                "40:   ASSIGN i 0 [line 6]\n"
                "41:   ASSIGN ^0 i [line 6]\n"
                "42:   IF_LESS i 20 44 [line 6]\n"
                "43:   JUMP 46 [line 6]\n"
                "44:   ASSIGN ^1 'true' [line 6]\n"
                "45:   JUMP 47 [line 6]\n"
                "46:   ASSIGN ^1 'false' [line 6]\n"
                "47:   IF_EQ ^1 'true' 52 [line 6]\n"
                "48:   JUMP 53 [line 6]\n"
                "49:   ASSIGN ^2 i [line 6]\n"
                "50:   ADD i i 1 [line 6]\n"
                "51:   JUMP 42 [line 6]\n"
                "52:   JUMP 49 [line 6]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/loop/for_many_nested.asc");    
    GTEST_ASSERT_EQ(expected, actual);           
}

TEST_F(InterCodeSuite, loop_for_continue_single) {
    expected =  "1:   IF_LESS i 0 3 [line 1]\n"
                "2:   JUMP 5 [line 1]\n"
                "3:   ASSIGN ^0 'true' [line 1]\n"
                "4:   JUMP 6 [line 1]\n"
                "5:   ASSIGN ^0 'false' [line 1]\n"
                "6:   IF_EQ ^0 'true' 11 [line 1]\n"
                "7:   JUMP 13 [line 1]\n"
                "8:   ASSIGN ^1 i [line 1]\n"
                "9:   ADD i i 1 [line 1]\n"
                "10:   JUMP 1 [line 1]\n"
                "11:   JUMP 8 [line 2]\n"
                "12:   JUMP 8 [line 3]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/loop/for_continue_single.asc");    
    GTEST_ASSERT_EQ(expected, actual);               
}

TEST_F(InterCodeSuite, loop_for_continue_many_nested) {
    expected =  "1:   IF_LESS i 0 3 [line 1]\n"
                "2:   JUMP 5 [line 1]\n"
                "3:   ASSIGN ^0 'true' [line 1]\n"
                "4:   JUMP 6 [line 1]\n"
                "5:   ASSIGN ^0 'false' [line 1]\n"
                "6:   IF_EQ ^0 'true' 11 [line 1]\n"
                "7:   JUMP 34 [line 1]\n"
                "8:   ASSIGN ^1 i [line 1]\n"
                "9:   ADD i i 1 [line 1]\n"
                "10:   JUMP 1 [line 1]\n"
                "11:   JUMP 8 [line 2]\n"
                "12:   JUMP 8 [line 2]\n"
                "13:   JUMP 8 [line 3]\n"
                "14:   JUMP 8 [line 3]\n"
                "15:   JUMP 8 [line 3]\n"
                "16:   JUMP 8 [line 3]\n"
                "17:   JUMP 8 [line 3]\n"
                "18:   JUMP 8 [line 3]\n"
                "19:   IF_LESS i 0 21 [line 4]\n"
                "20:   JUMP 23 [line 4]\n"
                "21:   ASSIGN ^0 'true' [line 4]\n"
                "22:   JUMP 24 [line 4]\n"
                "23:   ASSIGN ^0 'false' [line 4]\n"
                "24:   IF_EQ ^0 'true' 29 [line 4]\n"
                "25:   JUMP 33 [line 4]\n"
                "26:   ASSIGN ^1 i [line 4]\n"
                "27:   ADD i i 1 [line 4]\n"
                "28:   JUMP 19 [line 4]\n"
                "29:   JUMP 26 [line 4]\n"
                "30:   JUMP 26 [line 4]\n"
                "31:   JUMP 26 [line 4]\n"
                "32:   JUMP 26 [line 4]\n"
                "33:   JUMP 8 [line 5]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/loop/for_continue_many_nested.asc");    
    GTEST_ASSERT_EQ(expected, actual);            
}

TEST_F(InterCodeSuite, loop_for_break_single) {
    expected =  "1:   IF_LESS i 8 3 [line 1]\n"
                "2:   JUMP 5 [line 1]\n"
                "3:   ASSIGN ^0 'true' [line 1]\n"
                "4:   JUMP 6 [line 1]\n"
                "5:   ASSIGN ^0 'false' [line 1]\n"
                "6:   IF_EQ ^0 'true' 11 [line 1]\n"
                "7:   JUMP 13 [line 1]\n"
                "8:   ASSIGN ^1 i [line 1]\n"
                "9:   ADD i i 1 [line 1]\n"
                "10:   JUMP 1 [line 1]\n"
                "11:   JUMP 13 [line 2]\n"
                "12:   JUMP 8 [line 3]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/loop/for_break_single.asc");    
    GTEST_ASSERT_EQ(expected, actual);            
}

TEST_F(InterCodeSuite, loop_for_break_many_nested) {
    expected =  "1:   IF_LESS i 8 3 [line 1]\n"
                "2:   JUMP 5 [line 1]\n"
                "3:   ASSIGN ^0 'true' [line 1]\n"
                "4:   JUMP 6 [line 1]\n"
                "5:   ASSIGN ^0 'false' [line 1]\n"
                "6:   IF_EQ ^0 'true' 11 [line 1]\n"
                "7:   JUMP 33 [line 1]\n"
                "8:   ASSIGN ^1 i [line 1]\n"
                "9:   ADD i i 1 [line 1]\n"
                "10:   JUMP 1 [line 1]\n"
                "11:   JUMP 33 [line 2]\n"
                "12:   JUMP 33 [line 3]\n"
                "13:   JUMP 33 [line 3]\n"
                "14:   JUMP 33 [line 3]\n"
                "15:   JUMP 33 [line 3]\n"
                "16:   JUMP 33 [line 3]\n"
                "17:   JUMP 33 [line 3]\n"
                "18:   JUMP 33 [line 3]\n"
                "19:   IF_GREATER j 0 21 [line 4]\n"
                "20:   JUMP 23 [line 4]\n"
                "21:   ASSIGN ^0 'true' [line 4]\n"
                "22:   JUMP 24 [line 4]\n"
                "23:   ASSIGN ^0 'false' [line 4]\n"
                "24:   IF_EQ ^0 'true' 29 [line 4]\n"
                "25:   JUMP 32 [line 4]\n"
                "26:   ASSIGN ^1 j [line 4]\n"
                "27:   ADD j j 1 [line 4]\n"
                "28:   JUMP 19 [line 4]\n"
                "29:   JUMP 32 [line 4]\n"
                "30:   JUMP 32 [line 4]\n"
                "31:   JUMP 26 [line 4]\n"
                "32:   JUMP 8 [line 5]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/loop/for_break_many_nested.asc");    
    GTEST_ASSERT_EQ(expected, actual);             
}


/* ------------- Conditional -------------   */

TEST_F(InterCodeSuite, if_simple) {
    expected =  "1:   IF_EQ x 'true' 3 [line 1]\n"
                "2:   JUMP 3 [line 1]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/conditional/if_simple.asc");    
    GTEST_ASSERT_EQ(expected, actual);            
}

TEST_F(InterCodeSuite, if_expr) {
    expected =  "1:   IF_GREATER x 3 3 [line 1]\n"
                "2:   JUMP 5 [line 1]\n"
                "3:   ASSIGN ^0 'true' [line 1]\n"
                "4:   JUMP 6 [line 1]\n"
                "5:   ASSIGN ^0 'false' [line 1]\n"
                "6:   IF_EQ ^0 'true' 8 [line 1]\n"
                "7:   JUMP 8 [line 1]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/conditional/if_expr.asc");    
    GTEST_ASSERT_EQ(expected, actual);            
}

TEST_F(InterCodeSuite, if_stmts) {
    expected =  "1:   IF_LESS 3 x 3 [line 1]\n"
                "2:   JUMP 5 [line 1]\n"
                "3:   ASSIGN ^0 'true' [line 1]\n"
                "4:   JUMP 6 [line 1]\n"
                "5:   ASSIGN ^0 'false' [line 1]\n"
                "6:   IF_EQ ^0 'true' 8 [line 1]\n"
                "7:   JUMP 10 [line 1]\n"
                "8:   ASSIGN a f [line 5]\n"
                "9:   ASSIGN ^1 a [line 5]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/conditional/if_stmts.asc");    
    GTEST_ASSERT_EQ(expected, actual);              
}

TEST_F(InterCodeSuite, if_nested_many) {
    expected =  "1:   IF_EQ a 'true' 3 [line 1]\n"
                "2:   JUMP 11 [line 1]\n"
                "3:   IF_EQ sz 'true' 5 [line 2]\n"
                "4:   JUMP 11 [line 2]\n"
                "5:   IF_EQ z 'true' 7 [line 3]\n"
                "6:   JUMP 9 [line 3]\n"
                "7:   ASSIGN s 9 [line 4]\n"
                "8:   ASSIGN ^0 s [line 4]\n"
                "9:   ASSIGN k 0 [line 7]\n"
                "10:   ASSIGN ^0 k [line 7]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/conditional/if_nested_many.asc");    
    GTEST_ASSERT_EQ(expected, actual);                  
}

TEST_F(InterCodeSuite, if_else) {
    expected =  "1:   IF_EQ a 'true' 3 [line 1]\n"
                "2:   JUMP 4 [line 1]\n"
                "3:   JUMP 8 [line 2]\n"
                "4:   ASSIGN b 2 [line 2]\n"
                "5:   ASSIGN ^0 b [line 2]\n"
                "6:   ASSIGN a x [line 2]\n"
                "7:   ASSIGN ^0 a [line 2]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/conditional/if_else.asc");    
    GTEST_ASSERT_EQ(expected, actual);            
}

TEST_F(InterCodeSuite, if_elseif) {
    expected =  "1:   IF_EQ 1 'true' 3 [line 1]\n"
                "2:   JUMP 6 [line 1]\n"
                "3:   ASSIGN x 2 [line 1]\n"
                "4:   ASSIGN ^0 x [line 1]\n"
                "5:   JUMP 10 [line 2]\n"
                "6:   IF_EQ 1 'true' 8 [line 2]\n"
                "7:   JUMP 10 [line 2]\n"
                "8:   ASSIGN x 3 [line 2]\n"
                "9:   ASSIGN ^0 x [line 2]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/conditional/if_elseif.asc");    
    GTEST_ASSERT_EQ(expected, actual);            
}

TEST_F(InterCodeSuite, if_elseif_else) {
    expected =  "1:   IF_EQ 1 'true' 3 [line 1]\n"
                "2:   JUMP 6 [line 1]\n"
                "3:   ASSIGN x 2 [line 1]\n"
                "4:   ASSIGN ^0 x [line 1]\n"
                "5:   JUMP 28 [line 2]\n"
                "6:   IF_EQ 1 'true' 8 [line 2]\n"
                "7:   JUMP 11 [line 2]\n"
                "8:   ASSIGN x 3 [line 2]\n"
                "9:   ASSIGN ^0 x [line 2]\n"
                "10:   JUMP 28 [line 3]\n"
                "11:   IF_EQ 1 'true' 13 [line 3]\n"
                "12:   JUMP 16 [line 3]\n"
                "13:   ASSIGN x 3 [line 3]\n"
                "14:   ASSIGN ^0 x [line 3]\n"
                "15:   JUMP 28 [line 4]\n"
                "16:   IF_EQ 1 'true' 18 [line 4]\n"
                "17:   JUMP 21 [line 4]\n"
                "18:   ASSIGN x 3 [line 4]\n"
                "19:   ASSIGN ^0 x [line 4]\n"
                "20:   JUMP 28 [line 5]\n"
                "21:   IF_EQ 1 'true' 23 [line 5]\n"
                "22:   JUMP 26 [line 5]\n"
                "23:   ASSIGN x 3 [line 5]\n"
                "24:   ASSIGN ^0 x [line 5]\n"
                "25:   JUMP 28 [line 6]\n"
                "26:   ASSIGN x 3 [line 6]\n"
                "27:   ASSIGN ^0 x [line 6]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/conditional/if_elseif_else.asc");    
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(InterCodeSuite, if_else_many_nested) {
    expected =  "1:   IF_EQ 1 'true' 3 [line 1]\n"
                "2:   JUMP 16 [line 1]\n"
                "3:   IF_EQ 2 'true' 5 [line 1]\n"
                "4:   JUMP 13 [line 1]\n"
                "5:   IF_EQ a 0 7 [line 1]\n"
                "6:   JUMP 9 [line 1]\n"
                "7:   ASSIGN ^0 'true' [line 1]\n"
                "8:   JUMP 10 [line 1]\n"
                "9:   ASSIGN ^0 'false' [line 1]\n"
                "10:   IF_EQ ^0 'true' 12 [line 1]\n"
                "11:   JUMP 13 [line 1]\n"
                "12:   JUMP 13 [line 1]\n"
                "13:   ASSIGN x 2 [line 1]\n"
                "14:   ASSIGN ^0 x [line 1]\n"
                "15:   JUMP 27 [line 2]\n"
                "16:   IF_GREATER x 0 18 [line 3]\n"
                "17:   JUMP 20 [line 3]\n"
                "18:   ASSIGN ^0 'true' [line 3]\n"
                "19:   JUMP 21 [line 3]\n"
                "20:   ASSIGN ^0 'false' [line 3]\n"
                "21:   IF_EQ ^0 'true' 23 [line 3]\n"
                "22:   JUMP 24 [line 3]\n"
                "23:   JUMP 27 [line 4]\n"
                "24:   IF_EQ x 'true' 26 [line 4]\n"
                "25:   JUMP 27 [line 4]\n"
                "26:   JUMP 27 [line 5]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/conditional/if_else_many_nested.asc");    
    GTEST_ASSERT_EQ(expected, actual);            
}

/*  ------------ Arithmetic --------------   */

TEST_F(InterCodeSuite, arithmetic_simple) {
    expected =  "1:   ADD ^0 x 9 [line 1]\n"
                "2:   SUB ^0 a 0 [line 2]\n"
                "3:   MUL ^0 8 o [line 3]\n"
                "4:   MOD ^0 3 6 [line 4]\n"
                "5:   DIV ^0 1 1 [line 5]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/arithmetic/simple.asc");
    GTEST_ASSERT_EQ(expected, actual); 
}

TEST_F(InterCodeSuite, arithmetic_spaghetti) {
    expected =  "1:   ADD ^0 x 9 [line 1]\n"
                "2:   ADD ^1 ^0 8 [line 1]\n"
                "3:   ADD ^2 ^1 9 [line 1]\n"
                "4:   ADD ^3 ^2 u [line 1]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/arithmetic/spaghetti.asc");
    GTEST_ASSERT_EQ(expected, actual); 
}

TEST_F(InterCodeSuite, arithmetic_errors) {
    expected =  "Error, in line: 4: Invalid use of arithmetic operator on user function f\n"
                "Error, in line: 6: Invalid use of arithmetic operator on const bool \'true\'\n"
                "Error, in line: 6: Invalid use of arithmetic operator on const bool \'false\'\n"
                "Error, in line: 8: Invalid use of arithmetic operator on const string \"a\"\n"
                "Error, in line: 10: Invalid use of arithmetic operator on library function print\n";
                
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/arithmetic/errors.asc");
    GTEST_ASSERT_EQ(expected, actual); 
}

TEST_F(InterCodeSuite, arithmetic_plusplus_suffix) {
    expected =  "1:   ASSIGN ^0 i [line 1]\n"
                "2:   ADD i i 1 [line 1]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/arithmetic/plusplus_suffix.asc");
    GTEST_ASSERT_EQ(expected, actual);            
}

TEST_F(InterCodeSuite, arithmetic_plusplus_prefix) {
    expected =  "1:   ADD i i 1 [line 1]\n"
                "2:   ASSIGN ^0 i [line 1]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/arithmetic/plusplus_prefix.asc");
    GTEST_ASSERT_EQ(expected, actual);             
}

TEST_F(InterCodeSuite, arithmetic_uminus) {
    expected = "1:   UMINUS ^0 1 [line 1]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/arithmetic/uminus.asc");
    GTEST_ASSERT_EQ(expected, actual); 
}

TEST_F(InterCodeSuite, arithmetic_uminus_bool) {
    expected = "Error, in line: 1: Illegal use of unary minus on constant boolean\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/arithmetic/uminus_bool.asc");
    GTEST_ASSERT_EQ(expected, actual); 
}

TEST_F(InterCodeSuite, arithmetic_not) {
    expected =  "1:   IF_EQ 1 'true' 5 [line 1]\n"
                "2:   JUMP 3 [line 1]\n"
                "3:   ASSIGN ^0 'true' [line 1]\n"
                "4:   JUMP 6 [line 1]\n"
                "5:   ASSIGN ^0 'false' [line 1]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/arithmetic/not.asc");
    GTEST_ASSERT_EQ(expected, actual); 
}

TEST_F(InterCodeSuite, arithmetic_minusminus_prefix) {
    expected =  "1:   SUB a a 1 [line 1]\n"
                "2:   ASSIGN ^0 a [line 1]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/arithmetic/minusminus_prefix.asc");
    GTEST_ASSERT_EQ(expected, actual);             
}

TEST_F(InterCodeSuite, arithmetic_minusminus_suffix) {
    expected =  "1:   ASSIGN ^0 a [line 1]\n"
                "2:   SUB a a 1 [line 1]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/arithmetic/minusminus_suffix.asc");
    GTEST_ASSERT_EQ(expected, actual);             
}

/*  ------------ Table --------------   */

TEST_F(InterCodeSuite, table_tablemake_list_single) {
    expected =  "1:   TABLECREATE ^0 [line 1]\n"
                "2:   TABLESETELEM ^0 0 1 [line 1]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/table/tablemake_list_single.asc");
    GTEST_ASSERT_EQ(expected, actual);              
}

TEST_F(InterCodeSuite, table_tablemake_list_many) {
    expected =  "1:   CALL f [line 1]\n"
                "2:   GETRETVAL ^0 [line 1]\n"
                "3:   TABLECREATE ^1 [line 1]\n"
                "4:   TABLESETELEM ^1 0 1 [line 1]\n"
                "5:   TABLESETELEM ^1 1 3 [line 1]\n"
                "6:   TABLESETELEM ^1 2 s [line 1]\n"
                "7:   TABLESETELEM ^1 3 0 [line 1]\n"
                "8:   TABLESETELEM ^1 4 ^0 [line 1]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/table/tablemake_list_many.asc");
    GTEST_ASSERT_EQ(expected, actual);               
}

TEST_F(InterCodeSuite, table_tablemake_pairs_single) {
    expected =  "1:   TABLECREATE ^0 [line 1]\n"
                "2:   TABLESETELEM ^0 x 0 [line 1]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/table/tablemake_pairs_single.asc");
    GTEST_ASSERT_EQ(expected, actual);            
}

TEST_F(InterCodeSuite, table_tablemake_pairs_many) {
    expected =  "1:   TABLECREATE ^0 [line 1]\n"
                "2:   TABLESETELEM ^0 x 0 [line 1]\n"
                "3:   TABLESETELEM ^0 x 0 [line 1]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/table/tablemake_pairs_many.asc");
    GTEST_ASSERT_EQ(expected, actual);            
}

TEST_F(InterCodeSuite, table_tablemake_pairs_macaroni) {
    expected =  "1:   ASSIGN pi 3.14159 [line 1]\n"
                "2:   ASSIGN ^0 pi [line 1]\n"
                "3:   TABLECREATE ^1 [line 1]\n"
                "4:   TABLESETELEM ^1 \"pi\" ^0 [line 1]\n"
                "5:   TABLESETELEM ^1 \"r\" 0 [line 1]\n"
                "6:   TABLESETELEM ^1 \"x\" 0 [line 1]\n"
                "7:   TABLESETELEM ^1 \"y\" 0 [line 1]\n"
                "8:   ASSIGN circle ^1 [line 1]\n"
                "9:   ASSIGN ^2 circle [line 1]\n";
    actual = exec("./d_intermediate_code ../../test/files/phase3_tests/table/tablemake_pairs_macaroni.asc");
    GTEST_ASSERT_EQ(expected, actual);            
}

TEST_F(InterCodeSuite, table_tablemake_nested) {
    expected =  "1:   TABLECREATE ^0 [line 1]\n"
                "2:   TABLESETELEM ^0 0 1 [line 1]\n"
                "3:   TABLESETELEM ^0 1 2 [line 1]\n"
                "4:   TABLESETELEM ^0 2 3 [line 1]\n"
                "5:   TABLECREATE ^1 [line 1]\n"
                "6:   TABLESETELEM ^1 1 0 [line 1]\n"
                "7:   TABLECREATE ^2 [line 1]\n"
                "8:   TABLESETELEM ^2 0 ^1 [line 1]\n"
                "9:   TABLESETELEM ^2 1 0 [line 1]\n"
                "10:   ASSIGN s 2 [line 1]\n"
                "11:   ASSIGN ^3 s [line 1]\n"
                "12:   TABLECREATE ^4 [line 1]\n"
                "13:   TABLESETELEM ^4 ^3 9 [line 1]\n"
                "14:   TABLECREATE ^5 [line 1]\n"
                "15:   TABLESETELEM ^5 0 \"s\" [line 1]\n"
                "16:   TABLESETELEM ^5 1 ^4 [line 1]\n"
                "17:   TABLECREATE ^6 [line 1]\n"
                "18:   TABLESETELEM ^6 0 7 [line 1]\n"
                "19:   TABLECREATE ^7 [line 1]\n"
                "20:   TABLESETELEM ^7 0 ^0 [line 1]\n"
                "21:   TABLESETELEM ^7 1 ^2 [line 1]\n"
                "22:   TABLESETELEM ^7 2 ^5 [line 1]\n"
                "23:   TABLESETELEM ^7 3 ^6 [line 1]\n";
                actual = exec("./d_intermediate_code ../../test/files/phase3_tests/table/tablemake_nested.asc");
                GTEST_ASSERT_EQ(expected, actual);  
}

#ifdef TESTING
int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
#endif
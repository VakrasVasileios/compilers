#include <gtest/gtest.h>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

class QuadTest : public ::testing::Test {
   protected:
    std::string expected;
    std::string actual;  

    void SetUp() override {}
    void TearDown() override {}   

    std::string exec(const char* cmd) {
        std::array<char, 128> buffer;
        std::string result;
        std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
        if (!pipe) {
            throw std::runtime_error("popen() failed!");
        }
        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
            result += buffer.data();
        }
        return result;
    }
};

/*  ------------ Assign --------------   */

TEST_F(QuadTest, assign_simple) {
   expected = "1:   ASSIGN x 2 [line 1]\n";
   actual = exec("./scanner ../../test/files/phase3_tests/assign/simple.asc");
   GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(QuadTest, assign_calls_nested) {
    expected =  "1:   FUNCSTART f [line 1]\n"
                "2:   FUNCEND f [line 2]\n"
                "3:   FUNCSTART g [line 4]\n"
                "4:   FUNCEND g [line 5]\n"
                "5:   PARAM 3 [line 7]\n"
                "6:   CALL g [line 7]\n"
                "7:   GETRETVAL ^0 [line 7]\n"
                "8:   PARAM ^0 [line 7]\n"
                "9:   CALL f [line 7]\n"
                "10:   GETRETVAL ^1 [line 7]\n"
                "11:   ASSIGN x ^1 [line 7]\n";
    actual = exec("./scanner ../../test/files/phase3_tests/assign/calls_nested.asc");
    GTEST_ASSERT_EQ(expected, actual);            
}


/*  ------------ Function --------------   */

TEST_F(QuadTest, functions_single) {
    expected = "1:   FUNCSTART f [line 1]\n"
               "2:   FUNCEND f [line 2]\n";
    actual = exec("./scanner ../../test/files/phase3_tests/functions/single.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(QuadTest, functions_many) {
    expected =  "1:   FUNCSTART f [line 1]\n"
                "2:   FUNCEND f [line 2]\n"
                "3:   FUNCSTART g [line 4]\n"
                "4:   FUNCEND g [line 5]\n"
                "5:   FUNCSTART y [line 8]\n"
                "6:   FUNCEND y [line 9]\n"
                "7:   FUNCSTART r [line 17]\n"
                "8:   FUNCEND r [line 18]\n";
    actual = exec("./scanner ../../test/files/phase3_tests/functions/many.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(QuadTest, functions_nested) {
    expected =  "1:   FUNCSTART a [line 5]\n"
                "2:   FUNCSTART b [line 6]\n"
                "3:   FUNCSTART c [line 7]\n"
                "4:   FUNCSTART d [line 8]\n"
                "5:   FUNCEND d [line 9]\n"
                "6:   FUNCEND c [line 10]\n"
                "7:   FUNCEND b [line 11]\n"
                "8:   FUNCSTART e [line 12]\n"
                "9:   FUNCEND e [line 13]\n"
                "10:   FUNCEND a [line 14]\n"
                "11:   FUNCSTART e [line 16]\n"
                "12:   FUNCEND e [line 27]\n";
    actual = exec("./scanner ../../test/files/phase3_tests/functions/nested.asc");
    GTEST_ASSERT_EQ(expected, actual);            
}

TEST_F(QuadTest, functions_shadowed) {
    expected =  "1:   FUNCSTART lkdland [line 1]\n"
                "2:   FUNCSTART lkdland [line 2]\n"
                "3:   FUNCEND lkdland [line 3]\n"
                "4:   FUNCEND lkdland [line 5]\n";
    actual = exec("./scanner ../../test/files/phase3_tests/functions/shadowed.asc");
    GTEST_ASSERT_EQ(expected, actual);            
}

TEST_F(QuadTest, functions_anonymous) {
    expected =  "1:   FUNCSTART $1 [line 1]\n"
                "2:   FUNCSTART c [line 2]\n"
                "3:   FUNCEND c [line 3]\n"
                "4:   FUNCSTART $2 [line 4]\n"
                "5:   FUNCEND $2 [line 5]\n"
                "6:   FUNCEND $1 [line 6]\n";
    actual = exec("./scanner ../../test/files/phase3_tests/functions/anonymous.asc");
    GTEST_ASSERT_EQ(expected, actual);              
}

TEST_F(QuadTest, functions_call_warning) {
    expected =  "1:   CALL g [line 1]\n"
                "2:   GETRETVAL ^0 [line 1]\n";
    actual = exec("./scanner ../../test/files/phase3_tests/functions/call_warning.asc");
    GTEST_ASSERT_EQ(expected, actual);  
}

TEST_F(QuadTest, functions_call_noparameters) {
    expected =  "1:   FUNCSTART f [line 1]\n"
                "2:   FUNCEND f [line 2]\n"
                "3:   CALL f [line 4]\n"
                "4:   GETRETVAL ^0 [line 4]\n";
    actual = exec("./scanner ../../test/files/phase3_tests/functions/call_no_parameters.asc");
    GTEST_ASSERT_EQ(expected, actual);          
}

TEST_F(QuadTest, functions_call_parameters) {
    expected =  "1:   FUNCSTART f [line 1]\n"
                "2:   FUNCEND f [line 2]\n"
                "3:   PARAM 0 [line 4]\n"
                "4:   CALL f [line 4]\n"
                "5:   GETRETVAL ^0 [line 4]\n";
    actual = exec("./scanner ../../test/files/phase3_tests/functions/call_parameters.asc");
    GTEST_ASSERT_EQ(expected, actual);               
}

TEST_F(QuadTest, functions_call_with_symbol_parameters) {
    expected =  "Warning, in line: 10: Too many arguments passed to function: f, defined in line: 1\n"
                "Warning, in line: 11: Too many arguments passed to function: f, defined in line: 1\n"
                "Warning, in line: 12: Too many arguments passed to function: f, defined in line: 1\n"
                "1:   FUNCSTART f [line 1]\n"
                "2:   FUNCEND f [line 2]\n"
                "3:   PARAM c [line 5]\n"
                "4:   CALL f [line 5]\n"
                "5:   GETRETVAL ^0 [line 5]\n"
                "6:   PARAM 0 [line 10]\n"
                "7:   PARAM c [line 10]\n"
                "8:   CALL f [line 10]\n"
                "9:   GETRETVAL ^0 [line 10]\n"
                "10:   PARAM 3 [line 11]\n"
                "11:   PARAM 10 [line 11]\n"
                "12:   PARAM c [line 11]\n"
                "13:   CALL f [line 11]\n"
                "14:   GETRETVAL ^0 [line 11]\n"
                "15:   PARAM aaaaa [line 12]\n"
                "16:   PARAM aaaaa [line 12]\n"
                "17:   PARAM a [line 12]\n"
                "18:   PARAM 0 [line 12]\n"
                "19:   PARAM c [line 12]\n"
                "20:   CALL f [line 12]\n"
                "21:   GETRETVAL ^0 [line 12]\n";
    actual = exec("./scanner ../../test/files/phase3_tests/functions/call_with_symbol_parameters.asc");
    GTEST_ASSERT_EQ(expected, actual);            
}

TEST_F(QuadTest, functions_call_less_parameters) {
    expected = "Error, in line: 4: Too few arguments passed to function: f, defined in line: 1\n";
    actual = exec("./scanner ../../test/files/phase3_tests/functions/call_less_parameters.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(QuadTest, functions_call_nested) {
    expected =  "Warning, in line: 10: Too many arguments passed to function: g, defined in line: 6\n"
                "Warning, in line: 12: Too many arguments passed to function: g, defined in line: 6\n"
                "1:   FUNCSTART f [line 2]\n"
                "2:   FUNCEND f [line 4]\n"
                "3:   FUNCSTART g [line 6]\n"
                "4:   FUNCEND g [line 8]\n"
                "5:   PARAM 1 [line 10]\n"
                "6:   CALL g [line 10]\n"
                "7:   GETRETVAL ^0 [line 10]\n"
                "8:   PARAM ^0 [line 10]\n"
                "9:   CALL f [line 10]\n"
                "10:   GETRETVAL ^1 [line 10]\n"
                "11:   PARAM 2 [line 12]\n"
                "12:   CALL y [line 12]\n"
                "13:   GETRETVAL ^0 [line 12]\n"
                "14:   PARAM ^0 [line 12]\n"
                "15:   CALL g [line 12]\n"
                "16:   GETRETVAL ^1 [line 12]\n"
                "17:   PARAM ^1 [line 12]\n"
                "18:   CALL f [line 12]\n"
                "19:   GETRETVAL ^2 [line 12]\n";
    actual = exec("./scanner ../../test/files/phase3_tests/functions/call_nested.asc");
    GTEST_ASSERT_EQ(expected, actual);            
}

TEST_F(QuadTest, functions_call_more_parameters) {
    expected =  "Warning, in line: 5: Too many arguments passed to function: f, defined in line: 1\n"
                "1:   FUNCSTART f [line 1]\n"
                "2:   FUNCEND f [line 3]\n"
                "3:   PARAM 2 [line 5]\n"
                "4:   PARAM a [line 5]\n"
                "5:   CALL f [line 5]\n"
                "6:   GETRETVAL ^0 [line 5]\n";
    actual = exec("./scanner ../../test/files/phase3_tests/functions/call_more_parameters.asc");
    GTEST_ASSERT_EQ(expected, actual);            
}

TEST_F(QuadTest, functions_call_function_def) {
    expected =  "1:   FUNCSTART f [line 1]\n"
                "2:   FUNCEND f [line 1]\n"
                "3:   PARAM 1 [line 1]\n"
                "4:   CALL f [line 1]\n"
                "5:   GETRETVAL ^0 [line 1]\n";
    actual = exec("./scanner ../../test/files/phase3_tests/functions/call_function_def.asc");
    GTEST_ASSERT_EQ(expected, actual);        
}

TEST_F(QuadTest, functions_call_nested_with_func_def) {
    expected =  "1:   FUNCSTART x [line 1]\n"
                "2:   FUNCEND x [line 1]\n"
                "3:   PARAM d [line 1]\n"
                "4:   CALL x [line 1]\n"
                "5:   GETRETVAL ^0 [line 1]\n"
                "6:   PARAM ^0 [line 1]\n"
                "7:   CALL g [line 1]\n"
                "8:   GETRETVAL ^1 [line 1]\n";
    actual = exec("./scanner ../../test/files/phase3_tests/functions/call_nested_with_func_def.asc");
    GTEST_ASSERT_EQ(expected, actual);              
}

TEST_F(QuadTest, functions_call_anonymous_func_def) {
    expected =  "1:   FUNCSTART $1 [line 1]\n"
                "2:   FUNCEND $1 [line 1]\n"
                "3:   PARAM 4 [line 1]\n"
                "4:   PARAM 1 [line 1]\n"
                "5:   CALL $1 [line 1]\n"
                "6:   GETRETVAL ^0 [line 1]\n";
    actual = exec("./scanner ../../test/files/phase3_tests/functions/call_anonymous_func_def.asc");
    GTEST_ASSERT_EQ(expected, actual);
}
/*  ------------ Return --------------   */

// TEST_F(QuadTest, return_void) {
//     expected =  "1:   FUNCSTART f [line 1]\n"
//                 "2:   RETURN  [line 2]\n"
//                 "3:   JUMP 5 [line 2]\n"
//                 "4:   FUNCEND f [line 3]\n";
//     actual = exec("./scanner ../test/files/phase3_tests/return/return_void.asc");
//     GTEST_ASSERT_EQ(expected, actual);            
// }


// TEST_F(QuadTest, return_symbol) {
//     expected =  "1:   FUNCSTART f [line 3]\n"
//                 "2:   RETURN x [line 4]\n"
//                 "3:   JUMP 5 [line 4]\n"
//                 "4:   FUNCEND f [line 5]\n";
//     actual = exec("./scanner ../test/files/phase3_tests/return/return_symbol.asc");
//     GTEST_ASSERT_EQ(expected, actual);            
//}

/*  ------------ Arithmetic --------------   */

// TEST_F(QuadTest, arithmetic_simple) {
//     expected =  "1:   ADD ^0 x 9 [line 1]\n"
//                 "2:   SUB ^0 a 0 [line 2]\n"
//                 "3:   MUL ^0 8 o [line 3]\n"
//                 "4:   MOD ^0 3 6 [line 4]\n"
//                 "5:   DIV ^0 1 1 [line 5]\n";
//     actual = exec("./scanner ../test/files/phase3_tests/arithmetic/simple.asc");
//     GTEST_ASSERT_EQ(expected, actual); 
// }

// TEST_F(QuadTest, arithmetic_spaghetti) {
//     expected =  "1:   ADD ^0 x 9 [line 1]\n"
//                 "2:   ADD ^0 ^0 8 [line 1]\n"
//                 "3:   ADD ^0 ^0 9 [line 1]\n"
//                 "4:   ADD ^0 ^0 u [line 1]\n";
//     actual = exec("./scanner ../test/files/phase3_tests/arithmetic/spaghetti.asc");
//     GTEST_ASSERT_EQ(expected, actual); 
// }

// TEST_F(QuadTest, arithmetic_function_id_error) {
//     expected =  "Error, in line: 4: Use of addition with non variable type\n";
//     actual = exec("./scanner ../test/files/phase3_tests/arithmetic/function_id_error.asc");
//     GTEST_ASSERT_EQ(expected, actual); 
// }

#ifdef TESTING
int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
#endif
#include <gtest/gtest.h>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

/*
Provides a test class for testing the output of the parser.
*/
class ParserTest : public ::testing::Test {
   protected:
      /*
      The expected output.
      */
      std::string expected;
      /*
      The actual output.
      */
      std::string actual;
      /*
      The output of the parsing of the library functions.
      */
      std::string libfunc_out;
      /*
      Runs before every test.
      */
      void SetUp() override {
         libfunc_out ="-----------     Scope #0     -----------\n"
                    "[library function] \"print\" (line 0) (scope 0)\n"
                    "[library function] \"input\" (line 0) (scope 0)\n"
                    "[library function] \"objectmemberkeys\" (line 0) (scope 0)\n"
                    "[library function] \"objecttotalmembers\" (line 0) (scope 0)\n"
                    "[library function] \"objectcopy\" (line 0) (scope 0)\n"
                    "[library function] \"totalarguments\" (line 0) (scope 0)\n"
                    "[library function] \"argument\" (line 0) (scope 0)\n"
                    "[library function] \"typeof\" (line 0) (scope 0)\n"
                    "[library function] \"strtonum\" (line 0) (scope 0)\n"
                    "[library function] \"sqrt\" (line 0) (scope 0)\n"
                    "[library function] \"cos\" (line 0) (scope 0)\n"
                    "[library function] \"sin\" (line 0) (scope 0)\n";
      }
      /*
      Runs after every test.
      */
      void TearDown() override {}   
      /*
      Executes a comand on the current path.
      */
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

TEST_F(ParserTest, Error0) {
  expected = "Error, in line: 4: No global variable with id: Global\n"
          "Warning, in line: 4: Attempting to assign a value to NIL\n";
  actual = exec("./scanner ../test/files/phase2_tests/Errors/Error0.asc");
  GTEST_ASSERT_EQ(expected, actual);
}

 TEST_F(ParserTest, Error1) {
  expected = "Error, in line: 6: Cannot access f, previously defined in line: 4\n"
          "Error, in line: 7: Cannot access f, previously defined in line: 4\n";
  actual = exec("./scanner ../test/files/phase2_tests/Errors/Error1.asc");
  GTEST_ASSERT_EQ(expected, actual);
 }

TEST_F(ParserTest, Error2) {
  expected = "Error, in line: 8: Invalid return, used outside a function block\n"
          "Error, in line: 9: invalid keyword BREAK outside of loop\n"
          "Error, in line: 10: invalid keyword CONTINUE outside of loop\n"
          "Error, in line: 15: Invalid return, used outside a function block\n"
          "Error, in line: 16: invalid keyword BREAK outside of loop\n"
          "Error, in line: 17: invalid keyword CONTINUE outside of loop\n";
  actual = exec("./scanner ../test/files/phase2_tests/Errors/Error2.asc");    
  GTEST_ASSERT_EQ(expected, actual);
 }

TEST_F(ParserTest, Error3) {
  expected = "Error, in line: 10: Cannot access x, previously defined in line: 4\n";
  actual = exec("./scanner ../test/files/phase2_tests/Errors/Error3.asc");
   GTEST_ASSERT_EQ(expected, actual);
 }

TEST_F(ParserTest, Error4) {
  expected = "Error, in line: 4: x variable, previously defined in line: 3, cannot be redefined as a function\n";
  actual = exec("./scanner ../test/files/phase2_tests/Errors/Error4.asc");
  GTEST_ASSERT_EQ(expected, actual);
 }

TEST_F(ParserTest, Error5) {
  expected = "Error, in line: 5: Name collision with function f, previously defined in line: 4\n";
  actual = exec("./scanner ../test/files/phase2_tests/Errors/Error5.asc");
  GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(ParserTest, Error6) {
  expected = "Error, in line: 11: Cannot access x, previously defined in line: 6\n"
          "Error, in line: 12: Cannot access y, previously defined in line: 6\n";
  actual = exec("./scanner ../test/files/phase2_tests/Errors/Error6.asc");
  GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(ParserTest, Error7) {
  expected = "Error, in line: 4: Functions are constant their value cannot be changed\n"
          "Error, in line: 5: Functions are constant their value cannot be changed\n"
          "Error, in line: 6: Use of increment operator with non variable type\n"
          "Error, in line: 7: Use of decrement operator with non variable type\n";
  actual = exec("./scanner ../test/files/phase2_tests/Errors/Error7.asc");
  GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(ParserTest, Error8) {
  expected = "Error, in line: 3: x variable, previously defined in line: 1, cannot be redefined as a function\n";
  actual = exec("./scanner ../test/files/phase2_tests/Errors/Error8.asc");
  GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(ParserTest, Error9) {
  expected = "Error, in line: 4: Cannot access x, previously defined in line: 1\n"; 
  actual = exec("./scanner ../test/files/phase2_tests/Errors/Error9.asc");
   GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(ParserTest, Error10) {
  expected =  "Error, in line: 6: print library function cannot be shadowed by a user function\n"
          "Error, in line: 11: Attempting to redefine a library function\n";
  actual = exec("./scanner ../test/files/phase2_tests/Errors/Error10.asc");
  GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(ParserTest, Error11) {
  expected = "Error, formal argument x already declared, in line: 1\n";
  actual = exec("./scanner ../test/files/phase2_tests/Errors/Error11.asc");
  GTEST_ASSERT_EQ(expected, actual);
}

 TEST_F(ParserTest, Anonymous) {
   expected = libfunc_out;
   expected += "[user function] \"$1\" (line 5) (scope 0)\n"
            "[user function] \"$5\" (line 28) (scope 0)\n"
            "-----------     Scope #1     -----------\n"
            "[formal variable] \"x\" (line 5) (scope 1)\n"
            "[formal variable] \"AnonymousOne\" (line 5) (scope 1)\n"
            "[user function] \"$2\" (line 10) (scope 1)\n"
            "[formal variable] \"x\" (line 28) (scope 1)\n"
            "[formal variable] \"AnonymousFive\" (line 28) (scope 1)\n"
            "-----------     Scope #2     -----------\n"
            "[formal variable] \"AnonymousTwo\" (line 10) (scope 2)\n"
            "[user function] \"$3\" (line 15) (scope 2)\n"
            "[user function] \"$4\" (line 20) (scope 2)\n"
            "-----------     Scope #3     -----------\n"
            "[formal variable] \"AnonymousThree\" (line 15) (scope 3)\n"
            "[formal variable] \"AnonymousFour\" (line 20) (scope 3)\n";
   actual = exec("./scanner ../test/files/phase2_tests/Working/Anonymous.asc");
   GTEST_ASSERT_EQ(expected, actual);
 }

 TEST_F(ParserTest, Block) {
   expected = libfunc_out;
   expected +="-----------     Scope #1     -----------\n"
            "[local variable] \"x\" (line 2) (scope 1)\n";
   actual = exec("./scanner ../test/files/phase2_tests/Working/Block.asc");
   GTEST_ASSERT_EQ(expected, actual);
 }

//  TEST_F(ParserTest, Circle) {
//     expected = libfunc_out;
//     expected += "[global variable] \"nl\" (line 2) (scope 0)\n"
//             "[local variable] \"$0\" (line 0) (scope 0)\n"
//             "[global variable] \"circle\" (line 6) (scope 0)\n"
//             "[global variable] \"pi\" (line 6) (scope 0)\n"
//             "[local variable] \"$1\" (line 0) (scope 0)\n"
//             "[local variable] \"$2\" (line 0) (scope 0)\n"
//             "[user function] \"Area\" (line 11) (scope 0)\n"
//             "[user function] \"Perimeter\" (line 18) (scope 0)\n"
//             "[user function] \"InitCircle\" (line 25) (scope 0)\n";
//     actual = exec("./scanner ../test/files/phase2_tests/Working/Circle.asc");
//     GTEST_ASSERT_EQ(expected, actual);
//  }

TEST_F(ParserTest, GlobalAndLocal) {
  expected = libfunc_out;
  expected += "[global variable] \"global\" (line 2) (scope 0)\n"
          "[user function] \"f\" (line 6) (scope 0)\n"
          "[hidden variable] \"^0\" (line 0) (scope 0)\n"
          "[hidden variable] \"^1\" (line 0) (scope 0)\n"
          "[hidden variable] \"^2\" (line 0) (scope 0)\n"
          "-----------     Scope #1     -----------\n"
          "[local variable] \"global\" (line 9) (scope 1)\n";
  actual = exec("./scanner ../test/files/phase2_tests/Working/GlobalAndLocal.asc");
  GTEST_ASSERT_EQ(expected, actual); 
}

 TEST_F(ParserTest, Grammar) { //??????????
   expected = libfunc_out;
   expected +="[global variable] \"a1\" (line 2) (scope 0)\n"
         "[global variable] \"a2\" (line 2) (scope 0)\n"
         "[global variable] \"a3\" (line 2) (scope 0)\n"
         "[global variable] \"a4\" (line 2) (scope 0)\n"
         "[local variable] \"$0\" (line 0) (scope 0)\n"
         "[local variable] \"$1\" (line 0) (scope 0)\n"
         "[local variable] \"$2\" (line 0) (scope 0)\n"
         "[local variable] \"$3\" (line 0) (scope 0)\n"
         "[global variable] \"global_var\" (line 4) (scope 0)\n"
         "[user function] \"foo\" (line 10) (scope 0)\n"
         "[user function] \"foo1\" (line 17) (scope 0)\n"
         "[global variable] \"i\" (line 41) (scope 0)\n"
         "[global variable] \"object\" (line 71) (scope 0)\n"
         "[user function] \"$0\" (line 83) (scope 0)\n"
         "[user function] \"test\" (line 85) (scope 0)\n"
         "[user function] \"test1\" (line 98) (scope 0)\n"
         "\n"
         "-----------     Scope #1     -----------\n"
         "[local variable] \"a1\" (line 7) (scope 1)\n"
         "[local variable] \"$0\" (line 0) (scope 1)\n"
         "[local variable] \"$0\" (line 0) (scope 1)\n"
         "[local variable] \"global_var\" (line 18) (scope 1)\n"
         "[local variable] \"$0\" (line 0) (scope 1)\n"
         "[local variable] \"$3\" (line 0) (scope 1)\n"
         "[local variable] \"$0\" (line 0) (scope 1)\n"
         "[local variable] \"$1\" (line 0) (scope 1)\n"
         "[local variable] \"j\" (line 65) (scope 1)\n"
         "[local variable] \"$1\" (line 0) (scope 1)\n"
         "[local variable] \"$2\" (line 0) (scope 1)\n"
         "[user function] \"test1\" (line 86) (scope 1)\n"
         "[local variable] \"$0\" (line 0) (scope 1)\n"
         "[local variable] \"$1\" (line 0) (scope 1)\n"
         "[local variable] \"$2\" (line 0) (scope 1)\n"
         "\n"
         "-----------     Scope #2     -----------\n"
         "[local variable] \"$1\" (line 0) (scope 2)\n"
         "[local variable] \"$0\" (line 0) (scope 2)\n";
   actual = exec("./scanner ../test/files/phase2_tests/Working/Grammar.asc");
   GTEST_ASSERT_EQ(expected, actual); 
}

 TEST_F(ParserTest, Random) { //?
   expected = libfunc_out;
   expected += "[global variable] \"nl\" (line 2) (scope 0)\n"
            "[user function] \"Assert\" (line 7) (scope 0)\n"
            "[user function] \"IsEven\" (line 15) (scope 0)\n"
            "[user function] \"IsOdd\" (line 22) (scope 0)\n"
            "[user function] \"CanDivideByTen\" (line 29) (scope 0)\n"
            "[user function] \"PrintParityOfNumbers\" (line 36) (scope 0)\n"
            "[local variable] \"even\" (line 56) (scope 0)\n"
            "[local variable] \"odd\" (line 56) (scope 0)\n"
            "[local variable] \"cnt\" (line 56) (scope 0)\n"
            "[user function] \"Print\" (line 68) (scope 0)\n"
            "-----------     Scope #1     -----------\n"
            "[formal variable] \"con\" (line 7) (scope 1)\n"
            "[formal variable] \"num\" (line 15) (scope 1)\n"
            "[formal variable] \"num\" (line 22) (scope 1)\n"
            "[formal variable] \"num\" (line 29) (scope 1)\n"
            "[formal variable] \"MAX\" (line 36) (scope 1)\n"
            "[local variable] \"exit\" (line 37) (scope 1)\n"
            "[local variable] \"i\" (line 39) (scope 1)\n";
   actual = exec("./scanner ../test/files/phase2_tests/Working/Random.asc");
   GTEST_ASSERT_EQ(expected, actual);
 }

 TEST_F(ParserTest, ShadowedNameOffFunctions) {
   expected = libfunc_out;
   expected +="[global variable] \"nl\" (line 2) (scope 0)\n"
            "[user function] \"F\" (line 6) (scope 0)\n"
            "-----------     Scope #1     -----------\n"
            "[user function] \"F\" (line 11) (scope 1)\n"
            "-----------     Scope #2     -----------\n"
            "[user function] \"F\" (line 16) (scope 2)\n"
            "-----------     Scope #3     -----------\n"
            "[user function] \"F\" (line 21) (scope 3)\n";
   actual = exec("./scanner ../test/files/phase2_tests/Working/ShadowedNameOffunctions.asc");
   GTEST_ASSERT_EQ(expected, actual);
 }

 TEST_F(ParserTest, Simple) {
   expected = libfunc_out;
   expected +="[global variable] \"globalVarFirst\" (line 2) (scope 0)\n"
         "[local variable] \"$0\" (line 0) (scope 0)\n"
         "[user function] \"GlobalFunction\" (line 6) (scope 0)\n"
         "[global variable] \"globalVarSecond\" (line 22) (scope 0)\n"
         "[local variable] \"$1\" (line 0) (scope 0)\n"
         "[user function] \"F\" (line 34) (scope 0)\n"
         "[global variable] \"a\" (line 40) (scope 0)\n"
         "[global variable] \"b\" (line 40) (scope 0)\n"
         "[global variable] \"x\" (line 40) (scope 0)\n"
         "[global variable] \"y\" (line 40) (scope 0)\n"
         "[local variable] \"$2\" (line 0) (scope 0)\n"
         "[local variable] \"$3\" (line 0) (scope 0)\n"
         "[local variable] \"$4\" (line 0) (scope 0)\n"
         "[local variable] \"$5\" (line 0) (scope 0)\n"
         "[global variable] \"G\" (line 44) (scope 0)\n"
         "[global variable] \"lol\" (line 48) (scope 0)\n"
         "[global variable] \"print_\" (line 53) (scope 0)\n"
         "\n"
         "-----------     Scope #1     -----------\n"
         "[local variable] \"localVar\" (line 8) (scope 1)\n"
         "[local variable] \"$0\" (line 0) (scope 1)\n"
         "[user function] \"LocalFunction\" (line 14) (scope 1)\n"
         "[local variable] \"$1\" (line 0) (scope 1)\n"
         "[local variable] \"localVar\" (line 27) (scope 1)\n"
         "[local variable] \"$0\" (line 0) (scope 1)\n"
         "\n"
         "-----------     Scope #2     -----------\n"
         "[formal argument] \"x\" (line 14) (scope 2)\n"
         "[formal argument] \"y\" (line 14) (scope 2)\n"
         "[local variable] \"globalVarFirst\" (line 16) (scope 2)\n"
         "[local variable] \"$0\" (line 0) (scope 2)\n";
   actual = exec("./scanner ../test/files/phase2_tests/Working/Simple.asc");
    GTEST_ASSERT_EQ(expected, actual);
 }

 TEST_F(ParserTest, Tree) {
   expected = "syntax error: at line 1, before token: /\n"
            "INPUT NOT VALID\n";
   expected += libfunc_out;
   actual = exec("./scanner ../test/files/phase2_tests/Working/Tree.asc");
   GTEST_ASSERT_EQ(expected, actual);
 }

#ifdef TESTING
int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
#endif
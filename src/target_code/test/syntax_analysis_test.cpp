#include <gtest/gtest.h>
#include "../../../util/exec/include/exec.h"

/**
 * @brief Test suite for the syntax analysis.
 * 
**/ 
class SyntaxAnalysisSuite : public ::testing::Test {
protected:
  std::string expected;
  std::string actual;
  std::string libfunc_out;

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
};

TEST_F(SyntaxAnalysisSuite, Error0) {
  expected = "\x1B[31mError, in line: 4:\x1B[0m No global variable with id: Global\n";
  actual = util::exec::exec("../src/d_syntax_analysis  files/phase2_tests/Errors/Error0.asc");
  GTEST_ASSERT_EQ(expected, actual);
}

 TEST_F(SyntaxAnalysisSuite, Error1) {
  expected = "\x1B[31mError, in line: 6:\x1B[0m Cannot access f, previously defined in line: 4\n"
          "\x1B[31mError, in line: 7:\x1B[0m Cannot access f, previously defined in line: 4\n";
  actual = util::exec::exec("../src/d_syntax_analysis  files/phase2_tests/Errors/Error1.asc");
  GTEST_ASSERT_EQ(expected, actual);
 }

TEST_F(SyntaxAnalysisSuite, Error2) {
  expected = "\x1B[31mError, in line: 8:\x1B[0m Invalid return, used outside a function block\n"
          "\x1B[31mError, in line: 9:\x1B[0m invalid keyword BREAK outside of loop\n"
          "\x1B[31mError, in line: 10:\x1B[0m invalid keyword CONTINUE outside of loop\n"
          "\x1B[31mError, in line: 15:\x1B[0m Invalid return, used outside a function block\n"
          "\x1B[31mError, in line: 16:\x1B[0m invalid keyword BREAK outside of loop\n"
          "\x1B[31mError, in line: 17:\x1B[0m invalid keyword CONTINUE outside of loop\n";
  actual = util::exec::exec("../src/d_syntax_analysis  files/phase2_tests/Errors/Error2.asc");    
  GTEST_ASSERT_EQ(expected, actual);
 }

TEST_F(SyntaxAnalysisSuite, Error3) {
  expected = "\x1B[31mError, in line: 10:\x1B[0m Cannot access x, previously defined in line: 4\n";
  actual = util::exec::exec("../src/d_syntax_analysis  files/phase2_tests/Errors/Error3.asc");
   GTEST_ASSERT_EQ(expected, actual);
 }

TEST_F(SyntaxAnalysisSuite, Error4) {
  expected = "\x1B[31mError, in line: 4:\x1B[0m x variable, previously defined in line: 3, cannot be redefined as a function\n";
  actual = util::exec::exec("../src/d_syntax_analysis  files/phase2_tests/Errors/Error4.asc");
  GTEST_ASSERT_EQ(expected, actual);
 }

TEST_F(SyntaxAnalysisSuite, Error5) {
  expected = "\x1B[31mError, in line: 5:\x1B[0m Name collision with function f, previously defined in line: 4\n";
  actual = util::exec::exec("../src/d_syntax_analysis  files/phase2_tests/Errors/Error5.asc");
  GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(SyntaxAnalysisSuite, Error6) {
  expected = "\x1B[31mError, in line: 11:\x1B[0m Cannot access x, previously defined in line: 6\n"
          "\x1B[31mError, in line: 12:\x1B[0m Cannot access y, previously defined in line: 6\n";
  actual = util::exec::exec("../src/d_syntax_analysis  files/phase2_tests/Errors/Error6.asc");
  GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(SyntaxAnalysisSuite, Error7) {
  expected = "\x1B[31mError, in line: 4:\x1B[0m Functions are constant their value cannot be changed\n"
          "\x1B[31mError, in line: 5:\x1B[0m Functions are constant their value cannot be changed\n"
          "\x1B[31mError, in line: 6:\x1B[0m Invalid use of arithmetic operator on library function print\n"
          "\x1B[31mError, in line: 7:\x1B[0m Invalid use of arithmetic operator on user function foo\n";
  actual = util::exec::exec("../src/d_syntax_analysis  files/phase2_tests/Errors/Error7.asc");
  GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(SyntaxAnalysisSuite, Error8) {
  expected = "\x1B[31mError, in line: 3:\x1B[0m x variable, previously defined in line: 1, cannot be redefined as a function\n";
  actual = util::exec::exec("../src/d_syntax_analysis  files/phase2_tests/Errors/Error8.asc");
  GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(SyntaxAnalysisSuite, Error9) {
  expected = "\x1B[31mError, in line: 4:\x1B[0m Cannot access x, previously defined in line: 1\n"; 
  actual = util::exec::exec("../src/d_syntax_analysis  files/phase2_tests/Errors/Error9.asc");
   GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(SyntaxAnalysisSuite, Error10) {
  expected =  "\x1B[31mError, in line: 6:\x1B[0m print library function cannot be shadowed by a user function\n"
              "\x1B[31mError, in line: 11:\x1B[0m Attempting to redefine a library function\n";
  actual = util::exec::exec("../src/d_syntax_analysis  files/phase2_tests/Errors/Error10.asc");
  GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(SyntaxAnalysisSuite, Error11) {
  expected = "\x1B[31mError, in line: 1:\x1B[0m formal argument x already declared\n";
  actual = util::exec::exec("../src/d_syntax_analysis  files/phase2_tests/Errors/Error11.asc");
  GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(SyntaxAnalysisSuite, Anonymous) {
  expected = libfunc_out;
  expected += "[user function] \"$0\" (line 5) (scope 0)\n"
          "[user function] \"$4\" (line 28) (scope 0)\n"
          "-----------     Scope #1     -----------\n"
          "[formal variable] \"x\" (line 5) (scope 1)\n"
          "[formal variable] \"AnonymousOne\" (line 5) (scope 1)\n"
          "[user function] \"$1\" (line 10) (scope 1)\n"
          "[formal variable] \"x\" (line 28) (scope 1)\n"
          "[formal variable] \"AnonymousFive\" (line 28) (scope 1)\n"
          "-----------     Scope #2     -----------\n"
          "[formal variable] \"AnonymousTwo\" (line 10) (scope 2)\n"
          "[user function] \"$2\" (line 15) (scope 2)\n"
          "[user function] \"$3\" (line 20) (scope 2)\n"
          "-----------     Scope #3     -----------\n"
          "[formal variable] \"AnonymousThree\" (line 15) (scope 3)\n"
          "[formal variable] \"AnonymousFour\" (line 20) (scope 3)\n";
  actual = util::exec::exec("../src/d_syntax_analysis  files/phase2_tests/Working/Anonymous.asc");
  GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(SyntaxAnalysisSuite, Block) {
   expected = libfunc_out;
   expected +="-----------     Scope #1     -----------\n"
            "[local variable] \"x\" (line 2) (scope 1)\n"
            "[local variable] \"^0\" (line 0) (scope 1)\n";
   actual = util::exec::exec("../src/d_syntax_analysis  files/phase2_tests/Working/Block.asc");
   GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(SyntaxAnalysisSuite, GlobalAndLocal) {
  expected += libfunc_out;
  expected +=   "[global variable] \"global\" (line 2) (scope 0)\n"
                "[global variable] \"^0\" (line 0) (scope 0)\n"
                "[user function] \"f\" (line 6) (scope 0)\n"
                "[global variable] \"^1\" (line 0) (scope 0)\n"
                "[global variable] \"^2\" (line 0) (scope 0)\n"
                "-----------     Scope #1     -----------\n"
                "[local variable] \"global\" (line 9) (scope 1)\n";
          
  actual = util::exec::exec("../src/d_syntax_analysis  files/phase2_tests/Working/GlobalAndLocal.asc");
  GTEST_ASSERT_EQ(expected, actual); 
}

TEST_F(SyntaxAnalysisSuite, Random) { 
  expected += libfunc_out;
  expected += "[global variable] \"nl\" (line 2) (scope 0)\n"
              "[global variable] \"^0\" (line 0) (scope 0)\n"
              "[user function] \"Assert\" (line 7) (scope 0)\n"
              "[user function] \"IsEven\" (line 15) (scope 0)\n"
              "[user function] \"IsOdd\" (line 22) (scope 0)\n"
              "[user function] \"CanDivideByTen\" (line 29) (scope 0)\n"
              "[user function] \"PrintParityOfNumbers\" (line 36) (scope 0)\n"
              "[global variable] \"even\" (line 56) (scope 0)\n"
              "[global variable] \"odd\" (line 56) (scope 0)\n"
              "[global variable] \"^1\" (line 0) (scope 0)\n"
              "[global variable] \"cnt\" (line 56) (scope 0)\n"
              "[global variable] \"^2\" (line 0) (scope 0)\n"
              "[global variable] \"^3\" (line 0) (scope 0)\n"
              "[global variable] \"^4\" (line 0) (scope 0)\n"
              "[global variable] \"^5\" (line 0) (scope 0)\n"
              "[global variable] \"^6\" (line 0) (scope 0)\n"
              "[user function] \"Print\" (line 68) (scope 0)\n"
              "-----------     Scope #1     -----------\n"
              "[formal variable] \"con\" (line 7) (scope 1)\n"
              "[local variable] \"^1\" (line 0) (scope 1)\n"
              "[formal variable] \"num\" (line 15) (scope 1)\n"
              "[local variable] \"^1\" (line 0) (scope 1)\n"
              "[formal variable] \"num\" (line 22) (scope 1)\n"
              "[local variable] \"^2\" (line 0) (scope 1)\n"
              "[local variable] \"^3\" (line 0) (scope 1)\n"
              "[formal variable] \"num\" (line 29) (scope 1)\n"
              "[local variable] \"^4\" (line 0) (scope 1)\n"
              "[formal variable] \"MAX\" (line 36) (scope 1)\n"
              "[local variable] \"exit\" (line 37) (scope 1)\n"
              "[local variable] \"^5\" (line 0) (scope 1)\n"
              "[local variable] \"i\" (line 39) (scope 1)\n"
              "[local variable] \"^1\" (line 0) (scope 1)\n"
              "[local variable] \"^2\" (line 0) (scope 1)\n"
              "[local variable] \"^3\" (line 0) (scope 1)\n"
              "[local variable] \"^7\" (line 0) (scope 1)\n"
              "[local variable] \"^8\" (line 0) (scope 1)\n"
              "-----------     Scope #2     -----------\n"
              "[local variable] \"^4\" (line 0) (scope 2)\n"
              "[local variable] \"^9\" (line 0) (scope 2)\n";
  actual = util::exec::exec("../src/d_syntax_analysis  files/phase2_tests/Working/Random.asc");
   GTEST_ASSERT_EQ(expected, actual);
 }

TEST_F(SyntaxAnalysisSuite, ShadowedNameOffFunctions) {
  expected += libfunc_out;
  expected +=   "[global variable] \"nl\" (line 2) (scope 0)\n"
                "[global variable] \"^0\" (line 0) (scope 0)\n"
                "[user function] \"F\" (line 6) (scope 0)\n"
                "[global variable] \"^1\" (line 0) (scope 0)\n"
                "[global variable] \"^2\" (line 0) (scope 0)\n"
                "[global variable] \"^3\" (line 0) (scope 0)\n"
                "-----------     Scope #1     -----------\n"
                "[user function] \"F\" (line 11) (scope 1)\n"
                "-----------     Scope #2     -----------\n"
                "[user function] \"F\" (line 16) (scope 2)\n"
                "-----------     Scope #3     -----------\n"
                "[user function] \"F\" (line 21) (scope 3)\n";
  actual = util::exec::exec("../src/d_syntax_analysis  files/phase2_tests/Working/ShadowedNameOffunctions.asc");
  GTEST_ASSERT_EQ(expected, actual);
}

#ifdef TESTING
int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
#endif
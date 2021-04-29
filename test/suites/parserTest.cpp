#include <gtest/gtest.h>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

class ParserTest : public ::testing::Test {
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

TEST_F(ParserTest, Error0) {
    expected = "No global variable with id: Global, in line: 4\n"; //TO CHANGE
    actual = exec("./scanner ../test/files/phase2_tests/Errors/Error0.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

// TEST_F(ParserTest, Error1) {
//     actual = exec("./scanner ../test/files/phase2_tests/Errors/Error1.asc");
//     GTEST_ASSERT_EQ(expected, actual);
// }

// TEST_F(ParserTest, Error2) {
//     actual = exec("./scanner ../test/files/phase2_tests/Errors/Error2.asc");    
//     GTEST_ASSERT_EQ(expected, actual);
// }

// TEST_F(ParserTest, Error3) {
//     actual = exec("./scanner ../test/files/phase2_tests/Errors/Error3.asc");
//     GTEST_ASSERT_EQ(expected, actual);
// }

// TEST_F(ParserTest, Error4) {
//     actual = exec("./scanner ../test/files/phase2_tests/Errors/Error4.asc");
//     GTEST_ASSERT_EQ(expected, actual);
// }

// TEST_F(ParserTest, Error5) {
//     actual = exec("./scanner ../test/files/phase2_tests/Errors/Error5.asc");
//     GTEST_ASSERT_EQ(expected, actual);
// }

// TEST_F(ParserTest, Error6) {
//     actual = exec("./scanner ../test/files/phase2_tests/Errors/Error6.asc");
//     GTEST_ASSERT_EQ(expected, actual);
// }

// TEST_F(ParserTest, Error7) {
//     actual = exec("./scanner ../test/files/phase2_tests/Errors/Error7.asc");
//     GTEST_ASSERT_EQ(expected, actual);
// }

// TEST_F(ParserTest, Error8) {
//     actual = exec("./scanner ../test/files/phase2_tests/Errors/Error8.asc");
//     GTEST_ASSERT_EQ(expected, actual);
// }

// TEST_F(ParserTest, Error9) {
//     actual = exec("./scanner ../test/files/phase2_tests/Errors/Error9.asc");
//     GTEST_ASSERT_EQ(expected, actual);
// }

// TEST_F(ParserTest, Error10) {
//     actual = exec("./scanner ../test/files/phase2_tests/Errors/Error10.asc");
//     GTEST_ASSERT_EQ(expected, actual);
// }

// TEST_F(ParserTest, Error11) {
//     actual = exec("./scanner ../test/files/phase2_tests/Errors/Error11.asc");
//     GTEST_ASSERT_EQ(expected, actual);
// }

// TEST_F(ParserTest, Error12) {
//     actual = exec("./scanner ../test/files/phase2_tests/Errors/Error12.asc");
//     GTEST_ASSERT_EQ(expected, actual);
// }

// TEST_F(ParserTest, Anonymous) {
//     actual = exec("./scanner ../test/files/phase2_tests/Working/Anonymous.asc");
//     GTEST_ASSERT_EQ(expected, actual);
// }

// TEST_F(ParserTest, Block) {
//     actual = exec("./scanner ../test/files/phase2_tests/Working/Block.asc");
//     GTEST_ASSERT_EQ(expected, actual);
// }

// TEST_F(ParserTest, Circle) {
//     actual = exec("./scanner ../test/files/phase2_tests/Working/Circle.asc");
//     GTEST_ASSERT_EQ(expected, actual);
// }

// TEST_F(ParserTest, GlobalAndLocal) {
//     actual = exec("./scanner ../test/files/phase2_tests/Working/GlobalAndLocal.asc");
//     GTEST_ASSERT_EQ(expected, actual);
// }

// TEST_F(ParserTest, Grammar) {
//     actual = exec("./scanner ../test/files/phase2_tests/Working/Grammar.asc");
//     GTEST_ASSERT_EQ(expected, actual);
// }

// TEST_F(ParserTest, Random) {
//     actual = exec("./scanner ../test/files/phase2_tests/Working/Random.asc");
//     GTEST_ASSERT_EQ(expected, actual);
// }

// TEST_F(ParserTest, ShadowedNameOffFunctions) {
//     actual = exec("./scanner ../test/files/phase2_tests/Working/ShadowedNameOffunctions.asc");
//     GTEST_ASSERT_EQ(expected, actual);
// }

// TEST_F(ParserTest, Simple) {
//     actual = exec("./scanner ../test/files/phase2_tests/Working/Simple.asc");
//     GTEST_ASSERT_EQ(expected, actual);
// }

// TEST_F(ParserTest, Tree) {
//     actual = exec("./scanner ../test/files/phase2_tests/Working/Tree.asc");
//     GTEST_ASSERT_EQ(expected, actual);
// }

#ifdef TESTING
int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
#endif
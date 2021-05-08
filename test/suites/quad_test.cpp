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

TEST_F(QuadTest, Assign) {
   expected = "1:   ASSIGN 2 x [line 1]\n";
   actual = exec("./scanner ../test/files/phase3_tests/Assign.asc");
   GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(QuadTest, function1) {
    expected = "1:   FUNCSTART f [line 1]\n"
               "2:   FUNCEND f [line 2]\n";
    actual = exec("./scanner ../test/files/phase3_tests/functions/function1.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(QuadTest, function2) {
    expected =  "1:   FUNCSTART f [line 1]\n"
                "2:   FUNCEND f [line 2]\n"
                "3:   FUNCSTART g [line 4]\n"
                "4:   FUNCEND g [line 5]\n"
                "5:   FUNCSTART y [line 8]\n"
                "6:   FUNCEND y [line 9]\n"
                "7:   FUNCSTART r [line 17]\n"
                "8:   FUNCEND r [line 18]\n";
    actual = exec("./scanner ../test/files/phase3_tests/functions/function2.asc");
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(QuadTest, function3) {
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
    actual = exec("./scanner ../test/files/phase3_tests/functions/function3.asc");
    GTEST_ASSERT_EQ(expected, actual);            
}

#ifdef TESTING
int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
#endif
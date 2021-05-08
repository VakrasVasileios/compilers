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

#ifdef TESTING
int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
#endif
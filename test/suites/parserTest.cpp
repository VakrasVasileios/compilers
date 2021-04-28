#include <gtest/gtest.h>

class ParserTest : public ::testing::Test {
    protected:
        std::string expected;

        std::stringstream actual;

        void SetUp() override {}

        void TearDown() override {}   
};

TEST_F(ParserTest, Error0) {
    GTEST_ASSERT_EQ(expected, actual.str());
}

TEST_F(ParserTest, Error1) {
    GTEST_ASSERT_EQ(expected, actual.str());
}

TEST_F(ParserTest, Error2) {
    GTEST_ASSERT_EQ(expected, actual.str());
}

TEST_F(ParserTest, Error3) {
    GTEST_ASSERT_EQ(expected, actual.str());
}

TEST_F(ParserTest, Error4) {
    GTEST_ASSERT_EQ(expected, actual.str());
}

TEST_F(ParserTest, Error5) {
    GTEST_ASSERT_EQ(expected, actual.str());
}

TEST_F(ParserTest, Error6) {
    GTEST_ASSERT_EQ(expected, actual.str());
}

TEST_F(ParserTest, Error7) {
    GTEST_ASSERT_EQ(expected, actual.str());
}

TEST_F(ParserTest, Error8) {
    GTEST_ASSERT_EQ(expected, actual.str());
}

TEST_F(ParserTest, Error9) {
    GTEST_ASSERT_EQ(expected, actual.str());
}

TEST_F(ParserTest, Error10) {
    GTEST_ASSERT_EQ(expected, actual.str());
}

TEST_F(ParserTest, Error11) {
    GTEST_ASSERT_EQ(expected, actual.str());
}

TEST_F(ParserTest, Error12) {
    GTEST_ASSERT_EQ(expected, actual.str());
}

TEST_F(ParserTest, Anonymous) {
    GTEST_ASSERT_EQ(expected, actual.str());
}

TEST_F(ParserTest, Block) {
    GTEST_ASSERT_EQ(expected, actual.str());
}

TEST_F(ParserTest, Circle) {
    GTEST_ASSERT_EQ(expected, actual.str());
}

TEST_F(ParserTest, GlobalAndLocal) {
    GTEST_ASSERT_EQ(expected, actual.str());
}

TEST_F(ParserTest, Grammar) {
    GTEST_ASSERT_EQ(expected, actual.str());
}

TEST_F(ParserTest, Random) {
    GTEST_ASSERT_EQ(expected, actual.str());
}

TEST_F(ParserTest, ShadowedNameOffFunctions) {
    GTEST_ASSERT_EQ(expected, actual.str());
}

TEST_F(ParserTest, Simple) {
    GTEST_ASSERT_EQ(expected, actual.str());
}

TEST_F(ParserTest, Tree) {
    GTEST_ASSERT_EQ(expected, actual.str());
}

#ifdef TESTING
int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
#endif
#include <gtest/gtest.h>
#include "../include/range.h"

TEST(RangeTest, test_range_true) {
    GTEST_ASSERT_TRUE(util::range::in_range<int>(1, 0, 3));
}

TEST(RangeTest, test_range_false1) {
    GTEST_ASSERT_FALSE(util::range::in_range<int>(0, 1, 3));
}

TEST(RangeTest, test_range_false2) {
    GTEST_ASSERT_FALSE(util::range::in_range<int>(4, 1, 3));
}

#ifdef TESTING
int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
#endif
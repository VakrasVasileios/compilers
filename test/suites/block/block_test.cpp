#include <gtest/gtest.h>
#include "../../../include/block/block.h"
#include "../../../include/symbol_table/symbol_table_entry/function_entry/user_function_entry.h"
#include "../../../include/symbol_table/symbol_table_entry/symbol_table_entry.h"

class BlockTest : public ::testing::Test {
    protected:
        Block block;
        UserFunctionEntry* entry1;
        UserFunctionEntry* entry2;
        UserFunctionEntry* entry3;

        void SetUp() override {
            entry1 =  new UserFunctionEntry("entry1", 9, 0);
            entry2 =  new UserFunctionEntry("entry2", 90, 0);
            entry3 =  new UserFunctionEntry("entry3", 99, 2220);
        }
};

TEST_F(BlockTest, empty) {
    GTEST_ASSERT_TRUE(block.getEntries().empty());
}

TEST_F(BlockTest, insert_one_entry) {
    block.insert(entry1);

    GTEST_ASSERT_TRUE(block.getEntries().size() == 1);
    GTEST_ASSERT_EQ(block.getEntries().front(), entry1);
}

TEST_F(BlockTest, insert_two_entries) {
    block.insert(entry1);
    block.insert(entry2);

    GTEST_ASSERT_TRUE(block.getEntries().size() == 2);
    GTEST_ASSERT_EQ(block.getEntries().front(), entry1);
    GTEST_ASSERT_EQ(block.getEntries().back(), entry2);
}

TEST_F(BlockTest, insert_three_entries) {
    block.insert(entry1);
    block.insert(entry2);
    block.insert(entry3);

    GTEST_ASSERT_TRUE(block.getEntries().size() == 3);
    GTEST_ASSERT_EQ(block.getEntries().front(), entry1);
    GTEST_ASSERT_EQ(block.getEntries().back(), entry3);
}

TEST_F(BlockTest, activate) {
    block.insert(entry1);
    block.insert(entry2);
    block.insert(entry3);

    block.activate();

    for(auto entry : block.getEntries()) {
        GTEST_ASSERT_TRUE(entry->isActive());
    }
}

TEST_F(BlockTest, deactivate) {
    block.insert(entry1);
    block.insert(entry2);
    block.insert(entry3);

    block.deactivate();

    for(auto entry : block.getEntries()) {
        GTEST_ASSERT_FALSE(entry->isActive());
    }
}

TEST_F(BlockTest, lookup_non_inserted_entry) {
    GTEST_ASSERT_TRUE(block.lookup("o") == nullptr);
}

TEST_F(BlockTest, lookup_inactive_entry) {
    block.insert(entry1);
    block.deactivate();
    GTEST_ASSERT_TRUE(block.lookup("entry1") == nullptr);
}

TEST_F(BlockTest, lookup_active_entry) {
    block.insert(entry1);
    GTEST_ASSERT_EQ(block.lookup("entry1"), entry1);
}

#ifdef TESTING
int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
#endif
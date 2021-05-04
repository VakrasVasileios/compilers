#include <gtest/gtest.h>
#include "../../../include/block/block.h"
#include "../../../include/symbol_table/symbol_table_entry/function_entry/user_function_entry.h"
#include "../../../include/symbol_table/symbol_table_entry/symbol_table_entry.h"
#include "../../../include/symbol_table/symbol_table_entry/variable_entry/local_variable_entry.h"

class BlockTest : public ::testing::Test {
    protected:
        Block block;
        UserFunctionEntry* entry1;
        UserFunctionEntry* entry2;
        UserFunctionEntry* entry3;
        LocalVariableEntry* entry4;

        void SetUp() override {
            entry1 =  new UserFunctionEntry("entry1", 9, 0);
            entry2 =  new UserFunctionEntry("entry2", 90, 0);
            entry3 =  new UserFunctionEntry("entry3", 99, 2220);
            entry4 =  new LocalVariableEntry("entry4", 0, 2);
        }
};

TEST_F(BlockTest, empty) {
    GTEST_ASSERT_TRUE(block.get_entries().empty());
}

TEST_F(BlockTest, insert_one_entry) {
    block.Insert(entry1);

    GTEST_ASSERT_TRUE(block.get_entries().size() == 1);
    GTEST_ASSERT_EQ(block.get_entries().front(), entry1);
}

TEST_F(BlockTest, insert_two_entries) {
    block.Insert(entry1);
    block.Insert(entry2);

    GTEST_ASSERT_TRUE(block.get_entries().size() == 2);
    GTEST_ASSERT_EQ(block.get_entries().front(), entry1);
    GTEST_ASSERT_EQ(block.get_entries().back(), entry2);
}

TEST_F(BlockTest, insert_three_entries) {
    block.Insert(entry1);
    block.Insert(entry2);
    block.Insert(entry3);

    GTEST_ASSERT_TRUE(block.get_entries().size() == 3);
    GTEST_ASSERT_EQ(block.get_entries().front(), entry1);
    GTEST_ASSERT_EQ(block.get_entries().back(), entry3);
}

TEST_F(BlockTest, activate) {
    block.Insert(entry1);
    block.Insert(entry2);
    block.Insert(entry3);

    block.Activate();

    for(auto entry : block.get_entries()) {
        GTEST_ASSERT_TRUE(entry->is_active());
    }
}

TEST_F(BlockTest, deactivate) {
    block.Insert(entry1);
    block.Insert(entry2);
    block.Insert(entry3);

    block.Deactivate();

    for(auto entry : block.get_entries()) {
        if(entry->get_type() >= CONST)
            GTEST_ASSERT_FALSE(entry->is_active());
    }
}

TEST_F(BlockTest, lookup_non_inserted_entry) {
    GTEST_ASSERT_TRUE(block.Lookup("o") == nullptr);
}

TEST_F(BlockTest, lookup_function_entry_after_deactivating) {
    block.Insert(entry1);
    block.Deactivate();
    GTEST_ASSERT_FALSE(block.Lookup("entry1") == nullptr);
}

TEST_F(BlockTest, lookup_inactive) {
    block.Insert(entry4);
    block.Deactivate();
    GTEST_ASSERT_TRUE(block.Lookup("entry1") == nullptr);
}

TEST_F(BlockTest, lookup_active_entry) {
    block.Insert(entry1);
    GTEST_ASSERT_EQ(block.Lookup("entry1"), entry1);
}

#ifdef TESTING
int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
#endif
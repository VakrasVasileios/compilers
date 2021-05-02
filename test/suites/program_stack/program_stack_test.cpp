#include <gtest/gtest.h>
#include "../../../include/program_stack/program_stack.h"
#include "../../../include/block/block.h"
#include "../../../include/symbol_table/symbol_table_entry/function_entry/user_function_entry.h"
#include "../../../include/symbol_table/symbol_table_entry/symbol_table_entry.h"

class ProgramStackTest : public ::testing::Test {
    protected:
        ProgramStack ps;
        Block* block1;
        Block* block2;
        Block* block3;
        UserFunctionEntry* ss;

        void SetUp() override {
            block1 = new Block();
            block2 = new Block();
            block3 = new Block();
            ss = new UserFunctionEntry("ss", 99, 0);
        }
};

TEST_F(ProgramStackTest, empty) {
    GTEST_ASSERT_TRUE(ps.get_block_list().empty());
    GTEST_ASSERT_TRUE(ps.Top() == nullptr);
}

TEST_F(ProgramStackTest, push_one_block) {
    ps.Push(block1);
    GTEST_ASSERT_TRUE(ps.get_block_list().size() == 1);
    GTEST_ASSERT_EQ(ps.get_block_list().front(), block1);
    GTEST_ASSERT_EQ(ps.get_block_list().back(), block1);
}

TEST_F(ProgramStackTest, push_two_blocks) {
    ps.Push(block1);
    ps.Push(block2);
    GTEST_ASSERT_TRUE(ps.get_block_list().size() == 2);
    GTEST_ASSERT_EQ(ps.get_block_list().front(), block1);
    GTEST_ASSERT_EQ(ps.get_block_list().back(), block2);
}


TEST_F(ProgramStackTest, push_three_blocks) {
    ps.Push(block1);
    ps.Push(block2);
    ps.Push(block3);
    GTEST_ASSERT_TRUE(ps.get_block_list().size() == 3);
    GTEST_ASSERT_EQ(ps.get_block_list().front(), block1);
    GTEST_ASSERT_EQ(ps.get_block_list().back(), block3);
}

TEST_F(ProgramStackTest, top_after_one_push) {
    ps.Push(block1);
    GTEST_ASSERT_EQ(block1, ps.Top());
}

TEST_F(ProgramStackTest, top_after_many_pushes) {
    ps.Push(block1);
    ps.Push(block2);
    ps.Push(block3);
    ps.Push(block1);
    GTEST_ASSERT_EQ(block1, ps.Top());
}

TEST_F(ProgramStackTest, pop_after_a_push) {
    ps.Push(block1);
    ps.Pop();
    GTEST_ASSERT_TRUE(ps.get_block_list().empty());
    GTEST_ASSERT_TRUE(ps.Top() == nullptr);
}

TEST_F(ProgramStackTest, pop_after_two_pushes) {
    ps.Push(block1);
    ps.Push(block2);
    ps.Pop();
    GTEST_ASSERT_EQ(ps.Top(), block1);
}

TEST_F(ProgramStackTest, activateLowerScopes) {
    ps.Push(block1);
    ps.Push(block2);
    ps.Push(block3);
    
    for(auto entry : block1->get_entries()) {
        GTEST_ASSERT_TRUE(entry->is_active());
    }
    for(auto entry : block2->get_entries()) {
        GTEST_ASSERT_TRUE(entry->is_active());
    }
    for(auto entry : block3->get_entries()) {
        GTEST_ASSERT_TRUE(entry->is_active());
    }
}

TEST_F(ProgramStackTest, deactivateLowerScopes) {
    ps.Push(block1);
    ps.Push(block2);
    ps.Push(block3);
    
    for(auto entry : block1->get_entries()) {
        GTEST_ASSERT_TRUE(entry->is_active());
    }
    for(auto entry : block2->get_entries()) {
        GTEST_ASSERT_FALSE(entry->is_active());
    }
    for(auto entry : block3->get_entries()) {
        GTEST_ASSERT_FALSE(entry->is_active());
    }
}

TEST_F(ProgramStackTest, lookup_not_inserted_entry) {
    ps.Push(block1);
    ps.Push(block2);
    ps.Push(block3);

    GTEST_ASSERT_TRUE(ps.Lookup("s") == nullptr);
}

TEST_F(ProgramStackTest, lookup_inactive_entry) {
    ps.Push(block1);
    ps.Push(block2);
    ps.Push(block3);

    ps.Top()->Insert(ss);
    ps.DeactivateLowerScopes();

    GTEST_ASSERT_TRUE(ps.Lookup("ss") == nullptr);
}

TEST_F(ProgramStackTest, lookup_active_entry) {
    ps.Push(block1);
    ps.Push(block2);
    ps.Push(block3);

    ps.Top()->Insert(ss);
    GTEST_ASSERT_EQ(ss, ps.Lookup("ss"));
}

TEST_F(ProgramStackTest, lookupGlobal_not_inserted_entry) {
    ps.Push(block1);
    ps.Push(block2);
    ps.Push(block3);

    GTEST_ASSERT_TRUE(ps.LookupGlobal("s") == nullptr);
}

TEST_F(ProgramStackTest, lookupGlobal_global_entry) {
    ps.Push(block1);
    ps.Push(block2);
    ps.Push(block3);

    block1->Insert(ss);

    GTEST_ASSERT_EQ(ss, ps.LookupGlobal("ss"));
}

TEST_F(ProgramStackTest, lookupGlobal_nonglobal_entry) {
    ps.Push(block1);
    ps.Push(block2);
    ps.Push(block3);

    block2->Insert(ss);

    GTEST_ASSERT_TRUE(ps.LookupGlobal("ss") == nullptr);
}

#ifdef TESTING
int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
#endif
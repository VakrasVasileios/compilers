#include <gtest/gtest.h>
#include "../../../include/program_stack/ProgramStack.h"
#include "../../../include/block/Block.h"
#include "../../../include/symbol_table/symbol_table_entry/function_entry/UserFunctionEntry.h"
#include "../../../include/symbol_table/symbol_table_entry/SymbolTableEntry.h"

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
    GTEST_ASSERT_TRUE(ps.getBlockList().empty());
    GTEST_ASSERT_TRUE(ps.top() == nullptr);
}

TEST_F(ProgramStackTest, push_one_block) {
    ps.push(block1);
    GTEST_ASSERT_TRUE(ps.getBlockList().size() == 1);
    GTEST_ASSERT_EQ(ps.getBlockList().front(), block1);
    GTEST_ASSERT_EQ(ps.getBlockList().back(), block1);
}

TEST_F(ProgramStackTest, push_two_blocks) {
    ps.push(block1);
    ps.push(block2);
    GTEST_ASSERT_TRUE(ps.getBlockList().size() == 2);
    GTEST_ASSERT_EQ(ps.getBlockList().front(), block1);
    GTEST_ASSERT_EQ(ps.getBlockList().back(), block2);
}


TEST_F(ProgramStackTest, push_three_blocks) {
    ps.push(block1);
    ps.push(block2);
    ps.push(block3);
    GTEST_ASSERT_TRUE(ps.getBlockList().size() == 3);
    GTEST_ASSERT_EQ(ps.getBlockList().front(), block1);
    GTEST_ASSERT_EQ(ps.getBlockList().back(), block3);
}

TEST_F(ProgramStackTest, top_after_one_push) {
    ps.push(block1);
    GTEST_ASSERT_EQ(block1, ps.top());
}

TEST_F(ProgramStackTest, top_after_many_pushes) {
    ps.push(block1);
    ps.push(block2);
    ps.push(block3);
    ps.push(block1);
    GTEST_ASSERT_EQ(block1, ps.top());
}

TEST_F(ProgramStackTest, pop_after_a_push) {
    ps.push(block1);
    ps.pop();
    GTEST_ASSERT_TRUE(ps.getBlockList().empty());
    GTEST_ASSERT_TRUE(ps.top() == nullptr);
}

TEST_F(ProgramStackTest, pop_after_two_pushes) {
    ps.push(block1);
    ps.push(block2);
    ps.pop();
    GTEST_ASSERT_EQ(ps.top(), block1);
}

TEST_F(ProgramStackTest, activateLowerScopes) {
    ps.push(block1);
    ps.push(block2);
    ps.push(block3);
    
    for(auto entry : block1->getEntries()) {
        GTEST_ASSERT_TRUE(entry->isActive());
    }
    for(auto entry : block2->getEntries()) {
        GTEST_ASSERT_TRUE(entry->isActive());
    }
    for(auto entry : block3->getEntries()) {
        GTEST_ASSERT_TRUE(entry->isActive());
    }
}

TEST_F(ProgramStackTest, deactivateLowerScopes) {
    ps.push(block1);
    ps.push(block2);
    ps.push(block3);
    
    for(auto entry : block1->getEntries()) {
        GTEST_ASSERT_TRUE(entry->isActive());
    }
    for(auto entry : block2->getEntries()) {
        GTEST_ASSERT_FALSE(entry->isActive());
    }
    for(auto entry : block3->getEntries()) {
        GTEST_ASSERT_FALSE(entry->isActive());
    }
}

TEST_F(ProgramStackTest, lookup_not_inserted_entry) {
    ps.push(block1);
    ps.push(block2);
    ps.push(block3);

    GTEST_ASSERT_TRUE(ps.lookup("s") == nullptr);
}

TEST_F(ProgramStackTest, lookup_inactive_entry) {
    ps.push(block1);
    ps.push(block2);
    ps.push(block3);

    ps.top()->insert(ss);
    ps.deactivateLowerScopes();

    GTEST_ASSERT_TRUE(ps.lookup("ss") == nullptr);
}

TEST_F(ProgramStackTest, lookup_active_entry) {
    ps.push(block1);
    ps.push(block2);
    ps.push(block3);

    ps.top()->insert(ss);
    GTEST_ASSERT_EQ(ss, ps.lookup("ss"));
}

TEST_F(ProgramStackTest, lookupGlobal_not_inserted_entry) {
    ps.push(block1);
    ps.push(block2);
    ps.push(block3);

    GTEST_ASSERT_TRUE(ps.lookupGlobal("s") == nullptr);
}

TEST_F(ProgramStackTest, lookupGlobal_global_entry) {
    ps.push(block1);
    ps.push(block2);
    ps.push(block3);

    block1->insert(ss);

    GTEST_ASSERT_EQ(ss, ps.lookupGlobal("ss"));
}

TEST_F(ProgramStackTest, lookupGlobal_nonglobal_entry) {
    ps.push(block1);
    ps.push(block2);
    ps.push(block3);

    block2->insert(ss);

    GTEST_ASSERT_TRUE(ps.lookupGlobal("ss") == nullptr);
}

#ifdef TESTING
int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
#endif
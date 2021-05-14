#include <gtest/gtest.h>
#include "../include/symbol_table.h"
#include "../include/block.h"

class SymbolTableTest : public ::testing::Test {
    protected:
        SymbolTable st;
        Block* block1;
        Block* block2;
        Block* block3;

        void SetUp() override {
            block1 = new Block();
            block2 = new Block();
            block3 = new Block();
        }
};

TEST_F(SymbolTableTest, empty) {
    GTEST_ASSERT_TRUE(st.get_blocks_table().empty());
}

TEST_F(SymbolTableTest, insert_one_block) {
    st.Insert(0, block1);

    GTEST_ASSERT_TRUE(st.get_blocks_table().size() == 1);
    GTEST_ASSERT_EQ(st.get_blocks_table()[0].front(), block1);
    GTEST_ASSERT_EQ(st.get_blocks_table()[0].back(), block1);
}

TEST_F(SymbolTableTest, insert_two_blocks) {
    st.Insert(0, block1);
    st.Insert(0, block2);

    GTEST_ASSERT_TRUE(st.get_blocks_table().size() == 2);
    GTEST_ASSERT_EQ(st.get_blocks_table()[0].front(), block1);
    GTEST_ASSERT_EQ(st.get_blocks_table()[0].back(), block2);
}

TEST_F(SymbolTableTest, insert_three_blocks) {
    st.Insert(0, block1);
    st.Insert(0, block2);
    st.Insert(0, block3);

    GTEST_ASSERT_TRUE(st.get_blocks_table().size() == 3);
    GTEST_ASSERT_EQ(st.get_blocks_table()[0].front(), block1);
    GTEST_ASSERT_EQ(st.get_blocks_table()[0].back(), block3);
}

#ifdef TESTING
int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
#endif
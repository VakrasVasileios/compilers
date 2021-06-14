#include <gtest/gtest.h>
#include "../include/avm_stack_segment.h"

/**
 * @brief Test suite for the avm stack segment.
 * 
**/
class StackSegmentSuite : public ::testing::Test
{
protected:
    avm::stack_segment::AvmStackSegment*    stack;
    avm::memcell::AvmMemcell*               memcell;
    avm::memcell::AvmMemcell*               memcell2;
    target_code::GlobalVmarg*               global_vmarg_5;
    target_code::GlobalVmarg*               global_vmarg_0;
    target_code::LocalVmarg*                local_vmarg;
    target_code::FormalVmarg*               formal_vmarg;

    void SetUp() override {
        stack = new avm::stack_segment::AvmStackSegment();
        memcell = new avm::memcell::NumMemcell(1);
        memcell2 = new avm::memcell::NumMemcell(2);
        global_vmarg_5 = new target_code::GlobalVmarg(5);
        global_vmarg_0 = new target_code::GlobalVmarg(0);
        local_vmarg = new target_code::LocalVmarg(2);
        formal_vmarg = new target_code::FormalVmarg(1);
    }
    void TearDown() override {
        delete stack;
    }
};

TEST_F(StackSegmentSuite, new_stack_is_empty) {
    GTEST_ASSERT_TRUE(stack->empty());
}

TEST_F(StackSegmentSuite, new_stack_has_size_0) {
    GTEST_ASSERT_TRUE(stack->size() == 0);
}

TEST_F(StackSegmentSuite, after_push_stack_is_not_empty) {
    stack->push(memcell);
    GTEST_ASSERT_FALSE(stack->empty());
}

TEST_F(StackSegmentSuite, after_push_new_size_is_1) {
    stack->push(memcell);
    GTEST_ASSERT_EQ(stack->size(), 1);
}

TEST_F(StackSegmentSuite, after_push_n_times_new_size_is_n) {
    stack->push(memcell);
    stack->push(memcell);
    stack->push(memcell);
    stack->push(memcell);
    GTEST_ASSERT_EQ(stack->size(), 4);
}

TEST_F(StackSegmentSuite, pop_after_push_now_is_empty) {
    stack->push(memcell);
    stack->pop();
    GTEST_ASSERT_TRUE(stack->empty());
}

TEST_F(StackSegmentSuite, pop_after_n_pushes_returns_top_element) {
    stack->push(memcell);
    stack->push(memcell);
    stack->push(memcell2);
    GTEST_ASSERT_EQ(stack->pop(), memcell2);
}

TEST_F(StackSegmentSuite,
    environment_non_pushed_vmarg_with_n_offset_new_stack_size_is_n_plus_1) {
    stack->environment(*global_vmarg_5);
    GTEST_ASSERT_EQ(stack->size(), global_vmarg_5->get_value() + 1);
}

TEST_F(StackSegmentSuite, overload_access_works_from_zero) {
    stack->push(memcell);
    stack->push(memcell2);
    GTEST_ASSERT_EQ((*stack)[0], memcell);
}

TEST_F(StackSegmentSuite, environment_pushed_vmarg_leaves_size_intact) {
    stack->environment(*global_vmarg_5);
    stack->environment(*global_vmarg_5);
    GTEST_ASSERT_EQ(stack->size(), 6);
}

TEST_F(StackSegmentSuite, environment_pushed_vmarg_returns_it) {
    auto gmemcell0 = stack->environment(*global_vmarg_0);
    auto gmemcell5 = stack->environment(*global_vmarg_5);
    GTEST_ASSERT_EQ(gmemcell0, stack->environment(*global_vmarg_0));
}

#ifdef TESTING
int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
#endif
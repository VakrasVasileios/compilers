#include <gtest/gtest.h>
#include "../include/virtual_machine/stack_segment/avm_stack_segment.h"

/**
 * @brief Test suite for the avm stack segment.
 * 
**/
class StackSegmentSuite : public ::testing::Test
{
protected:
    virtual_machine::stack_segment::AvmStackSegment stack;
    virtual_machine::memcell::AvmMemcell            memcell;
    virtual_machine::memcell::AvmMemcell            memcell2;
    target_code::GlobalVmarg*                       global_vmarg_5;
    target_code::GlobalVmarg*                       global_vmarg_0;
    target_code::LocalVmarg*                        local_vmarg;
    target_code::FormalVmarg*                       formal_vmarg;

    void SetUp() override {
        global_vmarg_5 = new target_code::GlobalVmarg(5);
        global_vmarg_0 = new target_code::GlobalVmarg(0);
        local_vmarg = new target_code::LocalVmarg(2);
        formal_vmarg = new target_code::FormalVmarg(1);
    }
};

TEST_F(StackSegmentSuite, new_stack_is_empty) {
    GTEST_ASSERT_TRUE(stack.empty());
}

TEST_F(StackSegmentSuite, new_stack_has_size_0) {
    GTEST_ASSERT_TRUE(stack.size() == 0);
}

TEST_F(StackSegmentSuite, after_push_stack_is_not_empty) {
    stack.push(memcell);
    GTEST_ASSERT_FALSE(stack.empty());
}

TEST_F(StackSegmentSuite, after_push_n_times_new_size_is_n) {
    stack.push(memcell);
    stack.push(memcell);
    stack.push(memcell);
    stack.push(memcell);
    GTEST_ASSERT_TRUE(stack.size() == 4);
}

TEST_F(StackSegmentSuite, push_stacksize_times_violates_next_push_preconsdition)
{
    for (int i = 0; i < AVM_STACKENV_SIZE; i++)
        stack.push(memcell);
    ASSERT_DEATH( {stack.push(memcell); }, "stack overflow");    
}

TEST_F(StackSegmentSuite, pop_empty_stack_violates_preconsdition) {
    stack.pop();
    ASSERT_DEATH( {stack.pop(); }, "must not be empty");
}

TEST_F(StackSegmentSuite, pop_after_push_now_is_empty) {
    stack.push(memcell);
    stack.pop();
    GTEST_ASSERT_TRUE(stack.empty());
}

TEST_F(StackSegmentSuite, pop_after_n_pushes_returns_top_element) {
    stack.push(memcell);
    stack.push(memcell);
    stack.push(memcell2);
    GTEST_ASSERT_EQ(stack.pop(), memcell2);
}

TEST_F(StackSegmentSuite,
    environment_non_pushed_vmarg_with_n_offset_new_stack_size_is_n) {
    stack.environment(*global_vmarg_5);
    GTEST_ASSERT_TRUE(stack.size() == global_vmarg_5->get_value());
}

TEST_F(StackSegmentSuite,
    overload_access_with_empty_stack_violates_precondition) {
    ASSERT_DEATH( {stack[0]; }, "must not be empty");
}

TEST_F(StackSegmentSuite,
    overload_access_out_of_bounds_violates_precondition) {
    stack.push(memcell);
    ASSERT_DEATH( {stack[1]; }, "out of bounds");
}

TEST_F(StackSegmentSuite, overload_access_works_from_zero) {
    stack.push(memcell);
    stack.push(memcell2);
    GTEST_ASSERT_EQ(stack[0], memcell);
}

TEST_F(StackSegmentSuite, environment_pushed_vmarg_leaves_size_intact) {
    stack.environment(*global_vmarg_5);
    stack.environment(*global_vmarg_5);
    GTEST_ASSERT_TRUE(stack.size() == 1);
}

TEST_F(StackSegmentSuite, environment_pushed_vmarg_returns_it) {
    stack.environment(*global_vmarg_0);
    stack.environment(*global_vmarg_5);
    GTEST_ASSERT_EQ(*global_vmarg_0, stack.environment(*global_vmarg_0));
}

TEST_F(StackSegmentSuite, environment_global_vmarg_pushes_it_at_start) {
    stack.environment(*local_vmarg);
    stack.environment(*formal_vmarg);
    stack.environment(*global_vmarg_0);
    GTEST_ASSERT_EQ(stack[0], *global_vmarg_0);
}

#ifdef TESTING
int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
#endif
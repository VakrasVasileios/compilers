#include <gtest/gtest.h>
#include "../include/execute_arithmetic.h"
#include "../include/execute_assign.h"
#include "../include/execute_branch.h"
#include "../include/execute_function.h"
#include "../include/execute_table.h"


/**
 * @brief Test suite for the execute library.
 * 
**/
class ExecSuite : public ::testing::Test
{
protected:
    avm::memcell::AvmMemcell*   fnum0;
    avm::memcell::AvmMemcell*   fnum1;
    avm::memcell::AvmMemcell*   fnum2;

    avm::memcell::NumMemcell*   num0;
    avm::memcell::NumMemcell*   num1;
    avm::memcell::NumMemcell*   num2;

    void SetUp() override {
        num0 = new avm::memcell::NumMemcell(0);
        num1 = new avm::memcell::NumMemcell(1);
        num2 = new avm::memcell::NumMemcell(2);
        fnum0 = num0;
        fnum1 = num1;
        fnum2 = num2;
    }

    void TearDown() override {
        delete num0;
        delete num1;
        delete num2;
    }
};

TEST_F(ExecSuite, execute_assign_changes_father_val) {
    avm::exec::execute_assign(&fnum0, fnum1);
    GTEST_ASSERT_TRUE((*fnum0 == *fnum1)->to_bool());
}

TEST_F(ExecSuite, execute_assign_changes_child_val) {
    avm::exec::execute_assign(&fnum0, fnum1);
    GTEST_ASSERT_TRUE((*num0 == *num1)->to_bool());
}

TEST_F(ExecSuite, execute_assign_after_assign_doesnt_change_first_rv) {
    avm::exec::execute_assign(&fnum0, fnum1);
    avm::exec::execute_assign(&fnum0, fnum2);
    GTEST_ASSERT_FALSE((*fnum1 == *fnum2)->to_bool());
}

TEST_F(ExecSuite, execute_add_changes_father_val) {
    avm::exec::execute_add(&fnum0, fnum1, fnum2);
    auto exp = new avm::memcell::NumMemcell(3);
    GTEST_ASSERT_TRUE((*fnum0 == *exp)->to_bool());
}

TEST_F(ExecSuite, execute_add_changes_child_val) {
    avm::exec::execute_add(&fnum0, fnum1, fnum2);
    auto exp = new avm::memcell::NumMemcell(3);
    GTEST_ASSERT_TRUE((*num0 == *exp)->to_bool());
}

TEST_F(ExecSuite, execute_sub_changes_father_val) {
    avm::exec::execute_sub(&fnum0, fnum2, fnum1);
    auto exp = new avm::memcell::NumMemcell(1);
    GTEST_ASSERT_TRUE((*fnum0 == *exp)->to_bool());
}


TEST_F(ExecSuite, execute_sub_changes_child_val) {
    avm::exec::execute_sub(&fnum0, fnum2, fnum1);
    auto exp = new avm::memcell::NumMemcell(1);
    GTEST_ASSERT_TRUE((*num0 == *exp)->to_bool());
}

TEST_F(ExecSuite, execute_mul_changes_father_val) {
    avm::exec::execute_mul(&fnum2, fnum1, fnum0);
    auto exp = new avm::memcell::NumMemcell(0);
    GTEST_ASSERT_TRUE((*fnum2 == *exp)->to_bool());
}

TEST_F(ExecSuite, execute_mul_changes_child_val) {
    avm::exec::execute_mul(&fnum2, fnum1, fnum0);
    auto exp = new avm::memcell::NumMemcell(0);
    GTEST_ASSERT_TRUE((*num2 == *exp)->to_bool());
}

TEST_F(ExecSuite, execute_div_changes_father_val) {
    avm::exec::execute_div(&fnum0, fnum2, fnum1);
    auto exp = new avm::memcell::NumMemcell(2);
    GTEST_ASSERT_TRUE((*fnum0 == *exp)->to_bool());
}

TEST_F(ExecSuite, execute_div_changes_child_val) {
    avm::exec::execute_div(&fnum0, fnum2, fnum1);
    auto exp = new avm::memcell::NumMemcell(2);
    GTEST_ASSERT_TRUE((*num0 == *exp)->to_bool());
}

TEST_F(ExecSuite, execute_mod_changes_father_val) {
    avm::exec::execute_mod(&fnum0, fnum2, fnum1);
    auto exp = new avm::memcell::NumMemcell(0);
    GTEST_ASSERT_TRUE((*fnum0 == *exp)->to_bool());
}

TEST_F(ExecSuite, execute_mod_changes_child_val) {
    avm::exec::execute_mod(&fnum0, fnum2, fnum1);
    auto exp = new avm::memcell::NumMemcell(0);
    GTEST_ASSERT_TRUE((*num0 == *exp)->to_bool());
}

#ifdef TESTING
int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
#endif
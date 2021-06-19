#include <gtest/gtest.h>
#include "../include/execute_arithmetic.h"
#include "../include/execute_assign.h"
#include "../include/execute_branch.h"
#include "../include/execute_function.h"
#include "../include/execute_table.h"
#include "../../registers/include/registers.h"
#include "../../memory/include/memory.h"
#include "../../signals/include/signals.h"

/**
 * @brief Test suite for the execute library.
 * 
**/
class ExecSuite : public ::testing::Test
{
protected:
    avm::memcell::AvmMemcell*       fnum0;
    avm::memcell::AvmMemcell*       fnum1;
    avm::memcell::AvmMemcell*       fnum2;
    avm::memcell::AvmMemcell*       flabel;
    avm::memcell::AvmMemcell*       ftable;

    avm::memcell::NumMemcell*       num0;
    avm::memcell::NumMemcell*       num1;
    avm::memcell::NumMemcell*       num2;
    avm::memcell::NumMemcell*       label;
    avm::memcell::TableMemcell*     table;

    void SetUp() override {
        num0 = new avm::memcell::NumMemcell(0);
        num1 = new avm::memcell::NumMemcell(1);
        num2 = new avm::memcell::NumMemcell(2);
        label = new avm::memcell::NumMemcell(30069420);
        table = new avm::memcell::TableMemcell();
        fnum0 = num0;
        fnum1 = num1;
        fnum2 = num2;
        flabel = label;
        ftable = table;
    
        avm::memory::initialize_code_segment();
        avm::memory::initialize_stack_segment();
        avm::registers::initialize_registers(1);
    }

    void TearDown() override {
        delete num0;
        delete num1;
        delete num2;
        delete label;
        delete table;
    }
};

TEST_F(ExecSuite, execute_assign_changes_father_val) {
    avm::exec::execute_assign(&fnum0, fnum1);
    GTEST_ASSERT_TRUE(*fnum0 == *fnum1);
}

TEST_F(ExecSuite, execute_assign_changes_child_val) {
    avm::exec::execute_assign(&fnum0, fnum1);
    GTEST_ASSERT_TRUE(*num0 == *num1);
}

TEST_F(ExecSuite, execute_assign_table_rv_carries_tableref) {
    avm::exec::execute_assign(&fnum0, ftable);
    table->set_elem(fnum1, label);
    GTEST_ASSERT_TRUE(static_cast<avm::memcell::TableMemcell*>
        (fnum0)->table_val() == table->table_val());
    GTEST_ASSERT_TRUE(static_cast<avm::memcell::TableMemcell*>
        (fnum0)->contains(fnum1));
    GTEST_ASSERT_TRUE(*static_cast<avm::memcell::TableMemcell*>
        (fnum0)->get_elem(fnum1) == *label);
    GTEST_ASSERT_TRUE(table->contains(fnum1));
    GTEST_ASSERT_TRUE(*table->get_elem(fnum1) == *label);
    static_cast<avm::memcell::TableMemcell*>(fnum0)->set_elem(label, fnum1);
    GTEST_ASSERT_TRUE(table->contains(label));
    GTEST_ASSERT_TRUE(*table->get_elem(label) == *fnum1);
    GTEST_ASSERT_TRUE(static_cast<avm::memcell::TableMemcell*>
        (fnum0)->table_val() == table->table_val());
    GTEST_ASSERT_TRUE(static_cast<avm::memcell::TableMemcell*>
        (fnum0)->contains(label));
    GTEST_ASSERT_TRUE(*static_cast<avm::memcell::TableMemcell*>
        (fnum0)->get_elem(label) == *fnum1);
}

TEST_F(ExecSuite, execute_assign_table_in_table) {
    auto table2 = new avm::memcell::TableMemcell();
    table->set_elem(table2, label);
    GTEST_ASSERT_TRUE(table->contains(table2));
    GTEST_ASSERT_TRUE(*table->get_elem(table2) == *label);
    delete table2;
}

TEST_F(ExecSuite, execute_assign_table_macaroni_but_its_ok_king) {
    table->set_elem(label, label);
    avm::exec::execute_assign(&fnum0, ftable);
    avm::exec::execute_assign(&fnum1, fnum0);
    avm::exec::execute_assign(&fnum2, fnum1);
    GTEST_ASSERT_TRUE(static_cast<avm::memcell::TableMemcell*>
        (fnum2)->table_val() == table->table_val());
    GTEST_ASSERT_TRUE(static_cast<avm::memcell::TableMemcell*>
        (fnum2)->contains(label));
    GTEST_ASSERT_TRUE(*static_cast<avm::memcell::TableMemcell*>
        (fnum2)->get_elem(label) == *label);
}

TEST_F(ExecSuite, execute_assign_after_assign_doesnt_change_first_rv) {
    avm::exec::execute_assign(&fnum0, fnum1);
    avm::exec::execute_assign(&fnum0, fnum2);
    GTEST_ASSERT_FALSE(*fnum1 == *fnum2);
    GTEST_ASSERT_FALSE(*num1 == *num2);
}

TEST_F(ExecSuite, execute_add_changes_father_val) {
    avm::exec::execute_add(&fnum0, fnum1, fnum2);
    auto exp = new avm::memcell::NumMemcell(3);
    GTEST_ASSERT_TRUE(*fnum0 == *exp);
}

TEST_F(ExecSuite, execute_add_changes_child_val) {
    avm::exec::execute_add(&fnum0, fnum1, fnum2);
    auto exp = new avm::memcell::NumMemcell(3);
    GTEST_ASSERT_TRUE(*num0 == *exp);
}

TEST_F(ExecSuite, execute_sub_changes_father_val) {
    avm::exec::execute_sub(&fnum0, fnum2, fnum1);
    auto exp = new avm::memcell::NumMemcell(1);
    GTEST_ASSERT_TRUE(*fnum0 == *exp);
}

TEST_F(ExecSuite, execute_sub_changes_child_val) {
    avm::exec::execute_sub(&fnum0, fnum2, fnum1);
    auto exp = new avm::memcell::NumMemcell(1);
    GTEST_ASSERT_TRUE(*num0 == *exp);
}

TEST_F(ExecSuite, execute_mul_changes_father_val) {
    avm::exec::execute_mul(&fnum2, fnum1, fnum0);
    auto exp = new avm::memcell::NumMemcell(0);
    GTEST_ASSERT_TRUE(*fnum2 == *exp);
}

TEST_F(ExecSuite, execute_mul_changes_child_val) {
    avm::exec::execute_mul(&fnum2, fnum1, fnum0);
    auto exp = new avm::memcell::NumMemcell(0);
    GTEST_ASSERT_TRUE(*num2 == *exp);
}

TEST_F(ExecSuite, execute_div_changes_father_val) {
    avm::exec::execute_div(&fnum0, fnum2, fnum1);
    auto exp = new avm::memcell::NumMemcell(2);
    GTEST_ASSERT_TRUE(*fnum0 == *exp);
}

TEST_F(ExecSuite, execute_div_changes_child_val) {
    avm::exec::execute_div(&fnum0, fnum2, fnum1);
    auto exp = new avm::memcell::NumMemcell(2);
    GTEST_ASSERT_TRUE(*num0 == *exp);
}

TEST_F(ExecSuite, execute_mod_changes_father_val) {
    avm::exec::execute_mod(&fnum0, fnum2, fnum1);
    auto exp = new avm::memcell::NumMemcell(0);
    GTEST_ASSERT_TRUE(*fnum0 == *exp);
}

TEST_F(ExecSuite, execute_mod_changes_child_val) {
    avm::exec::execute_mod(&fnum0, fnum2, fnum1);
    auto exp = new avm::memcell::NumMemcell(0);
    GTEST_ASSERT_TRUE(*num0 == *exp);
}

TEST_F(ExecSuite, execute_jeq_true) {
    num2->set_num_val(1);
    avm::exec::execute_jeq(label, fnum1, fnum2);
    GTEST_ASSERT_TRUE(avm::registers::pc == 30069420);
}

TEST_F(ExecSuite, execute_jeq_false) {
    avm::exec::execute_jeq(label, fnum1, fnum2);
    GTEST_ASSERT_FALSE(avm::registers::pc == 30069420);
}

TEST_F(ExecSuite, execute_jne_true) {
    avm::exec::execute_jne(label, fnum1, fnum2);
    GTEST_ASSERT_TRUE(avm::registers::pc == 30069420);
}

TEST_F(ExecSuite, execute_jne_false) {
    num2->set_num_val(1);
    avm::exec::execute_jne(label, fnum1, fnum2);
    GTEST_ASSERT_FALSE(avm::registers::pc == 30069420);
}

TEST_F(ExecSuite, execute_jgt_true) {
    avm::exec::execute_jgt(label, fnum2, fnum1);
    GTEST_ASSERT_TRUE(avm::registers::pc == 30069420);
}

TEST_F(ExecSuite, execute_jgt_false) {
    avm::exec::execute_jgt(label, fnum1, fnum2);
    GTEST_ASSERT_FALSE(avm::registers::pc == 30069420);
}

TEST_F(ExecSuite, execute_jge_true) {
    avm::exec::execute_jge(label, fnum2, fnum1);
    GTEST_ASSERT_TRUE(avm::registers::pc == 30069420);
}

TEST_F(ExecSuite, execute_jge_false) {
    avm::exec::execute_jge(label, fnum1, fnum2);
    GTEST_ASSERT_FALSE(avm::registers::pc == 30069420);
}

TEST_F(ExecSuite, execute_jlt_false) {
    avm::exec::execute_jlt(label, fnum2, fnum1);
    GTEST_ASSERT_FALSE(avm::registers::pc == 30069420);
}

TEST_F(ExecSuite, execute_jlt_true) {
    avm::exec::execute_jlt(label, fnum1, fnum2);
    GTEST_ASSERT_TRUE(avm::registers::pc == 30069420);
}

TEST_F(ExecSuite, execute_jle_true) {
    avm::exec::execute_jle(label, fnum1, fnum1);
    GTEST_ASSERT_TRUE(avm::registers::pc == 30069420);
}

TEST_F(ExecSuite, execute_jle_false) {
    avm::exec::execute_jle(label, fnum2, fnum1);
    GTEST_ASSERT_FALSE(avm::registers::pc == 30069420);
}

TEST_F(ExecSuite, execute_jump) {
    avm::exec::execute_jmp(label);
    GTEST_ASSERT_TRUE(avm::registers::pc == 30069420);
}

TEST_F(ExecSuite, execute_callfunc_sets_pc_as_taddress) {
    avm::memory::code_segment.push(new target_code::EnterFunc(1, new target_code::GlobalVmarg(2), 12));
    avm::memory::code_segment.push(new target_code::ExitFunc(2, new target_code::GlobalVmarg(2), 17));
    avm::memory::code_segment.push(new target_code::CallFunc(3, new target_code::GlobalVmarg(2), 130));
    avm::memcell::UserfuncMemcell* userfunc = new avm::memcell::UserfuncMemcell(1);

    avm::exec::execute_callfunc(userfunc);
    GTEST_ASSERT_EQ(avm::registers::pc, 1);
    delete userfunc;
}

TEST_F(ExecSuite, execute_new_table) {
    
}

#ifdef TESTING
int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
#endif
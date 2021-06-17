#include <gtest/gtest.h>
#include "../include/avm_memcell.h"

#define NUM_MEMCELL_VAL         6
#define STR_MEMCELL_VAL         "aaa"
#define BOOL_MEMCELL_VAL        false
#define USERFUNC_MEMCELL_VAL    40
#define LIBFUNC_MEMCELL_VAL     "print"

/**
 * @brief Test suite for the avm memcells.
 * 
**/
class MemcellSuite : public ::testing::Test
{
protected:
    avm::memcell::AvmMemcell*       fnum_;
    avm::memcell::AvmMemcell*       fstr_;
    avm::memcell::AvmMemcell*       fbool_;
    avm::memcell::AvmMemcell*       ftable_;
    avm::memcell::AvmMemcell*       fuserfunc_;
    avm::memcell::AvmMemcell*       flibfunc_;
    avm::memcell::AvmMemcell*       fnil_;

    avm::memcell::NumMemcell*       cnum_;
    avm::memcell::StringMemcell*    cstr_;
    avm::memcell::BoolMemcell*      cbool_;
    avm::memcell::TableMemcell*     ctable_;
    avm::memcell::UserfuncMemcell*  cuserfunc_;
    avm::memcell::LibfuncMemcell*   clibfunc_;
    avm::memcell::NilMemcell*       cnil_;

    void SetUp() override {
        cnum_ = new avm::memcell::NumMemcell(NUM_MEMCELL_VAL);
        cstr_ = new avm::memcell::StringMemcell(STR_MEMCELL_VAL);
        cbool_ = new avm::memcell::BoolMemcell(BOOL_MEMCELL_VAL);
        ctable_ = new avm::memcell::TableMemcell();
        cuserfunc_ = new avm::memcell::UserfuncMemcell(USERFUNC_MEMCELL_VAL);
        clibfunc_ = new avm::memcell::LibfuncMemcell(LIBFUNC_MEMCELL_VAL);
        cnil_ = new avm::memcell::NilMemcell();

        fnum_ = cnum_;
        fstr_ = cstr_;
        fbool_ = cbool_;
        ftable_ = ctable_;
        fuserfunc_ = cuserfunc_;
        flibfunc_ = clibfunc_;
        fnil_ = cnil_;
    }

    void TearDown() override {
        delete cnum_;
        delete cstr_;
        delete cbool_;
        delete ctable_;
        delete cuserfunc_;
        delete clibfunc_;
        delete cnil_;
    }
};

TEST_F(MemcellSuite, to_bool_nonzero_num_is_true) {
    GTEST_ASSERT_TRUE(fnum_->to_bool());
}

TEST_F(MemcellSuite, to_bool_zero_num_is_false) {
    cnum_->set_num_val(0);
    GTEST_ASSERT_FALSE(fnum_->to_bool());
}

TEST_F(MemcellSuite, to_bool_userfunc_is_true) {
    GTEST_ASSERT_TRUE(fuserfunc_->to_bool());
}

TEST_F(MemcellSuite, to_bool_libfunc_is_true) {
    GTEST_ASSERT_TRUE(flibfunc_->to_bool());
}

TEST_F(MemcellSuite, to_bool_table_is_true) {
    GTEST_ASSERT_TRUE(ftable_->to_bool());
}

TEST_F(MemcellSuite, to_bool_nil_is_false) {
    GTEST_ASSERT_FALSE(fnil_->to_bool());
}

TEST_F(MemcellSuite, to_bool_nonempty_string_is_true) {
    GTEST_ASSERT_TRUE(fstr_->to_bool());
}

TEST_F(MemcellSuite, to_bool_empty_string_is_false) {
    cstr_->set_str_val("");
    GTEST_ASSERT_FALSE(fstr_->to_bool());
}

TEST_F(MemcellSuite, type_bool_is_boolean) {
    GTEST_ASSERT_EQ(fbool_->get_type(), "boolean");
}

TEST_F(MemcellSuite, type_num_is_number) {
    GTEST_ASSERT_EQ(fnum_->get_type(), "number");
}

TEST_F(MemcellSuite, type_str_is_string) {
    GTEST_ASSERT_EQ(fstr_->get_type(), "string");
}

TEST_F(MemcellSuite, type_nil_is_nil) {
    GTEST_ASSERT_EQ(fnil_->get_type(), "nil");
}

TEST_F(MemcellSuite, type_userfunc_is_userfunction) {
    GTEST_ASSERT_EQ(fuserfunc_->get_type(), "userfunction");
}

TEST_F(MemcellSuite, type_libfunc_is_libraryfunction) {
    GTEST_ASSERT_EQ(flibfunc_->get_type(), "libraryfunction");
}

TEST_F(MemcellSuite, type_table_is_table) {
    GTEST_ASSERT_EQ(ftable_->get_type(), "table");
}

#ifdef TESTING
int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
#endif
#include <gtest/gtest.h>
#include "../include/avm_memcell.h"
#include "../../signals/include/signals.h"

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
    avm::memcell::AvmMemcell*       fundef_;
    avm::memcell::AvmMemcell*       fnum2_;
    avm::memcell::AvmMemcell*       fstr2_;
    avm::memcell::AvmMemcell*       fbool2_;
    avm::memcell::AvmMemcell*       ftable2_;
    avm::memcell::AvmMemcell*       fuserfunc2_;
    avm::memcell::AvmMemcell*       flibfunc2_;
    avm::memcell::AvmMemcell*       fnil2_;
    avm::memcell::AvmMemcell*       fundef2_;

    avm::memcell::NumMemcell*       cnum_;
    avm::memcell::StringMemcell*    cstr_;
    avm::memcell::BoolMemcell*      cbool_;
    avm::memcell::TableMemcell*     ctable_;
    avm::memcell::UserfuncMemcell*  cuserfunc_;
    avm::memcell::LibfuncMemcell*   clibfunc_;
    avm::memcell::NilMemcell*       cnil_;
    avm::memcell::UndefMemcell*     cundef_;
    avm::memcell::NumMemcell*       cnum2_;
    avm::memcell::StringMemcell*    cstr2_;
    avm::memcell::BoolMemcell*      cbool2_;
    avm::memcell::TableMemcell*     ctable2_;
    avm::memcell::UserfuncMemcell*  cuserfunc2_;
    avm::memcell::LibfuncMemcell*   clibfunc2_;
    avm::memcell::NilMemcell*       cnil2_;
    avm::memcell::UndefMemcell*     cundef2_;

    std::stringstream               out;

    void SetUp() override {
        cnum_ = new avm::memcell::NumMemcell(6);
        cstr_ = new avm::memcell::StringMemcell("aaa");
        cbool_ = new avm::memcell::BoolMemcell(true);
        ctable_ = new avm::memcell::TableMemcell();
        cuserfunc_ = new avm::memcell::UserfuncMemcell(40);
        clibfunc_ = new avm::memcell::LibfuncMemcell("print");
        cnil_ = new avm::memcell::NilMemcell();
        cundef_ = new avm::memcell::UndefMemcell();
        cnum2_ = new avm::memcell::NumMemcell(0);
        cstr2_ = new avm::memcell::StringMemcell("");
        cbool2_ = new avm::memcell::BoolMemcell(false);
        ctable2_ = new avm::memcell::TableMemcell();
        cuserfunc2_ = new avm::memcell::UserfuncMemcell(40);
        clibfunc2_ = new avm::memcell::LibfuncMemcell("print");
        cnil2_ = new avm::memcell::NilMemcell();
        cundef2_ = new avm::memcell::UndefMemcell();

        fnum_ = cnum_;
        fstr_ = cstr_;
        fbool_ = cbool_;
        ftable_ = ctable_;
        fuserfunc_ = cuserfunc_;
        flibfunc_ = clibfunc_;
        fnil_ = cnil_;
        fundef_ = cundef_;
        fnum2_ = cnum2_;
        fstr2_ = cstr2_;
        fbool2_ = cbool2_;
        ftable2_ = ctable2_;
        fuserfunc2_ = cuserfunc2_;
        flibfunc2_ = clibfunc2_;
        fnil2_ = cnil2_;
        fundef2_ = cundef2_;

        avm::signals::execution_finished = false;
    }

    void TearDown() override {
        delete cnum_;
        delete cstr_;
        delete cbool_;
        delete ctable_;
        delete cuserfunc_;
        delete clibfunc_;
        delete cnil_;
        delete cundef_;
        delete cnum2_;
        delete cstr2_;
        delete cbool2_;
        delete ctable2_;
        delete cuserfunc2_;
        delete clibfunc2_;
        delete cnil2_;
        delete cundef2_;
    }
};

TEST_F(MemcellSuite, table_set_elem_now_table_contains_key) {
    ctable_->set_elem(fnum_, fnum2_);
    GTEST_ASSERT_TRUE(ctable_->contains(fnum_));
}

TEST_F(MemcellSuite, table_get_elem_returns_inserted_key) {
    ctable_->set_elem(fnum2_, fnum_);
    GTEST_ASSERT_TRUE(*fnum_ == *ctable_->get_elem(fnum2_));
}

TEST_F(MemcellSuite, table_delete_doesnt_delete_shared_ptr) {
    auto t = new avm::memcell::TableMemcell();
    avm::memcell::AvmMemcell* ft = t;
    ft = ctable2_->clone();
    delete t;
    GTEST_ASSERT_FALSE(ctable2_ == nullptr);
    GTEST_ASSERT_FALSE(ctable2_->table_val() == nullptr);
}

TEST_F(MemcellSuite, table_set_values_and_get_them) {
    ctable_->set_elem(fnum_, fnum2_);
    ctable_->set_elem(fstr_, fstr2_);
    ctable_->set_elem(fbool_, fbool2_);
    ctable_->set_elem(ftable2_, fnum2_);
    ctable_->set_elem(fuserfunc_, fuserfunc2_);
    ctable_->set_elem(flibfunc_, flibfunc2_);
    GTEST_ASSERT_TRUE(*ctable_->get_elem(fnum_) == *fnum2_);
    GTEST_ASSERT_TRUE(*ctable_->get_elem(fstr_) == *fstr2_);
    GTEST_ASSERT_TRUE(*ctable_->get_elem(fbool_) == *fbool2_);
    GTEST_ASSERT_TRUE(*ctable_->get_elem(ftable2_) == *fnum2_);
    GTEST_ASSERT_TRUE(*ctable_->get_elem(fuserfunc_) == *fuserfunc2_);
    GTEST_ASSERT_TRUE(*ctable_->get_elem(flibfunc_) == *flibfunc2_);
}

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

TEST_F(MemcellSuite, out_op_num_is_num_val) {
    out << *fnum_;
    GTEST_ASSERT_EQ(out.str(), "6");
}

TEST_F(MemcellSuite, out_op_true_bool_is_true) {
    cbool_->set_bool_val(true);
    out << *fbool_;
    GTEST_ASSERT_EQ(out.str(), "true");
}

TEST_F(MemcellSuite, out_op_false_bool_is_false) {
    out << *fbool2_;
    GTEST_ASSERT_EQ(out.str(), "false");
}

TEST_F(MemcellSuite, out_op_str_is_itsvalue) {
    out << *fstr_;
    GTEST_ASSERT_EQ(out.str(),  "aaa");
}

TEST_F(MemcellSuite, out_op_nil_is_nil) {
    out << *fnil_;
    GTEST_ASSERT_EQ(out.str(), "nil");
}

TEST_F(MemcellSuite, out_op_userfunc_is_user_function_plus_address) {
    out << *fuserfunc_;
    GTEST_ASSERT_EQ(out.str(), "user function 40");
}

TEST_F(MemcellSuite, out_op_libfunc_is_library_function_plus_id) {
    out << *flibfunc_;
    GTEST_ASSERT_EQ(out.str(), "library function print");
}

TEST_F(MemcellSuite, out_op_indexed_map_is_formatted) {
    ctable_->set_elem( fnum_, fstr_);
    ctable_->set_elem( fnum2_, flibfunc_);
    out << *ftable_;
    GTEST_ASSERT_EQ(out.str(), "[ library function print, aaa ]");
}

TEST_F(MemcellSuite, out_op_paired_map_is_formatted) {
    ctable_->set_elem( fstr_, fnum_);
    ctable_->set_elem( flibfunc_, fstr_);
    out << *ftable_;
    GTEST_ASSERT_EQ(out.str(), "[ { aaa : 6 } ][ { library function print : aaa } ]");
}

TEST_F(MemcellSuite, eq_op_num_wth_num_wth_same_val_is_true) {
    cnum2_->set_num_val(cnum_->num_val());
    GTEST_ASSERT_TRUE(*fnum_ == *fnum2_);
}

TEST_F(MemcellSuite, eq_op_num_wth_num_wth_dif_val_is_false) {
    cnum2_->set_num_val(90);
    GTEST_ASSERT_FALSE(*fnum_ == *fnum2_);
}

TEST_F(MemcellSuite, eq_op_nonzero_num_wth_nonempty_str_is_error) {
    GTEST_ASSERT_FALSE(*fnum_ == *fstr_);
}

TEST_F(MemcellSuite, eq_op_nonzero_num_wth_empty_str_is_error) {
    GTEST_ASSERT_FALSE(*fnum_ == *fstr2_);
}

TEST_F(MemcellSuite, eq_op_zero_num_wth_nonempty_str_is_error) {
    GTEST_ASSERT_FALSE(*fnum2_ == *fstr_);
}

TEST_F(MemcellSuite, eq_op_zero_num_wth_empty_str_is_error) {
    GTEST_ASSERT_FALSE(*fnum2_ == *fstr2_);
}

TEST_F(MemcellSuite, eq_op_truebool_wth_nonzeronum_is_true) {
    GTEST_ASSERT_TRUE(*fnum_ == *fbool_);
}

TEST_F(MemcellSuite, eq_op_truebool_wth_zeronum_is_true) {
    GTEST_ASSERT_FALSE(*fnum2_ == *fbool_);
}

TEST_F(MemcellSuite, eq_op_falsebool_wth_nonzeronum_is_false) {
    GTEST_ASSERT_FALSE(*fnum_ == *fbool2_);
}

TEST_F(MemcellSuite, eq_op_falsebool_wth_zeronum_is_true) {
    GTEST_ASSERT_TRUE(*fnum2_ == *fbool2_);
}

TEST_F(MemcellSuite, eq_op_falsebool_wth_userfunc_is_false) {
    GTEST_ASSERT_FALSE(*fbool2_ == *fuserfunc_);
}

TEST_F(MemcellSuite, eq_op_truebool_wth_userfunc_is_true) {
    GTEST_ASSERT_TRUE(*fbool_ == *fuserfunc_);
}

TEST_F(MemcellSuite, eq_op_falsebool_wth_libfunc_is_false) {
    GTEST_ASSERT_FALSE(*fbool2_ == *flibfunc_);
}

TEST_F(MemcellSuite, eq_op_truebool_wth_libfunc_is_true) {
    GTEST_ASSERT_TRUE(*fbool_ == *flibfunc_);
}

TEST_F(MemcellSuite, eq_op_num_wth_undefined_is_error) {
    auto res = *fnum_ == *fundef_;
    GTEST_ASSERT_TRUE(avm::signals::execution_finished);
}

TEST_F(MemcellSuite, eq_op_undefined_wth_num_is_error) {
    auto res = *fundef_ == *fnum_;
    GTEST_ASSERT_TRUE(avm::signals::execution_finished);
}

TEST_F(MemcellSuite, eq_op_str_wth_undefined_is_error) {
    auto res = *fstr_ == *fundef_;
    GTEST_ASSERT_TRUE(avm::signals::execution_finished);
}

TEST_F(MemcellSuite, eq_op_undefined_wth_str_is_error) {
    auto res = *fundef_ == *fstr_;
    GTEST_ASSERT_TRUE(avm::signals::execution_finished);
}

TEST_F(MemcellSuite, eq_op_bool_wth_undefined_is_error) {
    auto res = *fbool_ == *fundef_;
    GTEST_ASSERT_TRUE(avm::signals::execution_finished);
}

TEST_F(MemcellSuite, eq_op_undefined_wth_bool_is_error) {
    auto res = *fundef_ == *fbool_;
    GTEST_ASSERT_TRUE(avm::signals::execution_finished);
}

TEST_F(MemcellSuite, eq_op_table_wth_undefined_is_error) {
    auto res = *ftable_ == *fundef_;
    GTEST_ASSERT_TRUE(avm::signals::execution_finished);
}

TEST_F(MemcellSuite, eq_op_undefined_wth_table_is_error) {
    auto res = *fundef_ == *ftable_;
    GTEST_ASSERT_TRUE(avm::signals::execution_finished);
}

TEST_F(MemcellSuite, eq_op_userfunc_wth_undefined_is_error) {
    auto res = *fuserfunc_ == *fundef_;
    GTEST_ASSERT_TRUE(avm::signals::execution_finished);
}

TEST_F(MemcellSuite, eq_op_undefined_wth_userfunc_is_error) {
    auto res = *fundef_ == *fuserfunc_;
    GTEST_ASSERT_TRUE(avm::signals::execution_finished);
}

TEST_F(MemcellSuite, eq_op_libfunc_wth_undefined_is_error) {
    auto res = *flibfunc_ == *fundef_;
    GTEST_ASSERT_TRUE(avm::signals::execution_finished);
}

TEST_F(MemcellSuite, eq_op_undefined_wth_libfunc_is_error) {
    auto res = *fundef_ == *flibfunc_;
    GTEST_ASSERT_TRUE(avm::signals::execution_finished);
}

TEST_F(MemcellSuite, eq_op_nil_wth_undefined_is_error) {
    auto res = *fnil_ == *fundef_;
    GTEST_ASSERT_TRUE(avm::signals::execution_finished);
}

TEST_F(MemcellSuite, eq_op_undefined_wth_nil_is_error) {
    auto res = *fundef_ == *fnil_;
    GTEST_ASSERT_TRUE(avm::signals::execution_finished);
}

TEST_F(MemcellSuite, eq_op_undefined_wth_undefined_is_error) {
    auto res = *fundef_ == *fundef_;
    GTEST_ASSERT_TRUE(avm::signals::execution_finished);
}

TEST_F(MemcellSuite, eq_op_nill_with_nill_is_true) {
    GTEST_ASSERT_TRUE(*fnil_ == *fnil2_);
}

TEST_F(MemcellSuite, eq_op_nill_with_num_is_false) {
    GTEST_ASSERT_FALSE(*fnil_ == *fnum_);
}

TEST_F(MemcellSuite, eq_op_nill_with_num2_is_false) {
    GTEST_ASSERT_FALSE(*fnil_ == *fnum2_);
}

TEST_F(MemcellSuite, eq_op_nill_with_str_is_false) {
    GTEST_ASSERT_FALSE(*fnil_ == *fstr_);
}

TEST_F(MemcellSuite, eq_op_nill_with_str2_is_false) {
    GTEST_ASSERT_FALSE(*fnil_ == *fstr2_);
}

TEST_F(MemcellSuite, eq_op_nill_with_bool_is_false) {
    GTEST_ASSERT_FALSE(*fnil_ == *fbool_);
}

TEST_F(MemcellSuite, eq_op_nill_with_bool2_is_false) {
    GTEST_ASSERT_FALSE(*fnil_ == *fbool2_);
}

TEST_F(MemcellSuite, eq_op_nill_with_table_is_false) {
    GTEST_ASSERT_FALSE(*fnil_ == *ftable_);
}

TEST_F(MemcellSuite, eq_op_nill_with_table2_is_false) {
    GTEST_ASSERT_FALSE(*fnil_ == *ftable2_);
}

TEST_F(MemcellSuite, eq_op_nill_with_userfunc_is_false) {
    GTEST_ASSERT_FALSE(*fnil_ == *fuserfunc_);
}

TEST_F(MemcellSuite, eq_op_nill_with_userfunc2_is_false) {
    GTEST_ASSERT_FALSE(*fnil_ == *fuserfunc2_);
}

TEST_F(MemcellSuite, eq_op_nill_with_libfunc_is_false) {
    GTEST_ASSERT_FALSE(*fnil_ == *flibfunc_);
}

TEST_F(MemcellSuite, eq_op_nill_with_libfunc2_is_false) {
    GTEST_ASSERT_FALSE(*fnil_ == *flibfunc2_);
}

TEST_F(MemcellSuite, eq_op_num_with_nil_is_false) {
    GTEST_ASSERT_FALSE(*fnum_ == *fnil_);
}

TEST_F(MemcellSuite, eq_op_num2_with_nil_is_false) {
    GTEST_ASSERT_FALSE(*fnum2_ == *fnil_);
}

TEST_F(MemcellSuite, eq_op_str_with_nill_is_false) {
    GTEST_ASSERT_FALSE(*fstr_ == *fnil_);
}

TEST_F(MemcellSuite, eq_op_str2_with_nill_is_false) {
    GTEST_ASSERT_FALSE(*fstr2_ == *fnil_);
}

TEST_F(MemcellSuite, eq_op_bool_with_nil_is_false) {
    GTEST_ASSERT_FALSE(*fbool_ == *fnil_);
}

TEST_F(MemcellSuite, eq_op_bool2_with_nil_is_false) {
    GTEST_ASSERT_FALSE(*fbool2_ == *fnil_);
}

TEST_F(MemcellSuite, eq_op_table_with_nil_is_false) {
    GTEST_ASSERT_FALSE(*ftable_ == *fnil_);
}

TEST_F(MemcellSuite, eq_op_table2_with_nil_is_false) {
    GTEST_ASSERT_FALSE(*ftable2_ == *fnil_);
}

TEST_F(MemcellSuite, eq_op_userfunc_with_nil_is_false) {
    GTEST_ASSERT_FALSE(*fuserfunc_ == *fnil_);
}

TEST_F(MemcellSuite, eq_op_userfunc2_with_nil_is_false) {
    GTEST_ASSERT_FALSE(*fuserfunc2_ == *fnil_);
}

TEST_F(MemcellSuite, eq_op_libfunc_with_nil_is_false) {
    GTEST_ASSERT_FALSE(*flibfunc_ == *fnil_);
}

TEST_F(MemcellSuite, eq_op_libfunc2_with_nil_is_false) {
    GTEST_ASSERT_FALSE(*flibfunc2_ == *fnil_);
}


TEST_F(MemcellSuite, neq_op_num_wth_num_wth_same_val_is_false) {
    cnum2_->set_num_val(cnum_->num_val());
    GTEST_ASSERT_FALSE(*fnum_ != *fnum2_);
}

TEST_F(MemcellSuite, neq_op_num_wth_num_wth_dif_val_is_true) {
    cnum2_->set_num_val(90);
    GTEST_ASSERT_TRUE(*fnum_ != *fnum2_);
}

TEST_F(MemcellSuite, neq_op_nonzero_num_wth_nonempty_str_is_error) {
    GTEST_ASSERT_FALSE(*fnum_ != *fstr_);
}

TEST_F(MemcellSuite, neq_op_nonzero_num_wth_empty_str_is_error) {
     GTEST_ASSERT_FALSE(*fnum_ != *fstr2_);
}

TEST_F(MemcellSuite, neq_op_zero_num_wth_nonempty_str_is_error) {
    GTEST_ASSERT_FALSE(*fnum2_ != *fstr_);
}

TEST_F(MemcellSuite, neq_op_zero_num_wth_empty_str_is_error) {
    GTEST_ASSERT_FALSE(*fnum2_ != *fstr2_);
}

TEST_F(MemcellSuite, neq_op_truebool_wth_nonzeronum_is_false) {
    GTEST_ASSERT_FALSE(*fnum_ != *fbool_);
}

TEST_F(MemcellSuite, neq_op_truebool_wth_zeronum_is_false) {
    GTEST_ASSERT_TRUE(*fnum2_ != *fbool_);
}

TEST_F(MemcellSuite, neq_op_falsebool_wth_nonzeronum_is_true) {
    GTEST_ASSERT_TRUE(*fnum_ != *fbool2_);
}

TEST_F(MemcellSuite, neq_op_falsebool_wth_zeronum_is_false) {
    GTEST_ASSERT_FALSE(*fnum2_ != *fbool2_);
}

TEST_F(MemcellSuite, neq_op_falsebool_wth_userfunc_is_true) {
    GTEST_ASSERT_TRUE(*fbool2_ != *fuserfunc_);
}

TEST_F(MemcellSuite, neq_op_truebool_wth_userfunc_is_false) {
    GTEST_ASSERT_FALSE(*fbool_ != *fuserfunc_);
}

TEST_F(MemcellSuite, neq_op_falsebool_wth_libfunc_is_true) {
    GTEST_ASSERT_TRUE(*fbool2_ != *flibfunc_);
}

TEST_F(MemcellSuite, neq_op_truebool_wth_libfunc_is_false) {
    GTEST_ASSERT_FALSE(*fbool_ != *flibfunc_);
}

TEST_F(MemcellSuite, neq_op_num_wth_undefined_is_error) {
    auto res = *fnum_ != *fundef_;
    GTEST_ASSERT_TRUE(avm::signals::execution_finished);
}

TEST_F(MemcellSuite, neq_op_undefined_wth_num_is_error) {
    auto res = *fundef_ != *fnum_;
    GTEST_ASSERT_TRUE(avm::signals::execution_finished);
}

TEST_F(MemcellSuite, neq_op_str_wth_undefined_is_error) {
    auto res = *fstr_ != *fundef_;
    GTEST_ASSERT_TRUE(avm::signals::execution_finished);
}

TEST_F(MemcellSuite, neq_op_undefined_wth_str_is_error) {
    auto res = *fundef_ != *fstr_;
    GTEST_ASSERT_TRUE(avm::signals::execution_finished);
}

TEST_F(MemcellSuite, neq_op_bool_wth_undefined_is_error) {
    auto res = *fbool_ != *fundef_;
    GTEST_ASSERT_TRUE(avm::signals::execution_finished);
}

TEST_F(MemcellSuite, neq_op_undefined_wth_bool_is_error) {
    auto res = *fundef_ != *fbool_;
    GTEST_ASSERT_TRUE(avm::signals::execution_finished);
}

TEST_F(MemcellSuite, neq_op_table_wth_undefined_is_error) {
    auto res = *ftable_ != *fundef_;
    GTEST_ASSERT_TRUE(avm::signals::execution_finished);
}

TEST_F(MemcellSuite, neq_op_undefined_wth_table_is_error) {
    auto res = *fundef_ != *ftable_;
    GTEST_ASSERT_TRUE(avm::signals::execution_finished);
}

TEST_F(MemcellSuite, neq_op_userfunc_wth_undefined_is_error) {
    auto res = *fuserfunc_ != *fundef_;
    GTEST_ASSERT_TRUE(avm::signals::execution_finished);
}

TEST_F(MemcellSuite, neq_op_undefined_wth_userfunc_is_error) {
    auto res = *fundef_ != *fuserfunc_;
    GTEST_ASSERT_TRUE(avm::signals::execution_finished);
}

TEST_F(MemcellSuite, neq_op_libfunc_wth_undefined_is_error) {
    auto res = *flibfunc_ != *fundef_;
    GTEST_ASSERT_TRUE(avm::signals::execution_finished);
}

TEST_F(MemcellSuite, neq_op_undefined_wth_libfunc_is_error) {
    auto res = *fundef_ != *flibfunc_;
    GTEST_ASSERT_TRUE(avm::signals::execution_finished);
}

TEST_F(MemcellSuite, neq_op_nil_wth_undefined_is_error) {
    auto res = *fnil_ != *fundef_;
    GTEST_ASSERT_TRUE(avm::signals::execution_finished);
}

TEST_F(MemcellSuite, neq_op_undefined_wth_nil_is_error) {
    auto res = *fundef_ != *fnil_;
    GTEST_ASSERT_TRUE(avm::signals::execution_finished);
}

TEST_F(MemcellSuite, neq_op_undefined_wth_undefined_is_error) {
    auto res = *fundef_ != *fundef_;
    GTEST_ASSERT_TRUE(avm::signals::execution_finished);
}

TEST_F(MemcellSuite, neq_op_nill_with_nill_is_false) {
    GTEST_ASSERT_FALSE(*fnil_ != *fnil2_);
}

TEST_F(MemcellSuite, neq_op_nill_with_num_is_true) {
    GTEST_ASSERT_TRUE(*fnil_ != *fnum_);
}

TEST_F(MemcellSuite, neq_op_nill_with_num2_is_true) {
    GTEST_ASSERT_TRUE(*fnil_ != *fnum2_);
}

TEST_F(MemcellSuite, neq_op_nill_with_str_is_true) {
    GTEST_ASSERT_TRUE(*fnil_ != *fstr_);
}

TEST_F(MemcellSuite, neq_op_nill_with_str2_is_true) {
    GTEST_ASSERT_TRUE(*fnil_ != *fstr2_);
}

TEST_F(MemcellSuite, neq_op_nill_with_bool_is_true) {
    GTEST_ASSERT_TRUE(*fnil_ != *fbool_);
}

TEST_F(MemcellSuite, neq_op_nill_with_bool2_is_true) {
    GTEST_ASSERT_TRUE(*fnil_ != *fbool2_);
}

TEST_F(MemcellSuite, neq_op_nill_with_table_is_true) {
    GTEST_ASSERT_TRUE(*fnil_ != *ftable_);
}

TEST_F(MemcellSuite, neq_op_nill_with_table2_is_true) {
    GTEST_ASSERT_TRUE(*fnil_ != *ftable2_);
}

TEST_F(MemcellSuite, neq_op_nill_with_userfunc_is_true) {
    GTEST_ASSERT_TRUE(*fnil_ != *fuserfunc_);
}

TEST_F(MemcellSuite, neq_op_nill_with_userfunc2_is_true) {
    GTEST_ASSERT_TRUE(*fnil_ != *fuserfunc2_);
}

TEST_F(MemcellSuite, neq_op_nill_with_libfunc_is_true) {
    GTEST_ASSERT_TRUE(*fnil_ != *flibfunc_);
}

TEST_F(MemcellSuite, neq_op_nill_with_libfunc2_is_true) {
    GTEST_ASSERT_TRUE(*fnil_ != *flibfunc2_);
}

TEST_F(MemcellSuite, neq_op_num_with_nil_is_true) {
    GTEST_ASSERT_TRUE(*fnum_ != *fnil_);
}

TEST_F(MemcellSuite, neq_op_num2_with_nil_is_true) {
    GTEST_ASSERT_TRUE(*fnum2_ != *fnil_);
}

TEST_F(MemcellSuite, neq_op_str_with_nill_is_true) {
    GTEST_ASSERT_TRUE(*fstr_ != *fnil_);
}

TEST_F(MemcellSuite, neq_op_str2_with_nill_is_true) {
    GTEST_ASSERT_TRUE(*fstr2_ != *fnil_);
}

TEST_F(MemcellSuite, neq_op_bool_with_nil_is_true) {
    GTEST_ASSERT_TRUE(*fbool_ != *fnil_);
}

TEST_F(MemcellSuite, neq_op_bool2_with_nil_is_true) {
    GTEST_ASSERT_TRUE(*fbool2_ != *fnil_);
}

TEST_F(MemcellSuite, neq_op_table_with_nil_is_true) {
    GTEST_ASSERT_TRUE(*ftable_ != *fnil_);
}

TEST_F(MemcellSuite, neq_op_table2_with_nil_is_true) {
    GTEST_ASSERT_TRUE(*ftable2_ != *fnil_);
}

TEST_F(MemcellSuite, neq_op_userfunc_with_nil_is_true) {
    GTEST_ASSERT_TRUE(*fuserfunc_ != *fnil_);
}

TEST_F(MemcellSuite, neq_op_userfunc2_with_nil_is_true) {
    GTEST_ASSERT_TRUE(*fuserfunc2_ != *fnil_);
}

TEST_F(MemcellSuite, neq_op_libfunc_with_nil_is_true) {
    GTEST_ASSERT_TRUE(*flibfunc_ != *fnil_);
}

TEST_F(MemcellSuite, neq_op_libfunc2_with_nil_is_true) {
    GTEST_ASSERT_TRUE(*flibfunc2_ != *fnil_);
}

TEST_F(MemcellSuite, arithmetic_op_non_numerics_exc_strs_is_error) {
    GTEST_ASSERT_FALSE(*fnum_ - *fbool_);
    GTEST_ASSERT_FALSE(*fbool_ - *fnum_);
    GTEST_ASSERT_FALSE(*fnum_ * *ftable_);
    GTEST_ASSERT_FALSE(*ftable_ + *fnum_);
    GTEST_ASSERT_FALSE(*fnum_ * *flibfunc_);
    GTEST_ASSERT_FALSE(*flibfunc_ * *fnum_);
    GTEST_ASSERT_FALSE(*fnum_ / *fuserfunc_);
    GTEST_ASSERT_FALSE(*fuserfunc_ / *fnum_);
    GTEST_ASSERT_FALSE(*fnum_ * *fundef_);
    GTEST_ASSERT_FALSE(*fundef_ - *fnum_);
    GTEST_ASSERT_FALSE(*fnum_ % *fnil_);
    GTEST_ASSERT_FALSE(*fnil_ % *fnum_);
    GTEST_ASSERT_FALSE(*flibfunc_ / *fuserfunc_);
    GTEST_ASSERT_FALSE(*fnil_ % *ftable_);
    GTEST_ASSERT_FALSE(*fundef_ % *ftable_);
}

TEST_F(MemcellSuite, add_op_one_plus_one_is_two) {
    cnum_->set_num_val(1);
    cnum2_->set_num_val(1);
    GTEST_ASSERT_EQ(dynamic_cast<avm::memcell::NumMemcell*> (*fnum_ + *fnum2_)->num_val(), 2);
}

TEST_F(MemcellSuite, sub_op_three_minus_one_is_two) {
    cnum_->set_num_val(3);
    cnum2_->set_num_val(1);
    GTEST_ASSERT_EQ(dynamic_cast<avm::memcell::NumMemcell*> (*fnum_ - *fnum2_)->num_val(), 2);
}

TEST_F(MemcellSuite, mul_op_three_times_one_is_three) {
    cnum_->set_num_val(3);
    cnum2_->set_num_val(1);
    GTEST_ASSERT_EQ(dynamic_cast<avm::memcell::NumMemcell*> (*fnum_ * *fnum2_)->num_val(), 3);
}

TEST_F(MemcellSuite, div_op_three_over_three_is_one) {
    cnum_->set_num_val(3);
    cnum2_->set_num_val(3);
    GTEST_ASSERT_EQ(dynamic_cast<avm::memcell::NumMemcell*> (*fnum_ / *fnum2_)->num_val(), 1);
}

TEST_F(MemcellSuite, mod_op_three_mod_three_is_zero) {
    cnum_->set_num_val(3);
    cnum2_->set_num_val(3);
    GTEST_ASSERT_EQ(dynamic_cast<avm::memcell::NumMemcell*> (*fnum_ % *fnum2_)->num_val(), 0);
}

TEST_F(MemcellSuite, add_op_num_wth_str_appends_at_start) {
    cnum_->set_num_val(5);
    cstr_->set_str_val("ARPatin");
    GTEST_ASSERT_EQ(dynamic_cast<avm::memcell::StringMemcell*> (*fnum_ + *fstr_)->str_val(), "5.000000ARPatin");
}

TEST_F(MemcellSuite, add_op_str_wth_num_appends_at_end) {
    cnum_->set_num_val(5);
    cstr_->set_str_val("ARPatin");
    GTEST_ASSERT_EQ(dynamic_cast<avm::memcell::StringMemcell*> (*fstr_ + *fnum_)->str_val(), "ARPatin5.000000");
}

TEST_F(MemcellSuite, branch_op_non_numerics_is_error) {
    GTEST_ASSERT_FALSE(*fnum_ > *fbool_);
    GTEST_ASSERT_FALSE(*fbool_ < *fnum_);
    GTEST_ASSERT_FALSE(*fnum_ >= *ftable_);
    GTEST_ASSERT_FALSE(*ftable_ <= *fnum_);
    GTEST_ASSERT_FALSE(*fnum_ < *flibfunc_);
    GTEST_ASSERT_FALSE(*flibfunc_ < *fnum_);
    GTEST_ASSERT_FALSE(*fnum_ < *fuserfunc_);
    GTEST_ASSERT_FALSE(*fuserfunc_ > *fnum_);
    GTEST_ASSERT_FALSE(*fnum_ <= *fundef_);
    GTEST_ASSERT_FALSE(*fundef_ >= *fnum_);
    GTEST_ASSERT_FALSE(*fnum_ < *fnil_);
    GTEST_ASSERT_FALSE(*fnil_ > *fnum_);
    GTEST_ASSERT_FALSE(*flibfunc_ < *fuserfunc_);
    GTEST_ASSERT_FALSE(*fnil_ >= *ftable_);
    GTEST_ASSERT_FALSE(*fundef_ > *ftable_);
}

TEST_F(MemcellSuite, branch_op_one_lt_two) {
    cnum_->set_num_val(1);
    cnum2_->set_num_val(2);
    GTEST_ASSERT_TRUE(*cnum_ < *cnum2_);
}

TEST_F(MemcellSuite, branch_op_one_leq_one) {
    cnum_->set_num_val(1);
    cnum2_->set_num_val(1);
    GTEST_ASSERT_TRUE(*cnum_ <= *cnum2_);
}

TEST_F(MemcellSuite, branch_op_two_gt_one) {
    cnum_->set_num_val(2);
    cnum2_->set_num_val(1);
    GTEST_ASSERT_TRUE(*cnum_ > *cnum2_);
}

TEST_F(MemcellSuite, branch_op_five_geq_two) {
    cnum_->set_num_val(5);
    cnum2_->set_num_val(2);
    GTEST_ASSERT_TRUE(*cnum_ >= *cnum2_);
}

#ifdef TESTING
int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
#endif
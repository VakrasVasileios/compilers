#include <gtest/gtest.h>
#include "../util/exec/include/exec.h"

/**
 * @brief Test suite for the avm.
 * 
**/
class AvmSuite : public ::testing::Test
{
protected:
    std::string expected;
    std::string actual;
};

/* ------------ assign -------------- */

TEST_F(AvmSuite, assign_simple)
{
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(AvmSuite, assign_calls_nested)
{
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(AvmSuite, assign_string)
{
    GTEST_ASSERT_EQ(expected, actual);
}

/* ------------ Function -------------- */

TEST_F(AvmSuite, functions_single)
{
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(AvmSuite, functions_many)
{
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(AvmSuite, functions_nested)
{
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(AvmSuite, functions_shadowed)
{
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(AvmSuite, functions_anonymous)
{
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(AvmSuite, functions_call_warning)
{
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(AvmSuite, functions_call_noparameters)
{
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(AvmSuite, functions_call_parameters)
{
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(AvmSuite, functions_call_with_symbol_parameters)
{
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(AvmSuite, functions_call_less_parameters)
{
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(AvmSuite, functions_call_nested)
{
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(AvmSuite, functions_call_more_parameters)
{
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(AvmSuite, functions_call_function_def)
{
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(AvmSuite, functions_call_nested_with_func_def)
{
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(AvmSuite, functions_call_anonymous_func_def)
{
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(AvmSuite, functions_call_method_call) {
    GTEST_ASSERT_EQ(expected, actual);            
}

/* ------------ return -------------- */

TEST_F(AvmSuite, return_void)
{
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(AvmSuite, return_symbol)
{
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(AvmSuite, return_many_and_nested)
{
    GTEST_ASSERT_EQ(expected, actual);
}

/* ------------ Logical ----------------- */

TEST_F(AvmSuite, logical_greater_simple)
{
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(AvmSuite, logical_equal_simple)
{
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(AvmSuite, logical_greater_equal_simple)
{
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(AvmSuite, logical_less_equal_simple)
{
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(AvmSuite, logical_less_simple)
{
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(AvmSuite, logical_not_equal_simple)
{
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(AvmSuite, logical_errors)
{
    GTEST_ASSERT_EQ(expected, actual);
}

/* ------------- Loop ------------------- */

TEST_F(AvmSuite, loop_while_simple)
{
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(AvmSuite, loop_while_simple_with_stms)
{
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(AvmSuite, loop_while_many_nested)
{
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(AvmSuite, loop_while_continue_single)
{
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(AvmSuite, loop_while_continue_many_nested)
{
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(AvmSuite, loop_while_break_single)
{
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(AvmSuite, loop_for_simple)
{
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(AvmSuite, loop_for_simple_with_stmts)
{
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(AvmSuite, loop_for_many_nested)
{
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(AvmSuite, loop_for_continue_single)
{
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(AvmSuite, loop_for_continue_many_nested)
{
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(AvmSuite, loop_for_break_single)
{
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(AvmSuite, loop_for_break_many_nested)
{
    GTEST_ASSERT_EQ(expected, actual);
}

/* ------------- Conditional ------------- */

TEST_F(AvmSuite, if_simple)
{
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(AvmSuite, if_expr)
{
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(AvmSuite, if_stmts)
{
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(AvmSuite, if_nested_many)
{
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(AvmSuite, if_else)
{
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(AvmSuite, if_elseif)
{
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(AvmSuite, if_elseif_else)
{
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(AvmSuite, if_else_many_nested)
{
    GTEST_ASSERT_EQ(expected, actual);
}

/* ------------ Arithmetic -------------- */

TEST_F(AvmSuite, arithmetic_simple)
{
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(AvmSuite, arithmetic_spaghetti)
{
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(AvmSuite, arithmetic_errors)
{
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(AvmSuite, arithmetic_plusplus_suffix)
{
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(AvmSuite, arithmetic_plusplus_prefix)
{
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(AvmSuite, arithmetic_uminus)
{
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(AvmSuite, arithmetic_uminus_bool) {
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(AvmSuite, arithmetic_not)
{
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(AvmSuite, arithmetic_minusminus_prefix)
{
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(AvmSuite, arithmetic_minusminus_suffix)
{
    GTEST_ASSERT_EQ(expected, actual);
}

/* ------------ Table -------------- */

TEST_F(AvmSuite, table_tablemake_list_single)
{
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(AvmSuite, table_tablemake_list_many)
{
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(AvmSuite, table_tablemake_pairs_single)
{
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(AvmSuite, table_tablemake_pairs_many)
{
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(AvmSuite, table_tablemake_pairs_macaroni)
{
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(AvmSuite, table_tablemake_nested)
{
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(AvmSuite, table_tablemember_item_simple) {
    GTEST_ASSERT_EQ(expected, actual); 
}

TEST_F(AvmSuite, table_tableelem_item_seq) {
    GTEST_ASSERT_EQ(expected, actual); 
}        

TEST_F(AvmSuite, table_tableelem_ind_simp) {
    GTEST_ASSERT_EQ(expected, actual); 
}

TEST_F(AvmSuite, table_tablelem_ind_manyexprs) {
    GTEST_ASSERT_EQ(expected, actual);            
}

TEST_F(AvmSuite, table_tableelem_it_call) {
    GTEST_ASSERT_EQ(expected, actual);            
}

TEST_F(AvmSuite, table_tebleelem_ind_call) {
    GTEST_ASSERT_EQ(expected, actual);              
}

TEST_F(AvmSuite, table_emit_iftableitem_plusplusprefix) {    
    GTEST_ASSERT_EQ(expected, actual);     
}

TEST_F(AvmSuite, table_emit_iftableitem_plusplussufix) {     
    GTEST_ASSERT_EQ(expected, actual);            
}

TEST_F(AvmSuite, table_emit_iftableitem_minusminus_prefix) {

    GTEST_ASSERT_EQ(expected, actual);            
}

TEST_F(AvmSuite, table_emit_iftableitem_minusminus_suffix) {  
    GTEST_ASSERT_EQ(expected, actual);            
}

TEST_F(AvmSuite, table_emit_iftableitem_assignexpr) {    
    GTEST_ASSERT_EQ(expected, actual);            
}

TEST_F(AvmSuite, table_emit_iftableitem_call) {
    GTEST_ASSERT_EQ(expected, actual);              
}

//----------------------- short circuit ---------------------------------------------------------------------------------

TEST_F(AvmSuite, short_circuit_simple_or)
{
    GTEST_ASSERT_EQ(expected, actual);
}

//-----------------------assistant tests---------------------------------------------------------------------------------

TEST_F(AvmSuite, backpatch_backpatch0)
{
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(AvmSuite, backpatch_backpatch1)
{
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(AvmSuite, backpatch_backpatch2)
{
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(AvmSuite, backpatch_backpatch3)
{
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(AvmSuite, backpatch_p3t_assignments_complex)
{
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(AvmSuite, backpatch_p3t_assignments_objects)
{
    GTEST_ASSERT_EQ(expected, actual);   
}

TEST_F(AvmSuite, backpatch_p3t_assignments_simple)
{
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(AvmSuite, backpatch_p3t_var_maths)
{
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(AvmSuite, backpatch_p3t_relational)
{
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(AvmSuite, backpatch_p3t_object_creation_expr)
{
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(AvmSuite, backpatch_p3t_if_else)
{
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(AvmSuite, backpatch_p3t_funcdecl)
{
    GTEST_ASSERT_EQ(expected, actual);
}


TEST_F(AvmSuite, backpatch_p3t_flow_control)
{
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(AvmSuite, backpatch_p3t_flow_control_error)
{
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(AvmSuite, backpatch_p3t_const_maths)
{

    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(AvmSuite, backpatch_p3t_basic_expr)
{
    GTEST_ASSERT_EQ(expected, actual);
}

TEST_F(AvmSuite, backpatch_p3t_calls)
{
 
    GTEST_ASSERT_EQ(expected, actual);
}


#ifdef TESTING
int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
#endif
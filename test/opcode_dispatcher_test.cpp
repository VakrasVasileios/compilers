#include <gtest/gtest.h>
#include "../include/opcode_dispatcher.h"

/**
 * @brief Test suite for the opcode dispatcher.
 * 
**/
class OpcodeDispatcherTestSuite : public ::testing::Test
{
protected:
    IopCodeDispatcher dispatcher;
};

TEST_F(OpcodeDispatcherTestSuite, generate_assign) {
    auto quad = new Quad(ASSIGN_t, nullptr, nullptr, nullptr, 0, 0);
    GTEST_ASSERT_EQ(A, (dispatcher.Generate(quad))->opcode); //PROXY
}

TEST_F(OpcodeDispatcherTestSuite, generate_add) {
    auto quad = new Quad(ADD_t, nullptr, nullptr, nullptr, 0, 0);
    GTEST_ASSERT_EQ(B, (dispatcher.Generate(quad))->opcode); //PROXY
}

TEST_F(OpcodeDispatcherTestSuite, generate_sub) {
    auto quad = new Quad(SUB_t, nullptr, nullptr, nullptr, 0, 0);
    GTEST_ASSERT_EQ(C, (dispatcher.Generate(quad))->opcode); //PROXY
}

TEST_F(OpcodeDispatcherTestSuite, generate_mul) {
    auto quad = new Quad(MUL_t, nullptr, nullptr, nullptr, 0, 0);
    GTEST_ASSERT_EQ(D, (dispatcher.Generate(quad))->opcode); //PROXY
}

TEST_F(OpcodeDispatcherTestSuite, generate_div) {
    auto quad = new Quad(DIV_t, nullptr, nullptr, nullptr, 0, 0);
    GTEST_ASSERT_EQ(E, (dispatcher.Generate(quad))->opcode); //PROXY
}

TEST_F(OpcodeDispatcherTestSuite, generate_mod) {
    auto quad = new Quad(MOD_t, nullptr, nullptr, nullptr, 0, 0);
    GTEST_ASSERT_EQ(F, (dispatcher.Generate(quad))->opcode); //PROXY
}

TEST_F(OpcodeDispatcherTestSuite, generate_uminus) {
    auto quad = new Quad(UMINUS_t, nullptr, nullptr, nullptr, 0, 0);
    GTEST_ASSERT_EQ(G, (dispatcher.Generate(quad))->opcode); //PROXY
}

TEST_F(OpcodeDispatcherTestSuite, generate_and) {
    auto quad = new Quad(AND_t, nullptr, nullptr, nullptr, 0, 0);
    GTEST_ASSERT_EQ(H, (dispatcher.Generate(quad))->opcode); //PROXY
}

TEST_F(OpcodeDispatcherTestSuite, generate_or) {
    auto quad = new Quad(OR_t, nullptr, nullptr, nullptr, 0, 0);
    GTEST_ASSERT_EQ(I, (dispatcher.Generate(quad))->opcode); //PROXY
}

TEST_F(OpcodeDispatcherTestSuite, generate_not) {
    auto quad = new Quad(NOT_t, nullptr, nullptr, nullptr, 0, 0);
    GTEST_ASSERT_EQ(J, (dispatcher.Generate(quad))->opcode); //PROXY
}

TEST_F(OpcodeDispatcherTestSuite, generate_jump) {
    auto quad = new Quad(JUMP_t, nullptr, nullptr, nullptr, 0, 0);
    GTEST_ASSERT_EQ(K, (dispatcher.Generate(quad))->opcode); //PROXY
}

TEST_F(OpcodeDispatcherTestSuite, generate_if_eq) {
    auto quad = new Quad(IF_EQ_t, nullptr, nullptr, nullptr, 0, 0);
    GTEST_ASSERT_EQ(L, (dispatcher.Generate(quad))->opcode); //PROXY
}

TEST_F(OpcodeDispatcherTestSuite, generate_if_noteq) {
    auto quad = new Quad(IF_NOTEQ_t, nullptr, nullptr, nullptr, 0, 0);
    GTEST_ASSERT_EQ(M, (dispatcher.Generate(quad))->opcode); //PROXY
}

TEST_F(OpcodeDispatcherTestSuite, generate_if_lesseq) {
    auto quad = new Quad(IF_LESSEQ_t, nullptr, nullptr, nullptr, 0, 0);
    GTEST_ASSERT_EQ(N, (dispatcher.Generate(quad))->opcode); //PROXY
}

TEST_F(OpcodeDispatcherTestSuite, generate_if_greatereq) {
    auto quad = new Quad(IF_GREATEREQ_t, nullptr, nullptr, nullptr, 0, 0);
    GTEST_ASSERT_EQ(O, (dispatcher.Generate(quad))->opcode); //PROXY
}

TEST_F(OpcodeDispatcherTestSuite, generate_if_less) {
    auto quad = new Quad(IF_LESS_t, nullptr, nullptr, nullptr, 0, 0);
    GTEST_ASSERT_EQ(P, (dispatcher.Generate(quad))->opcode); //PROXY
}

TEST_F(OpcodeDispatcherTestSuite, generate_if_greater) {
    auto quad = new Quad(IF_GREATER_t, nullptr, nullptr, nullptr, 0, 0);
    GTEST_ASSERT_EQ(Q, (dispatcher.Generate(quad))->opcode); //PROXY
}

TEST_F(OpcodeDispatcherTestSuite, generate_call) {
    auto quad = new Quad(CALL_t, nullptr, nullptr, nullptr, 0, 0);
    GTEST_ASSERT_EQ(R, (dispatcher.Generate(quad))->opcode); //PROXY
}

TEST_F(OpcodeDispatcherTestSuite, generate_param) {
    auto quad = new Quad(PARAM_t, nullptr, nullptr, nullptr, 0, 0);
    GTEST_ASSERT_EQ(S, (dispatcher.Generate(quad))->opcode); //PROXY
}

TEST_F(OpcodeDispatcherTestSuite, generate_ret) {
    auto quad = new Quad(RET_t, nullptr, nullptr, nullptr, 0, 0);
    GTEST_ASSERT_EQ(T, (dispatcher.Generate(quad))->opcode); //PROXY
}

TEST_F(OpcodeDispatcherTestSuite, generate_getretval) {
    auto quad = new Quad(GETRETVAL_t, nullptr, nullptr, nullptr, 0, 0);
    GTEST_ASSERT_EQ(U, (dispatcher.Generate(quad))->opcode); //PROXY
}

TEST_F(OpcodeDispatcherTestSuite, generate_funcstart) {
    auto quad = new Quad(FUNCSTART_t, nullptr, nullptr, nullptr, 0, 0);
    GTEST_ASSERT_EQ(V, (dispatcher.Generate(quad))->opcode); //PROXY
}

TEST_F(OpcodeDispatcherTestSuite, generate_funcend) {
    auto quad = new Quad(FUNCEND_t, nullptr, nullptr, nullptr, 0, 0);
    GTEST_ASSERT_EQ(W, (dispatcher.Generate(quad))->opcode); //PROXY
}

TEST_F(OpcodeDispatcherTestSuite, generate_tablecreate) {
    auto quad = new Quad(TABLECREATE_t, nullptr, nullptr, nullptr, 0, 0);
    GTEST_ASSERT_EQ(X, (dispatcher.Generate(quad))->opcode); //PROXY
}

TEST_F(OpcodeDispatcherTestSuite, generate_tablegetelem) {
    auto quad = new Quad(TABLEGETELEM_t, nullptr, nullptr, nullptr, 0, 0);
    GTEST_ASSERT_EQ(Y, (dispatcher.Generate(quad))->opcode); //PROXY
}

TEST_F(OpcodeDispatcherTestSuite, generate_tablesetelem) {
    auto quad = new Quad(TABLESETELEM_t, nullptr, nullptr, nullptr, 0, 0);
    GTEST_ASSERT_EQ(Z, (dispatcher.Generate(quad))->opcode); //PROXY
}

#ifdef TESTING
int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
#endif
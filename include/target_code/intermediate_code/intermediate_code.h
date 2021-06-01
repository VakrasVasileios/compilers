#ifndef INTERMEDIATE_CODE_H
#define INTERMEDIATE_CODE_H

#include "../../../util/contract/contract.h"
#include "syntax_analysis/syntax_analysis.h"
#include "quad.h"
#include "stmt.h"
#include "loop_stmt.h"
#include "while_stmt.h"
#include "for_stmt.h"
#include "func_def_stmt.h"
#include "if_stmt.h"
#include "syntax_analysis.h"

/**
 * @brief Namespace for generating intermediate code quads.
 * 
 */
namespace intermediate_code {
    /**
     * @brief A list of all the intermediate code emitted
     * quads.
     * 
     */
    std::vector<Quad*>          quads;  
    /**
     * @brief A stack of all the parsed loop statements.
     * 
     */
    std::stack<LoopStmt*>       loop_stmts;
    /**
     * @brief A stack of all the parsed while loop statements.
     * 
     */
    std::stack<WhileStmt*>      while_stmts;
    /**
     * @brief A stack of all the parsed for loop statements.
     * 
     */
    std::stack<ForStmt*>        for_stmts;
    /**
     * @brief A stack of all the parsed function definition statements.
     * 
     */
    std::stack<FuncDefStmt*>    func_def_stmts;  
    /**
     * @brief A stack of all the parsed if statements.
     * 
     */
    std::stack<IfStmt*>         if_stmts;
    /**
     * @brief A stack of all the parsed statements.
     * 
     */
    std::list<StmtType>         stmt_stack;
    /**
     * @brief Logs a runtime warning, parsed at a line, with a message. 
     * 
     * @param msg the message of the runtime warning, not null
     * @param line the line on which the warning is parsed, greater or
     * equal to zero
     */
    void                        LogWarning(std::string msg, unsigned int line);
    /**
     * @brief Logs all of the previously emitted quads to an
     * output.
     * 
     * @param output the output on which to log all of the previously
     * emitted quads.
     */
    void                        LogQuads(std::ostream& output);
    /**
     * @brief Inserts a new hidden symbol at the current scope, in
     * case the generated name hasn't been already defined, else returns
     * the defined hidden symbol. 
     * 
     * @param type the type of the hidden symbol
     * @param index the index of the hidden table item symbol,
     * the symbol must be a table item
     * 
     * @return a new hidden symbol at the current scope, in
     * case the generated name hasn't been already defined, else
     * the defined hidden symbol, not null
     */
    expression::Symbol*         NewTemp(expression::ExprType type, expression::Expression* index);
    /**
     * @brief Resets the hidden symbol name counter, that is responsible
     * for generating new hidden symbols.
     * 
     */
    void                        ResetTemp();
    /**
     * @brief Emits a new quad, with its fields.
     * 
     * @param op the opcode of the emitted quad
     * @param result the result of the emitted quad, can be null
     * @param arg1 the arg1 of the emitted quad, can be null
     * @param arg2 the arg2 of the emitted quad, can be null
     * @param line the line of the emitted quad, can be null
     * 
     * @return the emitted quad, not null
     */
    Quad*                       Emit(Iopcode op, expression::Expression* result,
                                expression::Expression* arg1, expression::Expression* arg2, unsigned int line);
    /**
     * @brief Returns the label of the quad after the most
     * recently emitted quad.
     * 
     * @return the label of the quad after the most
     * recently emitted quad, greater or equal to zero
     */
    unsigned int                NextQuadLabel();
    /**
     * @brief Emits a TABLEGETELEM quad, in case of a table
     * item symbol
     * 
     * @param sym the symbol to be checked if it is a table item,
     * not null
     * @param line the currently parsed line, greater or equal to zero
     * 
     * @return the symbol in case it's not a table item, else
     * a new hidden table item symbol, not null
     */
    expression::Symbol*         EmitIfTableItem(expression::Symbol* sym, unsigned int line);
    /**
     * @brief Emits the quads of a member item symbol expression.
     * 
     * @param sym the member item symbol expression, not null
     * @param index the index of the member item symbol as a string
     * @param line the currently parsed line, greater or equal to zero
     * 
     * @return a new hidden table item symbol, not null
     */
    expression::Symbol*         MemberItem(expression::Symbol* sym, const char* index, unsigned int line);
    /**
     * @brief Emits the quads of a call expression and
     * creates a new call expression.
     * 
     * @param called_symbol the called symbol, not null
     * @param call_suffix the call suffix of the call, not null
     * @param line the currently parsed line, greater or equal to zero
     * 
     * @return  the created call expression, not null
     */
    expression::Call*           MakeCall(expression::Symbol* called_symbol,
                                expression::CallSuffix* call_suffix, unsigned int line);
    /**
     * @brief Backpatches all of the unpatched labels of the previously emitted
     * jump/branch quads, in short circuit fashion.
     * 
     * @param l_list the list of the unpatched labels of the previously emitted
     * jump/branch quads
     * @param q_label the label to patch the list of the unpatched labels of the
     * previously emitted jump/branch quads
     */
    void                        BackPatch(std::list<unsigned int> l_list, unsigned int q_label);
    /**
     * @brief Concludes the short circuit evaluation for a boolean
     * expression.
     * 
     * @param expr the boolean expression for which the short circuit
     * evaluation is to be concluded, not null
     * @param line the currently parsed line, greater or equal to zero
     * 
     * @return the new hidden variable generated for the emits of the
     * short circuit evaluation, not null
     */
    expression::Symbol*         ConcludeShortCircuit(expression::BoolExpr* expr, unsigned int line);
    /**
     * @brief Checks wether an arithmetic expression operation is valid.
     * 
     * @param expr the arithmetic expression operation to be evaluated,
     * not null
     * @param line the currently parsed line, greater or equal to zero
     * 
     * @return wether an arithmetic expression operation is valid 
     */
    bool                        IsValidArithmeticOp(expression::Expression* expr, unsigned int line);
    /**
     * @brief Checks wether an arithmetic expression comparison is valid.
     * 
     * @param expr the arithmetic expression comparison to be evaluated,
     * not null
     * @param line the currently parsed line, greater or equal to zero
     * 
     * @return wether an arithmetic expression comparison is valid 
     */
    bool                        IsValidArithmeticComp(expression::Expression* expr, unsigned int line);
    /**
     * @brief Checks wether an assignment expression is valid.
     * 
     * @param expr the assignment expression to be evaluated,
     * not null
     * @param line the currently parsed line, greater or equal to zero
     * 
     * @return wether an assignment expression is valid 
     */
    bool                        IsValidAssign(expression::Symbol* left_operand, unsigned int line);
    /**
     * @brief Checks the validity of a break/continue quad.
     * 
     * @return the validity of a break/continue quad
     */
    bool                        IsValidBreakContinue();
    /**
     * @brief Checks wether a loop statement is being parsed.
     * 
     * @return wether a loop statement is being parsed 
     */
    bool                        InLoop();
    /**
     * @brief Checks wether a function definition statement is being parsed.
     * 
     * @return wether a function definition statement is being parsed 
     */
    bool                        InFuncDef();
}

#endif

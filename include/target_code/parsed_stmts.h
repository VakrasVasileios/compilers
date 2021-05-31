#ifndef PARSED_STMTS_H
#define PARSED_STMTS_H

#include <vector>
#include <stack>
#include <list>
#include "stmt.h"
#include "loop_stmt.h"
#include "while_stmt.h"
#include "for_stmt.h"
#include "func_def_stmt.h"
#include "if_stmt.h"

namespace target_code {
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

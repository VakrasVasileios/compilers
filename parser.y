%{
    #include <stdio.h>
    #include <string>
    #include <iostream>
    #include "include/debuglog.h"
    #include <stack>
    #include "include/statement/statement.h"
    #include "include/statement/for_stmt.h"
    #include "include/statement/while_stmt.h"
    #include "include/statement/loop_stmt.h"
    #include "include/statement/func_def_stmt.h"
    #include <fstream>
    #include <string>
    #include <list>
    #include <map>
    #include <iostream>
    #include <assert.h>
    #include "include/symbol_table.h"
    #include "include/expression/library_function.h"
    #include "include/expression/user_function.h"
    #include "include/expression/local_variable.h"
    #include "include/expression/global_variable.h"
    #include "include/expression/formal_variable.h"
    #include "include/expression/symbol.h"
    #include "include/expression/function_call.h"
    #include "include/expression/bool_constant.h"
    #include "include/expression/constant.h"
    #include "include/expression/nil_constant.h"
    #include "include/expression/string_constant.h"
    #include "include/expression/double_constant.h"
    #include "include/expression/int_constant.h"
    #include "include/expression/numeric_constant.h"
    #include "include/instruction_opcodes.h"
    #include "include/quad.h"
    #include "include/program_stack.h"
    
    int yyerror(std::string yaccProvidedMessage);
    int yylex(void);

    extern int yylineno;
    extern char* yytext;
    extern FILE* yyin;

    bool        error_flag = false;
    inline bool NoErrorSignaled() { return error_flag == false; }
    inline void SignalError() { error_flag = 1; }

    #if !defined TEST
        #define     SIGNALERROR(message)  \
            do { \
                std::cout << "\033[31mError, in line: " << yylineno << ":\033[0m " << message << std::endl; \
                SignalError(); \
            } while (0)

        #define     LOGWARNING(message) std::cout << "\033[33mWarning, in line: " << yylineno << ":\033[0m " << message << std::endl 
    #else 
        #define     SIGNALERROR(message)  \
            do { \
                std::cout << "Error, in line: " << yylineno << ": " << message << std::endl; \
                SignalError(); \
            } while (0)

        #define     LOGWARNING(message) std::cout << "Warning, in line: " << yylineno << ": " << message << std::endl 
    #endif

    #define OUT_OF_SCOPE  -1
    #define LIB_FUNC_LINE  0
    #define TEMP_LINE 0

    const unsigned int          global_scope = 0;
    unsigned int                current_scope = OUT_OF_SCOPE;
    
    SymbolTable                 symbol_table;
    ProgramStack                program_stack;  // the top element returns a read/write reference to the most recent block.

    unsigned int                program_var_offset = 0;
    unsigned int                formal_args_offset = 0;

    std::list<FormalVariable*>  stashed_formal_arguments;
    
    unsigned int                anonymus_funcs_counter = 0;
    unsigned int                temp_counter = 0;

    std::vector<Quad*>          quads;

    void                IncreaseScope();
    void                DecreaseScope();
    void                HideLowerScopes();
    void                EnableLowerScopes();
    inline bool         ScopeIsGlobal() { return current_scope == global_scope;}

    void                InitLibraryFunctions(); 
    Symbol*             InsertLocalVariable(const char* name, unsigned int line);
    Symbol*             InsertGlobalVariable(const char* name, unsigned int line);
    Symbol*             InsertUserFunction(const char* name, unsigned int line);
    Symbol*             InsertUserFunction(unsigned int line);
    void                PushStashedFormalArguments();
    void                StashFormalArgument(const char* name, unsigned int line);
    inline bool         IsLibraryFunction(Symbol* symbol)   { return symbol->get_type() == LIB_FUNC; }
    inline bool         IsUserFunction(Symbol* symbol)      { return symbol->get_type() == USER_FUNC; }
    inline bool         IsVariable(Symbol* symbol)          { return symbol->get_type() == VAR; }
    inline bool         IsGlobalVar(Symbol* symbol)         { return IsVariable(symbol) && symbol->get_scope() == global_scope; }
    inline bool         IsAtCurrentScope(Symbol* symbol)    { return symbol->get_scope() == current_scope; }

    Symbol*             NewTemp();
    void                ResetTemp();

    Quad*               Emit(Iopcode op, Expression* result, Expression* arg1, Expression* arg2, unsigned int line);
    unsigned int        GetBackQuadLabel();

    /*    TO REMOVE START */
    unsigned int if_cnt = 0;

    unsigned int GetIfStmt() {
        return if_cnt;
    }

    void IncreaseIfStmt() {
        if_cnt++;
    }

    void DecreaseIfStmt() {
        if_cnt--;
    }

    std::map<unsigned int, Quad*> jump_quad_by_if_stmt; // Maps the depth of an if statement with its exit jump quad.

    void MapIfStmtJumpQuad(unsigned int if_stmt, Quad* exit_quad) {
        jump_quad_by_if_stmt.insert({if_stmt, exit_quad});
    }

    void PatchIfStmtJumpQuad(unsigned int if_stmt, unsigned int patch_label) {
        auto branch_quad = jump_quad_by_if_stmt[if_stmt];
        //PatchBranchQuad(branch_quad, patch_label);
        branch_quad->arg2 = new IntConstant(patch_label);
        jump_quad_by_if_stmt.erase(if_stmt);
    }

    std::map<unsigned int, std::list<Quad*>> else_jump_quads_by_if_stmt; // Maps the depth of an if statement with its else stmts jump quads.

    void PushElseJumpQuad(unsigned int if_stmt, Quad* else_jump_quad) {
        else_jump_quads_by_if_stmt[if_stmt].push_back(else_jump_quad);
    }

    void PatchElseJumpQuad(unsigned int if_stmt) {
        auto else_jump_quads = else_jump_quads_by_if_stmt[if_stmt];
        auto else_jump_quad = else_jump_quads.back();
        else_jump_quads.pop_back();
        //PatchJumpQuad(else_jump_quad, GetBackQuadLabel() + 1);
        else_jump_quad->result = new IntConstant(GetBackQuadLabel() + 1);
    }


    std::stack<FunctionCall*> call_stack; // provides the call stack containing all the active call stack frames.

    unsigned int GetCallDepth() {
        return call_stack.size();
    }

    void PushCallStackFrame(FunctionCall* function_call) {
        assert(function_call != nullptr);
        call_stack.push(function_call);
    }

    FunctionCall* PopCallStackFrame() {
        auto top_call = call_stack.top();
        call_stack.pop();
        return top_call;
    }

    void PushCallParam(Expression* expr) {
        call_stack.top()->IncludeParameter(expr);
    }
    /*    TO REMOVE END */
    

    std::stack<LoopStmt*>       loop_stmts;
    std::stack<WhileStmt*>      while_stmts;
    std::stack<ForStmt*>        for_stmts;
    std::stack<FuncDefStmt*>    func_def_stmts;

    std::map<LoopStmt*, std::list<Quad*>>   unpatched_break_quads_by_loop_stmt;
    std::map<LoopStmt*, std::list<Quad*>>   unpatched_continue_quads_by_loop_stmt;

    std::map<WhileStmt*, std::list<Quad*>>  unpatched_while_quads_by_while_stmt;

    std::map<ForStmt*, std::list<Quad*>>    unpatched_for_quads_by_for_stmt;
    std::map<ForStmt*, unsigned int>        logical_expr_first_quad_label_by_for_stmt;  
    std::map<ForStmt*, unsigned int>        exprs_first_quad_label_by_for_stmt;   
%}

%union {                                                    
    char*                   stringValue;
    int                     intValue;
    double                  doubleValue;
    class Expression*       expression;
    class Constant*         con;
    class FunctionCall*     funcCall;
    class Symbol*           sym;
}

%start program

%token EQUAL NOTEQUAL COLONCOLON DOTDOT GEQL LEQL MINUSMINUS PLUSPLUS
%token LOCAL FUNCTION IF ELSE FOR WHILE BREAK CONTINUE NIL TRUE FALSE RETURN

%token <stringValue>    STRING ID 
%token <intValue>       INTNUM
%token <doubleValue>    DOUBLENUM

%type <expression> primary term expr assignexpr
%type <con> const
%type <sym> lvalue funcdef
%type <stringValue> member
%type <funcCall> call

%right      '='
%left       OR
%left       AND
%nonassoc   EQUAL NOTEQUAL
%nonassoc   '>' GEQL '<' LEQL
%left       '-' '+'
%left       '*' '/' '%'
%nonassoc   UMINUS
%right      NOT MINUSMINUS PLUSPLUS 
%left       '.' DOTDOT
%left       '[' ']'
%left       '(' ')'

%%

program:      stmts                 {
                                        DLOG("program -> stmts");
                                    }
            ;

stmts:      stmt                    {
                                        ResetTemp();
                                    }
            stmts                   {
                                        DLOG("stmts -> stmt stmts");
                                    }
            |                       {
                                        DLOG("stmts -> EMPTY");
                                    }
            ;

stmt:         expr ';'              {
                                        DLOG("stmt -> expr;");
                                    }
            | ifstmt                {
                                        DLOG("stmt -> ifstmt");
                                    }
            | whilestmt             {
                                        DLOG("stmt -> whilestmt");
                                    }
            | forstmt               {
                                        DLOG("stmt -> forstmt");
                                    }
            | returnstmt            {
                                        DLOG("stmt -> returnstmt");
                                    }
            | BREAK ';'             { 
                                        if(loop_stmts.size() == 0) {
                                            SIGNALERROR("invalid keyword BREAK outside of loop");
                                        } else {
                                            auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);

                                            auto top_loop_stmt = loop_stmts.top();
                                            unpatched_break_quads_by_loop_stmt[top_loop_stmt].push_back(jump_quad);
                                        }

                                        DLOG("stmt -> break;");
                                    }
            | CONTINUE ';'          {
                                        if(loop_stmts.size() == 0) {
                                            SIGNALERROR("invalid keyword CONTINUE outside of loop");
                                        } else {
                                            auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);

                                            auto top_loop_stmt = loop_stmts.top();
                                            unpatched_continue_quads_by_loop_stmt[top_loop_stmt].push_back(jump_quad);
                                        }
                                        
                                        DLOG("stmt -> continue;");
                                    }
            | block                 {
                                        DLOG("stmt -> block");
                                    }
            | funcdef               {
                                        DLOG("stmt -> funcdef");
                                    }
            | ';'                   {
                                        DLOG("stmt -> ;");
                                    }
            ;

expr:         assignexpr            {
                                        auto temp = NewTemp();
                                        Emit(ASSIGN_t, temp, $1, nullptr, yylineno);

                                        $$ = temp;

                                        DLOG("expr -> assignexpr");
                                    }
            | expr '+' expr         {
                                        auto entry1 = $1;
                                        auto entry2 = $3;
                                        // if(entry1 == nullptr)
                                        //     SIGNALERROR("Cannot access " + entry1->get_id() + ", previously defined in line: " + std::to_string(entry1->get_line()));
                                        // else if (!entry1->is_active())
                                        //     SIGNALERROR("Cannot access " + entry1->get_id() + ", previously defined in line: " + std::to_string(entry1->get_line()));
                                        // else if(entry2 == nullptr)
                                        //     SIGNALERROR("Cannot access " + entry2->get_id() + ", previously defined in line: " + std::to_string(entry2->get_line()));
                                        // else if (!entry2->is_active())
                                        //     SIGNALERROR("Cannot access " + entry2->get_id() + ", previously defined in line: " + std::to_string(entry2->get_line()));
                                        // else if (!IsVariable(entry1) || (!IsVariable(entry2))
                                        //     SIGNALERROR("Use of addition with non variable type");
                                        // else if(entry1->get_type()!=CONTST_NUM || entry2->get_type()!=CONST_NUM)
                                        //     LOGWARNING("Entries must be type of Number");        
                                        // else{
                                            auto _t1 = NewTemp(); 
                                            $$ = _t1;
                                            Emit(ADD_t, _t1, entry1, entry2, yylineno);
                                        //} 
                                        DLOG("expr -> expr + expr");
                                    }
            | expr '-' expr         {
                                        // auto entry1 = $1;
                                        // auto entry2 = $3;
                                        // if(entry1 == nullptr)
                                        //     SIGNALERROR("Cannot access " + entry1->get_id() + ", previously defined in line: " + std::to_string(entry1->get_line()));
                                        // else if (!entry1->is_active())
                                        //     SIGNALERROR("Cannot access " + entry1->get_id() + ", previously defined in line: " + std::to_string(entry1->get_line()));
                                        // else if(entry2 == nullptr)
                                        //     SIGNALERROR("Cannot access " + entry2->get_id() + ", previously defined in line: " + std::to_string(entry2->get_line()));
                                        // else if (!entry2->is_active())
                                        //     SIGNALERROR("Cannot access " + entry2->get_id() + ", previously defined in line: " + std::to_string(entry2->get_line()));
                                        // else if (!IsVariable(entry1) || (!IsVariable(entry2))
                                        //     SIGNALERROR("Use of subtraction with non variable type");
                                        // else if(entry1->get_type()!=CONTST_NUM || entry2->get_type()!=CONST_NUM)
                                        //     LOGWARNING("Entries must be type of Number");       
                                        // else{
                                            // auto _t1 = NewTemp(); 
                                            // $$ = _t1;
                                            // Emit(SUB_t, _t1, entry1, entry2, yylineno);
                                        //} 
                                        DLOG("expr -> expr - expr");
                                    }
            | expr '*' expr         {
                                        // auto entry1 = $1;
                                        // auto entry2 = $3;
                                        // if(entry1 == nullptr)
                                        //     SIGNALERROR("Cannot access " + entry1->get_id() + ", previously defined in line: " + std::to_string(entry1->get_line()));
                                        // else if (!entry1->is_active())
                                        //     SIGNALERROR("Cannot access " + entry1->get_id() + ", previously defined in line: " + std::to_string(entry1->get_line()));
                                        // else if(entry2 == nullptr)
                                        //     SIGNALERROR("Cannot access " + entry2->get_id() + ", previously defined in line: " + std::to_string(entry2->get_line()));
                                        // else if (!entry2->is_active())
                                        //     SIGNALERROR("Cannot access " + entry2->get_id() + ", previously defined in line: " + std::to_string(entry2->get_line()));
                                        // else if (!IsVariable(entry1) || (!IsVariable(entry2))
                                        //     SIGNALERROR("Use of multiplication with non variable type");
                                        // else if(entry1->get_type()!=CONTST_NUM || entry2->get_type()!=CONST_NUM)
                                        //     LOGWARNING("Entries must be type of Number");     
                                        // else{
                                            // auto _t1 = NewTemp(); 
                                            // $$ = _t1;
                                            // Emit(MUL_t, _t1, entry1, entry2, yylineno);
                                        //} 
                                        DLOG("expr -> expr * expr");
                                    }
            | expr '/' expr         {
                                        // auto entry1 = $1;
                                        // auto entry2 = $3;
                                        // if(entry1 == nullptr)
                                        //     SIGNALERROR("Cannot access " + entry1->get_id() + ", previously defined in line: " + std::to_string(entry1->get_line()));
                                        // else if (!entry1->is_active())
                                        //     SIGNALERROR("Cannot access " + entry1->get_id() + ", previously defined in line: " + std::to_string(entry1->get_line()));
                                        // else if(entry2 == nullptr)
                                        //     SIGNALERROR("Cannot access " + entry2->get_id() + ", previously defined in line: " + std::to_string(entry2->get_line()));
                                        // else if (!entry2->is_active())
                                        //     SIGNALERROR("Cannot access " + entry2->get_id() + ", previously defined in line: " + std::to_string(entry2->get_line()));
                                        // else if (!IsVariable(entry1) || (!IsVariable(entry2))
                                        //     SIGNALERROR("Use of division with non variable type");
                                        // else if(entry1->get_type()!=CONTST_NUM || entry2->get_type()!=CONST_NUM)
                                        //     LOGWARNING("Entries must be type of Number");       
                                        // else{
                                            // auto _t1 = NewTemp(); 
                                            // $$ = _t1;
                                            // Emit(DIV_t, _t1, entry1, entry2, yylineno);
                                        //} 
                                        DLOG("expr -> expr / expr");
                                    }
            | expr '%' expr         {
                                        // auto entry1 = $1;
                                        // auto entry2 = $3;
                                        // if(entry1 == nullptr)
                                        //     SIGNALERROR("Cannot access " + entry1->get_id() + ", previously defined in line: " + std::to_string(entry1->get_line()));
                                        // else if (!entry1->is_active())
                                        //     SIGNALERROR("Cannot access " + entry1->get_id() + ", previously defined in line: " + std::to_string(entry1->get_line()));
                                        // else if(entry2 == nullptr)
                                        //     SIGNALERROR("Cannot access " + entry2->get_id() + ", previously defined in line: " + std::to_string(entry2->get_line()));
                                        // else if (!entry2->is_active())
                                        //     SIGNALERROR("Cannot access " + entry2->get_id() + ", previously defined in line: " + std::to_string(entry2->get_line()));
                                        // else if (!IsVariable(entry1) || (!IsVariable(entry2))
                                        //     SIGNALERROR("Use of modulo with non variable type");
                                        // else if(entry1->get_type()!=CONTST_NUM || entry2->get_type()!=CONST_NUM)
                                        //     LOGWARNING("Entries must be type of Number");     
                                        // else{
                                            // auto _t1 = NewTemp(); 
                                            // $$ = _t1;
                                            // Emit(MOD_t, _t1, entry1, entry2, yylineno);
                                        //} 
                                        DLOG("expr -> expr % expr");
                                    }
            | expr '>' expr         {
                                        // auto entry1 = $1;
                                        // auto entry2 = $3;
                                        // if(entry1 == nullptr)
                                        //     SIGNALERROR("Cannot access " + entry1->get_id() + ", previously defined in line: " + std::to_string(entry1->get_line()));
                                        // else if (!entry1->is_active())
                                        //     SIGNALERROR("Cannot access " + entry1->get_id() + ", previously defined in line: " + std::to_string(entry1->get_line()));
                                        // else if(entry2 == nullptr)
                                        //     SIGNALERROR("Cannot access " + entry2->get_id() + ", previously defined in line: " + std::to_string(entry2->get_line()));
                                        // else if (!entry2->is_active())
                                        //     SIGNALERROR("Cannot access " + entry2->get_id() + ", previously defined in line: " + std::to_string(entry2->get_line()));
                                        // else if (!IsVariable(entry1) || (!IsVariable(entry2))
                                        //     SIGNALERROR("Use of > with non variable type");
                                        // else if(entry1->get_type()!=CONTST_NUM || entry2->get_type()!=CONST_NUM)
                                        //     LOGWARNING("Entries must be type of Number");   
                                        // else{
                                            auto greater_quad = Emit(IF_GREATER_t, $1, $3, nullptr, yylineno);
                                            greater_quad->PatchBranchQuad(greater_quad->label + 2);

                                            auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);
                                            jump_quad->PatchJumpQuad(jump_quad->label + 3);

                                            auto temp = NewTemp();

                                            Emit(ASSIGN_t, temp, new BoolConstant(true), nullptr, yylineno);

                                            jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);
                                            jump_quad->PatchJumpQuad(jump_quad->label + 2);

                                            Emit(ASSIGN_t, temp, new BoolConstant(false), nullptr, yylineno);

                                            $$ = temp;
                                        //} 
                                        DLOG("expr -> expr > expr");
                                    }
            | expr GEQL expr        {

                                        // auto entry1 = $1;
                                        // auto entry2 = $3;
                                        // if(entry1 == nullptr)
                                        //     SIGNALERROR("Cannot access " + entry1->get_id() + ", previously defined in line: " + std::to_string(entry1->get_line()));
                                        // else if (!entry1->is_active())
                                        //     SIGNALERROR("Cannot access " + entry1->get_id() + ", previously defined in line: " + std::to_string(entry1->get_line()));
                                        // else if(entry2 == nullptr)
                                        //     SIGNALERROR("Cannot access " + entry2->get_id() + ", previously defined in line: " + std::to_string(entry2->get_line()));
                                        // else if (!entry2->is_active())
                                        //     SIGNALERROR("Cannot access " + entry2->get_id() + ", previously defined in line: " + std::to_string(entry2->get_line()));
                                        // else if (!IsVariable(entry1) || (!IsVariable(entry2))
                                        //     SIGNALERROR("Use of >= with non variable type");
                                        // else if(entry1->get_type()!=CONTST_NUM || entry2->get_type()!=CONST_NUM)
                                        //     LOGWARNING("Entries must be type of Number");   
                                        // else{
                                            auto greater_equal_quad = Emit(IF_GREATEREQ_t, $1, $3, nullptr, yylineno);
                                            greater_equal_quad->PatchBranchQuad(greater_equal_quad->label + 2);

                                            auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);
                                            jump_quad->PatchJumpQuad(jump_quad->label + 3);

                                            auto temp = NewTemp();

                                            Emit(ASSIGN_t, temp, new BoolConstant(true), nullptr, yylineno);

                                            jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);
                                            jump_quad->PatchJumpQuad(jump_quad->label + 2);

                                            Emit(ASSIGN_t, temp, new BoolConstant(false), nullptr, yylineno);

                                            $$ = temp;
                                        //} 
                                        DLOG("expr -> expr >= expr");
                                    }
            | expr '<' expr         {
                                        // auto entry1 = $1;
                                        // auto entry2 = $3;
                                        // if(entry1 == nullptr)
                                        //     SIGNALERROR("Cannot access " + entry1->get_id() + ", previously defined in line: " + std::to_string(entry1->get_line()));
                                        // else if (!entry1->is_active())
                                        //     SIGNALERROR("Cannot access " + entry1->get_id() + ", previously defined in line: " + std::to_string(entry1->get_line()));
                                        // else if(entry2 == nullptr)
                                        //     SIGNALERROR("Cannot access " + entry2->get_id() + ", previously defined in line: " + std::to_string(entry2->get_line()));
                                        // else if (!entry2->is_active())
                                        //     SIGNALERROR("Cannot access " + entry2->get_id() + ", previously defined in line: " + std::to_string(entry2->get_line()));
                                        // else if (!IsVariable(entry1) || (!IsVariable(entry2))
                                        //     SIGNALERROR("Use of < with non variable type");
                                        // else if(entry1->get_type()!=CONTST_NUM || entry2->get_type()!=CONST_NUM)
                                        //     LOGWARNING("Entries must be type of Number");
                                        // else{
                                            auto less_quad = Emit(IF_LESS_t, $1, $3, nullptr, yylineno);
                                            less_quad->PatchBranchQuad(less_quad->label + 2);

                                            auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);
                                            jump_quad->PatchJumpQuad(jump_quad->label + 3);

                                            auto temp = NewTemp();

                                            Emit(ASSIGN_t, temp, new BoolConstant(true), nullptr, yylineno);

                                            jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);
                                            jump_quad->PatchJumpQuad(jump_quad->label + 2);

                                            Emit(ASSIGN_t, temp, new BoolConstant(false), nullptr, yylineno);

                                            $$ = temp;
                                        //} 
                                        DLOG("expr -> expr + expr");
                                    }
            | expr LEQL expr        {
                                        // auto entry1 = $1;
                                        // auto entry2 = $3;
                                        // if(entry1 == nullptr)
                                        //     SIGNALERROR("Cannot access " + entry1->get_id() + ", previously defined in line: " + std::to_string(entry1->get_line()));
                                        // else if (!entry1->is_active())
                                        //     SIGNALERROR("Cannot access " + entry1->get_id() + ", previously defined in line: " + std::to_string(entry1->get_line()));
                                        // else if(entry2 == nullptr)
                                        //     SIGNALERROR("Cannot access " + entry2->get_id() + ", previously defined in line: " + std::to_string(entry2->get_line()));
                                        // else if (!entry2->is_active())
                                        //     SIGNALERROR("Cannot access " + entry2->get_id() + ", previously defined in line: " + std::to_string(entry2->get_line()));
                                        // else if (!IsVariable(entry1) || (!IsVariable(entry2))
                                        //     SIGNALERROR("Use of <= with non variable type");
                                        // else if(entry1->get_type()!=CONTST_NUM || entry2->get_type()!=CONST_NUM)
                                        //     LOGWARNING("Entries must be type of Number");
                                        // else{
                                            auto less_equal_quad = Emit(IF_LESSEQ_t, $1, $3, nullptr, yylineno);
                                            less_equal_quad->PatchBranchQuad(less_equal_quad->label + 2);

                                            auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);
                                            jump_quad->PatchJumpQuad(jump_quad->label + 3);

                                            auto temp = NewTemp();

                                            Emit(ASSIGN_t, temp, new BoolConstant(true), nullptr, yylineno);

                                            jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);
                                            jump_quad->PatchJumpQuad(jump_quad->label + 2);

                                            Emit(ASSIGN_t, temp, new BoolConstant(false), nullptr, yylineno);

                                            $$ = temp;
                                        //} 
                                        DLOG("expr -> expr <= expr");
                                    }
            | expr EQUAL expr       {
                                        // auto entry1 = $1;
                                        // auto entry2 = $3;
                                        // if(entry1 == nullptr)
                                        //     SIGNALERROR("Cannot access " + entry1->get_id() + ", previously defined in line: " + std::to_string(entry1->get_line()));
                                        // else if (!entry1->is_active())
                                        //     SIGNALERROR("Cannot access " + entry1->get_id() + ", previously defined in line: " + std::to_string(entry1->get_line()));
                                        // else if(entry2 == nullptr)
                                        //     SIGNALERROR("Cannot access " + entry2->get_id() + ", previously defined in line: " + std::to_string(entry2->get_line()));
                                        // else if (!entry2->is_active())
                                        //     SIGNALERROR("Cannot access " + entry2->get_id() + ", previously defined in line: " + std::to_string(entry2->get_line()));
                                        // else if (!IsVariable(entry1) || (!IsVariable(entry2))
                                        //     SIGNALERROR("Use of == with non variable type");
                                        // else if(entry1->get_type()!=CONTST_NUM || entry2->get_type()!=CONST_NUM)
                                        //     LOGWARNING("Entries must be type of Number");
                                        // else{
                                            auto equal_quad = Emit(IF_EQ_t, $1, $3, nullptr, yylineno);
                                            equal_quad->PatchBranchQuad(equal_quad->label + 2);

                                            auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);
                                            jump_quad->PatchJumpQuad(jump_quad->label + 3);

                                            auto temp = NewTemp();

                                            Emit(ASSIGN_t, temp, new BoolConstant(true), nullptr, yylineno);

                                            jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);
                                            jump_quad->PatchJumpQuad(jump_quad->label + 2);

                                            Emit(ASSIGN_t, temp, new BoolConstant(false), nullptr, yylineno);

                                            $$ = temp;
                                        //}
                                        DLOG("expr -> expr == expr");
                                    }
            | expr NOTEQUAL expr    {
                                        // auto entry1 = $1;
                                        // auto entry2 = $3;
                                        // if(entry1 == nullptr)
                                        //     SIGNALERROR("Cannot access " + entry1->get_id() + ", previously defined in line: " + std::to_string(entry1->get_line()));
                                        // else if (!entry1->is_active())
                                        //     SIGNALERROR("Cannot access " + entry1->get_id() + ", previously defined in line: " + std::to_string(entry1->get_line()));
                                        // else if(entry2 == nullptr)
                                        //     SIGNALERROR("Cannot access " + entry2->get_id() + ", previously defined in line: " + std::to_string(entry2->get_line()));
                                        // else if (!entry2->is_active())
                                        //     SIGNALERROR("Cannot access " + entry2->get_id() + ", previously defined in line: " + std::to_string(entry2->get_line()));
                                        // else if (!IsVariable(entry1) || (!IsVariable(entry2))
                                        //     SIGNALERROR("Use of != with non variable type");
                                        // else if(entry1->get_type()!=CONTST_NUM || entry2->get_type()!=CONST_NUM)
                                        //     LOGWARNING("Entries must be type of Number");
                                        // else{
                                            auto not_equal_quad = Emit(IF_NOTEQ_t, $1, $3, nullptr, yylineno);
                                            not_equal_quad->PatchBranchQuad(not_equal_quad->label + 2);

                                            auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);
                                            jump_quad->PatchJumpQuad(jump_quad->label + 3);

                                            auto temp = NewTemp();

                                            Emit(ASSIGN_t, temp, new BoolConstant(true), nullptr, yylineno);

                                            jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);
                                            jump_quad->PatchJumpQuad(jump_quad->label + 2);

                                            Emit(ASSIGN_t, temp, new BoolConstant(false), nullptr, yylineno);

                                            $$ = temp;
                                       // } 
                                        DLOG("expr -> expr != expr");
                                    }
            | expr AND expr         {
                                        // auto entry1 = $1;
                                        // auto entry2 = $3;
                                        // if(entry1 == nullptr)
                                        //     SIGNALERROR("Cannot access " + entry1->get_id() + ", previously defined in line: " + std::to_string(entry1->get_line()));
                                        // else if (!entry1->is_active())
                                        //     SIGNALERROR("Cannot access " + entry1->get_id() + ", previously defined in line: " + std::to_string(entry1->get_line()));
                                        // else if(entry2 == nullptr)
                                        //     SIGNALERROR("Cannot access " + entry2->get_id() + ", previously defined in line: " + std::to_string(entry2->get_line()));
                                        // else if (!entry2->is_active())
                                        //     SIGNALERROR("Cannot access " + entry2->get_id() + ", previously defined in line: " + std::to_string(entry2->get_line()));
                                        // else if (!IsVariable(entry1) || (!IsVariable(entry2))
                                        //     SIGNALERROR("Use of AND with non variable type");
                                        // else if(entry1->get_type()!=CONTST_NUM || entry2->get_type()!=CONST_NUM)
                                        //     LOGWARNING("Entries must be type of Number");
                                        // else{
                                        //     //  TODO
                                        // }
                                        DLOG("expr -> expr and expr");
                                    }
            | expr OR expr          {
                                        // auto entry1 = $1;
                                        // auto entry2 = $3;
                                        // if(entry1 == nullptr)
                                        //     SIGNALERROR("Cannot access " + entry1->get_id() + ", previously defined in line: " + std::to_string(entry1->get_line()));
                                        // else if (!entry1->is_active())
                                        //     SIGNALERROR("Cannot access " + entry1->get_id() + ", previously defined in line: " + std::to_string(entry1->get_line()));
                                        // else if(entry2 == nullptr)
                                        //     SIGNALERROR("Cannot access " + entry2->get_id() + ", previously defined in line: " + std::to_string(entry2->get_line()));
                                        // else if (!entry2->is_active())
                                        //     SIGNALERROR("Cannot access " + entry2->get_id() + ", previously defined in line: " + std::to_string(entry2->get_line()));
                                        // else if (!IsVariable(entry1) || (!IsVariable(entry2))
                                        //     SIGNALERROR("Use of OR with non variable type");
                                        // else if(entry1->get_type()!=CONTST_NUM || entry2->get_type()!=CONST_NUM)
                                        //     LOGWARNING("Entries must be type of Number");
                                        // else{
                                        //     //  TODO
                                        // }
                                        DLOG("expr -> assignexpr");
                                    }
            | term                  {
                                        DLOG("expr -> term");
                                    }
            ;

term:         '(' expr ')'          {
                                        DLOG("term -> (expr)");
                                    }
            | '-' expr %prec UMINUS {
                                        auto symbol = $2;
                                        if (symbol->get_type() == CONST_BOOL) {
                                            SIGNALERROR("Illegal use of unary minus on constant boolean");
                                        }
                                        else {
                                            auto temp = NewTemp();
                                            Emit(UMINUS_t, temp, symbol, nullptr, yylineno);
                                            $$ = symbol;
                                        }

                                        DLOG("term -> -expr");
                                    }
            | NOT expr              {
                                        auto equal_quad = Emit(IF_EQ_t, $2, new BoolConstant(true),  nullptr, yylineno);
                                        equal_quad->PatchBranchQuad(equal_quad->label + 4);

                                        auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);
                                        jump_quad->PatchJumpQuad(jump_quad->label + 1);

                                        auto temp = NewTemp();

                                        Emit(ASSIGN_t, temp, new BoolConstant(true), nullptr, yylineno);

                                        jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);
                                        jump_quad->PatchJumpQuad(jump_quad->label + 2);

                                        Emit(ASSIGN_t, temp, new BoolConstant(false), nullptr, yylineno);

                                        $$ = temp;
                                        DLOG("term -> not expr");
                                    }
            | PLUSPLUS lvalue       {
                                        auto symbol = $2;
                                        if(symbol == nullptr)
                                            SIGNALERROR("Cannot access " + symbol->get_id() + ", previously defined in line: " + std::to_string(symbol->get_line()));
                                        else if (!symbol->is_active())
                                            SIGNALERROR("Cannot access " + symbol->get_id() + ", previously defined in line: " + std::to_string(symbol->get_line()));    
                                        else if (!IsVariable(symbol))
                                            SIGNALERROR("Use of increment operator with non variable type");   
                                        else {
                                            auto temp = NewTemp(); 
                                               
                                            Emit(ADD_t, symbol, symbol, new IntConstant(1), yylineno);
                                            Emit(ASSIGN_t, temp, symbol, nullptr, yylineno);
                                            
                                            $$ = temp;
                                        }     

                                        DLOG("term -> ++lvalue"); 
                                    }
            | lvalue PLUSPLUS       {
                                        auto symbol = $1;
                                        if(symbol == nullptr)
                                            SIGNALERROR("Cannot access " + symbol->get_id() + ", previously defined in line: " + std::to_string(symbol->get_line()));
                                        else if (!symbol->is_active())
                                            SIGNALERROR("Cannot access " + symbol->get_id() + ", previously defined in line: " + std::to_string(symbol->get_line()));    
                                        else if (!IsVariable(symbol))
                                            SIGNALERROR("Use of increment operator with non variable type");   
                                        else {
                                            auto temp = NewTemp(); 

                                            Emit(ASSIGN_t, temp, symbol, nullptr, yylineno);    
                                            Emit(ADD_t, symbol, symbol, new IntConstant(1), yylineno);

                                            $$ = temp;
                                        }     
                                        DLOG("term -> lvalue++"); }
            | MINUSMINUS lvalue     { 
                                        auto symbol = $2;
                                        if(symbol == nullptr)
                                            SIGNALERROR("Cannot access " + symbol->get_id() + ", previously defined in line: " + std::to_string(symbol->get_line()));
                                        else if (!symbol->is_active())
                                            SIGNALERROR("Cannot access " + symbol->get_id() + ", previously defined in line: " + std::to_string(symbol->get_line()));    
                                        else if (!IsVariable(symbol))
                                            SIGNALERROR("Use of decrement operator with non variable type");   
                                        else {
                                            auto temp = NewTemp(); 
                                               
                                            Emit(SUB_t, symbol, symbol, new IntConstant(1), yylineno);
                                            Emit(ASSIGN_t, temp, symbol, nullptr, yylineno); 

                                            $$ = temp;
                                        }     
                                        DLOG("term -> --lvaule");
                                    }
            | lvalue MINUSMINUS     { 
                                        auto symbol = $1;
                                        if(symbol == nullptr)
                                            SIGNALERROR("Cannot access " + symbol->get_id() + ", previously defined in line: " + std::to_string(symbol->get_line()));
                                        else if (!symbol->is_active())
                                            SIGNALERROR("Cannot access " + symbol->get_id() + ", previously defined in line: " + std::to_string(symbol->get_line()));    
                                        else if (!IsVariable(symbol))
                                            SIGNALERROR("Use of decrement operator with non variable type");
                                        else {
                                            {
                                            auto temp = NewTemp(); 
                                               
                                            Emit(ASSIGN_t, temp, symbol, nullptr, yylineno);    
                                            Emit(SUB_t, symbol, symbol, new IntConstant(1), yylineno);

                                            $$ = temp;
                                        } 
                                        }    
                                        DLOG("term -> lvalue--");
                                    }
            | primary               {   
                                        DLOG("term -> primary"); 
                                    }
            ;

assignexpr:   lvalue '=' expr       {
                                        
                                        auto symbol = $1;
                                        if (symbol != nullptr) {
                                            if (IsLibraryFunction(symbol) || IsUserFunction(symbol)) {
                                                SIGNALERROR("Functions are constant their value cannot be changed");
                                            }
                                            else {
                                                auto assign_quad = Emit(ASSIGN_t, symbol, nullptr, $3, yylineno);
                                                $$ = assign_quad->result;
                                            }
                                        }
                                            
                                        DLOG("assignexpr -> lvalue = expr");
                                    }
            ;

primary:      lvalue                {
                                        $$ = $1;
                                        DLOG("primary -> lvalue");
                                    }
            | call                  {
                                        $$ = $1;
                                        DLOG("primary -> call");
                                    }
            | objectdef             {
                                        DLOG("primary -> objectdef");
                                    }
            | '(' funcdef ')'       {
                                        $$ = $2;
                                        DLOG("primary -> (funcdef)");
                                    }
            | const                 {
                                        $$ = $1;
                                        DLOG("primary -> const");
                                    }
            ;

lvalue:       ID                    {
                                        Symbol* symbol;
                                        if (ScopeIsGlobal()) {
                                            symbol = program_stack.LookupGlobal($1);
                                            if(symbol == nullptr) {
                                                symbol = InsertGlobalVariable($1, yylineno);
                                            }
                                            else if (!symbol->is_active()) {
                                                SIGNALERROR("Cannot access " + symbol->get_id() + ", previously defined in line: " + std::to_string(symbol->get_line()));
                                            }
                                            $$ = symbol;
                                        }
                                        else {
                                            symbol = program_stack.Lookup($1);
                                            if (symbol == nullptr) {
                                                symbol = InsertLocalVariable($1, yylineno);
                                            }
                                            else if (!symbol->is_active()) {
                                                SIGNALERROR("Cannot access " + symbol->get_id() + ", previously defined in line: " + std::to_string(symbol->get_line()));
                                            }
                                            $$ = symbol;
                                        }
                                        DLOG("lvalue -> id");
                                    }
            | LOCAL ID              {
                                        auto symbol = program_stack.Lookup($2);
                                        if (symbol == nullptr) { 
                                            symbol = InsertLocalVariable($2, yylineno);
                                        }
                                        else if (!symbol->is_active()) {
                                            SIGNALERROR("Cannot access " + symbol->get_id() + ", previously defined in line: " + std::to_string(symbol->get_line()));
                                        }
                                        else if (IsGlobalVar(symbol)) {
                                            symbol = InsertLocalVariable($2, yylineno);
                                        }
                                        else if (IsUserFunction(symbol)){
                                            if(IsAtCurrentScope(symbol)) {
                                                SIGNALERROR("Attempting to redefine a previously declared user function");
                                            }
                                            else {
                                                symbol = InsertLocalVariable($2, yylineno);
                                            }    
                                        }
                                        else if (IsLibraryFunction(symbol)) {
                                            SIGNALERROR("Attempting to redefine a library function");
                                        }
                                        $$ = symbol;
                                        DLOG("lvalue -> local id");
                                    }
            | COLONCOLON ID         {
                                        auto symbol = program_stack.LookupGlobal($2);
                                        if (symbol == nullptr || !symbol->is_active()) 
                                            SIGNALERROR("No global variable with id: " + std::string($2));

                                        $$ = symbol;
                                        DLOG("lvalue -> ::id");
                                    }
            | member                {
                                        DLOG("lvalue -> member");
                                    }
            ;

member:     lvalue '.' ID           {
                                        $$=$3;
                                        DLOG("member -> lvalue.id");
                                    }
            | lvalue '[' expr ']'   {
                                        DLOG("member -> lvalue[expr]");
                                    }
            | call '.' ID           {
                                        $$=$3;
                                        DLOG("member -> call.id");
                                    }
            | call '[' expr ']'     {
                                        DLOG("member -> call[expr]");
                                    }
            ;

call:       call  '(' elist ')'             {
                                                DLOG("call -> call(elist)");
                                            }
            | lvalue                        {
                                                
                                                auto called_symbol = $1;
                                                auto function_call = new FunctionCall(called_symbol, std::list<Expression*>());
                                                
                                                PushCallStackFrame(function_call);

                                                $<funcCall>$ = function_call;
                                            }
            callsuffix                      {
                                                auto function_call = PopCallStackFrame();
                                                auto called_symbol = function_call->get_called_symbol();

                                                $<funcCall>$ = function_call;

                                                auto temp_value = NewTemp();
                        
                                                Emit(CALL_t, called_symbol, nullptr, nullptr, yylineno);    
                                                Emit(GETRETVAL_t, temp_value, nullptr, nullptr, yylineno);

                                                function_call->set_ret_val(temp_value->get_id());

                                                if (IsLibraryFunction(called_symbol) || IsUserFunction(called_symbol)) {
                                                    auto args_num = static_cast<Function*>(called_symbol)->get_formal_arguments().size();
                                                    auto call_args_num = function_call->get_params().size();

                                                    if (call_args_num < args_num)
                                                        SIGNALERROR("Too few arguments passed to function: " << called_symbol->get_id() << ", defined in line: " << std::to_string(called_symbol->get_line()));
                                                    else if (call_args_num > args_num)
                                                        LOGWARNING("Too many arguments passed to function: " << called_symbol->get_id() << ", defined in line: " << std::to_string(called_symbol->get_line()));
                                                }

                                                DLOG("call -> lvalue callsuffix");
                                            }
            | '(' funcdef ')'               {                                                
                                                auto called_symbol = $2;
                                                auto function_call = new FunctionCall(called_symbol, std::list<Expression*>());

                                                PushCallStackFrame(function_call);

                                                $<funcCall>$ = function_call;
                                            }
            '(' elist ')'                   {
                                                auto function_call = PopCallStackFrame();
                                                auto called_symbol = function_call->get_called_symbol();

                                                $<funcCall>$ = function_call;

                                                auto temp_value = NewTemp();

                                                Emit(CALL_t, called_symbol, nullptr, nullptr, yylineno);
                                                Emit(GETRETVAL_t, temp_value, nullptr, nullptr, yylineno);

                                                function_call->set_ret_val(temp_value->get_id());

                                                if (IsLibraryFunction(called_symbol) || IsUserFunction(called_symbol)) {
                                                    auto args_num = static_cast<Function*>(called_symbol)->get_formal_arguments().size();
                                                    auto call_args_num = function_call->get_params().size();

                                                    if (call_args_num < args_num)
                                                        SIGNALERROR("Too few arguments passed to function: " << called_symbol->get_id() << ", defined in line: " << std::to_string(called_symbol->get_line()));
                                                    else if (call_args_num > args_num)
                                                        LOGWARNING("Too many arguments passed to function: " << called_symbol->get_id() << ", defined in line: " << std::to_string(called_symbol->get_line()));
                                                }

                                                DLOG("call -> (funcdef)(elist)");
                                            }
            ;

callsuffix: normcall        {
                                DLOG("callsuffix -> normcall");
                            }
            | methodcall    {
                                DLOG("callsuffix -> methodcall");
                            }
            ;

normcall:   '(' elist ')'   {
                                DLOG("normcall -> (elist)"); 
                            }
            ;

methodcall: DOTDOT ID '(' elist ')' {
                                        DLOG("methodcall -> ..id(elist)");
                                    }
            ;

multelist:  ',' expr multelist  {
                                    if (GetCallDepth() != 0) {
                                        PushCallParam($2);
                                        Emit(PARAM_t, $2, nullptr, nullptr, yylineno);
                                    }
                                    DLOG("multelist -> ,expr multelist");
                                }
            |                   {
                                    DLOG("multelist -> EMPTY");
                                }
            ;

elist:      expr multelist  {
                                if (GetCallDepth() != 0) {
                                    PushCallParam($1);
                                    Emit(PARAM_t, $1, nullptr, nullptr, yylineno);
                                }
                                             
                                DLOG("elist -> expr multelist");
                            }
            |               {
                                DLOG("elist -> EMPTY");
                            }
            ;

objectdef:  '[' elist ']'       {
                                    DLOG("objectdef -> [elist]");
                                }
            | '[' indexed ']'   { 
                                    DLOG("objectdef -> [indexed]");
                                }
            ;

multindexed:',' indexedelem multindexed {
                                            DLOG("multindexed -> , indexedelem multidexed"); 
                                        }
            |                           {
                                            DLOG("elsestmt -> EMPTY");
                                        }
            ;

indexed:    indexedelem multindexed {
                                        DLOG("indexed -> indexedelem multidexed"); 
                                    }
            ;

indexedelem:'{' expr ':' expr '}'   {
                                        DLOG("indexedelem -> { expr : expr }"); 
                                    }
            ;

block:      '{'         {
                            IncreaseScope();
                            PushStashedFormalArguments();
                        }
            stmts '}'   {
                            DecreaseScope();
                            DLOG("block -> { stmts }");
                        }
            ;

funcdef:    FUNCTION 
                '(' idlist ')'  
                            {
                                auto function = InsertUserFunction(yylineno);

                                auto func_def_stmt = new FuncDefStmt(function);

                                func_def_stmts.push(func_def_stmt);
                                
                                auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);
                                Emit(FUNCSTART_t, function, nullptr, nullptr, yylineno);

                                func_def_stmt->set_func_start_jump_quad(jump_quad);

                                HideLowerScopes();

                                $<sym>$ = function;
                            }
            block           {
                                auto top_func_def = func_def_stmts.top();
                                auto function = top_func_def->get_sym();
                                
                                auto func_end_quad = Emit(FUNCEND_t, function, nullptr, nullptr, yylineno);

                                top_func_def->PatchFuncStartJumpQuad(func_end_quad->label + 1);
                                top_func_def->PatchReturnJumpQuads(func_end_quad->label);

                                EnableLowerScopes();

                                func_def_stmts.pop();

                                $<sym>$ = function;
                                DLOG("funcdef -> function (idlist) block "); 
                            }
            | FUNCTION ID 
                '(' idlist ')'
                            {
                                auto symbol = program_stack.Lookup($2);
                                if (symbol == nullptr) {
                                    auto function = InsertUserFunction($2, yylineno);
                                    symbol = function;
                                }
                                else if (!symbol->is_active()) {
                                    SIGNALERROR("Cannot access " + symbol->get_id() + ", previously defined in line: " + std::to_string(symbol->get_line()));
                                }
                                else {
                                    if (IsVariable(symbol)) {
                                        SIGNALERROR(std::string($2) + " variable, previously defined in line: " + std::to_string(symbol->get_line()) + ", cannot be redefined as a function");
                                    }
                                    else if (IsLibraryFunction(symbol)) {
                                        SIGNALERROR(std::string($2) + " library function cannot be shadowed by a user function");
                                    }
                                    else if (IsAtCurrentScope(symbol)) {
                                        std::string message =  "Name collision with function " +  std::string($2) + ", previously defined in line: ";
                                        message += std::to_string(symbol->get_line());
                                        SIGNALERROR(message);
                                    }
                                    else{
                                        auto function = InsertUserFunction($2, yylineno); //Shadow user function. 
                                        symbol = function;
                                    }
                                }
                                auto func_def_stmt = new FuncDefStmt(symbol); //May want to move to else
                                func_def_stmts.push(func_def_stmt);

                                auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);
                                Emit(FUNCSTART_t, symbol, nullptr, nullptr, yylineno);

                                func_def_stmt->set_func_start_jump_quad(jump_quad);

                                HideLowerScopes();

                                $<sym>$ = symbol;
                            }
            block           { 
                                auto top_func_def =  func_def_stmts.top();
                                Symbol* function;
                                if (top_func_def != nullptr) {
                                    function = top_func_def->get_sym();
                                    
                                    auto func_end_quad = Emit(FUNCEND_t, function, nullptr, nullptr, yylineno);
                                    
                                    top_func_def->PatchFuncStartJumpQuad(func_end_quad->label + 1);
                                    top_func_def->PatchReturnJumpQuads(func_end_quad->label);

                                    func_def_stmts.pop();
                                }
                                
                                EnableLowerScopes();

                                $<sym>$ = function;

                                DLOG("funcdef -> function id (idlist) block"); 
                            }
            ;

const:        INTNUM    {
                            $$ = new IntConstant($1);
                            DLOG("const -> INTNUM");
                        }
            | DOUBLENUM { 
                            $$ = new DoubleConstant($1); 
                            DLOG("const -> DOUBLENUM"); 
                        }
            | STRING    { 
                            $$ = new StringConstant($1);
                            DLOG("const -> STRING"); 
                        }
            | NIL       { 
                            $$ = new NilConstant(nullptr);
                            DLOG("const -> NIL"); 
                        }
            | TRUE      { 
                            $$ = new BoolConstant(true);
                            DLOG("const -> TRUE"); 
                        }
            | FALSE     {   
                            $$ = new BoolConstant(false);
                            DLOG("const -> FALSE");
                        }
            ;

multid:     ',' ID  {
                        StashFormalArgument($2, yylineno);
                    } 
            multid  {
                        DLOG("multid -> , id multid");
                    }
            |       {
                        DLOG("multid -> EMPTY");
                    }
            ;

idlist:     ID      {
                        StashFormalArgument($1, yylineno);
                    } 
            multid  { 
                        DLOG("idlist -> id multid"); 
                    }
            |       {
                        DLOG("idlist -> EMPTY");
                    }
            ;

ifstmt:     IF '(' expr ')'                 {
                                                IncreaseIfStmt();

                                                auto if_stmt = GetIfStmt();

                                                auto branch_quad = Emit(IF_EQ_t, $3, new BoolConstant(true), nullptr, yylineno);
                                                branch_quad->PatchBranchQuad(branch_quad->label + 2);

                                                auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno); 
                                                MapIfStmtJumpQuad(if_stmt, jump_quad);
                                            }
            stmt                            {
                                                ResetTemp();
                                            }
            elsestmt                        {
                                                DLOG("ifstmt -> if (expr) stmt elsestmt"); 
                                            }
            ;

elsestmt:   ELSE            {
                                auto if_stmt = GetIfStmt();

                                auto else_jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);
                                PushElseJumpQuad(if_stmt, else_jump_quad);

                                auto patch_label = GetBackQuadLabel() + 1;
                                PatchIfStmtJumpQuad(if_stmt, patch_label);
                            }
            stmt            {
                                auto if_stmt = GetIfStmt();
                                PatchElseJumpQuad(if_stmt);

                                DecreaseIfStmt();

                                ResetTemp();

                                DLOG("elsestmt -> else stmt"); 
                            }
            |               {
                                auto if_stmt = GetIfStmt();

                                auto patch_label = GetBackQuadLabel() + 1;
                                PatchIfStmtJumpQuad(if_stmt, patch_label);

                                DecreaseIfStmt();
                                DLOG("elsestmt -> EMPTY");
                            }
            ;

whilestmt:  WHILE               { 
                                    auto first_quad_label = GetBackQuadLabel() + 1;
                                    auto while_stmt = new WhileStmt(first_quad_label);

                                    while_stmts.push(while_stmt);
                                    loop_stmts.push(while_stmt);
                                }
            '(' expr ')'        {
                                    auto top_while_stmt = while_stmts.top();

                                    auto branch_quad = Emit(IF_EQ_t, $4, new BoolConstant(true), nullptr, yylineno);
                                    auto exit_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);

                                    unpatched_while_quads_by_while_stmt[top_while_stmt].push_back(branch_quad);
                                    unpatched_while_quads_by_while_stmt[top_while_stmt].push_back(exit_quad);
                                }
            stmt                { 
                                    auto top_while_stmt = while_stmts.top();

                                    auto loop_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);

                                    auto top_while_stmt_unpatched_quads = unpatched_while_quads_by_while_stmt[top_while_stmt];

                                    auto exit_quad = top_while_stmt_unpatched_quads.back();
                                    top_while_stmt_unpatched_quads.pop_back();

                                    auto branch_quad = top_while_stmt_unpatched_quads.back();
                                    top_while_stmt_unpatched_quads.pop_back();

                                    auto top_while_stmt_first_quad_label = top_while_stmt->get_first_quad_label();
                                    loop_quad->PatchJumpQuad(top_while_stmt_first_quad_label);

                                    exit_quad->PatchJumpQuad(loop_quad->label + 1);

                                    branch_quad->PatchBranchQuad(branch_quad->label + 2);

                                    auto top_while_stmt_unpatched_break_quads = unpatched_break_quads_by_loop_stmt[top_while_stmt];
                                    for (auto top_while_stmt_unpatched_break_quad : top_while_stmt_unpatched_break_quads)
                                        top_while_stmt_unpatched_break_quad->PatchJumpQuad(loop_quad->label + 1);

                                    auto top_while_stmt_unpatched_continue_quads = unpatched_continue_quads_by_loop_stmt[top_while_stmt];
                                    for (auto top_while_stmt_unpatched_continue_quad : top_while_stmt_unpatched_continue_quads)
                                        top_while_stmt_unpatched_continue_quad->PatchJumpQuad(top_while_stmt_first_quad_label);    

                                    while_stmts.pop();
                                    loop_stmts.pop();

                                    ResetTemp();

                                    DLOG ("whilestmt -> WHILE (expr) stmt"); 
                                }
            ;

forstmt:    FOR                                     {
                                                        auto first_quad_label = GetBackQuadLabel() + 1;
                                                        auto for_stmt = new ForStmt(first_quad_label);

                                                        for_stmts.push(for_stmt);
                                                        loop_stmts.push(for_stmt);
                                                    }                   
            '(' elist ';'                           {
                                                        auto top_for_stmt = for_stmts.top();

                                                        auto logical_expr_first_quad_label = GetBackQuadLabel() + 1;
                                                        logical_expr_first_quad_label_by_for_stmt.insert({top_for_stmt, logical_expr_first_quad_label});
                                                    }
            expr ';'                                {
                                                        auto top_for_stmt = for_stmts.top();

                                                        auto branch_quad = Emit(IF_EQ_t, $7, new BoolConstant(true), nullptr, yylineno);
                                                        unpatched_for_quads_by_for_stmt[top_for_stmt].push_back(branch_quad);

                                                        auto exit_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);
                                                        unpatched_for_quads_by_for_stmt[top_for_stmt].push_back(exit_quad);

                                                        auto exprs_first_quad_label = GetBackQuadLabel() + 1;
                                                        exprs_first_quad_label_by_for_stmt.insert({top_for_stmt, exprs_first_quad_label});
                                                    }
            elist ')'                               {
                                                        auto top_for_stmt = for_stmts.top();

                                                        auto loop_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);
                                                        unpatched_for_quads_by_for_stmt[top_for_stmt].push_back(loop_quad);
                                                    }
            stmt                                    {
                                                        auto top_for_stmt = for_stmts.top();

                                                        auto expr_jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno); 

                                                        auto top_for_stmt_unpatched_for_quads = unpatched_for_quads_by_for_stmt[top_for_stmt];

                                                        auto loop_quad = top_for_stmt_unpatched_for_quads.back();
                                                        top_for_stmt_unpatched_for_quads.pop_back();

                                                        auto exit_quad = top_for_stmt_unpatched_for_quads.back();
                                                        top_for_stmt_unpatched_for_quads.pop_back();

                                                        auto branch_quad = top_for_stmt_unpatched_for_quads.back();
                                                        top_for_stmt_unpatched_for_quads.pop_back(); 

                                                        auto top_for_stmt_exprs_first_quad_label = exprs_first_quad_label_by_for_stmt[top_for_stmt];    // e.g the "i++" expression in the "for (i; i < 2; i++)" stmt
                                                                                                                                                        // has an first quad with a label. Its value is stored in this label.
                                                        expr_jump_quad->PatchJumpQuad(top_for_stmt_exprs_first_quad_label);

                                                        auto top_for_stmt_logical_expr_first_quad_label = logical_expr_first_quad_label_by_for_stmt[top_for_stmt];      // e.g the "i<2" expression in the "for (i; i < 2; i++)" stmt
                                                                                                                                                                        // has a first quad with a label. Its value is stored in
                                                                                                                                                                        //this label.
                                                        loop_quad->PatchJumpQuad(top_for_stmt_logical_expr_first_quad_label);
                                                        exit_quad->PatchJumpQuad(expr_jump_quad->label+1);
                                                        branch_quad->PatchBranchQuad(loop_quad->label + 1);

                                                        auto top_for_stmt_unpatched_break_quads = unpatched_break_quads_by_loop_stmt[top_for_stmt];
                                                        for (auto top_for_stmt_unpatched_break_quad : top_for_stmt_unpatched_break_quads)
                                                            top_for_stmt_unpatched_break_quad->PatchJumpQuad(GetBackQuadLabel() + 1);

                                                        auto top_for_stmt_unpatched_continue_quads = unpatched_continue_quads_by_loop_stmt[top_for_stmt];
                                                        for (auto top_for_stmt_unpatched_continue_quad : top_for_stmt_unpatched_continue_quads)
                                                            top_for_stmt_unpatched_continue_quad->PatchJumpQuad(top_for_stmt_exprs_first_quad_label);

                                                        for_stmts.pop();
                                                        loop_stmts.pop();    

                                                        ResetTemp();

                                                        DLOG("forstmt -> FOR ( elist ; expr ; elist ) stmt"); 
                                                    }
            ;

returnstmt: RETURN      {
                            if (func_def_stmts.size() == 0) {
                                SIGNALERROR("Invalid return, used outside a function block");
                            } else {
                                auto top_func_def = func_def_stmts.top();

                                Emit(RET_t, nullptr, nullptr, nullptr, yylineno);
                                auto return_jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);

                                top_func_def->PushReturnJumpQuad(return_jump_quad);
                            }
                        } 
            ';'         {
                            DLOG("returnstmt -> RETURN;"); 
                        }
            | RETURN    
            expr ';'    {
                            if (func_def_stmts.size() == 0) 
                                SIGNALERROR("Invalid return, used outside a function block");
                            else {
                                auto top_func_def = func_def_stmts.top();

                                Emit(RET_t, $2, nullptr, nullptr, yylineno);
                                auto return_jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);

                                top_func_def->PushReturnJumpQuad(return_jump_quad);

                                DLOG("returnstmt -> RETURN expr;");
                            }    
                        }
            ;

%%

int yyerror(std::string yaccProvidedMessage) {
    std::cout << yaccProvidedMessage << ": at line " << yylineno << ", before token: " << yytext << std::endl;
    std::cout << "INPUT NOT VALID" << std::endl;
    SignalError();
    return 1;
}

#ifndef TESTING
int main(int argc, char** argv) {
    if (argc > 1) {
        if (!(yyin = fopen(argv[1], "r"))) {
            fprintf(stderr, "Cannot read file: %s\n", argv[1]);
            return 1;
        }
    }
    else {
        yyin = stdin;
    }
    
    InitLibraryFunctions();

    yyparse();

    #if defined LOGQUADS
        if (NoErrorSignaled()) {
            for (auto quad : quads) 
                std::cout << *quad << std::endl;
        }
    #endif         
    #if defined LOGSYMTABLE
        if (NoErrorSignaled()) 
            std::cout << symbol_table; 
    #endif

    return 0;
}
#endif

void IncreaseScope() {
    Block* new_block = new Block();
    symbol_table.Insert(++current_scope, new_block);
    program_stack.Push(new_block);
}

void DecreaseScope() {
    program_stack.Top()->Deactivate();
    program_stack.Pop();
    --current_scope;
}

void HideLowerScopes() {
    if (current_scope != global_scope)
        program_stack.Top()->Deactivate();
    if (current_scope > 1)
        program_stack.DeactivateLowerScopes();
}

void EnableLowerScopes() {
    program_stack.ActivateLowerScopes();
}

void InitLibraryFunctions() {
    IncreaseScope(); 
    program_stack.Top()->Insert(new LibraryFunction("print", LIB_FUNC_LINE, global_scope, program_var_offset++));
    program_stack.Top()->Insert(new LibraryFunction("input", LIB_FUNC_LINE, global_scope, program_var_offset++));
    program_stack.Top()->Insert(new LibraryFunction("objectmemberkeys", LIB_FUNC_LINE, global_scope, program_var_offset++));
    program_stack.Top()->Insert(new LibraryFunction("objecttotalmembers", LIB_FUNC_LINE, global_scope, program_var_offset++));
    program_stack.Top()->Insert(new LibraryFunction("objectcopy", LIB_FUNC_LINE, global_scope, program_var_offset++));
    program_stack.Top()->Insert(new LibraryFunction("totalarguments", LIB_FUNC_LINE, global_scope, program_var_offset++));
    program_stack.Top()->Insert(new LibraryFunction("argument", LIB_FUNC_LINE, global_scope, program_var_offset++));
    program_stack.Top()->Insert(new LibraryFunction("typeof", LIB_FUNC_LINE, global_scope, program_var_offset++));
    program_stack.Top()->Insert(new LibraryFunction("strtonum", LIB_FUNC_LINE, global_scope, program_var_offset++));
    program_stack.Top()->Insert(new LibraryFunction("sqrt", LIB_FUNC_LINE, global_scope, program_var_offset++));
    program_stack.Top()->Insert(new LibraryFunction("cos", LIB_FUNC_LINE, global_scope, program_var_offset++));
    program_stack.Top()->Insert(new LibraryFunction("sin", LIB_FUNC_LINE, global_scope, program_var_offset++));
}

Symbol* InsertLocalVariable(const char* name, unsigned int line) {
    assert(name != nullptr);

    Symbol* symbol;
    if (func_def_stmts.size() == 0) {
        symbol = new LocalVariable(name, line, current_scope, PROGRAM_VAR, program_var_offset++);
    }
    else {
        symbol = new LocalVariable(name, line, current_scope, FUNCTION_LOCAL, func_def_stmts.top()->get_offset());  
        func_def_stmts.top()->IncreaseOffset();
    }
    program_stack.Top()->Insert(symbol);

    return symbol;
}
Symbol* InsertGlobalVariable(const char* name, unsigned int line) {
    assert(name != nullptr);

    Symbol* symbol = new GlobalVariable(name, line, current_scope, program_var_offset++);
    program_stack.Top()->Insert(symbol);
    
    return symbol;
}

Symbol* InsertUserFunction(const char* name, unsigned int line) {
    assert(name != nullptr);

    Symbol* symbol;
    if (func_def_stmts.size() == 0) {
        symbol = new UserFunction(name, line, current_scope, PROGRAM_VAR, program_var_offset++, stashed_formal_arguments);
    }
    else {
        symbol = new UserFunction(name, line, current_scope, FUNCTION_LOCAL, func_def_stmts.top()->get_offset(), stashed_formal_arguments);  
        func_def_stmts.top()->IncreaseOffset();
    }
    program_stack.Top()->Insert(symbol);

    return symbol;
}

Symbol* InsertUserFunction(unsigned int line) {
    std::string an = "$";
    an += std::to_string(++anonymus_funcs_counter);
    Symbol* symbol;
    if (func_def_stmts.size() == 0) {
        symbol = new UserFunction(an, line, current_scope, PROGRAM_VAR, program_var_offset++, stashed_formal_arguments);
    }
    else {
        symbol = new UserFunction(an, line, current_scope, FUNCTION_LOCAL, func_def_stmts.top()->get_offset(), stashed_formal_arguments);  
        func_def_stmts.top()->IncreaseOffset();
    }
    program_stack.Top()->Insert(symbol);
    
    return symbol;
}

void PushStashedFormalArguments(void) { 
    for (auto i : stashed_formal_arguments) {
        program_stack.Top()->Insert(i);
    }
    stashed_formal_arguments.clear();
    formal_args_offset = 0;
}

bool
IsStashed(const char* name) {
    assert(name != nullptr);
    std::string wanted = name;
    for (auto i : stashed_formal_arguments) {
        if (i->get_id() == wanted) {
            return true;
        }
    }
    return false;
}

void StashFormalArgument(const char* name, unsigned int line) {
    assert(name != nullptr);
    if (!IsStashed(name))
        stashed_formal_arguments.push_back(new FormalVariable(name, line, current_scope + 1, formal_args_offset++));
    else {
        std::cout << "Error, formal argument " << name << " already declared, in line: " << line << std::endl;
        SignalError();
    }
}

std::string NewTempName() {
    return  "^" + std::to_string(temp_counter++);
}

Symbol* NewTemp() {
    std::string name = NewTempName();
    Symbol* sym = program_stack.LookupHiddenVariable(name);

    if (sym == nullptr)
    {   
        Symbol* new_temp;
        if (ScopeIsGlobal())
            new_temp = InsertGlobalVariable(name.c_str(), TEMP_LINE);
        else    
            new_temp = InsertLocalVariable(name.c_str(), TEMP_LINE); 

        return new_temp;
    } else {
        return sym;
    }
}

void ResetTemp() {
    temp_counter = 0;
}

Quad*
Emit(Iopcode op, Expression* result, Expression* arg1, Expression* arg2, unsigned int line) {
    unsigned int label = quads.size() + 1;
    Quad* q = new quad(op, result, arg1, arg2, label, line);
    quads.push_back(q);

    return q;
}

unsigned int GetBackQuadLabel() {
    if (quads.size() == 0)
        return 0;
    else     
        return quads.back()->label;
}
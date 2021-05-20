%{
    #include <stdio.h>
    #include <iostream>
    #include <fstream>
    #include <list>
    #include <stack>
    #include <string>
    #include "../include/debuglog.h"
    #include "../include/expression/symbol.h"
    #include "../include/expression/bool_constant.h"
    #include "../include/expression/constant.h"
    #include "../include/expression/nil_constant.h"
    #include "../include/expression/string_constant.h"
    #include "../include/expression/double_constant.h"
    #include "../include/expression/int_constant.h"
    #include "../include/expression/numeric_constant.h"
    #include "../include/expression/call.h"
    #include "../include/expression/tablemake.h"
    #include "../include/expression/tablemake_elems.h"
    #include "../include/expression/tablemake_pairs.h"
    #include "../include/symbol_table.h"
    #include "../include/program_stack.h"
    #include "../include/instruction_opcodes.h"
    #include "../include/quad.h"
    #include "../include/for_stmt.h"
    #include "../include/while_stmt.h"
    #include "../include/loop_stmt.h"
    #include "../include/func_def_stmt.h"
    #include "../include/if_stmt.h"
    
    int yyerror(std::string yaccProvidedMessage);
    int yylex(void);

    extern int yylineno;
    extern char* yytext;
    extern FILE* yyin;

    #define OUT_OF_SCOPE   -1
    #define LIB_FUNC_LINE   0
    #define TEMP_LINE       0

    const unsigned int          global_scope = 0;
    unsigned int                current_scope = OUT_OF_SCOPE;
    
    SymbolTable                 symbol_table;
    ProgramStack                program_stack;  

    unsigned int                program_var_offset = 0;

    std::vector<Quad*>          quads;
    
    std::stack<Call*>           call_exprs;

    std::stack<LoopStmt*>       loop_stmts;
    std::stack<WhileStmt*>      while_stmts;
    std::stack<ForStmt*>        for_stmts;

    std::stack<FuncDefStmt*>    func_def_stmts;  

    std::stack<IfStmt*>         if_stmts;

    std::stack<TableMakeElems*> tablemake_elems_exprs;
    std::stack<TableMakePairs*> tablemake_pairs_exprs;

    bool                        NoErrorSignaled();
    void                        SignalError(std::string msg);
    void                        LogWarning(std::string msg);
    void                        LogQuads(std::ostream& output);
    void                        LogSymTable(std::ostream& output);
                           
    void                        InitLibraryFunctions();

    void                        IncreaseScope();
    void                        DecreaseScope();
    void                        HideLowerScopes();

    void                        DefineSymbol(Symbol* symbol);
    Symbol*                     DefineNewSymbol(ExprType type, const char* symbol);
    Symbol*                     DefineNewAnonymousFunc();
    void                        StashFormalArgument(Symbol* symbol);
    void                        DefineStashedFormalArguments();

    Symbol*                     NewTemp();
    void                        ResetTemp();
    Quad*                       Emit(Iopcode op, Expression* result, Expression* arg1, Expression* arg2);
    unsigned int                NextQuadLabel();

    bool                        IsLibraryFunction(Expression* symbol);
    bool                        IsUserFunction(Expression* symbol);
    bool                        IsVariable(Expression* symbol);
    bool                        IsGlobalVar(Symbol* symbol);
    bool                        IsAtCurrentScope(Symbol* symbol);
    bool                        InLoop();
    bool                        InFuncDef();
    bool                        InCall();
    bool                        InTableMakeElems();
%}

%union {                                                    
    char*                       stringValue;
    int                         intValue;
    double                      doubleValue;
    class Expression*           expression;
    class Constant*             con;
    class Call*                 call;
    class Symbol*               sym;
    class TableMake*            tablemake;
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
%type <call> call
%type <tablemake> objectdef

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
                                        if(!InLoop()) {
                                            SignalError("invalid keyword BREAK outside of loop");
                                        } else {
                                            auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr);

                                            auto top_loop_stmt = loop_stmts.top();
                                            top_loop_stmt->PushBreakJumpQuad(jump_quad);
                                        }

                                        DLOG("stmt -> break;");
                                    }
            | CONTINUE ';'          {
                                        if(!InLoop()) {
                                            SignalError("invalid keyword CONTINUE outside of loop");
                                        } else {
                                            auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr);

                                            auto top_loop_stmt = loop_stmts.top();
                                            top_loop_stmt->PushContinueJumpQuad(jump_quad);
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
                                        Emit(ASSIGN_t, temp, $1, nullptr);

                                        $$ = temp;

                                        DLOG("expr -> assignexpr");
                                    }
            | expr '+' expr         {
                                        auto entry1 = $1;
                                        auto entry2 = $3;
                                        // if(entry1 == nullptr)
                                        //     SignalError("Cannot access " + entry1->get_id() + ", previously defined in line: " + std::to_string(entry1->get_line()));
                                        // else if (!entry1->is_active())
                                        //     SignalError("Cannot access " + entry1->get_id() + ", previously defined in line: " + std::to_string(entry1->get_line()));
                                        // else if(entry2 == nullptr)
                                        //     SignalError("Cannot access " + entry2->get_id() + ", previously defined in line: " + std::to_string(entry2->get_line()));
                                        // else if (!entry2->is_active())
                                        //     SignalError("Cannot access " + entry2->get_id() + ", previously defined in line: " + std::to_string(entry2->get_line()));
                                        // else if (!IsVariable(entry1) || (!IsVariable(entry2))
                                        //     SignalError("Use of addition with non variable type");
                                        // else if(entry1->get_type()!=CONTST_NUM || entry2->get_type()!=CONST_NUM)
                                        //     LogWarning("Entries must be type of Number");        
                                        // else{
                                            auto _t1 = NewTemp(); 
                                            $$ = _t1;
                                            Emit(ADD_t, _t1, entry1, entry2);
                                        //} 
                                        DLOG("expr -> expr + expr");
                                    }
            | expr '-' expr         {
                                        // auto entry1 = $1;
                                        // auto entry2 = $3;
                                        // if(entry1 == nullptr)
                                        //     SignalError("Cannot access " + entry1->get_id() + ", previously defined in line: " + std::to_string(entry1->get_line()));
                                        // else if (!entry1->is_active())
                                        //     SignalError("Cannot access " + entry1->get_id() + ", previously defined in line: " + std::to_string(entry1->get_line()));
                                        // else if(entry2 == nullptr)
                                        //     SignalError("Cannot access " + entry2->get_id() + ", previously defined in line: " + std::to_string(entry2->get_line()));
                                        // else if (!entry2->is_active())
                                        //     SignalError("Cannot access " + entry2->get_id() + ", previously defined in line: " + std::to_string(entry2->get_line()));
                                        // else if (!IsVariable(entry1) || (!IsVariable(entry2))
                                        //     SignalError("Use of subtraction with non variable type");
                                        // else if(entry1->get_type()!=CONTST_NUM || entry2->get_type()!=CONST_NUM)
                                        //     LogWarning("Entries must be type of Number");       
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
                                        //     SignalError("Cannot access " + entry1->get_id() + ", previously defined in line: " + std::to_string(entry1->get_line()));
                                        // else if (!entry1->is_active())
                                        //     SignalError("Cannot access " + entry1->get_id() + ", previously defined in line: " + std::to_string(entry1->get_line()));
                                        // else if(entry2 == nullptr)
                                        //     SignalError("Cannot access " + entry2->get_id() + ", previously defined in line: " + std::to_string(entry2->get_line()));
                                        // else if (!entry2->is_active())
                                        //     SignalError("Cannot access " + entry2->get_id() + ", previously defined in line: " + std::to_string(entry2->get_line()));
                                        // else if (!IsVariable(entry1) || (!IsVariable(entry2))
                                        //     SignalError("Use of multiplication with non variable type");
                                        // else if(entry1->get_type()!=CONTST_NUM || entry2->get_type()!=CONST_NUM)
                                        //     LogWarning("Entries must be type of Number");     
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
                                        //     SignalError("Cannot access " + entry1->get_id() + ", previously defined in line: " + std::to_string(entry1->get_line()));
                                        // else if (!entry1->is_active())
                                        //     SignalError("Cannot access " + entry1->get_id() + ", previously defined in line: " + std::to_string(entry1->get_line()));
                                        // else if(entry2 == nullptr)
                                        //     SignalError("Cannot access " + entry2->get_id() + ", previously defined in line: " + std::to_string(entry2->get_line()));
                                        // else if (!entry2->is_active())
                                        //     SignalError("Cannot access " + entry2->get_id() + ", previously defined in line: " + std::to_string(entry2->get_line()));
                                        // else if (!IsVariable(entry1) || (!IsVariable(entry2))
                                        //     SignalError("Use of division with non variable type");
                                        // else if(entry1->get_type()!=CONTST_NUM || entry2->get_type()!=CONST_NUM)
                                        //     LogWarning("Entries must be type of Number");       
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
                                        //     SignalError("Cannot access " + entry1->get_id() + ", previously defined in line: " + std::to_string(entry1->get_line()));
                                        // else if (!entry1->is_active())
                                        //     SignalError("Cannot access " + entry1->get_id() + ", previously defined in line: " + std::to_string(entry1->get_line()));
                                        // else if(entry2 == nullptr)
                                        //     SignalError("Cannot access " + entry2->get_id() + ", previously defined in line: " + std::to_string(entry2->get_line()));
                                        // else if (!entry2->is_active())
                                        //     SignalError("Cannot access " + entry2->get_id() + ", previously defined in line: " + std::to_string(entry2->get_line()));
                                        // else if (!IsVariable(entry1) || (!IsVariable(entry2))
                                        //     SignalError("Use of modulo with non variable type");
                                        // else if(entry1->get_type()!=CONTST_NUM || entry2->get_type()!=CONST_NUM)
                                        //     LogWarning("Entries must be type of Number");     
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
                                        //     SignalError("Cannot access " + entry1->get_id() + ", previously defined in line: " + std::to_string(entry1->get_line()));
                                        // else if (!entry1->is_active())
                                        //     SignalError("Cannot access " + entry1->get_id() + ", previously defined in line: " + std::to_string(entry1->get_line()));
                                        // else if(entry2 == nullptr)
                                        //     SignalError("Cannot access " + entry2->get_id() + ", previously defined in line: " + std::to_string(entry2->get_line()));
                                        // else if (!entry2->is_active())
                                        //     SignalError("Cannot access " + entry2->get_id() + ", previously defined in line: " + std::to_string(entry2->get_line()));
                                        // else if (!IsVariable(entry1) || (!IsVariable(entry2))
                                        //     SignalError("Use of > with non variable type");
                                        // else if(entry1->get_type()!=CONTST_NUM || entry2->get_type()!=CONST_NUM)
                                        //     LogWarning("Entries must be type of Number");   
                                        // else{
                                            auto greater_quad = Emit(IF_GREATER_t, $1, $3, nullptr);
                                            PatchBranchQuad(greater_quad, greater_quad->label + 2);

                                            auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr);
                                            PatchJumpQuad(jump_quad, jump_quad->label + 3);

                                            auto temp = NewTemp();

                                            Emit(ASSIGN_t, temp, new BoolConstant(true), nullptr);

                                            jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr);
                                            PatchJumpQuad(jump_quad, jump_quad->label + 2);

                                            Emit(ASSIGN_t, temp, new BoolConstant(false), nullptr);

                                            $$ = temp;
                                        //} 
                                        DLOG("expr -> expr > expr");
                                    }
            | expr GEQL expr        {

                                        // auto entry1 = $1;
                                        // auto entry2 = $3;
                                        // if(entry1 == nullptr)
                                        //     SignalError("Cannot access " + entry1->get_id() + ", previously defined in line: " + std::to_string(entry1->get_line()));
                                        // else if (!entry1->is_active())
                                        //     SignalError("Cannot access " + entry1->get_id() + ", previously defined in line: " + std::to_string(entry1->get_line()));
                                        // else if(entry2 == nullptr)
                                        //     SignalError("Cannot access " + entry2->get_id() + ", previously defined in line: " + std::to_string(entry2->get_line()));
                                        // else if (!entry2->is_active())
                                        //     SignalError("Cannot access " + entry2->get_id() + ", previously defined in line: " + std::to_string(entry2->get_line()));
                                        // else if (!IsVariable(entry1) || (!IsVariable(entry2))
                                        //     SignalError("Use of >= with non variable type");
                                        // else if(entry1->get_type()!=CONTST_NUM || entry2->get_type()!=CONST_NUM)
                                        //     LogWarning("Entries must be type of Number");   
                                        // else{
                                            auto greater_equal_quad = Emit(IF_GREATEREQ_t, $1, $3, nullptr);
                                            PatchBranchQuad(greater_equal_quad, greater_equal_quad->label + 2);

                                            auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr);
                                            PatchJumpQuad(jump_quad, jump_quad->label + 3);

                                            auto temp = NewTemp();

                                            Emit(ASSIGN_t, temp, new BoolConstant(true), nullptr);

                                            jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr);
                                            PatchJumpQuad(jump_quad, jump_quad->label + 2);

                                            Emit(ASSIGN_t, temp, new BoolConstant(false), nullptr);

                                            $$ = temp;
                                        //} 
                                        DLOG("expr -> expr >= expr");
                                    }
            | expr '<' expr         {
                                        // auto entry1 = $1;
                                        // auto entry2 = $3;
                                        // if(entry1 == nullptr)
                                        //     SignalError("Cannot access " + entry1->get_id() + ", previously defined in line: " + std::to_string(entry1->get_line()));
                                        // else if (!entry1->is_active())
                                        //     SignalError("Cannot access " + entry1->get_id() + ", previously defined in line: " + std::to_string(entry1->get_line()));
                                        // else if(entry2 == nullptr)
                                        //     SignalError("Cannot access " + entry2->get_id() + ", previously defined in line: " + std::to_string(entry2->get_line()));
                                        // else if (!entry2->is_active())
                                        //     SignalError("Cannot access " + entry2->get_id() + ", previously defined in line: " + std::to_string(entry2->get_line()));
                                        // else if (!IsVariable(entry1) || (!IsVariable(entry2))
                                        //     SignalError("Use of < with non variable type");
                                        // else if(entry1->get_type()!=CONTST_NUM || entry2->get_type()!=CONST_NUM)
                                        //     LogWarning("Entries must be type of Number");
                                        // else{
                                            auto less_quad = Emit(IF_LESS_t, $1, $3, nullptr);
                                            PatchBranchQuad(less_quad, less_quad->label + 2);

                                            auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr);
                                            PatchJumpQuad(jump_quad, jump_quad->label + 3);

                                            auto temp = NewTemp();

                                            Emit(ASSIGN_t, temp, new BoolConstant(true), nullptr);

                                            jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr);
                                            PatchJumpQuad(jump_quad, jump_quad->label + 2);

                                            Emit(ASSIGN_t, temp, new BoolConstant(false), nullptr);

                                            $$ = temp;
                                        //} 
                                        DLOG("expr -> expr + expr");
                                    }
            | expr LEQL expr        {
                                        // auto entry1 = $1;
                                        // auto entry2 = $3;
                                        // if(entry1 == nullptr)
                                        //     SignalError("Cannot access " + entry1->get_id() + ", previously defined in line: " + std::to_string(entry1->get_line()));
                                        // else if (!entry1->is_active())
                                        //     SignalError("Cannot access " + entry1->get_id() + ", previously defined in line: " + std::to_string(entry1->get_line()));
                                        // else if(entry2 == nullptr)
                                        //     SignalError("Cannot access " + entry2->get_id() + ", previously defined in line: " + std::to_string(entry2->get_line()));
                                        // else if (!entry2->is_active())
                                        //     SignalError("Cannot access " + entry2->get_id() + ", previously defined in line: " + std::to_string(entry2->get_line()));
                                        // else if (!IsVariable(entry1) || (!IsVariable(entry2))
                                        //     SignalError("Use of <= with non variable type");
                                        // else if(entry1->get_type()!=CONTST_NUM || entry2->get_type()!=CONST_NUM)
                                        //     LogWarning("Entries must be type of Number");
                                        // else{
                                            auto less_equal_quad = Emit(IF_LESSEQ_t, $1, $3, nullptr);
                                            PatchBranchQuad(less_equal_quad, less_equal_quad->label + 2);

                                            auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr);
                                            PatchJumpQuad(jump_quad, jump_quad->label + 3);

                                            auto temp = NewTemp();

                                            Emit(ASSIGN_t, temp, new BoolConstant(true), nullptr);

                                            jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr);
                                            PatchJumpQuad(jump_quad, jump_quad->label + 2);

                                            Emit(ASSIGN_t, temp, new BoolConstant(false), nullptr);

                                            $$ = temp;
                                        //} 
                                        DLOG("expr -> expr <= expr");
                                    }
            | expr EQUAL expr       {
                                        // auto entry1 = $1;
                                        // auto entry2 = $3;
                                        // if(entry1 == nullptr)
                                        //     SignalError("Cannot access " + entry1->get_id() + ", previously defined in line: " + std::to_string(entry1->get_line()));
                                        // else if (!entry1->is_active())
                                        //     SignalError("Cannot access " + entry1->get_id() + ", previously defined in line: " + std::to_string(entry1->get_line()));
                                        // else if(entry2 == nullptr)
                                        //     SignalError("Cannot access " + entry2->get_id() + ", previously defined in line: " + std::to_string(entry2->get_line()));
                                        // else if (!entry2->is_active())
                                        //     SignalError("Cannot access " + entry2->get_id() + ", previously defined in line: " + std::to_string(entry2->get_line()));
                                        // else if (!IsVariable(entry1) || (!IsVariable(entry2))
                                        //     SignalError("Use of == with non variable type");
                                        // else if(entry1->get_type()!=CONTST_NUM || entry2->get_type()!=CONST_NUM)
                                        //     LogWarning("Entries must be type of Number");
                                        // else{
                                            auto equal_quad = Emit(IF_EQ_t, $1, $3, nullptr);
                                            PatchBranchQuad(equal_quad, equal_quad->label + 2);

                                            auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr);
                                            PatchJumpQuad(jump_quad, jump_quad->label + 3);

                                            auto temp = NewTemp();

                                            Emit(ASSIGN_t, temp, new BoolConstant(true), nullptr);

                                            jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr);
                                            PatchJumpQuad(jump_quad, jump_quad->label + 2);

                                            Emit(ASSIGN_t, temp, new BoolConstant(false), nullptr);

                                            $$ = temp;
                                        //}
                                        DLOG("expr -> expr == expr");
                                    }
            | expr NOTEQUAL expr    {
                                        // auto entry1 = $1;
                                        // auto entry2 = $3;
                                        // if(entry1 == nullptr)
                                        //     SignalError("Cannot access " + entry1->get_id() + ", previously defined in line: " + std::to_string(entry1->get_line()));
                                        // else if (!entry1->is_active())
                                        //     SignalError("Cannot access " + entry1->get_id() + ", previously defined in line: " + std::to_string(entry1->get_line()));
                                        // else if(entry2 == nullptr)
                                        //     SignalError("Cannot access " + entry2->get_id() + ", previously defined in line: " + std::to_string(entry2->get_line()));
                                        // else if (!entry2->is_active())
                                        //     SignalError("Cannot access " + entry2->get_id() + ", previously defined in line: " + std::to_string(entry2->get_line()));
                                        // else if (!IsVariable(entry1) || (!IsVariable(entry2))
                                        //     SignalError("Use of != with non variable type");
                                        // else if(entry1->get_type()!=CONTST_NUM || entry2->get_type()!=CONST_NUM)
                                        //     LogWarning("Entries must be type of Number");
                                        // else{
                                            auto not_equal_quad = Emit(IF_NOTEQ_t, $1, $3, nullptr);
                                            PatchBranchQuad(not_equal_quad, not_equal_quad->label + 2);

                                            auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr);
                                            PatchJumpQuad(jump_quad, jump_quad->label + 3);

                                            auto temp = NewTemp();

                                            Emit(ASSIGN_t, temp, new BoolConstant(true), nullptr);

                                            jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr);
                                            PatchJumpQuad(jump_quad, jump_quad->label + 2);

                                            Emit(ASSIGN_t, temp, new BoolConstant(false), nullptr);

                                            $$ = temp;
                                       // } 
                                        DLOG("expr -> expr != expr");
                                    }
            | expr AND expr         {
                                        // auto entry1 = $1;
                                        // auto entry2 = $3;
                                        // if(entry1 == nullptr)
                                        //     SignalError("Cannot access " + entry1->get_id() + ", previously defined in line: " + std::to_string(entry1->get_line()));
                                        // else if (!entry1->is_active())
                                        //     SignalError("Cannot access " + entry1->get_id() + ", previously defined in line: " + std::to_string(entry1->get_line()));
                                        // else if(entry2 == nullptr)
                                        //     SignalError("Cannot access " + entry2->get_id() + ", previously defined in line: " + std::to_string(entry2->get_line()));
                                        // else if (!entry2->is_active())
                                        //     SignalError("Cannot access " + entry2->get_id() + ", previously defined in line: " + std::to_string(entry2->get_line()));
                                        // else if (!IsVariable(entry1) || (!IsVariable(entry2))
                                        //     SignalError("Use of AND with non variable type");
                                        // else if(entry1->get_type()!=CONTST_NUM || entry2->get_type()!=CONST_NUM)
                                        //     LogWarning("Entries must be type of Number");
                                        // else{
                                        //     //  TODO
                                        // }
                                        DLOG("expr -> expr and expr");
                                    }
            | expr OR expr          {
                                        // auto entry1 = $1;
                                        // auto entry2 = $3;
                                        // if(entry1 == nullptr)
                                        //     SignalError("Cannot access " + entry1->get_id() + ", previously defined in line: " + std::to_string(entry1->get_line()));
                                        // else if (!entry1->is_active())
                                        //     SignalError("Cannot access " + entry1->get_id() + ", previously defined in line: " + std::to_string(entry1->get_line()));
                                        // else if(entry2 == nullptr)
                                        //     SignalError("Cannot access " + entry2->get_id() + ", previously defined in line: " + std::to_string(entry2->get_line()));
                                        // else if (!entry2->is_active())
                                        //     SignalError("Cannot access " + entry2->get_id() + ", previously defined in line: " + std::to_string(entry2->get_line()));
                                        // else if (!IsVariable(entry1) || (!IsVariable(entry2))
                                        //     SignalError("Use of OR with non variable type");
                                        // else if(entry1->get_type()!=CONTST_NUM || entry2->get_type()!=CONST_NUM)
                                        //     LogWarning("Entries must be type of Number");
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
                                            SignalError("Illegal use of unary minus on constant boolean");
                                        }
                                        else {
                                            auto temp = NewTemp();
                                            Emit(UMINUS_t, temp, symbol, nullptr);
                                            $$ = symbol;
                                        }

                                        DLOG("term -> -expr");
                                    }
            | NOT expr              {
                                        auto equal_quad = Emit(IF_EQ_t, $2, new BoolConstant(true),  nullptr);
                                        PatchBranchQuad(equal_quad, equal_quad->label + 4);

                                        auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr);
                                        PatchJumpQuad(jump_quad, jump_quad->label + 1);

                                        auto temp = NewTemp();

                                        Emit(ASSIGN_t, temp, new BoolConstant(true), nullptr);

                                        jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr);
                                        PatchJumpQuad(jump_quad, jump_quad->label + 2);

                                        Emit(ASSIGN_t, temp, new BoolConstant(false), nullptr);

                                        $$ = temp;
                                        DLOG("term -> not expr");
                                    }
            | PLUSPLUS lvalue       {
                                        auto symbol = $2;
                                        if (!symbol->is_active())
                                            SignalError("Cannot access " + symbol->get_id() + ", previously defined in line: " + std::to_string(symbol->get_line()));    
                                        else if (!IsVariable(symbol))
                                            SignalError("Use of increment operator with non variable type");   
                                        else {
                                            auto temp = NewTemp(); 
                                               
                                            Emit(ADD_t, symbol, symbol, new IntConstant(1));
                                            Emit(ASSIGN_t, temp, symbol, nullptr);
                                            
                                            $$ = temp;
                                        }     

                                        DLOG("term -> ++lvalue"); 
                                    }
            | lvalue PLUSPLUS       {
                                        auto symbol = $1;
                                        if (!symbol->is_active())
                                            SignalError("Cannot access " + symbol->get_id() + ", previously defined in line: " + std::to_string(symbol->get_line()));    
                                        else if (!IsVariable(symbol))
                                            SignalError("Use of increment operator with non variable type");   
                                        else {
                                            auto temp = NewTemp(); 

                                            Emit(ASSIGN_t, temp, symbol, nullptr);    
                                            Emit(ADD_t, symbol, symbol, new IntConstant(1));

                                            $$ = temp;
                                        }     
                                        DLOG("term -> lvalue++"); }
            | MINUSMINUS lvalue     { 
                                        auto symbol = $2;
                                        if (!symbol->is_active())
                                            SignalError("Cannot access " + symbol->get_id() + ", previously defined in line: " + std::to_string(symbol->get_line()));    
                                        else if (!IsVariable(symbol))
                                            SignalError("Use of decrement operator with non variable type");   
                                        else {
                                            auto temp = NewTemp(); 
                                               
                                            Emit(SUB_t, symbol, symbol, new IntConstant(1));
                                            Emit(ASSIGN_t, temp, symbol, nullptr); 

                                            $$ = temp;
                                        }     
                                        DLOG("term -> --lvaule");
                                    }
            | lvalue MINUSMINUS     { 
                                        auto symbol = $1;
                                        if (!symbol->is_active())
                                            SignalError("Cannot access " + symbol->get_id() + ", previously defined in line: " + std::to_string(symbol->get_line()));    
                                        else if (!IsVariable(symbol))
                                            SignalError("Use of decrement operator with non variable type");
                                        else {
                                            {
                                            auto temp = NewTemp(); 
                                               
                                            Emit(ASSIGN_t, temp, symbol, nullptr);    
                                            Emit(SUB_t, symbol, symbol, new IntConstant(1));

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
                                                SignalError("Functions are constant their value cannot be changed");
                                            }
                                            else {
                                                auto assign_quad = Emit(ASSIGN_t, symbol, nullptr, $3);
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
                                        auto symbol = program_stack.Lookup($1);
                                        if (symbol == nullptr) {
                                            symbol = DefineNewSymbol(VAR, $1);
                                        } else if (!symbol->is_active()) {
                                            SignalError("Cannot access " + symbol->get_id() + ", previously defined in line: " + std::to_string(symbol->get_line()));
                                        }
                                        $$ = symbol;

                                        DLOG("lvalue -> id");
                                    }
            | LOCAL ID              {
                                        auto symbol = program_stack.Lookup($2);
                                        if (symbol == nullptr) { 
                                            symbol = DefineNewSymbol(VAR, $2);
                                        }
                                        else if (!symbol->is_active()) {
                                            SignalError("Cannot access " + symbol->get_id() + ", previously defined in line: " + std::to_string(symbol->get_line()));
                                        }
                                        else if (IsGlobalVar(symbol)) {
                                            symbol = DefineNewSymbol(VAR, $2);
                                        }
                                        else if (IsUserFunction(symbol)){
                                            if(IsAtCurrentScope(symbol)) 
                                                SignalError("Attempting to redefine a previously declared user function");
                                            else 
                                                symbol = DefineNewSymbol(VAR, $2);
                                        }
                                        else if (IsLibraryFunction(symbol)) {
                                            SignalError("Attempting to redefine a library function");
                                        }
                                        $$ = symbol;
                                        DLOG("lvalue -> local id");
                                    }
            | COLONCOLON ID         {
                                        auto symbol = program_stack.LookupGlobal($2);
                                        if (symbol == nullptr || !symbol->is_active()) 
                                            SignalError("No global variable with id: " + std::string($2));

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
                                                auto call = new Call(called_symbol, std::list<Expression*>());
                                                
                                                call_exprs.push(call);

                                                $<call>$ = call;
                                            }
            callsuffix                      {
                                                auto top_call = call_exprs.top();
                                                auto called_symbol = top_call->get_called_symbol();

                                                $<call>$ = top_call;

                                                auto temp_value = NewTemp();
                        
                                                Emit(CALL_t, called_symbol, nullptr, nullptr);    
                                                Emit(GETRETVAL_t, temp_value, nullptr, nullptr);

                                                top_call->set_ret_val(temp_value->get_id());

                                                if (IsLibraryFunction(called_symbol) || IsUserFunction(called_symbol)) {
                                                    auto args_num = called_symbol->get_formal_arguments().size();
                                                    auto call_args_num = top_call->get_params().size();

                                                    if (call_args_num < args_num)
                                                        SignalError("Too few arguments passed to function: " + called_symbol->get_id() + ", defined in line: " + std::to_string(called_symbol->get_line()));
                                                    else if (call_args_num > args_num)
                                                        LogWarning("Too many arguments passed to function: " + called_symbol->get_id() + ", defined in line: " + std::to_string(called_symbol->get_line()));
                                                }

                                                call_exprs.pop();

                                                DLOG("call -> lvalue callsuffix");
                                            }
            | '(' funcdef ')'               {                                                
                                                auto called_symbol = $2;
                                                auto call = new Call(called_symbol, std::list<Expression*>());

                                                call_exprs.push(call);

                                                $<call>$ = call;
                                            }
            '(' elist ')'                   {
                                                auto top_call = call_exprs.top();
                                                auto called_symbol = top_call->get_called_symbol();

                                                $<call>$ = top_call;

                                                auto temp_value = NewTemp();

                                                Emit(CALL_t, called_symbol, nullptr, nullptr);
                                                Emit(GETRETVAL_t, temp_value, nullptr, nullptr);

                                                top_call->set_ret_val(temp_value->get_id());

                                                if (IsLibraryFunction(called_symbol) || IsUserFunction(called_symbol)) {
                                                    auto args_num = called_symbol->get_formal_arguments().size();
                                                    auto call_args_num = top_call->get_params().size();

                                                    if (call_args_num < args_num)
                                                        SignalError("Too few arguments passed to function: " + called_symbol->get_id() + ", defined in line: " + std::to_string(called_symbol->get_line()));
                                                    else if (call_args_num > args_num)
                                                        LogWarning("Too many arguments passed to function: " + called_symbol->get_id() + ", defined in line: " + std::to_string(called_symbol->get_line()));
                                                }

                                                call_exprs.pop();

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
                                    if (InCall()) {
                                        auto top_call = call_exprs.top();
                                        top_call->IncludeParameter($2);
                                        Emit(PARAM_t, $2, nullptr, nullptr);
                                    } 
                                    if (InTableMakeElems()) {
                                        auto top_tablemake_elems_expr = tablemake_elems_exprs.top();
                                        top_tablemake_elems_expr->AddElement($2);
                                    }

                                    DLOG("multelist -> ,expr multelist");
                                }
            |                   {
                                    DLOG("multelist -> EMPTY");
                                }
            ;

elist:      expr multelist  {
                                if (InCall()) {
                                    auto top_call = call_exprs.top();
                                    top_call->IncludeParameter($1);
                                    Emit(PARAM_t, $1, nullptr, nullptr);
                                }
                                if (InTableMakeElems()) {
                                    auto top_tablemake_elems_expr = tablemake_elems_exprs.top();
                                    top_tablemake_elems_expr->AddElement($1);
                                }
                                             
                                DLOG("elist -> expr multelist");
                            }
            |               {
                                DLOG("elist -> EMPTY");
                            }
            ;

objectdef:  '['                 {
                                    auto tablemake_elems_expr = new TableMakeElems();
                                    tablemake_elems_exprs.push(tablemake_elems_expr);
                                }
             elist ']'          {
                                    auto temp = NewTemp();
                                    Emit(TABLECREATE_t, temp, nullptr, nullptr);

                                    auto top_tablemake_elems_expr = tablemake_elems_exprs.top();

                                    top_tablemake_elems_expr->set_table(temp);

                                    unsigned int elem_cnt = 0;
                                    for (auto element : top_tablemake_elems_expr->get_elements())
                                        Emit(TABLESETELEM_t, temp, new IntConstant(elem_cnt++), element);

                                    tablemake_elems_exprs.pop();  

                                    $$ = top_tablemake_elems_expr; 

                                    DLOG("objectdef -> [elist]");
                                }
            | '['               {
                                    auto tablemake_pairs_expr = new TableMakePairs();
                                    tablemake_pairs_exprs.push(tablemake_pairs_expr);
                                }
            indexed ']'         { 
                                    auto temp = NewTemp();
                                    Emit(TABLECREATE_t, temp, nullptr, nullptr);

                                    auto top_tablemake_pairs_expr = tablemake_pairs_exprs.top();

                                    top_tablemake_pairs_expr->set_table(temp);

                                    for (auto pair : top_tablemake_pairs_expr->get_pairs())
                                        Emit(TABLESETELEM_t, temp, pair.first, pair.second);

                                    tablemake_pairs_exprs.pop();

                                    $$ = top_tablemake_pairs_expr;
                                        
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
                                        auto top_tablemake_pairs_expr = tablemake_pairs_exprs.top();
                                        top_tablemake_pairs_expr->AddPair($2, $4);

                                        DLOG("indexedelem -> { expr : expr }"); 
                                    }
            ;

block:      '{'         {
                            IncreaseScope();
                            DefineStashedFormalArguments();
                        }
            stmts '}'   {
                            DecreaseScope();
                            DLOG("block -> { stmts }");
                        }
            ;

funcdef:    FUNCTION        {
                                auto anonymous_function = DefineNewAnonymousFunc();
                                auto func_def_stmt = new FuncDefStmt(anonymous_function);
                                func_def_stmts.push(func_def_stmt);

                                auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr);
                                Emit(FUNCSTART_t, anonymous_function, nullptr, nullptr);

                                func_def_stmt->set_func_start_jump_quad(jump_quad);

                                $<sym>$ = anonymous_function;

                            }
            '(' idlist ')'  {
                                HideLowerScopes();
                            }
            block           {
                                auto top_func_def = func_def_stmts.top();
                                auto anonymous_function = top_func_def->get_sym();
                                
                                auto func_end_quad = Emit(FUNCEND_t, anonymous_function, nullptr, nullptr);

                                top_func_def->PatchFuncStartJumpQuad(func_end_quad->label + 1);
                                top_func_def->PatchReturnJumpQuads(func_end_quad->label);

                                program_stack.ActivateLowerScopes();

                                func_def_stmts.pop();

                                $<sym>$ = anonymous_function;
                                DLOG("funcdef -> function (idlist) block "); 
                            }
            | FUNCTION ID   {
                                auto symbol = program_stack.Lookup($2);
                                if (symbol == nullptr) {
                                    symbol = DefineNewSymbol(USER_FUNC, $2);
                                }
                                else if (!symbol->is_active()) {
                                    SignalError("Cannot access " + symbol->get_id() + ", previously defined in line: " + std::to_string(symbol->get_line()));
                                }
                                else {
                                    if (IsVariable(symbol)) {
                                        SignalError(std::string($2) + " variable, previously defined in line: " + std::to_string(symbol->get_line()) + ", cannot be redefined as a function");
                                    }
                                    else if (IsLibraryFunction(symbol)) {
                                        SignalError(std::string($2) + " library function cannot be shadowed by a user function");
                                    }
                                    else if (IsAtCurrentScope(symbol)) {
                                        SignalError("Name collision with function " + std::string($2) + ", previously defined in line: " + std::to_string(symbol->get_line()));
                                    }
                                    else{
                                        symbol = DefineNewSymbol(USER_FUNC, $2);
                                    }
                                }
                                auto func_def_stmt = new FuncDefStmt(symbol); 
                                func_def_stmts.push(func_def_stmt);

                                auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr);
                                Emit(FUNCSTART_t, symbol, nullptr, nullptr);

                                func_def_stmt->set_func_start_jump_quad(jump_quad);

                                $<sym>$ = symbol;
                            }
            '(' idlist ')'  {
                                HideLowerScopes();
                            }
            block           { 
                                auto top_func_def =  func_def_stmts.top();
                                auto function = top_func_def->get_sym();
                                
                                auto func_end_quad = Emit(FUNCEND_t, function, nullptr, nullptr);
                                
                                top_func_def->PatchFuncStartJumpQuad(func_end_quad->label + 1);
                                top_func_def->PatchReturnJumpQuads(func_end_quad->label);

                                func_def_stmts.pop();
                                
                                program_stack.ActivateLowerScopes();

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
                            $$ = new StringConstant(std::string($1));
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
                        auto top_func_def_stmt = func_def_stmts.top();
                        auto func = top_func_def_stmt->get_sym();
                        auto offset = func->get_formal_arguments().size();

                        auto new_formal_arg = new Symbol(VAR, $2, yylineno, current_scope + 1, FORMAL_ARG, offset);

                        if (func->HasFormalArg(new_formal_arg)) {
                            SignalError("formal argument " + std::string($2) + " already declared");
                        }
                        else {
                            func->AddFormalArg(new_formal_arg);
                            StashFormalArgument(new_formal_arg);       
                        }
                    } 
            multid  {
                        DLOG("multid -> , id multid");
                    }
            |       {
                        DLOG("multid -> EMPTY");
                    }
            ;

idlist:     ID      {
                        auto top_func_def_stmt = func_def_stmts.top();
                        auto func = top_func_def_stmt->get_sym();
                        auto offset = func->get_formal_arguments().size();

                        auto new_formal_arg = new Symbol(VAR, $1, yylineno, current_scope + 1, FORMAL_ARG, offset);

                        if (func->HasFormalArg(new_formal_arg)) {
                            SignalError("formal argument " + std::string($1) + " already declared");
                        }
                        else {
                            func->AddFormalArg(new_formal_arg);   
                            StashFormalArgument(new_formal_arg);  
                        }
                    } 
            multid  { 
                        DLOG("idlist -> id multid"); 
                    }
            |       {
                        DLOG("idlist -> EMPTY");
                    }
            ;

ifstmt:     IF '(' expr ')'                 {
                                                auto if_stmt = new IfStmt();
                                                if_stmts.push(if_stmt);

                                                auto branch_quad = Emit(IF_EQ_t, $3, new BoolConstant(true), nullptr);
                                                auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr); 

                                                PatchBranchQuad(branch_quad, branch_quad->label + 2);
                                                if_stmt->set_if_jump_quad(jump_quad);
                                            }
            stmt                            {
                                                ResetTemp();
                                            }
            elsestmt                        {
                                                DLOG("ifstmt -> if (expr) stmt elsestmt"); 
                                            }
            ;

elsestmt:   ELSE            {
                                auto top_if_stmt = if_stmts.top();

                                auto else_jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr);
                                top_if_stmt->PushElseJumpQuad(else_jump_quad);

                                top_if_stmt->PatchIfJumpQuad(NextQuadLabel());
                            }
            stmt            {
                                auto top_if_stmt = if_stmts.top();

                                top_if_stmt->PatchElseJumpQuad(NextQuadLabel());
                                top_if_stmt->PopElseJumpQuad();

                                if_stmts.pop();

                                ResetTemp();

                                DLOG("elsestmt -> else stmt"); 
                            }
            |               {
                                auto top_if_stmt = if_stmts.top();

                                top_if_stmt->PatchIfJumpQuad( NextQuadLabel());

                                if_stmts.pop();
                                DLOG("elsestmt -> EMPTY");
                            }
            ;

whilestmt:  WHILE               { 
                                    auto first_quad_label = NextQuadLabel();
                                    auto while_stmt = new WhileStmt(first_quad_label);

                                    while_stmts.push(while_stmt);
                                    loop_stmts.push(while_stmt);
                                }
            '(' expr ')'        {
                                    auto top_while_stmt = while_stmts.top();

                                    auto branch_quad = Emit(IF_EQ_t, $4, new BoolConstant(true), nullptr);
                                    auto exit_quad = Emit(JUMP_t, nullptr, nullptr, nullptr);

                                    top_while_stmt->PushLoopQuad(branch_quad);
                                    top_while_stmt->PushLoopQuad(exit_quad);
                                }
            stmt                { 
                                    auto top_while_stmt = while_stmts.top();

                                    auto loop_quad = Emit(JUMP_t, nullptr, nullptr, nullptr);

                                    top_while_stmt->PushLoopQuad(loop_quad);
                                    
                                    top_while_stmt->PatchLoopQuads();
                                    top_while_stmt->PatchBreakJumpQuads(NextQuadLabel());
                                    top_while_stmt->PatchContinueJumpQuads();
                                       
                                    while_stmts.pop();
                                    loop_stmts.pop();

                                    ResetTemp();

                                    DLOG ("whilestmt -> WHILE (expr) stmt"); 
                                }
            ;

forstmt:    FOR                                     {
                                                        auto first_quad_label = NextQuadLabel();
                                                        auto for_stmt = new ForStmt(first_quad_label);

                                                        for_stmts.push(for_stmt);
                                                        loop_stmts.push(for_stmt);
                                                    }                   
            '(' elist ';'                           {
                                                        auto top_for_stmt = for_stmts.top();

                                                        auto logical_expr_first_quad_label = NextQuadLabel();
                                                        top_for_stmt->set_logical_expr_first_quad_label(logical_expr_first_quad_label);
                                                    }
            expr ';'                                {
                                                        auto top_for_stmt = for_stmts.top();

                                                        auto branch_quad = Emit(IF_EQ_t, $7, new BoolConstant(true), nullptr);
                                                        top_for_stmt->PushLoopQuad(branch_quad);

                                                        auto exit_quad = Emit(JUMP_t, nullptr, nullptr, nullptr);
                                                        top_for_stmt->PushLoopQuad(exit_quad);

                                                        auto exprs_first_quad_label = NextQuadLabel();
                                                        top_for_stmt->set_exprs_first_quad_label(exprs_first_quad_label);
                                                    }
            elist ')'                               {
                                                        auto top_for_stmt = for_stmts.top();

                                                        auto loop_quad = Emit(JUMP_t, nullptr, nullptr, nullptr);
                                                        top_for_stmt->PushLoopQuad(loop_quad);
                                                    }
            stmt                                    {
                                                        auto top_for_stmt = for_stmts.top();

                                                        auto expr_jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr); 
                                                        top_for_stmt->PushLoopQuad(expr_jump_quad);

                                                        top_for_stmt->PatchLoopQuads();
                                                        top_for_stmt->PatchBreakJumpQuads(NextQuadLabel());
                                                        top_for_stmt->PatchContinueJumpQuads();
 
                                                        for_stmts.pop();
                                                        loop_stmts.pop();    

                                                        ResetTemp();

                                                        DLOG("forstmt -> FOR ( elist ; expr ; elist ) stmt"); 
                                                    }
            ;

returnstmt: RETURN      {
                            if (!InFuncDef()) {
                                SignalError("Invalid return, used outside a function block");
                            } else {
                                auto top_func_def = func_def_stmts.top();

                                Emit(RET_t, nullptr, nullptr, nullptr);
                                auto return_jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr);

                                top_func_def->PushReturnJumpQuad(return_jump_quad);
                            }
                        } 
            ';'         {
                            DLOG("returnstmt -> RETURN;"); 
                        }
            | RETURN    
            expr ';'    {
                            if (!InFuncDef()) 
                                SignalError("Invalid return, used outside a function block");
                            else {
                                auto top_func_def = func_def_stmts.top();

                                Emit(RET_t, $2, nullptr, nullptr);
                                auto return_jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr);

                                top_func_def->PushReturnJumpQuad(return_jump_quad);

                                DLOG("returnstmt -> RETURN expr;");
                            }    
                        }
            ;

%%

int yyerror(std::string yaccProvidedMessage) {
    SignalError(yaccProvidedMessage);
    return 1;
}
 
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
            LogQuads(std::cout);
        }
    #endif 
            
    #if defined LOGSYMTABLE
        if (NoErrorSignaled()) 
            LogSymTable(std::cout);
    #endif
    
    #if defined LOGQUADSTXT
        if (NoErrorSignaled()) {
            const char *path="../quads.txt";
            std::ofstream quad_file(path);
            LogQuads(quad_file);
            quad_file.close();
        }
    #endif  

    return 0;
}

bool error_flag = false;

inline bool NoErrorSignaled() {
    return error_flag == false;
}  

void SignalError(std::string msg) {
    #if !defined TEST
        std::cout << "\033[31mError, in line: " << yylineno << ":\033[0m " << msg << std::endl;
    #else
        std::cout << "Error, in line: " << yylineno << ": " << msg << std::endl; 
    #endif    

    error_flag = 1;
}

void LogWarning(std::string msg) {
    #if !defined TEST
        std::cout << "\033[33mWarning, in line: " << yylineno << ":\033[0m " << msg << std::endl;
    #else
        std::cout << "Warning, in line: " << yylineno << ": " << msg << std::endl ;
    #endif
}

inline void LogSymTable(std::ostream& output) {
    output << symbol_table;
}

void LogQuads(std::ostream& output) {
    for (auto quad : quads) {
        output << *quad << std::endl;
    }
}

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

void DefineSymbol(Symbol* symbol) {
    assert(symbol != nullptr);
    program_stack.Top()->Insert(symbol);
}

void InitLibraryFunctions() {
    IncreaseScope(); 

    DefineSymbol(new Symbol(LIB_FUNC, "print", LIB_FUNC_LINE, global_scope, PROGRAM_VAR, program_var_offset++));
    DefineSymbol(new Symbol(LIB_FUNC, "input", LIB_FUNC_LINE, global_scope, PROGRAM_VAR, program_var_offset++));
    DefineSymbol(new Symbol(LIB_FUNC, "objectmemberkeys", LIB_FUNC_LINE, global_scope, PROGRAM_VAR, program_var_offset++));
    DefineSymbol(new Symbol(LIB_FUNC, "objecttotalmembers", LIB_FUNC_LINE, global_scope, PROGRAM_VAR, program_var_offset++));
    DefineSymbol(new Symbol(LIB_FUNC, "objectcopy", LIB_FUNC_LINE, global_scope, PROGRAM_VAR, program_var_offset++));
    DefineSymbol(new Symbol(LIB_FUNC, "totalarguments", LIB_FUNC_LINE, global_scope, PROGRAM_VAR, program_var_offset++));
    DefineSymbol(new Symbol(LIB_FUNC, "argument", LIB_FUNC_LINE, global_scope, PROGRAM_VAR, program_var_offset++));
    DefineSymbol(new Symbol(LIB_FUNC, "typeof", LIB_FUNC_LINE, global_scope, PROGRAM_VAR, program_var_offset++));
    DefineSymbol(new Symbol(LIB_FUNC, "strtonum", LIB_FUNC_LINE, global_scope, PROGRAM_VAR,program_var_offset++));
    DefineSymbol(new Symbol(LIB_FUNC, "sqrt", LIB_FUNC_LINE, global_scope, PROGRAM_VAR, program_var_offset++));
    DefineSymbol(new Symbol(LIB_FUNC, "cos", LIB_FUNC_LINE, global_scope, PROGRAM_VAR, program_var_offset++));
    DefineSymbol(new Symbol(LIB_FUNC, "sin", LIB_FUNC_LINE, global_scope, PROGRAM_VAR, program_var_offset++));
}

Symbol* NewSymbol(ExprType type, const char* id) {
    assert (id != nullptr);
    if (InFuncDef()) {
        auto new_symbol = new Symbol(type, id, yylineno, current_scope, FUNCTION_LOCAL, func_def_stmts.top()->get_offset());
        func_def_stmts.top()->IncreaseOffset();
        
        return new_symbol;
    }
    else {
        return new Symbol(type, id, yylineno, current_scope, PROGRAM_VAR, program_var_offset++);
    }
}

Symbol* DefineNewSymbol(ExprType type, const char* id) {
    auto new_symbol = NewSymbol(type, id);
    DefineSymbol(new_symbol);

    return new_symbol;
}

unsigned int anonymus_funcs_counter = 0;

std::string NewAnonymousFuncName() {
    std::string an = "$";
    an += std::to_string(++anonymus_funcs_counter);

    return an;
}

Symbol* NewAnonymousFunc() {
    return NewSymbol(USER_FUNC, NewAnonymousFuncName().c_str());
}

Symbol* DefineNewAnonymousFunc() {
    auto new_an_func = NewAnonymousFunc();
    DefineSymbol(new_an_func);

    return new_an_func;
}

std::list<Symbol*>  stashed_formal_arguments;

void DefineStashedFormalArguments() { 
    for (auto i : stashed_formal_arguments) {
        DefineSymbol(i);
    }
    stashed_formal_arguments.clear();
}

void StashFormalArgument(Symbol* symbol) {
    assert(symbol != nullptr);
    stashed_formal_arguments.push_back(symbol);
}

unsigned int temp_counter = 0;

inline std::string NewTempName() {
    return  "^" + std::to_string(temp_counter++); 
}

inline void ResetTemp() { 
    temp_counter = 0; 
}

Symbol* NewTemp() {
    std::string id = NewTempName();

    auto new_temp = program_stack.Top()->Lookup(id);

    if (new_temp == nullptr)  
        new_temp = DefineNewSymbol(VAR, id.c_str());

    return new_temp;
}

Quad* Emit(Iopcode op, Expression* result, Expression* arg1, Expression* arg2) {
    unsigned int label = quads.size() + 1;
    Quad* q = new quad(op, result, arg1, arg2, label, yylineno);
    quads.push_back(q);

    return q;
}

unsigned int NextQuadLabel() {
    if (quads.size() == 0)
        return 1;
    else     
        return quads.back()->label + 1;
}

inline bool IsLibraryFunction(Expression* symbol) {
    return symbol->get_type() == LIB_FUNC; 
}

inline bool IsUserFunction(Expression* symbol) {
    return symbol->get_type() == USER_FUNC; 
}

inline bool IsVariable(Expression* symbol) {
    return symbol->get_type() == VAR;
}

inline bool IsGlobalVar(Symbol* symbol) { 
    return IsVariable(symbol) && symbol->get_scope() == global_scope; 
}

inline bool IsAtCurrentScope(Symbol* symbol) {
    return symbol->get_scope() == current_scope;
}
    
inline bool InLoop() {
    return loop_stmts.size() != 0; 
}

inline bool InFuncDef() { 
    return func_def_stmts.size() != 0; 
} 

inline bool InCall() {
    return call_exprs.size() != 0;
}

inline bool InTableMakeElems() { 
    return tablemake_elems_exprs.size() != 0; 
}
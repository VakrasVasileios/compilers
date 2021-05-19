%{
    #include "../include/parser_deps.h"
    
    int yyerror(std::string yaccProvidedMessage);
    int yylex(void);

    extern int yylineno;
    extern char* yytext;
    extern FILE* yyin;

    bool        error_flag = false;
    inline bool NoErrorSignaled()       { return error_flag == false; }
    inline void SignalError()           { error_flag = 1; }

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
    ProgramStack                program_stack;  

    unsigned int                program_var_offset = 0;
    unsigned int                formal_args_offset = 0;

    std::list<FormalVariable*>  stashed_formal_arguments;
    
    unsigned int                anonymus_funcs_counter = 0;
    unsigned int                temp_counter = 0;

    std::vector<Quad*>          quads;
    
    std::stack<FunctionCall*>   call_exprs;

    std::stack<LoopStmt*>       loop_stmts;
    std::stack<WhileStmt*>      while_stmts;
    std::stack<ForStmt*>        for_stmts;

    std::stack<FuncDefStmt*>    func_def_stmts;  

    std::stack<IfStmt*>         if_stmts;

    std::stack<TableMakeElemsStmt*>  tablemake_elems_stmts;
    std::stack<TableMakePairsStmt*>  tablemake_pairs_stmts;

    void                        IncreaseScope();
    void                        DecreaseScope();
    void                        HideLowerScopes();
    inline bool                 ScopeIsGlobal()                     { return current_scope == global_scope;}

    void                        InitLibraryFunctions(); 
    Symbol*                     InsertLocalVariable(const char* name, unsigned int line);
    Symbol*                     InsertGlobalVariable(const char* name, unsigned int line);
    Symbol*                     InsertUserFunction(const char* name, unsigned int line);
    Symbol*                     InsertUserFunction(unsigned int line);
    void                        PushStashedFormalArguments();
    void                        StashFormalArgument(const char* name, unsigned int line);
    inline bool                 IsLibraryFunction(Symbol* symbol)   { return symbol->get_type() == LIB_FUNC; }
    inline bool                 IsUserFunction(Symbol* symbol)      { return symbol->get_type() == USER_FUNC; }
    inline bool                 IsVariable(Symbol* symbol)          { return symbol->get_type() == VAR; }
    inline bool                 IsGlobalVar(Symbol* symbol)         { return IsVariable(symbol) && symbol->get_scope() == global_scope; }
    inline bool                 IsAtCurrentScope(Symbol* symbol)    { return symbol->get_scope() == current_scope; }

    Symbol*                     NewTemp();
    inline void                 ResetTemp()                         { temp_counter = 0; }

    Quad*                       Emit(Iopcode op, Expression* result, Expression* arg1, Expression* arg2, unsigned int line);
    unsigned int                NextQuadLabel();

    inline bool                 InLoop()                            { return loop_stmts.size() != 0; }
    inline bool                 InFuncDef()                         { return func_def_stmts.size() != 0; } 
    inline bool                 InFunctionCall()                    { return call_exprs.size() != 0; }
    inline bool                 InFuncCall()                        { return call_exprs.size() != 0; }
    inline bool                 InTableMake()                       { return tablemake_elems_stmts.size() != 0; }

    void                        LogSymTable(std::ostream& output);
    void                        LogQuads(std::ostream& output);
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
                                        if(!InLoop()) {
                                            SIGNALERROR("invalid keyword BREAK outside of loop");
                                        } else {
                                            auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);

                                            auto top_loop_stmt = loop_stmts.top();
                                            top_loop_stmt->PushBreakJumpQuad(jump_quad);
                                        }

                                        DLOG("stmt -> break;");
                                    }
            | CONTINUE ';'          {
                                        if(!InLoop()) {
                                            SIGNALERROR("invalid keyword CONTINUE outside of loop");
                                        } else {
                                            auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);

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
                                            PatchBranchQuad(greater_quad, greater_quad->label + 2);

                                            auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);
                                            PatchJumpQuad(jump_quad, jump_quad->label + 3);

                                            auto temp = NewTemp();

                                            Emit(ASSIGN_t, temp, new BoolConstant(true), nullptr, yylineno);

                                            jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);
                                            PatchJumpQuad(jump_quad, jump_quad->label + 2);

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
                                            PatchBranchQuad(greater_equal_quad, greater_equal_quad->label + 2);

                                            auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);
                                            PatchJumpQuad(jump_quad, jump_quad->label + 3);

                                            auto temp = NewTemp();

                                            Emit(ASSIGN_t, temp, new BoolConstant(true), nullptr, yylineno);

                                            jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);
                                            PatchJumpQuad(jump_quad, jump_quad->label + 2);

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
                                            PatchBranchQuad(less_quad, less_quad->label + 2);

                                            auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);
                                            PatchJumpQuad(jump_quad, jump_quad->label + 3);

                                            auto temp = NewTemp();

                                            Emit(ASSIGN_t, temp, new BoolConstant(true), nullptr, yylineno);

                                            jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);
                                            PatchJumpQuad(jump_quad, jump_quad->label + 2);

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
                                            PatchBranchQuad(less_equal_quad, less_equal_quad->label + 2);

                                            auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);
                                            PatchJumpQuad(jump_quad, jump_quad->label + 3);

                                            auto temp = NewTemp();

                                            Emit(ASSIGN_t, temp, new BoolConstant(true), nullptr, yylineno);

                                            jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);
                                            PatchJumpQuad(jump_quad, jump_quad->label + 2);

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
                                            PatchBranchQuad(equal_quad, equal_quad->label + 2);

                                            auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);
                                            PatchJumpQuad(jump_quad, jump_quad->label + 3);

                                            auto temp = NewTemp();

                                            Emit(ASSIGN_t, temp, new BoolConstant(true), nullptr, yylineno);

                                            jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);
                                            PatchJumpQuad(jump_quad, jump_quad->label + 2);

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
                                            PatchBranchQuad(not_equal_quad, not_equal_quad->label + 2);

                                            auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);
                                            PatchJumpQuad(jump_quad, jump_quad->label + 3);

                                            auto temp = NewTemp();

                                            Emit(ASSIGN_t, temp, new BoolConstant(true), nullptr, yylineno);

                                            jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);
                                            PatchJumpQuad(jump_quad, jump_quad->label + 2);

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
                                        PatchBranchQuad(equal_quad, equal_quad->label + 4);

                                        auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);
                                        PatchJumpQuad(jump_quad, jump_quad->label + 1);

                                        auto temp = NewTemp();

                                        Emit(ASSIGN_t, temp, new BoolConstant(true), nullptr, yylineno);

                                        jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);
                                        PatchJumpQuad(jump_quad, jump_quad->label + 2);

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
                                                auto func_call = new FunctionCall(called_symbol, std::list<Expression*>());
                                                
                                                call_exprs.push(func_call);

                                                $<funcCall>$ = func_call;
                                            }
            callsuffix                      {
                                                auto top_func_call = call_exprs.top();
                                                auto called_symbol = top_func_call->get_called_symbol();

                                                $<funcCall>$ = top_func_call;

                                                auto temp_value = NewTemp();
                        
                                                Emit(CALL_t, called_symbol, nullptr, nullptr, yylineno);    
                                                Emit(GETRETVAL_t, temp_value, nullptr, nullptr, yylineno);

                                                top_func_call->set_ret_val(temp_value->get_id());

                                                if (IsLibraryFunction(called_symbol) || IsUserFunction(called_symbol)) {
                                                    auto args_num = static_cast<Function*>(called_symbol)->get_formal_arguments().size();
                                                    auto call_args_num = top_func_call->get_params().size();

                                                    if (call_args_num < args_num)
                                                        SIGNALERROR("Too few arguments passed to function: " << called_symbol->get_id() << ", defined in line: " << std::to_string(called_symbol->get_line()));
                                                    else if (call_args_num > args_num)
                                                        LOGWARNING("Too many arguments passed to function: " << called_symbol->get_id() << ", defined in line: " << std::to_string(called_symbol->get_line()));
                                                }

                                                call_exprs.pop();

                                                DLOG("call -> lvalue callsuffix");
                                            }
            | '(' funcdef ')'               {                                                
                                                auto called_symbol = $2;
                                                auto func_call = new FunctionCall(called_symbol, std::list<Expression*>());

                                                call_exprs.push(func_call);

                                                $<funcCall>$ = func_call;
                                            }
            '(' elist ')'                   {
                                                auto func_call = call_exprs.top();
                                                auto called_symbol = func_call->get_called_symbol();

                                                $<funcCall>$ = func_call;

                                                auto temp_value = NewTemp();

                                                Emit(CALL_t, called_symbol, nullptr, nullptr, yylineno);
                                                Emit(GETRETVAL_t, temp_value, nullptr, nullptr, yylineno);

                                                func_call->set_ret_val(temp_value->get_id());

                                                if (IsLibraryFunction(called_symbol) || IsUserFunction(called_symbol)) {
                                                    auto args_num = static_cast<Function*>(called_symbol)->get_formal_arguments().size();
                                                    auto call_args_num = func_call->get_params().size();

                                                    if (call_args_num < args_num)
                                                        SIGNALERROR("Too few arguments passed to function: " << called_symbol->get_id() << ", defined in line: " << std::to_string(called_symbol->get_line()));
                                                    else if (call_args_num > args_num)
                                                        LOGWARNING("Too many arguments passed to function: " << called_symbol->get_id() << ", defined in line: " << std::to_string(called_symbol->get_line()));
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
                                    if (InFuncCall()) {
                                        auto top_call = call_exprs.top();
                                        top_call->IncludeParameter($2);
                                        Emit(PARAM_t, $2, nullptr, nullptr, yylineno);
                                    } else if (InTableMake()) {
                                        auto top_tablemake_elems_stmt = tablemake_elems_stmts.top();
                                        top_tablemake_elems_stmt->AddElement($2);
                                    }

                                    DLOG("multelist -> ,expr multelist");
                                }
            |                   {
                                    DLOG("multelist -> EMPTY");
                                }
            ;

elist:      expr multelist  {
                                if (InFuncCall()) {
                                    auto top_call = call_exprs.top();
                                    top_call->IncludeParameter($1);
                                    Emit(PARAM_t, $1, nullptr, nullptr, yylineno);
                                } else if (InTableMake()) {
                                    auto top_tablemake_elems_stmt = tablemake_elems_stmts.top();
                                    top_tablemake_elems_stmt->AddElement($1);
                                }
                                             
                                DLOG("elist -> expr multelist");
                            }
            |               {
                                DLOG("elist -> EMPTY");
                            }
            ;

objectdef:  '['                 {
                                    auto tablemake_elems_stmt = new TableMakeElemsStmt();
                                    tablemake_elems_stmts.push(tablemake_elems_stmt);
                                }
             elist ']'          {
                                    auto temp = NewTemp();
                                    Emit(TABLECREATE_t, temp, nullptr, nullptr, yylineno);

                                    auto top_tablemake_elems_stmt = tablemake_elems_stmts.top();

                                    unsigned int elem_cnt = 0;
                                    for (auto element : top_tablemake_elems_stmt->get_elements())
                                        Emit(TABLESETELEM_t, temp, new IntConstant(elem_cnt++), element, yylineno);

                                    tablemake_elems_stmts.pop();    

                                    DLOG("objectdef -> [elist]");
                                }
            | '['               {
                                    auto tablemake_pairs_stmt = new TableMakePairsStmt();
                                    tablemake_pairs_stmts.push(tablemake_pairs_stmt);
                                }
            indexed ']'         { 
                                    auto temp = NewTemp();
                                    Emit(TABLECREATE_t, temp, nullptr, nullptr, yylineno);

                                    auto top_tablemake_pairs_stmt = tablemake_pairs_stmts.top();
                                    for (auto pair : top_tablemake_pairs_stmt->get_pairs())
                                        Emit(TABLESETELEM_t, temp, pair.first, pair.second, yylineno);

                                    tablemake_elems_stmts.pop();
                                        
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
                                        auto top_tablemake_pairs_stmt = tablemake_pairs_stmts.top();
                                        top_tablemake_pairs_stmt->AddPair($2, $4);

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

                                program_stack.ActivateLowerScopes();

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
                                auto func_def_stmt = new FuncDefStmt(symbol); 
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
                                                auto if_stmt = new IfStmt();
                                                if_stmts.push(if_stmt);

                                                auto branch_quad = Emit(IF_EQ_t, $3, new BoolConstant(true), nullptr, yylineno);
                                                auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno); 

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

                                auto else_jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);
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

                                    auto branch_quad = Emit(IF_EQ_t, $4, new BoolConstant(true), nullptr, yylineno);
                                    auto exit_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);

                                    top_while_stmt->PushLoopQuad(branch_quad);
                                    top_while_stmt->PushLoopQuad(exit_quad);
                                }
            stmt                { 
                                    auto top_while_stmt = while_stmts.top();

                                    auto loop_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);

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

                                                        auto branch_quad = Emit(IF_EQ_t, $7, new BoolConstant(true), nullptr, yylineno);
                                                        top_for_stmt->PushLoopQuad(branch_quad);

                                                        auto exit_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);
                                                        top_for_stmt->PushLoopQuad(exit_quad);

                                                        auto exprs_first_quad_label = NextQuadLabel();
                                                        top_for_stmt->set_exprs_first_quad_label(exprs_first_quad_label);
                                                    }
            elist ')'                               {
                                                        auto top_for_stmt = for_stmts.top();

                                                        auto loop_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);
                                                        top_for_stmt->PushLoopQuad(loop_quad);
                                                    }
            stmt                                    {
                                                        auto top_for_stmt = for_stmts.top();

                                                        auto expr_jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno); 
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
                            if (!InFuncDef()) 
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

void LogSymTable(std::ostream& output) {
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
    if (!InFuncDef()) {
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
    if (!InFuncDef()) {
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
    if (!InFuncDef()) {
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
    else 
        SIGNALERROR("formal argument " << name << " already declared, in line: " << line);
}

inline std::string NewTempName() { return  "^" + std::to_string(temp_counter++); }

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

Quad* Emit(Iopcode op, Expression* result, Expression* arg1, Expression* arg2, unsigned int line) {
    unsigned int label = quads.size() + 1;
    Quad* q = new quad(op, result, arg1, arg2, label, line);
    quads.push_back(q);

    return q;
}

unsigned int NextQuadLabel() {
    if (quads.size() == 0)
        return 1;
    else     
        return quads.back()->label + 1;
}
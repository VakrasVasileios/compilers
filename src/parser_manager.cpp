#include <string>
#include <list>
#include <iostream>
#include <assert.h>
#include <stack>

#include "../include/parser_manager.h"


SymbolTable         symbol_table;
ProgramStack        program_stack;  // the top element returns a read/write reference to the most recent block.

/* ---------------------- Scope -------------------------- */

#define OUT_OF_SCOPE  -1

const unsigned int global_scope = 0;
unsigned int current_scope = OUT_OF_SCOPE;

bool ScopeIsGlobal() {
    return current_scope == global_scope;
}

unsigned int GetCurrentScope() {
    return current_scope;
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

void EnableLowerScopes() {
    program_stack.ActivateLowerScopes();
}


/* ---------------------- FuncDef -------------------------- */

std::stack<FunctionDef*> function_definitions;

void PushFuncDef(FunctionDef* func_def) {
    assert (func_def != nullptr);
    function_definitions.push(func_def);
}

FunctionDef* TopFuncDef() {
    if (GetFuncDefDepth() != 0)
        return function_definitions.top();
    return nullptr;    
}

FunctionDef* PopFuncDef() {
    FunctionDef* top;
    if (GetFuncDefDepth() != 0) {
        top = function_definitions.top();
        function_definitions.pop();
    }
    
    return top;
}

unsigned int GetFuncDefDepth() {
    return function_definitions.size();
}


/* ---------------------- Symbol -------------------------- */

#define LIB_FUNC_LINE  0
unsigned int program_var_offset = 0;

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

Symbol* LookupGlobal(const char* name) {
    assert(name != nullptr);
    return program_stack.LookupGlobal(name);
}

Symbol* Lookup(const char* name) {
    assert(name != nullptr);
    return program_stack.Lookup(name);
}

Symbol* InsertLocalVariable(const char* name, unsigned int line) {
    assert(name != nullptr);

    Symbol* symbol;
    if (GetFuncDefDepth() == 0) {
        symbol = new LocalVariable(name, line, current_scope, PROGRAM_VAR, program_var_offset++);
    }
    else {
        symbol = new LocalVariable(name, line, current_scope, FUNCTION_LOCAL, function_definitions.top()->get_offset());  
        function_definitions.top()->IncreaseOffset();
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

std::list<FormalVariable*> stashed_formal_arguments;

Symbol* InsertUserFunction(const char* name, unsigned int line) {
    assert(name != nullptr);

    Symbol* symbol;
    if (GetFuncDefDepth() == 0) {
        symbol = new UserFunction(name, line, current_scope, PROGRAM_VAR, program_var_offset++, stashed_formal_arguments);
    }
    else {
        symbol = new UserFunction(name, line, current_scope, FUNCTION_LOCAL, function_definitions.top()->get_offset(), stashed_formal_arguments);  
        function_definitions.top()->IncreaseOffset();
    }
    program_stack.Top()->Insert(symbol);

    return symbol;
}

unsigned int anonymus_funcs_counter = 0;

Symbol* InsertUserFunction(unsigned int line) {
    std::string an = "$";
    an += std::to_string(++anonymus_funcs_counter);
    Symbol* symbol;
    if (GetFuncDefDepth() == 0) {
        symbol = new UserFunction(an, line, current_scope, PROGRAM_VAR, program_var_offset++, stashed_formal_arguments);
    }
    else {
        symbol = new UserFunction(an, line, current_scope, FUNCTION_LOCAL, function_definitions.top()->get_offset(), stashed_formal_arguments);  
        function_definitions.top()->IncreaseOffset();
    }
    program_stack.Top()->Insert(symbol);
    
    return symbol;
}

unsigned int formal_args_offset = 0;

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

bool IsLibraryFunction(Symbol* symbol) {
    assert(symbol != nullptr);
    return symbol->get_type() == LIB_FUNC;
}

bool IsUserFunction(Symbol* symbol) {
    assert(symbol != nullptr);
    return symbol->get_type() == USER_FUNC;
}

bool IsVariable(Symbol* symbol) {
    assert(symbol != nullptr);
    return symbol->get_type() == VAR;
}

bool IsGlobalVar(Symbol* symbol) {
    assert(symbol != nullptr);
    return IsVariable(symbol) && symbol->get_scope() == global_scope;
}

bool IsAtCurrentScope(Symbol* symbol) {
    assert(symbol != nullptr);
    return symbol->get_scope() == current_scope;
}


/* ---------------------- Conditional -------------------------- */ 

unsigned int if_cnt = 0;

unsigned int GetIfStmtDepth() {
    return if_cnt;
}

void IncreaseIfStmtDepth() {
    if_cnt++;
}

void DecreaseIfStmtDepth() {
    if_cnt--;
}

/* ---------------------- Loop -------------------------- */

std::stack<unsigned int> loop_start_labels;  // provides a stack of loop start labels,
                                                        // that will be used to patch the loops branch quads.

unsigned int GetLoopDepth() {
    return loop_start_labels.size();
}

void PushLoopStartLabel(unsigned int start_label) {
    loop_start_labels.push(start_label);
}

unsigned int TopLoopStartLabel() {
    assert (GetLoopDepth != 0);

    return loop_start_labels.top();
}

unsigned int PopLoopStartLabel() {
    assert (GetLoopDepth != 0);

    unsigned int top = loop_start_labels.top();
    loop_start_labels.pop();

    return top;    
}


/* ---------------------- Quad -------------------------- */

std::vector<Quad*>   quads;

std::map<FunctionDef*, Quad*> jump_quads_by_func_defs; // Maps function definitions with their initial jump quad. 

std::map<FunctionDef*, std::list<Quad*> > jump_quad_lists_by_func_defs; // Maps function definitions with their list of jump quads.

Quad*
Emit(Iopcode op, Expression* result, Expression* arg1, Expression* arg2, unsigned int line) {
    unsigned int label = quads.size() + 1;
    Quad* q = new quad(op, result, arg1, arg2, label, line);
    quads.push_back(q);

    return q;
}

void PatchJumpQuad(Quad* jump_quad, int label) {
    assert (jump_quad != nullptr);
    
    jump_quad->result = new IntConstant(label);
}

void PatchBranchQuad(Quad* branch_quad, int label) {
    assert (branch_quad != nullptr);

    branch_quad->arg2 = new IntConstant(label);
}

void MapJumpQuad(FunctionDef* func_def, Quad* jump_quad) {
    jump_quads_by_func_defs.insert({func_def, jump_quad});
}

void PatchJumpQuad(FunctionDef* func_def, int label) {
    assert (jump_quads_by_func_defs[func_def] != nullptr);

    auto jump_quad = jump_quads_by_func_defs[func_def];
    PatchJumpQuad(jump_quad, label);
}

void PushJumpQuad(FunctionDef* func_def, Quad* jump_quad) {
    jump_quad_lists_by_func_defs[func_def].push_back(jump_quad);
}

void PatchJumpQuadList(FunctionDef* func_def, int label) {
    auto jump_quad_list = jump_quad_lists_by_func_defs[func_def];
    for (auto jump_quad : jump_quad_list) {
        PatchJumpQuad(jump_quad, label);
    }
}

unsigned int GetBackQuadLabel() {
    if (quads.size() == 0)
        return 0;
    else     
        return quads.back()->label;
}

std::map<unsigned int, std::list<Quad*>> loop_branch_quads_by_start_label;  // maps a loop's first quad start label with the 
                                                                            // standard loop branch quads.

std::map<unsigned int, std::list<Quad*>> break_jump_quads_by_start_label;   // maps a loop's first quad start label with the 
                                                                            // loops body break statements.

std::map<unsigned int, std::list<Quad*>> continue_jump_quads_by_start_label;    // maps a loop's first quad start label with the 
                                                                                // loops body continue statements.                                                                          

void PushLoopBranchQuad(unsigned int start_label, Quad* branch_quad) {
    loop_branch_quads_by_start_label[start_label].push_back(branch_quad);
}

void PatchWhileLoopBranchQuads(unsigned int start_label) {
    auto loop_branch_quads = loop_branch_quads_by_start_label[start_label];

    auto loop_quad = loop_branch_quads.back();
    loop_branch_quads.pop_back();

    auto exit_quad = loop_branch_quads.back();
    loop_branch_quads.pop_back();

    auto branch_quad = loop_branch_quads.back();
    loop_branch_quads.pop_back();

    PatchJumpQuad(loop_quad, start_label);
    PatchJumpQuad(exit_quad, loop_quad->label + 1);
    PatchBranchQuad(branch_quad, branch_quad->label + 2);
}

std::map<unsigned int, unsigned int> logical_expr_start_label_by_start_label;   // maps the label of the first quad of a for statement
                                                                                // with the logical expression with which the statement is evaluated.

std::map<unsigned int, unsigned int> exprs_start_label_by_start_label;    // maps the label of the first quad of a for statement
                                                                                    // with the first quad of the list of expressions be- 
                                                                                    // -fore the main body of the statement.

void MapLogicalExpressionStartLabel(unsigned int start_label, unsigned int logical_expr_start_label) {
    logical_expr_start_label_by_start_label.insert({start_label, logical_expr_start_label});
}

void MapExpressionsStartLabel(unsigned int start_label, unsigned int exprs_start_label) {
    exprs_start_label_by_start_label.insert({start_label, exprs_start_label});
}

void PatchForLoopBranchQuads(unsigned int start_label) {
    auto loop_branch_quads = loop_branch_quads_by_start_label[start_label];

    auto expr_jump_quad = loop_branch_quads.back();
    loop_branch_quads.pop_back();

    auto loop_quad = loop_branch_quads.back();
    loop_branch_quads.pop_back();

    auto exit_quad = loop_branch_quads.back();
    loop_branch_quads.pop_back();

    auto branch_quad = loop_branch_quads.back();
    loop_branch_quads.pop_back(); 

    auto exprs_start_label = exprs_start_label_by_start_label[start_label]; // e.g the "i++" expression in the "for (i; i < 2; i++)" stmt
                                                                            // has an first quad with a label. Its value is stored in this label.
    PatchJumpQuad(expr_jump_quad, exprs_start_label);

    auto logical_expr_start_label = logical_expr_start_label_by_start_label[start_label];   // e.g the "i<2" expression in the "for (i; i < 2; i++)" stmt
                                                                                            // has a first quad with a label. Its value is stored in
                                                                                            //this label.
    PatchJumpQuad(loop_quad, logical_expr_start_label);
    PatchJumpQuad(exit_quad, expr_jump_quad->label+1);
    PatchBranchQuad(branch_quad, loop_quad->label + 1);
}

void PushLoopBreakJumpQuad(unsigned int start_label, Quad* break_jump_quad) {
    break_jump_quads_by_start_label[start_label].push_back(break_jump_quad);
}

void PatchLoopBreakJumpQuads(unsigned int start_label, unsigned int patch_label) {
    auto loop_break_jump_quads = break_jump_quads_by_start_label[start_label];

    for (auto loop_break_jump_quad : loop_break_jump_quads)
        PatchJumpQuad(loop_break_jump_quad, patch_label);
}

void PushLoopContinueJumpQuad(unsigned int start_label, Quad* continue_jump_quad) {
    continue_jump_quads_by_start_label[start_label].push_back(continue_jump_quad);
}

void PatchWhileLoopContinueJumpQuads(unsigned int start_label) {
    auto loop_continue_jump_quads = continue_jump_quads_by_start_label[start_label];

    auto jump_label = TopLoopStartLabel();
    for (auto loop_continue_jump_quad : loop_continue_jump_quads)
        PatchJumpQuad(loop_continue_jump_quad, jump_label);
}

void PatchForLoopContinueJumpQuads(unsigned int start_label) {
    auto loop_continue_jump_quads = continue_jump_quads_by_start_label[start_label];

    auto exprs_start_label = exprs_start_label_by_start_label[start_label];
    for (auto loop_continue_jump_quad : loop_continue_jump_quads)
        PatchJumpQuad(loop_continue_jump_quad, exprs_start_label);
}

std::map<unsigned int, Quad*> jump_quad_by_if_stmt; // Maps the depth of an if statement with its exit jump quad.

void MapIfStmtJumpQuad(unsigned int if_stmt_depth, Quad* exit_quad) {
    jump_quad_by_if_stmt.insert({if_stmt_depth, exit_quad});
}

void PatchIfStmtJumpQuad(unsigned int if_stmt_depth, unsigned int patch_label) {
    auto branch_quad = jump_quad_by_if_stmt[if_stmt_depth];
    PatchBranchQuad(branch_quad, patch_label);
}

std::map<unsigned int, std::list<Quad*>> else_jump_quads_by_if_stmt; // Maps the depth of an if statement with its else stmts jump quads.

void PushElseJumpQuad(unsigned int if_stmt_depth, Quad* else_jump_quad) {
    else_jump_quads_by_if_stmt[if_stmt_depth].push_back(else_jump_quad);
}

void PatchElseJumpQuad(unsigned int if_stmt_depth) {
    auto else_jump_quads = else_jump_quads_by_if_stmt[if_stmt_depth];
    auto else_jump_quad = else_jump_quads.back();
    else_jump_quads.pop_back();
    PatchJumpQuad(else_jump_quad, GetBackQuadLabel() + 1);
}


/* ---------------------- Temp -------------------------- */

unsigned int temp_counter = 0;

#define TEMP_LINE 0

std::string NewTempName() {
    return  "^" + std::to_string(temp_counter);
}

void IncreaseTemp() {
    temp_counter++;
}

Symbol* NewTemp() {
    std::string name = NewTempName();
    Symbol* sym = program_stack.LookupHiddenVariable(name);

    IncreaseTemp();

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


/* ---------------------- Call -------------------------- */

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


/* ---------------------- Error -------------------------- */

bool error_flag = false;

bool NoErrorSignaled() {
    return !error_flag;
}

void SignalError() {
    error_flag = true;
}


/* ---------------------- Logs -------------------------- */

void LogSymbolTable(std::ostream& output) {
    output << symbol_table;
}

void LogQuads(std::ostream& output) {
    for (auto quad : quads) {
        output << *quad << std::endl;
    }
}























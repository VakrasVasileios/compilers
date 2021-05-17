#ifndef PARSERMANAGER_H
#define PARSERMANAGER_H

#include <fstream>
#include <string>
#include <list>
#include <map>
#include <iostream>
#include <assert.h>
#include <stack>
#include "symbol_table.h"
#include "expression/library_function.h"
#include "expression/user_function.h"
#include "expression/local_variable.h"
#include "expression/global_variable.h"
#include "expression/formal_variable.h"
#include "expression/symbol.h"
#include "expression/function_def.h"
#include "expression/function_call.h"
#include "expression/bool_constant.h"
#include "expression/constant.h"
#include "expression/nil_constant.h"
#include "expression/string_constant.h"
#include "expression/double_constant.h"
#include "expression/int_constant.h"
#include "expression/numeric_constant.h"
#include "instruction_opcodes.h"
#include "quad.h"
#include "program_stack.h"



/* ---------------------- Scope -------------------------- */

/*
Returns the current scope.
*/
unsigned int        GetCurrentScope();
/*
Increases the current scope.
*/
void                IncreaseScope();
/*
Decreases the current scope.
*/
void                DecreaseScope();
/*
Returns a read/write access to wether the current scope is the global scope.
*/
bool                ScopeIsGlobal();
/*
Hides all of the symbol table entries at lower scopes, except the global scope.
*/
void                HideLowerScopes();
/*
Shows all of the symbol table entries at lower scopes, except the global scope.
*/
void                EnableLowerScopes();


/* ---------------------- FuncDef -------------------------- */

/*
Pushes a function definition to the function definitions stack.
It's a checked runtime error for the function definition to be null.
*/
void                PushFuncDef(FunctionDef* func_def);
/*
Returns a read/write access to the top function definition from the
function definitions stack.
*/
FunctionDef*        TopFuncDef();
/*
Pops the top function definition from the function definitions stack.
Returns a read/write reference to the top function definition.
*/
FunctionDef*        PopFuncDef();
/*
Returns the depth of the current function definition.
Returns greater than 1 for nested definitions and
zero if no current function is being defined currently.
*/
unsigned int        GetFuncDefDepth();


/* ---------------------- Symbol -------------------------- */

/*
Inserts the library functions to the symbol table.
*/
void                InitLibraryFunctions();
/*
Searches for a symbol table entry reference by its name across all scopes. 
Returns a read/write access to a reference to the entry if it's found, else nullptr.
It's a checked runtime error for the name to be null.
*/
Symbol*             Lookup(const char* name);
/*
Searches for a symbol table entry reference by its name at the global_scope. 
Returns a read/write access to a reference to the entry if it's found, else nullptr.
It's a checked runtime error for the name to be null.
*/
Symbol*             LookupGlobal(const char* name);
/*
Inserts a local variable to the symbol table.
Returns a reference to the entry inserted.
It's a checked runtime error for the name to be null.
*/
Symbol*             InsertLocalVariable(const char* name, unsigned int line);
/*
Inserts a global variable to the symbol table.
Returns a reference to the entry inserted.
It's a checked runtime error for the name to be null.
*/
Symbol*             InsertGlobalVariable(const char* name, unsigned int line);
/*
Inserts a user function, with a name, at a line, with the stashed formal arguments, to the symbol table.
Returns a reference to the entry inserted.
It's a checked runtime error for the name to be null.
*/
Symbol*             InsertUserFunction(const char* name, unsigned int line);
/*
Inserts a user function, at a line, with the stashed formal arguments, to the symbol table.
Returns a reference to the entry inserted.
*/
Symbol*             InsertUserFunction(unsigned int line);
/*
Inserts all of the stashed formal arguments to the symbol table.
*/
void                PushStashedFormalArguments();
/*
Stashes a formal argument, with a name, at a line, in order for it to be pushed to the symbol table.
It's a checked runtime error for the name to be null.
*/
void                StashFormalArgument(const char* name, unsigned int line);
/*
Checks if a symbol table entry is declared at the current scope.
It's a checked runtime error for the entry to be null.
*/
bool                IsAtCurrentScope(Symbol* entry);
/*
Returns wether a symbol table entry is a global variable.
*/
bool                IsGlobalVar(Symbol* entry);
/*
Returns a read/write access to wether a symbol table entry is a library function.
It's a checked runtime error for the entry to be null.
*/
bool                IsLibraryFunction(Symbol* entry);
/*
Returns a read/write access to wether a symbol table entry is a user function.
It's a checked runtime error for the entry to be null.
*/
bool                IsUserFunction(Symbol* entry);
/*
Returns a read/write access to wether a symbol table entry is a variable.
It's a checked runtime error for the entry to be null.
*/
bool                IsVariable(Symbol* entry);


/* ---------------------- Conditional -------------------------- */ 

/*
Returns a read/write access to the counter of the current if statement. 
*/
unsigned int        GetIfStmt();
/*
Increases the counter of the current if statement.
*/
void                IncreaseIfStmt();
/*
Decreases the counter of the current if statement.
*/
void                DecreaseIfStmt();


/* ---------------------- Loop -------------------------- */

/*
Returns a read access to the loop depth.
*/
unsigned int        GetLoopDepth();
/*
Pushes the label of the first quad of a loop statement to the stack of loop statements.
*/
void                PushLoopStartLabel(unsigned int start_label);
/*
Returns a read/write access to the label of the first quad of the top loop statement from the stack of loop statements.
*/
unsigned int        TopLoopStartLabel();
/*
Pops a read/write access to the label of the first quad of the top loop statement from the stack of loop statements.
*/
unsigned int        PopLoopStartLabel();

/* ---------------------- Quad -------------------------- */

/*
Constructs a new quad and appends it to the quads list. 
Returns a read/write reference to the new quad.
*/
Quad*               Emit(Iopcode op, Expression* result, Expression* arg1, Expression* arg2, unsigned int line);
/*
Patches a jump quad's result with a label.
It's a checked runtime error for the quad to be null.
*/
void                PatchJumpQuad(Quad* jump_quad, int label);
/*
Patches a branch quad's arg2 with a label.
It's a checked runtime error for the quad to be null.
*/
void                PatchBranchQuad(Quad* branch_quad, int label);
/*
Maps a function definition with its initial jump quad.
*/
void                MapJumpQuad(FunctionDef* func_def, Quad* jump_quad);
/*
Patches a function definition's previously inserted jump quad label.
It's a runtime error for the function definition to not be mapped with a jump quad.
*/
void                PatchFuncDefJumpQuad(FunctionDef* func_def, int label);
/*
Pushes a jump quad to the mapped list of a function definition.
*/
void                PushFuncDefJumpQuad(FunctionDef* func_def, Quad* jump_quad);
/*
Patches the mapped jump quad list from return statements of a function definition
with a label.
*/
void                PatchFuncDefJumpQuadList(FunctionDef* func_def, int label);
/*
Returns a read/write access to the label of the last quad from the emitted quads list.
In case that no quads have been previously emitted, zero is returned.
*/
unsigned int        GetBackQuadLabel();
/*
Pushes a standard loop branch quad to the mapped list of a loop's first quad start label.
*/
void                PushLoopBranchQuad(unsigned int start_label, Quad* branch_quad);
/*
Patches the mapped while loop branch quad list with a loop's first quad start label
with the start label.
Standard while loop branch quads consist of a 1: branch quad, which evaluates a statement and jumps to a
2: jump quad, that jumps to the start of the loop, and a 3: jump quad that exits the loop in case
the branch quad is evaluated as false.
*/
void                PatchWhileLoopBranchQuads(unsigned int start_label);
/*
Maps the label of the first quad of a loop statement with the the first quad of the logical expression with which it's evaluated.
*/
void                MapLogicalExpressionStartLabel(unsigned int start_label, unsigned int logical_expr_start_label);
/*
Maps the label of the first quad of a loop statement with the the first quad of the expressions before its body.
*/
void                MapExpressionsStartLabel(unsigned int start_label, unsigned int exprs_start_label);
/*
Patches the mapped for loop branch quad list with a loop's first quad start label
with the start label and the mapped expressions first quads.
Standard for loop branch quads consist of a 1: branch quad, which evaluates a statement and jumps to a
2: jump quad, that jumps to the start of the logical evaluation, a 3: jump quad that exits the loop in case
the branch quad is evaluated as false, and a 4: jump quad that jumps to the start quad of the expressions before the
body of the for statement. 
*/
void                PatchForLoopBranchQuads(unsigned int start_label);
/*
Pushes a jump quad from a break statement to the mapped list of a loop's first quad start label.
*/
void                PushLoopBreakJumpQuad(unsigned int start_label, Quad* break_jump_quad);
/*
Pushes a jump quad from a continue statement to the mapped list of a loop's first quad start label.
*/
void                PushLoopContinueJumpQuad(unsigned int start_label, Quad* continue_jump_quad);
/*
Patches the mapped, to a loop's first quad start label, jump quads from break statements.
*/
void                PatchLoopBreakJumpQuads(unsigned int start_label, unsigned int patch_label);
/*
Patches the mapped, to a while loop's first quad start label, jump quads from continue statements.
*/
void                PatchWhileLoopContinueJumpQuads(unsigned int start_label);
/*
Patches the mapped, to a for loop's first quad start label, jump quads from continue statements.
*/
void                PatchForLoopContinueJumpQuads(unsigned int start_label);
/*
Maps the exit jump quad of an if statement with the if statement.
*/
void                MapIfStmtJumpQuad(unsigned int if_stmt, Quad* exit_quad);
/*
Patches the mapped, with the if statement, exit jump quad of the if statement.
Unmaps the exit jump quad with the if statement.
*/
void                PatchIfStmtJumpQuad(unsigned int if_stmt, unsigned int patch_label);
/*
Pushes a jump quad from a else statement to the mapped list of an if statement.
*/
void                PushElseJumpQuad(unsigned int if_stmt, Quad* else_jump_quad);
/*
Patches the most recent mapped, to an if statement, else jump quad.
*/
void                PatchElseJumpQuad(unsigned int if_stmt);


/* ---------------------- Temp -------------------------- */

/*
Returns either a new hidden variable, or an available hidden_variable ,
at the current scope.
Increments the hidden variable counter.
*/
Symbol*             NewTemp();
/*
Resets the hidden variable counter.
*/
void                ResetTemp();


/* ---------------------- Call -------------------------- */

/*
Creates a new call stack frame of a function call.
*/
void                PushCallStackFrame(FunctionCall* function_call);
/*
Returns the function call at the top call stack frame.
*/
FunctionCall*       PopCallStackFrame();
/*
Pushes an expression parameter, to the top call stack frame,
called function, parameters list.
It's a checked runtime error for the expression to be null.
*/
void                PushCallParam(Expression* expr);
/*
Returns the depth of the current function call.
*/
unsigned int        GetCallDepth();


/* ---------------------- Error -------------------------- */
/*
Returns wether a compilation error has previously been signaled.
*/
bool                NoErrorSignaled();
/*
Signals that a compination error has occured.
*/
void                SignalError();


/* ---------------------- Log -------------------------- */

/*
Logs the symbol table contents.
*/
void                LogSymbolTable(std::ostream& output);
/*
Logs all the emitted quads.
*/
void                LogQuads(std::ostream& output);


#endif
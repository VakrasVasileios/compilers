#ifndef PARSERMANAGER_H
#define PARSERMANAGER_H

#include "../symbol_table/symbol_table.h"
#include "../program_stack/program_stack.h"
#include "../expression/expression.h"
#include "../expression/constant/constant.h"
#include "../expression/constant/bool_constant.h"
#include "../expression/constant/nil_constant.h"
#include "../expression/constant/string_constant.h"
#include "../expression/constant/numeric_constant/double_constant.h"
#include "../expression/constant/numeric_constant/int_constant.h"
#include "../expression/symbol/function/library_function.h"
#include "../expression/symbol/function/user_function.h"
#include "../expression/symbol/variable/local_variable.h"
#include "../expression/symbol/variable/global_variable.h"
#include "../expression/symbol/variable/formal_variable.h"
#include "../expression/symbol/variable/hidden_variable.h"
#include "../expression/symbol/symbol.h"
#include "../quad/quad.h"
#include <fstream>
#include <string>

/*
Gets a read/write access to the validity flag of a possible return.
*/
bool                IsValidReturn(void);
/*
Returns a read/write access to wether the current scope is the global scope.
*/
bool                ScopeIsGlobal();
/*
Checks if a symbol table entry is declared at the current scope.
It's a checked runtime error for the entry to be null.
*/
bool                IsAtCurrentScope(Symbol* entry);
/*
Returns wether the state is a method call.
*/
bool                IsMethodCall();
/*
Returns wether the state is a function call.
*/
bool                IsFunctionCall();
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
/*
Returns wether a compilation error has previously been signaled.
*/
bool                NoErrorSignaled();
/*
Returns a read/write access to the return depth.
*/
unsigned int        GetReturnDepth();
/*
Returns a read/write access to the loop depth.
*/
unsigned int        GetLoopDepth();
/*
Returns the current scope.
*/
unsigned int        GetCurrentScope();
/*
Pops this stack frame, returning the counter that is responsible for counting
the number of passed arguments to the top function call.
*/
unsigned int        PopCallStackFrame();
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
Searches for a function entry reference by its name across all scopes. 
Returns a read/write access to a reference to the entry if it's found, else nullptr.
It's a checked runtime error for the name to be null.
*/
Function*           LookupFunc(const char* name);
/*
Searches the previously inserted function referenceacross all scopes. 
Returns a read/write access to a reference to the function if it's found, else nullptr.
*/
Function*           LookupPreviousFunction();
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
Returns either a new hidden variable, or an available hidden_variable ,
at the current scope.
*/
HiddenVariable*     NewTemp();
/*
Inserts the library functions to the symbol table.
*/
void                InitLibraryFunctions();
/*
Increases the current scope.
*/
void                IncreaseScope();
/*
Decreases the current scope.
*/
void                DecreaseScope();
/*
Flags the state as a method call.
*/
void                SetMethodCall(bool methodcall);
/*
Flags the state as a function call.
*/
void                SetFunctionCall(bool function_call);
/*
Flags the validity of a possible return.
*/
void                SetValidReturn(bool valid_return);
/*
Increases the loop depth.
*/
void                IncreaseLoopDepth();
/*
Decreases the loop depth.
*/
void                DecreaseLoopDepth();
/*
Increases the return depth.
*/
void                IncreaseReturnDepth();
/*
Decreases the return depth.
*/
void                DecreaseReturnDepth();
/*
Hides all of the symbol table entries at lower scopes, except the global scope.
*/
void                HideLowerScopes();
/*
Shows all of the symbol table entries at lower scopes, except the global scope.
*/
void                EnableLowerScopes();
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
Logs the symbol table contents.
*/
void                LogSymbolTable(std::ostream& output);
/*
Logs all the emitted quads.
*/
void                LogQuads(std::ostream& output);
/*
Constructs a new quad and appends it to the quads list. 
*/
void                Emit(Iopcode op, Expression* result, Expression* arg1, Expression* arg2, unsigned int line);
/*
Increases the hidden variable counter.
*/
void                IncreaseTemp();
/*
Resets the hidden variable counter.
*/
void                ResetTemp();
/*
Signals that a compination error has occured.
*/
void                SignalError();
/*
Increments the counter that is responsible for counting
the number of passed arguments to the top function call, at this stack frame.
*/
void                IncreaseCallArgsCount();
/*
Creates a new call stack frame.
*/
void                NewCallStackFrame();

#endif
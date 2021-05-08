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
bool                IsMethodCall(void);
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

void                LogQuads(std::ostream& output);
/*
Searches for a symbol table entry reference by its name across all scopes. 
Returns a read/write access to a reference to the entry if it's found, else nullptr.
It's a checked runtime error for the name to be null.
*/
Symbol*   Lookup(const char* name);
/*
Searches for a symbol table entry reference by its name at the global_scope. 
Returns a read/write access to a reference to the entry if it's found, else nullptr.
It's a checked runtime error for the name to be null.
*/
Symbol*   LookupGlobal(const char* name);
/*
Searches for a table entry reference by its name across all scopes. 
Returns a read/write access to a reference to the entry if it's found, else nullptr.
It's a checked runtime error for the name to be null.
*/
Symbol*   LookupFunc(const char* name);

void                Emit(Iopcode op, Expression* result, Expression* arg1, Expression* arg2, unsigned int line);
/*
Inserts a local variable to the symbol table.
Returns a reference to the entry inserted.
It's a checked runtime error for the name to be null.
*/
Symbol*   InsertLocalVariable(const char* name, unsigned int line);
/*
Inserts a global variable to the symbol table.
Returns a reference to the entry inserted.
It's a checked runtime error for the name to be null.
*/
Symbol*   InsertGlobalVariable(const char* name, unsigned int line);
/*
Inserts a user function, with a name, at a line, with the stashed formal arguments, to the symbol table.
Returns a reference to the entry inserted.
It's a checked runtime error for the name to be null.
*/
Symbol*   InsertUserFunction(const char* name, unsigned int line);
/*
Inserts a user function, at a line, with the stashed formal arguments, to the symbol table.
Returns a reference to the entry inserted.
*/
Symbol*   InsertUserFunction(unsigned int line);


/*
Returns the previously inserted, to the symbol table,
function.
*/
Function* LookupPreviousFunction();


#endif
#ifndef PARSERMANAGER_H
#define PARSERMANAGER_H

#include "../symbol_table/symbol_table_entry/function_entry/library_function_entry.h"
#include "../symbol_table/symbol_table_entry/function_entry/user_function_entry.h"
#include "../symbol_table/symbol_table_entry/variable_entry/local_variable_entry.h"
#include "../symbol_table/symbol_table_entry/variable_entry/global_variable_entry.h"
#include "../symbol_table/symbol_table_entry/variable_entry/formal_variable_entry.h"
#include "../symbol_table/symbol_table_entry/symbol_table_entry.h"
#include "../symbol_table/symbol_table.h"
#include "../program_stack/program_stack.h"
#include "../expression/expression.h"
#include "../expression/constant.h"
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
bool                IsAtCurrentScope(SymbolTableEntry* entry);
/*
Returns wether the state is a method call.
*/
bool                IsMethodCall(void);
/*
Returns wether a symbol table entry is a global variable.
*/
bool                IsGlobalVar(SymbolTableEntry* entry);
/*
Returns a read/write access to wether a symbol table entry is a library function.
It's a checked runtime error for the entry to be null.
*/
bool                IsLibraryFunction(SymbolTableEntry* entry);
/*
Returns a read/write access to wether a symbol table entry is a user function.
It's a checked runtime error for the entry to be null.
*/
bool                IsUserFunction(SymbolTableEntry* entry);
/*
Returns a read/write access to wether a symbol table entry is a variable.
It's a checked runtime error for the entry to be null.
*/
bool                IsVariable(SymbolTableEntry* entry);
/*
Returns a read/write access to the loop depth.
*/
unsigned int        GetLoopDepth();
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
Searches for a symbol table entry reference by its name across all scopes. 
Returns a read/write access to a reference to the entry if it's found, else nullptr.
It's a checked runtime error for the name to be null.
*/
SymbolTableEntry*   Lookup(const char* name);
/*
Searches for a symbol table entry reference by its name at the global_scope. 
Returns a read/write access to a reference to the entry if it's found, else nullptr.
It's a checked runtime error for the name to be null.
*/
SymbolTableEntry*   LookupGlobal(const char* name);
/*
Searches for a table entry reference by its name across all scopes. 
Returns a read/write access to a reference to the entry if it's found, else nullptr.
It's a checked runtime error for the name to be null.
*/
SymbolTableEntry*   LookupFunc(const char* name);
/*
Inserts a local variable to the symbol table.
Returns a reference to the entry inserted.
It's a checked runtime error for the name to be null.
*/
SymbolTableEntry*   InsertLocalVariable(const char* name, unsigned int line);
/*
Inserts a global variable to the symbol table.
Returns a reference to the entry inserted.
It's a checked runtime error for the name to be null.
*/
SymbolTableEntry*   InsertGlobalVariable(const char* name, unsigned int line);
/*
Inserts a user function, with a name, at a line, with the stashed formal arguments, to the symbol table.
Returns a reference to the entry inserted.
It's a checked runtime error for the name to be null.
*/
SymbolTableEntry*   InsertUserFunction(const char* name, unsigned int line);
/*
Inserts a user function, at a line, with the stashed formal arguments, to the symbol table.
Returns a reference to the entry inserted.
*/
SymbolTableEntry*   InsertUserFunction(unsigned int line);


#endif
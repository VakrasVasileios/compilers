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
Returns the current scope.
*/
unsigned int        GetCurrentScope();
/*
Flags the state as a method call.
*/
void                SetMethodCall(bool methodcall);
/*
Returns wether the state is a method call.
*/
bool                IsMethodCall(void);
/*
Flags the validity of a possible return.
*/
void                SetValidReturn(bool valid_return);
/*
Gets a read/write access to the validity flag of a possible return.
*/
bool                IsValidReturn(void);
/*
Increases the loop depth.
*/
void                IncreaseLoopDepth();
/*
Decreases the loop depth.
*/
void                DecreaseLoopDepth();
/*
Returns a read/write access to the loop depth.
*/
unsigned int        GetLoopDepth();
/*
Hides all of the symbol table entries at lower scopes, except the global scope.
*/
void                HideLowerScopes();
/*
Shows all of the symbol table entries at lower scopes, except the global scope.
*/
void                EnableLowerScopes();
/*
Returns a read/write access to wether the current scope is the global scope.
*/
bool                ScopeIsGlobal();
/*
Searches for an acitve symbol table entry by its name across all scopes. 
Returns a read/write access to a reference to the entry if it's found and it's active, else nullptr.
*/
SymbolTableEntry*   Lookup(const char* name);
/*
Searches for an active symbol table entry by its name at the global_scope. 
Returns a read/write access to a reference to the entry if it's found and it's active, else nullptr.
*/
SymbolTableEntry*   LookupGlobal(const char* name);
/*
Searches for an active symbol table entry by its name across all scopes at the function entry stack. 
Returns a read/write access to a reference to the entry if it's found and it's active, else nullptr.
*/
SymbolTableEntry*   LookupFunc(const char* name);
/*
Returns a read/write access to wether a symbol table entry is a library function.
*/
bool                IsLibraryFunction(SymbolTableEntry* entry);
/*
Returns a read/write access to wether a symbol table entry is a user function.
*/
bool                IsUserFunction(SymbolTableEntry* entry);

bool                IsVariable(SymbolTableEntry* entry);
/*
Returns a read/write access to wether a symbol table entry is a formal variable.
Returns false if entry is null.
*/
// bool                IsFormalVariable(SymbolTableEntry* entry); 
/*
Returns a read/write access to wether a symbol table entry is a global variable.
Returns false if entry is null.
*/  
// bool                IsGlobalVariable(SymbolTableEntry* entry); 
/*
Returns a read/write access to wether a symbol table entry is local variable.
Returns false if entry is null.
*/  
// bool                IsLocalVariable(SymbolTableEntry* entry);
/*
Inserts a local variable to the symbol table.
Returns a reference to the expression inserted.
*/
SymbolTableEntry*         InsertLocalVariable(const char* name, unsigned int line);
/*
Inserts a global variable to the symbol table.
Returns a reference to the expression inserted.
*/
SymbolTableEntry*         InsertGlobalVariable(const char* name, unsigned int line);
/*
Inserts a user function, with a name, at a line, with the stashed formal arguments, to the symbol table.
Returns a reference to the expression inserted.
*/
SymbolTableEntry*         InsertUserFunction(const char* name, unsigned int line);
/*
Inserts a user function, at a line, with the stashed formal arguments, to the symbol table.
Returns a reference to the expression inserted.
*/
SymbolTableEntry*         InsertUserFunction(unsigned int line);
/*
Inserts all of the stashed formal arguments to the symbol table.
*/
void                PushStashedFormalArguments();
/*
Stashes a formal argument, with a name, at a line, in order for it to be pushed to the symbol table.
*/
void                StashFormalArgument(const char* name, unsigned int line);
/*
Logs the symbol table contents.
*/
void                LogSymbolTable(std::ostream& output);

#endif
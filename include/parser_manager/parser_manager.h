#ifndef PARSERMANAGER_H
#define PARSERMANAGER_H

#include "../symbol_table/symbol_table_entry/function_entry/library_function_entry.h"
#include "../symbol_table/symbol_table_entry/function_entry/user_function_entry.h"
#include "../symbol_table/symbol_table_entry/variable_entry/local_variable_entry.h"
#include "../symbol_table/symbol_table_entry/variable_entry/global_variable_entry.h"
#include "../symbol_table/symbol_table_entry/variable_entry/formal_variable_entry.h"
#include "../../include/symbol_table/symbol_table.h"
#include "../../include/symbol_table/symbol_table_entry/symbol_table_entry.h"
#include "../../include/program_stack/program_stack.h"
#include <fstream>

/*
Inserts the library functions to the symbol table.
*/
void                init_library_functions();
/*
Increases the current scope.
*/
void                increase_scope();
/*
Decreases the current scope.
*/
void                decrease_scope();
/*
Flags the state as a method call.
*/
void                set_method_call(bool methodcall);
/*
Returns wether the state is a method call.
*/
bool                is_method_call(void);
/*
Flags the validity of a possible return.
*/
void                set_valid_return(bool _validReturn);
/*
Gets the validity flag of a possible return.
*/
bool                is_valid_return(void);
/*
Increases the loop depth.
*/
void                increase_loop_depth();
/*
Decreases the loop depth.
*/
void                decrease_loop_depth();
/*
Returns the loop depth.
*/
unsigned int        get_loop_depth();

/*
Hides all of the symbol table entries at lower scopes, except the global scope.
*/
void                hide_lower_scopes();
/*
Shows all of the symbol table entries at lower scopes, except the global scope.
*/
void                enable_lower_scopes();
/*
Returns wether the current scope is the global scope.
*/
bool                scope_is_global();
/*
Searches for an acitve symbol table entry by its name across all scopes. 
Returns the entry if it's found and it's active, else nullptr.
*/
SymbolTableEntry*   lookup(const char* name);
/*
Searches for an active symbol table entry by its name at the global_scope. 
Returns the entry if it's found and it's active, else nullptr.
*/
SymbolTableEntry*   LookupGlobal(const char* name);
/*
Returns wether a symbol table entry is a library function.
Returns false if entry is null.
*/
bool                is_library_function(SymbolTableEntry* entry);
/*
Returns wether a symbol table entry is a user function.
Returns false if entry is null.
*/
bool                is_user_function(SymbolTableEntry* entry);
/*
Returns wether a symbol table entry is a formal variable.
Returns false if entry is null.
*/
bool                is_formal_variable(SymbolTableEntry* entry); 
/*
Returns wether a symbol table entry is a global variable.
Returns false if entry is null.
*/  
bool                is_global_variable(SymbolTableEntry* entry); 
/*
Returns wether a symbol table entry is local variable.
Returns false if entry is null.
*/  
bool                is_local_variable(SymbolTableEntry* entry);
/*
Inserts a local variable to the symbol table.
*/
void                insert_variable_local(const char* name, unsigned int line);
/*
Inserts a global variable to the symbol table.
*/
void                insert_variable_global(const char* name, unsigned int line);
/*
Inserts a user function, with a name, at a line, with the stashed formal arguments, to the symbol table.
*/
void                insert_user_function(const char* name, unsigned int line);
/*
Inserts a user function, at a line, with the stashed formal arguments, to the symbol table.
*/
void                insert_user_function(unsigned int line);
/*
Inserts all of the stashed formal arguments to the symbol table.
*/
void                push_stashed_formal_arguments();
/*
Stashes a formal argument, with a name, at a line, in order for it to be pushed to the symbol table.
*/
void                stash_formal_argument(const char* name, unsigned int line);
/*
Logs the symbol table contents.
*/
void                log_symbol_table(std::ostream& output);

#endif
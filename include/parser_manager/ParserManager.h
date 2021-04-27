#include "../symbol_table/symbol_table_entry/function_entry/LibraryFunctionEntry.h"
#include "../symbol_table/symbol_table_entry/function_entry/UserFunctionEntry.h"
#include "../symbol_table/symbol_table_entry/variable_entry/LocalVariableEntry.h"
#include "../symbol_table/symbol_table_entry/variable_entry/GlobalVariableEntry.h"
#include "../symbol_table/symbol_table_entry/variable_entry/FormalVariableEntry.h"

#ifndef PARSERMANAGER_H
#define PARSERMANAGER_H

/*
Inserts the library functions to the symbol table.
*/
void            insert_library_functions();
/*
Increases the current scope.
*/
void            increase_scope();
/*
Decreases the current scope.
*/
void            decrease_scope();
/*
Flags the state as a method call.
*/
void            set_method_call(bool methodcall);
/*
Returns wether the state is a method call.
*/
bool            is_method_call();
/*
Increases the loop depth.
*/
void            increase_loop_depth();
/*
Decreases the loop depth.
*/
void            decrease_loop_depth();
/*
Returns the loop depth.
*/
unsigned int    get_loop_depth();

/*
Hides all of the symbol table entries at lower scopes.
*/
void            hide_lower_scopes();
/*
Shows all of the symbol table entries at lower scopes.
*/
void            enable_lower_scopes();
/*
Searches for a library function entry, in the symbol table, by name.
Returns true if it's found, else false.
*/
bool            lookup_library_function(const char* name);
/*
Searches for a user function entry, in the symbol table, by name.
Returns true if it's found, else false.
*/
bool            lookup_user_function(const char* name);
/*
Searches for a local variable entry, in the symbol table, by name.
Returns true if it's found, else false.
*/
bool            lookup_local_variable(const char* name);
/*
Searches for a global variable entry, in the symbol table, by name.
Returns true if it's found, else false.
*/
bool            lookup_global_variable(const char* name);
/*
Searches for a formal variable entry, in the symbol table, by name.
Returns true if it's found, else false.
*/
bool            lookup_formal_variable(const char* name);
/*
Inserts a user function, with a name, at a line, with the stashed formal arguments, to the symbol table.
*/
void            insert_user_function(const char* name, unsigned int line);
/*
Inserts a user function, at a line, with the stashed formal arguments, to the symbol table.
*/
void            insert_user_function(unsigned int line);
/*
Inserts all of the stashed lvalues to the symbol table.
*/
void            push_stashed_lvalues();
/*
Stashes a lvalue, with a name, at a line, in order for it to be pushed to the symbol table.
*/
void            stash_lvalue(const char* name, unsigned int line);
/*
Cleans the lvalues stash.
*/
void            reset_lvalues_stash();
/*
Stashes a formal argument, with a name, at a line, in order for it to be pushed to the symbol table.
*/
void            stash_formal_argument(const char* name, unsigned int line);
/*
Logs the symbol table contents.
*/
void            log_symbol_table();

#endif
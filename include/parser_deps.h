#ifndef PARSERDEPS_H
#define PARSERDEPS_H

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <list>
#include <stack>
#include <string>
#include "debuglog.h"
#include "expression/library_function.h"
#include "expression/user_function.h"
#include "expression/local_variable.h"
#include "expression/global_variable.h"
#include "expression/formal_variable.h"
#include "expression/symbol.h"
#include "expression/function_call.h"
#include "expression/bool_constant.h"
#include "expression/constant.h"
#include "expression/nil_constant.h"
#include "expression/string_constant.h"
#include "expression/double_constant.h"
#include "expression/int_constant.h"
#include "expression/numeric_constant.h"
#include "symbol_table.h"
#include "program_stack.h"
#include "instruction_opcodes.h"
#include "quad.h"
#include "for_stmt.h"
#include "while_stmt.h"
#include "loop_stmt.h"
#include "func_def_stmt.h"
#include "if_stmt.h"




#endif
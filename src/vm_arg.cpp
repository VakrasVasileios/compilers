#include "../include/vm_arg.h"
#include "../include/expression/symbol.h"
#include "../include/expression/bool_constant.h"
#include "../include/expression/nil_constant.h"
#include "../include/expression/int_constant.h"
#include "../include/expression/double_constant.h"
#include "../include/expression/string_constant.h"
#include "../include/program_consts.h"
#include <assert.h>
#include <string>

#define SYMBOL_CAST(expr)       static_cast<const Symbol*>(expr)
#define BOOLCONST_CAST(exr)     static_cast<const BoolConstant*>(expr)
#define INTCONST_CAST(exr)      static_cast<const IntConstant*>(expr)
#define DOUBLECONST_CAST(expr)  static_cast<const DoubleConstant*>(expr)
#define NILCONST_CAST(exr)      static_cast<const NilConstant*>(expr)
#define STRINGCONST_CAST(exr)   static_cast<const StringConstant*>(expr)

Vmarg*
make_operand (const Expression* expr) {
    Vmarg* arg = new Vmarg();
    switch (expr->get_type()) {
        case VAR:
        case TABLE_ITEM:
        case ARITHMETIC:
        case BOOL:
        case TABLE_MAKE: {
            arg->value = SYMBOL_CAST(expr)->get_offset();
            switch (SYMBOL_CAST(expr)->get_space()) {
                case PROGRAM_VAR:
                    arg->type = GLOBAL_a;
                    break;
                case FUNCTION_LOCAL:
                    arg->type = LOCAL_a;
                    break;
                case FORMAL_ARG:
                    arg->type = FORMAL_a;
                    break;
                default:
                    assert(false);
            }
        }
        case CONST_BOOL: {
            arg->type = BOOL_a;
            arg->value = BOOLCONST_CAST(expr)->get_value();
            break;
        }
        case CONST_STR: {
            arg->type = STRING_a;
            arg->value = ProgramConsts:: GetInstance().InsertString(STRINGCONST_CAST(expr)->get_value());
            break;
        }
        case CONST_INT: {
            arg->type = NUMBER_a;
            arg->value = ProgramConsts:: GetInstance().InsertNumber(INTCONST_CAST(expr)->get_value());
            break;
        }
        case CONST_DOUBLE: {
            arg->type = NUMBER_a;
            arg->value = ProgramConsts:: GetInstance().InsertNumber(DOUBLECONST_CAST(expr)->get_value());
            break;
        }
        case CONST_NIL:
            arg->type = NIL_a;
            break;
        case LIB_FUNC: {
            arg->type = LIBFUNC_a;
            arg->value = ProgramConsts:: GetInstance().InsertLibFunc(SYMBOL_CAST(expr)->get_id());
            break;
        }
        case USER_FUNC: {
            arg->type = USERFUNC_a; // TODO: and 'ere
            // arg->value = ProgramConsts:: GetInstance()
        }
        default:
            assert(false);
    }

    return arg;

}
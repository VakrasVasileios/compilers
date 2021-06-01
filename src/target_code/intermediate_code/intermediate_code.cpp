#include "../../../include/target_code/intermediate_code/intermediate_code.h"

#define TEMP_LINE 0

namespace intermediate_code {
    void LogWarning(std::string msg, unsigned int line) {
        PRECONDITION(line >= 0);
        #if !defined TEST
            std::cout << "\033[33mWarning, in line: " << line << ":\033[0m " << msg << std::endl;
        #else
            std::cout << "Warning, in line: " << yylineno << ": " << msg << std::endl ;
        #endif
    }

    void LogQuads(std::ostream& output) {
        for (auto quad : quads) {
            output << *quad << std::endl;
        }   
    }

    unsigned int temp_counter = 0;

    inline std::string NewTempName() {
        return  "^" + std::to_string(temp_counter++); 
    }

    expression::Symbol* NewTemp(expression::ExprType type, expression::Expression* index) {
       std::string id = NewTempName();

        auto new_temp = syntax_analysis::Lookup(id.c_str());
        
        if (new_temp == nullptr)  
            new_temp = syntax_analysis::DefineNewSymbol(type, id.c_str(), index, TEMP_LINE);

        return new_temp; 
    }

    inline void ResetTemp() {
        temp_counter = 0;
    }

    Quad* Emit(Iopcode op, expression::Expression* result, expression::Expression* arg1,
    expression::Expression* arg2, unsigned int line) {
        PRECONDITION(line >= 0);
        unsigned int label = quads.size() + 1;
        Quad* q = new quad(op, result, arg1, arg2, label, line);
        quads.push_back(q);

        return q;
    }

    unsigned int NextQuadLabel() {
        if (quads.size() == 0)
            return 1;
        else     
            return quads.back()->label + 1;
    }

    expression::Symbol* EmitIfTableItem(expression::Symbol* sym, unsigned int line) {
        PRECONDITION(sym != nullptr);
        PRECONDITION(line >= 0);
        if (!IsTableItem(sym)) {
            return sym;
        }
        else {
            auto temp = NewTemp(expression::TABLE_ITEM, sym->get_index());
            Emit(TABLEGETELEM_t, temp, sym, sym->get_index(), line);

            return temp;
        }  
    }

    expression::Symbol* MemberItem(expression::Symbol* sym, const char* id, unsigned int line) {
        PRECONDITION(sym != nullptr);
        PRECONDITION(id != nullptr);
        PRECONDITION(line >= 0);
        sym = EmitIfTableItem(sym, line);
        auto index = new expression::StringConstant(std::string(id));

        return syntax_analysis::DefineNewSymbol(expression::TABLE_ITEM, sym->get_id().c_str(), index, line);
    }

    void checkValidCall(expression::Symbol* called_symbol, 
    std::list<expression::Expression*> params, unsigned int line) {
        assert (called_symbol != nullptr);
        if (IsUserFunction(called_symbol)) {
            auto call_args_num = params.size();
            auto func_def_args_num = called_symbol->get_formal_arguments().size();
            if (call_args_num < func_def_args_num) 
                syntax_analysis::SignalError("Too few arguments passed to function: " + called_symbol->get_id() + ", defined in line: " + std::to_string(called_symbol->get_line()), line);
            else if (call_args_num > func_def_args_num) 
                LogWarning("Too many arguments passed to function: " + called_symbol->get_id() + ", defined in line: " + std::to_string(called_symbol->get_line()), line);
        }
    }

    expression::Call* MakeCall(expression::Symbol* called_symbol,
    expression::CallSuffix* call_suffix, unsigned int line) {
        PRECONDITION(called_symbol != nullptr);
        PRECONDITION(call_suffix != nullptr);
        PRECONDITION(line >= 0);
        auto params = call_suffix->get_elist()->exprs;

        checkValidCall(called_symbol, params, line);

        called_symbol = EmitIfTableItem(called_symbol, line);
        for (auto param : params)
            Emit(PARAM_t, param, nullptr, nullptr, line);

        auto return_value = NewTemp(expression::VAR, nullptr);
        Emit(CALL_t, called_symbol, nullptr, nullptr, line);
        Emit(GETRETVAL_t, return_value, nullptr, nullptr, line);

        return new expression::Call(called_symbol, call_suffix, return_value); 
    }

    void BackPatch(std::list<unsigned int> l_list, unsigned int q_label) {
        for (unsigned int i : l_list) {
            if (quads[i-1]->op == JUMP_t) {
                PatchJumpQuad(quads[i-1], q_label);
            }
            else{
                PatchBranchQuad(quads[i-1], q_label);
            }
        }
    }

    expression::Symbol* ConcludeShortCircuit(expression::BoolExpr* expr, unsigned int line) {
        auto temp = NewTemp(expression::VAR, nullptr);

        BackPatch(expr->true_list, NextQuadLabel());
        Emit(ASSIGN_t, temp, new expression::BoolConstant(true), nullptr, line);

        Quad* jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, line);
        PatchJumpQuad(jump_quad, jump_quad->label + 2);

        BackPatch(expr->false_list, NextQuadLabel());
        Emit(ASSIGN_t, temp, new expression::BoolConstant(false), nullptr, line);

        return temp;
    }

    bool IsValidArithmetic(expression::Expression* expr, std::string context, unsigned int line) {
        assert (expr != nullptr);
        if (IsLibraryFunction(expr)) {
            syntax_analysis::SignalError("Invalid use of " + context + " operator on library function " + expr->to_string(), line);
            return false;
        }
        else if (IsUserFunction(expr)) {
            syntax_analysis::SignalError("Invalid use of " + context + " operator on user function " + expr->to_string(), line);
            return false;
        }
        else if (IsConstString(expr)) {
            syntax_analysis::SignalError("Invalid use of " + context + " operator on const string " + expr->to_string(), line);
            return false;
        }
        else if (IsConstBool(expr)) {
            syntax_analysis::SignalError("Invalid use of " + context + " operator on const bool " + expr->to_string(), line);
            return false;
        }
        else if (IsTableMake(expr)) {
            syntax_analysis::SignalError("Invalid use of " + context + " operator on table " + expr->to_string(), line);
            return false;
        }

        return true;                
    }

    bool IsValidArithmeticOp(expression::Expression* expr, unsigned int line) {
        PRECONDITION(expr != nullptr);
        return IsValidArithmetic(expr, std::string("arithmetic"), line);
    }

    bool IsValidArithmeticComp(expression::Expression* expr, unsigned int line) {
        PRECONDITION(expr != nullptr);
        return IsValidArithmetic(expr, std::string("comparison"), line);
    }

    bool IsValidAssign(expression::Symbol* left_operand, unsigned int line) {
        PRECONDITION(left_operand != nullptr);
        if (IsUserFunction(left_operand) || IsLibraryFunction(left_operand)) {
            syntax_analysis::SignalError("Functions are constant their value cannot be changed", line);

            return false;
        }

        return true;  
    }

    inline bool IsValidBreakContinue() {
        return (stmt_stack.empty() || stmt_stack.back() != LOOP_t) ? false : true;
    }

    inline bool InLoop() {
        return loop_stmts.size() != 0; 
    }

    inline bool InFuncDef() {
        return func_def_stmts.size() != 0; 
    }
}
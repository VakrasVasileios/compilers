#ifndef EXECUTE_ARITHMETIC_H
#define EXECUTE_ARITHMETIC_H

#include "../../../../../util/contract/include/contract.h"
#include "../../memcell/include/avm_memcell.h"

namespace avm
{
    namespace exec
    {
    /**
     * @brief Executes the add target code 
     * instruction.
     * 
     * @param res the result memcell, not null
     * @param arg1 the arg1 memcell, not null
     * @param arg2 the arg2 memcell, not null
     */
    void execute_add(memcell::AvmMemcell* res, memcell::AvmMemcell* arg1, 
        memcell::AvmMemcell* arg2);
    /**
     * @brief Executes the sub target code 
     * instruction.
     * 
     * @param res the result memcell, not null
     * @param arg1 the arg1 memcell, not null
     * @param arg2 the arg2 memcell, not null
     */
    void execute_sub(memcell::AvmMemcell* res, memcell::AvmMemcell* arg1, 
        memcell::AvmMemcell* arg2);    
    /**
     * @brief Executes the mul target code 
     * instruction.
     * 
     * @param res the result memcell, not null
     * @param arg1 the arg1 memcell, not null
     * @param arg2 the arg2 memcell, not null
     */    
    void execute_mul(memcell::AvmMemcell* res, memcell::AvmMemcell* arg1, 
        memcell::AvmMemcell* arg2);  
     /**
     * @brief Executes the div target code 
     * instruction.
     * 
     * @param res the result memcell, not null
     * @param arg1 the arg1 memcell, not null
     * @param arg2 the arg2 memcell, not null
     */ 
    void execute_div(memcell::AvmMemcell* res, memcell::AvmMemcell* arg1, 
        memcell::AvmMemcell* arg2);   
     /**
     * @brief Executes the mod target code 
     * instruction.
     * 
     * @param res the result memcell, not null
     * @param arg1 the arg1 memcell, not null
     * @param arg2 the arg2 memcell, not null
     */  
    void execute_mod(memcell::AvmMemcell* res, memcell::AvmMemcell* arg1, 
        memcell::AvmMemcell* arg2);    
    }
}

#endif
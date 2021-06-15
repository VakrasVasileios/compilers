#ifndef EXECUTION_CYCLE_H
#define EXECUTION_CYCLE_H

#include <string>
#include <iostream>
#include "../../memory/include/memory.h"
#include "../../../../target_code/include/instruction.h"

#define AVM_ENDING_PC 0

namespace avm
{
    namespace cpu
    {
    /**
     * @brief Signals wether the execution cycle is finished.
     * 
     */
    extern bool     execution_finished;
    /**
     * @brief The current line (?) of the executed instruction.
     * 
     */
    extern unsigned curr_line;
    /**
     * @brief Logs an avm error and signals that the
     * execution cycle is finished.
     * 
     * @param msg the message to be prompted
     * @param os the output of the log
     */
    void log_error(std::string msg, std::ostream& os);
    /**
     * @brief Logs an avm warning.
     * 
     * @param msg the message to be prompted
     * @param os the output of the log
     */
    void log_warning(std::string msg, std::ostream& os);
    }
}

#endif
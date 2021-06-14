#ifndef EXEC_H
#define EXEC_H

#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include "../../contract/include/contract.h"

namespace util
{
    namespace exec
    {
    /**
     * @brief Executes a command from the current working directory.
     * 
     * @param cmd the command that is to be executed, not null
     * 
     * @return the output of the command as a  std::string.
     *  
    **/
    std::string exec(const char* cmd);
    }
}

#endif
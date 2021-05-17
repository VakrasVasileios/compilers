#ifndef EXEC_H
#define EXEC_H

#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <assert.h>

/**
 * @brief Executes a command from the current working directory.
 * It's a checked runtime error for the cmd parameter to be null.
 * 
 * @param cmd the command that is to be executed, not null
 * 
 * @return the output of the command as a  std::string.
 *  
**/
std::string exec(const char* cmd);

#endif
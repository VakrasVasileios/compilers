#ifndef USERFUNC_ENTRY_H
#define USERFUNC_ENTRY_H

#include <string>

namespace target_code
{
typedef struct userfunc_entry {
    unsigned int    taddress;
    unsigned int    local_count;
    unsigned int    total_args;
    std::string     id;

    userfunc_entry(unsigned int _taddress, unsigned int _local_count, unsigned int _totargs, std::string _id) 
                    : taddress(_taddress), local_count(_local_count), total_args(_totargs), id(_id) {}
} Userfunc_entry;
}

#endif
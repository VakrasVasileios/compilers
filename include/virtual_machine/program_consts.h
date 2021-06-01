#ifndef _PROGRAMCONSTS_H_
#define _PROGRAMCONSTS_H_

#include <vector>
#include <string>
#include <map>

namespace virtual_machine {
    typedef struct userfunc_entry {
        unsigned int    taddress;
        unsigned int    local_count;
        std::string     id;

        userfunc_entry(unsigned int _taddress, unsigned int _local_count, std::string _id) : taddress(_taddress), local_count(_local_count), id(_id) {}
    } Userfunc_entry;

    class ProgramConsts {
    public:
        static ProgramConsts&     GetInstance(void);
        static const ProgramConsts&     GetInstanceConst(void);

        unsigned        InsertNumber(double num);
        unsigned        InsertString(const std::string& str);
        unsigned        InsertLibFunc(const std::string& str);

        double          GetNumber(unsigned index);
        std::string     GetString(unsigned index);
        std::string     GetLibFunc(unsigned index);

    private:
        std::vector<double>         number_array;
        std::vector<std::string>    string_array;
        std::vector<std::string>    libfunc_array;

        std::map<double, unsigned>        number_map;
        std::map<std::string, unsigned>   string_map;
        std::map<std::string, unsigned>   libfunc_map;

        ProgramConsts() = default;
        ProgramConsts(const ProgramConsts&) = delete;
        ProgramConsts(ProgramConsts&&) = delete;
    };
}

#endif

#ifndef _PROGRAMCONSTS_H_
#define _PROGRAMCONSTS_H_

#include <vector>
#include <string>
#include <map>
#include <ostream>
#include "expression/include/expression.h"
#include "../include/userfunc_entry.h"

namespace target_code 
{
class ProgramConsts {
public:
    static ProgramConsts&           GetInstance(void);
    static const ProgramConsts&     GetInstanceConst(void);

    unsigned        InsertNumber(double num);
    unsigned        InsertString(const std::string& str);
    unsigned        InsertLibFunc(const std::string& str);
    unsigned        InsertUserFunc(expression::Symbol* userfunc);

    double          GetNumber(unsigned index);
    std::string     GetString(unsigned index);
    std::string     GetLibFunc(unsigned index);
    unsigned        GetUserFunc(unsigned index);

    std::vector<double>*            GetNumberArray() const;
    std::vector<std::string>*       GetStringArray() const;
    std::vector<std::string>*       GetLibFuncArray() const;
    std::vector<Userfunc_entry>*    GetUserFuncArray() const;

    friend std::ostream &operator<<(std::ostream &os, const ProgramConsts &rhs);
private:
    std::vector<double>*                number_array;
    std::vector<std::string>*           string_array;
    std::vector<std::string>*           libfunc_array;
    std::vector<Userfunc_entry>*        userfunc_array;

    std::map<double, unsigned>          number_map;
    std::map<std::string, unsigned>     string_map;
    std::map<std::string, unsigned>     libfunc_map;
    std::map<unsigned, unsigned>        userfunc_map;

    void                                LogNumberArray(std::ostream &os) const;
    void                                LogStringArray(std::ostream &os) const;
    void                                LogLibFuncArray(std::ostream &os) const;
    void                                LogUserFuncArray(std::ostream &os) const;

    ProgramConsts();
    ProgramConsts(const ProgramConsts&) = delete;
    ProgramConsts(ProgramConsts&&) = delete;
};
}

#endif

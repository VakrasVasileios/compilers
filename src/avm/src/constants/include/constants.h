#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <vector>
#include <string>
#include "../../../../target_code/include/userfunc_entry.h"

namespace constants {

    class Constants final {
    public:
        static Constants&                   GetInstance();

        void                                InsertNumber(double);
        void                                InsertString(std::string);
        void                                InsertLibfunc(std::string);
        void                                InsertUserfunc(target_code::Userfunc_entry);

        double                              GetNumber(unsigned);
        std::string                         GetString(unsigned);
        std::string                         GetLibfunc(unsigned);
        target_code::Userfunc_entry         GetUserfunc(unsigned);

        friend std::ostream&                operator<<(std::ostream& os, const Constants& cons);

    private:
        Constants() = default;
        Constants(const Constants&) = delete;
        Constants(Constants&&) = delete;

        std::vector<double>                         number_array;
        std::vector<std::string>                    string_array;
        std::vector<std::string>                    libfunc_array;
        std::vector<target_code::Userfunc_entry>    userfunc_array;
        
        void    LogArrays(std::ostream& os) const;
    };
}
#endif
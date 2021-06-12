#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <vector>
#include <string>
#include "target_code/target_code.h"

namespace constants {

    class Constants final {
    public:
        static auto    GetInstance() -> Constants&;

        void                                InsertNumber(double);
        void                                InsertString(std::string);
        void                                InsertLibfunc(std::string);
        void                                InsertUserfunc(target_code::Userfunc_entry);

        double                              GetNumber(unsigned);
        std::string                         GetString(unsigned);
        std::string                         GetLibfunc(unsigned);
        target_code::Userfunc_entry         GetUserfunc(unsigned);

        void    LogArrays();

    private:
        Constants() = default;
        Constants(Constants&) = delete;
        Constants(Constants&&) = delete;

        std::vector<double>                         number_array;
        std::vector<std::string>                    string_array;
        std::vector<std::string>                    libfunc_array;
        std::vector<target_code::Userfunc_entry>    userfunc_array;
    };
}
#endif
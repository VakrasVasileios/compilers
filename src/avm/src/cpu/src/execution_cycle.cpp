#include "execution_cycle.h"

namespace avm
{
    namespace cpu
    {
        bool        execution_finished = false;
        unsigned    curr_line = 0;

        void log_error(std::string msg, std::ostream& os) {
            os << msg << std::endl;
            execution_finished = true;
        }

        void log_warning(std::string msg, std::ostream& os) {
            os << msg << std::endl;
        }
    }
}
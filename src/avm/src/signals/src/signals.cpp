#include "../include/signals.h"

namespace avm
{
    namespace signals
    {
        bool        execution_finished = false;
        unsigned    curr_line = 0;

        void log_error(std::string msg, std::ostream& os) {
            os << msg << ", at line: " << curr_line << std::endl;
            execution_finished = true;
        }

        void log_warning(std::string msg, std::ostream& os) {
            os << msg << ", at line: " << curr_line << std::endl;
        }
    }
}
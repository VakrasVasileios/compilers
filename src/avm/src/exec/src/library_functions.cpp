#include "library_functions.h"
#include "math.h"
#include <map>
#include <iostream>
#include <functional>
#include "../../memcell/include/avm_memcell.h"
#include "../../signals/include/signals.h"
#include "../../registers/include/registers.h"
#include "../../memory/include/memory.h"
#include "../include/execute_function.h"
#include "../include/execute_assign.h"

namespace avm
{
    namespace exec
    {
        namespace
        {
            unsigned total_actuals() {
                return memory::stack_segment.get_envvalue(registers::topsp +
                    AVM_NUMACTUALS_OFFSET);
            }

            memcell::AvmMemcell* get_actual(unsigned i) {
                assert(i <= total_actuals());
                return memory::stack_segment[registers::topsp + AVM_STACKENV_SIZE + i];
            }

            void libfunc_print() {
                unsigned n = total_actuals();
                for (unsigned i = 0; i < n; i++) {
                    std::cout << *get_actual(i);
                }
            }

            void libfunc_typeof() {
                unsigned n = total_actuals();
                if (n != 1) {
                    std::string msg("One argument (not ");
                    msg += std::to_string(n);
                    msg += ") expected in 'typeof'!";
                    signals::log_error(msg, std::cerr);
                } else {
                    exec::execute_assign(&registers::retval,
                        new memcell::StringMemcell(get_actual(0)->get_type()));
                }
            }

            void libfunc_input() {
                std::string inp;
                std::cin >> inp;
                exec::execute_assign(&registers::retval, new memcell::StringMemcell(inp));
            }

            void libfunc_objectmemberkeys() {
                // if (n != 1) {
                //     signals::log_error(
                //         "one argument (not " + n + ") expected in \'typeof\'!",
                //             std::cerr);
                // } else {
                //     auto table = get_actual(0);
                //     exec::execute_assign(registers::retval,
                //         get_actual(0)->get_type());
                // }
            }

            void libfunc_objecttotalmembers() {

            }

            void libfunc_objectcopy() {

            }

            void libfunc_totalarguments() {
                exec::execute_assign(&registers::retval,
                        new memcell::NumMemcell(total_actuals()));
            }

            void libfunc_argument() {
                unsigned totalactuals = total_actuals();
                if (totalactuals < 1)
                    signals::log_error("Too few arguments for 'argument'", std::cerr);
                else if (dynamic_cast<memcell::NumMemcell*>(get_actual(0)) == nullptr) {
                    signals::log_error("Argument expected to be number", std::cerr);
                }
                else {
                    unsigned offset = dynamic_cast<memcell::NumMemcell*>(get_actual(0))->num_val();
                    
                    auto old_topsp_mem = memory::stack_segment[registers::topsp + AVM_SAVEDTOPSP_OFFSET];
                    unsigned old_topsp = ((memcell::NumMemcell*)old_topsp_mem)->num_val();

                    auto totargs_mem = memory::stack_segment[old_topsp + AVM_NUMACTUALS_OFFSET];
                    unsigned totargs = ((memcell::NumMemcell*)totargs_mem)->num_val();

                    if (totargs < offset) {
                        std::string msg("Argument ");
                        msg += std::to_string(offset);
                        msg += " does not exist";
                        signals::log_error(msg, std::cerr);
                    }
                    else
                        exec::execute_assign(&registers::retval, memory::stack_segment[old_topsp + AVM_STACKENV_SIZE + offset]);
                }
            }

            void libfunc_strtonum() {
                unsigned totalactuals = total_actuals();
                if (totalactuals < 1)
                    signals::log_error("Too few arguments", std::cerr);
                else if (dynamic_cast<memcell::StringMemcell*>(get_actual(0)) == nullptr)
                    signals::log_error("Argument expected to be string", std::cerr);
                else{
                    std::string str = dynamic_cast<memcell::StringMemcell*>(get_actual(0))->str_val();
                    double num = std::stod(str);
                    exec::execute_assign(&registers::retval, new memcell::NumMemcell(num));
                }
            }

            void libfunc_sqrt() {
                unsigned totalactuals = total_actuals();
                if (totalactuals < 1)
                    signals::log_error("Too few arguments", std::cerr);
                else if (dynamic_cast<memcell::NumMemcell*>(get_actual(0)) == nullptr)
                    signals::log_error("Argument expected to be number", std::cerr);
                else{
                    double arg = dynamic_cast<memcell::NumMemcell*>(get_actual(0))->num_val();
                    double sqrt_out = std::sqrt(arg);
                    exec::execute_assign(&registers::retval, new memcell::NumMemcell(sqrt_out));
                }
            }

            void libfunc_cos() {
                unsigned totalactuals = total_actuals();
                if (totalactuals < 1)
                    signals::log_error("Too few arguments", std::cerr);
                else if (dynamic_cast<memcell::NumMemcell*>(get_actual(0)) == nullptr)
                    signals::log_error("Argument expected to be number", std::cerr);
                else{
                    double arg = dynamic_cast<memcell::NumMemcell*>(get_actual(0))->num_val();
                    double cos_out = std::cos(arg);
                    exec::execute_assign(&registers::retval, new memcell::NumMemcell(cos_out));
                }
            }

            void libfunc_sin() {
                unsigned totalactuals = total_actuals();
                if (totalactuals < 1)
                    signals::log_error("Too few arguments", std::cerr);
                else if (dynamic_cast<memcell::NumMemcell*>(get_actual(0)) == nullptr)
                    signals::log_error("Argument expected to be number", std::cerr);
                else{
                    double arg = dynamic_cast<memcell::NumMemcell*>(get_actual(0))->num_val();
                    double sin_out = std::sin(arg);
                    exec::execute_assign(&registers::retval, new memcell::NumMemcell(sin_out));
                }
            }

            typedef void (*libfunc) ();

            typedef std::map<std::string, unsigned> libfuncs_by_id;

            libfuncs_by_id libs;
            
            const libfunc libfuncs[] = {
                &libfunc_print, &libfunc_typeof, &libfunc_input,
                &libfunc_objectmemberkeys, &libfunc_objectcopy,
                &libfunc_totalarguments, &libfunc_argument,
                &libfunc_strtonum, &libfunc_sqrt,
                &libfunc_cos, &libfunc_sin
            };

            void init_libfuncs() {
                libs.insert( {std::string("print"), 0} );
                libs.insert( {std::string("typeof"), 1} );
                libs.insert( {std::string("input"), 2} );
                libs.insert( {std::string("objectmemberkeys"), 3} );
                libs.insert( {std::string("objectcopy"), 4} );
                libs.insert( {std::string("totalarguments"), 5} );
                libs.insert( {std::string("argument"), 6} );
                libs.insert( {std::string("strtonum"), 7} );
                libs.insert( {std::string("sqrt"), 8} );
                libs.insert( {std::string("cos"), 9} );
                libs.insert( {std::string("sin"), 10} );
            }

            bool unsupported_libfunc(std::string libfunc_id) {
                return libs.find(libfunc_id) != libs.end();
            }

            void execute_enterlibfunc() {
                registers::topsp = registers::top;
                exec::total_actuals = 0;
            }

            void execute_libfunc(unsigned i) {
                std::cout << i << std::endl;
                assert (i < 11);
                libfuncs[i]();
            }

            void execute_exitlibfunc() {
                if (!signals::execution_finished)
                    execute_exitfunc();
            }
        } // namespace

        void call_libfunc(const std::string libfunc_id) {
            if (libs.empty())
                init_libfuncs();
            if (unsupported_libfunc(libfunc_id)) {
                signals::log_error(
                    "Unsupported library function " + libfunc_id + " is called", std::cerr);
            } else {
                unsigned index = libs[libfunc_id];
                execute_enterlibfunc();
                execute_libfunc(index);
                execute_exitlibfunc();
            }
        }
    } // namespace exec
} // namespace avm
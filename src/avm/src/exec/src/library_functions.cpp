#include "library_functions.h"
#include "../../memcell/include/avm_memcell.h"
#include "../../signals/include/signals.h"
#include "../../registers/include/registers.h"
#include "../../memory/include/memory.h"
#include "../include/execute_function.h"
#include "../include/execute_assign.h"
#include "math.h"
#include <map>
#include <string>
#include <cstring>
#include <iostream>

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

            unsigned total_actuals(unsigned topsp) {
                return memory::stack_segment.get_envvalue(topsp +
                    AVM_NUMACTUALS_OFFSET);
            }

            const memcell::AvmMemcell* get_actual(unsigned i) {
                assert(i <= total_actuals());
                return memory::stack_segment[registers::topsp + AVM_STACKENV_SIZE + i];
            }

            void libfunc_print() {
                unsigned n = total_actuals();
                for (unsigned i = 0; i < n; i++) {
                    std::cout << *get_actual(i);
                }
                exec::execute_assign(&registers::retval,
                    new memcell::UndefMemcell());
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
                /* if (*inp.begin() == '\"' && inp.back() == '\"') {
                    inp.pop_back();
                    inp.erase(inp.begin());
                    exec::execute_assign(&registers::retval, new memcell::StringMemcell(inp));
                } */
                /* else {
                    exec::execute_assign(&registers::retval, new memcell::NumMemcell(std::stod(inp)));
                } */
                if (!std::strcmp("true", inp.c_str())) {
                    exec::execute_assign(&registers::retval, new memcell::BoolMemcell(true));
                }
                else if (!std::strcmp("false", inp.c_str())) {
                    exec::execute_assign(&registers::retval, new memcell::BoolMemcell(false));
                }
                else if (!std::strcmp("nil", inp.c_str())) {
                    exec::execute_assign(&registers::retval, new memcell::NilMemcell());
                }
                else
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
                if (exec::scope_depth == 1)
                     exec::execute_assign(&registers::retval,
                        new memcell::NilMemcell());
                else {
                    auto mem = memory::stack_segment[registers::topsp+AVM_SAVEDTOPSP_OFFSET];
                    unsigned topsp = dynamic_cast<memcell::NumMemcell*>(mem)->num_val();
                    exec::execute_assign(&registers::retval,
                            new memcell::NumMemcell(total_actuals(topsp)));
                }
            }

            void libfunc_argument() {
                unsigned totalactuals = total_actuals();
                if (totalactuals < 1)
                    signals::log_error("Too few arguments for 'argument'", std::cerr);
                else if (dynamic_cast<const memcell::NumMemcell*>(get_actual(0)) == nullptr) {
                    signals::log_error("Argument expected to be number", std::cerr);
                }
                else {
                    unsigned offset = dynamic_cast<const memcell::NumMemcell*>(get_actual(0))->num_val();
                    
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
                else if (dynamic_cast<const memcell::StringMemcell*>(get_actual(0)) == nullptr)
                    signals::log_error("Argument expected to be string", std::cerr);
                else{
                    std::string str = dynamic_cast<const memcell::StringMemcell*>(get_actual(0))->str_val();
                    double num = std::stod(str);
                    exec::execute_assign(&registers::retval, new memcell::NumMemcell(num));
                }
            }

            void libfunc_sqrt() {
                unsigned totalactuals = total_actuals();
                if (totalactuals < 1)
                    signals::log_error("Too few arguments", std::cerr);
                else if (dynamic_cast<const memcell::NumMemcell*>(get_actual(0)) == nullptr)
                    signals::log_error("Argument expected to be number", std::cerr);
                else{
                    double arg = dynamic_cast<const memcell::NumMemcell*>(get_actual(0))->num_val();
                    double sqrt_out = std::sqrt(arg);
                    exec::execute_assign(&registers::retval, new memcell::NumMemcell(sqrt_out));
                }
            }

            void libfunc_cos() {
                unsigned totalactuals = total_actuals();
                if (totalactuals < 1)
                    signals::log_error("Too few arguments", std::cerr);
                else if (dynamic_cast<const memcell::NumMemcell*>(get_actual(0)) == nullptr)
                    signals::log_error("Argument expected to be number", std::cerr);
                else{
                    double arg = dynamic_cast<const memcell::NumMemcell*>(get_actual(0))->num_val();
                    double cos_out = std::cos(arg);
                    exec::execute_assign(&registers::retval, new memcell::NumMemcell(cos_out));
                }
            }

            void libfunc_sin() {
                unsigned totalactuals = total_actuals();
                if (totalactuals < 1)
                    signals::log_error("Too few arguments", std::cerr);
                else if (dynamic_cast<const memcell::NumMemcell*>(get_actual(0)) == nullptr)
                    signals::log_error("Argument expected to be number", std::cerr);
                else{
                    double arg = dynamic_cast<const memcell::NumMemcell*>(get_actual(0))->num_val();
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
                libs.insert( {"print", 0} );
                libs.insert( {"typeof", 1} );
                libs.insert( {"input", 2} );
                libs.insert( {"objectmemberkeys", 3} );
                libs.insert( {"objectcopy", 4} );
                libs.insert( {"totalarguments", 5} );
                libs.insert( {"argument", 6} );
                libs.insert( {"strtonum", 7} );
                libs.insert( {"sqrt", 8} );
                libs.insert( {"cos", 9} );
                libs.insert( {"sin", 10} );
            }

            bool unsupported_libfunc(std::string libfunc_id) {
                return libs.find(libfunc_id) != libs.end();
            }

            void execute_enterlibfunc() {
                registers::topsp = registers::top;
                exec::total_actuals = 0;
            }

            void execute_libfunc(unsigned i) {
                assert (i < 11);
                libfuncs[i]();
            }

            void execute_exitlibfunc() {
                if (!signals::execution_finished)
                    execute_exitfunc();
            }
        } // namespace

        void call_libfunc(const std::string& libfunc_id) {
            if (!std::strcmp("print", libfunc_id.c_str())) {
                execute_enterlibfunc();
                libfunc_print();
                execute_exitlibfunc();
            }
            else if (!std::strcmp("typeof", libfunc_id.c_str())) {
                execute_enterlibfunc();
                libfunc_typeof();
                execute_exitlibfunc();
            }
            else if (!std::strcmp("input", libfunc_id.c_str())) {
                execute_enterlibfunc();
                libfunc_input();
                execute_exitlibfunc();
            }
            else if (!std::strcmp("objectmemberkeys", libfunc_id.c_str())) {
                execute_enterlibfunc();
                libfunc_objectmemberkeys();
                execute_exitlibfunc();
            }
            else if (!std::strcmp("objectcopy", libfunc_id.c_str())) {
                execute_enterlibfunc();
                libfunc_objectcopy();
                execute_exitlibfunc();
            }
            else if (!std::strcmp("totalarguments", libfunc_id.c_str())) {
                execute_enterlibfunc();
                libfunc_totalarguments();
                execute_exitlibfunc();
            }
            else if (!std::strcmp("argument", libfunc_id.c_str())) {
                execute_enterlibfunc();
                libfunc_argument();
                execute_exitlibfunc();
            }
            else if (!std::strcmp("strtonum", libfunc_id.c_str())) {
                execute_enterlibfunc();
                libfunc_strtonum();
                execute_exitlibfunc();
            }
            else if (!std::strcmp("sqrt", libfunc_id.c_str())) {
                execute_enterlibfunc();
                
                libfunc_sqrt();
                execute_exitlibfunc();
            }
            else if (!std::strcmp("cos", libfunc_id.c_str())) {
                execute_enterlibfunc();
                
                libfunc_cos();
                execute_exitlibfunc();
            }
            else if (!std::strcmp("sin", libfunc_id.c_str())) {
                execute_enterlibfunc();
                
                libfunc_sin();
                execute_exitlibfunc();
            }
            else
                signals::log_error(
                    "Unsupported library function " + libfunc_id + " is called", std::cerr);
        }
    } // namespace exec
} // namespace avm
#include "../include/execute_table.h"
#include "../include/execute_assign.h"
#include "../../signals/include/signals.h"

namespace avm
{
    namespace exec
    {

    void execute_newtable(memcell::AvmMemcell* memcell) {
        PRECONDITION(memcell != nullptr);
        memcell = new memcell::TableMemcell();
    }

    namespace
    {
        class MemcellTableElemSetter final : public memcell::AvmMemcellVisitor {
        public:
            MemcellTableElemSetter(memcell::AvmMemcell* _key,
                memcell::AvmMemcell* _value) : key_(_key), value_(_value)
                {}
            ~MemcellTableElemSetter() = default;

            void visit_num_memcell(memcell::NumMemcell* memcell) override
            {
                assert(memcell != nullptr);
                signals::log_error("Illegal use of numerical type as a table",
                    std::cerr);
            }

            void visit_string_memcell(memcell::StringMemcell* memcell) override
            {
                assert(memcell != nullptr);
                signals::log_error("Illegal use of string type as a table",
                    std::cerr);
            }

            void visit_bool_memcell(memcell::BoolMemcell* memcell) override {
                assert(memcell != nullptr);
                signals::log_error("Illegal use of boolean type as a table",
                    std::cerr);
            }

            void visit_table_memcell(memcell::TableMemcell* memcell) override {
                assert(memcell != nullptr);
                memcell->set_elem(key_, value_);
            }

            void visit_userfunc_memcell(memcell::UserfuncMemcell* memcell) 
            override {
                assert(memcell != nullptr);
                signals::log_error("Illegal use of userfunc type as a table",
                    std::cerr);
            }

            void visit_libfunc_memcell(memcell::LibfuncMemcell* memcell)
            override {
                assert(memcell != nullptr);
                signals::log_error("Illegal use of libfunc type as a table",
                    std::cerr);
            }

            void visit_nill_memcell(memcell::NilMemcell* memcell) override {
                assert(memcell != nullptr);
                signals::log_error("Illegal use of NIL type as a table",
                    std::cerr);
            }
            void visit_undef_memcell(memcell::UndefMemcell* memcell) override {
                assert(memcell != nullptr);
                signals::log_error("Illegal use of undefined type as a table",
                    std::cerr);
            }
        private:
            memcell::AvmMemcell* key_;
            memcell::AvmMemcell* value_;  
        };
    }
    void execute_tablesetelem(memcell::AvmMemcell* memcell,
        memcell::AvmMemcell* key, memcell::AvmMemcell* value) {
        PRECONDITION(memcell != nullptr);
        PRECONDITION(key != nullptr);
        PRECONDITION(value != nullptr);
        auto visitor = new MemcellTableElemSetter(key, value);
        memcell->accept(visitor);
    }

    namespace
    {
        class MemcellTableElemGetter final : public memcell::AvmMemcellVisitor {
        public:
            MemcellTableElemGetter(memcell::AvmMemcell* _lv,
                memcell::AvmMemcell* _key) : lv_(_lv), key_(_key) {}
            ~MemcellTableElemGetter() = default;

            void visit_num_memcell(memcell::NumMemcell* memcell) override {
                assert(memcell != nullptr);
                signals::log_error("Illegal use of numerical type as a table",
                    std::cerr);
            }

            void visit_string_memcell(memcell::StringMemcell* memcell) override 
            {
                assert(memcell != nullptr);
                signals::log_error("Illegal use of string type as a table",
                    std::cerr);
            }

            void visit_bool_memcell(memcell::BoolMemcell* memcell) override {
                assert(memcell != nullptr);
                signals::log_error("Illegal use of boolean type as a table",
                    std::cerr);
            }

            void visit_table_memcell(memcell::TableMemcell* memcell) override {
                assert(memcell != nullptr);
                if (memcell->contains(key_))
                    execute_assign(lv_, memcell->get_elem(key_));
                else
                    signals::log_warning("Key not found", std::cerr);
            }

            void visit_userfunc_memcell(memcell::UserfuncMemcell* memcell)
            override {
                assert(memcell != nullptr);
                signals::log_error("Illegal use of userfunc type as a table",
                    std::cerr);
            }

            void visit_libfunc_memcell(memcell::LibfuncMemcell* memcell)
            override {
                assert(memcell != nullptr);
                signals::log_error("Illegal use of libfunc type as a table",
                    std::cerr);
            }

            void visit_nill_memcell(memcell::NilMemcell* memcell) override {
                assert(memcell != nullptr);
                signals::log_error("Illegal use of NIL type as a table",
                    std::cerr);
            }
            void visit_undef_memcell(memcell::UndefMemcell* memcell) override {
                assert(memcell != nullptr);
                signals::log_error("Illegal use of undefined type as a table",
                    std::cerr);
            }
        private:
            memcell::AvmMemcell* lv_;
            memcell::AvmMemcell* key_;
        };
        }

   void execute_tablegetelem(memcell::AvmMemcell* lvalue,
        memcell::AvmMemcell* memcell, memcell::AvmMemcell* key) 
        {
        PRECONDITION(lvalue != nullptr);
        PRECONDITION(memcell != nullptr);
        PRECONDITION(key != nullptr);
        auto visitor = new MemcellTableElemGetter(lvalue, key);
        memcell->accept(visitor);
    }
    }
}
#ifndef AVM_MEMCELL_H
#define AVM_MEMCELL_H

#include <string>
#include "../../util/contract/contract.h"
#include "avm_data.h"

namespace virtual_machine
{

class AvmMemcellVisitor;    // fwd declare

/**
 * @brief An abstract avm memory cell with different
 * data types.
 * 
 */
class AvmMemcell {
public:
    /**
     * @brief Constructs a new AvmMemcell object.
     * 
     */
    AvmMemcell() = default;
    /**
     * @brief Destroys this AvmMemcell object.
     * 
     */
    virtual ~AvmMemcell() = default;
    /**
     * @brief Accepts an avm memcell visitor at this
     * AvmMemcell.
     * 
     * @param visitor the avm memcell visitor to be
     * accepted, not null
     */
    virtual void    accept(const AvmMemcellVisitor* visitor) {}
};

/**
 * @brief A memcell containing numerical data.
 * 
 */
class NumMemcell final : public AvmMemcell {
public:
    /**
     * @brief Constructs a new NumMemcell object.
     * 
     * @param _num_val the numerical data of the new
     * NumMemcell object. 
     */
    NumMemcell(const double _num_val) : num_val_(_num_val) {}
    /**
     * @brief Destroys this NumMemcell object.
     * 
     */
    ~NumMemcell() = default;    
    /**
     * @brief Returns a read access to the numerical data
     * of this NumMemcell.
     * 
     * @return a read access to the numerical data
     * of this NumMemcell 
     */
    double  num_val() const;
    /**
     * @brief Sets the numerical data
     * of this NumMemcell.
     * 
     * @param _num_val the numerical value to set 
     * the numerical data of this NumMemcell
     */
    void    set_num_val(const double _num_val); 
    void    accept(const AvmMemcellVisitor* visitor) override;
private:
    double num_val_;
};

/**
 * @brief A memcell containing string data.
 * 
 */
class StringMemcell final : public AvmMemcell {
public:
    /**
     * @brief Constructs a new StringMemcell object.
     * 
     * @param _str_val the string data of the new
     * StringMemcell object
     */
    StringMemcell(const std::string _str_val) : str_val_(_str_val) {} 
    /**
     * @brief Destroys this StringMemcell object.
     * 
     */
    ~StringMemcell() = default;
    /**
     * @brief Returns a read access to this StringMemCell 
     * string data.
     * 
     * @return a read access to this StringMemCell 
     * string data  
     */
    std::string str_val() const;
    /**
     * @brief Sets the string data of this StringMemCell.
     * 
     * @param str_val the string value to set the string
     * data of this StringMemCell
     */
    void        set_str_val(const std::string str_val);
    void        accept(const AvmMemcellVisitor* visitor) override;
private:
    std::string str_val_;
};

/**
 * @brief A memcell containing boolean data.
 * 
 */
class BoolMemcell final : public AvmMemcell {
public:
    /**
     * @brief Constructs a new BoolMemcell object.
     * 
     * @param _bool_val the boolean value to set
     * the boolean data of the new BoolMemcell object 
     */
    BoolMemcell(const bool _bool_val) : bool_val_(_bool_val) {}
    /**
     * @brief Destroys this BoolMemcell object.
     * 
     */
    ~BoolMemcell() = default;
    /**
     * @brief Returns a read access to the boolean data
     * of this BoolMemcell.
     * 
     * @return a read access to the boolean data
     * of this BoolMemcell 
     */
    bool    bool_val() const;
    /**
     * @brief Sets the boolean data of this BoolMemcell.
     * 
     * @param val the boolean value to set the
     * boolean data of this BoolMemcell.
     */
    void    set_bool_val(const bool val);
    void    accept(const AvmMemcellVisitor* visitor) override;
private:
    bool bool_val_;
};

/**
 * @brief A memcell containing table data.
 * 
 */
class TableMemcell final : public AvmMemcell {
public:
    /**
     * @brief Constructs a new TableMemcell object.
     * 
     * @param _table_val the table data of the
     * new TableMemcell object, not null
     */
    TableMemcell(AvmTable* _table_val)
    : table_val_(verify_avm_table(_table_val)) {INVARIANT(table_val_ != nullptr);}
    /**
     * @brief Destroys this TableMemcell object.
     * 
     */
    ~TableMemcell() = default;
    /**
     * @brief Returns a read access to the table data
     * of this TableMemcell.
     * 
     * @return a read access to the table data
     * of this TableMemcell, not null 
     */
    AvmTable*   table_val() const;
    /**
     * @brief Sets the table data of this TableMemcell.
     * 
     * @param table the table value to set the table data
     * of this TableMemcell, not null
     */
    void        set_table_val(AvmTable* table);
    void        accept(const AvmMemcellVisitor* visitor) override;
private:
    AvmTable*   table_val_;
    AvmTable*   verify_avm_table(AvmTable* table) const;
};

/**
 * @brief A memcell containing the address of a user function as data.
 * 
 */
class UserfuncMemcell final : public AvmMemcell {
public:
    /**
     * @brief Constructs a new UserfuncMemcell object.
     * 
     * @param _func_val the address of a user function
     * to be set as the data of this UserfuncMemcell
     * 
     */
    UserfuncMemcell(const unsigned int _func_val) : func_val_(_func_val) {}
    /**
     * @brief Destroys this UserfuncMemcell object.
     * 
     */
    ~UserfuncMemcell() = default;
    /**
     * @brief Returns a read access to the
     * data of this UserfuncMemcell containing
     * the address of a user function.
     * 
     * @return a read access to the
     * data of this UserfuncMemcell containing
     * the address of a user function
     */
    unsigned int    func_val() const;
    /**
     * @brief Sets the address of a user function as the data
     * of this UserfuncMemcell.
     * 
     * @param _func_val the address of a user function to be set
     * as the data of this UserfuncMemcell
     */
    void            set_func_val(const unsigned int _func_val);
    void            accept(const AvmMemcellVisitor* visitor) override;       
private:
    unsigned int func_val_;
};

/**
 * @brief A memcell containing the id of a library function as data.
 * 
 */
class LibfuncMemcell final : public AvmMemcell {
public:
    /**
     * @brief Constructs a new LibfuncMemcell object.
     * 
     * @param _lib_func_val the id of a library function
     * to be set as the data of the new LibfuncMemcell object
     */
    LibfuncMemcell(const std::string _lib_func_val)
    : lib_func_val_(_lib_func_val) {};
    /**
     * @brief Destroys this LibfuncMemcell object.
     * 
     */
    ~LibfuncMemcell() = default;
    /**
     * @brief Returns a read access to the id of a library function
     * that is set as the data of this LibfuncMemcell.
     * 
     * @return a read access to the id of a library function
     * that is set as the data of this LibfuncMemcell
     */
    std::string lib_func_val() const;
    /**
     * @brief Sets the id of a library function as the data
     * of this LibfuncMemcell.
     * 
     * @param _lib_func_val the id of a library function to
     * be set as the data of this LibfuncMemcell
     */
    void        set_lib_func_val(const std::string _lib_func_val);
    void        accept(const AvmMemcellVisitor* visitor) override;
private:
    std::string lib_func_val_;
};

/**
 * @brief A memcell containing NIL data.
 * 
 */
class NilMemcell final : public AvmMemcell {
public:
    /**
     * @brief Constructs a new NilMemcell object.
     * 
     */
    NilMemcell() {}
    /**
     * @brief Destroys this NilMemcell object.
     * 
     */
    ~NilMemcell() = default;
    void        accept(const AvmMemcellVisitor* visitor) override;
};

/**
 * @brief A memcell with undefined data.
 * 
 */
class UndefMemcell final : public AvmMemcell {
public:
    /**
     * @brief Constructs a new UndefMemcell object.
     * 
     */
    UndefMemcell() {}
    /**
     * @brief Destroys this UndefMemcell object.
     * 
     */
    ~UndefMemcell() = default;
    void        accept(const AvmMemcellVisitor* visitor) override;
};

/**
 * @brief Interface for visiting concrete avm memcells.
 * 
 */
class AvmMemcellVisitor {
public:
    /**
     * @brief Visits a memcell containing numerical data.
     * 
     * @param memcell the memcell containing numerical data
     */
    virtual void    visit_num_memcell(NumMemcell* memcell) const = 0;
    /**
     * @brief Visits a memcell containing string data.
     * 
     * @param memcell the memcell containing string data
     */
    virtual void    visit_string_memcell(StringMemcell* memcell) const = 0;
    /**
     * @brief Visits a memcell containing bool data.
     * 
     * @param memcell the memcell containing bool data
     */
    virtual void    visit_bool_memcell(BoolMemcell* memcell) const = 0;
    /**
     * @brief Visits a memcell containing table data.
     * 
     * @param memcell the memcell containing table data
     */
    virtual void    visit_table_memcell(TableMemcell* memcell) const = 0;
    /**
     * @brief Visits a memcell containing the address of a user
     * function as data.
     * 
     * @param memcell the memcell containing the address of a user function
     * as data.
     */
    virtual void    visit_userfunc_memcell(UserfuncMemcell* memcell) const = 0;
    /**
     * @brief Visits a memcell containing the id of a library
     * function as data.
     * 
     * @param memcell the memcell containing the id of a library function
     * as data.
     */
    virtual void    visit_libfunc_memcell(LibfuncMemcell* memcell) const = 0;
    /**
     * @brief Visits a memcell containing NIL data.
     * 
     * @param memcell the memcell containing NIL data
     */
    virtual void    visit_nill_memcell(NilMemcell* memcell) const = 0;
    /**
     * @brief Visits a memcell containing undefined data.
     * 
     * @param memcell the memcell containing undefined data
     */
    virtual void    visit_undef_memcell(UndefMemcell* memcell) const = 0;
};

}

#endif
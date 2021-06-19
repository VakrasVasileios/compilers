#ifndef AVM_MEMCELL_H
#define AVM_MEMCELL_H

#include <string>
#include <map>
#include <memory>
#include "../../../../../util/contract/include/contract.h"

namespace avm
{
    namespace memcell
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
        virtual void            accept(AvmMemcellVisitor* visitor) = 0;
        /**
         * @brief Returns a read access to a deep copy
         * of this AvmMemcell.
         * 
         * @return a read access to a deep copy
         * of this AvmMemcell, not null 
         */
        virtual AvmMemcell*     clone() const = 0;
        friend 
        std::ostream&           operator << (std::ostream& os, 
                                    const AvmMemcell& memcell);
        /**
         * @brief Equality operation between two AvmMemcells.
         * A comparison between two memcells of the same type
         * is reduced to a comparison between their values.
         * A comparison with a boolean type casts the other
         * operand to a boolean type. A comparison between two
         * heteroids, or with a nil/undefined type is an avm error.
         * 
         * @return wether the memcell operands are equal
         */
        bool                    operator==(AvmMemcell const&) const; 
        /**
         * @brief Inequality operation between two AvmMemcells.
         * A comparison between two memcells of the same type
         * is reduced to a comparison between their values.
         * A comparison with a boolean type casts the other
         * operand to a boolean type. A comparison between two
         * heteroids, or with a nil/undefined type is an avm error.
         * 
         * @return wether the memcell operands are not equal
         */                   
        bool                    operator!=(AvmMemcell const&) const;
        /**
         * @brief Greater than operation between two AvmMemcells.
         * A comparison between two memcells of the same type
         * is reduced to a comparison between their values.
         * A comparison with a boolean type casts the other
         * operand to a boolean type. A comparison between two
         * heteroids, or with a nil/undefined type is an avm error.
         * 
         * @return wether the first memcell operand is greater than
         * the second memcell operand
         */ 
        bool                    operator>(AvmMemcell const&) const;
        /**
         * @brief Greater or equal operation between two AvmMemcells.
         * A comparison between two memcells of the same type
         * is reduced to a comparison between their values.
         * A comparison with a boolean type casts the other
         * operand to a boolean type. A comparison between two
         * heteroids, or with a nil/undefined type is an avm error.
         * 
         * @return wether the first memcell operand is greater or equal
         * to the second memcell operand
         */ 
        bool                    operator>=(AvmMemcell const&) const;
        /**
         * @brief Less than operation between two AvmMemcells.
         * A comparison between two memcells of the same type
         * is reduced to a comparison between their values.
         * A comparison with a boolean type casts the other
         * operand to a boolean type. A comparison between two
         * heteroids, or with a nil/undefined type is an avm error.
         * 
         * @return wether the first memcell operand is less than
         * the second memcell operand
         */ 
        bool                    operator<(AvmMemcell const&) const;
        /**
         * @brief Less or equal operation between two AvmMemcells.
         * A comparison between two memcells of the same type
         * is reduced to a comparison between their values.
         * A comparison with a boolean type casts the other
         * operand to a boolean type. A comparison between two
         * heteroids, or with a nil/undefined type is an avm error.
         * 
         * @return wether the first memcell operand is less or equal
         * to the second memcell operand
         */ 
        bool                    operator<=(AvmMemcell const&) const;
        /**
         * @brief Addition arithmetic operation bewtween two AvmMemcells.
         * An arithmetic operation between two memcells is an avm error
         * if they are not of numeric type.
         * 
         * @return the result of the addition operation,
         * or this AvmMemcell in case of an avm error. 
         */
        AvmMemcell*             operator+(AvmMemcell const&) const;
        /**
         * @brief Subtractio arithmetic operation bewtween two AvmMemcells.
         * An arithmetic operation between two memcells is an avm error
         * if they are not of numeric type.
         * 
         * @return the result of the subtraction operation,
         * or this AvmMemcell in case of an avm error. 
         */
        AvmMemcell*             operator-(AvmMemcell const&) const;
        /**
         * @brief Multiplication arithmetic operation bewtween two AvmMemcells.
         * An arithmetic operation between two memcells is an avm error
         * if they are not of numeric type.
         * 
         * @return the result of the multiplication operation,
         * or this AvmMemcell in case of an avm error. 
         */
        AvmMemcell*             operator*(AvmMemcell const&) const;
        /**
         * @brief Division arithmetic operation bewtween two AvmMemcells.
         * An arithmetic operation between two memcells is an avm error
         * if they are not of numeric type.
         * 
         * @return the result of the division operation,
         * or this AvmMemcell in case of an avm error. 
         */
        AvmMemcell*             operator/(AvmMemcell const&) const;
        /**
         * @brief Modulation arithmetic operation bewtween two AvmMemcells.
         * An arithmetic operation between two memcells is an avm error
         * if they are not of numeric type.
         * 
         * @return the result of the modulation operation,
         * or this AvmMemcell in case of an avm error. 
         */
        AvmMemcell*             operator%(AvmMemcell const&) const;
        /**
         * @brief Converts this AvmMemcell to a boolean type.
         * 
         * @return a read access to this AvmMemcell as a
         * boolean type
         */
        virtual bool            to_bool() const = 0;  
        /**
         * @brief Returns a read access to the type of this
         * memcell AvmMemcell.
         * 
         * @return a read access to the type of this
         * memcell AvmMemcell 
         */
        virtual std::string     get_type() const = 0;                
    protected:
        /**
         * @brief Logs this instruction to an output.
         * 
         * @param os the output to log this AvmMemcell
         * @return the output 
         */
        virtual std::ostream&   log(std::ostream& os) const = 0;
        /**
         * @brief Checks wether this AvmMemcell is equal with another
         * memcell.
         * 
         * @param other the other memcell
         * @return wether this AvmMemcell is equal with another
         * memcell
         */
        virtual bool            eq(AvmMemcell const& other) const = 0;
         /**
         * @brief Checks wether this AvmMemcell is greater to another
         * memcell.
         * 
         * @param other the other memcell
         * @return wether this AvmMemcell is greater to another
         * memcell
         */
        virtual bool            gt(AvmMemcell const& other) const = 0;
         /**
         * @brief Checks wether this AvmMemcell is greater or equal to
         * another memcell.
         * 
         * @param other the other memcell
         * @return wether this AvmMemcell is greater or equal to
         * another memcell
         */
        virtual bool            geq(AvmMemcell const& other) const = 0;
        /**
         * @brief Checks wether this AvmMemcell is less than another
         * memcell.
         * 
         * @param other the other memcell
         * @return wether this AvmMemcell is less than another
         * memcell
         */
        virtual bool            lt(AvmMemcell const& other) const = 0;
        /**
         * @brief Checks wether this AvmMemcell is less or equal to
         * another memcell.
         * 
         * @param other the other memcell
         * @return wether this AvmMemcell is less or equal to
         * another memcell
         */
        virtual bool            leq(AvmMemcell const& other) const = 0;
        /**
         * @brief Adds this AvmMemcell with another memcell.
         * 
         * @param other the other memcell
         * @return the result of the arithmetic operation
         */
        virtual AvmMemcell*     add(AvmMemcell const& other) const = 0;
        /**
         * @brief Subtracts this AvmMemcell with another memcell.
         * 
         * @param other the other memcell
         * @return the result of the arithmetic operation
         */
        virtual AvmMemcell*     sub(AvmMemcell const& other) const = 0;
        /**
         * @brief Multiplies this AvmMemcell with another memcell.
         * 
         * @param other the other memcell
         * @return the result of the arithmetic operation
         */
        virtual AvmMemcell*     mul(AvmMemcell const& other) const = 0;
        /**
         * @brief Divides this AvmMemcell with another memcell.
         * 
         * @param other the other memcell
         * @return the result of the arithmetic operation
         */
        virtual AvmMemcell*     div(AvmMemcell const& other) const = 0;
        /**
         * @brief Mods this AvmMemcell with another memcell.
         * 
         * @param other the other memcell
         * @return the result of the arithmetic operation
         */
        virtual AvmMemcell*     mod(AvmMemcell const& other) const = 0;
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
        NumMemcell() = default;
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
        double          num_val() const;
        /**
         * @brief Sets the numerical data
         * of this NumMemcell.
         * 
         * @param _num_val the numerical value to set 
         * the numerical data of this NumMemcell
         */
        void            set_num_val(const double _num_val); 
        void            accept(AvmMemcellVisitor* visitor) override;
        AvmMemcell*     clone() const override;
        std::string     get_type() const override;
        bool            to_bool() const override;

    private:
        double          num_val_;
        std::ostream&   log(std::ostream& os) const override;
        bool            eq(AvmMemcell const& other) const override;
        bool            gt(AvmMemcell const& other) const override;
        bool            geq(AvmMemcell const& other) const override;
        bool            lt(AvmMemcell const& other) const override;
        bool            leq(AvmMemcell const& other) const override;
        AvmMemcell*     add(AvmMemcell const& other) const override;
        AvmMemcell*     sub(AvmMemcell const& other) const override;
        AvmMemcell*     mul(AvmMemcell const& other) const override;
        AvmMemcell*     div(AvmMemcell const& other) const override;
        AvmMemcell*     mod(AvmMemcell const& other) const override;
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
        std::string     str_val() const;
        /**
         * @brief Sets the string data of this StringMemCell.
         * 
         * @param str_val the string value to set the string
         * data of this StringMemCell
         */
        void            set_str_val(const std::string str_val);
        void            accept(AvmMemcellVisitor* visitor) override;
        AvmMemcell*     clone() const override;
        bool            to_bool() const override;
        std::string     get_type() const override;

        StringMemcell*  operator+(StringMemcell*);
    private:
        std::string     str_val_;
        std::ostream&   log(std::ostream& os) const override;
        bool            eq(AvmMemcell const& other) const override;
        bool            gt(AvmMemcell const& other) const override;
        bool            geq(AvmMemcell const& other) const override;
        bool            lt(AvmMemcell const& other) const override;
        bool            leq(AvmMemcell const& other) const override;
        AvmMemcell*     add(AvmMemcell const& other) const override;
        AvmMemcell*     sub(AvmMemcell const& other) const override;
        AvmMemcell*     mul(AvmMemcell const& other) const override;
        AvmMemcell*     div(AvmMemcell const& other) const override;
        AvmMemcell*     mod(AvmMemcell const& other) const override;
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
        BoolMemcell() = default;
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
        bool            bool_val() const;
        /**
         * @brief Sets the boolean data of this BoolMemcell.
         * 
         * @param val the boolean value to set the
         * boolean data of this BoolMemcell.
         */
        void            set_bool_val(const bool val);
        void            accept(AvmMemcellVisitor* visitor) override;
        AvmMemcell*     clone() const override;
        bool            to_bool() const override;
        std::string     get_type() const override;
    private:
        bool            bool_val_;
        std::ostream&   log(std::ostream& os) const override;
        bool            eq(AvmMemcell const& other) const override;
        bool            gt(AvmMemcell const& other) const override;
        bool            geq(AvmMemcell const& other) const override;
        bool            lt(AvmMemcell const& other) const override;
        bool            leq(AvmMemcell const& other) const override;
        AvmMemcell*     add(AvmMemcell const& other) const override;
        AvmMemcell*     sub(AvmMemcell const& other) const override;
        AvmMemcell*     mul(AvmMemcell const& other) const override;
        AvmMemcell*     div(AvmMemcell const& other) const override;
        AvmMemcell*     mod(AvmMemcell const& other) const override;
    };

    class Table;
    typedef std::shared_ptr<Table> tableref;

    /**
     * @brief Avm table data.
     * 
     */
    class Table final {
    public:
        /**
         * @brief Constructs a new Table object.
         * 
         */
        Table() = default;
        /**
         * @brief Destroys this Table object.
         * 
         */
        ~Table() = default;
        /**
         * @brief Checks wether this Table contains
         * a number type key.
         * 
         * @param _key the number type key
         * @return wether this Table contains
         * a number type key 
         */
        bool        contains_num_elem(double _key) const;
        /**
         * @brief Checks wether this Table contains
         * a string type key.
         * 
         * @param _key the string type key
         * @return wether this Table contains
         * a string type key 
         */
        bool        contains_str_elem(std::string _key) const;
         /**
         * @brief Checks wether this Table contains
         * a bool type key.
         * 
         * @param _key the bool type key
         * @return wether this Table contains
         * a bool type key 
         */
        bool        contains_bool_elem(bool _key) const;
        /**
         * @brief Checks wether this Table contains
         * a typeref type key.
         * 
         * @param _key the typeref type key, not null
         * @return wether this Table contains
         * a typeref type key 
         */
        bool        contains_tableref_elem(tableref _key) const;
         /**
         * @brief Checks wether this Table contains
         * a userfunc type key.
         * 
         * @param _key the userfunc type key
         * @return wether this Table contains
         * a userfunc type key 
         */
        bool        contains_userfunc_elem(unsigned _key) const;
        /**
         * @brief Checks wether this Table contains
         * a libfunc type key.
         * 
         * @param _key the libfunc type key
         * @return wether this Table contains
         * a libfunc type key 
         */
        bool        contains_libfunc_elem(std::string _key) const;
        /**
         * @brief Returns a read access to the mapped
         * memcell value of a double type key.
         * 
         * @param _key the double type key
         * @return a read access to the mapped
         * memcell value of a double type key
         */
        AvmMemcell* get_num_elem(double _key) const;
         /**
         * @brief Returns a read access to the mapped
         * memcell value of a string type key.
         * 
         * @param _key the string type key
         * @return a read access to the mapped
         * memcell value of a string type key
         */
        AvmMemcell* get_str_elem(std::string _key) const;
        /**
         * @brief Returns a read access to the mapped
         * memcell value of a bool type key.
         * 
         * @param _key the bool type key
         * @return a read access to the mapped
         * memcell value of a bool type key
         */
        AvmMemcell* get_bool_elem(bool _key) const;
        /**
         * @brief Returns a read access to the mapped
         * memcell value of a tableref type key.
         * 
         * @param _key the tableref type key
         * @return a read access to the mapped
         * memcell value of a tableref type key
         */
        AvmMemcell* get_tableref_elem(tableref _key) const;
        /**
         * @brief Returns a read access to the mapped
         * memcell value of a userfunc type key.
         * 
         * @param _key the userfunc type key
         * @return a read access to the mapped
         * memcell value of a userfunc type key
         */
        AvmMemcell* get_userfunc_elem(unsigned _key) const;
        /**
         * @brief Returns a read access to the mapped
         * memcell value of a libfunc type key.
         * 
         * @param _key the libfunc type key
         * @return a read access to the mapped
         * memcell value of a libfunc type key
         */
        AvmMemcell* get_libfunc_elem(std::string _key) const;
        /**
         * @brief Inserts a pair of a double type and a
         * memcell.
         * 
         * @param _key the double type key
         * @param _value the memcell value, not null
         */
        void        set_num_elem(double _key, AvmMemcell* _value);
         /**
         * @brief Inserts a pair of a string type and a
         * memcell.
         * 
         * @param _key the string type key
         * @param _value the memcell value, not null
         */
        void        set_str_elem(std::string _key, AvmMemcell* _value);
         /**
         * @brief Inserts a pair of a bool type and a
         * memcell.
         * 
         * @param _key the bool type key
         * @param _value the memcell value, not null
         */
        void        set_bool_elem(bool _key, AvmMemcell* _value);
         /**
         * @brief Inserts a pair of a tableref type and a
         * memcell.
         * 
         * @param _key the tableref type key, not null
         * @param _value the memcell value, not null
         */
        void        set_tableref_elem(tableref _key, AvmMemcell* _value);
        /**
         * @brief Inserts a pair of a userfunc type and a
         * memcell.
         * 
         * @param _key the userfunc type key
         * @param _value the memcell value, not null
         */
        void        set_userfunc_elem(unsigned _key, AvmMemcell* _value);
        /**
         * @brief Inserts a pair of a libfunc type and a
         * memcell.
         * 
         * @param _key the libfunc type key
         * @param _value the memcell value, not null
         */
        void        set_libfunc_elem(std::string _key, AvmMemcell* _value);
        friend 
        std::ostream&           operator << (std::ostream& os, 
                                    const Table& table);
    private:
        std::map<double, AvmMemcell*> num_map;
        std::map<std::string, AvmMemcell*> str_map;
        std::map<bool, AvmMemcell*> bool_map;
        std::map<tableref, AvmMemcell*> tableref_map;
        std::map<unsigned, AvmMemcell*> userfunc_map;
        std::map<std::string, AvmMemcell*> libfunc_map;

        bool    is_indexed() const;
        void    log_paired(std::ostream& os) const;
        void    log_indexed(std::ostream& os) const;
        void    log_num_map(std::ostream& os, bool &first_log) const;
        void    log_str_map(std::ostream& os, bool &first_log) const;
        void    log_bool_map(std::ostream& os, bool &first_log) const;
        void    log_table_map(std::ostream& os, bool &first_log) const;
        void    log_userfunc_map(std::ostream& os, bool &first_log) const;
        void    log_libfunc_map(std::ostream& os, bool &first_log) const;
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
         */
        TableMemcell() : table_val_(std::make_shared<Table>()) {}
        TableMemcell(const TableMemcell &obj) : table_val_(obj.table_val()) {} 
        /**
         * @brief Destroys this TableMemcell object.
         * 
         */
        ~TableMemcell() = default;
        /**
         * @brief Returns a read access to this TableMemcell
         * table value.
         * 
         * @return a read access to this TableMemcell
         * table value 
         */
        tableref        table_val() const;
        /**
         * @brief Checks wether this TableMemcell contains
         * a key memcell.
         * 
         * @param key the key memcell to be checked, not null
         * 
         * @return wether this TableMemcell contains
         * a key memcell 
         */
        bool             contains(AvmMemcell* key) const;
        /**
         * @brief Returns a read/write access to the memcell value
         * that is mapped with a key memcell at this TableMemcell.
         * 
         * @param key the key memcell, not null and must be mapped
         * at this TableMemcell 
         * 
         * @return a read/write access to the memcell value
         * that is mapped with a key memcell at this TableMemcell, 
         * not null 
         */
        AvmMemcell*      get_elem(AvmMemcell* key) const;
        /**
         * @brief Inserts a key/value memcell pair to this TableMemcell.
         * 
         * @param key the key memcell, not null
         * @param value the value memcell, not null
         */
        void            set_elem(AvmMemcell* key, AvmMemcell* value);
        void            accept(AvmMemcellVisitor* visitor) override;
        TableMemcell*   clone() const override;
        bool            to_bool() const override;
        std::string     get_type() const override;
    private:
        tableref        table_val_;
        std::ostream&   log(std::ostream& os) const override;     
        bool            eq(AvmMemcell const& other) const override;
        bool            gt(AvmMemcell const& other) const override;
        bool            geq(AvmMemcell const& other) const override;
        bool            lt(AvmMemcell const& other) const override;
        bool            leq(AvmMemcell const& other) const override;
        AvmMemcell*     add(AvmMemcell const& other) const override;
        AvmMemcell*     sub(AvmMemcell const& other) const override;
        AvmMemcell*     mul(AvmMemcell const& other) const override;
        AvmMemcell*     div(AvmMemcell const& other) const override;
        AvmMemcell*     mod(AvmMemcell const& other) const override;
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
        void            accept(AvmMemcellVisitor* visitor) override;  
        AvmMemcell*     clone() const override;
        bool            to_bool() const override;     
        std::string     get_type() const override;
    private:
        unsigned int    func_val_;
        std::ostream&   log(std::ostream& os) const override;
        bool            eq(AvmMemcell const& other) const override;
        bool            gt(AvmMemcell const& other) const override;
        bool            geq(AvmMemcell const& other) const override;
        bool            lt(AvmMemcell const& other) const override;
        bool            leq(AvmMemcell const& other) const override;
        AvmMemcell*     add(AvmMemcell const& other) const override;
        AvmMemcell*     sub(AvmMemcell const& other) const override;
        AvmMemcell*     mul(AvmMemcell const& other) const override;
        AvmMemcell*     div(AvmMemcell const& other) const override;
        AvmMemcell*     mod(AvmMemcell const& other) const override;
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
        std::string     lib_func_val() const;
        /**
         * @brief Sets the id of a library function as the data
         * of this LibfuncMemcell.
         * 
         * @param _lib_func_val the id of a library function to
         * be set as the data of this LibfuncMemcell
         */
        void            set_lib_func_val(const std::string _lib_func_val);
        void            accept(AvmMemcellVisitor* visitor) override;
        AvmMemcell*     clone() const override;
        bool            to_bool() const override;
        std::string     get_type() const override;
    private:
        std::string     lib_func_val_;
        std::ostream&   log(std::ostream& os) const override;
        bool            eq(AvmMemcell const& other) const override;
        bool            gt(AvmMemcell const& other) const override;
        bool            geq(AvmMemcell const& other) const override;
        bool            lt(AvmMemcell const& other) const override;
        bool            leq(AvmMemcell const& other) const override;
        AvmMemcell*     add(AvmMemcell const& other) const override;
        AvmMemcell*     sub(AvmMemcell const& other) const override;
        AvmMemcell*     mul(AvmMemcell const& other) const override;
        AvmMemcell*     div(AvmMemcell const& other) const override;
        AvmMemcell*     mod(AvmMemcell const& other) const override;     
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
        NilMemcell() = default;
        /**
         * @brief Destroys this NilMemcell object.
         * 
         */
        ~NilMemcell() = default;
        void            accept(AvmMemcellVisitor* visitor) override;
        AvmMemcell*     clone() const override;
        bool            to_bool() const override;
        std::string     get_type() const override;
    private:
        std::ostream&   log(std::ostream& os) const override;
        bool            eq(AvmMemcell const& other) const override;
        bool            gt(AvmMemcell const& other) const override;
        bool            geq(AvmMemcell const& other) const override;
        bool            lt(AvmMemcell const& other) const override;
        bool            leq(AvmMemcell const& other) const override;
        AvmMemcell*     add(AvmMemcell const& other) const override;
        AvmMemcell*     sub(AvmMemcell const& other) const override;
        AvmMemcell*     mul(AvmMemcell const& other) const override;
        AvmMemcell*     div(AvmMemcell const& other) const override;
        AvmMemcell*     mod(AvmMemcell const& other) const override;
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
        void            accept(AvmMemcellVisitor* visitor) override;
        AvmMemcell*     clone() const override;
        bool            to_bool() const override;
        std::string     get_type() const override;
    private:
        std::ostream&   log(std::ostream& os) const override;
        bool            eq(AvmMemcell const& other) const override;
        bool            gt(AvmMemcell const& other) const override;
        bool            geq(AvmMemcell const& other) const override;
        bool            lt(AvmMemcell const& other) const override;
        bool            leq(AvmMemcell const& other) const override;
        AvmMemcell*     add(AvmMemcell const& other) const override;
        AvmMemcell*     sub(AvmMemcell const& other) const override;
        AvmMemcell*     mul(AvmMemcell const& other) const override;
        AvmMemcell*     div(AvmMemcell const& other) const override;
        AvmMemcell*     mod(AvmMemcell const& other) const override;  
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
        virtual void    visit_num_memcell(NumMemcell* memcell) = 0;
        /**
         * @brief Visits a memcell containing string data.
         * 
         * @param memcell the memcell containing string data
         */
        virtual void    visit_string_memcell(StringMemcell* memcell) = 0;
        /**
         * @brief Visits a memcell containing bool data.
         * 
         * @param memcell the memcell containing bool data
         */
        virtual void    visit_bool_memcell(BoolMemcell* memcell) = 0;
        /**
         * @brief Visits a memcell containing table data.
         * 
         * @param memcell the memcell containing table data
         */
        virtual void    visit_table_memcell(TableMemcell* memcell) = 0;
        /**
         * @brief Visits a memcell containing the address of a user
         * function as data.
         * 
         * @param memcell the memcell containing the address of a user function
         * as data.
         */
        virtual void    visit_userfunc_memcell(UserfuncMemcell* memcell) = 0;
        /**
         * @brief Visits a memcell containing the id of a library
         * function as data.
         * 
         * @param memcell the memcell containing the id of a library function
         * as data.
         */
        virtual void    visit_libfunc_memcell(LibfuncMemcell* memcell) = 0;
        /**
         * @brief Visits a memcell containing NIL data.
         * 
         * @param memcell the memcell containing NIL data
         */
        virtual void    visit_nill_memcell(NilMemcell* memcell) = 0;
        /**
         * @brief Visits a memcell containing undefined data.
         * 
         * @param memcell the memcell containing undefined data
         */
        virtual void    visit_undef_memcell(UndefMemcell* memcell) = 0;
    };

    }
}

#endif
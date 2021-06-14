#ifndef VM_ARG_H
#define VM_ARG_H

#include <ostream>
#include "../../../util/contract/include/contract.h"

namespace target_code 
{
    
/**
 * @brief A virtual machine agument type.
 * 
 */
enum Vmarg_t : uint8_t {
    GLOBAL_a,
    LOCAL_a,
    FORMAL_a,
    BOOL_a,
    STRING_a,
    NUMBER_a,
    NIL_a,
    LIBFUNC_a,
    USERFUNC_a,
    LABEL_a,
    RETVAL_a
};

class VmargVisitor; // fwd declare

/**
 * @brief A virtual machine argument.
 * 
 */
class Vmarg {
public:    
    /**
     * @brief Constructs a new Vmarg object.
     * 
     */
    Vmarg() = default;
    /**
     * @brief Constructs a new Vmarg object.
     * 
     * @param _value the value of this Vmarg object,
     * greater or equal to zero
     */
    Vmarg(unsigned int _value) : value(VerifyValue(_value)) {};
    /**
     * @brief Destroys this Vmarg object.
     * 
     */
    virtual ~Vmarg() = default;
    /**
     * @brief Returns a read access to this Vmarg value.
     * 
     * @return a read access to this Vmarg value,
     * greater or equal to zero 
     */
    unsigned int        get_value() const;
    /**
     * @brief Sets this Vmarg value.
     * 
     * @param value the value to set this Vmarg
     * value, greater or equal to zero
     */
    void                set_value(const unsigned int value);
    /**
     * @brief Returns a read access to this Vmarg type.
     * 
     * @return a read access to this Vmarg type
     */
    virtual uint8_t     get_type() const = 0;
    /**
     * @brief Accepts a vmarg visitor at this Vmarg.
     * 
     * @param visitor the vmarg visitor to be accepted at this vmarg,
     * not null
     */
    virtual void        Accept(const VmargVisitor* visitor) = 0;

    friend std::ostream &operator<<(std::ostream &os, const Vmarg* vmarg);   
private:
    unsigned int        value;  
    unsigned int        VerifyValue(unsigned int value) const;  
};

/**
 * @brief A global vm argument.
 * 
 */
class GlobalVmarg final : public Vmarg {
public:
    /**
     * @brief Constructs a new Global vmarg object.
     * 
     * @param value the value of the new Global vmarg object,
     * greater or equal to zero 
     */
    GlobalVmarg(unsigned int value)
    : Vmarg(value) {INVARIANT(get_value() >= 0);}
    /**
     * @brief Constructs a new Global vmarg object.
     * 
     */
    GlobalVmarg() : Vmarg() {INVARIANT(get_value() >= 0);}
    /**
     * @brief Destroys this Global vmarg object.
     * 
     */
    ~GlobalVmarg() = default; 
    uint8_t     get_type() const override;   
    void        Accept(const VmargVisitor* visitor) override;
};

/**
 * @brief A local vmarg.
 * 
 */
class LocalVmarg final : public Vmarg {
public:
    /**
     * @brief Constructs a new Local vmarg object.
     * 
     * @param value the value of the new Local vmarg object,
     * greater or equal to zero 
     */
    LocalVmarg(unsigned int value) : Vmarg(value) {INVARIANT(get_value() >= 0);}
    /**
     * @brief Constructs a new Local vmarg object.
     * 
     */
    LocalVmarg() : Vmarg() {INVARIANT(get_value() >= 0);}
    /**
     * @brief Destroys this Local vmarg object
     * 
     */
    ~LocalVmarg() = default;    
    uint8_t     get_type() const override; 
    void        Accept(const VmargVisitor* visitor) override;
};

/**
 * @brief A formal vmarg.
 * 
 */
class FormalVmarg final : public Vmarg {
public:
    /**
     * @brief Constructs a new Formal vmarg object.
     * 
     * @param value the value of the new Formal vmarg object,
     * greater or equal to zero 
     */
    FormalVmarg(unsigned int value)
    : Vmarg(value) {INVARIANT(get_value() >= 0);}
    /**
     * @brief Constructs a new Formal vmarg object.
     * 
     */
    FormalVmarg() : Vmarg() {INVARIANT(get_value() >= 0);}
    /**
     * @brief Destroys this Formal vmarg object
     * 
     */
    ~FormalVmarg() = default;
    uint8_t     get_type() const override;
    void        Accept(const VmargVisitor* visitor) override;
};

/**
 * @brief A boolean vmarg.
 * 
 */
class BoolVmarg final : public Vmarg {
public:
    /**
     * @brief Constructs a new Bool vmarg object.
     * 
     * @param value the value of the new Bool vmarg object,
     * greater or equal to zero 
     */
    BoolVmarg(unsigned int value) : Vmarg(value) {INVARIANT(get_value() >= 0);}
    /**
     * @brief Constructs a new Bool vmarg object.
     * 
     */
    BoolVmarg() : Vmarg() {INVARIANT(get_value() >= 0);}
    /**
     * @brief Destroys this Bool vmarg object
     * 
     */
    ~BoolVmarg() = default;   
    uint8_t     get_type() const override; 
    void        Accept(const VmargVisitor* visitor) override;
};

/**
 * @brief A string vmarg.
 * 
 */
class StringVmarg final : public Vmarg {
public:
    /**
     * @brief Constructs a new String vmarg object.
     * 
     * @param value the value of the new String vmarg object,
     * greater or equal to zero 
     */
    StringVmarg(unsigned int value)
    : Vmarg(value) {INVARIANT(get_value() >= 0);}
    /**
     * @brief Constructs a new String vmarg object.
     * 
     */
    StringVmarg() : Vmarg() {INVARIANT(get_value() >= 0);}
    /**
     * @brief Destroys this String Vmarg object
     * 
     */
    ~StringVmarg() = default;  
    uint8_t     get_type() const override;
    void        Accept(const VmargVisitor* visitor) override;
};

/**
 * @brief A number vmarg. 
 * 
 */
class NumberVmarg final : public Vmarg {
public:
    /**
     * @brief Constructs a new Number vmarg object.
     * 
     * @param value the value of the new Number vmarg object,
     * greater or equal to zero 
     */
    NumberVmarg(unsigned int value)
    : Vmarg(value) {INVARIANT(get_value() >= 0);}
    /**
     * @brief Constructs a new Number vmarg object.
     * 
     */
    NumberVmarg() : Vmarg() {INVARIANT(get_value() >= 0);}
    /**
     * @brief Destroys this Number vmarg object
     * 
     */
    ~NumberVmarg() = default;
    uint8_t     get_type() const override;
    void        Accept(const VmargVisitor* visitor) override;
};

/**
 * @brief A NIL vmarg.
 * 
 */
class NilVmarg final : public Vmarg {
public:
    /**
     * @brief Constructs a new NIL vmarg object.
     * 
     * @param value the value of the new NIL vmarg object,
     * greater or equal to zero 
     */
    NilVmarg(unsigned int value)
    : Vmarg(value) {INVARIANT(get_value() >= 0);}
    /**
     * @brief Constructs a new NIL vmarg object.
     * 
     */
    NilVmarg() : Vmarg() {INVARIANT(get_value() >= 0);}
    /**
     * @brief Destroys this NIL vmarg object
     * 
     */
    ~NilVmarg() = default;
    uint8_t     get_type() const override;
    void        Accept(const VmargVisitor* visitor) override;
};

/**
 * @brief A library function vmarg.
 * 
 */
class LibFuncVmarg final : public Vmarg {
public:
    /**
     * @brief Constructs a new LibFunc vmarg object.
     * 
     * @param value the value of the new LibFunc vmarg object,
     * greater or equal to zero 
     */
    LibFuncVmarg(unsigned int value)
    : Vmarg(value) {INVARIANT(get_value() >= 0);}
    /**
     * @brief Constructs a new LibFunc vmarg object.
     * 
     */
    LibFuncVmarg() : Vmarg() {INVARIANT(get_value() >= 0);}
    /**
     * @brief Destroys this LibFunc vmarg object
     * 
     */
    ~LibFuncVmarg() = default;
    uint8_t     get_type() const override;
    void        Accept(const VmargVisitor* visitor) override;
};

/**
 * @brief A user function vmarg.
 * 
 */
class UserFuncVmarg final : public Vmarg {
public:
    /**
     * @brief Constructs a new UserFunc vmarg object.
     * 
     * @param value the value of the new UserFunc vmarg object,
     * greater or equal to zero 
     */
    UserFuncVmarg(unsigned int value)
    : Vmarg(value) {INVARIANT(get_value() >= 0);}
    /**
     * @brief Constructs a new UserFunc vmarg object.
     * 
     */
    UserFuncVmarg() : Vmarg() {INVARIANT(get_value() >= 0);}
    /**
     * @brief Destroys this UserFunc vmarg object
     * 
     */
    ~UserFuncVmarg() = default;
    uint8_t     get_type() const override;
    void        Accept(const VmargVisitor* visitor) override;
};

/**
 * @brief A label vmarg.
 * 
 */
class LabelVmarg final : public Vmarg {
public:
    /**
     * @brief Constructs a new Label vmarg object.
     * 
     * @param value the value of the new Global vmarg object,
     * greater or equal to zero 
     */
    LabelVmarg(unsigned int value)
    : Vmarg(value) {INVARIANT(get_value() >= 0);}
    /**
     * @brief Constructs a new Label vmarg object.
     * 
     */
    LabelVmarg() : Vmarg() {INVARIANT(get_value() >= 0);}
    /**
     * @brief Destroys this Label object
     * 
     */
    ~LabelVmarg() = default;
    uint8_t     get_type() const override;
    void        Accept(const VmargVisitor* visitor) override;
};

/**
 * @brief A retval vmarg.
 * 
 */
class RetValVmarg final : public Vmarg {
public:
    /**
     * @brief Constructs a new RetVal vmarg object.
     * 
     * @param value the value of the new RetVal vmarg object,
     * greater or equal to zero 
     */
    RetValVmarg(unsigned int value)
    : Vmarg(value) {INVARIANT(get_value() >= 0);}
    /**
     * @brief Constructs a new RetVal vmarg object.
     * 
     */
    RetValVmarg() : Vmarg() {INVARIANT(get_value() >= 0);}
    /**
     * @brief Destroys this RetVal vmarg object
     * 
     */
    ~RetValVmarg() = default;
    uint8_t     get_type() const override;
    void        Accept(const VmargVisitor* visitor) override;
};

/**
 * @brief An interface for visiting concrete vmargs.
 * 
 */
class VmargVisitor {
public:
    /**
     * @brief Visits a global vmarg.
     * 
     * @param arg the global vmarg to be visited, not null
     */
    virtual void VisitGlobalVmarg(GlobalVmarg* arg) const = 0;
    /**
     * @brief Visits a local vmarg.
     * 
     * @param arg the local vmarg to be visited, not null
     */
    virtual void VisitLocalVmarg(LocalVmarg* arg) const = 0;
    /**
     * @brief Visits a formal vmarg.
     * 
     * @param arg the formal vmarg to be visited, not null
     */
    virtual void VisitFormalVmarg(FormalVmarg* arg) const = 0;
    /**
     * @brief Visits a bool vmarg.
     * 
     * @param arg the bool vmarg to be visited, not null
     */
    virtual void VisitBoolVmarg(BoolVmarg* arg) const = 0;
    /**
     * @brief Visits a string vmarg.
     * 
     * @param arg the string vmarg to be visited, not null
     */
    virtual void VisitStringVmarg(StringVmarg* arg) const = 0;
    /**
     * @brief Visits a number vmarg.
     * 
     * @param arg the number vmarg to be visited, not null
     */
    virtual void VisitNumberVmarg(NumberVmarg* arg) const = 0;
    /**
     * @brief Visits a nil vmarg.
     * 
     * @param arg the nil vmarg to be visited, not null
     */
    virtual void VisitNilVmarg(NilVmarg* arg) const = 0;
    /**
     * @brief Visits a libfunc vmarg.
     * 
     * @param arg the libfunc vmarg to be visited, not null
     */
    virtual void VisitLibFuncVmarg(LibFuncVmarg* arg) const = 0;
    /**
     * @brief Visits a userfunc vmarg.
     * 
     * @param arg the userfunc vmarg to be visited, not null
     */
    virtual void VisitUserFuncVmarg(UserFuncVmarg* arg) const = 0;
    /**
     * @brief Visits a label vmarg.
     * 
     * @param arg the label vmarg to be visited, not null
     */
    virtual void VisitLabelVmarg(LabelVmarg* arg) const = 0;
    /**
     * @brief Visits a retval vmarg.
     * 
     * @param arg the retval vmarg to be visited, not null
     */
    virtual void VisitRetValVmarg(RetValVmarg* arg) const = 0;
};

}

#endif 
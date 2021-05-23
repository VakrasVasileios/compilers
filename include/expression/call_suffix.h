#ifndef CALL_SUFFIX_H
#define CALL_SUFFIX_H

#include "expression.h"
#include "elist.h"
#include <list>
#include <string>
#include "../../util/contract/contract.h"

/**
 * @brief The call suffix type.
 * 
 */
enum CallSuffixType {
    NORM_CALL,
    METHOD_CALL
};

/**
 * @brief A call suffix.
 * 
 */
class CallSuffix {
public:
    /**
     * @brief Returns a read access to this CallSuffix
     * type.
     * 
     * @return a read access to this CallSuffix
     * type
     */
    CallSuffixType get_type() const;
    /**
     * @brief Returns a read access to this CallSuffix
     * name.
     * 
     * @return a read access to this CallSuffix
     * name 
     */
    std::string get_name() const;
    /**
     * @brief Returns a read access to this CallSuffix
     * elist.
     * 
     * @return a read access to this CallSuffix
     * elist 
     */
    Elist*      get_elist() const; 
    /**
     * @brief Inserts an argument to this CallSuffix
     * elist.
     * 
     * @param arg the argument to be inserted to this 
     * CallSuffix elist, not null
     */
    void        InsertArg(Expression* arg);  
protected:
    /**
     * @brief Constructs a new CallSuffix object.
     * 
     * @param type the CallSuffix type
     * @param name the name of the CallSuffix
     * @param elist the elist of the CallSuffix
     */
    CallSuffix(CallSuffixType type, std::string name, Elist* elist)
    : name(name), type(type), elist(elist) {};
    /**
     * @brief Destroys this Call Suffix object.
     * 
     */
    ~CallSuffix() = default;
private:
    CallSuffixType          type;
    std::string             name;
    Elist*                  elist;
};

#endif 

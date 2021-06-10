#ifndef AVM_DATA_H
#define AVM_DATA_H

#include <string>

namespace virtual_machine
{
/**
 * @brief Avm abstract data.
 * 
 */
typedef struct avm_data {
    /**
     * @brief Constructs a new AvmData object.
     * 
     */
    avm_data() = default;
    /**
     * @brief Destroys this AvmData object.
     * 
     */
    virtual ~avm_data() = default;
} AvmData;

/**
 * @brief An avm table.
 * 
 */
typedef struct avm_table final : public AvmData { //TODO
    
} AvmTable;

/**
 * @brief Numerical avm data.
 * 
 */
typedef struct num_data final : public AvmData {
    /**
     * @brief Constructs a new NumData object.
     * 
     * @param _num_val the numeric value of the
     * new NumData object
     */
    num_data(double _num_val) : num_val(_num_val) {}
    /**
     * @brief Destroys this num data object.
     * 
     */
    ~num_data() = default;
    /**
     * @brief The numeric value of this NumData.
     * 
     */
    double num_val;
} NumData;

/**
 * @brief String avm data.
 * 
 */
typedef struct str_data final : public AvmData {
    /**
     * @brief Constructs a new StrData object.
     * 
     * @param _str_val the string value of the new
     * StrData object.
     */
    str_data(std::string _str_val) : str_val(_str_val) {}
    /**
     * @brief Destroys this StrData object.
     * 
     */
    ~str_data() = default;
    /**
     * @brief The string value of this StrData.
     * 
     */
    std::string str_val;
} StrData;

/**
 * @brief Boolean avm data.
 * 
 */
typedef struct bool_data final : public AvmData {
    /**
     * @brief Constructs a new BoolData object.
     * 
     * @param _bool_val the boolean value of the
     * new BoolData object.
     */
    bool_data(bool _bool_val) : bool_val(_bool_val) {}
    /**
     * @brief Destroys this BoolData object.
     * 
     */
    ~bool_data() = default;
    /**
     * @brief The boolean data of this BoolData.
     * 
     */
    bool bool_val;
} BoolData;

/**
 * @brief Avm user function address data.
 * 
 */
typedef struct func_data final : public AvmData {
    /**
     * @brief Constructs a new FuncData object.
     * 
     * @param _func_val the address of a user function
     * to be stored as this FuncData.
     */
    func_data(unsigned _func_val) : func_val(_func_val) {}
    /**
     * @brief Destroys this FuncData object.
     * 
     */
    ~func_data() = default;
    /**
     * @brief The address of a user function of
     * this FuncData.
     * 
     */
    unsigned func_val;
} FuncData;

/**
 * @brief Avm library function id data.
 * 
 */
struct lib_func_data final : public AvmData {
    /**
     * @brief Constructs a new LibFuncData object.
     * 
     * @param _lib_func_val the id of a library function
     * to be stored as this LibFuncData.
     */
    lib_func_data(std::string _lib_func_val) : lib_func_val(_lib_func_val) {}
    /**
     * @brief Destroys this LibFuncData object.
     * 
     */
    ~lib_func_data() = default;
    /**
     * @brief The id of a library function
     * that is stored as this LibFuncData.
     * 
     */
    std::string lib_func_val;
} LibFuncData;

}

#endif
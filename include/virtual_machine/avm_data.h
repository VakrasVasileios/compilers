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
typedef struct avm_table final : public AvmData {
    
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
    std::string str_val;
} StrData;

typedef struct bool_data final : public AvmData {
    unsigned char bool_val;
} BoolData;

struct table_data final : public AvmData {
    AvmTable* table_val;
} TableData;

typedef struct func_data final : public AvmData {
    unsigned func_val;
} FuncData;

struct lib_func_data final : public AvmData {
    std::string lib_func_val;
} LibFuncData;

}

#endif
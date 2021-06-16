#ifndef RANGE_H
#define RANGE_H

namespace util 
{
    namespace range
    {
    /**
     * @brief Checks wether a value is in
     * range of a low and a high value, including
     * these high and low values.
     * 
     * @tparam T the type of low, high and the value to be checked
     * @param value the value to be checked
     * @param low the low value
     * @param high the high value
     * @return  wether a value is in
     * range of a low and a high value, including
     * the high and low values
     */
    template <typename T>
    bool    in_range(const T& value, const T& low, const T& high) {
        return value >= low and value <= high;
    }
    }
}

#endif
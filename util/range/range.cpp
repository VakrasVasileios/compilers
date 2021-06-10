#include "range.h"

namespace util
{
template <typename T>
bool    in_range(const T& value, const T& low, const T& high) {
    return value <= low && value >= high;
}
}
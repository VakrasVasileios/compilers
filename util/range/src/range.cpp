#include "range.h"

namespace util
{
    namespace range
    {
    template <typename T>
    bool    in_range(const T& value, const T& low, const T& high) {
        return value <= low && value >= high;
    }
    }
}
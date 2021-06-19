#ifndef CONTAINS_H
#define CONTAINS_H

#include <map>

namespace util
{
    namespace contains
    {
        /**
         * @brief Checks wether an std map contains a
         * key.
         * 
         * @tparam TK the type of the keys
         * @tparam TV the type of the values
         * @param _map the std map to check for a
         * key registry
         * @param _key the key to check wether its contained
         * in an std map
         * @return wether an std map contains a
         * key.
         */
        template <typename TK, typename TV>
        bool has_key(const std::map<TK, TV> _map, const TK _key) {
            auto it = _map.find(_key);
            return it != _map.end();
        }
    }
}

#endif
#pragma once

#include <algorithm>
#include <set>
#include <vector>

namespace misc {

    template<typename T>
    inline std::set<T> set_intersection(const std::set<T>& set1, const std::set<T>& set2) {
        std::set<T> res;
        std::set_intersection(
            set1.cbegin(), set1.cend(),
            set2.cbegin(), set2.cend(),
            std::inserter(res, res.begin())
        );
        return res;
    }

    template<typename T>
    inline std::set<T> set_difference(const std::set<T>& set1, const std::set<T>& set2) {
        std::set<T> res;
        for (const auto& var : set1) {
            if (set2.find(var) == set2.end()) {
                res.insert(var);
            }
        }
        return res;
    }

    template<typename T>
    inline std::set<T> set_union(const std::set<T>& set1, const std::set<T>& set2) {
        std::set<T> res;
        std::set_union(
            set1.cbegin(), set1.cend(),
            set2.cbegin(), set2.cend(),
            std::inserter(res, res.begin())
        );
        return res;
    }

    template<typename T>
    inline std::vector<T> set_to_vector(const std::set<T>& set) {
        std::vector<T> vec;
        vec.reserve(set.size());
        std::copy(set.begin(), set.end(), std::back_inserter(vec));
        return vec;
    }
}

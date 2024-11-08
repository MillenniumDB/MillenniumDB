#pragma once

#include <cassert>
#include <type_traits>
#include <vector>

template<typename T, std::size_t N = 4096>
class Arena {
    static_assert(std::is_trivially_default_constructible<T>::value);
public:
    Arena() {
        blocks.push_back(new T[N]);
    }

    ~Arena() {
        for (auto block : blocks) {
            delete[](block);
        }
    }

    template<typename... Args>
    T* add(Args&&... args) {
        if (current_index >= N) {
            blocks.push_back(new T[N]);
            current_index = 0;
        }

        auto& res = blocks.back()[current_index];
        res = T(std::forward<Args>(args)...);
        current_index++;
        return &res;
    }

    void clear() {
        for (std::size_t i = 1; i < blocks.size(); i++) {
            delete[](blocks[i]);
        }
        blocks.resize(1);
        current_index = 0;
    }

private:
    std::vector<T*> blocks;
    std::size_t current_index = 0;
};

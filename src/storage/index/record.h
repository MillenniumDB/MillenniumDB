#pragma once

#include <array>
#include <cstdint>
#include <ostream>

template <std::size_t N>
using Record = std::array<uint64_t, N>;

template <std::size_t N>
std::ostream& operator<<(std::ostream& os, const Record<N>& record) {
    os << '(';
    for (size_t i = 0; i < N; i++) {
        os << ", " << std::to_string(record[i]);
    }
    return os << ')';
}

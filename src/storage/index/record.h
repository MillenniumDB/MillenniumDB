#pragma once

#include <array>
#include <cstdint>
#include <ostream>

template <std::size_t N> class Record {
public:
    std::array<uint64_t, N> ids;

    Record(std::array<uint64_t, N>&& ids) noexcept
        : ids(ids) { }

    Record(Record&& other) noexcept
        : ids(move(other.ids)) { }

    Record(const std::array<uint64_t, N>& ids) noexcept
        : ids(ids) { }

    Record(const Record& other) noexcept
        : ids(other.ids) { }

    ~Record() = default;

    inline void operator=(const Record<N>& other) noexcept {
        ids = other.ids;
    }

    inline void operator=(Record<N>&& other) noexcept {
        ids = move(other.ids);
    }

    inline bool operator<(const Record<N>& other) const noexcept {
        for (size_t i = 0; i < N; i++) {
            if (ids[i] < other.ids[i]) {
                return true;
            } else if (ids[i] > other.ids[i]) {
                return false;
            }
        }
        return false;
    }

    inline bool operator<=(const Record<N>& other) const noexcept {
        for (size_t i = 0; i < N; i++) {
            if (ids[i] < other.ids[i]) {
                return true;
            } else if (ids[i] > other.ids[i]) {
                return false;
            }
        }
        return true;
    }

    inline uint64_t operator[](uint_fast32_t i) const noexcept {
        return ids[i];
    }

    friend std::ostream& operator<<(std::ostream& os, const Record<N>& record) {
        os << '(';
        for (size_t i = 0; i < N; i++) {
            os << ", " << std::to_string(record.ids[i]);
        }
        return os << ')';
    }
};

class RecordFactory {
public:
    inline static Record<1> get(uint64_t a1) noexcept {
        return Record<1>(std::array<uint64_t, 1> { a1 });
    }

    inline static Record<2> get(uint64_t a1, uint64_t a2) noexcept {
        return Record<2>(std::array<uint64_t, 2> { a1, a2 });
    }

    inline static Record<3> get(uint64_t a1, uint64_t a2, uint64_t a3) noexcept {
        return Record<3>(std::array<uint64_t, 3> { a1, a2, a3 });
    }

    inline static Record<4> get(uint64_t a1, uint64_t a2, uint64_t a3, uint64_t a4) noexcept {
        return Record<4>(std::array<uint64_t, 4> { a1, a2, a3, a4 });
    }
};

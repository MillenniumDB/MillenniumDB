#ifndef STORAGE__RECORD_H_
#define STORAGE__RECORD_H_

#include <array>
#include <cstdint>
#include <ostream>

template <std::size_t N>
class Record {
public:
    std::array<uint64_t, N> ids;

    Record(const std::array<uint64_t, N> ids) noexcept :
        ids(ids) { }

    Record(const Record& other) noexcept :
        ids(other.ids) { }

    ~Record() = default;

    inline void operator=(const Record<N>& other) noexcept {
        ids = other.ids;
    }

    inline bool operator<(const Record<N>& other) const noexcept {
        for (size_t i = 0; i < N; i++) {
            if (ids[i] < other.ids[i]) {
                return true;
            } else if (ids[i] > other.ids[i]){
                return false;
            }
        }
        return false;
    }

    inline bool operator<=(const Record<N>& other) const noexcept {
        for (size_t i = 0; i < N; i++) {
            if (ids[i] < other.ids[i]) {
                return true;
            } else if (ids[i] > other.ids[i]){
                return false;
            }
        }
        return true;
    }

    inline uint64_t operator[](const uint_fast32_t i) const noexcept {
        return ids[i];
    }

    // template <std::size_t M>
    // friend std::ostream& operator<< (std::ostream& os, const Record<N>& record);
    // template <std::size_t M>
    friend std::ostream& operator<<(std::ostream& os, const Record<N>& record) {
        std::string res = '(' + std::to_string(record.ids[0]); // Suposing record size > 0
        for (size_t i = 1; i < N; i++) {
            res += ", " + std::to_string(record.ids[i]);
        }
        res += ')';
        return os << res;
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

#endif // STORAGE__RECORD_H_

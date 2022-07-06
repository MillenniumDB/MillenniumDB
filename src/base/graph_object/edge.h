#pragma once

class Edge {
public:
    uint64_t id;

    Edge(uint64_t id) : id(id) { }

    inline bool operator==(const Edge& rhs) const noexcept {
        return this->id == rhs.id;
    }

    inline bool operator!=(const Edge& rhs) const noexcept {
        return this->id != rhs.id;
    }

    inline bool operator<=(const Edge& rhs) const noexcept {
        return this->id <= rhs.id;
    }

    inline bool operator>=(const Edge& rhs) const noexcept {
        return this->id >= rhs.id;
    }

    inline bool operator<(const Edge& rhs) const noexcept {
        return this->id < rhs.id;
    }

    inline bool operator>(const Edge& rhs) const noexcept {
        return this->id > rhs.id;
    }
};

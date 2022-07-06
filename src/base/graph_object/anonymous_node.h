#pragma once

class AnonymousNode {
public:
    uint64_t id;

    AnonymousNode(uint64_t id) : id(id) { }

    inline bool operator==(const AnonymousNode& rhs) const noexcept {
        return this->id == rhs.id;
    }

    inline bool operator!=(const AnonymousNode& rhs) const noexcept {
        return this->id != rhs.id;
    }

    inline bool operator<=(const AnonymousNode& rhs) const noexcept {
        return this->id <= rhs.id;
    }

    inline bool operator>=(const AnonymousNode& rhs) const noexcept {
        return this->id >= rhs.id;
    }

    inline bool operator<(const AnonymousNode& rhs) const noexcept {
        return this->id < rhs.id;
    }

    inline bool operator>(const AnonymousNode& rhs) const noexcept {
        return this->id > rhs.id;
    }
};

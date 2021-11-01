#ifndef BASE__EDGE_H_
#define BASE__EDGE_H_

class Edge {
public:
    uint64_t id;

    Edge(const uint64_t id)
        : id(id) { }
    ~Edge() = default;

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

#endif // BASE__EDGE_H_

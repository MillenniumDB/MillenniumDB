#pragma once

#include <ostream>
#include <string>

/*
Used to represent a NamedNode when processing the query string.
Query results won't use NamedNodes.
This class is necessary because the query may have external strings that doesn't exist in the graph.
*/
class NamedNode {
public:
    std::string name;

    explicit NamedNode(std::string&& name) : name(std::move(name)) { }

    explicit NamedNode(const std::string& name) : name(name) { }

    inline bool operator<(const NamedNode& rhs) const noexcept {
        return name < rhs.name;
    }

    inline bool operator<=(const NamedNode& rhs) const noexcept {
        return name <= rhs.name;
    }

    inline bool operator>(const NamedNode& rhs) const noexcept {
        return name > rhs.name;
    }

    inline bool operator>=(const NamedNode& rhs) const noexcept {
        return name >= rhs.name;
    }

    inline bool operator==(const NamedNode& rhs) const noexcept {
        return name == rhs.name;
    }

    inline bool operator!=(const NamedNode& rhs) const noexcept {
        return name != rhs.name;
    }

    friend std::ostream& operator<<(std::ostream& os, const NamedNode& node_name) {
        return os << node_name.name;
    }
};

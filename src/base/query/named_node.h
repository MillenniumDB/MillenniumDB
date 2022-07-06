#pragma once

#include <ostream>
#include <string>

/*
Used to represent a NamedNode when processing the query string. Query results
won't use NamedNodes, but NamedNodeExternal and NamedNodeInlined instead.
This class is necessary because NamedNodeExternal cannot be propertly used
when the query has a string that doesn't exist in the graph.
TODO: this may change if operations with strings are allowed in the
      query language, as they may produce strings that doesn't exist in the
      graph. Maybe NamedNode should be a GraphObject
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

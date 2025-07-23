#pragma once

#include <string>

#include "query/id.h"
#include "query/parser/op/mql/op.h"

namespace MQL {

class Property {
public:
    Id node; // TODO: rename to obj?
    ObjectId key;
    ObjectId value;

    Property(Id obj, ObjectId key, ObjectId value) :
        node(obj),
        key(key),
        value(value)
    { }

    // Only comparing node and key, not checking value
    bool operator<(const Property& other) const
    {
        if (node < other.node) {
            return true;
        } else if (other.node < node) {
            return false;
        } else if (key < other.key) {
            return true;
        } else if (key > other.key) {
            return false;
        } else {
            return value < other.value;
        }
    }

    std::ostream& print(std::ostream& os, int indent = 0) const
    {
        os << std::string(indent, ' ');
        os << "Property(" << node << ", " << key << ", " << value << ")\n";
        return os;
    }
};
} // namespace MQL

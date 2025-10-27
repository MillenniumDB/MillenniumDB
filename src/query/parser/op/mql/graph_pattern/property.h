#pragma once

#include <string>

#include "query/id.h"

namespace MQL {

class Property {
public:
    Id obj;
    ObjectId key;
    ObjectId value;

    Property(Id obj, ObjectId key, ObjectId value) :
        obj(obj),
        key(key),
        value(value)
    { }

    // Only comparing obj and key, not checking value
    bool operator<(const Property& other) const
    {
        if (obj < other.obj) {
            return true;
        } else if (other.obj < obj) {
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
        os << "Property(" << obj << ", " << key << ", " << value << ")\n";
        return os;
    }
};
} // namespace MQL

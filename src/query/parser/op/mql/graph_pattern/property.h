#pragma once

#include <string>

#include "query/id.h"

namespace MQL {

class Property {
public:
    Id obj;
    ObjectId key;
    Id value;

    Property(Id obj, ObjectId key, Id value) :
        obj(obj),
        key(key),
        value(value)
    { }

    bool operator<(const Property& other) const
    {
        if (obj != other.obj)
            return obj < other.obj;
        if (key != other.key)
            return key < other.key;
        else
            return value < other.value;
    }

    std::ostream& print(std::ostream& os, int indent = 0) const
    {
        os << std::string(indent, ' ');
        os << "Property(" << obj << ", " << key << ", " << value << ")\n";
        return os;
    }
};
} // namespace MQL

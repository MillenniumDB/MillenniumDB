#pragma once

#include "query/id.h"

namespace MQL {

class Label {
public:
    Id node;

    ObjectId label;

    Label(Id node, ObjectId label) :
        node(node),
        label(label)
    { }

    bool operator<(const Label& other) const
    {
        if (node < other.node) {
            return true;
        } else if (other.node < node) {
            return false;
        } else {
            return label < other.label;
        }
    }

    std::ostream& print(std::ostream& os, int indent = 0) const
    {
        os << std::string(indent, ' ');
        os << "Label(";
        os << node;
        os << ", ";
        os << label;
        os << ")\n";
        return os;
    }
};
} // namespace MQL

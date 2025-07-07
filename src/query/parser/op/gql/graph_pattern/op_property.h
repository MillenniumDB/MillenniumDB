#pragma once

#include "query/parser/op/gql/op.h"

namespace GQL {

struct PropertyValue {
    VarId object;
    ObjectId key;
    VarId var_with_property;
    ObjectId value;
    VarType::Type type;
};

class OpProperty : public Op {
public:
    PropertyValue property;

    OpProperty(PropertyValue property) :
        property(property)
    { }

    std::unique_ptr<Op> clone() const override
    {
        return std::make_unique<OpProperty>(property);
    }

    void accept_visitor(OpVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override
    {
        return { property.object };
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override
    {
        os << std::string(indent, ' ');
        os << "OpProperty(";

        switch (property.type) {
        case VarType::Node:
            os << "node ";
            break;
        case VarType::Edge:
            os << "edge ";
            break;
        default:
            break;
        }
        os << property.object << "." << property.key << "=" << property.value << ")\n";
        return os;
    }
};
} // namespace GQL

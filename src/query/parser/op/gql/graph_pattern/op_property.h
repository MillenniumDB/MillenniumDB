#pragma once

#include <memory>

#include "query/parser/op/gql/graph_pattern/op_optional_properties.h"
#include "query/parser/op/op.h"
#include "query/rewriter/gql/op/var_type.h"

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

    std::set<VarId> get_scope_vars() const override
    {
        return get_all_vars();
    }

    std::set<VarId> get_safe_vars() const override
    {
        return { property.object };
    }

    std::set<VarId> get_fixable_vars() const override
    {
        return get_all_vars();
    }

    std::map<VarId, std::unique_ptr<GQL::VarType>> get_var_types() const override
    {
        return {};
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

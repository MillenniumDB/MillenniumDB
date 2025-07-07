#pragma once

#include "query/parser/op/gql/op.h"

namespace GQL {
struct OptPropertyValue {
    VarId object;
    ObjectId key;
    VarId value;
    VarType::Type type;

    OptPropertyValue(VarId object, ObjectId key, VarId value, VarType::Type type) :
        object(object),
        key(key),
        value(value),
        type(type)
    { }

    bool operator==(const OptPropertyValue& other) const
    {
        return object == other.object && key == other.key && value == other.value && type == other.type;
    }
};

class OpOptProperties : public Op {
public:
    std::unique_ptr<Op> op;
    std::vector<OptPropertyValue> properties;

    OpOptProperties(std::unique_ptr<Op>&& op, std::vector<OptPropertyValue>& properties) :
        op(std::move(op)),
        properties(properties)
    { }

    std::unique_ptr<Op> clone() const override
    {
        auto new_properties = properties;
        return std::make_unique<OpOptProperties>(op->clone(), new_properties);
    }

    void accept_visitor(OpVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override
    {
        std::set<VarId> res;

        for (auto& property : properties) {
            res.insert(property.object);
            res.insert(property.value);
        }
        res.merge(op->get_all_vars());
        return res;
    }

    std::map<VarId, GQL::VarType> get_var_types() const override
    {
        // TODO: include expr variables
        return op->get_var_types();
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override
    {
        os << std::string(indent, ' ');
        os << "OpOptProperties((";

        auto it = properties.begin();
        os << it->object << ", " << it->key << ")";
        it++;

        for (; it != properties.end(); ++it) {
            os << ", (" << it->object << "," << it->key << "," << it->value << ")";
        }
        os << ")\n";
        op->print_to_ostream(os, indent + 2);
        return os;
    }
};
} // namespace GQL

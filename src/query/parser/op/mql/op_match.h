#pragma once

#include <memory>

#include "query/parser/op/op.h"

namespace MQL {

enum class PropertyType : uint64_t {
    TYPE_NULL = 1 << 4, // 10000
    TYPE_STRING = 1 << 3, // 01000
    TYPE_INTEGER = 1 << 2, // 00100
    TYPE_BOOL = 1 << 1, // 00010
    TYPE_FLOAT = 1 << 0, // 00001
};


enum class OperatorType {
    EQUALS,
    NOT_EQUALS,
    LESS,
    GREATER,
    LESS_OR_EQUALS,
    GREATER_OR_EQUALS,
};

class PropertyTypeConstraint {
public:
    VarId var_without_property;
    ObjectId key;
    VarId var_with_property;
    uint64_t type_bitmap;

    PropertyTypeConstraint(VarId v, ObjectId k, VarId val, uint64_t bitmap = 0) :
        var_without_property(v),
        key(k),
        var_with_property(val),
        type_bitmap(bitmap)
    { }
};

class PropertyOperatorConstraint {
public:
    VarId var;
    ObjectId key;
};

class OpMatch : public Op {
public:
    // graph pattern
    std::unique_ptr<Op> op;

    std::vector<OpProperty> optional_properties;

    // var, key, value, value_var
    std::vector<std::tuple<VarId, ObjectId, ObjectId, VarId>> fixed_properties;

    OpMatch(std::unique_ptr<Op> op, std::vector<OpProperty>&& optional_properties) :
        op(std::move(op)),
        optional_properties(optional_properties)
    { }

    OpMatch(
        std::unique_ptr<Op> op,
        std::vector<OpProperty>&& optional_properties,
        std::vector<std::tuple<VarId, ObjectId, ObjectId, VarId>>&& fixed_properties
    ) :
        op(std::move(op)),
        optional_properties(optional_properties),
        fixed_properties(fixed_properties)
    { }

    virtual std::unique_ptr<Op> clone() const override
    {
        std::vector<OpProperty> optional_properties_clone = optional_properties;
        std::vector<std::tuple<VarId, ObjectId, ObjectId, VarId>> fixed_properties_clone = fixed_properties;
        return std::make_unique<OpMatch>(
            op->clone(),
            std::move(optional_properties_clone),
            std::move(fixed_properties_clone)
        );
    }

    void accept_visitor(OpVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override
    {
        return op->get_all_vars();
    }

    std::set<VarId> get_scope_vars() const override
    {
        return op->get_scope_vars();
    }

    std::set<VarId> get_safe_vars() const override
    {
        return op->get_safe_vars();
    }

    std::set<VarId> get_fixable_vars() const override
    {
        return op->get_fixable_vars();
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override
    {
        os << std::string(indent, ' ');
        os << "OpMatch()\n";
        return op->print_to_ostream(os, indent + 2);
    }
};
} // namespace MQL

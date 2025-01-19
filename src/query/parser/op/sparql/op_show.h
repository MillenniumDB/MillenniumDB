#pragma once

#include "query/parser/op/op.h"

namespace SPARQL {

class OpShow : public Op {
public:
    enum Type : uint8_t {
        TEXT_SEARCH_INDEX,
    };

    Type type;

    OpShow(Type type_) :
        type(type_)
    { }

    virtual std::unique_ptr<Op> clone() const override
    {
        return std::make_unique<OpShow>(*this);
    }

    void accept_visitor(OpVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override
    {
        return {};
    }

    std::set<VarId> get_scope_vars() const override
    {
        return {};
    }

    std::set<VarId> get_safe_vars() const override
    {
        return {};
    }

    std::set<VarId> get_fixable_vars() const override
    {
        return {};
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override
    {
        os << std::string(indent, ' ');
        return os << "OpShow(" << type << ")\n";
    }
};
} // namespace SPARQL

inline std::ostream& operator<<(std::ostream& os, const SPARQL::OpShow::Type& type)
{
    switch (type) {
    case SPARQL::OpShow::Type::TEXT_SEARCH_INDEX:
        return os << "TEXT_SEARCH_INDEX";
    default:
        return os << "UNKNOWN_SHOW_TYPE";
    }
}

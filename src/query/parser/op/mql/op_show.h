#pragma once

#include "graph_models/object_id.h"
#include "query/parser/op/op.h"

namespace MQL {

class OpShow : public Op {
public:
    enum class Type : uint8_t {
        TENSOR_STORE,
        TEXT_SEARCH_INDEX,
    };

    static std::string get_type_string(Type type)
    {
        switch (type) {
        case Type::TENSOR_STORE:
            return "TENSOR_STORE";
        case Type::TEXT_SEARCH_INDEX:
            return "TEXT_SEARCH_INDEX";
        default:
            return "UNKNOWN";
        }
    }

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
        os << "OpShow(";
        os << OpShow::get_type_string(type);
        return os << ")\n";
    }
};

} // namespace MQL

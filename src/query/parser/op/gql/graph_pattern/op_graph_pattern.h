#pragma once

#include "query/parser/op/gql/graph_pattern/path_mode.h"
#include "query/parser/op/gql/op.h"

namespace GQL {

class OpGraphPattern : public Op {
public:
    std::unique_ptr<Op> op;

    PathMode mode;
    std::optional<VarId> path_var_id;

    OpGraphPattern(std::unique_ptr<Op>&& op, PathMode mode, VarId var_id) :
        op(std::move(op)),
        mode(mode),
        path_var_id(var_id)
    { }

    OpGraphPattern(std::unique_ptr<Op>&& op, PathMode mode, std::optional<VarId> var_id) :
        op(std::move(op)),
        mode(mode),
        path_var_id(var_id)
    { }

    OpGraphPattern(std::unique_ptr<Op>&& op, PathMode mode) :
        op(std::move(op)),
        mode(mode),
        path_var_id(std::nullopt)
    { }

    std::unique_ptr<Op> clone() const override
    {
        return std::make_unique<OpGraphPattern>(op->clone(), mode, path_var_id.value());
    }

    void accept_visitor(OpVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override
    {
        return op->get_all_vars();
    }

    std::map<VarId, GQL::VarType> get_var_types() const override
    {
        auto result = op->get_var_types();

        if (!path_var_id.has_value()) {
            return result;
        }

        if (result.count(*path_var_id) && result[*path_var_id].type != VarType::Path) {
            throw QuerySemanticException(
                "Variable \"" + get_query_ctx().get_var_name(path_var_id.value())
                + "\" has more than one type."
            );
        }

        result[*path_var_id] = VarType(VarType::Path);
        return result;
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override
    {
        os << std::string(indent, ' ');
        os << "OpGraphPattern(";
        if (path_var_id.has_value()) {
            os << path_var_id.value() << ", ";
        }
        mode.print_to_ostream(os);
        os << ")\n";

        op->print_to_ostream(os, indent + 2);
        return os;
    }
};
} // namespace GQL

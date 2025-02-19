#pragma once

#include <memory>

#include "query/parser/op/op.h"
#include "query/rewriter/gql/op/var_type.h"

namespace GQL {
struct OptLabel {
    VarId object;
    VarId labels_var;
    VarType::Type type;

    OptLabel(VarId object, VarId labels_var, VarType::Type type) :
        object(object),
        labels_var(labels_var),
        type(type)
    { }

    bool operator<(const OptLabel& other) const
    {
        return object < other.object;
    }
};

class OpOptLabels : public Op {
public:
    std::unique_ptr<Op> op;
    std::set<OptLabel> labels;

    OpOptLabels(std::unique_ptr<Op> op, std::set<OptLabel>&& labels) :
        op(std::move(op)),
        labels(std::move(labels))
    { }

    std::unique_ptr<Op> clone() const override
    {
        std::set<OptLabel> clone_labels = labels;
        return std::make_unique<OpOptLabels>(op->clone(), std::move(clone_labels));
    }

    void accept_visitor(OpVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override
    {
        std::set<VarId> res;

        for (auto& label : labels) {
            res.insert(label.object);
        }
        return res;
    }

    std::set<VarId> get_scope_vars() const override
    {
        return get_all_vars();
    }

    std::set<VarId> get_safe_vars() const override
    {
        return op->get_safe_vars();
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
        os << "OpOptLabels(";

        auto it = labels.begin();
        os << it->labels_var;
        it++;

        for (; it != labels.end(); ++it) {
            os << ", " << it->labels_var;
        }
        os << ")\n";
        op->print_to_ostream(os, indent + 1);
        return os;
    }
};
} // namespace GQL

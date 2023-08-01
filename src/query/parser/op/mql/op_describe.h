#pragma once

#include "graph_models/object_id.h"
#include "query/parser/op/op.h"

namespace MQL {

class OpDescribe : public Op {
public:
    ObjectId node;

    uint64_t labels_limit;
    uint64_t properties_limit;
    uint64_t outgoing_limit;
    uint64_t incoming_limit;

    OpDescribe(
        ObjectId node,
        uint64_t labels_limit,
        uint64_t properties_limit,
        uint64_t outgoing_limit,
        uint64_t incoming_limit
    ) :
        node             (node),
        labels_limit     (labels_limit),
        properties_limit (properties_limit),
        outgoing_limit   (outgoing_limit),
        incoming_limit   (incoming_limit) { }

    virtual std::unique_ptr<Op> clone() const override {
        return std::make_unique<OpDescribe>(*this);
    }

    void accept_visitor(OpVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override {
        return { };
    }

    std::set<VarId> get_scope_vars() const override {
        return { };
    }

    std::set<VarId> get_safe_vars() const override {
        return { };
    }

    std::set<VarId> get_fixable_vars() const override {
        return { };
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        os << std::string(indent, ' ');
        os << "OpDescribe(";
        if (labels_limit > 0) {
            os << "LABELS ";
            if (labels_limit != DEFAULT_LIMIT)
                os << "(LIMIT " << labels_limit << "), ";
        }
        if (properties_limit > 0){
            os << "PROPERTIES ";
            if (properties_limit != DEFAULT_LIMIT)
                os << "(LIMIT " << properties_limit << "), ";
        }
        if (outgoing_limit > 0){
            os << "OUTGOING ";
            if (outgoing_limit != DEFAULT_LIMIT)
                os << "(LIMIT " << outgoing_limit << "), ";
        }
        if (incoming_limit > 0){
            os << "INCOMING ";
            if (incoming_limit != DEFAULT_LIMIT)
                os << "(LIMIT " << incoming_limit << "), ";
        }
        os << node;
        os << ")\n";
        return os;
    }
};
} // namespace MQL

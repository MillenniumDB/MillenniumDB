#pragma once

#include "graph_models/object_id.h"
#include "query/parser/op/op.h"

namespace MQL {
class OpDeleteTensors : public Op {
public:
    const std::string tensor_store_name;
    const std::vector<ObjectId> deletes;

    OpDeleteTensors(std::string&& tensor_store_name, std::vector<ObjectId>&& deletes_) :
        tensor_store_name { std::move(tensor_store_name) },
        deletes { std::move(deletes_) }
    { }

    virtual std::unique_ptr<Op> clone() const override
    {
        auto tensor_store_name_clone = tensor_store_name;
        auto deletes_clone = deletes;
        return std::make_unique<OpDeleteTensors>(
            std::move(tensor_store_name_clone),
            std::move(deletes_clone)
        );
    }

    bool read_only() const override
    {
        return false;
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
        os << "OpDeleteTensors(tensor_store_name: " << tensor_store_name
           << ", deletes.size(): " << deletes.size() << ")\n";
        return os;
    }
};
} // namespace MQL

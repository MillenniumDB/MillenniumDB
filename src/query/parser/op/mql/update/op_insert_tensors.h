#pragma once

#include "query/parser/op/op.h"

namespace MQL {
class OpInsertTensors : public Op {
public:
    const std::string tensor_store_name;
    const std::vector<std::tuple<ObjectId, std::vector<float>>> inserts;

    OpInsertTensors(
        std::string&& tensor_store_name,
        std::vector<std::tuple<ObjectId, std::vector<float>>>&& inserts
    ) :
        tensor_store_name { std::move(tensor_store_name) },
        inserts { std::move(inserts) }
    { }

    virtual std::unique_ptr<Op> clone() const override
    {
        auto tensor_store_name_clone = tensor_store_name;
        auto inserts_clone = inserts;
        return std::make_unique<OpInsertTensors>(
            std::move(tensor_store_name_clone),
            std::move(inserts_clone)
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
        os << "OpInsertTensors(tensor_store_name: " << tensor_store_name << ", inserts.size(): " << inserts.size() << ")\n";
        return os;
    }
};
} // namespace MQL

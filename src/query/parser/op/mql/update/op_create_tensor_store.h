#pragma once

#include "query/parser/op/op.h"

namespace MQL {
class OpCreateTensorStore : public Op {
public:
    const std::string tensor_store_name;
    const uint64_t tensors_dim;

    OpCreateTensorStore(std::string&& tensor_store_name_, uint64_t tensors_dim_) :
        tensor_store_name { std::move(tensor_store_name_) },
        tensors_dim { tensors_dim_ }
    { }

    std::unique_ptr<Op> clone() const override
    {
        auto tensor_store_name_clone = tensor_store_name;
        return std::make_unique<OpCreateTensorStore>(std::move(tensor_store_name_clone), tensors_dim);
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
        os << "OpCreateTensorStore(tensor_store_name: " << tensor_store_name
           << ", tensors_dim: " << tensors_dim << ")\n";
        return os;
    }
};
} // namespace MQL

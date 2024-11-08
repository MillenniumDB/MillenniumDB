#pragma once

#include "query/executor/binding_iter.h"

namespace LSH {
class ForestIndexQueryIter;

class ForestIndexTopAll : public BindingIter {
public:
    const VarId object_var;
    const VarId similarity_var;

    ForestIndexTopAll(VarId object_var, VarId similarity_var, std::unique_ptr<ForestIndexQueryIter> query_iter);

    void accept_visitor(BindingIterVisitor& visitor) override;
    void _begin(Binding& parent_binding) override;
    bool _next() override;
    void _reset() override;
    void assign_nulls() override;

private:
    std::unique_ptr<LSH::ForestIndexQueryIter> query_iter;

    Binding* parent_binding;
};
} // namespace LSH
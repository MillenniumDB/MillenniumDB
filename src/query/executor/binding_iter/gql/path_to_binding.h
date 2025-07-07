#pragma once

#include "path_binding_iter.h"
#include "query/executor/binding_iter.h"
#include "query/rewriter/gql/op/var_type.h"

class PathToBinding : public BindingIter {
public:
    PathToBinding(
        std::unique_ptr<PathBindingIter> iter,
        std::map<VarId, GQL::VarType> types,
        std::optional<VarId> path_var = std::nullopt
    );

    void _begin(Binding& parent_binding) override;
    void _reset() override;
    bool _next() override;
    void assign_path_var(std::unique_ptr<PathBinding>& list);
    void assign_nulls() override;

    void print(std::ostream& os, int indent = 0, bool stats = false) const override;

    std::unique_ptr<PathBindingIter> child_iter;

    std::optional<VarId> path_var = std::nullopt;

private:
    Binding* parent_binding;

    std::map<VarId, GQL::VarType> types;
};

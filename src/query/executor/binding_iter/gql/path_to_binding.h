#pragma once

#include <utility>
#include <vector>

#include "path_binding_iter.h"
#include "query/executor/binding_iter.h"
#include "query/rewriter/gql/op/var_type.h"

class PathToBinding : public BindingIter {
public:
    PathToBinding(
        std::unique_ptr<PathBindingIter> iter,
        std::map<VarId, GQL::VarType> types,
        std::optional<VarId> path_var = std::nullopt,
        bool trail = false
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

    // TRAIL: a path that repeats an edge is not a match
    const bool trail;

    // Elements replaced by group lists in the binding; put back before the child is asked again
    std::vector<std::pair<VarId, ObjectId>> overwritten_values;

    // made once: the list of a group variable that has no element on the path
    ObjectId empty_list;

    void restore_overwritten_values();
};

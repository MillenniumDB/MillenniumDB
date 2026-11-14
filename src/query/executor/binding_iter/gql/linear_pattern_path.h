#pragma once

#include <set>

#include "path_binding_iter.h"
#include "query/executor/binding_iter.h"

namespace GQL {

class LinearPatternPath : public PathBindingIter {
public:
    LinearPatternPath(std::unique_ptr<BindingIter>&& iter, std::vector<VarId>&& variables_in_pattern);

    void begin(Binding& binding) override;
    void reset() override;
    void set_left_boundary(ObjectId boundary) override;
    void set_right_boundary(ObjectId boundary) override;
    std::unique_ptr<PathBinding> next() override;
    void assign_nulls() override;
    void assign_empty() override;

    // Variables of the pattern that are bound before it: assign_nulls leaves them alone
    void set_outer_vars(std::set<VarId>&& vars);

    void print(std::ostream& os, int indent = 0, bool stats = false) override;

private:
    std::unique_ptr<BindingIter> child_iter;
    const std::vector<VarId> variables_in_pattern;
    const uint64_t pattern_size;
    std::set<VarId> outer_vars;
    Binding* parent_binding;
};

} // namespace GQL

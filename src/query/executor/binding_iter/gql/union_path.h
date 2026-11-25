#pragma once

#include <optional>

#include "path_binding_iter.h"

namespace GQL {

class UnionPath : public PathBindingIter {
public:
    UnionPath(std::vector<std::unique_ptr<PathBindingIter>>&& iters);

    void begin(Binding& binding) override;
    void reset() override;
    void set_left_boundary(const PathElement& boundary) override;
    void set_right_boundary(ObjectId boundary) override;
    std::unique_ptr<PathBinding> next() override;
    void assign_nulls() override;
    void assign_empty() override;

    void print(std::ostream& os, int indent, bool stats) override;

private:
    // the union has no boundary while it is the first thing in its path
    std::optional<PathElement> parent_boundary;
    std::vector<std::unique_ptr<PathBindingIter>> iters;
    uint64_t current_iter = 0;
    Binding* parent_binding;
    // branches are begun lazily, when the union reaches them; only those can be reset
    std::vector<bool> begun;
};

} // namespace GQL

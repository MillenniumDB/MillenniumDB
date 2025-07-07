#pragma once

#include "path_binding_iter.h"

namespace GQL {

class UnionPath : public PathBindingIter {
public:
    UnionPath(std::vector<std::unique_ptr<PathBindingIter>>&& iters);

    void begin(Binding& binding) override;
    void reset() override;
    void set_left_boundary(ObjectId boundary) override;
    void set_right_boundary(ObjectId boundary) override;
    std::unique_ptr<PathBinding> next() override;
    void assign_nulls() override;
    void assign_empty() override;

    void print(std::ostream& os, int indent, bool stats) override;

private:
    ObjectId parent_boundary = ObjectId(0);
    std::vector<std::unique_ptr<PathBindingIter>> iters;
    uint64_t current_iter = 0;
    Binding* parent_binding;
};

} // namespace GQL

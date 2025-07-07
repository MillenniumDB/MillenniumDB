#pragma once

#include "path_binding_iter.h"

class CheckRepeatedVariable : public PathBindingIter {
public:
    CheckRepeatedVariable(std::unique_ptr<PathBindingIter> iter, VarId repeated_var);

    void begin(Binding& _parent_binding) override;
    void set_left_boundary(ObjectId boundary) override;
    void set_right_boundary(ObjectId boundary) override;
    void reset() override;
    std::unique_ptr<PathBinding> next() override;
    void assign_nulls() override;
    void assign_empty() override;

    void print(std::ostream& os, int indent, bool stats) override;

private:
    std::unique_ptr<PathBindingIter> child_iter;
    VarId repeated_var;
    Binding* parent_binding;
    bool vars_are_good = false;
};

#pragma once

#include <ostream>
#include <string>

#include "path_binding_iter.h"

namespace GQL {

class ExtendRight : public PathBindingIter {
public:
    ExtendRight(std::unique_ptr<PathBindingIter>&& left, std::unique_ptr<PathBindingIter>&& right);

    void begin(Binding& binding) override;
    void reset() override;
    void set_left_boundary(ObjectId boundary) override;
    void set_right_boundary(ObjectId boundary) override;
    std::unique_ptr<PathBinding> next() override;
    void assign_nulls() override;
    void assign_empty() override;

    void print(std::ostream& os, int indent = 0, bool stats = false) override;

private:
    std::unique_ptr<PathBindingIter> left;
    std::unique_ptr<PathBindingIter> right;

    std::unique_ptr<PathBinding> left_list = nullptr;
    std::unique_ptr<PathBinding> right_list = nullptr;

    bool left_has_value = false;

    Binding* parent_binding;
};
} // namespace GQL

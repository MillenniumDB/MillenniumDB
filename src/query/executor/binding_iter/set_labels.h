#pragma once

#include <memory>
#include <vector>

#include "query/executor/binding_iter.h"
#include "query/parser/op/gql/graph_pattern/op_opt_labels.h"
#include "storage/index/lists/tmp_lists.h"

using namespace GQL;

class SetLabels : public BindingIter {
public:
    SetLabels(std::unique_ptr<BindingIter>&& iter, std::set<OptLabel> opt_labels) :
        child_iter(std::move(iter)),
        opt_labels(opt_labels)
    { }

    void accept_visitor(BindingIterVisitor& visitor) override;
    void _begin(Binding& parent_binding) override;
    bool _next() override;
    void _reset() override;
    void assign_nulls() override;

    std::unique_ptr<BindingIter> child_iter;

    std::set<OptLabel> opt_labels;

private:
    Binding* parent_binding;
};

#pragma once

#include <memory>

#include "query/executor/binding_iter.h"
#include "query/executor/binding_iter/index_scan.h"
#include "query/parser/expr/gql/expr_property.h"

namespace GQL {
class AssignProperties : public BindingIter {
public:
    AssignProperties(
        std::unique_ptr<BindingIter> child_iter,
        std::vector<ExprProperty> var_properties
    );

    void _begin(Binding& parent_binding) override;

    void _reset() override;

    bool _next() override;

    void assign_nulls() override;

    void print(std::ostream& os, int indent, bool stats) const override;

    std::unique_ptr<BindingIter> child_iter;

    std::vector<IndexScan<3>> prop_scans;

private:
    std::vector<VarId> property_vars;

    Binding* parent_binding;
};
} // namespace GQL

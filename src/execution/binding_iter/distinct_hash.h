#pragma once

#include <memory>
#include <vector>

#include "base/binding/binding_iter.h"
#include "storage/index/hash/distinct_binding_hash/distinct_binding_hash.h"

class DistinctHash : public BindingIter {
public:
    DistinctHash(std::unique_ptr<BindingIter> child_iter, std::vector<VarId> projected_vars);

    void begin(std::ostream&) override;

    bool next() override;

    GraphObject operator[](VarId var_id) const override;

    void analyze(std::ostream&, int indent = 0) const override;

private:
    std::unique_ptr<BindingIter> child_iter;

    std::vector<VarId> projected_vars;

    DistinctBindingHash<GraphObject> extendable_table;

    std::vector<GraphObject> current_tuple;
};

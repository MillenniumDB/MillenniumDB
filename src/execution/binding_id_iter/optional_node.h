#pragma once

#include <memory>
#include <vector>

#include "base/ids/var_id.h"
#include "base/binding/binding_id_iter.h"

class OptionalNode : public BindingIdIter {
public:
    OptionalNode(std::unique_ptr<BindingIdIter> graph_pattern,
                 std::vector<std::unique_ptr<BindingIdIter>> children);

    void analyze(std::ostream& os, int indent = 0) const override;
    void begin(BindingId& parent_binding) override;
    bool next() override;
    void reset() override;
    void assign_nulls() override;
    bool has_result;

private:
    std::unique_ptr<BindingIdIter> graph_pattern;
    std::vector<std::unique_ptr<BindingIdIter>> children;
};

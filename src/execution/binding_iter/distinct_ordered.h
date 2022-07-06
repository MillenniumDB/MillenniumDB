#pragma once

#include <memory>
#include <vector>

#include "base/binding/binding_iter.h"

class DistinctOrdered : public BindingIter {
public:
    DistinctOrdered(std::unique_ptr<BindingIter> child_iter,
                    std::vector<VarId> projected_vars);

    void begin(std::ostream&) override;

    bool next() override;

    GraphObject operator[](VarId var_id) const override;

    void analyze(std::ostream&, int indent = 0) const override;

private:
    std::unique_ptr<BindingIter> child_iter;
    std::vector<VarId> projected_vars;

    std::vector<GraphObject> current_tuple;
    std::vector<GraphObject> last_tuple;
};

#pragma once

#include <memory>
#include <vector>

#include "base/binding/binding_iter.h"
#include "parser/query/op/sparql/op_select.h"

namespace SPARQL {

class Select : public BindingIter {
public:
    Select(std::unique_ptr<BindingIter>         child_iter,
           std::vector<std::pair<Var, VarId>>&& projection_vars,
           uint64_t                             limit,
           uint64_t                             offset);

    void begin(std::ostream&) override;

    bool next() override;

    GraphObject operator[](VarId var_id) const override;

    void analyze(std::ostream&, int indent = 0) const override;

private:
    std::unique_ptr<BindingIter>       child_iter;
    std::vector<std::pair<Var, VarId>> projection_vars;
    uint64_t                           limit;
    uint64_t                           offset;
    uint64_t                           count = 0;
    std::ostream*                      os;
};
} // namespace SPARQL
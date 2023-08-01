#pragma once

#include <memory>
#include <vector>

#include "query/executor/binding_iter.h"
#include "query/var_id.h"
#include "query/executor/binding_iter/empty_binding_iter.h"

class IndexLeftOuterJoin : public BindingIter {
public:
    IndexLeftOuterJoin(
        std::unique_ptr<BindingIter> lhs,
        std::unique_ptr<BindingIter> rhs,
        std::vector<VarId>&&           rhs_only_vars
    ) :
        lhs           (std::move(lhs)),
        original_rhs  (std::move(rhs)),
        rhs_only_vars (std::move(rhs_only_vars)) { }

    void analyze(std::ostream& os, int indent = 0) const override;
    void begin(Binding& parent_binding) override;
    bool next() override;
    void reset() override;
    void assign_nulls() override;

    // true if next rhs->next() == false has to return with nulls at the right
    bool must_return_null;
    uint_fast32_t results_found = 0;

private:
    std::unique_ptr<BindingIter> lhs;
    std::unique_ptr<BindingIter> original_rhs;

    std::vector<VarId> rhs_only_vars;

    BindingIter* rhs; // will point to original_rhs or a EmptyBindingIter

    Binding* parent_binding;

    EmptyBindingIter empty_iter;
};

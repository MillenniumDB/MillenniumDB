#pragma once

#include <vector>
#include <memory>

#include "query/executor/binding_iter.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

class Filter : public BindingIter {
public:
    Filter(
        std::unique_ptr<BindingIter>                child,
        std::vector<std::unique_ptr<BindingExpr>>&& filters,
        bool                                          is_having_filter
    ) :
        filters          (std::move(filters)),
        child            (std::move(child)),
        is_having_filter (is_having_filter) { }

    void analyze(std::ostream& os, int indent = 0) const override;
    void begin(Binding& parent_binding) override;
    bool next() override;
    void reset() override;
    void assign_nulls() override;


    Binding* parent_binding;

    std::vector<std::unique_ptr<BindingExpr>> filters;

    std::unique_ptr<BindingIter> child;

    // To differentiate normal Filter and Having
    const bool is_having_filter;

    // statistics
    uint_fast32_t filtered_results = 0;
    uint_fast32_t passed_results = 0;
};

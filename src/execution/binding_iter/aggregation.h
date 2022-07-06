#pragma once

#include <map>
#include <memory>
#include <set>
#include <vector>

#include "base/binding/binding_iter.h"
#include "base/query/var.h"
#include "execution/binding_iter/aggregation/agg.h"

class Aggregation : public BindingIter {
public:
    Aggregation(std::unique_ptr<BindingIter>          child_iter,
                std::map<VarId, std::unique_ptr<Agg>> aggregates,
                const std::set<VarId>&                saved_vars,
                std::vector<VarId>                    group_vars);

    ~Aggregation();

    void begin(std::ostream&) override;

    bool next() override;

    GraphObject operator[](VarId var_id) const override;

    void analyze(std::ostream&, int indent = 0) const override;

private:
    std::unique_ptr<BindingIter> child_iter;

    std::map<VarId, std::unique_ptr<Agg>> aggregates;

    // variables that will be saved into saved_result, must include all group_vars
    std::vector<VarId> saved_vars;

    // may be empty if when the query uses aggregates without any group
    std::vector<VarId> group_vars;

    // array
    GraphObject* saved_result;

    bool saved_next;
};

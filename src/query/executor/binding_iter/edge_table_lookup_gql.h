#pragma once

#include "query/executor/binding_iter.h"
#include "storage/index/random_access_table/random_access_table.h"

class EdgeTableLookupGQL : public BindingIter {
public:
    EdgeTableLookupGQL(
        RandomAccessTable<2>& table,
        VarId edge,
        Id from,
        Id to,
        bool from_assigned,
        bool to_assigned,
        uint64_t expected_mask
    ) :
        edge(edge),
        from(from),
        to(to),
        from_assigned(from_assigned),
        to_assigned(to_assigned),
        table(table),
        expected_mask(expected_mask)
    { }

    void print(std::ostream& os, int indent, bool stats) const override;

    void _begin(Binding& parent_binding) override;
    bool _next() override;
    void _reset() override;
    void assign_nulls() override;

    VarId edge;
    Id from;
    Id to;
    bool from_assigned;
    bool to_assigned;

private:
    RandomAccessTable<2>& table;
    bool already_looked;

    Binding* parent_binding;
    uint64_t expected_mask;
};

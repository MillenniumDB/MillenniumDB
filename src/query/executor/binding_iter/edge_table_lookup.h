#pragma once

#include "query/executor/binding_iter.h"
#include "storage/index/random_access_table/random_access_table.h"

class EdgeTableLookup : public BindingIter {
public:
    EdgeTableLookup(
        RandomAccessTable<3>& table,
        Id edge,
        Id from,
        Id to,
        Id type,
        bool from_assigned,
        bool to_assigned,
        bool type_assigned
    ) :
        table(table),
        edge(edge),
        from(from),
        to(to),
        type(type),
        from_assigned(from_assigned),
        to_assigned(to_assigned),
        type_assigned(type_assigned) { }

    void accept_visitor(BindingIterVisitor& visitor) override;
    void _begin(Binding& parent_binding) override;
    bool _next() override;
    void _reset() override;
    void assign_nulls() override;

    uint64_t lookups = 0;

private:
    RandomAccessTable<3>& table;
    Id edge;
    Id from;
    Id to;
    Id type;
    bool from_assigned;
    bool to_assigned;
    bool type_assigned;

    // because the interface will call next() until returns false, this variable
    // prevent giving the same result multiple times
    bool already_looked;

    Binding* parent_binding;
};

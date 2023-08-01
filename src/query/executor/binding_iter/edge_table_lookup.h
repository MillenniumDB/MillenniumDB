#pragma once

#include <memory>
#include <vector>
#include <variant>

#include "query/executor/binding_iter.h"
#include "query/id.h"
#include "storage/index/random_access_table/random_access_table.h"

class EdgeTableLookup : public BindingIter {
public:
    EdgeTableLookup(
        RandomAccessTable<3>& table,
        Id                    edge,
        Id                    from,
        Id                    to,
        Id                    type
    ) :
        table         (table),
        edge          (edge),
        from          (from),
        to            (to),
        type          (type) { }

    void analyze(std::ostream&, int indent = 0) const override;
    void begin(Binding& parent_binding) override;
    bool next() override;
    void reset() override;
    void assign_nulls() override;

private:
    RandomAccessTable<3>& table;
    Id edge;
    Id from;
    Id to;
    Id type;
    uint64_t lookups = 0;
    uint64_t results = 0;

    // because the interface will call next() until returns false, this variable prevent giving
    // the same result multiple times
    bool already_looked;

    Binding* parent_binding;
};

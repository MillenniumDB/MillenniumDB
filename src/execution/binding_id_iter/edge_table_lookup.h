#pragma once

#include <memory>
#include <vector>
#include <variant>

#include "base/binding/binding_id_iter.h"
#include "base/ids/id.h"
#include "base/thread/thread_info.h"
#include "storage/index/random_access_table/random_access_table.h"

class EdgeTableLookup : public BindingIdIter {
private:
    RandomAccessTable<3>& table;
    ThreadInfo* const thread_info;
    Id edge;
    Id from;
    Id to;
    Id type;
    uint64_t lookups = 0;
    uint64_t results = 0;

    // because the interface will call next() until returns false, this variable prevent giving
    // the same result multiple times
    bool already_looked;

    BindingId* parent_binding;

public:
    EdgeTableLookup(RandomAccessTable<3>& table, ThreadInfo*, Id edge, Id from, Id to, Id type);
    ~EdgeTableLookup() = default;

    void analyze(std::ostream&, int indent = 0) const override;
    void begin(BindingId& parent_binding) override;
    bool next() override;
    void reset() override;
    void assign_nulls() override;
};

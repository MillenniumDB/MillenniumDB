#ifndef RELATIONAL_MODEL__EDGE_TABLE_LOOKUP_H_
#define RELATIONAL_MODEL__EDGE_TABLE_LOOKUP_H_

#include <memory>
#include <vector>
#include <variant>

#include "base/binding/binding_id_iter.h"
#include "base/ids/object_id.h"
#include "base/ids/var_id.h"
#include "base/thread/thread_info.h"
#include "storage/index/random_access_table/random_access_table.h"

class EdgeTableLookup : public BindingIdIter {
    using Id = std::variant<VarId, ObjectId>;

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

#endif // RELATIONAL_MODEL__EDGE_TABLE_ENUM_H_

#ifndef RELATIONAL_MODEL__DISTINCT_ID_HASH_H_
#define RELATIONAL_MODEL__DISTINCT_ID_HASH_H_

#include <map>
#include <memory>

#include "base/binding/binding_id_iter.h"
#include "base/ids/var_id.h"
#include "storage/index/hash/distinct_binding_hash/distinct_binding_hash.h"

class DistinctIdHash : public BindingIdIter {
public:
    DistinctIdHash(std::unique_ptr<BindingIdIter> child_iter, std::vector<VarId> projected_vars);
    ~DistinctIdHash() = default;

    void begin(BindingId& parent_binding) override;
    void reset() override;
    bool next() override;
    void assign_nulls() override;

    void analyze(std::ostream&, int indent = 0) const override;

    bool current_tuple_distinct();

private:
    std::unique_ptr<BindingIdIter> child_iter;
    std::vector<VarId> projected_vars;
    DistinctBindingHash<ObjectId> extendable_table;

    std::vector<ObjectId> current_tuple;
    BindingId* parent_binding;
};

#endif // RELATIONAL_MODEL__DISTINCT_ID_HASH_H_

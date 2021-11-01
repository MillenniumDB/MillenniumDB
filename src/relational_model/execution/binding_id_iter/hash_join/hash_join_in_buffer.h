#ifndef RELATIONAL_MODEL__HASH_JOIN_IN_BUFFER_H_
#define RELATIONAL_MODEL__HASH_JOIN_IN_BUFFER_H_

#include <memory>
#include <vector>

#include "base/ids/var_id.h"
#include "base/binding/binding_id_iter.h"
#include "relational_model/execution/binding_id_iter/hash_join/key_value_pair_hasher.h"
#include "storage/index/hash/key_value_hash/key_value_hash.h"


class HashJoinInBuffer : public BindingIdIter {
public:
    HashJoinInBuffer(std::unique_ptr<BindingIdIter> lhs,
                     std::unique_ptr<BindingIdIter> rhs,
                     std::vector<VarId>             left_vars,
                     std::vector<VarId>             common_vars,
                     std::vector<VarId>             right_vars);
    ~HashJoinInBuffer() = default;

    void analyze(std::ostream& os, int indent = 0) const override;
    void begin(BindingId& parent_binding) override;
    bool next() override;
    void reset() override;
    void assign_nulls() override;

private:
    std::unique_ptr<BindingIdIter> lhs;
    std::unique_ptr<BindingIdIter> rhs;
    std::vector<VarId> left_vars;
    std::vector<VarId> common_vars;
    std::vector<VarId> right_vars;

    BindingId* parent_binding;

    // true if we found a value from rhs (current_key and current_value) and we got a pre-match (same hash to lhs bucket).
    // In this state we have to iterate over all the keys in that bucket (`current_bucket`) to check if
    // they have the same key. Once the iterating is over, enumerating turns into false.
    bool enumerating;

    KeyValueHash<ObjectId, ObjectId> lhs_hash;  // asumming lhs is the smaller one (from execution plan)
    uint_fast32_t current_bucket;
    uint_fast32_t current_bucket_pos;

    std::vector<ObjectId> current_key;
    std::vector<ObjectId> current_value;
};

#endif // RELATIONAL_MODEL__HASH_JOIN_IN_BUFFER_H_
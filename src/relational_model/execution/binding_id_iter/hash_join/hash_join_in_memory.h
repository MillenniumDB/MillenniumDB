#ifndef RELATIONAL_MODEL__HASH_JOIN_IN_MEMORY_H_
#define RELATIONAL_MODEL__HASH_JOIN_IN_MEMORY_H_

#include <memory>
#include <vector>
#include <unordered_map>

#include "base/ids/var_id.h"
#include "base/binding/binding_id_iter.h"
#include "relational_model/execution/binding_id_iter/hash_join/key_value_pair_hasher.h"


class HashJoinInMemory : public BindingIdIter {
public:
    HashJoinInMemory(std::unique_ptr<BindingIdIter> lhs,
                     std::unique_ptr<BindingIdIter> rhs,
                     std::vector<VarId>             left_vars,
                     std::vector<VarId>             common_vars,
                     std::vector<VarId>             right_vars);
    ~HashJoinInMemory() = default;

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

    bool enumerating;
    std::unordered_multimap<std::vector<ObjectId>,
                            std::vector<ObjectId>,
                            KeyValuePairHasher> lhs_hash;  // asume left is the smallest one (from execution plan)
    std::unordered_multimap<std::vector<ObjectId>,
                            std::vector<ObjectId>,
                            KeyValuePairHasher>::iterator current_pair_iter;
    std::unordered_multimap<std::vector<ObjectId>,
                            std::vector<ObjectId>,
                            KeyValuePairHasher>::iterator end_range_iter;

    std::vector<ObjectId> current_key;
    std::vector<ObjectId> current_value;
};

#endif // RELATIONAL_MODEL__HASH_JOIN_IN_MEMORY_H_

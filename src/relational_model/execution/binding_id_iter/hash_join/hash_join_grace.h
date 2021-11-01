#ifndef RELATIONAL_MODEL__HASH_JOIN_GRACE_H_
#define RELATIONAL_MODEL__HASH_JOIN_GRACE_H_

#include <memory>
#include <vector>
#include <unordered_map>

#include "base/ids/var_id.h"
#include "base/binding/binding_id_iter.h"
#include "relational_model/execution/binding_id_iter/hash_join/key_value_pair_hasher.h"
#include "storage/index/hash/key_value_hash/key_value_hash.h"
#include "storage/index/hash/hash_functions/hash_function_wrapper.h"
#include "storage/page.h"


class HashJoinGrace : public BindingIdIter {
    enum class State {
        // Enumerating results with second hash iter
        ENUM_WITH_SECOND_HASH_ITER,

        // Checking if we have at least one match in bucket with second hash
        ENUM_WITH_SECOND_HASH,

        // Enumerating results with nested loop and we already found the first position in bucket
        ENUM_WITH_NESTED_LOOP_ITER,

        // Checking if we have at least one match in bucket with nested loop
        ENUM_WITH_NESTED_LOOP,

        // Start state for a new bucket
        NOT_ENUM
    };

public:

    static constexpr uint_fast32_t MAX_SIZE_SMALL_HASH = Page::MDB_PAGE_SIZE*1024;

    HashJoinGrace(std::unique_ptr<BindingIdIter> lhs,
                  std::unique_ptr<BindingIdIter> rhs,
                  std::vector<VarId>             left_vars,
                  std::vector<VarId>             common_vars,
                  std::vector<VarId>             right_vars);
    ~HashJoinGrace() = default;

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

    KeyValueHash<ObjectId, ObjectId> lhs_hash;
    KeyValueHash<ObjectId, ObjectId> rhs_hash;
    uint_fast32_t total_buckets;

    State current_state = State::NOT_ENUM;
    bool left_min = false;
    std::unordered_multimap<std::vector<ObjectId>,
                            std::vector<ObjectId>,
                            KeyValuePairHasher2> small_hash;

    uint_fast32_t current_pos_left;   // for nested loop
    uint_fast32_t current_pos_right;
    uint_fast32_t current_bucket;
    std::unordered_multimap<std::vector<ObjectId>,
                            std::vector<ObjectId>,
                            KeyValuePairHasher2>::iterator current_pair_iter;  // for small hash
    std::unordered_multimap<std::vector<ObjectId>,
                            std::vector<ObjectId>,
                            KeyValuePairHasher2>::iterator end_range_iter;

    //std::vector<ObjectId> current_key;
    //std::vector<ObjectId> current_value;
    std::pair<std::vector<ObjectId>, std::vector<ObjectId>> saved_pair;

    void assign_left_binding(const std::vector<ObjectId>& lhs_value);
    void assign_key_binding(const std::vector<ObjectId>& my_key);
    void assign_right_binding(const std::vector<ObjectId>& rhs_value);
};

#endif // RELATIONAL_MODEL__HASH_JOIN_GRACE_H_

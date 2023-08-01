#pragma once

#include <memory>
#include <vector>
#include <unordered_map>

#include "query/executor/binding_iter.h"
#include "storage/index/hash/key_value_hash/key_value_pair_hasher.h"

class HashJoinInMemory : public BindingIter {
public:
    HashJoinInMemory(
        std::unique_ptr<BindingIter> lhs,
        std::unique_ptr<BindingIter> rhs,
        std::vector<VarId>             left_vars,
        std::vector<VarId>             common_vars,
        std::vector<VarId>             right_vars
    ) :
        lhs           (std::move(lhs)),
        rhs           (std::move(rhs)),
        left_vars     (left_vars),
        common_vars   (common_vars),
        right_vars    (right_vars) { }

    void analyze(std::ostream& os, int indent = 0) const override;
    void begin(Binding& parent_binding) override;
    bool next() override;
    void reset() override;
    void assign_nulls() override;

private:
    std::unique_ptr<BindingIter> lhs;
    std::unique_ptr<BindingIter> rhs;
    std::vector<VarId> left_vars;
    std::vector<VarId> common_vars;
    std::vector<VarId> right_vars;

    Binding* parent_binding;

    bool enumerating;
    std::unordered_multimap<std::vector<ObjectId>,
                            std::vector<ObjectId>,
                            KeyValuePairHasher> lhs_hash;  // assume left is the smallest one (from execution plan)
    std::unordered_multimap<std::vector<ObjectId>,
                            std::vector<ObjectId>,
                            KeyValuePairHasher>::iterator current_pair_iter;
    std::unordered_multimap<std::vector<ObjectId>,
                            std::vector<ObjectId>,
                            KeyValuePairHasher>::iterator end_range_iter;

    std::vector<ObjectId> current_key;
    std::vector<ObjectId> current_value;
};

#include "hash_join_in_memory.h"

#include "base/ids/var_id.h"

using namespace std;

HashJoinInMemory::HashJoinInMemory(unique_ptr<BindingIdIter> lhs,
                                   unique_ptr<BindingIdIter> rhs,
                                   vector<VarId> left_vars,
                                   vector<VarId> common_vars,
                                   vector<VarId> right_vars) :
    lhs         (move(lhs)),
    rhs         (move(rhs)),
    left_vars   (left_vars),
    common_vars (common_vars),
    right_vars  (right_vars)
    { }


void HashJoinInMemory::begin(BindingId& _parent_binding) {
    this->parent_binding = &_parent_binding;

    lhs->begin(_parent_binding);
    rhs->begin(_parent_binding);

    current_key = std::vector<ObjectId>(common_vars.size());
    current_value = std::vector<ObjectId>(left_vars.size());
    while (lhs->next()) {
        // save left keys and value
        for (size_t i = 0; i < common_vars.size(); i++) {
            current_key[i] = (*parent_binding)[common_vars[i]];
        }
        for (size_t i = 0; i < left_vars.size(); i++) {
            current_value[i] = (*parent_binding)[left_vars[i]];
        }
        lhs_hash.insert(std::make_pair(current_key, current_value));
    }

    current_value.resize(right_vars.size());
    // saved_pair = make_pair(current_key, current_value);  // init sizes

    current_pair_iter = lhs_hash.end();
    end_range_iter = lhs_hash.end();
    enumerating = false;
}


bool HashJoinInMemory::next() {
    while (true) {
        if (enumerating) {
            assert(current_pair_iter != end_range_iter);
            // set binding from lhs
            for (uint_fast32_t i = 0; i < left_vars.size(); i++) {
                parent_binding->add(left_vars[i], current_pair_iter->second[i]);
            }
            ++current_pair_iter;
            if (current_pair_iter == end_range_iter) {
                enumerating = false;
            }
            return true;
        }
        else {
            if (rhs->next()) {
                for (size_t i = 0; i < common_vars.size(); i++) {
                    current_key[i] = (*parent_binding)[common_vars[i]];
                }
                for (size_t i = 0; i < right_vars.size(); i++) {
                    current_value[i] = (*parent_binding)[right_vars[i]];
                }
                auto range = lhs_hash.equal_range(current_key);
                current_pair_iter = range.first;
                end_range_iter = range.second;
                if (current_pair_iter != end_range_iter) {
                    // set binding from rhs
                    for (uint_fast32_t i = 0; i < common_vars.size(); i++) {
                        parent_binding->add(common_vars[i], current_key[i]);
                    }
                    for (uint_fast32_t i = 0; i < right_vars.size(); i++) {
                        parent_binding->add(right_vars[i], current_value[i]);
                    }
                    enumerating = true;
                }
            }
            else {
                return false;
            }
        }
    }
}


void HashJoinInMemory::reset() {
    lhs->reset();
    rhs->reset();

    current_value = std::vector<ObjectId>(left_vars.size());
    lhs_hash.clear();
    while (lhs->next()){
        // save left keys and value
        for (size_t i = 0; i < common_vars.size(); i++) {
            current_key[i] = (*parent_binding)[common_vars[i]];
        }
        for (size_t i = 0; i < left_vars.size(); i++) {
            current_value[i] = (*parent_binding)[left_vars[i]];
        }
        lhs_hash.insert(std::make_pair(current_key, current_value));
    }

    current_value = std::vector<ObjectId>(right_vars.size());

    current_pair_iter = lhs_hash.end();
    end_range_iter = lhs_hash.end();
    enumerating = false;
}


void HashJoinInMemory::assign_nulls() {
    rhs->assign_nulls();
    lhs->assign_nulls();
}


void HashJoinInMemory::analyze(std::ostream& os, int indent) const {
    os << std::string(indent, ' ');
    os << "HashJoinInMemoryInMemory(\n";
    lhs->analyze(os, indent + 2);
    os << ",\n";
    rhs->analyze(os, indent + 2);
    os << "\n";
    os << std::string(indent, ' ');
    os << ")";
}

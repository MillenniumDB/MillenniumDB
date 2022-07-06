#include "hash_join_in_buffer.h"

#include "base/ids/var_id.h"

using namespace std;

HashJoinInBuffer::HashJoinInBuffer(unique_ptr<BindingIdIter> lhs,
                                   unique_ptr<BindingIdIter> rhs,
                                   std::vector<VarId> left_vars,
                                   std::vector<VarId> common_vars,
                                   std::vector<VarId> right_vars) :
    lhs         (move(lhs)),
    rhs         (move(rhs)),
    left_vars   (left_vars),
    common_vars (common_vars),
    right_vars  (right_vars),
    lhs_hash    (KeyValueHash<ObjectId, ObjectId>(common_vars.size(), left_vars.size()))  // empty initialization
    { }


void HashJoinInBuffer::begin(BindingId& _parent_binding) {
    this->parent_binding = &_parent_binding;

    lhs->begin(_parent_binding);
    rhs->begin(_parent_binding);
    lhs_hash.begin();

    current_key = std::vector<ObjectId>(common_vars.size());
    current_value = std::vector<ObjectId>(left_vars.size());
    while (lhs->next()){
        // save left keys and value
        for (size_t i = 0; i < common_vars.size(); i++) {
            current_key[i] = (*parent_binding)[common_vars[i]];
        }
        for (size_t i = 0; i < left_vars.size(); i++) {
            current_value[i] = (*parent_binding)[left_vars[i]];
        }
        lhs_hash.insert(current_key, current_value);
    }
    // sort buckets for faster comparison, we assume the insertion process is over
    lhs_hash.sort_buckets();

    current_value.resize(right_vars.size());
    enumerating = false;
}


bool HashJoinInBuffer::next() {
    while (true) {
        if (enumerating) {
            // TODO: first key is checked twice, we can return it before enumerating?
            auto left_key = lhs_hash.get_key(current_bucket, current_bucket_pos);
            for (uint_fast32_t i = 0; i < current_key.size(); i++) {
                if (current_key[i] != left_key[i]) {
                    enumerating = false;
                    break;
                }
            }
            if (!enumerating) {
                continue;
            }
            auto left_value = lhs_hash.get_value(current_bucket, current_bucket_pos);
            for (uint_fast32_t i = 0; i < left_vars.size(); i++) {
                parent_binding->add(left_vars[i], left_value[i]);
            }
            current_bucket_pos++;
            if (current_bucket_pos >= lhs_hash.get_bucket_size(current_bucket)) {
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
                current_bucket = lhs_hash.get_bucket(current_key);
                if (lhs_hash.find_first(current_key, current_bucket, &current_bucket_pos)) {
                    enumerating = true;
                }
            }
            else {
                return false;
            }
        }
    }
}


void HashJoinInBuffer::reset() {
    lhs->reset();
    rhs->reset();

    current_value = std::vector<ObjectId>(left_vars.size());
    lhs_hash.reset();
    while (lhs->next()){
        // save left keys and value
        for (size_t i = 0; i < common_vars.size(); i++) {
            current_key[i] = (*parent_binding)[common_vars[i]];
        }
        for (size_t i = 0; i < left_vars.size(); i++) {
            current_value[i] = (*parent_binding)[left_vars[i]];
        }
        lhs_hash.insert(current_key, current_value);
    }
    // sort buckets for faster comparison, we assume the insertion process is over
    lhs_hash.sort_buckets();

    current_value = std::vector<ObjectId>(right_vars.size());
    enumerating = false;
}


void HashJoinInBuffer::assign_nulls() {
    rhs->assign_nulls();
    lhs->assign_nulls();
}


void HashJoinInBuffer::analyze(std::ostream& os, int indent) const {
    os << std::string(indent, ' ');
    os << "HashJoinInBuffer(\n";
    lhs->analyze(os, indent + 2);
    os << ",\n";
    rhs->analyze(os, indent + 2);
    os << "\n";
    os << std::string(indent, ' ');
    os << ")";
}

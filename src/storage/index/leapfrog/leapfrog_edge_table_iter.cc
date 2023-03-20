#include "leapfrog_edge_table_iter.h"

using namespace std;

void LeapfrogEdgeTableIter::down() {
    level++;
    if (level == 0) {
        auto record = edge_table[0];
        current_tuple[0] = 1 | ObjectId::MASK_EDGE;
        current_tuple[1] = (*record)[permutation[0]];
        current_tuple[2] = (*record)[permutation[1]];
        current_tuple[3] = (*record)[permutation[2]];
    }
}


bool LeapfrogEdgeTableIter::next() {
    if (level == 0) {
        auto edge_value = (current_tuple[0] + 1) & ObjectId::VALUE_MASK;
        auto record = edge_table[edge_value - 1];
        if (record == nullptr) {
            return false;
        } else {
            current_tuple[0] = current_tuple[0] + 1;
            current_tuple[1] = (*record)[permutation[0]];
            current_tuple[2] = (*record)[permutation[1]];
            current_tuple[3] = (*record)[permutation[2]];
            return true;
        }
    } else {
        return false;
    }
}


bool LeapfrogEdgeTableIter::seek(uint64_t key) {
    if (level == 0) {
        // level == 0 implies initial_ranges is empty
        // check if key is an edge
        if ((key & ObjectId::TYPE_MASK) != ObjectId::MASK_EDGE) {
            // if key is not an edge, it must be something greater than all edges
            // else something is wrong with the leapfrog execution, because it needs to sort iterators
            // and seek calls only are going forward
            return false;
        }
        auto edge_value = key & ObjectId::VALUE_MASK;
        auto record = edge_table[edge_value - 1];
        if (record == nullptr) {
            return false;
        } else {
            current_tuple[0] = key;
            current_tuple[1] = (*record)[permutation[0]];
            current_tuple[2] = (*record)[permutation[1]];
            current_tuple[3] = (*record)[permutation[2]];
            return true;
        }
    } else {
        return current_tuple[level] >= key;
    }
}


bool LeapfrogEdgeTableIter::open_terms(BindingId& input_binding) {
    // cout << "LeapfrogEdgeTableIter::open_terms\n";
    if (initial_ranges.empty()) {
        // initialize with first edge if it exists
        auto record = edge_table[0];
        if (record == nullptr) {
            return false;
        } else {
            current_tuple[0] = 1 | ObjectId::MASK_EDGE;
            current_tuple[1] = (*record)[permutation[0]];
            current_tuple[2] = (*record)[permutation[1]];
            current_tuple[3] = (*record)[permutation[2]];
            return true;
        }
    }

    // initial_ranges is not empty so initial_ranges[0] must be the edge
    auto obj = initial_ranges[0]->get_min(input_binding);

    // check if obj is an edge
    if ((obj & ObjectId::TYPE_MASK) != ObjectId::MASK_EDGE) {
        return false;
    }
    auto edge_value = obj & ObjectId::VALUE_MASK;
    auto record = edge_table[edge_value];
    if (record == nullptr) {
        return false;
    } else {
        current_tuple[0] = obj;
        current_tuple[1] = (*record)[permutation[0]];
        current_tuple[2] = (*record)[permutation[1]];
        current_tuple[3] = (*record)[permutation[2]];

        // check assigned_vars
        for (std::size_t i = 1; i < initial_ranges.size(); i++) { // skip 0, already checked the edge
            if (current_tuple[i] != initial_ranges[i]->get_min(input_binding)) {
                return false;
            }
        }
        level = initial_ranges.size() - 1;
        return true;
    }
}


bool LeapfrogEdgeTableIter::next_enumeration(BindingId& binding) {
    if (level == -1) {
        current_edge_enum++;
        auto record = edge_table[current_edge_enum - 1];
        if (record == nullptr) {
            return false;
        } else {
            std::array<uint64_t, 4> enum_tuple;
            enum_tuple[0] = current_edge_enum | ObjectId::MASK_EDGE;
            enum_tuple[1] = (*record)[permutation[0]];
            enum_tuple[2] = (*record)[permutation[1]];
            enum_tuple[3] = (*record)[permutation[2]];
            for (size_t i = 0; i < enumeration_vars.size(); i++) {
                binding.add(enumeration_vars[i],
                            ObjectId(enum_tuple[initial_ranges.size() + intersection_vars.size() + i]));
            }
            return true;
        }
    } else {
        if (saved_edge_enum != current_edge_enum) {
            return false;
        } else {
            for (size_t i = 0; i < enumeration_vars.size(); i++) {
                binding.add(enumeration_vars[i],
                            ObjectId(current_tuple[initial_ranges.size() + intersection_vars.size() + i]));
            }
            current_edge_enum++;
            return true;
        }
    }
}


void LeapfrogEdgeTableIter::begin_enumeration() {
    saved_edge_enum = (current_tuple[0] & ObjectId::VALUE_MASK) - 1;
    current_edge_enum = saved_edge_enum;
}


void LeapfrogEdgeTableIter::reset_enumeration() {
    current_edge_enum = saved_edge_enum;
}

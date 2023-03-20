#include "path_index.h"

#include "query_optimizer/quad_model/quad_model.h"

using namespace std;
using namespace Paths;


// B+Tree
uint64_t BTreeIndexIterator::get_node() {
    return current->ids[2];
}


uint64_t BTreeIndexIterator::get_edge() {
    return current->ids[3];
}


bool BTreeIndexIterator::next() {
    // Don't do anything if already finished
    if (finished) {
        return false;
    }

    // Advance iterator
    current = iter->next();
    if (current != nullptr) {
        return true;
    }

    // Mark as finished
    finished = true;
    return false;
}


bool BTreeIndexIterator::at_end() {
    return finished;
}


// Trie
uint64_t TrieIndexIterator::get_node() {
    return *current;
}


bool TrieIndexIterator::next() {
    // Timeout
    if (__builtin_expect(!!(*interruption_requested), 0)) {
        throw InterruptedException();
    }

    // Return true if the pointer is in range
    current++;
    return current < end;
}


bool TrieIndexIterator::at_end() {
    return current >= end;
}


unique_ptr<IndexIterator> TrieIndex::get_iterator(uint64_t node_id, bool* interruption_requested) {
    auto query_result = trie->query(node_id);
    return make_unique<TrieIndexIterator>(interruption_requested, query_result.first, query_result.second);
}


// Edge Trie
uint64_t EdgeTrieIndexIterator::get_node() {
    return current->first;
}


uint64_t EdgeTrieIndexIterator::get_edge() {
    return current->second;
}


bool EdgeTrieIndexIterator::next() {
    // Timeout
    if (__builtin_expect(!!(*interruption_requested), 0)) {
        throw InterruptedException();
    }

    // Return true if the pointer is in range
    current++;
    return current < end;
}


bool EdgeTrieIndexIterator::at_end() {
    return current >= end;
}


unique_ptr<IndexIterator> EdgeTrieIndex::get_iterator(uint64_t node_id, bool* interruption_requested) {
    auto query_result = trie->query(node_id);
    return make_unique<EdgeTrieIndexIterator>(interruption_requested, query_result.first, query_result.second);
}


// Hash Trie
uint64_t HashTrieIndexIterator::get_node() {
    return *current;
}


bool HashTrieIndexIterator::next() {
    // Timeout
    if (__builtin_expect(!!(*interruption_requested), 0)) {
        throw InterruptedException();
    }

    // Return true if the pointer is in range
    current++;
    return current < end;
}


bool HashTrieIndexIterator::at_end() {
    return current >= end;
}


unique_ptr<IndexIterator> HashTrieIndex::get_iterator(uint64_t node_id, bool* interruption_requested) {
    auto query_result = hash_trie->query(node_id);
    return make_unique<HashTrieIndexIterator>(interruption_requested, query_result.first, query_result.second);
}


// Quad Model
bool QuadModelIndexProvider::node_exists(uint64_t node_id) {
    auto node_iter = quad_model.nodes->get_range(interruption_requested,
                                                 Record<1>({node_id}),
                                                 Record<1>({node_id}));
    return node_iter->next() != nullptr;
}


unique_ptr<IndexIterator> QuadModelIndexProvider::get_btree_iterator(uint64_t type_id, bool inverse, uint64_t node_id) {
    // B+Tree settings
    array<uint64_t, 4> min_ids;
    array<uint64_t, 4> max_ids;
    min_ids[0] = type_id;
    max_ids[0] = type_id;
    min_ids[1] = node_id;
    max_ids[1] = node_id;
    min_ids[2] = 0;
    max_ids[2] = UINT64_MAX;
    min_ids[3] = 0;
    max_ids[3] = UINT64_MAX;

    // Get iter from correct B+Tree
    if (inverse) {
        return make_unique<BTreeIndexIterator>(
            quad_model.type_to_from_edge->get_range(interruption_requested,
                                                    Record<4>(min_ids),
                                                    Record<4>(max_ids)));
    } else {
        return make_unique<BTreeIndexIterator>(
            quad_model.type_from_to_edge->get_range(interruption_requested,
                                                    Record<4>(min_ids),
                                                    Record<4>(max_ids)));
    }
}


unique_ptr<IndexIterator> QuadModelIndexProvider::get_iterator(uint64_t type_id, bool inverse, uint64_t node_id) {
    if (inverse) {
        auto assigned_index = t_inv_info.find(type_id);
        if (assigned_index->second == IndexType::BTREE) {
            return get_btree_iterator(type_id, inverse, node_id);
        }
        auto index = quad_model.get_index(type_id, inverse, assigned_index->second, interruption_requested);
        return index->get_iterator(node_id, interruption_requested);
    } else {
        auto assigned_index = t_info.find(type_id);
        if (assigned_index->second == IndexType::BTREE) {
            return get_btree_iterator(type_id, inverse, node_id);
        }
        auto index = quad_model.get_index(type_id, inverse, assigned_index->second, interruption_requested);
        return index->get_iterator(node_id, interruption_requested);
    }
}

#include "path_index.h"

#include "macros/likely.h"
#include "query/exceptions.h"

using namespace Paths;

// B+Tree
template<size_t N>
uint64_t BTreeIndexIterator<N>::get_starting_node() {
    return (*current)[starting_node_idx];
}

template<size_t N>
uint64_t BTreeIndexIterator<N>::get_reached_node() {
    return (*current)[reached_node_idx];
}

template<>
uint64_t BTreeIndexIterator<3>::get_edge() {
    return 0;
}

template<>
uint64_t BTreeIndexIterator<4>::get_edge() {
    return (*current)[3];
}


template<size_t N>
bool BTreeIndexIterator<N>::next() {
    // Don't do anything if already finished
    if (finished) {
        return false;
    }

    // Advance iterator
    current = iter.next();
    if (current != nullptr) {
        return true;
    }

    // Mark as finished
    finished = true;
    return false;
}


template<size_t N>
bool BTreeIndexIterator<N>::at_end() {
    return finished;
}


// Trie
uint64_t TrieIndexIterator::get_starting_node() {
    return 0;
}

uint64_t TrieIndexIterator::get_reached_node() {
    return *current;
}


bool TrieIndexIterator::next() {
    // Timeout
    if (MDB_unlikely(*interruption_requested)) {
        throw InterruptedException();
    }

    // Return true if the pointer is in range
    current++;
    return current < end;
}


bool TrieIndexIterator::at_end() {
    return current >= end;
}


std::unique_ptr<EdgeIter> TrieIndex::get_iter(uint64_t node_id, bool* interruption_requested) {
    auto query_result = trie->query(node_id);
    return std::make_unique<TrieIndexIterator>(interruption_requested, query_result.first, query_result.second);
}


// Edge Trie
uint64_t EdgeTrieIndexIterator::get_starting_node() {
    return 0;
}

uint64_t EdgeTrieIndexIterator::get_reached_node() {
    return current->first;
}


uint64_t EdgeTrieIndexIterator::get_edge() {
    return current->second;
}


bool EdgeTrieIndexIterator::next() {
    // Timeout
    if (MDB_unlikely(*interruption_requested)) {
        throw InterruptedException();
    }

    // Return true if the pointer is in range
    current++;
    return current < end;
}


bool EdgeTrieIndexIterator::at_end() {
    return current >= end;
}


std::unique_ptr<EdgeIter> EdgeTrieIndex::get_iter(uint64_t node_id, bool* interruption_requested) {
    auto query_result = trie->query(node_id);
    return std::make_unique<EdgeTrieIndexIterator>(interruption_requested, query_result.first, query_result.second);
}


// Hash Trie
uint64_t HashTrieIndexIterator::get_starting_node() {
    return 0;
}

uint64_t HashTrieIndexIterator::get_reached_node() {
    return *current;
}


bool HashTrieIndexIterator::next() {
    // Timeout
    if (MDB_unlikely(*interruption_requested)) {
        throw InterruptedException();
    }

    // Return true if the pointer is in range
    current++;
    return current < end;
}


bool HashTrieIndexIterator::at_end() {
    return current >= end;
}


std::unique_ptr<EdgeIter> HashTrieIndex::get_iter(uint64_t node_id, bool* interruption_requested) {
    auto query_result = hash_trie->query(node_id);
    return std::make_unique<HashTrieIndexIterator>(interruption_requested, query_result.first, query_result.second);
}


template class Paths::BTreeIndexIterator<3>;
template class Paths::BTreeIndexIterator<4>;

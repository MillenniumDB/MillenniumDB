/*
Index interface for property paths.
*/

/*
Timeout Exceptions:
- B+Tree index handles timeout exceptions in it's own class.
- Trie indexes handle timeout exceptions via the interruption requested arg.
  This behaviour is implemented here in the Iterator and Index classes for Tries.
*/

#pragma once

#include "execution/binding_id_iter/paths/index_provider/trie/edge_trie.h"
#include "execution/binding_id_iter/paths/index_provider/trie/hash_trie.h"
#include "execution/binding_id_iter/paths/index_provider/trie/trie.h"
#include "storage/index/bplus_tree/bplus_tree.h"

namespace Paths {

// Index types
enum class IndexType {
    BTREE,     // B+Tree
    TRIE,      // Trie
    HASH_TRIE, // Hash Trie
    EDGE_TRIE  // Edge Trie (Trails)
};

/*
Index iterator to explore results.
*/
class IndexIterator {
public:
    virtual ~IndexIterator() = default;

    // Iterator interface

    // Obtain current result
    // Calling get() before the first next() or after the iterator is at_end() gives undefined behaviour
    virtual uint64_t get_node() = 0;
    virtual uint64_t get_edge() {throw std::runtime_error("get_edge method is not supported!");}

    // Advance iterator and return true if there is a next result, else do nothing and return false
    virtual bool next() = 0;

    // Check if the iterator has finished (no more results)
    virtual bool at_end() = 0;
};

/*
Index for a specific transition.
Given a current node_id, it returns an iterator for all results that satisfy the transition.
*/
class Index {
public:
    virtual ~Index() = default;

    // Get iterator for current results
    virtual std::unique_ptr<IndexIterator> get_iterator(uint64_t node_id, bool* interruption_requested) = 0;
};

/*
Provides an index for each transition (considering the edge type and direction).
In-memory indexes are materialized after calling the materialize() method.
*/
class IndexProvider {
public:
    virtual ~IndexProvider() = default;

    // Get iterator for a transition defined by an edge type and direction, given the current node
    virtual std::unique_ptr<IndexIterator> get_iterator(uint64_t type_id, bool inverse, uint64_t node_id) = 0;

    // Check if a node exists in the database (using B+Tree)
    virtual bool node_exists(uint64_t node_id) = 0;
};

/*
Default index iterator.
*/
class NullIndexIterator : public IndexIterator {
public:
    // Interface
    uint64_t get_node() override {return 0;}
    bool next() override {return false;}
    bool at_end() override {return true;}
};

/*
B+Tree index iterator.
Assumes B+Trees with ids[2] as the result of interest:
    QuadModel: (type, from/to, to/from, edge) -> ids[2] = to/from
    RDF: (S,P,O) -> ids[2] = O
    RDF: (P,O,S) -> ids[2] = S
*/
class BTreeIndexIterator : public IndexIterator {
private:
    // B+Tree internal iterator
    std::unique_ptr<BptIter<4>> iter;

    // Current result
    std::unique_ptr<Record<4>> current;

    // Whether the iterator is finished or not
    bool finished = false;

public:
    BTreeIndexIterator(std::unique_ptr<BptIter<4>> iter) :
        iter (move(iter)) { }

    // Interface
    uint64_t get_node() override;
    uint64_t get_edge() override;
    bool next() override;
    bool at_end() override;
};

/*
Trie index iterator.
*/
class TrieIndexIterator : public IndexIterator {
private:
    // Pointer to current result
    uint64_t* current;

    // Pointer to final result
    uint64_t* end;

    // Interruption
    bool* interruption_requested;

public:
    TrieIndexIterator(bool* interruption_requested, uint64_t* start, uint64_t* end) :
        current                (start - 1),
        end                    (end),
        interruption_requested (interruption_requested) { }

    // Interface
    uint64_t get_node() override;
    bool next() override;
    bool at_end() override;
};

/*
Trie index.
*/
class TrieIndex : public Index {
private:
    // Trie data structure
    std::unique_ptr<Trie> trie;

public:
    TrieIndex(std::unique_ptr<Trie> trie) :
        trie (move(trie)) { }

    std::unique_ptr<IndexIterator> get_iterator(uint64_t node_id, bool* interruption_requested) override;
};

/*
Edge Trie index iterator.
*/
class EdgeTrieIndexIterator : public IndexIterator {
private:
    // Pointer to current result
    std::pair<uint64_t, uint64_t>* current;

    // Pointer to final result
    std::pair<uint64_t, uint64_t>* end;

    // Interruption
    bool* interruption_requested;

public:
    EdgeTrieIndexIterator(bool* interruption_requested, std::pair<uint64_t, uint64_t>* start, std::pair<uint64_t, uint64_t>* end) :
        current                (start - 1),
        end                    (end),
        interruption_requested (interruption_requested) { }

    // Interface
    uint64_t get_node() override;
    uint64_t get_edge() override;
    bool next() override;
    bool at_end() override;
};

/*
Edge Trie index.
*/
class EdgeTrieIndex : public Index {
private:
    // Trie data structure
    std::unique_ptr<EdgeTrie> trie;

public:
    EdgeTrieIndex(std::unique_ptr<EdgeTrie> trie) :
        trie (move(trie)) { }

    std::unique_ptr<IndexIterator> get_iterator(uint64_t node_id, bool* interruption_requested) override;
};

/*
Hash Trie index iterator.
*/
class HashTrieIndexIterator : public IndexIterator {
private:
    // Pointer to current result
    uint64_t* current;

    // Pointer to final result
    uint64_t* end;

    // Interruption
    bool* interruption_requested;

public:
    HashTrieIndexIterator(bool* interruption_requested, uint64_t* start, uint64_t* end) :
        current                (start - 1),
        end                    (end),
        interruption_requested (interruption_requested) { }

    // Interface
    uint64_t get_node() override;
    bool next() override;
    bool at_end() override;
};

/*
Hash Trie index.
*/
class HashTrieIndex : public Index {
private:
    // Hash Trie data structure
    std::unique_ptr<HashTrie> hash_trie;

public:
    HashTrieIndex(std::unique_ptr<HashTrie> hash_trie) :
        hash_trie (move(hash_trie)) { }

    std::unique_ptr<IndexIterator> get_iterator(uint64_t node_id, bool* interruption_requested) override;
};

/*
Provides indexes for QuadModel.
*/
class QuadModelIndexProvider : public IndexProvider {
private:
    // Store info about assigned indexes for each transition
    std::unordered_map<uint64_t, IndexType> t_info;
    std::unordered_map<uint64_t, IndexType> t_inv_info;

    // Interruption
    bool* interruption_requested;

    // Special Case: B+Tree index is stored on disk
    std::unique_ptr<IndexIterator> get_btree_iterator(uint64_t type_id, bool inverse, uint64_t node_id);

public:
    QuadModelIndexProvider(std::unordered_map<uint64_t, IndexType> t_info,
                           std::unordered_map<uint64_t, IndexType> t_inv_info,
                           bool* interruption_requested) :
        t_info     (move(t_info)),
        t_inv_info (move(t_inv_info)),
        interruption_requested (interruption_requested) { }

    bool node_exists(uint64_t node_id) override;
    std::unique_ptr<IndexIterator> get_iterator(uint64_t type_id, bool inverse, uint64_t node_id) override;
};
} // namespace Paths

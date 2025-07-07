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

#include "query/executor/binding_iter/paths/index_provider/trie/edge_trie.h"
#include "query/executor/binding_iter/paths/index_provider/trie/hash_trie.h"
#include "query/executor/binding_iter/paths/index_provider/trie/trie.h"
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
Iterator interface to explore results.
*/
class EdgeIter {
public:
    virtual ~EdgeIter() = default;

    // Obtain starting node of the current edge
    // Calling before the first next() or after the iterator is at_end() gives undefined behaviour
    virtual uint64_t get_starting_node() = 0;

    // Obtain reached node of the current edge
    // Calling before the first next() or after the iterator is at_end() gives undefined behaviour
    virtual uint64_t get_reached_node() = 0;

    // Obtain predicate node of the current edge
    // Calling before the first next() or after the iterator is at_end() gives undefined behaviour
    virtual uint64_t get_predicate() {
        throw std::runtime_error("get_predicates method is not supported!");
    }

    // get_edge is supported only for quad_model
    virtual uint64_t get_edge() {
        throw std::runtime_error("get_edge method is not supported!");
    }

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
    virtual std::unique_ptr<EdgeIter> get_iter(uint64_t node_id, bool* interruption_requested) = 0;
};

/*
Provides an index for each transition (considering the edge type and direction).
In-memory indexes are materialized after calling the materialize() method.
*/
class IndexProvider {
public:
    virtual ~IndexProvider() = default;

    // Get iterator for a transition defined by an edge type and direction, given the current node
    virtual std::unique_ptr<EdgeIter> get_iter(uint64_t type_id, bool inverse, uint64_t node_id) = 0;

    // Get iterator for a transition defined by an edge type and direction, given the current node
    virtual std::unique_ptr<EdgeIter> get_iter(uint64_t type_id, bool inverse) = 0;

    virtual std::unique_ptr<EdgeIter> get_outgoing(uint64_t node_id) = 0;

    virtual std::unique_ptr<EdgeIter> get_incoming(uint64_t node_id) = 0;

    // Check if a node exists in the database (using B+Tree)
    virtual bool node_exists(uint64_t node_id) = 0;
};

/*
Default index iterator.
*/
class NullIndexIterator : public EdgeIter {
public:
    // Interface
    uint64_t get_starting_node() override {return 0;}
    uint64_t get_reached_node() override {return 0;}
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
template<size_t N>
class BTreeIndexIterator : public EdgeIter {
private:
    // B+Tree internal iterator
    BptIter<N> iter;

    // Current result
    const Record<N>* current;

    size_t starting_node_idx;

    size_t reached_node_idx;

    size_t predicate_idx;

    // Whether the iterator is finished or not
    bool finished = false;

public:
    BTreeIndexIterator(
        BptIter<N>&& iter,
        size_t starting_node_idx,
        size_t reached_node_idx,
        size_t predicate_idx
    ) :
        iter(std::move(iter)),
        starting_node_idx(starting_node_idx),
        reached_node_idx(reached_node_idx),
        predicate_idx(predicate_idx)
    { }

    // Interface
    uint64_t get_starting_node() override;
    uint64_t get_reached_node() override;
    uint64_t get_predicate() override;
    uint64_t get_edge() override;
    bool next() override;
    bool at_end() override;
};

/*
Trie index iterator.
*/
class TrieIndexIterator : public EdgeIter {
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

    uint64_t get_starting_node() override;
    uint64_t get_reached_node() override;
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
        trie (std::move(trie)) { }

    std::unique_ptr<EdgeIter> get_iter(uint64_t node_id, bool* interruption_requested) override;
};

/*
Edge Trie index iterator.
*/
class EdgeTrieIndexIterator : public EdgeIter {
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

    uint64_t get_starting_node() override;
    uint64_t get_reached_node() override;
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
        trie (std::move(trie)) { }

    std::unique_ptr<EdgeIter> get_iter(uint64_t node_id, bool* interruption_requested) override;
};

/*
Hash Trie index iterator.
*/
class HashTrieIndexIterator : public EdgeIter {
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

    uint64_t get_starting_node() override;
    uint64_t get_reached_node() override;
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
        hash_trie (std::move(hash_trie)) { }

    std::unique_ptr<EdgeIter> get_iter(uint64_t node_id, bool* interruption_requested) override;
};

} // namespace Paths

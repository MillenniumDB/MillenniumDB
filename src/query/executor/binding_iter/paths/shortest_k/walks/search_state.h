#pragma once

#include <functional>
#include <ostream>

#include "graph_models/object_id.h"

namespace Paths { namespace ShortestKWalks {

struct SearchState;

// Holds relevant information from previous transitions
struct IterTransition {
    const SearchState* previous;
    bool inverse_direction;
    ObjectId type_id;
    IterTransition* next;

    // only to allow arena allocation
    IterTransition() = default;

    IterTransition(const SearchState* previous, bool inverse_direction, ObjectId type_id) :
        previous(previous),
        inverse_direction(inverse_direction),
        type_id(type_id),
        next(nullptr)
    { }
};

// Iterates over all previous transitions of a state
struct PathIter {
    IterTransition* begin;
    IterTransition* end;
    IterTransition* current;

    PathIter(IterTransition* _begin) :
        begin(_begin),
        end(_begin),
        current(nullptr)
    { }

    // only used to construct a PathIter to search in visited
    PathIter() = default;

    void add(IterTransition* iter_transition);
    void start_enumeration();
    bool next();

private:
    void reset();
};

struct SearchState {
    const ObjectId node_id;
    const uint32_t automaton_state;
    const uint32_t distance;
    mutable uint32_t path_count;
    mutable PathIter path_iter;

    // only used to construct a SearchState to search in visited
    SearchState(ObjectId node_id, uint32_t automaton_state, uint32_t distance) :
        node_id(node_id),
        automaton_state(automaton_state),
        distance(distance),
        path_count(1),
        path_iter(PathIter())
    { }

    SearchState(
        ObjectId node_id,
        uint32_t automaton_state,
        uint32_t distance,
        uint32_t path_count,
        IterTransition* iter_transition
    ) :
        node_id(node_id),
        automaton_state(automaton_state),
        distance(distance),
        path_count(path_count),
        path_iter(PathIter(iter_transition))
    { }

    bool operator==(const SearchState& other) const
    {
        return automaton_state == other.automaton_state && node_id == other.node_id
            && distance == other.distance;
    }

    void print(
        std::ostream& os,
        std::function<void(std::ostream& os, ObjectId)> print_node,
        std::function<void(std::ostream& os, ObjectId, bool)> print_edge,
        bool begin_at_left
    ) const;
};

struct NodeState {
    uint64_t node;
    uint64_t state;

    NodeState(uint64_t node, uint64_t state) :
        node(node),
        state(state)
    { }

    bool operator==(const NodeState& other) const
    {
        return node == other.node && state == other.state;
    }
};

struct NodeStateHasher {
    std::size_t operator()(const NodeState& s) const
    {
        return s.node ^ s.state;
    }
};

struct Solution {
    uint64_t count;

    // this is not in visited
    SearchState search_state;

    Solution(const SearchState* reached, IterTransition* iter_transition, uint64_t count) :
        count(count),
        search_state(
            reached->node_id,
            reached->automaton_state,
            reached->distance,
            reached->path_count,
            iter_transition
        )
    { }

    Solution(const SearchState* reached, uint64_t count) :
        count(count),
        search_state(reached->node_id, reached->automaton_state, reached->distance)
    { }
};

}} // namespace Paths::ShortestKWalks

// For unordered set
template<>
struct std::hash<Paths::ShortestKWalks::SearchState> {
    std::size_t operator()(const Paths::ShortestKWalks::SearchState& s) const
    {
        return s.automaton_state ^ s.node_id.id ^ s.distance;
    }
};

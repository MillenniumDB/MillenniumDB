#pragma once

#include <functional>
#include <ostream>

#include "graph_models/object_id.h"

namespace Paths { namespace ShortestKGroupsWalks {

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
    mutable PathIter path_iter;

    // only used to construct a SearchState to search in visited
    SearchState(ObjectId node_id, uint32_t automaton_state, uint32_t distance) :
        node_id(node_id),
        automaton_state(automaton_state),
        distance(distance),
        path_iter(PathIter())
    { }

    SearchState(
        ObjectId node_id,
        uint32_t automaton_state,
        uint32_t distance,
        IterTransition* iter_transition
    ) :
        node_id(node_id),
        automaton_state(automaton_state),
        distance(distance),
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

struct GroupsInfo {
    uint64_t num_groups;

    uint64_t last_group_len;

    GroupsInfo(uint64_t num_groups, uint64_t last_group_len) :
        num_groups(num_groups),
        last_group_len(last_group_len)
    { }

    GroupsInfo() = default;
};

struct NodeStateHasher {
    std::size_t operator()(const NodeState& s) const
    {
        return s.node ^ s.state;
    }
};

struct Solution {
    uint64_t num_groups;

    uint64_t last_group_len;

    // this is not in visited
    SearchState search_state;

    Solution(
        const SearchState* reached,
        IterTransition* iter_transition,
        uint64_t num_groups,
        uint64_t last_group_len
    ) :
        num_groups(num_groups),
        last_group_len(last_group_len),
        search_state(reached->node_id, reached->automaton_state, reached->distance, iter_transition)
    { }

    Solution(const SearchState* reached, uint64_t num_groups, uint64_t last_group_len) :
        num_groups(num_groups),
        last_group_len(last_group_len),
        search_state(reached->node_id, reached->automaton_state, reached->distance)
    { }
};

}} // namespace Paths::ShortestKGroupsWalks

// For unordered set
template<>
struct std::hash<Paths::ShortestKGroupsWalks::SearchState> {
    std::size_t operator()(const Paths::ShortestKGroupsWalks::SearchState& s) const
    {
        return s.automaton_state ^ s.node_id.id ^ s.distance;
    }
};

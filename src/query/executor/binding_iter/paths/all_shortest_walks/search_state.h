#pragma once

#include <functional>
#include <ostream>

#include "graph_models/object_id.h"

namespace Paths { namespace AllShortest {

struct SearchState;

// Holds relevant information from previous transitions
struct TransitionLinkedList {
    const SearchState* previous;
    bool inverse_direction;
    ObjectId type_id;
    TransitionLinkedList* next;

    // Only to be able to allocate
    TransitionLinkedList() = default;

    TransitionLinkedList(const SearchState* previous, bool inverse_direction, ObjectId type_id) :
        previous(previous),
        inverse_direction(inverse_direction),
        type_id(type_id),
        next(nullptr)
    { }
};

struct SearchState {
    const ObjectId node_id;
    const uint32_t automaton_state;
    const uint32_t distance;
    mutable TransitionLinkedList* begin;
    mutable TransitionLinkedList* end;

    // used for enumeration
    mutable TransitionLinkedList* current_transition;

    SearchState(ObjectId node_id, uint32_t automaton_state, uint32_t distance) :
        node_id(node_id),
        automaton_state(automaton_state),
        distance(distance),
        begin(nullptr),
        end(nullptr),
        current_transition(nullptr)
    { }

    SearchState(
        ObjectId node_id,
        uint32_t automaton_state,
        uint32_t distance,
        TransitionLinkedList* iter_transition
    ) :
        node_id(node_id),
        automaton_state(automaton_state),
        distance(distance),
        begin(iter_transition),
        end(iter_transition),
        current_transition(nullptr)
    { }

    void add_transition(TransitionLinkedList*) const;

    void start_enumeration() const;

    bool next() const;

    bool operator==(const SearchState& other) const
    {
        return automaton_state == other.automaton_state && node_id == other.node_id;
    }

    void print(
        std::ostream& os,
        std::function<void(std::ostream& os, ObjectId)> print_node,
        std::function<void(std::ostream& os, ObjectId, bool)> print_edge,
        bool begin_at_left
    ) const;
};

}} // namespace Paths::AllShortest

// For unordered set
template<>
struct std::hash<Paths::AllShortest::SearchState> {
    std::size_t operator()(const Paths::AllShortest::SearchState& lhs) const
    {
        return lhs.automaton_state ^ lhs.node_id.id;
    }
};

#pragma once

#include <memory>

#include "base/ids/object_id.h"

namespace Paths { namespace AllShortest {

struct SearchState;
struct PathIter;

/* Used to hold the relevant information of previous transitions */
struct IterTransition {
    const SearchState* const previous;
    bool inverse_direction;
    ObjectId type_id;
    std::unique_ptr<IterTransition> next;
    // TODO: add edge_id ?

    IterTransition(const SearchState* const previous, bool inverse_direction, ObjectId type_id) :
        previous          (previous),
        inverse_direction (inverse_direction),
        type_id           (type_id),
        next              (nullptr) { }
};


/* Allows to iterate over all previous transitions of a state */
struct PathIter {
    std::unique_ptr<IterTransition> begin;
    IterTransition* end;
    IterTransition* current;

    PathIter() :
        begin   (nullptr),
        end     (nullptr),
        current (nullptr) { }

    PathIter(const SearchState* const previous, bool inverse_direction, ObjectId type_id) :
        begin   (std::make_unique<IterTransition>(previous, inverse_direction, type_id)),
        end     (begin.get()),
        current (nullptr) { }

    void add(const SearchState* const previous, bool inverse_direction, ObjectId type_id);

    void get_path(ObjectId node_id, std::ostream& os) const;

    void start_enumeration();

    bool next();
};


struct SearchState {
    const uint32_t automaton_state;
    const ObjectId node_id;
    const uint32_t distance;
    mutable PathIter path_iter;

    SearchState(uint32_t automaton_state, ObjectId node_id, uint32_t distance) :
        automaton_state (automaton_state),
        node_id         (node_id),
        distance        (distance),
        path_iter       (PathIter()) { }

    SearchState(uint32_t           automaton_state,
                ObjectId           node_id,
                ObjectId           type_id,
                const SearchState* previous,
                bool               inverse_direction,
                uint32_t           distance) :
    automaton_state (automaton_state),
    node_id         (node_id),
    distance        (distance),
    path_iter       (PathIter(previous, inverse_direction, type_id)) { }

    bool operator==(const SearchState& other) const {
        return automaton_state == other.automaton_state && node_id == other.node_id;
    }
};

struct SearchStateHasher { // TODO: declare as template of std::hash
    std::size_t operator() (const SearchState& lhs) const {
      return lhs.automaton_state ^ lhs.node_id.id;
    }
};
}} // namespace Paths::AllShortest

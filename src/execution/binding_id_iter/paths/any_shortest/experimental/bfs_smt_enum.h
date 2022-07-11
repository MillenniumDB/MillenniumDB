#pragma once

#include <array>
#include <memory>
#include <queue>
#include <variant>

#include "base/binding/binding_id_iter.h"
#include "base/thread/thread_info.h"
#include "parser/query/paths/automaton/smt_automaton.h"
#include "execution/binding_id_iter/paths/any_shortest/experimental/search_state_smt.h"
#include "execution/binding_id_iter/scan_ranges/scan_range.h"
#include "storage/index/bplus_tree/bplus_tree.h"
#include "third_party/robin_hood/robin_hood.h"

namespace Paths { namespace AnyShortest {

/*
BFSIterEnum enumerates paths from or to a specifc node, using BFS algorithm.

Similar to BFSSimpleEnum but it returns results earlier (when it puts into
the queue, and BFSSimpleEnum returns when it comes out the queue).
The name of the class comes out because, compared with BFSSimpleEnum, we need
to keep a B+Tree Iter as calss member to be able to reanudate the search after
next returned.
*/
class SMTEnum : public BindingIdIter {
private:
    // Attributes determined in the constuctor
    ThreadInfo*  thread_info;
    VarId        path_var;
    Id           start;
    VarId        end;
    SMTAutomaton automaton;

    // Attributes determined in begin
    BindingId* parent_binding;
    bool first_next = true;

    // Ranges to search in BPT. They are not local variables because some positions are reused.
    std::array<uint64_t, 4> min_ids;
    std::array<uint64_t, 4> max_ids;

    // Structs for BFS
    robin_hood::unordered_node_set<SearchStateSMT> visited;
    // open stores a pointer to a SearchState stored in visited
    // that allows to avoid use visited.find to get a pointer and
    // use the state extracted of the open directly.
    std::queue<const SearchStateSMT*> open;

    // Stores the children of state in expansion
    std::unique_ptr<BptIter<4>> iter;
    // The index of the transition that set_iter method uses to
    // construct iter attribute.
    uint32_t current_transition = 0;

    // Statistics
    uint_fast32_t results_found = 0;
    uint_fast32_t bpt_searches = 0;

    robin_hood::unordered_node_set<SearchStateSMT>::iterator
      current_state_has_next(const SearchStateSMT* current_state);

    // Set iter attribute that give all states that connects with
    // current_state with label of a specific transition
    void set_iter(const SearchStateSMT* current_state);

    bool eval_formula(const SMTTransition& transition, ObjectId obj_id) const;

public:
    SMTEnum(ThreadInfo*  thread_info,
            VarId        path_var,
            Id           start,
            VarId        end,
            SMTAutomaton automaton);

    void analyze(std::ostream& os, int indent = 0) const override;
    void begin(BindingId& parent_binding) override;
    void reset() override;
    void assign_nulls() override;
    bool next() override;
};
}} // namespace Paths::AnyShortest

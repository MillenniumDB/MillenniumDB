#pragma once

#include <memory>
#include <queue>
#include <type_traits>

#include "query/executor/binding_iter.h"
#include "query/id.h"
#include "query/executor/binding_iter/paths/all_shortest_count/search_state.h"
#include "query/executor/binding_iter/paths/index_provider/path_index.h"
#include "query/parser/paths/automaton/rpq_automaton.h"
#include "third_party/robin_hood/robin_hood.h"

namespace Paths { namespace AllShortestCount {

// Holds necessary information for nodes reached with final states
struct ResultInfo {
    uint64_t count;
    uint64_t distance;

    ResultInfo(uint64_t count, uint64_t distance) :
        count    (count),
        distance (distance) { }
};

// Dummy structure for template usage
class DummyMap {
public:
    static inline void clear() { }
    static inline std::pair<uint64_t, ResultInfo>* end() {return nullptr;}
    template <class T>
    static inline void erase(T) { }
    static inline std::pair<uint64_t, ResultInfo>* find(uint64_t) {return nullptr;}
    static inline void insert(std::pair<uint64_t, ResultInfo>) { }
};

/*
AllShortestCount::BFSEnum finds all reachable nodes according to the RPQ from a fixed start
node, and then returns the count of shortest paths between the start node and each of these
reached nodes.
*/
template <bool MULTIPLE_FINAL>
class BFSEnum : public BindingIter {
private:
    // Attributes determined in the constructor
    VarId         path_var;
    Id            start;
    VarId         end;
    const RPQ_DFA automaton;
    std::unique_ptr<IndexProvider> provider;

    // where the results will be written, determined in begin()
    Binding* parent_binding;

    // Remembers which states were explored. A structure with pointer stability is required
    robin_hood::unordered_node_set<SearchState> visited;

    // Queue for BFS. Pointers point to the states in visited
    std::queue<const SearchState*> open;

    // true in the first call of next() and after a reset()
    bool first_next = true;

    // Statistics
    uint_fast32_t results_found = 0;
    uint_fast32_t idx_searches = 0;

    // Template type for storing reached nodes with a final state, and their necessary info: NodeId -> Info
    typename std::conditional<MULTIPLE_FINAL,
                              robin_hood::unordered_map<uint64_t, ResultInfo>,
                              DummyMap>::type reached_final;

public:
    BFSEnum(
        VarId                          path_var,
        Id                             start,
        VarId                          end,
        RPQ_DFA                        automaton,
        std::unique_ptr<IndexProvider> provider
    ) :
        path_var      (path_var),
        start         (start),
        end           (end),
        automaton     (automaton),
        provider      (std::move(provider)) { }

    // Explore neighbors from current state
    void explore_neighbors(const SearchState& current_state);

    void analyze(std::ostream& os, int indent = 0) const override;
    void begin(Binding& parent_binding) override;
    void reset() override;
    bool next() override;

    void assign_nulls() override {
        parent_binding->add(end, ObjectId::get_null());
        parent_binding->add(path_var, ObjectId::get_null());
    }
};
}} // namespace Paths::AllShortestCount

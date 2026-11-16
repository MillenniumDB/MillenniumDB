#pragma once

#include "graph_models/object_id.h"
#include "query/executor/binding_iter/gql/path_binding_iter.h"
#include "query/var_id.h"
#include <memory>
#include <queue>

class Repetition : public PathBindingIter {
public:
    static constexpr uint64_t MAX_REPETITIONS = 50;

    Repetition(std::unique_ptr<PathBindingIter> child_iter, uint64_t lower, std::optional<uint64_t> upper);

    void begin(Binding& binding) override;

    // set_left_boundary or set_right_boundary must be called before next
    void set_left_boundary(const PathElement& left_boundary) override;
    void set_right_boundary(ObjectId right_boundary) override;
    void reset() override;
    std::unique_ptr<PathBinding> next() override;

    void assign_nulls() override;
    void assign_empty() override;

    void print(std::ostream& os, int indent, bool stats) override;

private:
    std::unique_ptr<PathBindingIter> child_iter;

    bool pending = false;
    // the elements reached, each with its number of iterations capped at the lower bound
    std::set<std::pair<PathElement, uint64_t>> visited;
    std::queue<std::pair<PathBinding, uint64_t>> queue;
    uint64_t cur_repetitions = 0;
    PathBinding cur_list;

    uint64_t lower;
    uint64_t upper;

    // set when a search starts, as a Repetition may be asked for a result before it has a boundary
    bool return_zero_length = false;

    PathElement parent_boundary = PathElement::node(ObjectId(0));

    Binding* parent_binding;

    // Starts the search at parent_boundary
    void start_search();

    // Concatenates an iteration to cur_list; returns the new path if it is a result
    std::unique_ptr<PathBinding> add_iteration(const PathBinding& iteration);
};

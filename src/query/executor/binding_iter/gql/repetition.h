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
    void set_left_boundary(ObjectId left_boundary) override;
    void set_right_boundary(ObjectId right_boundary) override;
    void reset() override;
    std::unique_ptr<PathBinding> next() override;

    void assign_nulls() override;
    void assign_empty() override;

    void print(std::ostream& os, int indent, bool stats) override;

private:
    std::unique_ptr<PathBindingIter> child_iter;

    bool pending = false;
    std::set<ObjectId> visited;
    std::queue<std::pair<PathBinding, uint64_t>> queue;
    uint64_t cur_repetitions = 0;
    PathBinding cur_list;

    uint64_t lower;
    uint64_t upper;

    bool return_zero_length = false;

    ObjectId parent_boundary = ObjectId(0);

    Binding* parent_binding;
};

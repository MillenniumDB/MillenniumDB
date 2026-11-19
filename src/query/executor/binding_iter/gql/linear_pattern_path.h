#pragma once

#include <optional>
#include <set>

#include "path_binding_iter.h"
#include "query/executor/binding_iter.h"

namespace GQL {

class LinearPatternPath : public PathBindingIter {
public:
    // variables_in_pattern: node, (edge, direction, node)*
    LinearPatternPath(std::unique_ptr<BindingIter>&& iter, std::vector<VarId>&& variables_in_pattern);

    void begin(Binding& binding) override;
    void reset() override;
    void set_left_boundary(const PathElement& boundary) override;
    void set_right_boundary(ObjectId boundary) override;
    std::unique_ptr<PathBinding> next() override;
    void assign_nulls() override;
    void assign_empty() override;

    // Variables of the pattern that are bound before it: assign_nulls leaves them alone
    void set_outer_vars(std::set<VarId>&& vars);

    // MATCH COLLAPSE: nodes of the pattern that are not elements of the path
    void set_off_path_vars(std::set<VarId>&& vars);

    void print(std::ostream& os, int indent = 0, bool stats = false) override;

private:
    std::unique_ptr<BindingIter> child_iter;
    const std::vector<VarId> variables_in_pattern;
    const uint64_t pattern_size;
    std::set<VarId> outer_vars;
    std::set<VarId> off_path_vars;
    Binding* parent_binding;

    // set by an edge·edge join: the pattern must start with this edge
    std::optional<PathElement> required_first_edge;

    // the element that the current result of the scan has at position i of variables_in_pattern
    PathElement node_at(size_t i) const;
    PathElement edge_at(size_t i) const;
};

} // namespace GQL

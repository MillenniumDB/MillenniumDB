
#include "optional_node.h"

#include <algorithm>

#include "base/ids/var_id.h"
#include "index_left_outer_join.h"

using namespace std;

OptionalNode::OptionalNode(unique_ptr<BindingIdIter> _graph_pattern,
                           std::vector<std::unique_ptr<BindingIdIter>> children) :
    graph_pattern (move(_graph_pattern))
{
    for (auto& child : children) {
        graph_pattern = make_unique<IndexLeftOuterJoin>(move(graph_pattern), move(child));
    }
}


void OptionalNode::begin(BindingId& parent_binding) {
    graph_pattern->begin(parent_binding);
}


bool OptionalNode::next() {
    return graph_pattern->next();
}


void OptionalNode::reset() {
    graph_pattern->reset();
}


void OptionalNode::assign_nulls() {
    graph_pattern->assign_nulls();
}


void OptionalNode::analyze(std::ostream& os, int indent) const {
    graph_pattern->analyze(os, indent);
}

template class std::unique_ptr<OptionalNode>;

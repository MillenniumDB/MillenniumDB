#pragma once

#include <memory>
#include <set>
#include <vector>

#include "query/parser/op/op.h"

namespace MQL {
/*
Simplifies the logical plan generated. This simplifications only happen if the
query has an `OPTIONAL` clause. The simplifications are done by executing a DFS
algorithm over the Optional Nodes, going through their children in order and
saving the variables involved in each node. Since the query is well-defined,
once a variable is assigned it must not be assigned once again, so any node
that does not assign any new variables is eliminated.
*/
class OptimizeOptionalTree : public OpVisitor {
private:
    // these sets are used to avoid having redundant labels/properties inside an OPTIONAL
    std::set<OpLabel>    global_labels;
    std::set<OpProperty> global_properties;

    std::set<VarId>                  global_vars;
    std::vector<std::unique_ptr<Op>> optionals;

    bool delete_current    = false;
    bool move_children_up  = false;
    bool optional_to_match = true;

public:
    void visit(OpBasicGraphPattern&) override;
    void visit(OpGroupBy&) override;
    void visit(OpOptional&) override;
    void visit(OpOrderBy&) override;
    void visit(OpReturn&) override;
    void visit(OpSet&) override;
    void visit(OpWhere&) override;

    void visit(OpCreateTensorStore&) override { }
    void visit(OpCreateTextSearchIndex&) override { }
    void visit(OpDeleteTensors&) override { }
    void visit(OpDescribe&) override { }
    void visit(OpDisjointTerm&) override { }
    void visit(OpDisjointVar&) override { }
    void visit(OpEdge&) override { }
    void visit(OpInsert&) override { }
    void visit(OpInsertTensors&) override { }
    void visit(OpLabel&) override { }
    void visit(OpPath&) override { }
    void visit(OpProperty&) override { }
    void visit(OpShow&) override { }
};
} // namespace MQL
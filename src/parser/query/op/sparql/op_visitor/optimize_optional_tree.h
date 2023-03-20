#pragma once

#include <memory>
#include <set>
#include <vector>

#include "parser/query/op/op_visitor.h"
#include "parser/query/op/sparql/ops.h"

namespace SPARQL {
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
    std::set<Var>                    global_vars;
    std::vector<std::unique_ptr<Op>> optionals;

    bool delete_current    = false;
    bool move_children_up  = false;
    bool optional_to_match = true;

public:
    void visit(OpSelect&)   override;
    void visit(OpWhere&)    override;
    void visit(OpOptional&) override;
    void visit(OpTriples&)  override;
};
} // namespace SPARQL
#pragma once

#include <set>

#include "base/query/var.h"
#include "parser/query/op/op_visitor.h"

namespace SPARQL {
/*
Will throw an exception if the query graph pattern is not well-designed.

A graph pattern is well-designed iff for each of its sub-patterns of the form
(P1 OPTIONAL {P2}) the pattern P1 mentions all the variables of P2 which appear
outside this sub-pattern.

Examples of a Non well-designed pattern:
1) (?x <p> ?y) OPTIONAL { (?x <p> ?z) } OPTIONAL { (?y <p> ?z) }, ?z breaks the rule
2) (?x <p> ?y) OPTIONAL { (?x <p> ?z) OPTIONAL { (?y <p> ?z) } }, ?y breaks the rule

More details explained here:
http://marceloarenas.cl/publications/tods09a.pdf
*/
class CheckWellDesigned : public OpVisitor {
private:
    std::set<Var> parent_vars;
    std::set<Var> global_vars;

public:
    void visit(OpSelect&)   override;
    void visit(OpWhere&)    override;
    void visit(OpOptional&) override;
    void visit(OpTriples&)  override;
};
} // namespace SPARQL

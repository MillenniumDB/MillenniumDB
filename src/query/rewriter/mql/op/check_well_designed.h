#pragma once

#include <set>

#include "query/var_id.h"
#include "query/parser/op/op_visitor.h"

namespace MQL {
/*
Will throw an exception if the query graph pattern is not well-designed.

A graph pattern is well-designed iff for each of its sub-patterns of the form
(P1 OPTIONAL {P2}) the pattern P1 mentions all the variables of P2 which appear
outside this sub-pattern.

Examples of a Non well-designed pattern:
1) (?x)->(?y) OPTIONAL { (?x)->(?z) } OPTIONAL { (?y)->(?z) }, ?z breaks the rule
2) (?x)->(?y) OPTIONAL { (?x)->(?z) OPTIONAL { (?y)->(?z) } }, ?y breaks the rule

More details explained here:
http://marceloarenas.cl/publications/tods09a.pdf
*/
class CheckWellDesigned : public OpVisitor {
private:
    std::set<VarId> parent_vars;
    std::set<VarId> global_vars;

public:
    void visit(OpBasicGraphPattern&) override;
    void visit(OpGroupBy&) override;
    void visit(OpMatch&) override;
    void visit(OpOptional&) override;
    void visit(OpOrderBy&) override;
    void visit(OpReturn&) override;
    void visit(OpSet&) override;
    void visit(OpWhere&) override;
    void visit(OpProjectSimilarity&) override;

    void visit(OpInsert&) override { }
    void visit(OpEdge&) override { }
    void visit(OpDescribe&) override { }
    void visit(OpDisjointTerm&) override { }
    void visit(OpDisjointVar&) override { }
    void visit(OpLabel&) override { }
    void visit(OpPath&) override { }
    void visit(OpProperty&) override { }
    void visit(OpSimilaritySearch&) override { }
};
} // namespace MQL
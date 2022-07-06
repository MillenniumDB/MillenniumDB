#pragma once

#include <set>

#include "parser/query/op_visitor/op_visitor.h"
#include "base/query/var.h"

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
    std::set<Var> parent_vars;
    std::set<Var> global_vars;

public:
    void visit(OpBasicGraphPattern&) override;
    void visit(OpGroupBy&)           override;
    void visit(OpMatch&)             override;
    void visit(OpOptional&)          override;
    void visit(OpOrderBy&)           override;
    void visit(OpReturn&)            override;
    void visit(OpSet&)               override;
    void visit(OpWhere&)             override;

    void visit(OpEdge&)              override { }
    void visit(OpDescribe&)          override { }
    void visit(OpIsolatedTerm&)      override { }
    void visit(OpIsolatedVar&)       override { }
    void visit(OpLabel&)             override { }
    void visit(OpPath&)              override { }
    void visit(OpProperty&)          override { }
};

#pragma once

#include <set>

#include "base/query/var.h"
#include "parser/query/op/op_visitor.h"

namespace SPARQL {
/*
Will throw an exception if the same blank node is mentioned between different scopes.

Examples of patterns with non scoped blank nodes:
1) (?x <p> _:a) OPTIONAL { (?y <p> _:a) }, _:a breaks the rule
2) (?x <p> ?y) OPTIONAL { (?x <p> _:a) } OPTIONAL { (?y <p> _:a) }, _:a breaks the rule
3) (?x <p> _:a) OPTIONAL { (?y <p> _:b) OPTIONAL { (?z <p> _:a) } }, _:a breaks the rule
*/
class CheckScopedBlankNodes : public OpVisitor {
private:
    std::set<Var> mentioned_bnodes;

public:
    void visit(OpSelect&)   override;
    void visit(OpWhere&)    override;
    void visit(OpOptional&) override;
    void visit(OpTriples&)  override;
};
} // namespace SPARQL
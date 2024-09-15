#pragma once

#include <set>

#include "query/var_id.h"
#include "query/query_context.h"
#include "query/parser/op/op_visitor.h"

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
    std::set<VarId> mentioned_blank_nodes;

public:
    void visit(OpConstruct&)            override;
    void visit(OpDescribe&)             override;
    void visit(OpBasicGraphPattern&)    override;
    void visit(OpFilter&)               override;
    void visit(OpJoin&)                 override;
    void visit(OpSemiJoin&)             override;
    void visit(OpMinus&)                override;
    void visit(OpNotExists&)            override;
    void visit(OpUnion&)                override;
    void visit(OpSequence&)             override;
    void visit(OpOptional&)             override;
    void visit(OpOrderBy&)              override;
    void visit(OpGroupBy&)              override;
    void visit(OpHaving&)               override;
    void visit(OpSelect&)               override;
    void visit(OpAsk&)                  override;
    void visit(OpService&)              override;
    void visit(OpBind&)                 override;
    void visit(OpUnitTable&)            override;
    void visit(OpValues&)               override;
};
} // namespace SPARQL

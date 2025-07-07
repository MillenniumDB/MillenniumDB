#pragma once

#include <map>

#include "query/id.h"
#include "query/parser/op/sparql/op_visitor.h"

namespace SPARQL {
/*
Optimize queries with trivial assignment statements

example:
- `BIND(:Q1 AS ?x)`
- `VALUES ?x { :Q1 }`
*/
class ReplaceSingleValues : public OpVisitor {

public:
    void visit(OpConstruct&)         override;
    void visit(OpDescribe&)          override;
    void visit(OpBasicGraphPattern&) override;
    void visit(OpFilter&)            override;
    void visit(OpJoin&)              override;
    void visit(OpSemiJoin&)          override;
    void visit(OpMinus&)             override;
    void visit(OpNotExists&)         override;
    void visit(OpUnion&)             override;
    void visit(OpOptional&)          override;
    void visit(OpOrderBy&)           override;
    void visit(OpFrom&)              override;
    void visit(OpGraph&)             override;
    void visit(OpGroupBy&)           override;
    void visit(OpHaving&)            override;
    void visit(OpSelect&)            override;
    void visit(OpAsk&)               override;
    void visit(OpService&)           override;

    void visit(OpBind&)              override;
    void visit(OpValues&)            override;
    void visit(OpSequence&)          override;
    void visit(OpTriple&)            override;
    void visit(OpPath&)              override;

    void visit(OpEmpty&)             override { }
    void visit(OpProcedure&)         override { }
    void visit(OpShow&)              override { }
    void visit(OpUnitTable&)         override { }

private:
    std::map<VarId, ObjectId> assigned_values;

    bool delete_child;

    void try_replace(Id&);
};
} // namespace SPARQL

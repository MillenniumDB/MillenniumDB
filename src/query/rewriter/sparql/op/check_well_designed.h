#pragma once

#include <set>

#include "query/parser/op/sparql/op_visitor.h"
#include "query/var_id.h"

namespace SPARQL {

class CheckWellDesigned : public OpVisitor {
public:
    void visit(OpConstruct&) override;
    void visit(OpDescribe&) override;
    void visit(OpBasicGraphPattern&) override;
    void visit(OpFilter&) override;
    void visit(OpJoin&) override;
    void visit(OpSemiJoin&) override;
    void visit(OpMinus&) override;
    void visit(OpNotExists&) override;
    void visit(OpUnion&) override;
    void visit(OpOptional&) override;
    void visit(OpOrderBy&) override;
    void visit(OpFrom&) override;
    void visit(OpGraph&) override;
    void visit(OpGroupBy&) override;
    void visit(OpHaving&) override;
    void visit(OpProcedure&) override;
    void visit(OpSelect&) override;
    void visit(OpSequence&) override;
    void visit(OpAsk&) override;
    void visit(OpService&) override;
    void visit(OpBind&) override;
    void visit(OpValues&) override;

    void visit(OpEmpty&) override { }
    void visit(OpPath&) override { }
    void visit(OpShow&) override { }
    void visit(OpTriple&) override { }
    void visit(OpUnitTable&) override { }

    void visit(OpCreateHNSWIndex&) override { }
    void visit(OpCreateTextIndex&) override { }
    void visit(OpDeleteData&) override { }
    void visit(OpInsertData&) override { }
    void visit(OpUpdate&) override { }

    bool is_well_designed = true;

private:
    std::set<VarId> all_vars;
    std::set<VarId> seen_vars;
};
} // namespace SPARQL

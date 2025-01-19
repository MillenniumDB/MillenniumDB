#pragma once

#include <set>

#include "query/var_id.h"
#include "query/query_context.h"
#include "query/parser/op/op_visitor.h"

namespace SPARQL {

class CheckWellDesigned : public OpVisitor {
public:
    void visit(OpConstruct&)            override;
    void visit(OpDescribe&)             override;
    void visit(OpBasicGraphPattern&)    override;
    void visit(OpEmpty&)                override;
    void visit(OpFilter&)               override;
    void visit(OpJoin&)                 override;
    void visit(OpSemiJoin&)             override;
    void visit(OpMinus&)                override;
    void visit(OpNotExists&)            override;
    void visit(OpUnion&)                override;
    void visit(OpOptional&)             override;
    void visit(OpOrderBy&)              override;
    void visit(OpFrom&)                 override;
    void visit(OpGraph&)                override;
    void visit(OpGroupBy&)              override;
    void visit(OpHaving&)               override;
    void visit(OpSelect&)               override;
    void visit(OpSequence&)             override;
    void visit(OpAsk&)                  override;
    void visit(OpService&)              override;
    void visit(OpBind&)                 override;
    void visit(OpUnitTable&)            override;
    void visit(OpValues&)               override;
    void visit(OpShow&)                 override;

    bool is_well_designed = true;

private:
    std::set<VarId> all_vars;
    std::set<VarId> seen_vars;
//     std::set<VarId> declared_path_vars;
//     std::set<VarId> group_vars;

};
} // namespace SPARQL

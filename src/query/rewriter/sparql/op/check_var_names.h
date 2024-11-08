#pragma once

#include <set>

#include "query/var_id.h"
#include "query/query_context.h"
#include "query/parser/op/op_visitor.h"

namespace SPARQL {
/*
   Will throw an exception if a variable is not in the current scope.
        - Select: Select Vars are in the scope of the child op.
        - Filter: Scope Vars in the exprs are in the scope of the filtered op.
        - GroupBy: All vars in exprs are in the scope of the vars.
   Also check that every path variable is an unique variable
*/
class CheckVarNames : public OpVisitor {
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
    void visit(OpGroupBy&)              override;
    void visit(OpHaving&)               override;
    void visit(OpSelect&)               override;
    void visit(OpSequence&)             override;
    void visit(OpAsk&)                  override;
    void visit(OpService&)              override;
    void visit(OpBind&)                 override;
    void visit(OpUnitTable&)            override;
    void visit(OpValues&)               override;

private:
    std::set<VarId> declared_vars;
    std::set<VarId> declared_path_vars;
    std::set<VarId> group_vars;

    // Used to handle common functionality of SELECTs and SUB-SELECTs
    void handle_select(OpSelect& op_select);
};
} // namespace SPARQL

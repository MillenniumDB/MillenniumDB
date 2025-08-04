#pragma once

#include <set>
#include <vector>

#include "query/parser/op/gql/op_visitor.h"
#include "query/var_id.h"

namespace GQL {
class CheckVarExistence : public OpVisitor {
public:
    std::set<VarId> variables;
    std::set<VarId> let_variables;
    std::set<VarId> op_return_vars;
    std::set<VarId> group_vars;

    void check_expr_variables(const std::set<VarId>& expr_variables);

    virtual void visit(OpGraphPattern& op);
    virtual void visit(OpBasicGraphPattern& op);
    virtual void visit(OpGraphPatternList& op);
    virtual void visit(OpNode& op);
    virtual void visit(OpEdge& op);
    virtual void visit(OpWhere& op);
    virtual void visit(OpReturn& op);
    virtual void visit(OpPathUnion& op);
    virtual void visit(OpRepetition& op);
    virtual void visit(OpLinearPattern& op);
    virtual void visit(OpFilter& op);
    virtual void visit(OpLet& op);
    virtual void visit(OpOrderBy& op);
    virtual void visit(OpQueryStatements& op);
    virtual void visit(OpGroupBy& op);
};

} // namespace GQL

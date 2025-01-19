#pragma once

#include <memory>
#include <set>

#include "query/parser/expr/expr.h"
#include "query/parser/expr/expr_visitor.h"
#include "query/parser/op/op.h"
#include "query/parser/op/op_visitor.h"
#include "query/var_id.h"

namespace MQL {
/*
Pushes mandatory conditions into OpMatch
Example:
    `MATCH (?x) WHERE ?x.name == "Joe" RETURN ?x` is the same as `MATCH (?x {name:"Joe"}) RETURN ?x`

    expressions like `WHERE ?x.name == "Joe" OR ?x.name == "John"` can't be optimized
*/
class PushWhere : public OpVisitor {
private:
    std::set<VarId> parent_vars;
    std::set<VarId> global_vars;

    bool must_delete_where = false;
    std::unique_ptr<Op> where_child;

    std::vector<std::tuple<VarId, ObjectId, ObjectId, VarId>> fixed_properties;

public:
    void visit(OpGroupBy&) override;
    void visit(OpOrderBy&) override;
    void visit(OpReturn&) override;
    void visit(OpSet&) override;
    void visit(OpWhere&) override;

    void visit(OpBasicGraphPattern&) override { }
    void visit(OpCreateTensorStore&) override { }
    void visit(OpCreateTextSearchIndex&) override { }
    void visit(OpDeleteTensors&) override { }
    void visit(OpDescribe&) override { }
    void visit(OpDisjointTerm&) override { }
    void visit(OpDisjointVar&) override { }
    void visit(OpEdge&) override { }
    void visit(OpInsert&) override { }
    void visit(OpInsertTensors&) override { }
    void visit(OpLabel&) override { }
    void visit(OpOptional&) override { }
    void visit(OpPath&) override { }
    void visit(OpProperty&) override { }
    void visit(OpShow&) override { }
};

class PushWhereExpr : public ExprVisitor {
private:
public:
    bool can_remove = true;

    std::unique_ptr<Expr> remaining_expr;

    std::vector<std::tuple<VarId, ObjectId, ObjectId, VarId>>& fixed_properties;

    PushWhereExpr(std::vector<std::tuple<VarId, ObjectId, ObjectId, VarId>>& fixed_properties) :
        fixed_properties(fixed_properties)
    { }

    void visit(ExprConstant&) override { }
    void visit(ExprVar&) override { }
    void visit(ExprVarProperty&) override { }

    void visit(ExprAddition&) override;
    void visit(ExprDivision&) override;
    void visit(ExprModulo&) override;
    void visit(ExprMultiplication&) override;
    void visit(ExprSubtraction&) override;
    void visit(ExprUnaryMinus&) override;
    void visit(ExprUnaryPlus&) override;
    void visit(ExprEquals&) override;
    void visit(ExprGreaterOrEquals&) override;
    void visit(ExprGreater&) override;
    void visit(ExprIs&) override;
    void visit(ExprLessOrEquals&) override;
    void visit(ExprLess&) override;
    void visit(ExprNotEquals&) override;
    void visit(ExprAnd&) override;
    void visit(ExprNot&) override;
    void visit(ExprOr&) override;
    void visit(ExprRegex&) override;
    void visit(ExprTensorDistance&) override;
    void visit(ExprTextSearch&) override;
};
} // namespace MQL
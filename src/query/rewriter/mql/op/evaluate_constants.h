#pragma once

#include <memory>

#include "query/executor/binding.h"
#include "query/parser/expr/mql/expr.h"
#include "query/parser/expr/mql/expr_visitor.h"
#include "query/parser/op/mql/op_visitor.h"
#include "query/update/mql/update_action_visitor.h"

namespace MQL {

class EvaluateConstants : public OpVisitor {
public:
    void visit(OpCall&) override;
    void visit(OpLet&) override;
    void visit(OpGroupBy&) override;
    void visit(OpHaving&) override;
    void visit(OpOptional&) override;
    void visit(OpOrderBy&) override;
    void visit(OpReturn&) override;
    void visit(OpSequence&) override;
    void visit(OpUpdate&) override;
    void visit(OpWhere&) override;

    void visit(OpBasicGraphPattern&) override { }
    void visit(OpUnitTable&) override { }
    void visit(OpDescribe&) override { }
    void visit(OpShow&) override { }
};

class EvaluateConstantsExpr : public ExprVisitor {
public:
    void visit_and_try_eval_expr(std::unique_ptr<Expr>& expr);

private:
    Binding binding;

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
    void visit(ExprCosineSimilarity&) override;
    void visit(ExprCosineDistance&) override;
    void visit(ExprManhattanDistance&) override;
    void visit(ExprEuclideanDistance&) override;
    void visit(ExprEditDistance&) override;
    void visit(ExprNormalize&) override;
    void visit(ExprStr&) override;
    void visit(ExprLabels&) override;
    void visit(ExprType&) override;
    void visit(ExprProperties&) override;

    void visit(ExprAggAvg&) override;
    void visit(ExprAggCountAll&) override;
    void visit(ExprAggCount&) override;
    void visit(ExprAggMax&) override;
    void visit(ExprAggMin&) override;
    void visit(ExprAggSum&) override;
};

class EvaluateConstantsUpdateAction : public UpdateActionVisitor {
public:
    void visit(InsertPropertyExpr&) override;

    void visit(InsertNode&) override { }
    void visit(InsertLabel&) override { }
    void visit(SetLabelOrType&) override { }
    void visit(InsertProperty&) override { }
    void visit(DeleteProperty&) override { }
    void visit(DeleteLabel&) override { }
    void visit(InsertEdge&) override { }
    void visit(DeleteObject&) override { }
    void visit(CreateTextIndex&) override { }
    void visit(CreateHNSWIndex&) override { }
};
} // namespace MQL

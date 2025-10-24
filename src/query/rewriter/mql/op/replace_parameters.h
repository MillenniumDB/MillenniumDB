#pragma once

#include <map>
#include <memory>

#include <boost/container/flat_set.hpp>

#include "graph_models/object_id.h"
#include "query/id.h"
#include "query/parser/expr/mql/expr.h"
#include "query/parser/expr/mql/expr_visitor.h"
#include "query/parser/op/mql/op_visitor.h"
#include "query/var_id.h"

namespace MQL {

class ReplaceParameters : public OpVisitor {
public:
    ReplaceParameters(const std::map<VarId, ObjectId>& parameters_) :
        parameters { parameters_ }
    { }

    void visit(OpBasicGraphPattern&) override;
    void visit(OpCall&) override;
    void visit(OpLet&) override;
    void visit(OpGroupBy&) override;
    void visit(OpOptional&) override;
    void visit(OpOrderBy&) override;
    void visit(OpReturn&) override;
    void visit(OpSequence&) override;
    void visit(OpWhere&) override;

    void visit(OpDisjointTerm&) override;
    void visit(OpDisjointVar&) override;
    void visit(OpEdge&) override;
    void visit(OpLabel&) override;
    void visit(OpPath&) override;
    void visit(OpProperty&) override;

    void visit(OpDescribe&) override { }
    void visit(OpShow&) override { }

    void visit(OpInsert&) override { }
    void visit(OpUpdate&) override { }
    void visit(OpCreateHNSWIndex&) override { }
    void visit(OpCreateTextIndex&) override { }

private:
    Id var_to_parameter(const Id& id);

    const std::map<VarId, ObjectId>& parameters;
};

class ReplaceParametersExpr : public ExprVisitor {
public:
    ReplaceParametersExpr(const std::map<VarId, ObjectId>& parameters_) :
        parameters { parameters_ }
    { }

    void visit_or_replace_parameter(std::unique_ptr<Expr>& expr);

private:
    void visit(ExprConstant&) override { }
    void visit(ExprVar&) override;
    void visit(ExprVarProperty&) override;
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

    void visit(ExprAggAvg&) override;
    void visit(ExprAggCountAll&) override;
    void visit(ExprAggCount&) override;
    void visit(ExprAggMax&) override;
    void visit(ExprAggMin&) override;
    void visit(ExprAggSum&) override;

    const std::map<VarId, ObjectId>& parameters;
};
} // namespace MQL

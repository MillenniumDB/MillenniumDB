#pragma once

#include "query/optimizer/quad_model/binding_iter_constructor.h"
#include "query/parser/expr/mql/expr_visitor.h"

namespace MQL {
class ExprPropertyTypes : public ExprVisitor {
private:
    BindingIterConstructor* bic;

public:
    ExprPropertyTypes(BindingIterConstructor* _bic) :
        bic(_bic)
    { }

    // result of visitors.
    // var_with_propertyId, type_bitmap
    std::unique_ptr<std::unordered_map<uint64_t, uint64_t>> propId_and_bitmap;

    void visit(ExprIs&) override;
    void visit(ExprAnd&) override;
    void visit(ExprOr&) override;

    void visit(ExprVar&) override { }
    void visit(ExprVarProperty&) override { }
    void visit(ExprConstant&) override { }
    void visit(ExprAddition&) override { }
    void visit(ExprDivision&) override { }
    void visit(ExprModulo&) override { }
    void visit(ExprMultiplication&) override { }
    void visit(ExprSubtraction&) override { }
    void visit(ExprUnaryMinus&) override { }
    void visit(ExprUnaryPlus&) override { }
    void visit(ExprEquals&) override { }
    void visit(ExprGreaterOrEquals&) override { }
    void visit(ExprGreater&) override { }
    void visit(ExprLessOrEquals&) override { }
    void visit(ExprLess&) override { }
    void visit(ExprNotEquals&) override { }
    void visit(ExprNot&) override { }
    void visit(ExprLabels&) override { }
    void visit(ExprType&) override { }
    void visit(ExprProperties&) override { }

    void visit(ExprAggAvg&) override { }
    void visit(ExprAggCountAll&) override { }
    void visit(ExprAggCount&) override { }
    void visit(ExprAggMax&) override { }
    void visit(ExprAggMin&) override { }
    void visit(ExprAggSum&) override { }
    void visit(ExprRegex&) override { }
    void visit(ExprDay&) override { }
    void visit(ExprMonth&) override { }
    void visit(ExprYear&) override { }
    void visit(ExprCosineSimilarity&) override { }
    void visit(ExprCosineDistance&) override { }
    void visit(ExprManhattanDistance&) override { }
    void visit(ExprEuclideanDistance&) override { }
    void visit(ExprEditDistance&) override { }
    void visit(ExprNormalize&) override { }
    void visit(ExprStr&) override { }
};
} // namespace MQL

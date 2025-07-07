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

    virtual void visit(ExprVar&) override { }
    virtual void visit(ExprVarProperty&) override { }
    virtual void visit(ExprConstant&) override { }
    virtual void visit(ExprAddition&) override { }
    virtual void visit(ExprDivision&) override { }
    virtual void visit(ExprModulo&) override { }
    virtual void visit(ExprMultiplication&) override { }
    virtual void visit(ExprSubtraction&) override { }
    virtual void visit(ExprUnaryMinus&) override { }
    virtual void visit(ExprUnaryPlus&) override { }
    virtual void visit(ExprEquals&) override { }
    virtual void visit(ExprGreaterOrEquals&) override { }
    virtual void visit(ExprGreater&) override { }
    virtual void visit(ExprLessOrEquals&) override { }
    virtual void visit(ExprLess&) override { }
    virtual void visit(ExprNotEquals&) override { }
    virtual void visit(ExprNot&) override { }

    virtual void visit(ExprAggAvg&) override { }
    virtual void visit(ExprAggCountAll&) override { }
    virtual void visit(ExprAggCount&) override { }
    virtual void visit(ExprAggMax&) override { }
    virtual void visit(ExprAggMin&) override { }
    virtual void visit(ExprAggSum&) override { }
    virtual void visit(ExprRegex&) override { }
    virtual void visit(ExprCosineSimilarity&) override { }
    virtual void visit(ExprCosineDistance&) override { }
    virtual void visit(ExprManhattanDistance&) override { }
    virtual void visit(ExprEuclideanDistance&) override { }
};
} // namespace MQL

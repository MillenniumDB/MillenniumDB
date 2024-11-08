#pragma once

#include <optional>

#include "query/executor/binding_iter/binding_expr/binding_expr.h"
#include "query/optimizer/quad_model/binding_iter_constructor.h"
#include "query/parser/expr/expr.h"
#include "query/parser/expr/expr_visitor.h"


class ExprPropertyTypes : public ExprVisitor {
private:
    MQL::BindingIterConstructor* bic;

public:
    ExprPropertyTypes(MQL::BindingIterConstructor* _bic) :
        bic(_bic)
    { }


    // result of visitors.
    // var_with_propertyId, type_bitmap
    std::unique_ptr<std::unordered_map<uint64_t, uint64_t>> propId_and_bitmap;


    void visit(MQL::ExprIs&) override;
    void visit(MQL::ExprAnd&) override;
    void visit(MQL::ExprOr&) override;

    virtual void visit(MQL::ExprVar&) override { }
    virtual void visit(MQL::ExprVarProperty&) override { }
    virtual void visit(MQL::ExprConstant&) override { }
    virtual void visit(MQL::ExprAddition&) override { }
    virtual void visit(MQL::ExprDivision&) override { }
    virtual void visit(MQL::ExprModulo&) override { }
    virtual void visit(MQL::ExprMultiplication&) override { }
    virtual void visit(MQL::ExprSubtraction&) override { }
    virtual void visit(MQL::ExprUnaryMinus&) override { }
    virtual void visit(MQL::ExprUnaryPlus&) override { }
    virtual void visit(MQL::ExprEquals&) override { }
    virtual void visit(MQL::ExprGreaterOrEquals&) override { }
    virtual void visit(MQL::ExprGreater&) override { }
    virtual void visit(MQL::ExprLessOrEquals&) override { }
    virtual void visit(MQL::ExprLess&) override { }
    virtual void visit(MQL::ExprNotEquals&) override { }
    virtual void visit(MQL::ExprNot&) override { }

    virtual void visit(MQL::ExprAggAvg&) override { }
    virtual void visit(MQL::ExprAggCountAll&) override { }
    virtual void visit(MQL::ExprAggCount&) override { }
    virtual void visit(MQL::ExprAggMax&) override { }
    virtual void visit(MQL::ExprAggMin&) override { }
    virtual void visit(MQL::ExprAggSum&) override { }
    virtual void visit(MQL::ExprRegex&) override { }
};

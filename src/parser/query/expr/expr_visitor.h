#pragma once

class ExprVar;
class ExprVarProperty;
class ExprConstant;
class ExprAddition;
class ExprDivision;
class ExprModulo;
class ExprMultiplication;
class ExprSubtraction;
class ExprUnaryMinus;
class ExprUnaryPlus;
class ExprEquals;
class ExprGreaterOrEquals;
class ExprGreater;
class ExprIs;
class ExprLessOrEquals;
class ExprLess;
class ExprNotEquals;
class ExprAnd;
class ExprNot;
class ExprOr;

class ExprVisitor {
public:
    virtual ~ExprVisitor() = default;

    virtual void visit(ExprVar&) = 0;
    virtual void visit(ExprVarProperty&) = 0;
    virtual void visit(ExprConstant&) = 0;
    virtual void visit(ExprAddition&) = 0;
    virtual void visit(ExprDivision&) = 0;
    virtual void visit(ExprModulo&) = 0;
    virtual void visit(ExprMultiplication&) = 0;
    virtual void visit(ExprSubtraction&) = 0;
    virtual void visit(ExprUnaryMinus&) = 0;
    virtual void visit(ExprUnaryPlus&) = 0;
    virtual void visit(ExprEquals&) = 0;
    virtual void visit(ExprGreaterOrEquals&) = 0;
    virtual void visit(ExprGreater&) = 0;
    virtual void visit(ExprIs&) = 0;
    virtual void visit(ExprLessOrEquals&) = 0;
    virtual void visit(ExprLess&) = 0;
    virtual void visit(ExprNotEquals&) = 0;
    virtual void visit(ExprAnd&) = 0;
    virtual void visit(ExprNot&) = 0;
    virtual void visit(ExprOr&) = 0;
};

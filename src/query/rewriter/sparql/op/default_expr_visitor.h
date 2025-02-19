#pragma once

#include "query/parser/expr/expr_visitor.h"
#include "query/parser/expr/sparql_exprs.h"

namespace SPARQL {
class DefaultExprVisitor : public ExprVisitor {
public:
    virtual ~DefaultExprVisitor() = default;

    virtual void visit(ExprVar&) override { }
    virtual void visit(ExprTerm&) override { }

    virtual void visit(ExprAggCountAll&) override { }
    virtual void visit(ExprBound&) override { }
    virtual void visit(ExprNow&) override { }
    virtual void visit(ExprRand&) override { }
    virtual void visit(ExprStrUUID&) override { }
    virtual void visit(ExprUUID&) override { }


    virtual void visit(ExprNotExists& e) override = 0;

    virtual void visit(ExprExists& e) override = 0;


    virtual void visit(ExprNot& e) override {
        e.expr->accept_visitor(*this);
    }

    virtual void visit(ExprUnaryMinus& e) override {
        e.expr->accept_visitor(*this);
    }

    virtual void visit(ExprUnaryPlus& e) override {
        e.expr->accept_visitor(*this);
    }

    virtual void visit(ExprMultiplication& e) override {
        e.lhs->accept_visitor(*this);
        e.rhs->accept_visitor(*this);
    }

    virtual void visit(ExprDivision& e) override {
        e.lhs->accept_visitor(*this);
        e.rhs->accept_visitor(*this);
    }

    virtual void visit(ExprAddition& e) override {
        e.lhs->accept_visitor(*this);
        e.rhs->accept_visitor(*this);
    }

    virtual void visit(ExprSubtraction& e) override {
        e.lhs->accept_visitor(*this);
        e.rhs->accept_visitor(*this);
    }

    virtual void visit(ExprAnd& e) override {
        e.lhs->accept_visitor(*this);
        e.rhs->accept_visitor(*this);
    }

    virtual void visit(ExprOr& e) override {
        e.lhs->accept_visitor(*this);
        e.rhs->accept_visitor(*this);
    }

    virtual void visit(ExprLess& e) override {
        e.lhs->accept_visitor(*this);
        e.rhs->accept_visitor(*this);
    }

    virtual void visit(ExprGreater& e) override {
        e.lhs->accept_visitor(*this);
        e.rhs->accept_visitor(*this);
    }

    virtual void visit(ExprEqual& e) override {
        e.lhs->accept_visitor(*this);
        e.rhs->accept_visitor(*this);
    }

    virtual void visit(ExprNotEqual& e) override {
        e.lhs->accept_visitor(*this);
        e.rhs->accept_visitor(*this);
    }

    virtual void visit(ExprLessOrEqual& e) override {
        e.lhs->accept_visitor(*this);
        e.rhs->accept_visitor(*this);
    }

    virtual void visit(ExprGreaterOrEqual& e) override {
        e.lhs->accept_visitor(*this);
        e.rhs->accept_visitor(*this);
    }

    virtual void visit(ExprIn& e) override {
        e.lhs_expr->accept_visitor(*this);
        for (auto& ex : e.exprs) {
            ex->accept_visitor(*this);
        }
    }

    virtual void visit(ExprNotIn& e) override {
        e.lhs_expr->accept_visitor(*this);
        for (auto& ex : e.exprs) {
            ex->accept_visitor(*this);
        }
    }

    virtual void visit(ExprAggAvg& e) override {
        e.expr->accept_visitor(*this);
    }

    virtual void visit(ExprAggCount& e) override {
        e.expr->accept_visitor(*this);
    }

    virtual void visit(ExprAggGroupConcat& e) override {
        e.expr->accept_visitor(*this);
    }

    virtual void visit(ExprAggMax& e) override {
        e.expr->accept_visitor(*this);
    }

    virtual void visit(ExprAggMin& e) override {
        e.expr->accept_visitor(*this);
    }

    virtual void visit(ExprAggSample& e) override {
        e.expr->accept_visitor(*this);
    }

    virtual void visit(ExprAggSum& e) override {
        e.expr->accept_visitor(*this);
    }

    virtual void visit(ExprAbs& e) override {
        e.expr->accept_visitor(*this);
    }

    virtual void visit(ExprBNode& e) override {
        if (e.expr != nullptr) {
            e.expr->accept_visitor(*this);
        }
    }

    virtual void visit(ExprCeil& e) override {
        e.expr->accept_visitor(*this);
    }

    virtual void visit(ExprCoalesce& e) override {
        for (auto& ex : e.exprs) {
            ex->accept_visitor(*this);
        }
    }

    virtual void visit(ExprConcat& e) override {
        for (auto& ex : e.exprs) {
            ex->accept_visitor(*this);
        }
    }

    virtual void visit(ExprContains& e) override {
        e.lhs->accept_visitor(*this);
        e.rhs->accept_visitor(*this);
    }

    virtual void visit(ExprDatatype& e) override {
        e.expr->accept_visitor(*this);
    }

    virtual void visit(ExprDay& e) override {
        e.expr->accept_visitor(*this);
    }

    virtual void visit(ExprEncodeForUri& e) override {
        e.expr->accept_visitor(*this);
    }

    virtual void visit(ExprFloor& e) override {
        e.expr->accept_visitor(*this);
    }

    virtual void visit(ExprHours& e) override {
        e.expr->accept_visitor(*this);
    }

    virtual void visit(ExprIf& e) override {
        e.expr1->accept_visitor(*this);
        e.expr2->accept_visitor(*this);
        e.expr3->accept_visitor(*this);
    }

    virtual void visit(ExprIRI& e) override {
        e.expr->accept_visitor(*this);
    }

    virtual void visit(ExprIsBlank& e) override {
        e.expr->accept_visitor(*this);
    }

    virtual void visit(ExprIsIRI& e) override {
        e.expr->accept_visitor(*this);
    }

    virtual void visit(ExprIsLiteral& e) override {
        e.expr->accept_visitor(*this);
    }

    virtual void visit(ExprIsNumeric& e) override {
        e.expr->accept_visitor(*this);
    }

    virtual void visit(ExprIsURI& e) override {
        e.expr->accept_visitor(*this);
    }

    virtual void visit(ExprLang& e) override {
        e.expr->accept_visitor(*this);
    }

    virtual void visit(ExprLangMatches& e) override {
        e.expr1->accept_visitor(*this);
        e.expr2->accept_visitor(*this);
    }

    virtual void visit(ExprLCase& e) override {
        e.expr->accept_visitor(*this);
    }

    virtual void visit(ExprMD5& e) override {
        e.expr->accept_visitor(*this);
    }

    virtual void visit(ExprMinutes& e) override {
        e.expr->accept_visitor(*this);
    }

    virtual void visit(ExprMonth& e) override {
        e.expr->accept_visitor(*this);
    }

    virtual void visit(ExprRegex& e) override {
        e.expr1->accept_visitor(*this);
        e.expr2->accept_visitor(*this);
        if (e.expr3 != nullptr) {
            e.expr3->accept_visitor(*this);
        }
    }

    virtual void visit(ExprReplace& e) override {
        e.expr1->accept_visitor(*this);
        e.expr2->accept_visitor(*this);
        e.expr3->accept_visitor(*this);
        if (e.expr4 != nullptr) {
            e.expr4->accept_visitor(*this);
        }
    }

    virtual void visit(ExprRound& e) override {
        e.expr->accept_visitor(*this);
    }

    virtual void visit(ExprSameTerm& e) override {
        e.lhs->accept_visitor(*this);
        e.rhs->accept_visitor(*this);
    }

    virtual void visit(ExprSeconds& e) override {
        e.expr->accept_visitor(*this);
    }

    virtual void visit(ExprSHA1& e) override {
        e.expr->accept_visitor(*this);
    }

    virtual void visit(ExprSHA256& e) override {
        e.expr->accept_visitor(*this);
    }

    virtual void visit(ExprSHA384& e) override {
        e.expr->accept_visitor(*this);
    }

    virtual void visit(ExprSHA512& e) override {
        e.expr->accept_visitor(*this);
    }

    virtual void visit(ExprStrAfter& e) override {
        e.lhs->accept_visitor(*this);
        e.rhs->accept_visitor(*this);
    }

    virtual void visit(ExprStrBefore& e) override {
        e.lhs->accept_visitor(*this);
        e.rhs->accept_visitor(*this);
    }

    virtual void visit(ExprStrDT& e) override {
        e.expr1->accept_visitor(*this);
        e.expr2->accept_visitor(*this);
    }

    virtual void visit(ExprStrEnds& e) override {
        e.expr1->accept_visitor(*this);
        e.expr2->accept_visitor(*this);
    }

    virtual void visit(ExprStrLang& e) override {
        e.expr1->accept_visitor(*this);
        e.expr2->accept_visitor(*this);
    }

    virtual void visit(ExprStrLen& e) override {
        e.expr->accept_visitor(*this);
    }

    virtual void visit(ExprStrStarts& e) override {
        e.expr1->accept_visitor(*this);
        e.expr2->accept_visitor(*this);
    }


    virtual void visit(ExprStr& e) override {
        e.expr->accept_visitor(*this);
    }

    virtual void visit(ExprSubStr& e) override {
        e.expr1->accept_visitor(*this);
        e.expr2->accept_visitor(*this);
        if (e.expr3 != nullptr) {
            e.expr3->accept_visitor(*this);
        }
    }

    virtual void visit(ExprTimezone& e) override {
        e.expr->accept_visitor(*this);
    }

    virtual void visit(ExprTZ& e) override {
        e.expr->accept_visitor(*this);
    }

    virtual void visit(ExprUCase& e) override {
        e.expr->accept_visitor(*this);
    }

    virtual void visit(ExprURI& e) override {
        e.expr->accept_visitor(*this);
    }

    virtual void visit(ExprYear& e) override {
        e.expr->accept_visitor(*this);
    }

    virtual void visit(ExprCast& e) override {
        e.expr->accept_visitor(*this);
    }

    virtual void visit(ExprDot& e) override {
        e.lhs->accept_visitor(*this);
        e.rhs->accept_visitor(*this);
    }

    virtual void visit(ExprPow& e) override {
        e.lhs->accept_visitor(*this);
        e.rhs->accept_visitor(*this);
    }

    virtual void visit(ExprSqrt& e) override {
        e.expr->accept_visitor(*this);
    }

    virtual void visit(ExprSum& e) override {
        e.expr->accept_visitor(*this);
    }
};
} // namespace SPARQL

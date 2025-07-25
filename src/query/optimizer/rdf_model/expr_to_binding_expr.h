#pragma once

#include <optional>

#include "query/executor/binding_iter/binding_expr/binding_expr.h"
#include "query/parser/expr/sparql/expr.h"
#include "query/parser/expr/sparql/expr_visitor.h"
#include "query/var_id.h"

namespace SPARQL {

class BindingIterConstructor;

// This visitor returns nullptr if condition is pushed outside
class ExprToBindingExpr : public ExprVisitor {
public:
    BindingIterConstructor* bic = nullptr;

    // In expressions like "<expressions> AS <var>", as_var is <var>
    const std::optional<VarId> as_var;

    // Where the result will be stored
    std::unique_ptr<BindingExpr> tmp;

    // Has to be true while visiting something inside of an aggregation
    // and be false otherwise
    bool inside_aggregation = false;

    bool at_root = true; // The visitor is currently at the root node

    // This constructor is used to visit expressions that must not have aggregations or group variables.
    ExprToBindingExpr() :
        bic (nullptr) { }

    // This constructor is used to visit expressions that can have aggregations or group variables.
    ExprToBindingExpr(
        BindingIterConstructor* bic,
        std::optional<VarId> as_var
    ) :
        bic (bic),
        as_var (as_var) { }

    template<typename AggType, class ... Args>
    void check_and_make_aggregate(Expr*, Args&&... args);

    void visit(ExprVar&)            override;
    void visit(ExprTerm&)           override;
    void visit(ExprEqual&)          override;
    void visit(ExprNotEqual&)       override;

    void visit(ExprNot&)            override;
    void visit(ExprUnaryMinus&)     override;
    void visit(ExprUnaryPlus&)      override;
    void visit(ExprMultiplication&) override;
    void visit(ExprDivision&)       override;
    void visit(ExprAddition&)       override;
    void visit(ExprSubtraction&)    override;
    void visit(ExprAnd&)            override;
    void visit(ExprOr&)             override;

    void visit(ExprLess&)           override;
    void visit(ExprGreater&)        override;
    void visit(ExprLessOrEqual&)    override;
    void visit(ExprGreaterOrEqual&) override;

    void visit(ExprIn&)             override;
    void visit(ExprNotIn&)          override;

    void visit(ExprExists&)         override;
    void visit(ExprNotExists&)      override;

    void visit(ExprAggAvg&)         override;
    void visit(ExprAggCount&)       override;
    void visit(ExprAggCountAll&)    override;
    void visit(ExprAggGroupConcat&) override;
    void visit(ExprAggMax&)         override;
    void visit(ExprAggMin&)         override;
    void visit(ExprAggSample&)      override;
    void visit(ExprAggSum&)         override;

    void visit(ExprAbs&)            override;
    void visit(ExprBNode&)          override;
    void visit(ExprBound&)          override;
    void visit(ExprCeil&)           override;
    void visit(ExprCoalesce&)       override;
    void visit(ExprConcat&)         override;
    void visit(ExprContains&)       override;
    void visit(ExprDatatype&)       override;
    void visit(ExprDay&)            override;
    void visit(ExprEncodeForUri&)   override;
    void visit(ExprFloor&)          override;
    void visit(ExprHours&)          override;
    void visit(ExprIf&)             override;
    void visit(ExprIRI&)            override;
    void visit(ExprIsBlank&)        override;
    void visit(ExprIsIRI&)          override;
    void visit(ExprIsLiteral&)      override;
    void visit(ExprIsNumeric&)      override;
    void visit(ExprIsURI&)          override;
    void visit(ExprLang&)           override;
    void visit(ExprLangMatches&)    override;
    void visit(ExprLCase&)          override;
    void visit(ExprMD5&)            override;
    void visit(ExprMinutes&)        override;
    void visit(ExprMonth&)          override;
    void visit(ExprNow&)            override;
    void visit(ExprRand&)           override;
    void visit(ExprRegex&)          override;
    void visit(ExprReplace&)        override;
    void visit(ExprRound&)          override;
    void visit(ExprSameTerm&)       override;
    void visit(ExprSeconds&)        override;
    void visit(ExprSHA1&)           override;
    void visit(ExprSHA256&)         override;
    void visit(ExprSHA384&)         override;
    void visit(ExprSHA512&)         override;
    void visit(ExprStrAfter&)       override;
    void visit(ExprStrBefore&)      override;
    void visit(ExprStrDT&)          override;
    void visit(ExprStrEnds&)        override;
    void visit(ExprStrLang&)        override;
    void visit(ExprStrLen&)         override;
    void visit(ExprStrStarts&)      override;
    void visit(ExprStrUUID&)        override;
    void visit(ExprStr&)            override;
    void visit(ExprSubStr&)         override;
    void visit(ExprTimezone&)       override;
    void visit(ExprTZ&)             override;
    void visit(ExprUCase&)          override;
    void visit(ExprURI&)            override;
    void visit(ExprUUID&)           override;
    void visit(ExprYear&)           override;
    void visit(ExprCast&)           override;

    void visit(ExprCosineDistance&)    override;
    void visit(ExprCosineSimilarity&)  override;
    void visit(ExprDot&)               override;
    void visit(ExprEuclideanDistance&) override;
    void visit(ExprManhattanDistance&) override;
    void visit(ExprPow&)               override;
    void visit(ExprSqrt&)              override;
    void visit(ExprSum&)               override;
};
} // namespace SPARQL

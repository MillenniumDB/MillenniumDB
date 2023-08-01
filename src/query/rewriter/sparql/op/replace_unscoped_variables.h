#pragma once

#include <set>

#include "query/var_id.h"
#include "query/query_context.h"
#include "query/parser/expr/expr.h"
#include "query/parser/expr/expr_visitor.h"
#include "query/parser/expr/sparql_exprs.h"
#include "query/parser/op/op_visitor.h"

namespace SPARQL {

class ReplaceUnscopedVariables : public OpVisitor {
private:
    std::set<VarId> vars_in_scope = {};

public:
    void visit(OpConstruct&)            override;
    void visit(OpDescribe&)             override;
    void visit(OpBasicGraphPattern&)    override;
    void visit(OpEmpty&)                override;
    void visit(OpFilter&)               override;
    void visit(OpJoin&)                 override;
    void visit(OpSemiJoin&)             override;
    void visit(OpMinus&)                override;
    void visit(OpNotExists&)            override;
    void visit(OpUnion&)                override;
    void visit(OpOptional&)             override;
    void visit(OpOrderBy&)              override;
    void visit(OpGroupBy&)              override;
    void visit(OpHaving&)               override;
    void visit(OpSelect&)               override;
    void visit(OpSequence&)             override;
    void visit(OpAsk&)                  override;
    void visit(OpService&)              override;
    void visit(OpWhere&)                override;
    void visit(OpBind&)                 override;
    void visit(OpValues&)               override;
    void visit(OpUnitTable&)            override;
};

class ReplaceUnscopedVariablesExpr : public ExprVisitor {
private:
    const std::set<VarId>& vars_in_scope;

public:
    ReplaceUnscopedVariablesExpr(const std::set<VarId>& vars_in_scope) :
        vars_in_scope(vars_in_scope) { }

    void visit_and_replace_if_necessary(std::unique_ptr<Expr>& expr);

    void visit(SPARQL::ExprVar&)            override;
    void visit(SPARQL::ExprObjectId&)       override;
    void visit(SPARQL::ExprTerm&)           override;
    void visit(SPARQL::ExprEqual&)          override;
    void visit(SPARQL::ExprNotEqual&)       override;
    void visit(SPARQL::ExprNot&)            override;
    void visit(SPARQL::ExprUnaryMinus&)     override;
    void visit(SPARQL::ExprUnaryPlus&)      override;
    void visit(SPARQL::ExprMultiplication&) override;
    void visit(SPARQL::ExprDivision&)       override;
    void visit(SPARQL::ExprAddition&)       override;
    void visit(SPARQL::ExprSubtraction&)    override;
    void visit(SPARQL::ExprAnd&)            override;
    void visit(SPARQL::ExprOr&)             override;
    void visit(SPARQL::ExprLess&)           override;
    void visit(SPARQL::ExprGreater&)        override;
    void visit(SPARQL::ExprLessOrEqual&)    override;
    void visit(SPARQL::ExprGreaterOrEqual&) override;
    void visit(SPARQL::ExprIn&)             override;
    void visit(SPARQL::ExprNotIn&)          override;
    void visit(SPARQL::ExprExists&)         override;
    void visit(SPARQL::ExprNotExists&)      override;
    void visit(SPARQL::ExprAggAvg&)         override;
    void visit(SPARQL::ExprAggCount&)       override;
    void visit(SPARQL::ExprAggCountAll&)    override;
    void visit(SPARQL::ExprAggGroupConcat&) override;
    void visit(SPARQL::ExprAggMax&)         override;
    void visit(SPARQL::ExprAggMin&)         override;
    void visit(SPARQL::ExprAggSample&)      override;
    void visit(SPARQL::ExprAggSum&)         override;
    void visit(SPARQL::ExprAbs&)            override;
    void visit(SPARQL::ExprBNode&)          override;
    void visit(SPARQL::ExprBound&)          override;
    void visit(SPARQL::ExprCeil&)           override;
    void visit(SPARQL::ExprCoalesce&)       override;
    void visit(SPARQL::ExprConcat&)         override;
    void visit(SPARQL::ExprContains&)       override;
    void visit(SPARQL::ExprDatatype&)       override;
    void visit(SPARQL::ExprDay&)            override;
    void visit(SPARQL::ExprEncodeForUri&)   override;
    void visit(SPARQL::ExprFloor&)          override;
    void visit(SPARQL::ExprHours&)          override;
    void visit(SPARQL::ExprIf&)             override;
    void visit(SPARQL::ExprIRI&)            override;
    void visit(SPARQL::ExprIsBlank&)        override;
    void visit(SPARQL::ExprIsIRI&)          override;
    void visit(SPARQL::ExprIsLiteral&)      override;
    void visit(SPARQL::ExprIsNumeric&)      override;
    void visit(SPARQL::ExprIsURI&)          override;
    void visit(SPARQL::ExprLang&)           override;
    void visit(SPARQL::ExprLangMatches&)    override;
    void visit(SPARQL::ExprLCase&)          override;
    void visit(SPARQL::ExprMD5&)            override;
    void visit(SPARQL::ExprMinutes&)        override;
    void visit(SPARQL::ExprMonth&)          override;
    void visit(SPARQL::ExprNow&)            override;
    void visit(SPARQL::ExprRand&)           override;
    void visit(SPARQL::ExprRegex&)          override;
    void visit(SPARQL::ExprReplace&)        override;
    void visit(SPARQL::ExprRound&)          override;
    void visit(SPARQL::ExprSameTerm&)       override;
    void visit(SPARQL::ExprSeconds&)        override;
    void visit(SPARQL::ExprSHA1&)           override;
    void visit(SPARQL::ExprSHA256&)         override;
    void visit(SPARQL::ExprSHA384&)         override;
    void visit(SPARQL::ExprSHA512&)         override;
    void visit(SPARQL::ExprStrAfter&)       override;
    void visit(SPARQL::ExprStrBefore&)      override;
    void visit(SPARQL::ExprStrDT&)          override;
    void visit(SPARQL::ExprStrEnds&)        override;
    void visit(SPARQL::ExprStrLang&)        override;
    void visit(SPARQL::ExprStrLen&)         override;
    void visit(SPARQL::ExprStrStarts&)      override;
    void visit(SPARQL::ExprStrUUID&)        override;
    void visit(SPARQL::ExprStr&)            override;
    void visit(SPARQL::ExprSubStr&)         override;
    void visit(SPARQL::ExprTimezone&)       override;
    void visit(SPARQL::ExprTZ&)             override;
    void visit(SPARQL::ExprUCase&)          override;
    void visit(SPARQL::ExprURI&)            override;
    void visit(SPARQL::ExprUUID&)           override;
    void visit(SPARQL::ExprYear&)           override;
    void visit(SPARQL::ExprCast&)           override;
};

} // namespace SPARQL

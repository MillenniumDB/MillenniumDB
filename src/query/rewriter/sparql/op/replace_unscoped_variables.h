#pragma once

#include <set>

#include "query/parser/expr/sparql/expr.h"
#include "query/parser/expr/sparql/expr_visitor.h"
#include "query/parser/expr/sparql/exprs.h"
#include "query/parser/op/sparql/op_visitor.h"
#include "query/var_id.h"

namespace SPARQL {

class ReplaceUnscopedVariables : public OpVisitor {
public:
    std::set<VarId> parent_vars_in_scope = {};

    void visit(OpConstruct&) override;
    void visit(OpDescribe&) override;
    void visit(OpBasicGraphPattern&) override;
    void visit(OpEmpty&) override;
    void visit(OpFilter&) override;
    void visit(OpJoin&) override;
    void visit(OpSemiJoin&) override;
    void visit(OpMinus&) override;
    void visit(OpNotExists&) override;
    void visit(OpUnion&) override;
    void visit(OpOptional&) override;
    void visit(OpOrderBy&) override;
    void visit(OpFrom&) override;
    void visit(OpGraph&) override;
    void visit(OpGroupBy&) override;
    void visit(OpHaving&) override;
    void visit(OpProcedure&) override;
    void visit(OpSelect&) override;
    void visit(OpSequence&) override;
    void visit(OpAsk&) override;
    void visit(OpService&) override;
    void visit(OpBind&) override;
    void visit(OpValues&) override;
    void visit(OpUnitTable&) override;
    void visit(OpShow&) override;

    void visit(OpPath&) override { }
    void visit(OpTriple&) override { }

    void visit(OpCreateHNSWIndex&) override { }
    void visit(OpCreateTextIndex&) override { }
    void visit(OpDeleteData&) override { }
    void visit(OpInsertData&) override { }
    void visit(OpUpdate&) override { }
};

class ReplaceUnscopedVariablesExpr : public ExprVisitor {
private:
    const std::set<VarId>& vars_in_scope;

public:
    ReplaceUnscopedVariablesExpr(const std::set<VarId>& vars_in_scope) :
        vars_in_scope(vars_in_scope)
    { }

    void visit_and_replace_if_necessary(std::unique_ptr<Expr>& expr);

    void visit(ExprVar&) override;
    void visit(ExprTerm&) override;
    void visit(ExprEqual&) override;
    void visit(ExprNotEqual&) override;
    void visit(ExprNot&) override;
    void visit(ExprUnaryMinus&) override;
    void visit(ExprUnaryPlus&) override;
    void visit(ExprMultiplication&) override;
    void visit(ExprDivision&) override;
    void visit(ExprAddition&) override;
    void visit(ExprSubtraction&) override;
    void visit(ExprAnd&) override;
    void visit(ExprOr&) override;
    void visit(ExprLess&) override;
    void visit(ExprGreater&) override;
    void visit(ExprLessOrEqual&) override;
    void visit(ExprGreaterOrEqual&) override;
    void visit(ExprIn&) override;
    void visit(ExprNotIn&) override;
    void visit(ExprExists&) override;
    void visit(ExprNotExists&) override;
    void visit(ExprAggAvg&) override;
    void visit(ExprAggCount&) override;
    void visit(ExprAggCountAll&) override;
    void visit(ExprAggGroupConcat&) override;
    void visit(ExprAggMax&) override;
    void visit(ExprAggMin&) override;
    void visit(ExprAggSample&) override;
    void visit(ExprAggSum&) override;
    void visit(ExprAbs&) override;
    void visit(ExprBNode&) override;
    void visit(ExprBound&) override;
    void visit(ExprCeil&) override;
    void visit(ExprCoalesce&) override;
    void visit(ExprConcat&) override;
    void visit(ExprContains&) override;
    void visit(ExprDatatype&) override;
    void visit(ExprDay&) override;
    void visit(ExprEncodeForUri&) override;
    void visit(ExprFloor&) override;
    void visit(ExprHours&) override;
    void visit(ExprIf&) override;
    void visit(ExprIRI&) override;
    void visit(ExprIsBlank&) override;
    void visit(ExprIsIRI&) override;
    void visit(ExprIsLiteral&) override;
    void visit(ExprIsNumeric&) override;
    void visit(ExprIsURI&) override;
    void visit(ExprLang&) override;
    void visit(ExprLangMatches&) override;
    void visit(ExprLCase&) override;
    void visit(ExprMD5&) override;
    void visit(ExprMinutes&) override;
    void visit(ExprMonth&) override;
    void visit(ExprNow&) override;
    void visit(ExprRand&) override;
    void visit(ExprRegex&) override;
    void visit(ExprReplace&) override;
    void visit(ExprRound&) override;
    void visit(ExprSameTerm&) override;
    void visit(ExprSeconds&) override;
    void visit(ExprSHA1&) override;
    void visit(ExprSHA256&) override;
    void visit(ExprSHA384&) override;
    void visit(ExprSHA512&) override;
    void visit(ExprStrAfter&) override;
    void visit(ExprStrBefore&) override;
    void visit(ExprStrDT&) override;
    void visit(ExprStrEnds&) override;
    void visit(ExprStrLang&) override;
    void visit(ExprStrLen&) override;
    void visit(ExprStrStarts&) override;
    void visit(ExprStrUUID&) override;
    void visit(ExprStr&) override;
    void visit(ExprSubStr&) override;
    void visit(ExprTimezone&) override;
    void visit(ExprTZ&) override;
    void visit(ExprUCase&) override;
    void visit(ExprURI&) override;
    void visit(ExprUUID&) override;
    void visit(ExprYear&) override;
    void visit(ExprCast&) override;

    void visit(ExprCosineDistance&) override;
    void visit(ExprCosineSimilarity&) override;
    void visit(ExprDot&) override;
    void visit(ExprEuclideanDistance&) override;
    void visit(ExprManhattanDistance&) override;
    void visit(ExprPow&) override;
    void visit(ExprSqrt&) override;
    void visit(ExprSum&) override;
};

} // namespace SPARQL

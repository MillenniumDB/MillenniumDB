#pragma once

#include <vector>

#include "query/executor/binding_iter/binding_expr/binding_expr_visitor.h"
#include "query/executor/binding_iter/binding_expr/sparql_binding_exprs.h"
#include "query/executor/binding_iter/binding_expr/binding_expr_term.h"
#include "query/executor/binding_iter/binding_expr/binding_expr_var.h"

namespace SPARQL {

class BindingExprPrinter : public ::BindingExprPrinter {
public:
    using ::BindingExprPrinter::visit;

    BindingExprPrinter(std::ostream& os) : ::BindingExprPrinter(os) { }

    void visit(BindingExprAbs&)            override;
    void visit(BindingExprAddition&)       override;
    void visit(BindingExprAnd&)            override;
    void visit(BindingExprBNode&)          override;
    void visit(BindingExprBound&)          override;
    void visit(BindingExprCast&)           override;
    void visit(BindingExprCeil&)           override;
    void visit(BindingExprCoalesce&)       override;
    void visit(BindingExprConcat&)         override;
    void visit(BindingExprContains&)       override;
    void visit(BindingExprDatatype&)       override;
    void visit(BindingExprDay&)            override;
    void visit(BindingExprDivision&)       override;
    void visit(BindingExprEncodeForUri&)   override;
    void visit(BindingExprEquals&)         override;
    void visit(BindingExprExists&)         override;
    void visit(BindingExprFloor&)          override;
    void visit(BindingExprGreaterOrEqual&) override;
    void visit(BindingExprGreater&)        override;
    void visit(BindingExprHours&)          override;
    void visit(BindingExprIf&)             override;
    void visit(BindingExprIn&)             override;
    void visit(BindingExprIRI&)            override;
    void visit(BindingExprIsBlank&)        override;
    void visit(BindingExprIsIRI&)          override;
    void visit(BindingExprIsLiteral&)      override;
    void visit(BindingExprIsNumeric&)      override;
    void visit(BindingExprLangMatches&)    override;
    void visit(BindingExprLang&)           override;
    void visit(BindingExprLCase&)          override;
    void visit(BindingExprLessOrEqual&)    override;
    void visit(BindingExprLess&)           override;
    void visit(BindingExprMD5&)            override;
    void visit(BindingExprMinutes&)        override;
    void visit(BindingExprMonth&)          override;
    void visit(BindingExprMultiplication&) override;
    void visit(BindingExprNotExists&)      override;
    void visit(BindingExprNot&)            override;
    void visit(BindingExprOr&)             override;
    void visit(BindingExprRegex&)          override;
    void visit(BindingExprReplace&)        override;
    void visit(BindingExprRound&)          override;
    void visit(BindingExprSameTerm&)       override;
    void visit(BindingExprSeconds&)        override;
    void visit(BindingExprSHA1&)           override;
    void visit(BindingExprSHA256&)         override;
    void visit(BindingExprSHA384&)         override;
    void visit(BindingExprSHA512&)         override;
    void visit(BindingExprStrAfter&)       override;
    void visit(BindingExprStrBefore&)      override;
    void visit(BindingExprStrDT&)          override;
    void visit(BindingExprStrEnds&)        override;
    void visit(BindingExprStrLang&)        override;
    void visit(BindingExprStrLen&)         override;
    void visit(BindingExprStrStarts&)      override;
    void visit(BindingExprStr&)            override;
    void visit(BindingExprSubStr&)         override;
    void visit(BindingExprSubtraction&)    override;
    void visit(BindingExprTimezone&)       override;
    void visit(BindingExprTZ&)             override;
    void visit(BindingExprUCase&)          override;
    void visit(BindingExprUnaryMinus&)     override;
    void visit(BindingExprUnaryPlus&)      override;
    void visit(BindingExprYear&)           override;
};
} // namespace SPARQL
#pragma once

#include "query/parser/op/sparql/op_visitor.h"
#include "query/rewriter/sparql/op/default_expr_visitor.h"

#include <vector>

namespace SPARQL {
/**
 * This visitor applies the translation rules for property paths. Paths are "simple" if they involve only operators /
 * (sequence), ^ (inverse, unary or binary) and the form {n}, for some single integer n. Such paths are fixed length.
 * They are translated to triple patterns in the transformation to the SPARQL algebra and do not require special
 * path-evaluation at runtime. The rules applied are the following:
 *
 * Notes:
 * - X and Y are RDF term or variables
 * - ?_V is a fresh anonymous variable
 * - P and Q are path expressions
 * - These are only applied to property path patterns, not within property path expressions.
 * - Translations earlier in the list are applied in preference to the last translation.
 * - The final translation simply wraps any remaining property path expression to use a common form Path(...).
 *
 * X link(iri) Y => X iri Y .
 * X inv(iri) Y  => Y iri X .
 * X seq(P, Q) Y => X P ?_V . ?_V Q Y .
 * X P Y         => Path(X, P, Y) .
 *
 * src:
 *  https://www.w3.org/TR/sparql11-property-paths/
 *  https://www.w3.org/TR/sparql11-query/#sparqlTranslatePathPatterns
 */
class TranslatePropertyPaths : public OpVisitor {
// Whether to apply the translation rules on path sequences
static constexpr bool TRANSFORM_SEQUENCE = true;

public:
    void visit(OpAsk&)               override;
    void visit(OpBasicGraphPattern&) override;
    void visit(OpBind&)              override;
    void visit(OpConstruct&)         override;
    void visit(OpDescribe&)          override;
    void visit(OpEmpty&)             override;
    void visit(OpFilter&)            override;
    void visit(OpFrom&)              override;
    void visit(OpGraph&)             override;
    void visit(OpGroupBy&)           override;
    void visit(OpHaving&)            override;
    void visit(OpJoin&)              override;
    void visit(OpMinus&)             override;
    void visit(OpNotExists&)         override;
    void visit(OpOptional&)          override;
    void visit(OpOrderBy&)           override;
    void visit(OpProcedure&)         override;
    void visit(OpSelect&)            override;
    void visit(OpSemiJoin&)          override;
    void visit(OpSequence&)          override;
    void visit(OpService&)           override;
    void visit(OpUnion&)             override;

    void visit(OpPath&)      override { }
    void visit(OpTriple&)    override { }
    void visit(OpUnitTable&) override { }
    void visit(OpValues&)    override { }
    void visit(OpShow&)      override { }

private:
    // Apply translation rules to the OpPath, that could lead into creating new triples and paths
    void translate(OpPath& op_path, std::vector<OpTriple>& new_triples, std::vector<OpPath>& new_paths);
};

class TranslatePropertyPathsExpr : public DefaultExprVisitor {
public:
    TranslatePropertyPaths& op_visitor;

    TranslatePropertyPathsExpr(TranslatePropertyPaths& op_visitor) : op_visitor(op_visitor) {}

    virtual void visit(ExprNotExists& e) override;

    virtual void visit(ExprExists& e) override;
};

} // namespace SPARQL

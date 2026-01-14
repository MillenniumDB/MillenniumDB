#pragma once

#include <vector>

#include "query/parser/expr/mql/expr.h"
#include "query/parser/op/mql/graph_pattern/property.h"
#include "query/parser/op/mql/op_visitor.h"

namespace MQL {

// This class reads all mandatory expressions of type `?x.k == "constant"`
// and adds that to the corresponding graph patterns that have ?x as a Property
class PushWhereProperties : public OpVisitor {
public:
    std::vector<Property> props_to_push;

    void visit(OpBasicGraphPattern&) override;
    void visit(OpCall&) override { }
    void visit(OpLet&) override { }
    void visit(OpGroupBy&) override;
    void visit(OpHaving&) override;
    void visit(OpOptional&) override;
    void visit(OpOrderBy&) override;
    void visit(OpReturn&) override;
    void visit(OpSequence&) override;
    void visit(OpUpdate&) override;
    void visit(OpWhere&) override;

    void visit(OpUnitTable&) override { }
    void visit(OpDescribe&) override { }
    void visit(OpShow&) override { }
};

class PushWherePropertiesExpr : public ExprVisitor {
public:
    std::unique_ptr<Expr>* current_parent;

    std::vector<Property>& props_to_push;

    PushWherePropertiesExpr(std::unique_ptr<Expr>& e, std::vector<Property>& props_to_push) :
        current_parent(&e),
        props_to_push(props_to_push)
    { }

    void visit(ExprAnd&) override;
    void visit(ExprEquals&) override;

    void visit(ExprConstant&) override { }
    void visit(ExprVar&) override { }
    void visit(ExprVarProperty&) override { }
    void visit(ExprAddition&) override { }
    void visit(ExprDivision&) override { }
    void visit(ExprModulo&) override { }
    void visit(ExprMultiplication&) override { }
    void visit(ExprSubtraction&) override { }
    void visit(ExprUnaryMinus&) override { }
    void visit(ExprUnaryPlus&) override { }
    void visit(ExprGreaterOrEquals&) override { }
    void visit(ExprGreater&) override { }
    void visit(ExprIs&) override { }
    void visit(ExprLessOrEquals&) override { }
    void visit(ExprLess&) override { }
    void visit(ExprNotEquals&) override { }
    void visit(ExprNot&) override { }
    void visit(ExprOr&) override { }
    void visit(ExprRegex&) override { }
    void visit(ExprCosineSimilarity&) override { }
    void visit(ExprCosineDistance&) override { }
    void visit(ExprManhattanDistance&) override { }
    void visit(ExprEuclideanDistance&) override { }
    void visit(ExprEditDistance&) override { }
    void visit(ExprNormalize&) override { }
    void visit(ExprStr&) override { }
    void visit(ExprLabels&) override { }
    void visit(ExprType&) override { }
    void visit(ExprProperties&) override { }

    void visit(ExprAggAvg&) override { }
    void visit(ExprAggCountAll&) override { }
    void visit(ExprAggCount&) override { }
    void visit(ExprAggMax&) override { }
    void visit(ExprAggMin&) override { }
    void visit(ExprAggSum&) override { }
};

} // namespace MQL

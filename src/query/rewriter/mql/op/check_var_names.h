#pragma once

#include <boost/container/flat_set.hpp>

#include "query/id.h"
#include "query/parser/expr/mql/expr_visitor.h"
#include "query/parser/op/mql/op_visitor.h"

namespace MQL {
/**
 * Will throw an exception if:
 *   - undeclared variables are used where it is not supposed
 *   - unjoinable variables are joined
 *   - return alias vars are used in other place
 */
class CheckVarNames : public OpVisitor {
private:
    template<typename T>
    using SetType = boost::container::flat_set<T>;

    SetType<VarId> declared_vars; // all declared variables
    SetType<VarId> unjoinable_vars; // variables that must not be joined (e.g. path variable)
    SetType<VarId> alias_vars; // e.g. RETURN 1 + 1 AS ?alias

    void try_insert_joinable_var(Id var);
    void try_insert_unjoinable_var(Id var);

public:
    SetType<Id> used_properties;

    void visit(OpBasicGraphPattern&) override;
    void visit(OpCall&) override;
    void visit(OpLet&) override;
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

class CheckVarNamesExpr : public ExprVisitor {
public:
    template<typename T>
    using SetType = boost::container::flat_set<T>;

    const SetType<VarId>& declared_vars;
    const SetType<VarId>& unjoinable_vars;
    const SetType<VarId>& alias_vars;

    CheckVarNamesExpr(
        const SetType<VarId>& declared_vars,
        const SetType<VarId>& unjoinable_vars,
        const SetType<VarId>& alias_vars
    ) :
        declared_vars(declared_vars),
        unjoinable_vars(unjoinable_vars),
        alias_vars(alias_vars)
    { }

    void visit(ExprConstant&) override { }
    void visit(ExprVar&) override;
    void visit(ExprVarProperty&) override;
    void visit(ExprAddition&) override;
    void visit(ExprDivision&) override;
    void visit(ExprModulo&) override;
    void visit(ExprMultiplication&) override;
    void visit(ExprSubtraction&) override;
    void visit(ExprUnaryMinus&) override;
    void visit(ExprUnaryPlus&) override;
    void visit(ExprEquals&) override;
    void visit(ExprGreaterOrEquals&) override;
    void visit(ExprGreater&) override;
    void visit(ExprIs&) override;
    void visit(ExprLessOrEquals&) override;
    void visit(ExprLess&) override;
    void visit(ExprNotEquals&) override;
    void visit(ExprAnd&) override;
    void visit(ExprNot&) override;
    void visit(ExprOr&) override;
    void visit(ExprRegex&) override;
    void visit(ExprCosineSimilarity&) override;
    void visit(ExprCosineDistance&) override;
    void visit(ExprManhattanDistance&) override;
    void visit(ExprEuclideanDistance&) override;
    void visit(ExprEditDistance&) override;
    void visit(ExprNormalize&) override;
    void visit(ExprStr&) override;
    void visit(ExprLabels&) override;
    void visit(ExprType&) override;
    void visit(ExprProperties&) override;

    void visit(ExprAggAvg&) override;
    void visit(ExprAggCountAll&) override;
    void visit(ExprAggCount&) override;
    void visit(ExprAggMax&) override;
    void visit(ExprAggMin&) override;
    void visit(ExprAggSum&) override;
};
} // namespace MQL

#pragma once

#include <boost/container/flat_set.hpp>

#include "query/id.h"
#include "query/parser/expr/mql/expr_visitor.h"
#include "query/parser/op/mql/op_visitor.h"
#include "query/var_id.h"

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

    SetType<VarId> declared_vars;   // all declared variables
    SetType<VarId> unjoinable_vars; // variables that must not be joined (e.g. path variable)
    SetType<VarId> alias_vars;      // e.g. RETURN 1 + 1 AS ?alias

    void insert_joinable_var(VarId var);
    void insert_unjoinable_var(VarId var);

public:
    SetType<Id> used_properties;

    void visit(OpBasicGraphPattern&) override;
    void visit(OpCall&) override;
    void visit(OpLet&) override;
    void visit(OpGroupBy&) override;
    void visit(OpOptional&) override;
    void visit(OpOrderBy&) override;
    void visit(OpReturn&) override;
    void visit(OpSequence&) override;
    void visit(OpWhere&) override;

    void visit(OpDescribe&) override { }
    void visit(OpDisjointTerm&) override { }
    void visit(OpDisjointVar&) override { }
    void visit(OpEdge&) override { }
    void visit(OpLabel&) override { }
    void visit(OpPath&) override { }
    void visit(OpProperty&) override { }
    void visit(OpShow&) override { }

    /* There are impossible to have in a read only query*/
    void visit(OpInsert&) override { }
    void visit(OpUpdate&) override { }
    void visit(OpCreateHNSWIndex&) override { }
    void visit(OpCreateTextIndex&) override { }
};

class CheckVarNamesExpr : public ExprVisitor {
public:
    template<typename T>
    using SetType = boost::container::flat_set<T>;

    SetType<VarId>& declared_vars;
    SetType<VarId>& unjoinable_vars;
    SetType<VarId>& alias_vars;

    CheckVarNamesExpr(
        SetType<VarId>& declared_vars,
        SetType<VarId>& unjoinable_vars,
        SetType<VarId>& alias_vars
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

    void visit(ExprAggAvg&) override;
    void visit(ExprAggCountAll&) override;
    void visit(ExprAggCount&) override;
    void visit(ExprAggMax&) override;
    void visit(ExprAggMin&) override;
    void visit(ExprAggSum&) override;
};
} // namespace MQL

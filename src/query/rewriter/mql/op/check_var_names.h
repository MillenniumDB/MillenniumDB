#pragma once

#include <set>
#include <vector>
#include "query/id.h"

#include "query/parser/expr/expr_visitor.h"
#include "query/parser/op/op_visitor.h"
#include "query/var_id.h"

namespace MQL {
/**
 * Will throw an exception if undeclared variables are used inside of
 * RETURN, WHERE, ORDER BY, GROUP BY
 */
class CheckVarNames : public OpVisitor {
private:
    std::set<VarId> declared_vars;
    std::set<VarId> unjoinable_vars;
    std::set<VarId> return_vars;

public:
    std::vector<OpProperty> optional_properties;
    std::set<Id> used_properties;

    CheckVarNames(
        std::vector<OpProperty>         optional_properties
    ) :
        optional_properties     (optional_properties) { }

    void visit(OpBasicGraphPattern&) override;
    void visit(OpGroupBy&) override;
    void visit(OpOptional&) override;
    void visit(OpOrderBy&) override;
    void visit(OpReturn&) override;
    void visit(OpSet&) override;
    void visit(OpWhere&) override;

    void visit(OpCreateTensorStore&) override { }
    void visit(OpCreateTextSearchIndex&) override { }
    void visit(OpDeleteTensors&) override { }
    void visit(OpDescribe&) override { }
    void visit(OpDisjointTerm&) override { }
    void visit(OpDisjointVar&) override { }
    void visit(OpEdge&) override { }
    void visit(OpInsert&) override { }
    void visit(OpInsertTensors&) override { }
    void visit(OpLabel&) override { }
    void visit(OpPath&) override { }
    void visit(OpProperty&) override { }
    void visit(OpShow&) override { }
};

class CheckVarNamesExpr : public ExprVisitor {
public:
    std::set<VarId>& declared_vars;
    std::set<VarId>& unjoinable_vars;
    std::set<VarId>& return_vars;

    CheckVarNamesExpr(
        std::set<VarId>& declared_vars,
        std::set<VarId>& unjoinable_vars,
        std::set<VarId>& return_vars
    ) :
        declared_vars   (declared_vars),
        unjoinable_vars (unjoinable_vars),
        return_vars     (return_vars) { }

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
    void visit(ExprTensorDistance&) override;
    void visit(ExprTextSearch&) override;

    void visit(ExprAggAvg&) override;
    void visit(ExprAggCountAll&) override;
    void visit(ExprAggCount&) override;
    void visit(ExprAggMax&) override;
    void visit(ExprAggMin&) override;
    void visit(ExprAggSum&) override;
};
} // namespace MQL

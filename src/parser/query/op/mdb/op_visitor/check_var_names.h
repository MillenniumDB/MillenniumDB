#pragma once

#include <set>

#include "base/query/var.h"
#include "parser/query/expr/expr_visitor.h"
#include "parser/query/op/op_visitor.h"

namespace MDB {
/**
 * Will throw an exception if unexisting nodes/edges are used inside RETRUN or WHERE
 */
class CheckVarNames : public OpVisitor {
private:
    std::set<Var> declared_vars;

    std::set<std::string> group_var_names;

    std::set<Var> declared_path_vars;

public:
    void visit(OpBasicGraphPattern&) override;
    void visit(OpGroupBy&) override;
    void visit(OpMatch&) override;
    void visit(OpOptional&) override;
    void visit(OpOrderBy&) override;
    void visit(OpReturn&) override;
    void visit(OpSet&) override;
    void visit(OpWhere&) override;

    void visit(OpEdge&) override { }
    void visit(OpDescribe&) override { }
    void visit(OpIsolatedTerm&) override { }
    void visit(OpIsolatedVar&) override { }
    void visit(OpLabel&) override { }
    void visit(OpPath&) override { }
    void visit(OpProperty&) override { }
};

class CheckVarNamesExpr : public ExprVisitor {
public:
    std::set<Var>& declared_vars;

    std::set<Var>& declared_path_vars;

    CheckVarNamesExpr(std::set<Var>& declared_vars, std::set<Var>& declared_path_vars) :
        declared_vars(declared_vars), declared_path_vars(declared_path_vars) { }

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
};
} // namespace MDB
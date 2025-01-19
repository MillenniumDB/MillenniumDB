#pragma once

#include <memory>
#include <vector>

#include "query/parser/expr/expr.h"
#include "query/parser/expr/expr_visitor.h"
#include "query/parser/op/op.h"
#include "query/parser/op/op_visitor.h"

namespace MQL {

/*
Pushes mandatory TextSearch conditions from the filter as an iterator.

This visitor will modify the OpBasicGraphPattern inserting the pushable
text searches into it.
*/
class PushTextSearch : public OpVisitor {
private:
    bool must_delete_where { false };

    std::unique_ptr<Op> where_child;

    std::vector<ExprTextSearch> expr_text_searches;

public:
    void visit(OpBasicGraphPattern&) override;
    void visit(OpGroupBy&) override;
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
    void visit(OpOptional&) override { }
    void visit(OpPath&) override { }
    void visit(OpProperty&) override { }
    void visit(OpShow&) override { }
};

class PushTextSearchExpr : public ExprVisitor {
private:
public:
    bool can_remove { true };

    std::unique_ptr<Expr> remaining_expr;

    std::vector<ExprTextSearch>& expr_text_searches;

    PushTextSearchExpr(std::vector<ExprTextSearch>& expr_text_searches_) :
        expr_text_searches { expr_text_searches_ }
    { }

    void visit(ExprConstant&) override { }
    void visit(ExprVar&) override { }
    void visit(ExprVarProperty&) override { }

    void visit(ExprAddition&) override;
    void visit(ExprAnd&) override;
    void visit(ExprDivision&) override;
    void visit(ExprEquals&) override;
    void visit(ExprGreater&) override;
    void visit(ExprGreaterOrEquals&) override;
    void visit(ExprIs&) override;
    void visit(ExprLess&) override;
    void visit(ExprLessOrEquals&) override;
    void visit(ExprModulo&) override;
    void visit(ExprMultiplication&) override;
    void visit(ExprNot&) override;
    void visit(ExprNotEquals&) override;
    void visit(ExprOr&) override;
    void visit(ExprRegex&) override;
    void visit(ExprSubtraction&) override;
    void visit(ExprTensorDistance&) override;
    void visit(ExprTextSearch&) override;
    void visit(ExprUnaryMinus&) override;
    void visit(ExprUnaryPlus&) override;
};
} // namespace MQL
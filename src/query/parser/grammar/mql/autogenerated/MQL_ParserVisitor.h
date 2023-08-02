
// Generated from MQL_Parser.g4 by ANTLR 4.9.3

#pragma once


#include "antlr4-runtime.h"
#include "MQL_Parser.h"



/**
 * This class defines an abstract visitor for a parse tree
 * produced by MQL_Parser.
 */
class  MQL_ParserVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by MQL_Parser.
   */
    virtual antlrcpp::Any visitRoot(MQL_Parser::RootContext *context) = 0;

    virtual antlrcpp::Any visitMatchQuery(MQL_Parser::MatchQueryContext *context) = 0;

    virtual antlrcpp::Any visitDescribeQuery(MQL_Parser::DescribeQueryContext *context) = 0;

    virtual antlrcpp::Any visitDescribeFlag(MQL_Parser::DescribeFlagContext *context) = 0;

    virtual antlrcpp::Any visitInsertQuery(MQL_Parser::InsertQueryContext *context) = 0;

    virtual antlrcpp::Any visitInsertLabelList(MQL_Parser::InsertLabelListContext *context) = 0;

    virtual antlrcpp::Any visitInsertPropertyList(MQL_Parser::InsertPropertyListContext *context) = 0;

    virtual antlrcpp::Any visitInsertEdgeList(MQL_Parser::InsertEdgeListContext *context) = 0;

    virtual antlrcpp::Any visitInsertLabelElement(MQL_Parser::InsertLabelElementContext *context) = 0;

    virtual antlrcpp::Any visitInsertPropertyElement(MQL_Parser::InsertPropertyElementContext *context) = 0;

    virtual antlrcpp::Any visitInsertEdgeElement(MQL_Parser::InsertEdgeElementContext *context) = 0;

    virtual antlrcpp::Any visitSetStatement(MQL_Parser::SetStatementContext *context) = 0;

    virtual antlrcpp::Any visitMatchStatement(MQL_Parser::MatchStatementContext *context) = 0;

    virtual antlrcpp::Any visitWhereStatement(MQL_Parser::WhereStatementContext *context) = 0;

    virtual antlrcpp::Any visitGroupByStatement(MQL_Parser::GroupByStatementContext *context) = 0;

    virtual antlrcpp::Any visitOrderByStatement(MQL_Parser::OrderByStatementContext *context) = 0;

    virtual antlrcpp::Any visitReturnList(MQL_Parser::ReturnListContext *context) = 0;

    virtual antlrcpp::Any visitReturnAll(MQL_Parser::ReturnAllContext *context) = 0;

    virtual antlrcpp::Any visitSetItem(MQL_Parser::SetItemContext *context) = 0;

    virtual antlrcpp::Any visitReturnItemVar(MQL_Parser::ReturnItemVarContext *context) = 0;

    virtual antlrcpp::Any visitReturnItemAgg(MQL_Parser::ReturnItemAggContext *context) = 0;

    virtual antlrcpp::Any visitReturnItemCount(MQL_Parser::ReturnItemCountContext *context) = 0;

    virtual antlrcpp::Any visitAggregateFunc(MQL_Parser::AggregateFuncContext *context) = 0;

    virtual antlrcpp::Any visitOrderByItemVar(MQL_Parser::OrderByItemVarContext *context) = 0;

    virtual antlrcpp::Any visitOrderByItemAgg(MQL_Parser::OrderByItemAggContext *context) = 0;

    virtual antlrcpp::Any visitOrderByItemCount(MQL_Parser::OrderByItemCountContext *context) = 0;

    virtual antlrcpp::Any visitGroupByItem(MQL_Parser::GroupByItemContext *context) = 0;

    virtual antlrcpp::Any visitGraphPattern(MQL_Parser::GraphPatternContext *context) = 0;

    virtual antlrcpp::Any visitOptionalPattern(MQL_Parser::OptionalPatternContext *context) = 0;

    virtual antlrcpp::Any visitBasicPattern(MQL_Parser::BasicPatternContext *context) = 0;

    virtual antlrcpp::Any visitLinearPattern(MQL_Parser::LinearPatternContext *context) = 0;

    virtual antlrcpp::Any visitPath(MQL_Parser::PathContext *context) = 0;

    virtual antlrcpp::Any visitPathAlternatives(MQL_Parser::PathAlternativesContext *context) = 0;

    virtual antlrcpp::Any visitPathSequence(MQL_Parser::PathSequenceContext *context) = 0;

    virtual antlrcpp::Any visitPathAtomSimple(MQL_Parser::PathAtomSimpleContext *context) = 0;

    virtual antlrcpp::Any visitPathAtomAlternatives(MQL_Parser::PathAtomAlternativesContext *context) = 0;

    virtual antlrcpp::Any visitPathSuffix(MQL_Parser::PathSuffixContext *context) = 0;

    virtual antlrcpp::Any visitPathType(MQL_Parser::PathTypeContext *context) = 0;

    virtual antlrcpp::Any visitNode(MQL_Parser::NodeContext *context) = 0;

    virtual antlrcpp::Any visitFixedNode(MQL_Parser::FixedNodeContext *context) = 0;

    virtual antlrcpp::Any visitFixedNodeInside(MQL_Parser::FixedNodeInsideContext *context) = 0;

    virtual antlrcpp::Any visitEdge(MQL_Parser::EdgeContext *context) = 0;

    virtual antlrcpp::Any visitEdgeInside(MQL_Parser::EdgeInsideContext *context) = 0;

    virtual antlrcpp::Any visitVarNode(MQL_Parser::VarNodeContext *context) = 0;

    virtual antlrcpp::Any visitProperties(MQL_Parser::PropertiesContext *context) = 0;

    virtual antlrcpp::Any visitProperty(MQL_Parser::PropertyContext *context) = 0;

    virtual antlrcpp::Any visitIdentifier(MQL_Parser::IdentifierContext *context) = 0;

    virtual antlrcpp::Any visitBoolValue(MQL_Parser::BoolValueContext *context) = 0;

    virtual antlrcpp::Any visitNumericValue(MQL_Parser::NumericValueContext *context) = 0;

    virtual antlrcpp::Any visitValue(MQL_Parser::ValueContext *context) = 0;

    virtual antlrcpp::Any visitConditionalOrExpr(MQL_Parser::ConditionalOrExprContext *context) = 0;

    virtual antlrcpp::Any visitConditionalAndExpr(MQL_Parser::ConditionalAndExprContext *context) = 0;

    virtual antlrcpp::Any visitComparisonExprOp(MQL_Parser::ComparisonExprOpContext *context) = 0;

    virtual antlrcpp::Any visitComparisonExprIs(MQL_Parser::ComparisonExprIsContext *context) = 0;

    virtual antlrcpp::Any visitAdditiveExpr(MQL_Parser::AdditiveExprContext *context) = 0;

    virtual antlrcpp::Any visitMultiplicativeExpr(MQL_Parser::MultiplicativeExprContext *context) = 0;

    virtual antlrcpp::Any visitUnaryExpr(MQL_Parser::UnaryExprContext *context) = 0;

    virtual antlrcpp::Any visitExprVar(MQL_Parser::ExprVarContext *context) = 0;

    virtual antlrcpp::Any visitExprValueExpr(MQL_Parser::ExprValueExprContext *context) = 0;

    virtual antlrcpp::Any visitExprParenthesis(MQL_Parser::ExprParenthesisContext *context) = 0;

    virtual antlrcpp::Any visitValueExpr(MQL_Parser::ValueExprContext *context) = 0;

    virtual antlrcpp::Any visitExprTypename(MQL_Parser::ExprTypenameContext *context) = 0;

    virtual antlrcpp::Any visitKeyword(MQL_Parser::KeywordContext *context) = 0;


};


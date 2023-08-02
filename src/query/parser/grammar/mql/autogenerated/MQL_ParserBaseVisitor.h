
// Generated from MQL_Parser.g4 by ANTLR 4.9.3

#pragma once


#include "antlr4-runtime.h"
#include "MQL_ParserVisitor.h"


/**
 * This class provides an empty implementation of MQL_ParserVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  MQL_ParserBaseVisitor : public MQL_ParserVisitor {
public:

  virtual antlrcpp::Any visitRoot(MQL_Parser::RootContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitMatchQuery(MQL_Parser::MatchQueryContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitDescribeQuery(MQL_Parser::DescribeQueryContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitDescribeFlag(MQL_Parser::DescribeFlagContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitInsertQuery(MQL_Parser::InsertQueryContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitInsertLabelList(MQL_Parser::InsertLabelListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitInsertPropertyList(MQL_Parser::InsertPropertyListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitInsertEdgeList(MQL_Parser::InsertEdgeListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitInsertLabelElement(MQL_Parser::InsertLabelElementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitInsertPropertyElement(MQL_Parser::InsertPropertyElementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitInsertEdgeElement(MQL_Parser::InsertEdgeElementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitSetStatement(MQL_Parser::SetStatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitMatchStatement(MQL_Parser::MatchStatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitWhereStatement(MQL_Parser::WhereStatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitGroupByStatement(MQL_Parser::GroupByStatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitOrderByStatement(MQL_Parser::OrderByStatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitReturnList(MQL_Parser::ReturnListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitReturnAll(MQL_Parser::ReturnAllContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitSetItem(MQL_Parser::SetItemContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitReturnItemVar(MQL_Parser::ReturnItemVarContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitReturnItemAgg(MQL_Parser::ReturnItemAggContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitReturnItemCount(MQL_Parser::ReturnItemCountContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitAggregateFunc(MQL_Parser::AggregateFuncContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitOrderByItemVar(MQL_Parser::OrderByItemVarContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitOrderByItemAgg(MQL_Parser::OrderByItemAggContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitOrderByItemCount(MQL_Parser::OrderByItemCountContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitGroupByItem(MQL_Parser::GroupByItemContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitGraphPattern(MQL_Parser::GraphPatternContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitOptionalPattern(MQL_Parser::OptionalPatternContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitBasicPattern(MQL_Parser::BasicPatternContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitLinearPattern(MQL_Parser::LinearPatternContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitPath(MQL_Parser::PathContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitPathAlternatives(MQL_Parser::PathAlternativesContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitPathSequence(MQL_Parser::PathSequenceContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitPathAtomSimple(MQL_Parser::PathAtomSimpleContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitPathAtomAlternatives(MQL_Parser::PathAtomAlternativesContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitPathSuffix(MQL_Parser::PathSuffixContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitPathType(MQL_Parser::PathTypeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitNode(MQL_Parser::NodeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitFixedNode(MQL_Parser::FixedNodeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitFixedNodeInside(MQL_Parser::FixedNodeInsideContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitEdge(MQL_Parser::EdgeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitEdgeInside(MQL_Parser::EdgeInsideContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitVarNode(MQL_Parser::VarNodeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitProperties(MQL_Parser::PropertiesContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitProperty(MQL_Parser::PropertyContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitIdentifier(MQL_Parser::IdentifierContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitBoolValue(MQL_Parser::BoolValueContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitNumericValue(MQL_Parser::NumericValueContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitValue(MQL_Parser::ValueContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitConditionalOrExpr(MQL_Parser::ConditionalOrExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitConditionalAndExpr(MQL_Parser::ConditionalAndExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitComparisonExprOp(MQL_Parser::ComparisonExprOpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitComparisonExprIs(MQL_Parser::ComparisonExprIsContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitAdditiveExpr(MQL_Parser::AdditiveExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitMultiplicativeExpr(MQL_Parser::MultiplicativeExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitUnaryExpr(MQL_Parser::UnaryExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitExprVar(MQL_Parser::ExprVarContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitExprValueExpr(MQL_Parser::ExprValueExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitExprParenthesis(MQL_Parser::ExprParenthesisContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitValueExpr(MQL_Parser::ValueExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitExprTypename(MQL_Parser::ExprTypenameContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitKeyword(MQL_Parser::KeywordContext *ctx) override {
    return visitChildren(ctx);
  }


};


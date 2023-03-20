
// Generated from MDBParser.g4 by ANTLR 4.9.3

#pragma once


#include "antlr4-runtime.h"
#include "MDBParserVisitor.h"


/**
 * This class provides an empty implementation of MDBParserVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  MDBParserBaseVisitor : public MDBParserVisitor {
public:

  virtual antlrcpp::Any visitRoot(MDBParser::RootContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitMatchQuery(MDBParser::MatchQueryContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitDescribeQuery(MDBParser::DescribeQueryContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitInsertQuery(MDBParser::InsertQueryContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitInsertLabelList(MDBParser::InsertLabelListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitInsertPropertyList(MDBParser::InsertPropertyListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitInsertEdgeList(MDBParser::InsertEdgeListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitInsertLabelElement(MDBParser::InsertLabelElementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitInsertPropertyElement(MDBParser::InsertPropertyElementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitInsertEdgeElement(MDBParser::InsertEdgeElementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitSetStatement(MDBParser::SetStatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitMatchStatement(MDBParser::MatchStatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitWhereStatement(MDBParser::WhereStatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitGroupByStatement(MDBParser::GroupByStatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitOrderByStatement(MDBParser::OrderByStatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitReturnList(MDBParser::ReturnListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitReturnAll(MDBParser::ReturnAllContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitSetItem(MDBParser::SetItemContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitReturnItemVar(MDBParser::ReturnItemVarContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitReturnItemAgg(MDBParser::ReturnItemAggContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitReturnItemCount(MDBParser::ReturnItemCountContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitAggregateFunc(MDBParser::AggregateFuncContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitOrderByItemVar(MDBParser::OrderByItemVarContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitOrderByItemAgg(MDBParser::OrderByItemAggContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitOrderByItemCount(MDBParser::OrderByItemCountContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitGroupByItem(MDBParser::GroupByItemContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitGraphPattern(MDBParser::GraphPatternContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitOptionalPattern(MDBParser::OptionalPatternContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitBasicPattern(MDBParser::BasicPatternContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitLinearPattern(MDBParser::LinearPatternContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitPath(MDBParser::PathContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitPathAlternatives(MDBParser::PathAlternativesContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitPathSequence(MDBParser::PathSequenceContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitPathAtomSimple(MDBParser::PathAtomSimpleContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitPathAtomAlternatives(MDBParser::PathAtomAlternativesContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitPathSuffix(MDBParser::PathSuffixContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitPathType(MDBParser::PathTypeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitNode(MDBParser::NodeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitFixedNode(MDBParser::FixedNodeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitFixedNodeInside(MDBParser::FixedNodeInsideContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitEdge(MDBParser::EdgeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitEdgeInside(MDBParser::EdgeInsideContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitVarNode(MDBParser::VarNodeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitProperties(MDBParser::PropertiesContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitProperty(MDBParser::PropertyContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitIdentifier(MDBParser::IdentifierContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitBoolValue(MDBParser::BoolValueContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitNumericValue(MDBParser::NumericValueContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitValue(MDBParser::ValueContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitConditionalOrExpr(MDBParser::ConditionalOrExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitConditionalAndExpr(MDBParser::ConditionalAndExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitComparisonExprOp(MDBParser::ComparisonExprOpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitComparisonExprIs(MDBParser::ComparisonExprIsContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitAdditiveExpr(MDBParser::AdditiveExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitMultiplicativeExpr(MDBParser::MultiplicativeExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitUnaryExpr(MDBParser::UnaryExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitExprVar(MDBParser::ExprVarContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitExprValueExpr(MDBParser::ExprValueExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitExprParenthesis(MDBParser::ExprParenthesisContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitValueExpr(MDBParser::ValueExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitExprTypename(MDBParser::ExprTypenameContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitKeyword(MDBParser::KeywordContext *ctx) override {
    return visitChildren(ctx);
  }


};



// Generated from MQL_Parser.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"
#include "MQL_ParserVisitor.h"


/**
 * This class provides an empty implementation of MQL_ParserVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  MQL_ParserBaseVisitor : public MQL_ParserVisitor {
public:

  virtual std::any visitRoot(MQL_Parser::RootContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitMatchQuery(MQL_Parser::MatchQueryContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPrimitiveStatementList(MQL_Parser::PrimitiveStatementListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPrimitiveStatement(MQL_Parser::PrimitiveStatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitInsertPatterns(MQL_Parser::InsertPatternsContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitInsertLinearPattern(MQL_Parser::InsertLinearPatternContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitInsertPlainNode(MQL_Parser::InsertPlainNodeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitInsertPlainNodeInside(MQL_Parser::InsertPlainNodeInsideContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitInsertPlainEdge(MQL_Parser::InsertPlainEdgeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCreateIndexQuery(MQL_Parser::CreateIndexQueryContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCreateIndexOptions(MQL_Parser::CreateIndexOptionsContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCreateIndexOption(MQL_Parser::CreateIndexOptionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitShowQuery(MQL_Parser::ShowQueryContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitDescribeQuery(MQL_Parser::DescribeQueryContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitDescribeFlag(MQL_Parser::DescribeFlagContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitMatchStatement(MQL_Parser::MatchStatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLetStatement(MQL_Parser::LetStatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLetDefinitionList(MQL_Parser::LetDefinitionListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLetDefinition(MQL_Parser::LetDefinitionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitWhereStatement(MQL_Parser::WhereStatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitGroupByStatement(MQL_Parser::GroupByStatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitOrderByStatement(MQL_Parser::OrderByStatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitReturnList(MQL_Parser::ReturnListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitReturnAll(MQL_Parser::ReturnAllContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCallStatement(MQL_Parser::CallStatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitYieldStatement(MQL_Parser::YieldStatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitYieldItem(MQL_Parser::YieldItemContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCallArguments(MQL_Parser::CallArgumentsContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLimitOffsetClauses(MQL_Parser::LimitOffsetClausesContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLimitClause(MQL_Parser::LimitClauseContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitOffsetClause(MQL_Parser::OffsetClauseContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitReturnItemVar(MQL_Parser::ReturnItemVarContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitReturnItemAgg(MQL_Parser::ReturnItemAggContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitReturnItemCount(MQL_Parser::ReturnItemCountContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitReturnItemExpr(MQL_Parser::ReturnItemExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAlias(MQL_Parser::AliasContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAggregateFunc(MQL_Parser::AggregateFuncContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitOrderByItemVar(MQL_Parser::OrderByItemVarContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitOrderByItemAgg(MQL_Parser::OrderByItemAggContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitOrderByItemCount(MQL_Parser::OrderByItemCountContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitOrderByItemExpr(MQL_Parser::OrderByItemExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitGroupByItem(MQL_Parser::GroupByItemContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitGraphPattern(MQL_Parser::GraphPatternContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitOptionalPattern(MQL_Parser::OptionalPatternContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitBasicPattern(MQL_Parser::BasicPatternContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLinearPattern(MQL_Parser::LinearPatternContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPath(MQL_Parser::PathContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPathAlternatives(MQL_Parser::PathAlternativesContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPathSequence(MQL_Parser::PathSequenceContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPathAtomSimple(MQL_Parser::PathAtomSimpleContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPathAtomAlternatives(MQL_Parser::PathAtomAlternativesContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPathSuffix(MQL_Parser::PathSuffixContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPathType(MQL_Parser::PathTypeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitNode(MQL_Parser::NodeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFixedNode(MQL_Parser::FixedNodeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFixedNodeInside(MQL_Parser::FixedNodeInsideContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitEdge(MQL_Parser::EdgeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitEdgeInside(MQL_Parser::EdgeInsideContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitVarNode(MQL_Parser::VarNodeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitProperties(MQL_Parser::PropertiesContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitProperty1(MQL_Parser::Property1Context *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitProperty2(MQL_Parser::Property2Context *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitProperty3(MQL_Parser::Property3Context *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitProperty4(MQL_Parser::Property4Context *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitConditionalOrType(MQL_Parser::ConditionalOrTypeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitIdentifier(MQL_Parser::IdentifierContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitBoolValue(MQL_Parser::BoolValueContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitNumericValue(MQL_Parser::NumericValueContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitDatatypeValue(MQL_Parser::DatatypeValueContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitValue(MQL_Parser::ValueContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitConditionalOrExpr(MQL_Parser::ConditionalOrExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitConditionalAndExpr(MQL_Parser::ConditionalAndExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitComparisonExprOp(MQL_Parser::ComparisonExprOpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitComparisonExprIs(MQL_Parser::ComparisonExprIsContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAdditiveExpr(MQL_Parser::AdditiveExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitMultiplicativeExpr(MQL_Parser::MultiplicativeExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitUnaryExpr(MQL_Parser::UnaryExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExprVar(MQL_Parser::ExprVarContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExprValue(MQL_Parser::ExprValueContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExprFixedNodeInside(MQL_Parser::ExprFixedNodeInsideContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExprParenthesis(MQL_Parser::ExprParenthesisContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExprFunction(MQL_Parser::ExprFunctionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFunction(MQL_Parser::FunctionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitRegex(MQL_Parser::RegexContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitTextSearch(MQL_Parser::TextSearchContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCosineSimilarity(MQL_Parser::CosineSimilarityContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCosineDistance(MQL_Parser::CosineDistanceContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitManhattanDistance(MQL_Parser::ManhattanDistanceContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitEuclideanDistance(MQL_Parser::EuclideanDistanceContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitTextSearchIndexMode(MQL_Parser::TextSearchIndexModeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExprTypename(MQL_Parser::ExprTypenameContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitKeyword(MQL_Parser::KeywordContext *ctx) override {
    return visitChildren(ctx);
  }


};


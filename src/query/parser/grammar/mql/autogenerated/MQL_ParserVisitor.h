
// Generated from MQL_Parser.g4 by ANTLR 4.13.1

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
    virtual std::any visitRoot(MQL_Parser::RootContext *context) = 0;

    virtual std::any visitMatchQuery(MQL_Parser::MatchQueryContext *context) = 0;

    virtual std::any visitPrimitiveStatementList(MQL_Parser::PrimitiveStatementListContext *context) = 0;

    virtual std::any visitPrimitiveStatement(MQL_Parser::PrimitiveStatementContext *context) = 0;

    virtual std::any visitInsertPatterns(MQL_Parser::InsertPatternsContext *context) = 0;

    virtual std::any visitInsertLinearPattern(MQL_Parser::InsertLinearPatternContext *context) = 0;

    virtual std::any visitInsertPlainNode(MQL_Parser::InsertPlainNodeContext *context) = 0;

    virtual std::any visitInsertPlainNodeInside(MQL_Parser::InsertPlainNodeInsideContext *context) = 0;

    virtual std::any visitInsertPlainEdge(MQL_Parser::InsertPlainEdgeContext *context) = 0;

    virtual std::any visitCreateIndexQuery(MQL_Parser::CreateIndexQueryContext *context) = 0;

    virtual std::any visitCreateIndexOptions(MQL_Parser::CreateIndexOptionsContext *context) = 0;

    virtual std::any visitCreateIndexOption(MQL_Parser::CreateIndexOptionContext *context) = 0;

    virtual std::any visitShowQuery(MQL_Parser::ShowQueryContext *context) = 0;

    virtual std::any visitDescribeQuery(MQL_Parser::DescribeQueryContext *context) = 0;

    virtual std::any visitDescribeFlag(MQL_Parser::DescribeFlagContext *context) = 0;

    virtual std::any visitMatchStatement(MQL_Parser::MatchStatementContext *context) = 0;

    virtual std::any visitLetStatement(MQL_Parser::LetStatementContext *context) = 0;

    virtual std::any visitLetDefinitionList(MQL_Parser::LetDefinitionListContext *context) = 0;

    virtual std::any visitLetDefinition(MQL_Parser::LetDefinitionContext *context) = 0;

    virtual std::any visitWhereStatement(MQL_Parser::WhereStatementContext *context) = 0;

    virtual std::any visitGroupByStatement(MQL_Parser::GroupByStatementContext *context) = 0;

    virtual std::any visitOrderByStatement(MQL_Parser::OrderByStatementContext *context) = 0;

    virtual std::any visitReturnList(MQL_Parser::ReturnListContext *context) = 0;

    virtual std::any visitReturnAll(MQL_Parser::ReturnAllContext *context) = 0;

    virtual std::any visitCallStatement(MQL_Parser::CallStatementContext *context) = 0;

    virtual std::any visitYieldStatement(MQL_Parser::YieldStatementContext *context) = 0;

    virtual std::any visitYieldItem(MQL_Parser::YieldItemContext *context) = 0;

    virtual std::any visitCallArguments(MQL_Parser::CallArgumentsContext *context) = 0;

    virtual std::any visitLimitOffsetClauses(MQL_Parser::LimitOffsetClausesContext *context) = 0;

    virtual std::any visitLimitClause(MQL_Parser::LimitClauseContext *context) = 0;

    virtual std::any visitOffsetClause(MQL_Parser::OffsetClauseContext *context) = 0;

    virtual std::any visitReturnItemVar(MQL_Parser::ReturnItemVarContext *context) = 0;

    virtual std::any visitReturnItemAgg(MQL_Parser::ReturnItemAggContext *context) = 0;

    virtual std::any visitReturnItemCount(MQL_Parser::ReturnItemCountContext *context) = 0;

    virtual std::any visitReturnItemExpr(MQL_Parser::ReturnItemExprContext *context) = 0;

    virtual std::any visitAlias(MQL_Parser::AliasContext *context) = 0;

    virtual std::any visitAggregateFunc(MQL_Parser::AggregateFuncContext *context) = 0;

    virtual std::any visitOrderByItemVar(MQL_Parser::OrderByItemVarContext *context) = 0;

    virtual std::any visitOrderByItemAgg(MQL_Parser::OrderByItemAggContext *context) = 0;

    virtual std::any visitOrderByItemCount(MQL_Parser::OrderByItemCountContext *context) = 0;

    virtual std::any visitOrderByItemExpr(MQL_Parser::OrderByItemExprContext *context) = 0;

    virtual std::any visitGroupByItem(MQL_Parser::GroupByItemContext *context) = 0;

    virtual std::any visitGraphPattern(MQL_Parser::GraphPatternContext *context) = 0;

    virtual std::any visitOptionalPattern(MQL_Parser::OptionalPatternContext *context) = 0;

    virtual std::any visitBasicPattern(MQL_Parser::BasicPatternContext *context) = 0;

    virtual std::any visitLinearPattern(MQL_Parser::LinearPatternContext *context) = 0;

    virtual std::any visitPath(MQL_Parser::PathContext *context) = 0;

    virtual std::any visitPathAlternatives(MQL_Parser::PathAlternativesContext *context) = 0;

    virtual std::any visitPathSequence(MQL_Parser::PathSequenceContext *context) = 0;

    virtual std::any visitPathAtomSimple(MQL_Parser::PathAtomSimpleContext *context) = 0;

    virtual std::any visitPathAtomAlternatives(MQL_Parser::PathAtomAlternativesContext *context) = 0;

    virtual std::any visitPathSuffix(MQL_Parser::PathSuffixContext *context) = 0;

    virtual std::any visitPathType(MQL_Parser::PathTypeContext *context) = 0;

    virtual std::any visitNode(MQL_Parser::NodeContext *context) = 0;

    virtual std::any visitFixedNode(MQL_Parser::FixedNodeContext *context) = 0;

    virtual std::any visitFixedNodeInside(MQL_Parser::FixedNodeInsideContext *context) = 0;

    virtual std::any visitEdge(MQL_Parser::EdgeContext *context) = 0;

    virtual std::any visitEdgeInside(MQL_Parser::EdgeInsideContext *context) = 0;

    virtual std::any visitVarNode(MQL_Parser::VarNodeContext *context) = 0;

    virtual std::any visitProperties(MQL_Parser::PropertiesContext *context) = 0;

    virtual std::any visitProperty1(MQL_Parser::Property1Context *context) = 0;

    virtual std::any visitProperty2(MQL_Parser::Property2Context *context) = 0;

    virtual std::any visitProperty3(MQL_Parser::Property3Context *context) = 0;

    virtual std::any visitProperty4(MQL_Parser::Property4Context *context) = 0;

    virtual std::any visitConditionalOrType(MQL_Parser::ConditionalOrTypeContext *context) = 0;

    virtual std::any visitIdentifier(MQL_Parser::IdentifierContext *context) = 0;

    virtual std::any visitBoolValue(MQL_Parser::BoolValueContext *context) = 0;

    virtual std::any visitNumericValue(MQL_Parser::NumericValueContext *context) = 0;

    virtual std::any visitDatatypeValue(MQL_Parser::DatatypeValueContext *context) = 0;

    virtual std::any visitValue(MQL_Parser::ValueContext *context) = 0;

    virtual std::any visitConditionalOrExpr(MQL_Parser::ConditionalOrExprContext *context) = 0;

    virtual std::any visitConditionalAndExpr(MQL_Parser::ConditionalAndExprContext *context) = 0;

    virtual std::any visitComparisonExprOp(MQL_Parser::ComparisonExprOpContext *context) = 0;

    virtual std::any visitComparisonExprIs(MQL_Parser::ComparisonExprIsContext *context) = 0;

    virtual std::any visitAdditiveExpr(MQL_Parser::AdditiveExprContext *context) = 0;

    virtual std::any visitMultiplicativeExpr(MQL_Parser::MultiplicativeExprContext *context) = 0;

    virtual std::any visitUnaryExpr(MQL_Parser::UnaryExprContext *context) = 0;

    virtual std::any visitExprVar(MQL_Parser::ExprVarContext *context) = 0;

    virtual std::any visitExprValue(MQL_Parser::ExprValueContext *context) = 0;

    virtual std::any visitExprFixedNodeInside(MQL_Parser::ExprFixedNodeInsideContext *context) = 0;

    virtual std::any visitExprParenthesis(MQL_Parser::ExprParenthesisContext *context) = 0;

    virtual std::any visitExprFunction(MQL_Parser::ExprFunctionContext *context) = 0;

    virtual std::any visitFunction(MQL_Parser::FunctionContext *context) = 0;

    virtual std::any visitRegex(MQL_Parser::RegexContext *context) = 0;

    virtual std::any visitTextSearch(MQL_Parser::TextSearchContext *context) = 0;

    virtual std::any visitCosineSimilarity(MQL_Parser::CosineSimilarityContext *context) = 0;

    virtual std::any visitCosineDistance(MQL_Parser::CosineDistanceContext *context) = 0;

    virtual std::any visitManhattanDistance(MQL_Parser::ManhattanDistanceContext *context) = 0;

    virtual std::any visitEuclideanDistance(MQL_Parser::EuclideanDistanceContext *context) = 0;

    virtual std::any visitTextSearchIndexMode(MQL_Parser::TextSearchIndexModeContext *context) = 0;

    virtual std::any visitExprTypename(MQL_Parser::ExprTypenameContext *context) = 0;

    virtual std::any visitKeyword(MQL_Parser::KeywordContext *context) = 0;


};


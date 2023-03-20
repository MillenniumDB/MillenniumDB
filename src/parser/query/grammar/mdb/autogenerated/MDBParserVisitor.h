
// Generated from MDBParser.g4 by ANTLR 4.9.3

#pragma once


#include "antlr4-runtime.h"
#include "MDBParser.h"



/**
 * This class defines an abstract visitor for a parse tree
 * produced by MDBParser.
 */
class  MDBParserVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by MDBParser.
   */
    virtual antlrcpp::Any visitRoot(MDBParser::RootContext *context) = 0;

    virtual antlrcpp::Any visitMatchQuery(MDBParser::MatchQueryContext *context) = 0;

    virtual antlrcpp::Any visitDescribeQuery(MDBParser::DescribeQueryContext *context) = 0;

    virtual antlrcpp::Any visitInsertQuery(MDBParser::InsertQueryContext *context) = 0;

    virtual antlrcpp::Any visitInsertLabelList(MDBParser::InsertLabelListContext *context) = 0;

    virtual antlrcpp::Any visitInsertPropertyList(MDBParser::InsertPropertyListContext *context) = 0;

    virtual antlrcpp::Any visitInsertEdgeList(MDBParser::InsertEdgeListContext *context) = 0;

    virtual antlrcpp::Any visitInsertLabelElement(MDBParser::InsertLabelElementContext *context) = 0;

    virtual antlrcpp::Any visitInsertPropertyElement(MDBParser::InsertPropertyElementContext *context) = 0;

    virtual antlrcpp::Any visitInsertEdgeElement(MDBParser::InsertEdgeElementContext *context) = 0;

    virtual antlrcpp::Any visitSetStatement(MDBParser::SetStatementContext *context) = 0;

    virtual antlrcpp::Any visitMatchStatement(MDBParser::MatchStatementContext *context) = 0;

    virtual antlrcpp::Any visitWhereStatement(MDBParser::WhereStatementContext *context) = 0;

    virtual antlrcpp::Any visitGroupByStatement(MDBParser::GroupByStatementContext *context) = 0;

    virtual antlrcpp::Any visitOrderByStatement(MDBParser::OrderByStatementContext *context) = 0;

    virtual antlrcpp::Any visitReturnList(MDBParser::ReturnListContext *context) = 0;

    virtual antlrcpp::Any visitReturnAll(MDBParser::ReturnAllContext *context) = 0;

    virtual antlrcpp::Any visitSetItem(MDBParser::SetItemContext *context) = 0;

    virtual antlrcpp::Any visitReturnItemVar(MDBParser::ReturnItemVarContext *context) = 0;

    virtual antlrcpp::Any visitReturnItemAgg(MDBParser::ReturnItemAggContext *context) = 0;

    virtual antlrcpp::Any visitReturnItemCount(MDBParser::ReturnItemCountContext *context) = 0;

    virtual antlrcpp::Any visitAggregateFunc(MDBParser::AggregateFuncContext *context) = 0;

    virtual antlrcpp::Any visitOrderByItemVar(MDBParser::OrderByItemVarContext *context) = 0;

    virtual antlrcpp::Any visitOrderByItemAgg(MDBParser::OrderByItemAggContext *context) = 0;

    virtual antlrcpp::Any visitOrderByItemCount(MDBParser::OrderByItemCountContext *context) = 0;

    virtual antlrcpp::Any visitGroupByItem(MDBParser::GroupByItemContext *context) = 0;

    virtual antlrcpp::Any visitGraphPattern(MDBParser::GraphPatternContext *context) = 0;

    virtual antlrcpp::Any visitOptionalPattern(MDBParser::OptionalPatternContext *context) = 0;

    virtual antlrcpp::Any visitBasicPattern(MDBParser::BasicPatternContext *context) = 0;

    virtual antlrcpp::Any visitLinearPattern(MDBParser::LinearPatternContext *context) = 0;

    virtual antlrcpp::Any visitPath(MDBParser::PathContext *context) = 0;

    virtual antlrcpp::Any visitPathAlternatives(MDBParser::PathAlternativesContext *context) = 0;

    virtual antlrcpp::Any visitPathSequence(MDBParser::PathSequenceContext *context) = 0;

    virtual antlrcpp::Any visitPathAtomSimple(MDBParser::PathAtomSimpleContext *context) = 0;

    virtual antlrcpp::Any visitPathAtomAlternatives(MDBParser::PathAtomAlternativesContext *context) = 0;

    virtual antlrcpp::Any visitPathSuffix(MDBParser::PathSuffixContext *context) = 0;

    virtual antlrcpp::Any visitPathType(MDBParser::PathTypeContext *context) = 0;

    virtual antlrcpp::Any visitNode(MDBParser::NodeContext *context) = 0;

    virtual antlrcpp::Any visitFixedNode(MDBParser::FixedNodeContext *context) = 0;

    virtual antlrcpp::Any visitFixedNodeInside(MDBParser::FixedNodeInsideContext *context) = 0;

    virtual antlrcpp::Any visitEdge(MDBParser::EdgeContext *context) = 0;

    virtual antlrcpp::Any visitEdgeInside(MDBParser::EdgeInsideContext *context) = 0;

    virtual antlrcpp::Any visitVarNode(MDBParser::VarNodeContext *context) = 0;

    virtual antlrcpp::Any visitProperties(MDBParser::PropertiesContext *context) = 0;

    virtual antlrcpp::Any visitProperty(MDBParser::PropertyContext *context) = 0;

    virtual antlrcpp::Any visitIdentifier(MDBParser::IdentifierContext *context) = 0;

    virtual antlrcpp::Any visitBoolValue(MDBParser::BoolValueContext *context) = 0;

    virtual antlrcpp::Any visitNumericValue(MDBParser::NumericValueContext *context) = 0;

    virtual antlrcpp::Any visitValue(MDBParser::ValueContext *context) = 0;

    virtual antlrcpp::Any visitConditionalOrExpr(MDBParser::ConditionalOrExprContext *context) = 0;

    virtual antlrcpp::Any visitConditionalAndExpr(MDBParser::ConditionalAndExprContext *context) = 0;

    virtual antlrcpp::Any visitComparisonExprOp(MDBParser::ComparisonExprOpContext *context) = 0;

    virtual antlrcpp::Any visitComparisonExprIs(MDBParser::ComparisonExprIsContext *context) = 0;

    virtual antlrcpp::Any visitAdditiveExpr(MDBParser::AdditiveExprContext *context) = 0;

    virtual antlrcpp::Any visitMultiplicativeExpr(MDBParser::MultiplicativeExprContext *context) = 0;

    virtual antlrcpp::Any visitUnaryExpr(MDBParser::UnaryExprContext *context) = 0;

    virtual antlrcpp::Any visitExprVar(MDBParser::ExprVarContext *context) = 0;

    virtual antlrcpp::Any visitExprValueExpr(MDBParser::ExprValueExprContext *context) = 0;

    virtual antlrcpp::Any visitExprParenthesis(MDBParser::ExprParenthesisContext *context) = 0;

    virtual antlrcpp::Any visitValueExpr(MDBParser::ValueExprContext *context) = 0;

    virtual antlrcpp::Any visitExprTypename(MDBParser::ExprTypenameContext *context) = 0;

    virtual antlrcpp::Any visitKeyword(MDBParser::KeywordContext *context) = 0;


};


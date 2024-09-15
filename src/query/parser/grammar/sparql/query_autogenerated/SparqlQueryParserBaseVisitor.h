
// Generated from SparqlQueryParser.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"
#include "SparqlQueryParserVisitor.h"


/**
 * This class provides an empty implementation of SparqlQueryParserVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  SparqlQueryParserBaseVisitor : public SparqlQueryParserVisitor {
public:

  virtual std::any visitQuery(SparqlQueryParser::QueryContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPrologue(SparqlQueryParser::PrologueContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitBaseDecl(SparqlQueryParser::BaseDeclContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPrefixDecl(SparqlQueryParser::PrefixDeclContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSelectQuery(SparqlQueryParser::SelectQueryContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSubSelect(SparqlQueryParser::SubSelectContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSelectClause(SparqlQueryParser::SelectClauseContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSelectModifier(SparqlQueryParser::SelectModifierContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSelectSingleVariable(SparqlQueryParser::SelectSingleVariableContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSelectExpressionAsVariable(SparqlQueryParser::SelectExpressionAsVariableContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitConstructQuery(SparqlQueryParser::ConstructQueryContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitDescribeQuery(SparqlQueryParser::DescribeQueryContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAskQuery(SparqlQueryParser::AskQueryContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitDatasetClause(SparqlQueryParser::DatasetClauseContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitWhereClause(SparqlQueryParser::WhereClauseContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSolutionModifier(SparqlQueryParser::SolutionModifierContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitGroupClause(SparqlQueryParser::GroupClauseContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitGroupCondition(SparqlQueryParser::GroupConditionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitHavingClause(SparqlQueryParser::HavingClauseContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitHavingCondition(SparqlQueryParser::HavingConditionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitOrderClause(SparqlQueryParser::OrderClauseContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitOrderCondition(SparqlQueryParser::OrderConditionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLimitOffsetClauses(SparqlQueryParser::LimitOffsetClausesContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLimitClause(SparqlQueryParser::LimitClauseContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitOffsetClause(SparqlQueryParser::OffsetClauseContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitValuesClause(SparqlQueryParser::ValuesClauseContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitTriplesTemplate(SparqlQueryParser::TriplesTemplateContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitGroupGraphPattern(SparqlQueryParser::GroupGraphPatternContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitGroupGraphPatternSub(SparqlQueryParser::GroupGraphPatternSubContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitGroupGraphPatternSubList(SparqlQueryParser::GroupGraphPatternSubListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitTriplesBlock(SparqlQueryParser::TriplesBlockContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitGraphPatternNotTriples(SparqlQueryParser::GraphPatternNotTriplesContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitOptionalGraphPattern(SparqlQueryParser::OptionalGraphPatternContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitGraphGraphPattern(SparqlQueryParser::GraphGraphPatternContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitServiceGraphPattern(SparqlQueryParser::ServiceGraphPatternContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitBind(SparqlQueryParser::BindContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitInlineData(SparqlQueryParser::InlineDataContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitDataBlock(SparqlQueryParser::DataBlockContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitInlineDataOneVar(SparqlQueryParser::InlineDataOneVarContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitInlineDataFull(SparqlQueryParser::InlineDataFullContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitDataBlockValues(SparqlQueryParser::DataBlockValuesContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitDataBlockValue(SparqlQueryParser::DataBlockValueContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitMinusGraphPattern(SparqlQueryParser::MinusGraphPatternContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitGroupOrUnionGraphPattern(SparqlQueryParser::GroupOrUnionGraphPatternContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFilter(SparqlQueryParser::FilterContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitConstraint(SparqlQueryParser::ConstraintContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFunctionCall(SparqlQueryParser::FunctionCallContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitArgList(SparqlQueryParser::ArgListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExpressionList(SparqlQueryParser::ExpressionListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitConstructTemplate(SparqlQueryParser::ConstructTemplateContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitConstructTriples(SparqlQueryParser::ConstructTriplesContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitTriplesSameSubject(SparqlQueryParser::TriplesSameSubjectContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPropertyList(SparqlQueryParser::PropertyListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPropertyListNotEmpty(SparqlQueryParser::PropertyListNotEmptyContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitVerb(SparqlQueryParser::VerbContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitObjectList(SparqlQueryParser::ObjectListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitObject(SparqlQueryParser::ObjectContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitTriplesSameSubjectPath(SparqlQueryParser::TriplesSameSubjectPathContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPropertyListPath(SparqlQueryParser::PropertyListPathContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPropertyListPathNotEmpty(SparqlQueryParser::PropertyListPathNotEmptyContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPropertyListPathNotEmptyList(SparqlQueryParser::PropertyListPathNotEmptyListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitVerbPath(SparqlQueryParser::VerbPathContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitVerbSimple(SparqlQueryParser::VerbSimpleContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitObjectListPath(SparqlQueryParser::ObjectListPathContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitObjectPath(SparqlQueryParser::ObjectPathContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPath(SparqlQueryParser::PathContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPathAlternative(SparqlQueryParser::PathAlternativeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPathSequence(SparqlQueryParser::PathSequenceContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPathElt(SparqlQueryParser::PathEltContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPathEltOrInverse(SparqlQueryParser::PathEltOrInverseContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPathMod(SparqlQueryParser::PathModContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPathQuantity(SparqlQueryParser::PathQuantityContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPathQuantityExact(SparqlQueryParser::PathQuantityExactContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPathQuantityRange(SparqlQueryParser::PathQuantityRangeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPathQuantityMin(SparqlQueryParser::PathQuantityMinContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPathQuantityMax(SparqlQueryParser::PathQuantityMaxContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPathPrimary(SparqlQueryParser::PathPrimaryContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPathNegatedPropertySet(SparqlQueryParser::PathNegatedPropertySetContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPathOneInPropertySet(SparqlQueryParser::PathOneInPropertySetContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitInteger(SparqlQueryParser::IntegerContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitTriplesNode(SparqlQueryParser::TriplesNodeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitBlankNodePropertyList(SparqlQueryParser::BlankNodePropertyListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitTriplesNodePath(SparqlQueryParser::TriplesNodePathContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitBlankNodePropertyListPath(SparqlQueryParser::BlankNodePropertyListPathContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCollection(SparqlQueryParser::CollectionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCollectionPath(SparqlQueryParser::CollectionPathContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitGraphNode(SparqlQueryParser::GraphNodeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitGraphNodePath(SparqlQueryParser::GraphNodePathContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitVarOrTerm(SparqlQueryParser::VarOrTermContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitVarOrIRI(SparqlQueryParser::VarOrIRIContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitVar(SparqlQueryParser::VarContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitGraphTerm(SparqlQueryParser::GraphTermContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitNil(SparqlQueryParser::NilContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExpression(SparqlQueryParser::ExpressionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitConditionalOrExpression(SparqlQueryParser::ConditionalOrExpressionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitConditionalAndExpression(SparqlQueryParser::ConditionalAndExpressionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitRelationalExpression(SparqlQueryParser::RelationalExpressionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAdditiveExpression(SparqlQueryParser::AdditiveExpressionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitRhsAdditiveExpression(SparqlQueryParser::RhsAdditiveExpressionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitRhsAdditiveExpressionSub(SparqlQueryParser::RhsAdditiveExpressionSubContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitMultiplicativeExpression(SparqlQueryParser::MultiplicativeExpressionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitUnaryExpression(SparqlQueryParser::UnaryExpressionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPrimaryExpression(SparqlQueryParser::PrimaryExpressionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitBuiltInCall(SparqlQueryParser::BuiltInCallContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitRegexExpression(SparqlQueryParser::RegexExpressionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSubStringExpression(SparqlQueryParser::SubStringExpressionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitStrReplaceExpression(SparqlQueryParser::StrReplaceExpressionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExistsFunction(SparqlQueryParser::ExistsFunctionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitNotExistsFunction(SparqlQueryParser::NotExistsFunctionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAggregate(SparqlQueryParser::AggregateContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitIriOrFunction(SparqlQueryParser::IriOrFunctionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitRdfLiteral(SparqlQueryParser::RdfLiteralContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitNumericLiteral(SparqlQueryParser::NumericLiteralContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitNumericLiteralUnsigned(SparqlQueryParser::NumericLiteralUnsignedContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitNumericLiteralPositive(SparqlQueryParser::NumericLiteralPositiveContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitNumericLiteralNegative(SparqlQueryParser::NumericLiteralNegativeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitBooleanLiteral(SparqlQueryParser::BooleanLiteralContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitString(SparqlQueryParser::StringContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitIri(SparqlQueryParser::IriContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPrefixedName(SparqlQueryParser::PrefixedNameContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitBlankNode(SparqlQueryParser::BlankNodeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAnon(SparqlQueryParser::AnonContext *ctx) override {
    return visitChildren(ctx);
  }


};


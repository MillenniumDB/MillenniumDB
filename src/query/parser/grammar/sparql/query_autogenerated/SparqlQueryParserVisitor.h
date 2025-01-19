
// Generated from SparqlQueryParser.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"
#include "SparqlQueryParser.h"



/**
 * This class defines an abstract visitor for a parse tree
 * produced by SparqlQueryParser.
 */
class  SparqlQueryParserVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by SparqlQueryParser.
   */
    virtual std::any visitQuery(SparqlQueryParser::QueryContext *context) = 0;

    virtual std::any visitShowQuery(SparqlQueryParser::ShowQueryContext *context) = 0;

    virtual std::any visitPrologue(SparqlQueryParser::PrologueContext *context) = 0;

    virtual std::any visitBaseDecl(SparqlQueryParser::BaseDeclContext *context) = 0;

    virtual std::any visitPrefixDecl(SparqlQueryParser::PrefixDeclContext *context) = 0;

    virtual std::any visitSelectQuery(SparqlQueryParser::SelectQueryContext *context) = 0;

    virtual std::any visitSubSelect(SparqlQueryParser::SubSelectContext *context) = 0;

    virtual std::any visitSelectClause(SparqlQueryParser::SelectClauseContext *context) = 0;

    virtual std::any visitSelectModifier(SparqlQueryParser::SelectModifierContext *context) = 0;

    virtual std::any visitSelectSingleVariable(SparqlQueryParser::SelectSingleVariableContext *context) = 0;

    virtual std::any visitSelectExpressionAsVariable(SparqlQueryParser::SelectExpressionAsVariableContext *context) = 0;

    virtual std::any visitConstructQuery(SparqlQueryParser::ConstructQueryContext *context) = 0;

    virtual std::any visitDescribeQuery(SparqlQueryParser::DescribeQueryContext *context) = 0;

    virtual std::any visitAskQuery(SparqlQueryParser::AskQueryContext *context) = 0;

    virtual std::any visitDatasetClause(SparqlQueryParser::DatasetClauseContext *context) = 0;

    virtual std::any visitWhereClause(SparqlQueryParser::WhereClauseContext *context) = 0;

    virtual std::any visitSolutionModifier(SparqlQueryParser::SolutionModifierContext *context) = 0;

    virtual std::any visitGroupClause(SparqlQueryParser::GroupClauseContext *context) = 0;

    virtual std::any visitGroupCondition(SparqlQueryParser::GroupConditionContext *context) = 0;

    virtual std::any visitHavingClause(SparqlQueryParser::HavingClauseContext *context) = 0;

    virtual std::any visitHavingCondition(SparqlQueryParser::HavingConditionContext *context) = 0;

    virtual std::any visitOrderClause(SparqlQueryParser::OrderClauseContext *context) = 0;

    virtual std::any visitOrderCondition(SparqlQueryParser::OrderConditionContext *context) = 0;

    virtual std::any visitLimitOffsetClauses(SparqlQueryParser::LimitOffsetClausesContext *context) = 0;

    virtual std::any visitLimitClause(SparqlQueryParser::LimitClauseContext *context) = 0;

    virtual std::any visitOffsetClause(SparqlQueryParser::OffsetClauseContext *context) = 0;

    virtual std::any visitValuesClause(SparqlQueryParser::ValuesClauseContext *context) = 0;

    virtual std::any visitTriplesTemplate(SparqlQueryParser::TriplesTemplateContext *context) = 0;

    virtual std::any visitGroupGraphPattern(SparqlQueryParser::GroupGraphPatternContext *context) = 0;

    virtual std::any visitGroupGraphPatternSub(SparqlQueryParser::GroupGraphPatternSubContext *context) = 0;

    virtual std::any visitGroupGraphPatternSubList(SparqlQueryParser::GroupGraphPatternSubListContext *context) = 0;

    virtual std::any visitTriplesBlock(SparqlQueryParser::TriplesBlockContext *context) = 0;

    virtual std::any visitGraphPatternNotTriples(SparqlQueryParser::GraphPatternNotTriplesContext *context) = 0;

    virtual std::any visitOptionalGraphPattern(SparqlQueryParser::OptionalGraphPatternContext *context) = 0;

    virtual std::any visitGraphGraphPattern(SparqlQueryParser::GraphGraphPatternContext *context) = 0;

    virtual std::any visitServiceGraphPattern(SparqlQueryParser::ServiceGraphPatternContext *context) = 0;

    virtual std::any visitBind(SparqlQueryParser::BindContext *context) = 0;

    virtual std::any visitInlineData(SparqlQueryParser::InlineDataContext *context) = 0;

    virtual std::any visitDataBlock(SparqlQueryParser::DataBlockContext *context) = 0;

    virtual std::any visitInlineDataOneVar(SparqlQueryParser::InlineDataOneVarContext *context) = 0;

    virtual std::any visitInlineDataFull(SparqlQueryParser::InlineDataFullContext *context) = 0;

    virtual std::any visitDataBlockValues(SparqlQueryParser::DataBlockValuesContext *context) = 0;

    virtual std::any visitDataBlockValue(SparqlQueryParser::DataBlockValueContext *context) = 0;

    virtual std::any visitMinusGraphPattern(SparqlQueryParser::MinusGraphPatternContext *context) = 0;

    virtual std::any visitGroupOrUnionGraphPattern(SparqlQueryParser::GroupOrUnionGraphPatternContext *context) = 0;

    virtual std::any visitFilter(SparqlQueryParser::FilterContext *context) = 0;

    virtual std::any visitConstraint(SparqlQueryParser::ConstraintContext *context) = 0;

    virtual std::any visitFunctionCall(SparqlQueryParser::FunctionCallContext *context) = 0;

    virtual std::any visitArgList(SparqlQueryParser::ArgListContext *context) = 0;

    virtual std::any visitExpressionList(SparqlQueryParser::ExpressionListContext *context) = 0;

    virtual std::any visitConstructTemplate(SparqlQueryParser::ConstructTemplateContext *context) = 0;

    virtual std::any visitConstructTriples(SparqlQueryParser::ConstructTriplesContext *context) = 0;

    virtual std::any visitTriplesSameSubject(SparqlQueryParser::TriplesSameSubjectContext *context) = 0;

    virtual std::any visitPropertyList(SparqlQueryParser::PropertyListContext *context) = 0;

    virtual std::any visitPropertyListNotEmpty(SparqlQueryParser::PropertyListNotEmptyContext *context) = 0;

    virtual std::any visitVerb(SparqlQueryParser::VerbContext *context) = 0;

    virtual std::any visitObjectList(SparqlQueryParser::ObjectListContext *context) = 0;

    virtual std::any visitObject(SparqlQueryParser::ObjectContext *context) = 0;

    virtual std::any visitTriplesSameSubjectPath(SparqlQueryParser::TriplesSameSubjectPathContext *context) = 0;

    virtual std::any visitPropertyListPath(SparqlQueryParser::PropertyListPathContext *context) = 0;

    virtual std::any visitPropertyListPathNotEmpty(SparqlQueryParser::PropertyListPathNotEmptyContext *context) = 0;

    virtual std::any visitPropertyListPathNotEmptyList(SparqlQueryParser::PropertyListPathNotEmptyListContext *context) = 0;

    virtual std::any visitVerbPath(SparqlQueryParser::VerbPathContext *context) = 0;

    virtual std::any visitVerbSimple(SparqlQueryParser::VerbSimpleContext *context) = 0;

    virtual std::any visitObjectListPath(SparqlQueryParser::ObjectListPathContext *context) = 0;

    virtual std::any visitObjectPath(SparqlQueryParser::ObjectPathContext *context) = 0;

    virtual std::any visitPath(SparqlQueryParser::PathContext *context) = 0;

    virtual std::any visitPathAlternative(SparqlQueryParser::PathAlternativeContext *context) = 0;

    virtual std::any visitPathSequence(SparqlQueryParser::PathSequenceContext *context) = 0;

    virtual std::any visitPathElt(SparqlQueryParser::PathEltContext *context) = 0;

    virtual std::any visitPathEltOrInverse(SparqlQueryParser::PathEltOrInverseContext *context) = 0;

    virtual std::any visitPathMod(SparqlQueryParser::PathModContext *context) = 0;

    virtual std::any visitPathQuantity(SparqlQueryParser::PathQuantityContext *context) = 0;

    virtual std::any visitPathQuantityExact(SparqlQueryParser::PathQuantityExactContext *context) = 0;

    virtual std::any visitPathQuantityRange(SparqlQueryParser::PathQuantityRangeContext *context) = 0;

    virtual std::any visitPathQuantityMin(SparqlQueryParser::PathQuantityMinContext *context) = 0;

    virtual std::any visitPathQuantityMax(SparqlQueryParser::PathQuantityMaxContext *context) = 0;

    virtual std::any visitPathPrimary(SparqlQueryParser::PathPrimaryContext *context) = 0;

    virtual std::any visitPathNegatedPropertySet(SparqlQueryParser::PathNegatedPropertySetContext *context) = 0;

    virtual std::any visitPathOneInPropertySet(SparqlQueryParser::PathOneInPropertySetContext *context) = 0;

    virtual std::any visitInteger(SparqlQueryParser::IntegerContext *context) = 0;

    virtual std::any visitTriplesNode(SparqlQueryParser::TriplesNodeContext *context) = 0;

    virtual std::any visitBlankNodePropertyList(SparqlQueryParser::BlankNodePropertyListContext *context) = 0;

    virtual std::any visitTriplesNodePath(SparqlQueryParser::TriplesNodePathContext *context) = 0;

    virtual std::any visitBlankNodePropertyListPath(SparqlQueryParser::BlankNodePropertyListPathContext *context) = 0;

    virtual std::any visitCollection(SparqlQueryParser::CollectionContext *context) = 0;

    virtual std::any visitCollectionPath(SparqlQueryParser::CollectionPathContext *context) = 0;

    virtual std::any visitGraphNode(SparqlQueryParser::GraphNodeContext *context) = 0;

    virtual std::any visitGraphNodePath(SparqlQueryParser::GraphNodePathContext *context) = 0;

    virtual std::any visitVarOrTerm(SparqlQueryParser::VarOrTermContext *context) = 0;

    virtual std::any visitVarOrIRI(SparqlQueryParser::VarOrIRIContext *context) = 0;

    virtual std::any visitVar(SparqlQueryParser::VarContext *context) = 0;

    virtual std::any visitGraphTerm(SparqlQueryParser::GraphTermContext *context) = 0;

    virtual std::any visitNil(SparqlQueryParser::NilContext *context) = 0;

    virtual std::any visitExpression(SparqlQueryParser::ExpressionContext *context) = 0;

    virtual std::any visitConditionalOrExpression(SparqlQueryParser::ConditionalOrExpressionContext *context) = 0;

    virtual std::any visitConditionalAndExpression(SparqlQueryParser::ConditionalAndExpressionContext *context) = 0;

    virtual std::any visitRelationalExpression(SparqlQueryParser::RelationalExpressionContext *context) = 0;

    virtual std::any visitAdditiveExpression(SparqlQueryParser::AdditiveExpressionContext *context) = 0;

    virtual std::any visitRhsAdditiveExpression(SparqlQueryParser::RhsAdditiveExpressionContext *context) = 0;

    virtual std::any visitRhsAdditiveExpressionSub(SparqlQueryParser::RhsAdditiveExpressionSubContext *context) = 0;

    virtual std::any visitMultiplicativeExpression(SparqlQueryParser::MultiplicativeExpressionContext *context) = 0;

    virtual std::any visitUnaryExpression(SparqlQueryParser::UnaryExpressionContext *context) = 0;

    virtual std::any visitPrimaryExpression(SparqlQueryParser::PrimaryExpressionContext *context) = 0;

    virtual std::any visitBuiltInCall(SparqlQueryParser::BuiltInCallContext *context) = 0;

    virtual std::any visitRegexExpression(SparqlQueryParser::RegexExpressionContext *context) = 0;

    virtual std::any visitSubStringExpression(SparqlQueryParser::SubStringExpressionContext *context) = 0;

    virtual std::any visitStrReplaceExpression(SparqlQueryParser::StrReplaceExpressionContext *context) = 0;

    virtual std::any visitExistsFunction(SparqlQueryParser::ExistsFunctionContext *context) = 0;

    virtual std::any visitNotExistsFunction(SparqlQueryParser::NotExistsFunctionContext *context) = 0;

    virtual std::any visitAggregate(SparqlQueryParser::AggregateContext *context) = 0;

    virtual std::any visitIriOrFunction(SparqlQueryParser::IriOrFunctionContext *context) = 0;

    virtual std::any visitRdfLiteral(SparqlQueryParser::RdfLiteralContext *context) = 0;

    virtual std::any visitNumericLiteral(SparqlQueryParser::NumericLiteralContext *context) = 0;

    virtual std::any visitNumericLiteralUnsigned(SparqlQueryParser::NumericLiteralUnsignedContext *context) = 0;

    virtual std::any visitNumericLiteralPositive(SparqlQueryParser::NumericLiteralPositiveContext *context) = 0;

    virtual std::any visitNumericLiteralNegative(SparqlQueryParser::NumericLiteralNegativeContext *context) = 0;

    virtual std::any visitBooleanLiteral(SparqlQueryParser::BooleanLiteralContext *context) = 0;

    virtual std::any visitString(SparqlQueryParser::StringContext *context) = 0;

    virtual std::any visitIri(SparqlQueryParser::IriContext *context) = 0;

    virtual std::any visitPrefixedName(SparqlQueryParser::PrefixedNameContext *context) = 0;

    virtual std::any visitBlankNode(SparqlQueryParser::BlankNodeContext *context) = 0;

    virtual std::any visitAnon(SparqlQueryParser::AnonContext *context) = 0;


};


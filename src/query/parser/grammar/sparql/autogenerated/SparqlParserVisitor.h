
// Generated from SparqlParser.g4 by ANTLR 4.9.3

#pragma once


#include "antlr4-runtime.h"
#include "SparqlParser.h"



/**
 * This class defines an abstract visitor for a parse tree
 * produced by SparqlParser.
 */
class  SparqlParserVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by SparqlParser.
   */
    virtual antlrcpp::Any visitQuery(SparqlParser::QueryContext *context) = 0;

    virtual antlrcpp::Any visitPrologue(SparqlParser::PrologueContext *context) = 0;

    virtual antlrcpp::Any visitBaseDecl(SparqlParser::BaseDeclContext *context) = 0;

    virtual antlrcpp::Any visitPrefixDecl(SparqlParser::PrefixDeclContext *context) = 0;

    virtual antlrcpp::Any visitSelectQuery(SparqlParser::SelectQueryContext *context) = 0;

    virtual antlrcpp::Any visitSubSelect(SparqlParser::SubSelectContext *context) = 0;

    virtual antlrcpp::Any visitSelectClause(SparqlParser::SelectClauseContext *context) = 0;

    virtual antlrcpp::Any visitSelectModifier(SparqlParser::SelectModifierContext *context) = 0;

    virtual antlrcpp::Any visitSelectSingleVariable(SparqlParser::SelectSingleVariableContext *context) = 0;

    virtual antlrcpp::Any visitSelectExpressionAsVariable(SparqlParser::SelectExpressionAsVariableContext *context) = 0;

    virtual antlrcpp::Any visitConstructQuery(SparqlParser::ConstructQueryContext *context) = 0;

    virtual antlrcpp::Any visitDescribeQuery(SparqlParser::DescribeQueryContext *context) = 0;

    virtual antlrcpp::Any visitAskQuery(SparqlParser::AskQueryContext *context) = 0;

    virtual antlrcpp::Any visitDatasetClause(SparqlParser::DatasetClauseContext *context) = 0;

    virtual antlrcpp::Any visitWhereClause(SparqlParser::WhereClauseContext *context) = 0;

    virtual antlrcpp::Any visitSolutionModifier(SparqlParser::SolutionModifierContext *context) = 0;

    virtual antlrcpp::Any visitGroupClause(SparqlParser::GroupClauseContext *context) = 0;

    virtual antlrcpp::Any visitGroupCondition(SparqlParser::GroupConditionContext *context) = 0;

    virtual antlrcpp::Any visitHavingClause(SparqlParser::HavingClauseContext *context) = 0;

    virtual antlrcpp::Any visitHavingCondition(SparqlParser::HavingConditionContext *context) = 0;

    virtual antlrcpp::Any visitOrderClause(SparqlParser::OrderClauseContext *context) = 0;

    virtual antlrcpp::Any visitOrderCondition(SparqlParser::OrderConditionContext *context) = 0;

    virtual antlrcpp::Any visitLimitOffsetClauses(SparqlParser::LimitOffsetClausesContext *context) = 0;

    virtual antlrcpp::Any visitLimitClause(SparqlParser::LimitClauseContext *context) = 0;

    virtual antlrcpp::Any visitOffsetClause(SparqlParser::OffsetClauseContext *context) = 0;

    virtual antlrcpp::Any visitValuesClause(SparqlParser::ValuesClauseContext *context) = 0;

    virtual antlrcpp::Any visitUpdateCommand(SparqlParser::UpdateCommandContext *context) = 0;

    virtual antlrcpp::Any visitUpdate(SparqlParser::UpdateContext *context) = 0;

    virtual antlrcpp::Any visitLoad(SparqlParser::LoadContext *context) = 0;

    virtual antlrcpp::Any visitClear(SparqlParser::ClearContext *context) = 0;

    virtual antlrcpp::Any visitDrop(SparqlParser::DropContext *context) = 0;

    virtual antlrcpp::Any visitCreate(SparqlParser::CreateContext *context) = 0;

    virtual antlrcpp::Any visitAdd(SparqlParser::AddContext *context) = 0;

    virtual antlrcpp::Any visitMove(SparqlParser::MoveContext *context) = 0;

    virtual antlrcpp::Any visitCopy(SparqlParser::CopyContext *context) = 0;

    virtual antlrcpp::Any visitInsertData(SparqlParser::InsertDataContext *context) = 0;

    virtual antlrcpp::Any visitDeleteData(SparqlParser::DeleteDataContext *context) = 0;

    virtual antlrcpp::Any visitDeleteWhere(SparqlParser::DeleteWhereContext *context) = 0;

    virtual antlrcpp::Any visitModify(SparqlParser::ModifyContext *context) = 0;

    virtual antlrcpp::Any visitDeleteClause(SparqlParser::DeleteClauseContext *context) = 0;

    virtual antlrcpp::Any visitInsertClause(SparqlParser::InsertClauseContext *context) = 0;

    virtual antlrcpp::Any visitUsingClause(SparqlParser::UsingClauseContext *context) = 0;

    virtual antlrcpp::Any visitGraphOrDefault(SparqlParser::GraphOrDefaultContext *context) = 0;

    virtual antlrcpp::Any visitGraphRef(SparqlParser::GraphRefContext *context) = 0;

    virtual antlrcpp::Any visitGraphRefAll(SparqlParser::GraphRefAllContext *context) = 0;

    virtual antlrcpp::Any visitQuadPattern(SparqlParser::QuadPatternContext *context) = 0;

    virtual antlrcpp::Any visitQuadData(SparqlParser::QuadDataContext *context) = 0;

    virtual antlrcpp::Any visitQuads(SparqlParser::QuadsContext *context) = 0;

    virtual antlrcpp::Any visitQuadsDetails(SparqlParser::QuadsDetailsContext *context) = 0;

    virtual antlrcpp::Any visitQuadsNotTriples(SparqlParser::QuadsNotTriplesContext *context) = 0;

    virtual antlrcpp::Any visitTriplesTemplate(SparqlParser::TriplesTemplateContext *context) = 0;

    virtual antlrcpp::Any visitGroupGraphPattern(SparqlParser::GroupGraphPatternContext *context) = 0;

    virtual antlrcpp::Any visitGroupGraphPatternSub(SparqlParser::GroupGraphPatternSubContext *context) = 0;

    virtual antlrcpp::Any visitGroupGraphPatternSubList(SparqlParser::GroupGraphPatternSubListContext *context) = 0;

    virtual antlrcpp::Any visitTriplesBlock(SparqlParser::TriplesBlockContext *context) = 0;

    virtual antlrcpp::Any visitGraphPatternNotTriples(SparqlParser::GraphPatternNotTriplesContext *context) = 0;

    virtual antlrcpp::Any visitOptionalGraphPattern(SparqlParser::OptionalGraphPatternContext *context) = 0;

    virtual antlrcpp::Any visitGraphGraphPattern(SparqlParser::GraphGraphPatternContext *context) = 0;

    virtual antlrcpp::Any visitServiceGraphPattern(SparqlParser::ServiceGraphPatternContext *context) = 0;

    virtual antlrcpp::Any visitBind(SparqlParser::BindContext *context) = 0;

    virtual antlrcpp::Any visitInlineData(SparqlParser::InlineDataContext *context) = 0;

    virtual antlrcpp::Any visitDataBlock(SparqlParser::DataBlockContext *context) = 0;

    virtual antlrcpp::Any visitInlineDataOneVar(SparqlParser::InlineDataOneVarContext *context) = 0;

    virtual antlrcpp::Any visitInlineDataFull(SparqlParser::InlineDataFullContext *context) = 0;

    virtual antlrcpp::Any visitDataBlockValues(SparqlParser::DataBlockValuesContext *context) = 0;

    virtual antlrcpp::Any visitDataBlockValue(SparqlParser::DataBlockValueContext *context) = 0;

    virtual antlrcpp::Any visitMinusGraphPattern(SparqlParser::MinusGraphPatternContext *context) = 0;

    virtual antlrcpp::Any visitGroupOrUnionGraphPattern(SparqlParser::GroupOrUnionGraphPatternContext *context) = 0;

    virtual antlrcpp::Any visitFilter(SparqlParser::FilterContext *context) = 0;

    virtual antlrcpp::Any visitConstraint(SparqlParser::ConstraintContext *context) = 0;

    virtual antlrcpp::Any visitFunctionCall(SparqlParser::FunctionCallContext *context) = 0;

    virtual antlrcpp::Any visitArgList(SparqlParser::ArgListContext *context) = 0;

    virtual antlrcpp::Any visitExpressionList(SparqlParser::ExpressionListContext *context) = 0;

    virtual antlrcpp::Any visitConstructTemplate(SparqlParser::ConstructTemplateContext *context) = 0;

    virtual antlrcpp::Any visitConstructTriples(SparqlParser::ConstructTriplesContext *context) = 0;

    virtual antlrcpp::Any visitTriplesSameSubject(SparqlParser::TriplesSameSubjectContext *context) = 0;

    virtual antlrcpp::Any visitPropertyList(SparqlParser::PropertyListContext *context) = 0;

    virtual antlrcpp::Any visitPropertyListNotEmpty(SparqlParser::PropertyListNotEmptyContext *context) = 0;

    virtual antlrcpp::Any visitVerb(SparqlParser::VerbContext *context) = 0;

    virtual antlrcpp::Any visitObjectList(SparqlParser::ObjectListContext *context) = 0;

    virtual antlrcpp::Any visitObject(SparqlParser::ObjectContext *context) = 0;

    virtual antlrcpp::Any visitTriplesSameSubjectPath(SparqlParser::TriplesSameSubjectPathContext *context) = 0;

    virtual antlrcpp::Any visitPropertyListPath(SparqlParser::PropertyListPathContext *context) = 0;

    virtual antlrcpp::Any visitPropertyListPathNotEmpty(SparqlParser::PropertyListPathNotEmptyContext *context) = 0;

    virtual antlrcpp::Any visitPropertyListPathNotEmptyList(SparqlParser::PropertyListPathNotEmptyListContext *context) = 0;

    virtual antlrcpp::Any visitVerbPath(SparqlParser::VerbPathContext *context) = 0;

    virtual antlrcpp::Any visitVerbSimple(SparqlParser::VerbSimpleContext *context) = 0;

    virtual antlrcpp::Any visitObjectListPath(SparqlParser::ObjectListPathContext *context) = 0;

    virtual antlrcpp::Any visitObjectPath(SparqlParser::ObjectPathContext *context) = 0;

    virtual antlrcpp::Any visitPath(SparqlParser::PathContext *context) = 0;

    virtual antlrcpp::Any visitPathAlternative(SparqlParser::PathAlternativeContext *context) = 0;

    virtual antlrcpp::Any visitPathSequence(SparqlParser::PathSequenceContext *context) = 0;

    virtual antlrcpp::Any visitPathElt(SparqlParser::PathEltContext *context) = 0;

    virtual antlrcpp::Any visitPathEltOrInverse(SparqlParser::PathEltOrInverseContext *context) = 0;

    virtual antlrcpp::Any visitPathMod(SparqlParser::PathModContext *context) = 0;

    virtual antlrcpp::Any visitPathPrimary(SparqlParser::PathPrimaryContext *context) = 0;

    virtual antlrcpp::Any visitPathNegatedPropertySet(SparqlParser::PathNegatedPropertySetContext *context) = 0;

    virtual antlrcpp::Any visitPathOneInPropertySet(SparqlParser::PathOneInPropertySetContext *context) = 0;

    virtual antlrcpp::Any visitInteger(SparqlParser::IntegerContext *context) = 0;

    virtual antlrcpp::Any visitTriplesNode(SparqlParser::TriplesNodeContext *context) = 0;

    virtual antlrcpp::Any visitBlankNodePropertyList(SparqlParser::BlankNodePropertyListContext *context) = 0;

    virtual antlrcpp::Any visitTriplesNodePath(SparqlParser::TriplesNodePathContext *context) = 0;

    virtual antlrcpp::Any visitBlankNodePropertyListPath(SparqlParser::BlankNodePropertyListPathContext *context) = 0;

    virtual antlrcpp::Any visitCollection(SparqlParser::CollectionContext *context) = 0;

    virtual antlrcpp::Any visitCollectionPath(SparqlParser::CollectionPathContext *context) = 0;

    virtual antlrcpp::Any visitGraphNode(SparqlParser::GraphNodeContext *context) = 0;

    virtual antlrcpp::Any visitGraphNodePath(SparqlParser::GraphNodePathContext *context) = 0;

    virtual antlrcpp::Any visitVarOrTerm(SparqlParser::VarOrTermContext *context) = 0;

    virtual antlrcpp::Any visitVarOrIRI(SparqlParser::VarOrIRIContext *context) = 0;

    virtual antlrcpp::Any visitVar(SparqlParser::VarContext *context) = 0;

    virtual antlrcpp::Any visitGraphTerm(SparqlParser::GraphTermContext *context) = 0;

    virtual antlrcpp::Any visitNil(SparqlParser::NilContext *context) = 0;

    virtual antlrcpp::Any visitExpression(SparqlParser::ExpressionContext *context) = 0;

    virtual antlrcpp::Any visitConditionalOrExpression(SparqlParser::ConditionalOrExpressionContext *context) = 0;

    virtual antlrcpp::Any visitConditionalAndExpression(SparqlParser::ConditionalAndExpressionContext *context) = 0;

    virtual antlrcpp::Any visitRelationalExpression(SparqlParser::RelationalExpressionContext *context) = 0;

    virtual antlrcpp::Any visitAdditiveExpression(SparqlParser::AdditiveExpressionContext *context) = 0;

    virtual antlrcpp::Any visitRhsAdditiveExpression(SparqlParser::RhsAdditiveExpressionContext *context) = 0;

    virtual antlrcpp::Any visitRhsAdditiveExpressionSub(SparqlParser::RhsAdditiveExpressionSubContext *context) = 0;

    virtual antlrcpp::Any visitMultiplicativeExpression(SparqlParser::MultiplicativeExpressionContext *context) = 0;

    virtual antlrcpp::Any visitUnaryExpression(SparqlParser::UnaryExpressionContext *context) = 0;

    virtual antlrcpp::Any visitPrimaryExpression(SparqlParser::PrimaryExpressionContext *context) = 0;

    virtual antlrcpp::Any visitBuiltInCall(SparqlParser::BuiltInCallContext *context) = 0;

    virtual antlrcpp::Any visitRegexExpression(SparqlParser::RegexExpressionContext *context) = 0;

    virtual antlrcpp::Any visitSubStringExpression(SparqlParser::SubStringExpressionContext *context) = 0;

    virtual antlrcpp::Any visitStrReplaceExpression(SparqlParser::StrReplaceExpressionContext *context) = 0;

    virtual antlrcpp::Any visitExistsFunction(SparqlParser::ExistsFunctionContext *context) = 0;

    virtual antlrcpp::Any visitNotExistsFunction(SparqlParser::NotExistsFunctionContext *context) = 0;

    virtual antlrcpp::Any visitAggregate(SparqlParser::AggregateContext *context) = 0;

    virtual antlrcpp::Any visitIriOrFunction(SparqlParser::IriOrFunctionContext *context) = 0;

    virtual antlrcpp::Any visitRdfLiteral(SparqlParser::RdfLiteralContext *context) = 0;

    virtual antlrcpp::Any visitNumericLiteral(SparqlParser::NumericLiteralContext *context) = 0;

    virtual antlrcpp::Any visitNumericLiteralUnsigned(SparqlParser::NumericLiteralUnsignedContext *context) = 0;

    virtual antlrcpp::Any visitNumericLiteralPositive(SparqlParser::NumericLiteralPositiveContext *context) = 0;

    virtual antlrcpp::Any visitNumericLiteralNegative(SparqlParser::NumericLiteralNegativeContext *context) = 0;

    virtual antlrcpp::Any visitBooleanLiteral(SparqlParser::BooleanLiteralContext *context) = 0;

    virtual antlrcpp::Any visitString(SparqlParser::StringContext *context) = 0;

    virtual antlrcpp::Any visitIri(SparqlParser::IriContext *context) = 0;

    virtual antlrcpp::Any visitPrefixedName(SparqlParser::PrefixedNameContext *context) = 0;

    virtual antlrcpp::Any visitBlankNode(SparqlParser::BlankNodeContext *context) = 0;

    virtual antlrcpp::Any visitAnon(SparqlParser::AnonContext *context) = 0;


};



// Generated from GQLParser.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"
#include "GQLParser.h"



/**
 * This class defines an abstract visitor for a parse tree
 * produced by GQLParser.
 */
class  GQLParserVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by GQLParser.
   */
    virtual std::any visitRoot(GQLParser::RootContext *context) = 0;

    virtual std::any visitGqlProgram(GQLParser::GqlProgramContext *context) = 0;

    virtual std::any visitProgramActivity(GQLParser::ProgramActivityContext *context) = 0;

    virtual std::any visitSessionActivity(GQLParser::SessionActivityContext *context) = 0;

    virtual std::any visitSessionActivityCommand(GQLParser::SessionActivityCommandContext *context) = 0;

    virtual std::any visitTransactionActivity(GQLParser::TransactionActivityContext *context) = 0;

    virtual std::any visitEndTransactionCommand(GQLParser::EndTransactionCommandContext *context) = 0;

    virtual std::any visitSessionSetCommand(GQLParser::SessionSetCommandContext *context) = 0;

    virtual std::any visitSessionSetSchemaClause(GQLParser::SessionSetSchemaClauseContext *context) = 0;

    virtual std::any visitSessionSetGraphClause(GQLParser::SessionSetGraphClauseContext *context) = 0;

    virtual std::any visitSessionSetTimeZoneClause(GQLParser::SessionSetTimeZoneClauseContext *context) = 0;

    virtual std::any visitSetTimeZoneValue(GQLParser::SetTimeZoneValueContext *context) = 0;

    virtual std::any visitSessionSetParameterClause(GQLParser::SessionSetParameterClauseContext *context) = 0;

    virtual std::any visitSessionSetGraphParameterClause(GQLParser::SessionSetGraphParameterClauseContext *context) = 0;

    virtual std::any visitSessionSetBindingTableParameterClause(GQLParser::SessionSetBindingTableParameterClauseContext *context) = 0;

    virtual std::any visitSessionSetValueParameterClause(GQLParser::SessionSetValueParameterClauseContext *context) = 0;

    virtual std::any visitSessionSetParameterName(GQLParser::SessionSetParameterNameContext *context) = 0;

    virtual std::any visitSessionResetCommand(GQLParser::SessionResetCommandContext *context) = 0;

    virtual std::any visitSessionResetArguments(GQLParser::SessionResetArgumentsContext *context) = 0;

    virtual std::any visitSessionCloseCommand(GQLParser::SessionCloseCommandContext *context) = 0;

    virtual std::any visitStartTransactionCommand(GQLParser::StartTransactionCommandContext *context) = 0;

    virtual std::any visitTransactionCharacteristics(GQLParser::TransactionCharacteristicsContext *context) = 0;

    virtual std::any visitTransactionMode(GQLParser::TransactionModeContext *context) = 0;

    virtual std::any visitTransactionAccessMode(GQLParser::TransactionAccessModeContext *context) = 0;

    virtual std::any visitImplementationDefinedAccessMode(GQLParser::ImplementationDefinedAccessModeContext *context) = 0;

    virtual std::any visitRollbackCommand(GQLParser::RollbackCommandContext *context) = 0;

    virtual std::any visitCommitCommand(GQLParser::CommitCommandContext *context) = 0;

    virtual std::any visitNestedProcedureSpecification(GQLParser::NestedProcedureSpecificationContext *context) = 0;

    virtual std::any visitProcedureSpecification(GQLParser::ProcedureSpecificationContext *context) = 0;

    virtual std::any visitCatalogModifyingProcedureSpecification(GQLParser::CatalogModifyingProcedureSpecificationContext *context) = 0;

    virtual std::any visitNestedDataModifyingProcedureSpecification(GQLParser::NestedDataModifyingProcedureSpecificationContext *context) = 0;

    virtual std::any visitDataModifyingProcedureSpecification(GQLParser::DataModifyingProcedureSpecificationContext *context) = 0;

    virtual std::any visitNestedQuerySpecification(GQLParser::NestedQuerySpecificationContext *context) = 0;

    virtual std::any visitQuerySpecification(GQLParser::QuerySpecificationContext *context) = 0;

    virtual std::any visitUnsignedNumericLiteral(GQLParser::UnsignedNumericLiteralContext *context) = 0;

    virtual std::any visitIntegerLiteral(GQLParser::IntegerLiteralContext *context) = 0;

    virtual std::any visitFloatLiteral(GQLParser::FloatLiteralContext *context) = 0;

    virtual std::any visitUnbrokenSingleQuotedCharacterSequence(GQLParser::UnbrokenSingleQuotedCharacterSequenceContext *context) = 0;

    virtual std::any visitUnbrokenDoubleQuotedCharacterSequence(GQLParser::UnbrokenDoubleQuotedCharacterSequenceContext *context) = 0;

    virtual std::any visitUnbrokenAccentQuotedCharacterSequence(GQLParser::UnbrokenAccentQuotedCharacterSequenceContext *context) = 0;

    virtual std::any visitSingleQuotedCharacterSequence(GQLParser::SingleQuotedCharacterSequenceContext *context) = 0;

    virtual std::any visitDoubleQuotedCharacterSequence(GQLParser::DoubleQuotedCharacterSequenceContext *context) = 0;

    virtual std::any visitAccentQuotedCharacterSequence(GQLParser::AccentQuotedCharacterSequenceContext *context) = 0;

    virtual std::any visitUnbrokenCharacterStringLiteral(GQLParser::UnbrokenCharacterStringLiteralContext *context) = 0;

    virtual std::any visitNullLiteral(GQLParser::NullLiteralContext *context) = 0;

    virtual std::any visitTemporalLiteral(GQLParser::TemporalLiteralContext *context) = 0;

    virtual std::any visitSqlDatetimeLiteral(GQLParser::SqlDatetimeLiteralContext *context) = 0;

    virtual std::any visitDateLiteral(GQLParser::DateLiteralContext *context) = 0;

    virtual std::any visitTimeLiteral(GQLParser::TimeLiteralContext *context) = 0;

    virtual std::any visitDatetimeLiteral(GQLParser::DatetimeLiteralContext *context) = 0;

    virtual std::any visitDurationLiteral(GQLParser::DurationLiteralContext *context) = 0;

    virtual std::any visitSqlIntervalLiteral(GQLParser::SqlIntervalLiteralContext *context) = 0;

    virtual std::any visitSqlIntervalType(GQLParser::SqlIntervalTypeContext *context) = 0;

    virtual std::any visitIdentifier(GQLParser::IdentifierContext *context) = 0;

    virtual std::any visitDelimitedIdentifier(GQLParser::DelimitedIdentifierContext *context) = 0;

    virtual std::any visitObjectName(GQLParser::ObjectNameContext *context) = 0;

    virtual std::any visitObjectNameOrBindingVariable(GQLParser::ObjectNameOrBindingVariableContext *context) = 0;

    virtual std::any visitDirectoryName(GQLParser::DirectoryNameContext *context) = 0;

    virtual std::any visitSchemaName(GQLParser::SchemaNameContext *context) = 0;

    virtual std::any visitGraphName(GQLParser::GraphNameContext *context) = 0;

    virtual std::any visitDelimitedGraphName(GQLParser::DelimitedGraphNameContext *context) = 0;

    virtual std::any visitGraphTypeName(GQLParser::GraphTypeNameContext *context) = 0;

    virtual std::any visitElementTypeName(GQLParser::ElementTypeNameContext *context) = 0;

    virtual std::any visitBindingTableName(GQLParser::BindingTableNameContext *context) = 0;

    virtual std::any visitDelimitedBindingTableName(GQLParser::DelimitedBindingTableNameContext *context) = 0;

    virtual std::any visitProcedureName(GQLParser::ProcedureNameContext *context) = 0;

    virtual std::any visitLabelName(GQLParser::LabelNameContext *context) = 0;

    virtual std::any visitFunctionName(GQLParser::FunctionNameContext *context) = 0;

    virtual std::any visitPropertyName(GQLParser::PropertyNameContext *context) = 0;

    virtual std::any visitFieldName(GQLParser::FieldNameContext *context) = 0;

    virtual std::any visitParameterName(GQLParser::ParameterNameContext *context) = 0;

    virtual std::any visitVariable(GQLParser::VariableContext *context) = 0;

    virtual std::any visitGraphVariable(GQLParser::GraphVariableContext *context) = 0;

    virtual std::any visitGraphPatternVariable(GQLParser::GraphPatternVariableContext *context) = 0;

    virtual std::any visitPathOrSubpathVariable(GQLParser::PathOrSubpathVariableContext *context) = 0;

    virtual std::any visitElementVariable(GQLParser::ElementVariableContext *context) = 0;

    virtual std::any visitPathVariable(GQLParser::PathVariableContext *context) = 0;

    virtual std::any visitSubpathVariable(GQLParser::SubpathVariableContext *context) = 0;

    virtual std::any visitBindingTableVariable(GQLParser::BindingTableVariableContext *context) = 0;

    virtual std::any visitValueVariable(GQLParser::ValueVariableContext *context) = 0;

    virtual std::any visitBindingVariable(GQLParser::BindingVariableContext *context) = 0;

    virtual std::any visitPredefinedTypeLiteral(GQLParser::PredefinedTypeLiteralContext *context) = 0;

    virtual std::any visitBooleanLiteral(GQLParser::BooleanLiteralContext *context) = 0;

    virtual std::any visitCharacterStringLiteral(GQLParser::CharacterStringLiteralContext *context) = 0;

    virtual std::any visitByteStringLiteral(GQLParser::ByteStringLiteralContext *context) = 0;

    virtual std::any visitProcedureBody(GQLParser::ProcedureBodyContext *context) = 0;

    virtual std::any visitBindingVariableDefinitionBlock(GQLParser::BindingVariableDefinitionBlockContext *context) = 0;

    virtual std::any visitBindingVariableDefinition(GQLParser::BindingVariableDefinitionContext *context) = 0;

    virtual std::any visitStatementBlock(GQLParser::StatementBlockContext *context) = 0;

    virtual std::any visitStatement(GQLParser::StatementContext *context) = 0;

    virtual std::any visitNextStatement(GQLParser::NextStatementContext *context) = 0;

    virtual std::any visitGraphVariableDefinition(GQLParser::GraphVariableDefinitionContext *context) = 0;

    virtual std::any visitOptTypedGraphInitializer(GQLParser::OptTypedGraphInitializerContext *context) = 0;

    virtual std::any visitGraphInitializer(GQLParser::GraphInitializerContext *context) = 0;

    virtual std::any visitBindingTableVariableDefinition(GQLParser::BindingTableVariableDefinitionContext *context) = 0;

    virtual std::any visitOptTypedBindingTableInitializer(GQLParser::OptTypedBindingTableInitializerContext *context) = 0;

    virtual std::any visitBindingTableInitializer(GQLParser::BindingTableInitializerContext *context) = 0;

    virtual std::any visitValueVariableDefinition(GQLParser::ValueVariableDefinitionContext *context) = 0;

    virtual std::any visitOptTypedValueInitializer(GQLParser::OptTypedValueInitializerContext *context) = 0;

    virtual std::any visitGraphExpression(GQLParser::GraphExpressionContext *context) = 0;

    virtual std::any visitCurrentGraph(GQLParser::CurrentGraphContext *context) = 0;

    virtual std::any visitNestedGraphQuerySpecification(GQLParser::NestedGraphQuerySpecificationContext *context) = 0;

    virtual std::any visitBindingTableExpression(GQLParser::BindingTableExpressionContext *context) = 0;

    virtual std::any visitNestedBindingTableQuerySpecification(GQLParser::NestedBindingTableQuerySpecificationContext *context) = 0;

    virtual std::any visitObjectExpressionPrimary(GQLParser::ObjectExpressionPrimaryContext *context) = 0;

    virtual std::any visitLinearCatalogModifyingStatement(GQLParser::LinearCatalogModifyingStatementContext *context) = 0;

    virtual std::any visitSimpleCatalogModifyingStatement(GQLParser::SimpleCatalogModifyingStatementContext *context) = 0;

    virtual std::any visitPrimitiveCatalogModifyingStatement(GQLParser::PrimitiveCatalogModifyingStatementContext *context) = 0;

    virtual std::any visitCreateSchemaStatement(GQLParser::CreateSchemaStatementContext *context) = 0;

    virtual std::any visitDropSchemaStatement(GQLParser::DropSchemaStatementContext *context) = 0;

    virtual std::any visitCreateGraphStatement(GQLParser::CreateGraphStatementContext *context) = 0;

    virtual std::any visitOpenGraphType(GQLParser::OpenGraphTypeContext *context) = 0;

    virtual std::any visitOfGraphType(GQLParser::OfGraphTypeContext *context) = 0;

    virtual std::any visitGraphTypeLikeGraph(GQLParser::GraphTypeLikeGraphContext *context) = 0;

    virtual std::any visitGraphSource(GQLParser::GraphSourceContext *context) = 0;

    virtual std::any visitDropGraphStatement(GQLParser::DropGraphStatementContext *context) = 0;

    virtual std::any visitCreateGraphTypeStatement(GQLParser::CreateGraphTypeStatementContext *context) = 0;

    virtual std::any visitGraphTypeSource(GQLParser::GraphTypeSourceContext *context) = 0;

    virtual std::any visitCopyOfGraphType(GQLParser::CopyOfGraphTypeContext *context) = 0;

    virtual std::any visitDropGraphTypeStatement(GQLParser::DropGraphTypeStatementContext *context) = 0;

    virtual std::any visitCallCatalogModifyingProcedureStatement(GQLParser::CallCatalogModifyingProcedureStatementContext *context) = 0;

    virtual std::any visitLinearDataModifyingStatement(GQLParser::LinearDataModifyingStatementContext *context) = 0;

    virtual std::any visitFocusedLinearDataModifyingStatement(GQLParser::FocusedLinearDataModifyingStatementContext *context) = 0;

    virtual std::any visitFocusedLinearDataModifyingStatementBody(GQLParser::FocusedLinearDataModifyingStatementBodyContext *context) = 0;

    virtual std::any visitFocusedNestedDataModifyingProcedureSpecification(GQLParser::FocusedNestedDataModifyingProcedureSpecificationContext *context) = 0;

    virtual std::any visitAmbientLinearDataModifyingStatement(GQLParser::AmbientLinearDataModifyingStatementContext *context) = 0;

    virtual std::any visitAmbientLinearDataModifyingStatementBody(GQLParser::AmbientLinearDataModifyingStatementBodyContext *context) = 0;

    virtual std::any visitSimpleLinearDataAccessingStatement(GQLParser::SimpleLinearDataAccessingStatementContext *context) = 0;

    virtual std::any visitSimpleDataAccessingStatement(GQLParser::SimpleDataAccessingStatementContext *context) = 0;

    virtual std::any visitSimpleDataModifyingStatement(GQLParser::SimpleDataModifyingStatementContext *context) = 0;

    virtual std::any visitPrimitiveDataModifyingStatement(GQLParser::PrimitiveDataModifyingStatementContext *context) = 0;

    virtual std::any visitInsertStatement(GQLParser::InsertStatementContext *context) = 0;

    virtual std::any visitSetStatement(GQLParser::SetStatementContext *context) = 0;

    virtual std::any visitSetItemList(GQLParser::SetItemListContext *context) = 0;

    virtual std::any visitSetItem(GQLParser::SetItemContext *context) = 0;

    virtual std::any visitSetPropertyItem(GQLParser::SetPropertyItemContext *context) = 0;

    virtual std::any visitSetAllPropertiesItem(GQLParser::SetAllPropertiesItemContext *context) = 0;

    virtual std::any visitSetLabelItem(GQLParser::SetLabelItemContext *context) = 0;

    virtual std::any visitLabelSetSpecification(GQLParser::LabelSetSpecificationContext *context) = 0;

    virtual std::any visitRemoveStatement(GQLParser::RemoveStatementContext *context) = 0;

    virtual std::any visitRemoveItemList(GQLParser::RemoveItemListContext *context) = 0;

    virtual std::any visitRemoveItem(GQLParser::RemoveItemContext *context) = 0;

    virtual std::any visitRemovePropertyItem(GQLParser::RemovePropertyItemContext *context) = 0;

    virtual std::any visitRemoveLabelItem(GQLParser::RemoveLabelItemContext *context) = 0;

    virtual std::any visitDeleteStatement(GQLParser::DeleteStatementContext *context) = 0;

    virtual std::any visitDeleteItemList(GQLParser::DeleteItemListContext *context) = 0;

    virtual std::any visitDeleteItem(GQLParser::DeleteItemContext *context) = 0;

    virtual std::any visitCallDataModifyingProcedureStatement(GQLParser::CallDataModifyingProcedureStatementContext *context) = 0;

    virtual std::any visitCompositeQueryStatement(GQLParser::CompositeQueryStatementContext *context) = 0;

    virtual std::any visitCompositeQueryExpression(GQLParser::CompositeQueryExpressionContext *context) = 0;

    virtual std::any visitQueryConjunction(GQLParser::QueryConjunctionContext *context) = 0;

    virtual std::any visitSetOperator(GQLParser::SetOperatorContext *context) = 0;

    virtual std::any visitCompositeQueryPrimary(GQLParser::CompositeQueryPrimaryContext *context) = 0;

    virtual std::any visitLinearQueryStatement(GQLParser::LinearQueryStatementContext *context) = 0;

    virtual std::any visitFocusedLinearQueryStatement(GQLParser::FocusedLinearQueryStatementContext *context) = 0;

    virtual std::any visitFocusedLinearQueryStatementPart(GQLParser::FocusedLinearQueryStatementPartContext *context) = 0;

    virtual std::any visitFocusedLinearQueryAndPrimitiveResultStatementPart(GQLParser::FocusedLinearQueryAndPrimitiveResultStatementPartContext *context) = 0;

    virtual std::any visitFocusedPrimitiveResultStatement(GQLParser::FocusedPrimitiveResultStatementContext *context) = 0;

    virtual std::any visitFocusedNestedQuerySpecification(GQLParser::FocusedNestedQuerySpecificationContext *context) = 0;

    virtual std::any visitAmbientLinearQueryStatement(GQLParser::AmbientLinearQueryStatementContext *context) = 0;

    virtual std::any visitSimpleLinearQueryStatement(GQLParser::SimpleLinearQueryStatementContext *context) = 0;

    virtual std::any visitSimpleQueryStatement(GQLParser::SimpleQueryStatementContext *context) = 0;

    virtual std::any visitPrimitiveQueryStatement(GQLParser::PrimitiveQueryStatementContext *context) = 0;

    virtual std::any visitMatchStatement(GQLParser::MatchStatementContext *context) = 0;

    virtual std::any visitSimpleMatchStatement(GQLParser::SimpleMatchStatementContext *context) = 0;

    virtual std::any visitOptionalMatchStatement(GQLParser::OptionalMatchStatementContext *context) = 0;

    virtual std::any visitOptionalOperand(GQLParser::OptionalOperandContext *context) = 0;

    virtual std::any visitMatchStatementBlock(GQLParser::MatchStatementBlockContext *context) = 0;

    virtual std::any visitCallQueryStatement(GQLParser::CallQueryStatementContext *context) = 0;

    virtual std::any visitFilterStatement(GQLParser::FilterStatementContext *context) = 0;

    virtual std::any visitLetStatement(GQLParser::LetStatementContext *context) = 0;

    virtual std::any visitLetVariableDefinitionList(GQLParser::LetVariableDefinitionListContext *context) = 0;

    virtual std::any visitLetVariableDefinition(GQLParser::LetVariableDefinitionContext *context) = 0;

    virtual std::any visitForStatement(GQLParser::ForStatementContext *context) = 0;

    virtual std::any visitForItem(GQLParser::ForItemContext *context) = 0;

    virtual std::any visitForItemAlias(GQLParser::ForItemAliasContext *context) = 0;

    virtual std::any visitForOrdinalityOrOffset(GQLParser::ForOrdinalityOrOffsetContext *context) = 0;

    virtual std::any visitOrderByAndPageStatement(GQLParser::OrderByAndPageStatementContext *context) = 0;

    virtual std::any visitPrimitiveResultStatement(GQLParser::PrimitiveResultStatementContext *context) = 0;

    virtual std::any visitReturnStatement(GQLParser::ReturnStatementContext *context) = 0;

    virtual std::any visitReturnStatementBody(GQLParser::ReturnStatementBodyContext *context) = 0;

    virtual std::any visitReturnItemList(GQLParser::ReturnItemListContext *context) = 0;

    virtual std::any visitReturnItem(GQLParser::ReturnItemContext *context) = 0;

    virtual std::any visitReturnItemAlias(GQLParser::ReturnItemAliasContext *context) = 0;

    virtual std::any visitSelectStatement(GQLParser::SelectStatementContext *context) = 0;

    virtual std::any visitSelectItemList(GQLParser::SelectItemListContext *context) = 0;

    virtual std::any visitSelectItem(GQLParser::SelectItemContext *context) = 0;

    virtual std::any visitSelectItemAlias(GQLParser::SelectItemAliasContext *context) = 0;

    virtual std::any visitHavingClause(GQLParser::HavingClauseContext *context) = 0;

    virtual std::any visitSelectStatementBody(GQLParser::SelectStatementBodyContext *context) = 0;

    virtual std::any visitSelectGraphMatchList(GQLParser::SelectGraphMatchListContext *context) = 0;

    virtual std::any visitSelectGraphMatch(GQLParser::SelectGraphMatchContext *context) = 0;

    virtual std::any visitSelectQuerySpecification(GQLParser::SelectQuerySpecificationContext *context) = 0;

    virtual std::any visitCallProcedureStatement(GQLParser::CallProcedureStatementContext *context) = 0;

    virtual std::any visitProcedureCall(GQLParser::ProcedureCallContext *context) = 0;

    virtual std::any visitInlineProcedureCall(GQLParser::InlineProcedureCallContext *context) = 0;

    virtual std::any visitVariableScopeClause(GQLParser::VariableScopeClauseContext *context) = 0;

    virtual std::any visitBindingVariableReferenceList(GQLParser::BindingVariableReferenceListContext *context) = 0;

    virtual std::any visitNamedProcedureCall(GQLParser::NamedProcedureCallContext *context) = 0;

    virtual std::any visitProcedureArgumentList(GQLParser::ProcedureArgumentListContext *context) = 0;

    virtual std::any visitProcedureArgument(GQLParser::ProcedureArgumentContext *context) = 0;

    virtual std::any visitUseGraphClause(GQLParser::UseGraphClauseContext *context) = 0;

    virtual std::any visitAtSchemaClause(GQLParser::AtSchemaClauseContext *context) = 0;

    virtual std::any visitBindingVariableReference(GQLParser::BindingVariableReferenceContext *context) = 0;

    virtual std::any visitElementVariableReference(GQLParser::ElementVariableReferenceContext *context) = 0;

    virtual std::any visitPathVariableReference(GQLParser::PathVariableReferenceContext *context) = 0;

    virtual std::any visitParameter(GQLParser::ParameterContext *context) = 0;

    virtual std::any visitGraphPatternBindingTable(GQLParser::GraphPatternBindingTableContext *context) = 0;

    virtual std::any visitGraphPatternYieldClause(GQLParser::GraphPatternYieldClauseContext *context) = 0;

    virtual std::any visitGraphPatternYieldItemList(GQLParser::GraphPatternYieldItemListContext *context) = 0;

    virtual std::any visitGraphPatternYieldItem(GQLParser::GraphPatternYieldItemContext *context) = 0;

    virtual std::any visitGraphPattern(GQLParser::GraphPatternContext *context) = 0;

    virtual std::any visitMatchMode(GQLParser::MatchModeContext *context) = 0;

    virtual std::any visitRepeatableElementsMatchMode(GQLParser::RepeatableElementsMatchModeContext *context) = 0;

    virtual std::any visitDifferentEdgesMatchMode(GQLParser::DifferentEdgesMatchModeContext *context) = 0;

    virtual std::any visitElementBindingsOrElements(GQLParser::ElementBindingsOrElementsContext *context) = 0;

    virtual std::any visitEdgeBindingsOrEdges(GQLParser::EdgeBindingsOrEdgesContext *context) = 0;

    virtual std::any visitPathPatternList(GQLParser::PathPatternListContext *context) = 0;

    virtual std::any visitPathPattern(GQLParser::PathPatternContext *context) = 0;

    virtual std::any visitPathVariableDeclaration(GQLParser::PathVariableDeclarationContext *context) = 0;

    virtual std::any visitKeepClause(GQLParser::KeepClauseContext *context) = 0;

    virtual std::any visitGraphPatternWhereClause(GQLParser::GraphPatternWhereClauseContext *context) = 0;

    virtual std::any visitPathPatternPrefix(GQLParser::PathPatternPrefixContext *context) = 0;

    virtual std::any visitPathModePrefix(GQLParser::PathModePrefixContext *context) = 0;

    virtual std::any visitPathMode(GQLParser::PathModeContext *context) = 0;

    virtual std::any visitPathSearchPrefix(GQLParser::PathSearchPrefixContext *context) = 0;

    virtual std::any visitAllPathSearch(GQLParser::AllPathSearchContext *context) = 0;

    virtual std::any visitPathOrPaths(GQLParser::PathOrPathsContext *context) = 0;

    virtual std::any visitAnyPathSearch(GQLParser::AnyPathSearchContext *context) = 0;

    virtual std::any visitNumberOfPaths(GQLParser::NumberOfPathsContext *context) = 0;

    virtual std::any visitShortestPathSearch(GQLParser::ShortestPathSearchContext *context) = 0;

    virtual std::any visitAllShortestPathSearch(GQLParser::AllShortestPathSearchContext *context) = 0;

    virtual std::any visitAnyShortestPathSearch(GQLParser::AnyShortestPathSearchContext *context) = 0;

    virtual std::any visitCountedShortestPathSearch(GQLParser::CountedShortestPathSearchContext *context) = 0;

    virtual std::any visitCountedShortestGroupSearch(GQLParser::CountedShortestGroupSearchContext *context) = 0;

    virtual std::any visitNumberOfGroups(GQLParser::NumberOfGroupsContext *context) = 0;

    virtual std::any visitPathPatternExpression(GQLParser::PathPatternExpressionContext *context) = 0;

    virtual std::any visitPathMultisetAlternation(GQLParser::PathMultisetAlternationContext *context) = 0;

    virtual std::any visitPathPatternUnion(GQLParser::PathPatternUnionContext *context) = 0;

    virtual std::any visitPathTerm(GQLParser::PathTermContext *context) = 0;

    virtual std::any visitPathFactor(GQLParser::PathFactorContext *context) = 0;

    virtual std::any visitQuantifiedPathPrimary(GQLParser::QuantifiedPathPrimaryContext *context) = 0;

    virtual std::any visitQuestionedPathPrimary(GQLParser::QuestionedPathPrimaryContext *context) = 0;

    virtual std::any visitPathPrimary(GQLParser::PathPrimaryContext *context) = 0;

    virtual std::any visitElementPattern(GQLParser::ElementPatternContext *context) = 0;

    virtual std::any visitNodePattern(GQLParser::NodePatternContext *context) = 0;

    virtual std::any visitElementPatternFiller(GQLParser::ElementPatternFillerContext *context) = 0;

    virtual std::any visitElementVariableDeclaration(GQLParser::ElementVariableDeclarationContext *context) = 0;

    virtual std::any visitIsLabelExpression(GQLParser::IsLabelExpressionContext *context) = 0;

    virtual std::any visitIsOrColon(GQLParser::IsOrColonContext *context) = 0;

    virtual std::any visitElementPatternPredicate(GQLParser::ElementPatternPredicateContext *context) = 0;

    virtual std::any visitElementPatternWhereClause(GQLParser::ElementPatternWhereClauseContext *context) = 0;

    virtual std::any visitElementPropertySpecification(GQLParser::ElementPropertySpecificationContext *context) = 0;

    virtual std::any visitPropertyKeyValuePairList(GQLParser::PropertyKeyValuePairListContext *context) = 0;

    virtual std::any visitPropertyKeyValuePair(GQLParser::PropertyKeyValuePairContext *context) = 0;

    virtual std::any visitEdgePattern(GQLParser::EdgePatternContext *context) = 0;

    virtual std::any visitFullEdgePattern(GQLParser::FullEdgePatternContext *context) = 0;

    virtual std::any visitFullEdgePointingLeft(GQLParser::FullEdgePointingLeftContext *context) = 0;

    virtual std::any visitFullEdgeUndirected(GQLParser::FullEdgeUndirectedContext *context) = 0;

    virtual std::any visitFullEdgePointingRight(GQLParser::FullEdgePointingRightContext *context) = 0;

    virtual std::any visitFullEdgeLeftOrUndirected(GQLParser::FullEdgeLeftOrUndirectedContext *context) = 0;

    virtual std::any visitFullEdgeUndirectedOrRight(GQLParser::FullEdgeUndirectedOrRightContext *context) = 0;

    virtual std::any visitFullEdgeLeftOrRight(GQLParser::FullEdgeLeftOrRightContext *context) = 0;

    virtual std::any visitFullEdgeAnyDirection(GQLParser::FullEdgeAnyDirectionContext *context) = 0;

    virtual std::any visitAbbreviatedEdgePointingLeft(GQLParser::AbbreviatedEdgePointingLeftContext *context) = 0;

    virtual std::any visitAbbreviatedEdgeUndirected(GQLParser::AbbreviatedEdgeUndirectedContext *context) = 0;

    virtual std::any visitAbbreviatedEdgePointingRight(GQLParser::AbbreviatedEdgePointingRightContext *context) = 0;

    virtual std::any visitAbbreviatedEdgeLeftOrUndirected(GQLParser::AbbreviatedEdgeLeftOrUndirectedContext *context) = 0;

    virtual std::any visitAbbreviatedEdgeUndirectedOrRight(GQLParser::AbbreviatedEdgeUndirectedOrRightContext *context) = 0;

    virtual std::any visitAbbreviatedEdgeLeftOrRight(GQLParser::AbbreviatedEdgeLeftOrRightContext *context) = 0;

    virtual std::any visitAbbreviatedEdgeAnyDirection(GQLParser::AbbreviatedEdgeAnyDirectionContext *context) = 0;

    virtual std::any visitParenthesizedPathPatternExpression(GQLParser::ParenthesizedPathPatternExpressionContext *context) = 0;

    virtual std::any visitSubpathVariableDeclaration(GQLParser::SubpathVariableDeclarationContext *context) = 0;

    virtual std::any visitParenthesizedPathPatternWhereClause(GQLParser::ParenthesizedPathPatternWhereClauseContext *context) = 0;

    virtual std::any visitInsertGraphPattern(GQLParser::InsertGraphPatternContext *context) = 0;

    virtual std::any visitInsertPathPatternList(GQLParser::InsertPathPatternListContext *context) = 0;

    virtual std::any visitInsertPathPattern(GQLParser::InsertPathPatternContext *context) = 0;

    virtual std::any visitInsertNodePattern(GQLParser::InsertNodePatternContext *context) = 0;

    virtual std::any visitInsertEdgePattern(GQLParser::InsertEdgePatternContext *context) = 0;

    virtual std::any visitInsertEdgePointingLeft(GQLParser::InsertEdgePointingLeftContext *context) = 0;

    virtual std::any visitInsertEdgePointingRight(GQLParser::InsertEdgePointingRightContext *context) = 0;

    virtual std::any visitInsertEdgeUndirected(GQLParser::InsertEdgeUndirectedContext *context) = 0;

    virtual std::any visitInsertElementPatternFiller(GQLParser::InsertElementPatternFillerContext *context) = 0;

    virtual std::any visitLabelAndPropertySetSpecification(GQLParser::LabelAndPropertySetSpecificationContext *context) = 0;

    virtual std::any visitLabelExpression(GQLParser::LabelExpressionContext *context) = 0;

    virtual std::any visitLabelTerm(GQLParser::LabelTermContext *context) = 0;

    virtual std::any visitLabelFactor(GQLParser::LabelFactorContext *context) = 0;

    virtual std::any visitLabelPrimary(GQLParser::LabelPrimaryContext *context) = 0;

    virtual std::any visitWildcardLabel(GQLParser::WildcardLabelContext *context) = 0;

    virtual std::any visitParenthesizedLabelExpression(GQLParser::ParenthesizedLabelExpressionContext *context) = 0;

    virtual std::any visitGraphPatternQuantifier(GQLParser::GraphPatternQuantifierContext *context) = 0;

    virtual std::any visitFixedQuantifier(GQLParser::FixedQuantifierContext *context) = 0;

    virtual std::any visitGeneralQuantifier(GQLParser::GeneralQuantifierContext *context) = 0;

    virtual std::any visitLowerBound(GQLParser::LowerBoundContext *context) = 0;

    virtual std::any visitUpperBound(GQLParser::UpperBoundContext *context) = 0;

    virtual std::any visitSimplifiedPathPatternExpression(GQLParser::SimplifiedPathPatternExpressionContext *context) = 0;

    virtual std::any visitSimplifiedDefaultingLeft(GQLParser::SimplifiedDefaultingLeftContext *context) = 0;

    virtual std::any visitSimplifiedDefaultingUndirected(GQLParser::SimplifiedDefaultingUndirectedContext *context) = 0;

    virtual std::any visitSimplifiedDefaultingRight(GQLParser::SimplifiedDefaultingRightContext *context) = 0;

    virtual std::any visitSimplifiedDefaultingLeftOrUndirected(GQLParser::SimplifiedDefaultingLeftOrUndirectedContext *context) = 0;

    virtual std::any visitSimplifiedDefaultingUndirectedOrRight(GQLParser::SimplifiedDefaultingUndirectedOrRightContext *context) = 0;

    virtual std::any visitSimplifiedDefaultingLeftOrRight(GQLParser::SimplifiedDefaultingLeftOrRightContext *context) = 0;

    virtual std::any visitSimplifiedDefaultingAnyDirection(GQLParser::SimplifiedDefaultingAnyDirectionContext *context) = 0;

    virtual std::any visitSimplifiedContents(GQLParser::SimplifiedContentsContext *context) = 0;

    virtual std::any visitSimplifiedPathUnion(GQLParser::SimplifiedPathUnionContext *context) = 0;

    virtual std::any visitSimplifiedMultisetAlternation(GQLParser::SimplifiedMultisetAlternationContext *context) = 0;

    virtual std::any visitSimplifiedTerm(GQLParser::SimplifiedTermContext *context) = 0;

    virtual std::any visitSimplifiedFactorLow(GQLParser::SimplifiedFactorLowContext *context) = 0;

    virtual std::any visitSimplifiedFactorHigh(GQLParser::SimplifiedFactorHighContext *context) = 0;

    virtual std::any visitSimplifiedQuantified(GQLParser::SimplifiedQuantifiedContext *context) = 0;

    virtual std::any visitSimplifiedQuestioned(GQLParser::SimplifiedQuestionedContext *context) = 0;

    virtual std::any visitSimplifiedTertiary(GQLParser::SimplifiedTertiaryContext *context) = 0;

    virtual std::any visitSimplifiedDirectionOverride(GQLParser::SimplifiedDirectionOverrideContext *context) = 0;

    virtual std::any visitSimplifiedOverrideLeft(GQLParser::SimplifiedOverrideLeftContext *context) = 0;

    virtual std::any visitSimplifiedOverrideUndirected(GQLParser::SimplifiedOverrideUndirectedContext *context) = 0;

    virtual std::any visitSimplifiedOverrideRight(GQLParser::SimplifiedOverrideRightContext *context) = 0;

    virtual std::any visitSimplifiedOverrideLeftOrUndirected(GQLParser::SimplifiedOverrideLeftOrUndirectedContext *context) = 0;

    virtual std::any visitSimplifiedOverrideUndirectedOrRight(GQLParser::SimplifiedOverrideUndirectedOrRightContext *context) = 0;

    virtual std::any visitSimplifiedOverrideLeftOrRight(GQLParser::SimplifiedOverrideLeftOrRightContext *context) = 0;

    virtual std::any visitSimplifiedOverrideAnyDirection(GQLParser::SimplifiedOverrideAnyDirectionContext *context) = 0;

    virtual std::any visitSimplifiedSecondary(GQLParser::SimplifiedSecondaryContext *context) = 0;

    virtual std::any visitSimplifiedNegation(GQLParser::SimplifiedNegationContext *context) = 0;

    virtual std::any visitSimplifiedPrimary(GQLParser::SimplifiedPrimaryContext *context) = 0;

    virtual std::any visitWhereClause(GQLParser::WhereClauseContext *context) = 0;

    virtual std::any visitYieldClause(GQLParser::YieldClauseContext *context) = 0;

    virtual std::any visitYieldItemList(GQLParser::YieldItemListContext *context) = 0;

    virtual std::any visitYieldItem(GQLParser::YieldItemContext *context) = 0;

    virtual std::any visitYieldItemName(GQLParser::YieldItemNameContext *context) = 0;

    virtual std::any visitYieldItemAlias(GQLParser::YieldItemAliasContext *context) = 0;

    virtual std::any visitGroupByClause(GQLParser::GroupByClauseContext *context) = 0;

    virtual std::any visitGroupingElementList(GQLParser::GroupingElementListContext *context) = 0;

    virtual std::any visitGroupingElement(GQLParser::GroupingElementContext *context) = 0;

    virtual std::any visitEmptyGroupingSet(GQLParser::EmptyGroupingSetContext *context) = 0;

    virtual std::any visitOrderByClause(GQLParser::OrderByClauseContext *context) = 0;

    virtual std::any visitGqlCountAllFunction(GQLParser::GqlCountAllFunctionContext *context) = 0;

    virtual std::any visitGqlGeneralSetFunction(GQLParser::GqlGeneralSetFunctionContext *context) = 0;

    virtual std::any visitGqlBinarySetFunction(GQLParser::GqlBinarySetFunctionContext *context) = 0;

    virtual std::any visitGeneralSetFunctionType(GQLParser::GeneralSetFunctionTypeContext *context) = 0;

    virtual std::any visitSetQuantifier(GQLParser::SetQuantifierContext *context) = 0;

    virtual std::any visitBinarySetFunctionType(GQLParser::BinarySetFunctionTypeContext *context) = 0;

    virtual std::any visitSortSpecificationList(GQLParser::SortSpecificationListContext *context) = 0;

    virtual std::any visitSortSpecification(GQLParser::SortSpecificationContext *context) = 0;

    virtual std::any visitSortKey(GQLParser::SortKeyContext *context) = 0;

    virtual std::any visitOrderingSpecification(GQLParser::OrderingSpecificationContext *context) = 0;

    virtual std::any visitNullOrdering(GQLParser::NullOrderingContext *context) = 0;

    virtual std::any visitLimitClause(GQLParser::LimitClauseContext *context) = 0;

    virtual std::any visitOffsetClause(GQLParser::OffsetClauseContext *context) = 0;

    virtual std::any visitOffsetSynonym(GQLParser::OffsetSynonymContext *context) = 0;

    virtual std::any visitNestedGraphTypeSpecification(GQLParser::NestedGraphTypeSpecificationContext *context) = 0;

    virtual std::any visitGraphTypeSpecificationBody(GQLParser::GraphTypeSpecificationBodyContext *context) = 0;

    virtual std::any visitElementTypeDefinitionList(GQLParser::ElementTypeDefinitionListContext *context) = 0;

    virtual std::any visitElementTypeDefinition(GQLParser::ElementTypeDefinitionContext *context) = 0;

    virtual std::any visitNodeTypeDefinition(GQLParser::NodeTypeDefinitionContext *context) = 0;

    virtual std::any visitNodeTypePattern(GQLParser::NodeTypePatternContext *context) = 0;

    virtual std::any visitNodeTypePhrase(GQLParser::NodeTypePhraseContext *context) = 0;

    virtual std::any visitNodeTypeName(GQLParser::NodeTypeNameContext *context) = 0;

    virtual std::any visitNodeTypeFiller(GQLParser::NodeTypeFillerContext *context) = 0;

    virtual std::any visitNodeTypeLabelSetDefinition(GQLParser::NodeTypeLabelSetDefinitionContext *context) = 0;

    virtual std::any visitNodeTypePropertyTypeSetDefinition(GQLParser::NodeTypePropertyTypeSetDefinitionContext *context) = 0;

    virtual std::any visitEdgeTypeDefinition(GQLParser::EdgeTypeDefinitionContext *context) = 0;

    virtual std::any visitEdgeTypePattern(GQLParser::EdgeTypePatternContext *context) = 0;

    virtual std::any visitEdgeTypePhrase(GQLParser::EdgeTypePhraseContext *context) = 0;

    virtual std::any visitEdgeTypeName(GQLParser::EdgeTypeNameContext *context) = 0;

    virtual std::any visitEdgeTypeFiller(GQLParser::EdgeTypeFillerContext *context) = 0;

    virtual std::any visitEdgeTypeLabelSetDefinition(GQLParser::EdgeTypeLabelSetDefinitionContext *context) = 0;

    virtual std::any visitEdgeTypePropertyTypeSetDefinition(GQLParser::EdgeTypePropertyTypeSetDefinitionContext *context) = 0;

    virtual std::any visitFullEdgeTypePattern(GQLParser::FullEdgeTypePatternContext *context) = 0;

    virtual std::any visitFullEdgeTypePatternPointingRight(GQLParser::FullEdgeTypePatternPointingRightContext *context) = 0;

    virtual std::any visitFullEdgeTypePatternPointingLeft(GQLParser::FullEdgeTypePatternPointingLeftContext *context) = 0;

    virtual std::any visitFullEdgeTypePatternUndirected(GQLParser::FullEdgeTypePatternUndirectedContext *context) = 0;

    virtual std::any visitArcTypePointingRight(GQLParser::ArcTypePointingRightContext *context) = 0;

    virtual std::any visitArcTypePointingLeft(GQLParser::ArcTypePointingLeftContext *context) = 0;

    virtual std::any visitArcTypeUndirected(GQLParser::ArcTypeUndirectedContext *context) = 0;

    virtual std::any visitArcTypeFiller(GQLParser::ArcTypeFillerContext *context) = 0;

    virtual std::any visitAbbreviatedEdgeTypePattern(GQLParser::AbbreviatedEdgeTypePatternContext *context) = 0;

    virtual std::any visitAbbreviatedEdgeTypePatternPointingRight(GQLParser::AbbreviatedEdgeTypePatternPointingRightContext *context) = 0;

    virtual std::any visitAbbreviatedEdgeTypePatternPointingLeft(GQLParser::AbbreviatedEdgeTypePatternPointingLeftContext *context) = 0;

    virtual std::any visitAbbreviatedEdgeTypePatternUndirected(GQLParser::AbbreviatedEdgeTypePatternUndirectedContext *context) = 0;

    virtual std::any visitNodeTypeReference(GQLParser::NodeTypeReferenceContext *context) = 0;

    virtual std::any visitSourceNodeTypeReference(GQLParser::SourceNodeTypeReferenceContext *context) = 0;

    virtual std::any visitDestinationNodeTypeReference(GQLParser::DestinationNodeTypeReferenceContext *context) = 0;

    virtual std::any visitEdgeKind(GQLParser::EdgeKindContext *context) = 0;

    virtual std::any visitEndpointDefinition(GQLParser::EndpointDefinitionContext *context) = 0;

    virtual std::any visitEndpointPairDefinition(GQLParser::EndpointPairDefinitionContext *context) = 0;

    virtual std::any visitEndpointPairDefinitionPointingRight(GQLParser::EndpointPairDefinitionPointingRightContext *context) = 0;

    virtual std::any visitEndpointPairDefinitionPointingLeft(GQLParser::EndpointPairDefinitionPointingLeftContext *context) = 0;

    virtual std::any visitEndpointPairDefinitionUndirected(GQLParser::EndpointPairDefinitionUndirectedContext *context) = 0;

    virtual std::any visitConnectorPointingRight(GQLParser::ConnectorPointingRightContext *context) = 0;

    virtual std::any visitConnectorUndirected(GQLParser::ConnectorUndirectedContext *context) = 0;

    virtual std::any visitSourceNodeTypeName(GQLParser::SourceNodeTypeNameContext *context) = 0;

    virtual std::any visitDestinationNodeTypeName(GQLParser::DestinationNodeTypeNameContext *context) = 0;

    virtual std::any visitLabelSetDefinition(GQLParser::LabelSetDefinitionContext *context) = 0;

    virtual std::any visitPropertyTypeSetDefinition(GQLParser::PropertyTypeSetDefinitionContext *context) = 0;

    virtual std::any visitPropertyTypeDefinitionList(GQLParser::PropertyTypeDefinitionListContext *context) = 0;

    virtual std::any visitPropertyTypeDefinition(GQLParser::PropertyTypeDefinitionContext *context) = 0;

    virtual std::any visitPropertyValueType(GQLParser::PropertyValueTypeContext *context) = 0;

    virtual std::any visitBindingTableType(GQLParser::BindingTableTypeContext *context) = 0;

    virtual std::any visitDynamicPropertyValueType(GQLParser::DynamicPropertyValueTypeContext *context) = 0;

    virtual std::any visitPredefType(GQLParser::PredefTypeContext *context) = 0;

    virtual std::any visitListType1(GQLParser::ListType1Context *context) = 0;

    virtual std::any visitRecordType1(GQLParser::RecordType1Context *context) = 0;

    virtual std::any visitClosedDynamicUnionType2(GQLParser::ClosedDynamicUnionType2Context *context) = 0;

    virtual std::any visitRecordType2(GQLParser::RecordType2Context *context) = 0;

    virtual std::any visitListType2(GQLParser::ListType2Context *context) = 0;

    virtual std::any visitPathType(GQLParser::PathTypeContext *context) = 0;

    virtual std::any visitOpenDynamicUnionType(GQLParser::OpenDynamicUnionTypeContext *context) = 0;

    virtual std::any visitClosedDynamicUnionType1(GQLParser::ClosedDynamicUnionType1Context *context) = 0;

    virtual std::any visitTyped(GQLParser::TypedContext *context) = 0;

    virtual std::any visitPredefinedType(GQLParser::PredefinedTypeContext *context) = 0;

    virtual std::any visitBooleanType(GQLParser::BooleanTypeContext *context) = 0;

    virtual std::any visitCharacterStringType(GQLParser::CharacterStringTypeContext *context) = 0;

    virtual std::any visitByteStringType(GQLParser::ByteStringTypeContext *context) = 0;

    virtual std::any visitMinLength(GQLParser::MinLengthContext *context) = 0;

    virtual std::any visitMaxLength(GQLParser::MaxLengthContext *context) = 0;

    virtual std::any visitFixedLength(GQLParser::FixedLengthContext *context) = 0;

    virtual std::any visitNumericType(GQLParser::NumericTypeContext *context) = 0;

    virtual std::any visitExactNumericType(GQLParser::ExactNumericTypeContext *context) = 0;

    virtual std::any visitBinaryExactNumericType(GQLParser::BinaryExactNumericTypeContext *context) = 0;

    virtual std::any visitSignedBinaryExactNumericType(GQLParser::SignedBinaryExactNumericTypeContext *context) = 0;

    virtual std::any visitUnsignedBinaryExactNumericType(GQLParser::UnsignedBinaryExactNumericTypeContext *context) = 0;

    virtual std::any visitVerboseBinaryExactNumericType(GQLParser::VerboseBinaryExactNumericTypeContext *context) = 0;

    virtual std::any visitDecimalExactNumericType(GQLParser::DecimalExactNumericTypeContext *context) = 0;

    virtual std::any visitPrecision(GQLParser::PrecisionContext *context) = 0;

    virtual std::any visitScale(GQLParser::ScaleContext *context) = 0;

    virtual std::any visitApproximateNumericType(GQLParser::ApproximateNumericTypeContext *context) = 0;

    virtual std::any visitTemporalType(GQLParser::TemporalTypeContext *context) = 0;

    virtual std::any visitTemporalInstantType(GQLParser::TemporalInstantTypeContext *context) = 0;

    virtual std::any visitTemporalDurationType(GQLParser::TemporalDurationTypeContext *context) = 0;

    virtual std::any visitDatetimeType(GQLParser::DatetimeTypeContext *context) = 0;

    virtual std::any visitLocaldatetimeType(GQLParser::LocaldatetimeTypeContext *context) = 0;

    virtual std::any visitDateType(GQLParser::DateTypeContext *context) = 0;

    virtual std::any visitTimeType(GQLParser::TimeTypeContext *context) = 0;

    virtual std::any visitLocaltimeType(GQLParser::LocaltimeTypeContext *context) = 0;

    virtual std::any visitDurationType(GQLParser::DurationTypeContext *context) = 0;

    virtual std::any visitReferenceValueType(GQLParser::ReferenceValueTypeContext *context) = 0;

    virtual std::any visitGraphReferenceValueType(GQLParser::GraphReferenceValueTypeContext *context) = 0;

    virtual std::any visitClosedGraphReferenceValueType(GQLParser::ClosedGraphReferenceValueTypeContext *context) = 0;

    virtual std::any visitOpenGraphReferenceValueType(GQLParser::OpenGraphReferenceValueTypeContext *context) = 0;

    virtual std::any visitBindingTableReferenceValueType(GQLParser::BindingTableReferenceValueTypeContext *context) = 0;

    virtual std::any visitNodeReferenceValueType(GQLParser::NodeReferenceValueTypeContext *context) = 0;

    virtual std::any visitClosedNodeReferenceValueType(GQLParser::ClosedNodeReferenceValueTypeContext *context) = 0;

    virtual std::any visitOpenNodeReferenceValueType(GQLParser::OpenNodeReferenceValueTypeContext *context) = 0;

    virtual std::any visitEdgeReferenceValueType(GQLParser::EdgeReferenceValueTypeContext *context) = 0;

    virtual std::any visitClosedEdgeReferenceValueType(GQLParser::ClosedEdgeReferenceValueTypeContext *context) = 0;

    virtual std::any visitOpenEdgeReferenceValueType(GQLParser::OpenEdgeReferenceValueTypeContext *context) = 0;

    virtual std::any visitListValueTypeName(GQLParser::ListValueTypeNameContext *context) = 0;

    virtual std::any visitListValueTypeNameSynonym(GQLParser::ListValueTypeNameSynonymContext *context) = 0;

    virtual std::any visitFieldTypesSpecification(GQLParser::FieldTypesSpecificationContext *context) = 0;

    virtual std::any visitFieldTypeList(GQLParser::FieldTypeListContext *context) = 0;

    virtual std::any visitPathValueType(GQLParser::PathValueTypeContext *context) = 0;

    virtual std::any visitNotNull(GQLParser::NotNullContext *context) = 0;

    virtual std::any visitFieldType(GQLParser::FieldTypeContext *context) = 0;

    virtual std::any visitSchemaReference(GQLParser::SchemaReferenceContext *context) = 0;

    virtual std::any visitAbsoluteCatalogSchemaReference(GQLParser::AbsoluteCatalogSchemaReferenceContext *context) = 0;

    virtual std::any visitCatalogSchemaParentAndName(GQLParser::CatalogSchemaParentAndNameContext *context) = 0;

    virtual std::any visitRelativeCatalogSchemaReference(GQLParser::RelativeCatalogSchemaReferenceContext *context) = 0;

    virtual std::any visitPredefinedSchemaReference(GQLParser::PredefinedSchemaReferenceContext *context) = 0;

    virtual std::any visitAbsoluteDirectoryPath(GQLParser::AbsoluteDirectoryPathContext *context) = 0;

    virtual std::any visitRelativeDirectoryPath(GQLParser::RelativeDirectoryPathContext *context) = 0;

    virtual std::any visitSimpleDirectoryPath(GQLParser::SimpleDirectoryPathContext *context) = 0;

    virtual std::any visitGraphReference(GQLParser::GraphReferenceContext *context) = 0;

    virtual std::any visitCatalogGraphParentAndName(GQLParser::CatalogGraphParentAndNameContext *context) = 0;

    virtual std::any visitHomeGraph(GQLParser::HomeGraphContext *context) = 0;

    virtual std::any visitGraphTypeReference(GQLParser::GraphTypeReferenceContext *context) = 0;

    virtual std::any visitCatalogGraphTypeParentAndName(GQLParser::CatalogGraphTypeParentAndNameContext *context) = 0;

    virtual std::any visitBindingTableReference(GQLParser::BindingTableReferenceContext *context) = 0;

    virtual std::any visitCatalogBindingTableParentAndName(GQLParser::CatalogBindingTableParentAndNameContext *context) = 0;

    virtual std::any visitProcedureReference(GQLParser::ProcedureReferenceContext *context) = 0;

    virtual std::any visitCatalogProcedureParentAndName(GQLParser::CatalogProcedureParentAndNameContext *context) = 0;

    virtual std::any visitCatalogObjectParentReference(GQLParser::CatalogObjectParentReferenceContext *context) = 0;

    virtual std::any visitReferenceParameter(GQLParser::ReferenceParameterContext *context) = 0;

    virtual std::any visitExternalObjectReference(GQLParser::ExternalObjectReferenceContext *context) = 0;

    virtual std::any visitComparisonPredicateCond(GQLParser::ComparisonPredicateCondContext *context) = 0;

    virtual std::any visitCompOp(GQLParser::CompOpContext *context) = 0;

    virtual std::any visitNullPredicateCond(GQLParser::NullPredicateCondContext *context) = 0;

    virtual std::any visitNormalizedPredicateCond(GQLParser::NormalizedPredicateCondContext *context) = 0;

    virtual std::any visitDirectedPredicateCond(GQLParser::DirectedPredicateCondContext *context) = 0;

    virtual std::any visitLabeledPredicateCond(GQLParser::LabeledPredicateCondContext *context) = 0;

    virtual std::any visitSourceDestinationPredicateCond(GQLParser::SourceDestinationPredicateCondContext *context) = 0;

    virtual std::any visitUnsignedValueSpecification(GQLParser::UnsignedValueSpecificationContext *context) = 0;

    virtual std::any visitUnsignedIntegerSpecification(GQLParser::UnsignedIntegerSpecificationContext *context) = 0;

    virtual std::any visitParameterValueSpecification(GQLParser::ParameterValueSpecificationContext *context) = 0;

    virtual std::any visitPredefinedParameter(GQLParser::PredefinedParameterContext *context) = 0;

    virtual std::any visitGqlPredicateExpression(GQLParser::GqlPredicateExpressionContext *context) = 0;

    virtual std::any visitGqlNotExpression(GQLParser::GqlNotExpressionContext *context) = 0;

    virtual std::any visitGqlLogicalXorExpression(GQLParser::GqlLogicalXorExpressionContext *context) = 0;

    virtual std::any visitGqlLogicalOrExpression(GQLParser::GqlLogicalOrExpressionContext *context) = 0;

    virtual std::any visitGqlLogicalAndExpression(GQLParser::GqlLogicalAndExpressionContext *context) = 0;

    virtual std::any visitGqlAtomExpression(GQLParser::GqlAtomExpressionContext *context) = 0;

    virtual std::any visitGqlNormalizedExpression(GQLParser::GqlNormalizedExpressionContext *context) = 0;

    virtual std::any visitGqlSameExpression(GQLParser::GqlSameExpressionContext *context) = 0;

    virtual std::any visitGqlDirectedExpression(GQLParser::GqlDirectedExpressionContext *context) = 0;

    virtual std::any visitGqlBooleanTestExpression(GQLParser::GqlBooleanTestExpressionContext *context) = 0;

    virtual std::any visitGqlGraphRefValueExpression(GQLParser::GqlGraphRefValueExpressionContext *context) = 0;

    virtual std::any visitGqlAllDifferentExpression(GQLParser::GqlAllDifferentExpressionContext *context) = 0;

    virtual std::any visitGqlExistsExpression(GQLParser::GqlExistsExpressionContext *context) = 0;

    virtual std::any visitGqlLetExpression(GQLParser::GqlLetExpressionContext *context) = 0;

    virtual std::any visitGqlComparisonExpression(GQLParser::GqlComparisonExpressionContext *context) = 0;

    virtual std::any visitGqlLabeledExpression(GQLParser::GqlLabeledExpressionContext *context) = 0;

    virtual std::any visitGqlPropertyExistExpression(GQLParser::GqlPropertyExistExpressionContext *context) = 0;

    virtual std::any visitGqlNullExpression(GQLParser::GqlNullExpressionContext *context) = 0;

    virtual std::any visitGqlSourceDestinationExpression(GQLParser::GqlSourceDestinationExpressionContext *context) = 0;

    virtual std::any visitGqlBindingTableValueExpression(GQLParser::GqlBindingTableValueExpressionContext *context) = 0;

    virtual std::any visitGqlParameterExpression(GQLParser::GqlParameterExpressionContext *context) = 0;

    virtual std::any visitGqlUnaryExpression(GQLParser::GqlUnaryExpressionContext *context) = 0;

    virtual std::any visitGqlPropertyReference(GQLParser::GqlPropertyReferenceContext *context) = 0;

    virtual std::any visitGqlHighArithmeticExpression(GQLParser::GqlHighArithmeticExpressionContext *context) = 0;

    virtual std::any visitGqlValueQueryExpression(GQLParser::GqlValueQueryExpressionContext *context) = 0;

    virtual std::any visitGqlLiteralExpression(GQLParser::GqlLiteralExpressionContext *context) = 0;

    virtual std::any visitGqlCollectionExpression(GQLParser::GqlCollectionExpressionContext *context) = 0;

    virtual std::any visitGqlConcatenationExpression(GQLParser::GqlConcatenationExpressionContext *context) = 0;

    virtual std::any visitGqlLowArithmeticExpression(GQLParser::GqlLowArithmeticExpressionContext *context) = 0;

    virtual std::any visitGqlParenthesizedExpression(GQLParser::GqlParenthesizedExpressionContext *context) = 0;

    virtual std::any visitGqlFunctionExpression(GQLParser::GqlFunctionExpressionContext *context) = 0;

    virtual std::any visitGqlVariableExpression(GQLParser::GqlVariableExpressionContext *context) = 0;

    virtual std::any visitTruthValue(GQLParser::TruthValueContext *context) = 0;

    virtual std::any visitUnaryOperator(GQLParser::UnaryOperatorContext *context) = 0;

    virtual std::any visitFunctionCall(GQLParser::FunctionCallContext *context) = 0;

    virtual std::any visitGqlOneArgScalarFunction(GQLParser::GqlOneArgScalarFunctionContext *context) = 0;

    virtual std::any visitGqlTwoArgScalarFunction(GQLParser::GqlTwoArgScalarFunctionContext *context) = 0;

    virtual std::any visitFunctionParameter(GQLParser::FunctionParameterContext *context) = 0;

    virtual std::any visitPropertyReference(GQLParser::PropertyReferenceContext *context) = 0;

    virtual std::any visitOneArgNumericFunctionName(GQLParser::OneArgNumericFunctionNameContext *context) = 0;

    virtual std::any visitTwoArgNumericFunctionName(GQLParser::TwoArgNumericFunctionNameContext *context) = 0;

    virtual std::any visitGqlSubstringFunction(GQLParser::GqlSubstringFunctionContext *context) = 0;

    virtual std::any visitGqlFoldStringFunction(GQLParser::GqlFoldStringFunctionContext *context) = 0;

    virtual std::any visitGqlSingleTrimStringFunction(GQLParser::GqlSingleTrimStringFunctionContext *context) = 0;

    virtual std::any visitGqlMultiTrimStringFunction(GQLParser::GqlMultiTrimStringFunctionContext *context) = 0;

    virtual std::any visitGqlNormStringFunction(GQLParser::GqlNormStringFunctionContext *context) = 0;

    virtual std::any visitGqlListTrimFunction(GQLParser::GqlListTrimFunctionContext *context) = 0;

    virtual std::any visitGqlElementsOfPathFunction(GQLParser::GqlElementsOfPathFunctionContext *context) = 0;

    virtual std::any visitGqlNullIfCaseFunction(GQLParser::GqlNullIfCaseFunctionContext *context) = 0;

    virtual std::any visitGqlCoalesceCaseFunction(GQLParser::GqlCoalesceCaseFunctionContext *context) = 0;

    virtual std::any visitGqlSimpleCaseFunction(GQLParser::GqlSimpleCaseFunctionContext *context) = 0;

    virtual std::any visitGqlSearchedCaseFunction(GQLParser::GqlSearchedCaseFunctionContext *context) = 0;

    virtual std::any visitSimpleWhenClause(GQLParser::SimpleWhenClauseContext *context) = 0;

    virtual std::any visitSearchedWhenClause(GQLParser::SearchedWhenClauseContext *context) = 0;

    virtual std::any visitElseClause(GQLParser::ElseClauseContext *context) = 0;

    virtual std::any visitWhenOperand(GQLParser::WhenOperandContext *context) = 0;

    virtual std::any visitCastFunction(GQLParser::CastFunctionContext *context) = 0;

    virtual std::any visitElementIdFunction(GQLParser::ElementIdFunctionContext *context) = 0;

    virtual std::any visitDatetimeValueFunction(GQLParser::DatetimeValueFunctionContext *context) = 0;

    virtual std::any visitDateFunction(GQLParser::DateFunctionContext *context) = 0;

    virtual std::any visitTimeFunction(GQLParser::TimeFunctionContext *context) = 0;

    virtual std::any visitLocalTimeFunction(GQLParser::LocalTimeFunctionContext *context) = 0;

    virtual std::any visitDatetimeFunction(GQLParser::DatetimeFunctionContext *context) = 0;

    virtual std::any visitLocalDatetimeFunction(GQLParser::LocalDatetimeFunctionContext *context) = 0;

    virtual std::any visitDateFunctionParameters(GQLParser::DateFunctionParametersContext *context) = 0;

    virtual std::any visitTimeFunctionParameters(GQLParser::TimeFunctionParametersContext *context) = 0;

    virtual std::any visitDatetimeFunctionParameters(GQLParser::DatetimeFunctionParametersContext *context) = 0;

    virtual std::any visitDateString(GQLParser::DateStringContext *context) = 0;

    virtual std::any visitTimeString(GQLParser::TimeStringContext *context) = 0;

    virtual std::any visitDatetimeString(GQLParser::DatetimeStringContext *context) = 0;

    virtual std::any visitGqlDatetimeSubtractionFunction(GQLParser::GqlDatetimeSubtractionFunctionContext *context) = 0;

    virtual std::any visitGqlDurationFunction(GQLParser::GqlDurationFunctionContext *context) = 0;

    virtual std::any visitDurationFunctionParameters(GQLParser::DurationFunctionParametersContext *context) = 0;

    virtual std::any visitDurationString(GQLParser::DurationStringContext *context) = 0;

    virtual std::any visitGeneralFunction(GQLParser::GeneralFunctionContext *context) = 0;

    virtual std::any visitCollectionValueConstructor(GQLParser::CollectionValueConstructorContext *context) = 0;

    virtual std::any visitTrimSpecification(GQLParser::TrimSpecificationContext *context) = 0;

    virtual std::any visitNormalForm(GQLParser::NormalFormContext *context) = 0;

    virtual std::any visitListValueConstructor(GQLParser::ListValueConstructorContext *context) = 0;

    virtual std::any visitRecordValueConstructor(GQLParser::RecordValueConstructorContext *context) = 0;

    virtual std::any visitField(GQLParser::FieldContext *context) = 0;

    virtual std::any visitPathValueConstructor(GQLParser::PathValueConstructorContext *context) = 0;

    virtual std::any visitUnsignedLiteral(GQLParser::UnsignedLiteralContext *context) = 0;

    virtual std::any visitGeneralLiteral(GQLParser::GeneralLiteralContext *context) = 0;

    virtual std::any visitListLiteral(GQLParser::ListLiteralContext *context) = 0;

    virtual std::any visitRecordLiteral(GQLParser::RecordLiteralContext *context) = 0;

    virtual std::any visitRecordFieldLiteral(GQLParser::RecordFieldLiteralContext *context) = 0;


};


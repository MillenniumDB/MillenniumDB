
// Generated from SparqlQueryParser.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"




class  SparqlQueryParser : public antlr4::Parser {
public:
  enum {
    WS = 1, BASE = 2, PREFIX = 3, SELECT = 4, DISTINCT = 5, REDUCED = 6, 
    CONSTRUCT = 7, DESCRIBE = 8, ASK = 9, FROM = 10, NAMED = 11, WHERE = 12, 
    ORDER = 13, BY = 14, ASC = 15, DESC = 16, LIMIT = 17, OFFSET = 18, VALUES = 19, 
    OPTIONAL = 20, GRAPH = 21, UNION = 22, FILTER = 23, A = 24, STR = 25, 
    LANG = 26, LANGMATCHES = 27, DATATYPE = 28, BOUND = 29, SAMETERM = 30, 
    ISIRI = 31, ISURI = 32, ISBLANK = 33, ISLITERAL = 34, REGEX = 35, SUBSTR = 36, 
    K_TRUE = 37, K_FALSE = 38, SILENT = 39, ALL = 40, AS = 41, GROUP = 42, 
    HAVING = 43, UNDEF = 44, BINDINGS = 45, SERVICE = 46, BIND = 47, MINUS = 48, 
    IRI = 49, URI = 50, BNODE = 51, RAND = 52, ABS = 53, CEIL = 54, FLOOR = 55, 
    ROUND = 56, CONCAT = 57, STRLEN = 58, UCASE = 59, LCASE = 60, ENCODE_FOR_URI = 61, 
    CONTAINS = 62, STRSTARTS = 63, STRENDS = 64, STRBEFORE = 65, STRAFTER = 66, 
    REPLACE = 67, YEAR = 68, MONTH = 69, DAY = 70, HOURS = 71, MINUTES = 72, 
    SECONDS = 73, TIMEZONE = 74, TZ = 75, NOW = 76, UUID = 77, STRUUID = 78, 
    MD5 = 79, SHA1 = 80, SHA256 = 81, SHA384 = 82, SHA512 = 83, COALESCE = 84, 
    IF = 85, STRLANG = 86, STRDT = 87, ISNUMERIC = 88, COUNT = 89, SUM = 90, 
    MIN = 91, MAX = 92, AVG = 93, SAMPLE = 94, GROUP_CONCAT = 95, NOT = 96, 
    IN = 97, EXISTS = 98, SEPARATOR = 99, ANY = 100, SHORTEST = 101, SIMPLE = 102, 
    ACYCLIC = 103, WALKS = 104, SHOW = 105, INDEX = 106, ALPHANUMERIC_IDENTIFIER = 107, 
    IRIREF = 108, PNAME_NS = 109, PNAME_LN = 110, BLANK_NODE_LABEL = 111, 
    VAR1 = 112, VAR2 = 113, LANGTAG = 114, INTEGER = 115, DECIMAL = 116, 
    DOUBLE = 117, INTEGER_POSITIVE = 118, DECIMAL_POSITIVE = 119, DOUBLE_POSITIVE = 120, 
    INTEGER_NEGATIVE = 121, DECIMAL_NEGATIVE = 122, DOUBLE_NEGATIVE = 123, 
    STRING_LITERAL1 = 124, STRING_LITERAL2 = 125, STRING_LITERAL_LONG1 = 126, 
    STRING_LITERAL_LONG2 = 127, COMMENT = 128, REFERENCE = 129, LESS_EQUAL = 130, 
    GREATER_EQUAL = 131, NOT_EQUAL = 132, AND = 133, OR = 134, INVERSE = 135, 
    OPEN_BRACE = 136, CLOSE_BRACE = 137, OPEN_CURLY_BRACE = 138, CLOSE_CURLY_BRACE = 139, 
    OPEN_SQUARE_BRACKET = 140, CLOSE_SQUARE_BRACKET = 141, SEMICOLON = 142, 
    DOT = 143, PLUS_SIGN = 144, MINUS_SIGN = 145, ASTERISK = 146, QUESTION_MARK = 147, 
    COMMA = 148, NEGATION = 149, DIVIDE = 150, EQUAL = 151, LESS = 152, 
    GREATER = 153, PIPE = 154, ANY_OTHER = 155
  };

  enum {
    RuleQuery = 0, RuleShowQuery = 1, RulePrologue = 2, RuleBaseDecl = 3, 
    RulePrefixDecl = 4, RuleSelectQuery = 5, RuleSubSelect = 6, RuleSelectClause = 7, 
    RuleSelectModifier = 8, RuleSelectVariables = 9, RuleConstructQuery = 10, 
    RuleDescribeQuery = 11, RuleAskQuery = 12, RuleDatasetClause = 13, RuleWhereClause = 14, 
    RuleSolutionModifier = 15, RuleGroupClause = 16, RuleGroupCondition = 17, 
    RuleHavingClause = 18, RuleHavingCondition = 19, RuleOrderClause = 20, 
    RuleOrderCondition = 21, RuleLimitOffsetClauses = 22, RuleLimitClause = 23, 
    RuleOffsetClause = 24, RuleValuesClause = 25, RuleTriplesTemplate = 26, 
    RuleGroupGraphPattern = 27, RuleGroupGraphPatternSub = 28, RuleGroupGraphPatternSubList = 29, 
    RuleTriplesBlock = 30, RuleGraphPatternNotTriples = 31, RuleOptionalGraphPattern = 32, 
    RuleGraphGraphPattern = 33, RuleServiceGraphPattern = 34, RuleBind = 35, 
    RuleInlineData = 36, RuleDataBlock = 37, RuleInlineDataOneVar = 38, 
    RuleInlineDataFull = 39, RuleDataBlockValues = 40, RuleDataBlockValue = 41, 
    RuleMinusGraphPattern = 42, RuleGroupOrUnionGraphPattern = 43, RuleFilter = 44, 
    RuleConstraint = 45, RuleFunctionCall = 46, RuleArgList = 47, RuleExpressionList = 48, 
    RuleConstructTemplate = 49, RuleConstructTriples = 50, RuleTriplesSameSubject = 51, 
    RulePropertyList = 52, RulePropertyListNotEmpty = 53, RuleVerb = 54, 
    RuleObjectList = 55, RuleObject = 56, RuleTriplesSameSubjectPath = 57, 
    RulePropertyListPath = 58, RulePropertyListPathNotEmpty = 59, RulePropertyListPathNotEmptyList = 60, 
    RuleVerbPath = 61, RuleVerbSimple = 62, RuleObjectListPath = 63, RuleObjectPath = 64, 
    RulePath = 65, RulePathAlternative = 66, RulePathSequence = 67, RulePathElt = 68, 
    RulePathEltOrInverse = 69, RulePathMod = 70, RulePathQuantity = 71, 
    RulePathQuantityExact = 72, RulePathQuantityRange = 73, RulePathQuantityMin = 74, 
    RulePathQuantityMax = 75, RulePathPrimary = 76, RulePathNegatedPropertySet = 77, 
    RulePathOneInPropertySet = 78, RuleInteger = 79, RuleTriplesNode = 80, 
    RuleBlankNodePropertyList = 81, RuleTriplesNodePath = 82, RuleBlankNodePropertyListPath = 83, 
    RuleCollection = 84, RuleCollectionPath = 85, RuleGraphNode = 86, RuleGraphNodePath = 87, 
    RuleVarOrTerm = 88, RuleVarOrIRI = 89, RuleVar = 90, RuleGraphTerm = 91, 
    RuleNil = 92, RuleExpression = 93, RuleConditionalOrExpression = 94, 
    RuleConditionalAndExpression = 95, RuleRelationalExpression = 96, RuleAdditiveExpression = 97, 
    RuleRhsAdditiveExpression = 98, RuleRhsAdditiveExpressionSub = 99, RuleMultiplicativeExpression = 100, 
    RuleUnaryExpression = 101, RulePrimaryExpression = 102, RuleBuiltInCall = 103, 
    RuleProcedure = 104, RuleProcedureArguments = 105, RuleProcedureBindings = 106, 
    RuleRegexExpression = 107, RuleSubStringExpression = 108, RuleStrReplaceExpression = 109, 
    RuleExistsFunction = 110, RuleNotExistsFunction = 111, RuleAggregate = 112, 
    RuleIriOrFunction = 113, RuleRdfLiteral = 114, RuleNumericLiteral = 115, 
    RuleNumericLiteralUnsigned = 116, RuleNumericLiteralPositive = 117, 
    RuleNumericLiteralNegative = 118, RuleBooleanLiteral = 119, RuleString = 120, 
    RuleIri = 121, RulePrefixedName = 122, RuleBlankNode = 123, RuleAnon = 124
  };

  explicit SparqlQueryParser(antlr4::TokenStream *input);

  SparqlQueryParser(antlr4::TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options);

  ~SparqlQueryParser() override;

  std::string getGrammarFileName() const override;

  const antlr4::atn::ATN& getATN() const override;

  const std::vector<std::string>& getRuleNames() const override;

  const antlr4::dfa::Vocabulary& getVocabulary() const override;

  antlr4::atn::SerializedATNView getSerializedATN() const override;


  class QueryContext;
  class ShowQueryContext;
  class PrologueContext;
  class BaseDeclContext;
  class PrefixDeclContext;
  class SelectQueryContext;
  class SubSelectContext;
  class SelectClauseContext;
  class SelectModifierContext;
  class SelectVariablesContext;
  class ConstructQueryContext;
  class DescribeQueryContext;
  class AskQueryContext;
  class DatasetClauseContext;
  class WhereClauseContext;
  class SolutionModifierContext;
  class GroupClauseContext;
  class GroupConditionContext;
  class HavingClauseContext;
  class HavingConditionContext;
  class OrderClauseContext;
  class OrderConditionContext;
  class LimitOffsetClausesContext;
  class LimitClauseContext;
  class OffsetClauseContext;
  class ValuesClauseContext;
  class TriplesTemplateContext;
  class GroupGraphPatternContext;
  class GroupGraphPatternSubContext;
  class GroupGraphPatternSubListContext;
  class TriplesBlockContext;
  class GraphPatternNotTriplesContext;
  class OptionalGraphPatternContext;
  class GraphGraphPatternContext;
  class ServiceGraphPatternContext;
  class BindContext;
  class InlineDataContext;
  class DataBlockContext;
  class InlineDataOneVarContext;
  class InlineDataFullContext;
  class DataBlockValuesContext;
  class DataBlockValueContext;
  class MinusGraphPatternContext;
  class GroupOrUnionGraphPatternContext;
  class FilterContext;
  class ConstraintContext;
  class FunctionCallContext;
  class ArgListContext;
  class ExpressionListContext;
  class ConstructTemplateContext;
  class ConstructTriplesContext;
  class TriplesSameSubjectContext;
  class PropertyListContext;
  class PropertyListNotEmptyContext;
  class VerbContext;
  class ObjectListContext;
  class ObjectContext;
  class TriplesSameSubjectPathContext;
  class PropertyListPathContext;
  class PropertyListPathNotEmptyContext;
  class PropertyListPathNotEmptyListContext;
  class VerbPathContext;
  class VerbSimpleContext;
  class ObjectListPathContext;
  class ObjectPathContext;
  class PathContext;
  class PathAlternativeContext;
  class PathSequenceContext;
  class PathEltContext;
  class PathEltOrInverseContext;
  class PathModContext;
  class PathQuantityContext;
  class PathQuantityExactContext;
  class PathQuantityRangeContext;
  class PathQuantityMinContext;
  class PathQuantityMaxContext;
  class PathPrimaryContext;
  class PathNegatedPropertySetContext;
  class PathOneInPropertySetContext;
  class IntegerContext;
  class TriplesNodeContext;
  class BlankNodePropertyListContext;
  class TriplesNodePathContext;
  class BlankNodePropertyListPathContext;
  class CollectionContext;
  class CollectionPathContext;
  class GraphNodeContext;
  class GraphNodePathContext;
  class VarOrTermContext;
  class VarOrIRIContext;
  class VarContext;
  class GraphTermContext;
  class NilContext;
  class ExpressionContext;
  class ConditionalOrExpressionContext;
  class ConditionalAndExpressionContext;
  class RelationalExpressionContext;
  class AdditiveExpressionContext;
  class RhsAdditiveExpressionContext;
  class RhsAdditiveExpressionSubContext;
  class MultiplicativeExpressionContext;
  class UnaryExpressionContext;
  class PrimaryExpressionContext;
  class BuiltInCallContext;
  class ProcedureContext;
  class ProcedureArgumentsContext;
  class ProcedureBindingsContext;
  class RegexExpressionContext;
  class SubStringExpressionContext;
  class StrReplaceExpressionContext;
  class ExistsFunctionContext;
  class NotExistsFunctionContext;
  class AggregateContext;
  class IriOrFunctionContext;
  class RdfLiteralContext;
  class NumericLiteralContext;
  class NumericLiteralUnsignedContext;
  class NumericLiteralPositiveContext;
  class NumericLiteralNegativeContext;
  class BooleanLiteralContext;
  class StringContext;
  class IriContext;
  class PrefixedNameContext;
  class BlankNodeContext;
  class AnonContext; 

  class  QueryContext : public antlr4::ParserRuleContext {
  public:
    QueryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    PrologueContext *prologue();
    ValuesClauseContext *valuesClause();
    antlr4::tree::TerminalNode *EOF();
    SelectQueryContext *selectQuery();
    ConstructQueryContext *constructQuery();
    DescribeQueryContext *describeQuery();
    AskQueryContext *askQuery();
    ShowQueryContext *showQuery();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  QueryContext* query();

  class  ShowQueryContext : public antlr4::ParserRuleContext {
  public:
    ShowQueryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *SHOW();
    antlr4::tree::TerminalNode *ALPHANUMERIC_IDENTIFIER();
    antlr4::tree::TerminalNode *INDEX();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ShowQueryContext* showQuery();

  class  PrologueContext : public antlr4::ParserRuleContext {
  public:
    PrologueContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<BaseDeclContext *> baseDecl();
    BaseDeclContext* baseDecl(size_t i);
    std::vector<PrefixDeclContext *> prefixDecl();
    PrefixDeclContext* prefixDecl(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PrologueContext* prologue();

  class  BaseDeclContext : public antlr4::ParserRuleContext {
  public:
    BaseDeclContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *BASE();
    antlr4::tree::TerminalNode *IRIREF();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  BaseDeclContext* baseDecl();

  class  PrefixDeclContext : public antlr4::ParserRuleContext {
  public:
    PrefixDeclContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *PREFIX();
    antlr4::tree::TerminalNode *PNAME_NS();
    antlr4::tree::TerminalNode *IRIREF();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PrefixDeclContext* prefixDecl();

  class  SelectQueryContext : public antlr4::ParserRuleContext {
  public:
    SelectQueryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    SelectClauseContext *selectClause();
    WhereClauseContext *whereClause();
    SolutionModifierContext *solutionModifier();
    std::vector<DatasetClauseContext *> datasetClause();
    DatasetClauseContext* datasetClause(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SelectQueryContext* selectQuery();

  class  SubSelectContext : public antlr4::ParserRuleContext {
  public:
    SubSelectContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    SelectClauseContext *selectClause();
    WhereClauseContext *whereClause();
    SolutionModifierContext *solutionModifier();
    ValuesClauseContext *valuesClause();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SubSelectContext* subSelect();

  class  SelectClauseContext : public antlr4::ParserRuleContext {
  public:
    SelectClauseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *SELECT();
    antlr4::tree::TerminalNode *ASTERISK();
    SelectModifierContext *selectModifier();
    std::vector<SelectVariablesContext *> selectVariables();
    SelectVariablesContext* selectVariables(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SelectClauseContext* selectClause();

  class  SelectModifierContext : public antlr4::ParserRuleContext {
  public:
    SelectModifierContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DISTINCT();
    antlr4::tree::TerminalNode *REDUCED();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SelectModifierContext* selectModifier();

  class  SelectVariablesContext : public antlr4::ParserRuleContext {
  public:
    SelectVariablesContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    SelectVariablesContext() = default;
    void copyFrom(SelectVariablesContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  SelectSingleVariableContext : public SelectVariablesContext {
  public:
    SelectSingleVariableContext(SelectVariablesContext *ctx);

    VarContext *var();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  SelectExpressionAsVariableContext : public SelectVariablesContext {
  public:
    SelectExpressionAsVariableContext(SelectVariablesContext *ctx);

    antlr4::tree::TerminalNode *OPEN_BRACE();
    ExpressionContext *expression();
    antlr4::tree::TerminalNode *AS();
    VarContext *var();
    antlr4::tree::TerminalNode *CLOSE_BRACE();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  SelectVariablesContext* selectVariables();

  class  ConstructQueryContext : public antlr4::ParserRuleContext {
  public:
    ConstructQueryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *CONSTRUCT();
    ConstructTemplateContext *constructTemplate();
    WhereClauseContext *whereClause();
    SolutionModifierContext *solutionModifier();
    antlr4::tree::TerminalNode *WHERE();
    antlr4::tree::TerminalNode *OPEN_CURLY_BRACE();
    antlr4::tree::TerminalNode *CLOSE_CURLY_BRACE();
    std::vector<DatasetClauseContext *> datasetClause();
    DatasetClauseContext* datasetClause(size_t i);
    TriplesTemplateContext *triplesTemplate();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ConstructQueryContext* constructQuery();

  class  DescribeQueryContext : public antlr4::ParserRuleContext {
  public:
    DescribeQueryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DESCRIBE();
    SolutionModifierContext *solutionModifier();
    antlr4::tree::TerminalNode *ASTERISK();
    std::vector<DatasetClauseContext *> datasetClause();
    DatasetClauseContext* datasetClause(size_t i);
    WhereClauseContext *whereClause();
    std::vector<VarOrIRIContext *> varOrIRI();
    VarOrIRIContext* varOrIRI(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DescribeQueryContext* describeQuery();

  class  AskQueryContext : public antlr4::ParserRuleContext {
  public:
    AskQueryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ASK();
    WhereClauseContext *whereClause();
    SolutionModifierContext *solutionModifier();
    std::vector<DatasetClauseContext *> datasetClause();
    DatasetClauseContext* datasetClause(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AskQueryContext* askQuery();

  class  DatasetClauseContext : public antlr4::ParserRuleContext {
  public:
    DatasetClauseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *FROM();
    IriContext *iri();
    antlr4::tree::TerminalNode *NAMED();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DatasetClauseContext* datasetClause();

  class  WhereClauseContext : public antlr4::ParserRuleContext {
  public:
    WhereClauseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    GroupGraphPatternContext *groupGraphPattern();
    antlr4::tree::TerminalNode *WHERE();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  WhereClauseContext* whereClause();

  class  SolutionModifierContext : public antlr4::ParserRuleContext {
  public:
    SolutionModifierContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    GroupClauseContext *groupClause();
    HavingClauseContext *havingClause();
    OrderClauseContext *orderClause();
    LimitOffsetClausesContext *limitOffsetClauses();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SolutionModifierContext* solutionModifier();

  class  GroupClauseContext : public antlr4::ParserRuleContext {
  public:
    GroupClauseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *GROUP();
    antlr4::tree::TerminalNode *BY();
    std::vector<GroupConditionContext *> groupCondition();
    GroupConditionContext* groupCondition(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  GroupClauseContext* groupClause();

  class  GroupConditionContext : public antlr4::ParserRuleContext {
  public:
    GroupConditionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    BuiltInCallContext *builtInCall();
    FunctionCallContext *functionCall();
    antlr4::tree::TerminalNode *OPEN_BRACE();
    ExpressionContext *expression();
    antlr4::tree::TerminalNode *CLOSE_BRACE();
    antlr4::tree::TerminalNode *AS();
    VarContext *var();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  GroupConditionContext* groupCondition();

  class  HavingClauseContext : public antlr4::ParserRuleContext {
  public:
    HavingClauseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *HAVING();
    std::vector<HavingConditionContext *> havingCondition();
    HavingConditionContext* havingCondition(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  HavingClauseContext* havingClause();

  class  HavingConditionContext : public antlr4::ParserRuleContext {
  public:
    HavingConditionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ConstraintContext *constraint();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  HavingConditionContext* havingCondition();

  class  OrderClauseContext : public antlr4::ParserRuleContext {
  public:
    OrderClauseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ORDER();
    antlr4::tree::TerminalNode *BY();
    std::vector<OrderConditionContext *> orderCondition();
    OrderConditionContext* orderCondition(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  OrderClauseContext* orderClause();

  class  OrderConditionContext : public antlr4::ParserRuleContext {
  public:
    OrderConditionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *OPEN_BRACE();
    ExpressionContext *expression();
    antlr4::tree::TerminalNode *CLOSE_BRACE();
    antlr4::tree::TerminalNode *ASC();
    antlr4::tree::TerminalNode *DESC();
    ConstraintContext *constraint();
    VarContext *var();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  OrderConditionContext* orderCondition();

  class  LimitOffsetClausesContext : public antlr4::ParserRuleContext {
  public:
    LimitOffsetClausesContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    LimitClauseContext *limitClause();
    OffsetClauseContext *offsetClause();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  LimitOffsetClausesContext* limitOffsetClauses();

  class  LimitClauseContext : public antlr4::ParserRuleContext {
  public:
    LimitClauseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LIMIT();
    antlr4::tree::TerminalNode *INTEGER();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  LimitClauseContext* limitClause();

  class  OffsetClauseContext : public antlr4::ParserRuleContext {
  public:
    OffsetClauseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *OFFSET();
    antlr4::tree::TerminalNode *INTEGER();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  OffsetClauseContext* offsetClause();

  class  ValuesClauseContext : public antlr4::ParserRuleContext {
  public:
    ValuesClauseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *VALUES();
    DataBlockContext *dataBlock();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ValuesClauseContext* valuesClause();

  class  TriplesTemplateContext : public antlr4::ParserRuleContext {
  public:
    TriplesTemplateContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<TriplesSameSubjectContext *> triplesSameSubject();
    TriplesSameSubjectContext* triplesSameSubject(size_t i);
    std::vector<antlr4::tree::TerminalNode *> DOT();
    antlr4::tree::TerminalNode* DOT(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TriplesTemplateContext* triplesTemplate();

  class  GroupGraphPatternContext : public antlr4::ParserRuleContext {
  public:
    GroupGraphPatternContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *OPEN_CURLY_BRACE();
    antlr4::tree::TerminalNode *CLOSE_CURLY_BRACE();
    SubSelectContext *subSelect();
    GroupGraphPatternSubContext *groupGraphPatternSub();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  GroupGraphPatternContext* groupGraphPattern();

  class  GroupGraphPatternSubContext : public antlr4::ParserRuleContext {
  public:
    GroupGraphPatternSubContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    TriplesBlockContext *triplesBlock();
    std::vector<GroupGraphPatternSubListContext *> groupGraphPatternSubList();
    GroupGraphPatternSubListContext* groupGraphPatternSubList(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  GroupGraphPatternSubContext* groupGraphPatternSub();

  class  GroupGraphPatternSubListContext : public antlr4::ParserRuleContext {
  public:
    GroupGraphPatternSubListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    GraphPatternNotTriplesContext *graphPatternNotTriples();
    antlr4::tree::TerminalNode *DOT();
    TriplesBlockContext *triplesBlock();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  GroupGraphPatternSubListContext* groupGraphPatternSubList();

  class  TriplesBlockContext : public antlr4::ParserRuleContext {
  public:
    TriplesBlockContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<TriplesSameSubjectPathContext *> triplesSameSubjectPath();
    TriplesSameSubjectPathContext* triplesSameSubjectPath(size_t i);
    std::vector<antlr4::tree::TerminalNode *> DOT();
    antlr4::tree::TerminalNode* DOT(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TriplesBlockContext* triplesBlock();

  class  GraphPatternNotTriplesContext : public antlr4::ParserRuleContext {
  public:
    GraphPatternNotTriplesContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    GroupOrUnionGraphPatternContext *groupOrUnionGraphPattern();
    OptionalGraphPatternContext *optionalGraphPattern();
    MinusGraphPatternContext *minusGraphPattern();
    GraphGraphPatternContext *graphGraphPattern();
    ServiceGraphPatternContext *serviceGraphPattern();
    FilterContext *filter();
    BindContext *bind();
    InlineDataContext *inlineData();
    ProcedureContext *procedure();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  GraphPatternNotTriplesContext* graphPatternNotTriples();

  class  OptionalGraphPatternContext : public antlr4::ParserRuleContext {
  public:
    OptionalGraphPatternContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *OPTIONAL();
    GroupGraphPatternContext *groupGraphPattern();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  OptionalGraphPatternContext* optionalGraphPattern();

  class  GraphGraphPatternContext : public antlr4::ParserRuleContext {
  public:
    GraphGraphPatternContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *GRAPH();
    VarOrIRIContext *varOrIRI();
    GroupGraphPatternContext *groupGraphPattern();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  GraphGraphPatternContext* graphGraphPattern();

  class  ServiceGraphPatternContext : public antlr4::ParserRuleContext {
  public:
    ServiceGraphPatternContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *SERVICE();
    VarOrIRIContext *varOrIRI();
    GroupGraphPatternContext *groupGraphPattern();
    antlr4::tree::TerminalNode *SILENT();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ServiceGraphPatternContext* serviceGraphPattern();

  class  BindContext : public antlr4::ParserRuleContext {
  public:
    BindContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *BIND();
    antlr4::tree::TerminalNode *OPEN_BRACE();
    ExpressionContext *expression();
    antlr4::tree::TerminalNode *AS();
    VarContext *var();
    antlr4::tree::TerminalNode *CLOSE_BRACE();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  BindContext* bind();

  class  InlineDataContext : public antlr4::ParserRuleContext {
  public:
    InlineDataContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *VALUES();
    DataBlockContext *dataBlock();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  InlineDataContext* inlineData();

  class  DataBlockContext : public antlr4::ParserRuleContext {
  public:
    DataBlockContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    InlineDataOneVarContext *inlineDataOneVar();
    InlineDataFullContext *inlineDataFull();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DataBlockContext* dataBlock();

  class  InlineDataOneVarContext : public antlr4::ParserRuleContext {
  public:
    InlineDataOneVarContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    VarContext *var();
    antlr4::tree::TerminalNode *OPEN_CURLY_BRACE();
    antlr4::tree::TerminalNode *CLOSE_CURLY_BRACE();
    std::vector<DataBlockValueContext *> dataBlockValue();
    DataBlockValueContext* dataBlockValue(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  InlineDataOneVarContext* inlineDataOneVar();

  class  InlineDataFullContext : public antlr4::ParserRuleContext {
  public:
    InlineDataFullContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *OPEN_BRACE();
    antlr4::tree::TerminalNode *CLOSE_BRACE();
    antlr4::tree::TerminalNode *OPEN_CURLY_BRACE();
    antlr4::tree::TerminalNode *CLOSE_CURLY_BRACE();
    std::vector<VarContext *> var();
    VarContext* var(size_t i);
    std::vector<DataBlockValuesContext *> dataBlockValues();
    DataBlockValuesContext* dataBlockValues(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  InlineDataFullContext* inlineDataFull();

  class  DataBlockValuesContext : public antlr4::ParserRuleContext {
  public:
    DataBlockValuesContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *OPEN_BRACE();
    antlr4::tree::TerminalNode *CLOSE_BRACE();
    std::vector<DataBlockValueContext *> dataBlockValue();
    DataBlockValueContext* dataBlockValue(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DataBlockValuesContext* dataBlockValues();

  class  DataBlockValueContext : public antlr4::ParserRuleContext {
  public:
    DataBlockValueContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IriContext *iri();
    RdfLiteralContext *rdfLiteral();
    NumericLiteralContext *numericLiteral();
    BooleanLiteralContext *booleanLiteral();
    antlr4::tree::TerminalNode *UNDEF();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DataBlockValueContext* dataBlockValue();

  class  MinusGraphPatternContext : public antlr4::ParserRuleContext {
  public:
    MinusGraphPatternContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *MINUS();
    GroupGraphPatternContext *groupGraphPattern();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  MinusGraphPatternContext* minusGraphPattern();

  class  GroupOrUnionGraphPatternContext : public antlr4::ParserRuleContext {
  public:
    GroupOrUnionGraphPatternContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<GroupGraphPatternContext *> groupGraphPattern();
    GroupGraphPatternContext* groupGraphPattern(size_t i);
    std::vector<antlr4::tree::TerminalNode *> UNION();
    antlr4::tree::TerminalNode* UNION(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  GroupOrUnionGraphPatternContext* groupOrUnionGraphPattern();

  class  FilterContext : public antlr4::ParserRuleContext {
  public:
    FilterContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *FILTER();
    ConstraintContext *constraint();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FilterContext* filter();

  class  ConstraintContext : public antlr4::ParserRuleContext {
  public:
    ConstraintContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *OPEN_BRACE();
    ExpressionContext *expression();
    antlr4::tree::TerminalNode *CLOSE_BRACE();
    BuiltInCallContext *builtInCall();
    FunctionCallContext *functionCall();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ConstraintContext* constraint();

  class  FunctionCallContext : public antlr4::ParserRuleContext {
  public:
    FunctionCallContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IriContext *iri();
    ArgListContext *argList();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FunctionCallContext* functionCall();

  class  ArgListContext : public antlr4::ParserRuleContext {
  public:
    ArgListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *OPEN_BRACE();
    antlr4::tree::TerminalNode *CLOSE_BRACE();
    ExpressionListContext *expressionList();
    antlr4::tree::TerminalNode *DISTINCT();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ArgListContext* argList();

  class  ExpressionListContext : public antlr4::ParserRuleContext {
  public:
    ExpressionListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ExpressionContext *> expression();
    ExpressionContext* expression(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ExpressionListContext* expressionList();

  class  ConstructTemplateContext : public antlr4::ParserRuleContext {
  public:
    ConstructTemplateContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *OPEN_CURLY_BRACE();
    antlr4::tree::TerminalNode *CLOSE_CURLY_BRACE();
    ConstructTriplesContext *constructTriples();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ConstructTemplateContext* constructTemplate();

  class  ConstructTriplesContext : public antlr4::ParserRuleContext {
  public:
    ConstructTriplesContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    TriplesSameSubjectContext *triplesSameSubject();
    std::vector<antlr4::tree::TerminalNode *> DOT();
    antlr4::tree::TerminalNode* DOT(size_t i);
    std::vector<ConstructTriplesContext *> constructTriples();
    ConstructTriplesContext* constructTriples(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ConstructTriplesContext* constructTriples();

  class  TriplesSameSubjectContext : public antlr4::ParserRuleContext {
  public:
    TriplesSameSubjectContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    VarOrTermContext *varOrTerm();
    PropertyListNotEmptyContext *propertyListNotEmpty();
    TriplesNodeContext *triplesNode();
    PropertyListContext *propertyList();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TriplesSameSubjectContext* triplesSameSubject();

  class  PropertyListContext : public antlr4::ParserRuleContext {
  public:
    PropertyListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    PropertyListNotEmptyContext *propertyListNotEmpty();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PropertyListContext* propertyList();

  class  PropertyListNotEmptyContext : public antlr4::ParserRuleContext {
  public:
    PropertyListNotEmptyContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<VerbContext *> verb();
    VerbContext* verb(size_t i);
    std::vector<ObjectListContext *> objectList();
    ObjectListContext* objectList(size_t i);
    std::vector<antlr4::tree::TerminalNode *> SEMICOLON();
    antlr4::tree::TerminalNode* SEMICOLON(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PropertyListNotEmptyContext* propertyListNotEmpty();

  class  VerbContext : public antlr4::ParserRuleContext {
  public:
    VerbContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    VarOrIRIContext *varOrIRI();
    antlr4::tree::TerminalNode *A();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  VerbContext* verb();

  class  ObjectListContext : public antlr4::ParserRuleContext {
  public:
    ObjectListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ObjectContext *> object();
    ObjectContext* object(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ObjectListContext* objectList();

  class  ObjectContext : public antlr4::ParserRuleContext {
  public:
    ObjectContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    GraphNodeContext *graphNode();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ObjectContext* object();

  class  TriplesSameSubjectPathContext : public antlr4::ParserRuleContext {
  public:
    TriplesSameSubjectPathContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    VarOrTermContext *varOrTerm();
    PropertyListPathNotEmptyContext *propertyListPathNotEmpty();
    TriplesNodePathContext *triplesNodePath();
    PropertyListPathContext *propertyListPath();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TriplesSameSubjectPathContext* triplesSameSubjectPath();

  class  PropertyListPathContext : public antlr4::ParserRuleContext {
  public:
    PropertyListPathContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    PropertyListPathNotEmptyContext *propertyListPathNotEmpty();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PropertyListPathContext* propertyListPath();

  class  PropertyListPathNotEmptyContext : public antlr4::ParserRuleContext {
  public:
    PropertyListPathNotEmptyContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ObjectListPathContext *objectListPath();
    VerbPathContext *verbPath();
    VerbSimpleContext *verbSimple();
    std::vector<antlr4::tree::TerminalNode *> SEMICOLON();
    antlr4::tree::TerminalNode* SEMICOLON(size_t i);
    std::vector<PropertyListPathNotEmptyListContext *> propertyListPathNotEmptyList();
    PropertyListPathNotEmptyListContext* propertyListPathNotEmptyList(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PropertyListPathNotEmptyContext* propertyListPathNotEmpty();

  class  PropertyListPathNotEmptyListContext : public antlr4::ParserRuleContext {
  public:
    PropertyListPathNotEmptyListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ObjectListContext *objectList();
    VerbPathContext *verbPath();
    VerbSimpleContext *verbSimple();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PropertyListPathNotEmptyListContext* propertyListPathNotEmptyList();

  class  VerbPathContext : public antlr4::ParserRuleContext {
  public:
    VerbPathContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    PathContext *path();
    antlr4::tree::TerminalNode *OPEN_BRACE();
    antlr4::tree::TerminalNode *AS();
    VarContext *var();
    antlr4::tree::TerminalNode *CLOSE_BRACE();
    antlr4::tree::TerminalNode *ANY();
    antlr4::tree::TerminalNode *ALL();
    antlr4::tree::TerminalNode *SHORTEST();
    antlr4::tree::TerminalNode *WALKS();
    antlr4::tree::TerminalNode *SIMPLE();
    antlr4::tree::TerminalNode *ACYCLIC();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  VerbPathContext* verbPath();

  class  VerbSimpleContext : public antlr4::ParserRuleContext {
  public:
    VerbSimpleContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    VarContext *var();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  VerbSimpleContext* verbSimple();

  class  ObjectListPathContext : public antlr4::ParserRuleContext {
  public:
    ObjectListPathContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ObjectPathContext *> objectPath();
    ObjectPathContext* objectPath(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ObjectListPathContext* objectListPath();

  class  ObjectPathContext : public antlr4::ParserRuleContext {
  public:
    ObjectPathContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    GraphNodePathContext *graphNodePath();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ObjectPathContext* objectPath();

  class  PathContext : public antlr4::ParserRuleContext {
  public:
    PathContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    PathAlternativeContext *pathAlternative();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PathContext* path();

  class  PathAlternativeContext : public antlr4::ParserRuleContext {
  public:
    PathAlternativeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<PathSequenceContext *> pathSequence();
    PathSequenceContext* pathSequence(size_t i);
    std::vector<antlr4::tree::TerminalNode *> PIPE();
    antlr4::tree::TerminalNode* PIPE(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PathAlternativeContext* pathAlternative();

  class  PathSequenceContext : public antlr4::ParserRuleContext {
  public:
    PathSequenceContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<PathEltOrInverseContext *> pathEltOrInverse();
    PathEltOrInverseContext* pathEltOrInverse(size_t i);
    std::vector<antlr4::tree::TerminalNode *> DIVIDE();
    antlr4::tree::TerminalNode* DIVIDE(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PathSequenceContext* pathSequence();

  class  PathEltContext : public antlr4::ParserRuleContext {
  public:
    PathEltContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    PathPrimaryContext *pathPrimary();
    PathModContext *pathMod();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PathEltContext* pathElt();

  class  PathEltOrInverseContext : public antlr4::ParserRuleContext {
  public:
    PathEltOrInverseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    PathEltContext *pathElt();
    antlr4::tree::TerminalNode *INVERSE();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PathEltOrInverseContext* pathEltOrInverse();

  class  PathModContext : public antlr4::ParserRuleContext {
  public:
    PathModContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *QUESTION_MARK();
    antlr4::tree::TerminalNode *ASTERISK();
    antlr4::tree::TerminalNode *PLUS_SIGN();
    antlr4::tree::TerminalNode *OPEN_CURLY_BRACE();
    PathQuantityContext *pathQuantity();
    antlr4::tree::TerminalNode *CLOSE_CURLY_BRACE();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PathModContext* pathMod();

  class  PathQuantityContext : public antlr4::ParserRuleContext {
  public:
    PathQuantityContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    PathQuantityExactContext *pathQuantityExact();
    PathQuantityRangeContext *pathQuantityRange();
    PathQuantityMinContext *pathQuantityMin();
    PathQuantityMaxContext *pathQuantityMax();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PathQuantityContext* pathQuantity();

  class  PathQuantityExactContext : public antlr4::ParserRuleContext {
  public:
    PathQuantityExactContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *INTEGER();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PathQuantityExactContext* pathQuantityExact();

  class  PathQuantityRangeContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *min = nullptr;
    antlr4::Token *max = nullptr;
    PathQuantityRangeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *COMMA();
    std::vector<antlr4::tree::TerminalNode *> INTEGER();
    antlr4::tree::TerminalNode* INTEGER(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PathQuantityRangeContext* pathQuantityRange();

  class  PathQuantityMinContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *min = nullptr;
    PathQuantityMinContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *COMMA();
    antlr4::tree::TerminalNode *INTEGER();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PathQuantityMinContext* pathQuantityMin();

  class  PathQuantityMaxContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *max = nullptr;
    PathQuantityMaxContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *COMMA();
    antlr4::tree::TerminalNode *INTEGER();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PathQuantityMaxContext* pathQuantityMax();

  class  PathPrimaryContext : public antlr4::ParserRuleContext {
  public:
    PathPrimaryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IriContext *iri();
    antlr4::tree::TerminalNode *A();
    antlr4::tree::TerminalNode *NEGATION();
    PathNegatedPropertySetContext *pathNegatedPropertySet();
    antlr4::tree::TerminalNode *OPEN_BRACE();
    PathContext *path();
    antlr4::tree::TerminalNode *CLOSE_BRACE();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PathPrimaryContext* pathPrimary();

  class  PathNegatedPropertySetContext : public antlr4::ParserRuleContext {
  public:
    PathNegatedPropertySetContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<PathOneInPropertySetContext *> pathOneInPropertySet();
    PathOneInPropertySetContext* pathOneInPropertySet(size_t i);
    antlr4::tree::TerminalNode *OPEN_BRACE();
    antlr4::tree::TerminalNode *CLOSE_BRACE();
    std::vector<antlr4::tree::TerminalNode *> PIPE();
    antlr4::tree::TerminalNode* PIPE(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PathNegatedPropertySetContext* pathNegatedPropertySet();

  class  PathOneInPropertySetContext : public antlr4::ParserRuleContext {
  public:
    PathOneInPropertySetContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IriContext *iri();
    antlr4::tree::TerminalNode *A();
    antlr4::tree::TerminalNode *INVERSE();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PathOneInPropertySetContext* pathOneInPropertySet();

  class  IntegerContext : public antlr4::ParserRuleContext {
  public:
    IntegerContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *INTEGER();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  IntegerContext* integer();

  class  TriplesNodeContext : public antlr4::ParserRuleContext {
  public:
    TriplesNodeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    CollectionContext *collection();
    BlankNodePropertyListContext *blankNodePropertyList();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TriplesNodeContext* triplesNode();

  class  BlankNodePropertyListContext : public antlr4::ParserRuleContext {
  public:
    BlankNodePropertyListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *OPEN_SQUARE_BRACKET();
    PropertyListNotEmptyContext *propertyListNotEmpty();
    antlr4::tree::TerminalNode *CLOSE_SQUARE_BRACKET();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  BlankNodePropertyListContext* blankNodePropertyList();

  class  TriplesNodePathContext : public antlr4::ParserRuleContext {
  public:
    TriplesNodePathContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    CollectionPathContext *collectionPath();
    BlankNodePropertyListPathContext *blankNodePropertyListPath();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TriplesNodePathContext* triplesNodePath();

  class  BlankNodePropertyListPathContext : public antlr4::ParserRuleContext {
  public:
    BlankNodePropertyListPathContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *OPEN_SQUARE_BRACKET();
    PropertyListPathNotEmptyContext *propertyListPathNotEmpty();
    antlr4::tree::TerminalNode *CLOSE_SQUARE_BRACKET();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  BlankNodePropertyListPathContext* blankNodePropertyListPath();

  class  CollectionContext : public antlr4::ParserRuleContext {
  public:
    CollectionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *OPEN_BRACE();
    antlr4::tree::TerminalNode *CLOSE_BRACE();
    std::vector<GraphNodeContext *> graphNode();
    GraphNodeContext* graphNode(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CollectionContext* collection();

  class  CollectionPathContext : public antlr4::ParserRuleContext {
  public:
    CollectionPathContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *OPEN_BRACE();
    antlr4::tree::TerminalNode *CLOSE_BRACE();
    std::vector<GraphNodePathContext *> graphNodePath();
    GraphNodePathContext* graphNodePath(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CollectionPathContext* collectionPath();

  class  GraphNodeContext : public antlr4::ParserRuleContext {
  public:
    GraphNodeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    VarOrTermContext *varOrTerm();
    TriplesNodeContext *triplesNode();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  GraphNodeContext* graphNode();

  class  GraphNodePathContext : public antlr4::ParserRuleContext {
  public:
    GraphNodePathContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    VarOrTermContext *varOrTerm();
    TriplesNodePathContext *triplesNodePath();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  GraphNodePathContext* graphNodePath();

  class  VarOrTermContext : public antlr4::ParserRuleContext {
  public:
    VarOrTermContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    VarContext *var();
    GraphTermContext *graphTerm();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  VarOrTermContext* varOrTerm();

  class  VarOrIRIContext : public antlr4::ParserRuleContext {
  public:
    VarOrIRIContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    VarContext *var();
    IriContext *iri();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  VarOrIRIContext* varOrIRI();

  class  VarContext : public antlr4::ParserRuleContext {
  public:
    VarContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *VAR1();
    antlr4::tree::TerminalNode *VAR2();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  VarContext* var();

  class  GraphTermContext : public antlr4::ParserRuleContext {
  public:
    GraphTermContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IriContext *iri();
    RdfLiteralContext *rdfLiteral();
    NumericLiteralContext *numericLiteral();
    BooleanLiteralContext *booleanLiteral();
    BlankNodeContext *blankNode();
    NilContext *nil();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  GraphTermContext* graphTerm();

  class  NilContext : public antlr4::ParserRuleContext {
  public:
    NilContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *OPEN_BRACE();
    antlr4::tree::TerminalNode *CLOSE_BRACE();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  NilContext* nil();

  class  ExpressionContext : public antlr4::ParserRuleContext {
  public:
    ExpressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ConditionalOrExpressionContext *conditionalOrExpression();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ExpressionContext* expression();

  class  ConditionalOrExpressionContext : public antlr4::ParserRuleContext {
  public:
    ConditionalOrExpressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ConditionalAndExpressionContext *> conditionalAndExpression();
    ConditionalAndExpressionContext* conditionalAndExpression(size_t i);
    std::vector<antlr4::tree::TerminalNode *> OR();
    antlr4::tree::TerminalNode* OR(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ConditionalOrExpressionContext* conditionalOrExpression();

  class  ConditionalAndExpressionContext : public antlr4::ParserRuleContext {
  public:
    ConditionalAndExpressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<RelationalExpressionContext *> relationalExpression();
    RelationalExpressionContext* relationalExpression(size_t i);
    std::vector<antlr4::tree::TerminalNode *> AND();
    antlr4::tree::TerminalNode* AND(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ConditionalAndExpressionContext* conditionalAndExpression();

  class  RelationalExpressionContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *op = nullptr;
    RelationalExpressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<AdditiveExpressionContext *> additiveExpression();
    AdditiveExpressionContext* additiveExpression(size_t i);
    antlr4::tree::TerminalNode *EQUAL();
    antlr4::tree::TerminalNode *NOT_EQUAL();
    antlr4::tree::TerminalNode *LESS();
    antlr4::tree::TerminalNode *GREATER();
    antlr4::tree::TerminalNode *LESS_EQUAL();
    antlr4::tree::TerminalNode *GREATER_EQUAL();
    antlr4::tree::TerminalNode *IN();
    antlr4::tree::TerminalNode *OPEN_BRACE();
    antlr4::tree::TerminalNode *CLOSE_BRACE();
    antlr4::tree::TerminalNode *NOT();
    ExpressionListContext *expressionList();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  RelationalExpressionContext* relationalExpression();

  class  AdditiveExpressionContext : public antlr4::ParserRuleContext {
  public:
    AdditiveExpressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    MultiplicativeExpressionContext *multiplicativeExpression();
    std::vector<RhsAdditiveExpressionContext *> rhsAdditiveExpression();
    RhsAdditiveExpressionContext* rhsAdditiveExpression(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AdditiveExpressionContext* additiveExpression();

  class  RhsAdditiveExpressionContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *s146 = nullptr;
    std::vector<antlr4::Token *> op;
    antlr4::Token *s150 = nullptr;
    antlr4::Token *_tset1723 = nullptr;
    RhsAdditiveExpressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    RhsAdditiveExpressionSubContext *rhsAdditiveExpressionSub();
    std::vector<UnaryExpressionContext *> unaryExpression();
    UnaryExpressionContext* unaryExpression(size_t i);
    std::vector<antlr4::tree::TerminalNode *> ASTERISK();
    antlr4::tree::TerminalNode* ASTERISK(size_t i);
    std::vector<antlr4::tree::TerminalNode *> DIVIDE();
    antlr4::tree::TerminalNode* DIVIDE(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  RhsAdditiveExpressionContext* rhsAdditiveExpression();

  class  RhsAdditiveExpressionSubContext : public antlr4::ParserRuleContext {
  public:
    RhsAdditiveExpressionSubContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    MultiplicativeExpressionContext *multiplicativeExpression();
    antlr4::tree::TerminalNode *PLUS_SIGN();
    antlr4::tree::TerminalNode *MINUS_SIGN();
    NumericLiteralPositiveContext *numericLiteralPositive();
    NumericLiteralNegativeContext *numericLiteralNegative();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  RhsAdditiveExpressionSubContext* rhsAdditiveExpressionSub();

  class  MultiplicativeExpressionContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *s146 = nullptr;
    std::vector<antlr4::Token *> op;
    antlr4::Token *s150 = nullptr;
    antlr4::Token *_tset1766 = nullptr;
    MultiplicativeExpressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<UnaryExpressionContext *> unaryExpression();
    UnaryExpressionContext* unaryExpression(size_t i);
    std::vector<antlr4::tree::TerminalNode *> ASTERISK();
    antlr4::tree::TerminalNode* ASTERISK(size_t i);
    std::vector<antlr4::tree::TerminalNode *> DIVIDE();
    antlr4::tree::TerminalNode* DIVIDE(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  MultiplicativeExpressionContext* multiplicativeExpression();

  class  UnaryExpressionContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *op = nullptr;
    UnaryExpressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    PrimaryExpressionContext *primaryExpression();
    antlr4::tree::TerminalNode *NEGATION();
    antlr4::tree::TerminalNode *PLUS_SIGN();
    antlr4::tree::TerminalNode *MINUS_SIGN();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  UnaryExpressionContext* unaryExpression();

  class  PrimaryExpressionContext : public antlr4::ParserRuleContext {
  public:
    PrimaryExpressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *OPEN_BRACE();
    ExpressionContext *expression();
    antlr4::tree::TerminalNode *CLOSE_BRACE();
    BuiltInCallContext *builtInCall();
    IriOrFunctionContext *iriOrFunction();
    RdfLiteralContext *rdfLiteral();
    NumericLiteralContext *numericLiteral();
    BooleanLiteralContext *booleanLiteral();
    VarContext *var();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PrimaryExpressionContext* primaryExpression();

  class  BuiltInCallContext : public antlr4::ParserRuleContext {
  public:
    BuiltInCallContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    AggregateContext *aggregate();
    antlr4::tree::TerminalNode *STR();
    antlr4::tree::TerminalNode *OPEN_BRACE();
    std::vector<ExpressionContext *> expression();
    ExpressionContext* expression(size_t i);
    antlr4::tree::TerminalNode *CLOSE_BRACE();
    antlr4::tree::TerminalNode *LANG();
    antlr4::tree::TerminalNode *LANGMATCHES();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);
    antlr4::tree::TerminalNode *DATATYPE();
    antlr4::tree::TerminalNode *BOUND();
    VarContext *var();
    antlr4::tree::TerminalNode *IRI();
    antlr4::tree::TerminalNode *URI();
    antlr4::tree::TerminalNode *BNODE();
    antlr4::tree::TerminalNode *RAND();
    antlr4::tree::TerminalNode *ABS();
    antlr4::tree::TerminalNode *CEIL();
    antlr4::tree::TerminalNode *FLOOR();
    antlr4::tree::TerminalNode *ROUND();
    antlr4::tree::TerminalNode *CONCAT();
    ExpressionListContext *expressionList();
    SubStringExpressionContext *subStringExpression();
    antlr4::tree::TerminalNode *STRLEN();
    StrReplaceExpressionContext *strReplaceExpression();
    antlr4::tree::TerminalNode *UCASE();
    antlr4::tree::TerminalNode *LCASE();
    antlr4::tree::TerminalNode *ENCODE_FOR_URI();
    antlr4::tree::TerminalNode *CONTAINS();
    antlr4::tree::TerminalNode *STRSTARTS();
    antlr4::tree::TerminalNode *STRENDS();
    antlr4::tree::TerminalNode *STRBEFORE();
    antlr4::tree::TerminalNode *STRAFTER();
    antlr4::tree::TerminalNode *YEAR();
    antlr4::tree::TerminalNode *MONTH();
    antlr4::tree::TerminalNode *DAY();
    antlr4::tree::TerminalNode *HOURS();
    antlr4::tree::TerminalNode *MINUTES();
    antlr4::tree::TerminalNode *SECONDS();
    antlr4::tree::TerminalNode *TIMEZONE();
    antlr4::tree::TerminalNode *TZ();
    antlr4::tree::TerminalNode *NOW();
    antlr4::tree::TerminalNode *UUID();
    antlr4::tree::TerminalNode *STRUUID();
    antlr4::tree::TerminalNode *MD5();
    antlr4::tree::TerminalNode *SHA1();
    antlr4::tree::TerminalNode *SHA256();
    antlr4::tree::TerminalNode *SHA384();
    antlr4::tree::TerminalNode *SHA512();
    antlr4::tree::TerminalNode *COALESCE();
    antlr4::tree::TerminalNode *IF();
    antlr4::tree::TerminalNode *STRLANG();
    antlr4::tree::TerminalNode *STRDT();
    antlr4::tree::TerminalNode *SAMETERM();
    antlr4::tree::TerminalNode *ISIRI();
    antlr4::tree::TerminalNode *ISURI();
    antlr4::tree::TerminalNode *ISBLANK();
    antlr4::tree::TerminalNode *ISLITERAL();
    antlr4::tree::TerminalNode *ISNUMERIC();
    RegexExpressionContext *regexExpression();
    ExistsFunctionContext *existsFunction();
    NotExistsFunctionContext *notExistsFunction();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  BuiltInCallContext* builtInCall();

  class  ProcedureContext : public antlr4::ParserRuleContext {
  public:
    ProcedureContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ALPHANUMERIC_IDENTIFIER();
    antlr4::tree::TerminalNode *OPEN_BRACE();
    antlr4::tree::TerminalNode *CLOSE_BRACE();
    antlr4::tree::TerminalNode *AS();
    ProcedureBindingsContext *procedureBindings();
    ProcedureArgumentsContext *procedureArguments();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ProcedureContext* procedure();

  class  ProcedureArgumentsContext : public antlr4::ParserRuleContext {
  public:
    ProcedureArgumentsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ExpressionContext *> expression();
    ExpressionContext* expression(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ProcedureArgumentsContext* procedureArguments();

  class  ProcedureBindingsContext : public antlr4::ParserRuleContext {
  public:
    ProcedureBindingsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<VarContext *> var();
    VarContext* var(size_t i);
    antlr4::tree::TerminalNode *OPEN_BRACE();
    antlr4::tree::TerminalNode *CLOSE_BRACE();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ProcedureBindingsContext* procedureBindings();

  class  RegexExpressionContext : public antlr4::ParserRuleContext {
  public:
    RegexExpressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *REGEX();
    antlr4::tree::TerminalNode *OPEN_BRACE();
    std::vector<ExpressionContext *> expression();
    ExpressionContext* expression(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);
    antlr4::tree::TerminalNode *CLOSE_BRACE();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  RegexExpressionContext* regexExpression();

  class  SubStringExpressionContext : public antlr4::ParserRuleContext {
  public:
    SubStringExpressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *SUBSTR();
    antlr4::tree::TerminalNode *OPEN_BRACE();
    std::vector<ExpressionContext *> expression();
    ExpressionContext* expression(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);
    antlr4::tree::TerminalNode *CLOSE_BRACE();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SubStringExpressionContext* subStringExpression();

  class  StrReplaceExpressionContext : public antlr4::ParserRuleContext {
  public:
    StrReplaceExpressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *REPLACE();
    antlr4::tree::TerminalNode *OPEN_BRACE();
    std::vector<ExpressionContext *> expression();
    ExpressionContext* expression(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);
    antlr4::tree::TerminalNode *CLOSE_BRACE();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  StrReplaceExpressionContext* strReplaceExpression();

  class  ExistsFunctionContext : public antlr4::ParserRuleContext {
  public:
    ExistsFunctionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *EXISTS();
    GroupGraphPatternContext *groupGraphPattern();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ExistsFunctionContext* existsFunction();

  class  NotExistsFunctionContext : public antlr4::ParserRuleContext {
  public:
    NotExistsFunctionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *NOT();
    antlr4::tree::TerminalNode *EXISTS();
    GroupGraphPatternContext *groupGraphPattern();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  NotExistsFunctionContext* notExistsFunction();

  class  AggregateContext : public antlr4::ParserRuleContext {
  public:
    AggregateContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *COUNT();
    antlr4::tree::TerminalNode *OPEN_BRACE();
    antlr4::tree::TerminalNode *CLOSE_BRACE();
    antlr4::tree::TerminalNode *ASTERISK();
    ExpressionContext *expression();
    antlr4::tree::TerminalNode *DISTINCT();
    antlr4::tree::TerminalNode *SUM();
    antlr4::tree::TerminalNode *MIN();
    antlr4::tree::TerminalNode *MAX();
    antlr4::tree::TerminalNode *AVG();
    antlr4::tree::TerminalNode *SAMPLE();
    antlr4::tree::TerminalNode *GROUP_CONCAT();
    antlr4::tree::TerminalNode *SEMICOLON();
    antlr4::tree::TerminalNode *SEPARATOR();
    antlr4::tree::TerminalNode *EQUAL();
    StringContext *string();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AggregateContext* aggregate();

  class  IriOrFunctionContext : public antlr4::ParserRuleContext {
  public:
    IriOrFunctionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IriContext *iri();
    ArgListContext *argList();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  IriOrFunctionContext* iriOrFunction();

  class  RdfLiteralContext : public antlr4::ParserRuleContext {
  public:
    RdfLiteralContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    StringContext *string();
    antlr4::tree::TerminalNode *LANGTAG();
    antlr4::tree::TerminalNode *REFERENCE();
    IriContext *iri();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  RdfLiteralContext* rdfLiteral();

  class  NumericLiteralContext : public antlr4::ParserRuleContext {
  public:
    NumericLiteralContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    NumericLiteralUnsignedContext *numericLiteralUnsigned();
    NumericLiteralPositiveContext *numericLiteralPositive();
    NumericLiteralNegativeContext *numericLiteralNegative();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  NumericLiteralContext* numericLiteral();

  class  NumericLiteralUnsignedContext : public antlr4::ParserRuleContext {
  public:
    NumericLiteralUnsignedContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *INTEGER();
    antlr4::tree::TerminalNode *DECIMAL();
    antlr4::tree::TerminalNode *DOUBLE();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  NumericLiteralUnsignedContext* numericLiteralUnsigned();

  class  NumericLiteralPositiveContext : public antlr4::ParserRuleContext {
  public:
    NumericLiteralPositiveContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *INTEGER_POSITIVE();
    antlr4::tree::TerminalNode *DECIMAL_POSITIVE();
    antlr4::tree::TerminalNode *DOUBLE_POSITIVE();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  NumericLiteralPositiveContext* numericLiteralPositive();

  class  NumericLiteralNegativeContext : public antlr4::ParserRuleContext {
  public:
    NumericLiteralNegativeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *INTEGER_NEGATIVE();
    antlr4::tree::TerminalNode *DECIMAL_NEGATIVE();
    antlr4::tree::TerminalNode *DOUBLE_NEGATIVE();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  NumericLiteralNegativeContext* numericLiteralNegative();

  class  BooleanLiteralContext : public antlr4::ParserRuleContext {
  public:
    BooleanLiteralContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *K_TRUE();
    antlr4::tree::TerminalNode *K_FALSE();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  BooleanLiteralContext* booleanLiteral();

  class  StringContext : public antlr4::ParserRuleContext {
  public:
    StringContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *STRING_LITERAL1();
    antlr4::tree::TerminalNode *STRING_LITERAL2();
    antlr4::tree::TerminalNode *STRING_LITERAL_LONG1();
    antlr4::tree::TerminalNode *STRING_LITERAL_LONG2();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  StringContext* string();

  class  IriContext : public antlr4::ParserRuleContext {
  public:
    IriContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *IRIREF();
    PrefixedNameContext *prefixedName();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  IriContext* iri();

  class  PrefixedNameContext : public antlr4::ParserRuleContext {
  public:
    PrefixedNameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *PNAME_LN();
    antlr4::tree::TerminalNode *PNAME_NS();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PrefixedNameContext* prefixedName();

  class  BlankNodeContext : public antlr4::ParserRuleContext {
  public:
    BlankNodeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *BLANK_NODE_LABEL();
    AnonContext *anon();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  BlankNodeContext* blankNode();

  class  AnonContext : public antlr4::ParserRuleContext {
  public:
    AnonContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *OPEN_SQUARE_BRACKET();
    antlr4::tree::TerminalNode *CLOSE_SQUARE_BRACKET();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AnonContext* anon();


  // By default the static state used to implement the parser is lazily initialized during the first
  // call to the constructor. You can call this function if you wish to initialize the static state
  // ahead of time.
  static void initialize();

private:
};


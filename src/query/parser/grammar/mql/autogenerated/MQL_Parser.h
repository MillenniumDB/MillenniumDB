
// Generated from MQL_Parser.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"




class  MQL_Parser : public antlr4::Parser {
public:
  enum {
    K_ACYCLIC = 1, K_AND = 2, K_ANY = 3, K_AS = 4, K_AVG = 5, K_ALL = 6, 
    K_ASC = 7, K_BY = 8, K_BOOL = 9, K_CALL = 10, K_COSINE_DISTANCE = 11, 
    K_COSINE_SIMILARITY = 12, K_COUNT = 13, K_CREATE = 14, K_DELETE = 15, 
    K_DESCRIBE = 16, K_DESC = 17, K_DIMENSIONS = 18, K_DISTINCT = 19, K_EDGE = 20, 
    K_EUCLIDEAN_DISTANCE = 21, K_FROM = 22, K_INCOMING = 23, K_INDEX = 24, 
    K_INSERT = 25, K_INTEGER = 26, K_INTO = 27, K_IS = 28, K_FALSE = 29, 
    K_FLOAT = 30, K_GROUP = 31, K_GROUPS = 32, K_IDENTITY = 33, K_LABELS = 34, 
    K_LABEL = 35, K_LET = 36, K_LIMIT = 37, K_MANHATTAN_DISTANCE = 38, K_MATCH = 39, 
    K_MAX = 40, K_MIN = 41, K_OBJECTS = 42, K_OFFSET = 43, K_OPTIONAL = 44, 
    K_ORDER = 45, K_OR = 46, K_OUTGOING = 47, K_PREFIX = 48, K_PROPERTIES = 49, 
    K_PROPERTY = 50, K_NOT = 51, K_NULL = 52, K_SHORTEST = 53, K_SHOW = 54, 
    K_SIMPLE = 55, K_TEXT_SEARCH = 56, K_REGEX = 57, K_RETURN = 58, K_SUM = 59, 
    K_STRING = 60, K_TRUE = 61, K_TRAILS = 62, K_VALUES = 63, K_WALKS = 64, 
    K_WITH = 65, K_WHERE = 66, K_YIELD = 67, TRUE_PROP = 68, FALSE_PROP = 69, 
    ANON_ID = 70, EDGE_ID = 71, KEY = 72, TYPE = 73, TYPE_VAR = 74, VARIABLE = 75, 
    STRING = 76, UNSIGNED_INTEGER = 77, UNSIGNED_FLOAT = 78, UNSIGNED_SCIENTIFIC_NOTATION = 79, 
    NAME = 80, LEQ = 81, GEQ = 82, EQ = 83, NEQ = 84, LT = 85, GT = 86, 
    SINGLE_EQ = 87, PATH_SEQUENCE = 88, PATH_ALTERNATIVE = 89, PATH_NEGATION = 90, 
    STAR = 91, PERCENT = 92, QUESTION_MARK = 93, PLUS = 94, MINUS = 95, 
    L_PAR = 96, R_PAR = 97, LCURLY_BRACKET = 98, RCURLY_BRACKET = 99, LSQUARE_BRACKET = 100, 
    RSQUARE_BRACKET = 101, COMMA = 102, COLON = 103, WHITE_SPACE = 104, 
    SINGLE_LINE_COMMENT = 105, UNRECOGNIZED = 106
  };

  enum {
    RuleRoot = 0, RuleMatchQuery = 1, RulePrimitiveStatementList = 2, RulePrimitiveStatement = 3, 
    RuleInsertPatterns = 4, RuleInsertLinearPattern = 5, RuleInsertPlainNode = 6, 
    RuleInsertPlainNodeInside = 7, RuleInsertPlainEdge = 8, RuleCreateIndexQuery = 9, 
    RuleCreateIndexOptions = 10, RuleCreateIndexOption = 11, RuleShowQuery = 12, 
    RuleDescribeQuery = 13, RuleDescribeFlag = 14, RuleMatchStatement = 15, 
    RuleLetStatement = 16, RuleLetDefinitionList = 17, RuleLetDefinition = 18, 
    RuleWhereStatement = 19, RuleGroupByStatement = 20, RuleOrderByStatement = 21, 
    RuleReturnStatement = 22, RuleCallStatement = 23, RuleYieldStatement = 24, 
    RuleYieldItem = 25, RuleCallArguments = 26, RuleLimitOffsetClauses = 27, 
    RuleLimitClause = 28, RuleOffsetClause = 29, RuleReturnItem = 30, RuleAlias = 31, 
    RuleAggregateFunc = 32, RuleOrderByItem = 33, RuleGroupByItem = 34, 
    RuleGraphPattern = 35, RuleOptionalPattern = 36, RuleBasicPattern = 37, 
    RuleLinearPattern = 38, RulePath = 39, RulePathAlternatives = 40, RulePathSequence = 41, 
    RulePathAtom = 42, RulePathSuffix = 43, RulePathType = 44, RuleNode = 45, 
    RuleFixedNode = 46, RuleFixedNodeInside = 47, RuleEdge = 48, RuleEdgeInside = 49, 
    RuleVarNode = 50, RuleProperties = 51, RuleProperty = 52, RuleConditionalOrType = 53, 
    RuleIdentifier = 54, RuleBoolValue = 55, RuleNumericValue = 56, RuleDatatypeValue = 57, 
    RuleValue = 58, RuleConditionalOrExpr = 59, RuleConditionalAndExpr = 60, 
    RuleComparisonExpr = 61, RuleAdditiveExpr = 62, RuleMultiplicativeExpr = 63, 
    RuleUnaryExpr = 64, RuleAtomicExpr = 65, RuleFunction = 66, RuleRegex = 67, 
    RuleTextSearch = 68, RuleCosineSimilarity = 69, RuleCosineDistance = 70, 
    RuleManhattanDistance = 71, RuleEuclideanDistance = 72, RuleTextSearchIndexMode = 73, 
    RuleExprTypename = 74, RuleKeyword = 75
  };

  explicit MQL_Parser(antlr4::TokenStream *input);

  MQL_Parser(antlr4::TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options);

  ~MQL_Parser() override;

  std::string getGrammarFileName() const override;

  const antlr4::atn::ATN& getATN() const override;

  const std::vector<std::string>& getRuleNames() const override;

  const antlr4::dfa::Vocabulary& getVocabulary() const override;

  antlr4::atn::SerializedATNView getSerializedATN() const override;


  class RootContext;
  class MatchQueryContext;
  class PrimitiveStatementListContext;
  class PrimitiveStatementContext;
  class InsertPatternsContext;
  class InsertLinearPatternContext;
  class InsertPlainNodeContext;
  class InsertPlainNodeInsideContext;
  class InsertPlainEdgeContext;
  class CreateIndexQueryContext;
  class CreateIndexOptionsContext;
  class CreateIndexOptionContext;
  class ShowQueryContext;
  class DescribeQueryContext;
  class DescribeFlagContext;
  class MatchStatementContext;
  class LetStatementContext;
  class LetDefinitionListContext;
  class LetDefinitionContext;
  class WhereStatementContext;
  class GroupByStatementContext;
  class OrderByStatementContext;
  class ReturnStatementContext;
  class CallStatementContext;
  class YieldStatementContext;
  class YieldItemContext;
  class CallArgumentsContext;
  class LimitOffsetClausesContext;
  class LimitClauseContext;
  class OffsetClauseContext;
  class ReturnItemContext;
  class AliasContext;
  class AggregateFuncContext;
  class OrderByItemContext;
  class GroupByItemContext;
  class GraphPatternContext;
  class OptionalPatternContext;
  class BasicPatternContext;
  class LinearPatternContext;
  class PathContext;
  class PathAlternativesContext;
  class PathSequenceContext;
  class PathAtomContext;
  class PathSuffixContext;
  class PathTypeContext;
  class NodeContext;
  class FixedNodeContext;
  class FixedNodeInsideContext;
  class EdgeContext;
  class EdgeInsideContext;
  class VarNodeContext;
  class PropertiesContext;
  class PropertyContext;
  class ConditionalOrTypeContext;
  class IdentifierContext;
  class BoolValueContext;
  class NumericValueContext;
  class DatatypeValueContext;
  class ValueContext;
  class ConditionalOrExprContext;
  class ConditionalAndExprContext;
  class ComparisonExprContext;
  class AdditiveExprContext;
  class MultiplicativeExprContext;
  class UnaryExprContext;
  class AtomicExprContext;
  class FunctionContext;
  class RegexContext;
  class TextSearchContext;
  class CosineSimilarityContext;
  class CosineDistanceContext;
  class ManhattanDistanceContext;
  class EuclideanDistanceContext;
  class TextSearchIndexModeContext;
  class ExprTypenameContext;
  class KeywordContext; 

  class  RootContext : public antlr4::ParserRuleContext {
  public:
    RootContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *EOF();
    MatchQueryContext *matchQuery();
    DescribeQueryContext *describeQuery();
    InsertPatternsContext *insertPatterns();
    CreateIndexQueryContext *createIndexQuery();
    ShowQueryContext *showQuery();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  RootContext* root();

  class  MatchQueryContext : public antlr4::ParserRuleContext {
  public:
    MatchQueryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    PrimitiveStatementListContext *primitiveStatementList();
    ReturnStatementContext *returnStatement();
    WhereStatementContext *whereStatement();
    GroupByStatementContext *groupByStatement();
    OrderByStatementContext *orderByStatement();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  MatchQueryContext* matchQuery();

  class  PrimitiveStatementListContext : public antlr4::ParserRuleContext {
  public:
    PrimitiveStatementListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<PrimitiveStatementContext *> primitiveStatement();
    PrimitiveStatementContext* primitiveStatement(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PrimitiveStatementListContext* primitiveStatementList();

  class  PrimitiveStatementContext : public antlr4::ParserRuleContext {
  public:
    PrimitiveStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    MatchStatementContext *matchStatement();
    CallStatementContext *callStatement();
    LetStatementContext *letStatement();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PrimitiveStatementContext* primitiveStatement();

  class  InsertPatternsContext : public antlr4::ParserRuleContext {
  public:
    InsertPatternsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *K_INSERT();
    std::vector<InsertLinearPatternContext *> insertLinearPattern();
    InsertLinearPatternContext* insertLinearPattern(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  InsertPatternsContext* insertPatterns();

  class  InsertLinearPatternContext : public antlr4::ParserRuleContext {
  public:
    InsertLinearPatternContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<InsertPlainNodeContext *> insertPlainNode();
    InsertPlainNodeContext* insertPlainNode(size_t i);
    std::vector<InsertPlainEdgeContext *> insertPlainEdge();
    InsertPlainEdgeContext* insertPlainEdge(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  InsertLinearPatternContext* insertLinearPattern();

  class  InsertPlainNodeContext : public antlr4::ParserRuleContext {
  public:
    InsertPlainNodeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *L_PAR();
    antlr4::tree::TerminalNode *R_PAR();
    InsertPlainNodeInsideContext *insertPlainNodeInside();
    std::vector<antlr4::tree::TerminalNode *> TYPE();
    antlr4::tree::TerminalNode* TYPE(size_t i);
    PropertiesContext *properties();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  InsertPlainNodeContext* insertPlainNode();

  class  InsertPlainNodeInsideContext : public antlr4::ParserRuleContext {
  public:
    InsertPlainNodeInsideContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdentifierContext *identifier();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  InsertPlainNodeInsideContext* insertPlainNodeInside();

  class  InsertPlainEdgeContext : public antlr4::ParserRuleContext {
  public:
    InsertPlainEdgeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LT();
    std::vector<antlr4::tree::TerminalNode *> MINUS();
    antlr4::tree::TerminalNode* MINUS(size_t i);
    antlr4::tree::TerminalNode *LSQUARE_BRACKET();
    antlr4::tree::TerminalNode *TYPE();
    antlr4::tree::TerminalNode *RSQUARE_BRACKET();
    PropertiesContext *properties();
    antlr4::tree::TerminalNode *GT();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  InsertPlainEdgeContext* insertPlainEdge();

  class  CreateIndexQueryContext : public antlr4::ParserRuleContext {
  public:
    CreateIndexQueryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *K_CREATE();
    IdentifierContext *identifier();
    antlr4::tree::TerminalNode *K_INDEX();
    antlr4::tree::TerminalNode *STRING();
    antlr4::tree::TerminalNode *K_WITH();
    CreateIndexOptionsContext *createIndexOptions();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CreateIndexQueryContext* createIndexQuery();

  class  CreateIndexOptionsContext : public antlr4::ParserRuleContext {
  public:
    CreateIndexOptionsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LCURLY_BRACKET();
    antlr4::tree::TerminalNode *RCURLY_BRACKET();
    std::vector<CreateIndexOptionContext *> createIndexOption();
    CreateIndexOptionContext* createIndexOption(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CreateIndexOptionsContext* createIndexOptions();

  class  CreateIndexOptionContext : public antlr4::ParserRuleContext {
  public:
    CreateIndexOptionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *STRING();
    antlr4::tree::TerminalNode *SINGLE_EQ();
    ValueContext *value();
    BoolValueContext *boolValue();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CreateIndexOptionContext* createIndexOption();

  class  ShowQueryContext : public antlr4::ParserRuleContext {
  public:
    ShowQueryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *K_SHOW();
    IdentifierContext *identifier();
    antlr4::tree::TerminalNode *K_INDEX();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ShowQueryContext* showQuery();

  class  DescribeQueryContext : public antlr4::ParserRuleContext {
  public:
    DescribeQueryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *K_DESCRIBE();
    FixedNodeInsideContext *fixedNodeInside();
    std::vector<DescribeFlagContext *> describeFlag();
    DescribeFlagContext* describeFlag(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DescribeQueryContext* describeQuery();

  class  DescribeFlagContext : public antlr4::ParserRuleContext {
  public:
    DescribeFlagContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *K_LABELS();
    antlr4::tree::TerminalNode *K_PROPERTIES();
    antlr4::tree::TerminalNode *K_OUTGOING();
    antlr4::tree::TerminalNode *K_INCOMING();
    antlr4::tree::TerminalNode *K_LIMIT();
    antlr4::tree::TerminalNode *UNSIGNED_INTEGER();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DescribeFlagContext* describeFlag();

  class  MatchStatementContext : public antlr4::ParserRuleContext {
  public:
    MatchStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *K_MATCH();
    GraphPatternContext *graphPattern();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  MatchStatementContext* matchStatement();

  class  LetStatementContext : public antlr4::ParserRuleContext {
  public:
    LetStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *K_LET();
    LetDefinitionListContext *letDefinitionList();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  LetStatementContext* letStatement();

  class  LetDefinitionListContext : public antlr4::ParserRuleContext {
  public:
    LetDefinitionListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<LetDefinitionContext *> letDefinition();
    LetDefinitionContext* letDefinition(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  LetDefinitionListContext* letDefinitionList();

  class  LetDefinitionContext : public antlr4::ParserRuleContext {
  public:
    LetDefinitionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *VARIABLE();
    antlr4::tree::TerminalNode *SINGLE_EQ();
    ConditionalOrExprContext *conditionalOrExpr();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  LetDefinitionContext* letDefinition();

  class  WhereStatementContext : public antlr4::ParserRuleContext {
  public:
    WhereStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *K_WHERE();
    ConditionalOrExprContext *conditionalOrExpr();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  WhereStatementContext* whereStatement();

  class  GroupByStatementContext : public antlr4::ParserRuleContext {
  public:
    GroupByStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *K_GROUP();
    antlr4::tree::TerminalNode *K_BY();
    std::vector<GroupByItemContext *> groupByItem();
    GroupByItemContext* groupByItem(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  GroupByStatementContext* groupByStatement();

  class  OrderByStatementContext : public antlr4::ParserRuleContext {
  public:
    OrderByStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *K_ORDER();
    antlr4::tree::TerminalNode *K_BY();
    std::vector<OrderByItemContext *> orderByItem();
    OrderByItemContext* orderByItem(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  OrderByStatementContext* orderByStatement();

  class  ReturnStatementContext : public antlr4::ParserRuleContext {
  public:
    ReturnStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    ReturnStatementContext() = default;
    void copyFrom(ReturnStatementContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  ReturnAllContext : public ReturnStatementContext {
  public:
    ReturnAllContext(ReturnStatementContext *ctx);

    antlr4::tree::TerminalNode *K_RETURN();
    antlr4::tree::TerminalNode *STAR();
    antlr4::tree::TerminalNode *K_DISTINCT();
    LimitOffsetClausesContext *limitOffsetClauses();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  ReturnListContext : public ReturnStatementContext {
  public:
    ReturnListContext(ReturnStatementContext *ctx);

    antlr4::tree::TerminalNode *K_RETURN();
    std::vector<ReturnItemContext *> returnItem();
    ReturnItemContext* returnItem(size_t i);
    antlr4::tree::TerminalNode *K_DISTINCT();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);
    LimitOffsetClausesContext *limitOffsetClauses();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  ReturnStatementContext* returnStatement();

  class  CallStatementContext : public antlr4::ParserRuleContext {
  public:
    CallStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *K_CALL();
    IdentifierContext *identifier();
    antlr4::tree::TerminalNode *L_PAR();
    antlr4::tree::TerminalNode *R_PAR();
    CallArgumentsContext *callArguments();
    YieldStatementContext *yieldStatement();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CallStatementContext* callStatement();

  class  YieldStatementContext : public antlr4::ParserRuleContext {
  public:
    YieldStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *K_YIELD();
    antlr4::tree::TerminalNode *STAR();
    std::vector<YieldItemContext *> yieldItem();
    YieldItemContext* yieldItem(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  YieldStatementContext* yieldStatement();

  class  YieldItemContext : public antlr4::ParserRuleContext {
  public:
    YieldItemContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *VARIABLE();
    AliasContext *alias();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  YieldItemContext* yieldItem();

  class  CallArgumentsContext : public antlr4::ParserRuleContext {
  public:
    CallArgumentsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ConditionalOrExprContext *> conditionalOrExpr();
    ConditionalOrExprContext* conditionalOrExpr(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CallArgumentsContext* callArguments();

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
    antlr4::tree::TerminalNode *K_LIMIT();
    antlr4::tree::TerminalNode *UNSIGNED_INTEGER();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  LimitClauseContext* limitClause();

  class  OffsetClauseContext : public antlr4::ParserRuleContext {
  public:
    OffsetClauseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *K_OFFSET();
    antlr4::tree::TerminalNode *UNSIGNED_INTEGER();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  OffsetClauseContext* offsetClause();

  class  ReturnItemContext : public antlr4::ParserRuleContext {
  public:
    ReturnItemContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    ReturnItemContext() = default;
    void copyFrom(ReturnItemContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  ReturnItemExprContext : public ReturnItemContext {
  public:
    ReturnItemExprContext(ReturnItemContext *ctx);

    ConditionalOrExprContext *conditionalOrExpr();
    AliasContext *alias();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  ReturnItemVarContext : public ReturnItemContext {
  public:
    ReturnItemVarContext(ReturnItemContext *ctx);

    antlr4::tree::TerminalNode *VARIABLE();
    antlr4::tree::TerminalNode *KEY();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  ReturnItemAggContext : public ReturnItemContext {
  public:
    ReturnItemAggContext(ReturnItemContext *ctx);

    AggregateFuncContext *aggregateFunc();
    antlr4::tree::TerminalNode *L_PAR();
    antlr4::tree::TerminalNode *VARIABLE();
    antlr4::tree::TerminalNode *R_PAR();
    antlr4::tree::TerminalNode *KEY();
    AliasContext *alias();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  ReturnItemCountContext : public ReturnItemContext {
  public:
    ReturnItemCountContext(ReturnItemContext *ctx);

    antlr4::tree::TerminalNode *K_COUNT();
    antlr4::tree::TerminalNode *L_PAR();
    antlr4::tree::TerminalNode *R_PAR();
    antlr4::tree::TerminalNode *VARIABLE();
    antlr4::tree::TerminalNode *STAR();
    antlr4::tree::TerminalNode *K_DISTINCT();
    AliasContext *alias();
    antlr4::tree::TerminalNode *KEY();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  ReturnItemContext* returnItem();

  class  AliasContext : public antlr4::ParserRuleContext {
  public:
    AliasContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *K_AS();
    antlr4::tree::TerminalNode *VARIABLE();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AliasContext* alias();

  class  AggregateFuncContext : public antlr4::ParserRuleContext {
  public:
    AggregateFuncContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *K_SUM();
    antlr4::tree::TerminalNode *K_MAX();
    antlr4::tree::TerminalNode *K_MIN();
    antlr4::tree::TerminalNode *K_AVG();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AggregateFuncContext* aggregateFunc();

  class  OrderByItemContext : public antlr4::ParserRuleContext {
  public:
    OrderByItemContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    OrderByItemContext() = default;
    void copyFrom(OrderByItemContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  OrderByItemCountContext : public OrderByItemContext {
  public:
    OrderByItemCountContext(OrderByItemContext *ctx);

    antlr4::tree::TerminalNode *K_COUNT();
    antlr4::tree::TerminalNode *L_PAR();
    antlr4::tree::TerminalNode *VARIABLE();
    antlr4::tree::TerminalNode *R_PAR();
    antlr4::tree::TerminalNode *K_DISTINCT();
    antlr4::tree::TerminalNode *KEY();
    antlr4::tree::TerminalNode *K_ASC();
    antlr4::tree::TerminalNode *K_DESC();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  OrderByItemExprContext : public OrderByItemContext {
  public:
    OrderByItemExprContext(OrderByItemContext *ctx);

    ConditionalOrExprContext *conditionalOrExpr();
    antlr4::tree::TerminalNode *K_ASC();
    antlr4::tree::TerminalNode *K_DESC();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  OrderByItemVarContext : public OrderByItemContext {
  public:
    OrderByItemVarContext(OrderByItemContext *ctx);

    antlr4::tree::TerminalNode *VARIABLE();
    antlr4::tree::TerminalNode *KEY();
    antlr4::tree::TerminalNode *K_ASC();
    antlr4::tree::TerminalNode *K_DESC();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  OrderByItemAggContext : public OrderByItemContext {
  public:
    OrderByItemAggContext(OrderByItemContext *ctx);

    AggregateFuncContext *aggregateFunc();
    antlr4::tree::TerminalNode *L_PAR();
    antlr4::tree::TerminalNode *VARIABLE();
    antlr4::tree::TerminalNode *R_PAR();
    antlr4::tree::TerminalNode *KEY();
    antlr4::tree::TerminalNode *K_ASC();
    antlr4::tree::TerminalNode *K_DESC();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  OrderByItemContext* orderByItem();

  class  GroupByItemContext : public antlr4::ParserRuleContext {
  public:
    GroupByItemContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *VARIABLE();
    antlr4::tree::TerminalNode *KEY();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  GroupByItemContext* groupByItem();

  class  GraphPatternContext : public antlr4::ParserRuleContext {
  public:
    GraphPatternContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    BasicPatternContext *basicPattern();
    std::vector<OptionalPatternContext *> optionalPattern();
    OptionalPatternContext* optionalPattern(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  GraphPatternContext* graphPattern();

  class  OptionalPatternContext : public antlr4::ParserRuleContext {
  public:
    OptionalPatternContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *K_OPTIONAL();
    antlr4::tree::TerminalNode *LCURLY_BRACKET();
    GraphPatternContext *graphPattern();
    antlr4::tree::TerminalNode *RCURLY_BRACKET();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  OptionalPatternContext* optionalPattern();

  class  BasicPatternContext : public antlr4::ParserRuleContext {
  public:
    BasicPatternContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<LinearPatternContext *> linearPattern();
    LinearPatternContext* linearPattern(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  BasicPatternContext* basicPattern();

  class  LinearPatternContext : public antlr4::ParserRuleContext {
  public:
    LinearPatternContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<NodeContext *> node();
    NodeContext* node(size_t i);
    std::vector<EdgeContext *> edge();
    EdgeContext* edge(size_t i);
    std::vector<PathContext *> path();
    PathContext* path(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  LinearPatternContext* linearPattern();

  class  PathContext : public antlr4::ParserRuleContext {
  public:
    PathContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LEQ();
    antlr4::tree::TerminalNode *LSQUARE_BRACKET();
    PathAlternativesContext *pathAlternatives();
    antlr4::tree::TerminalNode *RSQUARE_BRACKET();
    std::vector<antlr4::tree::TerminalNode *> SINGLE_EQ();
    antlr4::tree::TerminalNode* SINGLE_EQ(size_t i);
    PathTypeContext *pathType();
    antlr4::tree::TerminalNode *VARIABLE();
    antlr4::tree::TerminalNode *GT();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PathContext* path();

  class  PathAlternativesContext : public antlr4::ParserRuleContext {
  public:
    PathAlternativesContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<PathSequenceContext *> pathSequence();
    PathSequenceContext* pathSequence(size_t i);
    std::vector<antlr4::tree::TerminalNode *> PATH_ALTERNATIVE();
    antlr4::tree::TerminalNode* PATH_ALTERNATIVE(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PathAlternativesContext* pathAlternatives();

  class  PathSequenceContext : public antlr4::ParserRuleContext {
  public:
    PathSequenceContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<PathAtomContext *> pathAtom();
    PathAtomContext* pathAtom(size_t i);
    std::vector<antlr4::tree::TerminalNode *> PATH_SEQUENCE();
    antlr4::tree::TerminalNode* PATH_SEQUENCE(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PathSequenceContext* pathSequence();

  class  PathAtomContext : public antlr4::ParserRuleContext {
  public:
    PathAtomContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    PathAtomContext() = default;
    void copyFrom(PathAtomContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  PathAtomAlternativesContext : public PathAtomContext {
  public:
    PathAtomAlternativesContext(PathAtomContext *ctx);

    antlr4::tree::TerminalNode *L_PAR();
    PathAlternativesContext *pathAlternatives();
    antlr4::tree::TerminalNode *R_PAR();
    antlr4::tree::TerminalNode *PATH_NEGATION();
    PathSuffixContext *pathSuffix();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  PathAtomSimpleContext : public PathAtomContext {
  public:
    PathAtomSimpleContext(PathAtomContext *ctx);

    antlr4::tree::TerminalNode *TYPE();
    antlr4::tree::TerminalNode *PATH_NEGATION();
    PathSuffixContext *pathSuffix();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  PathAtomContext* pathAtom();

  class  PathSuffixContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *op = nullptr;
    antlr4::Token *min = nullptr;
    antlr4::Token *max = nullptr;
    PathSuffixContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *STAR();
    antlr4::tree::TerminalNode *PLUS();
    antlr4::tree::TerminalNode *QUESTION_MARK();
    antlr4::tree::TerminalNode *LCURLY_BRACKET();
    antlr4::tree::TerminalNode *COMMA();
    antlr4::tree::TerminalNode *RCURLY_BRACKET();
    std::vector<antlr4::tree::TerminalNode *> UNSIGNED_INTEGER();
    antlr4::tree::TerminalNode* UNSIGNED_INTEGER(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PathSuffixContext* pathSuffix();

  class  PathTypeContext : public antlr4::ParserRuleContext {
  public:
    PathTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *K_ANY();
    antlr4::tree::TerminalNode *K_ALL();
    antlr4::tree::TerminalNode *K_SHORTEST();
    antlr4::tree::TerminalNode *K_WALKS();
    antlr4::tree::TerminalNode *K_SIMPLE();
    antlr4::tree::TerminalNode *K_ACYCLIC();
    antlr4::tree::TerminalNode *K_TRAILS();
    antlr4::tree::TerminalNode *UNSIGNED_INTEGER();
    antlr4::tree::TerminalNode *K_GROUPS();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PathTypeContext* pathType();

  class  NodeContext : public antlr4::ParserRuleContext {
  public:
    NodeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    FixedNodeContext *fixedNode();
    VarNodeContext *varNode();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  NodeContext* node();

  class  FixedNodeContext : public antlr4::ParserRuleContext {
  public:
    FixedNodeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *L_PAR();
    FixedNodeInsideContext *fixedNodeInside();
    antlr4::tree::TerminalNode *R_PAR();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FixedNodeContext* fixedNode();

  class  FixedNodeInsideContext : public antlr4::ParserRuleContext {
  public:
    FixedNodeInsideContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdentifierContext *identifier();
    antlr4::tree::TerminalNode *ANON_ID();
    antlr4::tree::TerminalNode *EDGE_ID();
    ValueContext *value();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FixedNodeInsideContext* fixedNodeInside();

  class  EdgeContext : public antlr4::ParserRuleContext {
  public:
    EdgeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LT();
    std::vector<antlr4::tree::TerminalNode *> MINUS();
    antlr4::tree::TerminalNode* MINUS(size_t i);
    EdgeInsideContext *edgeInside();
    antlr4::tree::TerminalNode *GT();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  EdgeContext* edge();

  class  EdgeInsideContext : public antlr4::ParserRuleContext {
  public:
    EdgeInsideContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LSQUARE_BRACKET();
    antlr4::tree::TerminalNode *RSQUARE_BRACKET();
    PropertiesContext *properties();
    antlr4::tree::TerminalNode *VARIABLE();
    antlr4::tree::TerminalNode *EDGE_ID();
    antlr4::tree::TerminalNode *TYPE();
    antlr4::tree::TerminalNode *TYPE_VAR();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  EdgeInsideContext* edgeInside();

  class  VarNodeContext : public antlr4::ParserRuleContext {
  public:
    VarNodeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *L_PAR();
    antlr4::tree::TerminalNode *R_PAR();
    antlr4::tree::TerminalNode *VARIABLE();
    std::vector<antlr4::tree::TerminalNode *> TYPE();
    antlr4::tree::TerminalNode* TYPE(size_t i);
    PropertiesContext *properties();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  VarNodeContext* varNode();

  class  PropertiesContext : public antlr4::ParserRuleContext {
  public:
    PropertiesContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LCURLY_BRACKET();
    std::vector<PropertyContext *> property();
    PropertyContext* property(size_t i);
    antlr4::tree::TerminalNode *RCURLY_BRACKET();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PropertiesContext* properties();

  class  PropertyContext : public antlr4::ParserRuleContext {
  public:
    PropertyContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    PropertyContext() = default;
    void copyFrom(PropertyContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  Property2Context : public PropertyContext {
  public:
    Property2Context(PropertyContext *ctx);

    IdentifierContext *identifier();
    antlr4::tree::TerminalNode *TYPE();
    antlr4::tree::TerminalNode *L_PAR();
    antlr4::tree::TerminalNode *STRING();
    antlr4::tree::TerminalNode *R_PAR();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Property1Context : public PropertyContext {
  public:
    Property1Context(PropertyContext *ctx);

    IdentifierContext *identifier();
    antlr4::tree::TerminalNode *COLON();
    ValueContext *value();
    antlr4::tree::TerminalNode *TRUE_PROP();
    antlr4::tree::TerminalNode *FALSE_PROP();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Property4Context : public PropertyContext {
  public:
    Property4Context(PropertyContext *ctx);

    antlr4::Token *op = nullptr;
    IdentifierContext *identifier();
    ValueContext *value();
    antlr4::tree::TerminalNode *EQ();
    antlr4::tree::TerminalNode *NEQ();
    antlr4::tree::TerminalNode *LT();
    antlr4::tree::TerminalNode *GT();
    antlr4::tree::TerminalNode *LEQ();
    antlr4::tree::TerminalNode *GEQ();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Property3Context : public PropertyContext {
  public:
    Property3Context(PropertyContext *ctx);

    IdentifierContext *identifier();
    antlr4::tree::TerminalNode *K_IS();
    ExprTypenameContext *exprTypename();
    antlr4::tree::TerminalNode *K_NOT();
    std::vector<ConditionalOrTypeContext *> conditionalOrType();
    ConditionalOrTypeContext* conditionalOrType(size_t i);

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  PropertyContext* property();

  class  ConditionalOrTypeContext : public antlr4::ParserRuleContext {
  public:
    ConditionalOrTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *K_OR();
    ExprTypenameContext *exprTypename();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ConditionalOrTypeContext* conditionalOrType();

  class  IdentifierContext : public antlr4::ParserRuleContext {
  public:
    IdentifierContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *NAME();
    KeywordContext *keyword();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  IdentifierContext* identifier();

  class  BoolValueContext : public antlr4::ParserRuleContext {
  public:
    BoolValueContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *K_TRUE();
    antlr4::tree::TerminalNode *K_FALSE();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  BoolValueContext* boolValue();

  class  NumericValueContext : public antlr4::ParserRuleContext {
  public:
    NumericValueContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *UNSIGNED_INTEGER();
    antlr4::tree::TerminalNode *UNSIGNED_FLOAT();
    antlr4::tree::TerminalNode *UNSIGNED_SCIENTIFIC_NOTATION();
    antlr4::tree::TerminalNode *PLUS();
    antlr4::tree::TerminalNode *MINUS();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  NumericValueContext* numericValue();

  class  DatatypeValueContext : public antlr4::ParserRuleContext {
  public:
    DatatypeValueContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdentifierContext *identifier();
    antlr4::tree::TerminalNode *L_PAR();
    antlr4::tree::TerminalNode *STRING();
    antlr4::tree::TerminalNode *R_PAR();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DatatypeValueContext* datatypeValue();

  class  ValueContext : public antlr4::ParserRuleContext {
  public:
    ValueContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    NumericValueContext *numericValue();
    antlr4::tree::TerminalNode *STRING();
    BoolValueContext *boolValue();
    DatatypeValueContext *datatypeValue();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ValueContext* value();

  class  ConditionalOrExprContext : public antlr4::ParserRuleContext {
  public:
    ConditionalOrExprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ConditionalAndExprContext *> conditionalAndExpr();
    ConditionalAndExprContext* conditionalAndExpr(size_t i);
    std::vector<antlr4::tree::TerminalNode *> K_OR();
    antlr4::tree::TerminalNode* K_OR(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ConditionalOrExprContext* conditionalOrExpr();

  class  ConditionalAndExprContext : public antlr4::ParserRuleContext {
  public:
    ConditionalAndExprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ComparisonExprContext *> comparisonExpr();
    ComparisonExprContext* comparisonExpr(size_t i);
    std::vector<antlr4::tree::TerminalNode *> K_AND();
    antlr4::tree::TerminalNode* K_AND(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ConditionalAndExprContext* conditionalAndExpr();

  class  ComparisonExprContext : public antlr4::ParserRuleContext {
  public:
    ComparisonExprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    ComparisonExprContext() = default;
    void copyFrom(ComparisonExprContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  ComparisonExprIsContext : public ComparisonExprContext {
  public:
    ComparisonExprIsContext(ComparisonExprContext *ctx);

    AdditiveExprContext *additiveExpr();
    antlr4::tree::TerminalNode *K_IS();
    ExprTypenameContext *exprTypename();
    antlr4::tree::TerminalNode *K_NOT();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  ComparisonExprOpContext : public ComparisonExprContext {
  public:
    ComparisonExprOpContext(ComparisonExprContext *ctx);

    antlr4::Token *op = nullptr;
    std::vector<AdditiveExprContext *> additiveExpr();
    AdditiveExprContext* additiveExpr(size_t i);
    antlr4::tree::TerminalNode *EQ();
    antlr4::tree::TerminalNode *NEQ();
    antlr4::tree::TerminalNode *LT();
    antlr4::tree::TerminalNode *GT();
    antlr4::tree::TerminalNode *LEQ();
    antlr4::tree::TerminalNode *GEQ();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  ComparisonExprContext* comparisonExpr();

  class  AdditiveExprContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *s94 = nullptr;
    std::vector<antlr4::Token *> op;
    antlr4::Token *s95 = nullptr;
    antlr4::Token *_tset1392 = nullptr;
    AdditiveExprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<MultiplicativeExprContext *> multiplicativeExpr();
    MultiplicativeExprContext* multiplicativeExpr(size_t i);
    std::vector<antlr4::tree::TerminalNode *> PLUS();
    antlr4::tree::TerminalNode* PLUS(size_t i);
    std::vector<antlr4::tree::TerminalNode *> MINUS();
    antlr4::tree::TerminalNode* MINUS(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AdditiveExprContext* additiveExpr();

  class  MultiplicativeExprContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *s91 = nullptr;
    std::vector<antlr4::Token *> op;
    antlr4::Token *s88 = nullptr;
    antlr4::Token *s92 = nullptr;
    antlr4::Token *_tset1411 = nullptr;
    MultiplicativeExprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<UnaryExprContext *> unaryExpr();
    UnaryExprContext* unaryExpr(size_t i);
    std::vector<antlr4::tree::TerminalNode *> STAR();
    antlr4::tree::TerminalNode* STAR(size_t i);
    std::vector<antlr4::tree::TerminalNode *> PATH_SEQUENCE();
    antlr4::tree::TerminalNode* PATH_SEQUENCE(size_t i);
    std::vector<antlr4::tree::TerminalNode *> PERCENT();
    antlr4::tree::TerminalNode* PERCENT(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  MultiplicativeExprContext* multiplicativeExpr();

  class  UnaryExprContext : public antlr4::ParserRuleContext {
  public:
    UnaryExprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *K_NOT();
    UnaryExprContext *unaryExpr();
    AtomicExprContext *atomicExpr();
    antlr4::tree::TerminalNode *PLUS();
    antlr4::tree::TerminalNode *MINUS();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  UnaryExprContext* unaryExpr();

  class  AtomicExprContext : public antlr4::ParserRuleContext {
  public:
    AtomicExprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    AtomicExprContext() = default;
    void copyFrom(AtomicExprContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  ExprFunctionContext : public AtomicExprContext {
  public:
    ExprFunctionContext(AtomicExprContext *ctx);

    FunctionContext *function();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  ExprValueContext : public AtomicExprContext {
  public:
    ExprValueContext(AtomicExprContext *ctx);

    ValueContext *value();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  ExprFixedNodeInsideContext : public AtomicExprContext {
  public:
    ExprFixedNodeInsideContext(AtomicExprContext *ctx);

    FixedNodeInsideContext *fixedNodeInside();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  ExprVarContext : public AtomicExprContext {
  public:
    ExprVarContext(AtomicExprContext *ctx);

    antlr4::tree::TerminalNode *VARIABLE();
    antlr4::tree::TerminalNode *KEY();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  ExprParenthesisContext : public AtomicExprContext {
  public:
    ExprParenthesisContext(AtomicExprContext *ctx);

    antlr4::tree::TerminalNode *L_PAR();
    ConditionalOrExprContext *conditionalOrExpr();
    antlr4::tree::TerminalNode *R_PAR();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  AtomicExprContext* atomicExpr();

  class  FunctionContext : public antlr4::ParserRuleContext {
  public:
    FunctionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    RegexContext *regex();
    TextSearchContext *textSearch();
    CosineSimilarityContext *cosineSimilarity();
    CosineDistanceContext *cosineDistance();
    ManhattanDistanceContext *manhattanDistance();
    EuclideanDistanceContext *euclideanDistance();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FunctionContext* function();

  class  RegexContext : public antlr4::ParserRuleContext {
  public:
    RegexContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *K_REGEX();
    antlr4::tree::TerminalNode *L_PAR();
    std::vector<ConditionalOrExprContext *> conditionalOrExpr();
    ConditionalOrExprContext* conditionalOrExpr(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);
    antlr4::tree::TerminalNode *R_PAR();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  RegexContext* regex();

  class  TextSearchContext : public antlr4::ParserRuleContext {
  public:
    TextSearchContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *K_TEXT_SEARCH();
    antlr4::tree::TerminalNode *L_PAR();
    std::vector<antlr4::tree::TerminalNode *> STRING();
    antlr4::tree::TerminalNode* STRING(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);
    TextSearchIndexModeContext *textSearchIndexMode();
    std::vector<antlr4::tree::TerminalNode *> VARIABLE();
    antlr4::tree::TerminalNode* VARIABLE(size_t i);
    antlr4::tree::TerminalNode *R_PAR();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TextSearchContext* textSearch();

  class  CosineSimilarityContext : public antlr4::ParserRuleContext {
  public:
    CosineSimilarityContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *K_COSINE_SIMILARITY();
    antlr4::tree::TerminalNode *L_PAR();
    std::vector<ConditionalOrExprContext *> conditionalOrExpr();
    ConditionalOrExprContext* conditionalOrExpr(size_t i);
    antlr4::tree::TerminalNode *COMMA();
    antlr4::tree::TerminalNode *R_PAR();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CosineSimilarityContext* cosineSimilarity();

  class  CosineDistanceContext : public antlr4::ParserRuleContext {
  public:
    CosineDistanceContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *K_COSINE_DISTANCE();
    antlr4::tree::TerminalNode *L_PAR();
    std::vector<ConditionalOrExprContext *> conditionalOrExpr();
    ConditionalOrExprContext* conditionalOrExpr(size_t i);
    antlr4::tree::TerminalNode *COMMA();
    antlr4::tree::TerminalNode *R_PAR();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CosineDistanceContext* cosineDistance();

  class  ManhattanDistanceContext : public antlr4::ParserRuleContext {
  public:
    ManhattanDistanceContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *K_MANHATTAN_DISTANCE();
    antlr4::tree::TerminalNode *L_PAR();
    std::vector<ConditionalOrExprContext *> conditionalOrExpr();
    ConditionalOrExprContext* conditionalOrExpr(size_t i);
    antlr4::tree::TerminalNode *COMMA();
    antlr4::tree::TerminalNode *R_PAR();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ManhattanDistanceContext* manhattanDistance();

  class  EuclideanDistanceContext : public antlr4::ParserRuleContext {
  public:
    EuclideanDistanceContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *K_EUCLIDEAN_DISTANCE();
    antlr4::tree::TerminalNode *L_PAR();
    std::vector<ConditionalOrExprContext *> conditionalOrExpr();
    ConditionalOrExprContext* conditionalOrExpr(size_t i);
    antlr4::tree::TerminalNode *COMMA();
    antlr4::tree::TerminalNode *R_PAR();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  EuclideanDistanceContext* euclideanDistance();

  class  TextSearchIndexModeContext : public antlr4::ParserRuleContext {
  public:
    TextSearchIndexModeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *K_PREFIX();
    antlr4::tree::TerminalNode *K_MATCH();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TextSearchIndexModeContext* textSearchIndexMode();

  class  ExprTypenameContext : public antlr4::ParserRuleContext {
  public:
    ExprTypenameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *K_NULL();
    antlr4::tree::TerminalNode *K_STRING();
    antlr4::tree::TerminalNode *K_BOOL();
    antlr4::tree::TerminalNode *K_INTEGER();
    antlr4::tree::TerminalNode *K_FLOAT();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ExprTypenameContext* exprTypename();

  class  KeywordContext : public antlr4::ParserRuleContext {
  public:
    KeywordContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *K_ACYCLIC();
    antlr4::tree::TerminalNode *K_AND();
    antlr4::tree::TerminalNode *K_ANY();
    antlr4::tree::TerminalNode *K_AS();
    antlr4::tree::TerminalNode *K_AVG();
    antlr4::tree::TerminalNode *K_ALL();
    antlr4::tree::TerminalNode *K_ASC();
    antlr4::tree::TerminalNode *K_BY();
    antlr4::tree::TerminalNode *K_BOOL();
    antlr4::tree::TerminalNode *K_CALL();
    antlr4::tree::TerminalNode *K_COSINE_DISTANCE();
    antlr4::tree::TerminalNode *K_COSINE_SIMILARITY();
    antlr4::tree::TerminalNode *K_COUNT();
    antlr4::tree::TerminalNode *K_CREATE();
    antlr4::tree::TerminalNode *K_DELETE();
    antlr4::tree::TerminalNode *K_DESCRIBE();
    antlr4::tree::TerminalNode *K_DESC();
    antlr4::tree::TerminalNode *K_DIMENSIONS();
    antlr4::tree::TerminalNode *K_DISTINCT();
    antlr4::tree::TerminalNode *K_EDGE();
    antlr4::tree::TerminalNode *K_EUCLIDEAN_DISTANCE();
    antlr4::tree::TerminalNode *K_FROM();
    antlr4::tree::TerminalNode *K_INCOMING();
    antlr4::tree::TerminalNode *K_INDEX();
    antlr4::tree::TerminalNode *K_INSERT();
    antlr4::tree::TerminalNode *K_INTEGER();
    antlr4::tree::TerminalNode *K_INTO();
    antlr4::tree::TerminalNode *K_IS();
    antlr4::tree::TerminalNode *K_FLOAT();
    antlr4::tree::TerminalNode *K_GROUP();
    antlr4::tree::TerminalNode *K_GROUPS();
    antlr4::tree::TerminalNode *K_IDENTITY();
    antlr4::tree::TerminalNode *K_LABELS();
    antlr4::tree::TerminalNode *K_LABEL();
    antlr4::tree::TerminalNode *K_LET();
    antlr4::tree::TerminalNode *K_LIMIT();
    antlr4::tree::TerminalNode *K_MANHATTAN_DISTANCE();
    antlr4::tree::TerminalNode *K_MATCH();
    antlr4::tree::TerminalNode *K_MAX();
    antlr4::tree::TerminalNode *K_MIN();
    antlr4::tree::TerminalNode *K_OBJECTS();
    antlr4::tree::TerminalNode *K_OFFSET();
    antlr4::tree::TerminalNode *K_OPTIONAL();
    antlr4::tree::TerminalNode *K_ORDER();
    antlr4::tree::TerminalNode *K_OR();
    antlr4::tree::TerminalNode *K_OUTGOING();
    antlr4::tree::TerminalNode *K_PREFIX();
    antlr4::tree::TerminalNode *K_PROPERTIES();
    antlr4::tree::TerminalNode *K_PROPERTY();
    antlr4::tree::TerminalNode *K_NOT();
    antlr4::tree::TerminalNode *K_NULL();
    antlr4::tree::TerminalNode *K_SHORTEST();
    antlr4::tree::TerminalNode *K_SHOW();
    antlr4::tree::TerminalNode *K_SIMPLE();
    antlr4::tree::TerminalNode *K_TEXT_SEARCH();
    antlr4::tree::TerminalNode *K_REGEX();
    antlr4::tree::TerminalNode *K_RETURN();
    antlr4::tree::TerminalNode *K_SUM();
    antlr4::tree::TerminalNode *K_STRING();
    antlr4::tree::TerminalNode *K_TRAILS();
    antlr4::tree::TerminalNode *K_VALUES();
    antlr4::tree::TerminalNode *K_WALKS();
    antlr4::tree::TerminalNode *K_WITH();
    antlr4::tree::TerminalNode *K_WHERE();
    antlr4::tree::TerminalNode *K_YIELD();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  KeywordContext* keyword();


  // By default the static state used to implement the parser is lazily initialized during the first
  // call to the constructor. You can call this function if you wish to initialize the static state
  // ahead of time.
  static void initialize();

private:
};



// Generated from MQL_Parser.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"




class  MQL_Parser : public antlr4::Parser {
public:
  enum {
    K_ACYCLIC = 1, K_AND = 2, K_ANY = 3, K_AS = 4, K_AVG = 5, K_ALL = 6, 
    K_ASC = 7, K_BY = 8, K_BOOL = 9, K_CALL = 10, K_COSINE_DISTANCE = 11, 
    K_COSINE_SIMILARITY = 12, K_COUNT = 13, K_CREATE = 14, K_DELETE = 15, 
    K_DESCRIBE = 16, K_DESC = 17, K_DETACH = 18, K_DIMENSIONS = 19, K_DISTINCT = 20, 
    K_EDGE = 21, K_EDIT_DISTANCE = 22, K_EUCLIDEAN_DISTANCE = 23, K_FROM = 24, 
    K_HAVING = 25, K_IDENTITY = 26, K_INCOMING = 27, K_INDEX = 28, K_INSERT = 29, 
    K_INTEGER = 30, K_INTO = 31, K_IS = 32, K_FALSE = 33, K_FLOAT = 34, 
    K_GROUP = 35, K_GROUPS = 36, K_LABELS = 37, K_LABEL = 38, K_LET = 39, 
    K_LIMIT = 40, K_MANHATTAN_DISTANCE = 41, K_MATCH = 42, K_MAX = 43, K_MIN = 44, 
    K_NORMALIZE = 45, K_OBJECTS = 46, K_OFFSET = 47, K_OPTIONAL = 48, K_ORDER = 49, 
    K_OR = 50, K_OUTGOING = 51, K_PREFIX = 52, K_PROPERTY = 53, K_PROPERTIES = 54, 
    K_NOT = 55, K_NULL = 56, K_SHORTEST = 57, K_SHOW = 58, K_SIMPLE = 59, 
    K_STRING = 60, K_STR = 61, K_TEXT_SEARCH = 62, K_TYPE = 63, K_REGEX = 64, 
    K_REMOVE = 65, K_RETURN = 66, K_SET = 67, K_SUM = 68, K_TRUE = 69, K_TRAILS = 70, 
    K_VALUES = 71, K_WALKS = 72, K_WITH = 73, K_WHERE = 74, K_YIELD = 75, 
    TRUE_PROP = 76, FALSE_PROP = 77, ANON_ID = 78, EDGE_ID = 79, KEY = 80, 
    TYPE = 81, TYPE_VAR = 82, VARIABLE = 83, STRING = 84, UNSIGNED_INTEGER = 85, 
    UNSIGNED_FLOAT = 86, UNSIGNED_SCIENTIFIC_NOTATION = 87, NAME = 88, LEQ = 89, 
    GEQ = 90, EQ = 91, NEQ = 92, LT = 93, GT = 94, SINGLE_EQ = 95, PATH_SEQUENCE = 96, 
    PATH_ALTERNATIVE = 97, PATH_NEGATION = 98, STAR = 99, PERCENT = 100, 
    QUESTION_MARK = 101, PLUS = 102, MINUS = 103, L_PAR = 104, R_PAR = 105, 
    LCURLY_BRACKET = 106, RCURLY_BRACKET = 107, LSQUARE_BRACKET = 108, RSQUARE_BRACKET = 109, 
    COMMA = 110, COLON = 111, WHITE_SPACE = 112, SINGLE_LINE_COMMENT = 113, 
    UNRECOGNIZED = 114
  };

  enum {
    RuleRoot = 0, RuleUpdateStatement = 1, RuleInsertStatement = 2, RuleInsertLinearPattern = 3, 
    RuleInsertNode = 4, RuleInsertEdge = 5, RuleInsertProperties = 6, RuleInsertProperty = 7, 
    RuleDeleteStatement = 8, RuleSetStatement = 9, RuleSetAtom = 10, RuleRemoveStatement = 11, 
    RuleRemoveAtom = 12, RuleSimpleQuery = 13, RulePrimitiveStatement = 14, 
    RuleCreateIndexQuery = 15, RuleCreateIndexOptions = 16, RuleCreateIndexOption = 17, 
    RuleShowQuery = 18, RuleDescribeQuery = 19, RuleDescribeFlag = 20, RuleMatchStatement = 21, 
    RuleLetStatement = 22, RuleLetDefinition = 23, RuleWhereStatement = 24, 
    RuleGroupByStatement = 25, RuleHavingStatement = 26, RuleOrderByStatement = 27, 
    RuleReturnStatement = 28, RuleCallStatement = 29, RuleYieldStatement = 30, 
    RuleYieldItem = 31, RuleCallArguments = 32, RuleLimitOffsetClauses = 33, 
    RuleLimitClause = 34, RuleOffsetClause = 35, RuleReturnItem = 36, RuleAlias = 37, 
    RuleAggregateFunc = 38, RuleOrderByItem = 39, RuleGroupByItem = 40, 
    RuleGraphPattern = 41, RuleOptionalPattern = 42, RuleBasicPattern = 43, 
    RuleLinearPattern = 44, RulePath = 45, RulePathAlternatives = 46, RulePathSequence = 47, 
    RulePathAtom = 48, RulePathSuffix = 49, RulePathType = 50, RuleNode = 51, 
    RuleFixedObj = 52, RuleVarNode = 53, RuleEdge = 54, RuleEdgeInside = 55, 
    RuleProperties = 56, RuleProperty = 57, RuleConditionalOrType = 58, 
    RuleIdentifier = 59, RuleBoolValue = 60, RuleNumericValue = 61, RuleDatatypeValue = 62, 
    RuleValue = 63, RuleConditionalOrExpr = 64, RuleConditionalAndExpr = 65, 
    RuleComparisonExpr = 66, RuleAdditiveExpr = 67, RuleMultiplicativeExpr = 68, 
    RuleUnaryExpr = 69, RuleAtomicExpr = 70, RuleFunction = 71, RuleRegex = 72, 
    RuleCosineSimilarity = 73, RuleCosineDistance = 74, RuleManhattanDistance = 75, 
    RuleEuclideanDistance = 76, RuleEditDistance = 77, RuleNormalize = 78, 
    RuleStr = 79, RuleLabels = 80, RulePropertiesFunction = 81, RuleType = 82, 
    RuleTextSearchIndexMode = 83, RuleExprTypename = 84, RuleKeyword = 85
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
  class UpdateStatementContext;
  class InsertStatementContext;
  class InsertLinearPatternContext;
  class InsertNodeContext;
  class InsertEdgeContext;
  class InsertPropertiesContext;
  class InsertPropertyContext;
  class DeleteStatementContext;
  class SetStatementContext;
  class SetAtomContext;
  class RemoveStatementContext;
  class RemoveAtomContext;
  class SimpleQueryContext;
  class PrimitiveStatementContext;
  class CreateIndexQueryContext;
  class CreateIndexOptionsContext;
  class CreateIndexOptionContext;
  class ShowQueryContext;
  class DescribeQueryContext;
  class DescribeFlagContext;
  class MatchStatementContext;
  class LetStatementContext;
  class LetDefinitionContext;
  class WhereStatementContext;
  class GroupByStatementContext;
  class HavingStatementContext;
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
  class FixedObjContext;
  class VarNodeContext;
  class EdgeContext;
  class EdgeInsideContext;
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
  class CosineSimilarityContext;
  class CosineDistanceContext;
  class ManhattanDistanceContext;
  class EuclideanDistanceContext;
  class EditDistanceContext;
  class NormalizeContext;
  class StrContext;
  class LabelsContext;
  class PropertiesFunctionContext;
  class TypeContext;
  class TextSearchIndexModeContext;
  class ExprTypenameContext;
  class KeywordContext; 

  class  RootContext : public antlr4::ParserRuleContext {
  public:
    RootContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *EOF();
    SimpleQueryContext *simpleQuery();
    DescribeQueryContext *describeQuery();
    CreateIndexQueryContext *createIndexQuery();
    ShowQueryContext *showQuery();
    UpdateStatementContext *updateStatement();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  RootContext* root();

  class  UpdateStatementContext : public antlr4::ParserRuleContext {
  public:
    UpdateStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    InsertStatementContext *insertStatement();
    DeleteStatementContext *deleteStatement();
    SetStatementContext *setStatement();
    RemoveStatementContext *removeStatement();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  UpdateStatementContext* updateStatement();

  class  InsertStatementContext : public antlr4::ParserRuleContext {
  public:
    InsertStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *K_INSERT();
    std::vector<InsertLinearPatternContext *> insertLinearPattern();
    InsertLinearPatternContext* insertLinearPattern(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  InsertStatementContext* insertStatement();

  class  InsertLinearPatternContext : public antlr4::ParserRuleContext {
  public:
    InsertLinearPatternContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<InsertNodeContext *> insertNode();
    InsertNodeContext* insertNode(size_t i);
    std::vector<InsertEdgeContext *> insertEdge();
    InsertEdgeContext* insertEdge(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  InsertLinearPatternContext* insertLinearPattern();

  class  InsertNodeContext : public antlr4::ParserRuleContext {
  public:
    InsertNodeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *L_PAR();
    antlr4::tree::TerminalNode *R_PAR();
    IdentifierContext *identifier();
    antlr4::tree::TerminalNode *VARIABLE();
    std::vector<antlr4::tree::TerminalNode *> TYPE();
    antlr4::tree::TerminalNode* TYPE(size_t i);
    InsertPropertiesContext *insertProperties();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  InsertNodeContext* insertNode();

  class  InsertEdgeContext : public antlr4::ParserRuleContext {
  public:
    InsertEdgeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LT();
    std::vector<antlr4::tree::TerminalNode *> MINUS();
    antlr4::tree::TerminalNode* MINUS(size_t i);
    antlr4::tree::TerminalNode *LSQUARE_BRACKET();
    antlr4::tree::TerminalNode *TYPE();
    antlr4::tree::TerminalNode *RSQUARE_BRACKET();
    InsertPropertiesContext *insertProperties();
    antlr4::tree::TerminalNode *GT();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  InsertEdgeContext* insertEdge();

  class  InsertPropertiesContext : public antlr4::ParserRuleContext {
  public:
    InsertPropertiesContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LCURLY_BRACKET();
    std::vector<InsertPropertyContext *> insertProperty();
    InsertPropertyContext* insertProperty(size_t i);
    antlr4::tree::TerminalNode *RCURLY_BRACKET();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  InsertPropertiesContext* insertProperties();

  class  InsertPropertyContext : public antlr4::ParserRuleContext {
  public:
    InsertPropertyContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    InsertPropertyContext() = default;
    void copyFrom(InsertPropertyContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  InsertProperty2Context : public InsertPropertyContext {
  public:
    InsertProperty2Context(InsertPropertyContext *ctx);

    IdentifierContext *identifier();
    antlr4::tree::TerminalNode *TYPE();
    antlr4::tree::TerminalNode *L_PAR();
    antlr4::tree::TerminalNode *STRING();
    antlr4::tree::TerminalNode *R_PAR();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  InsertProperty3Context : public InsertPropertyContext {
  public:
    InsertProperty3Context(InsertPropertyContext *ctx);

    IdentifierContext *identifier();
    antlr4::tree::TerminalNode *COLON();
    ConditionalOrExprContext *conditionalOrExpr();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  InsertProperty1Context : public InsertPropertyContext {
  public:
    InsertProperty1Context(InsertPropertyContext *ctx);

    IdentifierContext *identifier();
    antlr4::tree::TerminalNode *COLON();
    ValueContext *value();
    antlr4::tree::TerminalNode *TRUE_PROP();
    antlr4::tree::TerminalNode *FALSE_PROP();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  InsertPropertyContext* insertProperty();

  class  DeleteStatementContext : public antlr4::ParserRuleContext {
  public:
    DeleteStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *K_DELETE();
    std::vector<FixedObjContext *> fixedObj();
    FixedObjContext* fixedObj(size_t i);
    std::vector<antlr4::tree::TerminalNode *> VARIABLE();
    antlr4::tree::TerminalNode* VARIABLE(size_t i);
    antlr4::tree::TerminalNode *K_DETACH();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DeleteStatementContext* deleteStatement();

  class  SetStatementContext : public antlr4::ParserRuleContext {
  public:
    SetStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *K_SET();
    std::vector<SetAtomContext *> setAtom();
    SetAtomContext* setAtom(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SetStatementContext* setStatement();

  class  SetAtomContext : public antlr4::ParserRuleContext {
  public:
    SetAtomContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *KEY();
    antlr4::tree::TerminalNode *SINGLE_EQ();
    ConditionalOrExprContext *conditionalOrExpr();
    FixedObjContext *fixedObj();
    antlr4::tree::TerminalNode *VARIABLE();
    InsertPropertiesContext *insertProperties();
    std::vector<antlr4::tree::TerminalNode *> TYPE();
    antlr4::tree::TerminalNode* TYPE(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SetAtomContext* setAtom();

  class  RemoveStatementContext : public antlr4::ParserRuleContext {
  public:
    RemoveStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *K_REMOVE();
    std::vector<RemoveAtomContext *> removeAtom();
    RemoveAtomContext* removeAtom(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  RemoveStatementContext* removeStatement();

  class  RemoveAtomContext : public antlr4::ParserRuleContext {
  public:
    RemoveAtomContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *KEY();
    FixedObjContext *fixedObj();
    antlr4::tree::TerminalNode *VARIABLE();
    std::vector<antlr4::tree::TerminalNode *> TYPE();
    antlr4::tree::TerminalNode* TYPE(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  RemoveAtomContext* removeAtom();

  class  SimpleQueryContext : public antlr4::ParserRuleContext {
  public:
    SimpleQueryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ReturnStatementContext *returnStatement();
    std::vector<PrimitiveStatementContext *> primitiveStatement();
    PrimitiveStatementContext* primitiveStatement(size_t i);
    GroupByStatementContext *groupByStatement();
    HavingStatementContext *havingStatement();
    OrderByStatementContext *orderByStatement();
    std::vector<UpdateStatementContext *> updateStatement();
    UpdateStatementContext* updateStatement(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SimpleQueryContext* simpleQuery();

  class  PrimitiveStatementContext : public antlr4::ParserRuleContext {
  public:
    PrimitiveStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    MatchStatementContext *matchStatement();
    CallStatementContext *callStatement();
    WhereStatementContext *whereStatement();
    LetStatementContext *letStatement();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PrimitiveStatementContext* primitiveStatement();

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
    FixedObjContext *fixedObj();
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
    std::vector<LetDefinitionContext *> letDefinition();
    LetDefinitionContext* letDefinition(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  LetStatementContext* letStatement();

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

  class  HavingStatementContext : public antlr4::ParserRuleContext {
  public:
    HavingStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *K_HAVING();
    ConditionalOrExprContext *conditionalOrExpr();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  HavingStatementContext* havingStatement();

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
    virtual size_t getRuleIndex() const override;
    ConditionalOrExprContext *conditionalOrExpr();
    AliasContext *alias();


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
    virtual size_t getRuleIndex() const override;
    ConditionalOrExprContext *conditionalOrExpr();
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
    antlr4::tree::TerminalNode *L_PAR();
    antlr4::tree::TerminalNode *R_PAR();
    FixedObjContext *fixedObj();
    VarNodeContext *varNode();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  NodeContext* node();

  class  FixedObjContext : public antlr4::ParserRuleContext {
  public:
    FixedObjContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdentifierContext *identifier();
    antlr4::tree::TerminalNode *ANON_ID();
    antlr4::tree::TerminalNode *EDGE_ID();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FixedObjContext* fixedObj();

  class  VarNodeContext : public antlr4::ParserRuleContext {
  public:
    VarNodeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *VARIABLE();
    std::vector<antlr4::tree::TerminalNode *> TYPE();
    antlr4::tree::TerminalNode* TYPE(size_t i);
    PropertiesContext *properties();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  VarNodeContext* varNode();

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
    antlr4::Token *s102 = nullptr;
    std::vector<antlr4::Token *> op;
    antlr4::Token *s103 = nullptr;
    antlr4::Token *_tset1470 = nullptr;
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
    antlr4::Token *s99 = nullptr;
    std::vector<antlr4::Token *> op;
    antlr4::Token *s96 = nullptr;
    antlr4::Token *s100 = nullptr;
    antlr4::Token *_tset1493 = nullptr;
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

  class  ExprFixedObjContext : public AtomicExprContext {
  public:
    ExprFixedObjContext(AtomicExprContext *ctx);

    FixedObjContext *fixedObj();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
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

  class  ExprAggContext : public AtomicExprContext {
  public:
    ExprAggContext(AtomicExprContext *ctx);

    AggregateFuncContext *aggregateFunc();
    antlr4::tree::TerminalNode *L_PAR();
    antlr4::tree::TerminalNode *VARIABLE();
    antlr4::tree::TerminalNode *R_PAR();
    antlr4::tree::TerminalNode *KEY();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  ExprVarContext : public AtomicExprContext {
  public:
    ExprVarContext(AtomicExprContext *ctx);

    antlr4::tree::TerminalNode *VARIABLE();
    antlr4::tree::TerminalNode *KEY();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  ExprCountContext : public AtomicExprContext {
  public:
    ExprCountContext(AtomicExprContext *ctx);

    antlr4::tree::TerminalNode *K_COUNT();
    antlr4::tree::TerminalNode *L_PAR();
    antlr4::tree::TerminalNode *R_PAR();
    antlr4::tree::TerminalNode *VARIABLE();
    antlr4::tree::TerminalNode *STAR();
    antlr4::tree::TerminalNode *K_DISTINCT();
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
    CosineSimilarityContext *cosineSimilarity();
    CosineDistanceContext *cosineDistance();
    ManhattanDistanceContext *manhattanDistance();
    EuclideanDistanceContext *euclideanDistance();
    EditDistanceContext *editDistance();
    NormalizeContext *normalize();
    StrContext *str();
    LabelsContext *labels();
    TypeContext *type();
    PropertiesFunctionContext *propertiesFunction();


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

  class  EditDistanceContext : public antlr4::ParserRuleContext {
  public:
    EditDistanceContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *K_EDIT_DISTANCE();
    antlr4::tree::TerminalNode *L_PAR();
    std::vector<ConditionalOrExprContext *> conditionalOrExpr();
    ConditionalOrExprContext* conditionalOrExpr(size_t i);
    antlr4::tree::TerminalNode *COMMA();
    antlr4::tree::TerminalNode *R_PAR();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  EditDistanceContext* editDistance();

  class  NormalizeContext : public antlr4::ParserRuleContext {
  public:
    NormalizeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *K_NORMALIZE();
    antlr4::tree::TerminalNode *L_PAR();
    ConditionalOrExprContext *conditionalOrExpr();
    antlr4::tree::TerminalNode *R_PAR();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  NormalizeContext* normalize();

  class  StrContext : public antlr4::ParserRuleContext {
  public:
    StrContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *K_STR();
    antlr4::tree::TerminalNode *L_PAR();
    ConditionalOrExprContext *conditionalOrExpr();
    antlr4::tree::TerminalNode *R_PAR();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  StrContext* str();

  class  LabelsContext : public antlr4::ParserRuleContext {
  public:
    LabelsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *K_LABELS();
    antlr4::tree::TerminalNode *L_PAR();
    antlr4::tree::TerminalNode *VARIABLE();
    antlr4::tree::TerminalNode *R_PAR();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  LabelsContext* labels();

  class  PropertiesFunctionContext : public antlr4::ParserRuleContext {
  public:
    PropertiesFunctionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *K_PROPERTIES();
    antlr4::tree::TerminalNode *L_PAR();
    antlr4::tree::TerminalNode *VARIABLE();
    antlr4::tree::TerminalNode *R_PAR();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PropertiesFunctionContext* propertiesFunction();

  class  TypeContext : public antlr4::ParserRuleContext {
  public:
    TypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *K_TYPE();
    antlr4::tree::TerminalNode *L_PAR();
    antlr4::tree::TerminalNode *VARIABLE();
    antlr4::tree::TerminalNode *R_PAR();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TypeContext* type();

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
    antlr4::tree::TerminalNode *K_EDIT_DISTANCE();
    antlr4::tree::TerminalNode *K_EUCLIDEAN_DISTANCE();
    antlr4::tree::TerminalNode *K_FROM();
    antlr4::tree::TerminalNode *K_HAVING();
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
    antlr4::tree::TerminalNode *K_NORMALIZE();
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
    antlr4::tree::TerminalNode *K_STRING();
    antlr4::tree::TerminalNode *K_STR();
    antlr4::tree::TerminalNode *K_TEXT_SEARCH();
    antlr4::tree::TerminalNode *K_TYPE();
    antlr4::tree::TerminalNode *K_REGEX();
    antlr4::tree::TerminalNode *K_RETURN();
    antlr4::tree::TerminalNode *K_SUM();
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


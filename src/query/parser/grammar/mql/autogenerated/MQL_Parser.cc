
// Generated from MQL_Parser.g4 by ANTLR 4.13.1


#include "MQL_ParserVisitor.h"

#include "MQL_Parser.h"


using namespace antlrcpp;

using namespace antlr4;

namespace {

struct MQL_ParserStaticData final {
  MQL_ParserStaticData(std::vector<std::string> ruleNames,
                        std::vector<std::string> literalNames,
                        std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  MQL_ParserStaticData(const MQL_ParserStaticData&) = delete;
  MQL_ParserStaticData(MQL_ParserStaticData&&) = delete;
  MQL_ParserStaticData& operator=(const MQL_ParserStaticData&) = delete;
  MQL_ParserStaticData& operator=(MQL_ParserStaticData&&) = delete;

  std::vector<antlr4::dfa::DFA> decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string> ruleNames;
  const std::vector<std::string> literalNames;
  const std::vector<std::string> symbolicNames;
  const antlr4::dfa::Vocabulary vocabulary;
  antlr4::atn::SerializedATNView serializedATN;
  std::unique_ptr<antlr4::atn::ATN> atn;
};

::antlr4::internal::OnceFlag mql_parserParserOnceFlag;
#if ANTLR4_USE_THREAD_LOCAL_CACHE
static thread_local
#endif
MQL_ParserStaticData *mql_parserParserStaticData = nullptr;

void mql_parserParserInitialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  if (mql_parserParserStaticData != nullptr) {
    return;
  }
#else
  assert(mql_parserParserStaticData == nullptr);
#endif
  auto staticData = std::make_unique<MQL_ParserStaticData>(
    std::vector<std::string>{
      "root", "matchQuery", "primitiveStatementList", "primitiveStatement", 
      "insertPatterns", "insertLinearPattern", "insertPlainNode", "insertPlainNodeInside", 
      "insertPlainEdge", "createIndexQuery", "createIndexOptions", "createIndexOption", 
      "showQuery", "describeQuery", "describeFlag", "matchStatement", "letStatement", 
      "letDefinitionList", "letDefinition", "whereStatement", "groupByStatement", 
      "orderByStatement", "returnStatement", "callStatement", "yieldStatement", 
      "yieldItem", "callArguments", "limitOffsetClauses", "limitClause", 
      "offsetClause", "returnItem", "alias", "aggregateFunc", "orderByItem", 
      "groupByItem", "graphPattern", "optionalPattern", "basicPattern", 
      "linearPattern", "path", "pathAlternatives", "pathSequence", "pathAtom", 
      "pathSuffix", "pathType", "node", "fixedNode", "fixedNodeInside", 
      "edge", "edgeInside", "varNode", "properties", "property", "conditionalOrType", 
      "identifier", "boolValue", "numericValue", "datatypeValue", "value", 
      "conditionalOrExpr", "conditionalAndExpr", "comparisonExpr", "additiveExpr", 
      "multiplicativeExpr", "unaryExpr", "atomicExpr", "function", "regex", 
      "cosineSimilarity", "cosineDistance", "manhattanDistance", "euclideanDistance", 
      "editDistance", "normalize", "textSearchIndexMode", "exprTypename", 
      "keyword"
    },
    std::vector<std::string>{
      "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
      "", "", "", "", "", "", "", "", "", "", "", "", "", "'false'", "", 
      "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
      "", "", "", "", "", "", "", "", "", "", "", "", "", "", "'true'", 
      "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
      "", "", "'<='", "'>='", "'=='", "'!='", "'<'", "'>'", "'='", "'/'", 
      "'|'", "'^'", "'*'", "'%'", "'\\u003F'", "'+'", "'-'", "'('", "')'", 
      "'{'", "'}'", "'['", "']'", "','", "':'"
    },
    std::vector<std::string>{
      "", "K_ACYCLIC", "K_AND", "K_ANY", "K_AS", "K_AVG", "K_ALL", "K_ASC", 
      "K_BY", "K_BOOL", "K_CALL", "K_COSINE_DISTANCE", "K_COSINE_SIMILARITY", 
      "K_COUNT", "K_CREATE", "K_DELETE", "K_DESCRIBE", "K_DESC", "K_DIMENSIONS", 
      "K_DISTINCT", "K_EDGE", "K_EDIT_DISTANCE", "K_EUCLIDEAN_DISTANCE", 
      "K_FROM", "K_INCOMING", "K_INDEX", "K_INSERT", "K_INTEGER", "K_INTO", 
      "K_IS", "K_FALSE", "K_FLOAT", "K_GROUP", "K_GROUPS", "K_IDENTITY", 
      "K_LABELS", "K_LABEL", "K_LET", "K_LIMIT", "K_MANHATTAN_DISTANCE", 
      "K_MATCH", "K_MAX", "K_MIN", "K_NORMALIZE", "K_OBJECTS", "K_OFFSET", 
      "K_OPTIONAL", "K_ORDER", "K_OR", "K_OUTGOING", "K_PREFIX", "K_PROPERTIES", 
      "K_PROPERTY", "K_NOT", "K_NULL", "K_SHORTEST", "K_SHOW", "K_SIMPLE", 
      "K_TEXT_SEARCH", "K_REGEX", "K_RETURN", "K_SUM", "K_STRING", "K_TRUE", 
      "K_TRAILS", "K_VALUES", "K_WALKS", "K_WITH", "K_WHERE", "K_YIELD", 
      "TRUE_PROP", "FALSE_PROP", "ANON_ID", "EDGE_ID", "KEY", "TYPE", "TYPE_VAR", 
      "VARIABLE", "STRING", "UNSIGNED_INTEGER", "UNSIGNED_FLOAT", "UNSIGNED_SCIENTIFIC_NOTATION", 
      "NAME", "LEQ", "GEQ", "EQ", "NEQ", "LT", "GT", "SINGLE_EQ", "PATH_SEQUENCE", 
      "PATH_ALTERNATIVE", "PATH_NEGATION", "STAR", "PERCENT", "QUESTION_MARK", 
      "PLUS", "MINUS", "L_PAR", "R_PAR", "LCURLY_BRACKET", "RCURLY_BRACKET", 
      "LSQUARE_BRACKET", "RSQUARE_BRACKET", "COMMA", "COLON", "WHITE_SPACE", 
      "SINGLE_LINE_COMMENT", "UNRECOGNIZED"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,1,108,861,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,6,2,
  	7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,2,14,7,
  	14,2,15,7,15,2,16,7,16,2,17,7,17,2,18,7,18,2,19,7,19,2,20,7,20,2,21,7,
  	21,2,22,7,22,2,23,7,23,2,24,7,24,2,25,7,25,2,26,7,26,2,27,7,27,2,28,7,
  	28,2,29,7,29,2,30,7,30,2,31,7,31,2,32,7,32,2,33,7,33,2,34,7,34,2,35,7,
  	35,2,36,7,36,2,37,7,37,2,38,7,38,2,39,7,39,2,40,7,40,2,41,7,41,2,42,7,
  	42,2,43,7,43,2,44,7,44,2,45,7,45,2,46,7,46,2,47,7,47,2,48,7,48,2,49,7,
  	49,2,50,7,50,2,51,7,51,2,52,7,52,2,53,7,53,2,54,7,54,2,55,7,55,2,56,7,
  	56,2,57,7,57,2,58,7,58,2,59,7,59,2,60,7,60,2,61,7,61,2,62,7,62,2,63,7,
  	63,2,64,7,64,2,65,7,65,2,66,7,66,2,67,7,67,2,68,7,68,2,69,7,69,2,70,7,
  	70,2,71,7,71,2,72,7,72,2,73,7,73,2,74,7,74,2,75,7,75,2,76,7,76,1,0,1,
  	0,1,0,1,0,1,0,3,0,160,8,0,1,0,1,0,1,1,1,1,3,1,166,8,1,1,1,3,1,169,8,1,
  	1,1,3,1,172,8,1,1,1,1,1,1,2,4,2,177,8,2,11,2,12,2,178,1,3,1,3,1,3,3,3,
  	184,8,3,1,4,1,4,1,4,1,4,5,4,190,8,4,10,4,12,4,193,9,4,1,5,1,5,1,5,1,5,
  	5,5,199,8,5,10,5,12,5,202,9,5,1,6,1,6,3,6,206,8,6,1,6,5,6,209,8,6,10,
  	6,12,6,212,9,6,1,6,3,6,215,8,6,1,6,1,6,1,7,1,7,1,8,1,8,1,8,1,8,1,8,3,
  	8,226,8,8,1,8,1,8,1,8,1,8,1,8,1,8,3,8,234,8,8,1,8,1,8,1,8,3,8,239,8,8,
  	1,9,1,9,1,9,1,9,1,9,1,9,1,9,1,10,1,10,1,10,1,10,5,10,252,8,10,10,10,12,
  	10,255,9,10,3,10,257,8,10,1,10,1,10,1,11,1,11,1,11,1,11,3,11,265,8,11,
  	1,12,1,12,1,12,1,12,1,12,3,12,272,8,12,1,13,1,13,5,13,276,8,13,10,13,
  	12,13,279,9,13,1,13,1,13,1,14,1,14,1,14,3,14,286,8,14,1,15,1,15,1,15,
  	1,16,1,16,1,16,1,17,1,17,1,17,5,17,297,8,17,10,17,12,17,300,9,17,1,18,
  	1,18,1,18,1,18,1,19,1,19,1,19,1,20,1,20,1,20,1,20,1,20,5,20,314,8,20,
  	10,20,12,20,317,9,20,1,21,1,21,1,21,1,21,1,21,5,21,324,8,21,10,21,12,
  	21,327,9,21,1,22,1,22,3,22,331,8,22,1,22,1,22,1,22,5,22,336,8,22,10,22,
  	12,22,339,9,22,1,22,3,22,342,8,22,1,22,1,22,3,22,346,8,22,1,22,1,22,3,
  	22,350,8,22,3,22,352,8,22,1,23,1,23,1,23,1,23,3,23,358,8,23,1,23,1,23,
  	3,23,362,8,23,1,24,1,24,1,24,1,24,1,24,5,24,369,8,24,10,24,12,24,372,
  	9,24,3,24,374,8,24,1,25,1,25,3,25,378,8,25,1,26,1,26,1,26,5,26,383,8,
  	26,10,26,12,26,386,9,26,1,27,1,27,3,27,390,8,27,1,27,1,27,3,27,394,8,
  	27,3,27,396,8,27,1,28,1,28,1,28,1,29,1,29,1,29,1,30,1,30,3,30,406,8,30,
  	1,30,1,30,1,30,1,30,3,30,412,8,30,1,30,1,30,3,30,416,8,30,1,30,1,30,1,
  	30,3,30,421,8,30,1,30,1,30,3,30,425,8,30,1,30,3,30,428,8,30,1,30,1,30,
  	3,30,432,8,30,1,30,1,30,1,30,3,30,437,8,30,1,31,1,31,1,31,1,32,1,32,1,
  	33,1,33,3,33,446,8,33,1,33,3,33,449,8,33,1,33,1,33,1,33,1,33,3,33,455,
  	8,33,1,33,1,33,3,33,459,8,33,1,33,1,33,1,33,3,33,464,8,33,1,33,1,33,3,
  	33,468,8,33,1,33,1,33,3,33,472,8,33,1,33,1,33,3,33,476,8,33,3,33,478,
  	8,33,1,34,1,34,3,34,482,8,34,1,35,1,35,5,35,486,8,35,10,35,12,35,489,
  	9,35,1,36,1,36,1,36,1,36,1,36,1,37,1,37,1,37,5,37,499,8,37,10,37,12,37,
  	502,9,37,1,38,1,38,1,38,3,38,507,8,38,1,38,1,38,5,38,511,8,38,10,38,12,
  	38,514,9,38,1,39,1,39,1,39,3,39,519,8,39,1,39,3,39,522,8,39,1,39,1,39,
  	1,39,1,39,1,39,1,39,1,39,3,39,531,8,39,1,39,3,39,534,8,39,1,39,1,39,1,
  	39,1,39,1,39,3,39,541,8,39,1,40,1,40,1,40,5,40,546,8,40,10,40,12,40,549,
  	9,40,1,41,1,41,1,41,5,41,554,8,41,10,41,12,41,557,9,41,1,42,3,42,560,
  	8,42,1,42,1,42,3,42,564,8,42,1,42,3,42,567,8,42,1,42,1,42,1,42,1,42,3,
  	42,573,8,42,3,42,575,8,42,1,43,1,43,1,43,1,43,1,43,1,43,1,43,1,43,3,43,
  	585,8,43,1,44,1,44,3,44,589,8,44,1,44,3,44,592,8,44,1,44,1,44,1,44,3,
  	44,597,8,44,1,44,3,44,600,8,44,3,44,602,8,44,1,45,1,45,3,45,606,8,45,
  	1,46,1,46,1,46,1,46,1,47,1,47,1,47,1,47,3,47,616,8,47,1,48,1,48,1,48,
  	3,48,621,8,48,1,48,1,48,1,48,1,48,1,48,3,48,628,8,48,1,48,3,48,631,8,
  	48,1,49,1,49,3,49,635,8,49,1,49,3,49,638,8,49,1,49,3,49,641,8,49,1,49,
  	1,49,1,50,1,50,3,50,647,8,50,1,50,5,50,650,8,50,10,50,12,50,653,9,50,
  	1,50,3,50,656,8,50,1,50,1,50,1,51,1,51,1,51,1,51,5,51,664,8,51,10,51,
  	12,51,667,9,51,1,51,1,51,1,52,1,52,1,52,1,52,1,52,3,52,676,8,52,1,52,
  	1,52,1,52,1,52,1,52,1,52,1,52,1,52,1,52,3,52,687,8,52,1,52,1,52,5,52,
  	691,8,52,10,52,12,52,694,9,52,1,52,1,52,1,52,1,52,3,52,700,8,52,1,53,
  	1,53,1,53,1,54,1,54,3,54,707,8,54,1,55,1,55,1,56,3,56,712,8,56,1,56,1,
  	56,1,57,1,57,1,57,1,57,1,57,1,58,1,58,1,58,1,58,3,58,725,8,58,1,59,1,
  	59,1,59,5,59,730,8,59,10,59,12,59,733,9,59,1,60,1,60,1,60,5,60,738,8,
  	60,10,60,12,60,741,9,60,1,61,1,61,1,61,3,61,746,8,61,1,61,1,61,1,61,3,
  	61,751,8,61,1,61,1,61,3,61,755,8,61,1,62,1,62,1,62,5,62,760,8,62,10,62,
  	12,62,763,9,62,1,63,1,63,1,63,5,63,768,8,63,10,63,12,63,771,9,63,1,64,
  	1,64,1,64,1,64,1,64,1,64,1,64,3,64,780,8,64,1,65,1,65,3,65,784,8,65,1,
  	65,1,65,1,65,1,65,1,65,1,65,1,65,3,65,793,8,65,1,66,1,66,1,66,1,66,1,
  	66,1,66,1,66,3,66,802,8,66,1,67,1,67,1,67,1,67,1,67,1,67,1,67,3,67,811,
  	8,67,1,67,1,67,1,68,1,68,1,68,1,68,1,68,1,68,1,68,1,69,1,69,1,69,1,69,
  	1,69,1,69,1,69,1,70,1,70,1,70,1,70,1,70,1,70,1,70,1,71,1,71,1,71,1,71,
  	1,71,1,71,1,71,1,72,1,72,1,72,1,72,1,72,1,72,1,72,1,73,1,73,1,73,1,73,
  	1,73,1,74,1,74,1,75,1,75,1,76,1,76,1,76,0,0,77,0,2,4,6,8,10,12,14,16,
  	18,20,22,24,26,28,30,32,34,36,38,40,42,44,46,48,50,52,54,56,58,60,62,
  	64,66,68,70,72,74,76,78,80,82,84,86,88,90,92,94,96,98,100,102,104,106,
  	108,110,112,114,116,118,120,122,124,126,128,130,132,134,136,138,140,142,
  	144,146,148,150,152,0,15,4,0,24,24,35,35,49,49,51,51,3,0,5,5,41,42,61,
  	61,2,0,7,7,17,17,2,0,3,3,6,6,4,0,1,1,57,57,64,64,66,66,2,0,73,73,77,77,
  	1,0,75,76,1,0,83,88,2,0,30,30,63,63,1,0,96,97,1,0,79,81,2,0,90,90,93,
  	94,2,0,40,40,50,50,5,0,9,9,27,27,31,31,54,54,62,62,4,0,1,20,22,29,31,
  	62,64,69,919,0,159,1,0,0,0,2,163,1,0,0,0,4,176,1,0,0,0,6,183,1,0,0,0,
  	8,185,1,0,0,0,10,194,1,0,0,0,12,203,1,0,0,0,14,218,1,0,0,0,16,238,1,0,
  	0,0,18,240,1,0,0,0,20,247,1,0,0,0,22,260,1,0,0,0,24,266,1,0,0,0,26,273,
  	1,0,0,0,28,282,1,0,0,0,30,287,1,0,0,0,32,290,1,0,0,0,34,293,1,0,0,0,36,
  	301,1,0,0,0,38,305,1,0,0,0,40,308,1,0,0,0,42,318,1,0,0,0,44,351,1,0,0,
  	0,46,353,1,0,0,0,48,363,1,0,0,0,50,375,1,0,0,0,52,379,1,0,0,0,54,395,
  	1,0,0,0,56,397,1,0,0,0,58,400,1,0,0,0,60,436,1,0,0,0,62,438,1,0,0,0,64,
  	441,1,0,0,0,66,477,1,0,0,0,68,479,1,0,0,0,70,483,1,0,0,0,72,490,1,0,0,
  	0,74,495,1,0,0,0,76,503,1,0,0,0,78,540,1,0,0,0,80,542,1,0,0,0,82,550,
  	1,0,0,0,84,574,1,0,0,0,86,584,1,0,0,0,88,601,1,0,0,0,90,605,1,0,0,0,92,
  	607,1,0,0,0,94,615,1,0,0,0,96,630,1,0,0,0,98,632,1,0,0,0,100,644,1,0,
  	0,0,102,659,1,0,0,0,104,699,1,0,0,0,106,701,1,0,0,0,108,706,1,0,0,0,110,
  	708,1,0,0,0,112,711,1,0,0,0,114,715,1,0,0,0,116,724,1,0,0,0,118,726,1,
  	0,0,0,120,734,1,0,0,0,122,754,1,0,0,0,124,756,1,0,0,0,126,764,1,0,0,0,
  	128,779,1,0,0,0,130,792,1,0,0,0,132,801,1,0,0,0,134,803,1,0,0,0,136,814,
  	1,0,0,0,138,821,1,0,0,0,140,828,1,0,0,0,142,835,1,0,0,0,144,842,1,0,0,
  	0,146,849,1,0,0,0,148,854,1,0,0,0,150,856,1,0,0,0,152,858,1,0,0,0,154,
  	160,3,2,1,0,155,160,3,26,13,0,156,160,3,8,4,0,157,160,3,18,9,0,158,160,
  	3,24,12,0,159,154,1,0,0,0,159,155,1,0,0,0,159,156,1,0,0,0,159,157,1,0,
  	0,0,159,158,1,0,0,0,160,161,1,0,0,0,161,162,5,0,0,1,162,1,1,0,0,0,163,
  	165,3,4,2,0,164,166,3,38,19,0,165,164,1,0,0,0,165,166,1,0,0,0,166,168,
  	1,0,0,0,167,169,3,40,20,0,168,167,1,0,0,0,168,169,1,0,0,0,169,171,1,0,
  	0,0,170,172,3,42,21,0,171,170,1,0,0,0,171,172,1,0,0,0,172,173,1,0,0,0,
  	173,174,3,44,22,0,174,3,1,0,0,0,175,177,3,6,3,0,176,175,1,0,0,0,177,178,
  	1,0,0,0,178,176,1,0,0,0,178,179,1,0,0,0,179,5,1,0,0,0,180,184,3,30,15,
  	0,181,184,3,46,23,0,182,184,3,32,16,0,183,180,1,0,0,0,183,181,1,0,0,0,
  	183,182,1,0,0,0,184,7,1,0,0,0,185,186,5,26,0,0,186,191,3,10,5,0,187,188,
  	5,104,0,0,188,190,3,10,5,0,189,187,1,0,0,0,190,193,1,0,0,0,191,189,1,
  	0,0,0,191,192,1,0,0,0,192,9,1,0,0,0,193,191,1,0,0,0,194,200,3,12,6,0,
  	195,196,3,16,8,0,196,197,3,12,6,0,197,199,1,0,0,0,198,195,1,0,0,0,199,
  	202,1,0,0,0,200,198,1,0,0,0,200,201,1,0,0,0,201,11,1,0,0,0,202,200,1,
  	0,0,0,203,205,5,98,0,0,204,206,3,14,7,0,205,204,1,0,0,0,205,206,1,0,0,
  	0,206,210,1,0,0,0,207,209,5,75,0,0,208,207,1,0,0,0,209,212,1,0,0,0,210,
  	208,1,0,0,0,210,211,1,0,0,0,211,214,1,0,0,0,212,210,1,0,0,0,213,215,3,
  	102,51,0,214,213,1,0,0,0,214,215,1,0,0,0,215,216,1,0,0,0,216,217,5,99,
  	0,0,217,13,1,0,0,0,218,219,3,108,54,0,219,15,1,0,0,0,220,221,5,87,0,0,
  	221,222,5,97,0,0,222,223,5,102,0,0,223,225,5,75,0,0,224,226,3,102,51,
  	0,225,224,1,0,0,0,225,226,1,0,0,0,226,227,1,0,0,0,227,228,5,103,0,0,228,
  	239,5,97,0,0,229,230,5,97,0,0,230,231,5,102,0,0,231,233,5,75,0,0,232,
  	234,3,102,51,0,233,232,1,0,0,0,233,234,1,0,0,0,234,235,1,0,0,0,235,236,
  	5,103,0,0,236,237,5,97,0,0,237,239,5,88,0,0,238,220,1,0,0,0,238,229,1,
  	0,0,0,239,17,1,0,0,0,240,241,5,14,0,0,241,242,3,108,54,0,242,243,5,25,
  	0,0,243,244,5,78,0,0,244,245,5,67,0,0,245,246,3,20,10,0,246,19,1,0,0,
  	0,247,256,5,100,0,0,248,253,3,22,11,0,249,250,5,104,0,0,250,252,3,22,
  	11,0,251,249,1,0,0,0,252,255,1,0,0,0,253,251,1,0,0,0,253,254,1,0,0,0,
  	254,257,1,0,0,0,255,253,1,0,0,0,256,248,1,0,0,0,256,257,1,0,0,0,257,258,
  	1,0,0,0,258,259,5,101,0,0,259,21,1,0,0,0,260,264,5,78,0,0,261,262,5,89,
  	0,0,262,265,3,116,58,0,263,265,3,110,55,0,264,261,1,0,0,0,264,263,1,0,
  	0,0,265,23,1,0,0,0,266,271,5,56,0,0,267,272,1,0,0,0,268,269,3,108,54,
  	0,269,270,5,25,0,0,270,272,1,0,0,0,271,267,1,0,0,0,271,268,1,0,0,0,272,
  	25,1,0,0,0,273,277,5,16,0,0,274,276,3,28,14,0,275,274,1,0,0,0,276,279,
  	1,0,0,0,277,275,1,0,0,0,277,278,1,0,0,0,278,280,1,0,0,0,279,277,1,0,0,
  	0,280,281,3,94,47,0,281,27,1,0,0,0,282,285,7,0,0,0,283,284,5,38,0,0,284,
  	286,5,79,0,0,285,283,1,0,0,0,285,286,1,0,0,0,286,29,1,0,0,0,287,288,5,
  	40,0,0,288,289,3,70,35,0,289,31,1,0,0,0,290,291,5,37,0,0,291,292,3,34,
  	17,0,292,33,1,0,0,0,293,298,3,36,18,0,294,295,5,104,0,0,295,297,3,36,
  	18,0,296,294,1,0,0,0,297,300,1,0,0,0,298,296,1,0,0,0,298,299,1,0,0,0,
  	299,35,1,0,0,0,300,298,1,0,0,0,301,302,5,77,0,0,302,303,5,89,0,0,303,
  	304,3,118,59,0,304,37,1,0,0,0,305,306,5,68,0,0,306,307,3,118,59,0,307,
  	39,1,0,0,0,308,309,5,32,0,0,309,310,5,8,0,0,310,315,3,68,34,0,311,312,
  	5,104,0,0,312,314,3,68,34,0,313,311,1,0,0,0,314,317,1,0,0,0,315,313,1,
  	0,0,0,315,316,1,0,0,0,316,41,1,0,0,0,317,315,1,0,0,0,318,319,5,47,0,0,
  	319,320,5,8,0,0,320,325,3,66,33,0,321,322,5,104,0,0,322,324,3,66,33,0,
  	323,321,1,0,0,0,324,327,1,0,0,0,325,323,1,0,0,0,325,326,1,0,0,0,326,43,
  	1,0,0,0,327,325,1,0,0,0,328,330,5,60,0,0,329,331,5,19,0,0,330,329,1,0,
  	0,0,330,331,1,0,0,0,331,332,1,0,0,0,332,337,3,60,30,0,333,334,5,104,0,
  	0,334,336,3,60,30,0,335,333,1,0,0,0,336,339,1,0,0,0,337,335,1,0,0,0,337,
  	338,1,0,0,0,338,341,1,0,0,0,339,337,1,0,0,0,340,342,3,54,27,0,341,340,
  	1,0,0,0,341,342,1,0,0,0,342,352,1,0,0,0,343,345,5,60,0,0,344,346,5,19,
  	0,0,345,344,1,0,0,0,345,346,1,0,0,0,346,347,1,0,0,0,347,349,5,93,0,0,
  	348,350,3,54,27,0,349,348,1,0,0,0,349,350,1,0,0,0,350,352,1,0,0,0,351,
  	328,1,0,0,0,351,343,1,0,0,0,352,45,1,0,0,0,353,354,5,10,0,0,354,355,3,
  	108,54,0,355,357,5,98,0,0,356,358,3,52,26,0,357,356,1,0,0,0,357,358,1,
  	0,0,0,358,359,1,0,0,0,359,361,5,99,0,0,360,362,3,48,24,0,361,360,1,0,
  	0,0,361,362,1,0,0,0,362,47,1,0,0,0,363,373,5,69,0,0,364,374,5,93,0,0,
  	365,370,3,50,25,0,366,367,5,104,0,0,367,369,3,50,25,0,368,366,1,0,0,0,
  	369,372,1,0,0,0,370,368,1,0,0,0,370,371,1,0,0,0,371,374,1,0,0,0,372,370,
  	1,0,0,0,373,364,1,0,0,0,373,365,1,0,0,0,374,49,1,0,0,0,375,377,5,77,0,
  	0,376,378,3,62,31,0,377,376,1,0,0,0,377,378,1,0,0,0,378,51,1,0,0,0,379,
  	384,3,118,59,0,380,381,5,104,0,0,381,383,3,118,59,0,382,380,1,0,0,0,383,
  	386,1,0,0,0,384,382,1,0,0,0,384,385,1,0,0,0,385,53,1,0,0,0,386,384,1,
  	0,0,0,387,389,3,56,28,0,388,390,3,58,29,0,389,388,1,0,0,0,389,390,1,0,
  	0,0,390,396,1,0,0,0,391,393,3,58,29,0,392,394,3,56,28,0,393,392,1,0,0,
  	0,393,394,1,0,0,0,394,396,1,0,0,0,395,387,1,0,0,0,395,391,1,0,0,0,396,
  	55,1,0,0,0,397,398,5,38,0,0,398,399,5,79,0,0,399,57,1,0,0,0,400,401,5,
  	45,0,0,401,402,5,79,0,0,402,59,1,0,0,0,403,405,5,77,0,0,404,406,5,74,
  	0,0,405,404,1,0,0,0,405,406,1,0,0,0,406,437,1,0,0,0,407,408,3,64,32,0,
  	408,409,5,98,0,0,409,411,5,77,0,0,410,412,5,74,0,0,411,410,1,0,0,0,411,
  	412,1,0,0,0,412,413,1,0,0,0,413,415,5,99,0,0,414,416,3,62,31,0,415,414,
  	1,0,0,0,415,416,1,0,0,0,416,437,1,0,0,0,417,418,5,13,0,0,418,420,5,98,
  	0,0,419,421,5,19,0,0,420,419,1,0,0,0,420,421,1,0,0,0,421,427,1,0,0,0,
  	422,424,5,77,0,0,423,425,5,74,0,0,424,423,1,0,0,0,424,425,1,0,0,0,425,
  	428,1,0,0,0,426,428,5,93,0,0,427,422,1,0,0,0,427,426,1,0,0,0,428,429,
  	1,0,0,0,429,431,5,99,0,0,430,432,3,62,31,0,431,430,1,0,0,0,431,432,1,
  	0,0,0,432,437,1,0,0,0,433,434,3,118,59,0,434,435,3,62,31,0,435,437,1,
  	0,0,0,436,403,1,0,0,0,436,407,1,0,0,0,436,417,1,0,0,0,436,433,1,0,0,0,
  	437,61,1,0,0,0,438,439,5,4,0,0,439,440,5,77,0,0,440,63,1,0,0,0,441,442,
  	7,1,0,0,442,65,1,0,0,0,443,445,5,77,0,0,444,446,5,74,0,0,445,444,1,0,
  	0,0,445,446,1,0,0,0,446,448,1,0,0,0,447,449,7,2,0,0,448,447,1,0,0,0,448,
  	449,1,0,0,0,449,478,1,0,0,0,450,451,3,64,32,0,451,452,5,98,0,0,452,454,
  	5,77,0,0,453,455,5,74,0,0,454,453,1,0,0,0,454,455,1,0,0,0,455,456,1,0,
  	0,0,456,458,5,99,0,0,457,459,7,2,0,0,458,457,1,0,0,0,458,459,1,0,0,0,
  	459,478,1,0,0,0,460,461,5,13,0,0,461,463,5,98,0,0,462,464,5,19,0,0,463,
  	462,1,0,0,0,463,464,1,0,0,0,464,465,1,0,0,0,465,467,5,77,0,0,466,468,
  	5,74,0,0,467,466,1,0,0,0,467,468,1,0,0,0,468,469,1,0,0,0,469,471,5,99,
  	0,0,470,472,7,2,0,0,471,470,1,0,0,0,471,472,1,0,0,0,472,478,1,0,0,0,473,
  	475,3,118,59,0,474,476,7,2,0,0,475,474,1,0,0,0,475,476,1,0,0,0,476,478,
  	1,0,0,0,477,443,1,0,0,0,477,450,1,0,0,0,477,460,1,0,0,0,477,473,1,0,0,
  	0,478,67,1,0,0,0,479,481,5,77,0,0,480,482,5,74,0,0,481,480,1,0,0,0,481,
  	482,1,0,0,0,482,69,1,0,0,0,483,487,3,74,37,0,484,486,3,72,36,0,485,484,
  	1,0,0,0,486,489,1,0,0,0,487,485,1,0,0,0,487,488,1,0,0,0,488,71,1,0,0,
  	0,489,487,1,0,0,0,490,491,5,46,0,0,491,492,5,100,0,0,492,493,3,70,35,
  	0,493,494,5,101,0,0,494,73,1,0,0,0,495,500,3,76,38,0,496,497,5,104,0,
  	0,497,499,3,76,38,0,498,496,1,0,0,0,499,502,1,0,0,0,500,498,1,0,0,0,500,
  	501,1,0,0,0,501,75,1,0,0,0,502,500,1,0,0,0,503,512,3,90,45,0,504,507,
  	3,96,48,0,505,507,3,78,39,0,506,504,1,0,0,0,506,505,1,0,0,0,507,508,1,
  	0,0,0,508,509,3,90,45,0,509,511,1,0,0,0,510,506,1,0,0,0,511,514,1,0,0,
  	0,512,510,1,0,0,0,512,513,1,0,0,0,513,77,1,0,0,0,514,512,1,0,0,0,515,
  	516,5,83,0,0,516,518,5,102,0,0,517,519,3,88,44,0,518,517,1,0,0,0,518,
  	519,1,0,0,0,519,521,1,0,0,0,520,522,5,77,0,0,521,520,1,0,0,0,521,522,
  	1,0,0,0,522,523,1,0,0,0,523,524,3,80,40,0,524,525,5,103,0,0,525,526,5,
  	89,0,0,526,541,1,0,0,0,527,528,5,89,0,0,528,530,5,102,0,0,529,531,3,88,
  	44,0,530,529,1,0,0,0,530,531,1,0,0,0,531,533,1,0,0,0,532,534,5,77,0,0,
  	533,532,1,0,0,0,533,534,1,0,0,0,534,535,1,0,0,0,535,536,3,80,40,0,536,
  	537,5,103,0,0,537,538,5,89,0,0,538,539,5,88,0,0,539,541,1,0,0,0,540,515,
  	1,0,0,0,540,527,1,0,0,0,541,79,1,0,0,0,542,547,3,82,41,0,543,544,5,91,
  	0,0,544,546,3,82,41,0,545,543,1,0,0,0,546,549,1,0,0,0,547,545,1,0,0,0,
  	547,548,1,0,0,0,548,81,1,0,0,0,549,547,1,0,0,0,550,555,3,84,42,0,551,
  	552,5,90,0,0,552,554,3,84,42,0,553,551,1,0,0,0,554,557,1,0,0,0,555,553,
  	1,0,0,0,555,556,1,0,0,0,556,83,1,0,0,0,557,555,1,0,0,0,558,560,5,92,0,
  	0,559,558,1,0,0,0,559,560,1,0,0,0,560,561,1,0,0,0,561,563,5,75,0,0,562,
  	564,3,86,43,0,563,562,1,0,0,0,563,564,1,0,0,0,564,575,1,0,0,0,565,567,
  	5,92,0,0,566,565,1,0,0,0,566,567,1,0,0,0,567,568,1,0,0,0,568,569,5,98,
  	0,0,569,570,3,80,40,0,570,572,5,99,0,0,571,573,3,86,43,0,572,571,1,0,
  	0,0,572,573,1,0,0,0,573,575,1,0,0,0,574,559,1,0,0,0,574,566,1,0,0,0,575,
  	85,1,0,0,0,576,585,5,93,0,0,577,585,5,96,0,0,578,585,5,95,0,0,579,580,
  	5,100,0,0,580,581,5,79,0,0,581,582,5,104,0,0,582,583,5,79,0,0,583,585,
  	5,101,0,0,584,576,1,0,0,0,584,577,1,0,0,0,584,578,1,0,0,0,584,579,1,0,
  	0,0,585,87,1,0,0,0,586,588,7,3,0,0,587,589,5,55,0,0,588,587,1,0,0,0,588,
  	589,1,0,0,0,589,591,1,0,0,0,590,592,7,4,0,0,591,590,1,0,0,0,591,592,1,
  	0,0,0,592,602,1,0,0,0,593,594,5,55,0,0,594,596,5,79,0,0,595,597,5,33,
  	0,0,596,595,1,0,0,0,596,597,1,0,0,0,597,599,1,0,0,0,598,600,7,4,0,0,599,
  	598,1,0,0,0,599,600,1,0,0,0,600,602,1,0,0,0,601,586,1,0,0,0,601,593,1,
  	0,0,0,602,89,1,0,0,0,603,606,3,92,46,0,604,606,3,100,50,0,605,603,1,0,
  	0,0,605,604,1,0,0,0,606,91,1,0,0,0,607,608,5,98,0,0,608,609,3,94,47,0,
  	609,610,5,99,0,0,610,93,1,0,0,0,611,616,3,108,54,0,612,616,5,72,0,0,613,
  	616,5,73,0,0,614,616,3,116,58,0,615,611,1,0,0,0,615,612,1,0,0,0,615,613,
  	1,0,0,0,615,614,1,0,0,0,616,95,1,0,0,0,617,620,5,87,0,0,618,619,5,97,
  	0,0,619,621,3,98,49,0,620,618,1,0,0,0,620,621,1,0,0,0,621,622,1,0,0,0,
  	622,631,5,97,0,0,623,627,5,97,0,0,624,625,3,98,49,0,625,626,5,97,0,0,
  	626,628,1,0,0,0,627,624,1,0,0,0,627,628,1,0,0,0,628,629,1,0,0,0,629,631,
  	5,88,0,0,630,617,1,0,0,0,630,623,1,0,0,0,631,97,1,0,0,0,632,634,5,102,
  	0,0,633,635,7,5,0,0,634,633,1,0,0,0,634,635,1,0,0,0,635,637,1,0,0,0,636,
  	638,7,6,0,0,637,636,1,0,0,0,637,638,1,0,0,0,638,640,1,0,0,0,639,641,3,
  	102,51,0,640,639,1,0,0,0,640,641,1,0,0,0,641,642,1,0,0,0,642,643,5,103,
  	0,0,643,99,1,0,0,0,644,646,5,98,0,0,645,647,5,77,0,0,646,645,1,0,0,0,
  	646,647,1,0,0,0,647,651,1,0,0,0,648,650,5,75,0,0,649,648,1,0,0,0,650,
  	653,1,0,0,0,651,649,1,0,0,0,651,652,1,0,0,0,652,655,1,0,0,0,653,651,1,
  	0,0,0,654,656,3,102,51,0,655,654,1,0,0,0,655,656,1,0,0,0,656,657,1,0,
  	0,0,657,658,5,99,0,0,658,101,1,0,0,0,659,660,5,100,0,0,660,665,3,104,
  	52,0,661,662,5,104,0,0,662,664,3,104,52,0,663,661,1,0,0,0,664,667,1,0,
  	0,0,665,663,1,0,0,0,665,666,1,0,0,0,666,668,1,0,0,0,667,665,1,0,0,0,668,
  	669,5,101,0,0,669,103,1,0,0,0,670,675,3,108,54,0,671,672,5,105,0,0,672,
  	676,3,116,58,0,673,676,5,70,0,0,674,676,5,71,0,0,675,671,1,0,0,0,675,
  	673,1,0,0,0,675,674,1,0,0,0,676,700,1,0,0,0,677,678,3,108,54,0,678,679,
  	5,75,0,0,679,680,5,98,0,0,680,681,5,78,0,0,681,682,5,99,0,0,682,700,1,
  	0,0,0,683,684,3,108,54,0,684,686,5,29,0,0,685,687,5,53,0,0,686,685,1,
  	0,0,0,686,687,1,0,0,0,687,688,1,0,0,0,688,692,3,150,75,0,689,691,3,106,
  	53,0,690,689,1,0,0,0,691,694,1,0,0,0,692,690,1,0,0,0,692,693,1,0,0,0,
  	693,700,1,0,0,0,694,692,1,0,0,0,695,696,3,108,54,0,696,697,7,7,0,0,697,
  	698,3,116,58,0,698,700,1,0,0,0,699,670,1,0,0,0,699,677,1,0,0,0,699,683,
  	1,0,0,0,699,695,1,0,0,0,700,105,1,0,0,0,701,702,5,48,0,0,702,703,3,150,
  	75,0,703,107,1,0,0,0,704,707,5,82,0,0,705,707,3,152,76,0,706,704,1,0,
  	0,0,706,705,1,0,0,0,707,109,1,0,0,0,708,709,7,8,0,0,709,111,1,0,0,0,710,
  	712,7,9,0,0,711,710,1,0,0,0,711,712,1,0,0,0,712,713,1,0,0,0,713,714,7,
  	10,0,0,714,113,1,0,0,0,715,716,3,108,54,0,716,717,5,98,0,0,717,718,5,
  	78,0,0,718,719,5,99,0,0,719,115,1,0,0,0,720,725,3,112,56,0,721,725,5,
  	78,0,0,722,725,3,110,55,0,723,725,3,114,57,0,724,720,1,0,0,0,724,721,
  	1,0,0,0,724,722,1,0,0,0,724,723,1,0,0,0,725,117,1,0,0,0,726,731,3,120,
  	60,0,727,728,5,48,0,0,728,730,3,120,60,0,729,727,1,0,0,0,730,733,1,0,
  	0,0,731,729,1,0,0,0,731,732,1,0,0,0,732,119,1,0,0,0,733,731,1,0,0,0,734,
  	739,3,122,61,0,735,736,5,2,0,0,736,738,3,122,61,0,737,735,1,0,0,0,738,
  	741,1,0,0,0,739,737,1,0,0,0,739,740,1,0,0,0,740,121,1,0,0,0,741,739,1,
  	0,0,0,742,745,3,124,62,0,743,744,7,7,0,0,744,746,3,124,62,0,745,743,1,
  	0,0,0,745,746,1,0,0,0,746,755,1,0,0,0,747,748,3,124,62,0,748,750,5,29,
  	0,0,749,751,5,53,0,0,750,749,1,0,0,0,750,751,1,0,0,0,751,752,1,0,0,0,
  	752,753,3,150,75,0,753,755,1,0,0,0,754,742,1,0,0,0,754,747,1,0,0,0,755,
  	123,1,0,0,0,756,761,3,126,63,0,757,758,7,9,0,0,758,760,3,126,63,0,759,
  	757,1,0,0,0,760,763,1,0,0,0,761,759,1,0,0,0,761,762,1,0,0,0,762,125,1,
  	0,0,0,763,761,1,0,0,0,764,769,3,128,64,0,765,766,7,11,0,0,766,768,3,128,
  	64,0,767,765,1,0,0,0,768,771,1,0,0,0,769,767,1,0,0,0,769,770,1,0,0,0,
  	770,127,1,0,0,0,771,769,1,0,0,0,772,773,5,53,0,0,773,780,3,128,64,0,774,
  	780,3,130,65,0,775,776,5,96,0,0,776,780,3,128,64,0,777,778,5,97,0,0,778,
  	780,3,128,64,0,779,772,1,0,0,0,779,774,1,0,0,0,779,775,1,0,0,0,779,777,
  	1,0,0,0,780,129,1,0,0,0,781,783,5,77,0,0,782,784,5,74,0,0,783,782,1,0,
  	0,0,783,784,1,0,0,0,784,793,1,0,0,0,785,793,3,132,66,0,786,793,3,116,
  	58,0,787,793,3,94,47,0,788,789,5,98,0,0,789,790,3,118,59,0,790,791,5,
  	99,0,0,791,793,1,0,0,0,792,781,1,0,0,0,792,785,1,0,0,0,792,786,1,0,0,
  	0,792,787,1,0,0,0,792,788,1,0,0,0,793,131,1,0,0,0,794,802,3,134,67,0,
  	795,802,3,136,68,0,796,802,3,138,69,0,797,802,3,140,70,0,798,802,3,142,
  	71,0,799,802,3,144,72,0,800,802,3,146,73,0,801,794,1,0,0,0,801,795,1,
  	0,0,0,801,796,1,0,0,0,801,797,1,0,0,0,801,798,1,0,0,0,801,799,1,0,0,0,
  	801,800,1,0,0,0,802,133,1,0,0,0,803,804,5,59,0,0,804,805,5,98,0,0,805,
  	806,3,118,59,0,806,807,5,104,0,0,807,810,3,118,59,0,808,809,5,104,0,0,
  	809,811,3,118,59,0,810,808,1,0,0,0,810,811,1,0,0,0,811,812,1,0,0,0,812,
  	813,5,99,0,0,813,135,1,0,0,0,814,815,5,12,0,0,815,816,5,98,0,0,816,817,
  	3,118,59,0,817,818,5,104,0,0,818,819,3,118,59,0,819,820,5,99,0,0,820,
  	137,1,0,0,0,821,822,5,11,0,0,822,823,5,98,0,0,823,824,3,118,59,0,824,
  	825,5,104,0,0,825,826,3,118,59,0,826,827,5,99,0,0,827,139,1,0,0,0,828,
  	829,5,39,0,0,829,830,5,98,0,0,830,831,3,118,59,0,831,832,5,104,0,0,832,
  	833,3,118,59,0,833,834,5,99,0,0,834,141,1,0,0,0,835,836,5,22,0,0,836,
  	837,5,98,0,0,837,838,3,118,59,0,838,839,5,104,0,0,839,840,3,118,59,0,
  	840,841,5,99,0,0,841,143,1,0,0,0,842,843,5,21,0,0,843,844,5,98,0,0,844,
  	845,3,118,59,0,845,846,5,104,0,0,846,847,3,118,59,0,847,848,5,99,0,0,
  	848,145,1,0,0,0,849,850,5,43,0,0,850,851,5,98,0,0,851,852,3,118,59,0,
  	852,853,5,99,0,0,853,147,1,0,0,0,854,855,7,12,0,0,855,149,1,0,0,0,856,
  	857,7,13,0,0,857,151,1,0,0,0,858,859,7,14,0,0,859,153,1,0,0,0,109,159,
  	165,168,171,178,183,191,200,205,210,214,225,233,238,253,256,264,271,277,
  	285,298,315,325,330,337,341,345,349,351,357,361,370,373,377,384,389,393,
  	395,405,411,415,420,424,427,431,436,445,448,454,458,463,467,471,475,477,
  	481,487,500,506,512,518,521,530,533,540,547,555,559,563,566,572,574,584,
  	588,591,596,599,601,605,615,620,627,630,634,637,640,646,651,655,665,675,
  	686,692,699,706,711,724,731,739,745,750,754,761,769,779,783,792,801,810
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  mql_parserParserStaticData = staticData.release();
}

}

MQL_Parser::MQL_Parser(TokenStream *input) : MQL_Parser(input, antlr4::atn::ParserATNSimulatorOptions()) {}

MQL_Parser::MQL_Parser(TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options) : Parser(input) {
  MQL_Parser::initialize();
  _interpreter = new atn::ParserATNSimulator(this, *mql_parserParserStaticData->atn, mql_parserParserStaticData->decisionToDFA, mql_parserParserStaticData->sharedContextCache, options);
}

MQL_Parser::~MQL_Parser() {
  delete _interpreter;
}

const atn::ATN& MQL_Parser::getATN() const {
  return *mql_parserParserStaticData->atn;
}

std::string MQL_Parser::getGrammarFileName() const {
  return "MQL_Parser.g4";
}

const std::vector<std::string>& MQL_Parser::getRuleNames() const {
  return mql_parserParserStaticData->ruleNames;
}

const dfa::Vocabulary& MQL_Parser::getVocabulary() const {
  return mql_parserParserStaticData->vocabulary;
}

antlr4::atn::SerializedATNView MQL_Parser::getSerializedATN() const {
  return mql_parserParserStaticData->serializedATN;
}


//----------------- RootContext ------------------------------------------------------------------

MQL_Parser::RootContext::RootContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::RootContext::EOF() {
  return getToken(MQL_Parser::EOF, 0);
}

MQL_Parser::MatchQueryContext* MQL_Parser::RootContext::matchQuery() {
  return getRuleContext<MQL_Parser::MatchQueryContext>(0);
}

MQL_Parser::DescribeQueryContext* MQL_Parser::RootContext::describeQuery() {
  return getRuleContext<MQL_Parser::DescribeQueryContext>(0);
}

MQL_Parser::InsertPatternsContext* MQL_Parser::RootContext::insertPatterns() {
  return getRuleContext<MQL_Parser::InsertPatternsContext>(0);
}

MQL_Parser::CreateIndexQueryContext* MQL_Parser::RootContext::createIndexQuery() {
  return getRuleContext<MQL_Parser::CreateIndexQueryContext>(0);
}

MQL_Parser::ShowQueryContext* MQL_Parser::RootContext::showQuery() {
  return getRuleContext<MQL_Parser::ShowQueryContext>(0);
}


size_t MQL_Parser::RootContext::getRuleIndex() const {
  return MQL_Parser::RuleRoot;
}


std::any MQL_Parser::RootContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitRoot(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::RootContext* MQL_Parser::root() {
  RootContext *_localctx = _tracker.createInstance<RootContext>(_ctx, getState());
  enterRule(_localctx, 0, MQL_Parser::RuleRoot);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(159);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case MQL_Parser::K_CALL:
      case MQL_Parser::K_LET:
      case MQL_Parser::K_MATCH: {
        setState(154);
        matchQuery();
        break;
      }

      case MQL_Parser::K_DESCRIBE: {
        setState(155);
        describeQuery();
        break;
      }

      case MQL_Parser::K_INSERT: {
        setState(156);
        insertPatterns();
        break;
      }

      case MQL_Parser::K_CREATE: {
        setState(157);
        createIndexQuery();
        break;
      }

      case MQL_Parser::K_SHOW: {
        setState(158);
        showQuery();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    setState(161);
    match(MQL_Parser::EOF);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- MatchQueryContext ------------------------------------------------------------------

MQL_Parser::MatchQueryContext::MatchQueryContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

MQL_Parser::PrimitiveStatementListContext* MQL_Parser::MatchQueryContext::primitiveStatementList() {
  return getRuleContext<MQL_Parser::PrimitiveStatementListContext>(0);
}

MQL_Parser::ReturnStatementContext* MQL_Parser::MatchQueryContext::returnStatement() {
  return getRuleContext<MQL_Parser::ReturnStatementContext>(0);
}

MQL_Parser::WhereStatementContext* MQL_Parser::MatchQueryContext::whereStatement() {
  return getRuleContext<MQL_Parser::WhereStatementContext>(0);
}

MQL_Parser::GroupByStatementContext* MQL_Parser::MatchQueryContext::groupByStatement() {
  return getRuleContext<MQL_Parser::GroupByStatementContext>(0);
}

MQL_Parser::OrderByStatementContext* MQL_Parser::MatchQueryContext::orderByStatement() {
  return getRuleContext<MQL_Parser::OrderByStatementContext>(0);
}


size_t MQL_Parser::MatchQueryContext::getRuleIndex() const {
  return MQL_Parser::RuleMatchQuery;
}


std::any MQL_Parser::MatchQueryContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitMatchQuery(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::MatchQueryContext* MQL_Parser::matchQuery() {
  MatchQueryContext *_localctx = _tracker.createInstance<MatchQueryContext>(_ctx, getState());
  enterRule(_localctx, 2, MQL_Parser::RuleMatchQuery);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(163);
    primitiveStatementList();
    setState(165);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MQL_Parser::K_WHERE) {
      setState(164);
      whereStatement();
    }
    setState(168);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MQL_Parser::K_GROUP) {
      setState(167);
      groupByStatement();
    }
    setState(171);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MQL_Parser::K_ORDER) {
      setState(170);
      orderByStatement();
    }
    setState(173);
    returnStatement();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PrimitiveStatementListContext ------------------------------------------------------------------

MQL_Parser::PrimitiveStatementListContext::PrimitiveStatementListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<MQL_Parser::PrimitiveStatementContext *> MQL_Parser::PrimitiveStatementListContext::primitiveStatement() {
  return getRuleContexts<MQL_Parser::PrimitiveStatementContext>();
}

MQL_Parser::PrimitiveStatementContext* MQL_Parser::PrimitiveStatementListContext::primitiveStatement(size_t i) {
  return getRuleContext<MQL_Parser::PrimitiveStatementContext>(i);
}


size_t MQL_Parser::PrimitiveStatementListContext::getRuleIndex() const {
  return MQL_Parser::RulePrimitiveStatementList;
}


std::any MQL_Parser::PrimitiveStatementListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitPrimitiveStatementList(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::PrimitiveStatementListContext* MQL_Parser::primitiveStatementList() {
  PrimitiveStatementListContext *_localctx = _tracker.createInstance<PrimitiveStatementListContext>(_ctx, getState());
  enterRule(_localctx, 4, MQL_Parser::RulePrimitiveStatementList);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(176); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(175);
      primitiveStatement();
      setState(178); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 1236950582272) != 0));
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PrimitiveStatementContext ------------------------------------------------------------------

MQL_Parser::PrimitiveStatementContext::PrimitiveStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

MQL_Parser::MatchStatementContext* MQL_Parser::PrimitiveStatementContext::matchStatement() {
  return getRuleContext<MQL_Parser::MatchStatementContext>(0);
}

MQL_Parser::CallStatementContext* MQL_Parser::PrimitiveStatementContext::callStatement() {
  return getRuleContext<MQL_Parser::CallStatementContext>(0);
}

MQL_Parser::LetStatementContext* MQL_Parser::PrimitiveStatementContext::letStatement() {
  return getRuleContext<MQL_Parser::LetStatementContext>(0);
}


size_t MQL_Parser::PrimitiveStatementContext::getRuleIndex() const {
  return MQL_Parser::RulePrimitiveStatement;
}


std::any MQL_Parser::PrimitiveStatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitPrimitiveStatement(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::PrimitiveStatementContext* MQL_Parser::primitiveStatement() {
  PrimitiveStatementContext *_localctx = _tracker.createInstance<PrimitiveStatementContext>(_ctx, getState());
  enterRule(_localctx, 6, MQL_Parser::RulePrimitiveStatement);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(183);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case MQL_Parser::K_MATCH: {
        enterOuterAlt(_localctx, 1);
        setState(180);
        matchStatement();
        break;
      }

      case MQL_Parser::K_CALL: {
        enterOuterAlt(_localctx, 2);
        setState(181);
        callStatement();
        break;
      }

      case MQL_Parser::K_LET: {
        enterOuterAlt(_localctx, 3);
        setState(182);
        letStatement();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- InsertPatternsContext ------------------------------------------------------------------

MQL_Parser::InsertPatternsContext::InsertPatternsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::InsertPatternsContext::K_INSERT() {
  return getToken(MQL_Parser::K_INSERT, 0);
}

std::vector<MQL_Parser::InsertLinearPatternContext *> MQL_Parser::InsertPatternsContext::insertLinearPattern() {
  return getRuleContexts<MQL_Parser::InsertLinearPatternContext>();
}

MQL_Parser::InsertLinearPatternContext* MQL_Parser::InsertPatternsContext::insertLinearPattern(size_t i) {
  return getRuleContext<MQL_Parser::InsertLinearPatternContext>(i);
}

std::vector<tree::TerminalNode *> MQL_Parser::InsertPatternsContext::COMMA() {
  return getTokens(MQL_Parser::COMMA);
}

tree::TerminalNode* MQL_Parser::InsertPatternsContext::COMMA(size_t i) {
  return getToken(MQL_Parser::COMMA, i);
}


size_t MQL_Parser::InsertPatternsContext::getRuleIndex() const {
  return MQL_Parser::RuleInsertPatterns;
}


std::any MQL_Parser::InsertPatternsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitInsertPatterns(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::InsertPatternsContext* MQL_Parser::insertPatterns() {
  InsertPatternsContext *_localctx = _tracker.createInstance<InsertPatternsContext>(_ctx, getState());
  enterRule(_localctx, 8, MQL_Parser::RuleInsertPatterns);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(185);
    match(MQL_Parser::K_INSERT);
    setState(186);
    insertLinearPattern();
    setState(191);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MQL_Parser::COMMA) {
      setState(187);
      match(MQL_Parser::COMMA);
      setState(188);
      insertLinearPattern();
      setState(193);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- InsertLinearPatternContext ------------------------------------------------------------------

MQL_Parser::InsertLinearPatternContext::InsertLinearPatternContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<MQL_Parser::InsertPlainNodeContext *> MQL_Parser::InsertLinearPatternContext::insertPlainNode() {
  return getRuleContexts<MQL_Parser::InsertPlainNodeContext>();
}

MQL_Parser::InsertPlainNodeContext* MQL_Parser::InsertLinearPatternContext::insertPlainNode(size_t i) {
  return getRuleContext<MQL_Parser::InsertPlainNodeContext>(i);
}

std::vector<MQL_Parser::InsertPlainEdgeContext *> MQL_Parser::InsertLinearPatternContext::insertPlainEdge() {
  return getRuleContexts<MQL_Parser::InsertPlainEdgeContext>();
}

MQL_Parser::InsertPlainEdgeContext* MQL_Parser::InsertLinearPatternContext::insertPlainEdge(size_t i) {
  return getRuleContext<MQL_Parser::InsertPlainEdgeContext>(i);
}


size_t MQL_Parser::InsertLinearPatternContext::getRuleIndex() const {
  return MQL_Parser::RuleInsertLinearPattern;
}


std::any MQL_Parser::InsertLinearPatternContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitInsertLinearPattern(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::InsertLinearPatternContext* MQL_Parser::insertLinearPattern() {
  InsertLinearPatternContext *_localctx = _tracker.createInstance<InsertLinearPatternContext>(_ctx, getState());
  enterRule(_localctx, 10, MQL_Parser::RuleInsertLinearPattern);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(194);
    insertPlainNode();
    setState(200);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MQL_Parser::LT

    || _la == MQL_Parser::MINUS) {
      setState(195);
      insertPlainEdge();
      setState(196);
      insertPlainNode();
      setState(202);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- InsertPlainNodeContext ------------------------------------------------------------------

MQL_Parser::InsertPlainNodeContext::InsertPlainNodeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::InsertPlainNodeContext::L_PAR() {
  return getToken(MQL_Parser::L_PAR, 0);
}

tree::TerminalNode* MQL_Parser::InsertPlainNodeContext::R_PAR() {
  return getToken(MQL_Parser::R_PAR, 0);
}

MQL_Parser::InsertPlainNodeInsideContext* MQL_Parser::InsertPlainNodeContext::insertPlainNodeInside() {
  return getRuleContext<MQL_Parser::InsertPlainNodeInsideContext>(0);
}

std::vector<tree::TerminalNode *> MQL_Parser::InsertPlainNodeContext::TYPE() {
  return getTokens(MQL_Parser::TYPE);
}

tree::TerminalNode* MQL_Parser::InsertPlainNodeContext::TYPE(size_t i) {
  return getToken(MQL_Parser::TYPE, i);
}

MQL_Parser::PropertiesContext* MQL_Parser::InsertPlainNodeContext::properties() {
  return getRuleContext<MQL_Parser::PropertiesContext>(0);
}


size_t MQL_Parser::InsertPlainNodeContext::getRuleIndex() const {
  return MQL_Parser::RuleInsertPlainNode;
}


std::any MQL_Parser::InsertPlainNodeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitInsertPlainNode(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::InsertPlainNodeContext* MQL_Parser::insertPlainNode() {
  InsertPlainNodeContext *_localctx = _tracker.createInstance<InsertPlainNodeContext>(_ctx, getState());
  enterRule(_localctx, 12, MQL_Parser::RuleInsertPlainNode);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(203);
    match(MQL_Parser::L_PAR);
    setState(205);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 9223372035778936830) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 64)) & 262207) != 0)) {
      setState(204);
      insertPlainNodeInside();
    }
    setState(210);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MQL_Parser::TYPE) {
      setState(207);
      match(MQL_Parser::TYPE);
      setState(212);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(214);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MQL_Parser::LCURLY_BRACKET) {
      setState(213);
      properties();
    }
    setState(216);
    match(MQL_Parser::R_PAR);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- InsertPlainNodeInsideContext ------------------------------------------------------------------

MQL_Parser::InsertPlainNodeInsideContext::InsertPlainNodeInsideContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

MQL_Parser::IdentifierContext* MQL_Parser::InsertPlainNodeInsideContext::identifier() {
  return getRuleContext<MQL_Parser::IdentifierContext>(0);
}


size_t MQL_Parser::InsertPlainNodeInsideContext::getRuleIndex() const {
  return MQL_Parser::RuleInsertPlainNodeInside;
}


std::any MQL_Parser::InsertPlainNodeInsideContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitInsertPlainNodeInside(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::InsertPlainNodeInsideContext* MQL_Parser::insertPlainNodeInside() {
  InsertPlainNodeInsideContext *_localctx = _tracker.createInstance<InsertPlainNodeInsideContext>(_ctx, getState());
  enterRule(_localctx, 14, MQL_Parser::RuleInsertPlainNodeInside);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(218);
    identifier();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- InsertPlainEdgeContext ------------------------------------------------------------------

MQL_Parser::InsertPlainEdgeContext::InsertPlainEdgeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::InsertPlainEdgeContext::LT() {
  return getToken(MQL_Parser::LT, 0);
}

std::vector<tree::TerminalNode *> MQL_Parser::InsertPlainEdgeContext::MINUS() {
  return getTokens(MQL_Parser::MINUS);
}

tree::TerminalNode* MQL_Parser::InsertPlainEdgeContext::MINUS(size_t i) {
  return getToken(MQL_Parser::MINUS, i);
}

tree::TerminalNode* MQL_Parser::InsertPlainEdgeContext::LSQUARE_BRACKET() {
  return getToken(MQL_Parser::LSQUARE_BRACKET, 0);
}

tree::TerminalNode* MQL_Parser::InsertPlainEdgeContext::TYPE() {
  return getToken(MQL_Parser::TYPE, 0);
}

tree::TerminalNode* MQL_Parser::InsertPlainEdgeContext::RSQUARE_BRACKET() {
  return getToken(MQL_Parser::RSQUARE_BRACKET, 0);
}

MQL_Parser::PropertiesContext* MQL_Parser::InsertPlainEdgeContext::properties() {
  return getRuleContext<MQL_Parser::PropertiesContext>(0);
}

tree::TerminalNode* MQL_Parser::InsertPlainEdgeContext::GT() {
  return getToken(MQL_Parser::GT, 0);
}


size_t MQL_Parser::InsertPlainEdgeContext::getRuleIndex() const {
  return MQL_Parser::RuleInsertPlainEdge;
}


std::any MQL_Parser::InsertPlainEdgeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitInsertPlainEdge(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::InsertPlainEdgeContext* MQL_Parser::insertPlainEdge() {
  InsertPlainEdgeContext *_localctx = _tracker.createInstance<InsertPlainEdgeContext>(_ctx, getState());
  enterRule(_localctx, 16, MQL_Parser::RuleInsertPlainEdge);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(238);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case MQL_Parser::LT: {
        enterOuterAlt(_localctx, 1);
        setState(220);
        match(MQL_Parser::LT);
        setState(221);
        match(MQL_Parser::MINUS);
        setState(222);
        match(MQL_Parser::LSQUARE_BRACKET);
        setState(223);
        match(MQL_Parser::TYPE);
        setState(225);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == MQL_Parser::LCURLY_BRACKET) {
          setState(224);
          properties();
        }
        setState(227);
        match(MQL_Parser::RSQUARE_BRACKET);
        setState(228);
        match(MQL_Parser::MINUS);
        break;
      }

      case MQL_Parser::MINUS: {
        enterOuterAlt(_localctx, 2);
        setState(229);
        match(MQL_Parser::MINUS);
        setState(230);
        match(MQL_Parser::LSQUARE_BRACKET);
        setState(231);
        match(MQL_Parser::TYPE);
        setState(233);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == MQL_Parser::LCURLY_BRACKET) {
          setState(232);
          properties();
        }
        setState(235);
        match(MQL_Parser::RSQUARE_BRACKET);
        setState(236);
        match(MQL_Parser::MINUS);
        setState(237);
        match(MQL_Parser::GT);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CreateIndexQueryContext ------------------------------------------------------------------

MQL_Parser::CreateIndexQueryContext::CreateIndexQueryContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::CreateIndexQueryContext::K_CREATE() {
  return getToken(MQL_Parser::K_CREATE, 0);
}

MQL_Parser::IdentifierContext* MQL_Parser::CreateIndexQueryContext::identifier() {
  return getRuleContext<MQL_Parser::IdentifierContext>(0);
}

tree::TerminalNode* MQL_Parser::CreateIndexQueryContext::K_INDEX() {
  return getToken(MQL_Parser::K_INDEX, 0);
}

tree::TerminalNode* MQL_Parser::CreateIndexQueryContext::STRING() {
  return getToken(MQL_Parser::STRING, 0);
}

tree::TerminalNode* MQL_Parser::CreateIndexQueryContext::K_WITH() {
  return getToken(MQL_Parser::K_WITH, 0);
}

MQL_Parser::CreateIndexOptionsContext* MQL_Parser::CreateIndexQueryContext::createIndexOptions() {
  return getRuleContext<MQL_Parser::CreateIndexOptionsContext>(0);
}


size_t MQL_Parser::CreateIndexQueryContext::getRuleIndex() const {
  return MQL_Parser::RuleCreateIndexQuery;
}


std::any MQL_Parser::CreateIndexQueryContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitCreateIndexQuery(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::CreateIndexQueryContext* MQL_Parser::createIndexQuery() {
  CreateIndexQueryContext *_localctx = _tracker.createInstance<CreateIndexQueryContext>(_ctx, getState());
  enterRule(_localctx, 18, MQL_Parser::RuleCreateIndexQuery);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(240);
    match(MQL_Parser::K_CREATE);
    setState(241);
    identifier();
    setState(242);
    match(MQL_Parser::K_INDEX);
    setState(243);
    match(MQL_Parser::STRING);
    setState(244);
    match(MQL_Parser::K_WITH);
    setState(245);
    createIndexOptions();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CreateIndexOptionsContext ------------------------------------------------------------------

MQL_Parser::CreateIndexOptionsContext::CreateIndexOptionsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::CreateIndexOptionsContext::LCURLY_BRACKET() {
  return getToken(MQL_Parser::LCURLY_BRACKET, 0);
}

tree::TerminalNode* MQL_Parser::CreateIndexOptionsContext::RCURLY_BRACKET() {
  return getToken(MQL_Parser::RCURLY_BRACKET, 0);
}

std::vector<MQL_Parser::CreateIndexOptionContext *> MQL_Parser::CreateIndexOptionsContext::createIndexOption() {
  return getRuleContexts<MQL_Parser::CreateIndexOptionContext>();
}

MQL_Parser::CreateIndexOptionContext* MQL_Parser::CreateIndexOptionsContext::createIndexOption(size_t i) {
  return getRuleContext<MQL_Parser::CreateIndexOptionContext>(i);
}

std::vector<tree::TerminalNode *> MQL_Parser::CreateIndexOptionsContext::COMMA() {
  return getTokens(MQL_Parser::COMMA);
}

tree::TerminalNode* MQL_Parser::CreateIndexOptionsContext::COMMA(size_t i) {
  return getToken(MQL_Parser::COMMA, i);
}


size_t MQL_Parser::CreateIndexOptionsContext::getRuleIndex() const {
  return MQL_Parser::RuleCreateIndexOptions;
}


std::any MQL_Parser::CreateIndexOptionsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitCreateIndexOptions(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::CreateIndexOptionsContext* MQL_Parser::createIndexOptions() {
  CreateIndexOptionsContext *_localctx = _tracker.createInstance<CreateIndexOptionsContext>(_ctx, getState());
  enterRule(_localctx, 20, MQL_Parser::RuleCreateIndexOptions);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(247);
    match(MQL_Parser::LCURLY_BRACKET);
    setState(256);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MQL_Parser::STRING) {
      setState(248);
      createIndexOption();
      setState(253);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == MQL_Parser::COMMA) {
        setState(249);
        match(MQL_Parser::COMMA);
        setState(250);
        createIndexOption();
        setState(255);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
    }
    setState(258);
    match(MQL_Parser::RCURLY_BRACKET);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CreateIndexOptionContext ------------------------------------------------------------------

MQL_Parser::CreateIndexOptionContext::CreateIndexOptionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::CreateIndexOptionContext::STRING() {
  return getToken(MQL_Parser::STRING, 0);
}

tree::TerminalNode* MQL_Parser::CreateIndexOptionContext::SINGLE_EQ() {
  return getToken(MQL_Parser::SINGLE_EQ, 0);
}

MQL_Parser::ValueContext* MQL_Parser::CreateIndexOptionContext::value() {
  return getRuleContext<MQL_Parser::ValueContext>(0);
}

MQL_Parser::BoolValueContext* MQL_Parser::CreateIndexOptionContext::boolValue() {
  return getRuleContext<MQL_Parser::BoolValueContext>(0);
}


size_t MQL_Parser::CreateIndexOptionContext::getRuleIndex() const {
  return MQL_Parser::RuleCreateIndexOption;
}


std::any MQL_Parser::CreateIndexOptionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitCreateIndexOption(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::CreateIndexOptionContext* MQL_Parser::createIndexOption() {
  CreateIndexOptionContext *_localctx = _tracker.createInstance<CreateIndexOptionContext>(_ctx, getState());
  enterRule(_localctx, 22, MQL_Parser::RuleCreateIndexOption);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(260);
    match(MQL_Parser::STRING);
    setState(264);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case MQL_Parser::SINGLE_EQ: {
        setState(261);
        match(MQL_Parser::SINGLE_EQ);
        setState(262);
        value();
        break;
      }

      case MQL_Parser::K_FALSE:
      case MQL_Parser::K_TRUE: {
        setState(263);
        boolValue();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ShowQueryContext ------------------------------------------------------------------

MQL_Parser::ShowQueryContext::ShowQueryContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::ShowQueryContext::K_SHOW() {
  return getToken(MQL_Parser::K_SHOW, 0);
}

MQL_Parser::IdentifierContext* MQL_Parser::ShowQueryContext::identifier() {
  return getRuleContext<MQL_Parser::IdentifierContext>(0);
}

tree::TerminalNode* MQL_Parser::ShowQueryContext::K_INDEX() {
  return getToken(MQL_Parser::K_INDEX, 0);
}


size_t MQL_Parser::ShowQueryContext::getRuleIndex() const {
  return MQL_Parser::RuleShowQuery;
}


std::any MQL_Parser::ShowQueryContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitShowQuery(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::ShowQueryContext* MQL_Parser::showQuery() {
  ShowQueryContext *_localctx = _tracker.createInstance<ShowQueryContext>(_ctx, getState());
  enterRule(_localctx, 24, MQL_Parser::RuleShowQuery);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(266);
    match(MQL_Parser::K_SHOW);
    setState(271);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case MQL_Parser::EOF: {
        break;
      }

      case MQL_Parser::K_ACYCLIC:
      case MQL_Parser::K_AND:
      case MQL_Parser::K_ANY:
      case MQL_Parser::K_AS:
      case MQL_Parser::K_AVG:
      case MQL_Parser::K_ALL:
      case MQL_Parser::K_ASC:
      case MQL_Parser::K_BY:
      case MQL_Parser::K_BOOL:
      case MQL_Parser::K_CALL:
      case MQL_Parser::K_COSINE_DISTANCE:
      case MQL_Parser::K_COSINE_SIMILARITY:
      case MQL_Parser::K_COUNT:
      case MQL_Parser::K_CREATE:
      case MQL_Parser::K_DELETE:
      case MQL_Parser::K_DESCRIBE:
      case MQL_Parser::K_DESC:
      case MQL_Parser::K_DIMENSIONS:
      case MQL_Parser::K_DISTINCT:
      case MQL_Parser::K_EDGE:
      case MQL_Parser::K_EUCLIDEAN_DISTANCE:
      case MQL_Parser::K_FROM:
      case MQL_Parser::K_INCOMING:
      case MQL_Parser::K_INDEX:
      case MQL_Parser::K_INSERT:
      case MQL_Parser::K_INTEGER:
      case MQL_Parser::K_INTO:
      case MQL_Parser::K_IS:
      case MQL_Parser::K_FLOAT:
      case MQL_Parser::K_GROUP:
      case MQL_Parser::K_GROUPS:
      case MQL_Parser::K_IDENTITY:
      case MQL_Parser::K_LABELS:
      case MQL_Parser::K_LABEL:
      case MQL_Parser::K_LET:
      case MQL_Parser::K_LIMIT:
      case MQL_Parser::K_MANHATTAN_DISTANCE:
      case MQL_Parser::K_MATCH:
      case MQL_Parser::K_MAX:
      case MQL_Parser::K_MIN:
      case MQL_Parser::K_NORMALIZE:
      case MQL_Parser::K_OBJECTS:
      case MQL_Parser::K_OFFSET:
      case MQL_Parser::K_OPTIONAL:
      case MQL_Parser::K_ORDER:
      case MQL_Parser::K_OR:
      case MQL_Parser::K_OUTGOING:
      case MQL_Parser::K_PREFIX:
      case MQL_Parser::K_PROPERTIES:
      case MQL_Parser::K_PROPERTY:
      case MQL_Parser::K_NOT:
      case MQL_Parser::K_NULL:
      case MQL_Parser::K_SHORTEST:
      case MQL_Parser::K_SHOW:
      case MQL_Parser::K_SIMPLE:
      case MQL_Parser::K_TEXT_SEARCH:
      case MQL_Parser::K_REGEX:
      case MQL_Parser::K_RETURN:
      case MQL_Parser::K_SUM:
      case MQL_Parser::K_STRING:
      case MQL_Parser::K_TRAILS:
      case MQL_Parser::K_VALUES:
      case MQL_Parser::K_WALKS:
      case MQL_Parser::K_WITH:
      case MQL_Parser::K_WHERE:
      case MQL_Parser::K_YIELD:
      case MQL_Parser::NAME: {
        setState(268);
        identifier();
        setState(269);
        match(MQL_Parser::K_INDEX);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DescribeQueryContext ------------------------------------------------------------------

MQL_Parser::DescribeQueryContext::DescribeQueryContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::DescribeQueryContext::K_DESCRIBE() {
  return getToken(MQL_Parser::K_DESCRIBE, 0);
}

MQL_Parser::FixedNodeInsideContext* MQL_Parser::DescribeQueryContext::fixedNodeInside() {
  return getRuleContext<MQL_Parser::FixedNodeInsideContext>(0);
}

std::vector<MQL_Parser::DescribeFlagContext *> MQL_Parser::DescribeQueryContext::describeFlag() {
  return getRuleContexts<MQL_Parser::DescribeFlagContext>();
}

MQL_Parser::DescribeFlagContext* MQL_Parser::DescribeQueryContext::describeFlag(size_t i) {
  return getRuleContext<MQL_Parser::DescribeFlagContext>(i);
}


size_t MQL_Parser::DescribeQueryContext::getRuleIndex() const {
  return MQL_Parser::RuleDescribeQuery;
}


std::any MQL_Parser::DescribeQueryContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitDescribeQuery(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::DescribeQueryContext* MQL_Parser::describeQuery() {
  DescribeQueryContext *_localctx = _tracker.createInstance<DescribeQueryContext>(_ctx, getState());
  enterRule(_localctx, 26, MQL_Parser::RuleDescribeQuery);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(273);
    match(MQL_Parser::K_DESCRIBE);
    setState(277);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 18, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(274);
        describeFlag(); 
      }
      setState(279);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 18, _ctx);
    }
    setState(280);
    fixedNodeInside();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DescribeFlagContext ------------------------------------------------------------------

MQL_Parser::DescribeFlagContext::DescribeFlagContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::DescribeFlagContext::K_LABELS() {
  return getToken(MQL_Parser::K_LABELS, 0);
}

tree::TerminalNode* MQL_Parser::DescribeFlagContext::K_PROPERTIES() {
  return getToken(MQL_Parser::K_PROPERTIES, 0);
}

tree::TerminalNode* MQL_Parser::DescribeFlagContext::K_OUTGOING() {
  return getToken(MQL_Parser::K_OUTGOING, 0);
}

tree::TerminalNode* MQL_Parser::DescribeFlagContext::K_INCOMING() {
  return getToken(MQL_Parser::K_INCOMING, 0);
}

tree::TerminalNode* MQL_Parser::DescribeFlagContext::K_LIMIT() {
  return getToken(MQL_Parser::K_LIMIT, 0);
}

tree::TerminalNode* MQL_Parser::DescribeFlagContext::UNSIGNED_INTEGER() {
  return getToken(MQL_Parser::UNSIGNED_INTEGER, 0);
}


size_t MQL_Parser::DescribeFlagContext::getRuleIndex() const {
  return MQL_Parser::RuleDescribeFlag;
}


std::any MQL_Parser::DescribeFlagContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitDescribeFlag(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::DescribeFlagContext* MQL_Parser::describeFlag() {
  DescribeFlagContext *_localctx = _tracker.createInstance<DescribeFlagContext>(_ctx, getState());
  enterRule(_localctx, 28, MQL_Parser::RuleDescribeFlag);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(282);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 2814784143622144) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
    setState(285);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 19, _ctx)) {
    case 1: {
      setState(283);
      match(MQL_Parser::K_LIMIT);
      setState(284);
      match(MQL_Parser::UNSIGNED_INTEGER);
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- MatchStatementContext ------------------------------------------------------------------

MQL_Parser::MatchStatementContext::MatchStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::MatchStatementContext::K_MATCH() {
  return getToken(MQL_Parser::K_MATCH, 0);
}

MQL_Parser::GraphPatternContext* MQL_Parser::MatchStatementContext::graphPattern() {
  return getRuleContext<MQL_Parser::GraphPatternContext>(0);
}


size_t MQL_Parser::MatchStatementContext::getRuleIndex() const {
  return MQL_Parser::RuleMatchStatement;
}


std::any MQL_Parser::MatchStatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitMatchStatement(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::MatchStatementContext* MQL_Parser::matchStatement() {
  MatchStatementContext *_localctx = _tracker.createInstance<MatchStatementContext>(_ctx, getState());
  enterRule(_localctx, 30, MQL_Parser::RuleMatchStatement);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(287);
    match(MQL_Parser::K_MATCH);
    setState(288);
    graphPattern();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LetStatementContext ------------------------------------------------------------------

MQL_Parser::LetStatementContext::LetStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::LetStatementContext::K_LET() {
  return getToken(MQL_Parser::K_LET, 0);
}

MQL_Parser::LetDefinitionListContext* MQL_Parser::LetStatementContext::letDefinitionList() {
  return getRuleContext<MQL_Parser::LetDefinitionListContext>(0);
}


size_t MQL_Parser::LetStatementContext::getRuleIndex() const {
  return MQL_Parser::RuleLetStatement;
}


std::any MQL_Parser::LetStatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitLetStatement(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::LetStatementContext* MQL_Parser::letStatement() {
  LetStatementContext *_localctx = _tracker.createInstance<LetStatementContext>(_ctx, getState());
  enterRule(_localctx, 32, MQL_Parser::RuleLetStatement);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(290);
    match(MQL_Parser::K_LET);
    setState(291);
    letDefinitionList();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LetDefinitionListContext ------------------------------------------------------------------

MQL_Parser::LetDefinitionListContext::LetDefinitionListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<MQL_Parser::LetDefinitionContext *> MQL_Parser::LetDefinitionListContext::letDefinition() {
  return getRuleContexts<MQL_Parser::LetDefinitionContext>();
}

MQL_Parser::LetDefinitionContext* MQL_Parser::LetDefinitionListContext::letDefinition(size_t i) {
  return getRuleContext<MQL_Parser::LetDefinitionContext>(i);
}

std::vector<tree::TerminalNode *> MQL_Parser::LetDefinitionListContext::COMMA() {
  return getTokens(MQL_Parser::COMMA);
}

tree::TerminalNode* MQL_Parser::LetDefinitionListContext::COMMA(size_t i) {
  return getToken(MQL_Parser::COMMA, i);
}


size_t MQL_Parser::LetDefinitionListContext::getRuleIndex() const {
  return MQL_Parser::RuleLetDefinitionList;
}


std::any MQL_Parser::LetDefinitionListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitLetDefinitionList(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::LetDefinitionListContext* MQL_Parser::letDefinitionList() {
  LetDefinitionListContext *_localctx = _tracker.createInstance<LetDefinitionListContext>(_ctx, getState());
  enterRule(_localctx, 34, MQL_Parser::RuleLetDefinitionList);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(293);
    letDefinition();
    setState(298);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MQL_Parser::COMMA) {
      setState(294);
      match(MQL_Parser::COMMA);
      setState(295);
      letDefinition();
      setState(300);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LetDefinitionContext ------------------------------------------------------------------

MQL_Parser::LetDefinitionContext::LetDefinitionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::LetDefinitionContext::VARIABLE() {
  return getToken(MQL_Parser::VARIABLE, 0);
}

tree::TerminalNode* MQL_Parser::LetDefinitionContext::SINGLE_EQ() {
  return getToken(MQL_Parser::SINGLE_EQ, 0);
}

MQL_Parser::ConditionalOrExprContext* MQL_Parser::LetDefinitionContext::conditionalOrExpr() {
  return getRuleContext<MQL_Parser::ConditionalOrExprContext>(0);
}


size_t MQL_Parser::LetDefinitionContext::getRuleIndex() const {
  return MQL_Parser::RuleLetDefinition;
}


std::any MQL_Parser::LetDefinitionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitLetDefinition(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::LetDefinitionContext* MQL_Parser::letDefinition() {
  LetDefinitionContext *_localctx = _tracker.createInstance<LetDefinitionContext>(_ctx, getState());
  enterRule(_localctx, 36, MQL_Parser::RuleLetDefinition);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(301);
    match(MQL_Parser::VARIABLE);
    setState(302);
    match(MQL_Parser::SINGLE_EQ);
    setState(303);
    conditionalOrExpr();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- WhereStatementContext ------------------------------------------------------------------

MQL_Parser::WhereStatementContext::WhereStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::WhereStatementContext::K_WHERE() {
  return getToken(MQL_Parser::K_WHERE, 0);
}

MQL_Parser::ConditionalOrExprContext* MQL_Parser::WhereStatementContext::conditionalOrExpr() {
  return getRuleContext<MQL_Parser::ConditionalOrExprContext>(0);
}


size_t MQL_Parser::WhereStatementContext::getRuleIndex() const {
  return MQL_Parser::RuleWhereStatement;
}


std::any MQL_Parser::WhereStatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitWhereStatement(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::WhereStatementContext* MQL_Parser::whereStatement() {
  WhereStatementContext *_localctx = _tracker.createInstance<WhereStatementContext>(_ctx, getState());
  enterRule(_localctx, 38, MQL_Parser::RuleWhereStatement);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(305);
    match(MQL_Parser::K_WHERE);
    setState(306);
    conditionalOrExpr();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- GroupByStatementContext ------------------------------------------------------------------

MQL_Parser::GroupByStatementContext::GroupByStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::GroupByStatementContext::K_GROUP() {
  return getToken(MQL_Parser::K_GROUP, 0);
}

tree::TerminalNode* MQL_Parser::GroupByStatementContext::K_BY() {
  return getToken(MQL_Parser::K_BY, 0);
}

std::vector<MQL_Parser::GroupByItemContext *> MQL_Parser::GroupByStatementContext::groupByItem() {
  return getRuleContexts<MQL_Parser::GroupByItemContext>();
}

MQL_Parser::GroupByItemContext* MQL_Parser::GroupByStatementContext::groupByItem(size_t i) {
  return getRuleContext<MQL_Parser::GroupByItemContext>(i);
}

std::vector<tree::TerminalNode *> MQL_Parser::GroupByStatementContext::COMMA() {
  return getTokens(MQL_Parser::COMMA);
}

tree::TerminalNode* MQL_Parser::GroupByStatementContext::COMMA(size_t i) {
  return getToken(MQL_Parser::COMMA, i);
}


size_t MQL_Parser::GroupByStatementContext::getRuleIndex() const {
  return MQL_Parser::RuleGroupByStatement;
}


std::any MQL_Parser::GroupByStatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitGroupByStatement(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::GroupByStatementContext* MQL_Parser::groupByStatement() {
  GroupByStatementContext *_localctx = _tracker.createInstance<GroupByStatementContext>(_ctx, getState());
  enterRule(_localctx, 40, MQL_Parser::RuleGroupByStatement);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(308);
    match(MQL_Parser::K_GROUP);
    setState(309);
    match(MQL_Parser::K_BY);
    setState(310);
    groupByItem();
    setState(315);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MQL_Parser::COMMA) {
      setState(311);
      match(MQL_Parser::COMMA);
      setState(312);
      groupByItem();
      setState(317);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- OrderByStatementContext ------------------------------------------------------------------

MQL_Parser::OrderByStatementContext::OrderByStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::OrderByStatementContext::K_ORDER() {
  return getToken(MQL_Parser::K_ORDER, 0);
}

tree::TerminalNode* MQL_Parser::OrderByStatementContext::K_BY() {
  return getToken(MQL_Parser::K_BY, 0);
}

std::vector<MQL_Parser::OrderByItemContext *> MQL_Parser::OrderByStatementContext::orderByItem() {
  return getRuleContexts<MQL_Parser::OrderByItemContext>();
}

MQL_Parser::OrderByItemContext* MQL_Parser::OrderByStatementContext::orderByItem(size_t i) {
  return getRuleContext<MQL_Parser::OrderByItemContext>(i);
}

std::vector<tree::TerminalNode *> MQL_Parser::OrderByStatementContext::COMMA() {
  return getTokens(MQL_Parser::COMMA);
}

tree::TerminalNode* MQL_Parser::OrderByStatementContext::COMMA(size_t i) {
  return getToken(MQL_Parser::COMMA, i);
}


size_t MQL_Parser::OrderByStatementContext::getRuleIndex() const {
  return MQL_Parser::RuleOrderByStatement;
}


std::any MQL_Parser::OrderByStatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitOrderByStatement(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::OrderByStatementContext* MQL_Parser::orderByStatement() {
  OrderByStatementContext *_localctx = _tracker.createInstance<OrderByStatementContext>(_ctx, getState());
  enterRule(_localctx, 42, MQL_Parser::RuleOrderByStatement);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(318);
    match(MQL_Parser::K_ORDER);
    setState(319);
    match(MQL_Parser::K_BY);
    setState(320);
    orderByItem();
    setState(325);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MQL_Parser::COMMA) {
      setState(321);
      match(MQL_Parser::COMMA);
      setState(322);
      orderByItem();
      setState(327);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ReturnStatementContext ------------------------------------------------------------------

MQL_Parser::ReturnStatementContext::ReturnStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t MQL_Parser::ReturnStatementContext::getRuleIndex() const {
  return MQL_Parser::RuleReturnStatement;
}

void MQL_Parser::ReturnStatementContext::copyFrom(ReturnStatementContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- ReturnAllContext ------------------------------------------------------------------

tree::TerminalNode* MQL_Parser::ReturnAllContext::K_RETURN() {
  return getToken(MQL_Parser::K_RETURN, 0);
}

tree::TerminalNode* MQL_Parser::ReturnAllContext::STAR() {
  return getToken(MQL_Parser::STAR, 0);
}

tree::TerminalNode* MQL_Parser::ReturnAllContext::K_DISTINCT() {
  return getToken(MQL_Parser::K_DISTINCT, 0);
}

MQL_Parser::LimitOffsetClausesContext* MQL_Parser::ReturnAllContext::limitOffsetClauses() {
  return getRuleContext<MQL_Parser::LimitOffsetClausesContext>(0);
}

MQL_Parser::ReturnAllContext::ReturnAllContext(ReturnStatementContext *ctx) { copyFrom(ctx); }


std::any MQL_Parser::ReturnAllContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitReturnAll(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ReturnListContext ------------------------------------------------------------------

tree::TerminalNode* MQL_Parser::ReturnListContext::K_RETURN() {
  return getToken(MQL_Parser::K_RETURN, 0);
}

std::vector<MQL_Parser::ReturnItemContext *> MQL_Parser::ReturnListContext::returnItem() {
  return getRuleContexts<MQL_Parser::ReturnItemContext>();
}

MQL_Parser::ReturnItemContext* MQL_Parser::ReturnListContext::returnItem(size_t i) {
  return getRuleContext<MQL_Parser::ReturnItemContext>(i);
}

tree::TerminalNode* MQL_Parser::ReturnListContext::K_DISTINCT() {
  return getToken(MQL_Parser::K_DISTINCT, 0);
}

std::vector<tree::TerminalNode *> MQL_Parser::ReturnListContext::COMMA() {
  return getTokens(MQL_Parser::COMMA);
}

tree::TerminalNode* MQL_Parser::ReturnListContext::COMMA(size_t i) {
  return getToken(MQL_Parser::COMMA, i);
}

MQL_Parser::LimitOffsetClausesContext* MQL_Parser::ReturnListContext::limitOffsetClauses() {
  return getRuleContext<MQL_Parser::LimitOffsetClausesContext>(0);
}

MQL_Parser::ReturnListContext::ReturnListContext(ReturnStatementContext *ctx) { copyFrom(ctx); }


std::any MQL_Parser::ReturnListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitReturnList(this);
  else
    return visitor->visitChildren(this);
}
MQL_Parser::ReturnStatementContext* MQL_Parser::returnStatement() {
  ReturnStatementContext *_localctx = _tracker.createInstance<ReturnStatementContext>(_ctx, getState());
  enterRule(_localctx, 44, MQL_Parser::RuleReturnStatement);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(351);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 28, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<MQL_Parser::ReturnListContext>(_localctx);
      enterOuterAlt(_localctx, 1);
      setState(328);
      match(MQL_Parser::K_RETURN);
      setState(330);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 23, _ctx)) {
      case 1: {
        setState(329);
        match(MQL_Parser::K_DISTINCT);
        break;
      }

      default:
        break;
      }
      setState(332);
      returnItem();
      setState(337);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == MQL_Parser::COMMA) {
        setState(333);
        match(MQL_Parser::COMMA);
        setState(334);
        returnItem();
        setState(339);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(341);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == MQL_Parser::K_LIMIT

      || _la == MQL_Parser::K_OFFSET) {
        setState(340);
        limitOffsetClauses();
      }
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<MQL_Parser::ReturnAllContext>(_localctx);
      enterOuterAlt(_localctx, 2);
      setState(343);
      match(MQL_Parser::K_RETURN);
      setState(345);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == MQL_Parser::K_DISTINCT) {
        setState(344);
        match(MQL_Parser::K_DISTINCT);
      }
      setState(347);
      match(MQL_Parser::STAR);
      setState(349);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == MQL_Parser::K_LIMIT

      || _la == MQL_Parser::K_OFFSET) {
        setState(348);
        limitOffsetClauses();
      }
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CallStatementContext ------------------------------------------------------------------

MQL_Parser::CallStatementContext::CallStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::CallStatementContext::K_CALL() {
  return getToken(MQL_Parser::K_CALL, 0);
}

MQL_Parser::IdentifierContext* MQL_Parser::CallStatementContext::identifier() {
  return getRuleContext<MQL_Parser::IdentifierContext>(0);
}

tree::TerminalNode* MQL_Parser::CallStatementContext::L_PAR() {
  return getToken(MQL_Parser::L_PAR, 0);
}

tree::TerminalNode* MQL_Parser::CallStatementContext::R_PAR() {
  return getToken(MQL_Parser::R_PAR, 0);
}

MQL_Parser::CallArgumentsContext* MQL_Parser::CallStatementContext::callArguments() {
  return getRuleContext<MQL_Parser::CallArgumentsContext>(0);
}

MQL_Parser::YieldStatementContext* MQL_Parser::CallStatementContext::yieldStatement() {
  return getRuleContext<MQL_Parser::YieldStatementContext>(0);
}


size_t MQL_Parser::CallStatementContext::getRuleIndex() const {
  return MQL_Parser::RuleCallStatement;
}


std::any MQL_Parser::CallStatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitCallStatement(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::CallStatementContext* MQL_Parser::callStatement() {
  CallStatementContext *_localctx = _tracker.createInstance<CallStatementContext>(_ctx, getState());
  enterRule(_localctx, 46, MQL_Parser::RuleCallStatement);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(353);
    match(MQL_Parser::K_CALL);
    setState(354);
    identifier();
    setState(355);
    match(MQL_Parser::L_PAR);
    setState(357);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & -2) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 64)) & 30065287999) != 0)) {
      setState(356);
      callArguments();
    }
    setState(359);
    match(MQL_Parser::R_PAR);
    setState(361);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MQL_Parser::K_YIELD) {
      setState(360);
      yieldStatement();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- YieldStatementContext ------------------------------------------------------------------

MQL_Parser::YieldStatementContext::YieldStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::YieldStatementContext::K_YIELD() {
  return getToken(MQL_Parser::K_YIELD, 0);
}

tree::TerminalNode* MQL_Parser::YieldStatementContext::STAR() {
  return getToken(MQL_Parser::STAR, 0);
}

std::vector<MQL_Parser::YieldItemContext *> MQL_Parser::YieldStatementContext::yieldItem() {
  return getRuleContexts<MQL_Parser::YieldItemContext>();
}

MQL_Parser::YieldItemContext* MQL_Parser::YieldStatementContext::yieldItem(size_t i) {
  return getRuleContext<MQL_Parser::YieldItemContext>(i);
}

std::vector<tree::TerminalNode *> MQL_Parser::YieldStatementContext::COMMA() {
  return getTokens(MQL_Parser::COMMA);
}

tree::TerminalNode* MQL_Parser::YieldStatementContext::COMMA(size_t i) {
  return getToken(MQL_Parser::COMMA, i);
}


size_t MQL_Parser::YieldStatementContext::getRuleIndex() const {
  return MQL_Parser::RuleYieldStatement;
}


std::any MQL_Parser::YieldStatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitYieldStatement(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::YieldStatementContext* MQL_Parser::yieldStatement() {
  YieldStatementContext *_localctx = _tracker.createInstance<YieldStatementContext>(_ctx, getState());
  enterRule(_localctx, 48, MQL_Parser::RuleYieldStatement);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(363);
    match(MQL_Parser::K_YIELD);
    setState(373);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case MQL_Parser::STAR: {
        setState(364);
        match(MQL_Parser::STAR);
        break;
      }

      case MQL_Parser::VARIABLE: {
        setState(365);
        yieldItem();
        setState(370);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == MQL_Parser::COMMA) {
          setState(366);
          match(MQL_Parser::COMMA);
          setState(367);
          yieldItem();
          setState(372);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- YieldItemContext ------------------------------------------------------------------

MQL_Parser::YieldItemContext::YieldItemContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::YieldItemContext::VARIABLE() {
  return getToken(MQL_Parser::VARIABLE, 0);
}

MQL_Parser::AliasContext* MQL_Parser::YieldItemContext::alias() {
  return getRuleContext<MQL_Parser::AliasContext>(0);
}


size_t MQL_Parser::YieldItemContext::getRuleIndex() const {
  return MQL_Parser::RuleYieldItem;
}


std::any MQL_Parser::YieldItemContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitYieldItem(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::YieldItemContext* MQL_Parser::yieldItem() {
  YieldItemContext *_localctx = _tracker.createInstance<YieldItemContext>(_ctx, getState());
  enterRule(_localctx, 50, MQL_Parser::RuleYieldItem);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(375);
    match(MQL_Parser::VARIABLE);
    setState(377);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MQL_Parser::K_AS) {
      setState(376);
      alias();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CallArgumentsContext ------------------------------------------------------------------

MQL_Parser::CallArgumentsContext::CallArgumentsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<MQL_Parser::ConditionalOrExprContext *> MQL_Parser::CallArgumentsContext::conditionalOrExpr() {
  return getRuleContexts<MQL_Parser::ConditionalOrExprContext>();
}

MQL_Parser::ConditionalOrExprContext* MQL_Parser::CallArgumentsContext::conditionalOrExpr(size_t i) {
  return getRuleContext<MQL_Parser::ConditionalOrExprContext>(i);
}

std::vector<tree::TerminalNode *> MQL_Parser::CallArgumentsContext::COMMA() {
  return getTokens(MQL_Parser::COMMA);
}

tree::TerminalNode* MQL_Parser::CallArgumentsContext::COMMA(size_t i) {
  return getToken(MQL_Parser::COMMA, i);
}


size_t MQL_Parser::CallArgumentsContext::getRuleIndex() const {
  return MQL_Parser::RuleCallArguments;
}


std::any MQL_Parser::CallArgumentsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitCallArguments(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::CallArgumentsContext* MQL_Parser::callArguments() {
  CallArgumentsContext *_localctx = _tracker.createInstance<CallArgumentsContext>(_ctx, getState());
  enterRule(_localctx, 52, MQL_Parser::RuleCallArguments);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(379);
    conditionalOrExpr();
    setState(384);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MQL_Parser::COMMA) {
      setState(380);
      match(MQL_Parser::COMMA);
      setState(381);
      conditionalOrExpr();
      setState(386);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LimitOffsetClausesContext ------------------------------------------------------------------

MQL_Parser::LimitOffsetClausesContext::LimitOffsetClausesContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

MQL_Parser::LimitClauseContext* MQL_Parser::LimitOffsetClausesContext::limitClause() {
  return getRuleContext<MQL_Parser::LimitClauseContext>(0);
}

MQL_Parser::OffsetClauseContext* MQL_Parser::LimitOffsetClausesContext::offsetClause() {
  return getRuleContext<MQL_Parser::OffsetClauseContext>(0);
}


size_t MQL_Parser::LimitOffsetClausesContext::getRuleIndex() const {
  return MQL_Parser::RuleLimitOffsetClauses;
}


std::any MQL_Parser::LimitOffsetClausesContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitLimitOffsetClauses(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::LimitOffsetClausesContext* MQL_Parser::limitOffsetClauses() {
  LimitOffsetClausesContext *_localctx = _tracker.createInstance<LimitOffsetClausesContext>(_ctx, getState());
  enterRule(_localctx, 54, MQL_Parser::RuleLimitOffsetClauses);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(395);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case MQL_Parser::K_LIMIT: {
        enterOuterAlt(_localctx, 1);
        setState(387);
        limitClause();
        setState(389);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == MQL_Parser::K_OFFSET) {
          setState(388);
          offsetClause();
        }
        break;
      }

      case MQL_Parser::K_OFFSET: {
        enterOuterAlt(_localctx, 2);
        setState(391);
        offsetClause();
        setState(393);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == MQL_Parser::K_LIMIT) {
          setState(392);
          limitClause();
        }
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LimitClauseContext ------------------------------------------------------------------

MQL_Parser::LimitClauseContext::LimitClauseContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::LimitClauseContext::K_LIMIT() {
  return getToken(MQL_Parser::K_LIMIT, 0);
}

tree::TerminalNode* MQL_Parser::LimitClauseContext::UNSIGNED_INTEGER() {
  return getToken(MQL_Parser::UNSIGNED_INTEGER, 0);
}


size_t MQL_Parser::LimitClauseContext::getRuleIndex() const {
  return MQL_Parser::RuleLimitClause;
}


std::any MQL_Parser::LimitClauseContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitLimitClause(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::LimitClauseContext* MQL_Parser::limitClause() {
  LimitClauseContext *_localctx = _tracker.createInstance<LimitClauseContext>(_ctx, getState());
  enterRule(_localctx, 56, MQL_Parser::RuleLimitClause);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(397);
    match(MQL_Parser::K_LIMIT);
    setState(398);
    match(MQL_Parser::UNSIGNED_INTEGER);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- OffsetClauseContext ------------------------------------------------------------------

MQL_Parser::OffsetClauseContext::OffsetClauseContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::OffsetClauseContext::K_OFFSET() {
  return getToken(MQL_Parser::K_OFFSET, 0);
}

tree::TerminalNode* MQL_Parser::OffsetClauseContext::UNSIGNED_INTEGER() {
  return getToken(MQL_Parser::UNSIGNED_INTEGER, 0);
}


size_t MQL_Parser::OffsetClauseContext::getRuleIndex() const {
  return MQL_Parser::RuleOffsetClause;
}


std::any MQL_Parser::OffsetClauseContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitOffsetClause(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::OffsetClauseContext* MQL_Parser::offsetClause() {
  OffsetClauseContext *_localctx = _tracker.createInstance<OffsetClauseContext>(_ctx, getState());
  enterRule(_localctx, 58, MQL_Parser::RuleOffsetClause);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(400);
    match(MQL_Parser::K_OFFSET);
    setState(401);
    match(MQL_Parser::UNSIGNED_INTEGER);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ReturnItemContext ------------------------------------------------------------------

MQL_Parser::ReturnItemContext::ReturnItemContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t MQL_Parser::ReturnItemContext::getRuleIndex() const {
  return MQL_Parser::RuleReturnItem;
}

void MQL_Parser::ReturnItemContext::copyFrom(ReturnItemContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- ReturnItemExprContext ------------------------------------------------------------------

MQL_Parser::ConditionalOrExprContext* MQL_Parser::ReturnItemExprContext::conditionalOrExpr() {
  return getRuleContext<MQL_Parser::ConditionalOrExprContext>(0);
}

MQL_Parser::AliasContext* MQL_Parser::ReturnItemExprContext::alias() {
  return getRuleContext<MQL_Parser::AliasContext>(0);
}

MQL_Parser::ReturnItemExprContext::ReturnItemExprContext(ReturnItemContext *ctx) { copyFrom(ctx); }


std::any MQL_Parser::ReturnItemExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitReturnItemExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ReturnItemVarContext ------------------------------------------------------------------

tree::TerminalNode* MQL_Parser::ReturnItemVarContext::VARIABLE() {
  return getToken(MQL_Parser::VARIABLE, 0);
}

tree::TerminalNode* MQL_Parser::ReturnItemVarContext::KEY() {
  return getToken(MQL_Parser::KEY, 0);
}

MQL_Parser::ReturnItemVarContext::ReturnItemVarContext(ReturnItemContext *ctx) { copyFrom(ctx); }


std::any MQL_Parser::ReturnItemVarContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitReturnItemVar(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ReturnItemAggContext ------------------------------------------------------------------

MQL_Parser::AggregateFuncContext* MQL_Parser::ReturnItemAggContext::aggregateFunc() {
  return getRuleContext<MQL_Parser::AggregateFuncContext>(0);
}

tree::TerminalNode* MQL_Parser::ReturnItemAggContext::L_PAR() {
  return getToken(MQL_Parser::L_PAR, 0);
}

tree::TerminalNode* MQL_Parser::ReturnItemAggContext::VARIABLE() {
  return getToken(MQL_Parser::VARIABLE, 0);
}

tree::TerminalNode* MQL_Parser::ReturnItemAggContext::R_PAR() {
  return getToken(MQL_Parser::R_PAR, 0);
}

tree::TerminalNode* MQL_Parser::ReturnItemAggContext::KEY() {
  return getToken(MQL_Parser::KEY, 0);
}

MQL_Parser::AliasContext* MQL_Parser::ReturnItemAggContext::alias() {
  return getRuleContext<MQL_Parser::AliasContext>(0);
}

MQL_Parser::ReturnItemAggContext::ReturnItemAggContext(ReturnItemContext *ctx) { copyFrom(ctx); }


std::any MQL_Parser::ReturnItemAggContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitReturnItemAgg(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ReturnItemCountContext ------------------------------------------------------------------

tree::TerminalNode* MQL_Parser::ReturnItemCountContext::K_COUNT() {
  return getToken(MQL_Parser::K_COUNT, 0);
}

tree::TerminalNode* MQL_Parser::ReturnItemCountContext::L_PAR() {
  return getToken(MQL_Parser::L_PAR, 0);
}

tree::TerminalNode* MQL_Parser::ReturnItemCountContext::R_PAR() {
  return getToken(MQL_Parser::R_PAR, 0);
}

tree::TerminalNode* MQL_Parser::ReturnItemCountContext::VARIABLE() {
  return getToken(MQL_Parser::VARIABLE, 0);
}

tree::TerminalNode* MQL_Parser::ReturnItemCountContext::STAR() {
  return getToken(MQL_Parser::STAR, 0);
}

tree::TerminalNode* MQL_Parser::ReturnItemCountContext::K_DISTINCT() {
  return getToken(MQL_Parser::K_DISTINCT, 0);
}

MQL_Parser::AliasContext* MQL_Parser::ReturnItemCountContext::alias() {
  return getRuleContext<MQL_Parser::AliasContext>(0);
}

tree::TerminalNode* MQL_Parser::ReturnItemCountContext::KEY() {
  return getToken(MQL_Parser::KEY, 0);
}

MQL_Parser::ReturnItemCountContext::ReturnItemCountContext(ReturnItemContext *ctx) { copyFrom(ctx); }


std::any MQL_Parser::ReturnItemCountContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitReturnItemCount(this);
  else
    return visitor->visitChildren(this);
}
MQL_Parser::ReturnItemContext* MQL_Parser::returnItem() {
  ReturnItemContext *_localctx = _tracker.createInstance<ReturnItemContext>(_ctx, getState());
  enterRule(_localctx, 60, MQL_Parser::RuleReturnItem);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(436);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 45, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<MQL_Parser::ReturnItemVarContext>(_localctx);
      enterOuterAlt(_localctx, 1);
      setState(403);
      match(MQL_Parser::VARIABLE);
      setState(405);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == MQL_Parser::KEY) {
        setState(404);
        match(MQL_Parser::KEY);
      }
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<MQL_Parser::ReturnItemAggContext>(_localctx);
      enterOuterAlt(_localctx, 2);
      setState(407);
      aggregateFunc();
      setState(408);
      match(MQL_Parser::L_PAR);
      setState(409);
      match(MQL_Parser::VARIABLE);
      setState(411);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == MQL_Parser::KEY) {
        setState(410);
        match(MQL_Parser::KEY);
      }
      setState(413);
      match(MQL_Parser::R_PAR);
      setState(415);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == MQL_Parser::K_AS) {
        setState(414);
        alias();
      }
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<MQL_Parser::ReturnItemCountContext>(_localctx);
      enterOuterAlt(_localctx, 3);
      setState(417);
      match(MQL_Parser::K_COUNT);
      setState(418);
      match(MQL_Parser::L_PAR);
      setState(420);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == MQL_Parser::K_DISTINCT) {
        setState(419);
        match(MQL_Parser::K_DISTINCT);
      }
      setState(427);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case MQL_Parser::VARIABLE: {
          setState(422);
          match(MQL_Parser::VARIABLE);
          setState(424);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == MQL_Parser::KEY) {
            setState(423);
            match(MQL_Parser::KEY);
          }
          break;
        }

        case MQL_Parser::STAR: {
          setState(426);
          match(MQL_Parser::STAR);
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      setState(429);
      match(MQL_Parser::R_PAR);
      setState(431);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == MQL_Parser::K_AS) {
        setState(430);
        alias();
      }
      break;
    }

    case 4: {
      _localctx = _tracker.createInstance<MQL_Parser::ReturnItemExprContext>(_localctx);
      enterOuterAlt(_localctx, 4);
      setState(433);
      conditionalOrExpr();
      setState(434);
      alias();
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AliasContext ------------------------------------------------------------------

MQL_Parser::AliasContext::AliasContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::AliasContext::K_AS() {
  return getToken(MQL_Parser::K_AS, 0);
}

tree::TerminalNode* MQL_Parser::AliasContext::VARIABLE() {
  return getToken(MQL_Parser::VARIABLE, 0);
}


size_t MQL_Parser::AliasContext::getRuleIndex() const {
  return MQL_Parser::RuleAlias;
}


std::any MQL_Parser::AliasContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitAlias(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::AliasContext* MQL_Parser::alias() {
  AliasContext *_localctx = _tracker.createInstance<AliasContext>(_ctx, getState());
  enterRule(_localctx, 62, MQL_Parser::RuleAlias);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(438);
    match(MQL_Parser::K_AS);
    setState(439);
    match(MQL_Parser::VARIABLE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AggregateFuncContext ------------------------------------------------------------------

MQL_Parser::AggregateFuncContext::AggregateFuncContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::AggregateFuncContext::K_SUM() {
  return getToken(MQL_Parser::K_SUM, 0);
}

tree::TerminalNode* MQL_Parser::AggregateFuncContext::K_MAX() {
  return getToken(MQL_Parser::K_MAX, 0);
}

tree::TerminalNode* MQL_Parser::AggregateFuncContext::K_MIN() {
  return getToken(MQL_Parser::K_MIN, 0);
}

tree::TerminalNode* MQL_Parser::AggregateFuncContext::K_AVG() {
  return getToken(MQL_Parser::K_AVG, 0);
}


size_t MQL_Parser::AggregateFuncContext::getRuleIndex() const {
  return MQL_Parser::RuleAggregateFunc;
}


std::any MQL_Parser::AggregateFuncContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitAggregateFunc(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::AggregateFuncContext* MQL_Parser::aggregateFunc() {
  AggregateFuncContext *_localctx = _tracker.createInstance<AggregateFuncContext>(_ctx, getState());
  enterRule(_localctx, 64, MQL_Parser::RuleAggregateFunc);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(441);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 2305849606283460640) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- OrderByItemContext ------------------------------------------------------------------

MQL_Parser::OrderByItemContext::OrderByItemContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t MQL_Parser::OrderByItemContext::getRuleIndex() const {
  return MQL_Parser::RuleOrderByItem;
}

void MQL_Parser::OrderByItemContext::copyFrom(OrderByItemContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- OrderByItemCountContext ------------------------------------------------------------------

tree::TerminalNode* MQL_Parser::OrderByItemCountContext::K_COUNT() {
  return getToken(MQL_Parser::K_COUNT, 0);
}

tree::TerminalNode* MQL_Parser::OrderByItemCountContext::L_PAR() {
  return getToken(MQL_Parser::L_PAR, 0);
}

tree::TerminalNode* MQL_Parser::OrderByItemCountContext::VARIABLE() {
  return getToken(MQL_Parser::VARIABLE, 0);
}

tree::TerminalNode* MQL_Parser::OrderByItemCountContext::R_PAR() {
  return getToken(MQL_Parser::R_PAR, 0);
}

tree::TerminalNode* MQL_Parser::OrderByItemCountContext::K_DISTINCT() {
  return getToken(MQL_Parser::K_DISTINCT, 0);
}

tree::TerminalNode* MQL_Parser::OrderByItemCountContext::KEY() {
  return getToken(MQL_Parser::KEY, 0);
}

tree::TerminalNode* MQL_Parser::OrderByItemCountContext::K_ASC() {
  return getToken(MQL_Parser::K_ASC, 0);
}

tree::TerminalNode* MQL_Parser::OrderByItemCountContext::K_DESC() {
  return getToken(MQL_Parser::K_DESC, 0);
}

MQL_Parser::OrderByItemCountContext::OrderByItemCountContext(OrderByItemContext *ctx) { copyFrom(ctx); }


std::any MQL_Parser::OrderByItemCountContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitOrderByItemCount(this);
  else
    return visitor->visitChildren(this);
}
//----------------- OrderByItemExprContext ------------------------------------------------------------------

MQL_Parser::ConditionalOrExprContext* MQL_Parser::OrderByItemExprContext::conditionalOrExpr() {
  return getRuleContext<MQL_Parser::ConditionalOrExprContext>(0);
}

tree::TerminalNode* MQL_Parser::OrderByItemExprContext::K_ASC() {
  return getToken(MQL_Parser::K_ASC, 0);
}

tree::TerminalNode* MQL_Parser::OrderByItemExprContext::K_DESC() {
  return getToken(MQL_Parser::K_DESC, 0);
}

MQL_Parser::OrderByItemExprContext::OrderByItemExprContext(OrderByItemContext *ctx) { copyFrom(ctx); }


std::any MQL_Parser::OrderByItemExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitOrderByItemExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- OrderByItemVarContext ------------------------------------------------------------------

tree::TerminalNode* MQL_Parser::OrderByItemVarContext::VARIABLE() {
  return getToken(MQL_Parser::VARIABLE, 0);
}

tree::TerminalNode* MQL_Parser::OrderByItemVarContext::KEY() {
  return getToken(MQL_Parser::KEY, 0);
}

tree::TerminalNode* MQL_Parser::OrderByItemVarContext::K_ASC() {
  return getToken(MQL_Parser::K_ASC, 0);
}

tree::TerminalNode* MQL_Parser::OrderByItemVarContext::K_DESC() {
  return getToken(MQL_Parser::K_DESC, 0);
}

MQL_Parser::OrderByItemVarContext::OrderByItemVarContext(OrderByItemContext *ctx) { copyFrom(ctx); }


std::any MQL_Parser::OrderByItemVarContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitOrderByItemVar(this);
  else
    return visitor->visitChildren(this);
}
//----------------- OrderByItemAggContext ------------------------------------------------------------------

MQL_Parser::AggregateFuncContext* MQL_Parser::OrderByItemAggContext::aggregateFunc() {
  return getRuleContext<MQL_Parser::AggregateFuncContext>(0);
}

tree::TerminalNode* MQL_Parser::OrderByItemAggContext::L_PAR() {
  return getToken(MQL_Parser::L_PAR, 0);
}

tree::TerminalNode* MQL_Parser::OrderByItemAggContext::VARIABLE() {
  return getToken(MQL_Parser::VARIABLE, 0);
}

tree::TerminalNode* MQL_Parser::OrderByItemAggContext::R_PAR() {
  return getToken(MQL_Parser::R_PAR, 0);
}

tree::TerminalNode* MQL_Parser::OrderByItemAggContext::KEY() {
  return getToken(MQL_Parser::KEY, 0);
}

tree::TerminalNode* MQL_Parser::OrderByItemAggContext::K_ASC() {
  return getToken(MQL_Parser::K_ASC, 0);
}

tree::TerminalNode* MQL_Parser::OrderByItemAggContext::K_DESC() {
  return getToken(MQL_Parser::K_DESC, 0);
}

MQL_Parser::OrderByItemAggContext::OrderByItemAggContext(OrderByItemContext *ctx) { copyFrom(ctx); }


std::any MQL_Parser::OrderByItemAggContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitOrderByItemAgg(this);
  else
    return visitor->visitChildren(this);
}
MQL_Parser::OrderByItemContext* MQL_Parser::orderByItem() {
  OrderByItemContext *_localctx = _tracker.createInstance<OrderByItemContext>(_ctx, getState());
  enterRule(_localctx, 66, MQL_Parser::RuleOrderByItem);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(477);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 54, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<MQL_Parser::OrderByItemVarContext>(_localctx);
      enterOuterAlt(_localctx, 1);
      setState(443);
      match(MQL_Parser::VARIABLE);
      setState(445);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == MQL_Parser::KEY) {
        setState(444);
        match(MQL_Parser::KEY);
      }
      setState(448);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == MQL_Parser::K_ASC

      || _la == MQL_Parser::K_DESC) {
        setState(447);
        _la = _input->LA(1);
        if (!(_la == MQL_Parser::K_ASC

        || _la == MQL_Parser::K_DESC)) {
        _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
      }
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<MQL_Parser::OrderByItemAggContext>(_localctx);
      enterOuterAlt(_localctx, 2);
      setState(450);
      aggregateFunc();
      setState(451);
      match(MQL_Parser::L_PAR);
      setState(452);
      match(MQL_Parser::VARIABLE);
      setState(454);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == MQL_Parser::KEY) {
        setState(453);
        match(MQL_Parser::KEY);
      }
      setState(456);
      match(MQL_Parser::R_PAR);
      setState(458);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == MQL_Parser::K_ASC

      || _la == MQL_Parser::K_DESC) {
        setState(457);
        _la = _input->LA(1);
        if (!(_la == MQL_Parser::K_ASC

        || _la == MQL_Parser::K_DESC)) {
        _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
      }
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<MQL_Parser::OrderByItemCountContext>(_localctx);
      enterOuterAlt(_localctx, 3);
      setState(460);
      match(MQL_Parser::K_COUNT);
      setState(461);
      match(MQL_Parser::L_PAR);
      setState(463);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == MQL_Parser::K_DISTINCT) {
        setState(462);
        match(MQL_Parser::K_DISTINCT);
      }
      setState(465);
      match(MQL_Parser::VARIABLE);
      setState(467);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == MQL_Parser::KEY) {
        setState(466);
        match(MQL_Parser::KEY);
      }
      setState(469);
      match(MQL_Parser::R_PAR);
      setState(471);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == MQL_Parser::K_ASC

      || _la == MQL_Parser::K_DESC) {
        setState(470);
        _la = _input->LA(1);
        if (!(_la == MQL_Parser::K_ASC

        || _la == MQL_Parser::K_DESC)) {
        _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
      }
      break;
    }

    case 4: {
      _localctx = _tracker.createInstance<MQL_Parser::OrderByItemExprContext>(_localctx);
      enterOuterAlt(_localctx, 4);
      setState(473);
      conditionalOrExpr();
      setState(475);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == MQL_Parser::K_ASC

      || _la == MQL_Parser::K_DESC) {
        setState(474);
        _la = _input->LA(1);
        if (!(_la == MQL_Parser::K_ASC

        || _la == MQL_Parser::K_DESC)) {
        _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
      }
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- GroupByItemContext ------------------------------------------------------------------

MQL_Parser::GroupByItemContext::GroupByItemContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::GroupByItemContext::VARIABLE() {
  return getToken(MQL_Parser::VARIABLE, 0);
}

tree::TerminalNode* MQL_Parser::GroupByItemContext::KEY() {
  return getToken(MQL_Parser::KEY, 0);
}


size_t MQL_Parser::GroupByItemContext::getRuleIndex() const {
  return MQL_Parser::RuleGroupByItem;
}


std::any MQL_Parser::GroupByItemContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitGroupByItem(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::GroupByItemContext* MQL_Parser::groupByItem() {
  GroupByItemContext *_localctx = _tracker.createInstance<GroupByItemContext>(_ctx, getState());
  enterRule(_localctx, 68, MQL_Parser::RuleGroupByItem);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(479);
    match(MQL_Parser::VARIABLE);
    setState(481);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MQL_Parser::KEY) {
      setState(480);
      match(MQL_Parser::KEY);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- GraphPatternContext ------------------------------------------------------------------

MQL_Parser::GraphPatternContext::GraphPatternContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

MQL_Parser::BasicPatternContext* MQL_Parser::GraphPatternContext::basicPattern() {
  return getRuleContext<MQL_Parser::BasicPatternContext>(0);
}

std::vector<MQL_Parser::OptionalPatternContext *> MQL_Parser::GraphPatternContext::optionalPattern() {
  return getRuleContexts<MQL_Parser::OptionalPatternContext>();
}

MQL_Parser::OptionalPatternContext* MQL_Parser::GraphPatternContext::optionalPattern(size_t i) {
  return getRuleContext<MQL_Parser::OptionalPatternContext>(i);
}


size_t MQL_Parser::GraphPatternContext::getRuleIndex() const {
  return MQL_Parser::RuleGraphPattern;
}


std::any MQL_Parser::GraphPatternContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitGraphPattern(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::GraphPatternContext* MQL_Parser::graphPattern() {
  GraphPatternContext *_localctx = _tracker.createInstance<GraphPatternContext>(_ctx, getState());
  enterRule(_localctx, 70, MQL_Parser::RuleGraphPattern);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(483);
    basicPattern();
    setState(487);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MQL_Parser::K_OPTIONAL) {
      setState(484);
      optionalPattern();
      setState(489);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- OptionalPatternContext ------------------------------------------------------------------

MQL_Parser::OptionalPatternContext::OptionalPatternContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::OptionalPatternContext::K_OPTIONAL() {
  return getToken(MQL_Parser::K_OPTIONAL, 0);
}

tree::TerminalNode* MQL_Parser::OptionalPatternContext::LCURLY_BRACKET() {
  return getToken(MQL_Parser::LCURLY_BRACKET, 0);
}

MQL_Parser::GraphPatternContext* MQL_Parser::OptionalPatternContext::graphPattern() {
  return getRuleContext<MQL_Parser::GraphPatternContext>(0);
}

tree::TerminalNode* MQL_Parser::OptionalPatternContext::RCURLY_BRACKET() {
  return getToken(MQL_Parser::RCURLY_BRACKET, 0);
}


size_t MQL_Parser::OptionalPatternContext::getRuleIndex() const {
  return MQL_Parser::RuleOptionalPattern;
}


std::any MQL_Parser::OptionalPatternContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitOptionalPattern(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::OptionalPatternContext* MQL_Parser::optionalPattern() {
  OptionalPatternContext *_localctx = _tracker.createInstance<OptionalPatternContext>(_ctx, getState());
  enterRule(_localctx, 72, MQL_Parser::RuleOptionalPattern);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(490);
    match(MQL_Parser::K_OPTIONAL);
    setState(491);
    match(MQL_Parser::LCURLY_BRACKET);
    setState(492);
    graphPattern();
    setState(493);
    match(MQL_Parser::RCURLY_BRACKET);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BasicPatternContext ------------------------------------------------------------------

MQL_Parser::BasicPatternContext::BasicPatternContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<MQL_Parser::LinearPatternContext *> MQL_Parser::BasicPatternContext::linearPattern() {
  return getRuleContexts<MQL_Parser::LinearPatternContext>();
}

MQL_Parser::LinearPatternContext* MQL_Parser::BasicPatternContext::linearPattern(size_t i) {
  return getRuleContext<MQL_Parser::LinearPatternContext>(i);
}

std::vector<tree::TerminalNode *> MQL_Parser::BasicPatternContext::COMMA() {
  return getTokens(MQL_Parser::COMMA);
}

tree::TerminalNode* MQL_Parser::BasicPatternContext::COMMA(size_t i) {
  return getToken(MQL_Parser::COMMA, i);
}


size_t MQL_Parser::BasicPatternContext::getRuleIndex() const {
  return MQL_Parser::RuleBasicPattern;
}


std::any MQL_Parser::BasicPatternContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitBasicPattern(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::BasicPatternContext* MQL_Parser::basicPattern() {
  BasicPatternContext *_localctx = _tracker.createInstance<BasicPatternContext>(_ctx, getState());
  enterRule(_localctx, 74, MQL_Parser::RuleBasicPattern);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(495);
    linearPattern();
    setState(500);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MQL_Parser::COMMA) {
      setState(496);
      match(MQL_Parser::COMMA);
      setState(497);
      linearPattern();
      setState(502);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LinearPatternContext ------------------------------------------------------------------

MQL_Parser::LinearPatternContext::LinearPatternContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<MQL_Parser::NodeContext *> MQL_Parser::LinearPatternContext::node() {
  return getRuleContexts<MQL_Parser::NodeContext>();
}

MQL_Parser::NodeContext* MQL_Parser::LinearPatternContext::node(size_t i) {
  return getRuleContext<MQL_Parser::NodeContext>(i);
}

std::vector<MQL_Parser::EdgeContext *> MQL_Parser::LinearPatternContext::edge() {
  return getRuleContexts<MQL_Parser::EdgeContext>();
}

MQL_Parser::EdgeContext* MQL_Parser::LinearPatternContext::edge(size_t i) {
  return getRuleContext<MQL_Parser::EdgeContext>(i);
}

std::vector<MQL_Parser::PathContext *> MQL_Parser::LinearPatternContext::path() {
  return getRuleContexts<MQL_Parser::PathContext>();
}

MQL_Parser::PathContext* MQL_Parser::LinearPatternContext::path(size_t i) {
  return getRuleContext<MQL_Parser::PathContext>(i);
}


size_t MQL_Parser::LinearPatternContext::getRuleIndex() const {
  return MQL_Parser::RuleLinearPattern;
}


std::any MQL_Parser::LinearPatternContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitLinearPattern(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::LinearPatternContext* MQL_Parser::linearPattern() {
  LinearPatternContext *_localctx = _tracker.createInstance<LinearPatternContext>(_ctx, getState());
  enterRule(_localctx, 76, MQL_Parser::RuleLinearPattern);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(503);
    node();
    setState(512);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (((((_la - 83) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 83)) & 16465) != 0)) {
      setState(506);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case MQL_Parser::LT:
        case MQL_Parser::MINUS: {
          setState(504);
          edge();
          break;
        }

        case MQL_Parser::LEQ:
        case MQL_Parser::SINGLE_EQ: {
          setState(505);
          path();
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      setState(508);
      node();
      setState(514);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PathContext ------------------------------------------------------------------

MQL_Parser::PathContext::PathContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::PathContext::LEQ() {
  return getToken(MQL_Parser::LEQ, 0);
}

tree::TerminalNode* MQL_Parser::PathContext::LSQUARE_BRACKET() {
  return getToken(MQL_Parser::LSQUARE_BRACKET, 0);
}

MQL_Parser::PathAlternativesContext* MQL_Parser::PathContext::pathAlternatives() {
  return getRuleContext<MQL_Parser::PathAlternativesContext>(0);
}

tree::TerminalNode* MQL_Parser::PathContext::RSQUARE_BRACKET() {
  return getToken(MQL_Parser::RSQUARE_BRACKET, 0);
}

std::vector<tree::TerminalNode *> MQL_Parser::PathContext::SINGLE_EQ() {
  return getTokens(MQL_Parser::SINGLE_EQ);
}

tree::TerminalNode* MQL_Parser::PathContext::SINGLE_EQ(size_t i) {
  return getToken(MQL_Parser::SINGLE_EQ, i);
}

MQL_Parser::PathTypeContext* MQL_Parser::PathContext::pathType() {
  return getRuleContext<MQL_Parser::PathTypeContext>(0);
}

tree::TerminalNode* MQL_Parser::PathContext::VARIABLE() {
  return getToken(MQL_Parser::VARIABLE, 0);
}

tree::TerminalNode* MQL_Parser::PathContext::GT() {
  return getToken(MQL_Parser::GT, 0);
}


size_t MQL_Parser::PathContext::getRuleIndex() const {
  return MQL_Parser::RulePath;
}


std::any MQL_Parser::PathContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitPath(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::PathContext* MQL_Parser::path() {
  PathContext *_localctx = _tracker.createInstance<PathContext>(_ctx, getState());
  enterRule(_localctx, 78, MQL_Parser::RulePath);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(540);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case MQL_Parser::LEQ: {
        enterOuterAlt(_localctx, 1);
        setState(515);
        match(MQL_Parser::LEQ);
        setState(516);
        match(MQL_Parser::LSQUARE_BRACKET);
        setState(518);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if ((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & 36028797018964040) != 0)) {
          setState(517);
          pathType();
        }
        setState(521);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == MQL_Parser::VARIABLE) {
          setState(520);
          match(MQL_Parser::VARIABLE);
        }
        setState(523);
        pathAlternatives();
        setState(524);
        match(MQL_Parser::RSQUARE_BRACKET);
        setState(525);
        match(MQL_Parser::SINGLE_EQ);
        break;
      }

      case MQL_Parser::SINGLE_EQ: {
        enterOuterAlt(_localctx, 2);
        setState(527);
        match(MQL_Parser::SINGLE_EQ);
        setState(528);
        match(MQL_Parser::LSQUARE_BRACKET);
        setState(530);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if ((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & 36028797018964040) != 0)) {
          setState(529);
          pathType();
        }
        setState(533);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == MQL_Parser::VARIABLE) {
          setState(532);
          match(MQL_Parser::VARIABLE);
        }
        setState(535);
        pathAlternatives();
        setState(536);
        match(MQL_Parser::RSQUARE_BRACKET);
        setState(537);
        match(MQL_Parser::SINGLE_EQ);
        setState(538);
        match(MQL_Parser::GT);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PathAlternativesContext ------------------------------------------------------------------

MQL_Parser::PathAlternativesContext::PathAlternativesContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<MQL_Parser::PathSequenceContext *> MQL_Parser::PathAlternativesContext::pathSequence() {
  return getRuleContexts<MQL_Parser::PathSequenceContext>();
}

MQL_Parser::PathSequenceContext* MQL_Parser::PathAlternativesContext::pathSequence(size_t i) {
  return getRuleContext<MQL_Parser::PathSequenceContext>(i);
}

std::vector<tree::TerminalNode *> MQL_Parser::PathAlternativesContext::PATH_ALTERNATIVE() {
  return getTokens(MQL_Parser::PATH_ALTERNATIVE);
}

tree::TerminalNode* MQL_Parser::PathAlternativesContext::PATH_ALTERNATIVE(size_t i) {
  return getToken(MQL_Parser::PATH_ALTERNATIVE, i);
}


size_t MQL_Parser::PathAlternativesContext::getRuleIndex() const {
  return MQL_Parser::RulePathAlternatives;
}


std::any MQL_Parser::PathAlternativesContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitPathAlternatives(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::PathAlternativesContext* MQL_Parser::pathAlternatives() {
  PathAlternativesContext *_localctx = _tracker.createInstance<PathAlternativesContext>(_ctx, getState());
  enterRule(_localctx, 80, MQL_Parser::RulePathAlternatives);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(542);
    pathSequence();
    setState(547);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MQL_Parser::PATH_ALTERNATIVE) {
      setState(543);
      match(MQL_Parser::PATH_ALTERNATIVE);
      setState(544);
      pathSequence();
      setState(549);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PathSequenceContext ------------------------------------------------------------------

MQL_Parser::PathSequenceContext::PathSequenceContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<MQL_Parser::PathAtomContext *> MQL_Parser::PathSequenceContext::pathAtom() {
  return getRuleContexts<MQL_Parser::PathAtomContext>();
}

MQL_Parser::PathAtomContext* MQL_Parser::PathSequenceContext::pathAtom(size_t i) {
  return getRuleContext<MQL_Parser::PathAtomContext>(i);
}

std::vector<tree::TerminalNode *> MQL_Parser::PathSequenceContext::PATH_SEQUENCE() {
  return getTokens(MQL_Parser::PATH_SEQUENCE);
}

tree::TerminalNode* MQL_Parser::PathSequenceContext::PATH_SEQUENCE(size_t i) {
  return getToken(MQL_Parser::PATH_SEQUENCE, i);
}


size_t MQL_Parser::PathSequenceContext::getRuleIndex() const {
  return MQL_Parser::RulePathSequence;
}


std::any MQL_Parser::PathSequenceContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitPathSequence(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::PathSequenceContext* MQL_Parser::pathSequence() {
  PathSequenceContext *_localctx = _tracker.createInstance<PathSequenceContext>(_ctx, getState());
  enterRule(_localctx, 82, MQL_Parser::RulePathSequence);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(550);
    pathAtom();
    setState(555);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MQL_Parser::PATH_SEQUENCE) {
      setState(551);
      match(MQL_Parser::PATH_SEQUENCE);
      setState(552);
      pathAtom();
      setState(557);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PathAtomContext ------------------------------------------------------------------

MQL_Parser::PathAtomContext::PathAtomContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t MQL_Parser::PathAtomContext::getRuleIndex() const {
  return MQL_Parser::RulePathAtom;
}

void MQL_Parser::PathAtomContext::copyFrom(PathAtomContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- PathAtomAlternativesContext ------------------------------------------------------------------

tree::TerminalNode* MQL_Parser::PathAtomAlternativesContext::L_PAR() {
  return getToken(MQL_Parser::L_PAR, 0);
}

MQL_Parser::PathAlternativesContext* MQL_Parser::PathAtomAlternativesContext::pathAlternatives() {
  return getRuleContext<MQL_Parser::PathAlternativesContext>(0);
}

tree::TerminalNode* MQL_Parser::PathAtomAlternativesContext::R_PAR() {
  return getToken(MQL_Parser::R_PAR, 0);
}

tree::TerminalNode* MQL_Parser::PathAtomAlternativesContext::PATH_NEGATION() {
  return getToken(MQL_Parser::PATH_NEGATION, 0);
}

MQL_Parser::PathSuffixContext* MQL_Parser::PathAtomAlternativesContext::pathSuffix() {
  return getRuleContext<MQL_Parser::PathSuffixContext>(0);
}

MQL_Parser::PathAtomAlternativesContext::PathAtomAlternativesContext(PathAtomContext *ctx) { copyFrom(ctx); }


std::any MQL_Parser::PathAtomAlternativesContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitPathAtomAlternatives(this);
  else
    return visitor->visitChildren(this);
}
//----------------- PathAtomSimpleContext ------------------------------------------------------------------

tree::TerminalNode* MQL_Parser::PathAtomSimpleContext::TYPE() {
  return getToken(MQL_Parser::TYPE, 0);
}

tree::TerminalNode* MQL_Parser::PathAtomSimpleContext::PATH_NEGATION() {
  return getToken(MQL_Parser::PATH_NEGATION, 0);
}

MQL_Parser::PathSuffixContext* MQL_Parser::PathAtomSimpleContext::pathSuffix() {
  return getRuleContext<MQL_Parser::PathSuffixContext>(0);
}

MQL_Parser::PathAtomSimpleContext::PathAtomSimpleContext(PathAtomContext *ctx) { copyFrom(ctx); }


std::any MQL_Parser::PathAtomSimpleContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitPathAtomSimple(this);
  else
    return visitor->visitChildren(this);
}
MQL_Parser::PathAtomContext* MQL_Parser::pathAtom() {
  PathAtomContext *_localctx = _tracker.createInstance<PathAtomContext>(_ctx, getState());
  enterRule(_localctx, 84, MQL_Parser::RulePathAtom);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(574);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 71, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<MQL_Parser::PathAtomSimpleContext>(_localctx);
      enterOuterAlt(_localctx, 1);
      setState(559);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == MQL_Parser::PATH_NEGATION) {
        setState(558);
        match(MQL_Parser::PATH_NEGATION);
      }
      setState(561);
      match(MQL_Parser::TYPE);
      setState(563);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (((((_la - 93) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 93)) & 141) != 0)) {
        setState(562);
        pathSuffix();
      }
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<MQL_Parser::PathAtomAlternativesContext>(_localctx);
      enterOuterAlt(_localctx, 2);
      setState(566);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == MQL_Parser::PATH_NEGATION) {
        setState(565);
        match(MQL_Parser::PATH_NEGATION);
      }
      setState(568);
      match(MQL_Parser::L_PAR);
      setState(569);
      pathAlternatives();
      setState(570);
      match(MQL_Parser::R_PAR);
      setState(572);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (((((_la - 93) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 93)) & 141) != 0)) {
        setState(571);
        pathSuffix();
      }
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PathSuffixContext ------------------------------------------------------------------

MQL_Parser::PathSuffixContext::PathSuffixContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::PathSuffixContext::STAR() {
  return getToken(MQL_Parser::STAR, 0);
}

tree::TerminalNode* MQL_Parser::PathSuffixContext::PLUS() {
  return getToken(MQL_Parser::PLUS, 0);
}

tree::TerminalNode* MQL_Parser::PathSuffixContext::QUESTION_MARK() {
  return getToken(MQL_Parser::QUESTION_MARK, 0);
}

tree::TerminalNode* MQL_Parser::PathSuffixContext::LCURLY_BRACKET() {
  return getToken(MQL_Parser::LCURLY_BRACKET, 0);
}

tree::TerminalNode* MQL_Parser::PathSuffixContext::COMMA() {
  return getToken(MQL_Parser::COMMA, 0);
}

tree::TerminalNode* MQL_Parser::PathSuffixContext::RCURLY_BRACKET() {
  return getToken(MQL_Parser::RCURLY_BRACKET, 0);
}

std::vector<tree::TerminalNode *> MQL_Parser::PathSuffixContext::UNSIGNED_INTEGER() {
  return getTokens(MQL_Parser::UNSIGNED_INTEGER);
}

tree::TerminalNode* MQL_Parser::PathSuffixContext::UNSIGNED_INTEGER(size_t i) {
  return getToken(MQL_Parser::UNSIGNED_INTEGER, i);
}


size_t MQL_Parser::PathSuffixContext::getRuleIndex() const {
  return MQL_Parser::RulePathSuffix;
}


std::any MQL_Parser::PathSuffixContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitPathSuffix(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::PathSuffixContext* MQL_Parser::pathSuffix() {
  PathSuffixContext *_localctx = _tracker.createInstance<PathSuffixContext>(_ctx, getState());
  enterRule(_localctx, 86, MQL_Parser::RulePathSuffix);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(584);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case MQL_Parser::STAR: {
        enterOuterAlt(_localctx, 1);
        setState(576);
        antlrcpp::downCast<PathSuffixContext *>(_localctx)->op = match(MQL_Parser::STAR);
        break;
      }

      case MQL_Parser::PLUS: {
        enterOuterAlt(_localctx, 2);
        setState(577);
        antlrcpp::downCast<PathSuffixContext *>(_localctx)->op = match(MQL_Parser::PLUS);
        break;
      }

      case MQL_Parser::QUESTION_MARK: {
        enterOuterAlt(_localctx, 3);
        setState(578);
        antlrcpp::downCast<PathSuffixContext *>(_localctx)->op = match(MQL_Parser::QUESTION_MARK);
        break;
      }

      case MQL_Parser::LCURLY_BRACKET: {
        enterOuterAlt(_localctx, 4);
        setState(579);
        match(MQL_Parser::LCURLY_BRACKET);
        setState(580);
        antlrcpp::downCast<PathSuffixContext *>(_localctx)->min = match(MQL_Parser::UNSIGNED_INTEGER);
        setState(581);
        match(MQL_Parser::COMMA);
        setState(582);
        antlrcpp::downCast<PathSuffixContext *>(_localctx)->max = match(MQL_Parser::UNSIGNED_INTEGER);
        setState(583);
        match(MQL_Parser::RCURLY_BRACKET);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PathTypeContext ------------------------------------------------------------------

MQL_Parser::PathTypeContext::PathTypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::PathTypeContext::K_ANY() {
  return getToken(MQL_Parser::K_ANY, 0);
}

tree::TerminalNode* MQL_Parser::PathTypeContext::K_ALL() {
  return getToken(MQL_Parser::K_ALL, 0);
}

tree::TerminalNode* MQL_Parser::PathTypeContext::K_SHORTEST() {
  return getToken(MQL_Parser::K_SHORTEST, 0);
}

tree::TerminalNode* MQL_Parser::PathTypeContext::K_WALKS() {
  return getToken(MQL_Parser::K_WALKS, 0);
}

tree::TerminalNode* MQL_Parser::PathTypeContext::K_SIMPLE() {
  return getToken(MQL_Parser::K_SIMPLE, 0);
}

tree::TerminalNode* MQL_Parser::PathTypeContext::K_ACYCLIC() {
  return getToken(MQL_Parser::K_ACYCLIC, 0);
}

tree::TerminalNode* MQL_Parser::PathTypeContext::K_TRAILS() {
  return getToken(MQL_Parser::K_TRAILS, 0);
}

tree::TerminalNode* MQL_Parser::PathTypeContext::UNSIGNED_INTEGER() {
  return getToken(MQL_Parser::UNSIGNED_INTEGER, 0);
}

tree::TerminalNode* MQL_Parser::PathTypeContext::K_GROUPS() {
  return getToken(MQL_Parser::K_GROUPS, 0);
}


size_t MQL_Parser::PathTypeContext::getRuleIndex() const {
  return MQL_Parser::RulePathType;
}


std::any MQL_Parser::PathTypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitPathType(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::PathTypeContext* MQL_Parser::pathType() {
  PathTypeContext *_localctx = _tracker.createInstance<PathTypeContext>(_ctx, getState());
  enterRule(_localctx, 88, MQL_Parser::RulePathType);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(601);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case MQL_Parser::K_ANY:
      case MQL_Parser::K_ALL: {
        enterOuterAlt(_localctx, 1);
        setState(586);
        _la = _input->LA(1);
        if (!(_la == MQL_Parser::K_ANY

        || _la == MQL_Parser::K_ALL)) {
        _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        setState(588);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == MQL_Parser::K_SHORTEST) {
          setState(587);
          match(MQL_Parser::K_SHORTEST);
        }
        setState(591);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == MQL_Parser::K_ACYCLIC

        || _la == MQL_Parser::K_SIMPLE || _la == MQL_Parser::K_TRAILS

        || _la == MQL_Parser::K_WALKS) {
          setState(590);
          _la = _input->LA(1);
          if (!(_la == MQL_Parser::K_ACYCLIC

          || _la == MQL_Parser::K_SIMPLE || _la == MQL_Parser::K_TRAILS

          || _la == MQL_Parser::K_WALKS)) {
          _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
        }
        break;
      }

      case MQL_Parser::K_SHORTEST: {
        enterOuterAlt(_localctx, 2);
        setState(593);
        match(MQL_Parser::K_SHORTEST);
        setState(594);
        match(MQL_Parser::UNSIGNED_INTEGER);
        setState(596);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == MQL_Parser::K_GROUPS) {
          setState(595);
          match(MQL_Parser::K_GROUPS);
        }
        setState(599);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == MQL_Parser::K_ACYCLIC

        || _la == MQL_Parser::K_SIMPLE || _la == MQL_Parser::K_TRAILS

        || _la == MQL_Parser::K_WALKS) {
          setState(598);
          _la = _input->LA(1);
          if (!(_la == MQL_Parser::K_ACYCLIC

          || _la == MQL_Parser::K_SIMPLE || _la == MQL_Parser::K_TRAILS

          || _la == MQL_Parser::K_WALKS)) {
          _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
        }
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- NodeContext ------------------------------------------------------------------

MQL_Parser::NodeContext::NodeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

MQL_Parser::FixedNodeContext* MQL_Parser::NodeContext::fixedNode() {
  return getRuleContext<MQL_Parser::FixedNodeContext>(0);
}

MQL_Parser::VarNodeContext* MQL_Parser::NodeContext::varNode() {
  return getRuleContext<MQL_Parser::VarNodeContext>(0);
}


size_t MQL_Parser::NodeContext::getRuleIndex() const {
  return MQL_Parser::RuleNode;
}


std::any MQL_Parser::NodeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitNode(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::NodeContext* MQL_Parser::node() {
  NodeContext *_localctx = _tracker.createInstance<NodeContext>(_ctx, getState());
  enterRule(_localctx, 90, MQL_Parser::RuleNode);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(605);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 78, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(603);
      fixedNode();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(604);
      varNode();
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FixedNodeContext ------------------------------------------------------------------

MQL_Parser::FixedNodeContext::FixedNodeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::FixedNodeContext::L_PAR() {
  return getToken(MQL_Parser::L_PAR, 0);
}

MQL_Parser::FixedNodeInsideContext* MQL_Parser::FixedNodeContext::fixedNodeInside() {
  return getRuleContext<MQL_Parser::FixedNodeInsideContext>(0);
}

tree::TerminalNode* MQL_Parser::FixedNodeContext::R_PAR() {
  return getToken(MQL_Parser::R_PAR, 0);
}


size_t MQL_Parser::FixedNodeContext::getRuleIndex() const {
  return MQL_Parser::RuleFixedNode;
}


std::any MQL_Parser::FixedNodeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitFixedNode(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::FixedNodeContext* MQL_Parser::fixedNode() {
  FixedNodeContext *_localctx = _tracker.createInstance<FixedNodeContext>(_ctx, getState());
  enterRule(_localctx, 92, MQL_Parser::RuleFixedNode);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(607);
    match(MQL_Parser::L_PAR);
    setState(608);
    fixedNodeInside();
    setState(609);
    match(MQL_Parser::R_PAR);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FixedNodeInsideContext ------------------------------------------------------------------

MQL_Parser::FixedNodeInsideContext::FixedNodeInsideContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

MQL_Parser::IdentifierContext* MQL_Parser::FixedNodeInsideContext::identifier() {
  return getRuleContext<MQL_Parser::IdentifierContext>(0);
}

tree::TerminalNode* MQL_Parser::FixedNodeInsideContext::ANON_ID() {
  return getToken(MQL_Parser::ANON_ID, 0);
}

tree::TerminalNode* MQL_Parser::FixedNodeInsideContext::EDGE_ID() {
  return getToken(MQL_Parser::EDGE_ID, 0);
}

MQL_Parser::ValueContext* MQL_Parser::FixedNodeInsideContext::value() {
  return getRuleContext<MQL_Parser::ValueContext>(0);
}


size_t MQL_Parser::FixedNodeInsideContext::getRuleIndex() const {
  return MQL_Parser::RuleFixedNodeInside;
}


std::any MQL_Parser::FixedNodeInsideContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitFixedNodeInside(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::FixedNodeInsideContext* MQL_Parser::fixedNodeInside() {
  FixedNodeInsideContext *_localctx = _tracker.createInstance<FixedNodeInsideContext>(_ctx, getState());
  enterRule(_localctx, 94, MQL_Parser::RuleFixedNodeInside);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(615);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 79, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(611);
      identifier();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(612);
      match(MQL_Parser::ANON_ID);
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(613);
      match(MQL_Parser::EDGE_ID);
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(614);
      value();
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- EdgeContext ------------------------------------------------------------------

MQL_Parser::EdgeContext::EdgeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::EdgeContext::LT() {
  return getToken(MQL_Parser::LT, 0);
}

std::vector<tree::TerminalNode *> MQL_Parser::EdgeContext::MINUS() {
  return getTokens(MQL_Parser::MINUS);
}

tree::TerminalNode* MQL_Parser::EdgeContext::MINUS(size_t i) {
  return getToken(MQL_Parser::MINUS, i);
}

MQL_Parser::EdgeInsideContext* MQL_Parser::EdgeContext::edgeInside() {
  return getRuleContext<MQL_Parser::EdgeInsideContext>(0);
}

tree::TerminalNode* MQL_Parser::EdgeContext::GT() {
  return getToken(MQL_Parser::GT, 0);
}


size_t MQL_Parser::EdgeContext::getRuleIndex() const {
  return MQL_Parser::RuleEdge;
}


std::any MQL_Parser::EdgeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitEdge(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::EdgeContext* MQL_Parser::edge() {
  EdgeContext *_localctx = _tracker.createInstance<EdgeContext>(_ctx, getState());
  enterRule(_localctx, 96, MQL_Parser::RuleEdge);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(630);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case MQL_Parser::LT: {
        enterOuterAlt(_localctx, 1);
        setState(617);
        match(MQL_Parser::LT);
        setState(620);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 80, _ctx)) {
        case 1: {
          setState(618);
          match(MQL_Parser::MINUS);
          setState(619);
          edgeInside();
          break;
        }

        default:
          break;
        }
        setState(622);
        match(MQL_Parser::MINUS);
        break;
      }

      case MQL_Parser::MINUS: {
        enterOuterAlt(_localctx, 2);
        setState(623);
        match(MQL_Parser::MINUS);
        setState(627);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == MQL_Parser::LSQUARE_BRACKET) {
          setState(624);
          edgeInside();
          setState(625);
          match(MQL_Parser::MINUS);
        }
        setState(629);
        match(MQL_Parser::GT);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- EdgeInsideContext ------------------------------------------------------------------

MQL_Parser::EdgeInsideContext::EdgeInsideContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::EdgeInsideContext::LSQUARE_BRACKET() {
  return getToken(MQL_Parser::LSQUARE_BRACKET, 0);
}

tree::TerminalNode* MQL_Parser::EdgeInsideContext::RSQUARE_BRACKET() {
  return getToken(MQL_Parser::RSQUARE_BRACKET, 0);
}

MQL_Parser::PropertiesContext* MQL_Parser::EdgeInsideContext::properties() {
  return getRuleContext<MQL_Parser::PropertiesContext>(0);
}

tree::TerminalNode* MQL_Parser::EdgeInsideContext::VARIABLE() {
  return getToken(MQL_Parser::VARIABLE, 0);
}

tree::TerminalNode* MQL_Parser::EdgeInsideContext::EDGE_ID() {
  return getToken(MQL_Parser::EDGE_ID, 0);
}

tree::TerminalNode* MQL_Parser::EdgeInsideContext::TYPE() {
  return getToken(MQL_Parser::TYPE, 0);
}

tree::TerminalNode* MQL_Parser::EdgeInsideContext::TYPE_VAR() {
  return getToken(MQL_Parser::TYPE_VAR, 0);
}


size_t MQL_Parser::EdgeInsideContext::getRuleIndex() const {
  return MQL_Parser::RuleEdgeInside;
}


std::any MQL_Parser::EdgeInsideContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitEdgeInside(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::EdgeInsideContext* MQL_Parser::edgeInside() {
  EdgeInsideContext *_localctx = _tracker.createInstance<EdgeInsideContext>(_ctx, getState());
  enterRule(_localctx, 98, MQL_Parser::RuleEdgeInside);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(632);
    match(MQL_Parser::LSQUARE_BRACKET);
    setState(634);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MQL_Parser::EDGE_ID

    || _la == MQL_Parser::VARIABLE) {
      setState(633);
      _la = _input->LA(1);
      if (!(_la == MQL_Parser::EDGE_ID

      || _la == MQL_Parser::VARIABLE)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
    }
    setState(637);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MQL_Parser::TYPE

    || _la == MQL_Parser::TYPE_VAR) {
      setState(636);
      _la = _input->LA(1);
      if (!(_la == MQL_Parser::TYPE

      || _la == MQL_Parser::TYPE_VAR)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
    }
    setState(640);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MQL_Parser::LCURLY_BRACKET) {
      setState(639);
      properties();
    }
    setState(642);
    match(MQL_Parser::RSQUARE_BRACKET);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- VarNodeContext ------------------------------------------------------------------

MQL_Parser::VarNodeContext::VarNodeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::VarNodeContext::L_PAR() {
  return getToken(MQL_Parser::L_PAR, 0);
}

tree::TerminalNode* MQL_Parser::VarNodeContext::R_PAR() {
  return getToken(MQL_Parser::R_PAR, 0);
}

tree::TerminalNode* MQL_Parser::VarNodeContext::VARIABLE() {
  return getToken(MQL_Parser::VARIABLE, 0);
}

std::vector<tree::TerminalNode *> MQL_Parser::VarNodeContext::TYPE() {
  return getTokens(MQL_Parser::TYPE);
}

tree::TerminalNode* MQL_Parser::VarNodeContext::TYPE(size_t i) {
  return getToken(MQL_Parser::TYPE, i);
}

MQL_Parser::PropertiesContext* MQL_Parser::VarNodeContext::properties() {
  return getRuleContext<MQL_Parser::PropertiesContext>(0);
}


size_t MQL_Parser::VarNodeContext::getRuleIndex() const {
  return MQL_Parser::RuleVarNode;
}


std::any MQL_Parser::VarNodeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitVarNode(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::VarNodeContext* MQL_Parser::varNode() {
  VarNodeContext *_localctx = _tracker.createInstance<VarNodeContext>(_ctx, getState());
  enterRule(_localctx, 100, MQL_Parser::RuleVarNode);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(644);
    match(MQL_Parser::L_PAR);
    setState(646);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MQL_Parser::VARIABLE) {
      setState(645);
      match(MQL_Parser::VARIABLE);
    }
    setState(651);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MQL_Parser::TYPE) {
      setState(648);
      match(MQL_Parser::TYPE);
      setState(653);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(655);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MQL_Parser::LCURLY_BRACKET) {
      setState(654);
      properties();
    }
    setState(657);
    match(MQL_Parser::R_PAR);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PropertiesContext ------------------------------------------------------------------

MQL_Parser::PropertiesContext::PropertiesContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::PropertiesContext::LCURLY_BRACKET() {
  return getToken(MQL_Parser::LCURLY_BRACKET, 0);
}

std::vector<MQL_Parser::PropertyContext *> MQL_Parser::PropertiesContext::property() {
  return getRuleContexts<MQL_Parser::PropertyContext>();
}

MQL_Parser::PropertyContext* MQL_Parser::PropertiesContext::property(size_t i) {
  return getRuleContext<MQL_Parser::PropertyContext>(i);
}

tree::TerminalNode* MQL_Parser::PropertiesContext::RCURLY_BRACKET() {
  return getToken(MQL_Parser::RCURLY_BRACKET, 0);
}

std::vector<tree::TerminalNode *> MQL_Parser::PropertiesContext::COMMA() {
  return getTokens(MQL_Parser::COMMA);
}

tree::TerminalNode* MQL_Parser::PropertiesContext::COMMA(size_t i) {
  return getToken(MQL_Parser::COMMA, i);
}


size_t MQL_Parser::PropertiesContext::getRuleIndex() const {
  return MQL_Parser::RuleProperties;
}


std::any MQL_Parser::PropertiesContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitProperties(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::PropertiesContext* MQL_Parser::properties() {
  PropertiesContext *_localctx = _tracker.createInstance<PropertiesContext>(_ctx, getState());
  enterRule(_localctx, 102, MQL_Parser::RuleProperties);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(659);
    match(MQL_Parser::LCURLY_BRACKET);
    setState(660);
    property();
    setState(665);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MQL_Parser::COMMA) {
      setState(661);
      match(MQL_Parser::COMMA);
      setState(662);
      property();
      setState(667);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(668);
    match(MQL_Parser::RCURLY_BRACKET);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PropertyContext ------------------------------------------------------------------

MQL_Parser::PropertyContext::PropertyContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t MQL_Parser::PropertyContext::getRuleIndex() const {
  return MQL_Parser::RuleProperty;
}

void MQL_Parser::PropertyContext::copyFrom(PropertyContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- Property2Context ------------------------------------------------------------------

MQL_Parser::IdentifierContext* MQL_Parser::Property2Context::identifier() {
  return getRuleContext<MQL_Parser::IdentifierContext>(0);
}

tree::TerminalNode* MQL_Parser::Property2Context::TYPE() {
  return getToken(MQL_Parser::TYPE, 0);
}

tree::TerminalNode* MQL_Parser::Property2Context::L_PAR() {
  return getToken(MQL_Parser::L_PAR, 0);
}

tree::TerminalNode* MQL_Parser::Property2Context::STRING() {
  return getToken(MQL_Parser::STRING, 0);
}

tree::TerminalNode* MQL_Parser::Property2Context::R_PAR() {
  return getToken(MQL_Parser::R_PAR, 0);
}

MQL_Parser::Property2Context::Property2Context(PropertyContext *ctx) { copyFrom(ctx); }


std::any MQL_Parser::Property2Context::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitProperty2(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Property1Context ------------------------------------------------------------------

MQL_Parser::IdentifierContext* MQL_Parser::Property1Context::identifier() {
  return getRuleContext<MQL_Parser::IdentifierContext>(0);
}

tree::TerminalNode* MQL_Parser::Property1Context::COLON() {
  return getToken(MQL_Parser::COLON, 0);
}

MQL_Parser::ValueContext* MQL_Parser::Property1Context::value() {
  return getRuleContext<MQL_Parser::ValueContext>(0);
}

tree::TerminalNode* MQL_Parser::Property1Context::TRUE_PROP() {
  return getToken(MQL_Parser::TRUE_PROP, 0);
}

tree::TerminalNode* MQL_Parser::Property1Context::FALSE_PROP() {
  return getToken(MQL_Parser::FALSE_PROP, 0);
}

MQL_Parser::Property1Context::Property1Context(PropertyContext *ctx) { copyFrom(ctx); }


std::any MQL_Parser::Property1Context::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitProperty1(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Property4Context ------------------------------------------------------------------

MQL_Parser::IdentifierContext* MQL_Parser::Property4Context::identifier() {
  return getRuleContext<MQL_Parser::IdentifierContext>(0);
}

MQL_Parser::ValueContext* MQL_Parser::Property4Context::value() {
  return getRuleContext<MQL_Parser::ValueContext>(0);
}

tree::TerminalNode* MQL_Parser::Property4Context::EQ() {
  return getToken(MQL_Parser::EQ, 0);
}

tree::TerminalNode* MQL_Parser::Property4Context::NEQ() {
  return getToken(MQL_Parser::NEQ, 0);
}

tree::TerminalNode* MQL_Parser::Property4Context::LT() {
  return getToken(MQL_Parser::LT, 0);
}

tree::TerminalNode* MQL_Parser::Property4Context::GT() {
  return getToken(MQL_Parser::GT, 0);
}

tree::TerminalNode* MQL_Parser::Property4Context::LEQ() {
  return getToken(MQL_Parser::LEQ, 0);
}

tree::TerminalNode* MQL_Parser::Property4Context::GEQ() {
  return getToken(MQL_Parser::GEQ, 0);
}

MQL_Parser::Property4Context::Property4Context(PropertyContext *ctx) { copyFrom(ctx); }


std::any MQL_Parser::Property4Context::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitProperty4(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Property3Context ------------------------------------------------------------------

MQL_Parser::IdentifierContext* MQL_Parser::Property3Context::identifier() {
  return getRuleContext<MQL_Parser::IdentifierContext>(0);
}

tree::TerminalNode* MQL_Parser::Property3Context::K_IS() {
  return getToken(MQL_Parser::K_IS, 0);
}

MQL_Parser::ExprTypenameContext* MQL_Parser::Property3Context::exprTypename() {
  return getRuleContext<MQL_Parser::ExprTypenameContext>(0);
}

tree::TerminalNode* MQL_Parser::Property3Context::K_NOT() {
  return getToken(MQL_Parser::K_NOT, 0);
}

std::vector<MQL_Parser::ConditionalOrTypeContext *> MQL_Parser::Property3Context::conditionalOrType() {
  return getRuleContexts<MQL_Parser::ConditionalOrTypeContext>();
}

MQL_Parser::ConditionalOrTypeContext* MQL_Parser::Property3Context::conditionalOrType(size_t i) {
  return getRuleContext<MQL_Parser::ConditionalOrTypeContext>(i);
}

MQL_Parser::Property3Context::Property3Context(PropertyContext *ctx) { copyFrom(ctx); }


std::any MQL_Parser::Property3Context::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitProperty3(this);
  else
    return visitor->visitChildren(this);
}
MQL_Parser::PropertyContext* MQL_Parser::property() {
  PropertyContext *_localctx = _tracker.createInstance<PropertyContext>(_ctx, getState());
  enterRule(_localctx, 104, MQL_Parser::RuleProperty);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(699);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 93, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<MQL_Parser::Property1Context>(_localctx);
      enterOuterAlt(_localctx, 1);
      setState(670);
      identifier();
      setState(675);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case MQL_Parser::COLON: {
          setState(671);
          match(MQL_Parser::COLON);
          setState(672);
          value();
          break;
        }

        case MQL_Parser::TRUE_PROP: {
          setState(673);
          match(MQL_Parser::TRUE_PROP);
          break;
        }

        case MQL_Parser::FALSE_PROP: {
          setState(674);
          match(MQL_Parser::FALSE_PROP);
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<MQL_Parser::Property2Context>(_localctx);
      enterOuterAlt(_localctx, 2);
      setState(677);
      identifier();
      setState(678);
      match(MQL_Parser::TYPE);
      setState(679);
      match(MQL_Parser::L_PAR);
      setState(680);
      match(MQL_Parser::STRING);
      setState(681);
      match(MQL_Parser::R_PAR);
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<MQL_Parser::Property3Context>(_localctx);
      enterOuterAlt(_localctx, 3);
      setState(683);
      identifier();
      setState(684);
      match(MQL_Parser::K_IS);
      setState(686);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == MQL_Parser::K_NOT) {
        setState(685);
        match(MQL_Parser::K_NOT);
      }
      setState(688);
      exprTypename();
      setState(692);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == MQL_Parser::K_OR) {
        setState(689);
        conditionalOrType();
        setState(694);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      break;
    }

    case 4: {
      _localctx = _tracker.createInstance<MQL_Parser::Property4Context>(_localctx);
      enterOuterAlt(_localctx, 4);
      setState(695);
      identifier();

      setState(696);
      antlrcpp::downCast<Property4Context *>(_localctx)->op = _input->LT(1);
      _la = _input->LA(1);
      if (!(((((_la - 83) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 83)) & 63) != 0))) {
        antlrcpp::downCast<Property4Context *>(_localctx)->op = _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(697);
      value();
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ConditionalOrTypeContext ------------------------------------------------------------------

MQL_Parser::ConditionalOrTypeContext::ConditionalOrTypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::ConditionalOrTypeContext::K_OR() {
  return getToken(MQL_Parser::K_OR, 0);
}

MQL_Parser::ExprTypenameContext* MQL_Parser::ConditionalOrTypeContext::exprTypename() {
  return getRuleContext<MQL_Parser::ExprTypenameContext>(0);
}


size_t MQL_Parser::ConditionalOrTypeContext::getRuleIndex() const {
  return MQL_Parser::RuleConditionalOrType;
}


std::any MQL_Parser::ConditionalOrTypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitConditionalOrType(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::ConditionalOrTypeContext* MQL_Parser::conditionalOrType() {
  ConditionalOrTypeContext *_localctx = _tracker.createInstance<ConditionalOrTypeContext>(_ctx, getState());
  enterRule(_localctx, 106, MQL_Parser::RuleConditionalOrType);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(701);
    match(MQL_Parser::K_OR);
    setState(702);
    exprTypename();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- IdentifierContext ------------------------------------------------------------------

MQL_Parser::IdentifierContext::IdentifierContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::IdentifierContext::NAME() {
  return getToken(MQL_Parser::NAME, 0);
}

MQL_Parser::KeywordContext* MQL_Parser::IdentifierContext::keyword() {
  return getRuleContext<MQL_Parser::KeywordContext>(0);
}


size_t MQL_Parser::IdentifierContext::getRuleIndex() const {
  return MQL_Parser::RuleIdentifier;
}


std::any MQL_Parser::IdentifierContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitIdentifier(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::IdentifierContext* MQL_Parser::identifier() {
  IdentifierContext *_localctx = _tracker.createInstance<IdentifierContext>(_ctx, getState());
  enterRule(_localctx, 108, MQL_Parser::RuleIdentifier);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(706);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case MQL_Parser::NAME: {
        enterOuterAlt(_localctx, 1);
        setState(704);
        match(MQL_Parser::NAME);
        break;
      }

      case MQL_Parser::K_ACYCLIC:
      case MQL_Parser::K_AND:
      case MQL_Parser::K_ANY:
      case MQL_Parser::K_AS:
      case MQL_Parser::K_AVG:
      case MQL_Parser::K_ALL:
      case MQL_Parser::K_ASC:
      case MQL_Parser::K_BY:
      case MQL_Parser::K_BOOL:
      case MQL_Parser::K_CALL:
      case MQL_Parser::K_COSINE_DISTANCE:
      case MQL_Parser::K_COSINE_SIMILARITY:
      case MQL_Parser::K_COUNT:
      case MQL_Parser::K_CREATE:
      case MQL_Parser::K_DELETE:
      case MQL_Parser::K_DESCRIBE:
      case MQL_Parser::K_DESC:
      case MQL_Parser::K_DIMENSIONS:
      case MQL_Parser::K_DISTINCT:
      case MQL_Parser::K_EDGE:
      case MQL_Parser::K_EUCLIDEAN_DISTANCE:
      case MQL_Parser::K_FROM:
      case MQL_Parser::K_INCOMING:
      case MQL_Parser::K_INDEX:
      case MQL_Parser::K_INSERT:
      case MQL_Parser::K_INTEGER:
      case MQL_Parser::K_INTO:
      case MQL_Parser::K_IS:
      case MQL_Parser::K_FLOAT:
      case MQL_Parser::K_GROUP:
      case MQL_Parser::K_GROUPS:
      case MQL_Parser::K_IDENTITY:
      case MQL_Parser::K_LABELS:
      case MQL_Parser::K_LABEL:
      case MQL_Parser::K_LET:
      case MQL_Parser::K_LIMIT:
      case MQL_Parser::K_MANHATTAN_DISTANCE:
      case MQL_Parser::K_MATCH:
      case MQL_Parser::K_MAX:
      case MQL_Parser::K_MIN:
      case MQL_Parser::K_NORMALIZE:
      case MQL_Parser::K_OBJECTS:
      case MQL_Parser::K_OFFSET:
      case MQL_Parser::K_OPTIONAL:
      case MQL_Parser::K_ORDER:
      case MQL_Parser::K_OR:
      case MQL_Parser::K_OUTGOING:
      case MQL_Parser::K_PREFIX:
      case MQL_Parser::K_PROPERTIES:
      case MQL_Parser::K_PROPERTY:
      case MQL_Parser::K_NOT:
      case MQL_Parser::K_NULL:
      case MQL_Parser::K_SHORTEST:
      case MQL_Parser::K_SHOW:
      case MQL_Parser::K_SIMPLE:
      case MQL_Parser::K_TEXT_SEARCH:
      case MQL_Parser::K_REGEX:
      case MQL_Parser::K_RETURN:
      case MQL_Parser::K_SUM:
      case MQL_Parser::K_STRING:
      case MQL_Parser::K_TRAILS:
      case MQL_Parser::K_VALUES:
      case MQL_Parser::K_WALKS:
      case MQL_Parser::K_WITH:
      case MQL_Parser::K_WHERE:
      case MQL_Parser::K_YIELD: {
        enterOuterAlt(_localctx, 2);
        setState(705);
        keyword();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BoolValueContext ------------------------------------------------------------------

MQL_Parser::BoolValueContext::BoolValueContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::BoolValueContext::K_TRUE() {
  return getToken(MQL_Parser::K_TRUE, 0);
}

tree::TerminalNode* MQL_Parser::BoolValueContext::K_FALSE() {
  return getToken(MQL_Parser::K_FALSE, 0);
}


size_t MQL_Parser::BoolValueContext::getRuleIndex() const {
  return MQL_Parser::RuleBoolValue;
}


std::any MQL_Parser::BoolValueContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitBoolValue(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::BoolValueContext* MQL_Parser::boolValue() {
  BoolValueContext *_localctx = _tracker.createInstance<BoolValueContext>(_ctx, getState());
  enterRule(_localctx, 110, MQL_Parser::RuleBoolValue);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(708);
    _la = _input->LA(1);
    if (!(_la == MQL_Parser::K_FALSE

    || _la == MQL_Parser::K_TRUE)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- NumericValueContext ------------------------------------------------------------------

MQL_Parser::NumericValueContext::NumericValueContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::NumericValueContext::UNSIGNED_INTEGER() {
  return getToken(MQL_Parser::UNSIGNED_INTEGER, 0);
}

tree::TerminalNode* MQL_Parser::NumericValueContext::UNSIGNED_FLOAT() {
  return getToken(MQL_Parser::UNSIGNED_FLOAT, 0);
}

tree::TerminalNode* MQL_Parser::NumericValueContext::UNSIGNED_SCIENTIFIC_NOTATION() {
  return getToken(MQL_Parser::UNSIGNED_SCIENTIFIC_NOTATION, 0);
}

tree::TerminalNode* MQL_Parser::NumericValueContext::PLUS() {
  return getToken(MQL_Parser::PLUS, 0);
}

tree::TerminalNode* MQL_Parser::NumericValueContext::MINUS() {
  return getToken(MQL_Parser::MINUS, 0);
}


size_t MQL_Parser::NumericValueContext::getRuleIndex() const {
  return MQL_Parser::RuleNumericValue;
}


std::any MQL_Parser::NumericValueContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitNumericValue(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::NumericValueContext* MQL_Parser::numericValue() {
  NumericValueContext *_localctx = _tracker.createInstance<NumericValueContext>(_ctx, getState());
  enterRule(_localctx, 112, MQL_Parser::RuleNumericValue);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(711);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MQL_Parser::PLUS

    || _la == MQL_Parser::MINUS) {
      setState(710);
      _la = _input->LA(1);
      if (!(_la == MQL_Parser::PLUS

      || _la == MQL_Parser::MINUS)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
    }
    setState(713);
    _la = _input->LA(1);
    if (!(((((_la - 79) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 79)) & 7) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DatatypeValueContext ------------------------------------------------------------------

MQL_Parser::DatatypeValueContext::DatatypeValueContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

MQL_Parser::IdentifierContext* MQL_Parser::DatatypeValueContext::identifier() {
  return getRuleContext<MQL_Parser::IdentifierContext>(0);
}

tree::TerminalNode* MQL_Parser::DatatypeValueContext::L_PAR() {
  return getToken(MQL_Parser::L_PAR, 0);
}

tree::TerminalNode* MQL_Parser::DatatypeValueContext::STRING() {
  return getToken(MQL_Parser::STRING, 0);
}

tree::TerminalNode* MQL_Parser::DatatypeValueContext::R_PAR() {
  return getToken(MQL_Parser::R_PAR, 0);
}


size_t MQL_Parser::DatatypeValueContext::getRuleIndex() const {
  return MQL_Parser::RuleDatatypeValue;
}


std::any MQL_Parser::DatatypeValueContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitDatatypeValue(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::DatatypeValueContext* MQL_Parser::datatypeValue() {
  DatatypeValueContext *_localctx = _tracker.createInstance<DatatypeValueContext>(_ctx, getState());
  enterRule(_localctx, 114, MQL_Parser::RuleDatatypeValue);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(715);
    identifier();
    setState(716);
    match(MQL_Parser::L_PAR);
    setState(717);
    match(MQL_Parser::STRING);
    setState(718);
    match(MQL_Parser::R_PAR);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ValueContext ------------------------------------------------------------------

MQL_Parser::ValueContext::ValueContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

MQL_Parser::NumericValueContext* MQL_Parser::ValueContext::numericValue() {
  return getRuleContext<MQL_Parser::NumericValueContext>(0);
}

tree::TerminalNode* MQL_Parser::ValueContext::STRING() {
  return getToken(MQL_Parser::STRING, 0);
}

MQL_Parser::BoolValueContext* MQL_Parser::ValueContext::boolValue() {
  return getRuleContext<MQL_Parser::BoolValueContext>(0);
}

MQL_Parser::DatatypeValueContext* MQL_Parser::ValueContext::datatypeValue() {
  return getRuleContext<MQL_Parser::DatatypeValueContext>(0);
}


size_t MQL_Parser::ValueContext::getRuleIndex() const {
  return MQL_Parser::RuleValue;
}


std::any MQL_Parser::ValueContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitValue(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::ValueContext* MQL_Parser::value() {
  ValueContext *_localctx = _tracker.createInstance<ValueContext>(_ctx, getState());
  enterRule(_localctx, 116, MQL_Parser::RuleValue);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(724);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case MQL_Parser::UNSIGNED_INTEGER:
      case MQL_Parser::UNSIGNED_FLOAT:
      case MQL_Parser::UNSIGNED_SCIENTIFIC_NOTATION:
      case MQL_Parser::PLUS:
      case MQL_Parser::MINUS: {
        enterOuterAlt(_localctx, 1);
        setState(720);
        numericValue();
        break;
      }

      case MQL_Parser::STRING: {
        enterOuterAlt(_localctx, 2);
        setState(721);
        match(MQL_Parser::STRING);
        break;
      }

      case MQL_Parser::K_FALSE:
      case MQL_Parser::K_TRUE: {
        enterOuterAlt(_localctx, 3);
        setState(722);
        boolValue();
        break;
      }

      case MQL_Parser::K_ACYCLIC:
      case MQL_Parser::K_AND:
      case MQL_Parser::K_ANY:
      case MQL_Parser::K_AS:
      case MQL_Parser::K_AVG:
      case MQL_Parser::K_ALL:
      case MQL_Parser::K_ASC:
      case MQL_Parser::K_BY:
      case MQL_Parser::K_BOOL:
      case MQL_Parser::K_CALL:
      case MQL_Parser::K_COSINE_DISTANCE:
      case MQL_Parser::K_COSINE_SIMILARITY:
      case MQL_Parser::K_COUNT:
      case MQL_Parser::K_CREATE:
      case MQL_Parser::K_DELETE:
      case MQL_Parser::K_DESCRIBE:
      case MQL_Parser::K_DESC:
      case MQL_Parser::K_DIMENSIONS:
      case MQL_Parser::K_DISTINCT:
      case MQL_Parser::K_EDGE:
      case MQL_Parser::K_EUCLIDEAN_DISTANCE:
      case MQL_Parser::K_FROM:
      case MQL_Parser::K_INCOMING:
      case MQL_Parser::K_INDEX:
      case MQL_Parser::K_INSERT:
      case MQL_Parser::K_INTEGER:
      case MQL_Parser::K_INTO:
      case MQL_Parser::K_IS:
      case MQL_Parser::K_FLOAT:
      case MQL_Parser::K_GROUP:
      case MQL_Parser::K_GROUPS:
      case MQL_Parser::K_IDENTITY:
      case MQL_Parser::K_LABELS:
      case MQL_Parser::K_LABEL:
      case MQL_Parser::K_LET:
      case MQL_Parser::K_LIMIT:
      case MQL_Parser::K_MANHATTAN_DISTANCE:
      case MQL_Parser::K_MATCH:
      case MQL_Parser::K_MAX:
      case MQL_Parser::K_MIN:
      case MQL_Parser::K_NORMALIZE:
      case MQL_Parser::K_OBJECTS:
      case MQL_Parser::K_OFFSET:
      case MQL_Parser::K_OPTIONAL:
      case MQL_Parser::K_ORDER:
      case MQL_Parser::K_OR:
      case MQL_Parser::K_OUTGOING:
      case MQL_Parser::K_PREFIX:
      case MQL_Parser::K_PROPERTIES:
      case MQL_Parser::K_PROPERTY:
      case MQL_Parser::K_NOT:
      case MQL_Parser::K_NULL:
      case MQL_Parser::K_SHORTEST:
      case MQL_Parser::K_SHOW:
      case MQL_Parser::K_SIMPLE:
      case MQL_Parser::K_TEXT_SEARCH:
      case MQL_Parser::K_REGEX:
      case MQL_Parser::K_RETURN:
      case MQL_Parser::K_SUM:
      case MQL_Parser::K_STRING:
      case MQL_Parser::K_TRAILS:
      case MQL_Parser::K_VALUES:
      case MQL_Parser::K_WALKS:
      case MQL_Parser::K_WITH:
      case MQL_Parser::K_WHERE:
      case MQL_Parser::K_YIELD:
      case MQL_Parser::NAME: {
        enterOuterAlt(_localctx, 4);
        setState(723);
        datatypeValue();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ConditionalOrExprContext ------------------------------------------------------------------

MQL_Parser::ConditionalOrExprContext::ConditionalOrExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<MQL_Parser::ConditionalAndExprContext *> MQL_Parser::ConditionalOrExprContext::conditionalAndExpr() {
  return getRuleContexts<MQL_Parser::ConditionalAndExprContext>();
}

MQL_Parser::ConditionalAndExprContext* MQL_Parser::ConditionalOrExprContext::conditionalAndExpr(size_t i) {
  return getRuleContext<MQL_Parser::ConditionalAndExprContext>(i);
}

std::vector<tree::TerminalNode *> MQL_Parser::ConditionalOrExprContext::K_OR() {
  return getTokens(MQL_Parser::K_OR);
}

tree::TerminalNode* MQL_Parser::ConditionalOrExprContext::K_OR(size_t i) {
  return getToken(MQL_Parser::K_OR, i);
}


size_t MQL_Parser::ConditionalOrExprContext::getRuleIndex() const {
  return MQL_Parser::RuleConditionalOrExpr;
}


std::any MQL_Parser::ConditionalOrExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitConditionalOrExpr(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::ConditionalOrExprContext* MQL_Parser::conditionalOrExpr() {
  ConditionalOrExprContext *_localctx = _tracker.createInstance<ConditionalOrExprContext>(_ctx, getState());
  enterRule(_localctx, 118, MQL_Parser::RuleConditionalOrExpr);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(726);
    conditionalAndExpr();
    setState(731);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MQL_Parser::K_OR) {
      setState(727);
      match(MQL_Parser::K_OR);
      setState(728);
      conditionalAndExpr();
      setState(733);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ConditionalAndExprContext ------------------------------------------------------------------

MQL_Parser::ConditionalAndExprContext::ConditionalAndExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<MQL_Parser::ComparisonExprContext *> MQL_Parser::ConditionalAndExprContext::comparisonExpr() {
  return getRuleContexts<MQL_Parser::ComparisonExprContext>();
}

MQL_Parser::ComparisonExprContext* MQL_Parser::ConditionalAndExprContext::comparisonExpr(size_t i) {
  return getRuleContext<MQL_Parser::ComparisonExprContext>(i);
}

std::vector<tree::TerminalNode *> MQL_Parser::ConditionalAndExprContext::K_AND() {
  return getTokens(MQL_Parser::K_AND);
}

tree::TerminalNode* MQL_Parser::ConditionalAndExprContext::K_AND(size_t i) {
  return getToken(MQL_Parser::K_AND, i);
}


size_t MQL_Parser::ConditionalAndExprContext::getRuleIndex() const {
  return MQL_Parser::RuleConditionalAndExpr;
}


std::any MQL_Parser::ConditionalAndExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitConditionalAndExpr(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::ConditionalAndExprContext* MQL_Parser::conditionalAndExpr() {
  ConditionalAndExprContext *_localctx = _tracker.createInstance<ConditionalAndExprContext>(_ctx, getState());
  enterRule(_localctx, 120, MQL_Parser::RuleConditionalAndExpr);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(734);
    comparisonExpr();
    setState(739);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MQL_Parser::K_AND) {
      setState(735);
      match(MQL_Parser::K_AND);
      setState(736);
      comparisonExpr();
      setState(741);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ComparisonExprContext ------------------------------------------------------------------

MQL_Parser::ComparisonExprContext::ComparisonExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t MQL_Parser::ComparisonExprContext::getRuleIndex() const {
  return MQL_Parser::RuleComparisonExpr;
}

void MQL_Parser::ComparisonExprContext::copyFrom(ComparisonExprContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- ComparisonExprIsContext ------------------------------------------------------------------

MQL_Parser::AdditiveExprContext* MQL_Parser::ComparisonExprIsContext::additiveExpr() {
  return getRuleContext<MQL_Parser::AdditiveExprContext>(0);
}

tree::TerminalNode* MQL_Parser::ComparisonExprIsContext::K_IS() {
  return getToken(MQL_Parser::K_IS, 0);
}

MQL_Parser::ExprTypenameContext* MQL_Parser::ComparisonExprIsContext::exprTypename() {
  return getRuleContext<MQL_Parser::ExprTypenameContext>(0);
}

tree::TerminalNode* MQL_Parser::ComparisonExprIsContext::K_NOT() {
  return getToken(MQL_Parser::K_NOT, 0);
}

MQL_Parser::ComparisonExprIsContext::ComparisonExprIsContext(ComparisonExprContext *ctx) { copyFrom(ctx); }


std::any MQL_Parser::ComparisonExprIsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitComparisonExprIs(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ComparisonExprOpContext ------------------------------------------------------------------

std::vector<MQL_Parser::AdditiveExprContext *> MQL_Parser::ComparisonExprOpContext::additiveExpr() {
  return getRuleContexts<MQL_Parser::AdditiveExprContext>();
}

MQL_Parser::AdditiveExprContext* MQL_Parser::ComparisonExprOpContext::additiveExpr(size_t i) {
  return getRuleContext<MQL_Parser::AdditiveExprContext>(i);
}

tree::TerminalNode* MQL_Parser::ComparisonExprOpContext::EQ() {
  return getToken(MQL_Parser::EQ, 0);
}

tree::TerminalNode* MQL_Parser::ComparisonExprOpContext::NEQ() {
  return getToken(MQL_Parser::NEQ, 0);
}

tree::TerminalNode* MQL_Parser::ComparisonExprOpContext::LT() {
  return getToken(MQL_Parser::LT, 0);
}

tree::TerminalNode* MQL_Parser::ComparisonExprOpContext::GT() {
  return getToken(MQL_Parser::GT, 0);
}

tree::TerminalNode* MQL_Parser::ComparisonExprOpContext::LEQ() {
  return getToken(MQL_Parser::LEQ, 0);
}

tree::TerminalNode* MQL_Parser::ComparisonExprOpContext::GEQ() {
  return getToken(MQL_Parser::GEQ, 0);
}

MQL_Parser::ComparisonExprOpContext::ComparisonExprOpContext(ComparisonExprContext *ctx) { copyFrom(ctx); }


std::any MQL_Parser::ComparisonExprOpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitComparisonExprOp(this);
  else
    return visitor->visitChildren(this);
}
MQL_Parser::ComparisonExprContext* MQL_Parser::comparisonExpr() {
  ComparisonExprContext *_localctx = _tracker.createInstance<ComparisonExprContext>(_ctx, getState());
  enterRule(_localctx, 122, MQL_Parser::RuleComparisonExpr);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(754);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 101, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<MQL_Parser::ComparisonExprOpContext>(_localctx);
      enterOuterAlt(_localctx, 1);
      setState(742);
      additiveExpr();
      setState(745);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (((((_la - 83) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 83)) & 63) != 0)) {
        setState(743);
        antlrcpp::downCast<ComparisonExprOpContext *>(_localctx)->op = _input->LT(1);
        _la = _input->LA(1);
        if (!(((((_la - 83) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 83)) & 63) != 0))) {
          antlrcpp::downCast<ComparisonExprOpContext *>(_localctx)->op = _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        setState(744);
        additiveExpr();
      }
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<MQL_Parser::ComparisonExprIsContext>(_localctx);
      enterOuterAlt(_localctx, 2);
      setState(747);
      additiveExpr();
      setState(748);
      match(MQL_Parser::K_IS);
      setState(750);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == MQL_Parser::K_NOT) {
        setState(749);
        match(MQL_Parser::K_NOT);
      }
      setState(752);
      exprTypename();
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AdditiveExprContext ------------------------------------------------------------------

MQL_Parser::AdditiveExprContext::AdditiveExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<MQL_Parser::MultiplicativeExprContext *> MQL_Parser::AdditiveExprContext::multiplicativeExpr() {
  return getRuleContexts<MQL_Parser::MultiplicativeExprContext>();
}

MQL_Parser::MultiplicativeExprContext* MQL_Parser::AdditiveExprContext::multiplicativeExpr(size_t i) {
  return getRuleContext<MQL_Parser::MultiplicativeExprContext>(i);
}

std::vector<tree::TerminalNode *> MQL_Parser::AdditiveExprContext::PLUS() {
  return getTokens(MQL_Parser::PLUS);
}

tree::TerminalNode* MQL_Parser::AdditiveExprContext::PLUS(size_t i) {
  return getToken(MQL_Parser::PLUS, i);
}

std::vector<tree::TerminalNode *> MQL_Parser::AdditiveExprContext::MINUS() {
  return getTokens(MQL_Parser::MINUS);
}

tree::TerminalNode* MQL_Parser::AdditiveExprContext::MINUS(size_t i) {
  return getToken(MQL_Parser::MINUS, i);
}


size_t MQL_Parser::AdditiveExprContext::getRuleIndex() const {
  return MQL_Parser::RuleAdditiveExpr;
}


std::any MQL_Parser::AdditiveExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitAdditiveExpr(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::AdditiveExprContext* MQL_Parser::additiveExpr() {
  AdditiveExprContext *_localctx = _tracker.createInstance<AdditiveExprContext>(_ctx, getState());
  enterRule(_localctx, 124, MQL_Parser::RuleAdditiveExpr);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(756);
    multiplicativeExpr();
    setState(761);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MQL_Parser::PLUS

    || _la == MQL_Parser::MINUS) {
      setState(757);
      antlrcpp::downCast<AdditiveExprContext *>(_localctx)->_tset1417 = _input->LT(1);
      _la = _input->LA(1);
      if (!(_la == MQL_Parser::PLUS

      || _la == MQL_Parser::MINUS)) {
        antlrcpp::downCast<AdditiveExprContext *>(_localctx)->_tset1417 = _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      antlrcpp::downCast<AdditiveExprContext *>(_localctx)->op.push_back(antlrcpp::downCast<AdditiveExprContext *>(_localctx)->_tset1417);
      setState(758);
      multiplicativeExpr();
      setState(763);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- MultiplicativeExprContext ------------------------------------------------------------------

MQL_Parser::MultiplicativeExprContext::MultiplicativeExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<MQL_Parser::UnaryExprContext *> MQL_Parser::MultiplicativeExprContext::unaryExpr() {
  return getRuleContexts<MQL_Parser::UnaryExprContext>();
}

MQL_Parser::UnaryExprContext* MQL_Parser::MultiplicativeExprContext::unaryExpr(size_t i) {
  return getRuleContext<MQL_Parser::UnaryExprContext>(i);
}

std::vector<tree::TerminalNode *> MQL_Parser::MultiplicativeExprContext::STAR() {
  return getTokens(MQL_Parser::STAR);
}

tree::TerminalNode* MQL_Parser::MultiplicativeExprContext::STAR(size_t i) {
  return getToken(MQL_Parser::STAR, i);
}

std::vector<tree::TerminalNode *> MQL_Parser::MultiplicativeExprContext::PATH_SEQUENCE() {
  return getTokens(MQL_Parser::PATH_SEQUENCE);
}

tree::TerminalNode* MQL_Parser::MultiplicativeExprContext::PATH_SEQUENCE(size_t i) {
  return getToken(MQL_Parser::PATH_SEQUENCE, i);
}

std::vector<tree::TerminalNode *> MQL_Parser::MultiplicativeExprContext::PERCENT() {
  return getTokens(MQL_Parser::PERCENT);
}

tree::TerminalNode* MQL_Parser::MultiplicativeExprContext::PERCENT(size_t i) {
  return getToken(MQL_Parser::PERCENT, i);
}


size_t MQL_Parser::MultiplicativeExprContext::getRuleIndex() const {
  return MQL_Parser::RuleMultiplicativeExpr;
}


std::any MQL_Parser::MultiplicativeExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitMultiplicativeExpr(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::MultiplicativeExprContext* MQL_Parser::multiplicativeExpr() {
  MultiplicativeExprContext *_localctx = _tracker.createInstance<MultiplicativeExprContext>(_ctx, getState());
  enterRule(_localctx, 126, MQL_Parser::RuleMultiplicativeExpr);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(764);
    unaryExpr();
    setState(769);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (((((_la - 90) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 90)) & 25) != 0)) {
      setState(765);
      antlrcpp::downCast<MultiplicativeExprContext *>(_localctx)->_tset1440 = _input->LT(1);
      _la = _input->LA(1);
      if (!(((((_la - 90) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 90)) & 25) != 0))) {
        antlrcpp::downCast<MultiplicativeExprContext *>(_localctx)->_tset1440 = _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      antlrcpp::downCast<MultiplicativeExprContext *>(_localctx)->op.push_back(antlrcpp::downCast<MultiplicativeExprContext *>(_localctx)->_tset1440);
      setState(766);
      unaryExpr();
      setState(771);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- UnaryExprContext ------------------------------------------------------------------

MQL_Parser::UnaryExprContext::UnaryExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::UnaryExprContext::K_NOT() {
  return getToken(MQL_Parser::K_NOT, 0);
}

MQL_Parser::UnaryExprContext* MQL_Parser::UnaryExprContext::unaryExpr() {
  return getRuleContext<MQL_Parser::UnaryExprContext>(0);
}

MQL_Parser::AtomicExprContext* MQL_Parser::UnaryExprContext::atomicExpr() {
  return getRuleContext<MQL_Parser::AtomicExprContext>(0);
}

tree::TerminalNode* MQL_Parser::UnaryExprContext::PLUS() {
  return getToken(MQL_Parser::PLUS, 0);
}

tree::TerminalNode* MQL_Parser::UnaryExprContext::MINUS() {
  return getToken(MQL_Parser::MINUS, 0);
}


size_t MQL_Parser::UnaryExprContext::getRuleIndex() const {
  return MQL_Parser::RuleUnaryExpr;
}


std::any MQL_Parser::UnaryExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitUnaryExpr(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::UnaryExprContext* MQL_Parser::unaryExpr() {
  UnaryExprContext *_localctx = _tracker.createInstance<UnaryExprContext>(_ctx, getState());
  enterRule(_localctx, 128, MQL_Parser::RuleUnaryExpr);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(779);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 104, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(772);
      match(MQL_Parser::K_NOT);
      setState(773);
      unaryExpr();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(774);
      atomicExpr();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(775);
      match(MQL_Parser::PLUS);
      setState(776);
      unaryExpr();
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(777);
      match(MQL_Parser::MINUS);
      setState(778);
      unaryExpr();
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AtomicExprContext ------------------------------------------------------------------

MQL_Parser::AtomicExprContext::AtomicExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t MQL_Parser::AtomicExprContext::getRuleIndex() const {
  return MQL_Parser::RuleAtomicExpr;
}

void MQL_Parser::AtomicExprContext::copyFrom(AtomicExprContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- ExprFunctionContext ------------------------------------------------------------------

MQL_Parser::FunctionContext* MQL_Parser::ExprFunctionContext::function() {
  return getRuleContext<MQL_Parser::FunctionContext>(0);
}

MQL_Parser::ExprFunctionContext::ExprFunctionContext(AtomicExprContext *ctx) { copyFrom(ctx); }


std::any MQL_Parser::ExprFunctionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitExprFunction(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ExprValueContext ------------------------------------------------------------------

MQL_Parser::ValueContext* MQL_Parser::ExprValueContext::value() {
  return getRuleContext<MQL_Parser::ValueContext>(0);
}

MQL_Parser::ExprValueContext::ExprValueContext(AtomicExprContext *ctx) { copyFrom(ctx); }


std::any MQL_Parser::ExprValueContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitExprValue(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ExprFixedNodeInsideContext ------------------------------------------------------------------

MQL_Parser::FixedNodeInsideContext* MQL_Parser::ExprFixedNodeInsideContext::fixedNodeInside() {
  return getRuleContext<MQL_Parser::FixedNodeInsideContext>(0);
}

MQL_Parser::ExprFixedNodeInsideContext::ExprFixedNodeInsideContext(AtomicExprContext *ctx) { copyFrom(ctx); }


std::any MQL_Parser::ExprFixedNodeInsideContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitExprFixedNodeInside(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ExprVarContext ------------------------------------------------------------------

tree::TerminalNode* MQL_Parser::ExprVarContext::VARIABLE() {
  return getToken(MQL_Parser::VARIABLE, 0);
}

tree::TerminalNode* MQL_Parser::ExprVarContext::KEY() {
  return getToken(MQL_Parser::KEY, 0);
}

MQL_Parser::ExprVarContext::ExprVarContext(AtomicExprContext *ctx) { copyFrom(ctx); }


std::any MQL_Parser::ExprVarContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitExprVar(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ExprParenthesisContext ------------------------------------------------------------------

tree::TerminalNode* MQL_Parser::ExprParenthesisContext::L_PAR() {
  return getToken(MQL_Parser::L_PAR, 0);
}

MQL_Parser::ConditionalOrExprContext* MQL_Parser::ExprParenthesisContext::conditionalOrExpr() {
  return getRuleContext<MQL_Parser::ConditionalOrExprContext>(0);
}

tree::TerminalNode* MQL_Parser::ExprParenthesisContext::R_PAR() {
  return getToken(MQL_Parser::R_PAR, 0);
}

MQL_Parser::ExprParenthesisContext::ExprParenthesisContext(AtomicExprContext *ctx) { copyFrom(ctx); }


std::any MQL_Parser::ExprParenthesisContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitExprParenthesis(this);
  else
    return visitor->visitChildren(this);
}
MQL_Parser::AtomicExprContext* MQL_Parser::atomicExpr() {
  AtomicExprContext *_localctx = _tracker.createInstance<AtomicExprContext>(_ctx, getState());
  enterRule(_localctx, 130, MQL_Parser::RuleAtomicExpr);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(792);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 106, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<MQL_Parser::ExprVarContext>(_localctx);
      enterOuterAlt(_localctx, 1);
      setState(781);
      match(MQL_Parser::VARIABLE);
      setState(783);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == MQL_Parser::KEY) {
        setState(782);
        match(MQL_Parser::KEY);
      }
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<MQL_Parser::ExprFunctionContext>(_localctx);
      enterOuterAlt(_localctx, 2);
      setState(785);
      function();
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<MQL_Parser::ExprValueContext>(_localctx);
      enterOuterAlt(_localctx, 3);
      setState(786);
      value();
      break;
    }

    case 4: {
      _localctx = _tracker.createInstance<MQL_Parser::ExprFixedNodeInsideContext>(_localctx);
      enterOuterAlt(_localctx, 4);
      setState(787);
      fixedNodeInside();
      break;
    }

    case 5: {
      _localctx = _tracker.createInstance<MQL_Parser::ExprParenthesisContext>(_localctx);
      enterOuterAlt(_localctx, 5);
      setState(788);
      match(MQL_Parser::L_PAR);
      setState(789);
      conditionalOrExpr();
      setState(790);
      match(MQL_Parser::R_PAR);
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FunctionContext ------------------------------------------------------------------

MQL_Parser::FunctionContext::FunctionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

MQL_Parser::RegexContext* MQL_Parser::FunctionContext::regex() {
  return getRuleContext<MQL_Parser::RegexContext>(0);
}

MQL_Parser::CosineSimilarityContext* MQL_Parser::FunctionContext::cosineSimilarity() {
  return getRuleContext<MQL_Parser::CosineSimilarityContext>(0);
}

MQL_Parser::CosineDistanceContext* MQL_Parser::FunctionContext::cosineDistance() {
  return getRuleContext<MQL_Parser::CosineDistanceContext>(0);
}

MQL_Parser::ManhattanDistanceContext* MQL_Parser::FunctionContext::manhattanDistance() {
  return getRuleContext<MQL_Parser::ManhattanDistanceContext>(0);
}

MQL_Parser::EuclideanDistanceContext* MQL_Parser::FunctionContext::euclideanDistance() {
  return getRuleContext<MQL_Parser::EuclideanDistanceContext>(0);
}

MQL_Parser::EditDistanceContext* MQL_Parser::FunctionContext::editDistance() {
  return getRuleContext<MQL_Parser::EditDistanceContext>(0);
}

MQL_Parser::NormalizeContext* MQL_Parser::FunctionContext::normalize() {
  return getRuleContext<MQL_Parser::NormalizeContext>(0);
}


size_t MQL_Parser::FunctionContext::getRuleIndex() const {
  return MQL_Parser::RuleFunction;
}


std::any MQL_Parser::FunctionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitFunction(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::FunctionContext* MQL_Parser::function() {
  FunctionContext *_localctx = _tracker.createInstance<FunctionContext>(_ctx, getState());
  enterRule(_localctx, 132, MQL_Parser::RuleFunction);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(801);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case MQL_Parser::K_REGEX: {
        enterOuterAlt(_localctx, 1);
        setState(794);
        regex();
        break;
      }

      case MQL_Parser::K_COSINE_SIMILARITY: {
        enterOuterAlt(_localctx, 2);
        setState(795);
        cosineSimilarity();
        break;
      }

      case MQL_Parser::K_COSINE_DISTANCE: {
        enterOuterAlt(_localctx, 3);
        setState(796);
        cosineDistance();
        break;
      }

      case MQL_Parser::K_MANHATTAN_DISTANCE: {
        enterOuterAlt(_localctx, 4);
        setState(797);
        manhattanDistance();
        break;
      }

      case MQL_Parser::K_EUCLIDEAN_DISTANCE: {
        enterOuterAlt(_localctx, 5);
        setState(798);
        euclideanDistance();
        break;
      }

      case MQL_Parser::K_EDIT_DISTANCE: {
        enterOuterAlt(_localctx, 6);
        setState(799);
        editDistance();
        break;
      }

      case MQL_Parser::K_NORMALIZE: {
        enterOuterAlt(_localctx, 7);
        setState(800);
        normalize();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- RegexContext ------------------------------------------------------------------

MQL_Parser::RegexContext::RegexContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::RegexContext::K_REGEX() {
  return getToken(MQL_Parser::K_REGEX, 0);
}

tree::TerminalNode* MQL_Parser::RegexContext::L_PAR() {
  return getToken(MQL_Parser::L_PAR, 0);
}

std::vector<MQL_Parser::ConditionalOrExprContext *> MQL_Parser::RegexContext::conditionalOrExpr() {
  return getRuleContexts<MQL_Parser::ConditionalOrExprContext>();
}

MQL_Parser::ConditionalOrExprContext* MQL_Parser::RegexContext::conditionalOrExpr(size_t i) {
  return getRuleContext<MQL_Parser::ConditionalOrExprContext>(i);
}

std::vector<tree::TerminalNode *> MQL_Parser::RegexContext::COMMA() {
  return getTokens(MQL_Parser::COMMA);
}

tree::TerminalNode* MQL_Parser::RegexContext::COMMA(size_t i) {
  return getToken(MQL_Parser::COMMA, i);
}

tree::TerminalNode* MQL_Parser::RegexContext::R_PAR() {
  return getToken(MQL_Parser::R_PAR, 0);
}


size_t MQL_Parser::RegexContext::getRuleIndex() const {
  return MQL_Parser::RuleRegex;
}


std::any MQL_Parser::RegexContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitRegex(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::RegexContext* MQL_Parser::regex() {
  RegexContext *_localctx = _tracker.createInstance<RegexContext>(_ctx, getState());
  enterRule(_localctx, 134, MQL_Parser::RuleRegex);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(803);
    match(MQL_Parser::K_REGEX);
    setState(804);
    match(MQL_Parser::L_PAR);
    setState(805);
    conditionalOrExpr();
    setState(806);
    match(MQL_Parser::COMMA);
    setState(807);
    conditionalOrExpr();
    setState(810);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MQL_Parser::COMMA) {
      setState(808);
      match(MQL_Parser::COMMA);
      setState(809);
      conditionalOrExpr();
    }
    setState(812);
    match(MQL_Parser::R_PAR);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CosineSimilarityContext ------------------------------------------------------------------

MQL_Parser::CosineSimilarityContext::CosineSimilarityContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::CosineSimilarityContext::K_COSINE_SIMILARITY() {
  return getToken(MQL_Parser::K_COSINE_SIMILARITY, 0);
}

tree::TerminalNode* MQL_Parser::CosineSimilarityContext::L_PAR() {
  return getToken(MQL_Parser::L_PAR, 0);
}

std::vector<MQL_Parser::ConditionalOrExprContext *> MQL_Parser::CosineSimilarityContext::conditionalOrExpr() {
  return getRuleContexts<MQL_Parser::ConditionalOrExprContext>();
}

MQL_Parser::ConditionalOrExprContext* MQL_Parser::CosineSimilarityContext::conditionalOrExpr(size_t i) {
  return getRuleContext<MQL_Parser::ConditionalOrExprContext>(i);
}

tree::TerminalNode* MQL_Parser::CosineSimilarityContext::COMMA() {
  return getToken(MQL_Parser::COMMA, 0);
}

tree::TerminalNode* MQL_Parser::CosineSimilarityContext::R_PAR() {
  return getToken(MQL_Parser::R_PAR, 0);
}


size_t MQL_Parser::CosineSimilarityContext::getRuleIndex() const {
  return MQL_Parser::RuleCosineSimilarity;
}


std::any MQL_Parser::CosineSimilarityContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitCosineSimilarity(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::CosineSimilarityContext* MQL_Parser::cosineSimilarity() {
  CosineSimilarityContext *_localctx = _tracker.createInstance<CosineSimilarityContext>(_ctx, getState());
  enterRule(_localctx, 136, MQL_Parser::RuleCosineSimilarity);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(814);
    match(MQL_Parser::K_COSINE_SIMILARITY);
    setState(815);
    match(MQL_Parser::L_PAR);
    setState(816);
    conditionalOrExpr();
    setState(817);
    match(MQL_Parser::COMMA);
    setState(818);
    conditionalOrExpr();
    setState(819);
    match(MQL_Parser::R_PAR);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CosineDistanceContext ------------------------------------------------------------------

MQL_Parser::CosineDistanceContext::CosineDistanceContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::CosineDistanceContext::K_COSINE_DISTANCE() {
  return getToken(MQL_Parser::K_COSINE_DISTANCE, 0);
}

tree::TerminalNode* MQL_Parser::CosineDistanceContext::L_PAR() {
  return getToken(MQL_Parser::L_PAR, 0);
}

std::vector<MQL_Parser::ConditionalOrExprContext *> MQL_Parser::CosineDistanceContext::conditionalOrExpr() {
  return getRuleContexts<MQL_Parser::ConditionalOrExprContext>();
}

MQL_Parser::ConditionalOrExprContext* MQL_Parser::CosineDistanceContext::conditionalOrExpr(size_t i) {
  return getRuleContext<MQL_Parser::ConditionalOrExprContext>(i);
}

tree::TerminalNode* MQL_Parser::CosineDistanceContext::COMMA() {
  return getToken(MQL_Parser::COMMA, 0);
}

tree::TerminalNode* MQL_Parser::CosineDistanceContext::R_PAR() {
  return getToken(MQL_Parser::R_PAR, 0);
}


size_t MQL_Parser::CosineDistanceContext::getRuleIndex() const {
  return MQL_Parser::RuleCosineDistance;
}


std::any MQL_Parser::CosineDistanceContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitCosineDistance(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::CosineDistanceContext* MQL_Parser::cosineDistance() {
  CosineDistanceContext *_localctx = _tracker.createInstance<CosineDistanceContext>(_ctx, getState());
  enterRule(_localctx, 138, MQL_Parser::RuleCosineDistance);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(821);
    match(MQL_Parser::K_COSINE_DISTANCE);
    setState(822);
    match(MQL_Parser::L_PAR);
    setState(823);
    conditionalOrExpr();
    setState(824);
    match(MQL_Parser::COMMA);
    setState(825);
    conditionalOrExpr();
    setState(826);
    match(MQL_Parser::R_PAR);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ManhattanDistanceContext ------------------------------------------------------------------

MQL_Parser::ManhattanDistanceContext::ManhattanDistanceContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::ManhattanDistanceContext::K_MANHATTAN_DISTANCE() {
  return getToken(MQL_Parser::K_MANHATTAN_DISTANCE, 0);
}

tree::TerminalNode* MQL_Parser::ManhattanDistanceContext::L_PAR() {
  return getToken(MQL_Parser::L_PAR, 0);
}

std::vector<MQL_Parser::ConditionalOrExprContext *> MQL_Parser::ManhattanDistanceContext::conditionalOrExpr() {
  return getRuleContexts<MQL_Parser::ConditionalOrExprContext>();
}

MQL_Parser::ConditionalOrExprContext* MQL_Parser::ManhattanDistanceContext::conditionalOrExpr(size_t i) {
  return getRuleContext<MQL_Parser::ConditionalOrExprContext>(i);
}

tree::TerminalNode* MQL_Parser::ManhattanDistanceContext::COMMA() {
  return getToken(MQL_Parser::COMMA, 0);
}

tree::TerminalNode* MQL_Parser::ManhattanDistanceContext::R_PAR() {
  return getToken(MQL_Parser::R_PAR, 0);
}


size_t MQL_Parser::ManhattanDistanceContext::getRuleIndex() const {
  return MQL_Parser::RuleManhattanDistance;
}


std::any MQL_Parser::ManhattanDistanceContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitManhattanDistance(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::ManhattanDistanceContext* MQL_Parser::manhattanDistance() {
  ManhattanDistanceContext *_localctx = _tracker.createInstance<ManhattanDistanceContext>(_ctx, getState());
  enterRule(_localctx, 140, MQL_Parser::RuleManhattanDistance);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(828);
    match(MQL_Parser::K_MANHATTAN_DISTANCE);
    setState(829);
    match(MQL_Parser::L_PAR);
    setState(830);
    conditionalOrExpr();
    setState(831);
    match(MQL_Parser::COMMA);
    setState(832);
    conditionalOrExpr();
    setState(833);
    match(MQL_Parser::R_PAR);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- EuclideanDistanceContext ------------------------------------------------------------------

MQL_Parser::EuclideanDistanceContext::EuclideanDistanceContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::EuclideanDistanceContext::K_EUCLIDEAN_DISTANCE() {
  return getToken(MQL_Parser::K_EUCLIDEAN_DISTANCE, 0);
}

tree::TerminalNode* MQL_Parser::EuclideanDistanceContext::L_PAR() {
  return getToken(MQL_Parser::L_PAR, 0);
}

std::vector<MQL_Parser::ConditionalOrExprContext *> MQL_Parser::EuclideanDistanceContext::conditionalOrExpr() {
  return getRuleContexts<MQL_Parser::ConditionalOrExprContext>();
}

MQL_Parser::ConditionalOrExprContext* MQL_Parser::EuclideanDistanceContext::conditionalOrExpr(size_t i) {
  return getRuleContext<MQL_Parser::ConditionalOrExprContext>(i);
}

tree::TerminalNode* MQL_Parser::EuclideanDistanceContext::COMMA() {
  return getToken(MQL_Parser::COMMA, 0);
}

tree::TerminalNode* MQL_Parser::EuclideanDistanceContext::R_PAR() {
  return getToken(MQL_Parser::R_PAR, 0);
}


size_t MQL_Parser::EuclideanDistanceContext::getRuleIndex() const {
  return MQL_Parser::RuleEuclideanDistance;
}


std::any MQL_Parser::EuclideanDistanceContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitEuclideanDistance(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::EuclideanDistanceContext* MQL_Parser::euclideanDistance() {
  EuclideanDistanceContext *_localctx = _tracker.createInstance<EuclideanDistanceContext>(_ctx, getState());
  enterRule(_localctx, 142, MQL_Parser::RuleEuclideanDistance);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(835);
    match(MQL_Parser::K_EUCLIDEAN_DISTANCE);
    setState(836);
    match(MQL_Parser::L_PAR);
    setState(837);
    conditionalOrExpr();
    setState(838);
    match(MQL_Parser::COMMA);
    setState(839);
    conditionalOrExpr();
    setState(840);
    match(MQL_Parser::R_PAR);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- EditDistanceContext ------------------------------------------------------------------

MQL_Parser::EditDistanceContext::EditDistanceContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::EditDistanceContext::K_EDIT_DISTANCE() {
  return getToken(MQL_Parser::K_EDIT_DISTANCE, 0);
}

tree::TerminalNode* MQL_Parser::EditDistanceContext::L_PAR() {
  return getToken(MQL_Parser::L_PAR, 0);
}

std::vector<MQL_Parser::ConditionalOrExprContext *> MQL_Parser::EditDistanceContext::conditionalOrExpr() {
  return getRuleContexts<MQL_Parser::ConditionalOrExprContext>();
}

MQL_Parser::ConditionalOrExprContext* MQL_Parser::EditDistanceContext::conditionalOrExpr(size_t i) {
  return getRuleContext<MQL_Parser::ConditionalOrExprContext>(i);
}

tree::TerminalNode* MQL_Parser::EditDistanceContext::COMMA() {
  return getToken(MQL_Parser::COMMA, 0);
}

tree::TerminalNode* MQL_Parser::EditDistanceContext::R_PAR() {
  return getToken(MQL_Parser::R_PAR, 0);
}


size_t MQL_Parser::EditDistanceContext::getRuleIndex() const {
  return MQL_Parser::RuleEditDistance;
}


std::any MQL_Parser::EditDistanceContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitEditDistance(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::EditDistanceContext* MQL_Parser::editDistance() {
  EditDistanceContext *_localctx = _tracker.createInstance<EditDistanceContext>(_ctx, getState());
  enterRule(_localctx, 144, MQL_Parser::RuleEditDistance);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(842);
    match(MQL_Parser::K_EDIT_DISTANCE);
    setState(843);
    match(MQL_Parser::L_PAR);
    setState(844);
    conditionalOrExpr();
    setState(845);
    match(MQL_Parser::COMMA);
    setState(846);
    conditionalOrExpr();
    setState(847);
    match(MQL_Parser::R_PAR);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- NormalizeContext ------------------------------------------------------------------

MQL_Parser::NormalizeContext::NormalizeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::NormalizeContext::K_NORMALIZE() {
  return getToken(MQL_Parser::K_NORMALIZE, 0);
}

tree::TerminalNode* MQL_Parser::NormalizeContext::L_PAR() {
  return getToken(MQL_Parser::L_PAR, 0);
}

MQL_Parser::ConditionalOrExprContext* MQL_Parser::NormalizeContext::conditionalOrExpr() {
  return getRuleContext<MQL_Parser::ConditionalOrExprContext>(0);
}

tree::TerminalNode* MQL_Parser::NormalizeContext::R_PAR() {
  return getToken(MQL_Parser::R_PAR, 0);
}


size_t MQL_Parser::NormalizeContext::getRuleIndex() const {
  return MQL_Parser::RuleNormalize;
}


std::any MQL_Parser::NormalizeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitNormalize(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::NormalizeContext* MQL_Parser::normalize() {
  NormalizeContext *_localctx = _tracker.createInstance<NormalizeContext>(_ctx, getState());
  enterRule(_localctx, 146, MQL_Parser::RuleNormalize);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(849);
    match(MQL_Parser::K_NORMALIZE);
    setState(850);
    match(MQL_Parser::L_PAR);
    setState(851);
    conditionalOrExpr();
    setState(852);
    match(MQL_Parser::R_PAR);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TextSearchIndexModeContext ------------------------------------------------------------------

MQL_Parser::TextSearchIndexModeContext::TextSearchIndexModeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::TextSearchIndexModeContext::K_PREFIX() {
  return getToken(MQL_Parser::K_PREFIX, 0);
}

tree::TerminalNode* MQL_Parser::TextSearchIndexModeContext::K_MATCH() {
  return getToken(MQL_Parser::K_MATCH, 0);
}


size_t MQL_Parser::TextSearchIndexModeContext::getRuleIndex() const {
  return MQL_Parser::RuleTextSearchIndexMode;
}


std::any MQL_Parser::TextSearchIndexModeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitTextSearchIndexMode(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::TextSearchIndexModeContext* MQL_Parser::textSearchIndexMode() {
  TextSearchIndexModeContext *_localctx = _tracker.createInstance<TextSearchIndexModeContext>(_ctx, getState());
  enterRule(_localctx, 148, MQL_Parser::RuleTextSearchIndexMode);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(854);
    _la = _input->LA(1);
    if (!(_la == MQL_Parser::K_MATCH

    || _la == MQL_Parser::K_PREFIX)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExprTypenameContext ------------------------------------------------------------------

MQL_Parser::ExprTypenameContext::ExprTypenameContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::ExprTypenameContext::K_NULL() {
  return getToken(MQL_Parser::K_NULL, 0);
}

tree::TerminalNode* MQL_Parser::ExprTypenameContext::K_STRING() {
  return getToken(MQL_Parser::K_STRING, 0);
}

tree::TerminalNode* MQL_Parser::ExprTypenameContext::K_BOOL() {
  return getToken(MQL_Parser::K_BOOL, 0);
}

tree::TerminalNode* MQL_Parser::ExprTypenameContext::K_INTEGER() {
  return getToken(MQL_Parser::K_INTEGER, 0);
}

tree::TerminalNode* MQL_Parser::ExprTypenameContext::K_FLOAT() {
  return getToken(MQL_Parser::K_FLOAT, 0);
}


size_t MQL_Parser::ExprTypenameContext::getRuleIndex() const {
  return MQL_Parser::RuleExprTypename;
}


std::any MQL_Parser::ExprTypenameContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitExprTypename(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::ExprTypenameContext* MQL_Parser::exprTypename() {
  ExprTypenameContext *_localctx = _tracker.createInstance<ExprTypenameContext>(_ctx, getState());
  enterRule(_localctx, 150, MQL_Parser::RuleExprTypename);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(856);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 4629700419218571776) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- KeywordContext ------------------------------------------------------------------

MQL_Parser::KeywordContext::KeywordContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_ACYCLIC() {
  return getToken(MQL_Parser::K_ACYCLIC, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_AND() {
  return getToken(MQL_Parser::K_AND, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_ANY() {
  return getToken(MQL_Parser::K_ANY, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_AS() {
  return getToken(MQL_Parser::K_AS, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_AVG() {
  return getToken(MQL_Parser::K_AVG, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_ALL() {
  return getToken(MQL_Parser::K_ALL, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_ASC() {
  return getToken(MQL_Parser::K_ASC, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_BY() {
  return getToken(MQL_Parser::K_BY, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_BOOL() {
  return getToken(MQL_Parser::K_BOOL, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_CALL() {
  return getToken(MQL_Parser::K_CALL, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_COSINE_DISTANCE() {
  return getToken(MQL_Parser::K_COSINE_DISTANCE, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_COSINE_SIMILARITY() {
  return getToken(MQL_Parser::K_COSINE_SIMILARITY, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_COUNT() {
  return getToken(MQL_Parser::K_COUNT, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_CREATE() {
  return getToken(MQL_Parser::K_CREATE, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_DELETE() {
  return getToken(MQL_Parser::K_DELETE, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_DESCRIBE() {
  return getToken(MQL_Parser::K_DESCRIBE, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_DESC() {
  return getToken(MQL_Parser::K_DESC, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_DIMENSIONS() {
  return getToken(MQL_Parser::K_DIMENSIONS, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_DISTINCT() {
  return getToken(MQL_Parser::K_DISTINCT, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_EDGE() {
  return getToken(MQL_Parser::K_EDGE, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_EUCLIDEAN_DISTANCE() {
  return getToken(MQL_Parser::K_EUCLIDEAN_DISTANCE, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_FROM() {
  return getToken(MQL_Parser::K_FROM, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_INCOMING() {
  return getToken(MQL_Parser::K_INCOMING, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_INDEX() {
  return getToken(MQL_Parser::K_INDEX, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_INSERT() {
  return getToken(MQL_Parser::K_INSERT, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_INTEGER() {
  return getToken(MQL_Parser::K_INTEGER, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_INTO() {
  return getToken(MQL_Parser::K_INTO, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_IS() {
  return getToken(MQL_Parser::K_IS, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_FLOAT() {
  return getToken(MQL_Parser::K_FLOAT, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_GROUP() {
  return getToken(MQL_Parser::K_GROUP, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_GROUPS() {
  return getToken(MQL_Parser::K_GROUPS, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_IDENTITY() {
  return getToken(MQL_Parser::K_IDENTITY, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_LABELS() {
  return getToken(MQL_Parser::K_LABELS, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_LABEL() {
  return getToken(MQL_Parser::K_LABEL, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_LET() {
  return getToken(MQL_Parser::K_LET, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_LIMIT() {
  return getToken(MQL_Parser::K_LIMIT, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_MANHATTAN_DISTANCE() {
  return getToken(MQL_Parser::K_MANHATTAN_DISTANCE, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_MATCH() {
  return getToken(MQL_Parser::K_MATCH, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_MAX() {
  return getToken(MQL_Parser::K_MAX, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_MIN() {
  return getToken(MQL_Parser::K_MIN, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_NORMALIZE() {
  return getToken(MQL_Parser::K_NORMALIZE, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_OBJECTS() {
  return getToken(MQL_Parser::K_OBJECTS, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_OFFSET() {
  return getToken(MQL_Parser::K_OFFSET, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_OPTIONAL() {
  return getToken(MQL_Parser::K_OPTIONAL, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_ORDER() {
  return getToken(MQL_Parser::K_ORDER, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_OR() {
  return getToken(MQL_Parser::K_OR, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_OUTGOING() {
  return getToken(MQL_Parser::K_OUTGOING, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_PREFIX() {
  return getToken(MQL_Parser::K_PREFIX, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_PROPERTIES() {
  return getToken(MQL_Parser::K_PROPERTIES, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_PROPERTY() {
  return getToken(MQL_Parser::K_PROPERTY, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_NOT() {
  return getToken(MQL_Parser::K_NOT, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_NULL() {
  return getToken(MQL_Parser::K_NULL, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_SHORTEST() {
  return getToken(MQL_Parser::K_SHORTEST, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_SHOW() {
  return getToken(MQL_Parser::K_SHOW, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_SIMPLE() {
  return getToken(MQL_Parser::K_SIMPLE, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_TEXT_SEARCH() {
  return getToken(MQL_Parser::K_TEXT_SEARCH, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_REGEX() {
  return getToken(MQL_Parser::K_REGEX, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_RETURN() {
  return getToken(MQL_Parser::K_RETURN, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_SUM() {
  return getToken(MQL_Parser::K_SUM, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_STRING() {
  return getToken(MQL_Parser::K_STRING, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_TRAILS() {
  return getToken(MQL_Parser::K_TRAILS, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_VALUES() {
  return getToken(MQL_Parser::K_VALUES, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_WALKS() {
  return getToken(MQL_Parser::K_WALKS, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_WITH() {
  return getToken(MQL_Parser::K_WITH, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_WHERE() {
  return getToken(MQL_Parser::K_WHERE, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_YIELD() {
  return getToken(MQL_Parser::K_YIELD, 0);
}


size_t MQL_Parser::KeywordContext::getRuleIndex() const {
  return MQL_Parser::RuleKeyword;
}


std::any MQL_Parser::KeywordContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitKeyword(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::KeywordContext* MQL_Parser::keyword() {
  KeywordContext *_localctx = _tracker.createInstance<KeywordContext>(_ctx, getState());
  enterRule(_localctx, 152, MQL_Parser::RuleKeyword);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(858);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 9223372035778936830) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 64)) & 63) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

void MQL_Parser::initialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  mql_parserParserInitialize();
#else
  ::antlr4::internal::call_once(mql_parserParserOnceFlag, mql_parserParserInitialize);
#endif
}

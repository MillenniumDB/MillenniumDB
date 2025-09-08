
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
      "editDistance", "normalize", "str", "labels", "type", "textSearchIndexMode", 
      "exprTypename", "keyword"
    },
    std::vector<std::string>{
      "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
      "", "", "", "", "", "", "", "", "", "", "", "", "", "'false'", "", 
      "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
      "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "'true'", 
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
      "K_STRING", "K_STR", "K_TEXT_SEARCH", "K_TYPE", "K_REGEX", "K_RETURN", 
      "K_SUM", "K_TRUE", "K_TRAILS", "K_VALUES", "K_WALKS", "K_WITH", "K_WHERE", 
      "K_YIELD", "TRUE_PROP", "FALSE_PROP", "ANON_ID", "EDGE_ID", "KEY", 
      "TYPE", "TYPE_VAR", "VARIABLE", "STRING", "UNSIGNED_INTEGER", "UNSIGNED_FLOAT", 
      "UNSIGNED_SCIENTIFIC_NOTATION", "NAME", "LEQ", "GEQ", "EQ", "NEQ", 
      "LT", "GT", "SINGLE_EQ", "PATH_SEQUENCE", "PATH_ALTERNATIVE", "PATH_NEGATION", 
      "STAR", "PERCENT", "QUESTION_MARK", "PLUS", "MINUS", "L_PAR", "R_PAR", 
      "LCURLY_BRACKET", "RCURLY_BRACKET", "LSQUARE_BRACKET", "RSQUARE_BRACKET", 
      "COMMA", "COLON", "WHITE_SPACE", "SINGLE_LINE_COMMENT", "UNRECOGNIZED"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,1,110,885,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,6,2,
  	7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,2,14,7,
  	14,2,15,7,15,2,16,7,16,2,17,7,17,2,18,7,18,2,19,7,19,2,20,7,20,2,21,7,
  	21,2,22,7,22,2,23,7,23,2,24,7,24,2,25,7,25,2,26,7,26,2,27,7,27,2,28,7,
  	28,2,29,7,29,2,30,7,30,2,31,7,31,2,32,7,32,2,33,7,33,2,34,7,34,2,35,7,
  	35,2,36,7,36,2,37,7,37,2,38,7,38,2,39,7,39,2,40,7,40,2,41,7,41,2,42,7,
  	42,2,43,7,43,2,44,7,44,2,45,7,45,2,46,7,46,2,47,7,47,2,48,7,48,2,49,7,
  	49,2,50,7,50,2,51,7,51,2,52,7,52,2,53,7,53,2,54,7,54,2,55,7,55,2,56,7,
  	56,2,57,7,57,2,58,7,58,2,59,7,59,2,60,7,60,2,61,7,61,2,62,7,62,2,63,7,
  	63,2,64,7,64,2,65,7,65,2,66,7,66,2,67,7,67,2,68,7,68,2,69,7,69,2,70,7,
  	70,2,71,7,71,2,72,7,72,2,73,7,73,2,74,7,74,2,75,7,75,2,76,7,76,2,77,7,
  	77,2,78,7,78,2,79,7,79,1,0,1,0,1,0,1,0,1,0,3,0,166,8,0,1,0,1,0,1,1,1,
  	1,3,1,172,8,1,1,1,3,1,175,8,1,1,1,3,1,178,8,1,1,1,1,1,1,2,4,2,183,8,2,
  	11,2,12,2,184,1,3,1,3,1,3,3,3,190,8,3,1,4,1,4,1,4,1,4,5,4,196,8,4,10,
  	4,12,4,199,9,4,1,5,1,5,1,5,1,5,5,5,205,8,5,10,5,12,5,208,9,5,1,6,1,6,
  	3,6,212,8,6,1,6,5,6,215,8,6,10,6,12,6,218,9,6,1,6,3,6,221,8,6,1,6,1,6,
  	1,7,1,7,1,8,1,8,1,8,1,8,1,8,3,8,232,8,8,1,8,1,8,1,8,1,8,1,8,1,8,3,8,240,
  	8,8,1,8,1,8,1,8,3,8,245,8,8,1,9,1,9,1,9,1,9,1,9,1,9,1,9,1,10,1,10,1,10,
  	1,10,5,10,258,8,10,10,10,12,10,261,9,10,3,10,263,8,10,1,10,1,10,1,11,
  	1,11,1,11,1,11,3,11,271,8,11,1,12,1,12,1,12,1,12,1,12,3,12,278,8,12,1,
  	13,1,13,5,13,282,8,13,10,13,12,13,285,9,13,1,13,1,13,1,14,1,14,1,14,3,
  	14,292,8,14,1,15,1,15,1,15,1,16,1,16,1,16,1,17,1,17,1,17,5,17,303,8,17,
  	10,17,12,17,306,9,17,1,18,1,18,1,18,1,18,1,19,1,19,1,19,1,20,1,20,1,20,
  	1,20,1,20,5,20,320,8,20,10,20,12,20,323,9,20,1,21,1,21,1,21,1,21,1,21,
  	5,21,330,8,21,10,21,12,21,333,9,21,1,22,1,22,3,22,337,8,22,1,22,1,22,
  	1,22,5,22,342,8,22,10,22,12,22,345,9,22,1,22,3,22,348,8,22,1,22,1,22,
  	3,22,352,8,22,1,22,1,22,3,22,356,8,22,3,22,358,8,22,1,23,1,23,1,23,1,
  	23,3,23,364,8,23,1,23,1,23,3,23,368,8,23,1,24,1,24,1,24,1,24,1,24,5,24,
  	375,8,24,10,24,12,24,378,9,24,3,24,380,8,24,1,25,1,25,3,25,384,8,25,1,
  	26,1,26,1,26,5,26,389,8,26,10,26,12,26,392,9,26,1,27,1,27,3,27,396,8,
  	27,1,27,1,27,3,27,400,8,27,3,27,402,8,27,1,28,1,28,1,28,1,29,1,29,1,29,
  	1,30,1,30,3,30,412,8,30,1,30,1,30,1,30,1,30,3,30,418,8,30,1,30,1,30,3,
  	30,422,8,30,1,30,1,30,1,30,3,30,427,8,30,1,30,1,30,3,30,431,8,30,1,30,
  	3,30,434,8,30,1,30,1,30,3,30,438,8,30,1,30,1,30,1,30,3,30,443,8,30,1,
  	31,1,31,1,31,1,32,1,32,1,33,1,33,3,33,452,8,33,1,33,3,33,455,8,33,1,33,
  	1,33,1,33,1,33,3,33,461,8,33,1,33,1,33,3,33,465,8,33,1,33,1,33,1,33,3,
  	33,470,8,33,1,33,1,33,3,33,474,8,33,1,33,1,33,3,33,478,8,33,1,33,1,33,
  	3,33,482,8,33,3,33,484,8,33,1,34,1,34,3,34,488,8,34,1,35,1,35,5,35,492,
  	8,35,10,35,12,35,495,9,35,1,36,1,36,1,36,1,36,1,36,1,37,1,37,1,37,5,37,
  	505,8,37,10,37,12,37,508,9,37,1,38,1,38,1,38,3,38,513,8,38,1,38,1,38,
  	5,38,517,8,38,10,38,12,38,520,9,38,1,39,1,39,1,39,3,39,525,8,39,1,39,
  	3,39,528,8,39,1,39,1,39,1,39,1,39,1,39,1,39,1,39,3,39,537,8,39,1,39,3,
  	39,540,8,39,1,39,1,39,1,39,1,39,1,39,3,39,547,8,39,1,40,1,40,1,40,5,40,
  	552,8,40,10,40,12,40,555,9,40,1,41,1,41,1,41,5,41,560,8,41,10,41,12,41,
  	563,9,41,1,42,3,42,566,8,42,1,42,1,42,3,42,570,8,42,1,42,3,42,573,8,42,
  	1,42,1,42,1,42,1,42,3,42,579,8,42,3,42,581,8,42,1,43,1,43,1,43,1,43,1,
  	43,1,43,1,43,1,43,3,43,591,8,43,1,44,1,44,3,44,595,8,44,1,44,3,44,598,
  	8,44,1,44,1,44,1,44,3,44,603,8,44,1,44,3,44,606,8,44,3,44,608,8,44,1,
  	45,1,45,3,45,612,8,45,1,46,1,46,1,46,1,46,1,47,1,47,1,47,1,47,3,47,622,
  	8,47,1,48,1,48,1,48,3,48,627,8,48,1,48,1,48,1,48,1,48,1,48,3,48,634,8,
  	48,1,48,3,48,637,8,48,1,49,1,49,3,49,641,8,49,1,49,3,49,644,8,49,1,49,
  	3,49,647,8,49,1,49,1,49,1,50,1,50,3,50,653,8,50,1,50,5,50,656,8,50,10,
  	50,12,50,659,9,50,1,50,3,50,662,8,50,1,50,1,50,1,51,1,51,1,51,1,51,5,
  	51,670,8,51,10,51,12,51,673,9,51,1,51,1,51,1,52,1,52,1,52,1,52,1,52,3,
  	52,682,8,52,1,52,1,52,1,52,1,52,1,52,1,52,1,52,1,52,1,52,3,52,693,8,52,
  	1,52,1,52,5,52,697,8,52,10,52,12,52,700,9,52,1,52,1,52,1,52,1,52,3,52,
  	706,8,52,1,53,1,53,1,53,1,54,1,54,3,54,713,8,54,1,55,1,55,1,56,3,56,718,
  	8,56,1,56,1,56,1,57,1,57,1,57,1,57,1,57,1,58,1,58,1,58,1,58,3,58,731,
  	8,58,1,59,1,59,1,59,5,59,736,8,59,10,59,12,59,739,9,59,1,60,1,60,1,60,
  	5,60,744,8,60,10,60,12,60,747,9,60,1,61,1,61,1,61,3,61,752,8,61,1,61,
  	1,61,1,61,3,61,757,8,61,1,61,1,61,3,61,761,8,61,1,62,1,62,1,62,5,62,766,
  	8,62,10,62,12,62,769,9,62,1,63,1,63,1,63,5,63,774,8,63,10,63,12,63,777,
  	9,63,1,64,1,64,1,64,1,64,1,64,1,64,1,64,3,64,786,8,64,1,65,1,65,3,65,
  	790,8,65,1,65,1,65,1,65,1,65,1,65,1,65,1,65,3,65,799,8,65,1,66,1,66,1,
  	66,1,66,1,66,1,66,1,66,1,66,1,66,1,66,3,66,811,8,66,1,67,1,67,1,67,1,
  	67,1,67,1,67,1,67,3,67,820,8,67,1,67,1,67,1,68,1,68,1,68,1,68,1,68,1,
  	68,1,68,1,69,1,69,1,69,1,69,1,69,1,69,1,69,1,70,1,70,1,70,1,70,1,70,1,
  	70,1,70,1,71,1,71,1,71,1,71,1,71,1,71,1,71,1,72,1,72,1,72,1,72,1,72,1,
  	72,1,72,1,73,1,73,1,73,1,73,1,73,1,74,1,74,1,74,1,74,1,74,1,75,1,75,1,
  	75,1,75,1,75,1,76,1,76,1,76,1,76,1,76,1,77,1,77,1,78,1,78,1,79,1,79,1,
  	79,0,0,80,0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,34,36,38,40,42,
  	44,46,48,50,52,54,56,58,60,62,64,66,68,70,72,74,76,78,80,82,84,86,88,
  	90,92,94,96,98,100,102,104,106,108,110,112,114,116,118,120,122,124,126,
  	128,130,132,134,136,138,140,142,144,146,148,150,152,154,156,158,0,15,
  	4,0,24,24,35,35,49,49,51,51,3,0,5,5,41,42,64,64,2,0,7,7,17,17,2,0,3,3,
  	6,6,4,0,1,1,57,57,66,66,68,68,2,0,75,75,79,79,1,0,77,78,1,0,85,90,2,0,
  	30,30,65,65,1,0,98,99,1,0,81,83,2,0,92,92,95,96,2,0,40,40,50,50,5,0,9,
  	9,27,27,31,31,54,54,58,58,3,0,1,29,31,64,66,71,943,0,165,1,0,0,0,2,169,
  	1,0,0,0,4,182,1,0,0,0,6,189,1,0,0,0,8,191,1,0,0,0,10,200,1,0,0,0,12,209,
  	1,0,0,0,14,224,1,0,0,0,16,244,1,0,0,0,18,246,1,0,0,0,20,253,1,0,0,0,22,
  	266,1,0,0,0,24,272,1,0,0,0,26,279,1,0,0,0,28,288,1,0,0,0,30,293,1,0,0,
  	0,32,296,1,0,0,0,34,299,1,0,0,0,36,307,1,0,0,0,38,311,1,0,0,0,40,314,
  	1,0,0,0,42,324,1,0,0,0,44,357,1,0,0,0,46,359,1,0,0,0,48,369,1,0,0,0,50,
  	381,1,0,0,0,52,385,1,0,0,0,54,401,1,0,0,0,56,403,1,0,0,0,58,406,1,0,0,
  	0,60,442,1,0,0,0,62,444,1,0,0,0,64,447,1,0,0,0,66,483,1,0,0,0,68,485,
  	1,0,0,0,70,489,1,0,0,0,72,496,1,0,0,0,74,501,1,0,0,0,76,509,1,0,0,0,78,
  	546,1,0,0,0,80,548,1,0,0,0,82,556,1,0,0,0,84,580,1,0,0,0,86,590,1,0,0,
  	0,88,607,1,0,0,0,90,611,1,0,0,0,92,613,1,0,0,0,94,621,1,0,0,0,96,636,
  	1,0,0,0,98,638,1,0,0,0,100,650,1,0,0,0,102,665,1,0,0,0,104,705,1,0,0,
  	0,106,707,1,0,0,0,108,712,1,0,0,0,110,714,1,0,0,0,112,717,1,0,0,0,114,
  	721,1,0,0,0,116,730,1,0,0,0,118,732,1,0,0,0,120,740,1,0,0,0,122,760,1,
  	0,0,0,124,762,1,0,0,0,126,770,1,0,0,0,128,785,1,0,0,0,130,798,1,0,0,0,
  	132,810,1,0,0,0,134,812,1,0,0,0,136,823,1,0,0,0,138,830,1,0,0,0,140,837,
  	1,0,0,0,142,844,1,0,0,0,144,851,1,0,0,0,146,858,1,0,0,0,148,863,1,0,0,
  	0,150,868,1,0,0,0,152,873,1,0,0,0,154,878,1,0,0,0,156,880,1,0,0,0,158,
  	882,1,0,0,0,160,166,3,2,1,0,161,166,3,26,13,0,162,166,3,8,4,0,163,166,
  	3,18,9,0,164,166,3,24,12,0,165,160,1,0,0,0,165,161,1,0,0,0,165,162,1,
  	0,0,0,165,163,1,0,0,0,165,164,1,0,0,0,166,167,1,0,0,0,167,168,5,0,0,1,
  	168,1,1,0,0,0,169,171,3,4,2,0,170,172,3,38,19,0,171,170,1,0,0,0,171,172,
  	1,0,0,0,172,174,1,0,0,0,173,175,3,40,20,0,174,173,1,0,0,0,174,175,1,0,
  	0,0,175,177,1,0,0,0,176,178,3,42,21,0,177,176,1,0,0,0,177,178,1,0,0,0,
  	178,179,1,0,0,0,179,180,3,44,22,0,180,3,1,0,0,0,181,183,3,6,3,0,182,181,
  	1,0,0,0,183,184,1,0,0,0,184,182,1,0,0,0,184,185,1,0,0,0,185,5,1,0,0,0,
  	186,190,3,30,15,0,187,190,3,46,23,0,188,190,3,32,16,0,189,186,1,0,0,0,
  	189,187,1,0,0,0,189,188,1,0,0,0,190,7,1,0,0,0,191,192,5,26,0,0,192,197,
  	3,10,5,0,193,194,5,106,0,0,194,196,3,10,5,0,195,193,1,0,0,0,196,199,1,
  	0,0,0,197,195,1,0,0,0,197,198,1,0,0,0,198,9,1,0,0,0,199,197,1,0,0,0,200,
  	206,3,12,6,0,201,202,3,16,8,0,202,203,3,12,6,0,203,205,1,0,0,0,204,201,
  	1,0,0,0,205,208,1,0,0,0,206,204,1,0,0,0,206,207,1,0,0,0,207,11,1,0,0,
  	0,208,206,1,0,0,0,209,211,5,100,0,0,210,212,3,14,7,0,211,210,1,0,0,0,
  	211,212,1,0,0,0,212,216,1,0,0,0,213,215,5,77,0,0,214,213,1,0,0,0,215,
  	218,1,0,0,0,216,214,1,0,0,0,216,217,1,0,0,0,217,220,1,0,0,0,218,216,1,
  	0,0,0,219,221,3,102,51,0,220,219,1,0,0,0,220,221,1,0,0,0,221,222,1,0,
  	0,0,222,223,5,101,0,0,223,13,1,0,0,0,224,225,3,108,54,0,225,15,1,0,0,
  	0,226,227,5,89,0,0,227,228,5,99,0,0,228,229,5,104,0,0,229,231,5,77,0,
  	0,230,232,3,102,51,0,231,230,1,0,0,0,231,232,1,0,0,0,232,233,1,0,0,0,
  	233,234,5,105,0,0,234,245,5,99,0,0,235,236,5,99,0,0,236,237,5,104,0,0,
  	237,239,5,77,0,0,238,240,3,102,51,0,239,238,1,0,0,0,239,240,1,0,0,0,240,
  	241,1,0,0,0,241,242,5,105,0,0,242,243,5,99,0,0,243,245,5,90,0,0,244,226,
  	1,0,0,0,244,235,1,0,0,0,245,17,1,0,0,0,246,247,5,14,0,0,247,248,3,108,
  	54,0,248,249,5,25,0,0,249,250,5,80,0,0,250,251,5,69,0,0,251,252,3,20,
  	10,0,252,19,1,0,0,0,253,262,5,102,0,0,254,259,3,22,11,0,255,256,5,106,
  	0,0,256,258,3,22,11,0,257,255,1,0,0,0,258,261,1,0,0,0,259,257,1,0,0,0,
  	259,260,1,0,0,0,260,263,1,0,0,0,261,259,1,0,0,0,262,254,1,0,0,0,262,263,
  	1,0,0,0,263,264,1,0,0,0,264,265,5,103,0,0,265,21,1,0,0,0,266,270,5,80,
  	0,0,267,268,5,91,0,0,268,271,3,116,58,0,269,271,3,110,55,0,270,267,1,
  	0,0,0,270,269,1,0,0,0,271,23,1,0,0,0,272,277,5,56,0,0,273,278,1,0,0,0,
  	274,275,3,108,54,0,275,276,5,25,0,0,276,278,1,0,0,0,277,273,1,0,0,0,277,
  	274,1,0,0,0,278,25,1,0,0,0,279,283,5,16,0,0,280,282,3,28,14,0,281,280,
  	1,0,0,0,282,285,1,0,0,0,283,281,1,0,0,0,283,284,1,0,0,0,284,286,1,0,0,
  	0,285,283,1,0,0,0,286,287,3,94,47,0,287,27,1,0,0,0,288,291,7,0,0,0,289,
  	290,5,38,0,0,290,292,5,81,0,0,291,289,1,0,0,0,291,292,1,0,0,0,292,29,
  	1,0,0,0,293,294,5,40,0,0,294,295,3,70,35,0,295,31,1,0,0,0,296,297,5,37,
  	0,0,297,298,3,34,17,0,298,33,1,0,0,0,299,304,3,36,18,0,300,301,5,106,
  	0,0,301,303,3,36,18,0,302,300,1,0,0,0,303,306,1,0,0,0,304,302,1,0,0,0,
  	304,305,1,0,0,0,305,35,1,0,0,0,306,304,1,0,0,0,307,308,5,79,0,0,308,309,
  	5,91,0,0,309,310,3,118,59,0,310,37,1,0,0,0,311,312,5,70,0,0,312,313,3,
  	118,59,0,313,39,1,0,0,0,314,315,5,32,0,0,315,316,5,8,0,0,316,321,3,68,
  	34,0,317,318,5,106,0,0,318,320,3,68,34,0,319,317,1,0,0,0,320,323,1,0,
  	0,0,321,319,1,0,0,0,321,322,1,0,0,0,322,41,1,0,0,0,323,321,1,0,0,0,324,
  	325,5,47,0,0,325,326,5,8,0,0,326,331,3,66,33,0,327,328,5,106,0,0,328,
  	330,3,66,33,0,329,327,1,0,0,0,330,333,1,0,0,0,331,329,1,0,0,0,331,332,
  	1,0,0,0,332,43,1,0,0,0,333,331,1,0,0,0,334,336,5,63,0,0,335,337,5,19,
  	0,0,336,335,1,0,0,0,336,337,1,0,0,0,337,338,1,0,0,0,338,343,3,60,30,0,
  	339,340,5,106,0,0,340,342,3,60,30,0,341,339,1,0,0,0,342,345,1,0,0,0,343,
  	341,1,0,0,0,343,344,1,0,0,0,344,347,1,0,0,0,345,343,1,0,0,0,346,348,3,
  	54,27,0,347,346,1,0,0,0,347,348,1,0,0,0,348,358,1,0,0,0,349,351,5,63,
  	0,0,350,352,5,19,0,0,351,350,1,0,0,0,351,352,1,0,0,0,352,353,1,0,0,0,
  	353,355,5,95,0,0,354,356,3,54,27,0,355,354,1,0,0,0,355,356,1,0,0,0,356,
  	358,1,0,0,0,357,334,1,0,0,0,357,349,1,0,0,0,358,45,1,0,0,0,359,360,5,
  	10,0,0,360,361,3,108,54,0,361,363,5,100,0,0,362,364,3,52,26,0,363,362,
  	1,0,0,0,363,364,1,0,0,0,364,365,1,0,0,0,365,367,5,101,0,0,366,368,3,48,
  	24,0,367,366,1,0,0,0,367,368,1,0,0,0,368,47,1,0,0,0,369,379,5,71,0,0,
  	370,380,5,95,0,0,371,376,3,50,25,0,372,373,5,106,0,0,373,375,3,50,25,
  	0,374,372,1,0,0,0,375,378,1,0,0,0,376,374,1,0,0,0,376,377,1,0,0,0,377,
  	380,1,0,0,0,378,376,1,0,0,0,379,370,1,0,0,0,379,371,1,0,0,0,380,49,1,
  	0,0,0,381,383,5,79,0,0,382,384,3,62,31,0,383,382,1,0,0,0,383,384,1,0,
  	0,0,384,51,1,0,0,0,385,390,3,118,59,0,386,387,5,106,0,0,387,389,3,118,
  	59,0,388,386,1,0,0,0,389,392,1,0,0,0,390,388,1,0,0,0,390,391,1,0,0,0,
  	391,53,1,0,0,0,392,390,1,0,0,0,393,395,3,56,28,0,394,396,3,58,29,0,395,
  	394,1,0,0,0,395,396,1,0,0,0,396,402,1,0,0,0,397,399,3,58,29,0,398,400,
  	3,56,28,0,399,398,1,0,0,0,399,400,1,0,0,0,400,402,1,0,0,0,401,393,1,0,
  	0,0,401,397,1,0,0,0,402,55,1,0,0,0,403,404,5,38,0,0,404,405,5,81,0,0,
  	405,57,1,0,0,0,406,407,5,45,0,0,407,408,5,81,0,0,408,59,1,0,0,0,409,411,
  	5,79,0,0,410,412,5,76,0,0,411,410,1,0,0,0,411,412,1,0,0,0,412,443,1,0,
  	0,0,413,414,3,64,32,0,414,415,5,100,0,0,415,417,5,79,0,0,416,418,5,76,
  	0,0,417,416,1,0,0,0,417,418,1,0,0,0,418,419,1,0,0,0,419,421,5,101,0,0,
  	420,422,3,62,31,0,421,420,1,0,0,0,421,422,1,0,0,0,422,443,1,0,0,0,423,
  	424,5,13,0,0,424,426,5,100,0,0,425,427,5,19,0,0,426,425,1,0,0,0,426,427,
  	1,0,0,0,427,433,1,0,0,0,428,430,5,79,0,0,429,431,5,76,0,0,430,429,1,0,
  	0,0,430,431,1,0,0,0,431,434,1,0,0,0,432,434,5,95,0,0,433,428,1,0,0,0,
  	433,432,1,0,0,0,434,435,1,0,0,0,435,437,5,101,0,0,436,438,3,62,31,0,437,
  	436,1,0,0,0,437,438,1,0,0,0,438,443,1,0,0,0,439,440,3,118,59,0,440,441,
  	3,62,31,0,441,443,1,0,0,0,442,409,1,0,0,0,442,413,1,0,0,0,442,423,1,0,
  	0,0,442,439,1,0,0,0,443,61,1,0,0,0,444,445,5,4,0,0,445,446,5,79,0,0,446,
  	63,1,0,0,0,447,448,7,1,0,0,448,65,1,0,0,0,449,451,5,79,0,0,450,452,5,
  	76,0,0,451,450,1,0,0,0,451,452,1,0,0,0,452,454,1,0,0,0,453,455,7,2,0,
  	0,454,453,1,0,0,0,454,455,1,0,0,0,455,484,1,0,0,0,456,457,3,64,32,0,457,
  	458,5,100,0,0,458,460,5,79,0,0,459,461,5,76,0,0,460,459,1,0,0,0,460,461,
  	1,0,0,0,461,462,1,0,0,0,462,464,5,101,0,0,463,465,7,2,0,0,464,463,1,0,
  	0,0,464,465,1,0,0,0,465,484,1,0,0,0,466,467,5,13,0,0,467,469,5,100,0,
  	0,468,470,5,19,0,0,469,468,1,0,0,0,469,470,1,0,0,0,470,471,1,0,0,0,471,
  	473,5,79,0,0,472,474,5,76,0,0,473,472,1,0,0,0,473,474,1,0,0,0,474,475,
  	1,0,0,0,475,477,5,101,0,0,476,478,7,2,0,0,477,476,1,0,0,0,477,478,1,0,
  	0,0,478,484,1,0,0,0,479,481,3,118,59,0,480,482,7,2,0,0,481,480,1,0,0,
  	0,481,482,1,0,0,0,482,484,1,0,0,0,483,449,1,0,0,0,483,456,1,0,0,0,483,
  	466,1,0,0,0,483,479,1,0,0,0,484,67,1,0,0,0,485,487,5,79,0,0,486,488,5,
  	76,0,0,487,486,1,0,0,0,487,488,1,0,0,0,488,69,1,0,0,0,489,493,3,74,37,
  	0,490,492,3,72,36,0,491,490,1,0,0,0,492,495,1,0,0,0,493,491,1,0,0,0,493,
  	494,1,0,0,0,494,71,1,0,0,0,495,493,1,0,0,0,496,497,5,46,0,0,497,498,5,
  	102,0,0,498,499,3,70,35,0,499,500,5,103,0,0,500,73,1,0,0,0,501,506,3,
  	76,38,0,502,503,5,106,0,0,503,505,3,76,38,0,504,502,1,0,0,0,505,508,1,
  	0,0,0,506,504,1,0,0,0,506,507,1,0,0,0,507,75,1,0,0,0,508,506,1,0,0,0,
  	509,518,3,90,45,0,510,513,3,96,48,0,511,513,3,78,39,0,512,510,1,0,0,0,
  	512,511,1,0,0,0,513,514,1,0,0,0,514,515,3,90,45,0,515,517,1,0,0,0,516,
  	512,1,0,0,0,517,520,1,0,0,0,518,516,1,0,0,0,518,519,1,0,0,0,519,77,1,
  	0,0,0,520,518,1,0,0,0,521,522,5,85,0,0,522,524,5,104,0,0,523,525,3,88,
  	44,0,524,523,1,0,0,0,524,525,1,0,0,0,525,527,1,0,0,0,526,528,5,79,0,0,
  	527,526,1,0,0,0,527,528,1,0,0,0,528,529,1,0,0,0,529,530,3,80,40,0,530,
  	531,5,105,0,0,531,532,5,91,0,0,532,547,1,0,0,0,533,534,5,91,0,0,534,536,
  	5,104,0,0,535,537,3,88,44,0,536,535,1,0,0,0,536,537,1,0,0,0,537,539,1,
  	0,0,0,538,540,5,79,0,0,539,538,1,0,0,0,539,540,1,0,0,0,540,541,1,0,0,
  	0,541,542,3,80,40,0,542,543,5,105,0,0,543,544,5,91,0,0,544,545,5,90,0,
  	0,545,547,1,0,0,0,546,521,1,0,0,0,546,533,1,0,0,0,547,79,1,0,0,0,548,
  	553,3,82,41,0,549,550,5,93,0,0,550,552,3,82,41,0,551,549,1,0,0,0,552,
  	555,1,0,0,0,553,551,1,0,0,0,553,554,1,0,0,0,554,81,1,0,0,0,555,553,1,
  	0,0,0,556,561,3,84,42,0,557,558,5,92,0,0,558,560,3,84,42,0,559,557,1,
  	0,0,0,560,563,1,0,0,0,561,559,1,0,0,0,561,562,1,0,0,0,562,83,1,0,0,0,
  	563,561,1,0,0,0,564,566,5,94,0,0,565,564,1,0,0,0,565,566,1,0,0,0,566,
  	567,1,0,0,0,567,569,5,77,0,0,568,570,3,86,43,0,569,568,1,0,0,0,569,570,
  	1,0,0,0,570,581,1,0,0,0,571,573,5,94,0,0,572,571,1,0,0,0,572,573,1,0,
  	0,0,573,574,1,0,0,0,574,575,5,100,0,0,575,576,3,80,40,0,576,578,5,101,
  	0,0,577,579,3,86,43,0,578,577,1,0,0,0,578,579,1,0,0,0,579,581,1,0,0,0,
  	580,565,1,0,0,0,580,572,1,0,0,0,581,85,1,0,0,0,582,591,5,95,0,0,583,591,
  	5,98,0,0,584,591,5,97,0,0,585,586,5,102,0,0,586,587,5,81,0,0,587,588,
  	5,106,0,0,588,589,5,81,0,0,589,591,5,103,0,0,590,582,1,0,0,0,590,583,
  	1,0,0,0,590,584,1,0,0,0,590,585,1,0,0,0,591,87,1,0,0,0,592,594,7,3,0,
  	0,593,595,5,55,0,0,594,593,1,0,0,0,594,595,1,0,0,0,595,597,1,0,0,0,596,
  	598,7,4,0,0,597,596,1,0,0,0,597,598,1,0,0,0,598,608,1,0,0,0,599,600,5,
  	55,0,0,600,602,5,81,0,0,601,603,5,33,0,0,602,601,1,0,0,0,602,603,1,0,
  	0,0,603,605,1,0,0,0,604,606,7,4,0,0,605,604,1,0,0,0,605,606,1,0,0,0,606,
  	608,1,0,0,0,607,592,1,0,0,0,607,599,1,0,0,0,608,89,1,0,0,0,609,612,3,
  	92,46,0,610,612,3,100,50,0,611,609,1,0,0,0,611,610,1,0,0,0,612,91,1,0,
  	0,0,613,614,5,100,0,0,614,615,3,94,47,0,615,616,5,101,0,0,616,93,1,0,
  	0,0,617,622,3,108,54,0,618,622,5,74,0,0,619,622,5,75,0,0,620,622,3,116,
  	58,0,621,617,1,0,0,0,621,618,1,0,0,0,621,619,1,0,0,0,621,620,1,0,0,0,
  	622,95,1,0,0,0,623,626,5,89,0,0,624,625,5,99,0,0,625,627,3,98,49,0,626,
  	624,1,0,0,0,626,627,1,0,0,0,627,628,1,0,0,0,628,637,5,99,0,0,629,633,
  	5,99,0,0,630,631,3,98,49,0,631,632,5,99,0,0,632,634,1,0,0,0,633,630,1,
  	0,0,0,633,634,1,0,0,0,634,635,1,0,0,0,635,637,5,90,0,0,636,623,1,0,0,
  	0,636,629,1,0,0,0,637,97,1,0,0,0,638,640,5,104,0,0,639,641,7,5,0,0,640,
  	639,1,0,0,0,640,641,1,0,0,0,641,643,1,0,0,0,642,644,7,6,0,0,643,642,1,
  	0,0,0,643,644,1,0,0,0,644,646,1,0,0,0,645,647,3,102,51,0,646,645,1,0,
  	0,0,646,647,1,0,0,0,647,648,1,0,0,0,648,649,5,105,0,0,649,99,1,0,0,0,
  	650,652,5,100,0,0,651,653,5,79,0,0,652,651,1,0,0,0,652,653,1,0,0,0,653,
  	657,1,0,0,0,654,656,5,77,0,0,655,654,1,0,0,0,656,659,1,0,0,0,657,655,
  	1,0,0,0,657,658,1,0,0,0,658,661,1,0,0,0,659,657,1,0,0,0,660,662,3,102,
  	51,0,661,660,1,0,0,0,661,662,1,0,0,0,662,663,1,0,0,0,663,664,5,101,0,
  	0,664,101,1,0,0,0,665,666,5,102,0,0,666,671,3,104,52,0,667,668,5,106,
  	0,0,668,670,3,104,52,0,669,667,1,0,0,0,670,673,1,0,0,0,671,669,1,0,0,
  	0,671,672,1,0,0,0,672,674,1,0,0,0,673,671,1,0,0,0,674,675,5,103,0,0,675,
  	103,1,0,0,0,676,681,3,108,54,0,677,678,5,107,0,0,678,682,3,116,58,0,679,
  	682,5,72,0,0,680,682,5,73,0,0,681,677,1,0,0,0,681,679,1,0,0,0,681,680,
  	1,0,0,0,682,706,1,0,0,0,683,684,3,108,54,0,684,685,5,77,0,0,685,686,5,
  	100,0,0,686,687,5,80,0,0,687,688,5,101,0,0,688,706,1,0,0,0,689,690,3,
  	108,54,0,690,692,5,29,0,0,691,693,5,53,0,0,692,691,1,0,0,0,692,693,1,
  	0,0,0,693,694,1,0,0,0,694,698,3,156,78,0,695,697,3,106,53,0,696,695,1,
  	0,0,0,697,700,1,0,0,0,698,696,1,0,0,0,698,699,1,0,0,0,699,706,1,0,0,0,
  	700,698,1,0,0,0,701,702,3,108,54,0,702,703,7,7,0,0,703,704,3,116,58,0,
  	704,706,1,0,0,0,705,676,1,0,0,0,705,683,1,0,0,0,705,689,1,0,0,0,705,701,
  	1,0,0,0,706,105,1,0,0,0,707,708,5,48,0,0,708,709,3,156,78,0,709,107,1,
  	0,0,0,710,713,5,84,0,0,711,713,3,158,79,0,712,710,1,0,0,0,712,711,1,0,
  	0,0,713,109,1,0,0,0,714,715,7,8,0,0,715,111,1,0,0,0,716,718,7,9,0,0,717,
  	716,1,0,0,0,717,718,1,0,0,0,718,719,1,0,0,0,719,720,7,10,0,0,720,113,
  	1,0,0,0,721,722,3,108,54,0,722,723,5,100,0,0,723,724,5,80,0,0,724,725,
  	5,101,0,0,725,115,1,0,0,0,726,731,3,112,56,0,727,731,5,80,0,0,728,731,
  	3,110,55,0,729,731,3,114,57,0,730,726,1,0,0,0,730,727,1,0,0,0,730,728,
  	1,0,0,0,730,729,1,0,0,0,731,117,1,0,0,0,732,737,3,120,60,0,733,734,5,
  	48,0,0,734,736,3,120,60,0,735,733,1,0,0,0,736,739,1,0,0,0,737,735,1,0,
  	0,0,737,738,1,0,0,0,738,119,1,0,0,0,739,737,1,0,0,0,740,745,3,122,61,
  	0,741,742,5,2,0,0,742,744,3,122,61,0,743,741,1,0,0,0,744,747,1,0,0,0,
  	745,743,1,0,0,0,745,746,1,0,0,0,746,121,1,0,0,0,747,745,1,0,0,0,748,751,
  	3,124,62,0,749,750,7,7,0,0,750,752,3,124,62,0,751,749,1,0,0,0,751,752,
  	1,0,0,0,752,761,1,0,0,0,753,754,3,124,62,0,754,756,5,29,0,0,755,757,5,
  	53,0,0,756,755,1,0,0,0,756,757,1,0,0,0,757,758,1,0,0,0,758,759,3,156,
  	78,0,759,761,1,0,0,0,760,748,1,0,0,0,760,753,1,0,0,0,761,123,1,0,0,0,
  	762,767,3,126,63,0,763,764,7,9,0,0,764,766,3,126,63,0,765,763,1,0,0,0,
  	766,769,1,0,0,0,767,765,1,0,0,0,767,768,1,0,0,0,768,125,1,0,0,0,769,767,
  	1,0,0,0,770,775,3,128,64,0,771,772,7,11,0,0,772,774,3,128,64,0,773,771,
  	1,0,0,0,774,777,1,0,0,0,775,773,1,0,0,0,775,776,1,0,0,0,776,127,1,0,0,
  	0,777,775,1,0,0,0,778,779,5,53,0,0,779,786,3,128,64,0,780,786,3,130,65,
  	0,781,782,5,98,0,0,782,786,3,128,64,0,783,784,5,99,0,0,784,786,3,128,
  	64,0,785,778,1,0,0,0,785,780,1,0,0,0,785,781,1,0,0,0,785,783,1,0,0,0,
  	786,129,1,0,0,0,787,789,5,79,0,0,788,790,5,76,0,0,789,788,1,0,0,0,789,
  	790,1,0,0,0,790,799,1,0,0,0,791,799,3,132,66,0,792,799,3,116,58,0,793,
  	799,3,94,47,0,794,795,5,100,0,0,795,796,3,118,59,0,796,797,5,101,0,0,
  	797,799,1,0,0,0,798,787,1,0,0,0,798,791,1,0,0,0,798,792,1,0,0,0,798,793,
  	1,0,0,0,798,794,1,0,0,0,799,131,1,0,0,0,800,811,3,134,67,0,801,811,3,
  	136,68,0,802,811,3,138,69,0,803,811,3,140,70,0,804,811,3,142,71,0,805,
  	811,3,144,72,0,806,811,3,146,73,0,807,811,3,148,74,0,808,811,3,150,75,
  	0,809,811,3,152,76,0,810,800,1,0,0,0,810,801,1,0,0,0,810,802,1,0,0,0,
  	810,803,1,0,0,0,810,804,1,0,0,0,810,805,1,0,0,0,810,806,1,0,0,0,810,807,
  	1,0,0,0,810,808,1,0,0,0,810,809,1,0,0,0,811,133,1,0,0,0,812,813,5,62,
  	0,0,813,814,5,100,0,0,814,815,3,118,59,0,815,816,5,106,0,0,816,819,3,
  	118,59,0,817,818,5,106,0,0,818,820,3,118,59,0,819,817,1,0,0,0,819,820,
  	1,0,0,0,820,821,1,0,0,0,821,822,5,101,0,0,822,135,1,0,0,0,823,824,5,12,
  	0,0,824,825,5,100,0,0,825,826,3,118,59,0,826,827,5,106,0,0,827,828,3,
  	118,59,0,828,829,5,101,0,0,829,137,1,0,0,0,830,831,5,11,0,0,831,832,5,
  	100,0,0,832,833,3,118,59,0,833,834,5,106,0,0,834,835,3,118,59,0,835,836,
  	5,101,0,0,836,139,1,0,0,0,837,838,5,39,0,0,838,839,5,100,0,0,839,840,
  	3,118,59,0,840,841,5,106,0,0,841,842,3,118,59,0,842,843,5,101,0,0,843,
  	141,1,0,0,0,844,845,5,22,0,0,845,846,5,100,0,0,846,847,3,118,59,0,847,
  	848,5,106,0,0,848,849,3,118,59,0,849,850,5,101,0,0,850,143,1,0,0,0,851,
  	852,5,21,0,0,852,853,5,100,0,0,853,854,3,118,59,0,854,855,5,106,0,0,855,
  	856,3,118,59,0,856,857,5,101,0,0,857,145,1,0,0,0,858,859,5,43,0,0,859,
  	860,5,100,0,0,860,861,3,118,59,0,861,862,5,101,0,0,862,147,1,0,0,0,863,
  	864,5,59,0,0,864,865,5,100,0,0,865,866,3,118,59,0,866,867,5,101,0,0,867,
  	149,1,0,0,0,868,869,5,35,0,0,869,870,5,100,0,0,870,871,5,79,0,0,871,872,
  	5,101,0,0,872,151,1,0,0,0,873,874,5,61,0,0,874,875,5,100,0,0,875,876,
  	5,79,0,0,876,877,5,101,0,0,877,153,1,0,0,0,878,879,7,12,0,0,879,155,1,
  	0,0,0,880,881,7,13,0,0,881,157,1,0,0,0,882,883,7,14,0,0,883,159,1,0,0,
  	0,109,165,171,174,177,184,189,197,206,211,216,220,231,239,244,259,262,
  	270,277,283,291,304,321,331,336,343,347,351,355,357,363,367,376,379,383,
  	390,395,399,401,411,417,421,426,430,433,437,442,451,454,460,464,469,473,
  	477,481,483,487,493,506,512,518,524,527,536,539,546,553,561,565,569,572,
  	578,580,590,594,597,602,605,607,611,621,626,633,636,640,643,646,652,657,
  	661,671,681,692,698,705,712,717,730,737,745,751,756,760,767,775,785,789,
  	798,810,819
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
    setState(165);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case MQL_Parser::K_CALL:
      case MQL_Parser::K_LET:
      case MQL_Parser::K_MATCH: {
        setState(160);
        matchQuery();
        break;
      }

      case MQL_Parser::K_DESCRIBE: {
        setState(161);
        describeQuery();
        break;
      }

      case MQL_Parser::K_INSERT: {
        setState(162);
        insertPatterns();
        break;
      }

      case MQL_Parser::K_CREATE: {
        setState(163);
        createIndexQuery();
        break;
      }

      case MQL_Parser::K_SHOW: {
        setState(164);
        showQuery();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    setState(167);
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
    setState(169);
    primitiveStatementList();
    setState(171);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MQL_Parser::K_WHERE) {
      setState(170);
      whereStatement();
    }
    setState(174);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MQL_Parser::K_GROUP) {
      setState(173);
      groupByStatement();
    }
    setState(177);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MQL_Parser::K_ORDER) {
      setState(176);
      orderByStatement();
    }
    setState(179);
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
    setState(182); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(181);
      primitiveStatement();
      setState(184); 
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
    setState(189);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case MQL_Parser::K_MATCH: {
        enterOuterAlt(_localctx, 1);
        setState(186);
        matchStatement();
        break;
      }

      case MQL_Parser::K_CALL: {
        enterOuterAlt(_localctx, 2);
        setState(187);
        callStatement();
        break;
      }

      case MQL_Parser::K_LET: {
        enterOuterAlt(_localctx, 3);
        setState(188);
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
    setState(191);
    match(MQL_Parser::K_INSERT);
    setState(192);
    insertLinearPattern();
    setState(197);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MQL_Parser::COMMA) {
      setState(193);
      match(MQL_Parser::COMMA);
      setState(194);
      insertLinearPattern();
      setState(199);
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
    setState(200);
    insertPlainNode();
    setState(206);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MQL_Parser::LT

    || _la == MQL_Parser::MINUS) {
      setState(201);
      insertPlainEdge();
      setState(202);
      insertPlainNode();
      setState(208);
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
    setState(209);
    match(MQL_Parser::L_PAR);
    setState(211);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & -1073741826) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 64)) & 1048829) != 0)) {
      setState(210);
      insertPlainNodeInside();
    }
    setState(216);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MQL_Parser::TYPE) {
      setState(213);
      match(MQL_Parser::TYPE);
      setState(218);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(220);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MQL_Parser::LCURLY_BRACKET) {
      setState(219);
      properties();
    }
    setState(222);
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
    setState(224);
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
    setState(244);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case MQL_Parser::LT: {
        enterOuterAlt(_localctx, 1);
        setState(226);
        match(MQL_Parser::LT);
        setState(227);
        match(MQL_Parser::MINUS);
        setState(228);
        match(MQL_Parser::LSQUARE_BRACKET);
        setState(229);
        match(MQL_Parser::TYPE);
        setState(231);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == MQL_Parser::LCURLY_BRACKET) {
          setState(230);
          properties();
        }
        setState(233);
        match(MQL_Parser::RSQUARE_BRACKET);
        setState(234);
        match(MQL_Parser::MINUS);
        break;
      }

      case MQL_Parser::MINUS: {
        enterOuterAlt(_localctx, 2);
        setState(235);
        match(MQL_Parser::MINUS);
        setState(236);
        match(MQL_Parser::LSQUARE_BRACKET);
        setState(237);
        match(MQL_Parser::TYPE);
        setState(239);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == MQL_Parser::LCURLY_BRACKET) {
          setState(238);
          properties();
        }
        setState(241);
        match(MQL_Parser::RSQUARE_BRACKET);
        setState(242);
        match(MQL_Parser::MINUS);
        setState(243);
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
    setState(246);
    match(MQL_Parser::K_CREATE);
    setState(247);
    identifier();
    setState(248);
    match(MQL_Parser::K_INDEX);
    setState(249);
    match(MQL_Parser::STRING);
    setState(250);
    match(MQL_Parser::K_WITH);
    setState(251);
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
    setState(253);
    match(MQL_Parser::LCURLY_BRACKET);
    setState(262);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MQL_Parser::STRING) {
      setState(254);
      createIndexOption();
      setState(259);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == MQL_Parser::COMMA) {
        setState(255);
        match(MQL_Parser::COMMA);
        setState(256);
        createIndexOption();
        setState(261);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
    }
    setState(264);
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
    setState(266);
    match(MQL_Parser::STRING);
    setState(270);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case MQL_Parser::SINGLE_EQ: {
        setState(267);
        match(MQL_Parser::SINGLE_EQ);
        setState(268);
        value();
        break;
      }

      case MQL_Parser::K_FALSE:
      case MQL_Parser::K_TRUE: {
        setState(269);
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
    setState(272);
    match(MQL_Parser::K_SHOW);
    setState(277);
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
      case MQL_Parser::K_EDIT_DISTANCE:
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
      case MQL_Parser::K_STRING:
      case MQL_Parser::K_STR:
      case MQL_Parser::K_TEXT_SEARCH:
      case MQL_Parser::K_TYPE:
      case MQL_Parser::K_REGEX:
      case MQL_Parser::K_RETURN:
      case MQL_Parser::K_SUM:
      case MQL_Parser::K_TRAILS:
      case MQL_Parser::K_VALUES:
      case MQL_Parser::K_WALKS:
      case MQL_Parser::K_WITH:
      case MQL_Parser::K_WHERE:
      case MQL_Parser::K_YIELD:
      case MQL_Parser::NAME: {
        setState(274);
        identifier();
        setState(275);
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
    setState(279);
    match(MQL_Parser::K_DESCRIBE);
    setState(283);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 18, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(280);
        describeFlag(); 
      }
      setState(285);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 18, _ctx);
    }
    setState(286);
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
    setState(288);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 2814784143622144) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
    setState(291);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 19, _ctx)) {
    case 1: {
      setState(289);
      match(MQL_Parser::K_LIMIT);
      setState(290);
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
    setState(293);
    match(MQL_Parser::K_MATCH);
    setState(294);
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
    setState(296);
    match(MQL_Parser::K_LET);
    setState(297);
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
    setState(299);
    letDefinition();
    setState(304);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MQL_Parser::COMMA) {
      setState(300);
      match(MQL_Parser::COMMA);
      setState(301);
      letDefinition();
      setState(306);
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
    setState(307);
    match(MQL_Parser::VARIABLE);
    setState(308);
    match(MQL_Parser::SINGLE_EQ);
    setState(309);
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
    setState(311);
    match(MQL_Parser::K_WHERE);
    setState(312);
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
    setState(314);
    match(MQL_Parser::K_GROUP);
    setState(315);
    match(MQL_Parser::K_BY);
    setState(316);
    groupByItem();
    setState(321);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MQL_Parser::COMMA) {
      setState(317);
      match(MQL_Parser::COMMA);
      setState(318);
      groupByItem();
      setState(323);
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
    setState(324);
    match(MQL_Parser::K_ORDER);
    setState(325);
    match(MQL_Parser::K_BY);
    setState(326);
    orderByItem();
    setState(331);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MQL_Parser::COMMA) {
      setState(327);
      match(MQL_Parser::COMMA);
      setState(328);
      orderByItem();
      setState(333);
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
    setState(357);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 28, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<MQL_Parser::ReturnListContext>(_localctx);
      enterOuterAlt(_localctx, 1);
      setState(334);
      match(MQL_Parser::K_RETURN);
      setState(336);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 23, _ctx)) {
      case 1: {
        setState(335);
        match(MQL_Parser::K_DISTINCT);
        break;
      }

      default:
        break;
      }
      setState(338);
      returnItem();
      setState(343);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == MQL_Parser::COMMA) {
        setState(339);
        match(MQL_Parser::COMMA);
        setState(340);
        returnItem();
        setState(345);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(347);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == MQL_Parser::K_LIMIT

      || _la == MQL_Parser::K_OFFSET) {
        setState(346);
        limitOffsetClauses();
      }
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<MQL_Parser::ReturnAllContext>(_localctx);
      enterOuterAlt(_localctx, 2);
      setState(349);
      match(MQL_Parser::K_RETURN);
      setState(351);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == MQL_Parser::K_DISTINCT) {
        setState(350);
        match(MQL_Parser::K_DISTINCT);
      }
      setState(353);
      match(MQL_Parser::STAR);
      setState(355);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == MQL_Parser::K_LIMIT

      || _la == MQL_Parser::K_OFFSET) {
        setState(354);
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
    setState(359);
    match(MQL_Parser::K_CALL);
    setState(360);
    identifier();
    setState(361);
    match(MQL_Parser::L_PAR);
    setState(363);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & -2) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 64)) & 120261151999) != 0)) {
      setState(362);
      callArguments();
    }
    setState(365);
    match(MQL_Parser::R_PAR);
    setState(367);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MQL_Parser::K_YIELD) {
      setState(366);
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
    setState(369);
    match(MQL_Parser::K_YIELD);
    setState(379);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case MQL_Parser::STAR: {
        setState(370);
        match(MQL_Parser::STAR);
        break;
      }

      case MQL_Parser::VARIABLE: {
        setState(371);
        yieldItem();
        setState(376);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == MQL_Parser::COMMA) {
          setState(372);
          match(MQL_Parser::COMMA);
          setState(373);
          yieldItem();
          setState(378);
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
    setState(381);
    match(MQL_Parser::VARIABLE);
    setState(383);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MQL_Parser::K_AS) {
      setState(382);
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
    setState(385);
    conditionalOrExpr();
    setState(390);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MQL_Parser::COMMA) {
      setState(386);
      match(MQL_Parser::COMMA);
      setState(387);
      conditionalOrExpr();
      setState(392);
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
    setState(401);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case MQL_Parser::K_LIMIT: {
        enterOuterAlt(_localctx, 1);
        setState(393);
        limitClause();
        setState(395);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == MQL_Parser::K_OFFSET) {
          setState(394);
          offsetClause();
        }
        break;
      }

      case MQL_Parser::K_OFFSET: {
        enterOuterAlt(_localctx, 2);
        setState(397);
        offsetClause();
        setState(399);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == MQL_Parser::K_LIMIT) {
          setState(398);
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
    setState(403);
    match(MQL_Parser::K_LIMIT);
    setState(404);
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
    setState(406);
    match(MQL_Parser::K_OFFSET);
    setState(407);
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
    setState(442);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 45, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<MQL_Parser::ReturnItemVarContext>(_localctx);
      enterOuterAlt(_localctx, 1);
      setState(409);
      match(MQL_Parser::VARIABLE);
      setState(411);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == MQL_Parser::KEY) {
        setState(410);
        match(MQL_Parser::KEY);
      }
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<MQL_Parser::ReturnItemAggContext>(_localctx);
      enterOuterAlt(_localctx, 2);
      setState(413);
      aggregateFunc();
      setState(414);
      match(MQL_Parser::L_PAR);
      setState(415);
      match(MQL_Parser::VARIABLE);
      setState(417);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == MQL_Parser::KEY) {
        setState(416);
        match(MQL_Parser::KEY);
      }
      setState(419);
      match(MQL_Parser::R_PAR);
      setState(421);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == MQL_Parser::K_AS) {
        setState(420);
        alias();
      }
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<MQL_Parser::ReturnItemCountContext>(_localctx);
      enterOuterAlt(_localctx, 3);
      setState(423);
      match(MQL_Parser::K_COUNT);
      setState(424);
      match(MQL_Parser::L_PAR);
      setState(426);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == MQL_Parser::K_DISTINCT) {
        setState(425);
        match(MQL_Parser::K_DISTINCT);
      }
      setState(433);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case MQL_Parser::VARIABLE: {
          setState(428);
          match(MQL_Parser::VARIABLE);
          setState(430);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == MQL_Parser::KEY) {
            setState(429);
            match(MQL_Parser::KEY);
          }
          break;
        }

        case MQL_Parser::STAR: {
          setState(432);
          match(MQL_Parser::STAR);
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      setState(435);
      match(MQL_Parser::R_PAR);
      setState(437);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == MQL_Parser::K_AS) {
        setState(436);
        alias();
      }
      break;
    }

    case 4: {
      _localctx = _tracker.createInstance<MQL_Parser::ReturnItemExprContext>(_localctx);
      enterOuterAlt(_localctx, 4);
      setState(439);
      conditionalOrExpr();
      setState(440);
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
    setState(444);
    match(MQL_Parser::K_AS);
    setState(445);
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
    setState(447);
    _la = _input->LA(1);
    if (!(((((_la - 5) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 5)) & 576460958461853697) != 0))) {
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
    setState(483);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 54, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<MQL_Parser::OrderByItemVarContext>(_localctx);
      enterOuterAlt(_localctx, 1);
      setState(449);
      match(MQL_Parser::VARIABLE);
      setState(451);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == MQL_Parser::KEY) {
        setState(450);
        match(MQL_Parser::KEY);
      }
      setState(454);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == MQL_Parser::K_ASC

      || _la == MQL_Parser::K_DESC) {
        setState(453);
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
      setState(456);
      aggregateFunc();
      setState(457);
      match(MQL_Parser::L_PAR);
      setState(458);
      match(MQL_Parser::VARIABLE);
      setState(460);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == MQL_Parser::KEY) {
        setState(459);
        match(MQL_Parser::KEY);
      }
      setState(462);
      match(MQL_Parser::R_PAR);
      setState(464);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == MQL_Parser::K_ASC

      || _la == MQL_Parser::K_DESC) {
        setState(463);
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
      setState(466);
      match(MQL_Parser::K_COUNT);
      setState(467);
      match(MQL_Parser::L_PAR);
      setState(469);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == MQL_Parser::K_DISTINCT) {
        setState(468);
        match(MQL_Parser::K_DISTINCT);
      }
      setState(471);
      match(MQL_Parser::VARIABLE);
      setState(473);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == MQL_Parser::KEY) {
        setState(472);
        match(MQL_Parser::KEY);
      }
      setState(475);
      match(MQL_Parser::R_PAR);
      setState(477);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == MQL_Parser::K_ASC

      || _la == MQL_Parser::K_DESC) {
        setState(476);
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
      setState(479);
      conditionalOrExpr();
      setState(481);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == MQL_Parser::K_ASC

      || _la == MQL_Parser::K_DESC) {
        setState(480);
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
    setState(485);
    match(MQL_Parser::VARIABLE);
    setState(487);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MQL_Parser::KEY) {
      setState(486);
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
    setState(489);
    basicPattern();
    setState(493);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MQL_Parser::K_OPTIONAL) {
      setState(490);
      optionalPattern();
      setState(495);
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
    setState(496);
    match(MQL_Parser::K_OPTIONAL);
    setState(497);
    match(MQL_Parser::LCURLY_BRACKET);
    setState(498);
    graphPattern();
    setState(499);
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
    setState(501);
    linearPattern();
    setState(506);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MQL_Parser::COMMA) {
      setState(502);
      match(MQL_Parser::COMMA);
      setState(503);
      linearPattern();
      setState(508);
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
    setState(509);
    node();
    setState(518);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (((((_la - 85) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 85)) & 16465) != 0)) {
      setState(512);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case MQL_Parser::LT:
        case MQL_Parser::MINUS: {
          setState(510);
          edge();
          break;
        }

        case MQL_Parser::LEQ:
        case MQL_Parser::SINGLE_EQ: {
          setState(511);
          path();
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      setState(514);
      node();
      setState(520);
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
    setState(546);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case MQL_Parser::LEQ: {
        enterOuterAlt(_localctx, 1);
        setState(521);
        match(MQL_Parser::LEQ);
        setState(522);
        match(MQL_Parser::LSQUARE_BRACKET);
        setState(524);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if ((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & 36028797018964040) != 0)) {
          setState(523);
          pathType();
        }
        setState(527);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == MQL_Parser::VARIABLE) {
          setState(526);
          match(MQL_Parser::VARIABLE);
        }
        setState(529);
        pathAlternatives();
        setState(530);
        match(MQL_Parser::RSQUARE_BRACKET);
        setState(531);
        match(MQL_Parser::SINGLE_EQ);
        break;
      }

      case MQL_Parser::SINGLE_EQ: {
        enterOuterAlt(_localctx, 2);
        setState(533);
        match(MQL_Parser::SINGLE_EQ);
        setState(534);
        match(MQL_Parser::LSQUARE_BRACKET);
        setState(536);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if ((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & 36028797018964040) != 0)) {
          setState(535);
          pathType();
        }
        setState(539);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == MQL_Parser::VARIABLE) {
          setState(538);
          match(MQL_Parser::VARIABLE);
        }
        setState(541);
        pathAlternatives();
        setState(542);
        match(MQL_Parser::RSQUARE_BRACKET);
        setState(543);
        match(MQL_Parser::SINGLE_EQ);
        setState(544);
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
    setState(548);
    pathSequence();
    setState(553);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MQL_Parser::PATH_ALTERNATIVE) {
      setState(549);
      match(MQL_Parser::PATH_ALTERNATIVE);
      setState(550);
      pathSequence();
      setState(555);
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
    setState(556);
    pathAtom();
    setState(561);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MQL_Parser::PATH_SEQUENCE) {
      setState(557);
      match(MQL_Parser::PATH_SEQUENCE);
      setState(558);
      pathAtom();
      setState(563);
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
    setState(580);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 71, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<MQL_Parser::PathAtomSimpleContext>(_localctx);
      enterOuterAlt(_localctx, 1);
      setState(565);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == MQL_Parser::PATH_NEGATION) {
        setState(564);
        match(MQL_Parser::PATH_NEGATION);
      }
      setState(567);
      match(MQL_Parser::TYPE);
      setState(569);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (((((_la - 95) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 95)) & 141) != 0)) {
        setState(568);
        pathSuffix();
      }
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<MQL_Parser::PathAtomAlternativesContext>(_localctx);
      enterOuterAlt(_localctx, 2);
      setState(572);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == MQL_Parser::PATH_NEGATION) {
        setState(571);
        match(MQL_Parser::PATH_NEGATION);
      }
      setState(574);
      match(MQL_Parser::L_PAR);
      setState(575);
      pathAlternatives();
      setState(576);
      match(MQL_Parser::R_PAR);
      setState(578);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (((((_la - 95) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 95)) & 141) != 0)) {
        setState(577);
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
    setState(590);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case MQL_Parser::STAR: {
        enterOuterAlt(_localctx, 1);
        setState(582);
        antlrcpp::downCast<PathSuffixContext *>(_localctx)->op = match(MQL_Parser::STAR);
        break;
      }

      case MQL_Parser::PLUS: {
        enterOuterAlt(_localctx, 2);
        setState(583);
        antlrcpp::downCast<PathSuffixContext *>(_localctx)->op = match(MQL_Parser::PLUS);
        break;
      }

      case MQL_Parser::QUESTION_MARK: {
        enterOuterAlt(_localctx, 3);
        setState(584);
        antlrcpp::downCast<PathSuffixContext *>(_localctx)->op = match(MQL_Parser::QUESTION_MARK);
        break;
      }

      case MQL_Parser::LCURLY_BRACKET: {
        enterOuterAlt(_localctx, 4);
        setState(585);
        match(MQL_Parser::LCURLY_BRACKET);
        setState(586);
        antlrcpp::downCast<PathSuffixContext *>(_localctx)->min = match(MQL_Parser::UNSIGNED_INTEGER);
        setState(587);
        match(MQL_Parser::COMMA);
        setState(588);
        antlrcpp::downCast<PathSuffixContext *>(_localctx)->max = match(MQL_Parser::UNSIGNED_INTEGER);
        setState(589);
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
    setState(607);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case MQL_Parser::K_ANY:
      case MQL_Parser::K_ALL: {
        enterOuterAlt(_localctx, 1);
        setState(592);
        _la = _input->LA(1);
        if (!(_la == MQL_Parser::K_ANY

        || _la == MQL_Parser::K_ALL)) {
        _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        setState(594);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == MQL_Parser::K_SHORTEST) {
          setState(593);
          match(MQL_Parser::K_SHORTEST);
        }
        setState(597);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == MQL_Parser::K_ACYCLIC

        || _la == MQL_Parser::K_SIMPLE || _la == MQL_Parser::K_TRAILS

        || _la == MQL_Parser::K_WALKS) {
          setState(596);
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
        setState(599);
        match(MQL_Parser::K_SHORTEST);
        setState(600);
        match(MQL_Parser::UNSIGNED_INTEGER);
        setState(602);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == MQL_Parser::K_GROUPS) {
          setState(601);
          match(MQL_Parser::K_GROUPS);
        }
        setState(605);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == MQL_Parser::K_ACYCLIC

        || _la == MQL_Parser::K_SIMPLE || _la == MQL_Parser::K_TRAILS

        || _la == MQL_Parser::K_WALKS) {
          setState(604);
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
    setState(611);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 78, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(609);
      fixedNode();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(610);
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
    setState(613);
    match(MQL_Parser::L_PAR);
    setState(614);
    fixedNodeInside();
    setState(615);
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
    setState(621);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 79, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(617);
      identifier();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(618);
      match(MQL_Parser::ANON_ID);
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(619);
      match(MQL_Parser::EDGE_ID);
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(620);
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
    setState(636);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case MQL_Parser::LT: {
        enterOuterAlt(_localctx, 1);
        setState(623);
        match(MQL_Parser::LT);
        setState(626);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 80, _ctx)) {
        case 1: {
          setState(624);
          match(MQL_Parser::MINUS);
          setState(625);
          edgeInside();
          break;
        }

        default:
          break;
        }
        setState(628);
        match(MQL_Parser::MINUS);
        break;
      }

      case MQL_Parser::MINUS: {
        enterOuterAlt(_localctx, 2);
        setState(629);
        match(MQL_Parser::MINUS);
        setState(633);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == MQL_Parser::LSQUARE_BRACKET) {
          setState(630);
          edgeInside();
          setState(631);
          match(MQL_Parser::MINUS);
        }
        setState(635);
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
    setState(638);
    match(MQL_Parser::LSQUARE_BRACKET);
    setState(640);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MQL_Parser::EDGE_ID

    || _la == MQL_Parser::VARIABLE) {
      setState(639);
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
    setState(643);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MQL_Parser::TYPE

    || _la == MQL_Parser::TYPE_VAR) {
      setState(642);
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
    setState(646);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MQL_Parser::LCURLY_BRACKET) {
      setState(645);
      properties();
    }
    setState(648);
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
    setState(650);
    match(MQL_Parser::L_PAR);
    setState(652);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MQL_Parser::VARIABLE) {
      setState(651);
      match(MQL_Parser::VARIABLE);
    }
    setState(657);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MQL_Parser::TYPE) {
      setState(654);
      match(MQL_Parser::TYPE);
      setState(659);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(661);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MQL_Parser::LCURLY_BRACKET) {
      setState(660);
      properties();
    }
    setState(663);
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
    setState(665);
    match(MQL_Parser::LCURLY_BRACKET);
    setState(666);
    property();
    setState(671);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MQL_Parser::COMMA) {
      setState(667);
      match(MQL_Parser::COMMA);
      setState(668);
      property();
      setState(673);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(674);
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
    setState(705);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 93, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<MQL_Parser::Property1Context>(_localctx);
      enterOuterAlt(_localctx, 1);
      setState(676);
      identifier();
      setState(681);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case MQL_Parser::COLON: {
          setState(677);
          match(MQL_Parser::COLON);
          setState(678);
          value();
          break;
        }

        case MQL_Parser::TRUE_PROP: {
          setState(679);
          match(MQL_Parser::TRUE_PROP);
          break;
        }

        case MQL_Parser::FALSE_PROP: {
          setState(680);
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
      setState(683);
      identifier();
      setState(684);
      match(MQL_Parser::TYPE);
      setState(685);
      match(MQL_Parser::L_PAR);
      setState(686);
      match(MQL_Parser::STRING);
      setState(687);
      match(MQL_Parser::R_PAR);
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<MQL_Parser::Property3Context>(_localctx);
      enterOuterAlt(_localctx, 3);
      setState(689);
      identifier();
      setState(690);
      match(MQL_Parser::K_IS);
      setState(692);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == MQL_Parser::K_NOT) {
        setState(691);
        match(MQL_Parser::K_NOT);
      }
      setState(694);
      exprTypename();
      setState(698);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == MQL_Parser::K_OR) {
        setState(695);
        conditionalOrType();
        setState(700);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      break;
    }

    case 4: {
      _localctx = _tracker.createInstance<MQL_Parser::Property4Context>(_localctx);
      enterOuterAlt(_localctx, 4);
      setState(701);
      identifier();

      setState(702);
      antlrcpp::downCast<Property4Context *>(_localctx)->op = _input->LT(1);
      _la = _input->LA(1);
      if (!(((((_la - 85) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 85)) & 63) != 0))) {
        antlrcpp::downCast<Property4Context *>(_localctx)->op = _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(703);
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
    setState(707);
    match(MQL_Parser::K_OR);
    setState(708);
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
    setState(712);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case MQL_Parser::NAME: {
        enterOuterAlt(_localctx, 1);
        setState(710);
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
      case MQL_Parser::K_EDIT_DISTANCE:
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
      case MQL_Parser::K_STRING:
      case MQL_Parser::K_STR:
      case MQL_Parser::K_TEXT_SEARCH:
      case MQL_Parser::K_TYPE:
      case MQL_Parser::K_REGEX:
      case MQL_Parser::K_RETURN:
      case MQL_Parser::K_SUM:
      case MQL_Parser::K_TRAILS:
      case MQL_Parser::K_VALUES:
      case MQL_Parser::K_WALKS:
      case MQL_Parser::K_WITH:
      case MQL_Parser::K_WHERE:
      case MQL_Parser::K_YIELD: {
        enterOuterAlt(_localctx, 2);
        setState(711);
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
    setState(714);
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
    setState(717);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MQL_Parser::PLUS

    || _la == MQL_Parser::MINUS) {
      setState(716);
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
    setState(719);
    _la = _input->LA(1);
    if (!(((((_la - 81) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 81)) & 7) != 0))) {
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
    setState(721);
    identifier();
    setState(722);
    match(MQL_Parser::L_PAR);
    setState(723);
    match(MQL_Parser::STRING);
    setState(724);
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
    setState(730);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case MQL_Parser::UNSIGNED_INTEGER:
      case MQL_Parser::UNSIGNED_FLOAT:
      case MQL_Parser::UNSIGNED_SCIENTIFIC_NOTATION:
      case MQL_Parser::PLUS:
      case MQL_Parser::MINUS: {
        enterOuterAlt(_localctx, 1);
        setState(726);
        numericValue();
        break;
      }

      case MQL_Parser::STRING: {
        enterOuterAlt(_localctx, 2);
        setState(727);
        match(MQL_Parser::STRING);
        break;
      }

      case MQL_Parser::K_FALSE:
      case MQL_Parser::K_TRUE: {
        enterOuterAlt(_localctx, 3);
        setState(728);
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
      case MQL_Parser::K_EDIT_DISTANCE:
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
      case MQL_Parser::K_STRING:
      case MQL_Parser::K_STR:
      case MQL_Parser::K_TEXT_SEARCH:
      case MQL_Parser::K_TYPE:
      case MQL_Parser::K_REGEX:
      case MQL_Parser::K_RETURN:
      case MQL_Parser::K_SUM:
      case MQL_Parser::K_TRAILS:
      case MQL_Parser::K_VALUES:
      case MQL_Parser::K_WALKS:
      case MQL_Parser::K_WITH:
      case MQL_Parser::K_WHERE:
      case MQL_Parser::K_YIELD:
      case MQL_Parser::NAME: {
        enterOuterAlt(_localctx, 4);
        setState(729);
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
    setState(732);
    conditionalAndExpr();
    setState(737);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MQL_Parser::K_OR) {
      setState(733);
      match(MQL_Parser::K_OR);
      setState(734);
      conditionalAndExpr();
      setState(739);
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
    setState(740);
    comparisonExpr();
    setState(745);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MQL_Parser::K_AND) {
      setState(741);
      match(MQL_Parser::K_AND);
      setState(742);
      comparisonExpr();
      setState(747);
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
    setState(760);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 101, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<MQL_Parser::ComparisonExprOpContext>(_localctx);
      enterOuterAlt(_localctx, 1);
      setState(748);
      additiveExpr();
      setState(751);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (((((_la - 85) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 85)) & 63) != 0)) {
        setState(749);
        antlrcpp::downCast<ComparisonExprOpContext *>(_localctx)->op = _input->LT(1);
        _la = _input->LA(1);
        if (!(((((_la - 85) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 85)) & 63) != 0))) {
          antlrcpp::downCast<ComparisonExprOpContext *>(_localctx)->op = _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        setState(750);
        additiveExpr();
      }
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<MQL_Parser::ComparisonExprIsContext>(_localctx);
      enterOuterAlt(_localctx, 2);
      setState(753);
      additiveExpr();
      setState(754);
      match(MQL_Parser::K_IS);
      setState(756);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == MQL_Parser::K_NOT) {
        setState(755);
        match(MQL_Parser::K_NOT);
      }
      setState(758);
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
    setState(762);
    multiplicativeExpr();
    setState(767);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MQL_Parser::PLUS

    || _la == MQL_Parser::MINUS) {
      setState(763);
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
      setState(764);
      multiplicativeExpr();
      setState(769);
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
    setState(770);
    unaryExpr();
    setState(775);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (((((_la - 92) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 92)) & 25) != 0)) {
      setState(771);
      antlrcpp::downCast<MultiplicativeExprContext *>(_localctx)->_tset1440 = _input->LT(1);
      _la = _input->LA(1);
      if (!(((((_la - 92) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 92)) & 25) != 0))) {
        antlrcpp::downCast<MultiplicativeExprContext *>(_localctx)->_tset1440 = _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      antlrcpp::downCast<MultiplicativeExprContext *>(_localctx)->op.push_back(antlrcpp::downCast<MultiplicativeExprContext *>(_localctx)->_tset1440);
      setState(772);
      unaryExpr();
      setState(777);
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
    setState(785);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 104, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(778);
      match(MQL_Parser::K_NOT);
      setState(779);
      unaryExpr();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(780);
      atomicExpr();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(781);
      match(MQL_Parser::PLUS);
      setState(782);
      unaryExpr();
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(783);
      match(MQL_Parser::MINUS);
      setState(784);
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
    setState(798);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 106, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<MQL_Parser::ExprVarContext>(_localctx);
      enterOuterAlt(_localctx, 1);
      setState(787);
      match(MQL_Parser::VARIABLE);
      setState(789);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == MQL_Parser::KEY) {
        setState(788);
        match(MQL_Parser::KEY);
      }
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<MQL_Parser::ExprFunctionContext>(_localctx);
      enterOuterAlt(_localctx, 2);
      setState(791);
      function();
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<MQL_Parser::ExprValueContext>(_localctx);
      enterOuterAlt(_localctx, 3);
      setState(792);
      value();
      break;
    }

    case 4: {
      _localctx = _tracker.createInstance<MQL_Parser::ExprFixedNodeInsideContext>(_localctx);
      enterOuterAlt(_localctx, 4);
      setState(793);
      fixedNodeInside();
      break;
    }

    case 5: {
      _localctx = _tracker.createInstance<MQL_Parser::ExprParenthesisContext>(_localctx);
      enterOuterAlt(_localctx, 5);
      setState(794);
      match(MQL_Parser::L_PAR);
      setState(795);
      conditionalOrExpr();
      setState(796);
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

MQL_Parser::StrContext* MQL_Parser::FunctionContext::str() {
  return getRuleContext<MQL_Parser::StrContext>(0);
}

MQL_Parser::LabelsContext* MQL_Parser::FunctionContext::labels() {
  return getRuleContext<MQL_Parser::LabelsContext>(0);
}

MQL_Parser::TypeContext* MQL_Parser::FunctionContext::type() {
  return getRuleContext<MQL_Parser::TypeContext>(0);
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
    setState(810);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case MQL_Parser::K_REGEX: {
        enterOuterAlt(_localctx, 1);
        setState(800);
        regex();
        break;
      }

      case MQL_Parser::K_COSINE_SIMILARITY: {
        enterOuterAlt(_localctx, 2);
        setState(801);
        cosineSimilarity();
        break;
      }

      case MQL_Parser::K_COSINE_DISTANCE: {
        enterOuterAlt(_localctx, 3);
        setState(802);
        cosineDistance();
        break;
      }

      case MQL_Parser::K_MANHATTAN_DISTANCE: {
        enterOuterAlt(_localctx, 4);
        setState(803);
        manhattanDistance();
        break;
      }

      case MQL_Parser::K_EUCLIDEAN_DISTANCE: {
        enterOuterAlt(_localctx, 5);
        setState(804);
        euclideanDistance();
        break;
      }

      case MQL_Parser::K_EDIT_DISTANCE: {
        enterOuterAlt(_localctx, 6);
        setState(805);
        editDistance();
        break;
      }

      case MQL_Parser::K_NORMALIZE: {
        enterOuterAlt(_localctx, 7);
        setState(806);
        normalize();
        break;
      }

      case MQL_Parser::K_STR: {
        enterOuterAlt(_localctx, 8);
        setState(807);
        str();
        break;
      }

      case MQL_Parser::K_LABELS: {
        enterOuterAlt(_localctx, 9);
        setState(808);
        labels();
        break;
      }

      case MQL_Parser::K_TYPE: {
        enterOuterAlt(_localctx, 10);
        setState(809);
        type();
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
    setState(812);
    match(MQL_Parser::K_REGEX);
    setState(813);
    match(MQL_Parser::L_PAR);
    setState(814);
    conditionalOrExpr();
    setState(815);
    match(MQL_Parser::COMMA);
    setState(816);
    conditionalOrExpr();
    setState(819);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MQL_Parser::COMMA) {
      setState(817);
      match(MQL_Parser::COMMA);
      setState(818);
      conditionalOrExpr();
    }
    setState(821);
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
    setState(823);
    match(MQL_Parser::K_COSINE_SIMILARITY);
    setState(824);
    match(MQL_Parser::L_PAR);
    setState(825);
    conditionalOrExpr();
    setState(826);
    match(MQL_Parser::COMMA);
    setState(827);
    conditionalOrExpr();
    setState(828);
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
    setState(830);
    match(MQL_Parser::K_COSINE_DISTANCE);
    setState(831);
    match(MQL_Parser::L_PAR);
    setState(832);
    conditionalOrExpr();
    setState(833);
    match(MQL_Parser::COMMA);
    setState(834);
    conditionalOrExpr();
    setState(835);
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
    setState(837);
    match(MQL_Parser::K_MANHATTAN_DISTANCE);
    setState(838);
    match(MQL_Parser::L_PAR);
    setState(839);
    conditionalOrExpr();
    setState(840);
    match(MQL_Parser::COMMA);
    setState(841);
    conditionalOrExpr();
    setState(842);
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
    setState(844);
    match(MQL_Parser::K_EUCLIDEAN_DISTANCE);
    setState(845);
    match(MQL_Parser::L_PAR);
    setState(846);
    conditionalOrExpr();
    setState(847);
    match(MQL_Parser::COMMA);
    setState(848);
    conditionalOrExpr();
    setState(849);
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
    setState(851);
    match(MQL_Parser::K_EDIT_DISTANCE);
    setState(852);
    match(MQL_Parser::L_PAR);
    setState(853);
    conditionalOrExpr();
    setState(854);
    match(MQL_Parser::COMMA);
    setState(855);
    conditionalOrExpr();
    setState(856);
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
    setState(858);
    match(MQL_Parser::K_NORMALIZE);
    setState(859);
    match(MQL_Parser::L_PAR);
    setState(860);
    conditionalOrExpr();
    setState(861);
    match(MQL_Parser::R_PAR);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StrContext ------------------------------------------------------------------

MQL_Parser::StrContext::StrContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::StrContext::K_STR() {
  return getToken(MQL_Parser::K_STR, 0);
}

tree::TerminalNode* MQL_Parser::StrContext::L_PAR() {
  return getToken(MQL_Parser::L_PAR, 0);
}

MQL_Parser::ConditionalOrExprContext* MQL_Parser::StrContext::conditionalOrExpr() {
  return getRuleContext<MQL_Parser::ConditionalOrExprContext>(0);
}

tree::TerminalNode* MQL_Parser::StrContext::R_PAR() {
  return getToken(MQL_Parser::R_PAR, 0);
}


size_t MQL_Parser::StrContext::getRuleIndex() const {
  return MQL_Parser::RuleStr;
}


std::any MQL_Parser::StrContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitStr(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::StrContext* MQL_Parser::str() {
  StrContext *_localctx = _tracker.createInstance<StrContext>(_ctx, getState());
  enterRule(_localctx, 148, MQL_Parser::RuleStr);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(863);
    match(MQL_Parser::K_STR);
    setState(864);
    match(MQL_Parser::L_PAR);
    setState(865);
    conditionalOrExpr();
    setState(866);
    match(MQL_Parser::R_PAR);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LabelsContext ------------------------------------------------------------------

MQL_Parser::LabelsContext::LabelsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::LabelsContext::K_LABELS() {
  return getToken(MQL_Parser::K_LABELS, 0);
}

tree::TerminalNode* MQL_Parser::LabelsContext::L_PAR() {
  return getToken(MQL_Parser::L_PAR, 0);
}

tree::TerminalNode* MQL_Parser::LabelsContext::VARIABLE() {
  return getToken(MQL_Parser::VARIABLE, 0);
}

tree::TerminalNode* MQL_Parser::LabelsContext::R_PAR() {
  return getToken(MQL_Parser::R_PAR, 0);
}


size_t MQL_Parser::LabelsContext::getRuleIndex() const {
  return MQL_Parser::RuleLabels;
}


std::any MQL_Parser::LabelsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitLabels(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::LabelsContext* MQL_Parser::labels() {
  LabelsContext *_localctx = _tracker.createInstance<LabelsContext>(_ctx, getState());
  enterRule(_localctx, 150, MQL_Parser::RuleLabels);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(868);
    match(MQL_Parser::K_LABELS);
    setState(869);
    match(MQL_Parser::L_PAR);
    setState(870);
    match(MQL_Parser::VARIABLE);
    setState(871);
    match(MQL_Parser::R_PAR);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TypeContext ------------------------------------------------------------------

MQL_Parser::TypeContext::TypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::TypeContext::K_TYPE() {
  return getToken(MQL_Parser::K_TYPE, 0);
}

tree::TerminalNode* MQL_Parser::TypeContext::L_PAR() {
  return getToken(MQL_Parser::L_PAR, 0);
}

tree::TerminalNode* MQL_Parser::TypeContext::VARIABLE() {
  return getToken(MQL_Parser::VARIABLE, 0);
}

tree::TerminalNode* MQL_Parser::TypeContext::R_PAR() {
  return getToken(MQL_Parser::R_PAR, 0);
}


size_t MQL_Parser::TypeContext::getRuleIndex() const {
  return MQL_Parser::RuleType;
}


std::any MQL_Parser::TypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitType(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::TypeContext* MQL_Parser::type() {
  TypeContext *_localctx = _tracker.createInstance<TypeContext>(_ctx, getState());
  enterRule(_localctx, 152, MQL_Parser::RuleType);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(873);
    match(MQL_Parser::K_TYPE);
    setState(874);
    match(MQL_Parser::L_PAR);
    setState(875);
    match(MQL_Parser::VARIABLE);
    setState(876);
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
  enterRule(_localctx, 154, MQL_Parser::RuleTextSearchIndexMode);
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
    setState(878);
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
  enterRule(_localctx, 156, MQL_Parser::RuleExprTypename);
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
    setState(880);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 306244776942895616) != 0))) {
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

tree::TerminalNode* MQL_Parser::KeywordContext::K_EDIT_DISTANCE() {
  return getToken(MQL_Parser::K_EDIT_DISTANCE, 0);
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

tree::TerminalNode* MQL_Parser::KeywordContext::K_STRING() {
  return getToken(MQL_Parser::K_STRING, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_STR() {
  return getToken(MQL_Parser::K_STR, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_TEXT_SEARCH() {
  return getToken(MQL_Parser::K_TEXT_SEARCH, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_TYPE() {
  return getToken(MQL_Parser::K_TYPE, 0);
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
  enterRule(_localctx, 158, MQL_Parser::RuleKeyword);
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
    setState(882);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & -1073741826) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 64)) & 253) != 0))) {
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

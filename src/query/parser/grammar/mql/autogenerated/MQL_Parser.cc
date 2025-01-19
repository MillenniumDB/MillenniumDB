
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
      "root", "matchQuery", "insertPatterns", "insertLinearPattern", "insertPlainNode", 
      "insertPlainNodeInside", "insertPlainEdge", "tensorStoreQuery", "createTensorStore", 
      "insertTensors", "insertTensorsTuple", "deleteTensors", "tensor", 
      "tensorDistanceReference", "metricType", "textIndexQuery", "createTextIndex", 
      "normalizeTextIndex", "normalizeType", "tokenizeTextIndex", "tokenizeType", 
      "showQuery", "describeQuery", "describeFlag", "setStatement", "matchStatement", 
      "whereStatement", "groupByStatement", "orderByStatement", "returnStatement", 
      "limitOffsetClauses", "limitClause", "offsetClause", "setItem", "returnItem", 
      "alias", "aggregateFunc", "orderByItem", "groupByItem", "graphPattern", 
      "optionalPattern", "basicPattern", "linearPattern", "path", "pathAlternatives", 
      "pathSequence", "pathAtom", "pathSuffix", "pathType", "node", "fixedNode", 
      "fixedNodeInside", "edge", "edgeInside", "varNode", "properties", 
      "property", "conditionalOrType", "identifier", "boolValue", "numericValue", 
      "datatypeValue", "value", "conditionalOrExpr", "conditionalAndExpr", 
      "comparisonExpr", "additiveExpr", "multiplicativeExpr", "unaryExpr", 
      "atomicExpr", "function", "regex", "tensorDistance", "textSearch", 
      "textSearchIndexMode", "exprTypename", "keyword"
    },
    std::vector<std::string>{
      "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
      "", "", "", "", "", "", "", "", "", "", "'false'", "", "", "", "", 
      "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
      "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "'true'", 
      "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
      "", "", "", "", "", "'<='", "'>='", "'=='", "'!='", "'<'", "'>'", 
      "'='", "'/'", "'|'", "'^'", "'*'", "'%'", "'\\u003F'", "'+'", "'-'", 
      "'('", "')'", "'{'", "'}'", "'['", "']'", "','", "':'"
    },
    std::vector<std::string>{
      "", "K_ACYCLIC", "K_AND", "K_ANGULAR", "K_ANY", "K_AS", "K_AVG", "K_ALL", 
      "K_ASC", "K_BY", "K_BOOL", "K_COUNT", "K_CREATE", "K_DELETE", "K_DESCRIBE", 
      "K_DESC", "K_DIMENSIONS", "K_DISTINCT", "K_EDGE", "K_EUCLIDEAN", "K_FROM", 
      "K_INCOMING", "K_INDEX", "K_INSERT", "K_INTEGER", "K_INTO", "K_IS", 
      "K_FALSE", "K_FLOAT", "K_GROUP", "K_IDENTITY", "K_LABELS", "K_LABEL", 
      "K_LIMIT", "K_MANHATTAN", "K_MATCH", "K_MAX", "K_MIN", "K_NFKD_CASEFOLD", 
      "K_NORMALIZE", "K_OBJECTS", "K_OFFSET", "K_ON", "K_OPTIONAL", "K_ORDER", 
      "K_OR", "K_OUTGOING", "K_PREFIX", "K_PROPERTIES", "K_PROPERTY", "K_NOT", 
      "K_NULL", "K_SHORTEST", "K_SHOW", "K_SIMPLE", "K_TENSOR_DISTANCE", 
      "K_TEXT_SEARCH", "K_REGEX", "K_RETURN", "K_SET", "K_SUM", "K_STRING", 
      "K_STORE", "K_TENSOR", "K_TEXT", "K_TRUE", "K_TOKENIZE", "K_TRAILS", 
      "K_VALUES", "K_WALKS", "K_WITH", "K_WHERE", "K_WS_KEEP_PUNCT", "K_WS_RM_PUNCT", 
      "K_WS_SPLIT_PUNCT", "TRUE_PROP", "FALSE_PROP", "ANON_ID", "EDGE_ID", 
      "KEY", "TYPE", "TYPE_VAR", "VARIABLE", "STRING", "UNSIGNED_INTEGER", 
      "UNSIGNED_FLOAT", "UNSIGNED_SCIENTIFIC_NOTATION", "NAME", "LEQ", "GEQ", 
      "EQ", "NEQ", "LT", "GT", "SINGLE_EQ", "PATH_SEQUENCE", "PATH_ALTERNATIVE", 
      "PATH_NEGATION", "STAR", "PERCENT", "QUESTION_MARK", "PLUS", "MINUS", 
      "L_PAR", "R_PAR", "LCURLY_BRACKET", "RCURLY_BRACKET", "LSQUARE_BRACKET", 
      "RSQUARE_BRACKET", "COMMA", "COLON", "WHITE_SPACE", "SINGLE_LINE_COMMENT", 
      "UNRECOGNIZED"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,1,113,864,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,6,2,
  	7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,2,14,7,
  	14,2,15,7,15,2,16,7,16,2,17,7,17,2,18,7,18,2,19,7,19,2,20,7,20,2,21,7,
  	21,2,22,7,22,2,23,7,23,2,24,7,24,2,25,7,25,2,26,7,26,2,27,7,27,2,28,7,
  	28,2,29,7,29,2,30,7,30,2,31,7,31,2,32,7,32,2,33,7,33,2,34,7,34,2,35,7,
  	35,2,36,7,36,2,37,7,37,2,38,7,38,2,39,7,39,2,40,7,40,2,41,7,41,2,42,7,
  	42,2,43,7,43,2,44,7,44,2,45,7,45,2,46,7,46,2,47,7,47,2,48,7,48,2,49,7,
  	49,2,50,7,50,2,51,7,51,2,52,7,52,2,53,7,53,2,54,7,54,2,55,7,55,2,56,7,
  	56,2,57,7,57,2,58,7,58,2,59,7,59,2,60,7,60,2,61,7,61,2,62,7,62,2,63,7,
  	63,2,64,7,64,2,65,7,65,2,66,7,66,2,67,7,67,2,68,7,68,2,69,7,69,2,70,7,
  	70,2,71,7,71,2,72,7,72,2,73,7,73,2,74,7,74,2,75,7,75,2,76,7,76,1,0,3,
  	0,156,8,0,1,0,1,0,3,0,160,8,0,1,0,1,0,1,0,1,0,3,0,166,8,0,1,0,1,0,1,1,
  	1,1,3,1,172,8,1,1,1,3,1,175,8,1,1,1,3,1,178,8,1,1,1,1,1,1,2,1,2,1,2,1,
  	2,5,2,186,8,2,10,2,12,2,189,9,2,1,3,1,3,1,3,1,3,5,3,195,8,3,10,3,12,3,
  	198,9,3,1,4,1,4,3,4,202,8,4,1,4,5,4,205,8,4,10,4,12,4,208,9,4,1,4,3,4,
  	211,8,4,1,4,1,4,1,5,1,5,1,6,1,6,1,6,1,6,1,6,3,6,222,8,6,1,6,1,6,1,6,1,
  	6,1,6,1,6,3,6,230,8,6,1,6,1,6,1,6,3,6,235,8,6,1,7,1,7,1,7,3,7,240,8,7,
  	1,8,1,8,1,8,1,8,1,8,1,8,1,8,1,9,1,9,1,9,1,9,1,9,1,9,1,9,1,9,1,9,5,9,258,
  	8,9,10,9,12,9,261,9,9,1,10,1,10,1,10,1,10,1,10,1,10,1,11,1,11,1,11,1,
  	11,1,11,1,11,1,11,1,11,1,11,1,11,5,11,279,8,11,10,11,12,11,282,9,11,1,
  	11,1,11,1,12,1,12,1,12,1,12,5,12,290,8,12,10,12,12,12,293,9,12,1,12,1,
  	12,1,13,1,13,3,13,299,8,13,1,14,1,14,1,15,1,15,1,16,1,16,1,16,1,16,1,
  	16,1,16,1,16,1,16,1,16,1,16,1,16,1,16,1,16,1,16,1,16,3,16,320,8,16,3,
  	16,322,8,16,1,17,1,17,1,17,1,18,1,18,1,19,1,19,1,19,1,20,1,20,1,21,1,
  	21,1,21,1,21,1,21,3,21,339,8,21,1,22,1,22,5,22,343,8,22,10,22,12,22,346,
  	9,22,1,22,1,22,1,23,1,23,1,23,3,23,353,8,23,1,24,1,24,1,24,1,24,5,24,
  	359,8,24,10,24,12,24,362,9,24,1,25,1,25,1,25,1,26,1,26,1,26,1,27,1,27,
  	1,27,1,27,1,27,5,27,375,8,27,10,27,12,27,378,9,27,1,28,1,28,1,28,1,28,
  	1,28,5,28,385,8,28,10,28,12,28,388,9,28,1,29,1,29,3,29,392,8,29,1,29,
  	1,29,1,29,5,29,397,8,29,10,29,12,29,400,9,29,1,29,3,29,403,8,29,1,29,
  	1,29,3,29,407,8,29,1,29,1,29,3,29,411,8,29,3,29,413,8,29,1,30,1,30,3,
  	30,417,8,30,1,30,1,30,3,30,421,8,30,3,30,423,8,30,1,31,1,31,1,31,1,32,
  	1,32,1,32,1,33,1,33,1,33,1,33,1,34,1,34,3,34,437,8,34,1,34,1,34,1,34,
  	1,34,3,34,443,8,34,1,34,1,34,3,34,447,8,34,1,34,1,34,1,34,3,34,452,8,
  	34,1,34,1,34,3,34,456,8,34,1,34,3,34,459,8,34,1,34,1,34,3,34,463,8,34,
  	1,34,1,34,1,34,3,34,468,8,34,1,35,1,35,1,35,1,36,1,36,1,37,1,37,3,37,
  	477,8,37,1,37,3,37,480,8,37,1,37,1,37,1,37,1,37,3,37,486,8,37,1,37,1,
  	37,3,37,490,8,37,1,37,1,37,1,37,3,37,495,8,37,1,37,1,37,3,37,499,8,37,
  	1,37,1,37,3,37,503,8,37,1,37,1,37,3,37,507,8,37,3,37,509,8,37,1,38,1,
  	38,3,38,513,8,38,1,39,1,39,5,39,517,8,39,10,39,12,39,520,9,39,1,40,1,
  	40,1,40,1,40,1,40,1,41,1,41,1,41,5,41,530,8,41,10,41,12,41,533,9,41,1,
  	42,1,42,1,42,3,42,538,8,42,1,42,1,42,5,42,542,8,42,10,42,12,42,545,9,
  	42,1,43,1,43,1,43,3,43,550,8,43,1,43,3,43,553,8,43,1,43,1,43,1,43,1,43,
  	1,43,1,43,1,43,3,43,562,8,43,1,43,3,43,565,8,43,1,43,1,43,1,43,1,43,1,
  	43,3,43,572,8,43,1,44,1,44,1,44,5,44,577,8,44,10,44,12,44,580,9,44,1,
  	45,1,45,1,45,5,45,585,8,45,10,45,12,45,588,9,45,1,46,3,46,591,8,46,1,
  	46,1,46,3,46,595,8,46,1,46,3,46,598,8,46,1,46,1,46,1,46,1,46,3,46,604,
  	8,46,3,46,606,8,46,1,47,1,47,1,47,1,47,1,47,1,47,1,47,1,47,3,47,616,8,
  	47,1,48,1,48,3,48,620,8,48,1,48,3,48,623,8,48,1,49,1,49,3,49,627,8,49,
  	1,50,1,50,1,50,1,50,1,51,1,51,1,51,1,51,3,51,637,8,51,1,52,1,52,1,52,
  	3,52,642,8,52,1,52,1,52,1,52,1,52,1,52,3,52,649,8,52,1,52,3,52,652,8,
  	52,1,53,1,53,3,53,656,8,53,1,53,3,53,659,8,53,1,53,3,53,662,8,53,1,53,
  	1,53,1,54,1,54,3,54,668,8,54,1,54,5,54,671,8,54,10,54,12,54,674,9,54,
  	1,54,3,54,677,8,54,1,54,1,54,1,55,1,55,1,55,1,55,5,55,685,8,55,10,55,
  	12,55,688,9,55,1,55,1,55,1,56,1,56,1,56,1,56,1,56,3,56,697,8,56,1,56,
  	1,56,1,56,1,56,1,56,1,56,1,56,1,56,1,56,3,56,708,8,56,1,56,1,56,5,56,
  	712,8,56,10,56,12,56,715,9,56,1,56,1,56,1,56,1,56,3,56,721,8,56,1,57,
  	1,57,1,57,1,58,1,58,3,58,728,8,58,1,59,1,59,1,60,3,60,733,8,60,1,60,1,
  	60,1,61,1,61,1,61,1,61,1,61,1,62,1,62,1,62,1,62,3,62,746,8,62,1,63,1,
  	63,1,63,5,63,751,8,63,10,63,12,63,754,9,63,1,64,1,64,1,64,5,64,759,8,
  	64,10,64,12,64,762,9,64,1,65,1,65,1,65,3,65,767,8,65,1,65,1,65,1,65,3,
  	65,772,8,65,1,65,1,65,3,65,776,8,65,1,66,1,66,1,66,5,66,781,8,66,10,66,
  	12,66,784,9,66,1,67,1,67,1,67,5,67,789,8,67,10,67,12,67,792,9,67,1,68,
  	1,68,1,68,1,68,1,68,1,68,1,68,3,68,801,8,68,1,69,1,69,3,69,805,8,69,1,
  	69,1,69,1,69,1,69,1,69,1,69,1,69,3,69,814,8,69,1,70,1,70,1,70,3,70,819,
  	8,70,1,71,1,71,1,71,1,71,1,71,1,71,1,71,3,71,828,8,71,1,71,1,71,1,72,
  	1,72,1,72,1,72,1,72,1,72,1,72,1,72,1,72,1,72,1,72,1,73,1,73,1,73,1,73,
  	1,73,1,73,1,73,1,73,1,73,1,73,1,73,3,73,854,8,73,1,73,1,73,1,74,1,74,
  	1,75,1,75,1,76,1,76,1,76,0,0,77,0,2,4,6,8,10,12,14,16,18,20,22,24,26,
  	28,30,32,34,36,38,40,42,44,46,48,50,52,54,56,58,60,62,64,66,68,70,72,
  	74,76,78,80,82,84,86,88,90,92,94,96,98,100,102,104,106,108,110,112,114,
  	116,118,120,122,124,126,128,130,132,134,136,138,140,142,144,146,148,150,
  	152,0,18,3,0,3,3,19,19,34,34,2,0,30,30,38,38,2,0,30,30,72,74,4,0,21,21,
  	31,31,46,46,48,48,3,0,6,6,36,37,60,60,2,0,8,8,15,15,2,0,4,4,7,7,4,0,1,
  	1,54,54,67,67,69,69,2,0,78,78,82,82,1,0,80,81,1,0,88,93,2,0,27,27,65,
  	65,1,0,101,102,1,0,84,86,2,0,95,95,98,99,2,0,35,35,47,47,5,0,10,10,24,
  	24,28,28,51,51,61,61,3,0,1,26,28,64,66,74,916,0,165,1,0,0,0,2,169,1,0,
  	0,0,4,181,1,0,0,0,6,190,1,0,0,0,8,199,1,0,0,0,10,214,1,0,0,0,12,234,1,
  	0,0,0,14,239,1,0,0,0,16,241,1,0,0,0,18,248,1,0,0,0,20,262,1,0,0,0,22,
  	268,1,0,0,0,24,285,1,0,0,0,26,298,1,0,0,0,28,300,1,0,0,0,30,302,1,0,0,
  	0,32,304,1,0,0,0,34,323,1,0,0,0,36,326,1,0,0,0,38,328,1,0,0,0,40,331,
  	1,0,0,0,42,333,1,0,0,0,44,340,1,0,0,0,46,349,1,0,0,0,48,354,1,0,0,0,50,
  	363,1,0,0,0,52,366,1,0,0,0,54,369,1,0,0,0,56,379,1,0,0,0,58,412,1,0,0,
  	0,60,422,1,0,0,0,62,424,1,0,0,0,64,427,1,0,0,0,66,430,1,0,0,0,68,467,
  	1,0,0,0,70,469,1,0,0,0,72,472,1,0,0,0,74,508,1,0,0,0,76,510,1,0,0,0,78,
  	514,1,0,0,0,80,521,1,0,0,0,82,526,1,0,0,0,84,534,1,0,0,0,86,571,1,0,0,
  	0,88,573,1,0,0,0,90,581,1,0,0,0,92,605,1,0,0,0,94,615,1,0,0,0,96,617,
  	1,0,0,0,98,626,1,0,0,0,100,628,1,0,0,0,102,636,1,0,0,0,104,651,1,0,0,
  	0,106,653,1,0,0,0,108,665,1,0,0,0,110,680,1,0,0,0,112,720,1,0,0,0,114,
  	722,1,0,0,0,116,727,1,0,0,0,118,729,1,0,0,0,120,732,1,0,0,0,122,736,1,
  	0,0,0,124,745,1,0,0,0,126,747,1,0,0,0,128,755,1,0,0,0,130,775,1,0,0,0,
  	132,777,1,0,0,0,134,785,1,0,0,0,136,800,1,0,0,0,138,813,1,0,0,0,140,818,
  	1,0,0,0,142,820,1,0,0,0,144,831,1,0,0,0,146,842,1,0,0,0,148,857,1,0,0,
  	0,150,859,1,0,0,0,152,861,1,0,0,0,154,156,3,48,24,0,155,154,1,0,0,0,155,
  	156,1,0,0,0,156,157,1,0,0,0,157,160,3,2,1,0,158,160,3,44,22,0,159,155,
  	1,0,0,0,159,158,1,0,0,0,160,166,1,0,0,0,161,166,3,4,2,0,162,166,3,14,
  	7,0,163,166,3,30,15,0,164,166,3,42,21,0,165,159,1,0,0,0,165,161,1,0,0,
  	0,165,162,1,0,0,0,165,163,1,0,0,0,165,164,1,0,0,0,166,167,1,0,0,0,167,
  	168,5,0,0,1,168,1,1,0,0,0,169,171,3,50,25,0,170,172,3,52,26,0,171,170,
  	1,0,0,0,171,172,1,0,0,0,172,174,1,0,0,0,173,175,3,54,27,0,174,173,1,0,
  	0,0,174,175,1,0,0,0,175,177,1,0,0,0,176,178,3,56,28,0,177,176,1,0,0,0,
  	177,178,1,0,0,0,178,179,1,0,0,0,179,180,3,58,29,0,180,3,1,0,0,0,181,182,
  	5,23,0,0,182,187,3,6,3,0,183,184,5,109,0,0,184,186,3,6,3,0,185,183,1,
  	0,0,0,186,189,1,0,0,0,187,185,1,0,0,0,187,188,1,0,0,0,188,5,1,0,0,0,189,
  	187,1,0,0,0,190,196,3,8,4,0,191,192,3,12,6,0,192,193,3,8,4,0,193,195,
  	1,0,0,0,194,191,1,0,0,0,195,198,1,0,0,0,196,194,1,0,0,0,196,197,1,0,0,
  	0,197,7,1,0,0,0,198,196,1,0,0,0,199,201,5,103,0,0,200,202,3,10,5,0,201,
  	200,1,0,0,0,201,202,1,0,0,0,202,206,1,0,0,0,203,205,5,80,0,0,204,203,
  	1,0,0,0,205,208,1,0,0,0,206,204,1,0,0,0,206,207,1,0,0,0,207,210,1,0,0,
  	0,208,206,1,0,0,0,209,211,3,110,55,0,210,209,1,0,0,0,210,211,1,0,0,0,
  	211,212,1,0,0,0,212,213,5,104,0,0,213,9,1,0,0,0,214,215,3,116,58,0,215,
  	11,1,0,0,0,216,217,5,92,0,0,217,218,5,102,0,0,218,219,5,107,0,0,219,221,
  	5,80,0,0,220,222,3,110,55,0,221,220,1,0,0,0,221,222,1,0,0,0,222,223,1,
  	0,0,0,223,224,5,108,0,0,224,235,5,102,0,0,225,226,5,102,0,0,226,227,5,
  	107,0,0,227,229,5,80,0,0,228,230,3,110,55,0,229,228,1,0,0,0,229,230,1,
  	0,0,0,230,231,1,0,0,0,231,232,5,108,0,0,232,233,5,102,0,0,233,235,5,93,
  	0,0,234,216,1,0,0,0,234,225,1,0,0,0,235,13,1,0,0,0,236,240,3,16,8,0,237,
  	240,3,18,9,0,238,240,3,22,11,0,239,236,1,0,0,0,239,237,1,0,0,0,239,238,
  	1,0,0,0,240,15,1,0,0,0,241,242,5,12,0,0,242,243,5,63,0,0,243,244,5,62,
  	0,0,244,245,5,83,0,0,245,246,5,16,0,0,246,247,5,84,0,0,247,17,1,0,0,0,
  	248,249,5,23,0,0,249,250,5,25,0,0,250,251,5,63,0,0,251,252,5,62,0,0,252,
  	253,5,83,0,0,253,254,5,68,0,0,254,259,3,20,10,0,255,256,5,109,0,0,256,
  	258,3,20,10,0,257,255,1,0,0,0,258,261,1,0,0,0,259,257,1,0,0,0,259,260,
  	1,0,0,0,260,19,1,0,0,0,261,259,1,0,0,0,262,263,5,103,0,0,263,264,3,116,
  	58,0,264,265,5,109,0,0,265,266,3,24,12,0,266,267,5,104,0,0,267,21,1,0,
  	0,0,268,269,5,13,0,0,269,270,5,20,0,0,270,271,5,63,0,0,271,272,5,62,0,
  	0,272,273,5,83,0,0,273,274,5,40,0,0,274,275,5,103,0,0,275,280,3,116,58,
  	0,276,277,5,109,0,0,277,279,3,116,58,0,278,276,1,0,0,0,279,282,1,0,0,
  	0,280,278,1,0,0,0,280,281,1,0,0,0,281,283,1,0,0,0,282,280,1,0,0,0,283,
  	284,5,104,0,0,284,23,1,0,0,0,285,286,5,107,0,0,286,291,3,120,60,0,287,
  	288,5,109,0,0,288,290,3,120,60,0,289,287,1,0,0,0,290,293,1,0,0,0,291,
  	289,1,0,0,0,291,292,1,0,0,0,292,294,1,0,0,0,293,291,1,0,0,0,294,295,5,
  	108,0,0,295,25,1,0,0,0,296,299,3,126,63,0,297,299,3,24,12,0,298,296,1,
  	0,0,0,298,297,1,0,0,0,299,27,1,0,0,0,300,301,7,0,0,0,301,29,1,0,0,0,302,
  	303,3,32,16,0,303,31,1,0,0,0,304,305,5,12,0,0,305,306,5,64,0,0,306,307,
  	5,22,0,0,307,308,5,83,0,0,308,309,5,42,0,0,309,321,3,116,58,0,310,319,
  	5,70,0,0,311,320,3,34,17,0,312,320,3,38,19,0,313,314,3,34,17,0,314,315,
  	3,38,19,0,315,320,1,0,0,0,316,317,3,38,19,0,317,318,3,34,17,0,318,320,
  	1,0,0,0,319,311,1,0,0,0,319,312,1,0,0,0,319,313,1,0,0,0,319,316,1,0,0,
  	0,320,322,1,0,0,0,321,310,1,0,0,0,321,322,1,0,0,0,322,33,1,0,0,0,323,
  	324,5,39,0,0,324,325,3,36,18,0,325,35,1,0,0,0,326,327,7,1,0,0,327,37,
  	1,0,0,0,328,329,5,66,0,0,329,330,3,40,20,0,330,39,1,0,0,0,331,332,7,2,
  	0,0,332,41,1,0,0,0,333,338,5,53,0,0,334,335,5,63,0,0,335,339,5,62,0,0,
  	336,337,5,64,0,0,337,339,5,22,0,0,338,334,1,0,0,0,338,336,1,0,0,0,339,
  	43,1,0,0,0,340,344,5,14,0,0,341,343,3,46,23,0,342,341,1,0,0,0,343,346,
  	1,0,0,0,344,342,1,0,0,0,344,345,1,0,0,0,345,347,1,0,0,0,346,344,1,0,0,
  	0,347,348,3,102,51,0,348,45,1,0,0,0,349,352,7,3,0,0,350,351,5,33,0,0,
  	351,353,5,84,0,0,352,350,1,0,0,0,352,353,1,0,0,0,353,47,1,0,0,0,354,355,
  	5,59,0,0,355,360,3,66,33,0,356,357,5,109,0,0,357,359,3,66,33,0,358,356,
  	1,0,0,0,359,362,1,0,0,0,360,358,1,0,0,0,360,361,1,0,0,0,361,49,1,0,0,
  	0,362,360,1,0,0,0,363,364,5,35,0,0,364,365,3,78,39,0,365,51,1,0,0,0,366,
  	367,5,71,0,0,367,368,3,126,63,0,368,53,1,0,0,0,369,370,5,29,0,0,370,371,
  	5,9,0,0,371,376,3,76,38,0,372,373,5,109,0,0,373,375,3,76,38,0,374,372,
  	1,0,0,0,375,378,1,0,0,0,376,374,1,0,0,0,376,377,1,0,0,0,377,55,1,0,0,
  	0,378,376,1,0,0,0,379,380,5,44,0,0,380,381,5,9,0,0,381,386,3,74,37,0,
  	382,383,5,109,0,0,383,385,3,74,37,0,384,382,1,0,0,0,385,388,1,0,0,0,386,
  	384,1,0,0,0,386,387,1,0,0,0,387,57,1,0,0,0,388,386,1,0,0,0,389,391,5,
  	58,0,0,390,392,5,17,0,0,391,390,1,0,0,0,391,392,1,0,0,0,392,393,1,0,0,
  	0,393,398,3,68,34,0,394,395,5,109,0,0,395,397,3,68,34,0,396,394,1,0,0,
  	0,397,400,1,0,0,0,398,396,1,0,0,0,398,399,1,0,0,0,399,402,1,0,0,0,400,
  	398,1,0,0,0,401,403,3,60,30,0,402,401,1,0,0,0,402,403,1,0,0,0,403,413,
  	1,0,0,0,404,406,5,58,0,0,405,407,5,17,0,0,406,405,1,0,0,0,406,407,1,0,
  	0,0,407,408,1,0,0,0,408,410,5,98,0,0,409,411,3,60,30,0,410,409,1,0,0,
  	0,410,411,1,0,0,0,411,413,1,0,0,0,412,389,1,0,0,0,412,404,1,0,0,0,413,
  	59,1,0,0,0,414,416,3,62,31,0,415,417,3,64,32,0,416,415,1,0,0,0,416,417,
  	1,0,0,0,417,423,1,0,0,0,418,420,3,64,32,0,419,421,3,62,31,0,420,419,1,
  	0,0,0,420,421,1,0,0,0,421,423,1,0,0,0,422,414,1,0,0,0,422,418,1,0,0,0,
  	423,61,1,0,0,0,424,425,5,33,0,0,425,426,5,84,0,0,426,63,1,0,0,0,427,428,
  	5,41,0,0,428,429,5,84,0,0,429,65,1,0,0,0,430,431,5,82,0,0,431,432,5,94,
  	0,0,432,433,3,102,51,0,433,67,1,0,0,0,434,436,5,82,0,0,435,437,5,79,0,
  	0,436,435,1,0,0,0,436,437,1,0,0,0,437,468,1,0,0,0,438,439,3,72,36,0,439,
  	440,5,103,0,0,440,442,5,82,0,0,441,443,5,79,0,0,442,441,1,0,0,0,442,443,
  	1,0,0,0,443,444,1,0,0,0,444,446,5,104,0,0,445,447,3,70,35,0,446,445,1,
  	0,0,0,446,447,1,0,0,0,447,468,1,0,0,0,448,449,5,11,0,0,449,451,5,103,
  	0,0,450,452,5,17,0,0,451,450,1,0,0,0,451,452,1,0,0,0,452,458,1,0,0,0,
  	453,455,5,82,0,0,454,456,5,79,0,0,455,454,1,0,0,0,455,456,1,0,0,0,456,
  	459,1,0,0,0,457,459,5,98,0,0,458,453,1,0,0,0,458,457,1,0,0,0,459,460,
  	1,0,0,0,460,462,5,104,0,0,461,463,3,70,35,0,462,461,1,0,0,0,462,463,1,
  	0,0,0,463,468,1,0,0,0,464,465,3,126,63,0,465,466,3,70,35,0,466,468,1,
  	0,0,0,467,434,1,0,0,0,467,438,1,0,0,0,467,448,1,0,0,0,467,464,1,0,0,0,
  	468,69,1,0,0,0,469,470,5,5,0,0,470,471,5,82,0,0,471,71,1,0,0,0,472,473,
  	7,4,0,0,473,73,1,0,0,0,474,476,5,82,0,0,475,477,5,79,0,0,476,475,1,0,
  	0,0,476,477,1,0,0,0,477,479,1,0,0,0,478,480,7,5,0,0,479,478,1,0,0,0,479,
  	480,1,0,0,0,480,509,1,0,0,0,481,482,3,72,36,0,482,483,5,103,0,0,483,485,
  	5,82,0,0,484,486,5,79,0,0,485,484,1,0,0,0,485,486,1,0,0,0,486,487,1,0,
  	0,0,487,489,5,104,0,0,488,490,7,5,0,0,489,488,1,0,0,0,489,490,1,0,0,0,
  	490,509,1,0,0,0,491,492,5,11,0,0,492,494,5,103,0,0,493,495,5,17,0,0,494,
  	493,1,0,0,0,494,495,1,0,0,0,495,496,1,0,0,0,496,498,5,82,0,0,497,499,
  	5,79,0,0,498,497,1,0,0,0,498,499,1,0,0,0,499,500,1,0,0,0,500,502,5,104,
  	0,0,501,503,7,5,0,0,502,501,1,0,0,0,502,503,1,0,0,0,503,509,1,0,0,0,504,
  	506,3,126,63,0,505,507,7,5,0,0,506,505,1,0,0,0,506,507,1,0,0,0,507,509,
  	1,0,0,0,508,474,1,0,0,0,508,481,1,0,0,0,508,491,1,0,0,0,508,504,1,0,0,
  	0,509,75,1,0,0,0,510,512,5,82,0,0,511,513,5,79,0,0,512,511,1,0,0,0,512,
  	513,1,0,0,0,513,77,1,0,0,0,514,518,3,82,41,0,515,517,3,80,40,0,516,515,
  	1,0,0,0,517,520,1,0,0,0,518,516,1,0,0,0,518,519,1,0,0,0,519,79,1,0,0,
  	0,520,518,1,0,0,0,521,522,5,43,0,0,522,523,5,105,0,0,523,524,3,78,39,
  	0,524,525,5,106,0,0,525,81,1,0,0,0,526,531,3,84,42,0,527,528,5,109,0,
  	0,528,530,3,84,42,0,529,527,1,0,0,0,530,533,1,0,0,0,531,529,1,0,0,0,531,
  	532,1,0,0,0,532,83,1,0,0,0,533,531,1,0,0,0,534,543,3,98,49,0,535,538,
  	3,104,52,0,536,538,3,86,43,0,537,535,1,0,0,0,537,536,1,0,0,0,538,539,
  	1,0,0,0,539,540,3,98,49,0,540,542,1,0,0,0,541,537,1,0,0,0,542,545,1,0,
  	0,0,543,541,1,0,0,0,543,544,1,0,0,0,544,85,1,0,0,0,545,543,1,0,0,0,546,
  	547,5,88,0,0,547,549,5,107,0,0,548,550,3,96,48,0,549,548,1,0,0,0,549,
  	550,1,0,0,0,550,552,1,0,0,0,551,553,5,82,0,0,552,551,1,0,0,0,552,553,
  	1,0,0,0,553,554,1,0,0,0,554,555,3,88,44,0,555,556,5,108,0,0,556,557,5,
  	94,0,0,557,572,1,0,0,0,558,559,5,94,0,0,559,561,5,107,0,0,560,562,3,96,
  	48,0,561,560,1,0,0,0,561,562,1,0,0,0,562,564,1,0,0,0,563,565,5,82,0,0,
  	564,563,1,0,0,0,564,565,1,0,0,0,565,566,1,0,0,0,566,567,3,88,44,0,567,
  	568,5,108,0,0,568,569,5,94,0,0,569,570,5,93,0,0,570,572,1,0,0,0,571,546,
  	1,0,0,0,571,558,1,0,0,0,572,87,1,0,0,0,573,578,3,90,45,0,574,575,5,96,
  	0,0,575,577,3,90,45,0,576,574,1,0,0,0,577,580,1,0,0,0,578,576,1,0,0,0,
  	578,579,1,0,0,0,579,89,1,0,0,0,580,578,1,0,0,0,581,586,3,92,46,0,582,
  	583,5,95,0,0,583,585,3,92,46,0,584,582,1,0,0,0,585,588,1,0,0,0,586,584,
  	1,0,0,0,586,587,1,0,0,0,587,91,1,0,0,0,588,586,1,0,0,0,589,591,5,97,0,
  	0,590,589,1,0,0,0,590,591,1,0,0,0,591,592,1,0,0,0,592,594,5,80,0,0,593,
  	595,3,94,47,0,594,593,1,0,0,0,594,595,1,0,0,0,595,606,1,0,0,0,596,598,
  	5,97,0,0,597,596,1,0,0,0,597,598,1,0,0,0,598,599,1,0,0,0,599,600,5,103,
  	0,0,600,601,3,88,44,0,601,603,5,104,0,0,602,604,3,94,47,0,603,602,1,0,
  	0,0,603,604,1,0,0,0,604,606,1,0,0,0,605,590,1,0,0,0,605,597,1,0,0,0,606,
  	93,1,0,0,0,607,616,5,98,0,0,608,616,5,101,0,0,609,616,5,100,0,0,610,611,
  	5,105,0,0,611,612,5,84,0,0,612,613,5,109,0,0,613,614,5,84,0,0,614,616,
  	5,106,0,0,615,607,1,0,0,0,615,608,1,0,0,0,615,609,1,0,0,0,615,610,1,0,
  	0,0,616,95,1,0,0,0,617,619,7,6,0,0,618,620,5,52,0,0,619,618,1,0,0,0,619,
  	620,1,0,0,0,620,622,1,0,0,0,621,623,7,7,0,0,622,621,1,0,0,0,622,623,1,
  	0,0,0,623,97,1,0,0,0,624,627,3,100,50,0,625,627,3,108,54,0,626,624,1,
  	0,0,0,626,625,1,0,0,0,627,99,1,0,0,0,628,629,5,103,0,0,629,630,3,102,
  	51,0,630,631,5,104,0,0,631,101,1,0,0,0,632,637,3,116,58,0,633,637,5,77,
  	0,0,634,637,5,78,0,0,635,637,3,124,62,0,636,632,1,0,0,0,636,633,1,0,0,
  	0,636,634,1,0,0,0,636,635,1,0,0,0,637,103,1,0,0,0,638,641,5,92,0,0,639,
  	640,5,102,0,0,640,642,3,106,53,0,641,639,1,0,0,0,641,642,1,0,0,0,642,
  	643,1,0,0,0,643,652,5,102,0,0,644,648,5,102,0,0,645,646,3,106,53,0,646,
  	647,5,102,0,0,647,649,1,0,0,0,648,645,1,0,0,0,648,649,1,0,0,0,649,650,
  	1,0,0,0,650,652,5,93,0,0,651,638,1,0,0,0,651,644,1,0,0,0,652,105,1,0,
  	0,0,653,655,5,107,0,0,654,656,7,8,0,0,655,654,1,0,0,0,655,656,1,0,0,0,
  	656,658,1,0,0,0,657,659,7,9,0,0,658,657,1,0,0,0,658,659,1,0,0,0,659,661,
  	1,0,0,0,660,662,3,110,55,0,661,660,1,0,0,0,661,662,1,0,0,0,662,663,1,
  	0,0,0,663,664,5,108,0,0,664,107,1,0,0,0,665,667,5,103,0,0,666,668,5,82,
  	0,0,667,666,1,0,0,0,667,668,1,0,0,0,668,672,1,0,0,0,669,671,5,80,0,0,
  	670,669,1,0,0,0,671,674,1,0,0,0,672,670,1,0,0,0,672,673,1,0,0,0,673,676,
  	1,0,0,0,674,672,1,0,0,0,675,677,3,110,55,0,676,675,1,0,0,0,676,677,1,
  	0,0,0,677,678,1,0,0,0,678,679,5,104,0,0,679,109,1,0,0,0,680,681,5,105,
  	0,0,681,686,3,112,56,0,682,683,5,109,0,0,683,685,3,112,56,0,684,682,1,
  	0,0,0,685,688,1,0,0,0,686,684,1,0,0,0,686,687,1,0,0,0,687,689,1,0,0,0,
  	688,686,1,0,0,0,689,690,5,106,0,0,690,111,1,0,0,0,691,696,3,116,58,0,
  	692,693,5,110,0,0,693,697,3,124,62,0,694,697,5,75,0,0,695,697,5,76,0,
  	0,696,692,1,0,0,0,696,694,1,0,0,0,696,695,1,0,0,0,697,721,1,0,0,0,698,
  	699,3,116,58,0,699,700,5,80,0,0,700,701,5,103,0,0,701,702,5,83,0,0,702,
  	703,5,104,0,0,703,721,1,0,0,0,704,705,3,116,58,0,705,707,5,26,0,0,706,
  	708,5,50,0,0,707,706,1,0,0,0,707,708,1,0,0,0,708,709,1,0,0,0,709,713,
  	3,150,75,0,710,712,3,114,57,0,711,710,1,0,0,0,712,715,1,0,0,0,713,711,
  	1,0,0,0,713,714,1,0,0,0,714,721,1,0,0,0,715,713,1,0,0,0,716,717,3,116,
  	58,0,717,718,7,10,0,0,718,719,3,124,62,0,719,721,1,0,0,0,720,691,1,0,
  	0,0,720,698,1,0,0,0,720,704,1,0,0,0,720,716,1,0,0,0,721,113,1,0,0,0,722,
  	723,5,45,0,0,723,724,3,150,75,0,724,115,1,0,0,0,725,728,5,87,0,0,726,
  	728,3,152,76,0,727,725,1,0,0,0,727,726,1,0,0,0,728,117,1,0,0,0,729,730,
  	7,11,0,0,730,119,1,0,0,0,731,733,7,12,0,0,732,731,1,0,0,0,732,733,1,0,
  	0,0,733,734,1,0,0,0,734,735,7,13,0,0,735,121,1,0,0,0,736,737,3,116,58,
  	0,737,738,5,103,0,0,738,739,5,83,0,0,739,740,5,104,0,0,740,123,1,0,0,
  	0,741,746,3,120,60,0,742,746,5,83,0,0,743,746,3,118,59,0,744,746,3,122,
  	61,0,745,741,1,0,0,0,745,742,1,0,0,0,745,743,1,0,0,0,745,744,1,0,0,0,
  	746,125,1,0,0,0,747,752,3,128,64,0,748,749,5,45,0,0,749,751,3,128,64,
  	0,750,748,1,0,0,0,751,754,1,0,0,0,752,750,1,0,0,0,752,753,1,0,0,0,753,
  	127,1,0,0,0,754,752,1,0,0,0,755,760,3,130,65,0,756,757,5,2,0,0,757,759,
  	3,130,65,0,758,756,1,0,0,0,759,762,1,0,0,0,760,758,1,0,0,0,760,761,1,
  	0,0,0,761,129,1,0,0,0,762,760,1,0,0,0,763,766,3,132,66,0,764,765,7,10,
  	0,0,765,767,3,132,66,0,766,764,1,0,0,0,766,767,1,0,0,0,767,776,1,0,0,
  	0,768,769,3,132,66,0,769,771,5,26,0,0,770,772,5,50,0,0,771,770,1,0,0,
  	0,771,772,1,0,0,0,772,773,1,0,0,0,773,774,3,150,75,0,774,776,1,0,0,0,
  	775,763,1,0,0,0,775,768,1,0,0,0,776,131,1,0,0,0,777,782,3,134,67,0,778,
  	779,7,12,0,0,779,781,3,134,67,0,780,778,1,0,0,0,781,784,1,0,0,0,782,780,
  	1,0,0,0,782,783,1,0,0,0,783,133,1,0,0,0,784,782,1,0,0,0,785,790,3,136,
  	68,0,786,787,7,14,0,0,787,789,3,136,68,0,788,786,1,0,0,0,789,792,1,0,
  	0,0,790,788,1,0,0,0,790,791,1,0,0,0,791,135,1,0,0,0,792,790,1,0,0,0,793,
  	794,5,50,0,0,794,801,3,136,68,0,795,801,3,138,69,0,796,797,5,101,0,0,
  	797,801,3,136,68,0,798,799,5,102,0,0,799,801,3,136,68,0,800,793,1,0,0,
  	0,800,795,1,0,0,0,800,796,1,0,0,0,800,798,1,0,0,0,801,137,1,0,0,0,802,
  	804,5,82,0,0,803,805,5,79,0,0,804,803,1,0,0,0,804,805,1,0,0,0,805,814,
  	1,0,0,0,806,814,3,124,62,0,807,814,3,102,51,0,808,809,5,103,0,0,809,810,
  	3,126,63,0,810,811,5,104,0,0,811,814,1,0,0,0,812,814,3,140,70,0,813,802,
  	1,0,0,0,813,806,1,0,0,0,813,807,1,0,0,0,813,808,1,0,0,0,813,812,1,0,0,
  	0,814,139,1,0,0,0,815,819,3,142,71,0,816,819,3,144,72,0,817,819,3,146,
  	73,0,818,815,1,0,0,0,818,816,1,0,0,0,818,817,1,0,0,0,819,141,1,0,0,0,
  	820,821,5,57,0,0,821,822,5,103,0,0,822,823,3,126,63,0,823,824,5,109,0,
  	0,824,827,3,126,63,0,825,826,5,109,0,0,826,828,3,126,63,0,827,825,1,0,
  	0,0,827,828,1,0,0,0,828,829,1,0,0,0,829,830,5,104,0,0,830,143,1,0,0,0,
  	831,832,5,55,0,0,832,833,5,103,0,0,833,834,5,83,0,0,834,835,5,109,0,0,
  	835,836,3,126,63,0,836,837,5,109,0,0,837,838,3,26,13,0,838,839,5,109,
  	0,0,839,840,3,28,14,0,840,841,5,104,0,0,841,145,1,0,0,0,842,843,5,56,
  	0,0,843,844,5,103,0,0,844,845,5,83,0,0,845,846,5,109,0,0,846,847,5,83,
  	0,0,847,848,5,109,0,0,848,849,3,148,74,0,849,850,5,109,0,0,850,853,5,
  	82,0,0,851,852,5,109,0,0,852,854,5,82,0,0,853,851,1,0,0,0,853,854,1,0,
  	0,0,854,855,1,0,0,0,855,856,5,104,0,0,856,147,1,0,0,0,857,858,7,15,0,
  	0,858,149,1,0,0,0,859,860,7,16,0,0,860,151,1,0,0,0,861,862,7,17,0,0,862,
  	153,1,0,0,0,105,155,159,165,171,174,177,187,196,201,206,210,221,229,234,
  	239,259,280,291,298,319,321,338,344,352,360,376,386,391,398,402,406,410,
  	412,416,420,422,436,442,446,451,455,458,462,467,476,479,485,489,494,498,
  	502,506,508,512,518,531,537,543,549,552,561,564,571,578,586,590,594,597,
  	603,605,615,619,622,626,636,641,648,651,655,658,661,667,672,676,686,696,
  	707,713,720,727,732,745,752,760,766,771,775,782,790,800,804,813,818,827,
  	853
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

MQL_Parser::InsertPatternsContext* MQL_Parser::RootContext::insertPatterns() {
  return getRuleContext<MQL_Parser::InsertPatternsContext>(0);
}

MQL_Parser::TensorStoreQueryContext* MQL_Parser::RootContext::tensorStoreQuery() {
  return getRuleContext<MQL_Parser::TensorStoreQueryContext>(0);
}

MQL_Parser::TextIndexQueryContext* MQL_Parser::RootContext::textIndexQuery() {
  return getRuleContext<MQL_Parser::TextIndexQueryContext>(0);
}

MQL_Parser::ShowQueryContext* MQL_Parser::RootContext::showQuery() {
  return getRuleContext<MQL_Parser::ShowQueryContext>(0);
}

MQL_Parser::MatchQueryContext* MQL_Parser::RootContext::matchQuery() {
  return getRuleContext<MQL_Parser::MatchQueryContext>(0);
}

MQL_Parser::DescribeQueryContext* MQL_Parser::RootContext::describeQuery() {
  return getRuleContext<MQL_Parser::DescribeQueryContext>(0);
}

MQL_Parser::SetStatementContext* MQL_Parser::RootContext::setStatement() {
  return getRuleContext<MQL_Parser::SetStatementContext>(0);
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
    setState(165);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 2, _ctx)) {
    case 1: {
      setState(159);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case MQL_Parser::K_MATCH:
        case MQL_Parser::K_SET: {
          setState(155);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == MQL_Parser::K_SET) {
            setState(154);
            setStatement();
          }
          setState(157);
          matchQuery();
          break;
        }

        case MQL_Parser::K_DESCRIBE: {
          setState(158);
          describeQuery();
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      break;
    }

    case 2: {
      setState(161);
      insertPatterns();
      break;
    }

    case 3: {
      setState(162);
      tensorStoreQuery();
      break;
    }

    case 4: {
      setState(163);
      textIndexQuery();
      break;
    }

    case 5: {
      setState(164);
      showQuery();
      break;
    }

    default:
      break;
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

MQL_Parser::MatchStatementContext* MQL_Parser::MatchQueryContext::matchStatement() {
  return getRuleContext<MQL_Parser::MatchStatementContext>(0);
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
    matchStatement();
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
  enterRule(_localctx, 4, MQL_Parser::RuleInsertPatterns);
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
    setState(181);
    match(MQL_Parser::K_INSERT);
    setState(182);
    insertLinearPattern();
    setState(187);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MQL_Parser::COMMA) {
      setState(183);
      match(MQL_Parser::COMMA);
      setState(184);
      insertLinearPattern();
      setState(189);
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
  enterRule(_localctx, 6, MQL_Parser::RuleInsertLinearPattern);
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
    setState(190);
    insertPlainNode();
    setState(196);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MQL_Parser::LT

    || _la == MQL_Parser::MINUS) {
      setState(191);
      insertPlainEdge();
      setState(192);
      insertPlainNode();
      setState(198);
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
  enterRule(_localctx, 8, MQL_Parser::RuleInsertPlainNode);
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
    setState(199);
    match(MQL_Parser::L_PAR);
    setState(201);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & -134217730) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 64)) & 8390653) != 0)) {
      setState(200);
      insertPlainNodeInside();
    }
    setState(206);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MQL_Parser::TYPE) {
      setState(203);
      match(MQL_Parser::TYPE);
      setState(208);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(210);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MQL_Parser::LCURLY_BRACKET) {
      setState(209);
      properties();
    }
    setState(212);
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
  enterRule(_localctx, 10, MQL_Parser::RuleInsertPlainNodeInside);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(214);
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
  enterRule(_localctx, 12, MQL_Parser::RuleInsertPlainEdge);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(234);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case MQL_Parser::LT: {
        enterOuterAlt(_localctx, 1);
        setState(216);
        match(MQL_Parser::LT);
        setState(217);
        match(MQL_Parser::MINUS);
        setState(218);
        match(MQL_Parser::LSQUARE_BRACKET);
        setState(219);
        match(MQL_Parser::TYPE);
        setState(221);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == MQL_Parser::LCURLY_BRACKET) {
          setState(220);
          properties();
        }
        setState(223);
        match(MQL_Parser::RSQUARE_BRACKET);
        setState(224);
        match(MQL_Parser::MINUS);
        break;
      }

      case MQL_Parser::MINUS: {
        enterOuterAlt(_localctx, 2);
        setState(225);
        match(MQL_Parser::MINUS);
        setState(226);
        match(MQL_Parser::LSQUARE_BRACKET);
        setState(227);
        match(MQL_Parser::TYPE);
        setState(229);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == MQL_Parser::LCURLY_BRACKET) {
          setState(228);
          properties();
        }
        setState(231);
        match(MQL_Parser::RSQUARE_BRACKET);
        setState(232);
        match(MQL_Parser::MINUS);
        setState(233);
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

//----------------- TensorStoreQueryContext ------------------------------------------------------------------

MQL_Parser::TensorStoreQueryContext::TensorStoreQueryContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

MQL_Parser::CreateTensorStoreContext* MQL_Parser::TensorStoreQueryContext::createTensorStore() {
  return getRuleContext<MQL_Parser::CreateTensorStoreContext>(0);
}

MQL_Parser::InsertTensorsContext* MQL_Parser::TensorStoreQueryContext::insertTensors() {
  return getRuleContext<MQL_Parser::InsertTensorsContext>(0);
}

MQL_Parser::DeleteTensorsContext* MQL_Parser::TensorStoreQueryContext::deleteTensors() {
  return getRuleContext<MQL_Parser::DeleteTensorsContext>(0);
}


size_t MQL_Parser::TensorStoreQueryContext::getRuleIndex() const {
  return MQL_Parser::RuleTensorStoreQuery;
}


std::any MQL_Parser::TensorStoreQueryContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitTensorStoreQuery(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::TensorStoreQueryContext* MQL_Parser::tensorStoreQuery() {
  TensorStoreQueryContext *_localctx = _tracker.createInstance<TensorStoreQueryContext>(_ctx, getState());
  enterRule(_localctx, 14, MQL_Parser::RuleTensorStoreQuery);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(239);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case MQL_Parser::K_CREATE: {
        enterOuterAlt(_localctx, 1);
        setState(236);
        createTensorStore();
        break;
      }

      case MQL_Parser::K_INSERT: {
        enterOuterAlt(_localctx, 2);
        setState(237);
        insertTensors();
        break;
      }

      case MQL_Parser::K_DELETE: {
        enterOuterAlt(_localctx, 3);
        setState(238);
        deleteTensors();
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

//----------------- CreateTensorStoreContext ------------------------------------------------------------------

MQL_Parser::CreateTensorStoreContext::CreateTensorStoreContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::CreateTensorStoreContext::K_CREATE() {
  return getToken(MQL_Parser::K_CREATE, 0);
}

tree::TerminalNode* MQL_Parser::CreateTensorStoreContext::K_TENSOR() {
  return getToken(MQL_Parser::K_TENSOR, 0);
}

tree::TerminalNode* MQL_Parser::CreateTensorStoreContext::K_STORE() {
  return getToken(MQL_Parser::K_STORE, 0);
}

tree::TerminalNode* MQL_Parser::CreateTensorStoreContext::STRING() {
  return getToken(MQL_Parser::STRING, 0);
}

tree::TerminalNode* MQL_Parser::CreateTensorStoreContext::K_DIMENSIONS() {
  return getToken(MQL_Parser::K_DIMENSIONS, 0);
}

tree::TerminalNode* MQL_Parser::CreateTensorStoreContext::UNSIGNED_INTEGER() {
  return getToken(MQL_Parser::UNSIGNED_INTEGER, 0);
}


size_t MQL_Parser::CreateTensorStoreContext::getRuleIndex() const {
  return MQL_Parser::RuleCreateTensorStore;
}


std::any MQL_Parser::CreateTensorStoreContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitCreateTensorStore(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::CreateTensorStoreContext* MQL_Parser::createTensorStore() {
  CreateTensorStoreContext *_localctx = _tracker.createInstance<CreateTensorStoreContext>(_ctx, getState());
  enterRule(_localctx, 16, MQL_Parser::RuleCreateTensorStore);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(241);
    match(MQL_Parser::K_CREATE);
    setState(242);
    match(MQL_Parser::K_TENSOR);
    setState(243);
    match(MQL_Parser::K_STORE);
    setState(244);
    match(MQL_Parser::STRING);
    setState(245);
    match(MQL_Parser::K_DIMENSIONS);
    setState(246);
    match(MQL_Parser::UNSIGNED_INTEGER);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- InsertTensorsContext ------------------------------------------------------------------

MQL_Parser::InsertTensorsContext::InsertTensorsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::InsertTensorsContext::K_INSERT() {
  return getToken(MQL_Parser::K_INSERT, 0);
}

tree::TerminalNode* MQL_Parser::InsertTensorsContext::K_INTO() {
  return getToken(MQL_Parser::K_INTO, 0);
}

tree::TerminalNode* MQL_Parser::InsertTensorsContext::K_TENSOR() {
  return getToken(MQL_Parser::K_TENSOR, 0);
}

tree::TerminalNode* MQL_Parser::InsertTensorsContext::K_STORE() {
  return getToken(MQL_Parser::K_STORE, 0);
}

tree::TerminalNode* MQL_Parser::InsertTensorsContext::STRING() {
  return getToken(MQL_Parser::STRING, 0);
}

tree::TerminalNode* MQL_Parser::InsertTensorsContext::K_VALUES() {
  return getToken(MQL_Parser::K_VALUES, 0);
}

std::vector<MQL_Parser::InsertTensorsTupleContext *> MQL_Parser::InsertTensorsContext::insertTensorsTuple() {
  return getRuleContexts<MQL_Parser::InsertTensorsTupleContext>();
}

MQL_Parser::InsertTensorsTupleContext* MQL_Parser::InsertTensorsContext::insertTensorsTuple(size_t i) {
  return getRuleContext<MQL_Parser::InsertTensorsTupleContext>(i);
}

std::vector<tree::TerminalNode *> MQL_Parser::InsertTensorsContext::COMMA() {
  return getTokens(MQL_Parser::COMMA);
}

tree::TerminalNode* MQL_Parser::InsertTensorsContext::COMMA(size_t i) {
  return getToken(MQL_Parser::COMMA, i);
}


size_t MQL_Parser::InsertTensorsContext::getRuleIndex() const {
  return MQL_Parser::RuleInsertTensors;
}


std::any MQL_Parser::InsertTensorsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitInsertTensors(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::InsertTensorsContext* MQL_Parser::insertTensors() {
  InsertTensorsContext *_localctx = _tracker.createInstance<InsertTensorsContext>(_ctx, getState());
  enterRule(_localctx, 18, MQL_Parser::RuleInsertTensors);
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
    setState(248);
    match(MQL_Parser::K_INSERT);
    setState(249);
    match(MQL_Parser::K_INTO);
    setState(250);
    match(MQL_Parser::K_TENSOR);
    setState(251);
    match(MQL_Parser::K_STORE);
    setState(252);
    match(MQL_Parser::STRING);
    setState(253);
    match(MQL_Parser::K_VALUES);
    setState(254);
    insertTensorsTuple();
    setState(259);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MQL_Parser::COMMA) {
      setState(255);
      match(MQL_Parser::COMMA);
      setState(256);
      insertTensorsTuple();
      setState(261);
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

//----------------- InsertTensorsTupleContext ------------------------------------------------------------------

MQL_Parser::InsertTensorsTupleContext::InsertTensorsTupleContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::InsertTensorsTupleContext::L_PAR() {
  return getToken(MQL_Parser::L_PAR, 0);
}

MQL_Parser::IdentifierContext* MQL_Parser::InsertTensorsTupleContext::identifier() {
  return getRuleContext<MQL_Parser::IdentifierContext>(0);
}

tree::TerminalNode* MQL_Parser::InsertTensorsTupleContext::COMMA() {
  return getToken(MQL_Parser::COMMA, 0);
}

MQL_Parser::TensorContext* MQL_Parser::InsertTensorsTupleContext::tensor() {
  return getRuleContext<MQL_Parser::TensorContext>(0);
}

tree::TerminalNode* MQL_Parser::InsertTensorsTupleContext::R_PAR() {
  return getToken(MQL_Parser::R_PAR, 0);
}


size_t MQL_Parser::InsertTensorsTupleContext::getRuleIndex() const {
  return MQL_Parser::RuleInsertTensorsTuple;
}


std::any MQL_Parser::InsertTensorsTupleContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitInsertTensorsTuple(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::InsertTensorsTupleContext* MQL_Parser::insertTensorsTuple() {
  InsertTensorsTupleContext *_localctx = _tracker.createInstance<InsertTensorsTupleContext>(_ctx, getState());
  enterRule(_localctx, 20, MQL_Parser::RuleInsertTensorsTuple);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(262);
    match(MQL_Parser::L_PAR);
    setState(263);
    identifier();
    setState(264);
    match(MQL_Parser::COMMA);
    setState(265);
    tensor();
    setState(266);
    match(MQL_Parser::R_PAR);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DeleteTensorsContext ------------------------------------------------------------------

MQL_Parser::DeleteTensorsContext::DeleteTensorsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::DeleteTensorsContext::K_DELETE() {
  return getToken(MQL_Parser::K_DELETE, 0);
}

tree::TerminalNode* MQL_Parser::DeleteTensorsContext::K_FROM() {
  return getToken(MQL_Parser::K_FROM, 0);
}

tree::TerminalNode* MQL_Parser::DeleteTensorsContext::K_TENSOR() {
  return getToken(MQL_Parser::K_TENSOR, 0);
}

tree::TerminalNode* MQL_Parser::DeleteTensorsContext::K_STORE() {
  return getToken(MQL_Parser::K_STORE, 0);
}

tree::TerminalNode* MQL_Parser::DeleteTensorsContext::STRING() {
  return getToken(MQL_Parser::STRING, 0);
}

tree::TerminalNode* MQL_Parser::DeleteTensorsContext::K_OBJECTS() {
  return getToken(MQL_Parser::K_OBJECTS, 0);
}

tree::TerminalNode* MQL_Parser::DeleteTensorsContext::L_PAR() {
  return getToken(MQL_Parser::L_PAR, 0);
}

std::vector<MQL_Parser::IdentifierContext *> MQL_Parser::DeleteTensorsContext::identifier() {
  return getRuleContexts<MQL_Parser::IdentifierContext>();
}

MQL_Parser::IdentifierContext* MQL_Parser::DeleteTensorsContext::identifier(size_t i) {
  return getRuleContext<MQL_Parser::IdentifierContext>(i);
}

tree::TerminalNode* MQL_Parser::DeleteTensorsContext::R_PAR() {
  return getToken(MQL_Parser::R_PAR, 0);
}

std::vector<tree::TerminalNode *> MQL_Parser::DeleteTensorsContext::COMMA() {
  return getTokens(MQL_Parser::COMMA);
}

tree::TerminalNode* MQL_Parser::DeleteTensorsContext::COMMA(size_t i) {
  return getToken(MQL_Parser::COMMA, i);
}


size_t MQL_Parser::DeleteTensorsContext::getRuleIndex() const {
  return MQL_Parser::RuleDeleteTensors;
}


std::any MQL_Parser::DeleteTensorsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitDeleteTensors(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::DeleteTensorsContext* MQL_Parser::deleteTensors() {
  DeleteTensorsContext *_localctx = _tracker.createInstance<DeleteTensorsContext>(_ctx, getState());
  enterRule(_localctx, 22, MQL_Parser::RuleDeleteTensors);
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
    setState(268);
    match(MQL_Parser::K_DELETE);
    setState(269);
    match(MQL_Parser::K_FROM);
    setState(270);
    match(MQL_Parser::K_TENSOR);
    setState(271);
    match(MQL_Parser::K_STORE);
    setState(272);
    match(MQL_Parser::STRING);
    setState(273);
    match(MQL_Parser::K_OBJECTS);
    setState(274);
    match(MQL_Parser::L_PAR);
    setState(275);
    identifier();
    setState(280);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MQL_Parser::COMMA) {
      setState(276);
      match(MQL_Parser::COMMA);
      setState(277);
      identifier();
      setState(282);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(283);
    match(MQL_Parser::R_PAR);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TensorContext ------------------------------------------------------------------

MQL_Parser::TensorContext::TensorContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::TensorContext::LSQUARE_BRACKET() {
  return getToken(MQL_Parser::LSQUARE_BRACKET, 0);
}

std::vector<MQL_Parser::NumericValueContext *> MQL_Parser::TensorContext::numericValue() {
  return getRuleContexts<MQL_Parser::NumericValueContext>();
}

MQL_Parser::NumericValueContext* MQL_Parser::TensorContext::numericValue(size_t i) {
  return getRuleContext<MQL_Parser::NumericValueContext>(i);
}

tree::TerminalNode* MQL_Parser::TensorContext::RSQUARE_BRACKET() {
  return getToken(MQL_Parser::RSQUARE_BRACKET, 0);
}

std::vector<tree::TerminalNode *> MQL_Parser::TensorContext::COMMA() {
  return getTokens(MQL_Parser::COMMA);
}

tree::TerminalNode* MQL_Parser::TensorContext::COMMA(size_t i) {
  return getToken(MQL_Parser::COMMA, i);
}


size_t MQL_Parser::TensorContext::getRuleIndex() const {
  return MQL_Parser::RuleTensor;
}


std::any MQL_Parser::TensorContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitTensor(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::TensorContext* MQL_Parser::tensor() {
  TensorContext *_localctx = _tracker.createInstance<TensorContext>(_ctx, getState());
  enterRule(_localctx, 24, MQL_Parser::RuleTensor);
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
    setState(285);
    match(MQL_Parser::LSQUARE_BRACKET);
    setState(286);
    numericValue();
    setState(291);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MQL_Parser::COMMA) {
      setState(287);
      match(MQL_Parser::COMMA);
      setState(288);
      numericValue();
      setState(293);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(294);
    match(MQL_Parser::RSQUARE_BRACKET);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TensorDistanceReferenceContext ------------------------------------------------------------------

MQL_Parser::TensorDistanceReferenceContext::TensorDistanceReferenceContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

MQL_Parser::ConditionalOrExprContext* MQL_Parser::TensorDistanceReferenceContext::conditionalOrExpr() {
  return getRuleContext<MQL_Parser::ConditionalOrExprContext>(0);
}

MQL_Parser::TensorContext* MQL_Parser::TensorDistanceReferenceContext::tensor() {
  return getRuleContext<MQL_Parser::TensorContext>(0);
}


size_t MQL_Parser::TensorDistanceReferenceContext::getRuleIndex() const {
  return MQL_Parser::RuleTensorDistanceReference;
}


std::any MQL_Parser::TensorDistanceReferenceContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitTensorDistanceReference(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::TensorDistanceReferenceContext* MQL_Parser::tensorDistanceReference() {
  TensorDistanceReferenceContext *_localctx = _tracker.createInstance<TensorDistanceReferenceContext>(_ctx, getState());
  enterRule(_localctx, 26, MQL_Parser::RuleTensorDistanceReference);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(298);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case MQL_Parser::K_ACYCLIC:
      case MQL_Parser::K_AND:
      case MQL_Parser::K_ANGULAR:
      case MQL_Parser::K_ANY:
      case MQL_Parser::K_AS:
      case MQL_Parser::K_AVG:
      case MQL_Parser::K_ALL:
      case MQL_Parser::K_ASC:
      case MQL_Parser::K_BY:
      case MQL_Parser::K_BOOL:
      case MQL_Parser::K_COUNT:
      case MQL_Parser::K_CREATE:
      case MQL_Parser::K_DELETE:
      case MQL_Parser::K_DESCRIBE:
      case MQL_Parser::K_DESC:
      case MQL_Parser::K_DIMENSIONS:
      case MQL_Parser::K_DISTINCT:
      case MQL_Parser::K_EDGE:
      case MQL_Parser::K_EUCLIDEAN:
      case MQL_Parser::K_FROM:
      case MQL_Parser::K_INCOMING:
      case MQL_Parser::K_INDEX:
      case MQL_Parser::K_INSERT:
      case MQL_Parser::K_INTEGER:
      case MQL_Parser::K_INTO:
      case MQL_Parser::K_IS:
      case MQL_Parser::K_FALSE:
      case MQL_Parser::K_FLOAT:
      case MQL_Parser::K_GROUP:
      case MQL_Parser::K_IDENTITY:
      case MQL_Parser::K_LABELS:
      case MQL_Parser::K_LABEL:
      case MQL_Parser::K_LIMIT:
      case MQL_Parser::K_MANHATTAN:
      case MQL_Parser::K_MATCH:
      case MQL_Parser::K_MAX:
      case MQL_Parser::K_MIN:
      case MQL_Parser::K_NFKD_CASEFOLD:
      case MQL_Parser::K_NORMALIZE:
      case MQL_Parser::K_OBJECTS:
      case MQL_Parser::K_OFFSET:
      case MQL_Parser::K_ON:
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
      case MQL_Parser::K_TENSOR_DISTANCE:
      case MQL_Parser::K_TEXT_SEARCH:
      case MQL_Parser::K_REGEX:
      case MQL_Parser::K_RETURN:
      case MQL_Parser::K_SET:
      case MQL_Parser::K_SUM:
      case MQL_Parser::K_STRING:
      case MQL_Parser::K_STORE:
      case MQL_Parser::K_TENSOR:
      case MQL_Parser::K_TEXT:
      case MQL_Parser::K_TRUE:
      case MQL_Parser::K_TOKENIZE:
      case MQL_Parser::K_TRAILS:
      case MQL_Parser::K_VALUES:
      case MQL_Parser::K_WALKS:
      case MQL_Parser::K_WITH:
      case MQL_Parser::K_WHERE:
      case MQL_Parser::K_WS_KEEP_PUNCT:
      case MQL_Parser::K_WS_RM_PUNCT:
      case MQL_Parser::K_WS_SPLIT_PUNCT:
      case MQL_Parser::ANON_ID:
      case MQL_Parser::EDGE_ID:
      case MQL_Parser::VARIABLE:
      case MQL_Parser::STRING:
      case MQL_Parser::UNSIGNED_INTEGER:
      case MQL_Parser::UNSIGNED_FLOAT:
      case MQL_Parser::UNSIGNED_SCIENTIFIC_NOTATION:
      case MQL_Parser::NAME:
      case MQL_Parser::PLUS:
      case MQL_Parser::MINUS:
      case MQL_Parser::L_PAR: {
        enterOuterAlt(_localctx, 1);
        setState(296);
        conditionalOrExpr();
        break;
      }

      case MQL_Parser::LSQUARE_BRACKET: {
        enterOuterAlt(_localctx, 2);
        setState(297);
        tensor();
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

//----------------- MetricTypeContext ------------------------------------------------------------------

MQL_Parser::MetricTypeContext::MetricTypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::MetricTypeContext::K_ANGULAR() {
  return getToken(MQL_Parser::K_ANGULAR, 0);
}

tree::TerminalNode* MQL_Parser::MetricTypeContext::K_EUCLIDEAN() {
  return getToken(MQL_Parser::K_EUCLIDEAN, 0);
}

tree::TerminalNode* MQL_Parser::MetricTypeContext::K_MANHATTAN() {
  return getToken(MQL_Parser::K_MANHATTAN, 0);
}


size_t MQL_Parser::MetricTypeContext::getRuleIndex() const {
  return MQL_Parser::RuleMetricType;
}


std::any MQL_Parser::MetricTypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitMetricType(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::MetricTypeContext* MQL_Parser::metricType() {
  MetricTypeContext *_localctx = _tracker.createInstance<MetricTypeContext>(_ctx, getState());
  enterRule(_localctx, 28, MQL_Parser::RuleMetricType);
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
    setState(300);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 17180393480) != 0))) {
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

//----------------- TextIndexQueryContext ------------------------------------------------------------------

MQL_Parser::TextIndexQueryContext::TextIndexQueryContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

MQL_Parser::CreateTextIndexContext* MQL_Parser::TextIndexQueryContext::createTextIndex() {
  return getRuleContext<MQL_Parser::CreateTextIndexContext>(0);
}


size_t MQL_Parser::TextIndexQueryContext::getRuleIndex() const {
  return MQL_Parser::RuleTextIndexQuery;
}


std::any MQL_Parser::TextIndexQueryContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitTextIndexQuery(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::TextIndexQueryContext* MQL_Parser::textIndexQuery() {
  TextIndexQueryContext *_localctx = _tracker.createInstance<TextIndexQueryContext>(_ctx, getState());
  enterRule(_localctx, 30, MQL_Parser::RuleTextIndexQuery);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(302);
    createTextIndex();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CreateTextIndexContext ------------------------------------------------------------------

MQL_Parser::CreateTextIndexContext::CreateTextIndexContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::CreateTextIndexContext::K_CREATE() {
  return getToken(MQL_Parser::K_CREATE, 0);
}

tree::TerminalNode* MQL_Parser::CreateTextIndexContext::K_TEXT() {
  return getToken(MQL_Parser::K_TEXT, 0);
}

tree::TerminalNode* MQL_Parser::CreateTextIndexContext::K_INDEX() {
  return getToken(MQL_Parser::K_INDEX, 0);
}

tree::TerminalNode* MQL_Parser::CreateTextIndexContext::STRING() {
  return getToken(MQL_Parser::STRING, 0);
}

tree::TerminalNode* MQL_Parser::CreateTextIndexContext::K_ON() {
  return getToken(MQL_Parser::K_ON, 0);
}

MQL_Parser::IdentifierContext* MQL_Parser::CreateTextIndexContext::identifier() {
  return getRuleContext<MQL_Parser::IdentifierContext>(0);
}

tree::TerminalNode* MQL_Parser::CreateTextIndexContext::K_WITH() {
  return getToken(MQL_Parser::K_WITH, 0);
}

MQL_Parser::NormalizeTextIndexContext* MQL_Parser::CreateTextIndexContext::normalizeTextIndex() {
  return getRuleContext<MQL_Parser::NormalizeTextIndexContext>(0);
}

MQL_Parser::TokenizeTextIndexContext* MQL_Parser::CreateTextIndexContext::tokenizeTextIndex() {
  return getRuleContext<MQL_Parser::TokenizeTextIndexContext>(0);
}


size_t MQL_Parser::CreateTextIndexContext::getRuleIndex() const {
  return MQL_Parser::RuleCreateTextIndex;
}


std::any MQL_Parser::CreateTextIndexContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitCreateTextIndex(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::CreateTextIndexContext* MQL_Parser::createTextIndex() {
  CreateTextIndexContext *_localctx = _tracker.createInstance<CreateTextIndexContext>(_ctx, getState());
  enterRule(_localctx, 32, MQL_Parser::RuleCreateTextIndex);
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
    setState(304);
    match(MQL_Parser::K_CREATE);
    setState(305);
    match(MQL_Parser::K_TEXT);
    setState(306);
    match(MQL_Parser::K_INDEX);
    setState(307);
    match(MQL_Parser::STRING);
    setState(308);
    match(MQL_Parser::K_ON);
    setState(309);
    identifier();
    setState(321);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MQL_Parser::K_WITH) {
      setState(310);
      match(MQL_Parser::K_WITH);
      setState(319);
      _errHandler->sync(this);
      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 19, _ctx)) {
      case 1: {
        setState(311);
        normalizeTextIndex();
        break;
      }

      case 2: {
        setState(312);
        tokenizeTextIndex();
        break;
      }

      case 3: {
        setState(313);
        normalizeTextIndex();
        setState(314);
        tokenizeTextIndex();
        break;
      }

      case 4: {
        setState(316);
        tokenizeTextIndex();
        setState(317);
        normalizeTextIndex();
        break;
      }

      default:
        break;
      }
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- NormalizeTextIndexContext ------------------------------------------------------------------

MQL_Parser::NormalizeTextIndexContext::NormalizeTextIndexContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::NormalizeTextIndexContext::K_NORMALIZE() {
  return getToken(MQL_Parser::K_NORMALIZE, 0);
}

MQL_Parser::NormalizeTypeContext* MQL_Parser::NormalizeTextIndexContext::normalizeType() {
  return getRuleContext<MQL_Parser::NormalizeTypeContext>(0);
}


size_t MQL_Parser::NormalizeTextIndexContext::getRuleIndex() const {
  return MQL_Parser::RuleNormalizeTextIndex;
}


std::any MQL_Parser::NormalizeTextIndexContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitNormalizeTextIndex(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::NormalizeTextIndexContext* MQL_Parser::normalizeTextIndex() {
  NormalizeTextIndexContext *_localctx = _tracker.createInstance<NormalizeTextIndexContext>(_ctx, getState());
  enterRule(_localctx, 34, MQL_Parser::RuleNormalizeTextIndex);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(323);
    match(MQL_Parser::K_NORMALIZE);
    setState(324);
    normalizeType();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- NormalizeTypeContext ------------------------------------------------------------------

MQL_Parser::NormalizeTypeContext::NormalizeTypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::NormalizeTypeContext::K_IDENTITY() {
  return getToken(MQL_Parser::K_IDENTITY, 0);
}

tree::TerminalNode* MQL_Parser::NormalizeTypeContext::K_NFKD_CASEFOLD() {
  return getToken(MQL_Parser::K_NFKD_CASEFOLD, 0);
}


size_t MQL_Parser::NormalizeTypeContext::getRuleIndex() const {
  return MQL_Parser::RuleNormalizeType;
}


std::any MQL_Parser::NormalizeTypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitNormalizeType(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::NormalizeTypeContext* MQL_Parser::normalizeType() {
  NormalizeTypeContext *_localctx = _tracker.createInstance<NormalizeTypeContext>(_ctx, getState());
  enterRule(_localctx, 36, MQL_Parser::RuleNormalizeType);
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
    setState(326);
    _la = _input->LA(1);
    if (!(_la == MQL_Parser::K_IDENTITY

    || _la == MQL_Parser::K_NFKD_CASEFOLD)) {
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

//----------------- TokenizeTextIndexContext ------------------------------------------------------------------

MQL_Parser::TokenizeTextIndexContext::TokenizeTextIndexContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::TokenizeTextIndexContext::K_TOKENIZE() {
  return getToken(MQL_Parser::K_TOKENIZE, 0);
}

MQL_Parser::TokenizeTypeContext* MQL_Parser::TokenizeTextIndexContext::tokenizeType() {
  return getRuleContext<MQL_Parser::TokenizeTypeContext>(0);
}


size_t MQL_Parser::TokenizeTextIndexContext::getRuleIndex() const {
  return MQL_Parser::RuleTokenizeTextIndex;
}


std::any MQL_Parser::TokenizeTextIndexContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitTokenizeTextIndex(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::TokenizeTextIndexContext* MQL_Parser::tokenizeTextIndex() {
  TokenizeTextIndexContext *_localctx = _tracker.createInstance<TokenizeTextIndexContext>(_ctx, getState());
  enterRule(_localctx, 38, MQL_Parser::RuleTokenizeTextIndex);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(328);
    match(MQL_Parser::K_TOKENIZE);
    setState(329);
    tokenizeType();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TokenizeTypeContext ------------------------------------------------------------------

MQL_Parser::TokenizeTypeContext::TokenizeTypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::TokenizeTypeContext::K_IDENTITY() {
  return getToken(MQL_Parser::K_IDENTITY, 0);
}

tree::TerminalNode* MQL_Parser::TokenizeTypeContext::K_WS_SPLIT_PUNCT() {
  return getToken(MQL_Parser::K_WS_SPLIT_PUNCT, 0);
}

tree::TerminalNode* MQL_Parser::TokenizeTypeContext::K_WS_RM_PUNCT() {
  return getToken(MQL_Parser::K_WS_RM_PUNCT, 0);
}

tree::TerminalNode* MQL_Parser::TokenizeTypeContext::K_WS_KEEP_PUNCT() {
  return getToken(MQL_Parser::K_WS_KEEP_PUNCT, 0);
}


size_t MQL_Parser::TokenizeTypeContext::getRuleIndex() const {
  return MQL_Parser::RuleTokenizeType;
}


std::any MQL_Parser::TokenizeTypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitTokenizeType(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::TokenizeTypeContext* MQL_Parser::tokenizeType() {
  TokenizeTypeContext *_localctx = _tracker.createInstance<TokenizeTypeContext>(_ctx, getState());
  enterRule(_localctx, 40, MQL_Parser::RuleTokenizeType);
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
    setState(331);
    _la = _input->LA(1);
    if (!(((((_la - 30) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 30)) & 30786325577729) != 0))) {
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

//----------------- ShowQueryContext ------------------------------------------------------------------

MQL_Parser::ShowQueryContext::ShowQueryContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::ShowQueryContext::K_SHOW() {
  return getToken(MQL_Parser::K_SHOW, 0);
}

tree::TerminalNode* MQL_Parser::ShowQueryContext::K_TENSOR() {
  return getToken(MQL_Parser::K_TENSOR, 0);
}

tree::TerminalNode* MQL_Parser::ShowQueryContext::K_STORE() {
  return getToken(MQL_Parser::K_STORE, 0);
}

tree::TerminalNode* MQL_Parser::ShowQueryContext::K_TEXT() {
  return getToken(MQL_Parser::K_TEXT, 0);
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
  enterRule(_localctx, 42, MQL_Parser::RuleShowQuery);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(333);
    match(MQL_Parser::K_SHOW);
    setState(338);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case MQL_Parser::K_TENSOR: {
        setState(334);
        match(MQL_Parser::K_TENSOR);
        setState(335);
        match(MQL_Parser::K_STORE);
        break;
      }

      case MQL_Parser::K_TEXT: {
        setState(336);
        match(MQL_Parser::K_TEXT);
        setState(337);
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
  enterRule(_localctx, 44, MQL_Parser::RuleDescribeQuery);

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
    setState(340);
    match(MQL_Parser::K_DESCRIBE);
    setState(344);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 22, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(341);
        describeFlag(); 
      }
      setState(346);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 22, _ctx);
    }
    setState(347);
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
  enterRule(_localctx, 46, MQL_Parser::RuleDescribeFlag);
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
    setState(349);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 351845870469120) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
    setState(352);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 23, _ctx)) {
    case 1: {
      setState(350);
      match(MQL_Parser::K_LIMIT);
      setState(351);
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

//----------------- SetStatementContext ------------------------------------------------------------------

MQL_Parser::SetStatementContext::SetStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::SetStatementContext::K_SET() {
  return getToken(MQL_Parser::K_SET, 0);
}

std::vector<MQL_Parser::SetItemContext *> MQL_Parser::SetStatementContext::setItem() {
  return getRuleContexts<MQL_Parser::SetItemContext>();
}

MQL_Parser::SetItemContext* MQL_Parser::SetStatementContext::setItem(size_t i) {
  return getRuleContext<MQL_Parser::SetItemContext>(i);
}

std::vector<tree::TerminalNode *> MQL_Parser::SetStatementContext::COMMA() {
  return getTokens(MQL_Parser::COMMA);
}

tree::TerminalNode* MQL_Parser::SetStatementContext::COMMA(size_t i) {
  return getToken(MQL_Parser::COMMA, i);
}


size_t MQL_Parser::SetStatementContext::getRuleIndex() const {
  return MQL_Parser::RuleSetStatement;
}


std::any MQL_Parser::SetStatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitSetStatement(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::SetStatementContext* MQL_Parser::setStatement() {
  SetStatementContext *_localctx = _tracker.createInstance<SetStatementContext>(_ctx, getState());
  enterRule(_localctx, 48, MQL_Parser::RuleSetStatement);
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
    setState(354);
    match(MQL_Parser::K_SET);
    setState(355);
    setItem();
    setState(360);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MQL_Parser::COMMA) {
      setState(356);
      match(MQL_Parser::COMMA);
      setState(357);
      setItem();
      setState(362);
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
  enterRule(_localctx, 50, MQL_Parser::RuleMatchStatement);

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
    match(MQL_Parser::K_MATCH);
    setState(364);
    graphPattern();
   
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
  enterRule(_localctx, 52, MQL_Parser::RuleWhereStatement);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(366);
    match(MQL_Parser::K_WHERE);
    setState(367);
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
  enterRule(_localctx, 54, MQL_Parser::RuleGroupByStatement);
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
    match(MQL_Parser::K_GROUP);
    setState(370);
    match(MQL_Parser::K_BY);
    setState(371);
    groupByItem();
    setState(376);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MQL_Parser::COMMA) {
      setState(372);
      match(MQL_Parser::COMMA);
      setState(373);
      groupByItem();
      setState(378);
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
  enterRule(_localctx, 56, MQL_Parser::RuleOrderByStatement);
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
    match(MQL_Parser::K_ORDER);
    setState(380);
    match(MQL_Parser::K_BY);
    setState(381);
    orderByItem();
    setState(386);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MQL_Parser::COMMA) {
      setState(382);
      match(MQL_Parser::COMMA);
      setState(383);
      orderByItem();
      setState(388);
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
  enterRule(_localctx, 58, MQL_Parser::RuleReturnStatement);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(412);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 32, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<MQL_Parser::ReturnListContext>(_localctx);
      enterOuterAlt(_localctx, 1);
      setState(389);
      match(MQL_Parser::K_RETURN);
      setState(391);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 27, _ctx)) {
      case 1: {
        setState(390);
        match(MQL_Parser::K_DISTINCT);
        break;
      }

      default:
        break;
      }
      setState(393);
      returnItem();
      setState(398);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == MQL_Parser::COMMA) {
        setState(394);
        match(MQL_Parser::COMMA);
        setState(395);
        returnItem();
        setState(400);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(402);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == MQL_Parser::K_LIMIT

      || _la == MQL_Parser::K_OFFSET) {
        setState(401);
        limitOffsetClauses();
      }
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<MQL_Parser::ReturnAllContext>(_localctx);
      enterOuterAlt(_localctx, 2);
      setState(404);
      match(MQL_Parser::K_RETURN);
      setState(406);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == MQL_Parser::K_DISTINCT) {
        setState(405);
        match(MQL_Parser::K_DISTINCT);
      }
      setState(408);
      match(MQL_Parser::STAR);
      setState(410);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == MQL_Parser::K_LIMIT

      || _la == MQL_Parser::K_OFFSET) {
        setState(409);
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
  enterRule(_localctx, 60, MQL_Parser::RuleLimitOffsetClauses);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(422);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case MQL_Parser::K_LIMIT: {
        enterOuterAlt(_localctx, 1);
        setState(414);
        limitClause();
        setState(416);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == MQL_Parser::K_OFFSET) {
          setState(415);
          offsetClause();
        }
        break;
      }

      case MQL_Parser::K_OFFSET: {
        enterOuterAlt(_localctx, 2);
        setState(418);
        offsetClause();
        setState(420);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == MQL_Parser::K_LIMIT) {
          setState(419);
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
  enterRule(_localctx, 62, MQL_Parser::RuleLimitClause);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(424);
    match(MQL_Parser::K_LIMIT);
    setState(425);
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
  enterRule(_localctx, 64, MQL_Parser::RuleOffsetClause);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(427);
    match(MQL_Parser::K_OFFSET);
    setState(428);
    match(MQL_Parser::UNSIGNED_INTEGER);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SetItemContext ------------------------------------------------------------------

MQL_Parser::SetItemContext::SetItemContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::SetItemContext::VARIABLE() {
  return getToken(MQL_Parser::VARIABLE, 0);
}

tree::TerminalNode* MQL_Parser::SetItemContext::SINGLE_EQ() {
  return getToken(MQL_Parser::SINGLE_EQ, 0);
}

MQL_Parser::FixedNodeInsideContext* MQL_Parser::SetItemContext::fixedNodeInside() {
  return getRuleContext<MQL_Parser::FixedNodeInsideContext>(0);
}


size_t MQL_Parser::SetItemContext::getRuleIndex() const {
  return MQL_Parser::RuleSetItem;
}


std::any MQL_Parser::SetItemContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitSetItem(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::SetItemContext* MQL_Parser::setItem() {
  SetItemContext *_localctx = _tracker.createInstance<SetItemContext>(_ctx, getState());
  enterRule(_localctx, 66, MQL_Parser::RuleSetItem);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(430);
    match(MQL_Parser::VARIABLE);
    setState(431);
    match(MQL_Parser::SINGLE_EQ);
    setState(432);
    fixedNodeInside();
   
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
  enterRule(_localctx, 68, MQL_Parser::RuleReturnItem);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(467);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 43, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<MQL_Parser::ReturnItemVarContext>(_localctx);
      enterOuterAlt(_localctx, 1);
      setState(434);
      match(MQL_Parser::VARIABLE);
      setState(436);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == MQL_Parser::KEY) {
        setState(435);
        match(MQL_Parser::KEY);
      }
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<MQL_Parser::ReturnItemAggContext>(_localctx);
      enterOuterAlt(_localctx, 2);
      setState(438);
      aggregateFunc();
      setState(439);
      match(MQL_Parser::L_PAR);
      setState(440);
      match(MQL_Parser::VARIABLE);
      setState(442);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == MQL_Parser::KEY) {
        setState(441);
        match(MQL_Parser::KEY);
      }
      setState(444);
      match(MQL_Parser::R_PAR);
      setState(446);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == MQL_Parser::K_AS) {
        setState(445);
        alias();
      }
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<MQL_Parser::ReturnItemCountContext>(_localctx);
      enterOuterAlt(_localctx, 3);
      setState(448);
      match(MQL_Parser::K_COUNT);
      setState(449);
      match(MQL_Parser::L_PAR);
      setState(451);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == MQL_Parser::K_DISTINCT) {
        setState(450);
        match(MQL_Parser::K_DISTINCT);
      }
      setState(458);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case MQL_Parser::VARIABLE: {
          setState(453);
          match(MQL_Parser::VARIABLE);
          setState(455);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == MQL_Parser::KEY) {
            setState(454);
            match(MQL_Parser::KEY);
          }
          break;
        }

        case MQL_Parser::STAR: {
          setState(457);
          match(MQL_Parser::STAR);
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      setState(460);
      match(MQL_Parser::R_PAR);
      setState(462);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == MQL_Parser::K_AS) {
        setState(461);
        alias();
      }
      break;
    }

    case 4: {
      _localctx = _tracker.createInstance<MQL_Parser::ReturnItemExprContext>(_localctx);
      enterOuterAlt(_localctx, 4);
      setState(464);
      conditionalOrExpr();
      setState(465);
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
  enterRule(_localctx, 70, MQL_Parser::RuleAlias);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(469);
    match(MQL_Parser::K_AS);
    setState(470);
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
  enterRule(_localctx, 72, MQL_Parser::RuleAggregateFunc);
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
    setState(472);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 1152921710765277248) != 0))) {
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
  enterRule(_localctx, 74, MQL_Parser::RuleOrderByItem);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(508);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 52, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<MQL_Parser::OrderByItemVarContext>(_localctx);
      enterOuterAlt(_localctx, 1);
      setState(474);
      match(MQL_Parser::VARIABLE);
      setState(476);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == MQL_Parser::KEY) {
        setState(475);
        match(MQL_Parser::KEY);
      }
      setState(479);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == MQL_Parser::K_ASC

      || _la == MQL_Parser::K_DESC) {
        setState(478);
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
      setState(481);
      aggregateFunc();
      setState(482);
      match(MQL_Parser::L_PAR);
      setState(483);
      match(MQL_Parser::VARIABLE);
      setState(485);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == MQL_Parser::KEY) {
        setState(484);
        match(MQL_Parser::KEY);
      }
      setState(487);
      match(MQL_Parser::R_PAR);
      setState(489);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == MQL_Parser::K_ASC

      || _la == MQL_Parser::K_DESC) {
        setState(488);
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
      setState(491);
      match(MQL_Parser::K_COUNT);
      setState(492);
      match(MQL_Parser::L_PAR);
      setState(494);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == MQL_Parser::K_DISTINCT) {
        setState(493);
        match(MQL_Parser::K_DISTINCT);
      }
      setState(496);
      match(MQL_Parser::VARIABLE);
      setState(498);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == MQL_Parser::KEY) {
        setState(497);
        match(MQL_Parser::KEY);
      }
      setState(500);
      match(MQL_Parser::R_PAR);
      setState(502);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == MQL_Parser::K_ASC

      || _la == MQL_Parser::K_DESC) {
        setState(501);
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
      setState(504);
      conditionalOrExpr();
      setState(506);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == MQL_Parser::K_ASC

      || _la == MQL_Parser::K_DESC) {
        setState(505);
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
  enterRule(_localctx, 76, MQL_Parser::RuleGroupByItem);
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
    setState(510);
    match(MQL_Parser::VARIABLE);
    setState(512);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MQL_Parser::KEY) {
      setState(511);
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
  enterRule(_localctx, 78, MQL_Parser::RuleGraphPattern);
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
    setState(514);
    basicPattern();
    setState(518);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MQL_Parser::K_OPTIONAL) {
      setState(515);
      optionalPattern();
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
  enterRule(_localctx, 80, MQL_Parser::RuleOptionalPattern);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(521);
    match(MQL_Parser::K_OPTIONAL);
    setState(522);
    match(MQL_Parser::LCURLY_BRACKET);
    setState(523);
    graphPattern();
    setState(524);
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
  enterRule(_localctx, 82, MQL_Parser::RuleBasicPattern);
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
    setState(526);
    linearPattern();
    setState(531);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MQL_Parser::COMMA) {
      setState(527);
      match(MQL_Parser::COMMA);
      setState(528);
      linearPattern();
      setState(533);
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
  enterRule(_localctx, 84, MQL_Parser::RuleLinearPattern);
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
    setState(534);
    node();
    setState(543);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (((((_la - 88) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 88)) & 16465) != 0)) {
      setState(537);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case MQL_Parser::LT:
        case MQL_Parser::MINUS: {
          setState(535);
          edge();
          break;
        }

        case MQL_Parser::LEQ:
        case MQL_Parser::SINGLE_EQ: {
          setState(536);
          path();
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      setState(539);
      node();
      setState(545);
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
  enterRule(_localctx, 86, MQL_Parser::RulePath);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(571);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case MQL_Parser::LEQ: {
        enterOuterAlt(_localctx, 1);
        setState(546);
        match(MQL_Parser::LEQ);
        setState(547);
        match(MQL_Parser::LSQUARE_BRACKET);
        setState(549);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == MQL_Parser::K_ANY

        || _la == MQL_Parser::K_ALL) {
          setState(548);
          pathType();
        }
        setState(552);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == MQL_Parser::VARIABLE) {
          setState(551);
          match(MQL_Parser::VARIABLE);
        }
        setState(554);
        pathAlternatives();
        setState(555);
        match(MQL_Parser::RSQUARE_BRACKET);
        setState(556);
        match(MQL_Parser::SINGLE_EQ);
        break;
      }

      case MQL_Parser::SINGLE_EQ: {
        enterOuterAlt(_localctx, 2);
        setState(558);
        match(MQL_Parser::SINGLE_EQ);
        setState(559);
        match(MQL_Parser::LSQUARE_BRACKET);
        setState(561);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == MQL_Parser::K_ANY

        || _la == MQL_Parser::K_ALL) {
          setState(560);
          pathType();
        }
        setState(564);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == MQL_Parser::VARIABLE) {
          setState(563);
          match(MQL_Parser::VARIABLE);
        }
        setState(566);
        pathAlternatives();
        setState(567);
        match(MQL_Parser::RSQUARE_BRACKET);
        setState(568);
        match(MQL_Parser::SINGLE_EQ);
        setState(569);
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
  enterRule(_localctx, 88, MQL_Parser::RulePathAlternatives);
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
    setState(573);
    pathSequence();
    setState(578);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MQL_Parser::PATH_ALTERNATIVE) {
      setState(574);
      match(MQL_Parser::PATH_ALTERNATIVE);
      setState(575);
      pathSequence();
      setState(580);
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
  enterRule(_localctx, 90, MQL_Parser::RulePathSequence);
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
    setState(581);
    pathAtom();
    setState(586);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MQL_Parser::PATH_SEQUENCE) {
      setState(582);
      match(MQL_Parser::PATH_SEQUENCE);
      setState(583);
      pathAtom();
      setState(588);
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
  enterRule(_localctx, 92, MQL_Parser::RulePathAtom);
  size_t _la = 0;

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
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 69, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<MQL_Parser::PathAtomSimpleContext>(_localctx);
      enterOuterAlt(_localctx, 1);
      setState(590);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == MQL_Parser::PATH_NEGATION) {
        setState(589);
        match(MQL_Parser::PATH_NEGATION);
      }
      setState(592);
      match(MQL_Parser::TYPE);
      setState(594);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (((((_la - 98) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 98)) & 141) != 0)) {
        setState(593);
        pathSuffix();
      }
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<MQL_Parser::PathAtomAlternativesContext>(_localctx);
      enterOuterAlt(_localctx, 2);
      setState(597);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == MQL_Parser::PATH_NEGATION) {
        setState(596);
        match(MQL_Parser::PATH_NEGATION);
      }
      setState(599);
      match(MQL_Parser::L_PAR);
      setState(600);
      pathAlternatives();
      setState(601);
      match(MQL_Parser::R_PAR);
      setState(603);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (((((_la - 98) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 98)) & 141) != 0)) {
        setState(602);
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
  enterRule(_localctx, 94, MQL_Parser::RulePathSuffix);

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
    switch (_input->LA(1)) {
      case MQL_Parser::STAR: {
        enterOuterAlt(_localctx, 1);
        setState(607);
        antlrcpp::downCast<PathSuffixContext *>(_localctx)->op = match(MQL_Parser::STAR);
        break;
      }

      case MQL_Parser::PLUS: {
        enterOuterAlt(_localctx, 2);
        setState(608);
        antlrcpp::downCast<PathSuffixContext *>(_localctx)->op = match(MQL_Parser::PLUS);
        break;
      }

      case MQL_Parser::QUESTION_MARK: {
        enterOuterAlt(_localctx, 3);
        setState(609);
        antlrcpp::downCast<PathSuffixContext *>(_localctx)->op = match(MQL_Parser::QUESTION_MARK);
        break;
      }

      case MQL_Parser::LCURLY_BRACKET: {
        enterOuterAlt(_localctx, 4);
        setState(610);
        match(MQL_Parser::LCURLY_BRACKET);
        setState(611);
        antlrcpp::downCast<PathSuffixContext *>(_localctx)->min = match(MQL_Parser::UNSIGNED_INTEGER);
        setState(612);
        match(MQL_Parser::COMMA);
        setState(613);
        antlrcpp::downCast<PathSuffixContext *>(_localctx)->max = match(MQL_Parser::UNSIGNED_INTEGER);
        setState(614);
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
  enterRule(_localctx, 96, MQL_Parser::RulePathType);
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
    setState(617);
    _la = _input->LA(1);
    if (!(_la == MQL_Parser::K_ANY

    || _la == MQL_Parser::K_ALL)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
    setState(619);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MQL_Parser::K_SHORTEST) {
      setState(618);
      match(MQL_Parser::K_SHORTEST);
    }
    setState(622);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MQL_Parser::K_ACYCLIC

    || _la == MQL_Parser::K_SIMPLE || _la == MQL_Parser::K_TRAILS

    || _la == MQL_Parser::K_WALKS) {
      setState(621);
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
  enterRule(_localctx, 98, MQL_Parser::RuleNode);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(626);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 73, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(624);
      fixedNode();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(625);
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
  enterRule(_localctx, 100, MQL_Parser::RuleFixedNode);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(628);
    match(MQL_Parser::L_PAR);
    setState(629);
    fixedNodeInside();
    setState(630);
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
  enterRule(_localctx, 102, MQL_Parser::RuleFixedNodeInside);

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
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 74, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(632);
      identifier();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(633);
      match(MQL_Parser::ANON_ID);
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(634);
      match(MQL_Parser::EDGE_ID);
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(635);
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
  enterRule(_localctx, 104, MQL_Parser::RuleEdge);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(651);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case MQL_Parser::LT: {
        enterOuterAlt(_localctx, 1);
        setState(638);
        match(MQL_Parser::LT);
        setState(641);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 75, _ctx)) {
        case 1: {
          setState(639);
          match(MQL_Parser::MINUS);
          setState(640);
          edgeInside();
          break;
        }

        default:
          break;
        }
        setState(643);
        match(MQL_Parser::MINUS);
        break;
      }

      case MQL_Parser::MINUS: {
        enterOuterAlt(_localctx, 2);
        setState(644);
        match(MQL_Parser::MINUS);
        setState(648);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == MQL_Parser::LSQUARE_BRACKET) {
          setState(645);
          edgeInside();
          setState(646);
          match(MQL_Parser::MINUS);
        }
        setState(650);
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
  enterRule(_localctx, 106, MQL_Parser::RuleEdgeInside);
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
    setState(653);
    match(MQL_Parser::LSQUARE_BRACKET);
    setState(655);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MQL_Parser::EDGE_ID

    || _la == MQL_Parser::VARIABLE) {
      setState(654);
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
    setState(658);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MQL_Parser::TYPE

    || _la == MQL_Parser::TYPE_VAR) {
      setState(657);
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
    setState(661);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MQL_Parser::LCURLY_BRACKET) {
      setState(660);
      properties();
    }
    setState(663);
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
  enterRule(_localctx, 108, MQL_Parser::RuleVarNode);
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
    match(MQL_Parser::L_PAR);
    setState(667);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MQL_Parser::VARIABLE) {
      setState(666);
      match(MQL_Parser::VARIABLE);
    }
    setState(672);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MQL_Parser::TYPE) {
      setState(669);
      match(MQL_Parser::TYPE);
      setState(674);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(676);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MQL_Parser::LCURLY_BRACKET) {
      setState(675);
      properties();
    }
    setState(678);
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
  enterRule(_localctx, 110, MQL_Parser::RuleProperties);
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
    setState(680);
    match(MQL_Parser::LCURLY_BRACKET);
    setState(681);
    property();
    setState(686);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MQL_Parser::COMMA) {
      setState(682);
      match(MQL_Parser::COMMA);
      setState(683);
      property();
      setState(688);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(689);
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
  enterRule(_localctx, 112, MQL_Parser::RuleProperty);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(720);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 88, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<MQL_Parser::Property1Context>(_localctx);
      enterOuterAlt(_localctx, 1);
      setState(691);
      identifier();
      setState(696);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case MQL_Parser::COLON: {
          setState(692);
          match(MQL_Parser::COLON);
          setState(693);
          value();
          break;
        }

        case MQL_Parser::TRUE_PROP: {
          setState(694);
          match(MQL_Parser::TRUE_PROP);
          break;
        }

        case MQL_Parser::FALSE_PROP: {
          setState(695);
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
      setState(698);
      identifier();
      setState(699);
      match(MQL_Parser::TYPE);
      setState(700);
      match(MQL_Parser::L_PAR);
      setState(701);
      match(MQL_Parser::STRING);
      setState(702);
      match(MQL_Parser::R_PAR);
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<MQL_Parser::Property3Context>(_localctx);
      enterOuterAlt(_localctx, 3);
      setState(704);
      identifier();
      setState(705);
      match(MQL_Parser::K_IS);
      setState(707);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == MQL_Parser::K_NOT) {
        setState(706);
        match(MQL_Parser::K_NOT);
      }
      setState(709);
      exprTypename();
      setState(713);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == MQL_Parser::K_OR) {
        setState(710);
        conditionalOrType();
        setState(715);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      break;
    }

    case 4: {
      _localctx = _tracker.createInstance<MQL_Parser::Property4Context>(_localctx);
      enterOuterAlt(_localctx, 4);
      setState(716);
      identifier();

      setState(717);
      antlrcpp::downCast<Property4Context *>(_localctx)->op = _input->LT(1);
      _la = _input->LA(1);
      if (!(((((_la - 88) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 88)) & 63) != 0))) {
        antlrcpp::downCast<Property4Context *>(_localctx)->op = _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(718);
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
  enterRule(_localctx, 114, MQL_Parser::RuleConditionalOrType);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(722);
    match(MQL_Parser::K_OR);
    setState(723);
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
  enterRule(_localctx, 116, MQL_Parser::RuleIdentifier);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(727);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case MQL_Parser::NAME: {
        enterOuterAlt(_localctx, 1);
        setState(725);
        match(MQL_Parser::NAME);
        break;
      }

      case MQL_Parser::K_ACYCLIC:
      case MQL_Parser::K_AND:
      case MQL_Parser::K_ANGULAR:
      case MQL_Parser::K_ANY:
      case MQL_Parser::K_AS:
      case MQL_Parser::K_AVG:
      case MQL_Parser::K_ALL:
      case MQL_Parser::K_ASC:
      case MQL_Parser::K_BY:
      case MQL_Parser::K_BOOL:
      case MQL_Parser::K_COUNT:
      case MQL_Parser::K_CREATE:
      case MQL_Parser::K_DELETE:
      case MQL_Parser::K_DESCRIBE:
      case MQL_Parser::K_DESC:
      case MQL_Parser::K_DIMENSIONS:
      case MQL_Parser::K_DISTINCT:
      case MQL_Parser::K_EDGE:
      case MQL_Parser::K_EUCLIDEAN:
      case MQL_Parser::K_FROM:
      case MQL_Parser::K_INCOMING:
      case MQL_Parser::K_INDEX:
      case MQL_Parser::K_INSERT:
      case MQL_Parser::K_INTEGER:
      case MQL_Parser::K_INTO:
      case MQL_Parser::K_IS:
      case MQL_Parser::K_FLOAT:
      case MQL_Parser::K_GROUP:
      case MQL_Parser::K_IDENTITY:
      case MQL_Parser::K_LABELS:
      case MQL_Parser::K_LABEL:
      case MQL_Parser::K_LIMIT:
      case MQL_Parser::K_MANHATTAN:
      case MQL_Parser::K_MATCH:
      case MQL_Parser::K_MAX:
      case MQL_Parser::K_MIN:
      case MQL_Parser::K_NFKD_CASEFOLD:
      case MQL_Parser::K_NORMALIZE:
      case MQL_Parser::K_OBJECTS:
      case MQL_Parser::K_OFFSET:
      case MQL_Parser::K_ON:
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
      case MQL_Parser::K_TENSOR_DISTANCE:
      case MQL_Parser::K_TEXT_SEARCH:
      case MQL_Parser::K_REGEX:
      case MQL_Parser::K_RETURN:
      case MQL_Parser::K_SET:
      case MQL_Parser::K_SUM:
      case MQL_Parser::K_STRING:
      case MQL_Parser::K_STORE:
      case MQL_Parser::K_TENSOR:
      case MQL_Parser::K_TEXT:
      case MQL_Parser::K_TOKENIZE:
      case MQL_Parser::K_TRAILS:
      case MQL_Parser::K_VALUES:
      case MQL_Parser::K_WALKS:
      case MQL_Parser::K_WITH:
      case MQL_Parser::K_WHERE:
      case MQL_Parser::K_WS_KEEP_PUNCT:
      case MQL_Parser::K_WS_RM_PUNCT:
      case MQL_Parser::K_WS_SPLIT_PUNCT: {
        enterOuterAlt(_localctx, 2);
        setState(726);
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
  enterRule(_localctx, 118, MQL_Parser::RuleBoolValue);
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
    setState(729);
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
  enterRule(_localctx, 120, MQL_Parser::RuleNumericValue);
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
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MQL_Parser::PLUS

    || _la == MQL_Parser::MINUS) {
      setState(731);
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
    setState(734);
    _la = _input->LA(1);
    if (!(((((_la - 84) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 84)) & 7) != 0))) {
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
  enterRule(_localctx, 122, MQL_Parser::RuleDatatypeValue);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(736);
    identifier();
    setState(737);
    match(MQL_Parser::L_PAR);
    setState(738);
    match(MQL_Parser::STRING);
    setState(739);
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
  enterRule(_localctx, 124, MQL_Parser::RuleValue);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(745);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case MQL_Parser::UNSIGNED_INTEGER:
      case MQL_Parser::UNSIGNED_FLOAT:
      case MQL_Parser::UNSIGNED_SCIENTIFIC_NOTATION:
      case MQL_Parser::PLUS:
      case MQL_Parser::MINUS: {
        enterOuterAlt(_localctx, 1);
        setState(741);
        numericValue();
        break;
      }

      case MQL_Parser::STRING: {
        enterOuterAlt(_localctx, 2);
        setState(742);
        match(MQL_Parser::STRING);
        break;
      }

      case MQL_Parser::K_FALSE:
      case MQL_Parser::K_TRUE: {
        enterOuterAlt(_localctx, 3);
        setState(743);
        boolValue();
        break;
      }

      case MQL_Parser::K_ACYCLIC:
      case MQL_Parser::K_AND:
      case MQL_Parser::K_ANGULAR:
      case MQL_Parser::K_ANY:
      case MQL_Parser::K_AS:
      case MQL_Parser::K_AVG:
      case MQL_Parser::K_ALL:
      case MQL_Parser::K_ASC:
      case MQL_Parser::K_BY:
      case MQL_Parser::K_BOOL:
      case MQL_Parser::K_COUNT:
      case MQL_Parser::K_CREATE:
      case MQL_Parser::K_DELETE:
      case MQL_Parser::K_DESCRIBE:
      case MQL_Parser::K_DESC:
      case MQL_Parser::K_DIMENSIONS:
      case MQL_Parser::K_DISTINCT:
      case MQL_Parser::K_EDGE:
      case MQL_Parser::K_EUCLIDEAN:
      case MQL_Parser::K_FROM:
      case MQL_Parser::K_INCOMING:
      case MQL_Parser::K_INDEX:
      case MQL_Parser::K_INSERT:
      case MQL_Parser::K_INTEGER:
      case MQL_Parser::K_INTO:
      case MQL_Parser::K_IS:
      case MQL_Parser::K_FLOAT:
      case MQL_Parser::K_GROUP:
      case MQL_Parser::K_IDENTITY:
      case MQL_Parser::K_LABELS:
      case MQL_Parser::K_LABEL:
      case MQL_Parser::K_LIMIT:
      case MQL_Parser::K_MANHATTAN:
      case MQL_Parser::K_MATCH:
      case MQL_Parser::K_MAX:
      case MQL_Parser::K_MIN:
      case MQL_Parser::K_NFKD_CASEFOLD:
      case MQL_Parser::K_NORMALIZE:
      case MQL_Parser::K_OBJECTS:
      case MQL_Parser::K_OFFSET:
      case MQL_Parser::K_ON:
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
      case MQL_Parser::K_TENSOR_DISTANCE:
      case MQL_Parser::K_TEXT_SEARCH:
      case MQL_Parser::K_REGEX:
      case MQL_Parser::K_RETURN:
      case MQL_Parser::K_SET:
      case MQL_Parser::K_SUM:
      case MQL_Parser::K_STRING:
      case MQL_Parser::K_STORE:
      case MQL_Parser::K_TENSOR:
      case MQL_Parser::K_TEXT:
      case MQL_Parser::K_TOKENIZE:
      case MQL_Parser::K_TRAILS:
      case MQL_Parser::K_VALUES:
      case MQL_Parser::K_WALKS:
      case MQL_Parser::K_WITH:
      case MQL_Parser::K_WHERE:
      case MQL_Parser::K_WS_KEEP_PUNCT:
      case MQL_Parser::K_WS_RM_PUNCT:
      case MQL_Parser::K_WS_SPLIT_PUNCT:
      case MQL_Parser::NAME: {
        enterOuterAlt(_localctx, 4);
        setState(744);
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
  enterRule(_localctx, 126, MQL_Parser::RuleConditionalOrExpr);
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
    setState(747);
    conditionalAndExpr();
    setState(752);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MQL_Parser::K_OR) {
      setState(748);
      match(MQL_Parser::K_OR);
      setState(749);
      conditionalAndExpr();
      setState(754);
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
  enterRule(_localctx, 128, MQL_Parser::RuleConditionalAndExpr);
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
    setState(755);
    comparisonExpr();
    setState(760);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MQL_Parser::K_AND) {
      setState(756);
      match(MQL_Parser::K_AND);
      setState(757);
      comparisonExpr();
      setState(762);
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
  enterRule(_localctx, 130, MQL_Parser::RuleComparisonExpr);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(775);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 96, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<MQL_Parser::ComparisonExprOpContext>(_localctx);
      enterOuterAlt(_localctx, 1);
      setState(763);
      additiveExpr();
      setState(766);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (((((_la - 88) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 88)) & 63) != 0)) {
        setState(764);
        antlrcpp::downCast<ComparisonExprOpContext *>(_localctx)->op = _input->LT(1);
        _la = _input->LA(1);
        if (!(((((_la - 88) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 88)) & 63) != 0))) {
          antlrcpp::downCast<ComparisonExprOpContext *>(_localctx)->op = _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        setState(765);
        additiveExpr();
      }
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<MQL_Parser::ComparisonExprIsContext>(_localctx);
      enterOuterAlt(_localctx, 2);
      setState(768);
      additiveExpr();
      setState(769);
      match(MQL_Parser::K_IS);
      setState(771);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == MQL_Parser::K_NOT) {
        setState(770);
        match(MQL_Parser::K_NOT);
      }
      setState(773);
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
  enterRule(_localctx, 132, MQL_Parser::RuleAdditiveExpr);
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
    setState(777);
    multiplicativeExpr();
    setState(782);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MQL_Parser::PLUS

    || _la == MQL_Parser::MINUS) {
      setState(778);
      antlrcpp::downCast<AdditiveExprContext *>(_localctx)->_tset1478 = _input->LT(1);
      _la = _input->LA(1);
      if (!(_la == MQL_Parser::PLUS

      || _la == MQL_Parser::MINUS)) {
        antlrcpp::downCast<AdditiveExprContext *>(_localctx)->_tset1478 = _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      antlrcpp::downCast<AdditiveExprContext *>(_localctx)->op.push_back(antlrcpp::downCast<AdditiveExprContext *>(_localctx)->_tset1478);
      setState(779);
      multiplicativeExpr();
      setState(784);
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
  enterRule(_localctx, 134, MQL_Parser::RuleMultiplicativeExpr);
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
    setState(785);
    unaryExpr();
    setState(790);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (((((_la - 95) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 95)) & 25) != 0)) {
      setState(786);
      antlrcpp::downCast<MultiplicativeExprContext *>(_localctx)->_tset1497 = _input->LT(1);
      _la = _input->LA(1);
      if (!(((((_la - 95) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 95)) & 25) != 0))) {
        antlrcpp::downCast<MultiplicativeExprContext *>(_localctx)->_tset1497 = _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      antlrcpp::downCast<MultiplicativeExprContext *>(_localctx)->op.push_back(antlrcpp::downCast<MultiplicativeExprContext *>(_localctx)->_tset1497);
      setState(787);
      unaryExpr();
      setState(792);
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
  enterRule(_localctx, 136, MQL_Parser::RuleUnaryExpr);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(800);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 99, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(793);
      match(MQL_Parser::K_NOT);
      setState(794);
      unaryExpr();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(795);
      atomicExpr();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(796);
      match(MQL_Parser::PLUS);
      setState(797);
      unaryExpr();
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(798);
      match(MQL_Parser::MINUS);
      setState(799);
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
  enterRule(_localctx, 138, MQL_Parser::RuleAtomicExpr);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(813);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 101, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<MQL_Parser::ExprVarContext>(_localctx);
      enterOuterAlt(_localctx, 1);
      setState(802);
      match(MQL_Parser::VARIABLE);
      setState(804);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == MQL_Parser::KEY) {
        setState(803);
        match(MQL_Parser::KEY);
      }
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<MQL_Parser::ExprValueContext>(_localctx);
      enterOuterAlt(_localctx, 2);
      setState(806);
      value();
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<MQL_Parser::ExprFixedNodeInsideContext>(_localctx);
      enterOuterAlt(_localctx, 3);
      setState(807);
      fixedNodeInside();
      break;
    }

    case 4: {
      _localctx = _tracker.createInstance<MQL_Parser::ExprParenthesisContext>(_localctx);
      enterOuterAlt(_localctx, 4);
      setState(808);
      match(MQL_Parser::L_PAR);
      setState(809);
      conditionalOrExpr();
      setState(810);
      match(MQL_Parser::R_PAR);
      break;
    }

    case 5: {
      _localctx = _tracker.createInstance<MQL_Parser::ExprFunctionContext>(_localctx);
      enterOuterAlt(_localctx, 5);
      setState(812);
      function();
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

MQL_Parser::TensorDistanceContext* MQL_Parser::FunctionContext::tensorDistance() {
  return getRuleContext<MQL_Parser::TensorDistanceContext>(0);
}

MQL_Parser::TextSearchContext* MQL_Parser::FunctionContext::textSearch() {
  return getRuleContext<MQL_Parser::TextSearchContext>(0);
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
  enterRule(_localctx, 140, MQL_Parser::RuleFunction);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(818);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case MQL_Parser::K_REGEX: {
        enterOuterAlt(_localctx, 1);
        setState(815);
        regex();
        break;
      }

      case MQL_Parser::K_TENSOR_DISTANCE: {
        enterOuterAlt(_localctx, 2);
        setState(816);
        tensorDistance();
        break;
      }

      case MQL_Parser::K_TEXT_SEARCH: {
        enterOuterAlt(_localctx, 3);
        setState(817);
        textSearch();
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
  enterRule(_localctx, 142, MQL_Parser::RuleRegex);
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
    setState(820);
    match(MQL_Parser::K_REGEX);
    setState(821);
    match(MQL_Parser::L_PAR);
    setState(822);
    conditionalOrExpr();
    setState(823);
    match(MQL_Parser::COMMA);
    setState(824);
    conditionalOrExpr();
    setState(827);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MQL_Parser::COMMA) {
      setState(825);
      match(MQL_Parser::COMMA);
      setState(826);
      conditionalOrExpr();
    }
    setState(829);
    match(MQL_Parser::R_PAR);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TensorDistanceContext ------------------------------------------------------------------

MQL_Parser::TensorDistanceContext::TensorDistanceContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::TensorDistanceContext::K_TENSOR_DISTANCE() {
  return getToken(MQL_Parser::K_TENSOR_DISTANCE, 0);
}

tree::TerminalNode* MQL_Parser::TensorDistanceContext::L_PAR() {
  return getToken(MQL_Parser::L_PAR, 0);
}

tree::TerminalNode* MQL_Parser::TensorDistanceContext::STRING() {
  return getToken(MQL_Parser::STRING, 0);
}

std::vector<tree::TerminalNode *> MQL_Parser::TensorDistanceContext::COMMA() {
  return getTokens(MQL_Parser::COMMA);
}

tree::TerminalNode* MQL_Parser::TensorDistanceContext::COMMA(size_t i) {
  return getToken(MQL_Parser::COMMA, i);
}

MQL_Parser::ConditionalOrExprContext* MQL_Parser::TensorDistanceContext::conditionalOrExpr() {
  return getRuleContext<MQL_Parser::ConditionalOrExprContext>(0);
}

MQL_Parser::TensorDistanceReferenceContext* MQL_Parser::TensorDistanceContext::tensorDistanceReference() {
  return getRuleContext<MQL_Parser::TensorDistanceReferenceContext>(0);
}

MQL_Parser::MetricTypeContext* MQL_Parser::TensorDistanceContext::metricType() {
  return getRuleContext<MQL_Parser::MetricTypeContext>(0);
}

tree::TerminalNode* MQL_Parser::TensorDistanceContext::R_PAR() {
  return getToken(MQL_Parser::R_PAR, 0);
}


size_t MQL_Parser::TensorDistanceContext::getRuleIndex() const {
  return MQL_Parser::RuleTensorDistance;
}


std::any MQL_Parser::TensorDistanceContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitTensorDistance(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::TensorDistanceContext* MQL_Parser::tensorDistance() {
  TensorDistanceContext *_localctx = _tracker.createInstance<TensorDistanceContext>(_ctx, getState());
  enterRule(_localctx, 144, MQL_Parser::RuleTensorDistance);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(831);
    match(MQL_Parser::K_TENSOR_DISTANCE);
    setState(832);
    match(MQL_Parser::L_PAR);
    setState(833);
    match(MQL_Parser::STRING);
    setState(834);
    match(MQL_Parser::COMMA);
    setState(835);
    conditionalOrExpr();
    setState(836);
    match(MQL_Parser::COMMA);
    setState(837);
    tensorDistanceReference();
    setState(838);
    match(MQL_Parser::COMMA);
    setState(839);
    metricType();
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

//----------------- TextSearchContext ------------------------------------------------------------------

MQL_Parser::TextSearchContext::TextSearchContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::TextSearchContext::K_TEXT_SEARCH() {
  return getToken(MQL_Parser::K_TEXT_SEARCH, 0);
}

tree::TerminalNode* MQL_Parser::TextSearchContext::L_PAR() {
  return getToken(MQL_Parser::L_PAR, 0);
}

std::vector<tree::TerminalNode *> MQL_Parser::TextSearchContext::STRING() {
  return getTokens(MQL_Parser::STRING);
}

tree::TerminalNode* MQL_Parser::TextSearchContext::STRING(size_t i) {
  return getToken(MQL_Parser::STRING, i);
}

std::vector<tree::TerminalNode *> MQL_Parser::TextSearchContext::COMMA() {
  return getTokens(MQL_Parser::COMMA);
}

tree::TerminalNode* MQL_Parser::TextSearchContext::COMMA(size_t i) {
  return getToken(MQL_Parser::COMMA, i);
}

MQL_Parser::TextSearchIndexModeContext* MQL_Parser::TextSearchContext::textSearchIndexMode() {
  return getRuleContext<MQL_Parser::TextSearchIndexModeContext>(0);
}

std::vector<tree::TerminalNode *> MQL_Parser::TextSearchContext::VARIABLE() {
  return getTokens(MQL_Parser::VARIABLE);
}

tree::TerminalNode* MQL_Parser::TextSearchContext::VARIABLE(size_t i) {
  return getToken(MQL_Parser::VARIABLE, i);
}

tree::TerminalNode* MQL_Parser::TextSearchContext::R_PAR() {
  return getToken(MQL_Parser::R_PAR, 0);
}


size_t MQL_Parser::TextSearchContext::getRuleIndex() const {
  return MQL_Parser::RuleTextSearch;
}


std::any MQL_Parser::TextSearchContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitTextSearch(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::TextSearchContext* MQL_Parser::textSearch() {
  TextSearchContext *_localctx = _tracker.createInstance<TextSearchContext>(_ctx, getState());
  enterRule(_localctx, 146, MQL_Parser::RuleTextSearch);
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
    setState(842);
    match(MQL_Parser::K_TEXT_SEARCH);
    setState(843);
    match(MQL_Parser::L_PAR);
    setState(844);
    match(MQL_Parser::STRING);
    setState(845);
    match(MQL_Parser::COMMA);
    setState(846);
    match(MQL_Parser::STRING);
    setState(847);
    match(MQL_Parser::COMMA);
    setState(848);
    textSearchIndexMode();
    setState(849);
    match(MQL_Parser::COMMA);
    setState(850);
    match(MQL_Parser::VARIABLE);
    setState(853);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MQL_Parser::COMMA) {
      setState(851);
      match(MQL_Parser::COMMA);
      setState(852);
      match(MQL_Parser::VARIABLE);
    }
    setState(855);
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
    setState(857);
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
    setState(859);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 2308094809312592896) != 0))) {
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

tree::TerminalNode* MQL_Parser::KeywordContext::K_ANGULAR() {
  return getToken(MQL_Parser::K_ANGULAR, 0);
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

tree::TerminalNode* MQL_Parser::KeywordContext::K_EUCLIDEAN() {
  return getToken(MQL_Parser::K_EUCLIDEAN, 0);
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

tree::TerminalNode* MQL_Parser::KeywordContext::K_IDENTITY() {
  return getToken(MQL_Parser::K_IDENTITY, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_LABELS() {
  return getToken(MQL_Parser::K_LABELS, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_LABEL() {
  return getToken(MQL_Parser::K_LABEL, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_LIMIT() {
  return getToken(MQL_Parser::K_LIMIT, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_MANHATTAN() {
  return getToken(MQL_Parser::K_MANHATTAN, 0);
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

tree::TerminalNode* MQL_Parser::KeywordContext::K_NFKD_CASEFOLD() {
  return getToken(MQL_Parser::K_NFKD_CASEFOLD, 0);
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

tree::TerminalNode* MQL_Parser::KeywordContext::K_ON() {
  return getToken(MQL_Parser::K_ON, 0);
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

tree::TerminalNode* MQL_Parser::KeywordContext::K_TENSOR_DISTANCE() {
  return getToken(MQL_Parser::K_TENSOR_DISTANCE, 0);
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

tree::TerminalNode* MQL_Parser::KeywordContext::K_SET() {
  return getToken(MQL_Parser::K_SET, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_SUM() {
  return getToken(MQL_Parser::K_SUM, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_STRING() {
  return getToken(MQL_Parser::K_STRING, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_STORE() {
  return getToken(MQL_Parser::K_STORE, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_TENSOR() {
  return getToken(MQL_Parser::K_TENSOR, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_TEXT() {
  return getToken(MQL_Parser::K_TEXT, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_TOKENIZE() {
  return getToken(MQL_Parser::K_TOKENIZE, 0);
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

tree::TerminalNode* MQL_Parser::KeywordContext::K_WS_KEEP_PUNCT() {
  return getToken(MQL_Parser::K_WS_KEEP_PUNCT, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_WS_RM_PUNCT() {
  return getToken(MQL_Parser::K_WS_RM_PUNCT, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_WS_SPLIT_PUNCT() {
  return getToken(MQL_Parser::K_WS_SPLIT_PUNCT, 0);
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
    setState(861);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & -134217730) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 64)) & 2045) != 0))) {
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

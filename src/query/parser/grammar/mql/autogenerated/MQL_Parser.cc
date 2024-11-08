
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
      "insertPlainNodeInside", "insertPlainEdge", "projectSimilarity", "metricType", 
      "describeQuery", "describeFlag", "setStatement", "matchStatement", 
      "whereStatement", "groupByStatement", "orderByStatement", "returnStatement", 
      "limitOffsetClauses", "limitClause", "offsetClause", "setItem", "returnItem", 
      "aggregateFunc", "orderByItem", "groupByItem", "graphPattern", "optionalPattern", 
      "similaritySearch", "bruteSimilaritySearch", "tensor", "basicPattern", 
      "linearPattern", "path", "pathAlternatives", "pathSequence", "pathAtom", 
      "pathSuffix", "pathType", "node", "fixedNode", "fixedNodeInside", 
      "edge", "edgeInside", "varNode", "properties", "property", "conditionalOrType", 
      "identifier", "boolValue", "numericValue", "datatypeValue", "value", 
      "conditionalOrExpr", "conditionalAndExpr", "comparisonExpr", "additiveExpr", 
      "multiplicativeExpr", "unaryExpr", "atomicExpr", "function", "regex", 
      "exprTypename", "keyword"
    },
    std::vector<std::string>{
      "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
      "", "", "", "", "", "'false'", "", "", "", "", "", "", "", "", "", 
      "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
      "", "'true'", "", "", "", "", "", "", "", "", "", "", "", "", "", 
      "", "", "", "'<='", "'>='", "'=='", "'!='", "'<'", "'>'", "'='", "'/'", 
      "'|'", "'^'", "'*'", "'%'", "'\\u003F'", "'+'", "'-'", "'('", "')'", 
      "'{'", "'}'", "'['", "']'", "','", "':'"
    },
    std::vector<std::string>{
      "", "K_ACYCLIC", "K_AND", "K_ANGULAR", "K_ANY", "K_AVG", "K_ALL", 
      "K_ASC", "K_BRUTE_SIMILARITY_SEARCH", "K_BY", "K_BOOL", "K_COUNT", 
      "K_DELETE", "K_DESCRIBE", "K_DESC", "K_DISTINCT", "K_EDGE", "K_EUCLIDEAN", 
      "K_INCOMING", "K_INSERT", "K_INTEGER", "K_IS", "K_FALSE", "K_FLOAT", 
      "K_GROUP", "K_LABELS", "K_LABEL", "K_LIMIT", "K_MANHATTAN", "K_MATCH", 
      "K_MAX", "K_MIN", "K_OFFSET", "K_OPTIONAL", "K_ORDER", "K_OR", "K_OUTGOING", 
      "K_PROJECT_SIMILARITY", "K_PROPERTIES", "K_PROPERTY", "K_NOT", "K_NULL", 
      "K_SHORTEST", "K_SIMPLE", "K_REGEX", "K_RETURN", "K_SET", "K_SIMILARITY_SEARCH", 
      "K_SUM", "K_STRING", "K_TRUE", "K_TRAILS", "K_WALKS", "K_WHERE", "TRUE_PROP", 
      "FALSE_PROP", "ANON_ID", "EDGE_ID", "KEY", "TYPE", "TYPE_VAR", "VARIABLE", 
      "STRING", "UNSIGNED_INTEGER", "UNSIGNED_FLOAT", "UNSIGNED_SCIENTIFIC_NOTATION", 
      "NAME", "LEQ", "GEQ", "EQ", "NEQ", "LT", "GT", "SINGLE_EQ", "PATH_SEQUENCE", 
      "PATH_ALTERNATIVE", "PATH_NEGATION", "STAR", "PERCENT", "QUESTION_MARK", 
      "PLUS", "MINUS", "L_PAR", "R_PAR", "LCURLY_BRACKET", "RCURLY_BRACKET", 
      "LSQUARE_BRACKET", "RSQUARE_BRACKET", "COMMA", "COLON", "WHITE_SPACE", 
      "SINGLE_LINE_COMMENT", "UNRECOGNIZED"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,1,92,765,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,6,2,
  	7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,2,14,7,
  	14,2,15,7,15,2,16,7,16,2,17,7,17,2,18,7,18,2,19,7,19,2,20,7,20,2,21,7,
  	21,2,22,7,22,2,23,7,23,2,24,7,24,2,25,7,25,2,26,7,26,2,27,7,27,2,28,7,
  	28,2,29,7,29,2,30,7,30,2,31,7,31,2,32,7,32,2,33,7,33,2,34,7,34,2,35,7,
  	35,2,36,7,36,2,37,7,37,2,38,7,38,2,39,7,39,2,40,7,40,2,41,7,41,2,42,7,
  	42,2,43,7,43,2,44,7,44,2,45,7,45,2,46,7,46,2,47,7,47,2,48,7,48,2,49,7,
  	49,2,50,7,50,2,51,7,51,2,52,7,52,2,53,7,53,2,54,7,54,2,55,7,55,2,56,7,
  	56,2,57,7,57,2,58,7,58,2,59,7,59,2,60,7,60,2,61,7,61,2,62,7,62,1,0,3,
  	0,128,8,0,1,0,1,0,3,0,132,8,0,1,0,3,0,135,8,0,1,0,1,0,1,1,1,1,1,1,3,1,
  	142,8,1,1,1,3,1,145,8,1,1,1,3,1,148,8,1,1,1,3,1,151,8,1,1,1,1,1,1,2,1,
  	2,1,2,1,2,5,2,159,8,2,10,2,12,2,162,9,2,1,3,1,3,1,3,1,3,5,3,168,8,3,10,
  	3,12,3,171,9,3,1,4,1,4,3,4,175,8,4,1,4,5,4,178,8,4,10,4,12,4,181,9,4,
  	1,4,3,4,184,8,4,1,4,1,4,1,5,1,5,1,6,1,6,1,6,1,6,1,6,3,6,195,8,6,1,6,1,
  	6,1,6,1,6,1,6,1,6,3,6,203,8,6,1,6,1,6,1,6,3,6,208,8,6,1,7,1,7,1,7,1,7,
  	1,7,1,7,1,7,1,7,1,7,1,7,3,7,220,8,7,1,7,1,7,1,7,1,7,1,8,1,8,1,9,1,9,5,
  	9,230,8,9,10,9,12,9,233,9,9,1,9,1,9,1,10,1,10,1,10,3,10,240,8,10,1,11,
  	1,11,1,11,1,11,5,11,246,8,11,10,11,12,11,249,9,11,1,12,1,12,1,12,1,13,
  	1,13,1,13,1,14,1,14,1,14,1,14,1,14,5,14,262,8,14,10,14,12,14,265,9,14,
  	1,15,1,15,1,15,1,15,1,15,5,15,272,8,15,10,15,12,15,275,9,15,1,16,1,16,
  	3,16,279,8,16,1,16,1,16,1,16,5,16,284,8,16,10,16,12,16,287,9,16,1,16,
  	3,16,290,8,16,1,16,1,16,3,16,294,8,16,1,16,1,16,3,16,298,8,16,3,16,300,
  	8,16,1,17,1,17,3,17,304,8,17,1,17,1,17,3,17,308,8,17,3,17,310,8,17,1,
  	18,1,18,1,18,1,19,1,19,1,19,1,20,1,20,1,20,1,20,1,21,1,21,3,21,324,8,
  	21,1,21,1,21,1,21,1,21,3,21,330,8,21,1,21,1,21,1,21,1,21,1,21,3,21,337,
  	8,21,1,21,1,21,3,21,341,8,21,1,21,3,21,344,8,21,1,21,3,21,347,8,21,1,
  	22,1,22,1,23,1,23,3,23,353,8,23,1,23,3,23,356,8,23,1,23,1,23,1,23,1,23,
  	3,23,362,8,23,1,23,1,23,3,23,366,8,23,1,23,1,23,1,23,3,23,371,8,23,1,
  	23,1,23,3,23,375,8,23,1,23,1,23,3,23,379,8,23,3,23,381,8,23,1,24,1,24,
  	3,24,385,8,24,1,25,1,25,5,25,389,8,25,10,25,12,25,392,9,25,1,26,1,26,
  	1,26,1,26,1,26,1,27,1,27,1,27,1,27,1,27,1,27,1,27,1,27,1,27,1,27,3,27,
  	409,8,27,1,27,1,27,3,27,413,8,27,1,27,1,27,1,27,3,27,418,8,27,1,27,3,
  	27,421,8,27,1,27,1,27,1,28,1,28,1,28,1,28,1,28,1,28,1,28,1,28,1,28,1,
  	28,3,28,435,8,28,1,28,1,28,1,28,1,28,3,28,441,8,28,1,28,1,28,1,28,1,29,
  	1,29,1,29,1,29,5,29,450,8,29,10,29,12,29,453,9,29,1,29,1,29,1,30,1,30,
  	1,30,5,30,460,8,30,10,30,12,30,463,9,30,1,31,1,31,1,31,3,31,468,8,31,
  	1,31,1,31,5,31,472,8,31,10,31,12,31,475,9,31,1,31,3,31,478,8,31,1,32,
  	1,32,1,32,3,32,483,8,32,1,32,3,32,486,8,32,1,32,1,32,1,32,1,32,1,32,1,
  	32,1,32,3,32,495,8,32,1,32,3,32,498,8,32,1,32,1,32,1,32,1,32,1,32,3,32,
  	505,8,32,1,33,1,33,1,33,5,33,510,8,33,10,33,12,33,513,9,33,1,34,1,34,
  	1,34,5,34,518,8,34,10,34,12,34,521,9,34,1,35,3,35,524,8,35,1,35,1,35,
  	3,35,528,8,35,1,35,3,35,531,8,35,1,35,1,35,1,35,1,35,3,35,537,8,35,3,
  	35,539,8,35,1,36,1,36,1,36,1,36,1,36,1,36,1,36,1,36,3,36,549,8,36,1,37,
  	1,37,3,37,553,8,37,1,37,3,37,556,8,37,1,38,1,38,3,38,560,8,38,1,39,1,
  	39,1,39,1,39,1,40,1,40,1,40,1,40,3,40,570,8,40,1,41,1,41,1,41,3,41,575,
  	8,41,1,41,1,41,1,41,1,41,1,41,3,41,582,8,41,1,41,3,41,585,8,41,1,42,1,
  	42,3,42,589,8,42,1,42,3,42,592,8,42,1,42,3,42,595,8,42,1,42,1,42,1,43,
  	1,43,3,43,601,8,43,1,43,5,43,604,8,43,10,43,12,43,607,9,43,1,43,3,43,
  	610,8,43,1,43,1,43,1,44,1,44,1,44,1,44,5,44,618,8,44,10,44,12,44,621,
  	9,44,1,44,1,44,1,45,1,45,1,45,1,45,1,45,3,45,630,8,45,1,45,1,45,1,45,
  	1,45,1,45,1,45,1,45,1,45,1,45,3,45,641,8,45,1,45,1,45,5,45,645,8,45,10,
  	45,12,45,648,9,45,1,45,1,45,1,45,1,45,3,45,654,8,45,1,46,1,46,1,46,1,
  	47,1,47,3,47,661,8,47,1,48,1,48,1,49,3,49,666,8,49,1,49,1,49,1,50,1,50,
  	1,50,1,50,1,50,1,51,1,51,1,51,1,51,3,51,679,8,51,1,52,1,52,1,52,5,52,
  	684,8,52,10,52,12,52,687,9,52,1,53,1,53,1,53,5,53,692,8,53,10,53,12,53,
  	695,9,53,1,54,1,54,1,54,3,54,700,8,54,1,54,1,54,1,54,3,54,705,8,54,1,
  	54,1,54,3,54,709,8,54,1,55,1,55,1,55,5,55,714,8,55,10,55,12,55,717,9,
  	55,1,56,1,56,1,56,5,56,722,8,56,10,56,12,56,725,9,56,1,57,1,57,1,57,1,
  	57,1,57,1,57,1,57,3,57,734,8,57,1,58,1,58,3,58,738,8,58,1,58,1,58,1,58,
  	1,58,1,58,1,58,3,58,746,8,58,1,59,1,59,1,60,1,60,1,60,1,60,1,60,1,60,
  	1,60,3,60,757,8,60,1,60,1,60,1,61,1,61,1,62,1,62,1,62,0,0,63,0,2,4,6,
  	8,10,12,14,16,18,20,22,24,26,28,30,32,34,36,38,40,42,44,46,48,50,52,54,
  	56,58,60,62,64,66,68,70,72,74,76,78,80,82,84,86,88,90,92,94,96,98,100,
  	102,104,106,108,110,112,114,116,118,120,122,124,0,15,3,0,3,3,17,17,28,
  	28,4,0,18,18,25,25,36,36,38,38,3,0,5,5,30,31,48,48,2,0,7,7,14,14,1,0,
  	80,81,2,0,4,4,6,6,3,0,1,1,43,43,51,52,2,0,57,57,61,61,1,0,59,60,1,0,67,
  	72,2,0,22,22,50,50,1,0,63,65,2,0,74,74,77,78,5,0,10,10,20,20,23,23,41,
  	41,49,49,6,0,1,7,9,21,23,36,38,43,45,49,51,53,819,0,134,1,0,0,0,2,138,
  	1,0,0,0,4,154,1,0,0,0,6,163,1,0,0,0,8,172,1,0,0,0,10,187,1,0,0,0,12,207,
  	1,0,0,0,14,209,1,0,0,0,16,225,1,0,0,0,18,227,1,0,0,0,20,236,1,0,0,0,22,
  	241,1,0,0,0,24,250,1,0,0,0,26,253,1,0,0,0,28,256,1,0,0,0,30,266,1,0,0,
  	0,32,299,1,0,0,0,34,309,1,0,0,0,36,311,1,0,0,0,38,314,1,0,0,0,40,317,
  	1,0,0,0,42,346,1,0,0,0,44,348,1,0,0,0,46,380,1,0,0,0,48,382,1,0,0,0,50,
  	386,1,0,0,0,52,393,1,0,0,0,54,398,1,0,0,0,56,424,1,0,0,0,58,445,1,0,0,
  	0,60,456,1,0,0,0,62,477,1,0,0,0,64,504,1,0,0,0,66,506,1,0,0,0,68,514,
  	1,0,0,0,70,538,1,0,0,0,72,548,1,0,0,0,74,550,1,0,0,0,76,559,1,0,0,0,78,
  	561,1,0,0,0,80,569,1,0,0,0,82,584,1,0,0,0,84,586,1,0,0,0,86,598,1,0,0,
  	0,88,613,1,0,0,0,90,653,1,0,0,0,92,655,1,0,0,0,94,660,1,0,0,0,96,662,
  	1,0,0,0,98,665,1,0,0,0,100,669,1,0,0,0,102,678,1,0,0,0,104,680,1,0,0,
  	0,106,688,1,0,0,0,108,708,1,0,0,0,110,710,1,0,0,0,112,718,1,0,0,0,114,
  	733,1,0,0,0,116,745,1,0,0,0,118,747,1,0,0,0,120,749,1,0,0,0,122,760,1,
  	0,0,0,124,762,1,0,0,0,126,128,3,22,11,0,127,126,1,0,0,0,127,128,1,0,0,
  	0,128,129,1,0,0,0,129,132,3,2,1,0,130,132,3,18,9,0,131,127,1,0,0,0,131,
  	130,1,0,0,0,132,135,1,0,0,0,133,135,3,4,2,0,134,131,1,0,0,0,134,133,1,
  	0,0,0,135,136,1,0,0,0,136,137,5,0,0,1,137,1,1,0,0,0,138,141,3,24,12,0,
  	139,142,3,14,7,0,140,142,3,56,28,0,141,139,1,0,0,0,141,140,1,0,0,0,141,
  	142,1,0,0,0,142,144,1,0,0,0,143,145,3,26,13,0,144,143,1,0,0,0,144,145,
  	1,0,0,0,145,147,1,0,0,0,146,148,3,28,14,0,147,146,1,0,0,0,147,148,1,0,
  	0,0,148,150,1,0,0,0,149,151,3,30,15,0,150,149,1,0,0,0,150,151,1,0,0,0,
  	151,152,1,0,0,0,152,153,3,32,16,0,153,3,1,0,0,0,154,155,5,19,0,0,155,
  	160,3,6,3,0,156,157,5,88,0,0,157,159,3,6,3,0,158,156,1,0,0,0,159,162,
  	1,0,0,0,160,158,1,0,0,0,160,161,1,0,0,0,161,5,1,0,0,0,162,160,1,0,0,0,
  	163,169,3,8,4,0,164,165,3,12,6,0,165,166,3,8,4,0,166,168,1,0,0,0,167,
  	164,1,0,0,0,168,171,1,0,0,0,169,167,1,0,0,0,169,170,1,0,0,0,170,7,1,0,
  	0,0,171,169,1,0,0,0,172,174,5,82,0,0,173,175,3,10,5,0,174,173,1,0,0,0,
  	174,175,1,0,0,0,175,179,1,0,0,0,176,178,5,59,0,0,177,176,1,0,0,0,178,
  	181,1,0,0,0,179,177,1,0,0,0,179,180,1,0,0,0,180,183,1,0,0,0,181,179,1,
  	0,0,0,182,184,3,88,44,0,183,182,1,0,0,0,183,184,1,0,0,0,184,185,1,0,0,
  	0,185,186,5,83,0,0,186,9,1,0,0,0,187,188,3,94,47,0,188,11,1,0,0,0,189,
  	190,5,71,0,0,190,191,5,81,0,0,191,192,5,86,0,0,192,194,5,59,0,0,193,195,
  	3,88,44,0,194,193,1,0,0,0,194,195,1,0,0,0,195,196,1,0,0,0,196,197,5,87,
  	0,0,197,208,5,81,0,0,198,199,5,81,0,0,199,200,5,86,0,0,200,202,5,59,0,
  	0,201,203,3,88,44,0,202,201,1,0,0,0,202,203,1,0,0,0,203,204,1,0,0,0,204,
  	205,5,87,0,0,205,206,5,81,0,0,206,208,5,72,0,0,207,189,1,0,0,0,207,198,
  	1,0,0,0,208,13,1,0,0,0,209,210,5,37,0,0,210,211,5,82,0,0,211,212,5,61,
  	0,0,212,213,5,88,0,0,213,214,5,61,0,0,214,215,5,88,0,0,215,216,5,62,0,
  	0,216,219,5,88,0,0,217,220,3,80,40,0,218,220,3,58,29,0,219,217,1,0,0,
  	0,219,218,1,0,0,0,220,221,1,0,0,0,221,222,5,88,0,0,222,223,3,16,8,0,223,
  	224,5,83,0,0,224,15,1,0,0,0,225,226,7,0,0,0,226,17,1,0,0,0,227,231,5,
  	13,0,0,228,230,3,20,10,0,229,228,1,0,0,0,230,233,1,0,0,0,231,229,1,0,
  	0,0,231,232,1,0,0,0,232,234,1,0,0,0,233,231,1,0,0,0,234,235,3,80,40,0,
  	235,19,1,0,0,0,236,239,7,1,0,0,237,238,5,27,0,0,238,240,5,63,0,0,239,
  	237,1,0,0,0,239,240,1,0,0,0,240,21,1,0,0,0,241,242,5,46,0,0,242,247,3,
  	40,20,0,243,244,5,88,0,0,244,246,3,40,20,0,245,243,1,0,0,0,246,249,1,
  	0,0,0,247,245,1,0,0,0,247,248,1,0,0,0,248,23,1,0,0,0,249,247,1,0,0,0,
  	250,251,5,29,0,0,251,252,3,50,25,0,252,25,1,0,0,0,253,254,5,53,0,0,254,
  	255,3,104,52,0,255,27,1,0,0,0,256,257,5,24,0,0,257,258,5,9,0,0,258,263,
  	3,48,24,0,259,260,5,88,0,0,260,262,3,48,24,0,261,259,1,0,0,0,262,265,
  	1,0,0,0,263,261,1,0,0,0,263,264,1,0,0,0,264,29,1,0,0,0,265,263,1,0,0,
  	0,266,267,5,34,0,0,267,268,5,9,0,0,268,273,3,46,23,0,269,270,5,88,0,0,
  	270,272,3,46,23,0,271,269,1,0,0,0,272,275,1,0,0,0,273,271,1,0,0,0,273,
  	274,1,0,0,0,274,31,1,0,0,0,275,273,1,0,0,0,276,278,5,45,0,0,277,279,5,
  	15,0,0,278,277,1,0,0,0,278,279,1,0,0,0,279,280,1,0,0,0,280,285,3,42,21,
  	0,281,282,5,88,0,0,282,284,3,42,21,0,283,281,1,0,0,0,284,287,1,0,0,0,
  	285,283,1,0,0,0,285,286,1,0,0,0,286,289,1,0,0,0,287,285,1,0,0,0,288,290,
  	3,34,17,0,289,288,1,0,0,0,289,290,1,0,0,0,290,300,1,0,0,0,291,293,5,45,
  	0,0,292,294,5,15,0,0,293,292,1,0,0,0,293,294,1,0,0,0,294,295,1,0,0,0,
  	295,297,5,77,0,0,296,298,3,34,17,0,297,296,1,0,0,0,297,298,1,0,0,0,298,
  	300,1,0,0,0,299,276,1,0,0,0,299,291,1,0,0,0,300,33,1,0,0,0,301,303,3,
  	36,18,0,302,304,3,38,19,0,303,302,1,0,0,0,303,304,1,0,0,0,304,310,1,0,
  	0,0,305,307,3,38,19,0,306,308,3,36,18,0,307,306,1,0,0,0,307,308,1,0,0,
  	0,308,310,1,0,0,0,309,301,1,0,0,0,309,305,1,0,0,0,310,35,1,0,0,0,311,
  	312,5,27,0,0,312,313,5,63,0,0,313,37,1,0,0,0,314,315,5,32,0,0,315,316,
  	5,63,0,0,316,39,1,0,0,0,317,318,5,61,0,0,318,319,5,73,0,0,319,320,3,80,
  	40,0,320,41,1,0,0,0,321,323,5,61,0,0,322,324,5,58,0,0,323,322,1,0,0,0,
  	323,324,1,0,0,0,324,347,1,0,0,0,325,326,3,44,22,0,326,327,5,82,0,0,327,
  	329,5,61,0,0,328,330,5,58,0,0,329,328,1,0,0,0,329,330,1,0,0,0,330,331,
  	1,0,0,0,331,332,5,83,0,0,332,347,1,0,0,0,333,334,5,11,0,0,334,336,5,82,
  	0,0,335,337,5,15,0,0,336,335,1,0,0,0,336,337,1,0,0,0,337,343,1,0,0,0,
  	338,340,5,61,0,0,339,341,5,58,0,0,340,339,1,0,0,0,340,341,1,0,0,0,341,
  	344,1,0,0,0,342,344,5,77,0,0,343,338,1,0,0,0,343,342,1,0,0,0,344,345,
  	1,0,0,0,345,347,5,83,0,0,346,321,1,0,0,0,346,325,1,0,0,0,346,333,1,0,
  	0,0,347,43,1,0,0,0,348,349,7,2,0,0,349,45,1,0,0,0,350,352,5,61,0,0,351,
  	353,5,58,0,0,352,351,1,0,0,0,352,353,1,0,0,0,353,355,1,0,0,0,354,356,
  	7,3,0,0,355,354,1,0,0,0,355,356,1,0,0,0,356,381,1,0,0,0,357,358,3,44,
  	22,0,358,359,5,82,0,0,359,361,5,61,0,0,360,362,5,58,0,0,361,360,1,0,0,
  	0,361,362,1,0,0,0,362,363,1,0,0,0,363,365,5,83,0,0,364,366,7,3,0,0,365,
  	364,1,0,0,0,365,366,1,0,0,0,366,381,1,0,0,0,367,368,5,11,0,0,368,370,
  	5,82,0,0,369,371,5,15,0,0,370,369,1,0,0,0,370,371,1,0,0,0,371,372,1,0,
  	0,0,372,374,5,61,0,0,373,375,5,58,0,0,374,373,1,0,0,0,374,375,1,0,0,0,
  	375,376,1,0,0,0,376,378,5,83,0,0,377,379,7,3,0,0,378,377,1,0,0,0,378,
  	379,1,0,0,0,379,381,1,0,0,0,380,350,1,0,0,0,380,357,1,0,0,0,380,367,1,
  	0,0,0,381,47,1,0,0,0,382,384,5,61,0,0,383,385,5,58,0,0,384,383,1,0,0,
  	0,384,385,1,0,0,0,385,49,1,0,0,0,386,390,3,60,30,0,387,389,3,52,26,0,
  	388,387,1,0,0,0,389,392,1,0,0,0,390,388,1,0,0,0,390,391,1,0,0,0,391,51,
  	1,0,0,0,392,390,1,0,0,0,393,394,5,33,0,0,394,395,5,84,0,0,395,396,3,50,
  	25,0,396,397,5,85,0,0,397,53,1,0,0,0,398,399,5,47,0,0,399,400,5,82,0,
  	0,400,401,5,61,0,0,401,402,5,88,0,0,402,403,5,61,0,0,403,404,5,88,0,0,
  	404,405,5,62,0,0,405,408,5,88,0,0,406,409,3,80,40,0,407,409,3,58,29,0,
  	408,406,1,0,0,0,408,407,1,0,0,0,409,410,1,0,0,0,410,412,5,88,0,0,411,
  	413,7,4,0,0,412,411,1,0,0,0,412,413,1,0,0,0,413,414,1,0,0,0,414,420,5,
  	63,0,0,415,417,5,88,0,0,416,418,5,80,0,0,417,416,1,0,0,0,417,418,1,0,
  	0,0,418,419,1,0,0,0,419,421,5,63,0,0,420,415,1,0,0,0,420,421,1,0,0,0,
  	421,422,1,0,0,0,422,423,5,83,0,0,423,55,1,0,0,0,424,425,5,8,0,0,425,426,
  	5,82,0,0,426,427,5,61,0,0,427,428,5,88,0,0,428,429,5,61,0,0,429,430,5,
  	88,0,0,430,431,5,62,0,0,431,434,5,88,0,0,432,435,3,80,40,0,433,435,3,
  	58,29,0,434,432,1,0,0,0,434,433,1,0,0,0,435,436,1,0,0,0,436,437,5,88,
  	0,0,437,438,3,16,8,0,438,440,5,88,0,0,439,441,5,80,0,0,440,439,1,0,0,
  	0,440,441,1,0,0,0,441,442,1,0,0,0,442,443,5,63,0,0,443,444,5,83,0,0,444,
  	57,1,0,0,0,445,446,5,86,0,0,446,451,3,98,49,0,447,448,5,88,0,0,448,450,
  	3,98,49,0,449,447,1,0,0,0,450,453,1,0,0,0,451,449,1,0,0,0,451,452,1,0,
  	0,0,452,454,1,0,0,0,453,451,1,0,0,0,454,455,5,87,0,0,455,59,1,0,0,0,456,
  	461,3,62,31,0,457,458,5,88,0,0,458,460,3,62,31,0,459,457,1,0,0,0,460,
  	463,1,0,0,0,461,459,1,0,0,0,461,462,1,0,0,0,462,61,1,0,0,0,463,461,1,
  	0,0,0,464,473,3,76,38,0,465,468,3,82,41,0,466,468,3,64,32,0,467,465,1,
  	0,0,0,467,466,1,0,0,0,468,469,1,0,0,0,469,470,3,76,38,0,470,472,1,0,0,
  	0,471,467,1,0,0,0,472,475,1,0,0,0,473,471,1,0,0,0,473,474,1,0,0,0,474,
  	478,1,0,0,0,475,473,1,0,0,0,476,478,3,54,27,0,477,464,1,0,0,0,477,476,
  	1,0,0,0,478,63,1,0,0,0,479,480,5,67,0,0,480,482,5,86,0,0,481,483,3,74,
  	37,0,482,481,1,0,0,0,482,483,1,0,0,0,483,485,1,0,0,0,484,486,5,61,0,0,
  	485,484,1,0,0,0,485,486,1,0,0,0,486,487,1,0,0,0,487,488,3,66,33,0,488,
  	489,5,87,0,0,489,490,5,73,0,0,490,505,1,0,0,0,491,492,5,73,0,0,492,494,
  	5,86,0,0,493,495,3,74,37,0,494,493,1,0,0,0,494,495,1,0,0,0,495,497,1,
  	0,0,0,496,498,5,61,0,0,497,496,1,0,0,0,497,498,1,0,0,0,498,499,1,0,0,
  	0,499,500,3,66,33,0,500,501,5,87,0,0,501,502,5,73,0,0,502,503,5,72,0,
  	0,503,505,1,0,0,0,504,479,1,0,0,0,504,491,1,0,0,0,505,65,1,0,0,0,506,
  	511,3,68,34,0,507,508,5,75,0,0,508,510,3,68,34,0,509,507,1,0,0,0,510,
  	513,1,0,0,0,511,509,1,0,0,0,511,512,1,0,0,0,512,67,1,0,0,0,513,511,1,
  	0,0,0,514,519,3,70,35,0,515,516,5,74,0,0,516,518,3,70,35,0,517,515,1,
  	0,0,0,518,521,1,0,0,0,519,517,1,0,0,0,519,520,1,0,0,0,520,69,1,0,0,0,
  	521,519,1,0,0,0,522,524,5,76,0,0,523,522,1,0,0,0,523,524,1,0,0,0,524,
  	525,1,0,0,0,525,527,5,59,0,0,526,528,3,72,36,0,527,526,1,0,0,0,527,528,
  	1,0,0,0,528,539,1,0,0,0,529,531,5,76,0,0,530,529,1,0,0,0,530,531,1,0,
  	0,0,531,532,1,0,0,0,532,533,5,82,0,0,533,534,3,66,33,0,534,536,5,83,0,
  	0,535,537,3,72,36,0,536,535,1,0,0,0,536,537,1,0,0,0,537,539,1,0,0,0,538,
  	523,1,0,0,0,538,530,1,0,0,0,539,71,1,0,0,0,540,549,5,77,0,0,541,549,5,
  	80,0,0,542,549,5,79,0,0,543,544,5,84,0,0,544,545,5,63,0,0,545,546,5,88,
  	0,0,546,547,5,63,0,0,547,549,5,85,0,0,548,540,1,0,0,0,548,541,1,0,0,0,
  	548,542,1,0,0,0,548,543,1,0,0,0,549,73,1,0,0,0,550,552,7,5,0,0,551,553,
  	5,42,0,0,552,551,1,0,0,0,552,553,1,0,0,0,553,555,1,0,0,0,554,556,7,6,
  	0,0,555,554,1,0,0,0,555,556,1,0,0,0,556,75,1,0,0,0,557,560,3,78,39,0,
  	558,560,3,86,43,0,559,557,1,0,0,0,559,558,1,0,0,0,560,77,1,0,0,0,561,
  	562,5,82,0,0,562,563,3,80,40,0,563,564,5,83,0,0,564,79,1,0,0,0,565,570,
  	3,94,47,0,566,570,5,56,0,0,567,570,5,57,0,0,568,570,3,102,51,0,569,565,
  	1,0,0,0,569,566,1,0,0,0,569,567,1,0,0,0,569,568,1,0,0,0,570,81,1,0,0,
  	0,571,574,5,71,0,0,572,573,5,81,0,0,573,575,3,84,42,0,574,572,1,0,0,0,
  	574,575,1,0,0,0,575,576,1,0,0,0,576,585,5,81,0,0,577,581,5,81,0,0,578,
  	579,3,84,42,0,579,580,5,81,0,0,580,582,1,0,0,0,581,578,1,0,0,0,581,582,
  	1,0,0,0,582,583,1,0,0,0,583,585,5,72,0,0,584,571,1,0,0,0,584,577,1,0,
  	0,0,585,83,1,0,0,0,586,588,5,86,0,0,587,589,7,7,0,0,588,587,1,0,0,0,588,
  	589,1,0,0,0,589,591,1,0,0,0,590,592,7,8,0,0,591,590,1,0,0,0,591,592,1,
  	0,0,0,592,594,1,0,0,0,593,595,3,88,44,0,594,593,1,0,0,0,594,595,1,0,0,
  	0,595,596,1,0,0,0,596,597,5,87,0,0,597,85,1,0,0,0,598,600,5,82,0,0,599,
  	601,5,61,0,0,600,599,1,0,0,0,600,601,1,0,0,0,601,605,1,0,0,0,602,604,
  	5,59,0,0,603,602,1,0,0,0,604,607,1,0,0,0,605,603,1,0,0,0,605,606,1,0,
  	0,0,606,609,1,0,0,0,607,605,1,0,0,0,608,610,3,88,44,0,609,608,1,0,0,0,
  	609,610,1,0,0,0,610,611,1,0,0,0,611,612,5,83,0,0,612,87,1,0,0,0,613,614,
  	5,84,0,0,614,619,3,90,45,0,615,616,5,88,0,0,616,618,3,90,45,0,617,615,
  	1,0,0,0,618,621,1,0,0,0,619,617,1,0,0,0,619,620,1,0,0,0,620,622,1,0,0,
  	0,621,619,1,0,0,0,622,623,5,85,0,0,623,89,1,0,0,0,624,629,3,94,47,0,625,
  	626,5,89,0,0,626,630,3,102,51,0,627,630,5,54,0,0,628,630,5,55,0,0,629,
  	625,1,0,0,0,629,627,1,0,0,0,629,628,1,0,0,0,630,654,1,0,0,0,631,632,3,
  	94,47,0,632,633,5,59,0,0,633,634,5,82,0,0,634,635,5,62,0,0,635,636,5,
  	83,0,0,636,654,1,0,0,0,637,638,3,94,47,0,638,640,5,21,0,0,639,641,5,40,
  	0,0,640,639,1,0,0,0,640,641,1,0,0,0,641,642,1,0,0,0,642,646,3,122,61,
  	0,643,645,3,92,46,0,644,643,1,0,0,0,645,648,1,0,0,0,646,644,1,0,0,0,646,
  	647,1,0,0,0,647,654,1,0,0,0,648,646,1,0,0,0,649,650,3,94,47,0,650,651,
  	7,9,0,0,651,652,3,102,51,0,652,654,1,0,0,0,653,624,1,0,0,0,653,631,1,
  	0,0,0,653,637,1,0,0,0,653,649,1,0,0,0,654,91,1,0,0,0,655,656,5,35,0,0,
  	656,657,3,122,61,0,657,93,1,0,0,0,658,661,5,66,0,0,659,661,3,124,62,0,
  	660,658,1,0,0,0,660,659,1,0,0,0,661,95,1,0,0,0,662,663,7,10,0,0,663,97,
  	1,0,0,0,664,666,7,4,0,0,665,664,1,0,0,0,665,666,1,0,0,0,666,667,1,0,0,
  	0,667,668,7,11,0,0,668,99,1,0,0,0,669,670,3,94,47,0,670,671,5,82,0,0,
  	671,672,5,62,0,0,672,673,5,83,0,0,673,101,1,0,0,0,674,679,3,98,49,0,675,
  	679,5,62,0,0,676,679,3,96,48,0,677,679,3,100,50,0,678,674,1,0,0,0,678,
  	675,1,0,0,0,678,676,1,0,0,0,678,677,1,0,0,0,679,103,1,0,0,0,680,685,3,
  	106,53,0,681,682,5,35,0,0,682,684,3,106,53,0,683,681,1,0,0,0,684,687,
  	1,0,0,0,685,683,1,0,0,0,685,686,1,0,0,0,686,105,1,0,0,0,687,685,1,0,0,
  	0,688,693,3,108,54,0,689,690,5,2,0,0,690,692,3,108,54,0,691,689,1,0,0,
  	0,692,695,1,0,0,0,693,691,1,0,0,0,693,694,1,0,0,0,694,107,1,0,0,0,695,
  	693,1,0,0,0,696,699,3,110,55,0,697,698,7,9,0,0,698,700,3,110,55,0,699,
  	697,1,0,0,0,699,700,1,0,0,0,700,709,1,0,0,0,701,702,3,110,55,0,702,704,
  	5,21,0,0,703,705,5,40,0,0,704,703,1,0,0,0,704,705,1,0,0,0,705,706,1,0,
  	0,0,706,707,3,122,61,0,707,709,1,0,0,0,708,696,1,0,0,0,708,701,1,0,0,
  	0,709,109,1,0,0,0,710,715,3,112,56,0,711,712,7,4,0,0,712,714,3,112,56,
  	0,713,711,1,0,0,0,714,717,1,0,0,0,715,713,1,0,0,0,715,716,1,0,0,0,716,
  	111,1,0,0,0,717,715,1,0,0,0,718,723,3,114,57,0,719,720,7,12,0,0,720,722,
  	3,114,57,0,721,719,1,0,0,0,722,725,1,0,0,0,723,721,1,0,0,0,723,724,1,
  	0,0,0,724,113,1,0,0,0,725,723,1,0,0,0,726,727,5,40,0,0,727,734,3,114,
  	57,0,728,734,3,116,58,0,729,730,5,80,0,0,730,734,3,114,57,0,731,732,5,
  	81,0,0,732,734,3,114,57,0,733,726,1,0,0,0,733,728,1,0,0,0,733,729,1,0,
  	0,0,733,731,1,0,0,0,734,115,1,0,0,0,735,737,5,61,0,0,736,738,5,58,0,0,
  	737,736,1,0,0,0,737,738,1,0,0,0,738,746,1,0,0,0,739,746,3,102,51,0,740,
  	741,5,82,0,0,741,742,3,104,52,0,742,743,5,83,0,0,743,746,1,0,0,0,744,
  	746,3,118,59,0,745,735,1,0,0,0,745,739,1,0,0,0,745,740,1,0,0,0,745,744,
  	1,0,0,0,746,117,1,0,0,0,747,748,3,120,60,0,748,119,1,0,0,0,749,750,5,
  	44,0,0,750,751,5,82,0,0,751,752,3,104,52,0,752,753,5,88,0,0,753,756,3,
  	104,52,0,754,755,5,88,0,0,755,757,3,104,52,0,756,754,1,0,0,0,756,757,
  	1,0,0,0,757,758,1,0,0,0,758,759,5,83,0,0,759,121,1,0,0,0,760,761,7,13,
  	0,0,761,123,1,0,0,0,762,763,7,14,0,0,763,125,1,0,0,0,102,127,131,134,
  	141,144,147,150,160,169,174,179,183,194,202,207,219,231,239,247,263,273,
  	278,285,289,293,297,299,303,307,309,323,329,336,340,343,346,352,355,361,
  	365,370,374,378,380,384,390,408,412,417,420,434,440,451,461,467,473,477,
  	482,485,494,497,504,511,519,523,527,530,536,538,548,552,555,559,569,574,
  	581,584,588,591,594,600,605,609,619,629,640,646,653,660,665,678,685,693,
  	699,704,708,715,723,733,737,745,756
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
    setState(134);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case MQL_Parser::K_DESCRIBE:
      case MQL_Parser::K_MATCH:
      case MQL_Parser::K_SET: {
        setState(131);
        _errHandler->sync(this);
        switch (_input->LA(1)) {
          case MQL_Parser::K_MATCH:
          case MQL_Parser::K_SET: {
            setState(127);
            _errHandler->sync(this);

            _la = _input->LA(1);
            if (_la == MQL_Parser::K_SET) {
              setState(126);
              setStatement();
            }
            setState(129);
            matchQuery();
            break;
          }

          case MQL_Parser::K_DESCRIBE: {
            setState(130);
            describeQuery();
            break;
          }

        default:
          throw NoViableAltException(this);
        }
        break;
      }

      case MQL_Parser::K_INSERT: {
        setState(133);
        insertPatterns();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    setState(136);
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

MQL_Parser::ProjectSimilarityContext* MQL_Parser::MatchQueryContext::projectSimilarity() {
  return getRuleContext<MQL_Parser::ProjectSimilarityContext>(0);
}

MQL_Parser::BruteSimilaritySearchContext* MQL_Parser::MatchQueryContext::bruteSimilaritySearch() {
  return getRuleContext<MQL_Parser::BruteSimilaritySearchContext>(0);
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
    setState(138);
    matchStatement();
    setState(141);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case MQL_Parser::K_PROJECT_SIMILARITY: {
        setState(139);
        projectSimilarity();
        break;
      }

      case MQL_Parser::K_BRUTE_SIMILARITY_SEARCH: {
        setState(140);
        bruteSimilaritySearch();
        break;
      }

      case MQL_Parser::K_GROUP:
      case MQL_Parser::K_ORDER:
      case MQL_Parser::K_RETURN:
      case MQL_Parser::K_WHERE: {
        break;
      }

    default:
      break;
    }
    setState(144);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MQL_Parser::K_WHERE) {
      setState(143);
      whereStatement();
    }
    setState(147);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MQL_Parser::K_GROUP) {
      setState(146);
      groupByStatement();
    }
    setState(150);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MQL_Parser::K_ORDER) {
      setState(149);
      orderByStatement();
    }
    setState(152);
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
    setState(154);
    match(MQL_Parser::K_INSERT);
    setState(155);
    insertLinearPattern();
    setState(160);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MQL_Parser::COMMA) {
      setState(156);
      match(MQL_Parser::COMMA);
      setState(157);
      insertLinearPattern();
      setState(162);
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
    setState(163);
    insertPlainNode();
    setState(169);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MQL_Parser::LT

    || _la == MQL_Parser::MINUS) {
      setState(164);
      insertPlainEdge();
      setState(165);
      insertPlainNode();
      setState(171);
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
    setState(172);
    match(MQL_Parser::L_PAR);
    setState(174);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 16870768973446910) != 0) || _la == MQL_Parser::NAME) {
      setState(173);
      insertPlainNodeInside();
    }
    setState(179);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MQL_Parser::TYPE) {
      setState(176);
      match(MQL_Parser::TYPE);
      setState(181);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(183);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MQL_Parser::LCURLY_BRACKET) {
      setState(182);
      properties();
    }
    setState(185);
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
    setState(187);
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
    setState(207);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case MQL_Parser::LT: {
        enterOuterAlt(_localctx, 1);
        setState(189);
        match(MQL_Parser::LT);
        setState(190);
        match(MQL_Parser::MINUS);
        setState(191);
        match(MQL_Parser::LSQUARE_BRACKET);
        setState(192);
        match(MQL_Parser::TYPE);
        setState(194);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == MQL_Parser::LCURLY_BRACKET) {
          setState(193);
          properties();
        }
        setState(196);
        match(MQL_Parser::RSQUARE_BRACKET);
        setState(197);
        match(MQL_Parser::MINUS);
        break;
      }

      case MQL_Parser::MINUS: {
        enterOuterAlt(_localctx, 2);
        setState(198);
        match(MQL_Parser::MINUS);
        setState(199);
        match(MQL_Parser::LSQUARE_BRACKET);
        setState(200);
        match(MQL_Parser::TYPE);
        setState(202);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == MQL_Parser::LCURLY_BRACKET) {
          setState(201);
          properties();
        }
        setState(204);
        match(MQL_Parser::RSQUARE_BRACKET);
        setState(205);
        match(MQL_Parser::MINUS);
        setState(206);
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

//----------------- ProjectSimilarityContext ------------------------------------------------------------------

MQL_Parser::ProjectSimilarityContext::ProjectSimilarityContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::ProjectSimilarityContext::K_PROJECT_SIMILARITY() {
  return getToken(MQL_Parser::K_PROJECT_SIMILARITY, 0);
}

tree::TerminalNode* MQL_Parser::ProjectSimilarityContext::L_PAR() {
  return getToken(MQL_Parser::L_PAR, 0);
}

std::vector<tree::TerminalNode *> MQL_Parser::ProjectSimilarityContext::VARIABLE() {
  return getTokens(MQL_Parser::VARIABLE);
}

tree::TerminalNode* MQL_Parser::ProjectSimilarityContext::VARIABLE(size_t i) {
  return getToken(MQL_Parser::VARIABLE, i);
}

std::vector<tree::TerminalNode *> MQL_Parser::ProjectSimilarityContext::COMMA() {
  return getTokens(MQL_Parser::COMMA);
}

tree::TerminalNode* MQL_Parser::ProjectSimilarityContext::COMMA(size_t i) {
  return getToken(MQL_Parser::COMMA, i);
}

tree::TerminalNode* MQL_Parser::ProjectSimilarityContext::STRING() {
  return getToken(MQL_Parser::STRING, 0);
}

MQL_Parser::MetricTypeContext* MQL_Parser::ProjectSimilarityContext::metricType() {
  return getRuleContext<MQL_Parser::MetricTypeContext>(0);
}

tree::TerminalNode* MQL_Parser::ProjectSimilarityContext::R_PAR() {
  return getToken(MQL_Parser::R_PAR, 0);
}

MQL_Parser::FixedNodeInsideContext* MQL_Parser::ProjectSimilarityContext::fixedNodeInside() {
  return getRuleContext<MQL_Parser::FixedNodeInsideContext>(0);
}

MQL_Parser::TensorContext* MQL_Parser::ProjectSimilarityContext::tensor() {
  return getRuleContext<MQL_Parser::TensorContext>(0);
}


size_t MQL_Parser::ProjectSimilarityContext::getRuleIndex() const {
  return MQL_Parser::RuleProjectSimilarity;
}


std::any MQL_Parser::ProjectSimilarityContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitProjectSimilarity(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::ProjectSimilarityContext* MQL_Parser::projectSimilarity() {
  ProjectSimilarityContext *_localctx = _tracker.createInstance<ProjectSimilarityContext>(_ctx, getState());
  enterRule(_localctx, 14, MQL_Parser::RuleProjectSimilarity);

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
    match(MQL_Parser::K_PROJECT_SIMILARITY);
    setState(210);
    match(MQL_Parser::L_PAR);
    setState(211);
    match(MQL_Parser::VARIABLE);
    setState(212);
    match(MQL_Parser::COMMA);
    setState(213);
    match(MQL_Parser::VARIABLE);
    setState(214);
    match(MQL_Parser::COMMA);
    setState(215);
    match(MQL_Parser::STRING);
    setState(216);
    match(MQL_Parser::COMMA);
    setState(219);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case MQL_Parser::K_ACYCLIC:
      case MQL_Parser::K_AND:
      case MQL_Parser::K_ANGULAR:
      case MQL_Parser::K_ANY:
      case MQL_Parser::K_AVG:
      case MQL_Parser::K_ALL:
      case MQL_Parser::K_ASC:
      case MQL_Parser::K_BY:
      case MQL_Parser::K_BOOL:
      case MQL_Parser::K_COUNT:
      case MQL_Parser::K_DELETE:
      case MQL_Parser::K_DESCRIBE:
      case MQL_Parser::K_DESC:
      case MQL_Parser::K_DISTINCT:
      case MQL_Parser::K_EDGE:
      case MQL_Parser::K_EUCLIDEAN:
      case MQL_Parser::K_INCOMING:
      case MQL_Parser::K_INSERT:
      case MQL_Parser::K_INTEGER:
      case MQL_Parser::K_IS:
      case MQL_Parser::K_FALSE:
      case MQL_Parser::K_FLOAT:
      case MQL_Parser::K_GROUP:
      case MQL_Parser::K_LABELS:
      case MQL_Parser::K_LABEL:
      case MQL_Parser::K_LIMIT:
      case MQL_Parser::K_MANHATTAN:
      case MQL_Parser::K_MATCH:
      case MQL_Parser::K_MAX:
      case MQL_Parser::K_MIN:
      case MQL_Parser::K_OFFSET:
      case MQL_Parser::K_OPTIONAL:
      case MQL_Parser::K_ORDER:
      case MQL_Parser::K_OR:
      case MQL_Parser::K_OUTGOING:
      case MQL_Parser::K_PROPERTIES:
      case MQL_Parser::K_PROPERTY:
      case MQL_Parser::K_NOT:
      case MQL_Parser::K_NULL:
      case MQL_Parser::K_SHORTEST:
      case MQL_Parser::K_SIMPLE:
      case MQL_Parser::K_RETURN:
      case MQL_Parser::K_SET:
      case MQL_Parser::K_SIMILARITY_SEARCH:
      case MQL_Parser::K_SUM:
      case MQL_Parser::K_STRING:
      case MQL_Parser::K_TRUE:
      case MQL_Parser::K_TRAILS:
      case MQL_Parser::K_WALKS:
      case MQL_Parser::K_WHERE:
      case MQL_Parser::ANON_ID:
      case MQL_Parser::EDGE_ID:
      case MQL_Parser::STRING:
      case MQL_Parser::UNSIGNED_INTEGER:
      case MQL_Parser::UNSIGNED_FLOAT:
      case MQL_Parser::UNSIGNED_SCIENTIFIC_NOTATION:
      case MQL_Parser::NAME:
      case MQL_Parser::PLUS:
      case MQL_Parser::MINUS: {
        setState(217);
        fixedNodeInside();
        break;
      }

      case MQL_Parser::LSQUARE_BRACKET: {
        setState(218);
        tensor();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    setState(221);
    match(MQL_Parser::COMMA);
    setState(222);
    metricType();
    setState(223);
    match(MQL_Parser::R_PAR);
   
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
  enterRule(_localctx, 16, MQL_Parser::RuleMetricType);
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
    setState(225);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 268566536) != 0))) {
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
  enterRule(_localctx, 18, MQL_Parser::RuleDescribeQuery);

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
    setState(227);
    match(MQL_Parser::K_DESCRIBE);
    setState(231);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 16, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(228);
        describeFlag(); 
      }
      setState(233);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 16, _ctx);
    }
    setState(234);
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
  enterRule(_localctx, 20, MQL_Parser::RuleDescribeFlag);
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
    setState(236);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 343631200256) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
    setState(239);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 17, _ctx)) {
    case 1: {
      setState(237);
      match(MQL_Parser::K_LIMIT);
      setState(238);
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
  enterRule(_localctx, 22, MQL_Parser::RuleSetStatement);
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
    setState(241);
    match(MQL_Parser::K_SET);
    setState(242);
    setItem();
    setState(247);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MQL_Parser::COMMA) {
      setState(243);
      match(MQL_Parser::COMMA);
      setState(244);
      setItem();
      setState(249);
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
  enterRule(_localctx, 24, MQL_Parser::RuleMatchStatement);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(250);
    match(MQL_Parser::K_MATCH);
    setState(251);
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
  enterRule(_localctx, 26, MQL_Parser::RuleWhereStatement);

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
    match(MQL_Parser::K_WHERE);
    setState(254);
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
  enterRule(_localctx, 28, MQL_Parser::RuleGroupByStatement);
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
    setState(256);
    match(MQL_Parser::K_GROUP);
    setState(257);
    match(MQL_Parser::K_BY);
    setState(258);
    groupByItem();
    setState(263);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MQL_Parser::COMMA) {
      setState(259);
      match(MQL_Parser::COMMA);
      setState(260);
      groupByItem();
      setState(265);
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
  enterRule(_localctx, 30, MQL_Parser::RuleOrderByStatement);
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
    setState(266);
    match(MQL_Parser::K_ORDER);
    setState(267);
    match(MQL_Parser::K_BY);
    setState(268);
    orderByItem();
    setState(273);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MQL_Parser::COMMA) {
      setState(269);
      match(MQL_Parser::COMMA);
      setState(270);
      orderByItem();
      setState(275);
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
  enterRule(_localctx, 32, MQL_Parser::RuleReturnStatement);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(299);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 26, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<MQL_Parser::ReturnListContext>(_localctx);
      enterOuterAlt(_localctx, 1);
      setState(276);
      match(MQL_Parser::K_RETURN);
      setState(278);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == MQL_Parser::K_DISTINCT) {
        setState(277);
        match(MQL_Parser::K_DISTINCT);
      }
      setState(280);
      returnItem();
      setState(285);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == MQL_Parser::COMMA) {
        setState(281);
        match(MQL_Parser::COMMA);
        setState(282);
        returnItem();
        setState(287);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(289);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == MQL_Parser::K_LIMIT

      || _la == MQL_Parser::K_OFFSET) {
        setState(288);
        limitOffsetClauses();
      }
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<MQL_Parser::ReturnAllContext>(_localctx);
      enterOuterAlt(_localctx, 2);
      setState(291);
      match(MQL_Parser::K_RETURN);
      setState(293);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == MQL_Parser::K_DISTINCT) {
        setState(292);
        match(MQL_Parser::K_DISTINCT);
      }
      setState(295);
      match(MQL_Parser::STAR);
      setState(297);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == MQL_Parser::K_LIMIT

      || _la == MQL_Parser::K_OFFSET) {
        setState(296);
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
  enterRule(_localctx, 34, MQL_Parser::RuleLimitOffsetClauses);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(309);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case MQL_Parser::K_LIMIT: {
        enterOuterAlt(_localctx, 1);
        setState(301);
        limitClause();
        setState(303);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == MQL_Parser::K_OFFSET) {
          setState(302);
          offsetClause();
        }
        break;
      }

      case MQL_Parser::K_OFFSET: {
        enterOuterAlt(_localctx, 2);
        setState(305);
        offsetClause();
        setState(307);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == MQL_Parser::K_LIMIT) {
          setState(306);
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
  enterRule(_localctx, 36, MQL_Parser::RuleLimitClause);

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
    match(MQL_Parser::K_LIMIT);
    setState(312);
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
  enterRule(_localctx, 38, MQL_Parser::RuleOffsetClause);

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
    match(MQL_Parser::K_OFFSET);
    setState(315);
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
  enterRule(_localctx, 40, MQL_Parser::RuleSetItem);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(317);
    match(MQL_Parser::VARIABLE);
    setState(318);
    match(MQL_Parser::SINGLE_EQ);
    setState(319);
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
  enterRule(_localctx, 42, MQL_Parser::RuleReturnItem);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(346);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case MQL_Parser::VARIABLE: {
        _localctx = _tracker.createInstance<MQL_Parser::ReturnItemVarContext>(_localctx);
        enterOuterAlt(_localctx, 1);
        setState(321);
        match(MQL_Parser::VARIABLE);
        setState(323);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == MQL_Parser::KEY) {
          setState(322);
          match(MQL_Parser::KEY);
        }
        break;
      }

      case MQL_Parser::K_AVG:
      case MQL_Parser::K_MAX:
      case MQL_Parser::K_MIN:
      case MQL_Parser::K_SUM: {
        _localctx = _tracker.createInstance<MQL_Parser::ReturnItemAggContext>(_localctx);
        enterOuterAlt(_localctx, 2);
        setState(325);
        aggregateFunc();
        setState(326);
        match(MQL_Parser::L_PAR);
        setState(327);
        match(MQL_Parser::VARIABLE);
        setState(329);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == MQL_Parser::KEY) {
          setState(328);
          match(MQL_Parser::KEY);
        }
        setState(331);
        match(MQL_Parser::R_PAR);
        break;
      }

      case MQL_Parser::K_COUNT: {
        _localctx = _tracker.createInstance<MQL_Parser::ReturnItemCountContext>(_localctx);
        enterOuterAlt(_localctx, 3);
        setState(333);
        match(MQL_Parser::K_COUNT);
        setState(334);
        match(MQL_Parser::L_PAR);
        setState(336);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == MQL_Parser::K_DISTINCT) {
          setState(335);
          match(MQL_Parser::K_DISTINCT);
        }
        setState(343);
        _errHandler->sync(this);
        switch (_input->LA(1)) {
          case MQL_Parser::VARIABLE: {
            setState(338);
            match(MQL_Parser::VARIABLE);
            setState(340);
            _errHandler->sync(this);

            _la = _input->LA(1);
            if (_la == MQL_Parser::KEY) {
              setState(339);
              match(MQL_Parser::KEY);
            }
            break;
          }

          case MQL_Parser::STAR: {
            setState(342);
            match(MQL_Parser::STAR);
            break;
          }

        default:
          throw NoViableAltException(this);
        }
        setState(345);
        match(MQL_Parser::R_PAR);
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
  enterRule(_localctx, 44, MQL_Parser::RuleAggregateFunc);
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
    setState(348);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 281478197936160) != 0))) {
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
  enterRule(_localctx, 46, MQL_Parser::RuleOrderByItem);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(380);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case MQL_Parser::VARIABLE: {
        _localctx = _tracker.createInstance<MQL_Parser::OrderByItemVarContext>(_localctx);
        enterOuterAlt(_localctx, 1);
        setState(350);
        match(MQL_Parser::VARIABLE);
        setState(352);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == MQL_Parser::KEY) {
          setState(351);
          match(MQL_Parser::KEY);
        }
        setState(355);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == MQL_Parser::K_ASC

        || _la == MQL_Parser::K_DESC) {
          setState(354);
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

      case MQL_Parser::K_AVG:
      case MQL_Parser::K_MAX:
      case MQL_Parser::K_MIN:
      case MQL_Parser::K_SUM: {
        _localctx = _tracker.createInstance<MQL_Parser::OrderByItemAggContext>(_localctx);
        enterOuterAlt(_localctx, 2);
        setState(357);
        aggregateFunc();
        setState(358);
        match(MQL_Parser::L_PAR);
        setState(359);
        match(MQL_Parser::VARIABLE);
        setState(361);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == MQL_Parser::KEY) {
          setState(360);
          match(MQL_Parser::KEY);
        }
        setState(363);
        match(MQL_Parser::R_PAR);
        setState(365);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == MQL_Parser::K_ASC

        || _la == MQL_Parser::K_DESC) {
          setState(364);
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

      case MQL_Parser::K_COUNT: {
        _localctx = _tracker.createInstance<MQL_Parser::OrderByItemCountContext>(_localctx);
        enterOuterAlt(_localctx, 3);
        setState(367);
        match(MQL_Parser::K_COUNT);
        setState(368);
        match(MQL_Parser::L_PAR);
        setState(370);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == MQL_Parser::K_DISTINCT) {
          setState(369);
          match(MQL_Parser::K_DISTINCT);
        }
        setState(372);
        match(MQL_Parser::VARIABLE);
        setState(374);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == MQL_Parser::KEY) {
          setState(373);
          match(MQL_Parser::KEY);
        }
        setState(376);
        match(MQL_Parser::R_PAR);
        setState(378);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == MQL_Parser::K_ASC

        || _la == MQL_Parser::K_DESC) {
          setState(377);
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
  enterRule(_localctx, 48, MQL_Parser::RuleGroupByItem);
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
    setState(382);
    match(MQL_Parser::VARIABLE);
    setState(384);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MQL_Parser::KEY) {
      setState(383);
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
  enterRule(_localctx, 50, MQL_Parser::RuleGraphPattern);
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
    setState(386);
    basicPattern();
    setState(390);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MQL_Parser::K_OPTIONAL) {
      setState(387);
      optionalPattern();
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
  enterRule(_localctx, 52, MQL_Parser::RuleOptionalPattern);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(393);
    match(MQL_Parser::K_OPTIONAL);
    setState(394);
    match(MQL_Parser::LCURLY_BRACKET);
    setState(395);
    graphPattern();
    setState(396);
    match(MQL_Parser::RCURLY_BRACKET);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SimilaritySearchContext ------------------------------------------------------------------

MQL_Parser::SimilaritySearchContext::SimilaritySearchContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::SimilaritySearchContext::K_SIMILARITY_SEARCH() {
  return getToken(MQL_Parser::K_SIMILARITY_SEARCH, 0);
}

tree::TerminalNode* MQL_Parser::SimilaritySearchContext::L_PAR() {
  return getToken(MQL_Parser::L_PAR, 0);
}

std::vector<tree::TerminalNode *> MQL_Parser::SimilaritySearchContext::VARIABLE() {
  return getTokens(MQL_Parser::VARIABLE);
}

tree::TerminalNode* MQL_Parser::SimilaritySearchContext::VARIABLE(size_t i) {
  return getToken(MQL_Parser::VARIABLE, i);
}

std::vector<tree::TerminalNode *> MQL_Parser::SimilaritySearchContext::COMMA() {
  return getTokens(MQL_Parser::COMMA);
}

tree::TerminalNode* MQL_Parser::SimilaritySearchContext::COMMA(size_t i) {
  return getToken(MQL_Parser::COMMA, i);
}

tree::TerminalNode* MQL_Parser::SimilaritySearchContext::STRING() {
  return getToken(MQL_Parser::STRING, 0);
}

std::vector<tree::TerminalNode *> MQL_Parser::SimilaritySearchContext::UNSIGNED_INTEGER() {
  return getTokens(MQL_Parser::UNSIGNED_INTEGER);
}

tree::TerminalNode* MQL_Parser::SimilaritySearchContext::UNSIGNED_INTEGER(size_t i) {
  return getToken(MQL_Parser::UNSIGNED_INTEGER, i);
}

tree::TerminalNode* MQL_Parser::SimilaritySearchContext::R_PAR() {
  return getToken(MQL_Parser::R_PAR, 0);
}

MQL_Parser::FixedNodeInsideContext* MQL_Parser::SimilaritySearchContext::fixedNodeInside() {
  return getRuleContext<MQL_Parser::FixedNodeInsideContext>(0);
}

MQL_Parser::TensorContext* MQL_Parser::SimilaritySearchContext::tensor() {
  return getRuleContext<MQL_Parser::TensorContext>(0);
}

std::vector<tree::TerminalNode *> MQL_Parser::SimilaritySearchContext::PLUS() {
  return getTokens(MQL_Parser::PLUS);
}

tree::TerminalNode* MQL_Parser::SimilaritySearchContext::PLUS(size_t i) {
  return getToken(MQL_Parser::PLUS, i);
}

tree::TerminalNode* MQL_Parser::SimilaritySearchContext::MINUS() {
  return getToken(MQL_Parser::MINUS, 0);
}


size_t MQL_Parser::SimilaritySearchContext::getRuleIndex() const {
  return MQL_Parser::RuleSimilaritySearch;
}


std::any MQL_Parser::SimilaritySearchContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitSimilaritySearch(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::SimilaritySearchContext* MQL_Parser::similaritySearch() {
  SimilaritySearchContext *_localctx = _tracker.createInstance<SimilaritySearchContext>(_ctx, getState());
  enterRule(_localctx, 54, MQL_Parser::RuleSimilaritySearch);
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
    setState(398);
    match(MQL_Parser::K_SIMILARITY_SEARCH);
    setState(399);
    match(MQL_Parser::L_PAR);
    setState(400);
    match(MQL_Parser::VARIABLE);
    setState(401);
    match(MQL_Parser::COMMA);
    setState(402);
    match(MQL_Parser::VARIABLE);
    setState(403);
    match(MQL_Parser::COMMA);
    setState(404);
    match(MQL_Parser::STRING);
    setState(405);
    match(MQL_Parser::COMMA);
    setState(408);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case MQL_Parser::K_ACYCLIC:
      case MQL_Parser::K_AND:
      case MQL_Parser::K_ANGULAR:
      case MQL_Parser::K_ANY:
      case MQL_Parser::K_AVG:
      case MQL_Parser::K_ALL:
      case MQL_Parser::K_ASC:
      case MQL_Parser::K_BY:
      case MQL_Parser::K_BOOL:
      case MQL_Parser::K_COUNT:
      case MQL_Parser::K_DELETE:
      case MQL_Parser::K_DESCRIBE:
      case MQL_Parser::K_DESC:
      case MQL_Parser::K_DISTINCT:
      case MQL_Parser::K_EDGE:
      case MQL_Parser::K_EUCLIDEAN:
      case MQL_Parser::K_INCOMING:
      case MQL_Parser::K_INSERT:
      case MQL_Parser::K_INTEGER:
      case MQL_Parser::K_IS:
      case MQL_Parser::K_FALSE:
      case MQL_Parser::K_FLOAT:
      case MQL_Parser::K_GROUP:
      case MQL_Parser::K_LABELS:
      case MQL_Parser::K_LABEL:
      case MQL_Parser::K_LIMIT:
      case MQL_Parser::K_MANHATTAN:
      case MQL_Parser::K_MATCH:
      case MQL_Parser::K_MAX:
      case MQL_Parser::K_MIN:
      case MQL_Parser::K_OFFSET:
      case MQL_Parser::K_OPTIONAL:
      case MQL_Parser::K_ORDER:
      case MQL_Parser::K_OR:
      case MQL_Parser::K_OUTGOING:
      case MQL_Parser::K_PROPERTIES:
      case MQL_Parser::K_PROPERTY:
      case MQL_Parser::K_NOT:
      case MQL_Parser::K_NULL:
      case MQL_Parser::K_SHORTEST:
      case MQL_Parser::K_SIMPLE:
      case MQL_Parser::K_RETURN:
      case MQL_Parser::K_SET:
      case MQL_Parser::K_SIMILARITY_SEARCH:
      case MQL_Parser::K_SUM:
      case MQL_Parser::K_STRING:
      case MQL_Parser::K_TRUE:
      case MQL_Parser::K_TRAILS:
      case MQL_Parser::K_WALKS:
      case MQL_Parser::K_WHERE:
      case MQL_Parser::ANON_ID:
      case MQL_Parser::EDGE_ID:
      case MQL_Parser::STRING:
      case MQL_Parser::UNSIGNED_INTEGER:
      case MQL_Parser::UNSIGNED_FLOAT:
      case MQL_Parser::UNSIGNED_SCIENTIFIC_NOTATION:
      case MQL_Parser::NAME:
      case MQL_Parser::PLUS:
      case MQL_Parser::MINUS: {
        setState(406);
        fixedNodeInside();
        break;
      }

      case MQL_Parser::LSQUARE_BRACKET: {
        setState(407);
        tensor();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    setState(410);
    match(MQL_Parser::COMMA);
    setState(412);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MQL_Parser::PLUS

    || _la == MQL_Parser::MINUS) {
      setState(411);
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
    setState(414);
    match(MQL_Parser::UNSIGNED_INTEGER);
    setState(420);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MQL_Parser::COMMA) {
      setState(415);
      match(MQL_Parser::COMMA);
      setState(417);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == MQL_Parser::PLUS) {
        setState(416);
        match(MQL_Parser::PLUS);
      }
      setState(419);
      match(MQL_Parser::UNSIGNED_INTEGER);
    }
    setState(422);
    match(MQL_Parser::R_PAR);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BruteSimilaritySearchContext ------------------------------------------------------------------

MQL_Parser::BruteSimilaritySearchContext::BruteSimilaritySearchContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::BruteSimilaritySearchContext::K_BRUTE_SIMILARITY_SEARCH() {
  return getToken(MQL_Parser::K_BRUTE_SIMILARITY_SEARCH, 0);
}

tree::TerminalNode* MQL_Parser::BruteSimilaritySearchContext::L_PAR() {
  return getToken(MQL_Parser::L_PAR, 0);
}

std::vector<tree::TerminalNode *> MQL_Parser::BruteSimilaritySearchContext::VARIABLE() {
  return getTokens(MQL_Parser::VARIABLE);
}

tree::TerminalNode* MQL_Parser::BruteSimilaritySearchContext::VARIABLE(size_t i) {
  return getToken(MQL_Parser::VARIABLE, i);
}

std::vector<tree::TerminalNode *> MQL_Parser::BruteSimilaritySearchContext::COMMA() {
  return getTokens(MQL_Parser::COMMA);
}

tree::TerminalNode* MQL_Parser::BruteSimilaritySearchContext::COMMA(size_t i) {
  return getToken(MQL_Parser::COMMA, i);
}

tree::TerminalNode* MQL_Parser::BruteSimilaritySearchContext::STRING() {
  return getToken(MQL_Parser::STRING, 0);
}

MQL_Parser::MetricTypeContext* MQL_Parser::BruteSimilaritySearchContext::metricType() {
  return getRuleContext<MQL_Parser::MetricTypeContext>(0);
}

tree::TerminalNode* MQL_Parser::BruteSimilaritySearchContext::UNSIGNED_INTEGER() {
  return getToken(MQL_Parser::UNSIGNED_INTEGER, 0);
}

tree::TerminalNode* MQL_Parser::BruteSimilaritySearchContext::R_PAR() {
  return getToken(MQL_Parser::R_PAR, 0);
}

MQL_Parser::FixedNodeInsideContext* MQL_Parser::BruteSimilaritySearchContext::fixedNodeInside() {
  return getRuleContext<MQL_Parser::FixedNodeInsideContext>(0);
}

MQL_Parser::TensorContext* MQL_Parser::BruteSimilaritySearchContext::tensor() {
  return getRuleContext<MQL_Parser::TensorContext>(0);
}

tree::TerminalNode* MQL_Parser::BruteSimilaritySearchContext::PLUS() {
  return getToken(MQL_Parser::PLUS, 0);
}


size_t MQL_Parser::BruteSimilaritySearchContext::getRuleIndex() const {
  return MQL_Parser::RuleBruteSimilaritySearch;
}


std::any MQL_Parser::BruteSimilaritySearchContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitBruteSimilaritySearch(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::BruteSimilaritySearchContext* MQL_Parser::bruteSimilaritySearch() {
  BruteSimilaritySearchContext *_localctx = _tracker.createInstance<BruteSimilaritySearchContext>(_ctx, getState());
  enterRule(_localctx, 56, MQL_Parser::RuleBruteSimilaritySearch);
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
    setState(424);
    match(MQL_Parser::K_BRUTE_SIMILARITY_SEARCH);
    setState(425);
    match(MQL_Parser::L_PAR);
    setState(426);
    match(MQL_Parser::VARIABLE);
    setState(427);
    match(MQL_Parser::COMMA);
    setState(428);
    match(MQL_Parser::VARIABLE);
    setState(429);
    match(MQL_Parser::COMMA);
    setState(430);
    match(MQL_Parser::STRING);
    setState(431);
    match(MQL_Parser::COMMA);
    setState(434);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case MQL_Parser::K_ACYCLIC:
      case MQL_Parser::K_AND:
      case MQL_Parser::K_ANGULAR:
      case MQL_Parser::K_ANY:
      case MQL_Parser::K_AVG:
      case MQL_Parser::K_ALL:
      case MQL_Parser::K_ASC:
      case MQL_Parser::K_BY:
      case MQL_Parser::K_BOOL:
      case MQL_Parser::K_COUNT:
      case MQL_Parser::K_DELETE:
      case MQL_Parser::K_DESCRIBE:
      case MQL_Parser::K_DESC:
      case MQL_Parser::K_DISTINCT:
      case MQL_Parser::K_EDGE:
      case MQL_Parser::K_EUCLIDEAN:
      case MQL_Parser::K_INCOMING:
      case MQL_Parser::K_INSERT:
      case MQL_Parser::K_INTEGER:
      case MQL_Parser::K_IS:
      case MQL_Parser::K_FALSE:
      case MQL_Parser::K_FLOAT:
      case MQL_Parser::K_GROUP:
      case MQL_Parser::K_LABELS:
      case MQL_Parser::K_LABEL:
      case MQL_Parser::K_LIMIT:
      case MQL_Parser::K_MANHATTAN:
      case MQL_Parser::K_MATCH:
      case MQL_Parser::K_MAX:
      case MQL_Parser::K_MIN:
      case MQL_Parser::K_OFFSET:
      case MQL_Parser::K_OPTIONAL:
      case MQL_Parser::K_ORDER:
      case MQL_Parser::K_OR:
      case MQL_Parser::K_OUTGOING:
      case MQL_Parser::K_PROPERTIES:
      case MQL_Parser::K_PROPERTY:
      case MQL_Parser::K_NOT:
      case MQL_Parser::K_NULL:
      case MQL_Parser::K_SHORTEST:
      case MQL_Parser::K_SIMPLE:
      case MQL_Parser::K_RETURN:
      case MQL_Parser::K_SET:
      case MQL_Parser::K_SIMILARITY_SEARCH:
      case MQL_Parser::K_SUM:
      case MQL_Parser::K_STRING:
      case MQL_Parser::K_TRUE:
      case MQL_Parser::K_TRAILS:
      case MQL_Parser::K_WALKS:
      case MQL_Parser::K_WHERE:
      case MQL_Parser::ANON_ID:
      case MQL_Parser::EDGE_ID:
      case MQL_Parser::STRING:
      case MQL_Parser::UNSIGNED_INTEGER:
      case MQL_Parser::UNSIGNED_FLOAT:
      case MQL_Parser::UNSIGNED_SCIENTIFIC_NOTATION:
      case MQL_Parser::NAME:
      case MQL_Parser::PLUS:
      case MQL_Parser::MINUS: {
        setState(432);
        fixedNodeInside();
        break;
      }

      case MQL_Parser::LSQUARE_BRACKET: {
        setState(433);
        tensor();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    setState(436);
    match(MQL_Parser::COMMA);
    setState(437);
    metricType();
    setState(438);
    match(MQL_Parser::COMMA);
    setState(440);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MQL_Parser::PLUS) {
      setState(439);
      match(MQL_Parser::PLUS);
    }
    setState(442);
    match(MQL_Parser::UNSIGNED_INTEGER);
    setState(443);
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
  enterRule(_localctx, 58, MQL_Parser::RuleTensor);
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
    setState(445);
    match(MQL_Parser::LSQUARE_BRACKET);
    setState(446);
    numericValue();
    setState(451);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MQL_Parser::COMMA) {
      setState(447);
      match(MQL_Parser::COMMA);
      setState(448);
      numericValue();
      setState(453);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(454);
    match(MQL_Parser::RSQUARE_BRACKET);
   
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
  enterRule(_localctx, 60, MQL_Parser::RuleBasicPattern);
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
    setState(456);
    linearPattern();
    setState(461);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MQL_Parser::COMMA) {
      setState(457);
      match(MQL_Parser::COMMA);
      setState(458);
      linearPattern();
      setState(463);
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

MQL_Parser::SimilaritySearchContext* MQL_Parser::LinearPatternContext::similaritySearch() {
  return getRuleContext<MQL_Parser::SimilaritySearchContext>(0);
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
  enterRule(_localctx, 62, MQL_Parser::RuleLinearPattern);
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
    switch (_input->LA(1)) {
      case MQL_Parser::L_PAR: {
        enterOuterAlt(_localctx, 1);
        setState(464);
        node();
        setState(473);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (((((_la - 67) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 67)) & 16465) != 0)) {
          setState(467);
          _errHandler->sync(this);
          switch (_input->LA(1)) {
            case MQL_Parser::LT:
            case MQL_Parser::MINUS: {
              setState(465);
              edge();
              break;
            }

            case MQL_Parser::LEQ:
            case MQL_Parser::SINGLE_EQ: {
              setState(466);
              path();
              break;
            }

          default:
            throw NoViableAltException(this);
          }
          setState(469);
          node();
          setState(475);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
        break;
      }

      case MQL_Parser::K_SIMILARITY_SEARCH: {
        enterOuterAlt(_localctx, 2);
        setState(476);
        similaritySearch();
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
  enterRule(_localctx, 64, MQL_Parser::RulePath);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(504);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case MQL_Parser::LEQ: {
        enterOuterAlt(_localctx, 1);
        setState(479);
        match(MQL_Parser::LEQ);
        setState(480);
        match(MQL_Parser::LSQUARE_BRACKET);
        setState(482);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == MQL_Parser::K_ANY

        || _la == MQL_Parser::K_ALL) {
          setState(481);
          pathType();
        }
        setState(485);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == MQL_Parser::VARIABLE) {
          setState(484);
          match(MQL_Parser::VARIABLE);
        }
        setState(487);
        pathAlternatives();
        setState(488);
        match(MQL_Parser::RSQUARE_BRACKET);
        setState(489);
        match(MQL_Parser::SINGLE_EQ);
        break;
      }

      case MQL_Parser::SINGLE_EQ: {
        enterOuterAlt(_localctx, 2);
        setState(491);
        match(MQL_Parser::SINGLE_EQ);
        setState(492);
        match(MQL_Parser::LSQUARE_BRACKET);
        setState(494);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == MQL_Parser::K_ANY

        || _la == MQL_Parser::K_ALL) {
          setState(493);
          pathType();
        }
        setState(497);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == MQL_Parser::VARIABLE) {
          setState(496);
          match(MQL_Parser::VARIABLE);
        }
        setState(499);
        pathAlternatives();
        setState(500);
        match(MQL_Parser::RSQUARE_BRACKET);
        setState(501);
        match(MQL_Parser::SINGLE_EQ);
        setState(502);
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
  enterRule(_localctx, 66, MQL_Parser::RulePathAlternatives);
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
    setState(506);
    pathSequence();
    setState(511);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MQL_Parser::PATH_ALTERNATIVE) {
      setState(507);
      match(MQL_Parser::PATH_ALTERNATIVE);
      setState(508);
      pathSequence();
      setState(513);
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
  enterRule(_localctx, 68, MQL_Parser::RulePathSequence);
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
    pathAtom();
    setState(519);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MQL_Parser::PATH_SEQUENCE) {
      setState(515);
      match(MQL_Parser::PATH_SEQUENCE);
      setState(516);
      pathAtom();
      setState(521);
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
  enterRule(_localctx, 70, MQL_Parser::RulePathAtom);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(538);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 68, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<MQL_Parser::PathAtomSimpleContext>(_localctx);
      enterOuterAlt(_localctx, 1);
      setState(523);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == MQL_Parser::PATH_NEGATION) {
        setState(522);
        match(MQL_Parser::PATH_NEGATION);
      }
      setState(525);
      match(MQL_Parser::TYPE);
      setState(527);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (((((_la - 77) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 77)) & 141) != 0)) {
        setState(526);
        pathSuffix();
      }
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<MQL_Parser::PathAtomAlternativesContext>(_localctx);
      enterOuterAlt(_localctx, 2);
      setState(530);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == MQL_Parser::PATH_NEGATION) {
        setState(529);
        match(MQL_Parser::PATH_NEGATION);
      }
      setState(532);
      match(MQL_Parser::L_PAR);
      setState(533);
      pathAlternatives();
      setState(534);
      match(MQL_Parser::R_PAR);
      setState(536);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (((((_la - 77) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 77)) & 141) != 0)) {
        setState(535);
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
  enterRule(_localctx, 72, MQL_Parser::RulePathSuffix);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(548);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case MQL_Parser::STAR: {
        enterOuterAlt(_localctx, 1);
        setState(540);
        antlrcpp::downCast<PathSuffixContext *>(_localctx)->op = match(MQL_Parser::STAR);
        break;
      }

      case MQL_Parser::PLUS: {
        enterOuterAlt(_localctx, 2);
        setState(541);
        antlrcpp::downCast<PathSuffixContext *>(_localctx)->op = match(MQL_Parser::PLUS);
        break;
      }

      case MQL_Parser::QUESTION_MARK: {
        enterOuterAlt(_localctx, 3);
        setState(542);
        antlrcpp::downCast<PathSuffixContext *>(_localctx)->op = match(MQL_Parser::QUESTION_MARK);
        break;
      }

      case MQL_Parser::LCURLY_BRACKET: {
        enterOuterAlt(_localctx, 4);
        setState(543);
        match(MQL_Parser::LCURLY_BRACKET);
        setState(544);
        antlrcpp::downCast<PathSuffixContext *>(_localctx)->min = match(MQL_Parser::UNSIGNED_INTEGER);
        setState(545);
        match(MQL_Parser::COMMA);
        setState(546);
        antlrcpp::downCast<PathSuffixContext *>(_localctx)->max = match(MQL_Parser::UNSIGNED_INTEGER);
        setState(547);
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
  enterRule(_localctx, 74, MQL_Parser::RulePathType);
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
    _la = _input->LA(1);
    if (!(_la == MQL_Parser::K_ANY

    || _la == MQL_Parser::K_ALL)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
    setState(552);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MQL_Parser::K_SHORTEST) {
      setState(551);
      match(MQL_Parser::K_SHORTEST);
    }
    setState(555);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 6764195534077954) != 0)) {
      setState(554);
      _la = _input->LA(1);
      if (!((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & 6764195534077954) != 0))) {
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
  enterRule(_localctx, 76, MQL_Parser::RuleNode);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(559);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 72, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(557);
      fixedNode();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(558);
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
  enterRule(_localctx, 78, MQL_Parser::RuleFixedNode);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(561);
    match(MQL_Parser::L_PAR);
    setState(562);
    fixedNodeInside();
    setState(563);
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
  enterRule(_localctx, 80, MQL_Parser::RuleFixedNodeInside);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(569);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 73, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(565);
      identifier();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(566);
      match(MQL_Parser::ANON_ID);
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(567);
      match(MQL_Parser::EDGE_ID);
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(568);
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
  enterRule(_localctx, 82, MQL_Parser::RuleEdge);
  size_t _la = 0;

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
      case MQL_Parser::LT: {
        enterOuterAlt(_localctx, 1);
        setState(571);
        match(MQL_Parser::LT);
        setState(574);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 74, _ctx)) {
        case 1: {
          setState(572);
          match(MQL_Parser::MINUS);
          setState(573);
          edgeInside();
          break;
        }

        default:
          break;
        }
        setState(576);
        match(MQL_Parser::MINUS);
        break;
      }

      case MQL_Parser::MINUS: {
        enterOuterAlt(_localctx, 2);
        setState(577);
        match(MQL_Parser::MINUS);
        setState(581);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == MQL_Parser::LSQUARE_BRACKET) {
          setState(578);
          edgeInside();
          setState(579);
          match(MQL_Parser::MINUS);
        }
        setState(583);
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
  enterRule(_localctx, 84, MQL_Parser::RuleEdgeInside);
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
    setState(586);
    match(MQL_Parser::LSQUARE_BRACKET);
    setState(588);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MQL_Parser::EDGE_ID

    || _la == MQL_Parser::VARIABLE) {
      setState(587);
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
    setState(591);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MQL_Parser::TYPE

    || _la == MQL_Parser::TYPE_VAR) {
      setState(590);
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
    setState(594);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MQL_Parser::LCURLY_BRACKET) {
      setState(593);
      properties();
    }
    setState(596);
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
  enterRule(_localctx, 86, MQL_Parser::RuleVarNode);
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
    setState(598);
    match(MQL_Parser::L_PAR);
    setState(600);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MQL_Parser::VARIABLE) {
      setState(599);
      match(MQL_Parser::VARIABLE);
    }
    setState(605);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MQL_Parser::TYPE) {
      setState(602);
      match(MQL_Parser::TYPE);
      setState(607);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(609);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MQL_Parser::LCURLY_BRACKET) {
      setState(608);
      properties();
    }
    setState(611);
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
  enterRule(_localctx, 88, MQL_Parser::RuleProperties);
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
    setState(613);
    match(MQL_Parser::LCURLY_BRACKET);
    setState(614);
    property();
    setState(619);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MQL_Parser::COMMA) {
      setState(615);
      match(MQL_Parser::COMMA);
      setState(616);
      property();
      setState(621);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(622);
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
  enterRule(_localctx, 90, MQL_Parser::RuleProperty);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(653);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 87, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<MQL_Parser::Property1Context>(_localctx);
      enterOuterAlt(_localctx, 1);
      setState(624);
      identifier();
      setState(629);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case MQL_Parser::COLON: {
          setState(625);
          match(MQL_Parser::COLON);
          setState(626);
          value();
          break;
        }

        case MQL_Parser::TRUE_PROP: {
          setState(627);
          match(MQL_Parser::TRUE_PROP);
          break;
        }

        case MQL_Parser::FALSE_PROP: {
          setState(628);
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
      setState(631);
      identifier();
      setState(632);
      match(MQL_Parser::TYPE);
      setState(633);
      match(MQL_Parser::L_PAR);
      setState(634);
      match(MQL_Parser::STRING);
      setState(635);
      match(MQL_Parser::R_PAR);
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<MQL_Parser::Property3Context>(_localctx);
      enterOuterAlt(_localctx, 3);
      setState(637);
      identifier();
      setState(638);
      match(MQL_Parser::K_IS);
      setState(640);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == MQL_Parser::K_NOT) {
        setState(639);
        match(MQL_Parser::K_NOT);
      }
      setState(642);
      exprTypename();
      setState(646);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == MQL_Parser::K_OR) {
        setState(643);
        conditionalOrType();
        setState(648);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      break;
    }

    case 4: {
      _localctx = _tracker.createInstance<MQL_Parser::Property4Context>(_localctx);
      enterOuterAlt(_localctx, 4);
      setState(649);
      identifier();

      setState(650);
      antlrcpp::downCast<Property4Context *>(_localctx)->op = _input->LT(1);
      _la = _input->LA(1);
      if (!(((((_la - 67) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 67)) & 63) != 0))) {
        antlrcpp::downCast<Property4Context *>(_localctx)->op = _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(651);
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
  enterRule(_localctx, 92, MQL_Parser::RuleConditionalOrType);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(655);
    match(MQL_Parser::K_OR);
    setState(656);
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
  enterRule(_localctx, 94, MQL_Parser::RuleIdentifier);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(660);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case MQL_Parser::NAME: {
        enterOuterAlt(_localctx, 1);
        setState(658);
        match(MQL_Parser::NAME);
        break;
      }

      case MQL_Parser::K_ACYCLIC:
      case MQL_Parser::K_AND:
      case MQL_Parser::K_ANGULAR:
      case MQL_Parser::K_ANY:
      case MQL_Parser::K_AVG:
      case MQL_Parser::K_ALL:
      case MQL_Parser::K_ASC:
      case MQL_Parser::K_BY:
      case MQL_Parser::K_BOOL:
      case MQL_Parser::K_COUNT:
      case MQL_Parser::K_DELETE:
      case MQL_Parser::K_DESCRIBE:
      case MQL_Parser::K_DESC:
      case MQL_Parser::K_DISTINCT:
      case MQL_Parser::K_EDGE:
      case MQL_Parser::K_EUCLIDEAN:
      case MQL_Parser::K_INCOMING:
      case MQL_Parser::K_INSERT:
      case MQL_Parser::K_INTEGER:
      case MQL_Parser::K_IS:
      case MQL_Parser::K_FLOAT:
      case MQL_Parser::K_GROUP:
      case MQL_Parser::K_LABELS:
      case MQL_Parser::K_LABEL:
      case MQL_Parser::K_LIMIT:
      case MQL_Parser::K_MANHATTAN:
      case MQL_Parser::K_MATCH:
      case MQL_Parser::K_MAX:
      case MQL_Parser::K_MIN:
      case MQL_Parser::K_OFFSET:
      case MQL_Parser::K_OPTIONAL:
      case MQL_Parser::K_ORDER:
      case MQL_Parser::K_OR:
      case MQL_Parser::K_OUTGOING:
      case MQL_Parser::K_PROPERTIES:
      case MQL_Parser::K_PROPERTY:
      case MQL_Parser::K_NOT:
      case MQL_Parser::K_NULL:
      case MQL_Parser::K_SHORTEST:
      case MQL_Parser::K_SIMPLE:
      case MQL_Parser::K_RETURN:
      case MQL_Parser::K_SET:
      case MQL_Parser::K_SIMILARITY_SEARCH:
      case MQL_Parser::K_SUM:
      case MQL_Parser::K_STRING:
      case MQL_Parser::K_TRAILS:
      case MQL_Parser::K_WALKS:
      case MQL_Parser::K_WHERE: {
        enterOuterAlt(_localctx, 2);
        setState(659);
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
  enterRule(_localctx, 96, MQL_Parser::RuleBoolValue);
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
    setState(662);
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
  enterRule(_localctx, 98, MQL_Parser::RuleNumericValue);
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
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MQL_Parser::PLUS

    || _la == MQL_Parser::MINUS) {
      setState(664);
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
    setState(667);
    _la = _input->LA(1);
    if (!(((((_la - 63) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 63)) & 7) != 0))) {
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
  enterRule(_localctx, 100, MQL_Parser::RuleDatatypeValue);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(669);
    identifier();
    setState(670);
    match(MQL_Parser::L_PAR);
    setState(671);
    match(MQL_Parser::STRING);
    setState(672);
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
  enterRule(_localctx, 102, MQL_Parser::RuleValue);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(678);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case MQL_Parser::UNSIGNED_INTEGER:
      case MQL_Parser::UNSIGNED_FLOAT:
      case MQL_Parser::UNSIGNED_SCIENTIFIC_NOTATION:
      case MQL_Parser::PLUS:
      case MQL_Parser::MINUS: {
        enterOuterAlt(_localctx, 1);
        setState(674);
        numericValue();
        break;
      }

      case MQL_Parser::STRING: {
        enterOuterAlt(_localctx, 2);
        setState(675);
        match(MQL_Parser::STRING);
        break;
      }

      case MQL_Parser::K_FALSE:
      case MQL_Parser::K_TRUE: {
        enterOuterAlt(_localctx, 3);
        setState(676);
        boolValue();
        break;
      }

      case MQL_Parser::K_ACYCLIC:
      case MQL_Parser::K_AND:
      case MQL_Parser::K_ANGULAR:
      case MQL_Parser::K_ANY:
      case MQL_Parser::K_AVG:
      case MQL_Parser::K_ALL:
      case MQL_Parser::K_ASC:
      case MQL_Parser::K_BY:
      case MQL_Parser::K_BOOL:
      case MQL_Parser::K_COUNT:
      case MQL_Parser::K_DELETE:
      case MQL_Parser::K_DESCRIBE:
      case MQL_Parser::K_DESC:
      case MQL_Parser::K_DISTINCT:
      case MQL_Parser::K_EDGE:
      case MQL_Parser::K_EUCLIDEAN:
      case MQL_Parser::K_INCOMING:
      case MQL_Parser::K_INSERT:
      case MQL_Parser::K_INTEGER:
      case MQL_Parser::K_IS:
      case MQL_Parser::K_FLOAT:
      case MQL_Parser::K_GROUP:
      case MQL_Parser::K_LABELS:
      case MQL_Parser::K_LABEL:
      case MQL_Parser::K_LIMIT:
      case MQL_Parser::K_MANHATTAN:
      case MQL_Parser::K_MATCH:
      case MQL_Parser::K_MAX:
      case MQL_Parser::K_MIN:
      case MQL_Parser::K_OFFSET:
      case MQL_Parser::K_OPTIONAL:
      case MQL_Parser::K_ORDER:
      case MQL_Parser::K_OR:
      case MQL_Parser::K_OUTGOING:
      case MQL_Parser::K_PROPERTIES:
      case MQL_Parser::K_PROPERTY:
      case MQL_Parser::K_NOT:
      case MQL_Parser::K_NULL:
      case MQL_Parser::K_SHORTEST:
      case MQL_Parser::K_SIMPLE:
      case MQL_Parser::K_RETURN:
      case MQL_Parser::K_SET:
      case MQL_Parser::K_SIMILARITY_SEARCH:
      case MQL_Parser::K_SUM:
      case MQL_Parser::K_STRING:
      case MQL_Parser::K_TRAILS:
      case MQL_Parser::K_WALKS:
      case MQL_Parser::K_WHERE:
      case MQL_Parser::NAME: {
        enterOuterAlt(_localctx, 4);
        setState(677);
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
  enterRule(_localctx, 104, MQL_Parser::RuleConditionalOrExpr);
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
    conditionalAndExpr();
    setState(685);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MQL_Parser::K_OR) {
      setState(681);
      match(MQL_Parser::K_OR);
      setState(682);
      conditionalAndExpr();
      setState(687);
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
  enterRule(_localctx, 106, MQL_Parser::RuleConditionalAndExpr);
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
    setState(688);
    comparisonExpr();
    setState(693);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MQL_Parser::K_AND) {
      setState(689);
      match(MQL_Parser::K_AND);
      setState(690);
      comparisonExpr();
      setState(695);
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
  enterRule(_localctx, 108, MQL_Parser::RuleComparisonExpr);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(708);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 95, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<MQL_Parser::ComparisonExprOpContext>(_localctx);
      enterOuterAlt(_localctx, 1);
      setState(696);
      additiveExpr();
      setState(699);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (((((_la - 67) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 67)) & 63) != 0)) {
        setState(697);
        antlrcpp::downCast<ComparisonExprOpContext *>(_localctx)->op = _input->LT(1);
        _la = _input->LA(1);
        if (!(((((_la - 67) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 67)) & 63) != 0))) {
          antlrcpp::downCast<ComparisonExprOpContext *>(_localctx)->op = _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        setState(698);
        additiveExpr();
      }
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<MQL_Parser::ComparisonExprIsContext>(_localctx);
      enterOuterAlt(_localctx, 2);
      setState(701);
      additiveExpr();
      setState(702);
      match(MQL_Parser::K_IS);
      setState(704);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == MQL_Parser::K_NOT) {
        setState(703);
        match(MQL_Parser::K_NOT);
      }
      setState(706);
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
  enterRule(_localctx, 110, MQL_Parser::RuleAdditiveExpr);
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
    setState(710);
    multiplicativeExpr();
    setState(715);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MQL_Parser::PLUS

    || _la == MQL_Parser::MINUS) {
      setState(711);
      antlrcpp::downCast<AdditiveExprContext *>(_localctx)->_tset1342 = _input->LT(1);
      _la = _input->LA(1);
      if (!(_la == MQL_Parser::PLUS

      || _la == MQL_Parser::MINUS)) {
        antlrcpp::downCast<AdditiveExprContext *>(_localctx)->_tset1342 = _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      antlrcpp::downCast<AdditiveExprContext *>(_localctx)->op.push_back(antlrcpp::downCast<AdditiveExprContext *>(_localctx)->_tset1342);
      setState(712);
      multiplicativeExpr();
      setState(717);
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
  enterRule(_localctx, 112, MQL_Parser::RuleMultiplicativeExpr);
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
    setState(718);
    unaryExpr();
    setState(723);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (((((_la - 74) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 74)) & 25) != 0)) {
      setState(719);
      antlrcpp::downCast<MultiplicativeExprContext *>(_localctx)->_tset1361 = _input->LT(1);
      _la = _input->LA(1);
      if (!(((((_la - 74) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 74)) & 25) != 0))) {
        antlrcpp::downCast<MultiplicativeExprContext *>(_localctx)->_tset1361 = _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      antlrcpp::downCast<MultiplicativeExprContext *>(_localctx)->op.push_back(antlrcpp::downCast<MultiplicativeExprContext *>(_localctx)->_tset1361);
      setState(720);
      unaryExpr();
      setState(725);
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
  enterRule(_localctx, 114, MQL_Parser::RuleUnaryExpr);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(733);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 98, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(726);
      match(MQL_Parser::K_NOT);
      setState(727);
      unaryExpr();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(728);
      atomicExpr();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(729);
      match(MQL_Parser::PLUS);
      setState(730);
      unaryExpr();
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(731);
      match(MQL_Parser::MINUS);
      setState(732);
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
  enterRule(_localctx, 116, MQL_Parser::RuleAtomicExpr);
  size_t _la = 0;

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
      case MQL_Parser::VARIABLE: {
        _localctx = _tracker.createInstance<MQL_Parser::ExprVarContext>(_localctx);
        enterOuterAlt(_localctx, 1);
        setState(735);
        match(MQL_Parser::VARIABLE);
        setState(737);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == MQL_Parser::KEY) {
          setState(736);
          match(MQL_Parser::KEY);
        }
        break;
      }

      case MQL_Parser::K_ACYCLIC:
      case MQL_Parser::K_AND:
      case MQL_Parser::K_ANGULAR:
      case MQL_Parser::K_ANY:
      case MQL_Parser::K_AVG:
      case MQL_Parser::K_ALL:
      case MQL_Parser::K_ASC:
      case MQL_Parser::K_BY:
      case MQL_Parser::K_BOOL:
      case MQL_Parser::K_COUNT:
      case MQL_Parser::K_DELETE:
      case MQL_Parser::K_DESCRIBE:
      case MQL_Parser::K_DESC:
      case MQL_Parser::K_DISTINCT:
      case MQL_Parser::K_EDGE:
      case MQL_Parser::K_EUCLIDEAN:
      case MQL_Parser::K_INCOMING:
      case MQL_Parser::K_INSERT:
      case MQL_Parser::K_INTEGER:
      case MQL_Parser::K_IS:
      case MQL_Parser::K_FALSE:
      case MQL_Parser::K_FLOAT:
      case MQL_Parser::K_GROUP:
      case MQL_Parser::K_LABELS:
      case MQL_Parser::K_LABEL:
      case MQL_Parser::K_LIMIT:
      case MQL_Parser::K_MANHATTAN:
      case MQL_Parser::K_MATCH:
      case MQL_Parser::K_MAX:
      case MQL_Parser::K_MIN:
      case MQL_Parser::K_OFFSET:
      case MQL_Parser::K_OPTIONAL:
      case MQL_Parser::K_ORDER:
      case MQL_Parser::K_OR:
      case MQL_Parser::K_OUTGOING:
      case MQL_Parser::K_PROPERTIES:
      case MQL_Parser::K_PROPERTY:
      case MQL_Parser::K_NOT:
      case MQL_Parser::K_NULL:
      case MQL_Parser::K_SHORTEST:
      case MQL_Parser::K_SIMPLE:
      case MQL_Parser::K_RETURN:
      case MQL_Parser::K_SET:
      case MQL_Parser::K_SIMILARITY_SEARCH:
      case MQL_Parser::K_SUM:
      case MQL_Parser::K_STRING:
      case MQL_Parser::K_TRUE:
      case MQL_Parser::K_TRAILS:
      case MQL_Parser::K_WALKS:
      case MQL_Parser::K_WHERE:
      case MQL_Parser::STRING:
      case MQL_Parser::UNSIGNED_INTEGER:
      case MQL_Parser::UNSIGNED_FLOAT:
      case MQL_Parser::UNSIGNED_SCIENTIFIC_NOTATION:
      case MQL_Parser::NAME:
      case MQL_Parser::PLUS:
      case MQL_Parser::MINUS: {
        _localctx = _tracker.createInstance<MQL_Parser::ExprValueContext>(_localctx);
        enterOuterAlt(_localctx, 2);
        setState(739);
        value();
        break;
      }

      case MQL_Parser::L_PAR: {
        _localctx = _tracker.createInstance<MQL_Parser::ExprParenthesisContext>(_localctx);
        enterOuterAlt(_localctx, 3);
        setState(740);
        match(MQL_Parser::L_PAR);
        setState(741);
        conditionalOrExpr();
        setState(742);
        match(MQL_Parser::R_PAR);
        break;
      }

      case MQL_Parser::K_REGEX: {
        _localctx = _tracker.createInstance<MQL_Parser::ExprFunctionContext>(_localctx);
        enterOuterAlt(_localctx, 4);
        setState(744);
        function();
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

//----------------- FunctionContext ------------------------------------------------------------------

MQL_Parser::FunctionContext::FunctionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

MQL_Parser::RegexContext* MQL_Parser::FunctionContext::regex() {
  return getRuleContext<MQL_Parser::RegexContext>(0);
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
  enterRule(_localctx, 118, MQL_Parser::RuleFunction);

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
    regex();
   
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
  enterRule(_localctx, 120, MQL_Parser::RuleRegex);
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
    setState(749);
    match(MQL_Parser::K_REGEX);
    setState(750);
    match(MQL_Parser::L_PAR);
    setState(751);
    conditionalOrExpr();
    setState(752);
    match(MQL_Parser::COMMA);
    setState(753);
    conditionalOrExpr();
    setState(756);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MQL_Parser::COMMA) {
      setState(754);
      match(MQL_Parser::COMMA);
      setState(755);
      conditionalOrExpr();
    }
    setState(758);
    match(MQL_Parser::R_PAR);
   
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
  enterRule(_localctx, 122, MQL_Parser::RuleExprTypename);
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
    setState(760);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 565148986115072) != 0))) {
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

tree::TerminalNode* MQL_Parser::KeywordContext::K_ALL() {
  return getToken(MQL_Parser::K_ALL, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_AVG() {
  return getToken(MQL_Parser::K_AVG, 0);
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

tree::TerminalNode* MQL_Parser::KeywordContext::K_DELETE() {
  return getToken(MQL_Parser::K_DELETE, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_DESCRIBE() {
  return getToken(MQL_Parser::K_DESCRIBE, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_DESC() {
  return getToken(MQL_Parser::K_DESC, 0);
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

tree::TerminalNode* MQL_Parser::KeywordContext::K_INCOMING() {
  return getToken(MQL_Parser::K_INCOMING, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_INSERT() {
  return getToken(MQL_Parser::K_INSERT, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_INTEGER() {
  return getToken(MQL_Parser::K_INTEGER, 0);
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

tree::TerminalNode* MQL_Parser::KeywordContext::K_SIMPLE() {
  return getToken(MQL_Parser::K_SIMPLE, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_RETURN() {
  return getToken(MQL_Parser::K_RETURN, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_SET() {
  return getToken(MQL_Parser::K_SET, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_SIMILARITY_SEARCH() {
  return getToken(MQL_Parser::K_SIMILARITY_SEARCH, 0);
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

tree::TerminalNode* MQL_Parser::KeywordContext::K_WALKS() {
  return getToken(MQL_Parser::K_WALKS, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_WHERE() {
  return getToken(MQL_Parser::K_WHERE, 0);
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
  enterRule(_localctx, 124, MQL_Parser::RuleKeyword);
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
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 16870768973446910) != 0))) {
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


// Generated from SparqlUpdateParser.g4 by ANTLR 4.13.1


#include "SparqlUpdateParserVisitor.h"

#include "SparqlUpdateParser.h"


using namespace antlrcpp;

using namespace antlr4;

namespace {

struct SparqlUpdateParserStaticData final {
  SparqlUpdateParserStaticData(std::vector<std::string> ruleNames,
                        std::vector<std::string> literalNames,
                        std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  SparqlUpdateParserStaticData(const SparqlUpdateParserStaticData&) = delete;
  SparqlUpdateParserStaticData(SparqlUpdateParserStaticData&&) = delete;
  SparqlUpdateParserStaticData& operator=(const SparqlUpdateParserStaticData&) = delete;
  SparqlUpdateParserStaticData& operator=(SparqlUpdateParserStaticData&&) = delete;

  std::vector<antlr4::dfa::DFA> decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string> ruleNames;
  const std::vector<std::string> literalNames;
  const std::vector<std::string> symbolicNames;
  const antlr4::dfa::Vocabulary vocabulary;
  antlr4::atn::SerializedATNView serializedATN;
  std::unique_ptr<antlr4::atn::ATN> atn;
};

::antlr4::internal::OnceFlag sparqlupdateparserParserOnceFlag;
#if ANTLR4_USE_THREAD_LOCAL_CACHE
static thread_local
#endif
SparqlUpdateParserStaticData *sparqlupdateparserParserStaticData = nullptr;

void sparqlupdateparserParserInitialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  if (sparqlupdateparserParserStaticData != nullptr) {
    return;
  }
#else
  assert(sparqlupdateparserParserStaticData == nullptr);
#endif
  auto staticData = std::make_unique<SparqlUpdateParserStaticData>(
    std::vector<std::string>{
      "query", "prologue", "baseDecl", "prefixDecl", "update", "createIndexQuery", 
      "createIndexOptions", "createIndexOption", "insertData", "deleteData", 
      "quadData", "triplesTemplate", "constructTemplate", "constructTriples", 
      "triplesSameSubject", "propertyList", "propertyListNotEmpty", "verb", 
      "objectList", "object", "propertyListPathNotEmpty", "propertyListPathNotEmptyList", 
      "objectListPath", "objectPath", "integer", "triplesNode", "blankNodePropertyList", 
      "triplesNodePath", "blankNodePropertyListPath", "collection", "collectionPath", 
      "graphNode", "graphNodePath", "graphTerm", "nil", "rdfLiteral", "numericLiteral", 
      "numericLiteralUnsigned", "numericLiteralPositive", "numericLiteralNegative", 
      "booleanLiteral", "string", "iri", "prefixedName", "blankNode", "anon"
    },
    std::vector<std::string>{
      "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
      "", "", "", "", "", "", "", "'a'", "", "", "", "", "", "", "", "", 
      "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
      "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
      "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
      "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
      "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
      "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
      "", "", "", "", "", "", "", "", "'^^'", "'<='", "'>='", "'!='", "'&&'", 
      "'||'", "'^'", "'('", "')'", "'{'", "'}'", "'['", "']'", "';'", "'.'", 
      "'+'", "'-'", "'*'", "'\\u003F'", "','", "'!'", "'/'", "'='", "'<'", 
      "'>'", "'|'"
    },
    std::vector<std::string>{
      "", "WS", "BASE", "PREFIX", "SELECT", "DISTINCT", "REDUCED", "CONSTRUCT", 
      "DESCRIBE", "ASK", "FROM", "NAMED", "WHERE", "ORDER", "BY", "ASC", 
      "DESC", "LIMIT", "OFFSET", "VALUES", "OPTIONAL", "GRAPH", "UNION", 
      "FILTER", "A", "STR", "LANG", "LANGMATCHES", "DATATYPE", "BOUND", 
      "SAMETERM", "ISIRI", "ISURI", "ISBLANK", "ISLITERAL", "REGEX", "SUBSTR", 
      "K_TRUE", "K_FALSE", "LOAD", "CLEAR", "DROP", "ADD", "MOVE", "COPY", 
      "CREATE", "DELETE", "INSERT", "USING", "SILENT", "DEFAULT", "ALL", 
      "DATA", "WITH", "INTO", "TO", "AS", "GROUP", "HAVING", "UNDEF", "BINDINGS", 
      "SERVICE", "BIND", "MINUS", "IRI", "URI", "BNODE", "RAND", "ABS", 
      "CEIL", "FLOOR", "ROUND", "CONCAT", "STRLEN", "UCASE", "LCASE", "ENCODE_FOR_URI", 
      "CONTAINS", "STRSTARTS", "STRENDS", "STRBEFORE", "STRAFTER", "REPLACE", 
      "YEAR", "MONTH", "DAY", "HOURS", "MINUTES", "SECONDS", "TIMEZONE", 
      "TZ", "NOW", "UUID", "STRUUID", "MD5", "SHA1", "SHA256", "SHA384", 
      "SHA512", "COALESCE", "IF", "STRLANG", "STRDT", "ISNUMERIC", "COUNT", 
      "SUM", "MIN", "MAX", "AVG", "SAMPLE", "GROUP_CONCAT", "NOT", "IN", 
      "EXISTS", "SEPARATOR", "ANY", "SHORTEST", "SIMPLE", "ACYCLIC", "WALKS", 
      "INDEX", "ALPHANUMERIC_IDENTIFIER", "IRIREF", "PNAME_NS", "PNAME_LN", 
      "BLANK_NODE_LABEL", "VAR1", "VAR2", "LANGTAG", "INTEGER", "DECIMAL", 
      "DOUBLE", "INTEGER_POSITIVE", "DECIMAL_POSITIVE", "DOUBLE_POSITIVE", 
      "INTEGER_NEGATIVE", "DECIMAL_NEGATIVE", "DOUBLE_NEGATIVE", "STRING_LITERAL1", 
      "STRING_LITERAL2", "STRING_LITERAL_LONG1", "STRING_LITERAL_LONG2", 
      "COMMENT", "REFERENCE", "LESS_EQUAL", "GREATER_EQUAL", "NOT_EQUAL", 
      "AND", "OR", "INVERSE", "OPEN_BRACE", "CLOSE_BRACE", "OPEN_CURLY_BRACE", 
      "CLOSE_CURLY_BRACE", "OPEN_SQUARE_BRACKET", "CLOSE_SQUARE_BRACKET", 
      "SEMICOLON", "DOT", "PLUS_SIGN", "MINUS_SIGN", "ASTERISK", "QUESTION_MARK", 
      "COMMA", "NEGATION", "DIVIDE", "EQUAL", "LESS", "GREATER", "PIPE", 
      "ANY_OTHER"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,1,169,338,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,6,2,
  	7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,2,14,7,
  	14,2,15,7,15,2,16,7,16,2,17,7,17,2,18,7,18,2,19,7,19,2,20,7,20,2,21,7,
  	21,2,22,7,22,2,23,7,23,2,24,7,24,2,25,7,25,2,26,7,26,2,27,7,27,2,28,7,
  	28,2,29,7,29,2,30,7,30,2,31,7,31,2,32,7,32,2,33,7,33,2,34,7,34,2,35,7,
  	35,2,36,7,36,2,37,7,37,2,38,7,38,2,39,7,39,2,40,7,40,2,41,7,41,2,42,7,
  	42,2,43,7,43,2,44,7,44,2,45,7,45,1,0,1,0,1,0,3,0,96,8,0,1,0,1,0,1,0,5,
  	0,101,8,0,10,0,12,0,104,9,0,1,0,1,0,1,1,1,1,5,1,110,8,1,10,1,12,1,113,
  	9,1,1,2,1,2,1,2,1,3,1,3,1,3,1,3,1,4,1,4,1,4,3,4,125,8,4,1,5,1,5,1,5,1,
  	5,1,5,1,5,1,5,1,6,1,6,1,6,1,6,5,6,138,8,6,10,6,12,6,141,9,6,3,6,143,8,
  	6,1,6,1,6,1,7,1,7,1,7,1,7,1,8,1,8,1,8,1,8,1,9,1,9,1,9,1,9,1,10,1,10,1,
  	10,1,10,1,11,1,11,1,11,3,11,166,8,11,5,11,168,8,11,10,11,12,11,171,9,
  	11,1,12,1,12,3,12,175,8,12,1,12,1,12,1,13,1,13,1,13,3,13,182,8,13,5,13,
  	184,8,13,10,13,12,13,187,9,13,1,14,1,14,1,14,1,14,1,14,1,14,3,14,195,
  	8,14,1,15,3,15,198,8,15,1,16,1,16,1,16,1,16,1,16,1,16,3,16,206,8,16,5,
  	16,208,8,16,10,16,12,16,211,9,16,1,17,1,17,3,17,215,8,17,1,18,1,18,1,
  	18,5,18,220,8,18,10,18,12,18,223,9,18,1,19,1,19,1,20,1,20,1,20,1,20,3,
  	20,231,8,20,5,20,233,8,20,10,20,12,20,236,9,20,1,21,1,21,1,21,1,22,1,
  	22,1,22,5,22,244,8,22,10,22,12,22,247,9,22,1,23,1,23,1,24,1,24,1,25,1,
  	25,3,25,255,8,25,1,26,1,26,1,26,1,26,1,27,1,27,3,27,263,8,27,1,28,1,28,
  	1,28,1,28,1,29,1,29,4,29,271,8,29,11,29,12,29,272,1,29,1,29,1,30,1,30,
  	4,30,279,8,30,11,30,12,30,280,1,30,1,30,1,31,1,31,3,31,287,8,31,1,32,
  	1,32,3,32,291,8,32,1,33,1,33,1,33,1,33,1,33,1,33,3,33,299,8,33,1,34,1,
  	34,1,34,1,35,1,35,1,35,1,35,3,35,308,8,35,1,36,1,36,1,36,3,36,313,8,36,
  	1,37,1,37,1,38,1,38,1,39,1,39,1,40,1,40,1,41,1,41,1,42,1,42,3,42,327,
  	8,42,1,43,1,43,1,44,1,44,3,44,333,8,44,1,45,1,45,1,45,1,45,0,0,46,0,2,
  	4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,34,36,38,40,42,44,46,48,50,
  	52,54,56,58,60,62,64,66,68,70,72,74,76,78,80,82,84,86,88,90,0,6,1,0,129,
  	131,1,0,132,134,1,0,135,137,1,0,37,38,1,0,138,141,1,0,123,124,330,0,92,
  	1,0,0,0,2,111,1,0,0,0,4,114,1,0,0,0,6,117,1,0,0,0,8,124,1,0,0,0,10,126,
  	1,0,0,0,12,133,1,0,0,0,14,146,1,0,0,0,16,150,1,0,0,0,18,154,1,0,0,0,20,
  	158,1,0,0,0,22,162,1,0,0,0,24,172,1,0,0,0,26,178,1,0,0,0,28,194,1,0,0,
  	0,30,197,1,0,0,0,32,199,1,0,0,0,34,214,1,0,0,0,36,216,1,0,0,0,38,224,
  	1,0,0,0,40,226,1,0,0,0,42,237,1,0,0,0,44,240,1,0,0,0,46,248,1,0,0,0,48,
  	250,1,0,0,0,50,254,1,0,0,0,52,256,1,0,0,0,54,262,1,0,0,0,56,264,1,0,0,
  	0,58,268,1,0,0,0,60,276,1,0,0,0,62,286,1,0,0,0,64,290,1,0,0,0,66,298,
  	1,0,0,0,68,300,1,0,0,0,70,303,1,0,0,0,72,312,1,0,0,0,74,314,1,0,0,0,76,
  	316,1,0,0,0,78,318,1,0,0,0,80,320,1,0,0,0,82,322,1,0,0,0,84,326,1,0,0,
  	0,86,328,1,0,0,0,88,332,1,0,0,0,90,334,1,0,0,0,92,93,3,2,1,0,93,102,3,
  	8,4,0,94,96,5,156,0,0,95,94,1,0,0,0,95,96,1,0,0,0,96,97,1,0,0,0,97,98,
  	3,2,1,0,98,99,3,8,4,0,99,101,1,0,0,0,100,95,1,0,0,0,101,104,1,0,0,0,102,
  	100,1,0,0,0,102,103,1,0,0,0,103,105,1,0,0,0,104,102,1,0,0,0,105,106,5,
  	0,0,1,106,1,1,0,0,0,107,110,3,4,2,0,108,110,3,6,3,0,109,107,1,0,0,0,109,
  	108,1,0,0,0,110,113,1,0,0,0,111,109,1,0,0,0,111,112,1,0,0,0,112,3,1,0,
  	0,0,113,111,1,0,0,0,114,115,5,2,0,0,115,116,5,122,0,0,116,5,1,0,0,0,117,
  	118,5,3,0,0,118,119,5,123,0,0,119,120,5,122,0,0,120,7,1,0,0,0,121,125,
  	3,16,8,0,122,125,3,18,9,0,123,125,3,10,5,0,124,121,1,0,0,0,124,122,1,
  	0,0,0,124,123,1,0,0,0,125,9,1,0,0,0,126,127,5,45,0,0,127,128,5,121,0,
  	0,128,129,5,120,0,0,129,130,3,82,41,0,130,131,5,53,0,0,131,132,3,12,6,
  	0,132,11,1,0,0,0,133,142,5,152,0,0,134,139,3,14,7,0,135,136,5,162,0,0,
  	136,138,3,14,7,0,137,135,1,0,0,0,138,141,1,0,0,0,139,137,1,0,0,0,139,
  	140,1,0,0,0,140,143,1,0,0,0,141,139,1,0,0,0,142,134,1,0,0,0,142,143,1,
  	0,0,0,143,144,1,0,0,0,144,145,5,153,0,0,145,13,1,0,0,0,146,147,3,82,41,
  	0,147,148,5,165,0,0,148,149,3,66,33,0,149,15,1,0,0,0,150,151,5,47,0,0,
  	151,152,5,52,0,0,152,153,3,20,10,0,153,17,1,0,0,0,154,155,5,46,0,0,155,
  	156,5,52,0,0,156,157,3,20,10,0,157,19,1,0,0,0,158,159,5,152,0,0,159,160,
  	3,22,11,0,160,161,5,153,0,0,161,21,1,0,0,0,162,169,3,28,14,0,163,165,
  	5,157,0,0,164,166,3,28,14,0,165,164,1,0,0,0,165,166,1,0,0,0,166,168,1,
  	0,0,0,167,163,1,0,0,0,168,171,1,0,0,0,169,167,1,0,0,0,169,170,1,0,0,0,
  	170,23,1,0,0,0,171,169,1,0,0,0,172,174,5,152,0,0,173,175,3,26,13,0,174,
  	173,1,0,0,0,174,175,1,0,0,0,175,176,1,0,0,0,176,177,5,153,0,0,177,25,
  	1,0,0,0,178,185,3,28,14,0,179,181,5,157,0,0,180,182,3,26,13,0,181,180,
  	1,0,0,0,181,182,1,0,0,0,182,184,1,0,0,0,183,179,1,0,0,0,184,187,1,0,0,
  	0,185,183,1,0,0,0,185,186,1,0,0,0,186,27,1,0,0,0,187,185,1,0,0,0,188,
  	189,3,66,33,0,189,190,3,32,16,0,190,195,1,0,0,0,191,192,3,50,25,0,192,
  	193,3,30,15,0,193,195,1,0,0,0,194,188,1,0,0,0,194,191,1,0,0,0,195,29,
  	1,0,0,0,196,198,3,32,16,0,197,196,1,0,0,0,197,198,1,0,0,0,198,31,1,0,
  	0,0,199,200,3,34,17,0,200,209,3,36,18,0,201,205,5,156,0,0,202,203,3,34,
  	17,0,203,204,3,36,18,0,204,206,1,0,0,0,205,202,1,0,0,0,205,206,1,0,0,
  	0,206,208,1,0,0,0,207,201,1,0,0,0,208,211,1,0,0,0,209,207,1,0,0,0,209,
  	210,1,0,0,0,210,33,1,0,0,0,211,209,1,0,0,0,212,215,3,84,42,0,213,215,
  	5,24,0,0,214,212,1,0,0,0,214,213,1,0,0,0,215,35,1,0,0,0,216,221,3,38,
  	19,0,217,218,5,162,0,0,218,220,3,38,19,0,219,217,1,0,0,0,220,223,1,0,
  	0,0,221,219,1,0,0,0,221,222,1,0,0,0,222,37,1,0,0,0,223,221,1,0,0,0,224,
  	225,3,62,31,0,225,39,1,0,0,0,226,227,3,34,17,0,227,234,3,44,22,0,228,
  	230,5,156,0,0,229,231,3,42,21,0,230,229,1,0,0,0,230,231,1,0,0,0,231,233,
  	1,0,0,0,232,228,1,0,0,0,233,236,1,0,0,0,234,232,1,0,0,0,234,235,1,0,0,
  	0,235,41,1,0,0,0,236,234,1,0,0,0,237,238,3,34,17,0,238,239,3,36,18,0,
  	239,43,1,0,0,0,240,245,3,46,23,0,241,242,5,162,0,0,242,244,3,46,23,0,
  	243,241,1,0,0,0,244,247,1,0,0,0,245,243,1,0,0,0,245,246,1,0,0,0,246,45,
  	1,0,0,0,247,245,1,0,0,0,248,249,3,64,32,0,249,47,1,0,0,0,250,251,5,129,
  	0,0,251,49,1,0,0,0,252,255,3,58,29,0,253,255,3,52,26,0,254,252,1,0,0,
  	0,254,253,1,0,0,0,255,51,1,0,0,0,256,257,5,154,0,0,257,258,3,32,16,0,
  	258,259,5,155,0,0,259,53,1,0,0,0,260,263,3,60,30,0,261,263,3,56,28,0,
  	262,260,1,0,0,0,262,261,1,0,0,0,263,55,1,0,0,0,264,265,5,154,0,0,265,
  	266,3,40,20,0,266,267,5,155,0,0,267,57,1,0,0,0,268,270,5,150,0,0,269,
  	271,3,62,31,0,270,269,1,0,0,0,271,272,1,0,0,0,272,270,1,0,0,0,272,273,
  	1,0,0,0,273,274,1,0,0,0,274,275,5,151,0,0,275,59,1,0,0,0,276,278,5,150,
  	0,0,277,279,3,64,32,0,278,277,1,0,0,0,279,280,1,0,0,0,280,278,1,0,0,0,
  	280,281,1,0,0,0,281,282,1,0,0,0,282,283,5,151,0,0,283,61,1,0,0,0,284,
  	287,3,66,33,0,285,287,3,50,25,0,286,284,1,0,0,0,286,285,1,0,0,0,287,63,
  	1,0,0,0,288,291,3,66,33,0,289,291,3,54,27,0,290,288,1,0,0,0,290,289,1,
  	0,0,0,291,65,1,0,0,0,292,299,3,84,42,0,293,299,3,70,35,0,294,299,3,72,
  	36,0,295,299,3,80,40,0,296,299,3,88,44,0,297,299,3,68,34,0,298,292,1,
  	0,0,0,298,293,1,0,0,0,298,294,1,0,0,0,298,295,1,0,0,0,298,296,1,0,0,0,
  	298,297,1,0,0,0,299,67,1,0,0,0,300,301,5,150,0,0,301,302,5,151,0,0,302,
  	69,1,0,0,0,303,307,3,82,41,0,304,308,5,128,0,0,305,306,5,143,0,0,306,
  	308,3,84,42,0,307,304,1,0,0,0,307,305,1,0,0,0,307,308,1,0,0,0,308,71,
  	1,0,0,0,309,313,3,74,37,0,310,313,3,76,38,0,311,313,3,78,39,0,312,309,
  	1,0,0,0,312,310,1,0,0,0,312,311,1,0,0,0,313,73,1,0,0,0,314,315,7,0,0,
  	0,315,75,1,0,0,0,316,317,7,1,0,0,317,77,1,0,0,0,318,319,7,2,0,0,319,79,
  	1,0,0,0,320,321,7,3,0,0,321,81,1,0,0,0,322,323,7,4,0,0,323,83,1,0,0,0,
  	324,327,5,122,0,0,325,327,3,86,43,0,326,324,1,0,0,0,326,325,1,0,0,0,327,
  	85,1,0,0,0,328,329,7,5,0,0,329,87,1,0,0,0,330,333,5,125,0,0,331,333,3,
  	90,45,0,332,330,1,0,0,0,332,331,1,0,0,0,333,89,1,0,0,0,334,335,5,154,
  	0,0,335,336,5,155,0,0,336,91,1,0,0,0,32,95,102,109,111,124,139,142,165,
  	169,174,181,185,194,197,205,209,214,221,230,234,245,254,262,272,280,286,
  	290,298,307,312,326,332
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  sparqlupdateparserParserStaticData = staticData.release();
}

}

SparqlUpdateParser::SparqlUpdateParser(TokenStream *input) : SparqlUpdateParser(input, antlr4::atn::ParserATNSimulatorOptions()) {}

SparqlUpdateParser::SparqlUpdateParser(TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options) : Parser(input) {
  SparqlUpdateParser::initialize();
  _interpreter = new atn::ParserATNSimulator(this, *sparqlupdateparserParserStaticData->atn, sparqlupdateparserParserStaticData->decisionToDFA, sparqlupdateparserParserStaticData->sharedContextCache, options);
}

SparqlUpdateParser::~SparqlUpdateParser() {
  delete _interpreter;
}

const atn::ATN& SparqlUpdateParser::getATN() const {
  return *sparqlupdateparserParserStaticData->atn;
}

std::string SparqlUpdateParser::getGrammarFileName() const {
  return "SparqlUpdateParser.g4";
}

const std::vector<std::string>& SparqlUpdateParser::getRuleNames() const {
  return sparqlupdateparserParserStaticData->ruleNames;
}

const dfa::Vocabulary& SparqlUpdateParser::getVocabulary() const {
  return sparqlupdateparserParserStaticData->vocabulary;
}

antlr4::atn::SerializedATNView SparqlUpdateParser::getSerializedATN() const {
  return sparqlupdateparserParserStaticData->serializedATN;
}


//----------------- QueryContext ------------------------------------------------------------------

SparqlUpdateParser::QueryContext::QueryContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<SparqlUpdateParser::PrologueContext *> SparqlUpdateParser::QueryContext::prologue() {
  return getRuleContexts<SparqlUpdateParser::PrologueContext>();
}

SparqlUpdateParser::PrologueContext* SparqlUpdateParser::QueryContext::prologue(size_t i) {
  return getRuleContext<SparqlUpdateParser::PrologueContext>(i);
}

std::vector<SparqlUpdateParser::UpdateContext *> SparqlUpdateParser::QueryContext::update() {
  return getRuleContexts<SparqlUpdateParser::UpdateContext>();
}

SparqlUpdateParser::UpdateContext* SparqlUpdateParser::QueryContext::update(size_t i) {
  return getRuleContext<SparqlUpdateParser::UpdateContext>(i);
}

tree::TerminalNode* SparqlUpdateParser::QueryContext::EOF() {
  return getToken(SparqlUpdateParser::EOF, 0);
}

std::vector<tree::TerminalNode *> SparqlUpdateParser::QueryContext::SEMICOLON() {
  return getTokens(SparqlUpdateParser::SEMICOLON);
}

tree::TerminalNode* SparqlUpdateParser::QueryContext::SEMICOLON(size_t i) {
  return getToken(SparqlUpdateParser::SEMICOLON, i);
}


size_t SparqlUpdateParser::QueryContext::getRuleIndex() const {
  return SparqlUpdateParser::RuleQuery;
}


std::any SparqlUpdateParser::QueryContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlUpdateParserVisitor*>(visitor))
    return parserVisitor->visitQuery(this);
  else
    return visitor->visitChildren(this);
}

SparqlUpdateParser::QueryContext* SparqlUpdateParser::query() {
  QueryContext *_localctx = _tracker.createInstance<QueryContext>(_ctx, getState());
  enterRule(_localctx, 0, SparqlUpdateParser::RuleQuery);
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
    setState(92);
    prologue();
    setState(93);
    update();
    setState(102);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 246290604621836) != 0) || _la == SparqlUpdateParser::SEMICOLON) {
      setState(95);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == SparqlUpdateParser::SEMICOLON) {
        setState(94);
        match(SparqlUpdateParser::SEMICOLON);
      }
      setState(97);
      prologue();
      setState(98);
      update();
      setState(104);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(105);
    match(SparqlUpdateParser::EOF);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PrologueContext ------------------------------------------------------------------

SparqlUpdateParser::PrologueContext::PrologueContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<SparqlUpdateParser::BaseDeclContext *> SparqlUpdateParser::PrologueContext::baseDecl() {
  return getRuleContexts<SparqlUpdateParser::BaseDeclContext>();
}

SparqlUpdateParser::BaseDeclContext* SparqlUpdateParser::PrologueContext::baseDecl(size_t i) {
  return getRuleContext<SparqlUpdateParser::BaseDeclContext>(i);
}

std::vector<SparqlUpdateParser::PrefixDeclContext *> SparqlUpdateParser::PrologueContext::prefixDecl() {
  return getRuleContexts<SparqlUpdateParser::PrefixDeclContext>();
}

SparqlUpdateParser::PrefixDeclContext* SparqlUpdateParser::PrologueContext::prefixDecl(size_t i) {
  return getRuleContext<SparqlUpdateParser::PrefixDeclContext>(i);
}


size_t SparqlUpdateParser::PrologueContext::getRuleIndex() const {
  return SparqlUpdateParser::RulePrologue;
}


std::any SparqlUpdateParser::PrologueContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlUpdateParserVisitor*>(visitor))
    return parserVisitor->visitPrologue(this);
  else
    return visitor->visitChildren(this);
}

SparqlUpdateParser::PrologueContext* SparqlUpdateParser::prologue() {
  PrologueContext *_localctx = _tracker.createInstance<PrologueContext>(_ctx, getState());
  enterRule(_localctx, 2, SparqlUpdateParser::RulePrologue);
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
    setState(111);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SparqlUpdateParser::BASE

    || _la == SparqlUpdateParser::PREFIX) {
      setState(109);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case SparqlUpdateParser::BASE: {
          setState(107);
          baseDecl();
          break;
        }

        case SparqlUpdateParser::PREFIX: {
          setState(108);
          prefixDecl();
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      setState(113);
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

//----------------- BaseDeclContext ------------------------------------------------------------------

SparqlUpdateParser::BaseDeclContext::BaseDeclContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlUpdateParser::BaseDeclContext::BASE() {
  return getToken(SparqlUpdateParser::BASE, 0);
}

tree::TerminalNode* SparqlUpdateParser::BaseDeclContext::IRIREF() {
  return getToken(SparqlUpdateParser::IRIREF, 0);
}


size_t SparqlUpdateParser::BaseDeclContext::getRuleIndex() const {
  return SparqlUpdateParser::RuleBaseDecl;
}


std::any SparqlUpdateParser::BaseDeclContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlUpdateParserVisitor*>(visitor))
    return parserVisitor->visitBaseDecl(this);
  else
    return visitor->visitChildren(this);
}

SparqlUpdateParser::BaseDeclContext* SparqlUpdateParser::baseDecl() {
  BaseDeclContext *_localctx = _tracker.createInstance<BaseDeclContext>(_ctx, getState());
  enterRule(_localctx, 4, SparqlUpdateParser::RuleBaseDecl);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(114);
    match(SparqlUpdateParser::BASE);
    setState(115);
    match(SparqlUpdateParser::IRIREF);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PrefixDeclContext ------------------------------------------------------------------

SparqlUpdateParser::PrefixDeclContext::PrefixDeclContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlUpdateParser::PrefixDeclContext::PREFIX() {
  return getToken(SparqlUpdateParser::PREFIX, 0);
}

tree::TerminalNode* SparqlUpdateParser::PrefixDeclContext::PNAME_NS() {
  return getToken(SparqlUpdateParser::PNAME_NS, 0);
}

tree::TerminalNode* SparqlUpdateParser::PrefixDeclContext::IRIREF() {
  return getToken(SparqlUpdateParser::IRIREF, 0);
}


size_t SparqlUpdateParser::PrefixDeclContext::getRuleIndex() const {
  return SparqlUpdateParser::RulePrefixDecl;
}


std::any SparqlUpdateParser::PrefixDeclContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlUpdateParserVisitor*>(visitor))
    return parserVisitor->visitPrefixDecl(this);
  else
    return visitor->visitChildren(this);
}

SparqlUpdateParser::PrefixDeclContext* SparqlUpdateParser::prefixDecl() {
  PrefixDeclContext *_localctx = _tracker.createInstance<PrefixDeclContext>(_ctx, getState());
  enterRule(_localctx, 6, SparqlUpdateParser::RulePrefixDecl);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(117);
    match(SparqlUpdateParser::PREFIX);
    setState(118);
    match(SparqlUpdateParser::PNAME_NS);
    setState(119);
    match(SparqlUpdateParser::IRIREF);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- UpdateContext ------------------------------------------------------------------

SparqlUpdateParser::UpdateContext::UpdateContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlUpdateParser::InsertDataContext* SparqlUpdateParser::UpdateContext::insertData() {
  return getRuleContext<SparqlUpdateParser::InsertDataContext>(0);
}

SparqlUpdateParser::DeleteDataContext* SparqlUpdateParser::UpdateContext::deleteData() {
  return getRuleContext<SparqlUpdateParser::DeleteDataContext>(0);
}

SparqlUpdateParser::CreateIndexQueryContext* SparqlUpdateParser::UpdateContext::createIndexQuery() {
  return getRuleContext<SparqlUpdateParser::CreateIndexQueryContext>(0);
}


size_t SparqlUpdateParser::UpdateContext::getRuleIndex() const {
  return SparqlUpdateParser::RuleUpdate;
}


std::any SparqlUpdateParser::UpdateContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlUpdateParserVisitor*>(visitor))
    return parserVisitor->visitUpdate(this);
  else
    return visitor->visitChildren(this);
}

SparqlUpdateParser::UpdateContext* SparqlUpdateParser::update() {
  UpdateContext *_localctx = _tracker.createInstance<UpdateContext>(_ctx, getState());
  enterRule(_localctx, 8, SparqlUpdateParser::RuleUpdate);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(124);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlUpdateParser::INSERT: {
        enterOuterAlt(_localctx, 1);
        setState(121);
        insertData();
        break;
      }

      case SparqlUpdateParser::DELETE: {
        enterOuterAlt(_localctx, 2);
        setState(122);
        deleteData();
        break;
      }

      case SparqlUpdateParser::CREATE: {
        enterOuterAlt(_localctx, 3);
        setState(123);
        createIndexQuery();
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

SparqlUpdateParser::CreateIndexQueryContext::CreateIndexQueryContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlUpdateParser::CreateIndexQueryContext::CREATE() {
  return getToken(SparqlUpdateParser::CREATE, 0);
}

tree::TerminalNode* SparqlUpdateParser::CreateIndexQueryContext::ALPHANUMERIC_IDENTIFIER() {
  return getToken(SparqlUpdateParser::ALPHANUMERIC_IDENTIFIER, 0);
}

tree::TerminalNode* SparqlUpdateParser::CreateIndexQueryContext::INDEX() {
  return getToken(SparqlUpdateParser::INDEX, 0);
}

SparqlUpdateParser::StringContext* SparqlUpdateParser::CreateIndexQueryContext::string() {
  return getRuleContext<SparqlUpdateParser::StringContext>(0);
}

tree::TerminalNode* SparqlUpdateParser::CreateIndexQueryContext::WITH() {
  return getToken(SparqlUpdateParser::WITH, 0);
}

SparqlUpdateParser::CreateIndexOptionsContext* SparqlUpdateParser::CreateIndexQueryContext::createIndexOptions() {
  return getRuleContext<SparqlUpdateParser::CreateIndexOptionsContext>(0);
}


size_t SparqlUpdateParser::CreateIndexQueryContext::getRuleIndex() const {
  return SparqlUpdateParser::RuleCreateIndexQuery;
}


std::any SparqlUpdateParser::CreateIndexQueryContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlUpdateParserVisitor*>(visitor))
    return parserVisitor->visitCreateIndexQuery(this);
  else
    return visitor->visitChildren(this);
}

SparqlUpdateParser::CreateIndexQueryContext* SparqlUpdateParser::createIndexQuery() {
  CreateIndexQueryContext *_localctx = _tracker.createInstance<CreateIndexQueryContext>(_ctx, getState());
  enterRule(_localctx, 10, SparqlUpdateParser::RuleCreateIndexQuery);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(126);
    match(SparqlUpdateParser::CREATE);
    setState(127);
    match(SparqlUpdateParser::ALPHANUMERIC_IDENTIFIER);
    setState(128);
    match(SparqlUpdateParser::INDEX);
    setState(129);
    string();
    setState(130);
    match(SparqlUpdateParser::WITH);
    setState(131);
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

SparqlUpdateParser::CreateIndexOptionsContext::CreateIndexOptionsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlUpdateParser::CreateIndexOptionsContext::OPEN_CURLY_BRACE() {
  return getToken(SparqlUpdateParser::OPEN_CURLY_BRACE, 0);
}

tree::TerminalNode* SparqlUpdateParser::CreateIndexOptionsContext::CLOSE_CURLY_BRACE() {
  return getToken(SparqlUpdateParser::CLOSE_CURLY_BRACE, 0);
}

std::vector<SparqlUpdateParser::CreateIndexOptionContext *> SparqlUpdateParser::CreateIndexOptionsContext::createIndexOption() {
  return getRuleContexts<SparqlUpdateParser::CreateIndexOptionContext>();
}

SparqlUpdateParser::CreateIndexOptionContext* SparqlUpdateParser::CreateIndexOptionsContext::createIndexOption(size_t i) {
  return getRuleContext<SparqlUpdateParser::CreateIndexOptionContext>(i);
}

std::vector<tree::TerminalNode *> SparqlUpdateParser::CreateIndexOptionsContext::COMMA() {
  return getTokens(SparqlUpdateParser::COMMA);
}

tree::TerminalNode* SparqlUpdateParser::CreateIndexOptionsContext::COMMA(size_t i) {
  return getToken(SparqlUpdateParser::COMMA, i);
}


size_t SparqlUpdateParser::CreateIndexOptionsContext::getRuleIndex() const {
  return SparqlUpdateParser::RuleCreateIndexOptions;
}


std::any SparqlUpdateParser::CreateIndexOptionsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlUpdateParserVisitor*>(visitor))
    return parserVisitor->visitCreateIndexOptions(this);
  else
    return visitor->visitChildren(this);
}

SparqlUpdateParser::CreateIndexOptionsContext* SparqlUpdateParser::createIndexOptions() {
  CreateIndexOptionsContext *_localctx = _tracker.createInstance<CreateIndexOptionsContext>(_ctx, getState());
  enterRule(_localctx, 12, SparqlUpdateParser::RuleCreateIndexOptions);
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
    setState(133);
    match(SparqlUpdateParser::OPEN_CURLY_BRACE);
    setState(142);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (((((_la - 138) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 138)) & 15) != 0)) {
      setState(134);
      createIndexOption();
      setState(139);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == SparqlUpdateParser::COMMA) {
        setState(135);
        match(SparqlUpdateParser::COMMA);
        setState(136);
        createIndexOption();
        setState(141);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
    }
    setState(144);
    match(SparqlUpdateParser::CLOSE_CURLY_BRACE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CreateIndexOptionContext ------------------------------------------------------------------

SparqlUpdateParser::CreateIndexOptionContext::CreateIndexOptionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlUpdateParser::StringContext* SparqlUpdateParser::CreateIndexOptionContext::string() {
  return getRuleContext<SparqlUpdateParser::StringContext>(0);
}

tree::TerminalNode* SparqlUpdateParser::CreateIndexOptionContext::EQUAL() {
  return getToken(SparqlUpdateParser::EQUAL, 0);
}

SparqlUpdateParser::GraphTermContext* SparqlUpdateParser::CreateIndexOptionContext::graphTerm() {
  return getRuleContext<SparqlUpdateParser::GraphTermContext>(0);
}


size_t SparqlUpdateParser::CreateIndexOptionContext::getRuleIndex() const {
  return SparqlUpdateParser::RuleCreateIndexOption;
}


std::any SparqlUpdateParser::CreateIndexOptionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlUpdateParserVisitor*>(visitor))
    return parserVisitor->visitCreateIndexOption(this);
  else
    return visitor->visitChildren(this);
}

SparqlUpdateParser::CreateIndexOptionContext* SparqlUpdateParser::createIndexOption() {
  CreateIndexOptionContext *_localctx = _tracker.createInstance<CreateIndexOptionContext>(_ctx, getState());
  enterRule(_localctx, 14, SparqlUpdateParser::RuleCreateIndexOption);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(146);
    string();
    setState(147);
    match(SparqlUpdateParser::EQUAL);
    setState(148);
    graphTerm();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- InsertDataContext ------------------------------------------------------------------

SparqlUpdateParser::InsertDataContext::InsertDataContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlUpdateParser::InsertDataContext::INSERT() {
  return getToken(SparqlUpdateParser::INSERT, 0);
}

tree::TerminalNode* SparqlUpdateParser::InsertDataContext::DATA() {
  return getToken(SparqlUpdateParser::DATA, 0);
}

SparqlUpdateParser::QuadDataContext* SparqlUpdateParser::InsertDataContext::quadData() {
  return getRuleContext<SparqlUpdateParser::QuadDataContext>(0);
}


size_t SparqlUpdateParser::InsertDataContext::getRuleIndex() const {
  return SparqlUpdateParser::RuleInsertData;
}


std::any SparqlUpdateParser::InsertDataContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlUpdateParserVisitor*>(visitor))
    return parserVisitor->visitInsertData(this);
  else
    return visitor->visitChildren(this);
}

SparqlUpdateParser::InsertDataContext* SparqlUpdateParser::insertData() {
  InsertDataContext *_localctx = _tracker.createInstance<InsertDataContext>(_ctx, getState());
  enterRule(_localctx, 16, SparqlUpdateParser::RuleInsertData);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(150);
    match(SparqlUpdateParser::INSERT);
    setState(151);
    match(SparqlUpdateParser::DATA);
    setState(152);
    quadData();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DeleteDataContext ------------------------------------------------------------------

SparqlUpdateParser::DeleteDataContext::DeleteDataContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlUpdateParser::DeleteDataContext::DELETE() {
  return getToken(SparqlUpdateParser::DELETE, 0);
}

tree::TerminalNode* SparqlUpdateParser::DeleteDataContext::DATA() {
  return getToken(SparqlUpdateParser::DATA, 0);
}

SparqlUpdateParser::QuadDataContext* SparqlUpdateParser::DeleteDataContext::quadData() {
  return getRuleContext<SparqlUpdateParser::QuadDataContext>(0);
}


size_t SparqlUpdateParser::DeleteDataContext::getRuleIndex() const {
  return SparqlUpdateParser::RuleDeleteData;
}


std::any SparqlUpdateParser::DeleteDataContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlUpdateParserVisitor*>(visitor))
    return parserVisitor->visitDeleteData(this);
  else
    return visitor->visitChildren(this);
}

SparqlUpdateParser::DeleteDataContext* SparqlUpdateParser::deleteData() {
  DeleteDataContext *_localctx = _tracker.createInstance<DeleteDataContext>(_ctx, getState());
  enterRule(_localctx, 18, SparqlUpdateParser::RuleDeleteData);

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
    match(SparqlUpdateParser::DELETE);
    setState(155);
    match(SparqlUpdateParser::DATA);
    setState(156);
    quadData();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- QuadDataContext ------------------------------------------------------------------

SparqlUpdateParser::QuadDataContext::QuadDataContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlUpdateParser::QuadDataContext::OPEN_CURLY_BRACE() {
  return getToken(SparqlUpdateParser::OPEN_CURLY_BRACE, 0);
}

SparqlUpdateParser::TriplesTemplateContext* SparqlUpdateParser::QuadDataContext::triplesTemplate() {
  return getRuleContext<SparqlUpdateParser::TriplesTemplateContext>(0);
}

tree::TerminalNode* SparqlUpdateParser::QuadDataContext::CLOSE_CURLY_BRACE() {
  return getToken(SparqlUpdateParser::CLOSE_CURLY_BRACE, 0);
}


size_t SparqlUpdateParser::QuadDataContext::getRuleIndex() const {
  return SparqlUpdateParser::RuleQuadData;
}


std::any SparqlUpdateParser::QuadDataContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlUpdateParserVisitor*>(visitor))
    return parserVisitor->visitQuadData(this);
  else
    return visitor->visitChildren(this);
}

SparqlUpdateParser::QuadDataContext* SparqlUpdateParser::quadData() {
  QuadDataContext *_localctx = _tracker.createInstance<QuadDataContext>(_ctx, getState());
  enterRule(_localctx, 20, SparqlUpdateParser::RuleQuadData);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(158);
    match(SparqlUpdateParser::OPEN_CURLY_BRACE);
    setState(159);
    triplesTemplate();
    setState(160);
    match(SparqlUpdateParser::CLOSE_CURLY_BRACE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TriplesTemplateContext ------------------------------------------------------------------

SparqlUpdateParser::TriplesTemplateContext::TriplesTemplateContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<SparqlUpdateParser::TriplesSameSubjectContext *> SparqlUpdateParser::TriplesTemplateContext::triplesSameSubject() {
  return getRuleContexts<SparqlUpdateParser::TriplesSameSubjectContext>();
}

SparqlUpdateParser::TriplesSameSubjectContext* SparqlUpdateParser::TriplesTemplateContext::triplesSameSubject(size_t i) {
  return getRuleContext<SparqlUpdateParser::TriplesSameSubjectContext>(i);
}

std::vector<tree::TerminalNode *> SparqlUpdateParser::TriplesTemplateContext::DOT() {
  return getTokens(SparqlUpdateParser::DOT);
}

tree::TerminalNode* SparqlUpdateParser::TriplesTemplateContext::DOT(size_t i) {
  return getToken(SparqlUpdateParser::DOT, i);
}


size_t SparqlUpdateParser::TriplesTemplateContext::getRuleIndex() const {
  return SparqlUpdateParser::RuleTriplesTemplate;
}


std::any SparqlUpdateParser::TriplesTemplateContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlUpdateParserVisitor*>(visitor))
    return parserVisitor->visitTriplesTemplate(this);
  else
    return visitor->visitChildren(this);
}

SparqlUpdateParser::TriplesTemplateContext* SparqlUpdateParser::triplesTemplate() {
  TriplesTemplateContext *_localctx = _tracker.createInstance<TriplesTemplateContext>(_ctx, getState());
  enterRule(_localctx, 22, SparqlUpdateParser::RuleTriplesTemplate);
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
    setState(162);
    triplesSameSubject();
    setState(169);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SparqlUpdateParser::DOT) {
      setState(163);
      match(SparqlUpdateParser::DOT);
      setState(165);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == SparqlUpdateParser::K_TRUE

      || _la == SparqlUpdateParser::K_FALSE || ((((_la - 122) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 122)) & 4564451215) != 0)) {
        setState(164);
        triplesSameSubject();
      }
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

//----------------- ConstructTemplateContext ------------------------------------------------------------------

SparqlUpdateParser::ConstructTemplateContext::ConstructTemplateContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlUpdateParser::ConstructTemplateContext::OPEN_CURLY_BRACE() {
  return getToken(SparqlUpdateParser::OPEN_CURLY_BRACE, 0);
}

tree::TerminalNode* SparqlUpdateParser::ConstructTemplateContext::CLOSE_CURLY_BRACE() {
  return getToken(SparqlUpdateParser::CLOSE_CURLY_BRACE, 0);
}

SparqlUpdateParser::ConstructTriplesContext* SparqlUpdateParser::ConstructTemplateContext::constructTriples() {
  return getRuleContext<SparqlUpdateParser::ConstructTriplesContext>(0);
}


size_t SparqlUpdateParser::ConstructTemplateContext::getRuleIndex() const {
  return SparqlUpdateParser::RuleConstructTemplate;
}


std::any SparqlUpdateParser::ConstructTemplateContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlUpdateParserVisitor*>(visitor))
    return parserVisitor->visitConstructTemplate(this);
  else
    return visitor->visitChildren(this);
}

SparqlUpdateParser::ConstructTemplateContext* SparqlUpdateParser::constructTemplate() {
  ConstructTemplateContext *_localctx = _tracker.createInstance<ConstructTemplateContext>(_ctx, getState());
  enterRule(_localctx, 24, SparqlUpdateParser::RuleConstructTemplate);
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
    match(SparqlUpdateParser::OPEN_CURLY_BRACE);
    setState(174);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == SparqlUpdateParser::K_TRUE

    || _la == SparqlUpdateParser::K_FALSE || ((((_la - 122) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 122)) & 4564451215) != 0)) {
      setState(173);
      constructTriples();
    }
    setState(176);
    match(SparqlUpdateParser::CLOSE_CURLY_BRACE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ConstructTriplesContext ------------------------------------------------------------------

SparqlUpdateParser::ConstructTriplesContext::ConstructTriplesContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlUpdateParser::TriplesSameSubjectContext* SparqlUpdateParser::ConstructTriplesContext::triplesSameSubject() {
  return getRuleContext<SparqlUpdateParser::TriplesSameSubjectContext>(0);
}

std::vector<tree::TerminalNode *> SparqlUpdateParser::ConstructTriplesContext::DOT() {
  return getTokens(SparqlUpdateParser::DOT);
}

tree::TerminalNode* SparqlUpdateParser::ConstructTriplesContext::DOT(size_t i) {
  return getToken(SparqlUpdateParser::DOT, i);
}

std::vector<SparqlUpdateParser::ConstructTriplesContext *> SparqlUpdateParser::ConstructTriplesContext::constructTriples() {
  return getRuleContexts<SparqlUpdateParser::ConstructTriplesContext>();
}

SparqlUpdateParser::ConstructTriplesContext* SparqlUpdateParser::ConstructTriplesContext::constructTriples(size_t i) {
  return getRuleContext<SparqlUpdateParser::ConstructTriplesContext>(i);
}


size_t SparqlUpdateParser::ConstructTriplesContext::getRuleIndex() const {
  return SparqlUpdateParser::RuleConstructTriples;
}


std::any SparqlUpdateParser::ConstructTriplesContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlUpdateParserVisitor*>(visitor))
    return parserVisitor->visitConstructTriples(this);
  else
    return visitor->visitChildren(this);
}

SparqlUpdateParser::ConstructTriplesContext* SparqlUpdateParser::constructTriples() {
  ConstructTriplesContext *_localctx = _tracker.createInstance<ConstructTriplesContext>(_ctx, getState());
  enterRule(_localctx, 26, SparqlUpdateParser::RuleConstructTriples);
  size_t _la = 0;

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
    setState(178);
    triplesSameSubject();
    setState(185);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 11, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(179);
        match(SparqlUpdateParser::DOT);
        setState(181);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == SparqlUpdateParser::K_TRUE

        || _la == SparqlUpdateParser::K_FALSE || ((((_la - 122) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 122)) & 4564451215) != 0)) {
          setState(180);
          constructTriples();
        } 
      }
      setState(187);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 11, _ctx);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TriplesSameSubjectContext ------------------------------------------------------------------

SparqlUpdateParser::TriplesSameSubjectContext::TriplesSameSubjectContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlUpdateParser::GraphTermContext* SparqlUpdateParser::TriplesSameSubjectContext::graphTerm() {
  return getRuleContext<SparqlUpdateParser::GraphTermContext>(0);
}

SparqlUpdateParser::PropertyListNotEmptyContext* SparqlUpdateParser::TriplesSameSubjectContext::propertyListNotEmpty() {
  return getRuleContext<SparqlUpdateParser::PropertyListNotEmptyContext>(0);
}

SparqlUpdateParser::TriplesNodeContext* SparqlUpdateParser::TriplesSameSubjectContext::triplesNode() {
  return getRuleContext<SparqlUpdateParser::TriplesNodeContext>(0);
}

SparqlUpdateParser::PropertyListContext* SparqlUpdateParser::TriplesSameSubjectContext::propertyList() {
  return getRuleContext<SparqlUpdateParser::PropertyListContext>(0);
}


size_t SparqlUpdateParser::TriplesSameSubjectContext::getRuleIndex() const {
  return SparqlUpdateParser::RuleTriplesSameSubject;
}


std::any SparqlUpdateParser::TriplesSameSubjectContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlUpdateParserVisitor*>(visitor))
    return parserVisitor->visitTriplesSameSubject(this);
  else
    return visitor->visitChildren(this);
}

SparqlUpdateParser::TriplesSameSubjectContext* SparqlUpdateParser::triplesSameSubject() {
  TriplesSameSubjectContext *_localctx = _tracker.createInstance<TriplesSameSubjectContext>(_ctx, getState());
  enterRule(_localctx, 28, SparqlUpdateParser::RuleTriplesSameSubject);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(194);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 12, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(188);
      graphTerm();
      setState(189);
      propertyListNotEmpty();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(191);
      triplesNode();
      setState(192);
      propertyList();
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

//----------------- PropertyListContext ------------------------------------------------------------------

SparqlUpdateParser::PropertyListContext::PropertyListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlUpdateParser::PropertyListNotEmptyContext* SparqlUpdateParser::PropertyListContext::propertyListNotEmpty() {
  return getRuleContext<SparqlUpdateParser::PropertyListNotEmptyContext>(0);
}


size_t SparqlUpdateParser::PropertyListContext::getRuleIndex() const {
  return SparqlUpdateParser::RulePropertyList;
}


std::any SparqlUpdateParser::PropertyListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlUpdateParserVisitor*>(visitor))
    return parserVisitor->visitPropertyList(this);
  else
    return visitor->visitChildren(this);
}

SparqlUpdateParser::PropertyListContext* SparqlUpdateParser::propertyList() {
  PropertyListContext *_localctx = _tracker.createInstance<PropertyListContext>(_ctx, getState());
  enterRule(_localctx, 30, SparqlUpdateParser::RulePropertyList);
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
    setState(197);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == SparqlUpdateParser::A || ((((_la - 122) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 122)) & 7) != 0)) {
      setState(196);
      propertyListNotEmpty();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PropertyListNotEmptyContext ------------------------------------------------------------------

SparqlUpdateParser::PropertyListNotEmptyContext::PropertyListNotEmptyContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<SparqlUpdateParser::VerbContext *> SparqlUpdateParser::PropertyListNotEmptyContext::verb() {
  return getRuleContexts<SparqlUpdateParser::VerbContext>();
}

SparqlUpdateParser::VerbContext* SparqlUpdateParser::PropertyListNotEmptyContext::verb(size_t i) {
  return getRuleContext<SparqlUpdateParser::VerbContext>(i);
}

std::vector<SparqlUpdateParser::ObjectListContext *> SparqlUpdateParser::PropertyListNotEmptyContext::objectList() {
  return getRuleContexts<SparqlUpdateParser::ObjectListContext>();
}

SparqlUpdateParser::ObjectListContext* SparqlUpdateParser::PropertyListNotEmptyContext::objectList(size_t i) {
  return getRuleContext<SparqlUpdateParser::ObjectListContext>(i);
}

std::vector<tree::TerminalNode *> SparqlUpdateParser::PropertyListNotEmptyContext::SEMICOLON() {
  return getTokens(SparqlUpdateParser::SEMICOLON);
}

tree::TerminalNode* SparqlUpdateParser::PropertyListNotEmptyContext::SEMICOLON(size_t i) {
  return getToken(SparqlUpdateParser::SEMICOLON, i);
}


size_t SparqlUpdateParser::PropertyListNotEmptyContext::getRuleIndex() const {
  return SparqlUpdateParser::RulePropertyListNotEmpty;
}


std::any SparqlUpdateParser::PropertyListNotEmptyContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlUpdateParserVisitor*>(visitor))
    return parserVisitor->visitPropertyListNotEmpty(this);
  else
    return visitor->visitChildren(this);
}

SparqlUpdateParser::PropertyListNotEmptyContext* SparqlUpdateParser::propertyListNotEmpty() {
  PropertyListNotEmptyContext *_localctx = _tracker.createInstance<PropertyListNotEmptyContext>(_ctx, getState());
  enterRule(_localctx, 32, SparqlUpdateParser::RulePropertyListNotEmpty);
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
    verb();
    setState(200);
    objectList();
    setState(209);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SparqlUpdateParser::SEMICOLON) {
      setState(201);
      match(SparqlUpdateParser::SEMICOLON);
      setState(205);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == SparqlUpdateParser::A || ((((_la - 122) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 122)) & 7) != 0)) {
        setState(202);
        verb();
        setState(203);
        objectList();
      }
      setState(211);
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

//----------------- VerbContext ------------------------------------------------------------------

SparqlUpdateParser::VerbContext::VerbContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlUpdateParser::IriContext* SparqlUpdateParser::VerbContext::iri() {
  return getRuleContext<SparqlUpdateParser::IriContext>(0);
}

tree::TerminalNode* SparqlUpdateParser::VerbContext::A() {
  return getToken(SparqlUpdateParser::A, 0);
}


size_t SparqlUpdateParser::VerbContext::getRuleIndex() const {
  return SparqlUpdateParser::RuleVerb;
}


std::any SparqlUpdateParser::VerbContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlUpdateParserVisitor*>(visitor))
    return parserVisitor->visitVerb(this);
  else
    return visitor->visitChildren(this);
}

SparqlUpdateParser::VerbContext* SparqlUpdateParser::verb() {
  VerbContext *_localctx = _tracker.createInstance<VerbContext>(_ctx, getState());
  enterRule(_localctx, 34, SparqlUpdateParser::RuleVerb);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(214);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlUpdateParser::IRIREF:
      case SparqlUpdateParser::PNAME_NS:
      case SparqlUpdateParser::PNAME_LN: {
        enterOuterAlt(_localctx, 1);
        setState(212);
        iri();
        break;
      }

      case SparqlUpdateParser::A: {
        enterOuterAlt(_localctx, 2);
        setState(213);
        match(SparqlUpdateParser::A);
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

//----------------- ObjectListContext ------------------------------------------------------------------

SparqlUpdateParser::ObjectListContext::ObjectListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<SparqlUpdateParser::ObjectContext *> SparqlUpdateParser::ObjectListContext::object() {
  return getRuleContexts<SparqlUpdateParser::ObjectContext>();
}

SparqlUpdateParser::ObjectContext* SparqlUpdateParser::ObjectListContext::object(size_t i) {
  return getRuleContext<SparqlUpdateParser::ObjectContext>(i);
}

std::vector<tree::TerminalNode *> SparqlUpdateParser::ObjectListContext::COMMA() {
  return getTokens(SparqlUpdateParser::COMMA);
}

tree::TerminalNode* SparqlUpdateParser::ObjectListContext::COMMA(size_t i) {
  return getToken(SparqlUpdateParser::COMMA, i);
}


size_t SparqlUpdateParser::ObjectListContext::getRuleIndex() const {
  return SparqlUpdateParser::RuleObjectList;
}


std::any SparqlUpdateParser::ObjectListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlUpdateParserVisitor*>(visitor))
    return parserVisitor->visitObjectList(this);
  else
    return visitor->visitChildren(this);
}

SparqlUpdateParser::ObjectListContext* SparqlUpdateParser::objectList() {
  ObjectListContext *_localctx = _tracker.createInstance<ObjectListContext>(_ctx, getState());
  enterRule(_localctx, 36, SparqlUpdateParser::RuleObjectList);
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
    setState(216);
    object();
    setState(221);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SparqlUpdateParser::COMMA) {
      setState(217);
      match(SparqlUpdateParser::COMMA);
      setState(218);
      object();
      setState(223);
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

//----------------- ObjectContext ------------------------------------------------------------------

SparqlUpdateParser::ObjectContext::ObjectContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlUpdateParser::GraphNodeContext* SparqlUpdateParser::ObjectContext::graphNode() {
  return getRuleContext<SparqlUpdateParser::GraphNodeContext>(0);
}


size_t SparqlUpdateParser::ObjectContext::getRuleIndex() const {
  return SparqlUpdateParser::RuleObject;
}


std::any SparqlUpdateParser::ObjectContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlUpdateParserVisitor*>(visitor))
    return parserVisitor->visitObject(this);
  else
    return visitor->visitChildren(this);
}

SparqlUpdateParser::ObjectContext* SparqlUpdateParser::object() {
  ObjectContext *_localctx = _tracker.createInstance<ObjectContext>(_ctx, getState());
  enterRule(_localctx, 38, SparqlUpdateParser::RuleObject);

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
    graphNode();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PropertyListPathNotEmptyContext ------------------------------------------------------------------

SparqlUpdateParser::PropertyListPathNotEmptyContext::PropertyListPathNotEmptyContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlUpdateParser::VerbContext* SparqlUpdateParser::PropertyListPathNotEmptyContext::verb() {
  return getRuleContext<SparqlUpdateParser::VerbContext>(0);
}

SparqlUpdateParser::ObjectListPathContext* SparqlUpdateParser::PropertyListPathNotEmptyContext::objectListPath() {
  return getRuleContext<SparqlUpdateParser::ObjectListPathContext>(0);
}

std::vector<tree::TerminalNode *> SparqlUpdateParser::PropertyListPathNotEmptyContext::SEMICOLON() {
  return getTokens(SparqlUpdateParser::SEMICOLON);
}

tree::TerminalNode* SparqlUpdateParser::PropertyListPathNotEmptyContext::SEMICOLON(size_t i) {
  return getToken(SparqlUpdateParser::SEMICOLON, i);
}

std::vector<SparqlUpdateParser::PropertyListPathNotEmptyListContext *> SparqlUpdateParser::PropertyListPathNotEmptyContext::propertyListPathNotEmptyList() {
  return getRuleContexts<SparqlUpdateParser::PropertyListPathNotEmptyListContext>();
}

SparqlUpdateParser::PropertyListPathNotEmptyListContext* SparqlUpdateParser::PropertyListPathNotEmptyContext::propertyListPathNotEmptyList(size_t i) {
  return getRuleContext<SparqlUpdateParser::PropertyListPathNotEmptyListContext>(i);
}


size_t SparqlUpdateParser::PropertyListPathNotEmptyContext::getRuleIndex() const {
  return SparqlUpdateParser::RulePropertyListPathNotEmpty;
}


std::any SparqlUpdateParser::PropertyListPathNotEmptyContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlUpdateParserVisitor*>(visitor))
    return parserVisitor->visitPropertyListPathNotEmpty(this);
  else
    return visitor->visitChildren(this);
}

SparqlUpdateParser::PropertyListPathNotEmptyContext* SparqlUpdateParser::propertyListPathNotEmpty() {
  PropertyListPathNotEmptyContext *_localctx = _tracker.createInstance<PropertyListPathNotEmptyContext>(_ctx, getState());
  enterRule(_localctx, 40, SparqlUpdateParser::RulePropertyListPathNotEmpty);
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
    setState(226);
    verb();
    setState(227);
    objectListPath();
    setState(234);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SparqlUpdateParser::SEMICOLON) {
      setState(228);
      match(SparqlUpdateParser::SEMICOLON);
      setState(230);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == SparqlUpdateParser::A || ((((_la - 122) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 122)) & 7) != 0)) {
        setState(229);
        propertyListPathNotEmptyList();
      }
      setState(236);
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

//----------------- PropertyListPathNotEmptyListContext ------------------------------------------------------------------

SparqlUpdateParser::PropertyListPathNotEmptyListContext::PropertyListPathNotEmptyListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlUpdateParser::VerbContext* SparqlUpdateParser::PropertyListPathNotEmptyListContext::verb() {
  return getRuleContext<SparqlUpdateParser::VerbContext>(0);
}

SparqlUpdateParser::ObjectListContext* SparqlUpdateParser::PropertyListPathNotEmptyListContext::objectList() {
  return getRuleContext<SparqlUpdateParser::ObjectListContext>(0);
}


size_t SparqlUpdateParser::PropertyListPathNotEmptyListContext::getRuleIndex() const {
  return SparqlUpdateParser::RulePropertyListPathNotEmptyList;
}


std::any SparqlUpdateParser::PropertyListPathNotEmptyListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlUpdateParserVisitor*>(visitor))
    return parserVisitor->visitPropertyListPathNotEmptyList(this);
  else
    return visitor->visitChildren(this);
}

SparqlUpdateParser::PropertyListPathNotEmptyListContext* SparqlUpdateParser::propertyListPathNotEmptyList() {
  PropertyListPathNotEmptyListContext *_localctx = _tracker.createInstance<PropertyListPathNotEmptyListContext>(_ctx, getState());
  enterRule(_localctx, 42, SparqlUpdateParser::RulePropertyListPathNotEmptyList);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(237);
    verb();
    setState(238);
    objectList();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ObjectListPathContext ------------------------------------------------------------------

SparqlUpdateParser::ObjectListPathContext::ObjectListPathContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<SparqlUpdateParser::ObjectPathContext *> SparqlUpdateParser::ObjectListPathContext::objectPath() {
  return getRuleContexts<SparqlUpdateParser::ObjectPathContext>();
}

SparqlUpdateParser::ObjectPathContext* SparqlUpdateParser::ObjectListPathContext::objectPath(size_t i) {
  return getRuleContext<SparqlUpdateParser::ObjectPathContext>(i);
}

std::vector<tree::TerminalNode *> SparqlUpdateParser::ObjectListPathContext::COMMA() {
  return getTokens(SparqlUpdateParser::COMMA);
}

tree::TerminalNode* SparqlUpdateParser::ObjectListPathContext::COMMA(size_t i) {
  return getToken(SparqlUpdateParser::COMMA, i);
}


size_t SparqlUpdateParser::ObjectListPathContext::getRuleIndex() const {
  return SparqlUpdateParser::RuleObjectListPath;
}


std::any SparqlUpdateParser::ObjectListPathContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlUpdateParserVisitor*>(visitor))
    return parserVisitor->visitObjectListPath(this);
  else
    return visitor->visitChildren(this);
}

SparqlUpdateParser::ObjectListPathContext* SparqlUpdateParser::objectListPath() {
  ObjectListPathContext *_localctx = _tracker.createInstance<ObjectListPathContext>(_ctx, getState());
  enterRule(_localctx, 44, SparqlUpdateParser::RuleObjectListPath);
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
    setState(240);
    objectPath();
    setState(245);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SparqlUpdateParser::COMMA) {
      setState(241);
      match(SparqlUpdateParser::COMMA);
      setState(242);
      objectPath();
      setState(247);
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

//----------------- ObjectPathContext ------------------------------------------------------------------

SparqlUpdateParser::ObjectPathContext::ObjectPathContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlUpdateParser::GraphNodePathContext* SparqlUpdateParser::ObjectPathContext::graphNodePath() {
  return getRuleContext<SparqlUpdateParser::GraphNodePathContext>(0);
}


size_t SparqlUpdateParser::ObjectPathContext::getRuleIndex() const {
  return SparqlUpdateParser::RuleObjectPath;
}


std::any SparqlUpdateParser::ObjectPathContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlUpdateParserVisitor*>(visitor))
    return parserVisitor->visitObjectPath(this);
  else
    return visitor->visitChildren(this);
}

SparqlUpdateParser::ObjectPathContext* SparqlUpdateParser::objectPath() {
  ObjectPathContext *_localctx = _tracker.createInstance<ObjectPathContext>(_ctx, getState());
  enterRule(_localctx, 46, SparqlUpdateParser::RuleObjectPath);

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
    graphNodePath();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- IntegerContext ------------------------------------------------------------------

SparqlUpdateParser::IntegerContext::IntegerContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlUpdateParser::IntegerContext::INTEGER() {
  return getToken(SparqlUpdateParser::INTEGER, 0);
}


size_t SparqlUpdateParser::IntegerContext::getRuleIndex() const {
  return SparqlUpdateParser::RuleInteger;
}


std::any SparqlUpdateParser::IntegerContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlUpdateParserVisitor*>(visitor))
    return parserVisitor->visitInteger(this);
  else
    return visitor->visitChildren(this);
}

SparqlUpdateParser::IntegerContext* SparqlUpdateParser::integer() {
  IntegerContext *_localctx = _tracker.createInstance<IntegerContext>(_ctx, getState());
  enterRule(_localctx, 48, SparqlUpdateParser::RuleInteger);

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
    match(SparqlUpdateParser::INTEGER);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TriplesNodeContext ------------------------------------------------------------------

SparqlUpdateParser::TriplesNodeContext::TriplesNodeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlUpdateParser::CollectionContext* SparqlUpdateParser::TriplesNodeContext::collection() {
  return getRuleContext<SparqlUpdateParser::CollectionContext>(0);
}

SparqlUpdateParser::BlankNodePropertyListContext* SparqlUpdateParser::TriplesNodeContext::blankNodePropertyList() {
  return getRuleContext<SparqlUpdateParser::BlankNodePropertyListContext>(0);
}


size_t SparqlUpdateParser::TriplesNodeContext::getRuleIndex() const {
  return SparqlUpdateParser::RuleTriplesNode;
}


std::any SparqlUpdateParser::TriplesNodeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlUpdateParserVisitor*>(visitor))
    return parserVisitor->visitTriplesNode(this);
  else
    return visitor->visitChildren(this);
}

SparqlUpdateParser::TriplesNodeContext* SparqlUpdateParser::triplesNode() {
  TriplesNodeContext *_localctx = _tracker.createInstance<TriplesNodeContext>(_ctx, getState());
  enterRule(_localctx, 50, SparqlUpdateParser::RuleTriplesNode);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(254);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlUpdateParser::OPEN_BRACE: {
        enterOuterAlt(_localctx, 1);
        setState(252);
        collection();
        break;
      }

      case SparqlUpdateParser::OPEN_SQUARE_BRACKET: {
        enterOuterAlt(_localctx, 2);
        setState(253);
        blankNodePropertyList();
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

//----------------- BlankNodePropertyListContext ------------------------------------------------------------------

SparqlUpdateParser::BlankNodePropertyListContext::BlankNodePropertyListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlUpdateParser::BlankNodePropertyListContext::OPEN_SQUARE_BRACKET() {
  return getToken(SparqlUpdateParser::OPEN_SQUARE_BRACKET, 0);
}

SparqlUpdateParser::PropertyListNotEmptyContext* SparqlUpdateParser::BlankNodePropertyListContext::propertyListNotEmpty() {
  return getRuleContext<SparqlUpdateParser::PropertyListNotEmptyContext>(0);
}

tree::TerminalNode* SparqlUpdateParser::BlankNodePropertyListContext::CLOSE_SQUARE_BRACKET() {
  return getToken(SparqlUpdateParser::CLOSE_SQUARE_BRACKET, 0);
}


size_t SparqlUpdateParser::BlankNodePropertyListContext::getRuleIndex() const {
  return SparqlUpdateParser::RuleBlankNodePropertyList;
}


std::any SparqlUpdateParser::BlankNodePropertyListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlUpdateParserVisitor*>(visitor))
    return parserVisitor->visitBlankNodePropertyList(this);
  else
    return visitor->visitChildren(this);
}

SparqlUpdateParser::BlankNodePropertyListContext* SparqlUpdateParser::blankNodePropertyList() {
  BlankNodePropertyListContext *_localctx = _tracker.createInstance<BlankNodePropertyListContext>(_ctx, getState());
  enterRule(_localctx, 52, SparqlUpdateParser::RuleBlankNodePropertyList);

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
    match(SparqlUpdateParser::OPEN_SQUARE_BRACKET);
    setState(257);
    propertyListNotEmpty();
    setState(258);
    match(SparqlUpdateParser::CLOSE_SQUARE_BRACKET);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TriplesNodePathContext ------------------------------------------------------------------

SparqlUpdateParser::TriplesNodePathContext::TriplesNodePathContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlUpdateParser::CollectionPathContext* SparqlUpdateParser::TriplesNodePathContext::collectionPath() {
  return getRuleContext<SparqlUpdateParser::CollectionPathContext>(0);
}

SparqlUpdateParser::BlankNodePropertyListPathContext* SparqlUpdateParser::TriplesNodePathContext::blankNodePropertyListPath() {
  return getRuleContext<SparqlUpdateParser::BlankNodePropertyListPathContext>(0);
}


size_t SparqlUpdateParser::TriplesNodePathContext::getRuleIndex() const {
  return SparqlUpdateParser::RuleTriplesNodePath;
}


std::any SparqlUpdateParser::TriplesNodePathContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlUpdateParserVisitor*>(visitor))
    return parserVisitor->visitTriplesNodePath(this);
  else
    return visitor->visitChildren(this);
}

SparqlUpdateParser::TriplesNodePathContext* SparqlUpdateParser::triplesNodePath() {
  TriplesNodePathContext *_localctx = _tracker.createInstance<TriplesNodePathContext>(_ctx, getState());
  enterRule(_localctx, 54, SparqlUpdateParser::RuleTriplesNodePath);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(262);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlUpdateParser::OPEN_BRACE: {
        enterOuterAlt(_localctx, 1);
        setState(260);
        collectionPath();
        break;
      }

      case SparqlUpdateParser::OPEN_SQUARE_BRACKET: {
        enterOuterAlt(_localctx, 2);
        setState(261);
        blankNodePropertyListPath();
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

//----------------- BlankNodePropertyListPathContext ------------------------------------------------------------------

SparqlUpdateParser::BlankNodePropertyListPathContext::BlankNodePropertyListPathContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlUpdateParser::BlankNodePropertyListPathContext::OPEN_SQUARE_BRACKET() {
  return getToken(SparqlUpdateParser::OPEN_SQUARE_BRACKET, 0);
}

SparqlUpdateParser::PropertyListPathNotEmptyContext* SparqlUpdateParser::BlankNodePropertyListPathContext::propertyListPathNotEmpty() {
  return getRuleContext<SparqlUpdateParser::PropertyListPathNotEmptyContext>(0);
}

tree::TerminalNode* SparqlUpdateParser::BlankNodePropertyListPathContext::CLOSE_SQUARE_BRACKET() {
  return getToken(SparqlUpdateParser::CLOSE_SQUARE_BRACKET, 0);
}


size_t SparqlUpdateParser::BlankNodePropertyListPathContext::getRuleIndex() const {
  return SparqlUpdateParser::RuleBlankNodePropertyListPath;
}


std::any SparqlUpdateParser::BlankNodePropertyListPathContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlUpdateParserVisitor*>(visitor))
    return parserVisitor->visitBlankNodePropertyListPath(this);
  else
    return visitor->visitChildren(this);
}

SparqlUpdateParser::BlankNodePropertyListPathContext* SparqlUpdateParser::blankNodePropertyListPath() {
  BlankNodePropertyListPathContext *_localctx = _tracker.createInstance<BlankNodePropertyListPathContext>(_ctx, getState());
  enterRule(_localctx, 56, SparqlUpdateParser::RuleBlankNodePropertyListPath);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(264);
    match(SparqlUpdateParser::OPEN_SQUARE_BRACKET);
    setState(265);
    propertyListPathNotEmpty();
    setState(266);
    match(SparqlUpdateParser::CLOSE_SQUARE_BRACKET);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CollectionContext ------------------------------------------------------------------

SparqlUpdateParser::CollectionContext::CollectionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlUpdateParser::CollectionContext::OPEN_BRACE() {
  return getToken(SparqlUpdateParser::OPEN_BRACE, 0);
}

tree::TerminalNode* SparqlUpdateParser::CollectionContext::CLOSE_BRACE() {
  return getToken(SparqlUpdateParser::CLOSE_BRACE, 0);
}

std::vector<SparqlUpdateParser::GraphNodeContext *> SparqlUpdateParser::CollectionContext::graphNode() {
  return getRuleContexts<SparqlUpdateParser::GraphNodeContext>();
}

SparqlUpdateParser::GraphNodeContext* SparqlUpdateParser::CollectionContext::graphNode(size_t i) {
  return getRuleContext<SparqlUpdateParser::GraphNodeContext>(i);
}


size_t SparqlUpdateParser::CollectionContext::getRuleIndex() const {
  return SparqlUpdateParser::RuleCollection;
}


std::any SparqlUpdateParser::CollectionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlUpdateParserVisitor*>(visitor))
    return parserVisitor->visitCollection(this);
  else
    return visitor->visitChildren(this);
}

SparqlUpdateParser::CollectionContext* SparqlUpdateParser::collection() {
  CollectionContext *_localctx = _tracker.createInstance<CollectionContext>(_ctx, getState());
  enterRule(_localctx, 58, SparqlUpdateParser::RuleCollection);
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
    match(SparqlUpdateParser::OPEN_BRACE);
    setState(270); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(269);
      graphNode();
      setState(272); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while (_la == SparqlUpdateParser::K_TRUE

    || _la == SparqlUpdateParser::K_FALSE || ((((_la - 122) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 122)) & 4564451215) != 0));
    setState(274);
    match(SparqlUpdateParser::CLOSE_BRACE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CollectionPathContext ------------------------------------------------------------------

SparqlUpdateParser::CollectionPathContext::CollectionPathContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlUpdateParser::CollectionPathContext::OPEN_BRACE() {
  return getToken(SparqlUpdateParser::OPEN_BRACE, 0);
}

tree::TerminalNode* SparqlUpdateParser::CollectionPathContext::CLOSE_BRACE() {
  return getToken(SparqlUpdateParser::CLOSE_BRACE, 0);
}

std::vector<SparqlUpdateParser::GraphNodePathContext *> SparqlUpdateParser::CollectionPathContext::graphNodePath() {
  return getRuleContexts<SparqlUpdateParser::GraphNodePathContext>();
}

SparqlUpdateParser::GraphNodePathContext* SparqlUpdateParser::CollectionPathContext::graphNodePath(size_t i) {
  return getRuleContext<SparqlUpdateParser::GraphNodePathContext>(i);
}


size_t SparqlUpdateParser::CollectionPathContext::getRuleIndex() const {
  return SparqlUpdateParser::RuleCollectionPath;
}


std::any SparqlUpdateParser::CollectionPathContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlUpdateParserVisitor*>(visitor))
    return parserVisitor->visitCollectionPath(this);
  else
    return visitor->visitChildren(this);
}

SparqlUpdateParser::CollectionPathContext* SparqlUpdateParser::collectionPath() {
  CollectionPathContext *_localctx = _tracker.createInstance<CollectionPathContext>(_ctx, getState());
  enterRule(_localctx, 60, SparqlUpdateParser::RuleCollectionPath);
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
    setState(276);
    match(SparqlUpdateParser::OPEN_BRACE);
    setState(278); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(277);
      graphNodePath();
      setState(280); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while (_la == SparqlUpdateParser::K_TRUE

    || _la == SparqlUpdateParser::K_FALSE || ((((_la - 122) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 122)) & 4564451215) != 0));
    setState(282);
    match(SparqlUpdateParser::CLOSE_BRACE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- GraphNodeContext ------------------------------------------------------------------

SparqlUpdateParser::GraphNodeContext::GraphNodeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlUpdateParser::GraphTermContext* SparqlUpdateParser::GraphNodeContext::graphTerm() {
  return getRuleContext<SparqlUpdateParser::GraphTermContext>(0);
}

SparqlUpdateParser::TriplesNodeContext* SparqlUpdateParser::GraphNodeContext::triplesNode() {
  return getRuleContext<SparqlUpdateParser::TriplesNodeContext>(0);
}


size_t SparqlUpdateParser::GraphNodeContext::getRuleIndex() const {
  return SparqlUpdateParser::RuleGraphNode;
}


std::any SparqlUpdateParser::GraphNodeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlUpdateParserVisitor*>(visitor))
    return parserVisitor->visitGraphNode(this);
  else
    return visitor->visitChildren(this);
}

SparqlUpdateParser::GraphNodeContext* SparqlUpdateParser::graphNode() {
  GraphNodeContext *_localctx = _tracker.createInstance<GraphNodeContext>(_ctx, getState());
  enterRule(_localctx, 62, SparqlUpdateParser::RuleGraphNode);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(286);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 25, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(284);
      graphTerm();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(285);
      triplesNode();
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

//----------------- GraphNodePathContext ------------------------------------------------------------------

SparqlUpdateParser::GraphNodePathContext::GraphNodePathContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlUpdateParser::GraphTermContext* SparqlUpdateParser::GraphNodePathContext::graphTerm() {
  return getRuleContext<SparqlUpdateParser::GraphTermContext>(0);
}

SparqlUpdateParser::TriplesNodePathContext* SparqlUpdateParser::GraphNodePathContext::triplesNodePath() {
  return getRuleContext<SparqlUpdateParser::TriplesNodePathContext>(0);
}


size_t SparqlUpdateParser::GraphNodePathContext::getRuleIndex() const {
  return SparqlUpdateParser::RuleGraphNodePath;
}


std::any SparqlUpdateParser::GraphNodePathContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlUpdateParserVisitor*>(visitor))
    return parserVisitor->visitGraphNodePath(this);
  else
    return visitor->visitChildren(this);
}

SparqlUpdateParser::GraphNodePathContext* SparqlUpdateParser::graphNodePath() {
  GraphNodePathContext *_localctx = _tracker.createInstance<GraphNodePathContext>(_ctx, getState());
  enterRule(_localctx, 64, SparqlUpdateParser::RuleGraphNodePath);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(290);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 26, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(288);
      graphTerm();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(289);
      triplesNodePath();
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

//----------------- GraphTermContext ------------------------------------------------------------------

SparqlUpdateParser::GraphTermContext::GraphTermContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlUpdateParser::IriContext* SparqlUpdateParser::GraphTermContext::iri() {
  return getRuleContext<SparqlUpdateParser::IriContext>(0);
}

SparqlUpdateParser::RdfLiteralContext* SparqlUpdateParser::GraphTermContext::rdfLiteral() {
  return getRuleContext<SparqlUpdateParser::RdfLiteralContext>(0);
}

SparqlUpdateParser::NumericLiteralContext* SparqlUpdateParser::GraphTermContext::numericLiteral() {
  return getRuleContext<SparqlUpdateParser::NumericLiteralContext>(0);
}

SparqlUpdateParser::BooleanLiteralContext* SparqlUpdateParser::GraphTermContext::booleanLiteral() {
  return getRuleContext<SparqlUpdateParser::BooleanLiteralContext>(0);
}

SparqlUpdateParser::BlankNodeContext* SparqlUpdateParser::GraphTermContext::blankNode() {
  return getRuleContext<SparqlUpdateParser::BlankNodeContext>(0);
}

SparqlUpdateParser::NilContext* SparqlUpdateParser::GraphTermContext::nil() {
  return getRuleContext<SparqlUpdateParser::NilContext>(0);
}


size_t SparqlUpdateParser::GraphTermContext::getRuleIndex() const {
  return SparqlUpdateParser::RuleGraphTerm;
}


std::any SparqlUpdateParser::GraphTermContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlUpdateParserVisitor*>(visitor))
    return parserVisitor->visitGraphTerm(this);
  else
    return visitor->visitChildren(this);
}

SparqlUpdateParser::GraphTermContext* SparqlUpdateParser::graphTerm() {
  GraphTermContext *_localctx = _tracker.createInstance<GraphTermContext>(_ctx, getState());
  enterRule(_localctx, 66, SparqlUpdateParser::RuleGraphTerm);

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
      case SparqlUpdateParser::IRIREF:
      case SparqlUpdateParser::PNAME_NS:
      case SparqlUpdateParser::PNAME_LN: {
        enterOuterAlt(_localctx, 1);
        setState(292);
        iri();
        break;
      }

      case SparqlUpdateParser::STRING_LITERAL1:
      case SparqlUpdateParser::STRING_LITERAL2:
      case SparqlUpdateParser::STRING_LITERAL_LONG1:
      case SparqlUpdateParser::STRING_LITERAL_LONG2: {
        enterOuterAlt(_localctx, 2);
        setState(293);
        rdfLiteral();
        break;
      }

      case SparqlUpdateParser::INTEGER:
      case SparqlUpdateParser::DECIMAL:
      case SparqlUpdateParser::DOUBLE:
      case SparqlUpdateParser::INTEGER_POSITIVE:
      case SparqlUpdateParser::DECIMAL_POSITIVE:
      case SparqlUpdateParser::DOUBLE_POSITIVE:
      case SparqlUpdateParser::INTEGER_NEGATIVE:
      case SparqlUpdateParser::DECIMAL_NEGATIVE:
      case SparqlUpdateParser::DOUBLE_NEGATIVE: {
        enterOuterAlt(_localctx, 3);
        setState(294);
        numericLiteral();
        break;
      }

      case SparqlUpdateParser::K_TRUE:
      case SparqlUpdateParser::K_FALSE: {
        enterOuterAlt(_localctx, 4);
        setState(295);
        booleanLiteral();
        break;
      }

      case SparqlUpdateParser::BLANK_NODE_LABEL:
      case SparqlUpdateParser::OPEN_SQUARE_BRACKET: {
        enterOuterAlt(_localctx, 5);
        setState(296);
        blankNode();
        break;
      }

      case SparqlUpdateParser::OPEN_BRACE: {
        enterOuterAlt(_localctx, 6);
        setState(297);
        nil();
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

//----------------- NilContext ------------------------------------------------------------------

SparqlUpdateParser::NilContext::NilContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlUpdateParser::NilContext::OPEN_BRACE() {
  return getToken(SparqlUpdateParser::OPEN_BRACE, 0);
}

tree::TerminalNode* SparqlUpdateParser::NilContext::CLOSE_BRACE() {
  return getToken(SparqlUpdateParser::CLOSE_BRACE, 0);
}


size_t SparqlUpdateParser::NilContext::getRuleIndex() const {
  return SparqlUpdateParser::RuleNil;
}


std::any SparqlUpdateParser::NilContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlUpdateParserVisitor*>(visitor))
    return parserVisitor->visitNil(this);
  else
    return visitor->visitChildren(this);
}

SparqlUpdateParser::NilContext* SparqlUpdateParser::nil() {
  NilContext *_localctx = _tracker.createInstance<NilContext>(_ctx, getState());
  enterRule(_localctx, 68, SparqlUpdateParser::RuleNil);

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
    match(SparqlUpdateParser::OPEN_BRACE);
    setState(301);
    match(SparqlUpdateParser::CLOSE_BRACE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- RdfLiteralContext ------------------------------------------------------------------

SparqlUpdateParser::RdfLiteralContext::RdfLiteralContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlUpdateParser::StringContext* SparqlUpdateParser::RdfLiteralContext::string() {
  return getRuleContext<SparqlUpdateParser::StringContext>(0);
}

tree::TerminalNode* SparqlUpdateParser::RdfLiteralContext::LANGTAG() {
  return getToken(SparqlUpdateParser::LANGTAG, 0);
}

tree::TerminalNode* SparqlUpdateParser::RdfLiteralContext::REFERENCE() {
  return getToken(SparqlUpdateParser::REFERENCE, 0);
}

SparqlUpdateParser::IriContext* SparqlUpdateParser::RdfLiteralContext::iri() {
  return getRuleContext<SparqlUpdateParser::IriContext>(0);
}


size_t SparqlUpdateParser::RdfLiteralContext::getRuleIndex() const {
  return SparqlUpdateParser::RuleRdfLiteral;
}


std::any SparqlUpdateParser::RdfLiteralContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlUpdateParserVisitor*>(visitor))
    return parserVisitor->visitRdfLiteral(this);
  else
    return visitor->visitChildren(this);
}

SparqlUpdateParser::RdfLiteralContext* SparqlUpdateParser::rdfLiteral() {
  RdfLiteralContext *_localctx = _tracker.createInstance<RdfLiteralContext>(_ctx, getState());
  enterRule(_localctx, 70, SparqlUpdateParser::RuleRdfLiteral);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(303);
    string();
    setState(307);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlUpdateParser::LANGTAG: {
        setState(304);
        match(SparqlUpdateParser::LANGTAG);
        break;
      }

      case SparqlUpdateParser::REFERENCE: {
        setState(305);
        match(SparqlUpdateParser::REFERENCE);
        setState(306);
        iri();
        break;
      }

      case SparqlUpdateParser::A:
      case SparqlUpdateParser::K_TRUE:
      case SparqlUpdateParser::K_FALSE:
      case SparqlUpdateParser::IRIREF:
      case SparqlUpdateParser::PNAME_NS:
      case SparqlUpdateParser::PNAME_LN:
      case SparqlUpdateParser::BLANK_NODE_LABEL:
      case SparqlUpdateParser::INTEGER:
      case SparqlUpdateParser::DECIMAL:
      case SparqlUpdateParser::DOUBLE:
      case SparqlUpdateParser::INTEGER_POSITIVE:
      case SparqlUpdateParser::DECIMAL_POSITIVE:
      case SparqlUpdateParser::DOUBLE_POSITIVE:
      case SparqlUpdateParser::INTEGER_NEGATIVE:
      case SparqlUpdateParser::DECIMAL_NEGATIVE:
      case SparqlUpdateParser::DOUBLE_NEGATIVE:
      case SparqlUpdateParser::STRING_LITERAL1:
      case SparqlUpdateParser::STRING_LITERAL2:
      case SparqlUpdateParser::STRING_LITERAL_LONG1:
      case SparqlUpdateParser::STRING_LITERAL_LONG2:
      case SparqlUpdateParser::OPEN_BRACE:
      case SparqlUpdateParser::CLOSE_BRACE:
      case SparqlUpdateParser::CLOSE_CURLY_BRACE:
      case SparqlUpdateParser::OPEN_SQUARE_BRACKET:
      case SparqlUpdateParser::CLOSE_SQUARE_BRACKET:
      case SparqlUpdateParser::SEMICOLON:
      case SparqlUpdateParser::DOT:
      case SparqlUpdateParser::COMMA: {
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

//----------------- NumericLiteralContext ------------------------------------------------------------------

SparqlUpdateParser::NumericLiteralContext::NumericLiteralContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlUpdateParser::NumericLiteralUnsignedContext* SparqlUpdateParser::NumericLiteralContext::numericLiteralUnsigned() {
  return getRuleContext<SparqlUpdateParser::NumericLiteralUnsignedContext>(0);
}

SparqlUpdateParser::NumericLiteralPositiveContext* SparqlUpdateParser::NumericLiteralContext::numericLiteralPositive() {
  return getRuleContext<SparqlUpdateParser::NumericLiteralPositiveContext>(0);
}

SparqlUpdateParser::NumericLiteralNegativeContext* SparqlUpdateParser::NumericLiteralContext::numericLiteralNegative() {
  return getRuleContext<SparqlUpdateParser::NumericLiteralNegativeContext>(0);
}


size_t SparqlUpdateParser::NumericLiteralContext::getRuleIndex() const {
  return SparqlUpdateParser::RuleNumericLiteral;
}


std::any SparqlUpdateParser::NumericLiteralContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlUpdateParserVisitor*>(visitor))
    return parserVisitor->visitNumericLiteral(this);
  else
    return visitor->visitChildren(this);
}

SparqlUpdateParser::NumericLiteralContext* SparqlUpdateParser::numericLiteral() {
  NumericLiteralContext *_localctx = _tracker.createInstance<NumericLiteralContext>(_ctx, getState());
  enterRule(_localctx, 72, SparqlUpdateParser::RuleNumericLiteral);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(312);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlUpdateParser::INTEGER:
      case SparqlUpdateParser::DECIMAL:
      case SparqlUpdateParser::DOUBLE: {
        enterOuterAlt(_localctx, 1);
        setState(309);
        numericLiteralUnsigned();
        break;
      }

      case SparqlUpdateParser::INTEGER_POSITIVE:
      case SparqlUpdateParser::DECIMAL_POSITIVE:
      case SparqlUpdateParser::DOUBLE_POSITIVE: {
        enterOuterAlt(_localctx, 2);
        setState(310);
        numericLiteralPositive();
        break;
      }

      case SparqlUpdateParser::INTEGER_NEGATIVE:
      case SparqlUpdateParser::DECIMAL_NEGATIVE:
      case SparqlUpdateParser::DOUBLE_NEGATIVE: {
        enterOuterAlt(_localctx, 3);
        setState(311);
        numericLiteralNegative();
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

//----------------- NumericLiteralUnsignedContext ------------------------------------------------------------------

SparqlUpdateParser::NumericLiteralUnsignedContext::NumericLiteralUnsignedContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlUpdateParser::NumericLiteralUnsignedContext::INTEGER() {
  return getToken(SparqlUpdateParser::INTEGER, 0);
}

tree::TerminalNode* SparqlUpdateParser::NumericLiteralUnsignedContext::DECIMAL() {
  return getToken(SparqlUpdateParser::DECIMAL, 0);
}

tree::TerminalNode* SparqlUpdateParser::NumericLiteralUnsignedContext::DOUBLE() {
  return getToken(SparqlUpdateParser::DOUBLE, 0);
}


size_t SparqlUpdateParser::NumericLiteralUnsignedContext::getRuleIndex() const {
  return SparqlUpdateParser::RuleNumericLiteralUnsigned;
}


std::any SparqlUpdateParser::NumericLiteralUnsignedContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlUpdateParserVisitor*>(visitor))
    return parserVisitor->visitNumericLiteralUnsigned(this);
  else
    return visitor->visitChildren(this);
}

SparqlUpdateParser::NumericLiteralUnsignedContext* SparqlUpdateParser::numericLiteralUnsigned() {
  NumericLiteralUnsignedContext *_localctx = _tracker.createInstance<NumericLiteralUnsignedContext>(_ctx, getState());
  enterRule(_localctx, 74, SparqlUpdateParser::RuleNumericLiteralUnsigned);
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
    _la = _input->LA(1);
    if (!(((((_la - 129) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 129)) & 7) != 0))) {
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

//----------------- NumericLiteralPositiveContext ------------------------------------------------------------------

SparqlUpdateParser::NumericLiteralPositiveContext::NumericLiteralPositiveContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlUpdateParser::NumericLiteralPositiveContext::INTEGER_POSITIVE() {
  return getToken(SparqlUpdateParser::INTEGER_POSITIVE, 0);
}

tree::TerminalNode* SparqlUpdateParser::NumericLiteralPositiveContext::DECIMAL_POSITIVE() {
  return getToken(SparqlUpdateParser::DECIMAL_POSITIVE, 0);
}

tree::TerminalNode* SparqlUpdateParser::NumericLiteralPositiveContext::DOUBLE_POSITIVE() {
  return getToken(SparqlUpdateParser::DOUBLE_POSITIVE, 0);
}


size_t SparqlUpdateParser::NumericLiteralPositiveContext::getRuleIndex() const {
  return SparqlUpdateParser::RuleNumericLiteralPositive;
}


std::any SparqlUpdateParser::NumericLiteralPositiveContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlUpdateParserVisitor*>(visitor))
    return parserVisitor->visitNumericLiteralPositive(this);
  else
    return visitor->visitChildren(this);
}

SparqlUpdateParser::NumericLiteralPositiveContext* SparqlUpdateParser::numericLiteralPositive() {
  NumericLiteralPositiveContext *_localctx = _tracker.createInstance<NumericLiteralPositiveContext>(_ctx, getState());
  enterRule(_localctx, 76, SparqlUpdateParser::RuleNumericLiteralPositive);
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
    setState(316);
    _la = _input->LA(1);
    if (!(((((_la - 132) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 132)) & 7) != 0))) {
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

//----------------- NumericLiteralNegativeContext ------------------------------------------------------------------

SparqlUpdateParser::NumericLiteralNegativeContext::NumericLiteralNegativeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlUpdateParser::NumericLiteralNegativeContext::INTEGER_NEGATIVE() {
  return getToken(SparqlUpdateParser::INTEGER_NEGATIVE, 0);
}

tree::TerminalNode* SparqlUpdateParser::NumericLiteralNegativeContext::DECIMAL_NEGATIVE() {
  return getToken(SparqlUpdateParser::DECIMAL_NEGATIVE, 0);
}

tree::TerminalNode* SparqlUpdateParser::NumericLiteralNegativeContext::DOUBLE_NEGATIVE() {
  return getToken(SparqlUpdateParser::DOUBLE_NEGATIVE, 0);
}


size_t SparqlUpdateParser::NumericLiteralNegativeContext::getRuleIndex() const {
  return SparqlUpdateParser::RuleNumericLiteralNegative;
}


std::any SparqlUpdateParser::NumericLiteralNegativeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlUpdateParserVisitor*>(visitor))
    return parserVisitor->visitNumericLiteralNegative(this);
  else
    return visitor->visitChildren(this);
}

SparqlUpdateParser::NumericLiteralNegativeContext* SparqlUpdateParser::numericLiteralNegative() {
  NumericLiteralNegativeContext *_localctx = _tracker.createInstance<NumericLiteralNegativeContext>(_ctx, getState());
  enterRule(_localctx, 78, SparqlUpdateParser::RuleNumericLiteralNegative);
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
    _la = _input->LA(1);
    if (!(((((_la - 135) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 135)) & 7) != 0))) {
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

//----------------- BooleanLiteralContext ------------------------------------------------------------------

SparqlUpdateParser::BooleanLiteralContext::BooleanLiteralContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlUpdateParser::BooleanLiteralContext::K_TRUE() {
  return getToken(SparqlUpdateParser::K_TRUE, 0);
}

tree::TerminalNode* SparqlUpdateParser::BooleanLiteralContext::K_FALSE() {
  return getToken(SparqlUpdateParser::K_FALSE, 0);
}


size_t SparqlUpdateParser::BooleanLiteralContext::getRuleIndex() const {
  return SparqlUpdateParser::RuleBooleanLiteral;
}


std::any SparqlUpdateParser::BooleanLiteralContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlUpdateParserVisitor*>(visitor))
    return parserVisitor->visitBooleanLiteral(this);
  else
    return visitor->visitChildren(this);
}

SparqlUpdateParser::BooleanLiteralContext* SparqlUpdateParser::booleanLiteral() {
  BooleanLiteralContext *_localctx = _tracker.createInstance<BooleanLiteralContext>(_ctx, getState());
  enterRule(_localctx, 80, SparqlUpdateParser::RuleBooleanLiteral);
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
    setState(320);
    _la = _input->LA(1);
    if (!(_la == SparqlUpdateParser::K_TRUE

    || _la == SparqlUpdateParser::K_FALSE)) {
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

//----------------- StringContext ------------------------------------------------------------------

SparqlUpdateParser::StringContext::StringContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlUpdateParser::StringContext::STRING_LITERAL1() {
  return getToken(SparqlUpdateParser::STRING_LITERAL1, 0);
}

tree::TerminalNode* SparqlUpdateParser::StringContext::STRING_LITERAL2() {
  return getToken(SparqlUpdateParser::STRING_LITERAL2, 0);
}

tree::TerminalNode* SparqlUpdateParser::StringContext::STRING_LITERAL_LONG1() {
  return getToken(SparqlUpdateParser::STRING_LITERAL_LONG1, 0);
}

tree::TerminalNode* SparqlUpdateParser::StringContext::STRING_LITERAL_LONG2() {
  return getToken(SparqlUpdateParser::STRING_LITERAL_LONG2, 0);
}


size_t SparqlUpdateParser::StringContext::getRuleIndex() const {
  return SparqlUpdateParser::RuleString;
}


std::any SparqlUpdateParser::StringContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlUpdateParserVisitor*>(visitor))
    return parserVisitor->visitString(this);
  else
    return visitor->visitChildren(this);
}

SparqlUpdateParser::StringContext* SparqlUpdateParser::string() {
  StringContext *_localctx = _tracker.createInstance<StringContext>(_ctx, getState());
  enterRule(_localctx, 82, SparqlUpdateParser::RuleString);
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
    setState(322);
    _la = _input->LA(1);
    if (!(((((_la - 138) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 138)) & 15) != 0))) {
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

//----------------- IriContext ------------------------------------------------------------------

SparqlUpdateParser::IriContext::IriContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlUpdateParser::IriContext::IRIREF() {
  return getToken(SparqlUpdateParser::IRIREF, 0);
}

SparqlUpdateParser::PrefixedNameContext* SparqlUpdateParser::IriContext::prefixedName() {
  return getRuleContext<SparqlUpdateParser::PrefixedNameContext>(0);
}


size_t SparqlUpdateParser::IriContext::getRuleIndex() const {
  return SparqlUpdateParser::RuleIri;
}


std::any SparqlUpdateParser::IriContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlUpdateParserVisitor*>(visitor))
    return parserVisitor->visitIri(this);
  else
    return visitor->visitChildren(this);
}

SparqlUpdateParser::IriContext* SparqlUpdateParser::iri() {
  IriContext *_localctx = _tracker.createInstance<IriContext>(_ctx, getState());
  enterRule(_localctx, 84, SparqlUpdateParser::RuleIri);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(326);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlUpdateParser::IRIREF: {
        enterOuterAlt(_localctx, 1);
        setState(324);
        match(SparqlUpdateParser::IRIREF);
        break;
      }

      case SparqlUpdateParser::PNAME_NS:
      case SparqlUpdateParser::PNAME_LN: {
        enterOuterAlt(_localctx, 2);
        setState(325);
        prefixedName();
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

//----------------- PrefixedNameContext ------------------------------------------------------------------

SparqlUpdateParser::PrefixedNameContext::PrefixedNameContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlUpdateParser::PrefixedNameContext::PNAME_LN() {
  return getToken(SparqlUpdateParser::PNAME_LN, 0);
}

tree::TerminalNode* SparqlUpdateParser::PrefixedNameContext::PNAME_NS() {
  return getToken(SparqlUpdateParser::PNAME_NS, 0);
}


size_t SparqlUpdateParser::PrefixedNameContext::getRuleIndex() const {
  return SparqlUpdateParser::RulePrefixedName;
}


std::any SparqlUpdateParser::PrefixedNameContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlUpdateParserVisitor*>(visitor))
    return parserVisitor->visitPrefixedName(this);
  else
    return visitor->visitChildren(this);
}

SparqlUpdateParser::PrefixedNameContext* SparqlUpdateParser::prefixedName() {
  PrefixedNameContext *_localctx = _tracker.createInstance<PrefixedNameContext>(_ctx, getState());
  enterRule(_localctx, 86, SparqlUpdateParser::RulePrefixedName);
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
    setState(328);
    _la = _input->LA(1);
    if (!(_la == SparqlUpdateParser::PNAME_NS

    || _la == SparqlUpdateParser::PNAME_LN)) {
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

//----------------- BlankNodeContext ------------------------------------------------------------------

SparqlUpdateParser::BlankNodeContext::BlankNodeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlUpdateParser::BlankNodeContext::BLANK_NODE_LABEL() {
  return getToken(SparqlUpdateParser::BLANK_NODE_LABEL, 0);
}

SparqlUpdateParser::AnonContext* SparqlUpdateParser::BlankNodeContext::anon() {
  return getRuleContext<SparqlUpdateParser::AnonContext>(0);
}


size_t SparqlUpdateParser::BlankNodeContext::getRuleIndex() const {
  return SparqlUpdateParser::RuleBlankNode;
}


std::any SparqlUpdateParser::BlankNodeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlUpdateParserVisitor*>(visitor))
    return parserVisitor->visitBlankNode(this);
  else
    return visitor->visitChildren(this);
}

SparqlUpdateParser::BlankNodeContext* SparqlUpdateParser::blankNode() {
  BlankNodeContext *_localctx = _tracker.createInstance<BlankNodeContext>(_ctx, getState());
  enterRule(_localctx, 88, SparqlUpdateParser::RuleBlankNode);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(332);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlUpdateParser::BLANK_NODE_LABEL: {
        enterOuterAlt(_localctx, 1);
        setState(330);
        match(SparqlUpdateParser::BLANK_NODE_LABEL);
        break;
      }

      case SparqlUpdateParser::OPEN_SQUARE_BRACKET: {
        enterOuterAlt(_localctx, 2);
        setState(331);
        anon();
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

//----------------- AnonContext ------------------------------------------------------------------

SparqlUpdateParser::AnonContext::AnonContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlUpdateParser::AnonContext::OPEN_SQUARE_BRACKET() {
  return getToken(SparqlUpdateParser::OPEN_SQUARE_BRACKET, 0);
}

tree::TerminalNode* SparqlUpdateParser::AnonContext::CLOSE_SQUARE_BRACKET() {
  return getToken(SparqlUpdateParser::CLOSE_SQUARE_BRACKET, 0);
}


size_t SparqlUpdateParser::AnonContext::getRuleIndex() const {
  return SparqlUpdateParser::RuleAnon;
}


std::any SparqlUpdateParser::AnonContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlUpdateParserVisitor*>(visitor))
    return parserVisitor->visitAnon(this);
  else
    return visitor->visitChildren(this);
}

SparqlUpdateParser::AnonContext* SparqlUpdateParser::anon() {
  AnonContext *_localctx = _tracker.createInstance<AnonContext>(_ctx, getState());
  enterRule(_localctx, 90, SparqlUpdateParser::RuleAnon);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(334);
    match(SparqlUpdateParser::OPEN_SQUARE_BRACKET);
    setState(335);
    match(SparqlUpdateParser::CLOSE_SQUARE_BRACKET);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

void SparqlUpdateParser::initialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  sparqlupdateparserParserInitialize();
#else
  ::antlr4::internal::call_once(sparqlupdateparserParserOnceFlag, sparqlupdateparserParserInitialize);
#endif
}

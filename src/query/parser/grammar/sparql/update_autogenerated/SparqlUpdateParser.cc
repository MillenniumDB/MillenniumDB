
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
      "query", "prologue", "baseDecl", "prefixDecl", "update", "insertData", 
      "deleteData", "createTextIndex", "normalizeTextIndex", "normalizeType", 
      "tokenizeTextIndex", "tokenizeType", "quadData", "triplesTemplate", 
      "constructTemplate", "constructTriples", "triplesSameSubject", "propertyList", 
      "propertyListNotEmpty", "verb", "objectList", "object", "propertyListPathNotEmpty", 
      "propertyListPathNotEmptyList", "objectListPath", "objectPath", "integer", 
      "triplesNode", "blankNodePropertyList", "triplesNodePath", "blankNodePropertyListPath", 
      "collection", "collectionPath", "graphNode", "graphNodePath", "graphTerm", 
      "nil", "rdfLiteral", "numericLiteral", "numericLiteralUnsigned", "numericLiteralPositive", 
      "numericLiteralNegative", "booleanLiteral", "string", "iri", "prefixedName", 
      "blankNode", "anon"
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
      "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
      "", "'^^'", "'<='", "'>='", "'!='", "'&&'", "'||'", "'^'", "'('", 
      "')'", "'{'", "'}'", "'['", "']'", "';'", "'.'", "'+'", "'-'", "'*'", 
      "'\\u003F'", "','", "'!'", "'/'", "'='", "'<'", "'>'", "'|'"
    },
    std::vector<std::string>{
      "", "WS", "BASE", "PREFIX", "SELECT", "DISTINCT", "REDUCED", "CONSTRUCT", 
      "DESCRIBE", "ASK", "FROM", "NAMED", "WHERE", "ORDER", "BY", "ASC", 
      "DESC", "LIMIT", "OFFSET", "VALUES", "OPTIONAL", "GRAPH", "UNION", 
      "FILTER", "A", "STR", "LANG", "LANGMATCHES", "DATATYPE", "BOUND", 
      "SAMETERM", "ISIRI", "ISURI", "ISBLANK", "ISLITERAL", "REGEX", "SUBSTR", 
      "TRUE", "FALSE", "LOAD", "CLEAR", "DROP", "ADD", "MOVE", "COPY", "CREATE", 
      "DELETE", "INSERT", "USING", "SILENT", "DEFAULT", "ALL", "DATA", "WITH", 
      "INTO", "TO", "AS", "GROUP", "HAVING", "UNDEF", "BINDINGS", "SERVICE", 
      "BIND", "MINUS", "IRI", "URI", "BNODE", "RAND", "ABS", "CEIL", "FLOOR", 
      "ROUND", "CONCAT", "STRLEN", "UCASE", "LCASE", "ENCODE_FOR_URI", "CONTAINS", 
      "STRSTARTS", "STRENDS", "STRBEFORE", "STRAFTER", "REPLACE", "YEAR", 
      "MONTH", "DAY", "HOURS", "MINUTES", "SECONDS", "TIMEZONE", "TZ", "NOW", 
      "UUID", "STRUUID", "MD5", "SHA1", "SHA256", "SHA384", "SHA512", "COALESCE", 
      "IF", "STRLANG", "STRDT", "ISNUMERIC", "COUNT", "SUM", "MIN", "MAX", 
      "AVG", "SAMPLE", "GROUP_CONCAT", "NOT", "IN", "EXISTS", "SEPARATOR", 
      "ANY", "SHORTEST", "SIMPLE", "ACYCLIC", "WALKS", "IDENTITY", "INDEX", 
      "NFKD_CASEFOLD", "NORMALIZE", "ON", "TEXT", "TOKENIZE", "WHITESPACE", 
      "WS_KEEP_PUNCT", "WS_RM_PUNCT", "WS_SPLIT_PUNCT", "WS_SYMBOLS", "IRIREF", 
      "PNAME_NS", "PNAME_LN", "BLANK_NODE_LABEL", "VAR1", "VAR2", "LANGTAG", 
      "INTEGER", "DECIMAL", "DOUBLE", "INTEGER_POSITIVE", "DECIMAL_POSITIVE", 
      "DOUBLE_POSITIVE", "INTEGER_NEGATIVE", "DECIMAL_NEGATIVE", "DOUBLE_NEGATIVE", 
      "STRING_LITERAL1", "STRING_LITERAL2", "STRING_LITERAL_LONG1", "STRING_LITERAL_LONG2", 
      "COMMENT", "REFERENCE", "LESS_EQUAL", "GREATER_EQUAL", "NOT_EQUAL", 
      "AND", "OR", "INVERSE", "OPEN_BRACE", "CLOSE_BRACE", "OPEN_CURLY_BRACE", 
      "CLOSE_CURLY_BRACE", "OPEN_SQUARE_BRACKET", "CLOSE_SQUARE_BRACKET", 
      "SEMICOLON", "DOT", "PLUS_SIGN", "MINUS_SIGN", "ASTERISK", "QUESTION_MARK", 
      "COMMA", "NEGATION", "DIVIDE", "EQUAL", "LESS", "GREATER", "PIPE", 
      "ANY_OTHER"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,1,179,341,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,6,2,
  	7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,2,14,7,
  	14,2,15,7,15,2,16,7,16,2,17,7,17,2,18,7,18,2,19,7,19,2,20,7,20,2,21,7,
  	21,2,22,7,22,2,23,7,23,2,24,7,24,2,25,7,25,2,26,7,26,2,27,7,27,2,28,7,
  	28,2,29,7,29,2,30,7,30,2,31,7,31,2,32,7,32,2,33,7,33,2,34,7,34,2,35,7,
  	35,2,36,7,36,2,37,7,37,2,38,7,38,2,39,7,39,2,40,7,40,2,41,7,41,2,42,7,
  	42,2,43,7,43,2,44,7,44,2,45,7,45,2,46,7,46,2,47,7,47,1,0,1,0,1,0,3,0,
  	100,8,0,1,0,1,0,1,0,5,0,105,8,0,10,0,12,0,108,9,0,1,0,1,0,1,1,1,1,5,1,
  	114,8,1,10,1,12,1,117,9,1,1,2,1,2,1,2,1,3,1,3,1,3,1,3,1,4,1,4,1,4,3,4,
  	129,8,4,1,5,1,5,1,5,1,5,1,6,1,6,1,6,1,6,1,7,1,7,1,7,1,7,1,7,1,7,1,7,1,
  	7,3,7,147,8,7,1,7,3,7,150,8,7,1,8,1,8,1,8,1,9,1,9,1,10,1,10,1,10,1,11,
  	1,11,1,12,1,12,1,12,1,12,1,13,1,13,1,13,3,13,169,8,13,5,13,171,8,13,10,
  	13,12,13,174,9,13,1,14,1,14,3,14,178,8,14,1,14,1,14,1,15,1,15,1,15,3,
  	15,185,8,15,5,15,187,8,15,10,15,12,15,190,9,15,1,16,1,16,1,16,1,16,1,
  	16,1,16,3,16,198,8,16,1,17,3,17,201,8,17,1,18,1,18,1,18,1,18,1,18,1,18,
  	3,18,209,8,18,5,18,211,8,18,10,18,12,18,214,9,18,1,19,1,19,3,19,218,8,
  	19,1,20,1,20,1,20,5,20,223,8,20,10,20,12,20,226,9,20,1,21,1,21,1,22,1,
  	22,1,22,1,22,3,22,234,8,22,5,22,236,8,22,10,22,12,22,239,9,22,1,23,1,
  	23,1,23,1,24,1,24,1,24,5,24,247,8,24,10,24,12,24,250,9,24,1,25,1,25,1,
  	26,1,26,1,27,1,27,3,27,258,8,27,1,28,1,28,1,28,1,28,1,29,1,29,3,29,266,
  	8,29,1,30,1,30,1,30,1,30,1,31,1,31,4,31,274,8,31,11,31,12,31,275,1,31,
  	1,31,1,32,1,32,4,32,282,8,32,11,32,12,32,283,1,32,1,32,1,33,1,33,3,33,
  	290,8,33,1,34,1,34,3,34,294,8,34,1,35,1,35,1,35,1,35,1,35,1,35,3,35,302,
  	8,35,1,36,1,36,1,36,1,37,1,37,1,37,1,37,3,37,311,8,37,1,38,1,38,1,38,
  	3,38,316,8,38,1,39,1,39,1,40,1,40,1,41,1,41,1,42,1,42,1,43,1,43,1,44,
  	1,44,3,44,330,8,44,1,45,1,45,1,46,1,46,3,46,336,8,46,1,47,1,47,1,47,1,
  	47,0,0,48,0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,34,36,38,40,42,
  	44,46,48,50,52,54,56,58,60,62,64,66,68,70,72,74,76,78,80,82,84,86,88,
  	90,92,94,0,8,2,0,120,120,122,122,2,0,120,120,128,130,1,0,139,141,1,0,
  	142,144,1,0,145,147,1,0,37,38,1,0,148,151,1,0,133,134,331,0,96,1,0,0,
  	0,2,115,1,0,0,0,4,118,1,0,0,0,6,121,1,0,0,0,8,128,1,0,0,0,10,130,1,0,
  	0,0,12,134,1,0,0,0,14,138,1,0,0,0,16,151,1,0,0,0,18,154,1,0,0,0,20,156,
  	1,0,0,0,22,159,1,0,0,0,24,161,1,0,0,0,26,165,1,0,0,0,28,175,1,0,0,0,30,
  	181,1,0,0,0,32,197,1,0,0,0,34,200,1,0,0,0,36,202,1,0,0,0,38,217,1,0,0,
  	0,40,219,1,0,0,0,42,227,1,0,0,0,44,229,1,0,0,0,46,240,1,0,0,0,48,243,
  	1,0,0,0,50,251,1,0,0,0,52,253,1,0,0,0,54,257,1,0,0,0,56,259,1,0,0,0,58,
  	265,1,0,0,0,60,267,1,0,0,0,62,271,1,0,0,0,64,279,1,0,0,0,66,289,1,0,0,
  	0,68,293,1,0,0,0,70,301,1,0,0,0,72,303,1,0,0,0,74,306,1,0,0,0,76,315,
  	1,0,0,0,78,317,1,0,0,0,80,319,1,0,0,0,82,321,1,0,0,0,84,323,1,0,0,0,86,
  	325,1,0,0,0,88,329,1,0,0,0,90,331,1,0,0,0,92,335,1,0,0,0,94,337,1,0,0,
  	0,96,97,3,2,1,0,97,106,3,8,4,0,98,100,5,166,0,0,99,98,1,0,0,0,99,100,
  	1,0,0,0,100,101,1,0,0,0,101,102,3,2,1,0,102,103,3,8,4,0,103,105,1,0,0,
  	0,104,99,1,0,0,0,105,108,1,0,0,0,106,104,1,0,0,0,106,107,1,0,0,0,107,
  	109,1,0,0,0,108,106,1,0,0,0,109,110,5,0,0,1,110,1,1,0,0,0,111,114,3,4,
  	2,0,112,114,3,6,3,0,113,111,1,0,0,0,113,112,1,0,0,0,114,117,1,0,0,0,115,
  	113,1,0,0,0,115,116,1,0,0,0,116,3,1,0,0,0,117,115,1,0,0,0,118,119,5,2,
  	0,0,119,120,5,132,0,0,120,5,1,0,0,0,121,122,5,3,0,0,122,123,5,133,0,0,
  	123,124,5,132,0,0,124,7,1,0,0,0,125,129,3,10,5,0,126,129,3,12,6,0,127,
  	129,3,14,7,0,128,125,1,0,0,0,128,126,1,0,0,0,128,127,1,0,0,0,129,9,1,
  	0,0,0,130,131,5,47,0,0,131,132,5,52,0,0,132,133,3,24,12,0,133,11,1,0,
  	0,0,134,135,5,46,0,0,135,136,5,52,0,0,136,137,3,24,12,0,137,13,1,0,0,
  	0,138,139,5,45,0,0,139,140,5,125,0,0,140,141,5,121,0,0,141,142,3,86,43,
  	0,142,143,5,124,0,0,143,149,3,88,44,0,144,146,5,53,0,0,145,147,3,16,8,
  	0,146,145,1,0,0,0,146,147,1,0,0,0,147,148,1,0,0,0,148,150,3,20,10,0,149,
  	144,1,0,0,0,149,150,1,0,0,0,150,15,1,0,0,0,151,152,5,123,0,0,152,153,
  	3,18,9,0,153,17,1,0,0,0,154,155,7,0,0,0,155,19,1,0,0,0,156,157,5,126,
  	0,0,157,158,3,22,11,0,158,21,1,0,0,0,159,160,7,1,0,0,160,23,1,0,0,0,161,
  	162,5,162,0,0,162,163,3,26,13,0,163,164,5,163,0,0,164,25,1,0,0,0,165,
  	172,3,32,16,0,166,168,5,167,0,0,167,169,3,32,16,0,168,167,1,0,0,0,168,
  	169,1,0,0,0,169,171,1,0,0,0,170,166,1,0,0,0,171,174,1,0,0,0,172,170,1,
  	0,0,0,172,173,1,0,0,0,173,27,1,0,0,0,174,172,1,0,0,0,175,177,5,162,0,
  	0,176,178,3,30,15,0,177,176,1,0,0,0,177,178,1,0,0,0,178,179,1,0,0,0,179,
  	180,5,163,0,0,180,29,1,0,0,0,181,188,3,32,16,0,182,184,5,167,0,0,183,
  	185,3,30,15,0,184,183,1,0,0,0,184,185,1,0,0,0,185,187,1,0,0,0,186,182,
  	1,0,0,0,187,190,1,0,0,0,188,186,1,0,0,0,188,189,1,0,0,0,189,31,1,0,0,
  	0,190,188,1,0,0,0,191,192,3,70,35,0,192,193,3,36,18,0,193,198,1,0,0,0,
  	194,195,3,54,27,0,195,196,3,34,17,0,196,198,1,0,0,0,197,191,1,0,0,0,197,
  	194,1,0,0,0,198,33,1,0,0,0,199,201,3,36,18,0,200,199,1,0,0,0,200,201,
  	1,0,0,0,201,35,1,0,0,0,202,203,3,38,19,0,203,212,3,40,20,0,204,208,5,
  	166,0,0,205,206,3,38,19,0,206,207,3,40,20,0,207,209,1,0,0,0,208,205,1,
  	0,0,0,208,209,1,0,0,0,209,211,1,0,0,0,210,204,1,0,0,0,211,214,1,0,0,0,
  	212,210,1,0,0,0,212,213,1,0,0,0,213,37,1,0,0,0,214,212,1,0,0,0,215,218,
  	3,88,44,0,216,218,5,24,0,0,217,215,1,0,0,0,217,216,1,0,0,0,218,39,1,0,
  	0,0,219,224,3,42,21,0,220,221,5,172,0,0,221,223,3,42,21,0,222,220,1,0,
  	0,0,223,226,1,0,0,0,224,222,1,0,0,0,224,225,1,0,0,0,225,41,1,0,0,0,226,
  	224,1,0,0,0,227,228,3,66,33,0,228,43,1,0,0,0,229,230,3,38,19,0,230,237,
  	3,48,24,0,231,233,5,166,0,0,232,234,3,46,23,0,233,232,1,0,0,0,233,234,
  	1,0,0,0,234,236,1,0,0,0,235,231,1,0,0,0,236,239,1,0,0,0,237,235,1,0,0,
  	0,237,238,1,0,0,0,238,45,1,0,0,0,239,237,1,0,0,0,240,241,3,38,19,0,241,
  	242,3,40,20,0,242,47,1,0,0,0,243,248,3,50,25,0,244,245,5,172,0,0,245,
  	247,3,50,25,0,246,244,1,0,0,0,247,250,1,0,0,0,248,246,1,0,0,0,248,249,
  	1,0,0,0,249,49,1,0,0,0,250,248,1,0,0,0,251,252,3,68,34,0,252,51,1,0,0,
  	0,253,254,5,139,0,0,254,53,1,0,0,0,255,258,3,62,31,0,256,258,3,56,28,
  	0,257,255,1,0,0,0,257,256,1,0,0,0,258,55,1,0,0,0,259,260,5,164,0,0,260,
  	261,3,36,18,0,261,262,5,165,0,0,262,57,1,0,0,0,263,266,3,64,32,0,264,
  	266,3,60,30,0,265,263,1,0,0,0,265,264,1,0,0,0,266,59,1,0,0,0,267,268,
  	5,164,0,0,268,269,3,44,22,0,269,270,5,165,0,0,270,61,1,0,0,0,271,273,
  	5,160,0,0,272,274,3,66,33,0,273,272,1,0,0,0,274,275,1,0,0,0,275,273,1,
  	0,0,0,275,276,1,0,0,0,276,277,1,0,0,0,277,278,5,161,0,0,278,63,1,0,0,
  	0,279,281,5,160,0,0,280,282,3,68,34,0,281,280,1,0,0,0,282,283,1,0,0,0,
  	283,281,1,0,0,0,283,284,1,0,0,0,284,285,1,0,0,0,285,286,5,161,0,0,286,
  	65,1,0,0,0,287,290,3,70,35,0,288,290,3,54,27,0,289,287,1,0,0,0,289,288,
  	1,0,0,0,290,67,1,0,0,0,291,294,3,70,35,0,292,294,3,58,29,0,293,291,1,
  	0,0,0,293,292,1,0,0,0,294,69,1,0,0,0,295,302,3,88,44,0,296,302,3,74,37,
  	0,297,302,3,76,38,0,298,302,3,84,42,0,299,302,3,92,46,0,300,302,3,72,
  	36,0,301,295,1,0,0,0,301,296,1,0,0,0,301,297,1,0,0,0,301,298,1,0,0,0,
  	301,299,1,0,0,0,301,300,1,0,0,0,302,71,1,0,0,0,303,304,5,160,0,0,304,
  	305,5,161,0,0,305,73,1,0,0,0,306,310,3,86,43,0,307,311,5,138,0,0,308,
  	309,5,153,0,0,309,311,3,88,44,0,310,307,1,0,0,0,310,308,1,0,0,0,310,311,
  	1,0,0,0,311,75,1,0,0,0,312,316,3,78,39,0,313,316,3,80,40,0,314,316,3,
  	82,41,0,315,312,1,0,0,0,315,313,1,0,0,0,315,314,1,0,0,0,316,77,1,0,0,
  	0,317,318,7,2,0,0,318,79,1,0,0,0,319,320,7,3,0,0,320,81,1,0,0,0,321,322,
  	7,4,0,0,322,83,1,0,0,0,323,324,7,5,0,0,324,85,1,0,0,0,325,326,7,6,0,0,
  	326,87,1,0,0,0,327,330,5,132,0,0,328,330,3,90,45,0,329,327,1,0,0,0,329,
  	328,1,0,0,0,330,89,1,0,0,0,331,332,7,7,0,0,332,91,1,0,0,0,333,336,5,135,
  	0,0,334,336,3,94,47,0,335,333,1,0,0,0,335,334,1,0,0,0,336,93,1,0,0,0,
  	337,338,5,164,0,0,338,339,5,165,0,0,339,95,1,0,0,0,32,99,106,113,115,
  	128,146,149,168,172,177,184,188,197,200,208,212,217,224,233,237,248,257,
  	265,275,283,289,293,301,310,315,329,335
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
    setState(96);
    prologue();
    setState(97);
    update();
    setState(106);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 246290604621836) != 0) || _la == SparqlUpdateParser::SEMICOLON) {
      setState(99);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == SparqlUpdateParser::SEMICOLON) {
        setState(98);
        match(SparqlUpdateParser::SEMICOLON);
      }
      setState(101);
      prologue();
      setState(102);
      update();
      setState(108);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(109);
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
    setState(115);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SparqlUpdateParser::BASE

    || _la == SparqlUpdateParser::PREFIX) {
      setState(113);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case SparqlUpdateParser::BASE: {
          setState(111);
          baseDecl();
          break;
        }

        case SparqlUpdateParser::PREFIX: {
          setState(112);
          prefixDecl();
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      setState(117);
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
    setState(118);
    match(SparqlUpdateParser::BASE);
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
    setState(121);
    match(SparqlUpdateParser::PREFIX);
    setState(122);
    match(SparqlUpdateParser::PNAME_NS);
    setState(123);
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

SparqlUpdateParser::CreateTextIndexContext* SparqlUpdateParser::UpdateContext::createTextIndex() {
  return getRuleContext<SparqlUpdateParser::CreateTextIndexContext>(0);
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
    setState(128);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlUpdateParser::INSERT: {
        enterOuterAlt(_localctx, 1);
        setState(125);
        insertData();
        break;
      }

      case SparqlUpdateParser::DELETE: {
        enterOuterAlt(_localctx, 2);
        setState(126);
        deleteData();
        break;
      }

      case SparqlUpdateParser::CREATE: {
        enterOuterAlt(_localctx, 3);
        setState(127);
        createTextIndex();
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
  enterRule(_localctx, 10, SparqlUpdateParser::RuleInsertData);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(130);
    match(SparqlUpdateParser::INSERT);
    setState(131);
    match(SparqlUpdateParser::DATA);
    setState(132);
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
  enterRule(_localctx, 12, SparqlUpdateParser::RuleDeleteData);

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
    match(SparqlUpdateParser::DELETE);
    setState(135);
    match(SparqlUpdateParser::DATA);
    setState(136);
    quadData();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CreateTextIndexContext ------------------------------------------------------------------

SparqlUpdateParser::CreateTextIndexContext::CreateTextIndexContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlUpdateParser::CreateTextIndexContext::CREATE() {
  return getToken(SparqlUpdateParser::CREATE, 0);
}

tree::TerminalNode* SparqlUpdateParser::CreateTextIndexContext::TEXT() {
  return getToken(SparqlUpdateParser::TEXT, 0);
}

tree::TerminalNode* SparqlUpdateParser::CreateTextIndexContext::INDEX() {
  return getToken(SparqlUpdateParser::INDEX, 0);
}

SparqlUpdateParser::StringContext* SparqlUpdateParser::CreateTextIndexContext::string() {
  return getRuleContext<SparqlUpdateParser::StringContext>(0);
}

tree::TerminalNode* SparqlUpdateParser::CreateTextIndexContext::ON() {
  return getToken(SparqlUpdateParser::ON, 0);
}

SparqlUpdateParser::IriContext* SparqlUpdateParser::CreateTextIndexContext::iri() {
  return getRuleContext<SparqlUpdateParser::IriContext>(0);
}

tree::TerminalNode* SparqlUpdateParser::CreateTextIndexContext::WITH() {
  return getToken(SparqlUpdateParser::WITH, 0);
}

SparqlUpdateParser::TokenizeTextIndexContext* SparqlUpdateParser::CreateTextIndexContext::tokenizeTextIndex() {
  return getRuleContext<SparqlUpdateParser::TokenizeTextIndexContext>(0);
}

SparqlUpdateParser::NormalizeTextIndexContext* SparqlUpdateParser::CreateTextIndexContext::normalizeTextIndex() {
  return getRuleContext<SparqlUpdateParser::NormalizeTextIndexContext>(0);
}


size_t SparqlUpdateParser::CreateTextIndexContext::getRuleIndex() const {
  return SparqlUpdateParser::RuleCreateTextIndex;
}


std::any SparqlUpdateParser::CreateTextIndexContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlUpdateParserVisitor*>(visitor))
    return parserVisitor->visitCreateTextIndex(this);
  else
    return visitor->visitChildren(this);
}

SparqlUpdateParser::CreateTextIndexContext* SparqlUpdateParser::createTextIndex() {
  CreateTextIndexContext *_localctx = _tracker.createInstance<CreateTextIndexContext>(_ctx, getState());
  enterRule(_localctx, 14, SparqlUpdateParser::RuleCreateTextIndex);
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
    match(SparqlUpdateParser::CREATE);
    setState(139);
    match(SparqlUpdateParser::TEXT);
    setState(140);
    match(SparqlUpdateParser::INDEX);
    setState(141);
    string();
    setState(142);
    match(SparqlUpdateParser::ON);
    setState(143);
    iri();
    setState(149);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == SparqlUpdateParser::WITH) {
      setState(144);
      match(SparqlUpdateParser::WITH);
      setState(146);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == SparqlUpdateParser::NORMALIZE) {
        setState(145);
        normalizeTextIndex();
      }
      setState(148);
      tokenizeTextIndex();
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

SparqlUpdateParser::NormalizeTextIndexContext::NormalizeTextIndexContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlUpdateParser::NormalizeTextIndexContext::NORMALIZE() {
  return getToken(SparqlUpdateParser::NORMALIZE, 0);
}

SparqlUpdateParser::NormalizeTypeContext* SparqlUpdateParser::NormalizeTextIndexContext::normalizeType() {
  return getRuleContext<SparqlUpdateParser::NormalizeTypeContext>(0);
}


size_t SparqlUpdateParser::NormalizeTextIndexContext::getRuleIndex() const {
  return SparqlUpdateParser::RuleNormalizeTextIndex;
}


std::any SparqlUpdateParser::NormalizeTextIndexContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlUpdateParserVisitor*>(visitor))
    return parserVisitor->visitNormalizeTextIndex(this);
  else
    return visitor->visitChildren(this);
}

SparqlUpdateParser::NormalizeTextIndexContext* SparqlUpdateParser::normalizeTextIndex() {
  NormalizeTextIndexContext *_localctx = _tracker.createInstance<NormalizeTextIndexContext>(_ctx, getState());
  enterRule(_localctx, 16, SparqlUpdateParser::RuleNormalizeTextIndex);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(151);
    match(SparqlUpdateParser::NORMALIZE);
    setState(152);
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

SparqlUpdateParser::NormalizeTypeContext::NormalizeTypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlUpdateParser::NormalizeTypeContext::IDENTITY() {
  return getToken(SparqlUpdateParser::IDENTITY, 0);
}

tree::TerminalNode* SparqlUpdateParser::NormalizeTypeContext::NFKD_CASEFOLD() {
  return getToken(SparqlUpdateParser::NFKD_CASEFOLD, 0);
}


size_t SparqlUpdateParser::NormalizeTypeContext::getRuleIndex() const {
  return SparqlUpdateParser::RuleNormalizeType;
}


std::any SparqlUpdateParser::NormalizeTypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlUpdateParserVisitor*>(visitor))
    return parserVisitor->visitNormalizeType(this);
  else
    return visitor->visitChildren(this);
}

SparqlUpdateParser::NormalizeTypeContext* SparqlUpdateParser::normalizeType() {
  NormalizeTypeContext *_localctx = _tracker.createInstance<NormalizeTypeContext>(_ctx, getState());
  enterRule(_localctx, 18, SparqlUpdateParser::RuleNormalizeType);
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
    _la = _input->LA(1);
    if (!(_la == SparqlUpdateParser::IDENTITY

    || _la == SparqlUpdateParser::NFKD_CASEFOLD)) {
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

SparqlUpdateParser::TokenizeTextIndexContext::TokenizeTextIndexContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlUpdateParser::TokenizeTextIndexContext::TOKENIZE() {
  return getToken(SparqlUpdateParser::TOKENIZE, 0);
}

SparqlUpdateParser::TokenizeTypeContext* SparqlUpdateParser::TokenizeTextIndexContext::tokenizeType() {
  return getRuleContext<SparqlUpdateParser::TokenizeTypeContext>(0);
}


size_t SparqlUpdateParser::TokenizeTextIndexContext::getRuleIndex() const {
  return SparqlUpdateParser::RuleTokenizeTextIndex;
}


std::any SparqlUpdateParser::TokenizeTextIndexContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlUpdateParserVisitor*>(visitor))
    return parserVisitor->visitTokenizeTextIndex(this);
  else
    return visitor->visitChildren(this);
}

SparqlUpdateParser::TokenizeTextIndexContext* SparqlUpdateParser::tokenizeTextIndex() {
  TokenizeTextIndexContext *_localctx = _tracker.createInstance<TokenizeTextIndexContext>(_ctx, getState());
  enterRule(_localctx, 20, SparqlUpdateParser::RuleTokenizeTextIndex);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(156);
    match(SparqlUpdateParser::TOKENIZE);
    setState(157);
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

SparqlUpdateParser::TokenizeTypeContext::TokenizeTypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlUpdateParser::TokenizeTypeContext::IDENTITY() {
  return getToken(SparqlUpdateParser::IDENTITY, 0);
}

tree::TerminalNode* SparqlUpdateParser::TokenizeTypeContext::WS_SPLIT_PUNCT() {
  return getToken(SparqlUpdateParser::WS_SPLIT_PUNCT, 0);
}

tree::TerminalNode* SparqlUpdateParser::TokenizeTypeContext::WS_RM_PUNCT() {
  return getToken(SparqlUpdateParser::WS_RM_PUNCT, 0);
}

tree::TerminalNode* SparqlUpdateParser::TokenizeTypeContext::WS_KEEP_PUNCT() {
  return getToken(SparqlUpdateParser::WS_KEEP_PUNCT, 0);
}


size_t SparqlUpdateParser::TokenizeTypeContext::getRuleIndex() const {
  return SparqlUpdateParser::RuleTokenizeType;
}


std::any SparqlUpdateParser::TokenizeTypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlUpdateParserVisitor*>(visitor))
    return parserVisitor->visitTokenizeType(this);
  else
    return visitor->visitChildren(this);
}

SparqlUpdateParser::TokenizeTypeContext* SparqlUpdateParser::tokenizeType() {
  TokenizeTypeContext *_localctx = _tracker.createInstance<TokenizeTypeContext>(_ctx, getState());
  enterRule(_localctx, 22, SparqlUpdateParser::RuleTokenizeType);
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
    setState(159);
    _la = _input->LA(1);
    if (!(((((_la - 120) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 120)) & 1793) != 0))) {
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
  enterRule(_localctx, 24, SparqlUpdateParser::RuleQuadData);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(161);
    match(SparqlUpdateParser::OPEN_CURLY_BRACE);
    setState(162);
    triplesTemplate();
    setState(163);
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
  enterRule(_localctx, 26, SparqlUpdateParser::RuleTriplesTemplate);
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
    triplesSameSubject();
    setState(172);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SparqlUpdateParser::DOT) {
      setState(166);
      match(SparqlUpdateParser::DOT);
      setState(168);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == SparqlUpdateParser::TRUE

      || _la == SparqlUpdateParser::FALSE || ((((_la - 132) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 132)) & 4564451215) != 0)) {
        setState(167);
        triplesSameSubject();
      }
      setState(174);
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
  enterRule(_localctx, 28, SparqlUpdateParser::RuleConstructTemplate);
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
    setState(175);
    match(SparqlUpdateParser::OPEN_CURLY_BRACE);
    setState(177);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == SparqlUpdateParser::TRUE

    || _la == SparqlUpdateParser::FALSE || ((((_la - 132) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 132)) & 4564451215) != 0)) {
      setState(176);
      constructTriples();
    }
    setState(179);
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
  enterRule(_localctx, 30, SparqlUpdateParser::RuleConstructTriples);
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
    setState(181);
    triplesSameSubject();
    setState(188);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 11, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(182);
        match(SparqlUpdateParser::DOT);
        setState(184);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == SparqlUpdateParser::TRUE

        || _la == SparqlUpdateParser::FALSE || ((((_la - 132) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 132)) & 4564451215) != 0)) {
          setState(183);
          constructTriples();
        } 
      }
      setState(190);
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
  enterRule(_localctx, 32, SparqlUpdateParser::RuleTriplesSameSubject);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(197);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 12, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(191);
      graphTerm();
      setState(192);
      propertyListNotEmpty();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(194);
      triplesNode();
      setState(195);
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
  enterRule(_localctx, 34, SparqlUpdateParser::RulePropertyList);
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
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == SparqlUpdateParser::A || ((((_la - 132) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 132)) & 7) != 0)) {
      setState(199);
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
  enterRule(_localctx, 36, SparqlUpdateParser::RulePropertyListNotEmpty);
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
    setState(202);
    verb();
    setState(203);
    objectList();
    setState(212);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SparqlUpdateParser::SEMICOLON) {
      setState(204);
      match(SparqlUpdateParser::SEMICOLON);
      setState(208);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == SparqlUpdateParser::A || ((((_la - 132) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 132)) & 7) != 0)) {
        setState(205);
        verb();
        setState(206);
        objectList();
      }
      setState(214);
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
  enterRule(_localctx, 38, SparqlUpdateParser::RuleVerb);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(217);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlUpdateParser::IRIREF:
      case SparqlUpdateParser::PNAME_NS:
      case SparqlUpdateParser::PNAME_LN: {
        enterOuterAlt(_localctx, 1);
        setState(215);
        iri();
        break;
      }

      case SparqlUpdateParser::A: {
        enterOuterAlt(_localctx, 2);
        setState(216);
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
  enterRule(_localctx, 40, SparqlUpdateParser::RuleObjectList);
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
    setState(219);
    object();
    setState(224);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SparqlUpdateParser::COMMA) {
      setState(220);
      match(SparqlUpdateParser::COMMA);
      setState(221);
      object();
      setState(226);
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
  enterRule(_localctx, 42, SparqlUpdateParser::RuleObject);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(227);
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
  enterRule(_localctx, 44, SparqlUpdateParser::RulePropertyListPathNotEmpty);
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
    setState(229);
    verb();
    setState(230);
    objectListPath();
    setState(237);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SparqlUpdateParser::SEMICOLON) {
      setState(231);
      match(SparqlUpdateParser::SEMICOLON);
      setState(233);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == SparqlUpdateParser::A || ((((_la - 132) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 132)) & 7) != 0)) {
        setState(232);
        propertyListPathNotEmptyList();
      }
      setState(239);
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
  enterRule(_localctx, 46, SparqlUpdateParser::RulePropertyListPathNotEmptyList);

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
    verb();
    setState(241);
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
  enterRule(_localctx, 48, SparqlUpdateParser::RuleObjectListPath);
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
    setState(243);
    objectPath();
    setState(248);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SparqlUpdateParser::COMMA) {
      setState(244);
      match(SparqlUpdateParser::COMMA);
      setState(245);
      objectPath();
      setState(250);
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
  enterRule(_localctx, 50, SparqlUpdateParser::RuleObjectPath);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(251);
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
  enterRule(_localctx, 52, SparqlUpdateParser::RuleInteger);

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
  enterRule(_localctx, 54, SparqlUpdateParser::RuleTriplesNode);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(257);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlUpdateParser::OPEN_BRACE: {
        enterOuterAlt(_localctx, 1);
        setState(255);
        collection();
        break;
      }

      case SparqlUpdateParser::OPEN_SQUARE_BRACKET: {
        enterOuterAlt(_localctx, 2);
        setState(256);
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
  enterRule(_localctx, 56, SparqlUpdateParser::RuleBlankNodePropertyList);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(259);
    match(SparqlUpdateParser::OPEN_SQUARE_BRACKET);
    setState(260);
    propertyListNotEmpty();
    setState(261);
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
  enterRule(_localctx, 58, SparqlUpdateParser::RuleTriplesNodePath);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(265);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlUpdateParser::OPEN_BRACE: {
        enterOuterAlt(_localctx, 1);
        setState(263);
        collectionPath();
        break;
      }

      case SparqlUpdateParser::OPEN_SQUARE_BRACKET: {
        enterOuterAlt(_localctx, 2);
        setState(264);
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
  enterRule(_localctx, 60, SparqlUpdateParser::RuleBlankNodePropertyListPath);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(267);
    match(SparqlUpdateParser::OPEN_SQUARE_BRACKET);
    setState(268);
    propertyListPathNotEmpty();
    setState(269);
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
  enterRule(_localctx, 62, SparqlUpdateParser::RuleCollection);
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
    setState(271);
    match(SparqlUpdateParser::OPEN_BRACE);
    setState(273); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(272);
      graphNode();
      setState(275); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while (_la == SparqlUpdateParser::TRUE

    || _la == SparqlUpdateParser::FALSE || ((((_la - 132) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 132)) & 4564451215) != 0));
    setState(277);
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
  enterRule(_localctx, 64, SparqlUpdateParser::RuleCollectionPath);
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
    setState(279);
    match(SparqlUpdateParser::OPEN_BRACE);
    setState(281); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(280);
      graphNodePath();
      setState(283); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while (_la == SparqlUpdateParser::TRUE

    || _la == SparqlUpdateParser::FALSE || ((((_la - 132) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 132)) & 4564451215) != 0));
    setState(285);
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
  enterRule(_localctx, 66, SparqlUpdateParser::RuleGraphNode);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(289);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 25, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(287);
      graphTerm();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(288);
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
  enterRule(_localctx, 68, SparqlUpdateParser::RuleGraphNodePath);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(293);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 26, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(291);
      graphTerm();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(292);
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
  enterRule(_localctx, 70, SparqlUpdateParser::RuleGraphTerm);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(301);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlUpdateParser::IRIREF:
      case SparqlUpdateParser::PNAME_NS:
      case SparqlUpdateParser::PNAME_LN: {
        enterOuterAlt(_localctx, 1);
        setState(295);
        iri();
        break;
      }

      case SparqlUpdateParser::STRING_LITERAL1:
      case SparqlUpdateParser::STRING_LITERAL2:
      case SparqlUpdateParser::STRING_LITERAL_LONG1:
      case SparqlUpdateParser::STRING_LITERAL_LONG2: {
        enterOuterAlt(_localctx, 2);
        setState(296);
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
        setState(297);
        numericLiteral();
        break;
      }

      case SparqlUpdateParser::TRUE:
      case SparqlUpdateParser::FALSE: {
        enterOuterAlt(_localctx, 4);
        setState(298);
        booleanLiteral();
        break;
      }

      case SparqlUpdateParser::BLANK_NODE_LABEL:
      case SparqlUpdateParser::OPEN_SQUARE_BRACKET: {
        enterOuterAlt(_localctx, 5);
        setState(299);
        blankNode();
        break;
      }

      case SparqlUpdateParser::OPEN_BRACE: {
        enterOuterAlt(_localctx, 6);
        setState(300);
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
  enterRule(_localctx, 72, SparqlUpdateParser::RuleNil);

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
    match(SparqlUpdateParser::OPEN_BRACE);
    setState(304);
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
  enterRule(_localctx, 74, SparqlUpdateParser::RuleRdfLiteral);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(306);
    string();
    setState(310);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlUpdateParser::LANGTAG: {
        setState(307);
        match(SparqlUpdateParser::LANGTAG);
        break;
      }

      case SparqlUpdateParser::REFERENCE: {
        setState(308);
        match(SparqlUpdateParser::REFERENCE);
        setState(309);
        iri();
        break;
      }

      case SparqlUpdateParser::A:
      case SparqlUpdateParser::TRUE:
      case SparqlUpdateParser::FALSE:
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
  enterRule(_localctx, 76, SparqlUpdateParser::RuleNumericLiteral);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(315);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlUpdateParser::INTEGER:
      case SparqlUpdateParser::DECIMAL:
      case SparqlUpdateParser::DOUBLE: {
        enterOuterAlt(_localctx, 1);
        setState(312);
        numericLiteralUnsigned();
        break;
      }

      case SparqlUpdateParser::INTEGER_POSITIVE:
      case SparqlUpdateParser::DECIMAL_POSITIVE:
      case SparqlUpdateParser::DOUBLE_POSITIVE: {
        enterOuterAlt(_localctx, 2);
        setState(313);
        numericLiteralPositive();
        break;
      }

      case SparqlUpdateParser::INTEGER_NEGATIVE:
      case SparqlUpdateParser::DECIMAL_NEGATIVE:
      case SparqlUpdateParser::DOUBLE_NEGATIVE: {
        enterOuterAlt(_localctx, 3);
        setState(314);
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
  enterRule(_localctx, 78, SparqlUpdateParser::RuleNumericLiteralUnsigned);
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
    setState(317);
    _la = _input->LA(1);
    if (!(((((_la - 139) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 139)) & 7) != 0))) {
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
  enterRule(_localctx, 80, SparqlUpdateParser::RuleNumericLiteralPositive);
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
    setState(319);
    _la = _input->LA(1);
    if (!(((((_la - 142) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 142)) & 7) != 0))) {
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
  enterRule(_localctx, 82, SparqlUpdateParser::RuleNumericLiteralNegative);
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
    setState(321);
    _la = _input->LA(1);
    if (!(((((_la - 145) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 145)) & 7) != 0))) {
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

tree::TerminalNode* SparqlUpdateParser::BooleanLiteralContext::TRUE() {
  return getToken(SparqlUpdateParser::TRUE, 0);
}

tree::TerminalNode* SparqlUpdateParser::BooleanLiteralContext::FALSE() {
  return getToken(SparqlUpdateParser::FALSE, 0);
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
  enterRule(_localctx, 84, SparqlUpdateParser::RuleBooleanLiteral);
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
    setState(323);
    _la = _input->LA(1);
    if (!(_la == SparqlUpdateParser::TRUE

    || _la == SparqlUpdateParser::FALSE)) {
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
  enterRule(_localctx, 86, SparqlUpdateParser::RuleString);
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
    setState(325);
    _la = _input->LA(1);
    if (!(((((_la - 148) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 148)) & 15) != 0))) {
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
  enterRule(_localctx, 88, SparqlUpdateParser::RuleIri);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(329);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlUpdateParser::IRIREF: {
        enterOuterAlt(_localctx, 1);
        setState(327);
        match(SparqlUpdateParser::IRIREF);
        break;
      }

      case SparqlUpdateParser::PNAME_NS:
      case SparqlUpdateParser::PNAME_LN: {
        enterOuterAlt(_localctx, 2);
        setState(328);
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
  enterRule(_localctx, 90, SparqlUpdateParser::RulePrefixedName);
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
  enterRule(_localctx, 92, SparqlUpdateParser::RuleBlankNode);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(335);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlUpdateParser::BLANK_NODE_LABEL: {
        enterOuterAlt(_localctx, 1);
        setState(333);
        match(SparqlUpdateParser::BLANK_NODE_LABEL);
        break;
      }

      case SparqlUpdateParser::OPEN_SQUARE_BRACKET: {
        enterOuterAlt(_localctx, 2);
        setState(334);
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
  enterRule(_localctx, 94, SparqlUpdateParser::RuleAnon);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(337);
    match(SparqlUpdateParser::OPEN_SQUARE_BRACKET);
    setState(338);
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


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
      "deleteData", "quadData", "triplesTemplate", "constructTemplate", 
      "constructTriples", "triplesSameSubject", "propertyList", "propertyListNotEmpty", 
      "verb", "objectList", "object", "propertyListPathNotEmpty", "propertyListPathNotEmptyList", 
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
      "", "", "", "", "", "", "'^^'", "'<='", "'>='", "'!='", "'&&'", "'||'", 
      "'^'", "'('", "')'", "'{'", "'}'", "'['", "']'", "';'", "'.'", "'+'", 
      "'-'", "'*'", "'\\u003F'", "','", "'!'", "'/'", "'='", "'<'", "'>'", 
      "'|'"
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
      "ANY", "SHORTEST", "SIMPLE", "ACYCLIC", "WALKS", "IRIREF", "PNAME_NS", 
      "PNAME_LN", "BLANK_NODE_LABEL", "VAR1", "VAR2", "LANGTAG", "INTEGER", 
      "DECIMAL", "DOUBLE", "INTEGER_POSITIVE", "DECIMAL_POSITIVE", "DOUBLE_POSITIVE", 
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
  	4,1,167,307,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,6,2,
  	7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,2,14,7,
  	14,2,15,7,15,2,16,7,16,2,17,7,17,2,18,7,18,2,19,7,19,2,20,7,20,2,21,7,
  	21,2,22,7,22,2,23,7,23,2,24,7,24,2,25,7,25,2,26,7,26,2,27,7,27,2,28,7,
  	28,2,29,7,29,2,30,7,30,2,31,7,31,2,32,7,32,2,33,7,33,2,34,7,34,2,35,7,
  	35,2,36,7,36,2,37,7,37,2,38,7,38,2,39,7,39,2,40,7,40,2,41,7,41,2,42,7,
  	42,1,0,1,0,1,0,3,0,90,8,0,1,0,1,0,1,0,5,0,95,8,0,10,0,12,0,98,9,0,1,0,
  	1,0,1,1,1,1,5,1,104,8,1,10,1,12,1,107,9,1,1,2,1,2,1,2,1,3,1,3,1,3,1,3,
  	1,4,1,4,3,4,118,8,4,1,5,1,5,1,5,1,5,1,6,1,6,1,6,1,6,1,7,1,7,1,7,1,7,1,
  	8,1,8,1,8,3,8,135,8,8,5,8,137,8,8,10,8,12,8,140,9,8,1,9,1,9,3,9,144,8,
  	9,1,9,1,9,1,10,1,10,1,10,3,10,151,8,10,5,10,153,8,10,10,10,12,10,156,
  	9,10,1,11,1,11,1,11,1,11,1,11,1,11,3,11,164,8,11,1,12,3,12,167,8,12,1,
  	13,1,13,1,13,1,13,1,13,1,13,3,13,175,8,13,5,13,177,8,13,10,13,12,13,180,
  	9,13,1,14,1,14,3,14,184,8,14,1,15,1,15,1,15,5,15,189,8,15,10,15,12,15,
  	192,9,15,1,16,1,16,1,17,1,17,1,17,1,17,3,17,200,8,17,5,17,202,8,17,10,
  	17,12,17,205,9,17,1,18,1,18,1,18,1,19,1,19,1,19,5,19,213,8,19,10,19,12,
  	19,216,9,19,1,20,1,20,1,21,1,21,1,22,1,22,3,22,224,8,22,1,23,1,23,1,23,
  	1,23,1,24,1,24,3,24,232,8,24,1,25,1,25,1,25,1,25,1,26,1,26,4,26,240,8,
  	26,11,26,12,26,241,1,26,1,26,1,27,1,27,4,27,248,8,27,11,27,12,27,249,
  	1,27,1,27,1,28,1,28,3,28,256,8,28,1,29,1,29,3,29,260,8,29,1,30,1,30,1,
  	30,1,30,1,30,1,30,3,30,268,8,30,1,31,1,31,1,31,1,32,1,32,1,32,1,32,3,
  	32,277,8,32,1,33,1,33,1,33,3,33,282,8,33,1,34,1,34,1,35,1,35,1,36,1,36,
  	1,37,1,37,1,38,1,38,1,39,1,39,3,39,296,8,39,1,40,1,40,1,41,1,41,3,41,
  	302,8,41,1,42,1,42,1,42,1,42,0,0,43,0,2,4,6,8,10,12,14,16,18,20,22,24,
  	26,28,30,32,34,36,38,40,42,44,46,48,50,52,54,56,58,60,62,64,66,68,70,
  	72,74,76,78,80,82,84,0,6,1,0,127,129,1,0,130,132,1,0,133,135,1,0,37,38,
  	1,0,136,139,1,0,121,122,299,0,86,1,0,0,0,2,105,1,0,0,0,4,108,1,0,0,0,
  	6,111,1,0,0,0,8,117,1,0,0,0,10,119,1,0,0,0,12,123,1,0,0,0,14,127,1,0,
  	0,0,16,131,1,0,0,0,18,141,1,0,0,0,20,147,1,0,0,0,22,163,1,0,0,0,24,166,
  	1,0,0,0,26,168,1,0,0,0,28,183,1,0,0,0,30,185,1,0,0,0,32,193,1,0,0,0,34,
  	195,1,0,0,0,36,206,1,0,0,0,38,209,1,0,0,0,40,217,1,0,0,0,42,219,1,0,0,
  	0,44,223,1,0,0,0,46,225,1,0,0,0,48,231,1,0,0,0,50,233,1,0,0,0,52,237,
  	1,0,0,0,54,245,1,0,0,0,56,255,1,0,0,0,58,259,1,0,0,0,60,267,1,0,0,0,62,
  	269,1,0,0,0,64,272,1,0,0,0,66,281,1,0,0,0,68,283,1,0,0,0,70,285,1,0,0,
  	0,72,287,1,0,0,0,74,289,1,0,0,0,76,291,1,0,0,0,78,295,1,0,0,0,80,297,
  	1,0,0,0,82,301,1,0,0,0,84,303,1,0,0,0,86,87,3,2,1,0,87,96,3,8,4,0,88,
  	90,5,154,0,0,89,88,1,0,0,0,89,90,1,0,0,0,90,91,1,0,0,0,91,92,3,2,1,0,
  	92,93,3,8,4,0,93,95,1,0,0,0,94,89,1,0,0,0,95,98,1,0,0,0,96,94,1,0,0,0,
  	96,97,1,0,0,0,97,99,1,0,0,0,98,96,1,0,0,0,99,100,5,0,0,1,100,1,1,0,0,
  	0,101,104,3,4,2,0,102,104,3,6,3,0,103,101,1,0,0,0,103,102,1,0,0,0,104,
  	107,1,0,0,0,105,103,1,0,0,0,105,106,1,0,0,0,106,3,1,0,0,0,107,105,1,0,
  	0,0,108,109,5,2,0,0,109,110,5,120,0,0,110,5,1,0,0,0,111,112,5,3,0,0,112,
  	113,5,121,0,0,113,114,5,120,0,0,114,7,1,0,0,0,115,118,3,10,5,0,116,118,
  	3,12,6,0,117,115,1,0,0,0,117,116,1,0,0,0,118,9,1,0,0,0,119,120,5,47,0,
  	0,120,121,5,52,0,0,121,122,3,14,7,0,122,11,1,0,0,0,123,124,5,46,0,0,124,
  	125,5,52,0,0,125,126,3,14,7,0,126,13,1,0,0,0,127,128,5,150,0,0,128,129,
  	3,16,8,0,129,130,5,151,0,0,130,15,1,0,0,0,131,138,3,22,11,0,132,134,5,
  	155,0,0,133,135,3,22,11,0,134,133,1,0,0,0,134,135,1,0,0,0,135,137,1,0,
  	0,0,136,132,1,0,0,0,137,140,1,0,0,0,138,136,1,0,0,0,138,139,1,0,0,0,139,
  	17,1,0,0,0,140,138,1,0,0,0,141,143,5,150,0,0,142,144,3,20,10,0,143,142,
  	1,0,0,0,143,144,1,0,0,0,144,145,1,0,0,0,145,146,5,151,0,0,146,19,1,0,
  	0,0,147,154,3,22,11,0,148,150,5,155,0,0,149,151,3,20,10,0,150,149,1,0,
  	0,0,150,151,1,0,0,0,151,153,1,0,0,0,152,148,1,0,0,0,153,156,1,0,0,0,154,
  	152,1,0,0,0,154,155,1,0,0,0,155,21,1,0,0,0,156,154,1,0,0,0,157,158,3,
  	60,30,0,158,159,3,26,13,0,159,164,1,0,0,0,160,161,3,44,22,0,161,162,3,
  	24,12,0,162,164,1,0,0,0,163,157,1,0,0,0,163,160,1,0,0,0,164,23,1,0,0,
  	0,165,167,3,26,13,0,166,165,1,0,0,0,166,167,1,0,0,0,167,25,1,0,0,0,168,
  	169,3,28,14,0,169,178,3,30,15,0,170,174,5,154,0,0,171,172,3,28,14,0,172,
  	173,3,30,15,0,173,175,1,0,0,0,174,171,1,0,0,0,174,175,1,0,0,0,175,177,
  	1,0,0,0,176,170,1,0,0,0,177,180,1,0,0,0,178,176,1,0,0,0,178,179,1,0,0,
  	0,179,27,1,0,0,0,180,178,1,0,0,0,181,184,3,78,39,0,182,184,5,24,0,0,183,
  	181,1,0,0,0,183,182,1,0,0,0,184,29,1,0,0,0,185,190,3,32,16,0,186,187,
  	5,160,0,0,187,189,3,32,16,0,188,186,1,0,0,0,189,192,1,0,0,0,190,188,1,
  	0,0,0,190,191,1,0,0,0,191,31,1,0,0,0,192,190,1,0,0,0,193,194,3,56,28,
  	0,194,33,1,0,0,0,195,196,3,28,14,0,196,203,3,38,19,0,197,199,5,154,0,
  	0,198,200,3,36,18,0,199,198,1,0,0,0,199,200,1,0,0,0,200,202,1,0,0,0,201,
  	197,1,0,0,0,202,205,1,0,0,0,203,201,1,0,0,0,203,204,1,0,0,0,204,35,1,
  	0,0,0,205,203,1,0,0,0,206,207,3,28,14,0,207,208,3,30,15,0,208,37,1,0,
  	0,0,209,214,3,40,20,0,210,211,5,160,0,0,211,213,3,40,20,0,212,210,1,0,
  	0,0,213,216,1,0,0,0,214,212,1,0,0,0,214,215,1,0,0,0,215,39,1,0,0,0,216,
  	214,1,0,0,0,217,218,3,58,29,0,218,41,1,0,0,0,219,220,5,127,0,0,220,43,
  	1,0,0,0,221,224,3,52,26,0,222,224,3,46,23,0,223,221,1,0,0,0,223,222,1,
  	0,0,0,224,45,1,0,0,0,225,226,5,152,0,0,226,227,3,26,13,0,227,228,5,153,
  	0,0,228,47,1,0,0,0,229,232,3,54,27,0,230,232,3,50,25,0,231,229,1,0,0,
  	0,231,230,1,0,0,0,232,49,1,0,0,0,233,234,5,152,0,0,234,235,3,34,17,0,
  	235,236,5,153,0,0,236,51,1,0,0,0,237,239,5,148,0,0,238,240,3,56,28,0,
  	239,238,1,0,0,0,240,241,1,0,0,0,241,239,1,0,0,0,241,242,1,0,0,0,242,243,
  	1,0,0,0,243,244,5,149,0,0,244,53,1,0,0,0,245,247,5,148,0,0,246,248,3,
  	58,29,0,247,246,1,0,0,0,248,249,1,0,0,0,249,247,1,0,0,0,249,250,1,0,0,
  	0,250,251,1,0,0,0,251,252,5,149,0,0,252,55,1,0,0,0,253,256,3,60,30,0,
  	254,256,3,44,22,0,255,253,1,0,0,0,255,254,1,0,0,0,256,57,1,0,0,0,257,
  	260,3,60,30,0,258,260,3,48,24,0,259,257,1,0,0,0,259,258,1,0,0,0,260,59,
  	1,0,0,0,261,268,3,78,39,0,262,268,3,64,32,0,263,268,3,66,33,0,264,268,
  	3,74,37,0,265,268,3,82,41,0,266,268,3,62,31,0,267,261,1,0,0,0,267,262,
  	1,0,0,0,267,263,1,0,0,0,267,264,1,0,0,0,267,265,1,0,0,0,267,266,1,0,0,
  	0,268,61,1,0,0,0,269,270,5,148,0,0,270,271,5,149,0,0,271,63,1,0,0,0,272,
  	276,3,76,38,0,273,277,5,126,0,0,274,275,5,141,0,0,275,277,3,78,39,0,276,
  	273,1,0,0,0,276,274,1,0,0,0,276,277,1,0,0,0,277,65,1,0,0,0,278,282,3,
  	68,34,0,279,282,3,70,35,0,280,282,3,72,36,0,281,278,1,0,0,0,281,279,1,
  	0,0,0,281,280,1,0,0,0,282,67,1,0,0,0,283,284,7,0,0,0,284,69,1,0,0,0,285,
  	286,7,1,0,0,286,71,1,0,0,0,287,288,7,2,0,0,288,73,1,0,0,0,289,290,7,3,
  	0,0,290,75,1,0,0,0,291,292,7,4,0,0,292,77,1,0,0,0,293,296,5,120,0,0,294,
  	296,3,80,40,0,295,293,1,0,0,0,295,294,1,0,0,0,296,79,1,0,0,0,297,298,
  	7,5,0,0,298,81,1,0,0,0,299,302,5,123,0,0,300,302,3,84,42,0,301,299,1,
  	0,0,0,301,300,1,0,0,0,302,83,1,0,0,0,303,304,5,152,0,0,304,305,5,153,
  	0,0,305,85,1,0,0,0,30,89,96,103,105,117,134,138,143,150,154,163,166,174,
  	178,183,190,199,203,214,223,231,241,249,255,259,267,276,281,295,301
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
    setState(86);
    prologue();
    setState(87);
    update();
    setState(96);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 211106232533004) != 0) || _la == SparqlUpdateParser::SEMICOLON) {
      setState(89);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == SparqlUpdateParser::SEMICOLON) {
        setState(88);
        match(SparqlUpdateParser::SEMICOLON);
      }
      setState(91);
      prologue();
      setState(92);
      update();
      setState(98);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(99);
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
    setState(105);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SparqlUpdateParser::BASE

    || _la == SparqlUpdateParser::PREFIX) {
      setState(103);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case SparqlUpdateParser::BASE: {
          setState(101);
          baseDecl();
          break;
        }

        case SparqlUpdateParser::PREFIX: {
          setState(102);
          prefixDecl();
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      setState(107);
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
    setState(108);
    match(SparqlUpdateParser::BASE);
    setState(109);
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
    setState(111);
    match(SparqlUpdateParser::PREFIX);
    setState(112);
    match(SparqlUpdateParser::PNAME_NS);
    setState(113);
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
    setState(117);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlUpdateParser::INSERT: {
        enterOuterAlt(_localctx, 1);
        setState(115);
        insertData();
        break;
      }

      case SparqlUpdateParser::DELETE: {
        enterOuterAlt(_localctx, 2);
        setState(116);
        deleteData();
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
    setState(119);
    match(SparqlUpdateParser::INSERT);
    setState(120);
    match(SparqlUpdateParser::DATA);
    setState(121);
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
    setState(123);
    match(SparqlUpdateParser::DELETE);
    setState(124);
    match(SparqlUpdateParser::DATA);
    setState(125);
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
  enterRule(_localctx, 14, SparqlUpdateParser::RuleQuadData);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(127);
    match(SparqlUpdateParser::OPEN_CURLY_BRACE);
    setState(128);
    triplesTemplate();
    setState(129);
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
  enterRule(_localctx, 16, SparqlUpdateParser::RuleTriplesTemplate);
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
    setState(131);
    triplesSameSubject();
    setState(138);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SparqlUpdateParser::DOT) {
      setState(132);
      match(SparqlUpdateParser::DOT);
      setState(134);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == SparqlUpdateParser::TRUE

      || _la == SparqlUpdateParser::FALSE || ((((_la - 120) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 120)) & 4564451215) != 0)) {
        setState(133);
        triplesSameSubject();
      }
      setState(140);
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
  enterRule(_localctx, 18, SparqlUpdateParser::RuleConstructTemplate);
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
    setState(141);
    match(SparqlUpdateParser::OPEN_CURLY_BRACE);
    setState(143);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == SparqlUpdateParser::TRUE

    || _la == SparqlUpdateParser::FALSE || ((((_la - 120) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 120)) & 4564451215) != 0)) {
      setState(142);
      constructTriples();
    }
    setState(145);
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
  enterRule(_localctx, 20, SparqlUpdateParser::RuleConstructTriples);
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
    setState(147);
    triplesSameSubject();
    setState(154);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 9, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(148);
        match(SparqlUpdateParser::DOT);
        setState(150);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == SparqlUpdateParser::TRUE

        || _la == SparqlUpdateParser::FALSE || ((((_la - 120) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 120)) & 4564451215) != 0)) {
          setState(149);
          constructTriples();
        } 
      }
      setState(156);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 9, _ctx);
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
  enterRule(_localctx, 22, SparqlUpdateParser::RuleTriplesSameSubject);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(163);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 10, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(157);
      graphTerm();
      setState(158);
      propertyListNotEmpty();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(160);
      triplesNode();
      setState(161);
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
  enterRule(_localctx, 24, SparqlUpdateParser::RulePropertyList);
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
    setState(166);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == SparqlUpdateParser::A || ((((_la - 120) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 120)) & 7) != 0)) {
      setState(165);
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
  enterRule(_localctx, 26, SparqlUpdateParser::RulePropertyListNotEmpty);
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
    setState(168);
    verb();
    setState(169);
    objectList();
    setState(178);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SparqlUpdateParser::SEMICOLON) {
      setState(170);
      match(SparqlUpdateParser::SEMICOLON);
      setState(174);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == SparqlUpdateParser::A || ((((_la - 120) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 120)) & 7) != 0)) {
        setState(171);
        verb();
        setState(172);
        objectList();
      }
      setState(180);
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
  enterRule(_localctx, 28, SparqlUpdateParser::RuleVerb);

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
      case SparqlUpdateParser::IRIREF:
      case SparqlUpdateParser::PNAME_NS:
      case SparqlUpdateParser::PNAME_LN: {
        enterOuterAlt(_localctx, 1);
        setState(181);
        iri();
        break;
      }

      case SparqlUpdateParser::A: {
        enterOuterAlt(_localctx, 2);
        setState(182);
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
  enterRule(_localctx, 30, SparqlUpdateParser::RuleObjectList);
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
    object();
    setState(190);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SparqlUpdateParser::COMMA) {
      setState(186);
      match(SparqlUpdateParser::COMMA);
      setState(187);
      object();
      setState(192);
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
  enterRule(_localctx, 32, SparqlUpdateParser::RuleObject);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(193);
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
  enterRule(_localctx, 34, SparqlUpdateParser::RulePropertyListPathNotEmpty);
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
    setState(195);
    verb();
    setState(196);
    objectListPath();
    setState(203);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SparqlUpdateParser::SEMICOLON) {
      setState(197);
      match(SparqlUpdateParser::SEMICOLON);
      setState(199);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == SparqlUpdateParser::A || ((((_la - 120) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 120)) & 7) != 0)) {
        setState(198);
        propertyListPathNotEmptyList();
      }
      setState(205);
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
  enterRule(_localctx, 36, SparqlUpdateParser::RulePropertyListPathNotEmptyList);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(206);
    verb();
    setState(207);
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
  enterRule(_localctx, 38, SparqlUpdateParser::RuleObjectListPath);
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
    objectPath();
    setState(214);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SparqlUpdateParser::COMMA) {
      setState(210);
      match(SparqlUpdateParser::COMMA);
      setState(211);
      objectPath();
      setState(216);
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
  enterRule(_localctx, 40, SparqlUpdateParser::RuleObjectPath);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(217);
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
  enterRule(_localctx, 42, SparqlUpdateParser::RuleInteger);

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
  enterRule(_localctx, 44, SparqlUpdateParser::RuleTriplesNode);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(223);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlUpdateParser::OPEN_BRACE: {
        enterOuterAlt(_localctx, 1);
        setState(221);
        collection();
        break;
      }

      case SparqlUpdateParser::OPEN_SQUARE_BRACKET: {
        enterOuterAlt(_localctx, 2);
        setState(222);
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
  enterRule(_localctx, 46, SparqlUpdateParser::RuleBlankNodePropertyList);

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
    match(SparqlUpdateParser::OPEN_SQUARE_BRACKET);
    setState(226);
    propertyListNotEmpty();
    setState(227);
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
  enterRule(_localctx, 48, SparqlUpdateParser::RuleTriplesNodePath);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(231);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlUpdateParser::OPEN_BRACE: {
        enterOuterAlt(_localctx, 1);
        setState(229);
        collectionPath();
        break;
      }

      case SparqlUpdateParser::OPEN_SQUARE_BRACKET: {
        enterOuterAlt(_localctx, 2);
        setState(230);
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
  enterRule(_localctx, 50, SparqlUpdateParser::RuleBlankNodePropertyListPath);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(233);
    match(SparqlUpdateParser::OPEN_SQUARE_BRACKET);
    setState(234);
    propertyListPathNotEmpty();
    setState(235);
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
  enterRule(_localctx, 52, SparqlUpdateParser::RuleCollection);
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
    setState(237);
    match(SparqlUpdateParser::OPEN_BRACE);
    setState(239); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(238);
      graphNode();
      setState(241); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while (_la == SparqlUpdateParser::TRUE

    || _la == SparqlUpdateParser::FALSE || ((((_la - 120) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 120)) & 4564451215) != 0));
    setState(243);
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
  enterRule(_localctx, 54, SparqlUpdateParser::RuleCollectionPath);
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
    setState(245);
    match(SparqlUpdateParser::OPEN_BRACE);
    setState(247); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(246);
      graphNodePath();
      setState(249); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while (_la == SparqlUpdateParser::TRUE

    || _la == SparqlUpdateParser::FALSE || ((((_la - 120) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 120)) & 4564451215) != 0));
    setState(251);
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
  enterRule(_localctx, 56, SparqlUpdateParser::RuleGraphNode);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(255);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 23, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(253);
      graphTerm();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(254);
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
  enterRule(_localctx, 58, SparqlUpdateParser::RuleGraphNodePath);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(259);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 24, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(257);
      graphTerm();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(258);
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
  enterRule(_localctx, 60, SparqlUpdateParser::RuleGraphTerm);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(267);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlUpdateParser::IRIREF:
      case SparqlUpdateParser::PNAME_NS:
      case SparqlUpdateParser::PNAME_LN: {
        enterOuterAlt(_localctx, 1);
        setState(261);
        iri();
        break;
      }

      case SparqlUpdateParser::STRING_LITERAL1:
      case SparqlUpdateParser::STRING_LITERAL2:
      case SparqlUpdateParser::STRING_LITERAL_LONG1:
      case SparqlUpdateParser::STRING_LITERAL_LONG2: {
        enterOuterAlt(_localctx, 2);
        setState(262);
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
        setState(263);
        numericLiteral();
        break;
      }

      case SparqlUpdateParser::TRUE:
      case SparqlUpdateParser::FALSE: {
        enterOuterAlt(_localctx, 4);
        setState(264);
        booleanLiteral();
        break;
      }

      case SparqlUpdateParser::BLANK_NODE_LABEL:
      case SparqlUpdateParser::OPEN_SQUARE_BRACKET: {
        enterOuterAlt(_localctx, 5);
        setState(265);
        blankNode();
        break;
      }

      case SparqlUpdateParser::OPEN_BRACE: {
        enterOuterAlt(_localctx, 6);
        setState(266);
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
  enterRule(_localctx, 62, SparqlUpdateParser::RuleNil);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(269);
    match(SparqlUpdateParser::OPEN_BRACE);
    setState(270);
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
  enterRule(_localctx, 64, SparqlUpdateParser::RuleRdfLiteral);

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
    string();
    setState(276);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlUpdateParser::LANGTAG: {
        setState(273);
        match(SparqlUpdateParser::LANGTAG);
        break;
      }

      case SparqlUpdateParser::REFERENCE: {
        setState(274);
        match(SparqlUpdateParser::REFERENCE);
        setState(275);
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
  enterRule(_localctx, 66, SparqlUpdateParser::RuleNumericLiteral);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(281);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlUpdateParser::INTEGER:
      case SparqlUpdateParser::DECIMAL:
      case SparqlUpdateParser::DOUBLE: {
        enterOuterAlt(_localctx, 1);
        setState(278);
        numericLiteralUnsigned();
        break;
      }

      case SparqlUpdateParser::INTEGER_POSITIVE:
      case SparqlUpdateParser::DECIMAL_POSITIVE:
      case SparqlUpdateParser::DOUBLE_POSITIVE: {
        enterOuterAlt(_localctx, 2);
        setState(279);
        numericLiteralPositive();
        break;
      }

      case SparqlUpdateParser::INTEGER_NEGATIVE:
      case SparqlUpdateParser::DECIMAL_NEGATIVE:
      case SparqlUpdateParser::DOUBLE_NEGATIVE: {
        enterOuterAlt(_localctx, 3);
        setState(280);
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
  enterRule(_localctx, 68, SparqlUpdateParser::RuleNumericLiteralUnsigned);
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
    setState(283);
    _la = _input->LA(1);
    if (!(((((_la - 127) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 127)) & 7) != 0))) {
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
  enterRule(_localctx, 70, SparqlUpdateParser::RuleNumericLiteralPositive);
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
    _la = _input->LA(1);
    if (!(((((_la - 130) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 130)) & 7) != 0))) {
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
  enterRule(_localctx, 72, SparqlUpdateParser::RuleNumericLiteralNegative);
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
    setState(287);
    _la = _input->LA(1);
    if (!(((((_la - 133) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 133)) & 7) != 0))) {
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
  enterRule(_localctx, 74, SparqlUpdateParser::RuleBooleanLiteral);
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
    setState(289);
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
  enterRule(_localctx, 76, SparqlUpdateParser::RuleString);
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
    setState(291);
    _la = _input->LA(1);
    if (!(((((_la - 136) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 136)) & 15) != 0))) {
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
  enterRule(_localctx, 78, SparqlUpdateParser::RuleIri);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(295);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlUpdateParser::IRIREF: {
        enterOuterAlt(_localctx, 1);
        setState(293);
        match(SparqlUpdateParser::IRIREF);
        break;
      }

      case SparqlUpdateParser::PNAME_NS:
      case SparqlUpdateParser::PNAME_LN: {
        enterOuterAlt(_localctx, 2);
        setState(294);
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
  enterRule(_localctx, 80, SparqlUpdateParser::RulePrefixedName);
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
    setState(297);
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
  enterRule(_localctx, 82, SparqlUpdateParser::RuleBlankNode);

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
      case SparqlUpdateParser::BLANK_NODE_LABEL: {
        enterOuterAlt(_localctx, 1);
        setState(299);
        match(SparqlUpdateParser::BLANK_NODE_LABEL);
        break;
      }

      case SparqlUpdateParser::OPEN_SQUARE_BRACKET: {
        enterOuterAlt(_localctx, 2);
        setState(300);
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
  enterRule(_localctx, 84, SparqlUpdateParser::RuleAnon);

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
    match(SparqlUpdateParser::OPEN_SQUARE_BRACKET);
    setState(304);
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

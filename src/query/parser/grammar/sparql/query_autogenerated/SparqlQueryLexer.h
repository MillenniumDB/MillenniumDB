
// Generated from SparqlQueryLexer.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"




class  SparqlQueryLexer : public antlr4::Lexer {
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

  explicit SparqlQueryLexer(antlr4::CharStream *input);

  ~SparqlQueryLexer() override;


  std::string getGrammarFileName() const override;

  const std::vector<std::string>& getRuleNames() const override;

  const std::vector<std::string>& getChannelNames() const override;

  const std::vector<std::string>& getModeNames() const override;

  const antlr4::dfa::Vocabulary& getVocabulary() const override;

  antlr4::atn::SerializedATNView getSerializedATN() const override;

  const antlr4::atn::ATN& getATN() const override;

  // By default the static state used to implement the lexer is lazily initialized during the first
  // call to the constructor. You can call this function if you wish to initialize the static state
  // ahead of time.
  static void initialize();

private:

  // Individual action functions triggered by action() above.

  // Individual semantic predicate functions triggered by sempred() above.

};


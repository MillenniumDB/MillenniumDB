
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
    TRUE = 37, FALSE = 38, SILENT = 39, ALL = 40, AS = 41, GROUP = 42, HAVING = 43, 
    UNDEF = 44, BINDINGS = 45, SERVICE = 46, BIND = 47, MINUS = 48, IRI = 49, 
    URI = 50, BNODE = 51, RAND = 52, ABS = 53, CEIL = 54, FLOOR = 55, ROUND = 56, 
    CONCAT = 57, STRLEN = 58, UCASE = 59, LCASE = 60, ENCODE_FOR_URI = 61, 
    CONTAINS = 62, STRSTARTS = 63, STRENDS = 64, STRBEFORE = 65, STRAFTER = 66, 
    REPLACE = 67, YEAR = 68, MONTH = 69, DAY = 70, HOURS = 71, MINUTES = 72, 
    SECONDS = 73, TIMEZONE = 74, TZ = 75, NOW = 76, UUID = 77, STRUUID = 78, 
    MD5 = 79, SHA1 = 80, SHA256 = 81, SHA384 = 82, SHA512 = 83, COALESCE = 84, 
    IF = 85, STRLANG = 86, STRDT = 87, ISNUMERIC = 88, COUNT = 89, SUM = 90, 
    MIN = 91, MAX = 92, AVG = 93, SAMPLE = 94, GROUP_CONCAT = 95, NOT = 96, 
    IN = 97, EXISTS = 98, SEPARATOR = 99, ANY = 100, SHORTEST = 101, SIMPLE = 102, 
    ACYCLIC = 103, WALKS = 104, IRIREF = 105, PNAME_NS = 106, PNAME_LN = 107, 
    BLANK_NODE_LABEL = 108, VAR1 = 109, VAR2 = 110, LANGTAG = 111, INTEGER = 112, 
    DECIMAL = 113, DOUBLE = 114, INTEGER_POSITIVE = 115, DECIMAL_POSITIVE = 116, 
    DOUBLE_POSITIVE = 117, INTEGER_NEGATIVE = 118, DECIMAL_NEGATIVE = 119, 
    DOUBLE_NEGATIVE = 120, STRING_LITERAL1 = 121, STRING_LITERAL2 = 122, 
    STRING_LITERAL_LONG1 = 123, STRING_LITERAL_LONG2 = 124, COMMENT = 125, 
    REFERENCE = 126, LESS_EQUAL = 127, GREATER_EQUAL = 128, NOT_EQUAL = 129, 
    AND = 130, OR = 131, INVERSE = 132, OPEN_BRACE = 133, CLOSE_BRACE = 134, 
    OPEN_CURLY_BRACE = 135, CLOSE_CURLY_BRACE = 136, OPEN_SQUARE_BRACKET = 137, 
    CLOSE_SQUARE_BRACKET = 138, SEMICOLON = 139, DOT = 140, PLUS_SIGN = 141, 
    MINUS_SIGN = 142, ASTERISK = 143, QUESTION_MARK = 144, COMMA = 145, 
    NEGATION = 146, DIVIDE = 147, EQUAL = 148, LESS = 149, GREATER = 150, 
    PIPE = 151, ANY_OTHER = 152
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


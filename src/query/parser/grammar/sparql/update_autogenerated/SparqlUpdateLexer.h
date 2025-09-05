
// Generated from SparqlUpdateLexer.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"




class  SparqlUpdateLexer : public antlr4::Lexer {
public:
  enum {
    WS = 1, BASE = 2, PREFIX = 3, SELECT = 4, DISTINCT = 5, REDUCED = 6, 
    CONSTRUCT = 7, DESCRIBE = 8, ASK = 9, FROM = 10, NAMED = 11, WHERE = 12, 
    ORDER = 13, BY = 14, ASC = 15, DESC = 16, LIMIT = 17, OFFSET = 18, VALUES = 19, 
    OPTIONAL = 20, GRAPH = 21, UNION = 22, FILTER = 23, A = 24, STR = 25, 
    LANG = 26, LANGMATCHES = 27, DATATYPE = 28, BOUND = 29, SAMETERM = 30, 
    ISIRI = 31, ISURI = 32, ISBLANK = 33, ISLITERAL = 34, REGEX = 35, SUBSTR = 36, 
    K_TRUE = 37, K_FALSE = 38, LOAD = 39, CLEAR = 40, DROP = 41, ADD = 42, 
    MOVE = 43, COPY = 44, CREATE = 45, DELETE = 46, INSERT = 47, USING = 48, 
    SILENT = 49, DEFAULT = 50, ALL = 51, DATA = 52, WITH = 53, INTO = 54, 
    TO = 55, AS = 56, GROUP = 57, HAVING = 58, UNDEF = 59, BINDINGS = 60, 
    SERVICE = 61, BIND = 62, MINUS = 63, IRI = 64, URI = 65, BNODE = 66, 
    RAND = 67, ABS = 68, CEIL = 69, FLOOR = 70, ROUND = 71, CONCAT = 72, 
    STRLEN = 73, UCASE = 74, LCASE = 75, ENCODE_FOR_URI = 76, CONTAINS = 77, 
    STRSTARTS = 78, STRENDS = 79, STRBEFORE = 80, STRAFTER = 81, REPLACE = 82, 
    YEAR = 83, MONTH = 84, DAY = 85, HOURS = 86, MINUTES = 87, SECONDS = 88, 
    TIMEZONE = 89, TZ = 90, NOW = 91, UUID = 92, STRUUID = 93, MD5 = 94, 
    SHA1 = 95, SHA256 = 96, SHA384 = 97, SHA512 = 98, COALESCE = 99, IF = 100, 
    STRLANG = 101, STRDT = 102, ISNUMERIC = 103, COUNT = 104, SUM = 105, 
    MIN = 106, MAX = 107, AVG = 108, SAMPLE = 109, GROUP_CONCAT = 110, NOT = 111, 
    IN = 112, EXISTS = 113, SEPARATOR = 114, ANY = 115, SHORTEST = 116, 
    SIMPLE = 117, ACYCLIC = 118, WALKS = 119, INDEX = 120, ALPHANUMERIC_IDENTIFIER = 121, 
    IRIREF = 122, PNAME_NS = 123, PNAME_LN = 124, BLANK_NODE_LABEL = 125, 
    VAR1 = 126, VAR2 = 127, LANGTAG = 128, INTEGER = 129, DECIMAL = 130, 
    DOUBLE = 131, INTEGER_POSITIVE = 132, DECIMAL_POSITIVE = 133, DOUBLE_POSITIVE = 134, 
    INTEGER_NEGATIVE = 135, DECIMAL_NEGATIVE = 136, DOUBLE_NEGATIVE = 137, 
    STRING_LITERAL1 = 138, STRING_LITERAL2 = 139, STRING_LITERAL_LONG1 = 140, 
    STRING_LITERAL_LONG2 = 141, STRING_RAW1 = 142, STRING_RAW2 = 143, STRING_RAW_LONG1 = 144, 
    STRING_RAW_LONG2 = 145, COMMENT = 146, REFERENCE = 147, LESS_EQUAL = 148, 
    GREATER_EQUAL = 149, NOT_EQUAL = 150, AND = 151, OR = 152, INVERSE = 153, 
    OPEN_BRACE = 154, CLOSE_BRACE = 155, OPEN_CURLY_BRACE = 156, CLOSE_CURLY_BRACE = 157, 
    OPEN_SQUARE_BRACKET = 158, CLOSE_SQUARE_BRACKET = 159, SEMICOLON = 160, 
    DOT = 161, PLUS_SIGN = 162, MINUS_SIGN = 163, ASTERISK = 164, QUESTION_MARK = 165, 
    COMMA = 166, NEGATION = 167, DIVIDE = 168, EQUAL = 169, LESS = 170, 
    GREATER = 171, PIPE = 172, ANY_OTHER = 173
  };

  explicit SparqlUpdateLexer(antlr4::CharStream *input);

  ~SparqlUpdateLexer() override;


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


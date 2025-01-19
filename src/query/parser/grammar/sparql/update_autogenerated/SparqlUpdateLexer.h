
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
    TRUE = 37, FALSE = 38, LOAD = 39, CLEAR = 40, DROP = 41, ADD = 42, MOVE = 43, 
    COPY = 44, CREATE = 45, DELETE = 46, INSERT = 47, USING = 48, SILENT = 49, 
    DEFAULT = 50, ALL = 51, DATA = 52, WITH = 53, INTO = 54, TO = 55, AS = 56, 
    GROUP = 57, HAVING = 58, UNDEF = 59, BINDINGS = 60, SERVICE = 61, BIND = 62, 
    MINUS = 63, IRI = 64, URI = 65, BNODE = 66, RAND = 67, ABS = 68, CEIL = 69, 
    FLOOR = 70, ROUND = 71, CONCAT = 72, STRLEN = 73, UCASE = 74, LCASE = 75, 
    ENCODE_FOR_URI = 76, CONTAINS = 77, STRSTARTS = 78, STRENDS = 79, STRBEFORE = 80, 
    STRAFTER = 81, REPLACE = 82, YEAR = 83, MONTH = 84, DAY = 85, HOURS = 86, 
    MINUTES = 87, SECONDS = 88, TIMEZONE = 89, TZ = 90, NOW = 91, UUID = 92, 
    STRUUID = 93, MD5 = 94, SHA1 = 95, SHA256 = 96, SHA384 = 97, SHA512 = 98, 
    COALESCE = 99, IF = 100, STRLANG = 101, STRDT = 102, ISNUMERIC = 103, 
    COUNT = 104, SUM = 105, MIN = 106, MAX = 107, AVG = 108, SAMPLE = 109, 
    GROUP_CONCAT = 110, NOT = 111, IN = 112, EXISTS = 113, SEPARATOR = 114, 
    ANY = 115, SHORTEST = 116, SIMPLE = 117, ACYCLIC = 118, WALKS = 119, 
    IDENTITY = 120, INDEX = 121, NFKD_CASEFOLD = 122, NORMALIZE = 123, ON = 124, 
    TEXT = 125, TOKENIZE = 126, WHITESPACE = 127, WS_KEEP_PUNCT = 128, WS_RM_PUNCT = 129, 
    WS_SPLIT_PUNCT = 130, WS_SYMBOLS = 131, IRIREF = 132, PNAME_NS = 133, 
    PNAME_LN = 134, BLANK_NODE_LABEL = 135, VAR1 = 136, VAR2 = 137, LANGTAG = 138, 
    INTEGER = 139, DECIMAL = 140, DOUBLE = 141, INTEGER_POSITIVE = 142, 
    DECIMAL_POSITIVE = 143, DOUBLE_POSITIVE = 144, INTEGER_NEGATIVE = 145, 
    DECIMAL_NEGATIVE = 146, DOUBLE_NEGATIVE = 147, STRING_LITERAL1 = 148, 
    STRING_LITERAL2 = 149, STRING_LITERAL_LONG1 = 150, STRING_LITERAL_LONG2 = 151, 
    COMMENT = 152, REFERENCE = 153, LESS_EQUAL = 154, GREATER_EQUAL = 155, 
    NOT_EQUAL = 156, AND = 157, OR = 158, INVERSE = 159, OPEN_BRACE = 160, 
    CLOSE_BRACE = 161, OPEN_CURLY_BRACE = 162, CLOSE_CURLY_BRACE = 163, 
    OPEN_SQUARE_BRACKET = 164, CLOSE_SQUARE_BRACKET = 165, SEMICOLON = 166, 
    DOT = 167, PLUS_SIGN = 168, MINUS_SIGN = 169, ASTERISK = 170, QUESTION_MARK = 171, 
    COMMA = 172, NEGATION = 173, DIVIDE = 174, EQUAL = 175, LESS = 176, 
    GREATER = 177, PIPE = 178, ANY_OTHER = 179
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


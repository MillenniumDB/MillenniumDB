
// Generated from MQL_Lexer.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"




class  MQL_Lexer : public antlr4::Lexer {
public:
  enum {
    K_ACYCLIC = 1, K_AND = 2, K_ANY = 3, K_AS = 4, K_AVG = 5, K_ALL = 6, 
    K_ASC = 7, K_BY = 8, K_BOOL = 9, K_CALL = 10, K_COSINE_DISTANCE = 11, 
    K_COSINE_SIMILARITY = 12, K_COUNT = 13, K_CREATE = 14, K_DELETE = 15, 
    K_DESCRIBE = 16, K_DESC = 17, K_DIMENSIONS = 18, K_DISTINCT = 19, K_EDGE = 20, 
    K_EUCLIDEAN_DISTANCE = 21, K_FROM = 22, K_INCOMING = 23, K_INDEX = 24, 
    K_INSERT = 25, K_INTEGER = 26, K_INTO = 27, K_IS = 28, K_FALSE = 29, 
    K_FLOAT = 30, K_GROUP = 31, K_GROUPS = 32, K_IDENTITY = 33, K_LABELS = 34, 
    K_LABEL = 35, K_LET = 36, K_LIMIT = 37, K_MANHATTAN_DISTANCE = 38, K_MATCH = 39, 
    K_MAX = 40, K_MIN = 41, K_OBJECTS = 42, K_OFFSET = 43, K_OPTIONAL = 44, 
    K_ORDER = 45, K_OR = 46, K_OUTGOING = 47, K_PREFIX = 48, K_PROPERTIES = 49, 
    K_PROPERTY = 50, K_NOT = 51, K_NULL = 52, K_SHORTEST = 53, K_SHOW = 54, 
    K_SIMPLE = 55, K_TEXT_SEARCH = 56, K_REGEX = 57, K_RETURN = 58, K_SUM = 59, 
    K_STRING = 60, K_TRUE = 61, K_TRAILS = 62, K_VALUES = 63, K_WALKS = 64, 
    K_WITH = 65, K_WHERE = 66, K_YIELD = 67, TRUE_PROP = 68, FALSE_PROP = 69, 
    ANON_ID = 70, EDGE_ID = 71, KEY = 72, TYPE = 73, TYPE_VAR = 74, VARIABLE = 75, 
    STRING = 76, UNSIGNED_INTEGER = 77, UNSIGNED_FLOAT = 78, UNSIGNED_SCIENTIFIC_NOTATION = 79, 
    NAME = 80, LEQ = 81, GEQ = 82, EQ = 83, NEQ = 84, LT = 85, GT = 86, 
    SINGLE_EQ = 87, PATH_SEQUENCE = 88, PATH_ALTERNATIVE = 89, PATH_NEGATION = 90, 
    STAR = 91, PERCENT = 92, QUESTION_MARK = 93, PLUS = 94, MINUS = 95, 
    L_PAR = 96, R_PAR = 97, LCURLY_BRACKET = 98, RCURLY_BRACKET = 99, LSQUARE_BRACKET = 100, 
    RSQUARE_BRACKET = 101, COMMA = 102, COLON = 103, WHITE_SPACE = 104, 
    SINGLE_LINE_COMMENT = 105, UNRECOGNIZED = 106
  };

  enum {
    WS_CHANNEL = 2
  };

  explicit MQL_Lexer(antlr4::CharStream *input);

  ~MQL_Lexer() override;


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


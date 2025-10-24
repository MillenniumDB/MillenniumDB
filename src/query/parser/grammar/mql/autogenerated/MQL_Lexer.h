
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
    K_EDIT_DISTANCE = 21, K_EUCLIDEAN_DISTANCE = 22, K_FROM = 23, K_INCOMING = 24, 
    K_INDEX = 25, K_INSERT = 26, K_INTEGER = 27, K_INTO = 28, K_IS = 29, 
    K_FALSE = 30, K_FLOAT = 31, K_GROUP = 32, K_GROUPS = 33, K_IDENTITY = 34, 
    K_LABELS = 35, K_LABEL = 36, K_LET = 37, K_LIMIT = 38, K_MANHATTAN_DISTANCE = 39, 
    K_MATCH = 40, K_MAX = 41, K_MIN = 42, K_NORMALIZE = 43, K_OBJECTS = 44, 
    K_OFFSET = 45, K_OPTIONAL = 46, K_ORDER = 47, K_OR = 48, K_OUTGOING = 49, 
    K_PREFIX = 50, K_PROPERTIES = 51, K_PROPERTY = 52, K_NOT = 53, K_NULL = 54, 
    K_SHORTEST = 55, K_SHOW = 56, K_SIMPLE = 57, K_STRING = 58, K_STR = 59, 
    K_TEXT_SEARCH = 60, K_TYPE = 61, K_REGEX = 62, K_RETURN = 63, K_SUM = 64, 
    K_TRUE = 65, K_TRAILS = 66, K_VALUES = 67, K_WALKS = 68, K_WITH = 69, 
    K_WHERE = 70, K_YIELD = 71, TRUE_PROP = 72, FALSE_PROP = 73, ANON_ID = 74, 
    EDGE_ID = 75, KEY = 76, TYPE = 77, TYPE_VAR = 78, VARIABLE = 79, STRING = 80, 
    UNSIGNED_INTEGER = 81, UNSIGNED_FLOAT = 82, UNSIGNED_SCIENTIFIC_NOTATION = 83, 
    NAME = 84, LEQ = 85, GEQ = 86, EQ = 87, NEQ = 88, LT = 89, GT = 90, 
    SINGLE_EQ = 91, PATH_SEQUENCE = 92, PATH_ALTERNATIVE = 93, PATH_NEGATION = 94, 
    STAR = 95, PERCENT = 96, QUESTION_MARK = 97, PLUS = 98, MINUS = 99, 
    L_PAR = 100, R_PAR = 101, LCURLY_BRACKET = 102, RCURLY_BRACKET = 103, 
    LSQUARE_BRACKET = 104, RSQUARE_BRACKET = 105, COMMA = 106, COLON = 107, 
    WHITE_SPACE = 108, SINGLE_LINE_COMMENT = 109, UNRECOGNIZED = 110
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



// Generated from MQL_Lexer.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"




class  MQL_Lexer : public antlr4::Lexer {
public:
  enum {
    K_ACYCLIC = 1, K_AND = 2, K_ANY = 3, K_AS = 4, K_AVG = 5, K_ALL = 6, 
    K_ASC = 7, K_BY = 8, K_BOOL = 9, K_CALL = 10, K_COSINE_DISTANCE = 11, 
    K_COSINE_SIMILARITY = 12, K_COUNT = 13, K_CREATE = 14, K_DELETE = 15, 
    K_DESCRIBE = 16, K_DESC = 17, K_DETACH = 18, K_DIMENSIONS = 19, K_DISTINCT = 20, 
    K_EDGE = 21, K_EDIT_DISTANCE = 22, K_EUCLIDEAN_DISTANCE = 23, K_FROM = 24, 
    K_IDENTITY = 25, K_INCOMING = 26, K_INDEX = 27, K_INSERT = 28, K_INTEGER = 29, 
    K_INTO = 30, K_IS = 31, K_FALSE = 32, K_FLOAT = 33, K_GROUP = 34, K_GROUPS = 35, 
    K_LABELS = 36, K_LABEL = 37, K_LET = 38, K_LIMIT = 39, K_MANHATTAN_DISTANCE = 40, 
    K_MATCH = 41, K_MAX = 42, K_MIN = 43, K_NORMALIZE = 44, K_OBJECTS = 45, 
    K_OFFSET = 46, K_OPTIONAL = 47, K_ORDER = 48, K_OR = 49, K_OUTGOING = 50, 
    K_PREFIX = 51, K_PROPERTY = 52, K_PROPERTIES = 53, K_NOT = 54, K_NULL = 55, 
    K_SHORTEST = 56, K_SHOW = 57, K_SIMPLE = 58, K_STRING = 59, K_STR = 60, 
    K_TEXT_SEARCH = 61, K_TYPE = 62, K_REGEX = 63, K_REMOVE = 64, K_RETURN = 65, 
    K_SET = 66, K_SUM = 67, K_TRUE = 68, K_TRAILS = 69, K_VALUES = 70, K_WALKS = 71, 
    K_WITH = 72, K_WHERE = 73, K_YIELD = 74, TRUE_PROP = 75, FALSE_PROP = 76, 
    ANON_ID = 77, EDGE_ID = 78, KEY = 79, TYPE = 80, TYPE_VAR = 81, VARIABLE = 82, 
    STRING = 83, UNSIGNED_INTEGER = 84, UNSIGNED_FLOAT = 85, UNSIGNED_SCIENTIFIC_NOTATION = 86, 
    NAME = 87, LEQ = 88, GEQ = 89, EQ = 90, NEQ = 91, LT = 92, GT = 93, 
    SINGLE_EQ = 94, PATH_SEQUENCE = 95, PATH_ALTERNATIVE = 96, PATH_NEGATION = 97, 
    STAR = 98, PERCENT = 99, QUESTION_MARK = 100, PLUS = 101, MINUS = 102, 
    L_PAR = 103, R_PAR = 104, LCURLY_BRACKET = 105, RCURLY_BRACKET = 106, 
    LSQUARE_BRACKET = 107, RSQUARE_BRACKET = 108, COMMA = 109, COLON = 110, 
    WHITE_SPACE = 111, SINGLE_LINE_COMMENT = 112, UNRECOGNIZED = 113
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


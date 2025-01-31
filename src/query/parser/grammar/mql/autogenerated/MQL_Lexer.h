
// Generated from MQL_Lexer.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"




class  MQL_Lexer : public antlr4::Lexer {
public:
  enum {
    K_ACYCLIC = 1, K_AND = 2, K_ANGULAR = 3, K_ANY = 4, K_AS = 5, K_AVG = 6, 
    K_ALL = 7, K_ASC = 8, K_BY = 9, K_BOOL = 10, K_COUNT = 11, K_CREATE = 12, 
    K_DELETE = 13, K_DESCRIBE = 14, K_DESC = 15, K_DIMENSIONS = 16, K_DISTINCT = 17, 
    K_EDGE = 18, K_EUCLIDEAN = 19, K_FROM = 20, K_INCOMING = 21, K_INDEX = 22, 
    K_INSERT = 23, K_INTEGER = 24, K_INTO = 25, K_IS = 26, K_FALSE = 27, 
    K_FLOAT = 28, K_GROUP = 29, K_IDENTITY = 30, K_LABELS = 31, K_LABEL = 32, 
    K_LIMIT = 33, K_MANHATTAN = 34, K_MATCH = 35, K_MAX = 36, K_MIN = 37, 
    K_NFKD_CASEFOLD = 38, K_NORMALIZE = 39, K_OBJECTS = 40, K_OFFSET = 41, 
    K_ON = 42, K_OPTIONAL = 43, K_ORDER = 44, K_OR = 45, K_OUTGOING = 46, 
    K_PREFIX = 47, K_PROPERTIES = 48, K_PROPERTY = 49, K_NOT = 50, K_NULL = 51, 
    K_SHORTEST = 52, K_SHOW = 53, K_SIMPLE = 54, K_TENSOR_DISTANCE = 55, 
    K_TEXT_SEARCH = 56, K_REGEX = 57, K_RETURN = 58, K_SET = 59, K_SUM = 60, 
    K_STRING = 61, K_STORE = 62, K_TENSOR = 63, K_TEXT = 64, K_TRUE = 65, 
    K_TOKENIZE = 66, K_TRAILS = 67, K_VALUES = 68, K_WALKS = 69, K_WITH = 70, 
    K_WHERE = 71, K_WS_KEEP_PUNCT = 72, K_WS_RM_PUNCT = 73, K_WS_SPLIT_PUNCT = 74, 
    TRUE_PROP = 75, FALSE_PROP = 76, ANON_ID = 77, EDGE_ID = 78, KEY = 79, 
    TYPE = 80, TYPE_VAR = 81, VARIABLE = 82, STRING = 83, UNSIGNED_INTEGER = 84, 
    UNSIGNED_FLOAT = 85, UNSIGNED_SCIENTIFIC_NOTATION = 86, NAME = 87, LEQ = 88, 
    GEQ = 89, EQ = 90, NEQ = 91, LT = 92, GT = 93, SINGLE_EQ = 94, PATH_SEQUENCE = 95, 
    PATH_ALTERNATIVE = 96, PATH_NEGATION = 97, STAR = 98, PERCENT = 99, 
    QUESTION_MARK = 100, PLUS = 101, MINUS = 102, L_PAR = 103, R_PAR = 104, 
    LCURLY_BRACKET = 105, RCURLY_BRACKET = 106, LSQUARE_BRACKET = 107, RSQUARE_BRACKET = 108, 
    COMMA = 109, COLON = 110, WHITE_SPACE = 111, SINGLE_LINE_COMMENT = 112, 
    UNRECOGNIZED = 113
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



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
    K_SHORTEST = 55, K_SHOW = 56, K_SIMPLE = 57, K_STR = 58, K_TEXT_SEARCH = 59, 
    K_REGEX = 60, K_RETURN = 61, K_SUM = 62, K_STRING = 63, K_TRUE = 64, 
    K_TRAILS = 65, K_VALUES = 66, K_WALKS = 67, K_WITH = 68, K_WHERE = 69, 
    K_YIELD = 70, TRUE_PROP = 71, FALSE_PROP = 72, ANON_ID = 73, EDGE_ID = 74, 
    KEY = 75, TYPE = 76, TYPE_VAR = 77, VARIABLE = 78, STRING = 79, UNSIGNED_INTEGER = 80, 
    UNSIGNED_FLOAT = 81, UNSIGNED_SCIENTIFIC_NOTATION = 82, NAME = 83, LEQ = 84, 
    GEQ = 85, EQ = 86, NEQ = 87, LT = 88, GT = 89, SINGLE_EQ = 90, PATH_SEQUENCE = 91, 
    PATH_ALTERNATIVE = 92, PATH_NEGATION = 93, STAR = 94, PERCENT = 95, 
    QUESTION_MARK = 96, PLUS = 97, MINUS = 98, L_PAR = 99, R_PAR = 100, 
    LCURLY_BRACKET = 101, RCURLY_BRACKET = 102, LSQUARE_BRACKET = 103, RSQUARE_BRACKET = 104, 
    COMMA = 105, COLON = 106, WHITE_SPACE = 107, SINGLE_LINE_COMMENT = 108, 
    UNRECOGNIZED = 109
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


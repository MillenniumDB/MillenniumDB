
// Generated from MQL_Lexer.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"




class  MQL_Lexer : public antlr4::Lexer {
public:
  enum {
    K_ACYCLIC = 1, K_AND = 2, K_ANY = 3, K_AS = 4, K_AVG = 5, K_ALL = 6, 
    K_ASC = 7, K_BY = 8, K_BOOL = 9, K_CALL = 10, K_COSINE_DISTANCE = 11, 
    K_COSINE_SIMILARITY = 12, K_COUNT = 13, K_CREATE = 14, K_DAY = 15, K_DELETE = 16, 
    K_DESCRIBE = 17, K_DESC = 18, K_DETACH = 19, K_DIMENSIONS = 20, K_DISTINCT = 21, 
    K_EDGE = 22, K_EDIT_DISTANCE = 23, K_EUCLIDEAN_DISTANCE = 24, K_FROM = 25, 
    K_HAVING = 26, K_IDENTITY = 27, K_INCOMING = 28, K_INDEX = 29, K_INSERT = 30, 
    K_INTEGER = 31, K_INTO = 32, K_IS = 33, K_FALSE = 34, K_FLOAT = 35, 
    K_GROUP = 36, K_GROUPS = 37, K_LABELS = 38, K_LABEL = 39, K_LET = 40, 
    K_LIMIT = 41, K_MANHATTAN_DISTANCE = 42, K_MATCH = 43, K_MAX = 44, K_MIN = 45, 
    K_MONTH = 46, K_NORMALIZE = 47, K_OBJECTS = 48, K_OFFSET = 49, K_OPTIONAL = 50, 
    K_ORDER = 51, K_OR = 52, K_OUTGOING = 53, K_PREFIX = 54, K_PROPERTY = 55, 
    K_PROPERTIES = 56, K_NOT = 57, K_NULL = 58, K_SHORTEST = 59, K_SHOW = 60, 
    K_SIMPLE = 61, K_STRING = 62, K_STR = 63, K_TEXT_SEARCH = 64, K_TYPE = 65, 
    K_REGEX = 66, K_REMOVE = 67, K_RETURN = 68, K_SET = 69, K_SUM = 70, 
    K_TRUE = 71, K_TRAILS = 72, K_VALUES = 73, K_WALKS = 74, K_WITH = 75, 
    K_WHERE = 76, K_YEAR = 77, K_YIELD = 78, TRUE_PROP = 79, FALSE_PROP = 80, 
    ANON_ID = 81, EDGE_ID = 82, KEY = 83, TYPE = 84, TYPE_VAR = 85, VARIABLE = 86, 
    STRING = 87, UNSIGNED_INTEGER = 88, UNSIGNED_FLOAT = 89, UNSIGNED_SCIENTIFIC_NOTATION = 90, 
    NAME = 91, LEQ = 92, GEQ = 93, EQ = 94, NEQ = 95, LT = 96, GT = 97, 
    SINGLE_EQ = 98, PATH_SEQUENCE = 99, PATH_ALTERNATIVE = 100, PATH_NEGATION = 101, 
    STAR = 102, PERCENT = 103, QUESTION_MARK = 104, PLUS = 105, MINUS = 106, 
    L_PAR = 107, R_PAR = 108, LCURLY_BRACKET = 109, RCURLY_BRACKET = 110, 
    LSQUARE_BRACKET = 111, RSQUARE_BRACKET = 112, COMMA = 113, COLON = 114, 
    WHITE_SPACE = 115, SINGLE_LINE_COMMENT = 116, UNRECOGNIZED = 117
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


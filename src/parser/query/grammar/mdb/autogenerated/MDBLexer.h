
// Generated from MDBLexer.g4 by ANTLR 4.9.3

#pragma once


#include "antlr4-runtime.h"




class  MDBLexer : public antlr4::Lexer {
public:
  enum {
    K_ANY_SIMPLE = 1, K_ANY_TRAILS = 2, K_ANY = 3, K_AND = 4, K_AVG = 5, 
    K_ALL_COUNT = 6, K_ALL = 7, K_ASC = 8, K_BY = 9, K_BOOL = 10, K_COUNT = 11, 
    K_DESCRIBE = 12, K_DESC = 13, K_DISTINCT = 14, K_EDGE = 15, K_INTEGER = 16, 
    K_INSERT = 17, K_IS = 18, K_FALSE = 19, K_FLOAT = 20, K_GROUP = 21, 
    K_LABEL = 22, K_LIMIT = 23, K_MAX = 24, K_MATCH = 25, K_MIN = 26, K_OPTIONAL = 27, 
    K_ORDER = 28, K_OR = 29, K_PROPERTY = 30, K_NOT = 31, K_NULL = 32, K_RETURN = 33, 
    K_SET = 34, K_SIMPLE = 35, K_STRING = 36, K_SUM = 37, K_TRAILS = 38, 
    K_TRUE = 39, K_WHERE = 40, TRUE_PROP = 41, FALSE_PROP = 42, ANON_ID = 43, 
    EDGE_ID = 44, KEY = 45, TYPE = 46, TYPE_VAR = 47, VARIABLE = 48, STRING = 49, 
    UNSIGNED_INTEGER = 50, UNSIGNED_FLOAT = 51, NAME = 52, LEQ = 53, GEQ = 54, 
    EQ = 55, NEQ = 56, LT = 57, GT = 58, SINGLE_EQ = 59, PATH_SEQUENCE = 60, 
    PATH_ALTERNATIVE = 61, PATH_NEGATION = 62, STAR = 63, PERCENT = 64, 
    QUESTION_MARK = 65, PLUS = 66, MINUS = 67, L_PAR = 68, R_PAR = 69, LCURLY_BRACKET = 70, 
    RCURLY_BRACKET = 71, LSQUARE_BRACKET = 72, RSQUARE_BRACKET = 73, COMMA = 74, 
    COLON = 75, WHITE_SPACE = 76, SINGLE_LINE_COMMENT = 77, UNRECOGNIZED = 78
  };

  enum {
    WS_CHANNEL = 2
  };

  explicit MDBLexer(antlr4::CharStream *input);
  ~MDBLexer();

  virtual std::string getGrammarFileName() const override;
  virtual const std::vector<std::string>& getRuleNames() const override;

  virtual const std::vector<std::string>& getChannelNames() const override;
  virtual const std::vector<std::string>& getModeNames() const override;
  virtual const std::vector<std::string>& getTokenNames() const override; // deprecated, use vocabulary instead
  virtual antlr4::dfa::Vocabulary& getVocabulary() const override;

  virtual const std::vector<uint16_t> getSerializedATN() const override;
  virtual const antlr4::atn::ATN& getATN() const override;

private:
  static std::vector<antlr4::dfa::DFA> _decisionToDFA;
  static antlr4::atn::PredictionContextCache _sharedContextCache;
  static std::vector<std::string> _ruleNames;
  static std::vector<std::string> _tokenNames;
  static std::vector<std::string> _channelNames;
  static std::vector<std::string> _modeNames;

  static std::vector<std::string> _literalNames;
  static std::vector<std::string> _symbolicNames;
  static antlr4::dfa::Vocabulary _vocabulary;
  static antlr4::atn::ATN _atn;
  static std::vector<uint16_t> _serializedATN;


  // Individual action functions triggered by action() above.

  // Individual semantic predicate functions triggered by sempred() above.

  struct Initializer {
    Initializer();
  };
  static Initializer _init;
};


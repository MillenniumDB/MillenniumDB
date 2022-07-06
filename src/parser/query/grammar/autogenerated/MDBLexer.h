
// Generated from MDBLexer.g4 by ANTLR 4.9.3

#pragma once


#include "antlr4-runtime.h"




class  MDBLexer : public antlr4::Lexer {
public:
  enum {
    K_ANY = 1, K_AND = 2, K_AVG = 3, K_ALL = 4, K_ASC = 5, K_BY = 6, K_BOOL = 7, 
    K_COUNT = 8, K_DESCRIBE = 9, K_DESC = 10, K_DISTINCT = 11, K_IS = 12, 
    K_INTEGER = 13, K_FALSE = 14, K_FLOAT = 15, K_GROUP = 16, K_LIMIT = 17, 
    K_MAX = 18, K_MATCH = 19, K_MIN = 20, K_OPTIONAL = 21, K_ORDER = 22, 
    K_OR = 23, K_NOT = 24, K_NULL = 25, K_SET = 26, K_SUM = 27, K_STRING = 28, 
    K_RETURN = 29, K_TRUE = 30, K_WHERE = 31, TRUE_PROP = 32, FALSE_PROP = 33, 
    ANON_ID = 34, EDGE_ID = 35, KEY = 36, TYPE = 37, TYPE_VAR = 38, VARIABLE = 39, 
    STRING = 40, UNSIGNED_INTEGER = 41, UNSIGNED_FLOAT = 42, NAME = 43, 
    LEQ = 44, GEQ = 45, EQ = 46, NEQ = 47, LT = 48, GT = 49, SINGLE_EQ = 50, 
    PATH_SEQUENCE = 51, PATH_ALTERNATIVE = 52, PATH_NEGATION = 53, STAR = 54, 
    PERCENT = 55, QUESTION_MARK = 56, PLUS = 57, MINUS = 58, L_PAR = 59, 
    R_PAR = 60, LCURLY_BRACKET = 61, RCURLY_BRACKET = 62, LSQUARE_BRACKET = 63, 
    RSQUARE_BRACKET = 64, COMMA = 65, COLON = 66, WHITE_SPACE = 67, SINGLE_LINE_COMMENT = 68, 
    UNRECOGNIZED = 69
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


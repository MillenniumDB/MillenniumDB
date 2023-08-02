
// Generated from MQL_Lexer.g4 by ANTLR 4.9.3

#pragma once


#include "antlr4-runtime.h"




class  MQL_Lexer : public antlr4::Lexer {
public:
  enum {
    K_ANY = 1, K_AND = 2, K_AVG = 3, K_ALL = 4, K_ASC = 5, K_BY = 6, K_BOOL = 7, 
    K_COUNT = 8, K_DESCRIBE = 9, K_DESC = 10, K_DISTINCT = 11, K_EDGE = 12, 
    K_INCOMING = 13, K_INTEGER = 14, K_INSERT = 15, K_IS = 16, K_FALSE = 17, 
    K_FLOAT = 18, K_GROUP = 19, K_LABEL = 20, K_LABELS = 21, K_LIMIT = 22, 
    K_MAX = 23, K_MATCH = 24, K_MIN = 25, K_OPTIONAL = 26, K_ORDER = 27, 
    K_OR = 28, K_OUTGOING = 29, K_PROPERTY = 30, K_PROPERTIES = 31, K_NOT = 32, 
    K_NULL = 33, K_SET = 34, K_SUM = 35, K_STRING = 36, K_RETURN = 37, K_TRUE = 38, 
    K_WHERE = 39, TRUE_PROP = 40, FALSE_PROP = 41, ANON_ID = 42, EDGE_ID = 43, 
    KEY = 44, TYPE = 45, TYPE_VAR = 46, VARIABLE = 47, STRING = 48, UNSIGNED_INTEGER = 49, 
    UNSIGNED_FLOAT = 50, NAME = 51, LEQ = 52, GEQ = 53, EQ = 54, NEQ = 55, 
    LT = 56, GT = 57, SINGLE_EQ = 58, PATH_SEQUENCE = 59, PATH_ALTERNATIVE = 60, 
    PATH_NEGATION = 61, STAR = 62, PERCENT = 63, QUESTION_MARK = 64, PLUS = 65, 
    MINUS = 66, L_PAR = 67, R_PAR = 68, LCURLY_BRACKET = 69, RCURLY_BRACKET = 70, 
    LSQUARE_BRACKET = 71, RSQUARE_BRACKET = 72, COMMA = 73, COLON = 74, 
    WHITE_SPACE = 75, SINGLE_LINE_COMMENT = 76, UNRECOGNIZED = 77
  };

  enum {
    WS_CHANNEL = 2
  };

  explicit MQL_Lexer(antlr4::CharStream *input);
  ~MQL_Lexer();

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


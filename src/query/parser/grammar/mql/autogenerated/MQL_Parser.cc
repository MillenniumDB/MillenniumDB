
// Generated from MQL_Parser.g4 by ANTLR 4.9.3


#include "MQL_ParserVisitor.h"

#include "MQL_Parser.h"


using namespace antlrcpp;
using namespace antlr4;

MQL_Parser::MQL_Parser(TokenStream *input) : Parser(input) {
  _interpreter = new atn::ParserATNSimulator(this, _atn, _decisionToDFA, _sharedContextCache);
}

MQL_Parser::~MQL_Parser() {
  delete _interpreter;
}

std::string MQL_Parser::getGrammarFileName() const {
  return "MQL_Parser.g4";
}

const std::vector<std::string>& MQL_Parser::getRuleNames() const {
  return _ruleNames;
}

dfa::Vocabulary& MQL_Parser::getVocabulary() const {
  return _vocabulary;
}


//----------------- RootContext ------------------------------------------------------------------

MQL_Parser::RootContext::RootContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::RootContext::EOF() {
  return getToken(MQL_Parser::EOF, 0);
}

MQL_Parser::MatchQueryContext* MQL_Parser::RootContext::matchQuery() {
  return getRuleContext<MQL_Parser::MatchQueryContext>(0);
}

MQL_Parser::DescribeQueryContext* MQL_Parser::RootContext::describeQuery() {
  return getRuleContext<MQL_Parser::DescribeQueryContext>(0);
}

MQL_Parser::InsertQueryContext* MQL_Parser::RootContext::insertQuery() {
  return getRuleContext<MQL_Parser::InsertQueryContext>(0);
}

MQL_Parser::SetStatementContext* MQL_Parser::RootContext::setStatement() {
  return getRuleContext<MQL_Parser::SetStatementContext>(0);
}


size_t MQL_Parser::RootContext::getRuleIndex() const {
  return MQL_Parser::RuleRoot;
}


antlrcpp::Any MQL_Parser::RootContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitRoot(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::RootContext* MQL_Parser::root() {
  RootContext *_localctx = _tracker.createInstance<RootContext>(_ctx, getState());
  enterRule(_localctx, 0, MQL_Parser::RuleRoot);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(109);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MQL_Parser::K_SET) {
      setState(108);
      setStatement();
    }
    setState(114);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case MQL_Parser::K_MATCH: {
        setState(111);
        matchQuery();
        break;
      }

      case MQL_Parser::K_DESCRIBE: {
        setState(112);
        describeQuery();
        break;
      }

      case MQL_Parser::K_INSERT: {
        setState(113);
        insertQuery();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    setState(116);
    match(MQL_Parser::EOF);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- MatchQueryContext ------------------------------------------------------------------

MQL_Parser::MatchQueryContext::MatchQueryContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

MQL_Parser::MatchStatementContext* MQL_Parser::MatchQueryContext::matchStatement() {
  return getRuleContext<MQL_Parser::MatchStatementContext>(0);
}

MQL_Parser::ReturnStatementContext* MQL_Parser::MatchQueryContext::returnStatement() {
  return getRuleContext<MQL_Parser::ReturnStatementContext>(0);
}

MQL_Parser::WhereStatementContext* MQL_Parser::MatchQueryContext::whereStatement() {
  return getRuleContext<MQL_Parser::WhereStatementContext>(0);
}

MQL_Parser::GroupByStatementContext* MQL_Parser::MatchQueryContext::groupByStatement() {
  return getRuleContext<MQL_Parser::GroupByStatementContext>(0);
}

MQL_Parser::OrderByStatementContext* MQL_Parser::MatchQueryContext::orderByStatement() {
  return getRuleContext<MQL_Parser::OrderByStatementContext>(0);
}


size_t MQL_Parser::MatchQueryContext::getRuleIndex() const {
  return MQL_Parser::RuleMatchQuery;
}


antlrcpp::Any MQL_Parser::MatchQueryContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitMatchQuery(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::MatchQueryContext* MQL_Parser::matchQuery() {
  MatchQueryContext *_localctx = _tracker.createInstance<MatchQueryContext>(_ctx, getState());
  enterRule(_localctx, 2, MQL_Parser::RuleMatchQuery);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(118);
    matchStatement();
    setState(120);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MQL_Parser::K_WHERE) {
      setState(119);
      whereStatement();
    }
    setState(123);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MQL_Parser::K_GROUP) {
      setState(122);
      groupByStatement();
    }
    setState(126);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MQL_Parser::K_ORDER) {
      setState(125);
      orderByStatement();
    }
    setState(128);
    returnStatement();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DescribeQueryContext ------------------------------------------------------------------

MQL_Parser::DescribeQueryContext::DescribeQueryContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::DescribeQueryContext::K_DESCRIBE() {
  return getToken(MQL_Parser::K_DESCRIBE, 0);
}

MQL_Parser::FixedNodeInsideContext* MQL_Parser::DescribeQueryContext::fixedNodeInside() {
  return getRuleContext<MQL_Parser::FixedNodeInsideContext>(0);
}

std::vector<MQL_Parser::DescribeFlagContext *> MQL_Parser::DescribeQueryContext::describeFlag() {
  return getRuleContexts<MQL_Parser::DescribeFlagContext>();
}

MQL_Parser::DescribeFlagContext* MQL_Parser::DescribeQueryContext::describeFlag(size_t i) {
  return getRuleContext<MQL_Parser::DescribeFlagContext>(i);
}


size_t MQL_Parser::DescribeQueryContext::getRuleIndex() const {
  return MQL_Parser::RuleDescribeQuery;
}


antlrcpp::Any MQL_Parser::DescribeQueryContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitDescribeQuery(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::DescribeQueryContext* MQL_Parser::describeQuery() {
  DescribeQueryContext *_localctx = _tracker.createInstance<DescribeQueryContext>(_ctx, getState());
  enterRule(_localctx, 4, MQL_Parser::RuleDescribeQuery);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(130);
    match(MQL_Parser::K_DESCRIBE);
    setState(134);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << MQL_Parser::K_INCOMING)
      | (1ULL << MQL_Parser::K_LABELS)
      | (1ULL << MQL_Parser::K_OUTGOING)
      | (1ULL << MQL_Parser::K_PROPERTIES))) != 0)) {
      setState(131);
      describeFlag();
      setState(136);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(137);
    fixedNodeInside();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DescribeFlagContext ------------------------------------------------------------------

MQL_Parser::DescribeFlagContext::DescribeFlagContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::DescribeFlagContext::K_LABELS() {
  return getToken(MQL_Parser::K_LABELS, 0);
}

tree::TerminalNode* MQL_Parser::DescribeFlagContext::K_PROPERTIES() {
  return getToken(MQL_Parser::K_PROPERTIES, 0);
}

tree::TerminalNode* MQL_Parser::DescribeFlagContext::K_OUTGOING() {
  return getToken(MQL_Parser::K_OUTGOING, 0);
}

tree::TerminalNode* MQL_Parser::DescribeFlagContext::K_INCOMING() {
  return getToken(MQL_Parser::K_INCOMING, 0);
}

tree::TerminalNode* MQL_Parser::DescribeFlagContext::K_LIMIT() {
  return getToken(MQL_Parser::K_LIMIT, 0);
}

tree::TerminalNode* MQL_Parser::DescribeFlagContext::UNSIGNED_INTEGER() {
  return getToken(MQL_Parser::UNSIGNED_INTEGER, 0);
}


size_t MQL_Parser::DescribeFlagContext::getRuleIndex() const {
  return MQL_Parser::RuleDescribeFlag;
}


antlrcpp::Any MQL_Parser::DescribeFlagContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitDescribeFlag(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::DescribeFlagContext* MQL_Parser::describeFlag() {
  DescribeFlagContext *_localctx = _tracker.createInstance<DescribeFlagContext>(_ctx, getState());
  enterRule(_localctx, 6, MQL_Parser::RuleDescribeFlag);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(139);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << MQL_Parser::K_INCOMING)
      | (1ULL << MQL_Parser::K_LABELS)
      | (1ULL << MQL_Parser::K_OUTGOING)
      | (1ULL << MQL_Parser::K_PROPERTIES))) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
    setState(142);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 6, _ctx)) {
    case 1: {
      setState(140);
      match(MQL_Parser::K_LIMIT);
      setState(141);
      match(MQL_Parser::UNSIGNED_INTEGER);
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- InsertQueryContext ------------------------------------------------------------------

MQL_Parser::InsertQueryContext::InsertQueryContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::InsertQueryContext::K_INSERT() {
  return getToken(MQL_Parser::K_INSERT, 0);
}

MQL_Parser::InsertLabelListContext* MQL_Parser::InsertQueryContext::insertLabelList() {
  return getRuleContext<MQL_Parser::InsertLabelListContext>(0);
}

MQL_Parser::InsertPropertyListContext* MQL_Parser::InsertQueryContext::insertPropertyList() {
  return getRuleContext<MQL_Parser::InsertPropertyListContext>(0);
}

MQL_Parser::InsertEdgeListContext* MQL_Parser::InsertQueryContext::insertEdgeList() {
  return getRuleContext<MQL_Parser::InsertEdgeListContext>(0);
}


size_t MQL_Parser::InsertQueryContext::getRuleIndex() const {
  return MQL_Parser::RuleInsertQuery;
}


antlrcpp::Any MQL_Parser::InsertQueryContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitInsertQuery(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::InsertQueryContext* MQL_Parser::insertQuery() {
  InsertQueryContext *_localctx = _tracker.createInstance<InsertQueryContext>(_ctx, getState());
  enterRule(_localctx, 8, MQL_Parser::RuleInsertQuery);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(144);
    match(MQL_Parser::K_INSERT);
    setState(148);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case MQL_Parser::K_LABEL: {
        setState(145);
        insertLabelList();
        break;
      }

      case MQL_Parser::K_PROPERTY: {
        setState(146);
        insertPropertyList();
        break;
      }

      case MQL_Parser::K_EDGE: {
        setState(147);
        insertEdgeList();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- InsertLabelListContext ------------------------------------------------------------------

MQL_Parser::InsertLabelListContext::InsertLabelListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::InsertLabelListContext::K_LABEL() {
  return getToken(MQL_Parser::K_LABEL, 0);
}

std::vector<MQL_Parser::InsertLabelElementContext *> MQL_Parser::InsertLabelListContext::insertLabelElement() {
  return getRuleContexts<MQL_Parser::InsertLabelElementContext>();
}

MQL_Parser::InsertLabelElementContext* MQL_Parser::InsertLabelListContext::insertLabelElement(size_t i) {
  return getRuleContext<MQL_Parser::InsertLabelElementContext>(i);
}

std::vector<tree::TerminalNode *> MQL_Parser::InsertLabelListContext::COMMA() {
  return getTokens(MQL_Parser::COMMA);
}

tree::TerminalNode* MQL_Parser::InsertLabelListContext::COMMA(size_t i) {
  return getToken(MQL_Parser::COMMA, i);
}


size_t MQL_Parser::InsertLabelListContext::getRuleIndex() const {
  return MQL_Parser::RuleInsertLabelList;
}


antlrcpp::Any MQL_Parser::InsertLabelListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitInsertLabelList(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::InsertLabelListContext* MQL_Parser::insertLabelList() {
  InsertLabelListContext *_localctx = _tracker.createInstance<InsertLabelListContext>(_ctx, getState());
  enterRule(_localctx, 10, MQL_Parser::RuleInsertLabelList);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(150);
    match(MQL_Parser::K_LABEL);
    setState(151);
    insertLabelElement();
    setState(156);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MQL_Parser::COMMA) {
      setState(152);
      match(MQL_Parser::COMMA);
      setState(153);
      insertLabelElement();
      setState(158);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- InsertPropertyListContext ------------------------------------------------------------------

MQL_Parser::InsertPropertyListContext::InsertPropertyListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::InsertPropertyListContext::K_PROPERTY() {
  return getToken(MQL_Parser::K_PROPERTY, 0);
}

std::vector<MQL_Parser::InsertPropertyElementContext *> MQL_Parser::InsertPropertyListContext::insertPropertyElement() {
  return getRuleContexts<MQL_Parser::InsertPropertyElementContext>();
}

MQL_Parser::InsertPropertyElementContext* MQL_Parser::InsertPropertyListContext::insertPropertyElement(size_t i) {
  return getRuleContext<MQL_Parser::InsertPropertyElementContext>(i);
}

std::vector<tree::TerminalNode *> MQL_Parser::InsertPropertyListContext::COMMA() {
  return getTokens(MQL_Parser::COMMA);
}

tree::TerminalNode* MQL_Parser::InsertPropertyListContext::COMMA(size_t i) {
  return getToken(MQL_Parser::COMMA, i);
}


size_t MQL_Parser::InsertPropertyListContext::getRuleIndex() const {
  return MQL_Parser::RuleInsertPropertyList;
}


antlrcpp::Any MQL_Parser::InsertPropertyListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitInsertPropertyList(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::InsertPropertyListContext* MQL_Parser::insertPropertyList() {
  InsertPropertyListContext *_localctx = _tracker.createInstance<InsertPropertyListContext>(_ctx, getState());
  enterRule(_localctx, 12, MQL_Parser::RuleInsertPropertyList);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(159);
    match(MQL_Parser::K_PROPERTY);
    setState(160);
    insertPropertyElement();
    setState(165);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MQL_Parser::COMMA) {
      setState(161);
      match(MQL_Parser::COMMA);
      setState(162);
      insertPropertyElement();
      setState(167);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- InsertEdgeListContext ------------------------------------------------------------------

MQL_Parser::InsertEdgeListContext::InsertEdgeListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::InsertEdgeListContext::K_EDGE() {
  return getToken(MQL_Parser::K_EDGE, 0);
}

std::vector<MQL_Parser::InsertEdgeElementContext *> MQL_Parser::InsertEdgeListContext::insertEdgeElement() {
  return getRuleContexts<MQL_Parser::InsertEdgeElementContext>();
}

MQL_Parser::InsertEdgeElementContext* MQL_Parser::InsertEdgeListContext::insertEdgeElement(size_t i) {
  return getRuleContext<MQL_Parser::InsertEdgeElementContext>(i);
}

std::vector<tree::TerminalNode *> MQL_Parser::InsertEdgeListContext::COMMA() {
  return getTokens(MQL_Parser::COMMA);
}

tree::TerminalNode* MQL_Parser::InsertEdgeListContext::COMMA(size_t i) {
  return getToken(MQL_Parser::COMMA, i);
}


size_t MQL_Parser::InsertEdgeListContext::getRuleIndex() const {
  return MQL_Parser::RuleInsertEdgeList;
}


antlrcpp::Any MQL_Parser::InsertEdgeListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitInsertEdgeList(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::InsertEdgeListContext* MQL_Parser::insertEdgeList() {
  InsertEdgeListContext *_localctx = _tracker.createInstance<InsertEdgeListContext>(_ctx, getState());
  enterRule(_localctx, 14, MQL_Parser::RuleInsertEdgeList);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(168);
    match(MQL_Parser::K_EDGE);
    setState(169);
    insertEdgeElement();
    setState(174);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MQL_Parser::COMMA) {
      setState(170);
      match(MQL_Parser::COMMA);
      setState(171);
      insertEdgeElement();
      setState(176);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- InsertLabelElementContext ------------------------------------------------------------------

MQL_Parser::InsertLabelElementContext::InsertLabelElementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::InsertLabelElementContext::L_PAR() {
  return getToken(MQL_Parser::L_PAR, 0);
}

tree::TerminalNode* MQL_Parser::InsertLabelElementContext::COMMA() {
  return getToken(MQL_Parser::COMMA, 0);
}

tree::TerminalNode* MQL_Parser::InsertLabelElementContext::STRING() {
  return getToken(MQL_Parser::STRING, 0);
}

tree::TerminalNode* MQL_Parser::InsertLabelElementContext::R_PAR() {
  return getToken(MQL_Parser::R_PAR, 0);
}

MQL_Parser::IdentifierContext* MQL_Parser::InsertLabelElementContext::identifier() {
  return getRuleContext<MQL_Parser::IdentifierContext>(0);
}

tree::TerminalNode* MQL_Parser::InsertLabelElementContext::ANON_ID() {
  return getToken(MQL_Parser::ANON_ID, 0);
}


size_t MQL_Parser::InsertLabelElementContext::getRuleIndex() const {
  return MQL_Parser::RuleInsertLabelElement;
}


antlrcpp::Any MQL_Parser::InsertLabelElementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitInsertLabelElement(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::InsertLabelElementContext* MQL_Parser::insertLabelElement() {
  InsertLabelElementContext *_localctx = _tracker.createInstance<InsertLabelElementContext>(_ctx, getState());
  enterRule(_localctx, 16, MQL_Parser::RuleInsertLabelElement);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(177);
    match(MQL_Parser::L_PAR);
    setState(180);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case MQL_Parser::K_ANY:
      case MQL_Parser::K_AND:
      case MQL_Parser::K_AVG:
      case MQL_Parser::K_ALL:
      case MQL_Parser::K_ASC:
      case MQL_Parser::K_BY:
      case MQL_Parser::K_BOOL:
      case MQL_Parser::K_COUNT:
      case MQL_Parser::K_DESCRIBE:
      case MQL_Parser::K_DESC:
      case MQL_Parser::K_DISTINCT:
      case MQL_Parser::K_EDGE:
      case MQL_Parser::K_INTEGER:
      case MQL_Parser::K_INSERT:
      case MQL_Parser::K_IS:
      case MQL_Parser::K_FLOAT:
      case MQL_Parser::K_GROUP:
      case MQL_Parser::K_LABEL:
      case MQL_Parser::K_LIMIT:
      case MQL_Parser::K_MAX:
      case MQL_Parser::K_MATCH:
      case MQL_Parser::K_MIN:
      case MQL_Parser::K_OPTIONAL:
      case MQL_Parser::K_ORDER:
      case MQL_Parser::K_OR:
      case MQL_Parser::K_PROPERTY:
      case MQL_Parser::K_NOT:
      case MQL_Parser::K_NULL:
      case MQL_Parser::K_SET:
      case MQL_Parser::K_SUM:
      case MQL_Parser::K_STRING:
      case MQL_Parser::K_RETURN:
      case MQL_Parser::K_WHERE:
      case MQL_Parser::NAME: {
        setState(178);
        identifier();
        break;
      }

      case MQL_Parser::ANON_ID: {
        setState(179);
        match(MQL_Parser::ANON_ID);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    setState(182);
    match(MQL_Parser::COMMA);
    setState(183);
    match(MQL_Parser::STRING);
    setState(184);
    match(MQL_Parser::R_PAR);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- InsertPropertyElementContext ------------------------------------------------------------------

MQL_Parser::InsertPropertyElementContext::InsertPropertyElementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::InsertPropertyElementContext::L_PAR() {
  return getToken(MQL_Parser::L_PAR, 0);
}

MQL_Parser::FixedNodeInsideContext* MQL_Parser::InsertPropertyElementContext::fixedNodeInside() {
  return getRuleContext<MQL_Parser::FixedNodeInsideContext>(0);
}

std::vector<tree::TerminalNode *> MQL_Parser::InsertPropertyElementContext::COMMA() {
  return getTokens(MQL_Parser::COMMA);
}

tree::TerminalNode* MQL_Parser::InsertPropertyElementContext::COMMA(size_t i) {
  return getToken(MQL_Parser::COMMA, i);
}

tree::TerminalNode* MQL_Parser::InsertPropertyElementContext::STRING() {
  return getToken(MQL_Parser::STRING, 0);
}

MQL_Parser::ValueContext* MQL_Parser::InsertPropertyElementContext::value() {
  return getRuleContext<MQL_Parser::ValueContext>(0);
}

tree::TerminalNode* MQL_Parser::InsertPropertyElementContext::R_PAR() {
  return getToken(MQL_Parser::R_PAR, 0);
}


size_t MQL_Parser::InsertPropertyElementContext::getRuleIndex() const {
  return MQL_Parser::RuleInsertPropertyElement;
}


antlrcpp::Any MQL_Parser::InsertPropertyElementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitInsertPropertyElement(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::InsertPropertyElementContext* MQL_Parser::insertPropertyElement() {
  InsertPropertyElementContext *_localctx = _tracker.createInstance<InsertPropertyElementContext>(_ctx, getState());
  enterRule(_localctx, 18, MQL_Parser::RuleInsertPropertyElement);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(186);
    match(MQL_Parser::L_PAR);
    setState(187);
    fixedNodeInside();
    setState(188);
    match(MQL_Parser::COMMA);
    setState(189);
    match(MQL_Parser::STRING);
    setState(190);
    match(MQL_Parser::COMMA);
    setState(191);
    value();
    setState(192);
    match(MQL_Parser::R_PAR);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- InsertEdgeElementContext ------------------------------------------------------------------

MQL_Parser::InsertEdgeElementContext::InsertEdgeElementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::InsertEdgeElementContext::L_PAR() {
  return getToken(MQL_Parser::L_PAR, 0);
}

std::vector<MQL_Parser::FixedNodeInsideContext *> MQL_Parser::InsertEdgeElementContext::fixedNodeInside() {
  return getRuleContexts<MQL_Parser::FixedNodeInsideContext>();
}

MQL_Parser::FixedNodeInsideContext* MQL_Parser::InsertEdgeElementContext::fixedNodeInside(size_t i) {
  return getRuleContext<MQL_Parser::FixedNodeInsideContext>(i);
}

std::vector<tree::TerminalNode *> MQL_Parser::InsertEdgeElementContext::COMMA() {
  return getTokens(MQL_Parser::COMMA);
}

tree::TerminalNode* MQL_Parser::InsertEdgeElementContext::COMMA(size_t i) {
  return getToken(MQL_Parser::COMMA, i);
}

MQL_Parser::IdentifierContext* MQL_Parser::InsertEdgeElementContext::identifier() {
  return getRuleContext<MQL_Parser::IdentifierContext>(0);
}

tree::TerminalNode* MQL_Parser::InsertEdgeElementContext::R_PAR() {
  return getToken(MQL_Parser::R_PAR, 0);
}


size_t MQL_Parser::InsertEdgeElementContext::getRuleIndex() const {
  return MQL_Parser::RuleInsertEdgeElement;
}


antlrcpp::Any MQL_Parser::InsertEdgeElementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitInsertEdgeElement(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::InsertEdgeElementContext* MQL_Parser::insertEdgeElement() {
  InsertEdgeElementContext *_localctx = _tracker.createInstance<InsertEdgeElementContext>(_ctx, getState());
  enterRule(_localctx, 20, MQL_Parser::RuleInsertEdgeElement);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(194);
    match(MQL_Parser::L_PAR);
    setState(195);
    fixedNodeInside();
    setState(196);
    match(MQL_Parser::COMMA);
    setState(197);
    fixedNodeInside();
    setState(198);
    match(MQL_Parser::COMMA);
    setState(199);
    identifier();
    setState(200);
    match(MQL_Parser::R_PAR);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SetStatementContext ------------------------------------------------------------------

MQL_Parser::SetStatementContext::SetStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::SetStatementContext::K_SET() {
  return getToken(MQL_Parser::K_SET, 0);
}

std::vector<MQL_Parser::SetItemContext *> MQL_Parser::SetStatementContext::setItem() {
  return getRuleContexts<MQL_Parser::SetItemContext>();
}

MQL_Parser::SetItemContext* MQL_Parser::SetStatementContext::setItem(size_t i) {
  return getRuleContext<MQL_Parser::SetItemContext>(i);
}

std::vector<tree::TerminalNode *> MQL_Parser::SetStatementContext::COMMA() {
  return getTokens(MQL_Parser::COMMA);
}

tree::TerminalNode* MQL_Parser::SetStatementContext::COMMA(size_t i) {
  return getToken(MQL_Parser::COMMA, i);
}


size_t MQL_Parser::SetStatementContext::getRuleIndex() const {
  return MQL_Parser::RuleSetStatement;
}


antlrcpp::Any MQL_Parser::SetStatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitSetStatement(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::SetStatementContext* MQL_Parser::setStatement() {
  SetStatementContext *_localctx = _tracker.createInstance<SetStatementContext>(_ctx, getState());
  enterRule(_localctx, 22, MQL_Parser::RuleSetStatement);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(202);
    match(MQL_Parser::K_SET);
    setState(203);
    setItem();
    setState(208);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MQL_Parser::COMMA) {
      setState(204);
      match(MQL_Parser::COMMA);
      setState(205);
      setItem();
      setState(210);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- MatchStatementContext ------------------------------------------------------------------

MQL_Parser::MatchStatementContext::MatchStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::MatchStatementContext::K_MATCH() {
  return getToken(MQL_Parser::K_MATCH, 0);
}

MQL_Parser::GraphPatternContext* MQL_Parser::MatchStatementContext::graphPattern() {
  return getRuleContext<MQL_Parser::GraphPatternContext>(0);
}


size_t MQL_Parser::MatchStatementContext::getRuleIndex() const {
  return MQL_Parser::RuleMatchStatement;
}


antlrcpp::Any MQL_Parser::MatchStatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitMatchStatement(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::MatchStatementContext* MQL_Parser::matchStatement() {
  MatchStatementContext *_localctx = _tracker.createInstance<MatchStatementContext>(_ctx, getState());
  enterRule(_localctx, 24, MQL_Parser::RuleMatchStatement);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(211);
    match(MQL_Parser::K_MATCH);
    setState(212);
    graphPattern();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- WhereStatementContext ------------------------------------------------------------------

MQL_Parser::WhereStatementContext::WhereStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::WhereStatementContext::K_WHERE() {
  return getToken(MQL_Parser::K_WHERE, 0);
}

MQL_Parser::ConditionalOrExprContext* MQL_Parser::WhereStatementContext::conditionalOrExpr() {
  return getRuleContext<MQL_Parser::ConditionalOrExprContext>(0);
}


size_t MQL_Parser::WhereStatementContext::getRuleIndex() const {
  return MQL_Parser::RuleWhereStatement;
}


antlrcpp::Any MQL_Parser::WhereStatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitWhereStatement(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::WhereStatementContext* MQL_Parser::whereStatement() {
  WhereStatementContext *_localctx = _tracker.createInstance<WhereStatementContext>(_ctx, getState());
  enterRule(_localctx, 26, MQL_Parser::RuleWhereStatement);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(214);
    match(MQL_Parser::K_WHERE);
    setState(215);
    conditionalOrExpr();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- GroupByStatementContext ------------------------------------------------------------------

MQL_Parser::GroupByStatementContext::GroupByStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::GroupByStatementContext::K_GROUP() {
  return getToken(MQL_Parser::K_GROUP, 0);
}

tree::TerminalNode* MQL_Parser::GroupByStatementContext::K_BY() {
  return getToken(MQL_Parser::K_BY, 0);
}

std::vector<MQL_Parser::GroupByItemContext *> MQL_Parser::GroupByStatementContext::groupByItem() {
  return getRuleContexts<MQL_Parser::GroupByItemContext>();
}

MQL_Parser::GroupByItemContext* MQL_Parser::GroupByStatementContext::groupByItem(size_t i) {
  return getRuleContext<MQL_Parser::GroupByItemContext>(i);
}

std::vector<tree::TerminalNode *> MQL_Parser::GroupByStatementContext::COMMA() {
  return getTokens(MQL_Parser::COMMA);
}

tree::TerminalNode* MQL_Parser::GroupByStatementContext::COMMA(size_t i) {
  return getToken(MQL_Parser::COMMA, i);
}


size_t MQL_Parser::GroupByStatementContext::getRuleIndex() const {
  return MQL_Parser::RuleGroupByStatement;
}


antlrcpp::Any MQL_Parser::GroupByStatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitGroupByStatement(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::GroupByStatementContext* MQL_Parser::groupByStatement() {
  GroupByStatementContext *_localctx = _tracker.createInstance<GroupByStatementContext>(_ctx, getState());
  enterRule(_localctx, 28, MQL_Parser::RuleGroupByStatement);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(217);
    match(MQL_Parser::K_GROUP);
    setState(218);
    match(MQL_Parser::K_BY);
    setState(219);
    groupByItem();
    setState(224);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MQL_Parser::COMMA) {
      setState(220);
      match(MQL_Parser::COMMA);
      setState(221);
      groupByItem();
      setState(226);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- OrderByStatementContext ------------------------------------------------------------------

MQL_Parser::OrderByStatementContext::OrderByStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::OrderByStatementContext::K_ORDER() {
  return getToken(MQL_Parser::K_ORDER, 0);
}

tree::TerminalNode* MQL_Parser::OrderByStatementContext::K_BY() {
  return getToken(MQL_Parser::K_BY, 0);
}

std::vector<MQL_Parser::OrderByItemContext *> MQL_Parser::OrderByStatementContext::orderByItem() {
  return getRuleContexts<MQL_Parser::OrderByItemContext>();
}

MQL_Parser::OrderByItemContext* MQL_Parser::OrderByStatementContext::orderByItem(size_t i) {
  return getRuleContext<MQL_Parser::OrderByItemContext>(i);
}

std::vector<tree::TerminalNode *> MQL_Parser::OrderByStatementContext::COMMA() {
  return getTokens(MQL_Parser::COMMA);
}

tree::TerminalNode* MQL_Parser::OrderByStatementContext::COMMA(size_t i) {
  return getToken(MQL_Parser::COMMA, i);
}


size_t MQL_Parser::OrderByStatementContext::getRuleIndex() const {
  return MQL_Parser::RuleOrderByStatement;
}


antlrcpp::Any MQL_Parser::OrderByStatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitOrderByStatement(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::OrderByStatementContext* MQL_Parser::orderByStatement() {
  OrderByStatementContext *_localctx = _tracker.createInstance<OrderByStatementContext>(_ctx, getState());
  enterRule(_localctx, 30, MQL_Parser::RuleOrderByStatement);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(227);
    match(MQL_Parser::K_ORDER);
    setState(228);
    match(MQL_Parser::K_BY);
    setState(229);
    orderByItem();
    setState(234);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MQL_Parser::COMMA) {
      setState(230);
      match(MQL_Parser::COMMA);
      setState(231);
      orderByItem();
      setState(236);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ReturnStatementContext ------------------------------------------------------------------

MQL_Parser::ReturnStatementContext::ReturnStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t MQL_Parser::ReturnStatementContext::getRuleIndex() const {
  return MQL_Parser::RuleReturnStatement;
}

void MQL_Parser::ReturnStatementContext::copyFrom(ReturnStatementContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- ReturnAllContext ------------------------------------------------------------------

tree::TerminalNode* MQL_Parser::ReturnAllContext::K_RETURN() {
  return getToken(MQL_Parser::K_RETURN, 0);
}

tree::TerminalNode* MQL_Parser::ReturnAllContext::STAR() {
  return getToken(MQL_Parser::STAR, 0);
}

tree::TerminalNode* MQL_Parser::ReturnAllContext::K_DISTINCT() {
  return getToken(MQL_Parser::K_DISTINCT, 0);
}

tree::TerminalNode* MQL_Parser::ReturnAllContext::K_LIMIT() {
  return getToken(MQL_Parser::K_LIMIT, 0);
}

tree::TerminalNode* MQL_Parser::ReturnAllContext::UNSIGNED_INTEGER() {
  return getToken(MQL_Parser::UNSIGNED_INTEGER, 0);
}

MQL_Parser::ReturnAllContext::ReturnAllContext(ReturnStatementContext *ctx) { copyFrom(ctx); }


antlrcpp::Any MQL_Parser::ReturnAllContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitReturnAll(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ReturnListContext ------------------------------------------------------------------

tree::TerminalNode* MQL_Parser::ReturnListContext::K_RETURN() {
  return getToken(MQL_Parser::K_RETURN, 0);
}

std::vector<MQL_Parser::ReturnItemContext *> MQL_Parser::ReturnListContext::returnItem() {
  return getRuleContexts<MQL_Parser::ReturnItemContext>();
}

MQL_Parser::ReturnItemContext* MQL_Parser::ReturnListContext::returnItem(size_t i) {
  return getRuleContext<MQL_Parser::ReturnItemContext>(i);
}

tree::TerminalNode* MQL_Parser::ReturnListContext::K_DISTINCT() {
  return getToken(MQL_Parser::K_DISTINCT, 0);
}

std::vector<tree::TerminalNode *> MQL_Parser::ReturnListContext::COMMA() {
  return getTokens(MQL_Parser::COMMA);
}

tree::TerminalNode* MQL_Parser::ReturnListContext::COMMA(size_t i) {
  return getToken(MQL_Parser::COMMA, i);
}

tree::TerminalNode* MQL_Parser::ReturnListContext::K_LIMIT() {
  return getToken(MQL_Parser::K_LIMIT, 0);
}

tree::TerminalNode* MQL_Parser::ReturnListContext::UNSIGNED_INTEGER() {
  return getToken(MQL_Parser::UNSIGNED_INTEGER, 0);
}

MQL_Parser::ReturnListContext::ReturnListContext(ReturnStatementContext *ctx) { copyFrom(ctx); }


antlrcpp::Any MQL_Parser::ReturnListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitReturnList(this);
  else
    return visitor->visitChildren(this);
}
MQL_Parser::ReturnStatementContext* MQL_Parser::returnStatement() {
  ReturnStatementContext *_localctx = _tracker.createInstance<ReturnStatementContext>(_ctx, getState());
  enterRule(_localctx, 32, MQL_Parser::RuleReturnStatement);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(262);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 20, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<MQL_Parser::ReturnListContext>(_localctx);
      enterOuterAlt(_localctx, 1);
      setState(237);
      match(MQL_Parser::K_RETURN);
      setState(239);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == MQL_Parser::K_DISTINCT) {
        setState(238);
        match(MQL_Parser::K_DISTINCT);
      }
      setState(241);
      returnItem();
      setState(246);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == MQL_Parser::COMMA) {
        setState(242);
        match(MQL_Parser::COMMA);
        setState(243);
        returnItem();
        setState(248);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(251);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == MQL_Parser::K_LIMIT) {
        setState(249);
        match(MQL_Parser::K_LIMIT);
        setState(250);
        match(MQL_Parser::UNSIGNED_INTEGER);
      }
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<MQL_Parser::ReturnAllContext>(_localctx);
      enterOuterAlt(_localctx, 2);
      setState(253);
      match(MQL_Parser::K_RETURN);
      setState(255);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == MQL_Parser::K_DISTINCT) {
        setState(254);
        match(MQL_Parser::K_DISTINCT);
      }
      setState(257);
      match(MQL_Parser::STAR);
      setState(260);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == MQL_Parser::K_LIMIT) {
        setState(258);
        match(MQL_Parser::K_LIMIT);
        setState(259);
        match(MQL_Parser::UNSIGNED_INTEGER);
      }
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SetItemContext ------------------------------------------------------------------

MQL_Parser::SetItemContext::SetItemContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::SetItemContext::VARIABLE() {
  return getToken(MQL_Parser::VARIABLE, 0);
}

tree::TerminalNode* MQL_Parser::SetItemContext::SINGLE_EQ() {
  return getToken(MQL_Parser::SINGLE_EQ, 0);
}

MQL_Parser::FixedNodeInsideContext* MQL_Parser::SetItemContext::fixedNodeInside() {
  return getRuleContext<MQL_Parser::FixedNodeInsideContext>(0);
}


size_t MQL_Parser::SetItemContext::getRuleIndex() const {
  return MQL_Parser::RuleSetItem;
}


antlrcpp::Any MQL_Parser::SetItemContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitSetItem(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::SetItemContext* MQL_Parser::setItem() {
  SetItemContext *_localctx = _tracker.createInstance<SetItemContext>(_ctx, getState());
  enterRule(_localctx, 34, MQL_Parser::RuleSetItem);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(264);
    match(MQL_Parser::VARIABLE);
    setState(265);
    match(MQL_Parser::SINGLE_EQ);
    setState(266);
    fixedNodeInside();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ReturnItemContext ------------------------------------------------------------------

MQL_Parser::ReturnItemContext::ReturnItemContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t MQL_Parser::ReturnItemContext::getRuleIndex() const {
  return MQL_Parser::RuleReturnItem;
}

void MQL_Parser::ReturnItemContext::copyFrom(ReturnItemContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- ReturnItemVarContext ------------------------------------------------------------------

tree::TerminalNode* MQL_Parser::ReturnItemVarContext::VARIABLE() {
  return getToken(MQL_Parser::VARIABLE, 0);
}

tree::TerminalNode* MQL_Parser::ReturnItemVarContext::KEY() {
  return getToken(MQL_Parser::KEY, 0);
}

MQL_Parser::ReturnItemVarContext::ReturnItemVarContext(ReturnItemContext *ctx) { copyFrom(ctx); }


antlrcpp::Any MQL_Parser::ReturnItemVarContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitReturnItemVar(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ReturnItemAggContext ------------------------------------------------------------------

MQL_Parser::AggregateFuncContext* MQL_Parser::ReturnItemAggContext::aggregateFunc() {
  return getRuleContext<MQL_Parser::AggregateFuncContext>(0);
}

tree::TerminalNode* MQL_Parser::ReturnItemAggContext::L_PAR() {
  return getToken(MQL_Parser::L_PAR, 0);
}

tree::TerminalNode* MQL_Parser::ReturnItemAggContext::VARIABLE() {
  return getToken(MQL_Parser::VARIABLE, 0);
}

tree::TerminalNode* MQL_Parser::ReturnItemAggContext::R_PAR() {
  return getToken(MQL_Parser::R_PAR, 0);
}

tree::TerminalNode* MQL_Parser::ReturnItemAggContext::KEY() {
  return getToken(MQL_Parser::KEY, 0);
}

MQL_Parser::ReturnItemAggContext::ReturnItemAggContext(ReturnItemContext *ctx) { copyFrom(ctx); }


antlrcpp::Any MQL_Parser::ReturnItemAggContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitReturnItemAgg(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ReturnItemCountContext ------------------------------------------------------------------

tree::TerminalNode* MQL_Parser::ReturnItemCountContext::K_COUNT() {
  return getToken(MQL_Parser::K_COUNT, 0);
}

tree::TerminalNode* MQL_Parser::ReturnItemCountContext::L_PAR() {
  return getToken(MQL_Parser::L_PAR, 0);
}

tree::TerminalNode* MQL_Parser::ReturnItemCountContext::R_PAR() {
  return getToken(MQL_Parser::R_PAR, 0);
}

tree::TerminalNode* MQL_Parser::ReturnItemCountContext::VARIABLE() {
  return getToken(MQL_Parser::VARIABLE, 0);
}

tree::TerminalNode* MQL_Parser::ReturnItemCountContext::STAR() {
  return getToken(MQL_Parser::STAR, 0);
}

tree::TerminalNode* MQL_Parser::ReturnItemCountContext::K_DISTINCT() {
  return getToken(MQL_Parser::K_DISTINCT, 0);
}

tree::TerminalNode* MQL_Parser::ReturnItemCountContext::KEY() {
  return getToken(MQL_Parser::KEY, 0);
}

MQL_Parser::ReturnItemCountContext::ReturnItemCountContext(ReturnItemContext *ctx) { copyFrom(ctx); }


antlrcpp::Any MQL_Parser::ReturnItemCountContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitReturnItemCount(this);
  else
    return visitor->visitChildren(this);
}
MQL_Parser::ReturnItemContext* MQL_Parser::returnItem() {
  ReturnItemContext *_localctx = _tracker.createInstance<ReturnItemContext>(_ctx, getState());
  enterRule(_localctx, 36, MQL_Parser::RuleReturnItem);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(293);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case MQL_Parser::VARIABLE: {
        _localctx = _tracker.createInstance<MQL_Parser::ReturnItemVarContext>(_localctx);
        enterOuterAlt(_localctx, 1);
        setState(268);
        match(MQL_Parser::VARIABLE);
        setState(270);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == MQL_Parser::KEY) {
          setState(269);
          match(MQL_Parser::KEY);
        }
        break;
      }

      case MQL_Parser::K_AVG:
      case MQL_Parser::K_MAX:
      case MQL_Parser::K_MIN:
      case MQL_Parser::K_SUM: {
        _localctx = _tracker.createInstance<MQL_Parser::ReturnItemAggContext>(_localctx);
        enterOuterAlt(_localctx, 2);
        setState(272);
        aggregateFunc();
        setState(273);
        match(MQL_Parser::L_PAR);
        setState(274);
        match(MQL_Parser::VARIABLE);
        setState(276);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == MQL_Parser::KEY) {
          setState(275);
          match(MQL_Parser::KEY);
        }
        setState(278);
        match(MQL_Parser::R_PAR);
        break;
      }

      case MQL_Parser::K_COUNT: {
        _localctx = _tracker.createInstance<MQL_Parser::ReturnItemCountContext>(_localctx);
        enterOuterAlt(_localctx, 3);
        setState(280);
        match(MQL_Parser::K_COUNT);
        setState(281);
        match(MQL_Parser::L_PAR);
        setState(283);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == MQL_Parser::K_DISTINCT) {
          setState(282);
          match(MQL_Parser::K_DISTINCT);
        }
        setState(290);
        _errHandler->sync(this);
        switch (_input->LA(1)) {
          case MQL_Parser::VARIABLE: {
            setState(285);
            match(MQL_Parser::VARIABLE);
            setState(287);
            _errHandler->sync(this);

            _la = _input->LA(1);
            if (_la == MQL_Parser::KEY) {
              setState(286);
              match(MQL_Parser::KEY);
            }
            break;
          }

          case MQL_Parser::STAR: {
            setState(289);
            match(MQL_Parser::STAR);
            break;
          }

        default:
          throw NoViableAltException(this);
        }
        setState(292);
        match(MQL_Parser::R_PAR);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AggregateFuncContext ------------------------------------------------------------------

MQL_Parser::AggregateFuncContext::AggregateFuncContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::AggregateFuncContext::K_SUM() {
  return getToken(MQL_Parser::K_SUM, 0);
}

tree::TerminalNode* MQL_Parser::AggregateFuncContext::K_MAX() {
  return getToken(MQL_Parser::K_MAX, 0);
}

tree::TerminalNode* MQL_Parser::AggregateFuncContext::K_MIN() {
  return getToken(MQL_Parser::K_MIN, 0);
}

tree::TerminalNode* MQL_Parser::AggregateFuncContext::K_AVG() {
  return getToken(MQL_Parser::K_AVG, 0);
}


size_t MQL_Parser::AggregateFuncContext::getRuleIndex() const {
  return MQL_Parser::RuleAggregateFunc;
}


antlrcpp::Any MQL_Parser::AggregateFuncContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitAggregateFunc(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::AggregateFuncContext* MQL_Parser::aggregateFunc() {
  AggregateFuncContext *_localctx = _tracker.createInstance<AggregateFuncContext>(_ctx, getState());
  enterRule(_localctx, 38, MQL_Parser::RuleAggregateFunc);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(295);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << MQL_Parser::K_AVG)
      | (1ULL << MQL_Parser::K_MAX)
      | (1ULL << MQL_Parser::K_MIN)
      | (1ULL << MQL_Parser::K_SUM))) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- OrderByItemContext ------------------------------------------------------------------

MQL_Parser::OrderByItemContext::OrderByItemContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t MQL_Parser::OrderByItemContext::getRuleIndex() const {
  return MQL_Parser::RuleOrderByItem;
}

void MQL_Parser::OrderByItemContext::copyFrom(OrderByItemContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- OrderByItemCountContext ------------------------------------------------------------------

tree::TerminalNode* MQL_Parser::OrderByItemCountContext::K_COUNT() {
  return getToken(MQL_Parser::K_COUNT, 0);
}

tree::TerminalNode* MQL_Parser::OrderByItemCountContext::L_PAR() {
  return getToken(MQL_Parser::L_PAR, 0);
}

tree::TerminalNode* MQL_Parser::OrderByItemCountContext::VARIABLE() {
  return getToken(MQL_Parser::VARIABLE, 0);
}

tree::TerminalNode* MQL_Parser::OrderByItemCountContext::R_PAR() {
  return getToken(MQL_Parser::R_PAR, 0);
}

tree::TerminalNode* MQL_Parser::OrderByItemCountContext::K_DISTINCT() {
  return getToken(MQL_Parser::K_DISTINCT, 0);
}

tree::TerminalNode* MQL_Parser::OrderByItemCountContext::KEY() {
  return getToken(MQL_Parser::KEY, 0);
}

tree::TerminalNode* MQL_Parser::OrderByItemCountContext::K_ASC() {
  return getToken(MQL_Parser::K_ASC, 0);
}

tree::TerminalNode* MQL_Parser::OrderByItemCountContext::K_DESC() {
  return getToken(MQL_Parser::K_DESC, 0);
}

MQL_Parser::OrderByItemCountContext::OrderByItemCountContext(OrderByItemContext *ctx) { copyFrom(ctx); }


antlrcpp::Any MQL_Parser::OrderByItemCountContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitOrderByItemCount(this);
  else
    return visitor->visitChildren(this);
}
//----------------- OrderByItemVarContext ------------------------------------------------------------------

tree::TerminalNode* MQL_Parser::OrderByItemVarContext::VARIABLE() {
  return getToken(MQL_Parser::VARIABLE, 0);
}

tree::TerminalNode* MQL_Parser::OrderByItemVarContext::KEY() {
  return getToken(MQL_Parser::KEY, 0);
}

tree::TerminalNode* MQL_Parser::OrderByItemVarContext::K_ASC() {
  return getToken(MQL_Parser::K_ASC, 0);
}

tree::TerminalNode* MQL_Parser::OrderByItemVarContext::K_DESC() {
  return getToken(MQL_Parser::K_DESC, 0);
}

MQL_Parser::OrderByItemVarContext::OrderByItemVarContext(OrderByItemContext *ctx) { copyFrom(ctx); }


antlrcpp::Any MQL_Parser::OrderByItemVarContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitOrderByItemVar(this);
  else
    return visitor->visitChildren(this);
}
//----------------- OrderByItemAggContext ------------------------------------------------------------------

MQL_Parser::AggregateFuncContext* MQL_Parser::OrderByItemAggContext::aggregateFunc() {
  return getRuleContext<MQL_Parser::AggregateFuncContext>(0);
}

tree::TerminalNode* MQL_Parser::OrderByItemAggContext::L_PAR() {
  return getToken(MQL_Parser::L_PAR, 0);
}

tree::TerminalNode* MQL_Parser::OrderByItemAggContext::VARIABLE() {
  return getToken(MQL_Parser::VARIABLE, 0);
}

tree::TerminalNode* MQL_Parser::OrderByItemAggContext::R_PAR() {
  return getToken(MQL_Parser::R_PAR, 0);
}

tree::TerminalNode* MQL_Parser::OrderByItemAggContext::KEY() {
  return getToken(MQL_Parser::KEY, 0);
}

tree::TerminalNode* MQL_Parser::OrderByItemAggContext::K_ASC() {
  return getToken(MQL_Parser::K_ASC, 0);
}

tree::TerminalNode* MQL_Parser::OrderByItemAggContext::K_DESC() {
  return getToken(MQL_Parser::K_DESC, 0);
}

MQL_Parser::OrderByItemAggContext::OrderByItemAggContext(OrderByItemContext *ctx) { copyFrom(ctx); }


antlrcpp::Any MQL_Parser::OrderByItemAggContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitOrderByItemAgg(this);
  else
    return visitor->visitChildren(this);
}
MQL_Parser::OrderByItemContext* MQL_Parser::orderByItem() {
  OrderByItemContext *_localctx = _tracker.createInstance<OrderByItemContext>(_ctx, getState());
  enterRule(_localctx, 40, MQL_Parser::RuleOrderByItem);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(327);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case MQL_Parser::VARIABLE: {
        _localctx = _tracker.createInstance<MQL_Parser::OrderByItemVarContext>(_localctx);
        enterOuterAlt(_localctx, 1);
        setState(297);
        match(MQL_Parser::VARIABLE);
        setState(299);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == MQL_Parser::KEY) {
          setState(298);
          match(MQL_Parser::KEY);
        }
        setState(302);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == MQL_Parser::K_ASC

        || _la == MQL_Parser::K_DESC) {
          setState(301);
          _la = _input->LA(1);
          if (!(_la == MQL_Parser::K_ASC

          || _la == MQL_Parser::K_DESC)) {
          _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
        }
        break;
      }

      case MQL_Parser::K_AVG:
      case MQL_Parser::K_MAX:
      case MQL_Parser::K_MIN:
      case MQL_Parser::K_SUM: {
        _localctx = _tracker.createInstance<MQL_Parser::OrderByItemAggContext>(_localctx);
        enterOuterAlt(_localctx, 2);
        setState(304);
        aggregateFunc();
        setState(305);
        match(MQL_Parser::L_PAR);
        setState(306);
        match(MQL_Parser::VARIABLE);
        setState(308);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == MQL_Parser::KEY) {
          setState(307);
          match(MQL_Parser::KEY);
        }
        setState(310);
        match(MQL_Parser::R_PAR);
        setState(312);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == MQL_Parser::K_ASC

        || _la == MQL_Parser::K_DESC) {
          setState(311);
          _la = _input->LA(1);
          if (!(_la == MQL_Parser::K_ASC

          || _la == MQL_Parser::K_DESC)) {
          _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
        }
        break;
      }

      case MQL_Parser::K_COUNT: {
        _localctx = _tracker.createInstance<MQL_Parser::OrderByItemCountContext>(_localctx);
        enterOuterAlt(_localctx, 3);
        setState(314);
        match(MQL_Parser::K_COUNT);
        setState(315);
        match(MQL_Parser::L_PAR);
        setState(317);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == MQL_Parser::K_DISTINCT) {
          setState(316);
          match(MQL_Parser::K_DISTINCT);
        }
        setState(319);
        match(MQL_Parser::VARIABLE);
        setState(321);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == MQL_Parser::KEY) {
          setState(320);
          match(MQL_Parser::KEY);
        }
        setState(323);
        match(MQL_Parser::R_PAR);
        setState(325);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == MQL_Parser::K_ASC

        || _la == MQL_Parser::K_DESC) {
          setState(324);
          _la = _input->LA(1);
          if (!(_la == MQL_Parser::K_ASC

          || _la == MQL_Parser::K_DESC)) {
          _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
        }
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- GroupByItemContext ------------------------------------------------------------------

MQL_Parser::GroupByItemContext::GroupByItemContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::GroupByItemContext::VARIABLE() {
  return getToken(MQL_Parser::VARIABLE, 0);
}

tree::TerminalNode* MQL_Parser::GroupByItemContext::KEY() {
  return getToken(MQL_Parser::KEY, 0);
}


size_t MQL_Parser::GroupByItemContext::getRuleIndex() const {
  return MQL_Parser::RuleGroupByItem;
}


antlrcpp::Any MQL_Parser::GroupByItemContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitGroupByItem(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::GroupByItemContext* MQL_Parser::groupByItem() {
  GroupByItemContext *_localctx = _tracker.createInstance<GroupByItemContext>(_ctx, getState());
  enterRule(_localctx, 42, MQL_Parser::RuleGroupByItem);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(329);
    match(MQL_Parser::VARIABLE);
    setState(331);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MQL_Parser::KEY) {
      setState(330);
      match(MQL_Parser::KEY);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- GraphPatternContext ------------------------------------------------------------------

MQL_Parser::GraphPatternContext::GraphPatternContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

MQL_Parser::BasicPatternContext* MQL_Parser::GraphPatternContext::basicPattern() {
  return getRuleContext<MQL_Parser::BasicPatternContext>(0);
}

std::vector<MQL_Parser::OptionalPatternContext *> MQL_Parser::GraphPatternContext::optionalPattern() {
  return getRuleContexts<MQL_Parser::OptionalPatternContext>();
}

MQL_Parser::OptionalPatternContext* MQL_Parser::GraphPatternContext::optionalPattern(size_t i) {
  return getRuleContext<MQL_Parser::OptionalPatternContext>(i);
}


size_t MQL_Parser::GraphPatternContext::getRuleIndex() const {
  return MQL_Parser::RuleGraphPattern;
}


antlrcpp::Any MQL_Parser::GraphPatternContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitGraphPattern(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::GraphPatternContext* MQL_Parser::graphPattern() {
  GraphPatternContext *_localctx = _tracker.createInstance<GraphPatternContext>(_ctx, getState());
  enterRule(_localctx, 44, MQL_Parser::RuleGraphPattern);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(333);
    basicPattern();
    setState(337);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MQL_Parser::K_OPTIONAL) {
      setState(334);
      optionalPattern();
      setState(339);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- OptionalPatternContext ------------------------------------------------------------------

MQL_Parser::OptionalPatternContext::OptionalPatternContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::OptionalPatternContext::K_OPTIONAL() {
  return getToken(MQL_Parser::K_OPTIONAL, 0);
}

tree::TerminalNode* MQL_Parser::OptionalPatternContext::LCURLY_BRACKET() {
  return getToken(MQL_Parser::LCURLY_BRACKET, 0);
}

MQL_Parser::GraphPatternContext* MQL_Parser::OptionalPatternContext::graphPattern() {
  return getRuleContext<MQL_Parser::GraphPatternContext>(0);
}

tree::TerminalNode* MQL_Parser::OptionalPatternContext::RCURLY_BRACKET() {
  return getToken(MQL_Parser::RCURLY_BRACKET, 0);
}


size_t MQL_Parser::OptionalPatternContext::getRuleIndex() const {
  return MQL_Parser::RuleOptionalPattern;
}


antlrcpp::Any MQL_Parser::OptionalPatternContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitOptionalPattern(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::OptionalPatternContext* MQL_Parser::optionalPattern() {
  OptionalPatternContext *_localctx = _tracker.createInstance<OptionalPatternContext>(_ctx, getState());
  enterRule(_localctx, 46, MQL_Parser::RuleOptionalPattern);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(340);
    match(MQL_Parser::K_OPTIONAL);
    setState(341);
    match(MQL_Parser::LCURLY_BRACKET);
    setState(342);
    graphPattern();
    setState(343);
    match(MQL_Parser::RCURLY_BRACKET);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BasicPatternContext ------------------------------------------------------------------

MQL_Parser::BasicPatternContext::BasicPatternContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<MQL_Parser::LinearPatternContext *> MQL_Parser::BasicPatternContext::linearPattern() {
  return getRuleContexts<MQL_Parser::LinearPatternContext>();
}

MQL_Parser::LinearPatternContext* MQL_Parser::BasicPatternContext::linearPattern(size_t i) {
  return getRuleContext<MQL_Parser::LinearPatternContext>(i);
}

std::vector<tree::TerminalNode *> MQL_Parser::BasicPatternContext::COMMA() {
  return getTokens(MQL_Parser::COMMA);
}

tree::TerminalNode* MQL_Parser::BasicPatternContext::COMMA(size_t i) {
  return getToken(MQL_Parser::COMMA, i);
}


size_t MQL_Parser::BasicPatternContext::getRuleIndex() const {
  return MQL_Parser::RuleBasicPattern;
}


antlrcpp::Any MQL_Parser::BasicPatternContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitBasicPattern(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::BasicPatternContext* MQL_Parser::basicPattern() {
  BasicPatternContext *_localctx = _tracker.createInstance<BasicPatternContext>(_ctx, getState());
  enterRule(_localctx, 48, MQL_Parser::RuleBasicPattern);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(345);
    linearPattern();
    setState(350);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MQL_Parser::COMMA) {
      setState(346);
      match(MQL_Parser::COMMA);
      setState(347);
      linearPattern();
      setState(352);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LinearPatternContext ------------------------------------------------------------------

MQL_Parser::LinearPatternContext::LinearPatternContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<MQL_Parser::NodeContext *> MQL_Parser::LinearPatternContext::node() {
  return getRuleContexts<MQL_Parser::NodeContext>();
}

MQL_Parser::NodeContext* MQL_Parser::LinearPatternContext::node(size_t i) {
  return getRuleContext<MQL_Parser::NodeContext>(i);
}

std::vector<MQL_Parser::EdgeContext *> MQL_Parser::LinearPatternContext::edge() {
  return getRuleContexts<MQL_Parser::EdgeContext>();
}

MQL_Parser::EdgeContext* MQL_Parser::LinearPatternContext::edge(size_t i) {
  return getRuleContext<MQL_Parser::EdgeContext>(i);
}

std::vector<MQL_Parser::PathContext *> MQL_Parser::LinearPatternContext::path() {
  return getRuleContexts<MQL_Parser::PathContext>();
}

MQL_Parser::PathContext* MQL_Parser::LinearPatternContext::path(size_t i) {
  return getRuleContext<MQL_Parser::PathContext>(i);
}


size_t MQL_Parser::LinearPatternContext::getRuleIndex() const {
  return MQL_Parser::RuleLinearPattern;
}


antlrcpp::Any MQL_Parser::LinearPatternContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitLinearPattern(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::LinearPatternContext* MQL_Parser::linearPattern() {
  LinearPatternContext *_localctx = _tracker.createInstance<LinearPatternContext>(_ctx, getState());
  enterRule(_localctx, 50, MQL_Parser::RuleLinearPattern);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(353);
    node();
    setState(362);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (((((_la - 52) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 52)) & ((1ULL << (MQL_Parser::LEQ - 52))
      | (1ULL << (MQL_Parser::LT - 52))
      | (1ULL << (MQL_Parser::SINGLE_EQ - 52))
      | (1ULL << (MQL_Parser::MINUS - 52)))) != 0)) {
      setState(356);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case MQL_Parser::LT:
        case MQL_Parser::MINUS: {
          setState(354);
          edge();
          break;
        }

        case MQL_Parser::LEQ:
        case MQL_Parser::SINGLE_EQ: {
          setState(355);
          path();
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      setState(358);
      node();
      setState(364);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PathContext ------------------------------------------------------------------

MQL_Parser::PathContext::PathContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::PathContext::LEQ() {
  return getToken(MQL_Parser::LEQ, 0);
}

tree::TerminalNode* MQL_Parser::PathContext::LSQUARE_BRACKET() {
  return getToken(MQL_Parser::LSQUARE_BRACKET, 0);
}

MQL_Parser::PathAlternativesContext* MQL_Parser::PathContext::pathAlternatives() {
  return getRuleContext<MQL_Parser::PathAlternativesContext>(0);
}

tree::TerminalNode* MQL_Parser::PathContext::RSQUARE_BRACKET() {
  return getToken(MQL_Parser::RSQUARE_BRACKET, 0);
}

std::vector<tree::TerminalNode *> MQL_Parser::PathContext::SINGLE_EQ() {
  return getTokens(MQL_Parser::SINGLE_EQ);
}

tree::TerminalNode* MQL_Parser::PathContext::SINGLE_EQ(size_t i) {
  return getToken(MQL_Parser::SINGLE_EQ, i);
}

MQL_Parser::PathTypeContext* MQL_Parser::PathContext::pathType() {
  return getRuleContext<MQL_Parser::PathTypeContext>(0);
}

tree::TerminalNode* MQL_Parser::PathContext::VARIABLE() {
  return getToken(MQL_Parser::VARIABLE, 0);
}

tree::TerminalNode* MQL_Parser::PathContext::GT() {
  return getToken(MQL_Parser::GT, 0);
}


size_t MQL_Parser::PathContext::getRuleIndex() const {
  return MQL_Parser::RulePath;
}


antlrcpp::Any MQL_Parser::PathContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitPath(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::PathContext* MQL_Parser::path() {
  PathContext *_localctx = _tracker.createInstance<PathContext>(_ctx, getState());
  enterRule(_localctx, 52, MQL_Parser::RulePath);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(390);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case MQL_Parser::LEQ: {
        enterOuterAlt(_localctx, 1);
        setState(365);
        match(MQL_Parser::LEQ);
        setState(366);
        match(MQL_Parser::LSQUARE_BRACKET);
        setState(368);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == MQL_Parser::K_ANY

        || _la == MQL_Parser::K_ALL) {
          setState(367);
          pathType();
        }
        setState(371);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == MQL_Parser::VARIABLE) {
          setState(370);
          match(MQL_Parser::VARIABLE);
        }
        setState(373);
        pathAlternatives();
        setState(374);
        match(MQL_Parser::RSQUARE_BRACKET);
        setState(375);
        match(MQL_Parser::SINGLE_EQ);
        break;
      }

      case MQL_Parser::SINGLE_EQ: {
        enterOuterAlt(_localctx, 2);
        setState(377);
        match(MQL_Parser::SINGLE_EQ);
        setState(378);
        match(MQL_Parser::LSQUARE_BRACKET);
        setState(380);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == MQL_Parser::K_ANY

        || _la == MQL_Parser::K_ALL) {
          setState(379);
          pathType();
        }
        setState(383);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == MQL_Parser::VARIABLE) {
          setState(382);
          match(MQL_Parser::VARIABLE);
        }
        setState(385);
        pathAlternatives();
        setState(386);
        match(MQL_Parser::RSQUARE_BRACKET);
        setState(387);
        match(MQL_Parser::SINGLE_EQ);
        setState(388);
        match(MQL_Parser::GT);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PathAlternativesContext ------------------------------------------------------------------

MQL_Parser::PathAlternativesContext::PathAlternativesContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<MQL_Parser::PathSequenceContext *> MQL_Parser::PathAlternativesContext::pathSequence() {
  return getRuleContexts<MQL_Parser::PathSequenceContext>();
}

MQL_Parser::PathSequenceContext* MQL_Parser::PathAlternativesContext::pathSequence(size_t i) {
  return getRuleContext<MQL_Parser::PathSequenceContext>(i);
}

std::vector<tree::TerminalNode *> MQL_Parser::PathAlternativesContext::PATH_ALTERNATIVE() {
  return getTokens(MQL_Parser::PATH_ALTERNATIVE);
}

tree::TerminalNode* MQL_Parser::PathAlternativesContext::PATH_ALTERNATIVE(size_t i) {
  return getToken(MQL_Parser::PATH_ALTERNATIVE, i);
}


size_t MQL_Parser::PathAlternativesContext::getRuleIndex() const {
  return MQL_Parser::RulePathAlternatives;
}


antlrcpp::Any MQL_Parser::PathAlternativesContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitPathAlternatives(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::PathAlternativesContext* MQL_Parser::pathAlternatives() {
  PathAlternativesContext *_localctx = _tracker.createInstance<PathAlternativesContext>(_ctx, getState());
  enterRule(_localctx, 54, MQL_Parser::RulePathAlternatives);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(392);
    pathSequence();
    setState(397);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MQL_Parser::PATH_ALTERNATIVE) {
      setState(393);
      match(MQL_Parser::PATH_ALTERNATIVE);
      setState(394);
      pathSequence();
      setState(399);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PathSequenceContext ------------------------------------------------------------------

MQL_Parser::PathSequenceContext::PathSequenceContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<MQL_Parser::PathAtomContext *> MQL_Parser::PathSequenceContext::pathAtom() {
  return getRuleContexts<MQL_Parser::PathAtomContext>();
}

MQL_Parser::PathAtomContext* MQL_Parser::PathSequenceContext::pathAtom(size_t i) {
  return getRuleContext<MQL_Parser::PathAtomContext>(i);
}

std::vector<tree::TerminalNode *> MQL_Parser::PathSequenceContext::PATH_SEQUENCE() {
  return getTokens(MQL_Parser::PATH_SEQUENCE);
}

tree::TerminalNode* MQL_Parser::PathSequenceContext::PATH_SEQUENCE(size_t i) {
  return getToken(MQL_Parser::PATH_SEQUENCE, i);
}


size_t MQL_Parser::PathSequenceContext::getRuleIndex() const {
  return MQL_Parser::RulePathSequence;
}


antlrcpp::Any MQL_Parser::PathSequenceContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitPathSequence(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::PathSequenceContext* MQL_Parser::pathSequence() {
  PathSequenceContext *_localctx = _tracker.createInstance<PathSequenceContext>(_ctx, getState());
  enterRule(_localctx, 56, MQL_Parser::RulePathSequence);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(400);
    pathAtom();
    setState(405);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MQL_Parser::PATH_SEQUENCE) {
      setState(401);
      match(MQL_Parser::PATH_SEQUENCE);
      setState(402);
      pathAtom();
      setState(407);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PathAtomContext ------------------------------------------------------------------

MQL_Parser::PathAtomContext::PathAtomContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t MQL_Parser::PathAtomContext::getRuleIndex() const {
  return MQL_Parser::RulePathAtom;
}

void MQL_Parser::PathAtomContext::copyFrom(PathAtomContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- PathAtomAlternativesContext ------------------------------------------------------------------

tree::TerminalNode* MQL_Parser::PathAtomAlternativesContext::L_PAR() {
  return getToken(MQL_Parser::L_PAR, 0);
}

MQL_Parser::PathAlternativesContext* MQL_Parser::PathAtomAlternativesContext::pathAlternatives() {
  return getRuleContext<MQL_Parser::PathAlternativesContext>(0);
}

tree::TerminalNode* MQL_Parser::PathAtomAlternativesContext::R_PAR() {
  return getToken(MQL_Parser::R_PAR, 0);
}

tree::TerminalNode* MQL_Parser::PathAtomAlternativesContext::PATH_NEGATION() {
  return getToken(MQL_Parser::PATH_NEGATION, 0);
}

MQL_Parser::PathSuffixContext* MQL_Parser::PathAtomAlternativesContext::pathSuffix() {
  return getRuleContext<MQL_Parser::PathSuffixContext>(0);
}

MQL_Parser::PathAtomAlternativesContext::PathAtomAlternativesContext(PathAtomContext *ctx) { copyFrom(ctx); }


antlrcpp::Any MQL_Parser::PathAtomAlternativesContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitPathAtomAlternatives(this);
  else
    return visitor->visitChildren(this);
}
//----------------- PathAtomSimpleContext ------------------------------------------------------------------

tree::TerminalNode* MQL_Parser::PathAtomSimpleContext::TYPE() {
  return getToken(MQL_Parser::TYPE, 0);
}

tree::TerminalNode* MQL_Parser::PathAtomSimpleContext::PATH_NEGATION() {
  return getToken(MQL_Parser::PATH_NEGATION, 0);
}

MQL_Parser::PathSuffixContext* MQL_Parser::PathAtomSimpleContext::pathSuffix() {
  return getRuleContext<MQL_Parser::PathSuffixContext>(0);
}

MQL_Parser::PathAtomSimpleContext::PathAtomSimpleContext(PathAtomContext *ctx) { copyFrom(ctx); }


antlrcpp::Any MQL_Parser::PathAtomSimpleContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitPathAtomSimple(this);
  else
    return visitor->visitChildren(this);
}
MQL_Parser::PathAtomContext* MQL_Parser::pathAtom() {
  PathAtomContext *_localctx = _tracker.createInstance<PathAtomContext>(_ctx, getState());
  enterRule(_localctx, 58, MQL_Parser::RulePathAtom);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(424);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 51, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<MQL_Parser::PathAtomSimpleContext>(_localctx);
      enterOuterAlt(_localctx, 1);
      setState(409);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == MQL_Parser::PATH_NEGATION) {
        setState(408);
        match(MQL_Parser::PATH_NEGATION);
      }
      setState(411);
      match(MQL_Parser::TYPE);
      setState(413);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (((((_la - 62) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 62)) & ((1ULL << (MQL_Parser::STAR - 62))
        | (1ULL << (MQL_Parser::QUESTION_MARK - 62))
        | (1ULL << (MQL_Parser::PLUS - 62))
        | (1ULL << (MQL_Parser::LCURLY_BRACKET - 62)))) != 0)) {
        setState(412);
        pathSuffix();
      }
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<MQL_Parser::PathAtomAlternativesContext>(_localctx);
      enterOuterAlt(_localctx, 2);
      setState(416);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == MQL_Parser::PATH_NEGATION) {
        setState(415);
        match(MQL_Parser::PATH_NEGATION);
      }
      setState(418);
      match(MQL_Parser::L_PAR);
      setState(419);
      pathAlternatives();
      setState(420);
      match(MQL_Parser::R_PAR);
      setState(422);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (((((_la - 62) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 62)) & ((1ULL << (MQL_Parser::STAR - 62))
        | (1ULL << (MQL_Parser::QUESTION_MARK - 62))
        | (1ULL << (MQL_Parser::PLUS - 62))
        | (1ULL << (MQL_Parser::LCURLY_BRACKET - 62)))) != 0)) {
        setState(421);
        pathSuffix();
      }
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PathSuffixContext ------------------------------------------------------------------

MQL_Parser::PathSuffixContext::PathSuffixContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::PathSuffixContext::STAR() {
  return getToken(MQL_Parser::STAR, 0);
}

tree::TerminalNode* MQL_Parser::PathSuffixContext::PLUS() {
  return getToken(MQL_Parser::PLUS, 0);
}

tree::TerminalNode* MQL_Parser::PathSuffixContext::QUESTION_MARK() {
  return getToken(MQL_Parser::QUESTION_MARK, 0);
}

tree::TerminalNode* MQL_Parser::PathSuffixContext::LCURLY_BRACKET() {
  return getToken(MQL_Parser::LCURLY_BRACKET, 0);
}

tree::TerminalNode* MQL_Parser::PathSuffixContext::COMMA() {
  return getToken(MQL_Parser::COMMA, 0);
}

tree::TerminalNode* MQL_Parser::PathSuffixContext::RCURLY_BRACKET() {
  return getToken(MQL_Parser::RCURLY_BRACKET, 0);
}

std::vector<tree::TerminalNode *> MQL_Parser::PathSuffixContext::UNSIGNED_INTEGER() {
  return getTokens(MQL_Parser::UNSIGNED_INTEGER);
}

tree::TerminalNode* MQL_Parser::PathSuffixContext::UNSIGNED_INTEGER(size_t i) {
  return getToken(MQL_Parser::UNSIGNED_INTEGER, i);
}


size_t MQL_Parser::PathSuffixContext::getRuleIndex() const {
  return MQL_Parser::RulePathSuffix;
}


antlrcpp::Any MQL_Parser::PathSuffixContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitPathSuffix(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::PathSuffixContext* MQL_Parser::pathSuffix() {
  PathSuffixContext *_localctx = _tracker.createInstance<PathSuffixContext>(_ctx, getState());
  enterRule(_localctx, 60, MQL_Parser::RulePathSuffix);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(434);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case MQL_Parser::STAR: {
        enterOuterAlt(_localctx, 1);
        setState(426);
        antlrcpp::downCast<PathSuffixContext *>(_localctx)->op = match(MQL_Parser::STAR);
        break;
      }

      case MQL_Parser::PLUS: {
        enterOuterAlt(_localctx, 2);
        setState(427);
        antlrcpp::downCast<PathSuffixContext *>(_localctx)->op = match(MQL_Parser::PLUS);
        break;
      }

      case MQL_Parser::QUESTION_MARK: {
        enterOuterAlt(_localctx, 3);
        setState(428);
        antlrcpp::downCast<PathSuffixContext *>(_localctx)->op = match(MQL_Parser::QUESTION_MARK);
        break;
      }

      case MQL_Parser::LCURLY_BRACKET: {
        enterOuterAlt(_localctx, 4);
        setState(429);
        match(MQL_Parser::LCURLY_BRACKET);
        setState(430);
        antlrcpp::downCast<PathSuffixContext *>(_localctx)->min = match(MQL_Parser::UNSIGNED_INTEGER);
        setState(431);
        match(MQL_Parser::COMMA);
        setState(432);
        antlrcpp::downCast<PathSuffixContext *>(_localctx)->max = match(MQL_Parser::UNSIGNED_INTEGER);
        setState(433);
        match(MQL_Parser::RCURLY_BRACKET);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PathTypeContext ------------------------------------------------------------------

MQL_Parser::PathTypeContext::PathTypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::PathTypeContext::K_ANY() {
  return getToken(MQL_Parser::K_ANY, 0);
}

tree::TerminalNode* MQL_Parser::PathTypeContext::K_ALL() {
  return getToken(MQL_Parser::K_ALL, 0);
}


size_t MQL_Parser::PathTypeContext::getRuleIndex() const {
  return MQL_Parser::RulePathType;
}


antlrcpp::Any MQL_Parser::PathTypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitPathType(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::PathTypeContext* MQL_Parser::pathType() {
  PathTypeContext *_localctx = _tracker.createInstance<PathTypeContext>(_ctx, getState());
  enterRule(_localctx, 62, MQL_Parser::RulePathType);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(436);
    _la = _input->LA(1);
    if (!(_la == MQL_Parser::K_ANY

    || _la == MQL_Parser::K_ALL)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- NodeContext ------------------------------------------------------------------

MQL_Parser::NodeContext::NodeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

MQL_Parser::FixedNodeContext* MQL_Parser::NodeContext::fixedNode() {
  return getRuleContext<MQL_Parser::FixedNodeContext>(0);
}

MQL_Parser::VarNodeContext* MQL_Parser::NodeContext::varNode() {
  return getRuleContext<MQL_Parser::VarNodeContext>(0);
}


size_t MQL_Parser::NodeContext::getRuleIndex() const {
  return MQL_Parser::RuleNode;
}


antlrcpp::Any MQL_Parser::NodeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitNode(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::NodeContext* MQL_Parser::node() {
  NodeContext *_localctx = _tracker.createInstance<NodeContext>(_ctx, getState());
  enterRule(_localctx, 64, MQL_Parser::RuleNode);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(440);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 53, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(438);
      fixedNode();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(439);
      varNode();
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FixedNodeContext ------------------------------------------------------------------

MQL_Parser::FixedNodeContext::FixedNodeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::FixedNodeContext::L_PAR() {
  return getToken(MQL_Parser::L_PAR, 0);
}

MQL_Parser::FixedNodeInsideContext* MQL_Parser::FixedNodeContext::fixedNodeInside() {
  return getRuleContext<MQL_Parser::FixedNodeInsideContext>(0);
}

tree::TerminalNode* MQL_Parser::FixedNodeContext::R_PAR() {
  return getToken(MQL_Parser::R_PAR, 0);
}


size_t MQL_Parser::FixedNodeContext::getRuleIndex() const {
  return MQL_Parser::RuleFixedNode;
}


antlrcpp::Any MQL_Parser::FixedNodeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitFixedNode(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::FixedNodeContext* MQL_Parser::fixedNode() {
  FixedNodeContext *_localctx = _tracker.createInstance<FixedNodeContext>(_ctx, getState());
  enterRule(_localctx, 66, MQL_Parser::RuleFixedNode);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(442);
    match(MQL_Parser::L_PAR);
    setState(443);
    fixedNodeInside();
    setState(444);
    match(MQL_Parser::R_PAR);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FixedNodeInsideContext ------------------------------------------------------------------

MQL_Parser::FixedNodeInsideContext::FixedNodeInsideContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

MQL_Parser::IdentifierContext* MQL_Parser::FixedNodeInsideContext::identifier() {
  return getRuleContext<MQL_Parser::IdentifierContext>(0);
}

tree::TerminalNode* MQL_Parser::FixedNodeInsideContext::ANON_ID() {
  return getToken(MQL_Parser::ANON_ID, 0);
}

tree::TerminalNode* MQL_Parser::FixedNodeInsideContext::EDGE_ID() {
  return getToken(MQL_Parser::EDGE_ID, 0);
}

MQL_Parser::ValueContext* MQL_Parser::FixedNodeInsideContext::value() {
  return getRuleContext<MQL_Parser::ValueContext>(0);
}


size_t MQL_Parser::FixedNodeInsideContext::getRuleIndex() const {
  return MQL_Parser::RuleFixedNodeInside;
}


antlrcpp::Any MQL_Parser::FixedNodeInsideContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitFixedNodeInside(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::FixedNodeInsideContext* MQL_Parser::fixedNodeInside() {
  FixedNodeInsideContext *_localctx = _tracker.createInstance<FixedNodeInsideContext>(_ctx, getState());
  enterRule(_localctx, 68, MQL_Parser::RuleFixedNodeInside);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(450);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case MQL_Parser::K_ANY:
      case MQL_Parser::K_AND:
      case MQL_Parser::K_AVG:
      case MQL_Parser::K_ALL:
      case MQL_Parser::K_ASC:
      case MQL_Parser::K_BY:
      case MQL_Parser::K_BOOL:
      case MQL_Parser::K_COUNT:
      case MQL_Parser::K_DESCRIBE:
      case MQL_Parser::K_DESC:
      case MQL_Parser::K_DISTINCT:
      case MQL_Parser::K_EDGE:
      case MQL_Parser::K_INTEGER:
      case MQL_Parser::K_INSERT:
      case MQL_Parser::K_IS:
      case MQL_Parser::K_FLOAT:
      case MQL_Parser::K_GROUP:
      case MQL_Parser::K_LABEL:
      case MQL_Parser::K_LIMIT:
      case MQL_Parser::K_MAX:
      case MQL_Parser::K_MATCH:
      case MQL_Parser::K_MIN:
      case MQL_Parser::K_OPTIONAL:
      case MQL_Parser::K_ORDER:
      case MQL_Parser::K_OR:
      case MQL_Parser::K_PROPERTY:
      case MQL_Parser::K_NOT:
      case MQL_Parser::K_NULL:
      case MQL_Parser::K_SET:
      case MQL_Parser::K_SUM:
      case MQL_Parser::K_STRING:
      case MQL_Parser::K_RETURN:
      case MQL_Parser::K_WHERE:
      case MQL_Parser::NAME: {
        enterOuterAlt(_localctx, 1);
        setState(446);
        identifier();
        break;
      }

      case MQL_Parser::ANON_ID: {
        enterOuterAlt(_localctx, 2);
        setState(447);
        match(MQL_Parser::ANON_ID);
        break;
      }

      case MQL_Parser::EDGE_ID: {
        enterOuterAlt(_localctx, 3);
        setState(448);
        match(MQL_Parser::EDGE_ID);
        break;
      }

      case MQL_Parser::K_FALSE:
      case MQL_Parser::K_TRUE:
      case MQL_Parser::STRING:
      case MQL_Parser::UNSIGNED_INTEGER:
      case MQL_Parser::UNSIGNED_FLOAT:
      case MQL_Parser::PLUS:
      case MQL_Parser::MINUS: {
        enterOuterAlt(_localctx, 4);
        setState(449);
        value();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- EdgeContext ------------------------------------------------------------------

MQL_Parser::EdgeContext::EdgeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::EdgeContext::LT() {
  return getToken(MQL_Parser::LT, 0);
}

std::vector<tree::TerminalNode *> MQL_Parser::EdgeContext::MINUS() {
  return getTokens(MQL_Parser::MINUS);
}

tree::TerminalNode* MQL_Parser::EdgeContext::MINUS(size_t i) {
  return getToken(MQL_Parser::MINUS, i);
}

MQL_Parser::EdgeInsideContext* MQL_Parser::EdgeContext::edgeInside() {
  return getRuleContext<MQL_Parser::EdgeInsideContext>(0);
}

tree::TerminalNode* MQL_Parser::EdgeContext::GT() {
  return getToken(MQL_Parser::GT, 0);
}


size_t MQL_Parser::EdgeContext::getRuleIndex() const {
  return MQL_Parser::RuleEdge;
}


antlrcpp::Any MQL_Parser::EdgeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitEdge(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::EdgeContext* MQL_Parser::edge() {
  EdgeContext *_localctx = _tracker.createInstance<EdgeContext>(_ctx, getState());
  enterRule(_localctx, 70, MQL_Parser::RuleEdge);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(465);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case MQL_Parser::LT: {
        enterOuterAlt(_localctx, 1);
        setState(452);
        match(MQL_Parser::LT);
        setState(455);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 55, _ctx)) {
        case 1: {
          setState(453);
          match(MQL_Parser::MINUS);
          setState(454);
          edgeInside();
          break;
        }

        default:
          break;
        }
        setState(457);
        match(MQL_Parser::MINUS);
        break;
      }

      case MQL_Parser::MINUS: {
        enterOuterAlt(_localctx, 2);
        setState(458);
        match(MQL_Parser::MINUS);
        setState(462);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == MQL_Parser::LSQUARE_BRACKET) {
          setState(459);
          edgeInside();
          setState(460);
          match(MQL_Parser::MINUS);
        }
        setState(464);
        match(MQL_Parser::GT);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- EdgeInsideContext ------------------------------------------------------------------

MQL_Parser::EdgeInsideContext::EdgeInsideContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::EdgeInsideContext::LSQUARE_BRACKET() {
  return getToken(MQL_Parser::LSQUARE_BRACKET, 0);
}

tree::TerminalNode* MQL_Parser::EdgeInsideContext::RSQUARE_BRACKET() {
  return getToken(MQL_Parser::RSQUARE_BRACKET, 0);
}

MQL_Parser::PropertiesContext* MQL_Parser::EdgeInsideContext::properties() {
  return getRuleContext<MQL_Parser::PropertiesContext>(0);
}

tree::TerminalNode* MQL_Parser::EdgeInsideContext::VARIABLE() {
  return getToken(MQL_Parser::VARIABLE, 0);
}

tree::TerminalNode* MQL_Parser::EdgeInsideContext::EDGE_ID() {
  return getToken(MQL_Parser::EDGE_ID, 0);
}

tree::TerminalNode* MQL_Parser::EdgeInsideContext::TYPE() {
  return getToken(MQL_Parser::TYPE, 0);
}

tree::TerminalNode* MQL_Parser::EdgeInsideContext::TYPE_VAR() {
  return getToken(MQL_Parser::TYPE_VAR, 0);
}


size_t MQL_Parser::EdgeInsideContext::getRuleIndex() const {
  return MQL_Parser::RuleEdgeInside;
}


antlrcpp::Any MQL_Parser::EdgeInsideContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitEdgeInside(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::EdgeInsideContext* MQL_Parser::edgeInside() {
  EdgeInsideContext *_localctx = _tracker.createInstance<EdgeInsideContext>(_ctx, getState());
  enterRule(_localctx, 72, MQL_Parser::RuleEdgeInside);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(467);
    match(MQL_Parser::LSQUARE_BRACKET);
    setState(469);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MQL_Parser::EDGE_ID

    || _la == MQL_Parser::VARIABLE) {
      setState(468);
      _la = _input->LA(1);
      if (!(_la == MQL_Parser::EDGE_ID

      || _la == MQL_Parser::VARIABLE)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
    }
    setState(472);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MQL_Parser::TYPE

    || _la == MQL_Parser::TYPE_VAR) {
      setState(471);
      _la = _input->LA(1);
      if (!(_la == MQL_Parser::TYPE

      || _la == MQL_Parser::TYPE_VAR)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
    }
    setState(475);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MQL_Parser::LCURLY_BRACKET) {
      setState(474);
      properties();
    }
    setState(477);
    match(MQL_Parser::RSQUARE_BRACKET);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- VarNodeContext ------------------------------------------------------------------

MQL_Parser::VarNodeContext::VarNodeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::VarNodeContext::L_PAR() {
  return getToken(MQL_Parser::L_PAR, 0);
}

tree::TerminalNode* MQL_Parser::VarNodeContext::R_PAR() {
  return getToken(MQL_Parser::R_PAR, 0);
}

tree::TerminalNode* MQL_Parser::VarNodeContext::VARIABLE() {
  return getToken(MQL_Parser::VARIABLE, 0);
}

std::vector<tree::TerminalNode *> MQL_Parser::VarNodeContext::TYPE() {
  return getTokens(MQL_Parser::TYPE);
}

tree::TerminalNode* MQL_Parser::VarNodeContext::TYPE(size_t i) {
  return getToken(MQL_Parser::TYPE, i);
}

MQL_Parser::PropertiesContext* MQL_Parser::VarNodeContext::properties() {
  return getRuleContext<MQL_Parser::PropertiesContext>(0);
}


size_t MQL_Parser::VarNodeContext::getRuleIndex() const {
  return MQL_Parser::RuleVarNode;
}


antlrcpp::Any MQL_Parser::VarNodeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitVarNode(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::VarNodeContext* MQL_Parser::varNode() {
  VarNodeContext *_localctx = _tracker.createInstance<VarNodeContext>(_ctx, getState());
  enterRule(_localctx, 74, MQL_Parser::RuleVarNode);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(479);
    match(MQL_Parser::L_PAR);
    setState(481);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MQL_Parser::VARIABLE) {
      setState(480);
      match(MQL_Parser::VARIABLE);
    }
    setState(486);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MQL_Parser::TYPE) {
      setState(483);
      match(MQL_Parser::TYPE);
      setState(488);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(490);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MQL_Parser::LCURLY_BRACKET) {
      setState(489);
      properties();
    }
    setState(492);
    match(MQL_Parser::R_PAR);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PropertiesContext ------------------------------------------------------------------

MQL_Parser::PropertiesContext::PropertiesContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::PropertiesContext::LCURLY_BRACKET() {
  return getToken(MQL_Parser::LCURLY_BRACKET, 0);
}

std::vector<MQL_Parser::PropertyContext *> MQL_Parser::PropertiesContext::property() {
  return getRuleContexts<MQL_Parser::PropertyContext>();
}

MQL_Parser::PropertyContext* MQL_Parser::PropertiesContext::property(size_t i) {
  return getRuleContext<MQL_Parser::PropertyContext>(i);
}

tree::TerminalNode* MQL_Parser::PropertiesContext::RCURLY_BRACKET() {
  return getToken(MQL_Parser::RCURLY_BRACKET, 0);
}

std::vector<tree::TerminalNode *> MQL_Parser::PropertiesContext::COMMA() {
  return getTokens(MQL_Parser::COMMA);
}

tree::TerminalNode* MQL_Parser::PropertiesContext::COMMA(size_t i) {
  return getToken(MQL_Parser::COMMA, i);
}


size_t MQL_Parser::PropertiesContext::getRuleIndex() const {
  return MQL_Parser::RuleProperties;
}


antlrcpp::Any MQL_Parser::PropertiesContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitProperties(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::PropertiesContext* MQL_Parser::properties() {
  PropertiesContext *_localctx = _tracker.createInstance<PropertiesContext>(_ctx, getState());
  enterRule(_localctx, 76, MQL_Parser::RuleProperties);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(494);
    match(MQL_Parser::LCURLY_BRACKET);
    setState(495);
    property();
    setState(500);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MQL_Parser::COMMA) {
      setState(496);
      match(MQL_Parser::COMMA);
      setState(497);
      property();
      setState(502);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(503);
    match(MQL_Parser::RCURLY_BRACKET);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PropertyContext ------------------------------------------------------------------

MQL_Parser::PropertyContext::PropertyContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

MQL_Parser::IdentifierContext* MQL_Parser::PropertyContext::identifier() {
  return getRuleContext<MQL_Parser::IdentifierContext>(0);
}

tree::TerminalNode* MQL_Parser::PropertyContext::COLON() {
  return getToken(MQL_Parser::COLON, 0);
}

MQL_Parser::ValueContext* MQL_Parser::PropertyContext::value() {
  return getRuleContext<MQL_Parser::ValueContext>(0);
}

tree::TerminalNode* MQL_Parser::PropertyContext::TRUE_PROP() {
  return getToken(MQL_Parser::TRUE_PROP, 0);
}

tree::TerminalNode* MQL_Parser::PropertyContext::FALSE_PROP() {
  return getToken(MQL_Parser::FALSE_PROP, 0);
}


size_t MQL_Parser::PropertyContext::getRuleIndex() const {
  return MQL_Parser::RuleProperty;
}


antlrcpp::Any MQL_Parser::PropertyContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitProperty(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::PropertyContext* MQL_Parser::property() {
  PropertyContext *_localctx = _tracker.createInstance<PropertyContext>(_ctx, getState());
  enterRule(_localctx, 78, MQL_Parser::RuleProperty);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(505);
    identifier();
    setState(510);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case MQL_Parser::COLON: {
        setState(506);
        match(MQL_Parser::COLON);
        setState(507);
        value();
        break;
      }

      case MQL_Parser::TRUE_PROP: {
        setState(508);
        match(MQL_Parser::TRUE_PROP);
        break;
      }

      case MQL_Parser::FALSE_PROP: {
        setState(509);
        match(MQL_Parser::FALSE_PROP);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- IdentifierContext ------------------------------------------------------------------

MQL_Parser::IdentifierContext::IdentifierContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::IdentifierContext::NAME() {
  return getToken(MQL_Parser::NAME, 0);
}

MQL_Parser::KeywordContext* MQL_Parser::IdentifierContext::keyword() {
  return getRuleContext<MQL_Parser::KeywordContext>(0);
}


size_t MQL_Parser::IdentifierContext::getRuleIndex() const {
  return MQL_Parser::RuleIdentifier;
}


antlrcpp::Any MQL_Parser::IdentifierContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitIdentifier(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::IdentifierContext* MQL_Parser::identifier() {
  IdentifierContext *_localctx = _tracker.createInstance<IdentifierContext>(_ctx, getState());
  enterRule(_localctx, 80, MQL_Parser::RuleIdentifier);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(514);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case MQL_Parser::NAME: {
        enterOuterAlt(_localctx, 1);
        setState(512);
        match(MQL_Parser::NAME);
        break;
      }

      case MQL_Parser::K_ANY:
      case MQL_Parser::K_AND:
      case MQL_Parser::K_AVG:
      case MQL_Parser::K_ALL:
      case MQL_Parser::K_ASC:
      case MQL_Parser::K_BY:
      case MQL_Parser::K_BOOL:
      case MQL_Parser::K_COUNT:
      case MQL_Parser::K_DESCRIBE:
      case MQL_Parser::K_DESC:
      case MQL_Parser::K_DISTINCT:
      case MQL_Parser::K_EDGE:
      case MQL_Parser::K_INTEGER:
      case MQL_Parser::K_INSERT:
      case MQL_Parser::K_IS:
      case MQL_Parser::K_FLOAT:
      case MQL_Parser::K_GROUP:
      case MQL_Parser::K_LABEL:
      case MQL_Parser::K_LIMIT:
      case MQL_Parser::K_MAX:
      case MQL_Parser::K_MATCH:
      case MQL_Parser::K_MIN:
      case MQL_Parser::K_OPTIONAL:
      case MQL_Parser::K_ORDER:
      case MQL_Parser::K_OR:
      case MQL_Parser::K_PROPERTY:
      case MQL_Parser::K_NOT:
      case MQL_Parser::K_NULL:
      case MQL_Parser::K_SET:
      case MQL_Parser::K_SUM:
      case MQL_Parser::K_STRING:
      case MQL_Parser::K_RETURN:
      case MQL_Parser::K_WHERE: {
        enterOuterAlt(_localctx, 2);
        setState(513);
        keyword();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BoolValueContext ------------------------------------------------------------------

MQL_Parser::BoolValueContext::BoolValueContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::BoolValueContext::K_TRUE() {
  return getToken(MQL_Parser::K_TRUE, 0);
}

tree::TerminalNode* MQL_Parser::BoolValueContext::K_FALSE() {
  return getToken(MQL_Parser::K_FALSE, 0);
}


size_t MQL_Parser::BoolValueContext::getRuleIndex() const {
  return MQL_Parser::RuleBoolValue;
}


antlrcpp::Any MQL_Parser::BoolValueContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitBoolValue(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::BoolValueContext* MQL_Parser::boolValue() {
  BoolValueContext *_localctx = _tracker.createInstance<BoolValueContext>(_ctx, getState());
  enterRule(_localctx, 82, MQL_Parser::RuleBoolValue);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(516);
    _la = _input->LA(1);
    if (!(_la == MQL_Parser::K_FALSE

    || _la == MQL_Parser::K_TRUE)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- NumericValueContext ------------------------------------------------------------------

MQL_Parser::NumericValueContext::NumericValueContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::NumericValueContext::UNSIGNED_INTEGER() {
  return getToken(MQL_Parser::UNSIGNED_INTEGER, 0);
}

tree::TerminalNode* MQL_Parser::NumericValueContext::UNSIGNED_FLOAT() {
  return getToken(MQL_Parser::UNSIGNED_FLOAT, 0);
}

tree::TerminalNode* MQL_Parser::NumericValueContext::PLUS() {
  return getToken(MQL_Parser::PLUS, 0);
}

tree::TerminalNode* MQL_Parser::NumericValueContext::MINUS() {
  return getToken(MQL_Parser::MINUS, 0);
}


size_t MQL_Parser::NumericValueContext::getRuleIndex() const {
  return MQL_Parser::RuleNumericValue;
}


antlrcpp::Any MQL_Parser::NumericValueContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitNumericValue(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::NumericValueContext* MQL_Parser::numericValue() {
  NumericValueContext *_localctx = _tracker.createInstance<NumericValueContext>(_ctx, getState());
  enterRule(_localctx, 84, MQL_Parser::RuleNumericValue);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(519);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MQL_Parser::PLUS

    || _la == MQL_Parser::MINUS) {
      setState(518);
      _la = _input->LA(1);
      if (!(_la == MQL_Parser::PLUS

      || _la == MQL_Parser::MINUS)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
    }
    setState(521);
    _la = _input->LA(1);
    if (!(_la == MQL_Parser::UNSIGNED_INTEGER

    || _la == MQL_Parser::UNSIGNED_FLOAT)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ValueContext ------------------------------------------------------------------

MQL_Parser::ValueContext::ValueContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

MQL_Parser::NumericValueContext* MQL_Parser::ValueContext::numericValue() {
  return getRuleContext<MQL_Parser::NumericValueContext>(0);
}

tree::TerminalNode* MQL_Parser::ValueContext::STRING() {
  return getToken(MQL_Parser::STRING, 0);
}

MQL_Parser::BoolValueContext* MQL_Parser::ValueContext::boolValue() {
  return getRuleContext<MQL_Parser::BoolValueContext>(0);
}


size_t MQL_Parser::ValueContext::getRuleIndex() const {
  return MQL_Parser::RuleValue;
}


antlrcpp::Any MQL_Parser::ValueContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitValue(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::ValueContext* MQL_Parser::value() {
  ValueContext *_localctx = _tracker.createInstance<ValueContext>(_ctx, getState());
  enterRule(_localctx, 86, MQL_Parser::RuleValue);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(526);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case MQL_Parser::UNSIGNED_INTEGER:
      case MQL_Parser::UNSIGNED_FLOAT:
      case MQL_Parser::PLUS:
      case MQL_Parser::MINUS: {
        enterOuterAlt(_localctx, 1);
        setState(523);
        numericValue();
        break;
      }

      case MQL_Parser::STRING: {
        enterOuterAlt(_localctx, 2);
        setState(524);
        match(MQL_Parser::STRING);
        break;
      }

      case MQL_Parser::K_FALSE:
      case MQL_Parser::K_TRUE: {
        enterOuterAlt(_localctx, 3);
        setState(525);
        boolValue();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ConditionalOrExprContext ------------------------------------------------------------------

MQL_Parser::ConditionalOrExprContext::ConditionalOrExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<MQL_Parser::ConditionalAndExprContext *> MQL_Parser::ConditionalOrExprContext::conditionalAndExpr() {
  return getRuleContexts<MQL_Parser::ConditionalAndExprContext>();
}

MQL_Parser::ConditionalAndExprContext* MQL_Parser::ConditionalOrExprContext::conditionalAndExpr(size_t i) {
  return getRuleContext<MQL_Parser::ConditionalAndExprContext>(i);
}

std::vector<tree::TerminalNode *> MQL_Parser::ConditionalOrExprContext::K_OR() {
  return getTokens(MQL_Parser::K_OR);
}

tree::TerminalNode* MQL_Parser::ConditionalOrExprContext::K_OR(size_t i) {
  return getToken(MQL_Parser::K_OR, i);
}


size_t MQL_Parser::ConditionalOrExprContext::getRuleIndex() const {
  return MQL_Parser::RuleConditionalOrExpr;
}


antlrcpp::Any MQL_Parser::ConditionalOrExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitConditionalOrExpr(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::ConditionalOrExprContext* MQL_Parser::conditionalOrExpr() {
  ConditionalOrExprContext *_localctx = _tracker.createInstance<ConditionalOrExprContext>(_ctx, getState());
  enterRule(_localctx, 88, MQL_Parser::RuleConditionalOrExpr);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(528);
    conditionalAndExpr();
    setState(533);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MQL_Parser::K_OR) {
      setState(529);
      match(MQL_Parser::K_OR);
      setState(530);
      conditionalAndExpr();
      setState(535);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ConditionalAndExprContext ------------------------------------------------------------------

MQL_Parser::ConditionalAndExprContext::ConditionalAndExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<MQL_Parser::ComparisonExprContext *> MQL_Parser::ConditionalAndExprContext::comparisonExpr() {
  return getRuleContexts<MQL_Parser::ComparisonExprContext>();
}

MQL_Parser::ComparisonExprContext* MQL_Parser::ConditionalAndExprContext::comparisonExpr(size_t i) {
  return getRuleContext<MQL_Parser::ComparisonExprContext>(i);
}

std::vector<tree::TerminalNode *> MQL_Parser::ConditionalAndExprContext::K_AND() {
  return getTokens(MQL_Parser::K_AND);
}

tree::TerminalNode* MQL_Parser::ConditionalAndExprContext::K_AND(size_t i) {
  return getToken(MQL_Parser::K_AND, i);
}


size_t MQL_Parser::ConditionalAndExprContext::getRuleIndex() const {
  return MQL_Parser::RuleConditionalAndExpr;
}


antlrcpp::Any MQL_Parser::ConditionalAndExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitConditionalAndExpr(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::ConditionalAndExprContext* MQL_Parser::conditionalAndExpr() {
  ConditionalAndExprContext *_localctx = _tracker.createInstance<ConditionalAndExprContext>(_ctx, getState());
  enterRule(_localctx, 90, MQL_Parser::RuleConditionalAndExpr);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(536);
    comparisonExpr();
    setState(541);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MQL_Parser::K_AND) {
      setState(537);
      match(MQL_Parser::K_AND);
      setState(538);
      comparisonExpr();
      setState(543);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ComparisonExprContext ------------------------------------------------------------------

MQL_Parser::ComparisonExprContext::ComparisonExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t MQL_Parser::ComparisonExprContext::getRuleIndex() const {
  return MQL_Parser::RuleComparisonExpr;
}

void MQL_Parser::ComparisonExprContext::copyFrom(ComparisonExprContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- ComparisonExprIsContext ------------------------------------------------------------------

MQL_Parser::AdditiveExprContext* MQL_Parser::ComparisonExprIsContext::additiveExpr() {
  return getRuleContext<MQL_Parser::AdditiveExprContext>(0);
}

tree::TerminalNode* MQL_Parser::ComparisonExprIsContext::K_IS() {
  return getToken(MQL_Parser::K_IS, 0);
}

MQL_Parser::ExprTypenameContext* MQL_Parser::ComparisonExprIsContext::exprTypename() {
  return getRuleContext<MQL_Parser::ExprTypenameContext>(0);
}

tree::TerminalNode* MQL_Parser::ComparisonExprIsContext::K_NOT() {
  return getToken(MQL_Parser::K_NOT, 0);
}

MQL_Parser::ComparisonExprIsContext::ComparisonExprIsContext(ComparisonExprContext *ctx) { copyFrom(ctx); }


antlrcpp::Any MQL_Parser::ComparisonExprIsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitComparisonExprIs(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ComparisonExprOpContext ------------------------------------------------------------------

std::vector<MQL_Parser::AdditiveExprContext *> MQL_Parser::ComparisonExprOpContext::additiveExpr() {
  return getRuleContexts<MQL_Parser::AdditiveExprContext>();
}

MQL_Parser::AdditiveExprContext* MQL_Parser::ComparisonExprOpContext::additiveExpr(size_t i) {
  return getRuleContext<MQL_Parser::AdditiveExprContext>(i);
}

tree::TerminalNode* MQL_Parser::ComparisonExprOpContext::EQ() {
  return getToken(MQL_Parser::EQ, 0);
}

tree::TerminalNode* MQL_Parser::ComparisonExprOpContext::NEQ() {
  return getToken(MQL_Parser::NEQ, 0);
}

tree::TerminalNode* MQL_Parser::ComparisonExprOpContext::LT() {
  return getToken(MQL_Parser::LT, 0);
}

tree::TerminalNode* MQL_Parser::ComparisonExprOpContext::GT() {
  return getToken(MQL_Parser::GT, 0);
}

tree::TerminalNode* MQL_Parser::ComparisonExprOpContext::LEQ() {
  return getToken(MQL_Parser::LEQ, 0);
}

tree::TerminalNode* MQL_Parser::ComparisonExprOpContext::GEQ() {
  return getToken(MQL_Parser::GEQ, 0);
}

MQL_Parser::ComparisonExprOpContext::ComparisonExprOpContext(ComparisonExprContext *ctx) { copyFrom(ctx); }


antlrcpp::Any MQL_Parser::ComparisonExprOpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitComparisonExprOp(this);
  else
    return visitor->visitChildren(this);
}
MQL_Parser::ComparisonExprContext* MQL_Parser::comparisonExpr() {
  ComparisonExprContext *_localctx = _tracker.createInstance<ComparisonExprContext>(_ctx, getState());
  enterRule(_localctx, 92, MQL_Parser::RuleComparisonExpr);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(556);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 73, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<MQL_Parser::ComparisonExprOpContext>(_localctx);
      enterOuterAlt(_localctx, 1);
      setState(544);
      additiveExpr();
      setState(547);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & ((1ULL << MQL_Parser::LEQ)
        | (1ULL << MQL_Parser::GEQ)
        | (1ULL << MQL_Parser::EQ)
        | (1ULL << MQL_Parser::NEQ)
        | (1ULL << MQL_Parser::LT)
        | (1ULL << MQL_Parser::GT))) != 0)) {
        setState(545);
        antlrcpp::downCast<ComparisonExprOpContext *>(_localctx)->op = _input->LT(1);
        _la = _input->LA(1);
        if (!((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & ((1ULL << MQL_Parser::LEQ)
          | (1ULL << MQL_Parser::GEQ)
          | (1ULL << MQL_Parser::EQ)
          | (1ULL << MQL_Parser::NEQ)
          | (1ULL << MQL_Parser::LT)
          | (1ULL << MQL_Parser::GT))) != 0))) {
          antlrcpp::downCast<ComparisonExprOpContext *>(_localctx)->op = _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        setState(546);
        additiveExpr();
      }
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<MQL_Parser::ComparisonExprIsContext>(_localctx);
      enterOuterAlt(_localctx, 2);
      setState(549);
      additiveExpr();
      setState(550);
      match(MQL_Parser::K_IS);
      setState(552);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == MQL_Parser::K_NOT) {
        setState(551);
        match(MQL_Parser::K_NOT);
      }
      setState(554);
      exprTypename();
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AdditiveExprContext ------------------------------------------------------------------

MQL_Parser::AdditiveExprContext::AdditiveExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<MQL_Parser::MultiplicativeExprContext *> MQL_Parser::AdditiveExprContext::multiplicativeExpr() {
  return getRuleContexts<MQL_Parser::MultiplicativeExprContext>();
}

MQL_Parser::MultiplicativeExprContext* MQL_Parser::AdditiveExprContext::multiplicativeExpr(size_t i) {
  return getRuleContext<MQL_Parser::MultiplicativeExprContext>(i);
}

std::vector<tree::TerminalNode *> MQL_Parser::AdditiveExprContext::PLUS() {
  return getTokens(MQL_Parser::PLUS);
}

tree::TerminalNode* MQL_Parser::AdditiveExprContext::PLUS(size_t i) {
  return getToken(MQL_Parser::PLUS, i);
}

std::vector<tree::TerminalNode *> MQL_Parser::AdditiveExprContext::MINUS() {
  return getTokens(MQL_Parser::MINUS);
}

tree::TerminalNode* MQL_Parser::AdditiveExprContext::MINUS(size_t i) {
  return getToken(MQL_Parser::MINUS, i);
}


size_t MQL_Parser::AdditiveExprContext::getRuleIndex() const {
  return MQL_Parser::RuleAdditiveExpr;
}


antlrcpp::Any MQL_Parser::AdditiveExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitAdditiveExpr(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::AdditiveExprContext* MQL_Parser::additiveExpr() {
  AdditiveExprContext *_localctx = _tracker.createInstance<AdditiveExprContext>(_ctx, getState());
  enterRule(_localctx, 94, MQL_Parser::RuleAdditiveExpr);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(558);
    multiplicativeExpr();
    setState(563);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MQL_Parser::PLUS

    || _la == MQL_Parser::MINUS) {
      setState(559);
      antlrcpp::downCast<AdditiveExprContext *>(_localctx)->_tset990 = _input->LT(1);
      _la = _input->LA(1);
      if (!(_la == MQL_Parser::PLUS

      || _la == MQL_Parser::MINUS)) {
        antlrcpp::downCast<AdditiveExprContext *>(_localctx)->_tset990 = _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      antlrcpp::downCast<AdditiveExprContext *>(_localctx)->op.push_back(antlrcpp::downCast<AdditiveExprContext *>(_localctx)->_tset990);
      setState(560);
      multiplicativeExpr();
      setState(565);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- MultiplicativeExprContext ------------------------------------------------------------------

MQL_Parser::MultiplicativeExprContext::MultiplicativeExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<MQL_Parser::UnaryExprContext *> MQL_Parser::MultiplicativeExprContext::unaryExpr() {
  return getRuleContexts<MQL_Parser::UnaryExprContext>();
}

MQL_Parser::UnaryExprContext* MQL_Parser::MultiplicativeExprContext::unaryExpr(size_t i) {
  return getRuleContext<MQL_Parser::UnaryExprContext>(i);
}

std::vector<tree::TerminalNode *> MQL_Parser::MultiplicativeExprContext::STAR() {
  return getTokens(MQL_Parser::STAR);
}

tree::TerminalNode* MQL_Parser::MultiplicativeExprContext::STAR(size_t i) {
  return getToken(MQL_Parser::STAR, i);
}

std::vector<tree::TerminalNode *> MQL_Parser::MultiplicativeExprContext::PATH_SEQUENCE() {
  return getTokens(MQL_Parser::PATH_SEQUENCE);
}

tree::TerminalNode* MQL_Parser::MultiplicativeExprContext::PATH_SEQUENCE(size_t i) {
  return getToken(MQL_Parser::PATH_SEQUENCE, i);
}

std::vector<tree::TerminalNode *> MQL_Parser::MultiplicativeExprContext::PERCENT() {
  return getTokens(MQL_Parser::PERCENT);
}

tree::TerminalNode* MQL_Parser::MultiplicativeExprContext::PERCENT(size_t i) {
  return getToken(MQL_Parser::PERCENT, i);
}


size_t MQL_Parser::MultiplicativeExprContext::getRuleIndex() const {
  return MQL_Parser::RuleMultiplicativeExpr;
}


antlrcpp::Any MQL_Parser::MultiplicativeExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitMultiplicativeExpr(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::MultiplicativeExprContext* MQL_Parser::multiplicativeExpr() {
  MultiplicativeExprContext *_localctx = _tracker.createInstance<MultiplicativeExprContext>(_ctx, getState());
  enterRule(_localctx, 96, MQL_Parser::RuleMultiplicativeExpr);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(566);
    unaryExpr();
    setState(571);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << MQL_Parser::PATH_SEQUENCE)
      | (1ULL << MQL_Parser::STAR)
      | (1ULL << MQL_Parser::PERCENT))) != 0)) {
      setState(567);
      antlrcpp::downCast<MultiplicativeExprContext *>(_localctx)->_tset1009 = _input->LT(1);
      _la = _input->LA(1);
      if (!((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & ((1ULL << MQL_Parser::PATH_SEQUENCE)
        | (1ULL << MQL_Parser::STAR)
        | (1ULL << MQL_Parser::PERCENT))) != 0))) {
        antlrcpp::downCast<MultiplicativeExprContext *>(_localctx)->_tset1009 = _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      antlrcpp::downCast<MultiplicativeExprContext *>(_localctx)->op.push_back(antlrcpp::downCast<MultiplicativeExprContext *>(_localctx)->_tset1009);
      setState(568);
      unaryExpr();
      setState(573);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- UnaryExprContext ------------------------------------------------------------------

MQL_Parser::UnaryExprContext::UnaryExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::UnaryExprContext::K_NOT() {
  return getToken(MQL_Parser::K_NOT, 0);
}

MQL_Parser::UnaryExprContext* MQL_Parser::UnaryExprContext::unaryExpr() {
  return getRuleContext<MQL_Parser::UnaryExprContext>(0);
}

tree::TerminalNode* MQL_Parser::UnaryExprContext::PLUS() {
  return getToken(MQL_Parser::PLUS, 0);
}

tree::TerminalNode* MQL_Parser::UnaryExprContext::MINUS() {
  return getToken(MQL_Parser::MINUS, 0);
}

MQL_Parser::AtomicExprContext* MQL_Parser::UnaryExprContext::atomicExpr() {
  return getRuleContext<MQL_Parser::AtomicExprContext>(0);
}


size_t MQL_Parser::UnaryExprContext::getRuleIndex() const {
  return MQL_Parser::RuleUnaryExpr;
}


antlrcpp::Any MQL_Parser::UnaryExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitUnaryExpr(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::UnaryExprContext* MQL_Parser::unaryExpr() {
  UnaryExprContext *_localctx = _tracker.createInstance<UnaryExprContext>(_ctx, getState());
  enterRule(_localctx, 98, MQL_Parser::RuleUnaryExpr);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(581);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case MQL_Parser::K_NOT: {
        enterOuterAlt(_localctx, 1);
        setState(574);
        match(MQL_Parser::K_NOT);
        setState(575);
        unaryExpr();
        break;
      }

      case MQL_Parser::PLUS: {
        enterOuterAlt(_localctx, 2);
        setState(576);
        match(MQL_Parser::PLUS);
        setState(577);
        unaryExpr();
        break;
      }

      case MQL_Parser::MINUS: {
        enterOuterAlt(_localctx, 3);
        setState(578);
        match(MQL_Parser::MINUS);
        setState(579);
        unaryExpr();
        break;
      }

      case MQL_Parser::K_FALSE:
      case MQL_Parser::K_TRUE:
      case MQL_Parser::VARIABLE:
      case MQL_Parser::STRING:
      case MQL_Parser::UNSIGNED_INTEGER:
      case MQL_Parser::UNSIGNED_FLOAT:
      case MQL_Parser::L_PAR: {
        enterOuterAlt(_localctx, 4);
        setState(580);
        atomicExpr();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AtomicExprContext ------------------------------------------------------------------

MQL_Parser::AtomicExprContext::AtomicExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t MQL_Parser::AtomicExprContext::getRuleIndex() const {
  return MQL_Parser::RuleAtomicExpr;
}

void MQL_Parser::AtomicExprContext::copyFrom(AtomicExprContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- ExprValueExprContext ------------------------------------------------------------------

MQL_Parser::ValueExprContext* MQL_Parser::ExprValueExprContext::valueExpr() {
  return getRuleContext<MQL_Parser::ValueExprContext>(0);
}

MQL_Parser::ExprValueExprContext::ExprValueExprContext(AtomicExprContext *ctx) { copyFrom(ctx); }


antlrcpp::Any MQL_Parser::ExprValueExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitExprValueExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ExprVarContext ------------------------------------------------------------------

tree::TerminalNode* MQL_Parser::ExprVarContext::VARIABLE() {
  return getToken(MQL_Parser::VARIABLE, 0);
}

tree::TerminalNode* MQL_Parser::ExprVarContext::KEY() {
  return getToken(MQL_Parser::KEY, 0);
}

MQL_Parser::ExprVarContext::ExprVarContext(AtomicExprContext *ctx) { copyFrom(ctx); }


antlrcpp::Any MQL_Parser::ExprVarContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitExprVar(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ExprParenthesisContext ------------------------------------------------------------------

tree::TerminalNode* MQL_Parser::ExprParenthesisContext::L_PAR() {
  return getToken(MQL_Parser::L_PAR, 0);
}

MQL_Parser::ConditionalOrExprContext* MQL_Parser::ExprParenthesisContext::conditionalOrExpr() {
  return getRuleContext<MQL_Parser::ConditionalOrExprContext>(0);
}

tree::TerminalNode* MQL_Parser::ExprParenthesisContext::R_PAR() {
  return getToken(MQL_Parser::R_PAR, 0);
}

MQL_Parser::ExprParenthesisContext::ExprParenthesisContext(AtomicExprContext *ctx) { copyFrom(ctx); }


antlrcpp::Any MQL_Parser::ExprParenthesisContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitExprParenthesis(this);
  else
    return visitor->visitChildren(this);
}
MQL_Parser::AtomicExprContext* MQL_Parser::atomicExpr() {
  AtomicExprContext *_localctx = _tracker.createInstance<AtomicExprContext>(_ctx, getState());
  enterRule(_localctx, 100, MQL_Parser::RuleAtomicExpr);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(592);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case MQL_Parser::VARIABLE: {
        _localctx = _tracker.createInstance<MQL_Parser::ExprVarContext>(_localctx);
        enterOuterAlt(_localctx, 1);
        setState(583);
        match(MQL_Parser::VARIABLE);
        setState(585);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == MQL_Parser::KEY) {
          setState(584);
          match(MQL_Parser::KEY);
        }
        break;
      }

      case MQL_Parser::K_FALSE:
      case MQL_Parser::K_TRUE:
      case MQL_Parser::STRING:
      case MQL_Parser::UNSIGNED_INTEGER:
      case MQL_Parser::UNSIGNED_FLOAT: {
        _localctx = _tracker.createInstance<MQL_Parser::ExprValueExprContext>(_localctx);
        enterOuterAlt(_localctx, 2);
        setState(587);
        valueExpr();
        break;
      }

      case MQL_Parser::L_PAR: {
        _localctx = _tracker.createInstance<MQL_Parser::ExprParenthesisContext>(_localctx);
        enterOuterAlt(_localctx, 3);
        setState(588);
        match(MQL_Parser::L_PAR);
        setState(589);
        conditionalOrExpr();
        setState(590);
        match(MQL_Parser::R_PAR);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ValueExprContext ------------------------------------------------------------------

MQL_Parser::ValueExprContext::ValueExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::ValueExprContext::UNSIGNED_INTEGER() {
  return getToken(MQL_Parser::UNSIGNED_INTEGER, 0);
}

tree::TerminalNode* MQL_Parser::ValueExprContext::UNSIGNED_FLOAT() {
  return getToken(MQL_Parser::UNSIGNED_FLOAT, 0);
}

tree::TerminalNode* MQL_Parser::ValueExprContext::STRING() {
  return getToken(MQL_Parser::STRING, 0);
}

MQL_Parser::BoolValueContext* MQL_Parser::ValueExprContext::boolValue() {
  return getRuleContext<MQL_Parser::BoolValueContext>(0);
}


size_t MQL_Parser::ValueExprContext::getRuleIndex() const {
  return MQL_Parser::RuleValueExpr;
}


antlrcpp::Any MQL_Parser::ValueExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitValueExpr(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::ValueExprContext* MQL_Parser::valueExpr() {
  ValueExprContext *_localctx = _tracker.createInstance<ValueExprContext>(_ctx, getState());
  enterRule(_localctx, 102, MQL_Parser::RuleValueExpr);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(598);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case MQL_Parser::UNSIGNED_INTEGER: {
        enterOuterAlt(_localctx, 1);
        setState(594);
        match(MQL_Parser::UNSIGNED_INTEGER);
        break;
      }

      case MQL_Parser::UNSIGNED_FLOAT: {
        enterOuterAlt(_localctx, 2);
        setState(595);
        match(MQL_Parser::UNSIGNED_FLOAT);
        break;
      }

      case MQL_Parser::STRING: {
        enterOuterAlt(_localctx, 3);
        setState(596);
        match(MQL_Parser::STRING);
        break;
      }

      case MQL_Parser::K_FALSE:
      case MQL_Parser::K_TRUE: {
        enterOuterAlt(_localctx, 4);
        setState(597);
        boolValue();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExprTypenameContext ------------------------------------------------------------------

MQL_Parser::ExprTypenameContext::ExprTypenameContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::ExprTypenameContext::K_NULL() {
  return getToken(MQL_Parser::K_NULL, 0);
}

tree::TerminalNode* MQL_Parser::ExprTypenameContext::K_STRING() {
  return getToken(MQL_Parser::K_STRING, 0);
}

tree::TerminalNode* MQL_Parser::ExprTypenameContext::K_BOOL() {
  return getToken(MQL_Parser::K_BOOL, 0);
}

tree::TerminalNode* MQL_Parser::ExprTypenameContext::K_INTEGER() {
  return getToken(MQL_Parser::K_INTEGER, 0);
}

tree::TerminalNode* MQL_Parser::ExprTypenameContext::K_FLOAT() {
  return getToken(MQL_Parser::K_FLOAT, 0);
}


size_t MQL_Parser::ExprTypenameContext::getRuleIndex() const {
  return MQL_Parser::RuleExprTypename;
}


antlrcpp::Any MQL_Parser::ExprTypenameContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitExprTypename(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::ExprTypenameContext* MQL_Parser::exprTypename() {
  ExprTypenameContext *_localctx = _tracker.createInstance<ExprTypenameContext>(_ctx, getState());
  enterRule(_localctx, 104, MQL_Parser::RuleExprTypename);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(600);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << MQL_Parser::K_BOOL)
      | (1ULL << MQL_Parser::K_INTEGER)
      | (1ULL << MQL_Parser::K_FLOAT)
      | (1ULL << MQL_Parser::K_NULL)
      | (1ULL << MQL_Parser::K_STRING))) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- KeywordContext ------------------------------------------------------------------

MQL_Parser::KeywordContext::KeywordContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_ALL() {
  return getToken(MQL_Parser::K_ALL, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_AND() {
  return getToken(MQL_Parser::K_AND, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_ANY() {
  return getToken(MQL_Parser::K_ANY, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_AVG() {
  return getToken(MQL_Parser::K_AVG, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_ASC() {
  return getToken(MQL_Parser::K_ASC, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_BY() {
  return getToken(MQL_Parser::K_BY, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_BOOL() {
  return getToken(MQL_Parser::K_BOOL, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_COUNT() {
  return getToken(MQL_Parser::K_COUNT, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_DESCRIBE() {
  return getToken(MQL_Parser::K_DESCRIBE, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_DESC() {
  return getToken(MQL_Parser::K_DESC, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_DISTINCT() {
  return getToken(MQL_Parser::K_DISTINCT, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_EDGE() {
  return getToken(MQL_Parser::K_EDGE, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_INSERT() {
  return getToken(MQL_Parser::K_INSERT, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_IS() {
  return getToken(MQL_Parser::K_IS, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_INTEGER() {
  return getToken(MQL_Parser::K_INTEGER, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_FLOAT() {
  return getToken(MQL_Parser::K_FLOAT, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_GROUP() {
  return getToken(MQL_Parser::K_GROUP, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_LABEL() {
  return getToken(MQL_Parser::K_LABEL, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_LIMIT() {
  return getToken(MQL_Parser::K_LIMIT, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_MAX() {
  return getToken(MQL_Parser::K_MAX, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_MATCH() {
  return getToken(MQL_Parser::K_MATCH, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_MIN() {
  return getToken(MQL_Parser::K_MIN, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_OPTIONAL() {
  return getToken(MQL_Parser::K_OPTIONAL, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_ORDER() {
  return getToken(MQL_Parser::K_ORDER, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_OR() {
  return getToken(MQL_Parser::K_OR, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_PROPERTY() {
  return getToken(MQL_Parser::K_PROPERTY, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_NOT() {
  return getToken(MQL_Parser::K_NOT, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_NULL() {
  return getToken(MQL_Parser::K_NULL, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_SET() {
  return getToken(MQL_Parser::K_SET, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_SUM() {
  return getToken(MQL_Parser::K_SUM, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_STRING() {
  return getToken(MQL_Parser::K_STRING, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_RETURN() {
  return getToken(MQL_Parser::K_RETURN, 0);
}

tree::TerminalNode* MQL_Parser::KeywordContext::K_WHERE() {
  return getToken(MQL_Parser::K_WHERE, 0);
}


size_t MQL_Parser::KeywordContext::getRuleIndex() const {
  return MQL_Parser::RuleKeyword;
}


antlrcpp::Any MQL_Parser::KeywordContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MQL_ParserVisitor*>(visitor))
    return parserVisitor->visitKeyword(this);
  else
    return visitor->visitChildren(this);
}

MQL_Parser::KeywordContext* MQL_Parser::keyword() {
  KeywordContext *_localctx = _tracker.createInstance<KeywordContext>(_ctx, getState());
  enterRule(_localctx, 106, MQL_Parser::RuleKeyword);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(602);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << MQL_Parser::K_ANY)
      | (1ULL << MQL_Parser::K_AND)
      | (1ULL << MQL_Parser::K_AVG)
      | (1ULL << MQL_Parser::K_ALL)
      | (1ULL << MQL_Parser::K_ASC)
      | (1ULL << MQL_Parser::K_BY)
      | (1ULL << MQL_Parser::K_BOOL)
      | (1ULL << MQL_Parser::K_COUNT)
      | (1ULL << MQL_Parser::K_DESCRIBE)
      | (1ULL << MQL_Parser::K_DESC)
      | (1ULL << MQL_Parser::K_DISTINCT)
      | (1ULL << MQL_Parser::K_EDGE)
      | (1ULL << MQL_Parser::K_INTEGER)
      | (1ULL << MQL_Parser::K_INSERT)
      | (1ULL << MQL_Parser::K_IS)
      | (1ULL << MQL_Parser::K_FLOAT)
      | (1ULL << MQL_Parser::K_GROUP)
      | (1ULL << MQL_Parser::K_LABEL)
      | (1ULL << MQL_Parser::K_LIMIT)
      | (1ULL << MQL_Parser::K_MAX)
      | (1ULL << MQL_Parser::K_MATCH)
      | (1ULL << MQL_Parser::K_MIN)
      | (1ULL << MQL_Parser::K_OPTIONAL)
      | (1ULL << MQL_Parser::K_ORDER)
      | (1ULL << MQL_Parser::K_OR)
      | (1ULL << MQL_Parser::K_PROPERTY)
      | (1ULL << MQL_Parser::K_NOT)
      | (1ULL << MQL_Parser::K_NULL)
      | (1ULL << MQL_Parser::K_SET)
      | (1ULL << MQL_Parser::K_SUM)
      | (1ULL << MQL_Parser::K_STRING)
      | (1ULL << MQL_Parser::K_RETURN)
      | (1ULL << MQL_Parser::K_WHERE))) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

// Static vars and initialization.
std::vector<dfa::DFA> MQL_Parser::_decisionToDFA;
atn::PredictionContextCache MQL_Parser::_sharedContextCache;

// We own the ATN which in turn owns the ATN states.
atn::ATN MQL_Parser::_atn;
std::vector<uint16_t> MQL_Parser::_serializedATN;

std::vector<std::string> MQL_Parser::_ruleNames = {
  "root", "matchQuery", "describeQuery", "describeFlag", "insertQuery", 
  "insertLabelList", "insertPropertyList", "insertEdgeList", "insertLabelElement", 
  "insertPropertyElement", "insertEdgeElement", "setStatement", "matchStatement", 
  "whereStatement", "groupByStatement", "orderByStatement", "returnStatement", 
  "setItem", "returnItem", "aggregateFunc", "orderByItem", "groupByItem", 
  "graphPattern", "optionalPattern", "basicPattern", "linearPattern", "path", 
  "pathAlternatives", "pathSequence", "pathAtom", "pathSuffix", "pathType", 
  "node", "fixedNode", "fixedNodeInside", "edge", "edgeInside", "varNode", 
  "properties", "property", "identifier", "boolValue", "numericValue", "value", 
  "conditionalOrExpr", "conditionalAndExpr", "comparisonExpr", "additiveExpr", 
  "multiplicativeExpr", "unaryExpr", "atomicExpr", "valueExpr", "exprTypename", 
  "keyword"
};

std::vector<std::string> MQL_Parser::_literalNames = {
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "'false'", 
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
  "", "", "'true'", "", "", "", "", "", "", "", "", "", "", "", "", "", 
  "'<='", "'>='", "'=='", "'!='", "'<'", "'>'", "'='", "'/'", "'|'", "'^'", 
  "'*'", "'%'", "'\u003F'", "'+'", "'-'", "'('", "')'", "'{'", "'}'", "'['", 
  "']'", "','", "':'"
};

std::vector<std::string> MQL_Parser::_symbolicNames = {
  "", "K_ANY", "K_AND", "K_AVG", "K_ALL", "K_ASC", "K_BY", "K_BOOL", "K_COUNT", 
  "K_DESCRIBE", "K_DESC", "K_DISTINCT", "K_EDGE", "K_INCOMING", "K_INTEGER", 
  "K_INSERT", "K_IS", "K_FALSE", "K_FLOAT", "K_GROUP", "K_LABEL", "K_LABELS", 
  "K_LIMIT", "K_MAX", "K_MATCH", "K_MIN", "K_OPTIONAL", "K_ORDER", "K_OR", 
  "K_OUTGOING", "K_PROPERTY", "K_PROPERTIES", "K_NOT", "K_NULL", "K_SET", 
  "K_SUM", "K_STRING", "K_RETURN", "K_TRUE", "K_WHERE", "TRUE_PROP", "FALSE_PROP", 
  "ANON_ID", "EDGE_ID", "KEY", "TYPE", "TYPE_VAR", "VARIABLE", "STRING", 
  "UNSIGNED_INTEGER", "UNSIGNED_FLOAT", "NAME", "LEQ", "GEQ", "EQ", "NEQ", 
  "LT", "GT", "SINGLE_EQ", "PATH_SEQUENCE", "PATH_ALTERNATIVE", "PATH_NEGATION", 
  "STAR", "PERCENT", "QUESTION_MARK", "PLUS", "MINUS", "L_PAR", "R_PAR", 
  "LCURLY_BRACKET", "RCURLY_BRACKET", "LSQUARE_BRACKET", "RSQUARE_BRACKET", 
  "COMMA", "COLON", "WHITE_SPACE", "SINGLE_LINE_COMMENT", "UNRECOGNIZED"
};

dfa::Vocabulary MQL_Parser::_vocabulary(_literalNames, _symbolicNames);

std::vector<std::string> MQL_Parser::_tokenNames;

MQL_Parser::Initializer::Initializer() {
	for (size_t i = 0; i < _symbolicNames.size(); ++i) {
		std::string name = _vocabulary.getLiteralName(i);
		if (name.empty()) {
			name = _vocabulary.getSymbolicName(i);
		}

		if (name.empty()) {
			_tokenNames.push_back("<INVALID>");
		} else {
      _tokenNames.push_back(name);
    }
	}

  static const uint16_t serializedATNSegment0[] = {
    0x3, 0x608b, 0xa72a, 0x8133, 0xb9ed, 0x417c, 0x3be7, 0x7786, 0x5964, 
       0x3, 0x4f, 0x25f, 0x4, 0x2, 0x9, 0x2, 0x4, 0x3, 0x9, 0x3, 0x4, 0x4, 
       0x9, 0x4, 0x4, 0x5, 0x9, 0x5, 0x4, 0x6, 0x9, 0x6, 0x4, 0x7, 0x9, 
       0x7, 0x4, 0x8, 0x9, 0x8, 0x4, 0x9, 0x9, 0x9, 0x4, 0xa, 0x9, 0xa, 
       0x4, 0xb, 0x9, 0xb, 0x4, 0xc, 0x9, 0xc, 0x4, 0xd, 0x9, 0xd, 0x4, 
       0xe, 0x9, 0xe, 0x4, 0xf, 0x9, 0xf, 0x4, 0x10, 0x9, 0x10, 0x4, 0x11, 
       0x9, 0x11, 0x4, 0x12, 0x9, 0x12, 0x4, 0x13, 0x9, 0x13, 0x4, 0x14, 
       0x9, 0x14, 0x4, 0x15, 0x9, 0x15, 0x4, 0x16, 0x9, 0x16, 0x4, 0x17, 
       0x9, 0x17, 0x4, 0x18, 0x9, 0x18, 0x4, 0x19, 0x9, 0x19, 0x4, 0x1a, 
       0x9, 0x1a, 0x4, 0x1b, 0x9, 0x1b, 0x4, 0x1c, 0x9, 0x1c, 0x4, 0x1d, 
       0x9, 0x1d, 0x4, 0x1e, 0x9, 0x1e, 0x4, 0x1f, 0x9, 0x1f, 0x4, 0x20, 
       0x9, 0x20, 0x4, 0x21, 0x9, 0x21, 0x4, 0x22, 0x9, 0x22, 0x4, 0x23, 
       0x9, 0x23, 0x4, 0x24, 0x9, 0x24, 0x4, 0x25, 0x9, 0x25, 0x4, 0x26, 
       0x9, 0x26, 0x4, 0x27, 0x9, 0x27, 0x4, 0x28, 0x9, 0x28, 0x4, 0x29, 
       0x9, 0x29, 0x4, 0x2a, 0x9, 0x2a, 0x4, 0x2b, 0x9, 0x2b, 0x4, 0x2c, 
       0x9, 0x2c, 0x4, 0x2d, 0x9, 0x2d, 0x4, 0x2e, 0x9, 0x2e, 0x4, 0x2f, 
       0x9, 0x2f, 0x4, 0x30, 0x9, 0x30, 0x4, 0x31, 0x9, 0x31, 0x4, 0x32, 
       0x9, 0x32, 0x4, 0x33, 0x9, 0x33, 0x4, 0x34, 0x9, 0x34, 0x4, 0x35, 
       0x9, 0x35, 0x4, 0x36, 0x9, 0x36, 0x4, 0x37, 0x9, 0x37, 0x3, 0x2, 
       0x5, 0x2, 0x70, 0xa, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 0x5, 0x2, 
       0x75, 0xa, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x3, 0x3, 0x3, 0x5, 0x3, 
       0x7b, 0xa, 0x3, 0x3, 0x3, 0x5, 0x3, 0x7e, 0xa, 0x3, 0x3, 0x3, 0x5, 
       0x3, 0x81, 0xa, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x4, 0x3, 0x4, 0x7, 
       0x4, 0x87, 0xa, 0x4, 0xc, 0x4, 0xe, 0x4, 0x8a, 0xb, 0x4, 0x3, 0x4, 
       0x3, 0x4, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x5, 0x5, 0x91, 0xa, 0x5, 
       0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x5, 0x6, 0x97, 0xa, 0x6, 
       0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x7, 0x7, 0x9d, 0xa, 0x7, 
       0xc, 0x7, 0xe, 0x7, 0xa0, 0xb, 0x7, 0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 
       0x3, 0x8, 0x7, 0x8, 0xa6, 0xa, 0x8, 0xc, 0x8, 0xe, 0x8, 0xa9, 0xb, 
       0x8, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x7, 0x9, 0xaf, 0xa, 
       0x9, 0xc, 0x9, 0xe, 0x9, 0xb2, 0xb, 0x9, 0x3, 0xa, 0x3, 0xa, 0x3, 
       0xa, 0x5, 0xa, 0xb7, 0xa, 0xa, 0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 0x3, 
       0xa, 0x3, 0xb, 0x3, 0xb, 0x3, 0xb, 0x3, 0xb, 0x3, 0xb, 0x3, 0xb, 
       0x3, 0xb, 0x3, 0xb, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 
       0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 
       0x3, 0xd, 0x7, 0xd, 0xd1, 0xa, 0xd, 0xc, 0xd, 0xe, 0xd, 0xd4, 0xb, 
       0xd, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 
       0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x7, 0x10, 
       0xe1, 0xa, 0x10, 0xc, 0x10, 0xe, 0x10, 0xe4, 0xb, 0x10, 0x3, 0x11, 
       0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 0x7, 0x11, 0xeb, 0xa, 
       0x11, 0xc, 0x11, 0xe, 0x11, 0xee, 0xb, 0x11, 0x3, 0x12, 0x3, 0x12, 
       0x5, 0x12, 0xf2, 0xa, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x7, 
       0x12, 0xf7, 0xa, 0x12, 0xc, 0x12, 0xe, 0x12, 0xfa, 0xb, 0x12, 0x3, 
       0x12, 0x3, 0x12, 0x5, 0x12, 0xfe, 0xa, 0x12, 0x3, 0x12, 0x3, 0x12, 
       0x5, 0x12, 0x102, 0xa, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x5, 
       0x12, 0x107, 0xa, 0x12, 0x5, 0x12, 0x109, 0xa, 0x12, 0x3, 0x13, 0x3, 
       0x13, 0x3, 0x13, 0x3, 0x13, 0x3, 0x14, 0x3, 0x14, 0x5, 0x14, 0x111, 
       0xa, 0x14, 0x3, 0x14, 0x3, 0x14, 0x3, 0x14, 0x3, 0x14, 0x5, 0x14, 
       0x117, 0xa, 0x14, 0x3, 0x14, 0x3, 0x14, 0x3, 0x14, 0x3, 0x14, 0x3, 
       0x14, 0x5, 0x14, 0x11e, 0xa, 0x14, 0x3, 0x14, 0x3, 0x14, 0x5, 0x14, 
       0x122, 0xa, 0x14, 0x3, 0x14, 0x5, 0x14, 0x125, 0xa, 0x14, 0x3, 0x14, 
       0x5, 0x14, 0x128, 0xa, 0x14, 0x3, 0x15, 0x3, 0x15, 0x3, 0x16, 0x3, 
       0x16, 0x5, 0x16, 0x12e, 0xa, 0x16, 0x3, 0x16, 0x5, 0x16, 0x131, 0xa, 
       0x16, 0x3, 0x16, 0x3, 0x16, 0x3, 0x16, 0x3, 0x16, 0x5, 0x16, 0x137, 
       0xa, 0x16, 0x3, 0x16, 0x3, 0x16, 0x5, 0x16, 0x13b, 0xa, 0x16, 0x3, 
       0x16, 0x3, 0x16, 0x3, 0x16, 0x5, 0x16, 0x140, 0xa, 0x16, 0x3, 0x16, 
       0x3, 0x16, 0x5, 0x16, 0x144, 0xa, 0x16, 0x3, 0x16, 0x3, 0x16, 0x5, 
       0x16, 0x148, 0xa, 0x16, 0x5, 0x16, 0x14a, 0xa, 0x16, 0x3, 0x17, 0x3, 
       0x17, 0x5, 0x17, 0x14e, 0xa, 0x17, 0x3, 0x18, 0x3, 0x18, 0x7, 0x18, 
       0x152, 0xa, 0x18, 0xc, 0x18, 0xe, 0x18, 0x155, 0xb, 0x18, 0x3, 0x19, 
       0x3, 0x19, 0x3, 0x19, 0x3, 0x19, 0x3, 0x19, 0x3, 0x1a, 0x3, 0x1a, 
       0x3, 0x1a, 0x7, 0x1a, 0x15f, 0xa, 0x1a, 0xc, 0x1a, 0xe, 0x1a, 0x162, 
       0xb, 0x1a, 0x3, 0x1b, 0x3, 0x1b, 0x3, 0x1b, 0x5, 0x1b, 0x167, 0xa, 
       0x1b, 0x3, 0x1b, 0x3, 0x1b, 0x7, 0x1b, 0x16b, 0xa, 0x1b, 0xc, 0x1b, 
       0xe, 0x1b, 0x16e, 0xb, 0x1b, 0x3, 0x1c, 0x3, 0x1c, 0x3, 0x1c, 0x5, 
       0x1c, 0x173, 0xa, 0x1c, 0x3, 0x1c, 0x5, 0x1c, 0x176, 0xa, 0x1c, 0x3, 
       0x1c, 0x3, 0x1c, 0x3, 0x1c, 0x3, 0x1c, 0x3, 0x1c, 0x3, 0x1c, 0x3, 
       0x1c, 0x5, 0x1c, 0x17f, 0xa, 0x1c, 0x3, 0x1c, 0x5, 0x1c, 0x182, 0xa, 
       0x1c, 0x3, 0x1c, 0x3, 0x1c, 0x3, 0x1c, 0x3, 0x1c, 0x3, 0x1c, 0x5, 
       0x1c, 0x189, 0xa, 0x1c, 0x3, 0x1d, 0x3, 0x1d, 0x3, 0x1d, 0x7, 0x1d, 
       0x18e, 0xa, 0x1d, 0xc, 0x1d, 0xe, 0x1d, 0x191, 0xb, 0x1d, 0x3, 0x1e, 
       0x3, 0x1e, 0x3, 0x1e, 0x7, 0x1e, 0x196, 0xa, 0x1e, 0xc, 0x1e, 0xe, 
       0x1e, 0x199, 0xb, 0x1e, 0x3, 0x1f, 0x5, 0x1f, 0x19c, 0xa, 0x1f, 0x3, 
       0x1f, 0x3, 0x1f, 0x5, 0x1f, 0x1a0, 0xa, 0x1f, 0x3, 0x1f, 0x5, 0x1f, 
       0x1a3, 0xa, 0x1f, 0x3, 0x1f, 0x3, 0x1f, 0x3, 0x1f, 0x3, 0x1f, 0x5, 
       0x1f, 0x1a9, 0xa, 0x1f, 0x5, 0x1f, 0x1ab, 0xa, 0x1f, 0x3, 0x20, 0x3, 
       0x20, 0x3, 0x20, 0x3, 0x20, 0x3, 0x20, 0x3, 0x20, 0x3, 0x20, 0x3, 
       0x20, 0x5, 0x20, 0x1b5, 0xa, 0x20, 0x3, 0x21, 0x3, 0x21, 0x3, 0x22, 
       0x3, 0x22, 0x5, 0x22, 0x1bb, 0xa, 0x22, 0x3, 0x23, 0x3, 0x23, 0x3, 
       0x23, 0x3, 0x23, 0x3, 0x24, 0x3, 0x24, 0x3, 0x24, 0x3, 0x24, 0x5, 
       0x24, 0x1c5, 0xa, 0x24, 0x3, 0x25, 0x3, 0x25, 0x3, 0x25, 0x5, 0x25, 
       0x1ca, 0xa, 0x25, 0x3, 0x25, 0x3, 0x25, 0x3, 0x25, 0x3, 0x25, 0x3, 
       0x25, 0x5, 0x25, 0x1d1, 0xa, 0x25, 0x3, 0x25, 0x5, 0x25, 0x1d4, 0xa, 
       0x25, 0x3, 0x26, 0x3, 0x26, 0x5, 0x26, 0x1d8, 0xa, 0x26, 0x3, 0x26, 
       0x5, 0x26, 0x1db, 0xa, 0x26, 0x3, 0x26, 0x5, 0x26, 0x1de, 0xa, 0x26, 
       0x3, 0x26, 0x3, 0x26, 0x3, 0x27, 0x3, 0x27, 0x5, 0x27, 0x1e4, 0xa, 
       0x27, 0x3, 0x27, 0x7, 0x27, 0x1e7, 0xa, 0x27, 0xc, 0x27, 0xe, 0x27, 
       0x1ea, 0xb, 0x27, 0x3, 0x27, 0x5, 0x27, 0x1ed, 0xa, 0x27, 0x3, 0x27, 
       0x3, 0x27, 0x3, 0x28, 0x3, 0x28, 0x3, 0x28, 0x3, 0x28, 0x7, 0x28, 
       0x1f5, 0xa, 0x28, 0xc, 0x28, 0xe, 0x28, 0x1f8, 0xb, 0x28, 0x3, 0x28, 
       0x3, 0x28, 0x3, 0x29, 0x3, 0x29, 0x3, 0x29, 0x3, 0x29, 0x3, 0x29, 
       0x5, 0x29, 0x201, 0xa, 0x29, 0x3, 0x2a, 0x3, 0x2a, 0x5, 0x2a, 0x205, 
       0xa, 0x2a, 0x3, 0x2b, 0x3, 0x2b, 0x3, 0x2c, 0x5, 0x2c, 0x20a, 0xa, 
       0x2c, 0x3, 0x2c, 0x3, 0x2c, 0x3, 0x2d, 0x3, 0x2d, 0x3, 0x2d, 0x5, 
       0x2d, 0x211, 0xa, 0x2d, 0x3, 0x2e, 0x3, 0x2e, 0x3, 0x2e, 0x7, 0x2e, 
       0x216, 0xa, 0x2e, 0xc, 0x2e, 0xe, 0x2e, 0x219, 0xb, 0x2e, 0x3, 0x2f, 
       0x3, 0x2f, 0x3, 0x2f, 0x7, 0x2f, 0x21e, 0xa, 0x2f, 0xc, 0x2f, 0xe, 
       0x2f, 0x221, 0xb, 0x2f, 0x3, 0x30, 0x3, 0x30, 0x3, 0x30, 0x5, 0x30, 
       0x226, 0xa, 0x30, 0x3, 0x30, 0x3, 0x30, 0x3, 0x30, 0x5, 0x30, 0x22b, 
       0xa, 0x30, 0x3, 0x30, 0x3, 0x30, 0x5, 0x30, 0x22f, 0xa, 0x30, 0x3, 
       0x31, 0x3, 0x31, 0x3, 0x31, 0x7, 0x31, 0x234, 0xa, 0x31, 0xc, 0x31, 
       0xe, 0x31, 0x237, 0xb, 0x31, 0x3, 0x32, 0x3, 0x32, 0x3, 0x32, 0x7, 
       0x32, 0x23c, 0xa, 0x32, 0xc, 0x32, 0xe, 0x32, 0x23f, 0xb, 0x32, 0x3, 
       0x33, 0x3, 0x33, 0x3, 0x33, 0x3, 0x33, 0x3, 0x33, 0x3, 0x33, 0x3, 
       0x33, 0x5, 0x33, 0x248, 0xa, 0x33, 0x3, 0x34, 0x3, 0x34, 0x5, 0x34, 
       0x24c, 0xa, 0x34, 0x3, 0x34, 0x3, 0x34, 0x3, 0x34, 0x3, 0x34, 0x3, 
       0x34, 0x5, 0x34, 0x253, 0xa, 0x34, 0x3, 0x35, 0x3, 0x35, 0x3, 0x35, 
       0x3, 0x35, 0x5, 0x35, 0x259, 0xa, 0x35, 0x3, 0x36, 0x3, 0x36, 0x3, 
       0x37, 0x3, 0x37, 0x3, 0x37, 0x2, 0x2, 0x38, 0x2, 0x4, 0x6, 0x8, 0xa, 
       0xc, 0xe, 0x10, 0x12, 0x14, 0x16, 0x18, 0x1a, 0x1c, 0x1e, 0x20, 0x22, 
       0x24, 0x26, 0x28, 0x2a, 0x2c, 0x2e, 0x30, 0x32, 0x34, 0x36, 0x38, 
       0x3a, 0x3c, 0x3e, 0x40, 0x42, 0x44, 0x46, 0x48, 0x4a, 0x4c, 0x4e, 
       0x50, 0x52, 0x54, 0x56, 0x58, 0x5a, 0x5c, 0x5e, 0x60, 0x62, 0x64, 
       0x66, 0x68, 0x6a, 0x6c, 0x2, 0xf, 0x6, 0x2, 0xf, 0xf, 0x17, 0x17, 
       0x1f, 0x1f, 0x21, 0x21, 0x6, 0x2, 0x5, 0x5, 0x19, 0x19, 0x1b, 0x1b, 
       0x25, 0x25, 0x4, 0x2, 0x7, 0x7, 0xc, 0xc, 0x4, 0x2, 0x3, 0x3, 0x6, 
       0x6, 0x4, 0x2, 0x2d, 0x2d, 0x31, 0x31, 0x3, 0x2, 0x2f, 0x30, 0x4, 
       0x2, 0x13, 0x13, 0x28, 0x28, 0x3, 0x2, 0x43, 0x44, 0x3, 0x2, 0x33, 
       0x34, 0x3, 0x2, 0x36, 0x3b, 0x4, 0x2, 0x3d, 0x3d, 0x40, 0x41, 0x7, 
       0x2, 0x9, 0x9, 0x10, 0x10, 0x14, 0x14, 0x23, 0x23, 0x26, 0x26, 0x9, 
       0x2, 0x3, 0xe, 0x10, 0x12, 0x14, 0x16, 0x18, 0x1e, 0x20, 0x20, 0x22, 
       0x27, 0x29, 0x29, 0x2, 0x287, 0x2, 0x6f, 0x3, 0x2, 0x2, 0x2, 0x4, 
       0x78, 0x3, 0x2, 0x2, 0x2, 0x6, 0x84, 0x3, 0x2, 0x2, 0x2, 0x8, 0x8d, 
       0x3, 0x2, 0x2, 0x2, 0xa, 0x92, 0x3, 0x2, 0x2, 0x2, 0xc, 0x98, 0x3, 
       0x2, 0x2, 0x2, 0xe, 0xa1, 0x3, 0x2, 0x2, 0x2, 0x10, 0xaa, 0x3, 0x2, 
       0x2, 0x2, 0x12, 0xb3, 0x3, 0x2, 0x2, 0x2, 0x14, 0xbc, 0x3, 0x2, 0x2, 
       0x2, 0x16, 0xc4, 0x3, 0x2, 0x2, 0x2, 0x18, 0xcc, 0x3, 0x2, 0x2, 0x2, 
       0x1a, 0xd5, 0x3, 0x2, 0x2, 0x2, 0x1c, 0xd8, 0x3, 0x2, 0x2, 0x2, 0x1e, 
       0xdb, 0x3, 0x2, 0x2, 0x2, 0x20, 0xe5, 0x3, 0x2, 0x2, 0x2, 0x22, 0x108, 
       0x3, 0x2, 0x2, 0x2, 0x24, 0x10a, 0x3, 0x2, 0x2, 0x2, 0x26, 0x127, 
       0x3, 0x2, 0x2, 0x2, 0x28, 0x129, 0x3, 0x2, 0x2, 0x2, 0x2a, 0x149, 
       0x3, 0x2, 0x2, 0x2, 0x2c, 0x14b, 0x3, 0x2, 0x2, 0x2, 0x2e, 0x14f, 
       0x3, 0x2, 0x2, 0x2, 0x30, 0x156, 0x3, 0x2, 0x2, 0x2, 0x32, 0x15b, 
       0x3, 0x2, 0x2, 0x2, 0x34, 0x163, 0x3, 0x2, 0x2, 0x2, 0x36, 0x188, 
       0x3, 0x2, 0x2, 0x2, 0x38, 0x18a, 0x3, 0x2, 0x2, 0x2, 0x3a, 0x192, 
       0x3, 0x2, 0x2, 0x2, 0x3c, 0x1aa, 0x3, 0x2, 0x2, 0x2, 0x3e, 0x1b4, 
       0x3, 0x2, 0x2, 0x2, 0x40, 0x1b6, 0x3, 0x2, 0x2, 0x2, 0x42, 0x1ba, 
       0x3, 0x2, 0x2, 0x2, 0x44, 0x1bc, 0x3, 0x2, 0x2, 0x2, 0x46, 0x1c4, 
       0x3, 0x2, 0x2, 0x2, 0x48, 0x1d3, 0x3, 0x2, 0x2, 0x2, 0x4a, 0x1d5, 
       0x3, 0x2, 0x2, 0x2, 0x4c, 0x1e1, 0x3, 0x2, 0x2, 0x2, 0x4e, 0x1f0, 
       0x3, 0x2, 0x2, 0x2, 0x50, 0x1fb, 0x3, 0x2, 0x2, 0x2, 0x52, 0x204, 
       0x3, 0x2, 0x2, 0x2, 0x54, 0x206, 0x3, 0x2, 0x2, 0x2, 0x56, 0x209, 
       0x3, 0x2, 0x2, 0x2, 0x58, 0x210, 0x3, 0x2, 0x2, 0x2, 0x5a, 0x212, 
       0x3, 0x2, 0x2, 0x2, 0x5c, 0x21a, 0x3, 0x2, 0x2, 0x2, 0x5e, 0x22e, 
       0x3, 0x2, 0x2, 0x2, 0x60, 0x230, 0x3, 0x2, 0x2, 0x2, 0x62, 0x238, 
       0x3, 0x2, 0x2, 0x2, 0x64, 0x247, 0x3, 0x2, 0x2, 0x2, 0x66, 0x252, 
       0x3, 0x2, 0x2, 0x2, 0x68, 0x258, 0x3, 0x2, 0x2, 0x2, 0x6a, 0x25a, 
       0x3, 0x2, 0x2, 0x2, 0x6c, 0x25c, 0x3, 0x2, 0x2, 0x2, 0x6e, 0x70, 
       0x5, 0x18, 0xd, 0x2, 0x6f, 0x6e, 0x3, 0x2, 0x2, 0x2, 0x6f, 0x70, 
       0x3, 0x2, 0x2, 0x2, 0x70, 0x74, 0x3, 0x2, 0x2, 0x2, 0x71, 0x75, 0x5, 
       0x4, 0x3, 0x2, 0x72, 0x75, 0x5, 0x6, 0x4, 0x2, 0x73, 0x75, 0x5, 0xa, 
       0x6, 0x2, 0x74, 0x71, 0x3, 0x2, 0x2, 0x2, 0x74, 0x72, 0x3, 0x2, 0x2, 
       0x2, 0x74, 0x73, 0x3, 0x2, 0x2, 0x2, 0x75, 0x76, 0x3, 0x2, 0x2, 0x2, 
       0x76, 0x77, 0x7, 0x2, 0x2, 0x3, 0x77, 0x3, 0x3, 0x2, 0x2, 0x2, 0x78, 
       0x7a, 0x5, 0x1a, 0xe, 0x2, 0x79, 0x7b, 0x5, 0x1c, 0xf, 0x2, 0x7a, 
       0x79, 0x3, 0x2, 0x2, 0x2, 0x7a, 0x7b, 0x3, 0x2, 0x2, 0x2, 0x7b, 0x7d, 
       0x3, 0x2, 0x2, 0x2, 0x7c, 0x7e, 0x5, 0x1e, 0x10, 0x2, 0x7d, 0x7c, 
       0x3, 0x2, 0x2, 0x2, 0x7d, 0x7e, 0x3, 0x2, 0x2, 0x2, 0x7e, 0x80, 0x3, 
       0x2, 0x2, 0x2, 0x7f, 0x81, 0x5, 0x20, 0x11, 0x2, 0x80, 0x7f, 0x3, 
       0x2, 0x2, 0x2, 0x80, 0x81, 0x3, 0x2, 0x2, 0x2, 0x81, 0x82, 0x3, 0x2, 
       0x2, 0x2, 0x82, 0x83, 0x5, 0x22, 0x12, 0x2, 0x83, 0x5, 0x3, 0x2, 
       0x2, 0x2, 0x84, 0x88, 0x7, 0xb, 0x2, 0x2, 0x85, 0x87, 0x5, 0x8, 0x5, 
       0x2, 0x86, 0x85, 0x3, 0x2, 0x2, 0x2, 0x87, 0x8a, 0x3, 0x2, 0x2, 0x2, 
       0x88, 0x86, 0x3, 0x2, 0x2, 0x2, 0x88, 0x89, 0x3, 0x2, 0x2, 0x2, 0x89, 
       0x8b, 0x3, 0x2, 0x2, 0x2, 0x8a, 0x88, 0x3, 0x2, 0x2, 0x2, 0x8b, 0x8c, 
       0x5, 0x46, 0x24, 0x2, 0x8c, 0x7, 0x3, 0x2, 0x2, 0x2, 0x8d, 0x90, 
       0x9, 0x2, 0x2, 0x2, 0x8e, 0x8f, 0x7, 0x18, 0x2, 0x2, 0x8f, 0x91, 
       0x7, 0x33, 0x2, 0x2, 0x90, 0x8e, 0x3, 0x2, 0x2, 0x2, 0x90, 0x91, 
       0x3, 0x2, 0x2, 0x2, 0x91, 0x9, 0x3, 0x2, 0x2, 0x2, 0x92, 0x96, 0x7, 
       0x11, 0x2, 0x2, 0x93, 0x97, 0x5, 0xc, 0x7, 0x2, 0x94, 0x97, 0x5, 
       0xe, 0x8, 0x2, 0x95, 0x97, 0x5, 0x10, 0x9, 0x2, 0x96, 0x93, 0x3, 
       0x2, 0x2, 0x2, 0x96, 0x94, 0x3, 0x2, 0x2, 0x2, 0x96, 0x95, 0x3, 0x2, 
       0x2, 0x2, 0x97, 0xb, 0x3, 0x2, 0x2, 0x2, 0x98, 0x99, 0x7, 0x16, 0x2, 
       0x2, 0x99, 0x9e, 0x5, 0x12, 0xa, 0x2, 0x9a, 0x9b, 0x7, 0x4b, 0x2, 
       0x2, 0x9b, 0x9d, 0x5, 0x12, 0xa, 0x2, 0x9c, 0x9a, 0x3, 0x2, 0x2, 
       0x2, 0x9d, 0xa0, 0x3, 0x2, 0x2, 0x2, 0x9e, 0x9c, 0x3, 0x2, 0x2, 0x2, 
       0x9e, 0x9f, 0x3, 0x2, 0x2, 0x2, 0x9f, 0xd, 0x3, 0x2, 0x2, 0x2, 0xa0, 
       0x9e, 0x3, 0x2, 0x2, 0x2, 0xa1, 0xa2, 0x7, 0x20, 0x2, 0x2, 0xa2, 
       0xa7, 0x5, 0x14, 0xb, 0x2, 0xa3, 0xa4, 0x7, 0x4b, 0x2, 0x2, 0xa4, 
       0xa6, 0x5, 0x14, 0xb, 0x2, 0xa5, 0xa3, 0x3, 0x2, 0x2, 0x2, 0xa6, 
       0xa9, 0x3, 0x2, 0x2, 0x2, 0xa7, 0xa5, 0x3, 0x2, 0x2, 0x2, 0xa7, 0xa8, 
       0x3, 0x2, 0x2, 0x2, 0xa8, 0xf, 0x3, 0x2, 0x2, 0x2, 0xa9, 0xa7, 0x3, 
       0x2, 0x2, 0x2, 0xaa, 0xab, 0x7, 0xe, 0x2, 0x2, 0xab, 0xb0, 0x5, 0x16, 
       0xc, 0x2, 0xac, 0xad, 0x7, 0x4b, 0x2, 0x2, 0xad, 0xaf, 0x5, 0x16, 
       0xc, 0x2, 0xae, 0xac, 0x3, 0x2, 0x2, 0x2, 0xaf, 0xb2, 0x3, 0x2, 0x2, 
       0x2, 0xb0, 0xae, 0x3, 0x2, 0x2, 0x2, 0xb0, 0xb1, 0x3, 0x2, 0x2, 0x2, 
       0xb1, 0x11, 0x3, 0x2, 0x2, 0x2, 0xb2, 0xb0, 0x3, 0x2, 0x2, 0x2, 0xb3, 
       0xb6, 0x7, 0x45, 0x2, 0x2, 0xb4, 0xb7, 0x5, 0x52, 0x2a, 0x2, 0xb5, 
       0xb7, 0x7, 0x2c, 0x2, 0x2, 0xb6, 0xb4, 0x3, 0x2, 0x2, 0x2, 0xb6, 
       0xb5, 0x3, 0x2, 0x2, 0x2, 0xb7, 0xb8, 0x3, 0x2, 0x2, 0x2, 0xb8, 0xb9, 
       0x7, 0x4b, 0x2, 0x2, 0xb9, 0xba, 0x7, 0x32, 0x2, 0x2, 0xba, 0xbb, 
       0x7, 0x46, 0x2, 0x2, 0xbb, 0x13, 0x3, 0x2, 0x2, 0x2, 0xbc, 0xbd, 
       0x7, 0x45, 0x2, 0x2, 0xbd, 0xbe, 0x5, 0x46, 0x24, 0x2, 0xbe, 0xbf, 
       0x7, 0x4b, 0x2, 0x2, 0xbf, 0xc0, 0x7, 0x32, 0x2, 0x2, 0xc0, 0xc1, 
       0x7, 0x4b, 0x2, 0x2, 0xc1, 0xc2, 0x5, 0x58, 0x2d, 0x2, 0xc2, 0xc3, 
       0x7, 0x46, 0x2, 0x2, 0xc3, 0x15, 0x3, 0x2, 0x2, 0x2, 0xc4, 0xc5, 
       0x7, 0x45, 0x2, 0x2, 0xc5, 0xc6, 0x5, 0x46, 0x24, 0x2, 0xc6, 0xc7, 
       0x7, 0x4b, 0x2, 0x2, 0xc7, 0xc8, 0x5, 0x46, 0x24, 0x2, 0xc8, 0xc9, 
       0x7, 0x4b, 0x2, 0x2, 0xc9, 0xca, 0x5, 0x52, 0x2a, 0x2, 0xca, 0xcb, 
       0x7, 0x46, 0x2, 0x2, 0xcb, 0x17, 0x3, 0x2, 0x2, 0x2, 0xcc, 0xcd, 
       0x7, 0x24, 0x2, 0x2, 0xcd, 0xd2, 0x5, 0x24, 0x13, 0x2, 0xce, 0xcf, 
       0x7, 0x4b, 0x2, 0x2, 0xcf, 0xd1, 0x5, 0x24, 0x13, 0x2, 0xd0, 0xce, 
       0x3, 0x2, 0x2, 0x2, 0xd1, 0xd4, 0x3, 0x2, 0x2, 0x2, 0xd2, 0xd0, 0x3, 
       0x2, 0x2, 0x2, 0xd2, 0xd3, 0x3, 0x2, 0x2, 0x2, 0xd3, 0x19, 0x3, 0x2, 
       0x2, 0x2, 0xd4, 0xd2, 0x3, 0x2, 0x2, 0x2, 0xd5, 0xd6, 0x7, 0x1a, 
       0x2, 0x2, 0xd6, 0xd7, 0x5, 0x2e, 0x18, 0x2, 0xd7, 0x1b, 0x3, 0x2, 
       0x2, 0x2, 0xd8, 0xd9, 0x7, 0x29, 0x2, 0x2, 0xd9, 0xda, 0x5, 0x5a, 
       0x2e, 0x2, 0xda, 0x1d, 0x3, 0x2, 0x2, 0x2, 0xdb, 0xdc, 0x7, 0x15, 
       0x2, 0x2, 0xdc, 0xdd, 0x7, 0x8, 0x2, 0x2, 0xdd, 0xe2, 0x5, 0x2c, 
       0x17, 0x2, 0xde, 0xdf, 0x7, 0x4b, 0x2, 0x2, 0xdf, 0xe1, 0x5, 0x2c, 
       0x17, 0x2, 0xe0, 0xde, 0x3, 0x2, 0x2, 0x2, 0xe1, 0xe4, 0x3, 0x2, 
       0x2, 0x2, 0xe2, 0xe0, 0x3, 0x2, 0x2, 0x2, 0xe2, 0xe3, 0x3, 0x2, 0x2, 
       0x2, 0xe3, 0x1f, 0x3, 0x2, 0x2, 0x2, 0xe4, 0xe2, 0x3, 0x2, 0x2, 0x2, 
       0xe5, 0xe6, 0x7, 0x1d, 0x2, 0x2, 0xe6, 0xe7, 0x7, 0x8, 0x2, 0x2, 
       0xe7, 0xec, 0x5, 0x2a, 0x16, 0x2, 0xe8, 0xe9, 0x7, 0x4b, 0x2, 0x2, 
       0xe9, 0xeb, 0x5, 0x2a, 0x16, 0x2, 0xea, 0xe8, 0x3, 0x2, 0x2, 0x2, 
       0xeb, 0xee, 0x3, 0x2, 0x2, 0x2, 0xec, 0xea, 0x3, 0x2, 0x2, 0x2, 0xec, 
       0xed, 0x3, 0x2, 0x2, 0x2, 0xed, 0x21, 0x3, 0x2, 0x2, 0x2, 0xee, 0xec, 
       0x3, 0x2, 0x2, 0x2, 0xef, 0xf1, 0x7, 0x27, 0x2, 0x2, 0xf0, 0xf2, 
       0x7, 0xd, 0x2, 0x2, 0xf1, 0xf0, 0x3, 0x2, 0x2, 0x2, 0xf1, 0xf2, 0x3, 
       0x2, 0x2, 0x2, 0xf2, 0xf3, 0x3, 0x2, 0x2, 0x2, 0xf3, 0xf8, 0x5, 0x26, 
       0x14, 0x2, 0xf4, 0xf5, 0x7, 0x4b, 0x2, 0x2, 0xf5, 0xf7, 0x5, 0x26, 
       0x14, 0x2, 0xf6, 0xf4, 0x3, 0x2, 0x2, 0x2, 0xf7, 0xfa, 0x3, 0x2, 
       0x2, 0x2, 0xf8, 0xf6, 0x3, 0x2, 0x2, 0x2, 0xf8, 0xf9, 0x3, 0x2, 0x2, 
       0x2, 0xf9, 0xfd, 0x3, 0x2, 0x2, 0x2, 0xfa, 0xf8, 0x3, 0x2, 0x2, 0x2, 
       0xfb, 0xfc, 0x7, 0x18, 0x2, 0x2, 0xfc, 0xfe, 0x7, 0x33, 0x2, 0x2, 
       0xfd, 0xfb, 0x3, 0x2, 0x2, 0x2, 0xfd, 0xfe, 0x3, 0x2, 0x2, 0x2, 0xfe, 
       0x109, 0x3, 0x2, 0x2, 0x2, 0xff, 0x101, 0x7, 0x27, 0x2, 0x2, 0x100, 
       0x102, 0x7, 0xd, 0x2, 0x2, 0x101, 0x100, 0x3, 0x2, 0x2, 0x2, 0x101, 
       0x102, 0x3, 0x2, 0x2, 0x2, 0x102, 0x103, 0x3, 0x2, 0x2, 0x2, 0x103, 
       0x106, 0x7, 0x40, 0x2, 0x2, 0x104, 0x105, 0x7, 0x18, 0x2, 0x2, 0x105, 
       0x107, 0x7, 0x33, 0x2, 0x2, 0x106, 0x104, 0x3, 0x2, 0x2, 0x2, 0x106, 
       0x107, 0x3, 0x2, 0x2, 0x2, 0x107, 0x109, 0x3, 0x2, 0x2, 0x2, 0x108, 
       0xef, 0x3, 0x2, 0x2, 0x2, 0x108, 0xff, 0x3, 0x2, 0x2, 0x2, 0x109, 
       0x23, 0x3, 0x2, 0x2, 0x2, 0x10a, 0x10b, 0x7, 0x31, 0x2, 0x2, 0x10b, 
       0x10c, 0x7, 0x3c, 0x2, 0x2, 0x10c, 0x10d, 0x5, 0x46, 0x24, 0x2, 0x10d, 
       0x25, 0x3, 0x2, 0x2, 0x2, 0x10e, 0x110, 0x7, 0x31, 0x2, 0x2, 0x10f, 
       0x111, 0x7, 0x2e, 0x2, 0x2, 0x110, 0x10f, 0x3, 0x2, 0x2, 0x2, 0x110, 
       0x111, 0x3, 0x2, 0x2, 0x2, 0x111, 0x128, 0x3, 0x2, 0x2, 0x2, 0x112, 
       0x113, 0x5, 0x28, 0x15, 0x2, 0x113, 0x114, 0x7, 0x45, 0x2, 0x2, 0x114, 
       0x116, 0x7, 0x31, 0x2, 0x2, 0x115, 0x117, 0x7, 0x2e, 0x2, 0x2, 0x116, 
       0x115, 0x3, 0x2, 0x2, 0x2, 0x116, 0x117, 0x3, 0x2, 0x2, 0x2, 0x117, 
       0x118, 0x3, 0x2, 0x2, 0x2, 0x118, 0x119, 0x7, 0x46, 0x2, 0x2, 0x119, 
       0x128, 0x3, 0x2, 0x2, 0x2, 0x11a, 0x11b, 0x7, 0xa, 0x2, 0x2, 0x11b, 
       0x11d, 0x7, 0x45, 0x2, 0x2, 0x11c, 0x11e, 0x7, 0xd, 0x2, 0x2, 0x11d, 
       0x11c, 0x3, 0x2, 0x2, 0x2, 0x11d, 0x11e, 0x3, 0x2, 0x2, 0x2, 0x11e, 
       0x124, 0x3, 0x2, 0x2, 0x2, 0x11f, 0x121, 0x7, 0x31, 0x2, 0x2, 0x120, 
       0x122, 0x7, 0x2e, 0x2, 0x2, 0x121, 0x120, 0x3, 0x2, 0x2, 0x2, 0x121, 
       0x122, 0x3, 0x2, 0x2, 0x2, 0x122, 0x125, 0x3, 0x2, 0x2, 0x2, 0x123, 
       0x125, 0x7, 0x40, 0x2, 0x2, 0x124, 0x11f, 0x3, 0x2, 0x2, 0x2, 0x124, 
       0x123, 0x3, 0x2, 0x2, 0x2, 0x125, 0x126, 0x3, 0x2, 0x2, 0x2, 0x126, 
       0x128, 0x7, 0x46, 0x2, 0x2, 0x127, 0x10e, 0x3, 0x2, 0x2, 0x2, 0x127, 
       0x112, 0x3, 0x2, 0x2, 0x2, 0x127, 0x11a, 0x3, 0x2, 0x2, 0x2, 0x128, 
       0x27, 0x3, 0x2, 0x2, 0x2, 0x129, 0x12a, 0x9, 0x3, 0x2, 0x2, 0x12a, 
       0x29, 0x3, 0x2, 0x2, 0x2, 0x12b, 0x12d, 0x7, 0x31, 0x2, 0x2, 0x12c, 
       0x12e, 0x7, 0x2e, 0x2, 0x2, 0x12d, 0x12c, 0x3, 0x2, 0x2, 0x2, 0x12d, 
       0x12e, 0x3, 0x2, 0x2, 0x2, 0x12e, 0x130, 0x3, 0x2, 0x2, 0x2, 0x12f, 
       0x131, 0x9, 0x4, 0x2, 0x2, 0x130, 0x12f, 0x3, 0x2, 0x2, 0x2, 0x130, 
       0x131, 0x3, 0x2, 0x2, 0x2, 0x131, 0x14a, 0x3, 0x2, 0x2, 0x2, 0x132, 
       0x133, 0x5, 0x28, 0x15, 0x2, 0x133, 0x134, 0x7, 0x45, 0x2, 0x2, 0x134, 
       0x136, 0x7, 0x31, 0x2, 0x2, 0x135, 0x137, 0x7, 0x2e, 0x2, 0x2, 0x136, 
       0x135, 0x3, 0x2, 0x2, 0x2, 0x136, 0x137, 0x3, 0x2, 0x2, 0x2, 0x137, 
       0x138, 0x3, 0x2, 0x2, 0x2, 0x138, 0x13a, 0x7, 0x46, 0x2, 0x2, 0x139, 
       0x13b, 0x9, 0x4, 0x2, 0x2, 0x13a, 0x139, 0x3, 0x2, 0x2, 0x2, 0x13a, 
       0x13b, 0x3, 0x2, 0x2, 0x2, 0x13b, 0x14a, 0x3, 0x2, 0x2, 0x2, 0x13c, 
       0x13d, 0x7, 0xa, 0x2, 0x2, 0x13d, 0x13f, 0x7, 0x45, 0x2, 0x2, 0x13e, 
       0x140, 0x7, 0xd, 0x2, 0x2, 0x13f, 0x13e, 0x3, 0x2, 0x2, 0x2, 0x13f, 
       0x140, 0x3, 0x2, 0x2, 0x2, 0x140, 0x141, 0x3, 0x2, 0x2, 0x2, 0x141, 
       0x143, 0x7, 0x31, 0x2, 0x2, 0x142, 0x144, 0x7, 0x2e, 0x2, 0x2, 0x143, 
       0x142, 0x3, 0x2, 0x2, 0x2, 0x143, 0x144, 0x3, 0x2, 0x2, 0x2, 0x144, 
       0x145, 0x3, 0x2, 0x2, 0x2, 0x145, 0x147, 0x7, 0x46, 0x2, 0x2, 0x146, 
       0x148, 0x9, 0x4, 0x2, 0x2, 0x147, 0x146, 0x3, 0x2, 0x2, 0x2, 0x147, 
       0x148, 0x3, 0x2, 0x2, 0x2, 0x148, 0x14a, 0x3, 0x2, 0x2, 0x2, 0x149, 
       0x12b, 0x3, 0x2, 0x2, 0x2, 0x149, 0x132, 0x3, 0x2, 0x2, 0x2, 0x149, 
       0x13c, 0x3, 0x2, 0x2, 0x2, 0x14a, 0x2b, 0x3, 0x2, 0x2, 0x2, 0x14b, 
       0x14d, 0x7, 0x31, 0x2, 0x2, 0x14c, 0x14e, 0x7, 0x2e, 0x2, 0x2, 0x14d, 
       0x14c, 0x3, 0x2, 0x2, 0x2, 0x14d, 0x14e, 0x3, 0x2, 0x2, 0x2, 0x14e, 
       0x2d, 0x3, 0x2, 0x2, 0x2, 0x14f, 0x153, 0x5, 0x32, 0x1a, 0x2, 0x150, 
       0x152, 0x5, 0x30, 0x19, 0x2, 0x151, 0x150, 0x3, 0x2, 0x2, 0x2, 0x152, 
       0x155, 0x3, 0x2, 0x2, 0x2, 0x153, 0x151, 0x3, 0x2, 0x2, 0x2, 0x153, 
       0x154, 0x3, 0x2, 0x2, 0x2, 0x154, 0x2f, 0x3, 0x2, 0x2, 0x2, 0x155, 
       0x153, 0x3, 0x2, 0x2, 0x2, 0x156, 0x157, 0x7, 0x1c, 0x2, 0x2, 0x157, 
       0x158, 0x7, 0x47, 0x2, 0x2, 0x158, 0x159, 0x5, 0x2e, 0x18, 0x2, 0x159, 
       0x15a, 0x7, 0x48, 0x2, 0x2, 0x15a, 0x31, 0x3, 0x2, 0x2, 0x2, 0x15b, 
       0x160, 0x5, 0x34, 0x1b, 0x2, 0x15c, 0x15d, 0x7, 0x4b, 0x2, 0x2, 0x15d, 
       0x15f, 0x5, 0x34, 0x1b, 0x2, 0x15e, 0x15c, 0x3, 0x2, 0x2, 0x2, 0x15f, 
       0x162, 0x3, 0x2, 0x2, 0x2, 0x160, 0x15e, 0x3, 0x2, 0x2, 0x2, 0x160, 
       0x161, 0x3, 0x2, 0x2, 0x2, 0x161, 0x33, 0x3, 0x2, 0x2, 0x2, 0x162, 
       0x160, 0x3, 0x2, 0x2, 0x2, 0x163, 0x16c, 0x5, 0x42, 0x22, 0x2, 0x164, 
       0x167, 0x5, 0x48, 0x25, 0x2, 0x165, 0x167, 0x5, 0x36, 0x1c, 0x2, 
       0x166, 0x164, 0x3, 0x2, 0x2, 0x2, 0x166, 0x165, 0x3, 0x2, 0x2, 0x2, 
       0x167, 0x168, 0x3, 0x2, 0x2, 0x2, 0x168, 0x169, 0x5, 0x42, 0x22, 
       0x2, 0x169, 0x16b, 0x3, 0x2, 0x2, 0x2, 0x16a, 0x166, 0x3, 0x2, 0x2, 
       0x2, 0x16b, 0x16e, 0x3, 0x2, 0x2, 0x2, 0x16c, 0x16a, 0x3, 0x2, 0x2, 
       0x2, 0x16c, 0x16d, 0x3, 0x2, 0x2, 0x2, 0x16d, 0x35, 0x3, 0x2, 0x2, 
       0x2, 0x16e, 0x16c, 0x3, 0x2, 0x2, 0x2, 0x16f, 0x170, 0x7, 0x36, 0x2, 
       0x2, 0x170, 0x172, 0x7, 0x49, 0x2, 0x2, 0x171, 0x173, 0x5, 0x40, 
       0x21, 0x2, 0x172, 0x171, 0x3, 0x2, 0x2, 0x2, 0x172, 0x173, 0x3, 0x2, 
       0x2, 0x2, 0x173, 0x175, 0x3, 0x2, 0x2, 0x2, 0x174, 0x176, 0x7, 0x31, 
       0x2, 0x2, 0x175, 0x174, 0x3, 0x2, 0x2, 0x2, 0x175, 0x176, 0x3, 0x2, 
       0x2, 0x2, 0x176, 0x177, 0x3, 0x2, 0x2, 0x2, 0x177, 0x178, 0x5, 0x38, 
       0x1d, 0x2, 0x178, 0x179, 0x7, 0x4a, 0x2, 0x2, 0x179, 0x17a, 0x7, 
       0x3c, 0x2, 0x2, 0x17a, 0x189, 0x3, 0x2, 0x2, 0x2, 0x17b, 0x17c, 0x7, 
       0x3c, 0x2, 0x2, 0x17c, 0x17e, 0x7, 0x49, 0x2, 0x2, 0x17d, 0x17f, 
       0x5, 0x40, 0x21, 0x2, 0x17e, 0x17d, 0x3, 0x2, 0x2, 0x2, 0x17e, 0x17f, 
       0x3, 0x2, 0x2, 0x2, 0x17f, 0x181, 0x3, 0x2, 0x2, 0x2, 0x180, 0x182, 
       0x7, 0x31, 0x2, 0x2, 0x181, 0x180, 0x3, 0x2, 0x2, 0x2, 0x181, 0x182, 
       0x3, 0x2, 0x2, 0x2, 0x182, 0x183, 0x3, 0x2, 0x2, 0x2, 0x183, 0x184, 
       0x5, 0x38, 0x1d, 0x2, 0x184, 0x185, 0x7, 0x4a, 0x2, 0x2, 0x185, 0x186, 
       0x7, 0x3c, 0x2, 0x2, 0x186, 0x187, 0x7, 0x3b, 0x2, 0x2, 0x187, 0x189, 
       0x3, 0x2, 0x2, 0x2, 0x188, 0x16f, 0x3, 0x2, 0x2, 0x2, 0x188, 0x17b, 
       0x3, 0x2, 0x2, 0x2, 0x189, 0x37, 0x3, 0x2, 0x2, 0x2, 0x18a, 0x18f, 
       0x5, 0x3a, 0x1e, 0x2, 0x18b, 0x18c, 0x7, 0x3e, 0x2, 0x2, 0x18c, 0x18e, 
       0x5, 0x3a, 0x1e, 0x2, 0x18d, 0x18b, 0x3, 0x2, 0x2, 0x2, 0x18e, 0x191, 
       0x3, 0x2, 0x2, 0x2, 0x18f, 0x18d, 0x3, 0x2, 0x2, 0x2, 0x18f, 0x190, 
       0x3, 0x2, 0x2, 0x2, 0x190, 0x39, 0x3, 0x2, 0x2, 0x2, 0x191, 0x18f, 
       0x3, 0x2, 0x2, 0x2, 0x192, 0x197, 0x5, 0x3c, 0x1f, 0x2, 0x193, 0x194, 
       0x7, 0x3d, 0x2, 0x2, 0x194, 0x196, 0x5, 0x3c, 0x1f, 0x2, 0x195, 0x193, 
       0x3, 0x2, 0x2, 0x2, 0x196, 0x199, 0x3, 0x2, 0x2, 0x2, 0x197, 0x195, 
       0x3, 0x2, 0x2, 0x2, 0x197, 0x198, 0x3, 0x2, 0x2, 0x2, 0x198, 0x3b, 
       0x3, 0x2, 0x2, 0x2, 0x199, 0x197, 0x3, 0x2, 0x2, 0x2, 0x19a, 0x19c, 
       0x7, 0x3f, 0x2, 0x2, 0x19b, 0x19a, 0x3, 0x2, 0x2, 0x2, 0x19b, 0x19c, 
       0x3, 0x2, 0x2, 0x2, 0x19c, 0x19d, 0x3, 0x2, 0x2, 0x2, 0x19d, 0x19f, 
       0x7, 0x2f, 0x2, 0x2, 0x19e, 0x1a0, 0x5, 0x3e, 0x20, 0x2, 0x19f, 0x19e, 
       0x3, 0x2, 0x2, 0x2, 0x19f, 0x1a0, 0x3, 0x2, 0x2, 0x2, 0x1a0, 0x1ab, 
       0x3, 0x2, 0x2, 0x2, 0x1a1, 0x1a3, 0x7, 0x3f, 0x2, 0x2, 0x1a2, 0x1a1, 
       0x3, 0x2, 0x2, 0x2, 0x1a2, 0x1a3, 0x3, 0x2, 0x2, 0x2, 0x1a3, 0x1a4, 
       0x3, 0x2, 0x2, 0x2, 0x1a4, 0x1a5, 0x7, 0x45, 0x2, 0x2, 0x1a5, 0x1a6, 
       0x5, 0x38, 0x1d, 0x2, 0x1a6, 0x1a8, 0x7, 0x46, 0x2, 0x2, 0x1a7, 0x1a9, 
       0x5, 0x3e, 0x20, 0x2, 0x1a8, 0x1a7, 0x3, 0x2, 0x2, 0x2, 0x1a8, 0x1a9, 
       0x3, 0x2, 0x2, 0x2, 0x1a9, 0x1ab, 0x3, 0x2, 0x2, 0x2, 0x1aa, 0x19b, 
       0x3, 0x2, 0x2, 0x2, 0x1aa, 0x1a2, 0x3, 0x2, 0x2, 0x2, 0x1ab, 0x3d, 
       0x3, 0x2, 0x2, 0x2, 0x1ac, 0x1b5, 0x7, 0x40, 0x2, 0x2, 0x1ad, 0x1b5, 
       0x7, 0x43, 0x2, 0x2, 0x1ae, 0x1b5, 0x7, 0x42, 0x2, 0x2, 0x1af, 0x1b0, 
       0x7, 0x47, 0x2, 0x2, 0x1b0, 0x1b1, 0x7, 0x33, 0x2, 0x2, 0x1b1, 0x1b2, 
       0x7, 0x4b, 0x2, 0x2, 0x1b2, 0x1b3, 0x7, 0x33, 0x2, 0x2, 0x1b3, 0x1b5, 
       0x7, 0x48, 0x2, 0x2, 0x1b4, 0x1ac, 0x3, 0x2, 0x2, 0x2, 0x1b4, 0x1ad, 
       0x3, 0x2, 0x2, 0x2, 0x1b4, 0x1ae, 0x3, 0x2, 0x2, 0x2, 0x1b4, 0x1af, 
       0x3, 0x2, 0x2, 0x2, 0x1b5, 0x3f, 0x3, 0x2, 0x2, 0x2, 0x1b6, 0x1b7, 
       0x9, 0x5, 0x2, 0x2, 0x1b7, 0x41, 0x3, 0x2, 0x2, 0x2, 0x1b8, 0x1bb, 
       0x5, 0x44, 0x23, 0x2, 0x1b9, 0x1bb, 0x5, 0x4c, 0x27, 0x2, 0x1ba, 
       0x1b8, 0x3, 0x2, 0x2, 0x2, 0x1ba, 0x1b9, 0x3, 0x2, 0x2, 0x2, 0x1bb, 
       0x43, 0x3, 0x2, 0x2, 0x2, 0x1bc, 0x1bd, 0x7, 0x45, 0x2, 0x2, 0x1bd, 
       0x1be, 0x5, 0x46, 0x24, 0x2, 0x1be, 0x1bf, 0x7, 0x46, 0x2, 0x2, 0x1bf, 
       0x45, 0x3, 0x2, 0x2, 0x2, 0x1c0, 0x1c5, 0x5, 0x52, 0x2a, 0x2, 0x1c1, 
       0x1c5, 0x7, 0x2c, 0x2, 0x2, 0x1c2, 0x1c5, 0x7, 0x2d, 0x2, 0x2, 0x1c3, 
       0x1c5, 0x5, 0x58, 0x2d, 0x2, 0x1c4, 0x1c0, 0x3, 0x2, 0x2, 0x2, 0x1c4, 
       0x1c1, 0x3, 0x2, 0x2, 0x2, 0x1c4, 0x1c2, 0x3, 0x2, 0x2, 0x2, 0x1c4, 
       0x1c3, 0x3, 0x2, 0x2, 0x2, 0x1c5, 0x47, 0x3, 0x2, 0x2, 0x2, 0x1c6, 
       0x1c9, 0x7, 0x3a, 0x2, 0x2, 0x1c7, 0x1c8, 0x7, 0x44, 0x2, 0x2, 0x1c8, 
       0x1ca, 0x5, 0x4a, 0x26, 0x2, 0x1c9, 0x1c7, 0x3, 0x2, 0x2, 0x2, 0x1c9, 
       0x1ca, 0x3, 0x2, 0x2, 0x2, 0x1ca, 0x1cb, 0x3, 0x2, 0x2, 0x2, 0x1cb, 
       0x1d4, 0x7, 0x44, 0x2, 0x2, 0x1cc, 0x1d0, 0x7, 0x44, 0x2, 0x2, 0x1cd, 
       0x1ce, 0x5, 0x4a, 0x26, 0x2, 0x1ce, 0x1cf, 0x7, 0x44, 0x2, 0x2, 0x1cf, 
       0x1d1, 0x3, 0x2, 0x2, 0x2, 0x1d0, 0x1cd, 0x3, 0x2, 0x2, 0x2, 0x1d0, 
       0x1d1, 0x3, 0x2, 0x2, 0x2, 0x1d1, 0x1d2, 0x3, 0x2, 0x2, 0x2, 0x1d2, 
       0x1d4, 0x7, 0x3b, 0x2, 0x2, 0x1d3, 0x1c6, 0x3, 0x2, 0x2, 0x2, 0x1d3, 
       0x1cc, 0x3, 0x2, 0x2, 0x2, 0x1d4, 0x49, 0x3, 0x2, 0x2, 0x2, 0x1d5, 
       0x1d7, 0x7, 0x49, 0x2, 0x2, 0x1d6, 0x1d8, 0x9, 0x6, 0x2, 0x2, 0x1d7, 
       0x1d6, 0x3, 0x2, 0x2, 0x2, 0x1d7, 0x1d8, 0x3, 0x2, 0x2, 0x2, 0x1d8, 
       0x1da, 0x3, 0x2, 0x2, 0x2, 0x1d9, 0x1db, 0x9, 0x7, 0x2, 0x2, 0x1da, 
       0x1d9, 0x3, 0x2, 0x2, 0x2, 0x1da, 0x1db, 0x3, 0x2, 0x2, 0x2, 0x1db, 
       0x1dd, 0x3, 0x2, 0x2, 0x2, 0x1dc, 0x1de, 0x5, 0x4e, 0x28, 0x2, 0x1dd, 
       0x1dc, 0x3, 0x2, 0x2, 0x2, 0x1dd, 0x1de, 0x3, 0x2, 0x2, 0x2, 0x1de, 
       0x1df, 0x3, 0x2, 0x2, 0x2, 0x1df, 0x1e0, 0x7, 0x4a, 0x2, 0x2, 0x1e0, 
       0x4b, 0x3, 0x2, 0x2, 0x2, 0x1e1, 0x1e3, 0x7, 0x45, 0x2, 0x2, 0x1e2, 
       0x1e4, 0x7, 0x31, 0x2, 0x2, 0x1e3, 0x1e2, 0x3, 0x2, 0x2, 0x2, 0x1e3, 
       0x1e4, 0x3, 0x2, 0x2, 0x2, 0x1e4, 0x1e8, 0x3, 0x2, 0x2, 0x2, 0x1e5, 
       0x1e7, 0x7, 0x2f, 0x2, 0x2, 0x1e6, 0x1e5, 0x3, 0x2, 0x2, 0x2, 0x1e7, 
       0x1ea, 0x3, 0x2, 0x2, 0x2, 0x1e8, 0x1e6, 0x3, 0x2, 0x2, 0x2, 0x1e8, 
       0x1e9, 0x3, 0x2, 0x2, 0x2, 0x1e9, 0x1ec, 0x3, 0x2, 0x2, 0x2, 0x1ea, 
       0x1e8, 0x3, 0x2, 0x2, 0x2, 0x1eb, 0x1ed, 0x5, 0x4e, 0x28, 0x2, 0x1ec, 
       0x1eb, 0x3, 0x2, 0x2, 0x2, 0x1ec, 0x1ed, 0x3, 0x2, 0x2, 0x2, 0x1ed, 
       0x1ee, 0x3, 0x2, 0x2, 0x2, 0x1ee, 0x1ef, 0x7, 0x46, 0x2, 0x2, 0x1ef, 
       0x4d, 0x3, 0x2, 0x2, 0x2, 0x1f0, 0x1f1, 0x7, 0x47, 0x2, 0x2, 0x1f1, 
       0x1f6, 0x5, 0x50, 0x29, 0x2, 0x1f2, 0x1f3, 0x7, 0x4b, 0x2, 0x2, 0x1f3, 
       0x1f5, 0x5, 0x50, 0x29, 0x2, 0x1f4, 0x1f2, 0x3, 0x2, 0x2, 0x2, 0x1f5, 
       0x1f8, 0x3, 0x2, 0x2, 0x2, 0x1f6, 0x1f4, 0x3, 0x2, 0x2, 0x2, 0x1f6, 
       0x1f7, 0x3, 0x2, 0x2, 0x2, 0x1f7, 0x1f9, 0x3, 0x2, 0x2, 0x2, 0x1f8, 
       0x1f6, 0x3, 0x2, 0x2, 0x2, 0x1f9, 0x1fa, 0x7, 0x48, 0x2, 0x2, 0x1fa, 
       0x4f, 0x3, 0x2, 0x2, 0x2, 0x1fb, 0x200, 0x5, 0x52, 0x2a, 0x2, 0x1fc, 
       0x1fd, 0x7, 0x4c, 0x2, 0x2, 0x1fd, 0x201, 0x5, 0x58, 0x2d, 0x2, 0x1fe, 
       0x201, 0x7, 0x2a, 0x2, 0x2, 0x1ff, 0x201, 0x7, 0x2b, 0x2, 0x2, 0x200, 
       0x1fc, 0x3, 0x2, 0x2, 0x2, 0x200, 0x1fe, 0x3, 0x2, 0x2, 0x2, 0x200, 
       0x1ff, 0x3, 0x2, 0x2, 0x2, 0x201, 0x51, 0x3, 0x2, 0x2, 0x2, 0x202, 
       0x205, 0x7, 0x35, 0x2, 0x2, 0x203, 0x205, 0x5, 0x6c, 0x37, 0x2, 0x204, 
       0x202, 0x3, 0x2, 0x2, 0x2, 0x204, 0x203, 0x3, 0x2, 0x2, 0x2, 0x205, 
       0x53, 0x3, 0x2, 0x2, 0x2, 0x206, 0x207, 0x9, 0x8, 0x2, 0x2, 0x207, 
       0x55, 0x3, 0x2, 0x2, 0x2, 0x208, 0x20a, 0x9, 0x9, 0x2, 0x2, 0x209, 
       0x208, 0x3, 0x2, 0x2, 0x2, 0x209, 0x20a, 0x3, 0x2, 0x2, 0x2, 0x20a, 
       0x20b, 0x3, 0x2, 0x2, 0x2, 0x20b, 0x20c, 0x9, 0xa, 0x2, 0x2, 0x20c, 
       0x57, 0x3, 0x2, 0x2, 0x2, 0x20d, 0x211, 0x5, 0x56, 0x2c, 0x2, 0x20e, 
       0x211, 0x7, 0x32, 0x2, 0x2, 0x20f, 0x211, 0x5, 0x54, 0x2b, 0x2, 0x210, 
       0x20d, 0x3, 0x2, 0x2, 0x2, 0x210, 0x20e, 0x3, 0x2, 0x2, 0x2, 0x210, 
       0x20f, 0x3, 0x2, 0x2, 0x2, 0x211, 0x59, 0x3, 0x2, 0x2, 0x2, 0x212, 
       0x217, 0x5, 0x5c, 0x2f, 0x2, 0x213, 0x214, 0x7, 0x1e, 0x2, 0x2, 0x214, 
       0x216, 0x5, 0x5c, 0x2f, 0x2, 0x215, 0x213, 0x3, 0x2, 0x2, 0x2, 0x216, 
       0x219, 0x3, 0x2, 0x2, 0x2, 0x217, 0x215, 0x3, 0x2, 0x2, 0x2, 0x217, 
       0x218, 0x3, 0x2, 0x2, 0x2, 0x218, 0x5b, 0x3, 0x2, 0x2, 0x2, 0x219, 
       0x217, 0x3, 0x2, 0x2, 0x2, 0x21a, 0x21f, 0x5, 0x5e, 0x30, 0x2, 0x21b, 
       0x21c, 0x7, 0x4, 0x2, 0x2, 0x21c, 0x21e, 0x5, 0x5e, 0x30, 0x2, 0x21d, 
       0x21b, 0x3, 0x2, 0x2, 0x2, 0x21e, 0x221, 0x3, 0x2, 0x2, 0x2, 0x21f, 
       0x21d, 0x3, 0x2, 0x2, 0x2, 0x21f, 0x220, 0x3, 0x2, 0x2, 0x2, 0x220, 
       0x5d, 0x3, 0x2, 0x2, 0x2, 0x221, 0x21f, 0x3, 0x2, 0x2, 0x2, 0x222, 
       0x225, 0x5, 0x60, 0x31, 0x2, 0x223, 0x224, 0x9, 0xb, 0x2, 0x2, 0x224, 
       0x226, 0x5, 0x60, 0x31, 0x2, 0x225, 0x223, 0x3, 0x2, 0x2, 0x2, 0x225, 
       0x226, 0x3, 0x2, 0x2, 0x2, 0x226, 0x22f, 0x3, 0x2, 0x2, 0x2, 0x227, 
       0x228, 0x5, 0x60, 0x31, 0x2, 0x228, 0x22a, 0x7, 0x12, 0x2, 0x2, 0x229, 
       0x22b, 0x7, 0x22, 0x2, 0x2, 0x22a, 0x229, 0x3, 0x2, 0x2, 0x2, 0x22a, 
       0x22b, 0x3, 0x2, 0x2, 0x2, 0x22b, 0x22c, 0x3, 0x2, 0x2, 0x2, 0x22c, 
       0x22d, 0x5, 0x6a, 0x36, 0x2, 0x22d, 0x22f, 0x3, 0x2, 0x2, 0x2, 0x22e, 
       0x222, 0x3, 0x2, 0x2, 0x2, 0x22e, 0x227, 0x3, 0x2, 0x2, 0x2, 0x22f, 
       0x5f, 0x3, 0x2, 0x2, 0x2, 0x230, 0x235, 0x5, 0x62, 0x32, 0x2, 0x231, 
       0x232, 0x9, 0x9, 0x2, 0x2, 0x232, 0x234, 0x5, 0x62, 0x32, 0x2, 0x233, 
       0x231, 0x3, 0x2, 0x2, 0x2, 0x234, 0x237, 0x3, 0x2, 0x2, 0x2, 0x235, 
       0x233, 0x3, 0x2, 0x2, 0x2, 0x235, 0x236, 0x3, 0x2, 0x2, 0x2, 0x236, 
       0x61, 0x3, 0x2, 0x2, 0x2, 0x237, 0x235, 0x3, 0x2, 0x2, 0x2, 0x238, 
       0x23d, 0x5, 0x64, 0x33, 0x2, 0x239, 0x23a, 0x9, 0xc, 0x2, 0x2, 0x23a, 
       0x23c, 0x5, 0x64, 0x33, 0x2, 0x23b, 0x239, 0x3, 0x2, 0x2, 0x2, 0x23c, 
       0x23f, 0x3, 0x2, 0x2, 0x2, 0x23d, 0x23b, 0x3, 0x2, 0x2, 0x2, 0x23d, 
       0x23e, 0x3, 0x2, 0x2, 0x2, 0x23e, 0x63, 0x3, 0x2, 0x2, 0x2, 0x23f, 
       0x23d, 0x3, 0x2, 0x2, 0x2, 0x240, 0x241, 0x7, 0x22, 0x2, 0x2, 0x241, 
       0x248, 0x5, 0x64, 0x33, 0x2, 0x242, 0x243, 0x7, 0x43, 0x2, 0x2, 0x243, 
       0x248, 0x5, 0x64, 0x33, 0x2, 0x244, 0x245, 0x7, 0x44, 0x2, 0x2, 0x245, 
       0x248, 0x5, 0x64, 0x33, 0x2, 0x246, 0x248, 0x5, 0x66, 0x34, 0x2, 
       0x247, 0x240, 0x3, 0x2, 0x2, 0x2, 0x247, 0x242, 0x3, 0x2, 0x2, 0x2, 
       0x247, 0x244, 0x3, 0x2, 0x2, 0x2, 0x247, 0x246, 0x3, 0x2, 0x2, 0x2, 
       0x248, 0x65, 0x3, 0x2, 0x2, 0x2, 0x249, 0x24b, 0x7, 0x31, 0x2, 0x2, 
       0x24a, 0x24c, 0x7, 0x2e, 0x2, 0x2, 0x24b, 0x24a, 0x3, 0x2, 0x2, 0x2, 
       0x24b, 0x24c, 0x3, 0x2, 0x2, 0x2, 0x24c, 0x253, 0x3, 0x2, 0x2, 0x2, 
       0x24d, 0x253, 0x5, 0x68, 0x35, 0x2, 0x24e, 0x24f, 0x7, 0x45, 0x2, 
       0x2, 0x24f, 0x250, 0x5, 0x5a, 0x2e, 0x2, 0x250, 0x251, 0x7, 0x46, 
       0x2, 0x2, 0x251, 0x253, 0x3, 0x2, 0x2, 0x2, 0x252, 0x249, 0x3, 0x2, 
       0x2, 0x2, 0x252, 0x24d, 0x3, 0x2, 0x2, 0x2, 0x252, 0x24e, 0x3, 0x2, 
       0x2, 0x2, 0x253, 0x67, 0x3, 0x2, 0x2, 0x2, 0x254, 0x259, 0x7, 0x33, 
       0x2, 0x2, 0x255, 0x259, 0x7, 0x34, 0x2, 0x2, 0x256, 0x259, 0x7, 0x32, 
       0x2, 0x2, 0x257, 0x259, 0x5, 0x54, 0x2b, 0x2, 0x258, 0x254, 0x3, 
       0x2, 0x2, 0x2, 0x258, 0x255, 0x3, 0x2, 0x2, 0x2, 0x258, 0x256, 0x3, 
       0x2, 0x2, 0x2, 0x258, 0x257, 0x3, 0x2, 0x2, 0x2, 0x259, 0x69, 0x3, 
       0x2, 0x2, 0x2, 0x25a, 0x25b, 0x9, 0xd, 0x2, 0x2, 0x25b, 0x6b, 0x3, 
       0x2, 0x2, 0x2, 0x25c, 0x25d, 0x9, 0xe, 0x2, 0x2, 0x25d, 0x6d, 0x3, 
       0x2, 0x2, 0x2, 0x52, 0x6f, 0x74, 0x7a, 0x7d, 0x80, 0x88, 0x90, 0x96, 
       0x9e, 0xa7, 0xb0, 0xb6, 0xd2, 0xe2, 0xec, 0xf1, 0xf8, 0xfd, 0x101, 
       0x106, 0x108, 0x110, 0x116, 0x11d, 0x121, 0x124, 0x127, 0x12d, 0x130, 
       0x136, 0x13a, 0x13f, 0x143, 0x147, 0x149, 0x14d, 0x153, 0x160, 0x166, 
       0x16c, 0x172, 0x175, 0x17e, 0x181, 0x188, 0x18f, 0x197, 0x19b, 0x19f, 
       0x1a2, 0x1a8, 0x1aa, 0x1b4, 0x1ba, 0x1c4, 0x1c9, 0x1d0, 0x1d3, 0x1d7, 
       0x1da, 0x1dd, 0x1e3, 0x1e8, 0x1ec, 0x1f6, 0x200, 0x204, 0x209, 0x210, 
       0x217, 0x21f, 0x225, 0x22a, 0x22e, 0x235, 0x23d, 0x247, 0x24b, 0x252, 
       0x258, 
  };

  _serializedATN.insert(_serializedATN.end(), serializedATNSegment0,
    serializedATNSegment0 + sizeof(serializedATNSegment0) / sizeof(serializedATNSegment0[0]));


  atn::ATNDeserializer deserializer;
  _atn = deserializer.deserialize(_serializedATN);

  size_t count = _atn.getNumberOfDecisions();
  _decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    _decisionToDFA.emplace_back(_atn.getDecisionState(i), i);
  }
}

MQL_Parser::Initializer MQL_Parser::_init;


// Generated from SparqlParser.g4 by ANTLR 4.9.3


#include "SparqlParserVisitor.h"

#include "SparqlParser.h"


using namespace antlrcpp;
using namespace antlr4;

SparqlParser::SparqlParser(TokenStream *input) : Parser(input) {
  _interpreter = new atn::ParserATNSimulator(this, _atn, _decisionToDFA, _sharedContextCache);
}

SparqlParser::~SparqlParser() {
  delete _interpreter;
}

std::string SparqlParser::getGrammarFileName() const {
  return "SparqlParser.g4";
}

const std::vector<std::string>& SparqlParser::getRuleNames() const {
  return _ruleNames;
}

dfa::Vocabulary& SparqlParser::getVocabulary() const {
  return _vocabulary;
}


//----------------- QueryContext ------------------------------------------------------------------

SparqlParser::QueryContext::QueryContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::PrologueContext* SparqlParser::QueryContext::prologue() {
  return getRuleContext<SparqlParser::PrologueContext>(0);
}

SparqlParser::ValuesClauseContext* SparqlParser::QueryContext::valuesClause() {
  return getRuleContext<SparqlParser::ValuesClauseContext>(0);
}

tree::TerminalNode* SparqlParser::QueryContext::EOF() {
  return getToken(SparqlParser::EOF, 0);
}

SparqlParser::SelectQueryContext* SparqlParser::QueryContext::selectQuery() {
  return getRuleContext<SparqlParser::SelectQueryContext>(0);
}

SparqlParser::ConstructQueryContext* SparqlParser::QueryContext::constructQuery() {
  return getRuleContext<SparqlParser::ConstructQueryContext>(0);
}

SparqlParser::DescribeQueryContext* SparqlParser::QueryContext::describeQuery() {
  return getRuleContext<SparqlParser::DescribeQueryContext>(0);
}

SparqlParser::AskQueryContext* SparqlParser::QueryContext::askQuery() {
  return getRuleContext<SparqlParser::AskQueryContext>(0);
}

SparqlParser::UpdateCommandContext* SparqlParser::QueryContext::updateCommand() {
  return getRuleContext<SparqlParser::UpdateCommandContext>(0);
}


size_t SparqlParser::QueryContext::getRuleIndex() const {
  return SparqlParser::RuleQuery;
}


antlrcpp::Any SparqlParser::QueryContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitQuery(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::QueryContext* SparqlParser::query() {
  QueryContext *_localctx = _tracker.createInstance<QueryContext>(_ctx, getState());
  enterRule(_localctx, 0, SparqlParser::RuleQuery);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(281);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 1, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(268);
      prologue();
      setState(273);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case SparqlParser::SELECT: {
          setState(269);
          selectQuery();
          break;
        }

        case SparqlParser::CONSTRUCT: {
          setState(270);
          constructQuery();
          break;
        }

        case SparqlParser::DESCRIBE: {
          setState(271);
          describeQuery();
          break;
        }

        case SparqlParser::ASK: {
          setState(272);
          askQuery();
          break;
        }

        case SparqlParser::EOF:
        case SparqlParser::VALUES: {
          break;
        }

      default:
        break;
      }
      setState(275);
      valuesClause();
      setState(276);
      match(SparqlParser::EOF);
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(278);
      updateCommand();
      setState(279);
      match(SparqlParser::EOF);
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

//----------------- PrologueContext ------------------------------------------------------------------

SparqlParser::PrologueContext::PrologueContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<SparqlParser::BaseDeclContext *> SparqlParser::PrologueContext::baseDecl() {
  return getRuleContexts<SparqlParser::BaseDeclContext>();
}

SparqlParser::BaseDeclContext* SparqlParser::PrologueContext::baseDecl(size_t i) {
  return getRuleContext<SparqlParser::BaseDeclContext>(i);
}

std::vector<SparqlParser::PrefixDeclContext *> SparqlParser::PrologueContext::prefixDecl() {
  return getRuleContexts<SparqlParser::PrefixDeclContext>();
}

SparqlParser::PrefixDeclContext* SparqlParser::PrologueContext::prefixDecl(size_t i) {
  return getRuleContext<SparqlParser::PrefixDeclContext>(i);
}


size_t SparqlParser::PrologueContext::getRuleIndex() const {
  return SparqlParser::RulePrologue;
}


antlrcpp::Any SparqlParser::PrologueContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitPrologue(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::PrologueContext* SparqlParser::prologue() {
  PrologueContext *_localctx = _tracker.createInstance<PrologueContext>(_ctx, getState());
  enterRule(_localctx, 2, SparqlParser::RulePrologue);
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
    setState(287);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SparqlParser::BASE

    || _la == SparqlParser::PREFIX) {
      setState(285);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case SparqlParser::BASE: {
          setState(283);
          baseDecl();
          break;
        }

        case SparqlParser::PREFIX: {
          setState(284);
          prefixDecl();
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      setState(289);
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

//----------------- BaseDeclContext ------------------------------------------------------------------

SparqlParser::BaseDeclContext::BaseDeclContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::BaseDeclContext::BASE() {
  return getToken(SparqlParser::BASE, 0);
}

tree::TerminalNode* SparqlParser::BaseDeclContext::IRIREF() {
  return getToken(SparqlParser::IRIREF, 0);
}


size_t SparqlParser::BaseDeclContext::getRuleIndex() const {
  return SparqlParser::RuleBaseDecl;
}


antlrcpp::Any SparqlParser::BaseDeclContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitBaseDecl(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::BaseDeclContext* SparqlParser::baseDecl() {
  BaseDeclContext *_localctx = _tracker.createInstance<BaseDeclContext>(_ctx, getState());
  enterRule(_localctx, 4, SparqlParser::RuleBaseDecl);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(290);
    match(SparqlParser::BASE);
    setState(291);
    match(SparqlParser::IRIREF);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PrefixDeclContext ------------------------------------------------------------------

SparqlParser::PrefixDeclContext::PrefixDeclContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::PrefixDeclContext::PREFIX() {
  return getToken(SparqlParser::PREFIX, 0);
}

tree::TerminalNode* SparqlParser::PrefixDeclContext::PNAME_NS() {
  return getToken(SparqlParser::PNAME_NS, 0);
}

tree::TerminalNode* SparqlParser::PrefixDeclContext::IRIREF() {
  return getToken(SparqlParser::IRIREF, 0);
}


size_t SparqlParser::PrefixDeclContext::getRuleIndex() const {
  return SparqlParser::RulePrefixDecl;
}


antlrcpp::Any SparqlParser::PrefixDeclContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitPrefixDecl(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::PrefixDeclContext* SparqlParser::prefixDecl() {
  PrefixDeclContext *_localctx = _tracker.createInstance<PrefixDeclContext>(_ctx, getState());
  enterRule(_localctx, 6, SparqlParser::RulePrefixDecl);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(293);
    match(SparqlParser::PREFIX);
    setState(294);
    match(SparqlParser::PNAME_NS);
    setState(295);
    match(SparqlParser::IRIREF);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SelectQueryContext ------------------------------------------------------------------

SparqlParser::SelectQueryContext::SelectQueryContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::SelectClauseContext* SparqlParser::SelectQueryContext::selectClause() {
  return getRuleContext<SparqlParser::SelectClauseContext>(0);
}

SparqlParser::WhereClauseContext* SparqlParser::SelectQueryContext::whereClause() {
  return getRuleContext<SparqlParser::WhereClauseContext>(0);
}

SparqlParser::SolutionModifierContext* SparqlParser::SelectQueryContext::solutionModifier() {
  return getRuleContext<SparqlParser::SolutionModifierContext>(0);
}

std::vector<SparqlParser::DatasetClauseContext *> SparqlParser::SelectQueryContext::datasetClause() {
  return getRuleContexts<SparqlParser::DatasetClauseContext>();
}

SparqlParser::DatasetClauseContext* SparqlParser::SelectQueryContext::datasetClause(size_t i) {
  return getRuleContext<SparqlParser::DatasetClauseContext>(i);
}


size_t SparqlParser::SelectQueryContext::getRuleIndex() const {
  return SparqlParser::RuleSelectQuery;
}


antlrcpp::Any SparqlParser::SelectQueryContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitSelectQuery(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::SelectQueryContext* SparqlParser::selectQuery() {
  SelectQueryContext *_localctx = _tracker.createInstance<SelectQueryContext>(_ctx, getState());
  enterRule(_localctx, 8, SparqlParser::RuleSelectQuery);
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
    setState(297);
    selectClause();
    setState(301);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SparqlParser::FROM) {
      setState(298);
      datasetClause();
      setState(303);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(304);
    whereClause();
    setState(305);
    solutionModifier();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SubSelectContext ------------------------------------------------------------------

SparqlParser::SubSelectContext::SubSelectContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::SelectClauseContext* SparqlParser::SubSelectContext::selectClause() {
  return getRuleContext<SparqlParser::SelectClauseContext>(0);
}

SparqlParser::WhereClauseContext* SparqlParser::SubSelectContext::whereClause() {
  return getRuleContext<SparqlParser::WhereClauseContext>(0);
}

SparqlParser::SolutionModifierContext* SparqlParser::SubSelectContext::solutionModifier() {
  return getRuleContext<SparqlParser::SolutionModifierContext>(0);
}

SparqlParser::ValuesClauseContext* SparqlParser::SubSelectContext::valuesClause() {
  return getRuleContext<SparqlParser::ValuesClauseContext>(0);
}


size_t SparqlParser::SubSelectContext::getRuleIndex() const {
  return SparqlParser::RuleSubSelect;
}


antlrcpp::Any SparqlParser::SubSelectContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitSubSelect(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::SubSelectContext* SparqlParser::subSelect() {
  SubSelectContext *_localctx = _tracker.createInstance<SubSelectContext>(_ctx, getState());
  enterRule(_localctx, 10, SparqlParser::RuleSubSelect);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(307);
    selectClause();
    setState(308);
    whereClause();
    setState(309);
    solutionModifier();
    setState(310);
    valuesClause();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SelectClauseContext ------------------------------------------------------------------

SparqlParser::SelectClauseContext::SelectClauseContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::SelectClauseContext::SELECT() {
  return getToken(SparqlParser::SELECT, 0);
}

tree::TerminalNode* SparqlParser::SelectClauseContext::ASTERISK() {
  return getToken(SparqlParser::ASTERISK, 0);
}

SparqlParser::SelectModifierContext* SparqlParser::SelectClauseContext::selectModifier() {
  return getRuleContext<SparqlParser::SelectModifierContext>(0);
}

std::vector<SparqlParser::SelectVariablesContext *> SparqlParser::SelectClauseContext::selectVariables() {
  return getRuleContexts<SparqlParser::SelectVariablesContext>();
}

SparqlParser::SelectVariablesContext* SparqlParser::SelectClauseContext::selectVariables(size_t i) {
  return getRuleContext<SparqlParser::SelectVariablesContext>(i);
}


size_t SparqlParser::SelectClauseContext::getRuleIndex() const {
  return SparqlParser::RuleSelectClause;
}


antlrcpp::Any SparqlParser::SelectClauseContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitSelectClause(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::SelectClauseContext* SparqlParser::selectClause() {
  SelectClauseContext *_localctx = _tracker.createInstance<SelectClauseContext>(_ctx, getState());
  enterRule(_localctx, 12, SparqlParser::RuleSelectClause);
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
    setState(312);
    match(SparqlParser::SELECT);
    setState(314);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == SparqlParser::DISTINCT

    || _la == SparqlParser::REDUCED) {
      setState(313);
      selectModifier();
    }
    setState(322);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlParser::VAR1:
      case SparqlParser::VAR2:
      case SparqlParser::OPEN_BRACE: {
        setState(317); 
        _errHandler->sync(this);
        _la = _input->LA(1);
        do {
          setState(316);
          selectVariables();
          setState(319); 
          _errHandler->sync(this);
          _la = _input->LA(1);
        } while (((((_la - 119) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 119)) & ((1ULL << (SparqlParser::VAR1 - 119))
          | (1ULL << (SparqlParser::VAR2 - 119))
          | (1ULL << (SparqlParser::OPEN_BRACE - 119)))) != 0));
        break;
      }

      case SparqlParser::ASTERISK: {
        setState(321);
        match(SparqlParser::ASTERISK);
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

//----------------- SelectModifierContext ------------------------------------------------------------------

SparqlParser::SelectModifierContext::SelectModifierContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::SelectModifierContext::DISTINCT() {
  return getToken(SparqlParser::DISTINCT, 0);
}

tree::TerminalNode* SparqlParser::SelectModifierContext::REDUCED() {
  return getToken(SparqlParser::REDUCED, 0);
}


size_t SparqlParser::SelectModifierContext::getRuleIndex() const {
  return SparqlParser::RuleSelectModifier;
}


antlrcpp::Any SparqlParser::SelectModifierContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitSelectModifier(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::SelectModifierContext* SparqlParser::selectModifier() {
  SelectModifierContext *_localctx = _tracker.createInstance<SelectModifierContext>(_ctx, getState());
  enterRule(_localctx, 14, SparqlParser::RuleSelectModifier);
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
    setState(324);
    _la = _input->LA(1);
    if (!(_la == SparqlParser::DISTINCT

    || _la == SparqlParser::REDUCED)) {
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

//----------------- SelectVariablesContext ------------------------------------------------------------------

SparqlParser::SelectVariablesContext::SelectVariablesContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t SparqlParser::SelectVariablesContext::getRuleIndex() const {
  return SparqlParser::RuleSelectVariables;
}

void SparqlParser::SelectVariablesContext::copyFrom(SelectVariablesContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- SelectSingleVariableContext ------------------------------------------------------------------

SparqlParser::VarContext* SparqlParser::SelectSingleVariableContext::var() {
  return getRuleContext<SparqlParser::VarContext>(0);
}

SparqlParser::SelectSingleVariableContext::SelectSingleVariableContext(SelectVariablesContext *ctx) { copyFrom(ctx); }


antlrcpp::Any SparqlParser::SelectSingleVariableContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitSelectSingleVariable(this);
  else
    return visitor->visitChildren(this);
}
//----------------- SelectExpressionAsVariableContext ------------------------------------------------------------------

tree::TerminalNode* SparqlParser::SelectExpressionAsVariableContext::OPEN_BRACE() {
  return getToken(SparqlParser::OPEN_BRACE, 0);
}

SparqlParser::ExpressionContext* SparqlParser::SelectExpressionAsVariableContext::expression() {
  return getRuleContext<SparqlParser::ExpressionContext>(0);
}

tree::TerminalNode* SparqlParser::SelectExpressionAsVariableContext::AS() {
  return getToken(SparqlParser::AS, 0);
}

SparqlParser::VarContext* SparqlParser::SelectExpressionAsVariableContext::var() {
  return getRuleContext<SparqlParser::VarContext>(0);
}

tree::TerminalNode* SparqlParser::SelectExpressionAsVariableContext::CLOSE_BRACE() {
  return getToken(SparqlParser::CLOSE_BRACE, 0);
}

SparqlParser::SelectExpressionAsVariableContext::SelectExpressionAsVariableContext(SelectVariablesContext *ctx) { copyFrom(ctx); }


antlrcpp::Any SparqlParser::SelectExpressionAsVariableContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitSelectExpressionAsVariable(this);
  else
    return visitor->visitChildren(this);
}
SparqlParser::SelectVariablesContext* SparqlParser::selectVariables() {
  SelectVariablesContext *_localctx = _tracker.createInstance<SelectVariablesContext>(_ctx, getState());
  enterRule(_localctx, 16, SparqlParser::RuleSelectVariables);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(333);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlParser::VAR1:
      case SparqlParser::VAR2: {
        _localctx = _tracker.createInstance<SparqlParser::SelectSingleVariableContext>(_localctx);
        enterOuterAlt(_localctx, 1);
        setState(326);
        var();
        break;
      }

      case SparqlParser::OPEN_BRACE: {
        _localctx = _tracker.createInstance<SparqlParser::SelectExpressionAsVariableContext>(_localctx);
        enterOuterAlt(_localctx, 2);
        setState(327);
        match(SparqlParser::OPEN_BRACE);
        setState(328);
        expression(0);
        setState(329);
        match(SparqlParser::AS);
        setState(330);
        var();
        setState(331);
        match(SparqlParser::CLOSE_BRACE);
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

//----------------- ConstructQueryContext ------------------------------------------------------------------

SparqlParser::ConstructQueryContext::ConstructQueryContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::ConstructQueryContext::CONSTRUCT() {
  return getToken(SparqlParser::CONSTRUCT, 0);
}

SparqlParser::ConstructTemplateContext* SparqlParser::ConstructQueryContext::constructTemplate() {
  return getRuleContext<SparqlParser::ConstructTemplateContext>(0);
}

SparqlParser::WhereClauseContext* SparqlParser::ConstructQueryContext::whereClause() {
  return getRuleContext<SparqlParser::WhereClauseContext>(0);
}

SparqlParser::SolutionModifierContext* SparqlParser::ConstructQueryContext::solutionModifier() {
  return getRuleContext<SparqlParser::SolutionModifierContext>(0);
}

tree::TerminalNode* SparqlParser::ConstructQueryContext::WHERE() {
  return getToken(SparqlParser::WHERE, 0);
}

tree::TerminalNode* SparqlParser::ConstructQueryContext::OPEN_CURLY_BRACE() {
  return getToken(SparqlParser::OPEN_CURLY_BRACE, 0);
}

tree::TerminalNode* SparqlParser::ConstructQueryContext::CLOSE_CURLY_BRACE() {
  return getToken(SparqlParser::CLOSE_CURLY_BRACE, 0);
}

std::vector<SparqlParser::DatasetClauseContext *> SparqlParser::ConstructQueryContext::datasetClause() {
  return getRuleContexts<SparqlParser::DatasetClauseContext>();
}

SparqlParser::DatasetClauseContext* SparqlParser::ConstructQueryContext::datasetClause(size_t i) {
  return getRuleContext<SparqlParser::DatasetClauseContext>(i);
}

SparqlParser::TriplesTemplateContext* SparqlParser::ConstructQueryContext::triplesTemplate() {
  return getRuleContext<SparqlParser::TriplesTemplateContext>(0);
}


size_t SparqlParser::ConstructQueryContext::getRuleIndex() const {
  return SparqlParser::RuleConstructQuery;
}


antlrcpp::Any SparqlParser::ConstructQueryContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitConstructQuery(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::ConstructQueryContext* SparqlParser::constructQuery() {
  ConstructQueryContext *_localctx = _tracker.createInstance<ConstructQueryContext>(_ctx, getState());
  enterRule(_localctx, 18, SparqlParser::RuleConstructQuery);
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
    setState(335);
    match(SparqlParser::CONSTRUCT);
    setState(359);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlParser::OPEN_CURLY_BRACE: {
        setState(336);
        constructTemplate();
        setState(340);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == SparqlParser::FROM) {
          setState(337);
          datasetClause();
          setState(342);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
        setState(343);
        whereClause();
        setState(344);
        solutionModifier();
        break;
      }

      case SparqlParser::FROM:
      case SparqlParser::WHERE: {
        setState(349);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == SparqlParser::FROM) {
          setState(346);
          datasetClause();
          setState(351);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
        setState(352);
        match(SparqlParser::WHERE);
        setState(353);
        match(SparqlParser::OPEN_CURLY_BRACE);
        setState(355);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == SparqlParser::TRUE

        || _la == SparqlParser::FALSE || ((((_la - 115) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 115)) & ((1ULL << (SparqlParser::IRIREF - 115))
          | (1ULL << (SparqlParser::PNAME_NS - 115))
          | (1ULL << (SparqlParser::PNAME_LN - 115))
          | (1ULL << (SparqlParser::BLANK_NODE_LABEL - 115))
          | (1ULL << (SparqlParser::VAR1 - 115))
          | (1ULL << (SparqlParser::VAR2 - 115))
          | (1ULL << (SparqlParser::INTEGER - 115))
          | (1ULL << (SparqlParser::DECIMAL - 115))
          | (1ULL << (SparqlParser::DOUBLE - 115))
          | (1ULL << (SparqlParser::INTEGER_POSITIVE - 115))
          | (1ULL << (SparqlParser::DECIMAL_POSITIVE - 115))
          | (1ULL << (SparqlParser::DOUBLE_POSITIVE - 115))
          | (1ULL << (SparqlParser::INTEGER_NEGATIVE - 115))
          | (1ULL << (SparqlParser::DECIMAL_NEGATIVE - 115))
          | (1ULL << (SparqlParser::DOUBLE_NEGATIVE - 115))
          | (1ULL << (SparqlParser::STRING_LITERAL1 - 115))
          | (1ULL << (SparqlParser::STRING_LITERAL2 - 115))
          | (1ULL << (SparqlParser::STRING_LITERAL_LONG1 - 115))
          | (1ULL << (SparqlParser::STRING_LITERAL_LONG2 - 115))
          | (1ULL << (SparqlParser::OPEN_BRACE - 115))
          | (1ULL << (SparqlParser::OPEN_SQUARE_BRACKET - 115)))) != 0)) {
          setState(354);
          triplesTemplate();
        }
        setState(357);
        match(SparqlParser::CLOSE_CURLY_BRACE);
        setState(358);
        solutionModifier();
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

//----------------- DescribeQueryContext ------------------------------------------------------------------

SparqlParser::DescribeQueryContext::DescribeQueryContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::DescribeQueryContext::DESCRIBE() {
  return getToken(SparqlParser::DESCRIBE, 0);
}

SparqlParser::SolutionModifierContext* SparqlParser::DescribeQueryContext::solutionModifier() {
  return getRuleContext<SparqlParser::SolutionModifierContext>(0);
}

tree::TerminalNode* SparqlParser::DescribeQueryContext::ASTERISK() {
  return getToken(SparqlParser::ASTERISK, 0);
}

std::vector<SparqlParser::DatasetClauseContext *> SparqlParser::DescribeQueryContext::datasetClause() {
  return getRuleContexts<SparqlParser::DatasetClauseContext>();
}

SparqlParser::DatasetClauseContext* SparqlParser::DescribeQueryContext::datasetClause(size_t i) {
  return getRuleContext<SparqlParser::DatasetClauseContext>(i);
}

SparqlParser::WhereClauseContext* SparqlParser::DescribeQueryContext::whereClause() {
  return getRuleContext<SparqlParser::WhereClauseContext>(0);
}

std::vector<SparqlParser::VarOrIRIContext *> SparqlParser::DescribeQueryContext::varOrIRI() {
  return getRuleContexts<SparqlParser::VarOrIRIContext>();
}

SparqlParser::VarOrIRIContext* SparqlParser::DescribeQueryContext::varOrIRI(size_t i) {
  return getRuleContext<SparqlParser::VarOrIRIContext>(i);
}


size_t SparqlParser::DescribeQueryContext::getRuleIndex() const {
  return SparqlParser::RuleDescribeQuery;
}


antlrcpp::Any SparqlParser::DescribeQueryContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitDescribeQuery(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::DescribeQueryContext* SparqlParser::describeQuery() {
  DescribeQueryContext *_localctx = _tracker.createInstance<DescribeQueryContext>(_ctx, getState());
  enterRule(_localctx, 20, SparqlParser::RuleDescribeQuery);
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
    setState(361);
    match(SparqlParser::DESCRIBE);
    setState(368);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlParser::IRIREF:
      case SparqlParser::PNAME_NS:
      case SparqlParser::PNAME_LN:
      case SparqlParser::VAR1:
      case SparqlParser::VAR2: {
        setState(363); 
        _errHandler->sync(this);
        _la = _input->LA(1);
        do {
          setState(362);
          varOrIRI();
          setState(365); 
          _errHandler->sync(this);
          _la = _input->LA(1);
        } while (((((_la - 115) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 115)) & ((1ULL << (SparqlParser::IRIREF - 115))
          | (1ULL << (SparqlParser::PNAME_NS - 115))
          | (1ULL << (SparqlParser::PNAME_LN - 115))
          | (1ULL << (SparqlParser::VAR1 - 115))
          | (1ULL << (SparqlParser::VAR2 - 115)))) != 0));
        break;
      }

      case SparqlParser::ASTERISK: {
        setState(367);
        match(SparqlParser::ASTERISK);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    setState(373);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SparqlParser::FROM) {
      setState(370);
      datasetClause();
      setState(375);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(377);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == SparqlParser::WHERE || _la == SparqlParser::OPEN_CURLY_BRACE) {
      setState(376);
      whereClause();
    }
    setState(379);
    solutionModifier();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AskQueryContext ------------------------------------------------------------------

SparqlParser::AskQueryContext::AskQueryContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::AskQueryContext::ASK() {
  return getToken(SparqlParser::ASK, 0);
}

SparqlParser::WhereClauseContext* SparqlParser::AskQueryContext::whereClause() {
  return getRuleContext<SparqlParser::WhereClauseContext>(0);
}

SparqlParser::SolutionModifierContext* SparqlParser::AskQueryContext::solutionModifier() {
  return getRuleContext<SparqlParser::SolutionModifierContext>(0);
}

std::vector<SparqlParser::DatasetClauseContext *> SparqlParser::AskQueryContext::datasetClause() {
  return getRuleContexts<SparqlParser::DatasetClauseContext>();
}

SparqlParser::DatasetClauseContext* SparqlParser::AskQueryContext::datasetClause(size_t i) {
  return getRuleContext<SparqlParser::DatasetClauseContext>(i);
}


size_t SparqlParser::AskQueryContext::getRuleIndex() const {
  return SparqlParser::RuleAskQuery;
}


antlrcpp::Any SparqlParser::AskQueryContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitAskQuery(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::AskQueryContext* SparqlParser::askQuery() {
  AskQueryContext *_localctx = _tracker.createInstance<AskQueryContext>(_ctx, getState());
  enterRule(_localctx, 22, SparqlParser::RuleAskQuery);
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
    setState(381);
    match(SparqlParser::ASK);
    setState(385);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SparqlParser::FROM) {
      setState(382);
      datasetClause();
      setState(387);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(388);
    whereClause();
    setState(389);
    solutionModifier();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DatasetClauseContext ------------------------------------------------------------------

SparqlParser::DatasetClauseContext::DatasetClauseContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::DatasetClauseContext::FROM() {
  return getToken(SparqlParser::FROM, 0);
}

SparqlParser::IriContext* SparqlParser::DatasetClauseContext::iri() {
  return getRuleContext<SparqlParser::IriContext>(0);
}

tree::TerminalNode* SparqlParser::DatasetClauseContext::NAMED() {
  return getToken(SparqlParser::NAMED, 0);
}


size_t SparqlParser::DatasetClauseContext::getRuleIndex() const {
  return SparqlParser::RuleDatasetClause;
}


antlrcpp::Any SparqlParser::DatasetClauseContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitDatasetClause(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::DatasetClauseContext* SparqlParser::datasetClause() {
  DatasetClauseContext *_localctx = _tracker.createInstance<DatasetClauseContext>(_ctx, getState());
  enterRule(_localctx, 24, SparqlParser::RuleDatasetClause);
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
    setState(391);
    match(SparqlParser::FROM);
    setState(393);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == SparqlParser::NAMED) {
      setState(392);
      match(SparqlParser::NAMED);
    }
    setState(395);
    iri();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- WhereClauseContext ------------------------------------------------------------------

SparqlParser::WhereClauseContext::WhereClauseContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::GroupGraphPatternContext* SparqlParser::WhereClauseContext::groupGraphPattern() {
  return getRuleContext<SparqlParser::GroupGraphPatternContext>(0);
}

tree::TerminalNode* SparqlParser::WhereClauseContext::WHERE() {
  return getToken(SparqlParser::WHERE, 0);
}


size_t SparqlParser::WhereClauseContext::getRuleIndex() const {
  return SparqlParser::RuleWhereClause;
}


antlrcpp::Any SparqlParser::WhereClauseContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitWhereClause(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::WhereClauseContext* SparqlParser::whereClause() {
  WhereClauseContext *_localctx = _tracker.createInstance<WhereClauseContext>(_ctx, getState());
  enterRule(_localctx, 26, SparqlParser::RuleWhereClause);
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
    setState(398);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == SparqlParser::WHERE) {
      setState(397);
      match(SparqlParser::WHERE);
    }
    setState(400);
    groupGraphPattern();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SolutionModifierContext ------------------------------------------------------------------

SparqlParser::SolutionModifierContext::SolutionModifierContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::GroupClauseContext* SparqlParser::SolutionModifierContext::groupClause() {
  return getRuleContext<SparqlParser::GroupClauseContext>(0);
}

SparqlParser::HavingClauseContext* SparqlParser::SolutionModifierContext::havingClause() {
  return getRuleContext<SparqlParser::HavingClauseContext>(0);
}

SparqlParser::OrderClauseContext* SparqlParser::SolutionModifierContext::orderClause() {
  return getRuleContext<SparqlParser::OrderClauseContext>(0);
}

SparqlParser::LimitOffsetClausesContext* SparqlParser::SolutionModifierContext::limitOffsetClauses() {
  return getRuleContext<SparqlParser::LimitOffsetClausesContext>(0);
}


size_t SparqlParser::SolutionModifierContext::getRuleIndex() const {
  return SparqlParser::RuleSolutionModifier;
}


antlrcpp::Any SparqlParser::SolutionModifierContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitSolutionModifier(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::SolutionModifierContext* SparqlParser::solutionModifier() {
  SolutionModifierContext *_localctx = _tracker.createInstance<SolutionModifierContext>(_ctx, getState());
  enterRule(_localctx, 28, SparqlParser::RuleSolutionModifier);
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
    setState(403);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == SparqlParser::GROUP) {
      setState(402);
      groupClause();
    }
    setState(406);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == SparqlParser::HAVING) {
      setState(405);
      havingClause();
    }
    setState(409);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == SparqlParser::ORDER) {
      setState(408);
      orderClause();
    }
    setState(412);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == SparqlParser::LIMIT

    || _la == SparqlParser::OFFSET) {
      setState(411);
      limitOffsetClauses();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- GroupClauseContext ------------------------------------------------------------------

SparqlParser::GroupClauseContext::GroupClauseContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::GroupClauseContext::GROUP() {
  return getToken(SparqlParser::GROUP, 0);
}

tree::TerminalNode* SparqlParser::GroupClauseContext::BY() {
  return getToken(SparqlParser::BY, 0);
}

std::vector<SparqlParser::GroupConditionContext *> SparqlParser::GroupClauseContext::groupCondition() {
  return getRuleContexts<SparqlParser::GroupConditionContext>();
}

SparqlParser::GroupConditionContext* SparqlParser::GroupClauseContext::groupCondition(size_t i) {
  return getRuleContext<SparqlParser::GroupConditionContext>(i);
}


size_t SparqlParser::GroupClauseContext::getRuleIndex() const {
  return SparqlParser::RuleGroupClause;
}


antlrcpp::Any SparqlParser::GroupClauseContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitGroupClause(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::GroupClauseContext* SparqlParser::groupClause() {
  GroupClauseContext *_localctx = _tracker.createInstance<GroupClauseContext>(_ctx, getState());
  enterRule(_localctx, 30, SparqlParser::RuleGroupClause);
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
    setState(414);
    match(SparqlParser::GROUP);
    setState(415);
    match(SparqlParser::BY);
    setState(417); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(416);
      groupCondition();
      setState(419); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while (((((_la - 25) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 25)) & ((1ULL << (SparqlParser::STR - 25))
      | (1ULL << (SparqlParser::LANG - 25))
      | (1ULL << (SparqlParser::LANGMATCHES - 25))
      | (1ULL << (SparqlParser::DATATYPE - 25))
      | (1ULL << (SparqlParser::BOUND - 25))
      | (1ULL << (SparqlParser::SAMETERM - 25))
      | (1ULL << (SparqlParser::ISIRI - 25))
      | (1ULL << (SparqlParser::ISURI - 25))
      | (1ULL << (SparqlParser::ISBLANK - 25))
      | (1ULL << (SparqlParser::ISLITERAL - 25))
      | (1ULL << (SparqlParser::REGEX - 25))
      | (1ULL << (SparqlParser::SUBSTR - 25))
      | (1ULL << (SparqlParser::IRI - 25))
      | (1ULL << (SparqlParser::URI - 25))
      | (1ULL << (SparqlParser::BNODE - 25))
      | (1ULL << (SparqlParser::RAND - 25))
      | (1ULL << (SparqlParser::ABS - 25))
      | (1ULL << (SparqlParser::CEIL - 25))
      | (1ULL << (SparqlParser::FLOOR - 25))
      | (1ULL << (SparqlParser::ROUND - 25))
      | (1ULL << (SparqlParser::CONCAT - 25))
      | (1ULL << (SparqlParser::STRLEN - 25))
      | (1ULL << (SparqlParser::UCASE - 25))
      | (1ULL << (SparqlParser::LCASE - 25))
      | (1ULL << (SparqlParser::ENCODE_FOR_URI - 25))
      | (1ULL << (SparqlParser::CONTAINS - 25))
      | (1ULL << (SparqlParser::STRSTARTS - 25))
      | (1ULL << (SparqlParser::STRENDS - 25))
      | (1ULL << (SparqlParser::STRBEFORE - 25))
      | (1ULL << (SparqlParser::STRAFTER - 25))
      | (1ULL << (SparqlParser::REPLACE - 25))
      | (1ULL << (SparqlParser::YEAR - 25))
      | (1ULL << (SparqlParser::MONTH - 25))
      | (1ULL << (SparqlParser::DAY - 25))
      | (1ULL << (SparqlParser::HOURS - 25))
      | (1ULL << (SparqlParser::MINUTES - 25))
      | (1ULL << (SparqlParser::SECONDS - 25)))) != 0) || ((((_la - 89) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 89)) & ((1ULL << (SparqlParser::TIMEZONE - 89))
      | (1ULL << (SparqlParser::TZ - 89))
      | (1ULL << (SparqlParser::NOW - 89))
      | (1ULL << (SparqlParser::UUID - 89))
      | (1ULL << (SparqlParser::STRUUID - 89))
      | (1ULL << (SparqlParser::MD5 - 89))
      | (1ULL << (SparqlParser::SHA1 - 89))
      | (1ULL << (SparqlParser::SHA256 - 89))
      | (1ULL << (SparqlParser::SHA384 - 89))
      | (1ULL << (SparqlParser::SHA512 - 89))
      | (1ULL << (SparqlParser::COALESCE - 89))
      | (1ULL << (SparqlParser::IF - 89))
      | (1ULL << (SparqlParser::STRLANG - 89))
      | (1ULL << (SparqlParser::STRDT - 89))
      | (1ULL << (SparqlParser::ISNUMERIC - 89))
      | (1ULL << (SparqlParser::COUNT - 89))
      | (1ULL << (SparqlParser::SUM - 89))
      | (1ULL << (SparqlParser::MIN - 89))
      | (1ULL << (SparqlParser::MAX - 89))
      | (1ULL << (SparqlParser::AVG - 89))
      | (1ULL << (SparqlParser::SAMPLE - 89))
      | (1ULL << (SparqlParser::GROUP_CONCAT - 89))
      | (1ULL << (SparqlParser::NOT - 89))
      | (1ULL << (SparqlParser::EXISTS - 89))
      | (1ULL << (SparqlParser::IRIREF - 89))
      | (1ULL << (SparqlParser::PNAME_NS - 89))
      | (1ULL << (SparqlParser::PNAME_LN - 89))
      | (1ULL << (SparqlParser::VAR1 - 89))
      | (1ULL << (SparqlParser::VAR2 - 89))
      | (1ULL << (SparqlParser::OPEN_BRACE - 89)))) != 0));
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- GroupConditionContext ------------------------------------------------------------------

SparqlParser::GroupConditionContext::GroupConditionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::BuiltInCallContext* SparqlParser::GroupConditionContext::builtInCall() {
  return getRuleContext<SparqlParser::BuiltInCallContext>(0);
}

SparqlParser::FunctionCallContext* SparqlParser::GroupConditionContext::functionCall() {
  return getRuleContext<SparqlParser::FunctionCallContext>(0);
}

tree::TerminalNode* SparqlParser::GroupConditionContext::OPEN_BRACE() {
  return getToken(SparqlParser::OPEN_BRACE, 0);
}

SparqlParser::ExpressionContext* SparqlParser::GroupConditionContext::expression() {
  return getRuleContext<SparqlParser::ExpressionContext>(0);
}

tree::TerminalNode* SparqlParser::GroupConditionContext::CLOSE_BRACE() {
  return getToken(SparqlParser::CLOSE_BRACE, 0);
}

tree::TerminalNode* SparqlParser::GroupConditionContext::AS() {
  return getToken(SparqlParser::AS, 0);
}

SparqlParser::VarContext* SparqlParser::GroupConditionContext::var() {
  return getRuleContext<SparqlParser::VarContext>(0);
}


size_t SparqlParser::GroupConditionContext::getRuleIndex() const {
  return SparqlParser::RuleGroupCondition;
}


antlrcpp::Any SparqlParser::GroupConditionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitGroupCondition(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::GroupConditionContext* SparqlParser::groupCondition() {
  GroupConditionContext *_localctx = _tracker.createInstance<GroupConditionContext>(_ctx, getState());
  enterRule(_localctx, 32, SparqlParser::RuleGroupCondition);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(432);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlParser::STR:
      case SparqlParser::LANG:
      case SparqlParser::LANGMATCHES:
      case SparqlParser::DATATYPE:
      case SparqlParser::BOUND:
      case SparqlParser::SAMETERM:
      case SparqlParser::ISIRI:
      case SparqlParser::ISURI:
      case SparqlParser::ISBLANK:
      case SparqlParser::ISLITERAL:
      case SparqlParser::REGEX:
      case SparqlParser::SUBSTR:
      case SparqlParser::IRI:
      case SparqlParser::URI:
      case SparqlParser::BNODE:
      case SparqlParser::RAND:
      case SparqlParser::ABS:
      case SparqlParser::CEIL:
      case SparqlParser::FLOOR:
      case SparqlParser::ROUND:
      case SparqlParser::CONCAT:
      case SparqlParser::STRLEN:
      case SparqlParser::UCASE:
      case SparqlParser::LCASE:
      case SparqlParser::ENCODE_FOR_URI:
      case SparqlParser::CONTAINS:
      case SparqlParser::STRSTARTS:
      case SparqlParser::STRENDS:
      case SparqlParser::STRBEFORE:
      case SparqlParser::STRAFTER:
      case SparqlParser::REPLACE:
      case SparqlParser::YEAR:
      case SparqlParser::MONTH:
      case SparqlParser::DAY:
      case SparqlParser::HOURS:
      case SparqlParser::MINUTES:
      case SparqlParser::SECONDS:
      case SparqlParser::TIMEZONE:
      case SparqlParser::TZ:
      case SparqlParser::NOW:
      case SparqlParser::UUID:
      case SparqlParser::STRUUID:
      case SparqlParser::MD5:
      case SparqlParser::SHA1:
      case SparqlParser::SHA256:
      case SparqlParser::SHA384:
      case SparqlParser::SHA512:
      case SparqlParser::COALESCE:
      case SparqlParser::IF:
      case SparqlParser::STRLANG:
      case SparqlParser::STRDT:
      case SparqlParser::ISNUMERIC:
      case SparqlParser::COUNT:
      case SparqlParser::SUM:
      case SparqlParser::MIN:
      case SparqlParser::MAX:
      case SparqlParser::AVG:
      case SparqlParser::SAMPLE:
      case SparqlParser::GROUP_CONCAT:
      case SparqlParser::NOT:
      case SparqlParser::EXISTS: {
        enterOuterAlt(_localctx, 1);
        setState(421);
        builtInCall();
        break;
      }

      case SparqlParser::IRIREF:
      case SparqlParser::PNAME_NS:
      case SparqlParser::PNAME_LN: {
        enterOuterAlt(_localctx, 2);
        setState(422);
        functionCall();
        break;
      }

      case SparqlParser::OPEN_BRACE: {
        enterOuterAlt(_localctx, 3);
        setState(423);
        match(SparqlParser::OPEN_BRACE);
        setState(424);
        expression(0);
        setState(427);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == SparqlParser::AS) {
          setState(425);
          match(SparqlParser::AS);
          setState(426);
          var();
        }
        setState(429);
        match(SparqlParser::CLOSE_BRACE);
        break;
      }

      case SparqlParser::VAR1:
      case SparqlParser::VAR2: {
        enterOuterAlt(_localctx, 4);
        setState(431);
        var();
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

//----------------- HavingClauseContext ------------------------------------------------------------------

SparqlParser::HavingClauseContext::HavingClauseContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::HavingClauseContext::HAVING() {
  return getToken(SparqlParser::HAVING, 0);
}

std::vector<SparqlParser::HavingConditionContext *> SparqlParser::HavingClauseContext::havingCondition() {
  return getRuleContexts<SparqlParser::HavingConditionContext>();
}

SparqlParser::HavingConditionContext* SparqlParser::HavingClauseContext::havingCondition(size_t i) {
  return getRuleContext<SparqlParser::HavingConditionContext>(i);
}


size_t SparqlParser::HavingClauseContext::getRuleIndex() const {
  return SparqlParser::RuleHavingClause;
}


antlrcpp::Any SparqlParser::HavingClauseContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitHavingClause(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::HavingClauseContext* SparqlParser::havingClause() {
  HavingClauseContext *_localctx = _tracker.createInstance<HavingClauseContext>(_ctx, getState());
  enterRule(_localctx, 34, SparqlParser::RuleHavingClause);
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
    setState(434);
    match(SparqlParser::HAVING);
    setState(436); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(435);
      havingCondition();
      setState(438); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while (((((_la - 25) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 25)) & ((1ULL << (SparqlParser::STR - 25))
      | (1ULL << (SparqlParser::LANG - 25))
      | (1ULL << (SparqlParser::LANGMATCHES - 25))
      | (1ULL << (SparqlParser::DATATYPE - 25))
      | (1ULL << (SparqlParser::BOUND - 25))
      | (1ULL << (SparqlParser::SAMETERM - 25))
      | (1ULL << (SparqlParser::ISIRI - 25))
      | (1ULL << (SparqlParser::ISURI - 25))
      | (1ULL << (SparqlParser::ISBLANK - 25))
      | (1ULL << (SparqlParser::ISLITERAL - 25))
      | (1ULL << (SparqlParser::REGEX - 25))
      | (1ULL << (SparqlParser::SUBSTR - 25))
      | (1ULL << (SparqlParser::IRI - 25))
      | (1ULL << (SparqlParser::URI - 25))
      | (1ULL << (SparqlParser::BNODE - 25))
      | (1ULL << (SparqlParser::RAND - 25))
      | (1ULL << (SparqlParser::ABS - 25))
      | (1ULL << (SparqlParser::CEIL - 25))
      | (1ULL << (SparqlParser::FLOOR - 25))
      | (1ULL << (SparqlParser::ROUND - 25))
      | (1ULL << (SparqlParser::CONCAT - 25))
      | (1ULL << (SparqlParser::STRLEN - 25))
      | (1ULL << (SparqlParser::UCASE - 25))
      | (1ULL << (SparqlParser::LCASE - 25))
      | (1ULL << (SparqlParser::ENCODE_FOR_URI - 25))
      | (1ULL << (SparqlParser::CONTAINS - 25))
      | (1ULL << (SparqlParser::STRSTARTS - 25))
      | (1ULL << (SparqlParser::STRENDS - 25))
      | (1ULL << (SparqlParser::STRBEFORE - 25))
      | (1ULL << (SparqlParser::STRAFTER - 25))
      | (1ULL << (SparqlParser::REPLACE - 25))
      | (1ULL << (SparqlParser::YEAR - 25))
      | (1ULL << (SparqlParser::MONTH - 25))
      | (1ULL << (SparqlParser::DAY - 25))
      | (1ULL << (SparqlParser::HOURS - 25))
      | (1ULL << (SparqlParser::MINUTES - 25))
      | (1ULL << (SparqlParser::SECONDS - 25)))) != 0) || ((((_la - 89) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 89)) & ((1ULL << (SparqlParser::TIMEZONE - 89))
      | (1ULL << (SparqlParser::TZ - 89))
      | (1ULL << (SparqlParser::NOW - 89))
      | (1ULL << (SparqlParser::UUID - 89))
      | (1ULL << (SparqlParser::STRUUID - 89))
      | (1ULL << (SparqlParser::MD5 - 89))
      | (1ULL << (SparqlParser::SHA1 - 89))
      | (1ULL << (SparqlParser::SHA256 - 89))
      | (1ULL << (SparqlParser::SHA384 - 89))
      | (1ULL << (SparqlParser::SHA512 - 89))
      | (1ULL << (SparqlParser::COALESCE - 89))
      | (1ULL << (SparqlParser::IF - 89))
      | (1ULL << (SparqlParser::STRLANG - 89))
      | (1ULL << (SparqlParser::STRDT - 89))
      | (1ULL << (SparqlParser::ISNUMERIC - 89))
      | (1ULL << (SparqlParser::COUNT - 89))
      | (1ULL << (SparqlParser::SUM - 89))
      | (1ULL << (SparqlParser::MIN - 89))
      | (1ULL << (SparqlParser::MAX - 89))
      | (1ULL << (SparqlParser::AVG - 89))
      | (1ULL << (SparqlParser::SAMPLE - 89))
      | (1ULL << (SparqlParser::GROUP_CONCAT - 89))
      | (1ULL << (SparqlParser::NOT - 89))
      | (1ULL << (SparqlParser::EXISTS - 89))
      | (1ULL << (SparqlParser::IRIREF - 89))
      | (1ULL << (SparqlParser::PNAME_NS - 89))
      | (1ULL << (SparqlParser::PNAME_LN - 89))
      | (1ULL << (SparqlParser::OPEN_BRACE - 89)))) != 0));
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- HavingConditionContext ------------------------------------------------------------------

SparqlParser::HavingConditionContext::HavingConditionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::ConstraintContext* SparqlParser::HavingConditionContext::constraint() {
  return getRuleContext<SparqlParser::ConstraintContext>(0);
}


size_t SparqlParser::HavingConditionContext::getRuleIndex() const {
  return SparqlParser::RuleHavingCondition;
}


antlrcpp::Any SparqlParser::HavingConditionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitHavingCondition(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::HavingConditionContext* SparqlParser::havingCondition() {
  HavingConditionContext *_localctx = _tracker.createInstance<HavingConditionContext>(_ctx, getState());
  enterRule(_localctx, 36, SparqlParser::RuleHavingCondition);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(440);
    constraint();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- OrderClauseContext ------------------------------------------------------------------

SparqlParser::OrderClauseContext::OrderClauseContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::OrderClauseContext::ORDER() {
  return getToken(SparqlParser::ORDER, 0);
}

tree::TerminalNode* SparqlParser::OrderClauseContext::BY() {
  return getToken(SparqlParser::BY, 0);
}

std::vector<SparqlParser::OrderConditionContext *> SparqlParser::OrderClauseContext::orderCondition() {
  return getRuleContexts<SparqlParser::OrderConditionContext>();
}

SparqlParser::OrderConditionContext* SparqlParser::OrderClauseContext::orderCondition(size_t i) {
  return getRuleContext<SparqlParser::OrderConditionContext>(i);
}


size_t SparqlParser::OrderClauseContext::getRuleIndex() const {
  return SparqlParser::RuleOrderClause;
}


antlrcpp::Any SparqlParser::OrderClauseContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitOrderClause(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::OrderClauseContext* SparqlParser::orderClause() {
  OrderClauseContext *_localctx = _tracker.createInstance<OrderClauseContext>(_ctx, getState());
  enterRule(_localctx, 38, SparqlParser::RuleOrderClause);
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
    setState(442);
    match(SparqlParser::ORDER);
    setState(443);
    match(SparqlParser::BY);
    setState(445); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(444);
      orderCondition();
      setState(447); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << SparqlParser::ASC)
      | (1ULL << SparqlParser::DESC)
      | (1ULL << SparqlParser::STR)
      | (1ULL << SparqlParser::LANG)
      | (1ULL << SparqlParser::LANGMATCHES)
      | (1ULL << SparqlParser::DATATYPE)
      | (1ULL << SparqlParser::BOUND)
      | (1ULL << SparqlParser::SAMETERM)
      | (1ULL << SparqlParser::ISIRI)
      | (1ULL << SparqlParser::ISURI)
      | (1ULL << SparqlParser::ISBLANK)
      | (1ULL << SparqlParser::ISLITERAL)
      | (1ULL << SparqlParser::REGEX)
      | (1ULL << SparqlParser::SUBSTR))) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 64)) & ((1ULL << (SparqlParser::IRI - 64))
      | (1ULL << (SparqlParser::URI - 64))
      | (1ULL << (SparqlParser::BNODE - 64))
      | (1ULL << (SparqlParser::RAND - 64))
      | (1ULL << (SparqlParser::ABS - 64))
      | (1ULL << (SparqlParser::CEIL - 64))
      | (1ULL << (SparqlParser::FLOOR - 64))
      | (1ULL << (SparqlParser::ROUND - 64))
      | (1ULL << (SparqlParser::CONCAT - 64))
      | (1ULL << (SparqlParser::STRLEN - 64))
      | (1ULL << (SparqlParser::UCASE - 64))
      | (1ULL << (SparqlParser::LCASE - 64))
      | (1ULL << (SparqlParser::ENCODE_FOR_URI - 64))
      | (1ULL << (SparqlParser::CONTAINS - 64))
      | (1ULL << (SparqlParser::STRSTARTS - 64))
      | (1ULL << (SparqlParser::STRENDS - 64))
      | (1ULL << (SparqlParser::STRBEFORE - 64))
      | (1ULL << (SparqlParser::STRAFTER - 64))
      | (1ULL << (SparqlParser::REPLACE - 64))
      | (1ULL << (SparqlParser::YEAR - 64))
      | (1ULL << (SparqlParser::MONTH - 64))
      | (1ULL << (SparqlParser::DAY - 64))
      | (1ULL << (SparqlParser::HOURS - 64))
      | (1ULL << (SparqlParser::MINUTES - 64))
      | (1ULL << (SparqlParser::SECONDS - 64))
      | (1ULL << (SparqlParser::TIMEZONE - 64))
      | (1ULL << (SparqlParser::TZ - 64))
      | (1ULL << (SparqlParser::NOW - 64))
      | (1ULL << (SparqlParser::UUID - 64))
      | (1ULL << (SparqlParser::STRUUID - 64))
      | (1ULL << (SparqlParser::MD5 - 64))
      | (1ULL << (SparqlParser::SHA1 - 64))
      | (1ULL << (SparqlParser::SHA256 - 64))
      | (1ULL << (SparqlParser::SHA384 - 64))
      | (1ULL << (SparqlParser::SHA512 - 64))
      | (1ULL << (SparqlParser::COALESCE - 64))
      | (1ULL << (SparqlParser::IF - 64))
      | (1ULL << (SparqlParser::STRLANG - 64))
      | (1ULL << (SparqlParser::STRDT - 64))
      | (1ULL << (SparqlParser::ISNUMERIC - 64))
      | (1ULL << (SparqlParser::COUNT - 64))
      | (1ULL << (SparqlParser::SUM - 64))
      | (1ULL << (SparqlParser::MIN - 64))
      | (1ULL << (SparqlParser::MAX - 64))
      | (1ULL << (SparqlParser::AVG - 64))
      | (1ULL << (SparqlParser::SAMPLE - 64))
      | (1ULL << (SparqlParser::GROUP_CONCAT - 64))
      | (1ULL << (SparqlParser::NOT - 64))
      | (1ULL << (SparqlParser::EXISTS - 64))
      | (1ULL << (SparqlParser::IRIREF - 64))
      | (1ULL << (SparqlParser::PNAME_NS - 64))
      | (1ULL << (SparqlParser::PNAME_LN - 64))
      | (1ULL << (SparqlParser::VAR1 - 64))
      | (1ULL << (SparqlParser::VAR2 - 64)))) != 0) || _la == SparqlParser::OPEN_BRACE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- OrderConditionContext ------------------------------------------------------------------

SparqlParser::OrderConditionContext::OrderConditionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::OrderConditionContext::OPEN_BRACE() {
  return getToken(SparqlParser::OPEN_BRACE, 0);
}

SparqlParser::ExpressionContext* SparqlParser::OrderConditionContext::expression() {
  return getRuleContext<SparqlParser::ExpressionContext>(0);
}

tree::TerminalNode* SparqlParser::OrderConditionContext::CLOSE_BRACE() {
  return getToken(SparqlParser::CLOSE_BRACE, 0);
}

tree::TerminalNode* SparqlParser::OrderConditionContext::ASC() {
  return getToken(SparqlParser::ASC, 0);
}

tree::TerminalNode* SparqlParser::OrderConditionContext::DESC() {
  return getToken(SparqlParser::DESC, 0);
}

SparqlParser::ConstraintContext* SparqlParser::OrderConditionContext::constraint() {
  return getRuleContext<SparqlParser::ConstraintContext>(0);
}

SparqlParser::VarContext* SparqlParser::OrderConditionContext::var() {
  return getRuleContext<SparqlParser::VarContext>(0);
}


size_t SparqlParser::OrderConditionContext::getRuleIndex() const {
  return SparqlParser::RuleOrderCondition;
}


antlrcpp::Any SparqlParser::OrderConditionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitOrderCondition(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::OrderConditionContext* SparqlParser::orderCondition() {
  OrderConditionContext *_localctx = _tracker.createInstance<OrderConditionContext>(_ctx, getState());
  enterRule(_localctx, 40, SparqlParser::RuleOrderCondition);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(456);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlParser::ASC:
      case SparqlParser::DESC: {
        enterOuterAlt(_localctx, 1);
        setState(449);
        _la = _input->LA(1);
        if (!(_la == SparqlParser::ASC

        || _la == SparqlParser::DESC)) {
        _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        setState(450);
        match(SparqlParser::OPEN_BRACE);
        setState(451);
        expression(0);
        setState(452);
        match(SparqlParser::CLOSE_BRACE);
        break;
      }

      case SparqlParser::STR:
      case SparqlParser::LANG:
      case SparqlParser::LANGMATCHES:
      case SparqlParser::DATATYPE:
      case SparqlParser::BOUND:
      case SparqlParser::SAMETERM:
      case SparqlParser::ISIRI:
      case SparqlParser::ISURI:
      case SparqlParser::ISBLANK:
      case SparqlParser::ISLITERAL:
      case SparqlParser::REGEX:
      case SparqlParser::SUBSTR:
      case SparqlParser::IRI:
      case SparqlParser::URI:
      case SparqlParser::BNODE:
      case SparqlParser::RAND:
      case SparqlParser::ABS:
      case SparqlParser::CEIL:
      case SparqlParser::FLOOR:
      case SparqlParser::ROUND:
      case SparqlParser::CONCAT:
      case SparqlParser::STRLEN:
      case SparqlParser::UCASE:
      case SparqlParser::LCASE:
      case SparqlParser::ENCODE_FOR_URI:
      case SparqlParser::CONTAINS:
      case SparqlParser::STRSTARTS:
      case SparqlParser::STRENDS:
      case SparqlParser::STRBEFORE:
      case SparqlParser::STRAFTER:
      case SparqlParser::REPLACE:
      case SparqlParser::YEAR:
      case SparqlParser::MONTH:
      case SparqlParser::DAY:
      case SparqlParser::HOURS:
      case SparqlParser::MINUTES:
      case SparqlParser::SECONDS:
      case SparqlParser::TIMEZONE:
      case SparqlParser::TZ:
      case SparqlParser::NOW:
      case SparqlParser::UUID:
      case SparqlParser::STRUUID:
      case SparqlParser::MD5:
      case SparqlParser::SHA1:
      case SparqlParser::SHA256:
      case SparqlParser::SHA384:
      case SparqlParser::SHA512:
      case SparqlParser::COALESCE:
      case SparqlParser::IF:
      case SparqlParser::STRLANG:
      case SparqlParser::STRDT:
      case SparqlParser::ISNUMERIC:
      case SparqlParser::COUNT:
      case SparqlParser::SUM:
      case SparqlParser::MIN:
      case SparqlParser::MAX:
      case SparqlParser::AVG:
      case SparqlParser::SAMPLE:
      case SparqlParser::GROUP_CONCAT:
      case SparqlParser::NOT:
      case SparqlParser::EXISTS:
      case SparqlParser::IRIREF:
      case SparqlParser::PNAME_NS:
      case SparqlParser::PNAME_LN:
      case SparqlParser::OPEN_BRACE: {
        enterOuterAlt(_localctx, 2);
        setState(454);
        constraint();
        break;
      }

      case SparqlParser::VAR1:
      case SparqlParser::VAR2: {
        enterOuterAlt(_localctx, 3);
        setState(455);
        var();
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

//----------------- LimitOffsetClausesContext ------------------------------------------------------------------

SparqlParser::LimitOffsetClausesContext::LimitOffsetClausesContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::LimitClauseContext* SparqlParser::LimitOffsetClausesContext::limitClause() {
  return getRuleContext<SparqlParser::LimitClauseContext>(0);
}

SparqlParser::OffsetClauseContext* SparqlParser::LimitOffsetClausesContext::offsetClause() {
  return getRuleContext<SparqlParser::OffsetClauseContext>(0);
}


size_t SparqlParser::LimitOffsetClausesContext::getRuleIndex() const {
  return SparqlParser::RuleLimitOffsetClauses;
}


antlrcpp::Any SparqlParser::LimitOffsetClausesContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitLimitOffsetClauses(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::LimitOffsetClausesContext* SparqlParser::limitOffsetClauses() {
  LimitOffsetClausesContext *_localctx = _tracker.createInstance<LimitOffsetClausesContext>(_ctx, getState());
  enterRule(_localctx, 42, SparqlParser::RuleLimitOffsetClauses);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(466);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlParser::LIMIT: {
        enterOuterAlt(_localctx, 1);
        setState(458);
        limitClause();
        setState(460);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == SparqlParser::OFFSET) {
          setState(459);
          offsetClause();
        }
        break;
      }

      case SparqlParser::OFFSET: {
        enterOuterAlt(_localctx, 2);
        setState(462);
        offsetClause();
        setState(464);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == SparqlParser::LIMIT) {
          setState(463);
          limitClause();
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

//----------------- LimitClauseContext ------------------------------------------------------------------

SparqlParser::LimitClauseContext::LimitClauseContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::LimitClauseContext::LIMIT() {
  return getToken(SparqlParser::LIMIT, 0);
}

tree::TerminalNode* SparqlParser::LimitClauseContext::INTEGER() {
  return getToken(SparqlParser::INTEGER, 0);
}


size_t SparqlParser::LimitClauseContext::getRuleIndex() const {
  return SparqlParser::RuleLimitClause;
}


antlrcpp::Any SparqlParser::LimitClauseContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitLimitClause(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::LimitClauseContext* SparqlParser::limitClause() {
  LimitClauseContext *_localctx = _tracker.createInstance<LimitClauseContext>(_ctx, getState());
  enterRule(_localctx, 44, SparqlParser::RuleLimitClause);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(468);
    match(SparqlParser::LIMIT);
    setState(469);
    match(SparqlParser::INTEGER);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- OffsetClauseContext ------------------------------------------------------------------

SparqlParser::OffsetClauseContext::OffsetClauseContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::OffsetClauseContext::OFFSET() {
  return getToken(SparqlParser::OFFSET, 0);
}

tree::TerminalNode* SparqlParser::OffsetClauseContext::INTEGER() {
  return getToken(SparqlParser::INTEGER, 0);
}


size_t SparqlParser::OffsetClauseContext::getRuleIndex() const {
  return SparqlParser::RuleOffsetClause;
}


antlrcpp::Any SparqlParser::OffsetClauseContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitOffsetClause(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::OffsetClauseContext* SparqlParser::offsetClause() {
  OffsetClauseContext *_localctx = _tracker.createInstance<OffsetClauseContext>(_ctx, getState());
  enterRule(_localctx, 46, SparqlParser::RuleOffsetClause);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(471);
    match(SparqlParser::OFFSET);
    setState(472);
    match(SparqlParser::INTEGER);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ValuesClauseContext ------------------------------------------------------------------

SparqlParser::ValuesClauseContext::ValuesClauseContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::ValuesClauseContext::VALUES() {
  return getToken(SparqlParser::VALUES, 0);
}

SparqlParser::DataBlockContext* SparqlParser::ValuesClauseContext::dataBlock() {
  return getRuleContext<SparqlParser::DataBlockContext>(0);
}


size_t SparqlParser::ValuesClauseContext::getRuleIndex() const {
  return SparqlParser::RuleValuesClause;
}


antlrcpp::Any SparqlParser::ValuesClauseContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitValuesClause(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::ValuesClauseContext* SparqlParser::valuesClause() {
  ValuesClauseContext *_localctx = _tracker.createInstance<ValuesClauseContext>(_ctx, getState());
  enterRule(_localctx, 48, SparqlParser::RuleValuesClause);
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
    setState(476);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == SparqlParser::VALUES) {
      setState(474);
      match(SparqlParser::VALUES);
      setState(475);
      dataBlock();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- UpdateCommandContext ------------------------------------------------------------------

SparqlParser::UpdateCommandContext::UpdateCommandContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<SparqlParser::PrologueContext *> SparqlParser::UpdateCommandContext::prologue() {
  return getRuleContexts<SparqlParser::PrologueContext>();
}

SparqlParser::PrologueContext* SparqlParser::UpdateCommandContext::prologue(size_t i) {
  return getRuleContext<SparqlParser::PrologueContext>(i);
}

std::vector<SparqlParser::UpdateContext *> SparqlParser::UpdateCommandContext::update() {
  return getRuleContexts<SparqlParser::UpdateContext>();
}

SparqlParser::UpdateContext* SparqlParser::UpdateCommandContext::update(size_t i) {
  return getRuleContext<SparqlParser::UpdateContext>(i);
}

std::vector<tree::TerminalNode *> SparqlParser::UpdateCommandContext::SEMICOLON() {
  return getTokens(SparqlParser::SEMICOLON);
}

tree::TerminalNode* SparqlParser::UpdateCommandContext::SEMICOLON(size_t i) {
  return getToken(SparqlParser::SEMICOLON, i);
}


size_t SparqlParser::UpdateCommandContext::getRuleIndex() const {
  return SparqlParser::RuleUpdateCommand;
}


antlrcpp::Any SparqlParser::UpdateCommandContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitUpdateCommand(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::UpdateCommandContext* SparqlParser::updateCommand() {
  UpdateCommandContext *_localctx = _tracker.createInstance<UpdateCommandContext>(_ctx, getState());
  enterRule(_localctx, 50, SparqlParser::RuleUpdateCommand);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(478);
    prologue();
    setState(493);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << SparqlParser::LOAD)
      | (1ULL << SparqlParser::CLEAR)
      | (1ULL << SparqlParser::DROP)
      | (1ULL << SparqlParser::ADD)
      | (1ULL << SparqlParser::MOVE)
      | (1ULL << SparqlParser::COPY)
      | (1ULL << SparqlParser::CREATE)
      | (1ULL << SparqlParser::DELETE)
      | (1ULL << SparqlParser::INSERT)
      | (1ULL << SparqlParser::WITH))) != 0)) {
      setState(479);
      update();
      setState(486);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 34, _ctx);
      while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
        if (alt == 1) {
          setState(480);
          match(SparqlParser::SEMICOLON);
          setState(481);
          prologue();
          setState(482);
          update(); 
        }
        setState(488);
        _errHandler->sync(this);
        alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 34, _ctx);
      }
      setState(491);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == SparqlParser::SEMICOLON) {
        setState(489);
        match(SparqlParser::SEMICOLON);
        setState(490);
        prologue();
      }
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- UpdateContext ------------------------------------------------------------------

SparqlParser::UpdateContext::UpdateContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::LoadContext* SparqlParser::UpdateContext::load() {
  return getRuleContext<SparqlParser::LoadContext>(0);
}

SparqlParser::ClearContext* SparqlParser::UpdateContext::clear() {
  return getRuleContext<SparqlParser::ClearContext>(0);
}

SparqlParser::DropContext* SparqlParser::UpdateContext::drop() {
  return getRuleContext<SparqlParser::DropContext>(0);
}

SparqlParser::AddContext* SparqlParser::UpdateContext::add() {
  return getRuleContext<SparqlParser::AddContext>(0);
}

SparqlParser::MoveContext* SparqlParser::UpdateContext::move() {
  return getRuleContext<SparqlParser::MoveContext>(0);
}

SparqlParser::CopyContext* SparqlParser::UpdateContext::copy() {
  return getRuleContext<SparqlParser::CopyContext>(0);
}

SparqlParser::CreateContext* SparqlParser::UpdateContext::create() {
  return getRuleContext<SparqlParser::CreateContext>(0);
}

SparqlParser::InsertDataContext* SparqlParser::UpdateContext::insertData() {
  return getRuleContext<SparqlParser::InsertDataContext>(0);
}

SparqlParser::DeleteDataContext* SparqlParser::UpdateContext::deleteData() {
  return getRuleContext<SparqlParser::DeleteDataContext>(0);
}

SparqlParser::DeleteWhereContext* SparqlParser::UpdateContext::deleteWhere() {
  return getRuleContext<SparqlParser::DeleteWhereContext>(0);
}

SparqlParser::ModifyContext* SparqlParser::UpdateContext::modify() {
  return getRuleContext<SparqlParser::ModifyContext>(0);
}


size_t SparqlParser::UpdateContext::getRuleIndex() const {
  return SparqlParser::RuleUpdate;
}


antlrcpp::Any SparqlParser::UpdateContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitUpdate(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::UpdateContext* SparqlParser::update() {
  UpdateContext *_localctx = _tracker.createInstance<UpdateContext>(_ctx, getState());
  enterRule(_localctx, 52, SparqlParser::RuleUpdate);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(506);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 37, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(495);
      load();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(496);
      clear();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(497);
      drop();
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(498);
      add();
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(499);
      move();
      break;
    }

    case 6: {
      enterOuterAlt(_localctx, 6);
      setState(500);
      copy();
      break;
    }

    case 7: {
      enterOuterAlt(_localctx, 7);
      setState(501);
      create();
      break;
    }

    case 8: {
      enterOuterAlt(_localctx, 8);
      setState(502);
      insertData();
      break;
    }

    case 9: {
      enterOuterAlt(_localctx, 9);
      setState(503);
      deleteData();
      break;
    }

    case 10: {
      enterOuterAlt(_localctx, 10);
      setState(504);
      deleteWhere();
      break;
    }

    case 11: {
      enterOuterAlt(_localctx, 11);
      setState(505);
      modify();
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

//----------------- LoadContext ------------------------------------------------------------------

SparqlParser::LoadContext::LoadContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::LoadContext::LOAD() {
  return getToken(SparqlParser::LOAD, 0);
}

SparqlParser::IriContext* SparqlParser::LoadContext::iri() {
  return getRuleContext<SparqlParser::IriContext>(0);
}

tree::TerminalNode* SparqlParser::LoadContext::SILENT() {
  return getToken(SparqlParser::SILENT, 0);
}

tree::TerminalNode* SparqlParser::LoadContext::INTO() {
  return getToken(SparqlParser::INTO, 0);
}

SparqlParser::GraphRefContext* SparqlParser::LoadContext::graphRef() {
  return getRuleContext<SparqlParser::GraphRefContext>(0);
}


size_t SparqlParser::LoadContext::getRuleIndex() const {
  return SparqlParser::RuleLoad;
}


antlrcpp::Any SparqlParser::LoadContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitLoad(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::LoadContext* SparqlParser::load() {
  LoadContext *_localctx = _tracker.createInstance<LoadContext>(_ctx, getState());
  enterRule(_localctx, 54, SparqlParser::RuleLoad);
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
    setState(508);
    match(SparqlParser::LOAD);
    setState(510);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == SparqlParser::SILENT) {
      setState(509);
      match(SparqlParser::SILENT);
    }
    setState(512);
    iri();
    setState(515);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == SparqlParser::INTO) {
      setState(513);
      match(SparqlParser::INTO);
      setState(514);
      graphRef();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ClearContext ------------------------------------------------------------------

SparqlParser::ClearContext::ClearContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::ClearContext::CLEAR() {
  return getToken(SparqlParser::CLEAR, 0);
}

SparqlParser::GraphRefAllContext* SparqlParser::ClearContext::graphRefAll() {
  return getRuleContext<SparqlParser::GraphRefAllContext>(0);
}

tree::TerminalNode* SparqlParser::ClearContext::SILENT() {
  return getToken(SparqlParser::SILENT, 0);
}


size_t SparqlParser::ClearContext::getRuleIndex() const {
  return SparqlParser::RuleClear;
}


antlrcpp::Any SparqlParser::ClearContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitClear(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::ClearContext* SparqlParser::clear() {
  ClearContext *_localctx = _tracker.createInstance<ClearContext>(_ctx, getState());
  enterRule(_localctx, 56, SparqlParser::RuleClear);
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
    setState(517);
    match(SparqlParser::CLEAR);
    setState(519);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == SparqlParser::SILENT) {
      setState(518);
      match(SparqlParser::SILENT);
    }
    setState(521);
    graphRefAll();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DropContext ------------------------------------------------------------------

SparqlParser::DropContext::DropContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::DropContext::DROP() {
  return getToken(SparqlParser::DROP, 0);
}

SparqlParser::GraphRefAllContext* SparqlParser::DropContext::graphRefAll() {
  return getRuleContext<SparqlParser::GraphRefAllContext>(0);
}

tree::TerminalNode* SparqlParser::DropContext::SILENT() {
  return getToken(SparqlParser::SILENT, 0);
}


size_t SparqlParser::DropContext::getRuleIndex() const {
  return SparqlParser::RuleDrop;
}


antlrcpp::Any SparqlParser::DropContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitDrop(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::DropContext* SparqlParser::drop() {
  DropContext *_localctx = _tracker.createInstance<DropContext>(_ctx, getState());
  enterRule(_localctx, 58, SparqlParser::RuleDrop);
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
    setState(523);
    match(SparqlParser::DROP);
    setState(525);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == SparqlParser::SILENT) {
      setState(524);
      match(SparqlParser::SILENT);
    }
    setState(527);
    graphRefAll();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CreateContext ------------------------------------------------------------------

SparqlParser::CreateContext::CreateContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::CreateContext::CREATE() {
  return getToken(SparqlParser::CREATE, 0);
}

SparqlParser::GraphRefContext* SparqlParser::CreateContext::graphRef() {
  return getRuleContext<SparqlParser::GraphRefContext>(0);
}

tree::TerminalNode* SparqlParser::CreateContext::SILENT() {
  return getToken(SparqlParser::SILENT, 0);
}


size_t SparqlParser::CreateContext::getRuleIndex() const {
  return SparqlParser::RuleCreate;
}


antlrcpp::Any SparqlParser::CreateContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitCreate(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::CreateContext* SparqlParser::create() {
  CreateContext *_localctx = _tracker.createInstance<CreateContext>(_ctx, getState());
  enterRule(_localctx, 60, SparqlParser::RuleCreate);
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
    setState(529);
    match(SparqlParser::CREATE);
    setState(531);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == SparqlParser::SILENT) {
      setState(530);
      match(SparqlParser::SILENT);
    }
    setState(533);
    graphRef();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AddContext ------------------------------------------------------------------

SparqlParser::AddContext::AddContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::AddContext::ADD() {
  return getToken(SparqlParser::ADD, 0);
}

std::vector<SparqlParser::GraphOrDefaultContext *> SparqlParser::AddContext::graphOrDefault() {
  return getRuleContexts<SparqlParser::GraphOrDefaultContext>();
}

SparqlParser::GraphOrDefaultContext* SparqlParser::AddContext::graphOrDefault(size_t i) {
  return getRuleContext<SparqlParser::GraphOrDefaultContext>(i);
}

tree::TerminalNode* SparqlParser::AddContext::TO() {
  return getToken(SparqlParser::TO, 0);
}

tree::TerminalNode* SparqlParser::AddContext::SILENT() {
  return getToken(SparqlParser::SILENT, 0);
}


size_t SparqlParser::AddContext::getRuleIndex() const {
  return SparqlParser::RuleAdd;
}


antlrcpp::Any SparqlParser::AddContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitAdd(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::AddContext* SparqlParser::add() {
  AddContext *_localctx = _tracker.createInstance<AddContext>(_ctx, getState());
  enterRule(_localctx, 62, SparqlParser::RuleAdd);
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
    setState(535);
    match(SparqlParser::ADD);
    setState(537);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == SparqlParser::SILENT) {
      setState(536);
      match(SparqlParser::SILENT);
    }
    setState(539);
    graphOrDefault();
    setState(540);
    match(SparqlParser::TO);
    setState(541);
    graphOrDefault();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- MoveContext ------------------------------------------------------------------

SparqlParser::MoveContext::MoveContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::MoveContext::MOVE() {
  return getToken(SparqlParser::MOVE, 0);
}

std::vector<SparqlParser::GraphOrDefaultContext *> SparqlParser::MoveContext::graphOrDefault() {
  return getRuleContexts<SparqlParser::GraphOrDefaultContext>();
}

SparqlParser::GraphOrDefaultContext* SparqlParser::MoveContext::graphOrDefault(size_t i) {
  return getRuleContext<SparqlParser::GraphOrDefaultContext>(i);
}

tree::TerminalNode* SparqlParser::MoveContext::TO() {
  return getToken(SparqlParser::TO, 0);
}

tree::TerminalNode* SparqlParser::MoveContext::SILENT() {
  return getToken(SparqlParser::SILENT, 0);
}


size_t SparqlParser::MoveContext::getRuleIndex() const {
  return SparqlParser::RuleMove;
}


antlrcpp::Any SparqlParser::MoveContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitMove(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::MoveContext* SparqlParser::move() {
  MoveContext *_localctx = _tracker.createInstance<MoveContext>(_ctx, getState());
  enterRule(_localctx, 64, SparqlParser::RuleMove);
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
    setState(543);
    match(SparqlParser::MOVE);
    setState(545);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == SparqlParser::SILENT) {
      setState(544);
      match(SparqlParser::SILENT);
    }
    setState(547);
    graphOrDefault();
    setState(548);
    match(SparqlParser::TO);
    setState(549);
    graphOrDefault();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CopyContext ------------------------------------------------------------------

SparqlParser::CopyContext::CopyContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::CopyContext::COPY() {
  return getToken(SparqlParser::COPY, 0);
}

std::vector<SparqlParser::GraphOrDefaultContext *> SparqlParser::CopyContext::graphOrDefault() {
  return getRuleContexts<SparqlParser::GraphOrDefaultContext>();
}

SparqlParser::GraphOrDefaultContext* SparqlParser::CopyContext::graphOrDefault(size_t i) {
  return getRuleContext<SparqlParser::GraphOrDefaultContext>(i);
}

tree::TerminalNode* SparqlParser::CopyContext::TO() {
  return getToken(SparqlParser::TO, 0);
}

tree::TerminalNode* SparqlParser::CopyContext::SILENT() {
  return getToken(SparqlParser::SILENT, 0);
}


size_t SparqlParser::CopyContext::getRuleIndex() const {
  return SparqlParser::RuleCopy;
}


antlrcpp::Any SparqlParser::CopyContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitCopy(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::CopyContext* SparqlParser::copy() {
  CopyContext *_localctx = _tracker.createInstance<CopyContext>(_ctx, getState());
  enterRule(_localctx, 66, SparqlParser::RuleCopy);
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
    setState(551);
    match(SparqlParser::COPY);
    setState(553);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == SparqlParser::SILENT) {
      setState(552);
      match(SparqlParser::SILENT);
    }
    setState(555);
    graphOrDefault();
    setState(556);
    match(SparqlParser::TO);
    setState(557);
    graphOrDefault();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- InsertDataContext ------------------------------------------------------------------

SparqlParser::InsertDataContext::InsertDataContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::InsertDataContext::INSERT() {
  return getToken(SparqlParser::INSERT, 0);
}

tree::TerminalNode* SparqlParser::InsertDataContext::DATA() {
  return getToken(SparqlParser::DATA, 0);
}

SparqlParser::QuadDataContext* SparqlParser::InsertDataContext::quadData() {
  return getRuleContext<SparqlParser::QuadDataContext>(0);
}


size_t SparqlParser::InsertDataContext::getRuleIndex() const {
  return SparqlParser::RuleInsertData;
}


antlrcpp::Any SparqlParser::InsertDataContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitInsertData(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::InsertDataContext* SparqlParser::insertData() {
  InsertDataContext *_localctx = _tracker.createInstance<InsertDataContext>(_ctx, getState());
  enterRule(_localctx, 68, SparqlParser::RuleInsertData);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(559);
    match(SparqlParser::INSERT);
    setState(560);
    match(SparqlParser::DATA);
    setState(561);
    quadData();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DeleteDataContext ------------------------------------------------------------------

SparqlParser::DeleteDataContext::DeleteDataContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::DeleteDataContext::DELETE() {
  return getToken(SparqlParser::DELETE, 0);
}

tree::TerminalNode* SparqlParser::DeleteDataContext::DATA() {
  return getToken(SparqlParser::DATA, 0);
}

SparqlParser::QuadDataContext* SparqlParser::DeleteDataContext::quadData() {
  return getRuleContext<SparqlParser::QuadDataContext>(0);
}


size_t SparqlParser::DeleteDataContext::getRuleIndex() const {
  return SparqlParser::RuleDeleteData;
}


antlrcpp::Any SparqlParser::DeleteDataContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitDeleteData(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::DeleteDataContext* SparqlParser::deleteData() {
  DeleteDataContext *_localctx = _tracker.createInstance<DeleteDataContext>(_ctx, getState());
  enterRule(_localctx, 70, SparqlParser::RuleDeleteData);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(563);
    match(SparqlParser::DELETE);
    setState(564);
    match(SparqlParser::DATA);
    setState(565);
    quadData();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DeleteWhereContext ------------------------------------------------------------------

SparqlParser::DeleteWhereContext::DeleteWhereContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::DeleteWhereContext::DELETE() {
  return getToken(SparqlParser::DELETE, 0);
}

tree::TerminalNode* SparqlParser::DeleteWhereContext::WHERE() {
  return getToken(SparqlParser::WHERE, 0);
}

SparqlParser::QuadPatternContext* SparqlParser::DeleteWhereContext::quadPattern() {
  return getRuleContext<SparqlParser::QuadPatternContext>(0);
}


size_t SparqlParser::DeleteWhereContext::getRuleIndex() const {
  return SparqlParser::RuleDeleteWhere;
}


antlrcpp::Any SparqlParser::DeleteWhereContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitDeleteWhere(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::DeleteWhereContext* SparqlParser::deleteWhere() {
  DeleteWhereContext *_localctx = _tracker.createInstance<DeleteWhereContext>(_ctx, getState());
  enterRule(_localctx, 72, SparqlParser::RuleDeleteWhere);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(567);
    match(SparqlParser::DELETE);
    setState(568);
    match(SparqlParser::WHERE);
    setState(569);
    quadPattern();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ModifyContext ------------------------------------------------------------------

SparqlParser::ModifyContext::ModifyContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::ModifyContext::WHERE() {
  return getToken(SparqlParser::WHERE, 0);
}

SparqlParser::GroupGraphPatternContext* SparqlParser::ModifyContext::groupGraphPattern() {
  return getRuleContext<SparqlParser::GroupGraphPatternContext>(0);
}

SparqlParser::DeleteClauseContext* SparqlParser::ModifyContext::deleteClause() {
  return getRuleContext<SparqlParser::DeleteClauseContext>(0);
}

SparqlParser::InsertClauseContext* SparqlParser::ModifyContext::insertClause() {
  return getRuleContext<SparqlParser::InsertClauseContext>(0);
}

tree::TerminalNode* SparqlParser::ModifyContext::WITH() {
  return getToken(SparqlParser::WITH, 0);
}

SparqlParser::IriContext* SparqlParser::ModifyContext::iri() {
  return getRuleContext<SparqlParser::IriContext>(0);
}

std::vector<SparqlParser::UsingClauseContext *> SparqlParser::ModifyContext::usingClause() {
  return getRuleContexts<SparqlParser::UsingClauseContext>();
}

SparqlParser::UsingClauseContext* SparqlParser::ModifyContext::usingClause(size_t i) {
  return getRuleContext<SparqlParser::UsingClauseContext>(i);
}


size_t SparqlParser::ModifyContext::getRuleIndex() const {
  return SparqlParser::RuleModify;
}


antlrcpp::Any SparqlParser::ModifyContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitModify(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::ModifyContext* SparqlParser::modify() {
  ModifyContext *_localctx = _tracker.createInstance<ModifyContext>(_ctx, getState());
  enterRule(_localctx, 74, SparqlParser::RuleModify);
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
    setState(573);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == SparqlParser::WITH) {
      setState(571);
      match(SparqlParser::WITH);
      setState(572);
      iri();
    }
    setState(580);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlParser::DELETE: {
        setState(575);
        deleteClause();
        setState(577);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == SparqlParser::INSERT) {
          setState(576);
          insertClause();
        }
        break;
      }

      case SparqlParser::INSERT: {
        setState(579);
        insertClause();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    setState(585);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SparqlParser::USING) {
      setState(582);
      usingClause();
      setState(587);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(588);
    match(SparqlParser::WHERE);
    setState(589);
    groupGraphPattern();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DeleteClauseContext ------------------------------------------------------------------

SparqlParser::DeleteClauseContext::DeleteClauseContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::DeleteClauseContext::DELETE() {
  return getToken(SparqlParser::DELETE, 0);
}

SparqlParser::QuadPatternContext* SparqlParser::DeleteClauseContext::quadPattern() {
  return getRuleContext<SparqlParser::QuadPatternContext>(0);
}


size_t SparqlParser::DeleteClauseContext::getRuleIndex() const {
  return SparqlParser::RuleDeleteClause;
}


antlrcpp::Any SparqlParser::DeleteClauseContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitDeleteClause(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::DeleteClauseContext* SparqlParser::deleteClause() {
  DeleteClauseContext *_localctx = _tracker.createInstance<DeleteClauseContext>(_ctx, getState());
  enterRule(_localctx, 76, SparqlParser::RuleDeleteClause);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(591);
    match(SparqlParser::DELETE);
    setState(592);
    quadPattern();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- InsertClauseContext ------------------------------------------------------------------

SparqlParser::InsertClauseContext::InsertClauseContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::InsertClauseContext::INSERT() {
  return getToken(SparqlParser::INSERT, 0);
}

SparqlParser::QuadPatternContext* SparqlParser::InsertClauseContext::quadPattern() {
  return getRuleContext<SparqlParser::QuadPatternContext>(0);
}


size_t SparqlParser::InsertClauseContext::getRuleIndex() const {
  return SparqlParser::RuleInsertClause;
}


antlrcpp::Any SparqlParser::InsertClauseContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitInsertClause(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::InsertClauseContext* SparqlParser::insertClause() {
  InsertClauseContext *_localctx = _tracker.createInstance<InsertClauseContext>(_ctx, getState());
  enterRule(_localctx, 78, SparqlParser::RuleInsertClause);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(594);
    match(SparqlParser::INSERT);
    setState(595);
    quadPattern();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- UsingClauseContext ------------------------------------------------------------------

SparqlParser::UsingClauseContext::UsingClauseContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::UsingClauseContext::USING() {
  return getToken(SparqlParser::USING, 0);
}

SparqlParser::IriContext* SparqlParser::UsingClauseContext::iri() {
  return getRuleContext<SparqlParser::IriContext>(0);
}

tree::TerminalNode* SparqlParser::UsingClauseContext::NAMED() {
  return getToken(SparqlParser::NAMED, 0);
}


size_t SparqlParser::UsingClauseContext::getRuleIndex() const {
  return SparqlParser::RuleUsingClause;
}


antlrcpp::Any SparqlParser::UsingClauseContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitUsingClause(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::UsingClauseContext* SparqlParser::usingClause() {
  UsingClauseContext *_localctx = _tracker.createInstance<UsingClauseContext>(_ctx, getState());
  enterRule(_localctx, 80, SparqlParser::RuleUsingClause);
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
    setState(597);
    match(SparqlParser::USING);
    setState(599);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == SparqlParser::NAMED) {
      setState(598);
      match(SparqlParser::NAMED);
    }
    setState(601);
    iri();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- GraphOrDefaultContext ------------------------------------------------------------------

SparqlParser::GraphOrDefaultContext::GraphOrDefaultContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::GraphOrDefaultContext::DEFAULT() {
  return getToken(SparqlParser::DEFAULT, 0);
}

SparqlParser::IriContext* SparqlParser::GraphOrDefaultContext::iri() {
  return getRuleContext<SparqlParser::IriContext>(0);
}

tree::TerminalNode* SparqlParser::GraphOrDefaultContext::GRAPH() {
  return getToken(SparqlParser::GRAPH, 0);
}


size_t SparqlParser::GraphOrDefaultContext::getRuleIndex() const {
  return SparqlParser::RuleGraphOrDefault;
}


antlrcpp::Any SparqlParser::GraphOrDefaultContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitGraphOrDefault(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::GraphOrDefaultContext* SparqlParser::graphOrDefault() {
  GraphOrDefaultContext *_localctx = _tracker.createInstance<GraphOrDefaultContext>(_ctx, getState());
  enterRule(_localctx, 82, SparqlParser::RuleGraphOrDefault);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(608);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlParser::DEFAULT: {
        enterOuterAlt(_localctx, 1);
        setState(603);
        match(SparqlParser::DEFAULT);
        break;
      }

      case SparqlParser::GRAPH:
      case SparqlParser::IRIREF:
      case SparqlParser::PNAME_NS:
      case SparqlParser::PNAME_LN: {
        enterOuterAlt(_localctx, 2);
        setState(605);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == SparqlParser::GRAPH) {
          setState(604);
          match(SparqlParser::GRAPH);
        }
        setState(607);
        iri();
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

//----------------- GraphRefContext ------------------------------------------------------------------

SparqlParser::GraphRefContext::GraphRefContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::GraphRefContext::GRAPH() {
  return getToken(SparqlParser::GRAPH, 0);
}

SparqlParser::IriContext* SparqlParser::GraphRefContext::iri() {
  return getRuleContext<SparqlParser::IriContext>(0);
}


size_t SparqlParser::GraphRefContext::getRuleIndex() const {
  return SparqlParser::RuleGraphRef;
}


antlrcpp::Any SparqlParser::GraphRefContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitGraphRef(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::GraphRefContext* SparqlParser::graphRef() {
  GraphRefContext *_localctx = _tracker.createInstance<GraphRefContext>(_ctx, getState());
  enterRule(_localctx, 84, SparqlParser::RuleGraphRef);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(610);
    match(SparqlParser::GRAPH);
    setState(611);
    iri();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- GraphRefAllContext ------------------------------------------------------------------

SparqlParser::GraphRefAllContext::GraphRefAllContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::GraphRefContext* SparqlParser::GraphRefAllContext::graphRef() {
  return getRuleContext<SparqlParser::GraphRefContext>(0);
}

tree::TerminalNode* SparqlParser::GraphRefAllContext::DEFAULT() {
  return getToken(SparqlParser::DEFAULT, 0);
}

tree::TerminalNode* SparqlParser::GraphRefAllContext::NAMED() {
  return getToken(SparqlParser::NAMED, 0);
}

tree::TerminalNode* SparqlParser::GraphRefAllContext::ALL() {
  return getToken(SparqlParser::ALL, 0);
}


size_t SparqlParser::GraphRefAllContext::getRuleIndex() const {
  return SparqlParser::RuleGraphRefAll;
}


antlrcpp::Any SparqlParser::GraphRefAllContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitGraphRefAll(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::GraphRefAllContext* SparqlParser::graphRefAll() {
  GraphRefAllContext *_localctx = _tracker.createInstance<GraphRefAllContext>(_ctx, getState());
  enterRule(_localctx, 86, SparqlParser::RuleGraphRefAll);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(617);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlParser::GRAPH: {
        enterOuterAlt(_localctx, 1);
        setState(613);
        graphRef();
        break;
      }

      case SparqlParser::DEFAULT: {
        enterOuterAlt(_localctx, 2);
        setState(614);
        match(SparqlParser::DEFAULT);
        break;
      }

      case SparqlParser::NAMED: {
        enterOuterAlt(_localctx, 3);
        setState(615);
        match(SparqlParser::NAMED);
        break;
      }

      case SparqlParser::ALL: {
        enterOuterAlt(_localctx, 4);
        setState(616);
        match(SparqlParser::ALL);
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

//----------------- QuadPatternContext ------------------------------------------------------------------

SparqlParser::QuadPatternContext::QuadPatternContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::QuadPatternContext::OPEN_CURLY_BRACE() {
  return getToken(SparqlParser::OPEN_CURLY_BRACE, 0);
}

SparqlParser::QuadsContext* SparqlParser::QuadPatternContext::quads() {
  return getRuleContext<SparqlParser::QuadsContext>(0);
}

tree::TerminalNode* SparqlParser::QuadPatternContext::CLOSE_CURLY_BRACE() {
  return getToken(SparqlParser::CLOSE_CURLY_BRACE, 0);
}


size_t SparqlParser::QuadPatternContext::getRuleIndex() const {
  return SparqlParser::RuleQuadPattern;
}


antlrcpp::Any SparqlParser::QuadPatternContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitQuadPattern(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::QuadPatternContext* SparqlParser::quadPattern() {
  QuadPatternContext *_localctx = _tracker.createInstance<QuadPatternContext>(_ctx, getState());
  enterRule(_localctx, 88, SparqlParser::RuleQuadPattern);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(619);
    match(SparqlParser::OPEN_CURLY_BRACE);
    setState(620);
    quads();
    setState(621);
    match(SparqlParser::CLOSE_CURLY_BRACE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- QuadDataContext ------------------------------------------------------------------

SparqlParser::QuadDataContext::QuadDataContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::QuadDataContext::OPEN_CURLY_BRACE() {
  return getToken(SparqlParser::OPEN_CURLY_BRACE, 0);
}

SparqlParser::QuadsContext* SparqlParser::QuadDataContext::quads() {
  return getRuleContext<SparqlParser::QuadsContext>(0);
}

tree::TerminalNode* SparqlParser::QuadDataContext::CLOSE_CURLY_BRACE() {
  return getToken(SparqlParser::CLOSE_CURLY_BRACE, 0);
}


size_t SparqlParser::QuadDataContext::getRuleIndex() const {
  return SparqlParser::RuleQuadData;
}


antlrcpp::Any SparqlParser::QuadDataContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitQuadData(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::QuadDataContext* SparqlParser::quadData() {
  QuadDataContext *_localctx = _tracker.createInstance<QuadDataContext>(_ctx, getState());
  enterRule(_localctx, 90, SparqlParser::RuleQuadData);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(623);
    match(SparqlParser::OPEN_CURLY_BRACE);
    setState(624);
    quads();
    setState(625);
    match(SparqlParser::CLOSE_CURLY_BRACE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- QuadsContext ------------------------------------------------------------------

SparqlParser::QuadsContext::QuadsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::TriplesTemplateContext* SparqlParser::QuadsContext::triplesTemplate() {
  return getRuleContext<SparqlParser::TriplesTemplateContext>(0);
}

std::vector<SparqlParser::QuadsDetailsContext *> SparqlParser::QuadsContext::quadsDetails() {
  return getRuleContexts<SparqlParser::QuadsDetailsContext>();
}

SparqlParser::QuadsDetailsContext* SparqlParser::QuadsContext::quadsDetails(size_t i) {
  return getRuleContext<SparqlParser::QuadsDetailsContext>(i);
}


size_t SparqlParser::QuadsContext::getRuleIndex() const {
  return SparqlParser::RuleQuads;
}


antlrcpp::Any SparqlParser::QuadsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitQuads(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::QuadsContext* SparqlParser::quads() {
  QuadsContext *_localctx = _tracker.createInstance<QuadsContext>(_ctx, getState());
  enterRule(_localctx, 92, SparqlParser::RuleQuads);
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
    setState(628);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == SparqlParser::TRUE

    || _la == SparqlParser::FALSE || ((((_la - 115) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 115)) & ((1ULL << (SparqlParser::IRIREF - 115))
      | (1ULL << (SparqlParser::PNAME_NS - 115))
      | (1ULL << (SparqlParser::PNAME_LN - 115))
      | (1ULL << (SparqlParser::BLANK_NODE_LABEL - 115))
      | (1ULL << (SparqlParser::VAR1 - 115))
      | (1ULL << (SparqlParser::VAR2 - 115))
      | (1ULL << (SparqlParser::INTEGER - 115))
      | (1ULL << (SparqlParser::DECIMAL - 115))
      | (1ULL << (SparqlParser::DOUBLE - 115))
      | (1ULL << (SparqlParser::INTEGER_POSITIVE - 115))
      | (1ULL << (SparqlParser::DECIMAL_POSITIVE - 115))
      | (1ULL << (SparqlParser::DOUBLE_POSITIVE - 115))
      | (1ULL << (SparqlParser::INTEGER_NEGATIVE - 115))
      | (1ULL << (SparqlParser::DECIMAL_NEGATIVE - 115))
      | (1ULL << (SparqlParser::DOUBLE_NEGATIVE - 115))
      | (1ULL << (SparqlParser::STRING_LITERAL1 - 115))
      | (1ULL << (SparqlParser::STRING_LITERAL2 - 115))
      | (1ULL << (SparqlParser::STRING_LITERAL_LONG1 - 115))
      | (1ULL << (SparqlParser::STRING_LITERAL_LONG2 - 115))
      | (1ULL << (SparqlParser::OPEN_BRACE - 115))
      | (1ULL << (SparqlParser::OPEN_SQUARE_BRACKET - 115)))) != 0)) {
      setState(627);
      triplesTemplate();
    }
    setState(633);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SparqlParser::GRAPH) {
      setState(630);
      quadsDetails();
      setState(635);
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

//----------------- QuadsDetailsContext ------------------------------------------------------------------

SparqlParser::QuadsDetailsContext::QuadsDetailsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::QuadsNotTriplesContext* SparqlParser::QuadsDetailsContext::quadsNotTriples() {
  return getRuleContext<SparqlParser::QuadsNotTriplesContext>(0);
}

tree::TerminalNode* SparqlParser::QuadsDetailsContext::DOT() {
  return getToken(SparqlParser::DOT, 0);
}

SparqlParser::TriplesTemplateContext* SparqlParser::QuadsDetailsContext::triplesTemplate() {
  return getRuleContext<SparqlParser::TriplesTemplateContext>(0);
}


size_t SparqlParser::QuadsDetailsContext::getRuleIndex() const {
  return SparqlParser::RuleQuadsDetails;
}


antlrcpp::Any SparqlParser::QuadsDetailsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitQuadsDetails(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::QuadsDetailsContext* SparqlParser::quadsDetails() {
  QuadsDetailsContext *_localctx = _tracker.createInstance<QuadsDetailsContext>(_ctx, getState());
  enterRule(_localctx, 94, SparqlParser::RuleQuadsDetails);
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
    setState(636);
    quadsNotTriples();
    setState(638);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == SparqlParser::DOT) {
      setState(637);
      match(SparqlParser::DOT);
    }
    setState(641);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == SparqlParser::TRUE

    || _la == SparqlParser::FALSE || ((((_la - 115) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 115)) & ((1ULL << (SparqlParser::IRIREF - 115))
      | (1ULL << (SparqlParser::PNAME_NS - 115))
      | (1ULL << (SparqlParser::PNAME_LN - 115))
      | (1ULL << (SparqlParser::BLANK_NODE_LABEL - 115))
      | (1ULL << (SparqlParser::VAR1 - 115))
      | (1ULL << (SparqlParser::VAR2 - 115))
      | (1ULL << (SparqlParser::INTEGER - 115))
      | (1ULL << (SparqlParser::DECIMAL - 115))
      | (1ULL << (SparqlParser::DOUBLE - 115))
      | (1ULL << (SparqlParser::INTEGER_POSITIVE - 115))
      | (1ULL << (SparqlParser::DECIMAL_POSITIVE - 115))
      | (1ULL << (SparqlParser::DOUBLE_POSITIVE - 115))
      | (1ULL << (SparqlParser::INTEGER_NEGATIVE - 115))
      | (1ULL << (SparqlParser::DECIMAL_NEGATIVE - 115))
      | (1ULL << (SparqlParser::DOUBLE_NEGATIVE - 115))
      | (1ULL << (SparqlParser::STRING_LITERAL1 - 115))
      | (1ULL << (SparqlParser::STRING_LITERAL2 - 115))
      | (1ULL << (SparqlParser::STRING_LITERAL_LONG1 - 115))
      | (1ULL << (SparqlParser::STRING_LITERAL_LONG2 - 115))
      | (1ULL << (SparqlParser::OPEN_BRACE - 115))
      | (1ULL << (SparqlParser::OPEN_SQUARE_BRACKET - 115)))) != 0)) {
      setState(640);
      triplesTemplate();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- QuadsNotTriplesContext ------------------------------------------------------------------

SparqlParser::QuadsNotTriplesContext::QuadsNotTriplesContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::QuadsNotTriplesContext::GRAPH() {
  return getToken(SparqlParser::GRAPH, 0);
}

SparqlParser::VarOrIRIContext* SparqlParser::QuadsNotTriplesContext::varOrIRI() {
  return getRuleContext<SparqlParser::VarOrIRIContext>(0);
}

tree::TerminalNode* SparqlParser::QuadsNotTriplesContext::OPEN_CURLY_BRACE() {
  return getToken(SparqlParser::OPEN_CURLY_BRACE, 0);
}

tree::TerminalNode* SparqlParser::QuadsNotTriplesContext::CLOSE_CURLY_BRACE() {
  return getToken(SparqlParser::CLOSE_CURLY_BRACE, 0);
}

SparqlParser::TriplesTemplateContext* SparqlParser::QuadsNotTriplesContext::triplesTemplate() {
  return getRuleContext<SparqlParser::TriplesTemplateContext>(0);
}


size_t SparqlParser::QuadsNotTriplesContext::getRuleIndex() const {
  return SparqlParser::RuleQuadsNotTriples;
}


antlrcpp::Any SparqlParser::QuadsNotTriplesContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitQuadsNotTriples(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::QuadsNotTriplesContext* SparqlParser::quadsNotTriples() {
  QuadsNotTriplesContext *_localctx = _tracker.createInstance<QuadsNotTriplesContext>(_ctx, getState());
  enterRule(_localctx, 96, SparqlParser::RuleQuadsNotTriples);
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
    setState(643);
    match(SparqlParser::GRAPH);
    setState(644);
    varOrIRI();
    setState(645);
    match(SparqlParser::OPEN_CURLY_BRACE);
    setState(647);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == SparqlParser::TRUE

    || _la == SparqlParser::FALSE || ((((_la - 115) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 115)) & ((1ULL << (SparqlParser::IRIREF - 115))
      | (1ULL << (SparqlParser::PNAME_NS - 115))
      | (1ULL << (SparqlParser::PNAME_LN - 115))
      | (1ULL << (SparqlParser::BLANK_NODE_LABEL - 115))
      | (1ULL << (SparqlParser::VAR1 - 115))
      | (1ULL << (SparqlParser::VAR2 - 115))
      | (1ULL << (SparqlParser::INTEGER - 115))
      | (1ULL << (SparqlParser::DECIMAL - 115))
      | (1ULL << (SparqlParser::DOUBLE - 115))
      | (1ULL << (SparqlParser::INTEGER_POSITIVE - 115))
      | (1ULL << (SparqlParser::DECIMAL_POSITIVE - 115))
      | (1ULL << (SparqlParser::DOUBLE_POSITIVE - 115))
      | (1ULL << (SparqlParser::INTEGER_NEGATIVE - 115))
      | (1ULL << (SparqlParser::DECIMAL_NEGATIVE - 115))
      | (1ULL << (SparqlParser::DOUBLE_NEGATIVE - 115))
      | (1ULL << (SparqlParser::STRING_LITERAL1 - 115))
      | (1ULL << (SparqlParser::STRING_LITERAL2 - 115))
      | (1ULL << (SparqlParser::STRING_LITERAL_LONG1 - 115))
      | (1ULL << (SparqlParser::STRING_LITERAL_LONG2 - 115))
      | (1ULL << (SparqlParser::OPEN_BRACE - 115))
      | (1ULL << (SparqlParser::OPEN_SQUARE_BRACKET - 115)))) != 0)) {
      setState(646);
      triplesTemplate();
    }
    setState(649);
    match(SparqlParser::CLOSE_CURLY_BRACE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TriplesTemplateContext ------------------------------------------------------------------

SparqlParser::TriplesTemplateContext::TriplesTemplateContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<SparqlParser::TriplesSameSubjectContext *> SparqlParser::TriplesTemplateContext::triplesSameSubject() {
  return getRuleContexts<SparqlParser::TriplesSameSubjectContext>();
}

SparqlParser::TriplesSameSubjectContext* SparqlParser::TriplesTemplateContext::triplesSameSubject(size_t i) {
  return getRuleContext<SparqlParser::TriplesSameSubjectContext>(i);
}

std::vector<tree::TerminalNode *> SparqlParser::TriplesTemplateContext::DOT() {
  return getTokens(SparqlParser::DOT);
}

tree::TerminalNode* SparqlParser::TriplesTemplateContext::DOT(size_t i) {
  return getToken(SparqlParser::DOT, i);
}


size_t SparqlParser::TriplesTemplateContext::getRuleIndex() const {
  return SparqlParser::RuleTriplesTemplate;
}


antlrcpp::Any SparqlParser::TriplesTemplateContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitTriplesTemplate(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::TriplesTemplateContext* SparqlParser::triplesTemplate() {
  TriplesTemplateContext *_localctx = _tracker.createInstance<TriplesTemplateContext>(_ctx, getState());
  enterRule(_localctx, 98, SparqlParser::RuleTriplesTemplate);
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
    setState(651);
    triplesSameSubject();
    setState(658);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SparqlParser::DOT) {
      setState(652);
      match(SparqlParser::DOT);
      setState(654);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == SparqlParser::TRUE

      || _la == SparqlParser::FALSE || ((((_la - 115) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 115)) & ((1ULL << (SparqlParser::IRIREF - 115))
        | (1ULL << (SparqlParser::PNAME_NS - 115))
        | (1ULL << (SparqlParser::PNAME_LN - 115))
        | (1ULL << (SparqlParser::BLANK_NODE_LABEL - 115))
        | (1ULL << (SparqlParser::VAR1 - 115))
        | (1ULL << (SparqlParser::VAR2 - 115))
        | (1ULL << (SparqlParser::INTEGER - 115))
        | (1ULL << (SparqlParser::DECIMAL - 115))
        | (1ULL << (SparqlParser::DOUBLE - 115))
        | (1ULL << (SparqlParser::INTEGER_POSITIVE - 115))
        | (1ULL << (SparqlParser::DECIMAL_POSITIVE - 115))
        | (1ULL << (SparqlParser::DOUBLE_POSITIVE - 115))
        | (1ULL << (SparqlParser::INTEGER_NEGATIVE - 115))
        | (1ULL << (SparqlParser::DECIMAL_NEGATIVE - 115))
        | (1ULL << (SparqlParser::DOUBLE_NEGATIVE - 115))
        | (1ULL << (SparqlParser::STRING_LITERAL1 - 115))
        | (1ULL << (SparqlParser::STRING_LITERAL2 - 115))
        | (1ULL << (SparqlParser::STRING_LITERAL_LONG1 - 115))
        | (1ULL << (SparqlParser::STRING_LITERAL_LONG2 - 115))
        | (1ULL << (SparqlParser::OPEN_BRACE - 115))
        | (1ULL << (SparqlParser::OPEN_SQUARE_BRACKET - 115)))) != 0)) {
        setState(653);
        triplesSameSubject();
      }
      setState(660);
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

//----------------- GroupGraphPatternContext ------------------------------------------------------------------

SparqlParser::GroupGraphPatternContext::GroupGraphPatternContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::GroupGraphPatternContext::OPEN_CURLY_BRACE() {
  return getToken(SparqlParser::OPEN_CURLY_BRACE, 0);
}

tree::TerminalNode* SparqlParser::GroupGraphPatternContext::CLOSE_CURLY_BRACE() {
  return getToken(SparqlParser::CLOSE_CURLY_BRACE, 0);
}

SparqlParser::SubSelectContext* SparqlParser::GroupGraphPatternContext::subSelect() {
  return getRuleContext<SparqlParser::SubSelectContext>(0);
}

SparqlParser::GroupGraphPatternSubContext* SparqlParser::GroupGraphPatternContext::groupGraphPatternSub() {
  return getRuleContext<SparqlParser::GroupGraphPatternSubContext>(0);
}


size_t SparqlParser::GroupGraphPatternContext::getRuleIndex() const {
  return SparqlParser::RuleGroupGraphPattern;
}


antlrcpp::Any SparqlParser::GroupGraphPatternContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitGroupGraphPattern(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::GroupGraphPatternContext* SparqlParser::groupGraphPattern() {
  GroupGraphPatternContext *_localctx = _tracker.createInstance<GroupGraphPatternContext>(_ctx, getState());
  enterRule(_localctx, 100, SparqlParser::RuleGroupGraphPattern);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(661);
    match(SparqlParser::OPEN_CURLY_BRACE);
    setState(664);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlParser::SELECT: {
        setState(662);
        subSelect();
        break;
      }

      case SparqlParser::VALUES:
      case SparqlParser::OPTIONAL:
      case SparqlParser::GRAPH:
      case SparqlParser::FILTER:
      case SparqlParser::TRUE:
      case SparqlParser::FALSE:
      case SparqlParser::SERVICE:
      case SparqlParser::BIND:
      case SparqlParser::MINUS:
      case SparqlParser::IRIREF:
      case SparqlParser::PNAME_NS:
      case SparqlParser::PNAME_LN:
      case SparqlParser::BLANK_NODE_LABEL:
      case SparqlParser::VAR1:
      case SparqlParser::VAR2:
      case SparqlParser::INTEGER:
      case SparqlParser::DECIMAL:
      case SparqlParser::DOUBLE:
      case SparqlParser::INTEGER_POSITIVE:
      case SparqlParser::DECIMAL_POSITIVE:
      case SparqlParser::DOUBLE_POSITIVE:
      case SparqlParser::INTEGER_NEGATIVE:
      case SparqlParser::DECIMAL_NEGATIVE:
      case SparqlParser::DOUBLE_NEGATIVE:
      case SparqlParser::STRING_LITERAL1:
      case SparqlParser::STRING_LITERAL2:
      case SparqlParser::STRING_LITERAL_LONG1:
      case SparqlParser::STRING_LITERAL_LONG2:
      case SparqlParser::OPEN_BRACE:
      case SparqlParser::OPEN_CURLY_BRACE:
      case SparqlParser::CLOSE_CURLY_BRACE:
      case SparqlParser::OPEN_SQUARE_BRACKET: {
        setState(663);
        groupGraphPatternSub();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    setState(666);
    match(SparqlParser::CLOSE_CURLY_BRACE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- GroupGraphPatternSubContext ------------------------------------------------------------------

SparqlParser::GroupGraphPatternSubContext::GroupGraphPatternSubContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::TriplesBlockContext* SparqlParser::GroupGraphPatternSubContext::triplesBlock() {
  return getRuleContext<SparqlParser::TriplesBlockContext>(0);
}

std::vector<SparqlParser::GroupGraphPatternSubListContext *> SparqlParser::GroupGraphPatternSubContext::groupGraphPatternSubList() {
  return getRuleContexts<SparqlParser::GroupGraphPatternSubListContext>();
}

SparqlParser::GroupGraphPatternSubListContext* SparqlParser::GroupGraphPatternSubContext::groupGraphPatternSubList(size_t i) {
  return getRuleContext<SparqlParser::GroupGraphPatternSubListContext>(i);
}


size_t SparqlParser::GroupGraphPatternSubContext::getRuleIndex() const {
  return SparqlParser::RuleGroupGraphPatternSub;
}


antlrcpp::Any SparqlParser::GroupGraphPatternSubContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitGroupGraphPatternSub(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::GroupGraphPatternSubContext* SparqlParser::groupGraphPatternSub() {
  GroupGraphPatternSubContext *_localctx = _tracker.createInstance<GroupGraphPatternSubContext>(_ctx, getState());
  enterRule(_localctx, 102, SparqlParser::RuleGroupGraphPatternSub);
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
    setState(669);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == SparqlParser::TRUE

    || _la == SparqlParser::FALSE || ((((_la - 115) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 115)) & ((1ULL << (SparqlParser::IRIREF - 115))
      | (1ULL << (SparqlParser::PNAME_NS - 115))
      | (1ULL << (SparqlParser::PNAME_LN - 115))
      | (1ULL << (SparqlParser::BLANK_NODE_LABEL - 115))
      | (1ULL << (SparqlParser::VAR1 - 115))
      | (1ULL << (SparqlParser::VAR2 - 115))
      | (1ULL << (SparqlParser::INTEGER - 115))
      | (1ULL << (SparqlParser::DECIMAL - 115))
      | (1ULL << (SparqlParser::DOUBLE - 115))
      | (1ULL << (SparqlParser::INTEGER_POSITIVE - 115))
      | (1ULL << (SparqlParser::DECIMAL_POSITIVE - 115))
      | (1ULL << (SparqlParser::DOUBLE_POSITIVE - 115))
      | (1ULL << (SparqlParser::INTEGER_NEGATIVE - 115))
      | (1ULL << (SparqlParser::DECIMAL_NEGATIVE - 115))
      | (1ULL << (SparqlParser::DOUBLE_NEGATIVE - 115))
      | (1ULL << (SparqlParser::STRING_LITERAL1 - 115))
      | (1ULL << (SparqlParser::STRING_LITERAL2 - 115))
      | (1ULL << (SparqlParser::STRING_LITERAL_LONG1 - 115))
      | (1ULL << (SparqlParser::STRING_LITERAL_LONG2 - 115))
      | (1ULL << (SparqlParser::OPEN_BRACE - 115))
      | (1ULL << (SparqlParser::OPEN_SQUARE_BRACKET - 115)))) != 0)) {
      setState(668);
      triplesBlock();
    }
    setState(674);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << SparqlParser::VALUES)
      | (1ULL << SparqlParser::OPTIONAL)
      | (1ULL << SparqlParser::GRAPH)
      | (1ULL << SparqlParser::FILTER)
      | (1ULL << SparqlParser::SERVICE)
      | (1ULL << SparqlParser::BIND)
      | (1ULL << SparqlParser::MINUS))) != 0) || _la == SparqlParser::OPEN_CURLY_BRACE) {
      setState(671);
      groupGraphPatternSubList();
      setState(676);
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

//----------------- GroupGraphPatternSubListContext ------------------------------------------------------------------

SparqlParser::GroupGraphPatternSubListContext::GroupGraphPatternSubListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::GraphPatternNotTriplesContext* SparqlParser::GroupGraphPatternSubListContext::graphPatternNotTriples() {
  return getRuleContext<SparqlParser::GraphPatternNotTriplesContext>(0);
}

tree::TerminalNode* SparqlParser::GroupGraphPatternSubListContext::DOT() {
  return getToken(SparqlParser::DOT, 0);
}

SparqlParser::TriplesBlockContext* SparqlParser::GroupGraphPatternSubListContext::triplesBlock() {
  return getRuleContext<SparqlParser::TriplesBlockContext>(0);
}


size_t SparqlParser::GroupGraphPatternSubListContext::getRuleIndex() const {
  return SparqlParser::RuleGroupGraphPatternSubList;
}


antlrcpp::Any SparqlParser::GroupGraphPatternSubListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitGroupGraphPatternSubList(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::GroupGraphPatternSubListContext* SparqlParser::groupGraphPatternSubList() {
  GroupGraphPatternSubListContext *_localctx = _tracker.createInstance<GroupGraphPatternSubListContext>(_ctx, getState());
  enterRule(_localctx, 104, SparqlParser::RuleGroupGraphPatternSubList);
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
    setState(677);
    graphPatternNotTriples();
    setState(679);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == SparqlParser::DOT) {
      setState(678);
      match(SparqlParser::DOT);
    }
    setState(682);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == SparqlParser::TRUE

    || _la == SparqlParser::FALSE || ((((_la - 115) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 115)) & ((1ULL << (SparqlParser::IRIREF - 115))
      | (1ULL << (SparqlParser::PNAME_NS - 115))
      | (1ULL << (SparqlParser::PNAME_LN - 115))
      | (1ULL << (SparqlParser::BLANK_NODE_LABEL - 115))
      | (1ULL << (SparqlParser::VAR1 - 115))
      | (1ULL << (SparqlParser::VAR2 - 115))
      | (1ULL << (SparqlParser::INTEGER - 115))
      | (1ULL << (SparqlParser::DECIMAL - 115))
      | (1ULL << (SparqlParser::DOUBLE - 115))
      | (1ULL << (SparqlParser::INTEGER_POSITIVE - 115))
      | (1ULL << (SparqlParser::DECIMAL_POSITIVE - 115))
      | (1ULL << (SparqlParser::DOUBLE_POSITIVE - 115))
      | (1ULL << (SparqlParser::INTEGER_NEGATIVE - 115))
      | (1ULL << (SparqlParser::DECIMAL_NEGATIVE - 115))
      | (1ULL << (SparqlParser::DOUBLE_NEGATIVE - 115))
      | (1ULL << (SparqlParser::STRING_LITERAL1 - 115))
      | (1ULL << (SparqlParser::STRING_LITERAL2 - 115))
      | (1ULL << (SparqlParser::STRING_LITERAL_LONG1 - 115))
      | (1ULL << (SparqlParser::STRING_LITERAL_LONG2 - 115))
      | (1ULL << (SparqlParser::OPEN_BRACE - 115))
      | (1ULL << (SparqlParser::OPEN_SQUARE_BRACKET - 115)))) != 0)) {
      setState(681);
      triplesBlock();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TriplesBlockContext ------------------------------------------------------------------

SparqlParser::TriplesBlockContext::TriplesBlockContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<SparqlParser::TriplesSameSubjectPathContext *> SparqlParser::TriplesBlockContext::triplesSameSubjectPath() {
  return getRuleContexts<SparqlParser::TriplesSameSubjectPathContext>();
}

SparqlParser::TriplesSameSubjectPathContext* SparqlParser::TriplesBlockContext::triplesSameSubjectPath(size_t i) {
  return getRuleContext<SparqlParser::TriplesSameSubjectPathContext>(i);
}

std::vector<tree::TerminalNode *> SparqlParser::TriplesBlockContext::DOT() {
  return getTokens(SparqlParser::DOT);
}

tree::TerminalNode* SparqlParser::TriplesBlockContext::DOT(size_t i) {
  return getToken(SparqlParser::DOT, i);
}


size_t SparqlParser::TriplesBlockContext::getRuleIndex() const {
  return SparqlParser::RuleTriplesBlock;
}


antlrcpp::Any SparqlParser::TriplesBlockContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitTriplesBlock(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::TriplesBlockContext* SparqlParser::triplesBlock() {
  TriplesBlockContext *_localctx = _tracker.createInstance<TriplesBlockContext>(_ctx, getState());
  enterRule(_localctx, 106, SparqlParser::RuleTriplesBlock);
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
    setState(684);
    triplesSameSubjectPath();
    setState(691);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SparqlParser::DOT) {
      setState(685);
      match(SparqlParser::DOT);
      setState(687);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == SparqlParser::TRUE

      || _la == SparqlParser::FALSE || ((((_la - 115) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 115)) & ((1ULL << (SparqlParser::IRIREF - 115))
        | (1ULL << (SparqlParser::PNAME_NS - 115))
        | (1ULL << (SparqlParser::PNAME_LN - 115))
        | (1ULL << (SparqlParser::BLANK_NODE_LABEL - 115))
        | (1ULL << (SparqlParser::VAR1 - 115))
        | (1ULL << (SparqlParser::VAR2 - 115))
        | (1ULL << (SparqlParser::INTEGER - 115))
        | (1ULL << (SparqlParser::DECIMAL - 115))
        | (1ULL << (SparqlParser::DOUBLE - 115))
        | (1ULL << (SparqlParser::INTEGER_POSITIVE - 115))
        | (1ULL << (SparqlParser::DECIMAL_POSITIVE - 115))
        | (1ULL << (SparqlParser::DOUBLE_POSITIVE - 115))
        | (1ULL << (SparqlParser::INTEGER_NEGATIVE - 115))
        | (1ULL << (SparqlParser::DECIMAL_NEGATIVE - 115))
        | (1ULL << (SparqlParser::DOUBLE_NEGATIVE - 115))
        | (1ULL << (SparqlParser::STRING_LITERAL1 - 115))
        | (1ULL << (SparqlParser::STRING_LITERAL2 - 115))
        | (1ULL << (SparqlParser::STRING_LITERAL_LONG1 - 115))
        | (1ULL << (SparqlParser::STRING_LITERAL_LONG2 - 115))
        | (1ULL << (SparqlParser::OPEN_BRACE - 115))
        | (1ULL << (SparqlParser::OPEN_SQUARE_BRACKET - 115)))) != 0)) {
        setState(686);
        triplesSameSubjectPath();
      }
      setState(693);
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

//----------------- GraphPatternNotTriplesContext ------------------------------------------------------------------

SparqlParser::GraphPatternNotTriplesContext::GraphPatternNotTriplesContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::GroupOrUnionGraphPatternContext* SparqlParser::GraphPatternNotTriplesContext::groupOrUnionGraphPattern() {
  return getRuleContext<SparqlParser::GroupOrUnionGraphPatternContext>(0);
}

SparqlParser::OptionalGraphPatternContext* SparqlParser::GraphPatternNotTriplesContext::optionalGraphPattern() {
  return getRuleContext<SparqlParser::OptionalGraphPatternContext>(0);
}

SparqlParser::MinusGraphPatternContext* SparqlParser::GraphPatternNotTriplesContext::minusGraphPattern() {
  return getRuleContext<SparqlParser::MinusGraphPatternContext>(0);
}

SparqlParser::GraphGraphPatternContext* SparqlParser::GraphPatternNotTriplesContext::graphGraphPattern() {
  return getRuleContext<SparqlParser::GraphGraphPatternContext>(0);
}

SparqlParser::ServiceGraphPatternContext* SparqlParser::GraphPatternNotTriplesContext::serviceGraphPattern() {
  return getRuleContext<SparqlParser::ServiceGraphPatternContext>(0);
}

SparqlParser::FilterContext* SparqlParser::GraphPatternNotTriplesContext::filter() {
  return getRuleContext<SparqlParser::FilterContext>(0);
}

SparqlParser::BindContext* SparqlParser::GraphPatternNotTriplesContext::bind() {
  return getRuleContext<SparqlParser::BindContext>(0);
}

SparqlParser::InlineDataContext* SparqlParser::GraphPatternNotTriplesContext::inlineData() {
  return getRuleContext<SparqlParser::InlineDataContext>(0);
}


size_t SparqlParser::GraphPatternNotTriplesContext::getRuleIndex() const {
  return SparqlParser::RuleGraphPatternNotTriples;
}


antlrcpp::Any SparqlParser::GraphPatternNotTriplesContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitGraphPatternNotTriples(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::GraphPatternNotTriplesContext* SparqlParser::graphPatternNotTriples() {
  GraphPatternNotTriplesContext *_localctx = _tracker.createInstance<GraphPatternNotTriplesContext>(_ctx, getState());
  enterRule(_localctx, 108, SparqlParser::RuleGraphPatternNotTriples);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(702);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlParser::OPEN_CURLY_BRACE: {
        enterOuterAlt(_localctx, 1);
        setState(694);
        groupOrUnionGraphPattern();
        break;
      }

      case SparqlParser::OPTIONAL: {
        enterOuterAlt(_localctx, 2);
        setState(695);
        optionalGraphPattern();
        break;
      }

      case SparqlParser::MINUS: {
        enterOuterAlt(_localctx, 3);
        setState(696);
        minusGraphPattern();
        break;
      }

      case SparqlParser::GRAPH: {
        enterOuterAlt(_localctx, 4);
        setState(697);
        graphGraphPattern();
        break;
      }

      case SparqlParser::SERVICE: {
        enterOuterAlt(_localctx, 5);
        setState(698);
        serviceGraphPattern();
        break;
      }

      case SparqlParser::FILTER: {
        enterOuterAlt(_localctx, 6);
        setState(699);
        filter();
        break;
      }

      case SparqlParser::BIND: {
        enterOuterAlt(_localctx, 7);
        setState(700);
        bind();
        break;
      }

      case SparqlParser::VALUES: {
        enterOuterAlt(_localctx, 8);
        setState(701);
        inlineData();
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

//----------------- OptionalGraphPatternContext ------------------------------------------------------------------

SparqlParser::OptionalGraphPatternContext::OptionalGraphPatternContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::OptionalGraphPatternContext::OPTIONAL() {
  return getToken(SparqlParser::OPTIONAL, 0);
}

SparqlParser::GroupGraphPatternContext* SparqlParser::OptionalGraphPatternContext::groupGraphPattern() {
  return getRuleContext<SparqlParser::GroupGraphPatternContext>(0);
}


size_t SparqlParser::OptionalGraphPatternContext::getRuleIndex() const {
  return SparqlParser::RuleOptionalGraphPattern;
}


antlrcpp::Any SparqlParser::OptionalGraphPatternContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitOptionalGraphPattern(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::OptionalGraphPatternContext* SparqlParser::optionalGraphPattern() {
  OptionalGraphPatternContext *_localctx = _tracker.createInstance<OptionalGraphPatternContext>(_ctx, getState());
  enterRule(_localctx, 110, SparqlParser::RuleOptionalGraphPattern);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(704);
    match(SparqlParser::OPTIONAL);
    setState(705);
    groupGraphPattern();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- GraphGraphPatternContext ------------------------------------------------------------------

SparqlParser::GraphGraphPatternContext::GraphGraphPatternContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::GraphGraphPatternContext::GRAPH() {
  return getToken(SparqlParser::GRAPH, 0);
}

SparqlParser::VarOrIRIContext* SparqlParser::GraphGraphPatternContext::varOrIRI() {
  return getRuleContext<SparqlParser::VarOrIRIContext>(0);
}

SparqlParser::GroupGraphPatternContext* SparqlParser::GraphGraphPatternContext::groupGraphPattern() {
  return getRuleContext<SparqlParser::GroupGraphPatternContext>(0);
}


size_t SparqlParser::GraphGraphPatternContext::getRuleIndex() const {
  return SparqlParser::RuleGraphGraphPattern;
}


antlrcpp::Any SparqlParser::GraphGraphPatternContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitGraphGraphPattern(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::GraphGraphPatternContext* SparqlParser::graphGraphPattern() {
  GraphGraphPatternContext *_localctx = _tracker.createInstance<GraphGraphPatternContext>(_ctx, getState());
  enterRule(_localctx, 112, SparqlParser::RuleGraphGraphPattern);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(707);
    match(SparqlParser::GRAPH);
    setState(708);
    varOrIRI();
    setState(709);
    groupGraphPattern();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ServiceGraphPatternContext ------------------------------------------------------------------

SparqlParser::ServiceGraphPatternContext::ServiceGraphPatternContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::ServiceGraphPatternContext::SERVICE() {
  return getToken(SparqlParser::SERVICE, 0);
}

SparqlParser::VarOrIRIContext* SparqlParser::ServiceGraphPatternContext::varOrIRI() {
  return getRuleContext<SparqlParser::VarOrIRIContext>(0);
}

SparqlParser::GroupGraphPatternContext* SparqlParser::ServiceGraphPatternContext::groupGraphPattern() {
  return getRuleContext<SparqlParser::GroupGraphPatternContext>(0);
}

tree::TerminalNode* SparqlParser::ServiceGraphPatternContext::SILENT() {
  return getToken(SparqlParser::SILENT, 0);
}


size_t SparqlParser::ServiceGraphPatternContext::getRuleIndex() const {
  return SparqlParser::RuleServiceGraphPattern;
}


antlrcpp::Any SparqlParser::ServiceGraphPatternContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitServiceGraphPattern(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::ServiceGraphPatternContext* SparqlParser::serviceGraphPattern() {
  ServiceGraphPatternContext *_localctx = _tracker.createInstance<ServiceGraphPatternContext>(_ctx, getState());
  enterRule(_localctx, 114, SparqlParser::RuleServiceGraphPattern);
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
    setState(711);
    match(SparqlParser::SERVICE);
    setState(713);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == SparqlParser::SILENT) {
      setState(712);
      match(SparqlParser::SILENT);
    }
    setState(715);
    varOrIRI();
    setState(716);
    groupGraphPattern();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BindContext ------------------------------------------------------------------

SparqlParser::BindContext::BindContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::BindContext::BIND() {
  return getToken(SparqlParser::BIND, 0);
}

tree::TerminalNode* SparqlParser::BindContext::OPEN_BRACE() {
  return getToken(SparqlParser::OPEN_BRACE, 0);
}

SparqlParser::ExpressionContext* SparqlParser::BindContext::expression() {
  return getRuleContext<SparqlParser::ExpressionContext>(0);
}

tree::TerminalNode* SparqlParser::BindContext::AS() {
  return getToken(SparqlParser::AS, 0);
}

SparqlParser::VarContext* SparqlParser::BindContext::var() {
  return getRuleContext<SparqlParser::VarContext>(0);
}

tree::TerminalNode* SparqlParser::BindContext::CLOSE_BRACE() {
  return getToken(SparqlParser::CLOSE_BRACE, 0);
}


size_t SparqlParser::BindContext::getRuleIndex() const {
  return SparqlParser::RuleBind;
}


antlrcpp::Any SparqlParser::BindContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitBind(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::BindContext* SparqlParser::bind() {
  BindContext *_localctx = _tracker.createInstance<BindContext>(_ctx, getState());
  enterRule(_localctx, 116, SparqlParser::RuleBind);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(718);
    match(SparqlParser::BIND);
    setState(719);
    match(SparqlParser::OPEN_BRACE);
    setState(720);
    expression(0);
    setState(721);
    match(SparqlParser::AS);
    setState(722);
    var();
    setState(723);
    match(SparqlParser::CLOSE_BRACE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- InlineDataContext ------------------------------------------------------------------

SparqlParser::InlineDataContext::InlineDataContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::InlineDataContext::VALUES() {
  return getToken(SparqlParser::VALUES, 0);
}

SparqlParser::DataBlockContext* SparqlParser::InlineDataContext::dataBlock() {
  return getRuleContext<SparqlParser::DataBlockContext>(0);
}


size_t SparqlParser::InlineDataContext::getRuleIndex() const {
  return SparqlParser::RuleInlineData;
}


antlrcpp::Any SparqlParser::InlineDataContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitInlineData(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::InlineDataContext* SparqlParser::inlineData() {
  InlineDataContext *_localctx = _tracker.createInstance<InlineDataContext>(_ctx, getState());
  enterRule(_localctx, 118, SparqlParser::RuleInlineData);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(725);
    match(SparqlParser::VALUES);
    setState(726);
    dataBlock();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DataBlockContext ------------------------------------------------------------------

SparqlParser::DataBlockContext::DataBlockContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::InlineDataOneVarContext* SparqlParser::DataBlockContext::inlineDataOneVar() {
  return getRuleContext<SparqlParser::InlineDataOneVarContext>(0);
}

SparqlParser::InlineDataFullContext* SparqlParser::DataBlockContext::inlineDataFull() {
  return getRuleContext<SparqlParser::InlineDataFullContext>(0);
}


size_t SparqlParser::DataBlockContext::getRuleIndex() const {
  return SparqlParser::RuleDataBlock;
}


antlrcpp::Any SparqlParser::DataBlockContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitDataBlock(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::DataBlockContext* SparqlParser::dataBlock() {
  DataBlockContext *_localctx = _tracker.createInstance<DataBlockContext>(_ctx, getState());
  enterRule(_localctx, 120, SparqlParser::RuleDataBlock);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(730);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlParser::VAR1:
      case SparqlParser::VAR2: {
        enterOuterAlt(_localctx, 1);
        setState(728);
        inlineDataOneVar();
        break;
      }

      case SparqlParser::OPEN_BRACE: {
        enterOuterAlt(_localctx, 2);
        setState(729);
        inlineDataFull();
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

//----------------- InlineDataOneVarContext ------------------------------------------------------------------

SparqlParser::InlineDataOneVarContext::InlineDataOneVarContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::VarContext* SparqlParser::InlineDataOneVarContext::var() {
  return getRuleContext<SparqlParser::VarContext>(0);
}

tree::TerminalNode* SparqlParser::InlineDataOneVarContext::OPEN_CURLY_BRACE() {
  return getToken(SparqlParser::OPEN_CURLY_BRACE, 0);
}

tree::TerminalNode* SparqlParser::InlineDataOneVarContext::CLOSE_CURLY_BRACE() {
  return getToken(SparqlParser::CLOSE_CURLY_BRACE, 0);
}

std::vector<SparqlParser::DataBlockValueContext *> SparqlParser::InlineDataOneVarContext::dataBlockValue() {
  return getRuleContexts<SparqlParser::DataBlockValueContext>();
}

SparqlParser::DataBlockValueContext* SparqlParser::InlineDataOneVarContext::dataBlockValue(size_t i) {
  return getRuleContext<SparqlParser::DataBlockValueContext>(i);
}


size_t SparqlParser::InlineDataOneVarContext::getRuleIndex() const {
  return SparqlParser::RuleInlineDataOneVar;
}


antlrcpp::Any SparqlParser::InlineDataOneVarContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitInlineDataOneVar(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::InlineDataOneVarContext* SparqlParser::inlineDataOneVar() {
  InlineDataOneVarContext *_localctx = _tracker.createInstance<InlineDataOneVarContext>(_ctx, getState());
  enterRule(_localctx, 122, SparqlParser::RuleInlineDataOneVar);
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
    setState(732);
    var();
    setState(733);
    match(SparqlParser::OPEN_CURLY_BRACE);
    setState(737);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << SparqlParser::TRUE)
      | (1ULL << SparqlParser::FALSE)
      | (1ULL << SparqlParser::UNDEF))) != 0) || ((((_la - 115) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 115)) & ((1ULL << (SparqlParser::IRIREF - 115))
      | (1ULL << (SparqlParser::PNAME_NS - 115))
      | (1ULL << (SparqlParser::PNAME_LN - 115))
      | (1ULL << (SparqlParser::INTEGER - 115))
      | (1ULL << (SparqlParser::DECIMAL - 115))
      | (1ULL << (SparqlParser::DOUBLE - 115))
      | (1ULL << (SparqlParser::INTEGER_POSITIVE - 115))
      | (1ULL << (SparqlParser::DECIMAL_POSITIVE - 115))
      | (1ULL << (SparqlParser::DOUBLE_POSITIVE - 115))
      | (1ULL << (SparqlParser::INTEGER_NEGATIVE - 115))
      | (1ULL << (SparqlParser::DECIMAL_NEGATIVE - 115))
      | (1ULL << (SparqlParser::DOUBLE_NEGATIVE - 115))
      | (1ULL << (SparqlParser::STRING_LITERAL1 - 115))
      | (1ULL << (SparqlParser::STRING_LITERAL2 - 115))
      | (1ULL << (SparqlParser::STRING_LITERAL_LONG1 - 115))
      | (1ULL << (SparqlParser::STRING_LITERAL_LONG2 - 115)))) != 0)) {
      setState(734);
      dataBlockValue();
      setState(739);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(740);
    match(SparqlParser::CLOSE_CURLY_BRACE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- InlineDataFullContext ------------------------------------------------------------------

SparqlParser::InlineDataFullContext::InlineDataFullContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::InlineDataFullContext::OPEN_BRACE() {
  return getToken(SparqlParser::OPEN_BRACE, 0);
}

tree::TerminalNode* SparqlParser::InlineDataFullContext::CLOSE_BRACE() {
  return getToken(SparqlParser::CLOSE_BRACE, 0);
}

tree::TerminalNode* SparqlParser::InlineDataFullContext::OPEN_CURLY_BRACE() {
  return getToken(SparqlParser::OPEN_CURLY_BRACE, 0);
}

tree::TerminalNode* SparqlParser::InlineDataFullContext::CLOSE_CURLY_BRACE() {
  return getToken(SparqlParser::CLOSE_CURLY_BRACE, 0);
}

std::vector<SparqlParser::VarContext *> SparqlParser::InlineDataFullContext::var() {
  return getRuleContexts<SparqlParser::VarContext>();
}

SparqlParser::VarContext* SparqlParser::InlineDataFullContext::var(size_t i) {
  return getRuleContext<SparqlParser::VarContext>(i);
}

std::vector<SparqlParser::DataBlockValuesContext *> SparqlParser::InlineDataFullContext::dataBlockValues() {
  return getRuleContexts<SparqlParser::DataBlockValuesContext>();
}

SparqlParser::DataBlockValuesContext* SparqlParser::InlineDataFullContext::dataBlockValues(size_t i) {
  return getRuleContext<SparqlParser::DataBlockValuesContext>(i);
}


size_t SparqlParser::InlineDataFullContext::getRuleIndex() const {
  return SparqlParser::RuleInlineDataFull;
}


antlrcpp::Any SparqlParser::InlineDataFullContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitInlineDataFull(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::InlineDataFullContext* SparqlParser::inlineDataFull() {
  InlineDataFullContext *_localctx = _tracker.createInstance<InlineDataFullContext>(_ctx, getState());
  enterRule(_localctx, 124, SparqlParser::RuleInlineDataFull);
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
    setState(742);
    match(SparqlParser::OPEN_BRACE);
    setState(746);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SparqlParser::VAR1

    || _la == SparqlParser::VAR2) {
      setState(743);
      var();
      setState(748);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(749);
    match(SparqlParser::CLOSE_BRACE);
    setState(750);
    match(SparqlParser::OPEN_CURLY_BRACE);
    setState(754);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SparqlParser::OPEN_BRACE) {
      setState(751);
      dataBlockValues();
      setState(756);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(757);
    match(SparqlParser::CLOSE_CURLY_BRACE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DataBlockValuesContext ------------------------------------------------------------------

SparqlParser::DataBlockValuesContext::DataBlockValuesContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::DataBlockValuesContext::OPEN_BRACE() {
  return getToken(SparqlParser::OPEN_BRACE, 0);
}

tree::TerminalNode* SparqlParser::DataBlockValuesContext::CLOSE_BRACE() {
  return getToken(SparqlParser::CLOSE_BRACE, 0);
}

std::vector<SparqlParser::DataBlockValueContext *> SparqlParser::DataBlockValuesContext::dataBlockValue() {
  return getRuleContexts<SparqlParser::DataBlockValueContext>();
}

SparqlParser::DataBlockValueContext* SparqlParser::DataBlockValuesContext::dataBlockValue(size_t i) {
  return getRuleContext<SparqlParser::DataBlockValueContext>(i);
}


size_t SparqlParser::DataBlockValuesContext::getRuleIndex() const {
  return SparqlParser::RuleDataBlockValues;
}


antlrcpp::Any SparqlParser::DataBlockValuesContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitDataBlockValues(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::DataBlockValuesContext* SparqlParser::dataBlockValues() {
  DataBlockValuesContext *_localctx = _tracker.createInstance<DataBlockValuesContext>(_ctx, getState());
  enterRule(_localctx, 126, SparqlParser::RuleDataBlockValues);
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
    setState(759);
    match(SparqlParser::OPEN_BRACE);
    setState(763);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << SparqlParser::TRUE)
      | (1ULL << SparqlParser::FALSE)
      | (1ULL << SparqlParser::UNDEF))) != 0) || ((((_la - 115) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 115)) & ((1ULL << (SparqlParser::IRIREF - 115))
      | (1ULL << (SparqlParser::PNAME_NS - 115))
      | (1ULL << (SparqlParser::PNAME_LN - 115))
      | (1ULL << (SparqlParser::INTEGER - 115))
      | (1ULL << (SparqlParser::DECIMAL - 115))
      | (1ULL << (SparqlParser::DOUBLE - 115))
      | (1ULL << (SparqlParser::INTEGER_POSITIVE - 115))
      | (1ULL << (SparqlParser::DECIMAL_POSITIVE - 115))
      | (1ULL << (SparqlParser::DOUBLE_POSITIVE - 115))
      | (1ULL << (SparqlParser::INTEGER_NEGATIVE - 115))
      | (1ULL << (SparqlParser::DECIMAL_NEGATIVE - 115))
      | (1ULL << (SparqlParser::DOUBLE_NEGATIVE - 115))
      | (1ULL << (SparqlParser::STRING_LITERAL1 - 115))
      | (1ULL << (SparqlParser::STRING_LITERAL2 - 115))
      | (1ULL << (SparqlParser::STRING_LITERAL_LONG1 - 115))
      | (1ULL << (SparqlParser::STRING_LITERAL_LONG2 - 115)))) != 0)) {
      setState(760);
      dataBlockValue();
      setState(765);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(766);
    match(SparqlParser::CLOSE_BRACE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DataBlockValueContext ------------------------------------------------------------------

SparqlParser::DataBlockValueContext::DataBlockValueContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::IriContext* SparqlParser::DataBlockValueContext::iri() {
  return getRuleContext<SparqlParser::IriContext>(0);
}

SparqlParser::RdfLiteralContext* SparqlParser::DataBlockValueContext::rdfLiteral() {
  return getRuleContext<SparqlParser::RdfLiteralContext>(0);
}

SparqlParser::NumericLiteralContext* SparqlParser::DataBlockValueContext::numericLiteral() {
  return getRuleContext<SparqlParser::NumericLiteralContext>(0);
}

SparqlParser::BooleanLiteralContext* SparqlParser::DataBlockValueContext::booleanLiteral() {
  return getRuleContext<SparqlParser::BooleanLiteralContext>(0);
}

tree::TerminalNode* SparqlParser::DataBlockValueContext::UNDEF() {
  return getToken(SparqlParser::UNDEF, 0);
}


size_t SparqlParser::DataBlockValueContext::getRuleIndex() const {
  return SparqlParser::RuleDataBlockValue;
}


antlrcpp::Any SparqlParser::DataBlockValueContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitDataBlockValue(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::DataBlockValueContext* SparqlParser::dataBlockValue() {
  DataBlockValueContext *_localctx = _tracker.createInstance<DataBlockValueContext>(_ctx, getState());
  enterRule(_localctx, 128, SparqlParser::RuleDataBlockValue);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(773);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlParser::IRIREF:
      case SparqlParser::PNAME_NS:
      case SparqlParser::PNAME_LN: {
        enterOuterAlt(_localctx, 1);
        setState(768);
        iri();
        break;
      }

      case SparqlParser::STRING_LITERAL1:
      case SparqlParser::STRING_LITERAL2:
      case SparqlParser::STRING_LITERAL_LONG1:
      case SparqlParser::STRING_LITERAL_LONG2: {
        enterOuterAlt(_localctx, 2);
        setState(769);
        rdfLiteral();
        break;
      }

      case SparqlParser::INTEGER:
      case SparqlParser::DECIMAL:
      case SparqlParser::DOUBLE:
      case SparqlParser::INTEGER_POSITIVE:
      case SparqlParser::DECIMAL_POSITIVE:
      case SparqlParser::DOUBLE_POSITIVE:
      case SparqlParser::INTEGER_NEGATIVE:
      case SparqlParser::DECIMAL_NEGATIVE:
      case SparqlParser::DOUBLE_NEGATIVE: {
        enterOuterAlt(_localctx, 3);
        setState(770);
        numericLiteral();
        break;
      }

      case SparqlParser::TRUE:
      case SparqlParser::FALSE: {
        enterOuterAlt(_localctx, 4);
        setState(771);
        booleanLiteral();
        break;
      }

      case SparqlParser::UNDEF: {
        enterOuterAlt(_localctx, 5);
        setState(772);
        match(SparqlParser::UNDEF);
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

//----------------- MinusGraphPatternContext ------------------------------------------------------------------

SparqlParser::MinusGraphPatternContext::MinusGraphPatternContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::MinusGraphPatternContext::MINUS() {
  return getToken(SparqlParser::MINUS, 0);
}

SparqlParser::GroupGraphPatternContext* SparqlParser::MinusGraphPatternContext::groupGraphPattern() {
  return getRuleContext<SparqlParser::GroupGraphPatternContext>(0);
}


size_t SparqlParser::MinusGraphPatternContext::getRuleIndex() const {
  return SparqlParser::RuleMinusGraphPattern;
}


antlrcpp::Any SparqlParser::MinusGraphPatternContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitMinusGraphPattern(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::MinusGraphPatternContext* SparqlParser::minusGraphPattern() {
  MinusGraphPatternContext *_localctx = _tracker.createInstance<MinusGraphPatternContext>(_ctx, getState());
  enterRule(_localctx, 130, SparqlParser::RuleMinusGraphPattern);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(775);
    match(SparqlParser::MINUS);
    setState(776);
    groupGraphPattern();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- GroupOrUnionGraphPatternContext ------------------------------------------------------------------

SparqlParser::GroupOrUnionGraphPatternContext::GroupOrUnionGraphPatternContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<SparqlParser::GroupGraphPatternContext *> SparqlParser::GroupOrUnionGraphPatternContext::groupGraphPattern() {
  return getRuleContexts<SparqlParser::GroupGraphPatternContext>();
}

SparqlParser::GroupGraphPatternContext* SparqlParser::GroupOrUnionGraphPatternContext::groupGraphPattern(size_t i) {
  return getRuleContext<SparqlParser::GroupGraphPatternContext>(i);
}

std::vector<tree::TerminalNode *> SparqlParser::GroupOrUnionGraphPatternContext::UNION() {
  return getTokens(SparqlParser::UNION);
}

tree::TerminalNode* SparqlParser::GroupOrUnionGraphPatternContext::UNION(size_t i) {
  return getToken(SparqlParser::UNION, i);
}


size_t SparqlParser::GroupOrUnionGraphPatternContext::getRuleIndex() const {
  return SparqlParser::RuleGroupOrUnionGraphPattern;
}


antlrcpp::Any SparqlParser::GroupOrUnionGraphPatternContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitGroupOrUnionGraphPattern(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::GroupOrUnionGraphPatternContext* SparqlParser::groupOrUnionGraphPattern() {
  GroupOrUnionGraphPatternContext *_localctx = _tracker.createInstance<GroupOrUnionGraphPatternContext>(_ctx, getState());
  enterRule(_localctx, 132, SparqlParser::RuleGroupOrUnionGraphPattern);
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
    setState(778);
    groupGraphPattern();
    setState(783);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SparqlParser::UNION) {
      setState(779);
      match(SparqlParser::UNION);
      setState(780);
      groupGraphPattern();
      setState(785);
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

//----------------- FilterContext ------------------------------------------------------------------

SparqlParser::FilterContext::FilterContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::FilterContext::FILTER() {
  return getToken(SparqlParser::FILTER, 0);
}

SparqlParser::ConstraintContext* SparqlParser::FilterContext::constraint() {
  return getRuleContext<SparqlParser::ConstraintContext>(0);
}


size_t SparqlParser::FilterContext::getRuleIndex() const {
  return SparqlParser::RuleFilter;
}


antlrcpp::Any SparqlParser::FilterContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitFilter(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::FilterContext* SparqlParser::filter() {
  FilterContext *_localctx = _tracker.createInstance<FilterContext>(_ctx, getState());
  enterRule(_localctx, 134, SparqlParser::RuleFilter);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(786);
    match(SparqlParser::FILTER);
    setState(787);
    constraint();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ConstraintContext ------------------------------------------------------------------

SparqlParser::ConstraintContext::ConstraintContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::ConstraintContext::OPEN_BRACE() {
  return getToken(SparqlParser::OPEN_BRACE, 0);
}

SparqlParser::ExpressionContext* SparqlParser::ConstraintContext::expression() {
  return getRuleContext<SparqlParser::ExpressionContext>(0);
}

tree::TerminalNode* SparqlParser::ConstraintContext::CLOSE_BRACE() {
  return getToken(SparqlParser::CLOSE_BRACE, 0);
}

SparqlParser::BuiltInCallContext* SparqlParser::ConstraintContext::builtInCall() {
  return getRuleContext<SparqlParser::BuiltInCallContext>(0);
}

SparqlParser::FunctionCallContext* SparqlParser::ConstraintContext::functionCall() {
  return getRuleContext<SparqlParser::FunctionCallContext>(0);
}


size_t SparqlParser::ConstraintContext::getRuleIndex() const {
  return SparqlParser::RuleConstraint;
}


antlrcpp::Any SparqlParser::ConstraintContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitConstraint(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::ConstraintContext* SparqlParser::constraint() {
  ConstraintContext *_localctx = _tracker.createInstance<ConstraintContext>(_ctx, getState());
  enterRule(_localctx, 136, SparqlParser::RuleConstraint);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(795);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlParser::OPEN_BRACE: {
        enterOuterAlt(_localctx, 1);
        setState(789);
        match(SparqlParser::OPEN_BRACE);
        setState(790);
        expression(0);
        setState(791);
        match(SparqlParser::CLOSE_BRACE);
        break;
      }

      case SparqlParser::STR:
      case SparqlParser::LANG:
      case SparqlParser::LANGMATCHES:
      case SparqlParser::DATATYPE:
      case SparqlParser::BOUND:
      case SparqlParser::SAMETERM:
      case SparqlParser::ISIRI:
      case SparqlParser::ISURI:
      case SparqlParser::ISBLANK:
      case SparqlParser::ISLITERAL:
      case SparqlParser::REGEX:
      case SparqlParser::SUBSTR:
      case SparqlParser::IRI:
      case SparqlParser::URI:
      case SparqlParser::BNODE:
      case SparqlParser::RAND:
      case SparqlParser::ABS:
      case SparqlParser::CEIL:
      case SparqlParser::FLOOR:
      case SparqlParser::ROUND:
      case SparqlParser::CONCAT:
      case SparqlParser::STRLEN:
      case SparqlParser::UCASE:
      case SparqlParser::LCASE:
      case SparqlParser::ENCODE_FOR_URI:
      case SparqlParser::CONTAINS:
      case SparqlParser::STRSTARTS:
      case SparqlParser::STRENDS:
      case SparqlParser::STRBEFORE:
      case SparqlParser::STRAFTER:
      case SparqlParser::REPLACE:
      case SparqlParser::YEAR:
      case SparqlParser::MONTH:
      case SparqlParser::DAY:
      case SparqlParser::HOURS:
      case SparqlParser::MINUTES:
      case SparqlParser::SECONDS:
      case SparqlParser::TIMEZONE:
      case SparqlParser::TZ:
      case SparqlParser::NOW:
      case SparqlParser::UUID:
      case SparqlParser::STRUUID:
      case SparqlParser::MD5:
      case SparqlParser::SHA1:
      case SparqlParser::SHA256:
      case SparqlParser::SHA384:
      case SparqlParser::SHA512:
      case SparqlParser::COALESCE:
      case SparqlParser::IF:
      case SparqlParser::STRLANG:
      case SparqlParser::STRDT:
      case SparqlParser::ISNUMERIC:
      case SparqlParser::COUNT:
      case SparqlParser::SUM:
      case SparqlParser::MIN:
      case SparqlParser::MAX:
      case SparqlParser::AVG:
      case SparqlParser::SAMPLE:
      case SparqlParser::GROUP_CONCAT:
      case SparqlParser::NOT:
      case SparqlParser::EXISTS: {
        enterOuterAlt(_localctx, 2);
        setState(793);
        builtInCall();
        break;
      }

      case SparqlParser::IRIREF:
      case SparqlParser::PNAME_NS:
      case SparqlParser::PNAME_LN: {
        enterOuterAlt(_localctx, 3);
        setState(794);
        functionCall();
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

//----------------- FunctionCallContext ------------------------------------------------------------------

SparqlParser::FunctionCallContext::FunctionCallContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::IriContext* SparqlParser::FunctionCallContext::iri() {
  return getRuleContext<SparqlParser::IriContext>(0);
}

SparqlParser::ArgListContext* SparqlParser::FunctionCallContext::argList() {
  return getRuleContext<SparqlParser::ArgListContext>(0);
}


size_t SparqlParser::FunctionCallContext::getRuleIndex() const {
  return SparqlParser::RuleFunctionCall;
}


antlrcpp::Any SparqlParser::FunctionCallContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitFunctionCall(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::FunctionCallContext* SparqlParser::functionCall() {
  FunctionCallContext *_localctx = _tracker.createInstance<FunctionCallContext>(_ctx, getState());
  enterRule(_localctx, 138, SparqlParser::RuleFunctionCall);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(797);
    iri();
    setState(798);
    argList();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ArgListContext ------------------------------------------------------------------

SparqlParser::ArgListContext::ArgListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::ArgListContext::OPEN_BRACE() {
  return getToken(SparqlParser::OPEN_BRACE, 0);
}

tree::TerminalNode* SparqlParser::ArgListContext::CLOSE_BRACE() {
  return getToken(SparqlParser::CLOSE_BRACE, 0);
}

SparqlParser::ExpressionListContext* SparqlParser::ArgListContext::expressionList() {
  return getRuleContext<SparqlParser::ExpressionListContext>(0);
}

tree::TerminalNode* SparqlParser::ArgListContext::DISTINCT() {
  return getToken(SparqlParser::DISTINCT, 0);
}


size_t SparqlParser::ArgListContext::getRuleIndex() const {
  return SparqlParser::RuleArgList;
}


antlrcpp::Any SparqlParser::ArgListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitArgList(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::ArgListContext* SparqlParser::argList() {
  ArgListContext *_localctx = _tracker.createInstance<ArgListContext>(_ctx, getState());
  enterRule(_localctx, 140, SparqlParser::RuleArgList);
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
    setState(800);
    match(SparqlParser::OPEN_BRACE);
    setState(806);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlParser::DISTINCT:
      case SparqlParser::STR:
      case SparqlParser::LANG:
      case SparqlParser::LANGMATCHES:
      case SparqlParser::DATATYPE:
      case SparqlParser::BOUND:
      case SparqlParser::SAMETERM:
      case SparqlParser::ISIRI:
      case SparqlParser::ISURI:
      case SparqlParser::ISBLANK:
      case SparqlParser::ISLITERAL:
      case SparqlParser::REGEX:
      case SparqlParser::SUBSTR:
      case SparqlParser::TRUE:
      case SparqlParser::FALSE:
      case SparqlParser::IRI:
      case SparqlParser::URI:
      case SparqlParser::BNODE:
      case SparqlParser::RAND:
      case SparqlParser::ABS:
      case SparqlParser::CEIL:
      case SparqlParser::FLOOR:
      case SparqlParser::ROUND:
      case SparqlParser::CONCAT:
      case SparqlParser::STRLEN:
      case SparqlParser::UCASE:
      case SparqlParser::LCASE:
      case SparqlParser::ENCODE_FOR_URI:
      case SparqlParser::CONTAINS:
      case SparqlParser::STRSTARTS:
      case SparqlParser::STRENDS:
      case SparqlParser::STRBEFORE:
      case SparqlParser::STRAFTER:
      case SparqlParser::REPLACE:
      case SparqlParser::YEAR:
      case SparqlParser::MONTH:
      case SparqlParser::DAY:
      case SparqlParser::HOURS:
      case SparqlParser::MINUTES:
      case SparqlParser::SECONDS:
      case SparqlParser::TIMEZONE:
      case SparqlParser::TZ:
      case SparqlParser::NOW:
      case SparqlParser::UUID:
      case SparqlParser::STRUUID:
      case SparqlParser::MD5:
      case SparqlParser::SHA1:
      case SparqlParser::SHA256:
      case SparqlParser::SHA384:
      case SparqlParser::SHA512:
      case SparqlParser::COALESCE:
      case SparqlParser::IF:
      case SparqlParser::STRLANG:
      case SparqlParser::STRDT:
      case SparqlParser::ISNUMERIC:
      case SparqlParser::COUNT:
      case SparqlParser::SUM:
      case SparqlParser::MIN:
      case SparqlParser::MAX:
      case SparqlParser::AVG:
      case SparqlParser::SAMPLE:
      case SparqlParser::GROUP_CONCAT:
      case SparqlParser::NOT:
      case SparqlParser::EXISTS:
      case SparqlParser::IRIREF:
      case SparqlParser::PNAME_NS:
      case SparqlParser::PNAME_LN:
      case SparqlParser::VAR1:
      case SparqlParser::VAR2:
      case SparqlParser::INTEGER:
      case SparqlParser::DECIMAL:
      case SparqlParser::DOUBLE:
      case SparqlParser::INTEGER_POSITIVE:
      case SparqlParser::DECIMAL_POSITIVE:
      case SparqlParser::DOUBLE_POSITIVE:
      case SparqlParser::INTEGER_NEGATIVE:
      case SparqlParser::DECIMAL_NEGATIVE:
      case SparqlParser::DOUBLE_NEGATIVE:
      case SparqlParser::STRING_LITERAL1:
      case SparqlParser::STRING_LITERAL2:
      case SparqlParser::STRING_LITERAL_LONG1:
      case SparqlParser::STRING_LITERAL_LONG2:
      case SparqlParser::OPEN_BRACE:
      case SparqlParser::PLUS_SIGN:
      case SparqlParser::MINUS_SIGN:
      case SparqlParser::ASTERISK:
      case SparqlParser::NEGATION:
      case SparqlParser::DIVIDE: {
        setState(802);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == SparqlParser::DISTINCT) {
          setState(801);
          match(SparqlParser::DISTINCT);
        }
        setState(804);
        expressionList();
        break;
      }

      case SparqlParser::CLOSE_BRACE: {
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    setState(808);
    match(SparqlParser::CLOSE_BRACE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExpressionListContext ------------------------------------------------------------------

SparqlParser::ExpressionListContext::ExpressionListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<SparqlParser::ExpressionContext *> SparqlParser::ExpressionListContext::expression() {
  return getRuleContexts<SparqlParser::ExpressionContext>();
}

SparqlParser::ExpressionContext* SparqlParser::ExpressionListContext::expression(size_t i) {
  return getRuleContext<SparqlParser::ExpressionContext>(i);
}

std::vector<tree::TerminalNode *> SparqlParser::ExpressionListContext::COMMA() {
  return getTokens(SparqlParser::COMMA);
}

tree::TerminalNode* SparqlParser::ExpressionListContext::COMMA(size_t i) {
  return getToken(SparqlParser::COMMA, i);
}


size_t SparqlParser::ExpressionListContext::getRuleIndex() const {
  return SparqlParser::RuleExpressionList;
}


antlrcpp::Any SparqlParser::ExpressionListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitExpressionList(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::ExpressionListContext* SparqlParser::expressionList() {
  ExpressionListContext *_localctx = _tracker.createInstance<ExpressionListContext>(_ctx, getState());
  enterRule(_localctx, 142, SparqlParser::RuleExpressionList);
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
    setState(810);
    expression(0);
    setState(815);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SparqlParser::COMMA) {
      setState(811);
      match(SparqlParser::COMMA);
      setState(812);
      expression(0);
      setState(817);
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

//----------------- ConstructTemplateContext ------------------------------------------------------------------

SparqlParser::ConstructTemplateContext::ConstructTemplateContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::ConstructTemplateContext::OPEN_CURLY_BRACE() {
  return getToken(SparqlParser::OPEN_CURLY_BRACE, 0);
}

tree::TerminalNode* SparqlParser::ConstructTemplateContext::CLOSE_CURLY_BRACE() {
  return getToken(SparqlParser::CLOSE_CURLY_BRACE, 0);
}

SparqlParser::ConstructTriplesContext* SparqlParser::ConstructTemplateContext::constructTriples() {
  return getRuleContext<SparqlParser::ConstructTriplesContext>(0);
}


size_t SparqlParser::ConstructTemplateContext::getRuleIndex() const {
  return SparqlParser::RuleConstructTemplate;
}


antlrcpp::Any SparqlParser::ConstructTemplateContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitConstructTemplate(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::ConstructTemplateContext* SparqlParser::constructTemplate() {
  ConstructTemplateContext *_localctx = _tracker.createInstance<ConstructTemplateContext>(_ctx, getState());
  enterRule(_localctx, 144, SparqlParser::RuleConstructTemplate);
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
    setState(818);
    match(SparqlParser::OPEN_CURLY_BRACE);
    setState(820);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == SparqlParser::TRUE

    || _la == SparqlParser::FALSE || ((((_la - 115) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 115)) & ((1ULL << (SparqlParser::IRIREF - 115))
      | (1ULL << (SparqlParser::PNAME_NS - 115))
      | (1ULL << (SparqlParser::PNAME_LN - 115))
      | (1ULL << (SparqlParser::BLANK_NODE_LABEL - 115))
      | (1ULL << (SparqlParser::VAR1 - 115))
      | (1ULL << (SparqlParser::VAR2 - 115))
      | (1ULL << (SparqlParser::INTEGER - 115))
      | (1ULL << (SparqlParser::DECIMAL - 115))
      | (1ULL << (SparqlParser::DOUBLE - 115))
      | (1ULL << (SparqlParser::INTEGER_POSITIVE - 115))
      | (1ULL << (SparqlParser::DECIMAL_POSITIVE - 115))
      | (1ULL << (SparqlParser::DOUBLE_POSITIVE - 115))
      | (1ULL << (SparqlParser::INTEGER_NEGATIVE - 115))
      | (1ULL << (SparqlParser::DECIMAL_NEGATIVE - 115))
      | (1ULL << (SparqlParser::DOUBLE_NEGATIVE - 115))
      | (1ULL << (SparqlParser::STRING_LITERAL1 - 115))
      | (1ULL << (SparqlParser::STRING_LITERAL2 - 115))
      | (1ULL << (SparqlParser::STRING_LITERAL_LONG1 - 115))
      | (1ULL << (SparqlParser::STRING_LITERAL_LONG2 - 115))
      | (1ULL << (SparqlParser::OPEN_BRACE - 115))
      | (1ULL << (SparqlParser::OPEN_SQUARE_BRACKET - 115)))) != 0)) {
      setState(819);
      constructTriples();
    }
    setState(822);
    match(SparqlParser::CLOSE_CURLY_BRACE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ConstructTriplesContext ------------------------------------------------------------------

SparqlParser::ConstructTriplesContext::ConstructTriplesContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::TriplesSameSubjectContext* SparqlParser::ConstructTriplesContext::triplesSameSubject() {
  return getRuleContext<SparqlParser::TriplesSameSubjectContext>(0);
}

std::vector<tree::TerminalNode *> SparqlParser::ConstructTriplesContext::DOT() {
  return getTokens(SparqlParser::DOT);
}

tree::TerminalNode* SparqlParser::ConstructTriplesContext::DOT(size_t i) {
  return getToken(SparqlParser::DOT, i);
}

std::vector<SparqlParser::ConstructTriplesContext *> SparqlParser::ConstructTriplesContext::constructTriples() {
  return getRuleContexts<SparqlParser::ConstructTriplesContext>();
}

SparqlParser::ConstructTriplesContext* SparqlParser::ConstructTriplesContext::constructTriples(size_t i) {
  return getRuleContext<SparqlParser::ConstructTriplesContext>(i);
}


size_t SparqlParser::ConstructTriplesContext::getRuleIndex() const {
  return SparqlParser::RuleConstructTriples;
}


antlrcpp::Any SparqlParser::ConstructTriplesContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitConstructTriples(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::ConstructTriplesContext* SparqlParser::constructTriples() {
  ConstructTriplesContext *_localctx = _tracker.createInstance<ConstructTriplesContext>(_ctx, getState());
  enterRule(_localctx, 146, SparqlParser::RuleConstructTriples);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(824);
    triplesSameSubject();
    setState(831);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 83, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(825);
        match(SparqlParser::DOT);
        setState(827);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == SparqlParser::TRUE

        || _la == SparqlParser::FALSE || ((((_la - 115) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 115)) & ((1ULL << (SparqlParser::IRIREF - 115))
          | (1ULL << (SparqlParser::PNAME_NS - 115))
          | (1ULL << (SparqlParser::PNAME_LN - 115))
          | (1ULL << (SparqlParser::BLANK_NODE_LABEL - 115))
          | (1ULL << (SparqlParser::VAR1 - 115))
          | (1ULL << (SparqlParser::VAR2 - 115))
          | (1ULL << (SparqlParser::INTEGER - 115))
          | (1ULL << (SparqlParser::DECIMAL - 115))
          | (1ULL << (SparqlParser::DOUBLE - 115))
          | (1ULL << (SparqlParser::INTEGER_POSITIVE - 115))
          | (1ULL << (SparqlParser::DECIMAL_POSITIVE - 115))
          | (1ULL << (SparqlParser::DOUBLE_POSITIVE - 115))
          | (1ULL << (SparqlParser::INTEGER_NEGATIVE - 115))
          | (1ULL << (SparqlParser::DECIMAL_NEGATIVE - 115))
          | (1ULL << (SparqlParser::DOUBLE_NEGATIVE - 115))
          | (1ULL << (SparqlParser::STRING_LITERAL1 - 115))
          | (1ULL << (SparqlParser::STRING_LITERAL2 - 115))
          | (1ULL << (SparqlParser::STRING_LITERAL_LONG1 - 115))
          | (1ULL << (SparqlParser::STRING_LITERAL_LONG2 - 115))
          | (1ULL << (SparqlParser::OPEN_BRACE - 115))
          | (1ULL << (SparqlParser::OPEN_SQUARE_BRACKET - 115)))) != 0)) {
          setState(826);
          constructTriples();
        } 
      }
      setState(833);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 83, _ctx);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TriplesSameSubjectContext ------------------------------------------------------------------

SparqlParser::TriplesSameSubjectContext::TriplesSameSubjectContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::VarOrTermContext* SparqlParser::TriplesSameSubjectContext::varOrTerm() {
  return getRuleContext<SparqlParser::VarOrTermContext>(0);
}

SparqlParser::PropertyListNotEmptyContext* SparqlParser::TriplesSameSubjectContext::propertyListNotEmpty() {
  return getRuleContext<SparqlParser::PropertyListNotEmptyContext>(0);
}

SparqlParser::TriplesNodeContext* SparqlParser::TriplesSameSubjectContext::triplesNode() {
  return getRuleContext<SparqlParser::TriplesNodeContext>(0);
}

SparqlParser::PropertyListContext* SparqlParser::TriplesSameSubjectContext::propertyList() {
  return getRuleContext<SparqlParser::PropertyListContext>(0);
}


size_t SparqlParser::TriplesSameSubjectContext::getRuleIndex() const {
  return SparqlParser::RuleTriplesSameSubject;
}


antlrcpp::Any SparqlParser::TriplesSameSubjectContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitTriplesSameSubject(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::TriplesSameSubjectContext* SparqlParser::triplesSameSubject() {
  TriplesSameSubjectContext *_localctx = _tracker.createInstance<TriplesSameSubjectContext>(_ctx, getState());
  enterRule(_localctx, 148, SparqlParser::RuleTriplesSameSubject);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(840);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 84, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(834);
      varOrTerm();
      setState(835);
      propertyListNotEmpty();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(837);
      triplesNode();
      setState(838);
      propertyList();
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

//----------------- PropertyListContext ------------------------------------------------------------------

SparqlParser::PropertyListContext::PropertyListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::PropertyListNotEmptyContext* SparqlParser::PropertyListContext::propertyListNotEmpty() {
  return getRuleContext<SparqlParser::PropertyListNotEmptyContext>(0);
}


size_t SparqlParser::PropertyListContext::getRuleIndex() const {
  return SparqlParser::RulePropertyList;
}


antlrcpp::Any SparqlParser::PropertyListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitPropertyList(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::PropertyListContext* SparqlParser::propertyList() {
  PropertyListContext *_localctx = _tracker.createInstance<PropertyListContext>(_ctx, getState());
  enterRule(_localctx, 150, SparqlParser::RulePropertyList);
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
    setState(843);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == SparqlParser::A || ((((_la - 115) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 115)) & ((1ULL << (SparqlParser::IRIREF - 115))
      | (1ULL << (SparqlParser::PNAME_NS - 115))
      | (1ULL << (SparqlParser::PNAME_LN - 115))
      | (1ULL << (SparqlParser::VAR1 - 115))
      | (1ULL << (SparqlParser::VAR2 - 115)))) != 0)) {
      setState(842);
      propertyListNotEmpty();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PropertyListNotEmptyContext ------------------------------------------------------------------

SparqlParser::PropertyListNotEmptyContext::PropertyListNotEmptyContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<SparqlParser::VerbContext *> SparqlParser::PropertyListNotEmptyContext::verb() {
  return getRuleContexts<SparqlParser::VerbContext>();
}

SparqlParser::VerbContext* SparqlParser::PropertyListNotEmptyContext::verb(size_t i) {
  return getRuleContext<SparqlParser::VerbContext>(i);
}

std::vector<SparqlParser::ObjectListContext *> SparqlParser::PropertyListNotEmptyContext::objectList() {
  return getRuleContexts<SparqlParser::ObjectListContext>();
}

SparqlParser::ObjectListContext* SparqlParser::PropertyListNotEmptyContext::objectList(size_t i) {
  return getRuleContext<SparqlParser::ObjectListContext>(i);
}

std::vector<tree::TerminalNode *> SparqlParser::PropertyListNotEmptyContext::SEMICOLON() {
  return getTokens(SparqlParser::SEMICOLON);
}

tree::TerminalNode* SparqlParser::PropertyListNotEmptyContext::SEMICOLON(size_t i) {
  return getToken(SparqlParser::SEMICOLON, i);
}


size_t SparqlParser::PropertyListNotEmptyContext::getRuleIndex() const {
  return SparqlParser::RulePropertyListNotEmpty;
}


antlrcpp::Any SparqlParser::PropertyListNotEmptyContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitPropertyListNotEmpty(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::PropertyListNotEmptyContext* SparqlParser::propertyListNotEmpty() {
  PropertyListNotEmptyContext *_localctx = _tracker.createInstance<PropertyListNotEmptyContext>(_ctx, getState());
  enterRule(_localctx, 152, SparqlParser::RulePropertyListNotEmpty);
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
    setState(845);
    verb();
    setState(846);
    objectList();
    setState(855);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SparqlParser::SEMICOLON) {
      setState(847);
      match(SparqlParser::SEMICOLON);
      setState(851);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == SparqlParser::A || ((((_la - 115) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 115)) & ((1ULL << (SparqlParser::IRIREF - 115))
        | (1ULL << (SparqlParser::PNAME_NS - 115))
        | (1ULL << (SparqlParser::PNAME_LN - 115))
        | (1ULL << (SparqlParser::VAR1 - 115))
        | (1ULL << (SparqlParser::VAR2 - 115)))) != 0)) {
        setState(848);
        verb();
        setState(849);
        objectList();
      }
      setState(857);
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

//----------------- VerbContext ------------------------------------------------------------------

SparqlParser::VerbContext::VerbContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::VarOrIRIContext* SparqlParser::VerbContext::varOrIRI() {
  return getRuleContext<SparqlParser::VarOrIRIContext>(0);
}

tree::TerminalNode* SparqlParser::VerbContext::A() {
  return getToken(SparqlParser::A, 0);
}


size_t SparqlParser::VerbContext::getRuleIndex() const {
  return SparqlParser::RuleVerb;
}


antlrcpp::Any SparqlParser::VerbContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitVerb(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::VerbContext* SparqlParser::verb() {
  VerbContext *_localctx = _tracker.createInstance<VerbContext>(_ctx, getState());
  enterRule(_localctx, 154, SparqlParser::RuleVerb);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(860);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlParser::IRIREF:
      case SparqlParser::PNAME_NS:
      case SparqlParser::PNAME_LN:
      case SparqlParser::VAR1:
      case SparqlParser::VAR2: {
        enterOuterAlt(_localctx, 1);
        setState(858);
        varOrIRI();
        break;
      }

      case SparqlParser::A: {
        enterOuterAlt(_localctx, 2);
        setState(859);
        match(SparqlParser::A);
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

//----------------- ObjectListContext ------------------------------------------------------------------

SparqlParser::ObjectListContext::ObjectListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<SparqlParser::ObjectContext *> SparqlParser::ObjectListContext::object() {
  return getRuleContexts<SparqlParser::ObjectContext>();
}

SparqlParser::ObjectContext* SparqlParser::ObjectListContext::object(size_t i) {
  return getRuleContext<SparqlParser::ObjectContext>(i);
}

std::vector<tree::TerminalNode *> SparqlParser::ObjectListContext::COMMA() {
  return getTokens(SparqlParser::COMMA);
}

tree::TerminalNode* SparqlParser::ObjectListContext::COMMA(size_t i) {
  return getToken(SparqlParser::COMMA, i);
}


size_t SparqlParser::ObjectListContext::getRuleIndex() const {
  return SparqlParser::RuleObjectList;
}


antlrcpp::Any SparqlParser::ObjectListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitObjectList(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::ObjectListContext* SparqlParser::objectList() {
  ObjectListContext *_localctx = _tracker.createInstance<ObjectListContext>(_ctx, getState());
  enterRule(_localctx, 156, SparqlParser::RuleObjectList);
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
    setState(862);
    object();
    setState(867);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SparqlParser::COMMA) {
      setState(863);
      match(SparqlParser::COMMA);
      setState(864);
      object();
      setState(869);
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

//----------------- ObjectContext ------------------------------------------------------------------

SparqlParser::ObjectContext::ObjectContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::GraphNodeContext* SparqlParser::ObjectContext::graphNode() {
  return getRuleContext<SparqlParser::GraphNodeContext>(0);
}


size_t SparqlParser::ObjectContext::getRuleIndex() const {
  return SparqlParser::RuleObject;
}


antlrcpp::Any SparqlParser::ObjectContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitObject(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::ObjectContext* SparqlParser::object() {
  ObjectContext *_localctx = _tracker.createInstance<ObjectContext>(_ctx, getState());
  enterRule(_localctx, 158, SparqlParser::RuleObject);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(870);
    graphNode();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TriplesSameSubjectPathContext ------------------------------------------------------------------

SparqlParser::TriplesSameSubjectPathContext::TriplesSameSubjectPathContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::VarOrTermContext* SparqlParser::TriplesSameSubjectPathContext::varOrTerm() {
  return getRuleContext<SparqlParser::VarOrTermContext>(0);
}

SparqlParser::PropertyListPathNotEmptyContext* SparqlParser::TriplesSameSubjectPathContext::propertyListPathNotEmpty() {
  return getRuleContext<SparqlParser::PropertyListPathNotEmptyContext>(0);
}

SparqlParser::TriplesNodePathContext* SparqlParser::TriplesSameSubjectPathContext::triplesNodePath() {
  return getRuleContext<SparqlParser::TriplesNodePathContext>(0);
}

SparqlParser::PropertyListPathContext* SparqlParser::TriplesSameSubjectPathContext::propertyListPath() {
  return getRuleContext<SparqlParser::PropertyListPathContext>(0);
}


size_t SparqlParser::TriplesSameSubjectPathContext::getRuleIndex() const {
  return SparqlParser::RuleTriplesSameSubjectPath;
}


antlrcpp::Any SparqlParser::TriplesSameSubjectPathContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitTriplesSameSubjectPath(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::TriplesSameSubjectPathContext* SparqlParser::triplesSameSubjectPath() {
  TriplesSameSubjectPathContext *_localctx = _tracker.createInstance<TriplesSameSubjectPathContext>(_ctx, getState());
  enterRule(_localctx, 160, SparqlParser::RuleTriplesSameSubjectPath);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(878);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 90, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(872);
      varOrTerm();
      setState(873);
      propertyListPathNotEmpty();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(875);
      triplesNodePath();
      setState(876);
      propertyListPath();
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

//----------------- PropertyListPathContext ------------------------------------------------------------------

SparqlParser::PropertyListPathContext::PropertyListPathContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::PropertyListPathNotEmptyContext* SparqlParser::PropertyListPathContext::propertyListPathNotEmpty() {
  return getRuleContext<SparqlParser::PropertyListPathNotEmptyContext>(0);
}


size_t SparqlParser::PropertyListPathContext::getRuleIndex() const {
  return SparqlParser::RulePropertyListPath;
}


antlrcpp::Any SparqlParser::PropertyListPathContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitPropertyListPath(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::PropertyListPathContext* SparqlParser::propertyListPath() {
  PropertyListPathContext *_localctx = _tracker.createInstance<PropertyListPathContext>(_ctx, getState());
  enterRule(_localctx, 162, SparqlParser::RulePropertyListPath);
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
    setState(881);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == SparqlParser::A || ((((_la - 115) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 115)) & ((1ULL << (SparqlParser::IRIREF - 115))
      | (1ULL << (SparqlParser::PNAME_NS - 115))
      | (1ULL << (SparqlParser::PNAME_LN - 115))
      | (1ULL << (SparqlParser::VAR1 - 115))
      | (1ULL << (SparqlParser::VAR2 - 115))
      | (1ULL << (SparqlParser::INVERSE - 115))
      | (1ULL << (SparqlParser::OPEN_BRACE - 115))
      | (1ULL << (SparqlParser::NEGATION - 115)))) != 0)) {
      setState(880);
      propertyListPathNotEmpty();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PropertyListPathNotEmptyContext ------------------------------------------------------------------

SparqlParser::PropertyListPathNotEmptyContext::PropertyListPathNotEmptyContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::ObjectListPathContext* SparqlParser::PropertyListPathNotEmptyContext::objectListPath() {
  return getRuleContext<SparqlParser::ObjectListPathContext>(0);
}

SparqlParser::VerbPathContext* SparqlParser::PropertyListPathNotEmptyContext::verbPath() {
  return getRuleContext<SparqlParser::VerbPathContext>(0);
}

SparqlParser::VerbSimpleContext* SparqlParser::PropertyListPathNotEmptyContext::verbSimple() {
  return getRuleContext<SparqlParser::VerbSimpleContext>(0);
}

std::vector<tree::TerminalNode *> SparqlParser::PropertyListPathNotEmptyContext::SEMICOLON() {
  return getTokens(SparqlParser::SEMICOLON);
}

tree::TerminalNode* SparqlParser::PropertyListPathNotEmptyContext::SEMICOLON(size_t i) {
  return getToken(SparqlParser::SEMICOLON, i);
}

std::vector<SparqlParser::PropertyListPathNotEmptyListContext *> SparqlParser::PropertyListPathNotEmptyContext::propertyListPathNotEmptyList() {
  return getRuleContexts<SparqlParser::PropertyListPathNotEmptyListContext>();
}

SparqlParser::PropertyListPathNotEmptyListContext* SparqlParser::PropertyListPathNotEmptyContext::propertyListPathNotEmptyList(size_t i) {
  return getRuleContext<SparqlParser::PropertyListPathNotEmptyListContext>(i);
}


size_t SparqlParser::PropertyListPathNotEmptyContext::getRuleIndex() const {
  return SparqlParser::RulePropertyListPathNotEmpty;
}


antlrcpp::Any SparqlParser::PropertyListPathNotEmptyContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitPropertyListPathNotEmpty(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::PropertyListPathNotEmptyContext* SparqlParser::propertyListPathNotEmpty() {
  PropertyListPathNotEmptyContext *_localctx = _tracker.createInstance<PropertyListPathNotEmptyContext>(_ctx, getState());
  enterRule(_localctx, 164, SparqlParser::RulePropertyListPathNotEmpty);
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
    setState(885);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlParser::A:
      case SparqlParser::IRIREF:
      case SparqlParser::PNAME_NS:
      case SparqlParser::PNAME_LN:
      case SparqlParser::INVERSE:
      case SparqlParser::OPEN_BRACE:
      case SparqlParser::NEGATION: {
        setState(883);
        verbPath();
        break;
      }

      case SparqlParser::VAR1:
      case SparqlParser::VAR2: {
        setState(884);
        verbSimple();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    setState(887);
    objectListPath();
    setState(894);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SparqlParser::SEMICOLON) {
      setState(888);
      match(SparqlParser::SEMICOLON);
      setState(890);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == SparqlParser::A || ((((_la - 115) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 115)) & ((1ULL << (SparqlParser::IRIREF - 115))
        | (1ULL << (SparqlParser::PNAME_NS - 115))
        | (1ULL << (SparqlParser::PNAME_LN - 115))
        | (1ULL << (SparqlParser::VAR1 - 115))
        | (1ULL << (SparqlParser::VAR2 - 115))
        | (1ULL << (SparqlParser::INVERSE - 115))
        | (1ULL << (SparqlParser::OPEN_BRACE - 115))
        | (1ULL << (SparqlParser::NEGATION - 115)))) != 0)) {
        setState(889);
        propertyListPathNotEmptyList();
      }
      setState(896);
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

//----------------- PropertyListPathNotEmptyListContext ------------------------------------------------------------------

SparqlParser::PropertyListPathNotEmptyListContext::PropertyListPathNotEmptyListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::ObjectListContext* SparqlParser::PropertyListPathNotEmptyListContext::objectList() {
  return getRuleContext<SparqlParser::ObjectListContext>(0);
}

SparqlParser::VerbPathContext* SparqlParser::PropertyListPathNotEmptyListContext::verbPath() {
  return getRuleContext<SparqlParser::VerbPathContext>(0);
}

SparqlParser::VerbSimpleContext* SparqlParser::PropertyListPathNotEmptyListContext::verbSimple() {
  return getRuleContext<SparqlParser::VerbSimpleContext>(0);
}


size_t SparqlParser::PropertyListPathNotEmptyListContext::getRuleIndex() const {
  return SparqlParser::RulePropertyListPathNotEmptyList;
}


antlrcpp::Any SparqlParser::PropertyListPathNotEmptyListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitPropertyListPathNotEmptyList(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::PropertyListPathNotEmptyListContext* SparqlParser::propertyListPathNotEmptyList() {
  PropertyListPathNotEmptyListContext *_localctx = _tracker.createInstance<PropertyListPathNotEmptyListContext>(_ctx, getState());
  enterRule(_localctx, 166, SparqlParser::RulePropertyListPathNotEmptyList);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(899);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlParser::A:
      case SparqlParser::IRIREF:
      case SparqlParser::PNAME_NS:
      case SparqlParser::PNAME_LN:
      case SparqlParser::INVERSE:
      case SparqlParser::OPEN_BRACE:
      case SparqlParser::NEGATION: {
        setState(897);
        verbPath();
        break;
      }

      case SparqlParser::VAR1:
      case SparqlParser::VAR2: {
        setState(898);
        verbSimple();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    setState(901);
    objectList();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- VerbPathContext ------------------------------------------------------------------

SparqlParser::VerbPathContext::VerbPathContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::PathContext* SparqlParser::VerbPathContext::path() {
  return getRuleContext<SparqlParser::PathContext>(0);
}


size_t SparqlParser::VerbPathContext::getRuleIndex() const {
  return SparqlParser::RuleVerbPath;
}


antlrcpp::Any SparqlParser::VerbPathContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitVerbPath(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::VerbPathContext* SparqlParser::verbPath() {
  VerbPathContext *_localctx = _tracker.createInstance<VerbPathContext>(_ctx, getState());
  enterRule(_localctx, 168, SparqlParser::RuleVerbPath);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(903);
    path();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- VerbSimpleContext ------------------------------------------------------------------

SparqlParser::VerbSimpleContext::VerbSimpleContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::VarContext* SparqlParser::VerbSimpleContext::var() {
  return getRuleContext<SparqlParser::VarContext>(0);
}


size_t SparqlParser::VerbSimpleContext::getRuleIndex() const {
  return SparqlParser::RuleVerbSimple;
}


antlrcpp::Any SparqlParser::VerbSimpleContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitVerbSimple(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::VerbSimpleContext* SparqlParser::verbSimple() {
  VerbSimpleContext *_localctx = _tracker.createInstance<VerbSimpleContext>(_ctx, getState());
  enterRule(_localctx, 170, SparqlParser::RuleVerbSimple);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(905);
    var();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ObjectListPathContext ------------------------------------------------------------------

SparqlParser::ObjectListPathContext::ObjectListPathContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<SparqlParser::ObjectPathContext *> SparqlParser::ObjectListPathContext::objectPath() {
  return getRuleContexts<SparqlParser::ObjectPathContext>();
}

SparqlParser::ObjectPathContext* SparqlParser::ObjectListPathContext::objectPath(size_t i) {
  return getRuleContext<SparqlParser::ObjectPathContext>(i);
}

std::vector<tree::TerminalNode *> SparqlParser::ObjectListPathContext::COMMA() {
  return getTokens(SparqlParser::COMMA);
}

tree::TerminalNode* SparqlParser::ObjectListPathContext::COMMA(size_t i) {
  return getToken(SparqlParser::COMMA, i);
}


size_t SparqlParser::ObjectListPathContext::getRuleIndex() const {
  return SparqlParser::RuleObjectListPath;
}


antlrcpp::Any SparqlParser::ObjectListPathContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitObjectListPath(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::ObjectListPathContext* SparqlParser::objectListPath() {
  ObjectListPathContext *_localctx = _tracker.createInstance<ObjectListPathContext>(_ctx, getState());
  enterRule(_localctx, 172, SparqlParser::RuleObjectListPath);
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
    setState(907);
    objectPath();
    setState(912);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SparqlParser::COMMA) {
      setState(908);
      match(SparqlParser::COMMA);
      setState(909);
      objectPath();
      setState(914);
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

//----------------- ObjectPathContext ------------------------------------------------------------------

SparqlParser::ObjectPathContext::ObjectPathContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::GraphNodePathContext* SparqlParser::ObjectPathContext::graphNodePath() {
  return getRuleContext<SparqlParser::GraphNodePathContext>(0);
}


size_t SparqlParser::ObjectPathContext::getRuleIndex() const {
  return SparqlParser::RuleObjectPath;
}


antlrcpp::Any SparqlParser::ObjectPathContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitObjectPath(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::ObjectPathContext* SparqlParser::objectPath() {
  ObjectPathContext *_localctx = _tracker.createInstance<ObjectPathContext>(_ctx, getState());
  enterRule(_localctx, 174, SparqlParser::RuleObjectPath);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(915);
    graphNodePath();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PathContext ------------------------------------------------------------------

SparqlParser::PathContext::PathContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::PathAlternativeContext* SparqlParser::PathContext::pathAlternative() {
  return getRuleContext<SparqlParser::PathAlternativeContext>(0);
}


size_t SparqlParser::PathContext::getRuleIndex() const {
  return SparqlParser::RulePath;
}


antlrcpp::Any SparqlParser::PathContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitPath(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::PathContext* SparqlParser::path() {
  PathContext *_localctx = _tracker.createInstance<PathContext>(_ctx, getState());
  enterRule(_localctx, 176, SparqlParser::RulePath);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(917);
    pathAlternative();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PathAlternativeContext ------------------------------------------------------------------

SparqlParser::PathAlternativeContext::PathAlternativeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<SparqlParser::PathSequenceContext *> SparqlParser::PathAlternativeContext::pathSequence() {
  return getRuleContexts<SparqlParser::PathSequenceContext>();
}

SparqlParser::PathSequenceContext* SparqlParser::PathAlternativeContext::pathSequence(size_t i) {
  return getRuleContext<SparqlParser::PathSequenceContext>(i);
}

std::vector<tree::TerminalNode *> SparqlParser::PathAlternativeContext::PIPE() {
  return getTokens(SparqlParser::PIPE);
}

tree::TerminalNode* SparqlParser::PathAlternativeContext::PIPE(size_t i) {
  return getToken(SparqlParser::PIPE, i);
}


size_t SparqlParser::PathAlternativeContext::getRuleIndex() const {
  return SparqlParser::RulePathAlternative;
}


antlrcpp::Any SparqlParser::PathAlternativeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitPathAlternative(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::PathAlternativeContext* SparqlParser::pathAlternative() {
  PathAlternativeContext *_localctx = _tracker.createInstance<PathAlternativeContext>(_ctx, getState());
  enterRule(_localctx, 178, SparqlParser::RulePathAlternative);
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
    setState(919);
    pathSequence();
    setState(924);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SparqlParser::PIPE) {
      setState(920);
      match(SparqlParser::PIPE);
      setState(921);
      pathSequence();
      setState(926);
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

SparqlParser::PathSequenceContext::PathSequenceContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<SparqlParser::PathEltOrInverseContext *> SparqlParser::PathSequenceContext::pathEltOrInverse() {
  return getRuleContexts<SparqlParser::PathEltOrInverseContext>();
}

SparqlParser::PathEltOrInverseContext* SparqlParser::PathSequenceContext::pathEltOrInverse(size_t i) {
  return getRuleContext<SparqlParser::PathEltOrInverseContext>(i);
}

std::vector<tree::TerminalNode *> SparqlParser::PathSequenceContext::DIVIDE() {
  return getTokens(SparqlParser::DIVIDE);
}

tree::TerminalNode* SparqlParser::PathSequenceContext::DIVIDE(size_t i) {
  return getToken(SparqlParser::DIVIDE, i);
}


size_t SparqlParser::PathSequenceContext::getRuleIndex() const {
  return SparqlParser::RulePathSequence;
}


antlrcpp::Any SparqlParser::PathSequenceContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitPathSequence(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::PathSequenceContext* SparqlParser::pathSequence() {
  PathSequenceContext *_localctx = _tracker.createInstance<PathSequenceContext>(_ctx, getState());
  enterRule(_localctx, 180, SparqlParser::RulePathSequence);
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
    setState(927);
    pathEltOrInverse();
    setState(932);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SparqlParser::DIVIDE) {
      setState(928);
      match(SparqlParser::DIVIDE);
      setState(929);
      pathEltOrInverse();
      setState(934);
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

//----------------- PathEltContext ------------------------------------------------------------------

SparqlParser::PathEltContext::PathEltContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::PathPrimaryContext* SparqlParser::PathEltContext::pathPrimary() {
  return getRuleContext<SparqlParser::PathPrimaryContext>(0);
}

SparqlParser::PathModContext* SparqlParser::PathEltContext::pathMod() {
  return getRuleContext<SparqlParser::PathModContext>(0);
}


size_t SparqlParser::PathEltContext::getRuleIndex() const {
  return SparqlParser::RulePathElt;
}


antlrcpp::Any SparqlParser::PathEltContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitPathElt(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::PathEltContext* SparqlParser::pathElt() {
  PathEltContext *_localctx = _tracker.createInstance<PathEltContext>(_ctx, getState());
  enterRule(_localctx, 182, SparqlParser::RulePathElt);
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
    setState(935);
    pathPrimary();
    setState(937);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (((((_la - 151) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 151)) & ((1ULL << (SparqlParser::PLUS_SIGN - 151))
      | (1ULL << (SparqlParser::ASTERISK - 151))
      | (1ULL << (SparqlParser::QUESTION_MARK - 151)))) != 0)) {
      setState(936);
      pathMod();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PathEltOrInverseContext ------------------------------------------------------------------

SparqlParser::PathEltOrInverseContext::PathEltOrInverseContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::PathEltContext* SparqlParser::PathEltOrInverseContext::pathElt() {
  return getRuleContext<SparqlParser::PathEltContext>(0);
}

tree::TerminalNode* SparqlParser::PathEltOrInverseContext::INVERSE() {
  return getToken(SparqlParser::INVERSE, 0);
}


size_t SparqlParser::PathEltOrInverseContext::getRuleIndex() const {
  return SparqlParser::RulePathEltOrInverse;
}


antlrcpp::Any SparqlParser::PathEltOrInverseContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitPathEltOrInverse(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::PathEltOrInverseContext* SparqlParser::pathEltOrInverse() {
  PathEltOrInverseContext *_localctx = _tracker.createInstance<PathEltOrInverseContext>(_ctx, getState());
  enterRule(_localctx, 184, SparqlParser::RulePathEltOrInverse);
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
    setState(940);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == SparqlParser::INVERSE) {
      setState(939);
      match(SparqlParser::INVERSE);
    }
    setState(942);
    pathElt();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PathModContext ------------------------------------------------------------------

SparqlParser::PathModContext::PathModContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::PathModContext::QUESTION_MARK() {
  return getToken(SparqlParser::QUESTION_MARK, 0);
}

tree::TerminalNode* SparqlParser::PathModContext::ASTERISK() {
  return getToken(SparqlParser::ASTERISK, 0);
}

tree::TerminalNode* SparqlParser::PathModContext::PLUS_SIGN() {
  return getToken(SparqlParser::PLUS_SIGN, 0);
}


size_t SparqlParser::PathModContext::getRuleIndex() const {
  return SparqlParser::RulePathMod;
}


antlrcpp::Any SparqlParser::PathModContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitPathMod(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::PathModContext* SparqlParser::pathMod() {
  PathModContext *_localctx = _tracker.createInstance<PathModContext>(_ctx, getState());
  enterRule(_localctx, 186, SparqlParser::RulePathMod);
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
    setState(944);
    antlrcpp::downCast<PathModContext *>(_localctx)->op = _input->LT(1);
    _la = _input->LA(1);
    if (!(((((_la - 151) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 151)) & ((1ULL << (SparqlParser::PLUS_SIGN - 151))
      | (1ULL << (SparqlParser::ASTERISK - 151))
      | (1ULL << (SparqlParser::QUESTION_MARK - 151)))) != 0))) {
      antlrcpp::downCast<PathModContext *>(_localctx)->op = _errHandler->recoverInline(this);
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

//----------------- PathPrimaryContext ------------------------------------------------------------------

SparqlParser::PathPrimaryContext::PathPrimaryContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::IriContext* SparqlParser::PathPrimaryContext::iri() {
  return getRuleContext<SparqlParser::IriContext>(0);
}

tree::TerminalNode* SparqlParser::PathPrimaryContext::A() {
  return getToken(SparqlParser::A, 0);
}

tree::TerminalNode* SparqlParser::PathPrimaryContext::NEGATION() {
  return getToken(SparqlParser::NEGATION, 0);
}

SparqlParser::PathNegatedPropertySetContext* SparqlParser::PathPrimaryContext::pathNegatedPropertySet() {
  return getRuleContext<SparqlParser::PathNegatedPropertySetContext>(0);
}

tree::TerminalNode* SparqlParser::PathPrimaryContext::OPEN_BRACE() {
  return getToken(SparqlParser::OPEN_BRACE, 0);
}

SparqlParser::PathContext* SparqlParser::PathPrimaryContext::path() {
  return getRuleContext<SparqlParser::PathContext>(0);
}

tree::TerminalNode* SparqlParser::PathPrimaryContext::CLOSE_BRACE() {
  return getToken(SparqlParser::CLOSE_BRACE, 0);
}


size_t SparqlParser::PathPrimaryContext::getRuleIndex() const {
  return SparqlParser::RulePathPrimary;
}


antlrcpp::Any SparqlParser::PathPrimaryContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitPathPrimary(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::PathPrimaryContext* SparqlParser::pathPrimary() {
  PathPrimaryContext *_localctx = _tracker.createInstance<PathPrimaryContext>(_ctx, getState());
  enterRule(_localctx, 188, SparqlParser::RulePathPrimary);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(954);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlParser::IRIREF:
      case SparqlParser::PNAME_NS:
      case SparqlParser::PNAME_LN: {
        enterOuterAlt(_localctx, 1);
        setState(946);
        iri();
        break;
      }

      case SparqlParser::A: {
        enterOuterAlt(_localctx, 2);
        setState(947);
        match(SparqlParser::A);
        break;
      }

      case SparqlParser::NEGATION: {
        enterOuterAlt(_localctx, 3);
        setState(948);
        match(SparqlParser::NEGATION);
        setState(949);
        pathNegatedPropertySet();
        break;
      }

      case SparqlParser::OPEN_BRACE: {
        enterOuterAlt(_localctx, 4);
        setState(950);
        match(SparqlParser::OPEN_BRACE);
        setState(951);
        path();
        setState(952);
        match(SparqlParser::CLOSE_BRACE);
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

//----------------- PathNegatedPropertySetContext ------------------------------------------------------------------

SparqlParser::PathNegatedPropertySetContext::PathNegatedPropertySetContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<SparqlParser::PathOneInPropertySetContext *> SparqlParser::PathNegatedPropertySetContext::pathOneInPropertySet() {
  return getRuleContexts<SparqlParser::PathOneInPropertySetContext>();
}

SparqlParser::PathOneInPropertySetContext* SparqlParser::PathNegatedPropertySetContext::pathOneInPropertySet(size_t i) {
  return getRuleContext<SparqlParser::PathOneInPropertySetContext>(i);
}

tree::TerminalNode* SparqlParser::PathNegatedPropertySetContext::OPEN_BRACE() {
  return getToken(SparqlParser::OPEN_BRACE, 0);
}

tree::TerminalNode* SparqlParser::PathNegatedPropertySetContext::CLOSE_BRACE() {
  return getToken(SparqlParser::CLOSE_BRACE, 0);
}

std::vector<tree::TerminalNode *> SparqlParser::PathNegatedPropertySetContext::PIPE() {
  return getTokens(SparqlParser::PIPE);
}

tree::TerminalNode* SparqlParser::PathNegatedPropertySetContext::PIPE(size_t i) {
  return getToken(SparqlParser::PIPE, i);
}


size_t SparqlParser::PathNegatedPropertySetContext::getRuleIndex() const {
  return SparqlParser::RulePathNegatedPropertySet;
}


antlrcpp::Any SparqlParser::PathNegatedPropertySetContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitPathNegatedPropertySet(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::PathNegatedPropertySetContext* SparqlParser::pathNegatedPropertySet() {
  PathNegatedPropertySetContext *_localctx = _tracker.createInstance<PathNegatedPropertySetContext>(_ctx, getState());
  enterRule(_localctx, 190, SparqlParser::RulePathNegatedPropertySet);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(969);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlParser::A:
      case SparqlParser::IRIREF:
      case SparqlParser::PNAME_NS:
      case SparqlParser::PNAME_LN:
      case SparqlParser::INVERSE: {
        enterOuterAlt(_localctx, 1);
        setState(956);
        pathOneInPropertySet();
        break;
      }

      case SparqlParser::OPEN_BRACE: {
        enterOuterAlt(_localctx, 2);
        setState(957);
        match(SparqlParser::OPEN_BRACE);
        setState(966);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == SparqlParser::A || ((((_la - 115) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 115)) & ((1ULL << (SparqlParser::IRIREF - 115))
          | (1ULL << (SparqlParser::PNAME_NS - 115))
          | (1ULL << (SparqlParser::PNAME_LN - 115))
          | (1ULL << (SparqlParser::INVERSE - 115)))) != 0)) {
          setState(958);
          pathOneInPropertySet();
          setState(963);
          _errHandler->sync(this);
          _la = _input->LA(1);
          while (_la == SparqlParser::PIPE) {
            setState(959);
            match(SparqlParser::PIPE);
            setState(960);
            pathOneInPropertySet();
            setState(965);
            _errHandler->sync(this);
            _la = _input->LA(1);
          }
        }
        setState(968);
        match(SparqlParser::CLOSE_BRACE);
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

//----------------- PathOneInPropertySetContext ------------------------------------------------------------------

SparqlParser::PathOneInPropertySetContext::PathOneInPropertySetContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::IriContext* SparqlParser::PathOneInPropertySetContext::iri() {
  return getRuleContext<SparqlParser::IriContext>(0);
}

tree::TerminalNode* SparqlParser::PathOneInPropertySetContext::A() {
  return getToken(SparqlParser::A, 0);
}

tree::TerminalNode* SparqlParser::PathOneInPropertySetContext::INVERSE() {
  return getToken(SparqlParser::INVERSE, 0);
}


size_t SparqlParser::PathOneInPropertySetContext::getRuleIndex() const {
  return SparqlParser::RulePathOneInPropertySet;
}


antlrcpp::Any SparqlParser::PathOneInPropertySetContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitPathOneInPropertySet(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::PathOneInPropertySetContext* SparqlParser::pathOneInPropertySet() {
  PathOneInPropertySetContext *_localctx = _tracker.createInstance<PathOneInPropertySetContext>(_ctx, getState());
  enterRule(_localctx, 192, SparqlParser::RulePathOneInPropertySet);
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
    setState(972);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == SparqlParser::INVERSE) {
      setState(971);
      match(SparqlParser::INVERSE);
    }
    setState(976);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlParser::IRIREF:
      case SparqlParser::PNAME_NS:
      case SparqlParser::PNAME_LN: {
        setState(974);
        iri();
        break;
      }

      case SparqlParser::A: {
        setState(975);
        match(SparqlParser::A);
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

//----------------- IntegerContext ------------------------------------------------------------------

SparqlParser::IntegerContext::IntegerContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::IntegerContext::INTEGER() {
  return getToken(SparqlParser::INTEGER, 0);
}


size_t SparqlParser::IntegerContext::getRuleIndex() const {
  return SparqlParser::RuleInteger;
}


antlrcpp::Any SparqlParser::IntegerContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitInteger(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::IntegerContext* SparqlParser::integer() {
  IntegerContext *_localctx = _tracker.createInstance<IntegerContext>(_ctx, getState());
  enterRule(_localctx, 194, SparqlParser::RuleInteger);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(978);
    match(SparqlParser::INTEGER);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TriplesNodeContext ------------------------------------------------------------------

SparqlParser::TriplesNodeContext::TriplesNodeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::CollectionContext* SparqlParser::TriplesNodeContext::collection() {
  return getRuleContext<SparqlParser::CollectionContext>(0);
}

SparqlParser::BlankNodePropertyListContext* SparqlParser::TriplesNodeContext::blankNodePropertyList() {
  return getRuleContext<SparqlParser::BlankNodePropertyListContext>(0);
}


size_t SparqlParser::TriplesNodeContext::getRuleIndex() const {
  return SparqlParser::RuleTriplesNode;
}


antlrcpp::Any SparqlParser::TriplesNodeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitTriplesNode(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::TriplesNodeContext* SparqlParser::triplesNode() {
  TriplesNodeContext *_localctx = _tracker.createInstance<TriplesNodeContext>(_ctx, getState());
  enterRule(_localctx, 196, SparqlParser::RuleTriplesNode);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(982);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlParser::OPEN_BRACE: {
        enterOuterAlt(_localctx, 1);
        setState(980);
        collection();
        break;
      }

      case SparqlParser::OPEN_SQUARE_BRACKET: {
        enterOuterAlt(_localctx, 2);
        setState(981);
        blankNodePropertyList();
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

//----------------- BlankNodePropertyListContext ------------------------------------------------------------------

SparqlParser::BlankNodePropertyListContext::BlankNodePropertyListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::BlankNodePropertyListContext::OPEN_SQUARE_BRACKET() {
  return getToken(SparqlParser::OPEN_SQUARE_BRACKET, 0);
}

SparqlParser::PropertyListNotEmptyContext* SparqlParser::BlankNodePropertyListContext::propertyListNotEmpty() {
  return getRuleContext<SparqlParser::PropertyListNotEmptyContext>(0);
}

tree::TerminalNode* SparqlParser::BlankNodePropertyListContext::CLOSE_SQUARE_BRACKET() {
  return getToken(SparqlParser::CLOSE_SQUARE_BRACKET, 0);
}


size_t SparqlParser::BlankNodePropertyListContext::getRuleIndex() const {
  return SparqlParser::RuleBlankNodePropertyList;
}


antlrcpp::Any SparqlParser::BlankNodePropertyListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitBlankNodePropertyList(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::BlankNodePropertyListContext* SparqlParser::blankNodePropertyList() {
  BlankNodePropertyListContext *_localctx = _tracker.createInstance<BlankNodePropertyListContext>(_ctx, getState());
  enterRule(_localctx, 198, SparqlParser::RuleBlankNodePropertyList);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(984);
    match(SparqlParser::OPEN_SQUARE_BRACKET);
    setState(985);
    propertyListNotEmpty();
    setState(986);
    match(SparqlParser::CLOSE_SQUARE_BRACKET);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TriplesNodePathContext ------------------------------------------------------------------

SparqlParser::TriplesNodePathContext::TriplesNodePathContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::CollectionPathContext* SparqlParser::TriplesNodePathContext::collectionPath() {
  return getRuleContext<SparqlParser::CollectionPathContext>(0);
}

SparqlParser::BlankNodePropertyListPathContext* SparqlParser::TriplesNodePathContext::blankNodePropertyListPath() {
  return getRuleContext<SparqlParser::BlankNodePropertyListPathContext>(0);
}


size_t SparqlParser::TriplesNodePathContext::getRuleIndex() const {
  return SparqlParser::RuleTriplesNodePath;
}


antlrcpp::Any SparqlParser::TriplesNodePathContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitTriplesNodePath(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::TriplesNodePathContext* SparqlParser::triplesNodePath() {
  TriplesNodePathContext *_localctx = _tracker.createInstance<TriplesNodePathContext>(_ctx, getState());
  enterRule(_localctx, 200, SparqlParser::RuleTriplesNodePath);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(990);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlParser::OPEN_BRACE: {
        enterOuterAlt(_localctx, 1);
        setState(988);
        collectionPath();
        break;
      }

      case SparqlParser::OPEN_SQUARE_BRACKET: {
        enterOuterAlt(_localctx, 2);
        setState(989);
        blankNodePropertyListPath();
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

//----------------- BlankNodePropertyListPathContext ------------------------------------------------------------------

SparqlParser::BlankNodePropertyListPathContext::BlankNodePropertyListPathContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::BlankNodePropertyListPathContext::OPEN_SQUARE_BRACKET() {
  return getToken(SparqlParser::OPEN_SQUARE_BRACKET, 0);
}

SparqlParser::PropertyListPathNotEmptyContext* SparqlParser::BlankNodePropertyListPathContext::propertyListPathNotEmpty() {
  return getRuleContext<SparqlParser::PropertyListPathNotEmptyContext>(0);
}

tree::TerminalNode* SparqlParser::BlankNodePropertyListPathContext::CLOSE_SQUARE_BRACKET() {
  return getToken(SparqlParser::CLOSE_SQUARE_BRACKET, 0);
}


size_t SparqlParser::BlankNodePropertyListPathContext::getRuleIndex() const {
  return SparqlParser::RuleBlankNodePropertyListPath;
}


antlrcpp::Any SparqlParser::BlankNodePropertyListPathContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitBlankNodePropertyListPath(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::BlankNodePropertyListPathContext* SparqlParser::blankNodePropertyListPath() {
  BlankNodePropertyListPathContext *_localctx = _tracker.createInstance<BlankNodePropertyListPathContext>(_ctx, getState());
  enterRule(_localctx, 202, SparqlParser::RuleBlankNodePropertyListPath);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(992);
    match(SparqlParser::OPEN_SQUARE_BRACKET);
    setState(993);
    propertyListPathNotEmpty();
    setState(994);
    match(SparqlParser::CLOSE_SQUARE_BRACKET);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CollectionContext ------------------------------------------------------------------

SparqlParser::CollectionContext::CollectionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::CollectionContext::OPEN_BRACE() {
  return getToken(SparqlParser::OPEN_BRACE, 0);
}

tree::TerminalNode* SparqlParser::CollectionContext::CLOSE_BRACE() {
  return getToken(SparqlParser::CLOSE_BRACE, 0);
}

std::vector<SparqlParser::GraphNodeContext *> SparqlParser::CollectionContext::graphNode() {
  return getRuleContexts<SparqlParser::GraphNodeContext>();
}

SparqlParser::GraphNodeContext* SparqlParser::CollectionContext::graphNode(size_t i) {
  return getRuleContext<SparqlParser::GraphNodeContext>(i);
}


size_t SparqlParser::CollectionContext::getRuleIndex() const {
  return SparqlParser::RuleCollection;
}


antlrcpp::Any SparqlParser::CollectionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitCollection(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::CollectionContext* SparqlParser::collection() {
  CollectionContext *_localctx = _tracker.createInstance<CollectionContext>(_ctx, getState());
  enterRule(_localctx, 204, SparqlParser::RuleCollection);
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
    setState(996);
    match(SparqlParser::OPEN_BRACE);
    setState(998); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(997);
      graphNode();
      setState(1000); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while (_la == SparqlParser::TRUE

    || _la == SparqlParser::FALSE || ((((_la - 115) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 115)) & ((1ULL << (SparqlParser::IRIREF - 115))
      | (1ULL << (SparqlParser::PNAME_NS - 115))
      | (1ULL << (SparqlParser::PNAME_LN - 115))
      | (1ULL << (SparqlParser::BLANK_NODE_LABEL - 115))
      | (1ULL << (SparqlParser::VAR1 - 115))
      | (1ULL << (SparqlParser::VAR2 - 115))
      | (1ULL << (SparqlParser::INTEGER - 115))
      | (1ULL << (SparqlParser::DECIMAL - 115))
      | (1ULL << (SparqlParser::DOUBLE - 115))
      | (1ULL << (SparqlParser::INTEGER_POSITIVE - 115))
      | (1ULL << (SparqlParser::DECIMAL_POSITIVE - 115))
      | (1ULL << (SparqlParser::DOUBLE_POSITIVE - 115))
      | (1ULL << (SparqlParser::INTEGER_NEGATIVE - 115))
      | (1ULL << (SparqlParser::DECIMAL_NEGATIVE - 115))
      | (1ULL << (SparqlParser::DOUBLE_NEGATIVE - 115))
      | (1ULL << (SparqlParser::STRING_LITERAL1 - 115))
      | (1ULL << (SparqlParser::STRING_LITERAL2 - 115))
      | (1ULL << (SparqlParser::STRING_LITERAL_LONG1 - 115))
      | (1ULL << (SparqlParser::STRING_LITERAL_LONG2 - 115))
      | (1ULL << (SparqlParser::OPEN_BRACE - 115))
      | (1ULL << (SparqlParser::OPEN_SQUARE_BRACKET - 115)))) != 0));
    setState(1002);
    match(SparqlParser::CLOSE_BRACE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CollectionPathContext ------------------------------------------------------------------

SparqlParser::CollectionPathContext::CollectionPathContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::CollectionPathContext::OPEN_BRACE() {
  return getToken(SparqlParser::OPEN_BRACE, 0);
}

tree::TerminalNode* SparqlParser::CollectionPathContext::CLOSE_BRACE() {
  return getToken(SparqlParser::CLOSE_BRACE, 0);
}

std::vector<SparqlParser::GraphNodePathContext *> SparqlParser::CollectionPathContext::graphNodePath() {
  return getRuleContexts<SparqlParser::GraphNodePathContext>();
}

SparqlParser::GraphNodePathContext* SparqlParser::CollectionPathContext::graphNodePath(size_t i) {
  return getRuleContext<SparqlParser::GraphNodePathContext>(i);
}


size_t SparqlParser::CollectionPathContext::getRuleIndex() const {
  return SparqlParser::RuleCollectionPath;
}


antlrcpp::Any SparqlParser::CollectionPathContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitCollectionPath(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::CollectionPathContext* SparqlParser::collectionPath() {
  CollectionPathContext *_localctx = _tracker.createInstance<CollectionPathContext>(_ctx, getState());
  enterRule(_localctx, 206, SparqlParser::RuleCollectionPath);
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
    setState(1004);
    match(SparqlParser::OPEN_BRACE);
    setState(1006); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(1005);
      graphNodePath();
      setState(1008); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while (_la == SparqlParser::TRUE

    || _la == SparqlParser::FALSE || ((((_la - 115) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 115)) & ((1ULL << (SparqlParser::IRIREF - 115))
      | (1ULL << (SparqlParser::PNAME_NS - 115))
      | (1ULL << (SparqlParser::PNAME_LN - 115))
      | (1ULL << (SparqlParser::BLANK_NODE_LABEL - 115))
      | (1ULL << (SparqlParser::VAR1 - 115))
      | (1ULL << (SparqlParser::VAR2 - 115))
      | (1ULL << (SparqlParser::INTEGER - 115))
      | (1ULL << (SparqlParser::DECIMAL - 115))
      | (1ULL << (SparqlParser::DOUBLE - 115))
      | (1ULL << (SparqlParser::INTEGER_POSITIVE - 115))
      | (1ULL << (SparqlParser::DECIMAL_POSITIVE - 115))
      | (1ULL << (SparqlParser::DOUBLE_POSITIVE - 115))
      | (1ULL << (SparqlParser::INTEGER_NEGATIVE - 115))
      | (1ULL << (SparqlParser::DECIMAL_NEGATIVE - 115))
      | (1ULL << (SparqlParser::DOUBLE_NEGATIVE - 115))
      | (1ULL << (SparqlParser::STRING_LITERAL1 - 115))
      | (1ULL << (SparqlParser::STRING_LITERAL2 - 115))
      | (1ULL << (SparqlParser::STRING_LITERAL_LONG1 - 115))
      | (1ULL << (SparqlParser::STRING_LITERAL_LONG2 - 115))
      | (1ULL << (SparqlParser::OPEN_BRACE - 115))
      | (1ULL << (SparqlParser::OPEN_SQUARE_BRACKET - 115)))) != 0));
    setState(1010);
    match(SparqlParser::CLOSE_BRACE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- GraphNodeContext ------------------------------------------------------------------

SparqlParser::GraphNodeContext::GraphNodeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::VarOrTermContext* SparqlParser::GraphNodeContext::varOrTerm() {
  return getRuleContext<SparqlParser::VarOrTermContext>(0);
}

SparqlParser::TriplesNodeContext* SparqlParser::GraphNodeContext::triplesNode() {
  return getRuleContext<SparqlParser::TriplesNodeContext>(0);
}


size_t SparqlParser::GraphNodeContext::getRuleIndex() const {
  return SparqlParser::RuleGraphNode;
}


antlrcpp::Any SparqlParser::GraphNodeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitGraphNode(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::GraphNodeContext* SparqlParser::graphNode() {
  GraphNodeContext *_localctx = _tracker.createInstance<GraphNodeContext>(_ctx, getState());
  enterRule(_localctx, 208, SparqlParser::RuleGraphNode);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(1014);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 111, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(1012);
      varOrTerm();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(1013);
      triplesNode();
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

//----------------- GraphNodePathContext ------------------------------------------------------------------

SparqlParser::GraphNodePathContext::GraphNodePathContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::VarOrTermContext* SparqlParser::GraphNodePathContext::varOrTerm() {
  return getRuleContext<SparqlParser::VarOrTermContext>(0);
}

SparqlParser::TriplesNodePathContext* SparqlParser::GraphNodePathContext::triplesNodePath() {
  return getRuleContext<SparqlParser::TriplesNodePathContext>(0);
}


size_t SparqlParser::GraphNodePathContext::getRuleIndex() const {
  return SparqlParser::RuleGraphNodePath;
}


antlrcpp::Any SparqlParser::GraphNodePathContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitGraphNodePath(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::GraphNodePathContext* SparqlParser::graphNodePath() {
  GraphNodePathContext *_localctx = _tracker.createInstance<GraphNodePathContext>(_ctx, getState());
  enterRule(_localctx, 210, SparqlParser::RuleGraphNodePath);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(1018);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 112, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(1016);
      varOrTerm();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(1017);
      triplesNodePath();
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

//----------------- VarOrTermContext ------------------------------------------------------------------

SparqlParser::VarOrTermContext::VarOrTermContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::VarContext* SparqlParser::VarOrTermContext::var() {
  return getRuleContext<SparqlParser::VarContext>(0);
}

SparqlParser::GraphTermContext* SparqlParser::VarOrTermContext::graphTerm() {
  return getRuleContext<SparqlParser::GraphTermContext>(0);
}


size_t SparqlParser::VarOrTermContext::getRuleIndex() const {
  return SparqlParser::RuleVarOrTerm;
}


antlrcpp::Any SparqlParser::VarOrTermContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitVarOrTerm(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::VarOrTermContext* SparqlParser::varOrTerm() {
  VarOrTermContext *_localctx = _tracker.createInstance<VarOrTermContext>(_ctx, getState());
  enterRule(_localctx, 212, SparqlParser::RuleVarOrTerm);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(1022);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlParser::VAR1:
      case SparqlParser::VAR2: {
        enterOuterAlt(_localctx, 1);
        setState(1020);
        var();
        break;
      }

      case SparqlParser::TRUE:
      case SparqlParser::FALSE:
      case SparqlParser::IRIREF:
      case SparqlParser::PNAME_NS:
      case SparqlParser::PNAME_LN:
      case SparqlParser::BLANK_NODE_LABEL:
      case SparqlParser::INTEGER:
      case SparqlParser::DECIMAL:
      case SparqlParser::DOUBLE:
      case SparqlParser::INTEGER_POSITIVE:
      case SparqlParser::DECIMAL_POSITIVE:
      case SparqlParser::DOUBLE_POSITIVE:
      case SparqlParser::INTEGER_NEGATIVE:
      case SparqlParser::DECIMAL_NEGATIVE:
      case SparqlParser::DOUBLE_NEGATIVE:
      case SparqlParser::STRING_LITERAL1:
      case SparqlParser::STRING_LITERAL2:
      case SparqlParser::STRING_LITERAL_LONG1:
      case SparqlParser::STRING_LITERAL_LONG2:
      case SparqlParser::OPEN_BRACE:
      case SparqlParser::OPEN_SQUARE_BRACKET: {
        enterOuterAlt(_localctx, 2);
        setState(1021);
        graphTerm();
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

//----------------- VarOrIRIContext ------------------------------------------------------------------

SparqlParser::VarOrIRIContext::VarOrIRIContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::VarContext* SparqlParser::VarOrIRIContext::var() {
  return getRuleContext<SparqlParser::VarContext>(0);
}

SparqlParser::IriContext* SparqlParser::VarOrIRIContext::iri() {
  return getRuleContext<SparqlParser::IriContext>(0);
}


size_t SparqlParser::VarOrIRIContext::getRuleIndex() const {
  return SparqlParser::RuleVarOrIRI;
}


antlrcpp::Any SparqlParser::VarOrIRIContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitVarOrIRI(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::VarOrIRIContext* SparqlParser::varOrIRI() {
  VarOrIRIContext *_localctx = _tracker.createInstance<VarOrIRIContext>(_ctx, getState());
  enterRule(_localctx, 214, SparqlParser::RuleVarOrIRI);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(1026);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlParser::VAR1:
      case SparqlParser::VAR2: {
        enterOuterAlt(_localctx, 1);
        setState(1024);
        var();
        break;
      }

      case SparqlParser::IRIREF:
      case SparqlParser::PNAME_NS:
      case SparqlParser::PNAME_LN: {
        enterOuterAlt(_localctx, 2);
        setState(1025);
        iri();
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

//----------------- VarContext ------------------------------------------------------------------

SparqlParser::VarContext::VarContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::VarContext::VAR1() {
  return getToken(SparqlParser::VAR1, 0);
}

tree::TerminalNode* SparqlParser::VarContext::VAR2() {
  return getToken(SparqlParser::VAR2, 0);
}


size_t SparqlParser::VarContext::getRuleIndex() const {
  return SparqlParser::RuleVar;
}


antlrcpp::Any SparqlParser::VarContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitVar(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::VarContext* SparqlParser::var() {
  VarContext *_localctx = _tracker.createInstance<VarContext>(_ctx, getState());
  enterRule(_localctx, 216, SparqlParser::RuleVar);
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
    setState(1028);
    _la = _input->LA(1);
    if (!(_la == SparqlParser::VAR1

    || _la == SparqlParser::VAR2)) {
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

//----------------- GraphTermContext ------------------------------------------------------------------

SparqlParser::GraphTermContext::GraphTermContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::IriContext* SparqlParser::GraphTermContext::iri() {
  return getRuleContext<SparqlParser::IriContext>(0);
}

SparqlParser::RdfLiteralContext* SparqlParser::GraphTermContext::rdfLiteral() {
  return getRuleContext<SparqlParser::RdfLiteralContext>(0);
}

SparqlParser::NumericLiteralContext* SparqlParser::GraphTermContext::numericLiteral() {
  return getRuleContext<SparqlParser::NumericLiteralContext>(0);
}

SparqlParser::BooleanLiteralContext* SparqlParser::GraphTermContext::booleanLiteral() {
  return getRuleContext<SparqlParser::BooleanLiteralContext>(0);
}

SparqlParser::BlankNodeContext* SparqlParser::GraphTermContext::blankNode() {
  return getRuleContext<SparqlParser::BlankNodeContext>(0);
}

SparqlParser::NilContext* SparqlParser::GraphTermContext::nil() {
  return getRuleContext<SparqlParser::NilContext>(0);
}


size_t SparqlParser::GraphTermContext::getRuleIndex() const {
  return SparqlParser::RuleGraphTerm;
}


antlrcpp::Any SparqlParser::GraphTermContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitGraphTerm(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::GraphTermContext* SparqlParser::graphTerm() {
  GraphTermContext *_localctx = _tracker.createInstance<GraphTermContext>(_ctx, getState());
  enterRule(_localctx, 218, SparqlParser::RuleGraphTerm);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(1036);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlParser::IRIREF:
      case SparqlParser::PNAME_NS:
      case SparqlParser::PNAME_LN: {
        enterOuterAlt(_localctx, 1);
        setState(1030);
        iri();
        break;
      }

      case SparqlParser::STRING_LITERAL1:
      case SparqlParser::STRING_LITERAL2:
      case SparqlParser::STRING_LITERAL_LONG1:
      case SparqlParser::STRING_LITERAL_LONG2: {
        enterOuterAlt(_localctx, 2);
        setState(1031);
        rdfLiteral();
        break;
      }

      case SparqlParser::INTEGER:
      case SparqlParser::DECIMAL:
      case SparqlParser::DOUBLE:
      case SparqlParser::INTEGER_POSITIVE:
      case SparqlParser::DECIMAL_POSITIVE:
      case SparqlParser::DOUBLE_POSITIVE:
      case SparqlParser::INTEGER_NEGATIVE:
      case SparqlParser::DECIMAL_NEGATIVE:
      case SparqlParser::DOUBLE_NEGATIVE: {
        enterOuterAlt(_localctx, 3);
        setState(1032);
        numericLiteral();
        break;
      }

      case SparqlParser::TRUE:
      case SparqlParser::FALSE: {
        enterOuterAlt(_localctx, 4);
        setState(1033);
        booleanLiteral();
        break;
      }

      case SparqlParser::BLANK_NODE_LABEL:
      case SparqlParser::OPEN_SQUARE_BRACKET: {
        enterOuterAlt(_localctx, 5);
        setState(1034);
        blankNode();
        break;
      }

      case SparqlParser::OPEN_BRACE: {
        enterOuterAlt(_localctx, 6);
        setState(1035);
        nil();
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

//----------------- NilContext ------------------------------------------------------------------

SparqlParser::NilContext::NilContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::NilContext::OPEN_BRACE() {
  return getToken(SparqlParser::OPEN_BRACE, 0);
}

tree::TerminalNode* SparqlParser::NilContext::CLOSE_BRACE() {
  return getToken(SparqlParser::CLOSE_BRACE, 0);
}


size_t SparqlParser::NilContext::getRuleIndex() const {
  return SparqlParser::RuleNil;
}


antlrcpp::Any SparqlParser::NilContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitNil(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::NilContext* SparqlParser::nil() {
  NilContext *_localctx = _tracker.createInstance<NilContext>(_ctx, getState());
  enterRule(_localctx, 220, SparqlParser::RuleNil);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1038);
    match(SparqlParser::OPEN_BRACE);
    setState(1039);
    match(SparqlParser::CLOSE_BRACE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExpressionContext ------------------------------------------------------------------

SparqlParser::ExpressionContext::ExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t SparqlParser::ExpressionContext::getRuleIndex() const {
  return SparqlParser::RuleExpression;
}

void SparqlParser::ExpressionContext::copyFrom(ExpressionContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- UnarySignedLiteralExpressionContext ------------------------------------------------------------------

SparqlParser::ExpressionContext* SparqlParser::UnarySignedLiteralExpressionContext::expression() {
  return getRuleContext<SparqlParser::ExpressionContext>(0);
}

SparqlParser::UnaryLiteralExpressionContext* SparqlParser::UnarySignedLiteralExpressionContext::unaryLiteralExpression() {
  return getRuleContext<SparqlParser::UnaryLiteralExpressionContext>(0);
}

SparqlParser::UnarySignedLiteralExpressionContext::UnarySignedLiteralExpressionContext(ExpressionContext *ctx) { copyFrom(ctx); }


antlrcpp::Any SparqlParser::UnarySignedLiteralExpressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitUnarySignedLiteralExpression(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ConditionalOrExpressionContext ------------------------------------------------------------------

std::vector<SparqlParser::ExpressionContext *> SparqlParser::ConditionalOrExpressionContext::expression() {
  return getRuleContexts<SparqlParser::ExpressionContext>();
}

SparqlParser::ExpressionContext* SparqlParser::ConditionalOrExpressionContext::expression(size_t i) {
  return getRuleContext<SparqlParser::ExpressionContext>(i);
}

tree::TerminalNode* SparqlParser::ConditionalOrExpressionContext::OR() {
  return getToken(SparqlParser::OR, 0);
}

SparqlParser::ConditionalOrExpressionContext::ConditionalOrExpressionContext(ExpressionContext *ctx) { copyFrom(ctx); }


antlrcpp::Any SparqlParser::ConditionalOrExpressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitConditionalOrExpression(this);
  else
    return visitor->visitChildren(this);
}
//----------------- AdditiveExpressionContext ------------------------------------------------------------------

std::vector<SparqlParser::ExpressionContext *> SparqlParser::AdditiveExpressionContext::expression() {
  return getRuleContexts<SparqlParser::ExpressionContext>();
}

SparqlParser::ExpressionContext* SparqlParser::AdditiveExpressionContext::expression(size_t i) {
  return getRuleContext<SparqlParser::ExpressionContext>(i);
}

tree::TerminalNode* SparqlParser::AdditiveExpressionContext::PLUS_SIGN() {
  return getToken(SparqlParser::PLUS_SIGN, 0);
}

tree::TerminalNode* SparqlParser::AdditiveExpressionContext::MINUS_SIGN() {
  return getToken(SparqlParser::MINUS_SIGN, 0);
}

SparqlParser::AdditiveExpressionContext::AdditiveExpressionContext(ExpressionContext *ctx) { copyFrom(ctx); }


antlrcpp::Any SparqlParser::AdditiveExpressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitAdditiveExpression(this);
  else
    return visitor->visitChildren(this);
}
//----------------- UnaryAdditiveExpressionContext ------------------------------------------------------------------

SparqlParser::ExpressionContext* SparqlParser::UnaryAdditiveExpressionContext::expression() {
  return getRuleContext<SparqlParser::ExpressionContext>(0);
}

tree::TerminalNode* SparqlParser::UnaryAdditiveExpressionContext::PLUS_SIGN() {
  return getToken(SparqlParser::PLUS_SIGN, 0);
}

tree::TerminalNode* SparqlParser::UnaryAdditiveExpressionContext::MINUS_SIGN() {
  return getToken(SparqlParser::MINUS_SIGN, 0);
}

SparqlParser::UnaryAdditiveExpressionContext::UnaryAdditiveExpressionContext(ExpressionContext *ctx) { copyFrom(ctx); }


antlrcpp::Any SparqlParser::UnaryAdditiveExpressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitUnaryAdditiveExpression(this);
  else
    return visitor->visitChildren(this);
}
//----------------- RelationalExpressionContext ------------------------------------------------------------------

std::vector<SparqlParser::ExpressionContext *> SparqlParser::RelationalExpressionContext::expression() {
  return getRuleContexts<SparqlParser::ExpressionContext>();
}

SparqlParser::ExpressionContext* SparqlParser::RelationalExpressionContext::expression(size_t i) {
  return getRuleContext<SparqlParser::ExpressionContext>(i);
}

tree::TerminalNode* SparqlParser::RelationalExpressionContext::EQUAL() {
  return getToken(SparqlParser::EQUAL, 0);
}

tree::TerminalNode* SparqlParser::RelationalExpressionContext::NOT_EQUAL() {
  return getToken(SparqlParser::NOT_EQUAL, 0);
}

tree::TerminalNode* SparqlParser::RelationalExpressionContext::LESS() {
  return getToken(SparqlParser::LESS, 0);
}

tree::TerminalNode* SparqlParser::RelationalExpressionContext::GREATER() {
  return getToken(SparqlParser::GREATER, 0);
}

tree::TerminalNode* SparqlParser::RelationalExpressionContext::LESS_EQUAL() {
  return getToken(SparqlParser::LESS_EQUAL, 0);
}

tree::TerminalNode* SparqlParser::RelationalExpressionContext::GREATER_EQUAL() {
  return getToken(SparqlParser::GREATER_EQUAL, 0);
}

SparqlParser::RelationalExpressionContext::RelationalExpressionContext(ExpressionContext *ctx) { copyFrom(ctx); }


antlrcpp::Any SparqlParser::RelationalExpressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitRelationalExpression(this);
  else
    return visitor->visitChildren(this);
}
//----------------- RelationalSetExpressionContext ------------------------------------------------------------------

SparqlParser::ExpressionContext* SparqlParser::RelationalSetExpressionContext::expression() {
  return getRuleContext<SparqlParser::ExpressionContext>(0);
}

tree::TerminalNode* SparqlParser::RelationalSetExpressionContext::IN() {
  return getToken(SparqlParser::IN, 0);
}

tree::TerminalNode* SparqlParser::RelationalSetExpressionContext::OPEN_BRACE() {
  return getToken(SparqlParser::OPEN_BRACE, 0);
}

tree::TerminalNode* SparqlParser::RelationalSetExpressionContext::CLOSE_BRACE() {
  return getToken(SparqlParser::CLOSE_BRACE, 0);
}

tree::TerminalNode* SparqlParser::RelationalSetExpressionContext::NOT() {
  return getToken(SparqlParser::NOT, 0);
}

SparqlParser::ExpressionListContext* SparqlParser::RelationalSetExpressionContext::expressionList() {
  return getRuleContext<SparqlParser::ExpressionListContext>(0);
}

SparqlParser::RelationalSetExpressionContext::RelationalSetExpressionContext(ExpressionContext *ctx) { copyFrom(ctx); }


antlrcpp::Any SparqlParser::RelationalSetExpressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitRelationalSetExpression(this);
  else
    return visitor->visitChildren(this);
}
//----------------- UnaryMultiplicativeExpressionContext ------------------------------------------------------------------

SparqlParser::ExpressionContext* SparqlParser::UnaryMultiplicativeExpressionContext::expression() {
  return getRuleContext<SparqlParser::ExpressionContext>(0);
}

tree::TerminalNode* SparqlParser::UnaryMultiplicativeExpressionContext::ASTERISK() {
  return getToken(SparqlParser::ASTERISK, 0);
}

tree::TerminalNode* SparqlParser::UnaryMultiplicativeExpressionContext::DIVIDE() {
  return getToken(SparqlParser::DIVIDE, 0);
}

SparqlParser::UnaryMultiplicativeExpressionContext::UnaryMultiplicativeExpressionContext(ExpressionContext *ctx) { copyFrom(ctx); }


antlrcpp::Any SparqlParser::UnaryMultiplicativeExpressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitUnaryMultiplicativeExpression(this);
  else
    return visitor->visitChildren(this);
}
//----------------- BaseExpressionContext ------------------------------------------------------------------

SparqlParser::PrimaryExpressionContext* SparqlParser::BaseExpressionContext::primaryExpression() {
  return getRuleContext<SparqlParser::PrimaryExpressionContext>(0);
}

SparqlParser::BaseExpressionContext::BaseExpressionContext(ExpressionContext *ctx) { copyFrom(ctx); }


antlrcpp::Any SparqlParser::BaseExpressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitBaseExpression(this);
  else
    return visitor->visitChildren(this);
}
//----------------- MultiplicativeExpressionContext ------------------------------------------------------------------

std::vector<SparqlParser::ExpressionContext *> SparqlParser::MultiplicativeExpressionContext::expression() {
  return getRuleContexts<SparqlParser::ExpressionContext>();
}

SparqlParser::ExpressionContext* SparqlParser::MultiplicativeExpressionContext::expression(size_t i) {
  return getRuleContext<SparqlParser::ExpressionContext>(i);
}

tree::TerminalNode* SparqlParser::MultiplicativeExpressionContext::ASTERISK() {
  return getToken(SparqlParser::ASTERISK, 0);
}

tree::TerminalNode* SparqlParser::MultiplicativeExpressionContext::DIVIDE() {
  return getToken(SparqlParser::DIVIDE, 0);
}

SparqlParser::MultiplicativeExpressionContext::MultiplicativeExpressionContext(ExpressionContext *ctx) { copyFrom(ctx); }


antlrcpp::Any SparqlParser::MultiplicativeExpressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitMultiplicativeExpression(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ConditionalAndExpressionContext ------------------------------------------------------------------

std::vector<SparqlParser::ExpressionContext *> SparqlParser::ConditionalAndExpressionContext::expression() {
  return getRuleContexts<SparqlParser::ExpressionContext>();
}

SparqlParser::ExpressionContext* SparqlParser::ConditionalAndExpressionContext::expression(size_t i) {
  return getRuleContext<SparqlParser::ExpressionContext>(i);
}

tree::TerminalNode* SparqlParser::ConditionalAndExpressionContext::AND() {
  return getToken(SparqlParser::AND, 0);
}

SparqlParser::ConditionalAndExpressionContext::ConditionalAndExpressionContext(ExpressionContext *ctx) { copyFrom(ctx); }


antlrcpp::Any SparqlParser::ConditionalAndExpressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitConditionalAndExpression(this);
  else
    return visitor->visitChildren(this);
}
//----------------- UnaryNegationExpressionContext ------------------------------------------------------------------

tree::TerminalNode* SparqlParser::UnaryNegationExpressionContext::NEGATION() {
  return getToken(SparqlParser::NEGATION, 0);
}

SparqlParser::ExpressionContext* SparqlParser::UnaryNegationExpressionContext::expression() {
  return getRuleContext<SparqlParser::ExpressionContext>(0);
}

SparqlParser::UnaryNegationExpressionContext::UnaryNegationExpressionContext(ExpressionContext *ctx) { copyFrom(ctx); }


antlrcpp::Any SparqlParser::UnaryNegationExpressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitUnaryNegationExpression(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::ExpressionContext* SparqlParser::expression() {
   return expression(0);
}

SparqlParser::ExpressionContext* SparqlParser::expression(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  SparqlParser::ExpressionContext *_localctx = _tracker.createInstance<ExpressionContext>(_ctx, parentState);
  SparqlParser::ExpressionContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 222;
  enterRecursionRule(_localctx, 222, SparqlParser::RuleExpression, precedence);

    size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(1049);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlParser::STR:
      case SparqlParser::LANG:
      case SparqlParser::LANGMATCHES:
      case SparqlParser::DATATYPE:
      case SparqlParser::BOUND:
      case SparqlParser::SAMETERM:
      case SparqlParser::ISIRI:
      case SparqlParser::ISURI:
      case SparqlParser::ISBLANK:
      case SparqlParser::ISLITERAL:
      case SparqlParser::REGEX:
      case SparqlParser::SUBSTR:
      case SparqlParser::TRUE:
      case SparqlParser::FALSE:
      case SparqlParser::IRI:
      case SparqlParser::URI:
      case SparqlParser::BNODE:
      case SparqlParser::RAND:
      case SparqlParser::ABS:
      case SparqlParser::CEIL:
      case SparqlParser::FLOOR:
      case SparqlParser::ROUND:
      case SparqlParser::CONCAT:
      case SparqlParser::STRLEN:
      case SparqlParser::UCASE:
      case SparqlParser::LCASE:
      case SparqlParser::ENCODE_FOR_URI:
      case SparqlParser::CONTAINS:
      case SparqlParser::STRSTARTS:
      case SparqlParser::STRENDS:
      case SparqlParser::STRBEFORE:
      case SparqlParser::STRAFTER:
      case SparqlParser::REPLACE:
      case SparqlParser::YEAR:
      case SparqlParser::MONTH:
      case SparqlParser::DAY:
      case SparqlParser::HOURS:
      case SparqlParser::MINUTES:
      case SparqlParser::SECONDS:
      case SparqlParser::TIMEZONE:
      case SparqlParser::TZ:
      case SparqlParser::NOW:
      case SparqlParser::UUID:
      case SparqlParser::STRUUID:
      case SparqlParser::MD5:
      case SparqlParser::SHA1:
      case SparqlParser::SHA256:
      case SparqlParser::SHA384:
      case SparqlParser::SHA512:
      case SparqlParser::COALESCE:
      case SparqlParser::IF:
      case SparqlParser::STRLANG:
      case SparqlParser::STRDT:
      case SparqlParser::ISNUMERIC:
      case SparqlParser::COUNT:
      case SparqlParser::SUM:
      case SparqlParser::MIN:
      case SparqlParser::MAX:
      case SparqlParser::AVG:
      case SparqlParser::SAMPLE:
      case SparqlParser::GROUP_CONCAT:
      case SparqlParser::NOT:
      case SparqlParser::EXISTS:
      case SparqlParser::IRIREF:
      case SparqlParser::PNAME_NS:
      case SparqlParser::PNAME_LN:
      case SparqlParser::VAR1:
      case SparqlParser::VAR2:
      case SparqlParser::INTEGER:
      case SparqlParser::DECIMAL:
      case SparqlParser::DOUBLE:
      case SparqlParser::INTEGER_POSITIVE:
      case SparqlParser::DECIMAL_POSITIVE:
      case SparqlParser::DOUBLE_POSITIVE:
      case SparqlParser::INTEGER_NEGATIVE:
      case SparqlParser::DECIMAL_NEGATIVE:
      case SparqlParser::DOUBLE_NEGATIVE:
      case SparqlParser::STRING_LITERAL1:
      case SparqlParser::STRING_LITERAL2:
      case SparqlParser::STRING_LITERAL_LONG1:
      case SparqlParser::STRING_LITERAL_LONG2:
      case SparqlParser::OPEN_BRACE: {
        _localctx = _tracker.createInstance<BaseExpressionContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;

        setState(1042);
        primaryExpression();
        break;
      }

      case SparqlParser::ASTERISK:
      case SparqlParser::DIVIDE: {
        _localctx = _tracker.createInstance<UnaryMultiplicativeExpressionContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(1043);
        antlrcpp::downCast<UnaryMultiplicativeExpressionContext *>(_localctx)->op = _input->LT(1);
        _la = _input->LA(1);
        if (!(_la == SparqlParser::ASTERISK

        || _la == SparqlParser::DIVIDE)) {
          antlrcpp::downCast<UnaryMultiplicativeExpressionContext *>(_localctx)->op = _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        setState(1044);
        expression(10);
        break;
      }

      case SparqlParser::PLUS_SIGN:
      case SparqlParser::MINUS_SIGN: {
        _localctx = _tracker.createInstance<UnaryAdditiveExpressionContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(1045);
        antlrcpp::downCast<UnaryAdditiveExpressionContext *>(_localctx)->op = _input->LT(1);
        _la = _input->LA(1);
        if (!(_la == SparqlParser::PLUS_SIGN

        || _la == SparqlParser::MINUS_SIGN)) {
          antlrcpp::downCast<UnaryAdditiveExpressionContext *>(_localctx)->op = _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        setState(1046);
        expression(9);
        break;
      }

      case SparqlParser::NEGATION: {
        _localctx = _tracker.createInstance<UnaryNegationExpressionContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(1047);
        match(SparqlParser::NEGATION);
        setState(1048);
        expression(8);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    _ctx->stop = _input->LT(-1);
    setState(1080);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 120, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(1078);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 119, _ctx)) {
        case 1: {
          auto newContext = _tracker.createInstance<MultiplicativeExpressionContext>(_tracker.createInstance<ExpressionContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpression);
          setState(1051);

          if (!(precpred(_ctx, 7))) throw FailedPredicateException(this, "precpred(_ctx, 7)");
          setState(1052);
          antlrcpp::downCast<MultiplicativeExpressionContext *>(_localctx)->op = _input->LT(1);
          _la = _input->LA(1);
          if (!(_la == SparqlParser::ASTERISK

          || _la == SparqlParser::DIVIDE)) {
            antlrcpp::downCast<MultiplicativeExpressionContext *>(_localctx)->op = _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(1053);
          expression(8);
          break;
        }

        case 2: {
          auto newContext = _tracker.createInstance<AdditiveExpressionContext>(_tracker.createInstance<ExpressionContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpression);
          setState(1054);

          if (!(precpred(_ctx, 6))) throw FailedPredicateException(this, "precpred(_ctx, 6)");
          setState(1055);
          antlrcpp::downCast<AdditiveExpressionContext *>(_localctx)->op = _input->LT(1);
          _la = _input->LA(1);
          if (!(_la == SparqlParser::PLUS_SIGN

          || _la == SparqlParser::MINUS_SIGN)) {
            antlrcpp::downCast<AdditiveExpressionContext *>(_localctx)->op = _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(1056);
          expression(7);
          break;
        }

        case 3: {
          auto newContext = _tracker.createInstance<RelationalExpressionContext>(_tracker.createInstance<ExpressionContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpression);
          setState(1057);

          if (!(precpred(_ctx, 3))) throw FailedPredicateException(this, "precpred(_ctx, 3)");
          setState(1058);
          antlrcpp::downCast<RelationalExpressionContext *>(_localctx)->op = _input->LT(1);
          _la = _input->LA(1);
          if (!(((((_la - 137) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 137)) & ((1ULL << (SparqlParser::LESS_EQUAL - 137))
            | (1ULL << (SparqlParser::GREATER_EQUAL - 137))
            | (1ULL << (SparqlParser::NOT_EQUAL - 137))
            | (1ULL << (SparqlParser::EQUAL - 137))
            | (1ULL << (SparqlParser::LESS - 137))
            | (1ULL << (SparqlParser::GREATER - 137)))) != 0))) {
            antlrcpp::downCast<RelationalExpressionContext *>(_localctx)->op = _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(1059);
          expression(4);
          break;
        }

        case 4: {
          auto newContext = _tracker.createInstance<UnarySignedLiteralExpressionContext>(_tracker.createInstance<ExpressionContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpression);
          setState(1060);

          if (!(precpred(_ctx, 5))) throw FailedPredicateException(this, "precpred(_ctx, 5)");
          setState(1061);
          unaryLiteralExpression();
          break;
        }

        case 5: {
          auto newContext = _tracker.createInstance<RelationalSetExpressionContext>(_tracker.createInstance<ExpressionContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpression);
          setState(1062);

          if (!(precpred(_ctx, 4))) throw FailedPredicateException(this, "precpred(_ctx, 4)");
          setState(1064);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == SparqlParser::NOT) {
            setState(1063);
            match(SparqlParser::NOT);
          }
          setState(1066);
          match(SparqlParser::IN);
          setState(1067);
          match(SparqlParser::OPEN_BRACE);
          setState(1069);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if ((((_la & ~ 0x3fULL) == 0) &&
            ((1ULL << _la) & ((1ULL << SparqlParser::STR)
            | (1ULL << SparqlParser::LANG)
            | (1ULL << SparqlParser::LANGMATCHES)
            | (1ULL << SparqlParser::DATATYPE)
            | (1ULL << SparqlParser::BOUND)
            | (1ULL << SparqlParser::SAMETERM)
            | (1ULL << SparqlParser::ISIRI)
            | (1ULL << SparqlParser::ISURI)
            | (1ULL << SparqlParser::ISBLANK)
            | (1ULL << SparqlParser::ISLITERAL)
            | (1ULL << SparqlParser::REGEX)
            | (1ULL << SparqlParser::SUBSTR)
            | (1ULL << SparqlParser::TRUE)
            | (1ULL << SparqlParser::FALSE))) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 64)) & ((1ULL << (SparqlParser::IRI - 64))
            | (1ULL << (SparqlParser::URI - 64))
            | (1ULL << (SparqlParser::BNODE - 64))
            | (1ULL << (SparqlParser::RAND - 64))
            | (1ULL << (SparqlParser::ABS - 64))
            | (1ULL << (SparqlParser::CEIL - 64))
            | (1ULL << (SparqlParser::FLOOR - 64))
            | (1ULL << (SparqlParser::ROUND - 64))
            | (1ULL << (SparqlParser::CONCAT - 64))
            | (1ULL << (SparqlParser::STRLEN - 64))
            | (1ULL << (SparqlParser::UCASE - 64))
            | (1ULL << (SparqlParser::LCASE - 64))
            | (1ULL << (SparqlParser::ENCODE_FOR_URI - 64))
            | (1ULL << (SparqlParser::CONTAINS - 64))
            | (1ULL << (SparqlParser::STRSTARTS - 64))
            | (1ULL << (SparqlParser::STRENDS - 64))
            | (1ULL << (SparqlParser::STRBEFORE - 64))
            | (1ULL << (SparqlParser::STRAFTER - 64))
            | (1ULL << (SparqlParser::REPLACE - 64))
            | (1ULL << (SparqlParser::YEAR - 64))
            | (1ULL << (SparqlParser::MONTH - 64))
            | (1ULL << (SparqlParser::DAY - 64))
            | (1ULL << (SparqlParser::HOURS - 64))
            | (1ULL << (SparqlParser::MINUTES - 64))
            | (1ULL << (SparqlParser::SECONDS - 64))
            | (1ULL << (SparqlParser::TIMEZONE - 64))
            | (1ULL << (SparqlParser::TZ - 64))
            | (1ULL << (SparqlParser::NOW - 64))
            | (1ULL << (SparqlParser::UUID - 64))
            | (1ULL << (SparqlParser::STRUUID - 64))
            | (1ULL << (SparqlParser::MD5 - 64))
            | (1ULL << (SparqlParser::SHA1 - 64))
            | (1ULL << (SparqlParser::SHA256 - 64))
            | (1ULL << (SparqlParser::SHA384 - 64))
            | (1ULL << (SparqlParser::SHA512 - 64))
            | (1ULL << (SparqlParser::COALESCE - 64))
            | (1ULL << (SparqlParser::IF - 64))
            | (1ULL << (SparqlParser::STRLANG - 64))
            | (1ULL << (SparqlParser::STRDT - 64))
            | (1ULL << (SparqlParser::ISNUMERIC - 64))
            | (1ULL << (SparqlParser::COUNT - 64))
            | (1ULL << (SparqlParser::SUM - 64))
            | (1ULL << (SparqlParser::MIN - 64))
            | (1ULL << (SparqlParser::MAX - 64))
            | (1ULL << (SparqlParser::AVG - 64))
            | (1ULL << (SparqlParser::SAMPLE - 64))
            | (1ULL << (SparqlParser::GROUP_CONCAT - 64))
            | (1ULL << (SparqlParser::NOT - 64))
            | (1ULL << (SparqlParser::EXISTS - 64))
            | (1ULL << (SparqlParser::IRIREF - 64))
            | (1ULL << (SparqlParser::PNAME_NS - 64))
            | (1ULL << (SparqlParser::PNAME_LN - 64))
            | (1ULL << (SparqlParser::VAR1 - 64))
            | (1ULL << (SparqlParser::VAR2 - 64))
            | (1ULL << (SparqlParser::INTEGER - 64))
            | (1ULL << (SparqlParser::DECIMAL - 64))
            | (1ULL << (SparqlParser::DOUBLE - 64))
            | (1ULL << (SparqlParser::INTEGER_POSITIVE - 64))
            | (1ULL << (SparqlParser::DECIMAL_POSITIVE - 64))
            | (1ULL << (SparqlParser::DOUBLE_POSITIVE - 64)))) != 0) || ((((_la - 128) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 128)) & ((1ULL << (SparqlParser::INTEGER_NEGATIVE - 128))
            | (1ULL << (SparqlParser::DECIMAL_NEGATIVE - 128))
            | (1ULL << (SparqlParser::DOUBLE_NEGATIVE - 128))
            | (1ULL << (SparqlParser::STRING_LITERAL1 - 128))
            | (1ULL << (SparqlParser::STRING_LITERAL2 - 128))
            | (1ULL << (SparqlParser::STRING_LITERAL_LONG1 - 128))
            | (1ULL << (SparqlParser::STRING_LITERAL_LONG2 - 128))
            | (1ULL << (SparqlParser::OPEN_BRACE - 128))
            | (1ULL << (SparqlParser::PLUS_SIGN - 128))
            | (1ULL << (SparqlParser::MINUS_SIGN - 128))
            | (1ULL << (SparqlParser::ASTERISK - 128))
            | (1ULL << (SparqlParser::NEGATION - 128))
            | (1ULL << (SparqlParser::DIVIDE - 128)))) != 0)) {
            setState(1068);
            expressionList();
          }
          setState(1071);
          match(SparqlParser::CLOSE_BRACE);
          break;
        }

        case 6: {
          auto newContext = _tracker.createInstance<ConditionalAndExpressionContext>(_tracker.createInstance<ExpressionContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpression);
          setState(1072);

          if (!(precpred(_ctx, 2))) throw FailedPredicateException(this, "precpred(_ctx, 2)");

          setState(1073);
          match(SparqlParser::AND);
          setState(1074);
          expression(0);
          break;
        }

        case 7: {
          auto newContext = _tracker.createInstance<ConditionalOrExpressionContext>(_tracker.createInstance<ExpressionContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpression);
          setState(1075);

          if (!(precpred(_ctx, 1))) throw FailedPredicateException(this, "precpred(_ctx, 1)");

          setState(1076);
          match(SparqlParser::OR);
          setState(1077);
          expression(0);
          break;
        }

        default:
          break;
        } 
      }
      setState(1082);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 120, _ctx);
    }
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }
  return _localctx;
}

//----------------- UnaryLiteralExpressionContext ------------------------------------------------------------------

SparqlParser::UnaryLiteralExpressionContext::UnaryLiteralExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::NumericLiteralPositiveContext* SparqlParser::UnaryLiteralExpressionContext::numericLiteralPositive() {
  return getRuleContext<SparqlParser::NumericLiteralPositiveContext>(0);
}

SparqlParser::NumericLiteralNegativeContext* SparqlParser::UnaryLiteralExpressionContext::numericLiteralNegative() {
  return getRuleContext<SparqlParser::NumericLiteralNegativeContext>(0);
}

SparqlParser::UnaryExpressionContext* SparqlParser::UnaryLiteralExpressionContext::unaryExpression() {
  return getRuleContext<SparqlParser::UnaryExpressionContext>(0);
}

tree::TerminalNode* SparqlParser::UnaryLiteralExpressionContext::ASTERISK() {
  return getToken(SparqlParser::ASTERISK, 0);
}

tree::TerminalNode* SparqlParser::UnaryLiteralExpressionContext::DIVIDE() {
  return getToken(SparqlParser::DIVIDE, 0);
}


size_t SparqlParser::UnaryLiteralExpressionContext::getRuleIndex() const {
  return SparqlParser::RuleUnaryLiteralExpression;
}


antlrcpp::Any SparqlParser::UnaryLiteralExpressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitUnaryLiteralExpression(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::UnaryLiteralExpressionContext* SparqlParser::unaryLiteralExpression() {
  UnaryLiteralExpressionContext *_localctx = _tracker.createInstance<UnaryLiteralExpressionContext>(_ctx, getState());
  enterRule(_localctx, 224, SparqlParser::RuleUnaryLiteralExpression);
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
    setState(1085);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlParser::INTEGER_POSITIVE:
      case SparqlParser::DECIMAL_POSITIVE:
      case SparqlParser::DOUBLE_POSITIVE: {
        setState(1083);
        numericLiteralPositive();
        break;
      }

      case SparqlParser::INTEGER_NEGATIVE:
      case SparqlParser::DECIMAL_NEGATIVE:
      case SparqlParser::DOUBLE_NEGATIVE: {
        setState(1084);
        numericLiteralNegative();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    setState(1089);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 122, _ctx)) {
    case 1: {
      setState(1087);
      antlrcpp::downCast<UnaryLiteralExpressionContext *>(_localctx)->op = _input->LT(1);
      _la = _input->LA(1);
      if (!(_la == SparqlParser::ASTERISK

      || _la == SparqlParser::DIVIDE)) {
        antlrcpp::downCast<UnaryLiteralExpressionContext *>(_localctx)->op = _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(1088);
      unaryExpression();
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

//----------------- UnaryExpressionContext ------------------------------------------------------------------

SparqlParser::UnaryExpressionContext::UnaryExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::PrimaryExpressionContext* SparqlParser::UnaryExpressionContext::primaryExpression() {
  return getRuleContext<SparqlParser::PrimaryExpressionContext>(0);
}

tree::TerminalNode* SparqlParser::UnaryExpressionContext::NEGATION() {
  return getToken(SparqlParser::NEGATION, 0);
}

tree::TerminalNode* SparqlParser::UnaryExpressionContext::PLUS_SIGN() {
  return getToken(SparqlParser::PLUS_SIGN, 0);
}

tree::TerminalNode* SparqlParser::UnaryExpressionContext::MINUS_SIGN() {
  return getToken(SparqlParser::MINUS_SIGN, 0);
}


size_t SparqlParser::UnaryExpressionContext::getRuleIndex() const {
  return SparqlParser::RuleUnaryExpression;
}


antlrcpp::Any SparqlParser::UnaryExpressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitUnaryExpression(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::UnaryExpressionContext* SparqlParser::unaryExpression() {
  UnaryExpressionContext *_localctx = _tracker.createInstance<UnaryExpressionContext>(_ctx, getState());
  enterRule(_localctx, 226, SparqlParser::RuleUnaryExpression);
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
    setState(1092);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (((((_la - 151) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 151)) & ((1ULL << (SparqlParser::PLUS_SIGN - 151))
      | (1ULL << (SparqlParser::MINUS_SIGN - 151))
      | (1ULL << (SparqlParser::NEGATION - 151)))) != 0)) {
      setState(1091);
      antlrcpp::downCast<UnaryExpressionContext *>(_localctx)->op = _input->LT(1);
      _la = _input->LA(1);
      if (!(((((_la - 151) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 151)) & ((1ULL << (SparqlParser::PLUS_SIGN - 151))
        | (1ULL << (SparqlParser::MINUS_SIGN - 151))
        | (1ULL << (SparqlParser::NEGATION - 151)))) != 0))) {
        antlrcpp::downCast<UnaryExpressionContext *>(_localctx)->op = _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
    }
    setState(1094);
    primaryExpression();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PrimaryExpressionContext ------------------------------------------------------------------

SparqlParser::PrimaryExpressionContext::PrimaryExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::PrimaryExpressionContext::OPEN_BRACE() {
  return getToken(SparqlParser::OPEN_BRACE, 0);
}

SparqlParser::ExpressionContext* SparqlParser::PrimaryExpressionContext::expression() {
  return getRuleContext<SparqlParser::ExpressionContext>(0);
}

tree::TerminalNode* SparqlParser::PrimaryExpressionContext::CLOSE_BRACE() {
  return getToken(SparqlParser::CLOSE_BRACE, 0);
}

SparqlParser::BuiltInCallContext* SparqlParser::PrimaryExpressionContext::builtInCall() {
  return getRuleContext<SparqlParser::BuiltInCallContext>(0);
}

SparqlParser::IriRefOrFunctionContext* SparqlParser::PrimaryExpressionContext::iriRefOrFunction() {
  return getRuleContext<SparqlParser::IriRefOrFunctionContext>(0);
}

SparqlParser::RdfLiteralContext* SparqlParser::PrimaryExpressionContext::rdfLiteral() {
  return getRuleContext<SparqlParser::RdfLiteralContext>(0);
}

SparqlParser::NumericLiteralContext* SparqlParser::PrimaryExpressionContext::numericLiteral() {
  return getRuleContext<SparqlParser::NumericLiteralContext>(0);
}

SparqlParser::BooleanLiteralContext* SparqlParser::PrimaryExpressionContext::booleanLiteral() {
  return getRuleContext<SparqlParser::BooleanLiteralContext>(0);
}

SparqlParser::VarContext* SparqlParser::PrimaryExpressionContext::var() {
  return getRuleContext<SparqlParser::VarContext>(0);
}


size_t SparqlParser::PrimaryExpressionContext::getRuleIndex() const {
  return SparqlParser::RulePrimaryExpression;
}


antlrcpp::Any SparqlParser::PrimaryExpressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitPrimaryExpression(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::PrimaryExpressionContext* SparqlParser::primaryExpression() {
  PrimaryExpressionContext *_localctx = _tracker.createInstance<PrimaryExpressionContext>(_ctx, getState());
  enterRule(_localctx, 228, SparqlParser::RulePrimaryExpression);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(1106);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlParser::OPEN_BRACE: {
        enterOuterAlt(_localctx, 1);
        setState(1096);
        match(SparqlParser::OPEN_BRACE);
        setState(1097);
        expression(0);
        setState(1098);
        match(SparqlParser::CLOSE_BRACE);
        break;
      }

      case SparqlParser::STR:
      case SparqlParser::LANG:
      case SparqlParser::LANGMATCHES:
      case SparqlParser::DATATYPE:
      case SparqlParser::BOUND:
      case SparqlParser::SAMETERM:
      case SparqlParser::ISIRI:
      case SparqlParser::ISURI:
      case SparqlParser::ISBLANK:
      case SparqlParser::ISLITERAL:
      case SparqlParser::REGEX:
      case SparqlParser::SUBSTR:
      case SparqlParser::IRI:
      case SparqlParser::URI:
      case SparqlParser::BNODE:
      case SparqlParser::RAND:
      case SparqlParser::ABS:
      case SparqlParser::CEIL:
      case SparqlParser::FLOOR:
      case SparqlParser::ROUND:
      case SparqlParser::CONCAT:
      case SparqlParser::STRLEN:
      case SparqlParser::UCASE:
      case SparqlParser::LCASE:
      case SparqlParser::ENCODE_FOR_URI:
      case SparqlParser::CONTAINS:
      case SparqlParser::STRSTARTS:
      case SparqlParser::STRENDS:
      case SparqlParser::STRBEFORE:
      case SparqlParser::STRAFTER:
      case SparqlParser::REPLACE:
      case SparqlParser::YEAR:
      case SparqlParser::MONTH:
      case SparqlParser::DAY:
      case SparqlParser::HOURS:
      case SparqlParser::MINUTES:
      case SparqlParser::SECONDS:
      case SparqlParser::TIMEZONE:
      case SparqlParser::TZ:
      case SparqlParser::NOW:
      case SparqlParser::UUID:
      case SparqlParser::STRUUID:
      case SparqlParser::MD5:
      case SparqlParser::SHA1:
      case SparqlParser::SHA256:
      case SparqlParser::SHA384:
      case SparqlParser::SHA512:
      case SparqlParser::COALESCE:
      case SparqlParser::IF:
      case SparqlParser::STRLANG:
      case SparqlParser::STRDT:
      case SparqlParser::ISNUMERIC:
      case SparqlParser::COUNT:
      case SparqlParser::SUM:
      case SparqlParser::MIN:
      case SparqlParser::MAX:
      case SparqlParser::AVG:
      case SparqlParser::SAMPLE:
      case SparqlParser::GROUP_CONCAT:
      case SparqlParser::NOT:
      case SparqlParser::EXISTS: {
        enterOuterAlt(_localctx, 2);
        setState(1100);
        builtInCall();
        break;
      }

      case SparqlParser::IRIREF:
      case SparqlParser::PNAME_NS:
      case SparqlParser::PNAME_LN: {
        enterOuterAlt(_localctx, 3);
        setState(1101);
        iriRefOrFunction();
        break;
      }

      case SparqlParser::STRING_LITERAL1:
      case SparqlParser::STRING_LITERAL2:
      case SparqlParser::STRING_LITERAL_LONG1:
      case SparqlParser::STRING_LITERAL_LONG2: {
        enterOuterAlt(_localctx, 4);
        setState(1102);
        rdfLiteral();
        break;
      }

      case SparqlParser::INTEGER:
      case SparqlParser::DECIMAL:
      case SparqlParser::DOUBLE:
      case SparqlParser::INTEGER_POSITIVE:
      case SparqlParser::DECIMAL_POSITIVE:
      case SparqlParser::DOUBLE_POSITIVE:
      case SparqlParser::INTEGER_NEGATIVE:
      case SparqlParser::DECIMAL_NEGATIVE:
      case SparqlParser::DOUBLE_NEGATIVE: {
        enterOuterAlt(_localctx, 5);
        setState(1103);
        numericLiteral();
        break;
      }

      case SparqlParser::TRUE:
      case SparqlParser::FALSE: {
        enterOuterAlt(_localctx, 6);
        setState(1104);
        booleanLiteral();
        break;
      }

      case SparqlParser::VAR1:
      case SparqlParser::VAR2: {
        enterOuterAlt(_localctx, 7);
        setState(1105);
        var();
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

//----------------- BuiltInCallContext ------------------------------------------------------------------

SparqlParser::BuiltInCallContext::BuiltInCallContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::AggregateContext* SparqlParser::BuiltInCallContext::aggregate() {
  return getRuleContext<SparqlParser::AggregateContext>(0);
}

tree::TerminalNode* SparqlParser::BuiltInCallContext::STR() {
  return getToken(SparqlParser::STR, 0);
}

tree::TerminalNode* SparqlParser::BuiltInCallContext::OPEN_BRACE() {
  return getToken(SparqlParser::OPEN_BRACE, 0);
}

std::vector<SparqlParser::ExpressionContext *> SparqlParser::BuiltInCallContext::expression() {
  return getRuleContexts<SparqlParser::ExpressionContext>();
}

SparqlParser::ExpressionContext* SparqlParser::BuiltInCallContext::expression(size_t i) {
  return getRuleContext<SparqlParser::ExpressionContext>(i);
}

tree::TerminalNode* SparqlParser::BuiltInCallContext::CLOSE_BRACE() {
  return getToken(SparqlParser::CLOSE_BRACE, 0);
}

tree::TerminalNode* SparqlParser::BuiltInCallContext::LANG() {
  return getToken(SparqlParser::LANG, 0);
}

tree::TerminalNode* SparqlParser::BuiltInCallContext::LANGMATCHES() {
  return getToken(SparqlParser::LANGMATCHES, 0);
}

std::vector<tree::TerminalNode *> SparqlParser::BuiltInCallContext::COMMA() {
  return getTokens(SparqlParser::COMMA);
}

tree::TerminalNode* SparqlParser::BuiltInCallContext::COMMA(size_t i) {
  return getToken(SparqlParser::COMMA, i);
}

tree::TerminalNode* SparqlParser::BuiltInCallContext::DATATYPE() {
  return getToken(SparqlParser::DATATYPE, 0);
}

tree::TerminalNode* SparqlParser::BuiltInCallContext::BOUND() {
  return getToken(SparqlParser::BOUND, 0);
}

SparqlParser::VarContext* SparqlParser::BuiltInCallContext::var() {
  return getRuleContext<SparqlParser::VarContext>(0);
}

tree::TerminalNode* SparqlParser::BuiltInCallContext::IRI() {
  return getToken(SparqlParser::IRI, 0);
}

tree::TerminalNode* SparqlParser::BuiltInCallContext::URI() {
  return getToken(SparqlParser::URI, 0);
}

tree::TerminalNode* SparqlParser::BuiltInCallContext::BNODE() {
  return getToken(SparqlParser::BNODE, 0);
}

tree::TerminalNode* SparqlParser::BuiltInCallContext::RAND() {
  return getToken(SparqlParser::RAND, 0);
}

tree::TerminalNode* SparqlParser::BuiltInCallContext::ABS() {
  return getToken(SparqlParser::ABS, 0);
}

tree::TerminalNode* SparqlParser::BuiltInCallContext::CEIL() {
  return getToken(SparqlParser::CEIL, 0);
}

tree::TerminalNode* SparqlParser::BuiltInCallContext::FLOOR() {
  return getToken(SparqlParser::FLOOR, 0);
}

tree::TerminalNode* SparqlParser::BuiltInCallContext::ROUND() {
  return getToken(SparqlParser::ROUND, 0);
}

tree::TerminalNode* SparqlParser::BuiltInCallContext::CONCAT() {
  return getToken(SparqlParser::CONCAT, 0);
}

SparqlParser::ExpressionListContext* SparqlParser::BuiltInCallContext::expressionList() {
  return getRuleContext<SparqlParser::ExpressionListContext>(0);
}

SparqlParser::SubStringExpressionContext* SparqlParser::BuiltInCallContext::subStringExpression() {
  return getRuleContext<SparqlParser::SubStringExpressionContext>(0);
}

tree::TerminalNode* SparqlParser::BuiltInCallContext::STRLEN() {
  return getToken(SparqlParser::STRLEN, 0);
}

SparqlParser::StrReplaceExpressionContext* SparqlParser::BuiltInCallContext::strReplaceExpression() {
  return getRuleContext<SparqlParser::StrReplaceExpressionContext>(0);
}

tree::TerminalNode* SparqlParser::BuiltInCallContext::UCASE() {
  return getToken(SparqlParser::UCASE, 0);
}

tree::TerminalNode* SparqlParser::BuiltInCallContext::LCASE() {
  return getToken(SparqlParser::LCASE, 0);
}

tree::TerminalNode* SparqlParser::BuiltInCallContext::ENCODE_FOR_URI() {
  return getToken(SparqlParser::ENCODE_FOR_URI, 0);
}

tree::TerminalNode* SparqlParser::BuiltInCallContext::CONTAINS() {
  return getToken(SparqlParser::CONTAINS, 0);
}

tree::TerminalNode* SparqlParser::BuiltInCallContext::STRSTARTS() {
  return getToken(SparqlParser::STRSTARTS, 0);
}

tree::TerminalNode* SparqlParser::BuiltInCallContext::STRENDS() {
  return getToken(SparqlParser::STRENDS, 0);
}

tree::TerminalNode* SparqlParser::BuiltInCallContext::STRBEFORE() {
  return getToken(SparqlParser::STRBEFORE, 0);
}

tree::TerminalNode* SparqlParser::BuiltInCallContext::STRAFTER() {
  return getToken(SparqlParser::STRAFTER, 0);
}

tree::TerminalNode* SparqlParser::BuiltInCallContext::YEAR() {
  return getToken(SparqlParser::YEAR, 0);
}

tree::TerminalNode* SparqlParser::BuiltInCallContext::MONTH() {
  return getToken(SparqlParser::MONTH, 0);
}

tree::TerminalNode* SparqlParser::BuiltInCallContext::DAY() {
  return getToken(SparqlParser::DAY, 0);
}

tree::TerminalNode* SparqlParser::BuiltInCallContext::HOURS() {
  return getToken(SparqlParser::HOURS, 0);
}

tree::TerminalNode* SparqlParser::BuiltInCallContext::MINUTES() {
  return getToken(SparqlParser::MINUTES, 0);
}

tree::TerminalNode* SparqlParser::BuiltInCallContext::SECONDS() {
  return getToken(SparqlParser::SECONDS, 0);
}

tree::TerminalNode* SparqlParser::BuiltInCallContext::TIMEZONE() {
  return getToken(SparqlParser::TIMEZONE, 0);
}

tree::TerminalNode* SparqlParser::BuiltInCallContext::TZ() {
  return getToken(SparqlParser::TZ, 0);
}

tree::TerminalNode* SparqlParser::BuiltInCallContext::NOW() {
  return getToken(SparqlParser::NOW, 0);
}

tree::TerminalNode* SparqlParser::BuiltInCallContext::UUID() {
  return getToken(SparqlParser::UUID, 0);
}

tree::TerminalNode* SparqlParser::BuiltInCallContext::STRUUID() {
  return getToken(SparqlParser::STRUUID, 0);
}

tree::TerminalNode* SparqlParser::BuiltInCallContext::MD5() {
  return getToken(SparqlParser::MD5, 0);
}

tree::TerminalNode* SparqlParser::BuiltInCallContext::SHA1() {
  return getToken(SparqlParser::SHA1, 0);
}

tree::TerminalNode* SparqlParser::BuiltInCallContext::SHA256() {
  return getToken(SparqlParser::SHA256, 0);
}

tree::TerminalNode* SparqlParser::BuiltInCallContext::SHA384() {
  return getToken(SparqlParser::SHA384, 0);
}

tree::TerminalNode* SparqlParser::BuiltInCallContext::SHA512() {
  return getToken(SparqlParser::SHA512, 0);
}

tree::TerminalNode* SparqlParser::BuiltInCallContext::COALESCE() {
  return getToken(SparqlParser::COALESCE, 0);
}

tree::TerminalNode* SparqlParser::BuiltInCallContext::IF() {
  return getToken(SparqlParser::IF, 0);
}

tree::TerminalNode* SparqlParser::BuiltInCallContext::STRLANG() {
  return getToken(SparqlParser::STRLANG, 0);
}

tree::TerminalNode* SparqlParser::BuiltInCallContext::STRDT() {
  return getToken(SparqlParser::STRDT, 0);
}

tree::TerminalNode* SparqlParser::BuiltInCallContext::SAMETERM() {
  return getToken(SparqlParser::SAMETERM, 0);
}

tree::TerminalNode* SparqlParser::BuiltInCallContext::ISIRI() {
  return getToken(SparqlParser::ISIRI, 0);
}

tree::TerminalNode* SparqlParser::BuiltInCallContext::ISURI() {
  return getToken(SparqlParser::ISURI, 0);
}

tree::TerminalNode* SparqlParser::BuiltInCallContext::ISBLANK() {
  return getToken(SparqlParser::ISBLANK, 0);
}

tree::TerminalNode* SparqlParser::BuiltInCallContext::ISLITERAL() {
  return getToken(SparqlParser::ISLITERAL, 0);
}

tree::TerminalNode* SparqlParser::BuiltInCallContext::ISNUMERIC() {
  return getToken(SparqlParser::ISNUMERIC, 0);
}

SparqlParser::RegexExpressionContext* SparqlParser::BuiltInCallContext::regexExpression() {
  return getRuleContext<SparqlParser::RegexExpressionContext>(0);
}

SparqlParser::ExistsFunctionContext* SparqlParser::BuiltInCallContext::existsFunction() {
  return getRuleContext<SparqlParser::ExistsFunctionContext>(0);
}

SparqlParser::NotExistsFunctionContext* SparqlParser::BuiltInCallContext::notExistsFunction() {
  return getRuleContext<SparqlParser::NotExistsFunctionContext>(0);
}


size_t SparqlParser::BuiltInCallContext::getRuleIndex() const {
  return SparqlParser::RuleBuiltInCall;
}


antlrcpp::Any SparqlParser::BuiltInCallContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitBuiltInCall(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::BuiltInCallContext* SparqlParser::builtInCall() {
  BuiltInCallContext *_localctx = _tracker.createInstance<BuiltInCallContext>(_ctx, getState());
  enterRule(_localctx, 230, SparqlParser::RuleBuiltInCall);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(1376);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlParser::COUNT:
      case SparqlParser::SUM:
      case SparqlParser::MIN:
      case SparqlParser::MAX:
      case SparqlParser::AVG:
      case SparqlParser::SAMPLE:
      case SparqlParser::GROUP_CONCAT: {
        enterOuterAlt(_localctx, 1);
        setState(1108);
        aggregate();
        break;
      }

      case SparqlParser::STR: {
        enterOuterAlt(_localctx, 2);
        setState(1109);
        match(SparqlParser::STR);
        setState(1110);
        match(SparqlParser::OPEN_BRACE);
        setState(1111);
        expression(0);
        setState(1112);
        match(SparqlParser::CLOSE_BRACE);
        break;
      }

      case SparqlParser::LANG: {
        enterOuterAlt(_localctx, 3);
        setState(1114);
        match(SparqlParser::LANG);
        setState(1115);
        match(SparqlParser::OPEN_BRACE);
        setState(1116);
        expression(0);
        setState(1117);
        match(SparqlParser::CLOSE_BRACE);
        break;
      }

      case SparqlParser::LANGMATCHES: {
        enterOuterAlt(_localctx, 4);
        setState(1119);
        match(SparqlParser::LANGMATCHES);
        setState(1120);
        match(SparqlParser::OPEN_BRACE);
        setState(1121);
        expression(0);
        setState(1122);
        match(SparqlParser::COMMA);
        setState(1123);
        expression(0);
        setState(1124);
        match(SparqlParser::CLOSE_BRACE);
        break;
      }

      case SparqlParser::DATATYPE: {
        enterOuterAlt(_localctx, 5);
        setState(1126);
        match(SparqlParser::DATATYPE);
        setState(1127);
        match(SparqlParser::OPEN_BRACE);
        setState(1128);
        expression(0);
        setState(1129);
        match(SparqlParser::CLOSE_BRACE);
        break;
      }

      case SparqlParser::BOUND: {
        enterOuterAlt(_localctx, 6);
        setState(1131);
        match(SparqlParser::BOUND);
        setState(1132);
        match(SparqlParser::OPEN_BRACE);
        setState(1133);
        var();
        setState(1134);
        match(SparqlParser::CLOSE_BRACE);
        break;
      }

      case SparqlParser::IRI: {
        enterOuterAlt(_localctx, 7);
        setState(1136);
        match(SparqlParser::IRI);
        setState(1137);
        match(SparqlParser::OPEN_BRACE);
        setState(1138);
        expression(0);
        setState(1139);
        match(SparqlParser::CLOSE_BRACE);
        break;
      }

      case SparqlParser::URI: {
        enterOuterAlt(_localctx, 8);
        setState(1141);
        match(SparqlParser::URI);
        setState(1142);
        match(SparqlParser::OPEN_BRACE);
        setState(1143);
        expression(0);
        setState(1144);
        match(SparqlParser::CLOSE_BRACE);
        break;
      }

      case SparqlParser::BNODE: {
        enterOuterAlt(_localctx, 9);
        setState(1146);
        match(SparqlParser::BNODE);
        setState(1147);
        match(SparqlParser::OPEN_BRACE);
        setState(1149);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if ((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & ((1ULL << SparqlParser::STR)
          | (1ULL << SparqlParser::LANG)
          | (1ULL << SparqlParser::LANGMATCHES)
          | (1ULL << SparqlParser::DATATYPE)
          | (1ULL << SparqlParser::BOUND)
          | (1ULL << SparqlParser::SAMETERM)
          | (1ULL << SparqlParser::ISIRI)
          | (1ULL << SparqlParser::ISURI)
          | (1ULL << SparqlParser::ISBLANK)
          | (1ULL << SparqlParser::ISLITERAL)
          | (1ULL << SparqlParser::REGEX)
          | (1ULL << SparqlParser::SUBSTR)
          | (1ULL << SparqlParser::TRUE)
          | (1ULL << SparqlParser::FALSE))) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 64)) & ((1ULL << (SparqlParser::IRI - 64))
          | (1ULL << (SparqlParser::URI - 64))
          | (1ULL << (SparqlParser::BNODE - 64))
          | (1ULL << (SparqlParser::RAND - 64))
          | (1ULL << (SparqlParser::ABS - 64))
          | (1ULL << (SparqlParser::CEIL - 64))
          | (1ULL << (SparqlParser::FLOOR - 64))
          | (1ULL << (SparqlParser::ROUND - 64))
          | (1ULL << (SparqlParser::CONCAT - 64))
          | (1ULL << (SparqlParser::STRLEN - 64))
          | (1ULL << (SparqlParser::UCASE - 64))
          | (1ULL << (SparqlParser::LCASE - 64))
          | (1ULL << (SparqlParser::ENCODE_FOR_URI - 64))
          | (1ULL << (SparqlParser::CONTAINS - 64))
          | (1ULL << (SparqlParser::STRSTARTS - 64))
          | (1ULL << (SparqlParser::STRENDS - 64))
          | (1ULL << (SparqlParser::STRBEFORE - 64))
          | (1ULL << (SparqlParser::STRAFTER - 64))
          | (1ULL << (SparqlParser::REPLACE - 64))
          | (1ULL << (SparqlParser::YEAR - 64))
          | (1ULL << (SparqlParser::MONTH - 64))
          | (1ULL << (SparqlParser::DAY - 64))
          | (1ULL << (SparqlParser::HOURS - 64))
          | (1ULL << (SparqlParser::MINUTES - 64))
          | (1ULL << (SparqlParser::SECONDS - 64))
          | (1ULL << (SparqlParser::TIMEZONE - 64))
          | (1ULL << (SparqlParser::TZ - 64))
          | (1ULL << (SparqlParser::NOW - 64))
          | (1ULL << (SparqlParser::UUID - 64))
          | (1ULL << (SparqlParser::STRUUID - 64))
          | (1ULL << (SparqlParser::MD5 - 64))
          | (1ULL << (SparqlParser::SHA1 - 64))
          | (1ULL << (SparqlParser::SHA256 - 64))
          | (1ULL << (SparqlParser::SHA384 - 64))
          | (1ULL << (SparqlParser::SHA512 - 64))
          | (1ULL << (SparqlParser::COALESCE - 64))
          | (1ULL << (SparqlParser::IF - 64))
          | (1ULL << (SparqlParser::STRLANG - 64))
          | (1ULL << (SparqlParser::STRDT - 64))
          | (1ULL << (SparqlParser::ISNUMERIC - 64))
          | (1ULL << (SparqlParser::COUNT - 64))
          | (1ULL << (SparqlParser::SUM - 64))
          | (1ULL << (SparqlParser::MIN - 64))
          | (1ULL << (SparqlParser::MAX - 64))
          | (1ULL << (SparqlParser::AVG - 64))
          | (1ULL << (SparqlParser::SAMPLE - 64))
          | (1ULL << (SparqlParser::GROUP_CONCAT - 64))
          | (1ULL << (SparqlParser::NOT - 64))
          | (1ULL << (SparqlParser::EXISTS - 64))
          | (1ULL << (SparqlParser::IRIREF - 64))
          | (1ULL << (SparqlParser::PNAME_NS - 64))
          | (1ULL << (SparqlParser::PNAME_LN - 64))
          | (1ULL << (SparqlParser::VAR1 - 64))
          | (1ULL << (SparqlParser::VAR2 - 64))
          | (1ULL << (SparqlParser::INTEGER - 64))
          | (1ULL << (SparqlParser::DECIMAL - 64))
          | (1ULL << (SparqlParser::DOUBLE - 64))
          | (1ULL << (SparqlParser::INTEGER_POSITIVE - 64))
          | (1ULL << (SparqlParser::DECIMAL_POSITIVE - 64))
          | (1ULL << (SparqlParser::DOUBLE_POSITIVE - 64)))) != 0) || ((((_la - 128) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 128)) & ((1ULL << (SparqlParser::INTEGER_NEGATIVE - 128))
          | (1ULL << (SparqlParser::DECIMAL_NEGATIVE - 128))
          | (1ULL << (SparqlParser::DOUBLE_NEGATIVE - 128))
          | (1ULL << (SparqlParser::STRING_LITERAL1 - 128))
          | (1ULL << (SparqlParser::STRING_LITERAL2 - 128))
          | (1ULL << (SparqlParser::STRING_LITERAL_LONG1 - 128))
          | (1ULL << (SparqlParser::STRING_LITERAL_LONG2 - 128))
          | (1ULL << (SparqlParser::OPEN_BRACE - 128))
          | (1ULL << (SparqlParser::PLUS_SIGN - 128))
          | (1ULL << (SparqlParser::MINUS_SIGN - 128))
          | (1ULL << (SparqlParser::ASTERISK - 128))
          | (1ULL << (SparqlParser::NEGATION - 128))
          | (1ULL << (SparqlParser::DIVIDE - 128)))) != 0)) {
          setState(1148);
          expression(0);
        }
        setState(1151);
        match(SparqlParser::CLOSE_BRACE);
        break;
      }

      case SparqlParser::RAND: {
        enterOuterAlt(_localctx, 10);
        setState(1152);
        match(SparqlParser::RAND);
        setState(1153);
        match(SparqlParser::OPEN_BRACE);
        setState(1154);
        match(SparqlParser::CLOSE_BRACE);
        break;
      }

      case SparqlParser::ABS: {
        enterOuterAlt(_localctx, 11);
        setState(1155);
        match(SparqlParser::ABS);
        setState(1156);
        match(SparqlParser::OPEN_BRACE);
        setState(1157);
        expression(0);
        setState(1158);
        match(SparqlParser::CLOSE_BRACE);
        break;
      }

      case SparqlParser::CEIL: {
        enterOuterAlt(_localctx, 12);
        setState(1160);
        match(SparqlParser::CEIL);
        setState(1161);
        match(SparqlParser::OPEN_BRACE);
        setState(1162);
        expression(0);
        setState(1163);
        match(SparqlParser::CLOSE_BRACE);
        break;
      }

      case SparqlParser::FLOOR: {
        enterOuterAlt(_localctx, 13);
        setState(1165);
        match(SparqlParser::FLOOR);
        setState(1166);
        match(SparqlParser::OPEN_BRACE);
        setState(1167);
        expression(0);
        setState(1168);
        match(SparqlParser::CLOSE_BRACE);
        break;
      }

      case SparqlParser::ROUND: {
        enterOuterAlt(_localctx, 14);
        setState(1170);
        match(SparqlParser::ROUND);
        setState(1171);
        match(SparqlParser::OPEN_BRACE);
        setState(1172);
        expression(0);
        setState(1173);
        match(SparqlParser::CLOSE_BRACE);
        break;
      }

      case SparqlParser::CONCAT: {
        enterOuterAlt(_localctx, 15);
        setState(1175);
        match(SparqlParser::CONCAT);
        setState(1176);
        match(SparqlParser::OPEN_BRACE);
        setState(1178);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if ((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & ((1ULL << SparqlParser::STR)
          | (1ULL << SparqlParser::LANG)
          | (1ULL << SparqlParser::LANGMATCHES)
          | (1ULL << SparqlParser::DATATYPE)
          | (1ULL << SparqlParser::BOUND)
          | (1ULL << SparqlParser::SAMETERM)
          | (1ULL << SparqlParser::ISIRI)
          | (1ULL << SparqlParser::ISURI)
          | (1ULL << SparqlParser::ISBLANK)
          | (1ULL << SparqlParser::ISLITERAL)
          | (1ULL << SparqlParser::REGEX)
          | (1ULL << SparqlParser::SUBSTR)
          | (1ULL << SparqlParser::TRUE)
          | (1ULL << SparqlParser::FALSE))) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 64)) & ((1ULL << (SparqlParser::IRI - 64))
          | (1ULL << (SparqlParser::URI - 64))
          | (1ULL << (SparqlParser::BNODE - 64))
          | (1ULL << (SparqlParser::RAND - 64))
          | (1ULL << (SparqlParser::ABS - 64))
          | (1ULL << (SparqlParser::CEIL - 64))
          | (1ULL << (SparqlParser::FLOOR - 64))
          | (1ULL << (SparqlParser::ROUND - 64))
          | (1ULL << (SparqlParser::CONCAT - 64))
          | (1ULL << (SparqlParser::STRLEN - 64))
          | (1ULL << (SparqlParser::UCASE - 64))
          | (1ULL << (SparqlParser::LCASE - 64))
          | (1ULL << (SparqlParser::ENCODE_FOR_URI - 64))
          | (1ULL << (SparqlParser::CONTAINS - 64))
          | (1ULL << (SparqlParser::STRSTARTS - 64))
          | (1ULL << (SparqlParser::STRENDS - 64))
          | (1ULL << (SparqlParser::STRBEFORE - 64))
          | (1ULL << (SparqlParser::STRAFTER - 64))
          | (1ULL << (SparqlParser::REPLACE - 64))
          | (1ULL << (SparqlParser::YEAR - 64))
          | (1ULL << (SparqlParser::MONTH - 64))
          | (1ULL << (SparqlParser::DAY - 64))
          | (1ULL << (SparqlParser::HOURS - 64))
          | (1ULL << (SparqlParser::MINUTES - 64))
          | (1ULL << (SparqlParser::SECONDS - 64))
          | (1ULL << (SparqlParser::TIMEZONE - 64))
          | (1ULL << (SparqlParser::TZ - 64))
          | (1ULL << (SparqlParser::NOW - 64))
          | (1ULL << (SparqlParser::UUID - 64))
          | (1ULL << (SparqlParser::STRUUID - 64))
          | (1ULL << (SparqlParser::MD5 - 64))
          | (1ULL << (SparqlParser::SHA1 - 64))
          | (1ULL << (SparqlParser::SHA256 - 64))
          | (1ULL << (SparqlParser::SHA384 - 64))
          | (1ULL << (SparqlParser::SHA512 - 64))
          | (1ULL << (SparqlParser::COALESCE - 64))
          | (1ULL << (SparqlParser::IF - 64))
          | (1ULL << (SparqlParser::STRLANG - 64))
          | (1ULL << (SparqlParser::STRDT - 64))
          | (1ULL << (SparqlParser::ISNUMERIC - 64))
          | (1ULL << (SparqlParser::COUNT - 64))
          | (1ULL << (SparqlParser::SUM - 64))
          | (1ULL << (SparqlParser::MIN - 64))
          | (1ULL << (SparqlParser::MAX - 64))
          | (1ULL << (SparqlParser::AVG - 64))
          | (1ULL << (SparqlParser::SAMPLE - 64))
          | (1ULL << (SparqlParser::GROUP_CONCAT - 64))
          | (1ULL << (SparqlParser::NOT - 64))
          | (1ULL << (SparqlParser::EXISTS - 64))
          | (1ULL << (SparqlParser::IRIREF - 64))
          | (1ULL << (SparqlParser::PNAME_NS - 64))
          | (1ULL << (SparqlParser::PNAME_LN - 64))
          | (1ULL << (SparqlParser::VAR1 - 64))
          | (1ULL << (SparqlParser::VAR2 - 64))
          | (1ULL << (SparqlParser::INTEGER - 64))
          | (1ULL << (SparqlParser::DECIMAL - 64))
          | (1ULL << (SparqlParser::DOUBLE - 64))
          | (1ULL << (SparqlParser::INTEGER_POSITIVE - 64))
          | (1ULL << (SparqlParser::DECIMAL_POSITIVE - 64))
          | (1ULL << (SparqlParser::DOUBLE_POSITIVE - 64)))) != 0) || ((((_la - 128) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 128)) & ((1ULL << (SparqlParser::INTEGER_NEGATIVE - 128))
          | (1ULL << (SparqlParser::DECIMAL_NEGATIVE - 128))
          | (1ULL << (SparqlParser::DOUBLE_NEGATIVE - 128))
          | (1ULL << (SparqlParser::STRING_LITERAL1 - 128))
          | (1ULL << (SparqlParser::STRING_LITERAL2 - 128))
          | (1ULL << (SparqlParser::STRING_LITERAL_LONG1 - 128))
          | (1ULL << (SparqlParser::STRING_LITERAL_LONG2 - 128))
          | (1ULL << (SparqlParser::OPEN_BRACE - 128))
          | (1ULL << (SparqlParser::PLUS_SIGN - 128))
          | (1ULL << (SparqlParser::MINUS_SIGN - 128))
          | (1ULL << (SparqlParser::ASTERISK - 128))
          | (1ULL << (SparqlParser::NEGATION - 128))
          | (1ULL << (SparqlParser::DIVIDE - 128)))) != 0)) {
          setState(1177);
          expressionList();
        }
        setState(1180);
        match(SparqlParser::CLOSE_BRACE);
        break;
      }

      case SparqlParser::SUBSTR: {
        enterOuterAlt(_localctx, 16);
        setState(1181);
        subStringExpression();
        break;
      }

      case SparqlParser::STRLEN: {
        enterOuterAlt(_localctx, 17);
        setState(1182);
        match(SparqlParser::STRLEN);
        setState(1183);
        match(SparqlParser::OPEN_BRACE);
        setState(1184);
        expression(0);
        setState(1185);
        match(SparqlParser::CLOSE_BRACE);
        break;
      }

      case SparqlParser::REPLACE: {
        enterOuterAlt(_localctx, 18);
        setState(1187);
        strReplaceExpression();
        break;
      }

      case SparqlParser::UCASE: {
        enterOuterAlt(_localctx, 19);
        setState(1188);
        match(SparqlParser::UCASE);
        setState(1189);
        match(SparqlParser::OPEN_BRACE);
        setState(1190);
        expression(0);
        setState(1191);
        match(SparqlParser::CLOSE_BRACE);
        break;
      }

      case SparqlParser::LCASE: {
        enterOuterAlt(_localctx, 20);
        setState(1193);
        match(SparqlParser::LCASE);
        setState(1194);
        match(SparqlParser::OPEN_BRACE);
        setState(1195);
        expression(0);
        setState(1196);
        match(SparqlParser::CLOSE_BRACE);
        break;
      }

      case SparqlParser::ENCODE_FOR_URI: {
        enterOuterAlt(_localctx, 21);
        setState(1198);
        match(SparqlParser::ENCODE_FOR_URI);
        setState(1199);
        match(SparqlParser::OPEN_BRACE);
        setState(1200);
        expression(0);
        setState(1201);
        match(SparqlParser::CLOSE_BRACE);
        break;
      }

      case SparqlParser::CONTAINS: {
        enterOuterAlt(_localctx, 22);
        setState(1203);
        match(SparqlParser::CONTAINS);
        setState(1204);
        match(SparqlParser::OPEN_BRACE);
        setState(1205);
        expression(0);
        setState(1206);
        match(SparqlParser::COMMA);
        setState(1207);
        expression(0);
        setState(1208);
        match(SparqlParser::CLOSE_BRACE);
        break;
      }

      case SparqlParser::STRSTARTS: {
        enterOuterAlt(_localctx, 23);
        setState(1210);
        match(SparqlParser::STRSTARTS);
        setState(1211);
        match(SparqlParser::OPEN_BRACE);
        setState(1212);
        expression(0);
        setState(1213);
        match(SparqlParser::COMMA);
        setState(1214);
        expression(0);
        setState(1215);
        match(SparqlParser::CLOSE_BRACE);
        break;
      }

      case SparqlParser::STRENDS: {
        enterOuterAlt(_localctx, 24);
        setState(1217);
        match(SparqlParser::STRENDS);
        setState(1218);
        match(SparqlParser::OPEN_BRACE);
        setState(1219);
        expression(0);
        setState(1220);
        match(SparqlParser::COMMA);
        setState(1221);
        expression(0);
        setState(1222);
        match(SparqlParser::CLOSE_BRACE);
        break;
      }

      case SparqlParser::STRBEFORE: {
        enterOuterAlt(_localctx, 25);
        setState(1224);
        match(SparqlParser::STRBEFORE);
        setState(1225);
        match(SparqlParser::OPEN_BRACE);
        setState(1226);
        expression(0);
        setState(1227);
        match(SparqlParser::COMMA);
        setState(1228);
        expression(0);
        setState(1229);
        match(SparqlParser::CLOSE_BRACE);
        break;
      }

      case SparqlParser::STRAFTER: {
        enterOuterAlt(_localctx, 26);
        setState(1231);
        match(SparqlParser::STRAFTER);
        setState(1232);
        match(SparqlParser::OPEN_BRACE);
        setState(1233);
        expression(0);
        setState(1234);
        match(SparqlParser::COMMA);
        setState(1235);
        expression(0);
        setState(1236);
        match(SparqlParser::CLOSE_BRACE);
        break;
      }

      case SparqlParser::YEAR: {
        enterOuterAlt(_localctx, 27);
        setState(1238);
        match(SparqlParser::YEAR);
        setState(1239);
        match(SparqlParser::OPEN_BRACE);
        setState(1240);
        expression(0);
        setState(1241);
        match(SparqlParser::CLOSE_BRACE);
        break;
      }

      case SparqlParser::MONTH: {
        enterOuterAlt(_localctx, 28);
        setState(1243);
        match(SparqlParser::MONTH);
        setState(1244);
        match(SparqlParser::OPEN_BRACE);
        setState(1245);
        expression(0);
        setState(1246);
        match(SparqlParser::CLOSE_BRACE);
        break;
      }

      case SparqlParser::DAY: {
        enterOuterAlt(_localctx, 29);
        setState(1248);
        match(SparqlParser::DAY);
        setState(1249);
        match(SparqlParser::OPEN_BRACE);
        setState(1250);
        expression(0);
        setState(1251);
        match(SparqlParser::CLOSE_BRACE);
        break;
      }

      case SparqlParser::HOURS: {
        enterOuterAlt(_localctx, 30);
        setState(1253);
        match(SparqlParser::HOURS);
        setState(1254);
        match(SparqlParser::OPEN_BRACE);
        setState(1255);
        expression(0);
        setState(1256);
        match(SparqlParser::CLOSE_BRACE);
        break;
      }

      case SparqlParser::MINUTES: {
        enterOuterAlt(_localctx, 31);
        setState(1258);
        match(SparqlParser::MINUTES);
        setState(1259);
        match(SparqlParser::OPEN_BRACE);
        setState(1260);
        expression(0);
        setState(1261);
        match(SparqlParser::CLOSE_BRACE);
        break;
      }

      case SparqlParser::SECONDS: {
        enterOuterAlt(_localctx, 32);
        setState(1263);
        match(SparqlParser::SECONDS);
        setState(1264);
        match(SparqlParser::OPEN_BRACE);
        setState(1265);
        expression(0);
        setState(1266);
        match(SparqlParser::CLOSE_BRACE);
        break;
      }

      case SparqlParser::TIMEZONE: {
        enterOuterAlt(_localctx, 33);
        setState(1268);
        match(SparqlParser::TIMEZONE);
        setState(1269);
        match(SparqlParser::OPEN_BRACE);
        setState(1270);
        expression(0);
        setState(1271);
        match(SparqlParser::CLOSE_BRACE);
        break;
      }

      case SparqlParser::TZ: {
        enterOuterAlt(_localctx, 34);
        setState(1273);
        match(SparqlParser::TZ);
        setState(1274);
        match(SparqlParser::OPEN_BRACE);
        setState(1275);
        expression(0);
        setState(1276);
        match(SparqlParser::CLOSE_BRACE);
        break;
      }

      case SparqlParser::NOW: {
        enterOuterAlt(_localctx, 35);
        setState(1278);
        match(SparqlParser::NOW);
        setState(1279);
        match(SparqlParser::OPEN_BRACE);
        setState(1280);
        match(SparqlParser::CLOSE_BRACE);
        break;
      }

      case SparqlParser::UUID: {
        enterOuterAlt(_localctx, 36);
        setState(1281);
        match(SparqlParser::UUID);
        setState(1282);
        match(SparqlParser::OPEN_BRACE);
        setState(1283);
        match(SparqlParser::CLOSE_BRACE);
        break;
      }

      case SparqlParser::STRUUID: {
        enterOuterAlt(_localctx, 37);
        setState(1284);
        match(SparqlParser::STRUUID);
        setState(1285);
        match(SparqlParser::OPEN_BRACE);
        setState(1286);
        match(SparqlParser::CLOSE_BRACE);
        break;
      }

      case SparqlParser::MD5: {
        enterOuterAlt(_localctx, 38);
        setState(1287);
        match(SparqlParser::MD5);
        setState(1288);
        match(SparqlParser::OPEN_BRACE);
        setState(1289);
        expression(0);
        setState(1290);
        match(SparqlParser::CLOSE_BRACE);
        break;
      }

      case SparqlParser::SHA1: {
        enterOuterAlt(_localctx, 39);
        setState(1292);
        match(SparqlParser::SHA1);
        setState(1293);
        match(SparqlParser::OPEN_BRACE);
        setState(1294);
        expression(0);
        setState(1295);
        match(SparqlParser::CLOSE_BRACE);
        break;
      }

      case SparqlParser::SHA256: {
        enterOuterAlt(_localctx, 40);
        setState(1297);
        match(SparqlParser::SHA256);
        setState(1298);
        match(SparqlParser::OPEN_BRACE);
        setState(1299);
        expression(0);
        setState(1300);
        match(SparqlParser::CLOSE_BRACE);
        break;
      }

      case SparqlParser::SHA384: {
        enterOuterAlt(_localctx, 41);
        setState(1302);
        match(SparqlParser::SHA384);
        setState(1303);
        match(SparqlParser::OPEN_BRACE);
        setState(1304);
        expression(0);
        setState(1305);
        match(SparqlParser::CLOSE_BRACE);
        break;
      }

      case SparqlParser::SHA512: {
        enterOuterAlt(_localctx, 42);
        setState(1307);
        match(SparqlParser::SHA512);
        setState(1308);
        match(SparqlParser::OPEN_BRACE);
        setState(1309);
        expression(0);
        setState(1310);
        match(SparqlParser::CLOSE_BRACE);
        break;
      }

      case SparqlParser::COALESCE: {
        enterOuterAlt(_localctx, 43);
        setState(1312);
        match(SparqlParser::COALESCE);
        setState(1313);
        match(SparqlParser::OPEN_BRACE);
        setState(1315);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if ((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & ((1ULL << SparqlParser::STR)
          | (1ULL << SparqlParser::LANG)
          | (1ULL << SparqlParser::LANGMATCHES)
          | (1ULL << SparqlParser::DATATYPE)
          | (1ULL << SparqlParser::BOUND)
          | (1ULL << SparqlParser::SAMETERM)
          | (1ULL << SparqlParser::ISIRI)
          | (1ULL << SparqlParser::ISURI)
          | (1ULL << SparqlParser::ISBLANK)
          | (1ULL << SparqlParser::ISLITERAL)
          | (1ULL << SparqlParser::REGEX)
          | (1ULL << SparqlParser::SUBSTR)
          | (1ULL << SparqlParser::TRUE)
          | (1ULL << SparqlParser::FALSE))) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 64)) & ((1ULL << (SparqlParser::IRI - 64))
          | (1ULL << (SparqlParser::URI - 64))
          | (1ULL << (SparqlParser::BNODE - 64))
          | (1ULL << (SparqlParser::RAND - 64))
          | (1ULL << (SparqlParser::ABS - 64))
          | (1ULL << (SparqlParser::CEIL - 64))
          | (1ULL << (SparqlParser::FLOOR - 64))
          | (1ULL << (SparqlParser::ROUND - 64))
          | (1ULL << (SparqlParser::CONCAT - 64))
          | (1ULL << (SparqlParser::STRLEN - 64))
          | (1ULL << (SparqlParser::UCASE - 64))
          | (1ULL << (SparqlParser::LCASE - 64))
          | (1ULL << (SparqlParser::ENCODE_FOR_URI - 64))
          | (1ULL << (SparqlParser::CONTAINS - 64))
          | (1ULL << (SparqlParser::STRSTARTS - 64))
          | (1ULL << (SparqlParser::STRENDS - 64))
          | (1ULL << (SparqlParser::STRBEFORE - 64))
          | (1ULL << (SparqlParser::STRAFTER - 64))
          | (1ULL << (SparqlParser::REPLACE - 64))
          | (1ULL << (SparqlParser::YEAR - 64))
          | (1ULL << (SparqlParser::MONTH - 64))
          | (1ULL << (SparqlParser::DAY - 64))
          | (1ULL << (SparqlParser::HOURS - 64))
          | (1ULL << (SparqlParser::MINUTES - 64))
          | (1ULL << (SparqlParser::SECONDS - 64))
          | (1ULL << (SparqlParser::TIMEZONE - 64))
          | (1ULL << (SparqlParser::TZ - 64))
          | (1ULL << (SparqlParser::NOW - 64))
          | (1ULL << (SparqlParser::UUID - 64))
          | (1ULL << (SparqlParser::STRUUID - 64))
          | (1ULL << (SparqlParser::MD5 - 64))
          | (1ULL << (SparqlParser::SHA1 - 64))
          | (1ULL << (SparqlParser::SHA256 - 64))
          | (1ULL << (SparqlParser::SHA384 - 64))
          | (1ULL << (SparqlParser::SHA512 - 64))
          | (1ULL << (SparqlParser::COALESCE - 64))
          | (1ULL << (SparqlParser::IF - 64))
          | (1ULL << (SparqlParser::STRLANG - 64))
          | (1ULL << (SparqlParser::STRDT - 64))
          | (1ULL << (SparqlParser::ISNUMERIC - 64))
          | (1ULL << (SparqlParser::COUNT - 64))
          | (1ULL << (SparqlParser::SUM - 64))
          | (1ULL << (SparqlParser::MIN - 64))
          | (1ULL << (SparqlParser::MAX - 64))
          | (1ULL << (SparqlParser::AVG - 64))
          | (1ULL << (SparqlParser::SAMPLE - 64))
          | (1ULL << (SparqlParser::GROUP_CONCAT - 64))
          | (1ULL << (SparqlParser::NOT - 64))
          | (1ULL << (SparqlParser::EXISTS - 64))
          | (1ULL << (SparqlParser::IRIREF - 64))
          | (1ULL << (SparqlParser::PNAME_NS - 64))
          | (1ULL << (SparqlParser::PNAME_LN - 64))
          | (1ULL << (SparqlParser::VAR1 - 64))
          | (1ULL << (SparqlParser::VAR2 - 64))
          | (1ULL << (SparqlParser::INTEGER - 64))
          | (1ULL << (SparqlParser::DECIMAL - 64))
          | (1ULL << (SparqlParser::DOUBLE - 64))
          | (1ULL << (SparqlParser::INTEGER_POSITIVE - 64))
          | (1ULL << (SparqlParser::DECIMAL_POSITIVE - 64))
          | (1ULL << (SparqlParser::DOUBLE_POSITIVE - 64)))) != 0) || ((((_la - 128) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 128)) & ((1ULL << (SparqlParser::INTEGER_NEGATIVE - 128))
          | (1ULL << (SparqlParser::DECIMAL_NEGATIVE - 128))
          | (1ULL << (SparqlParser::DOUBLE_NEGATIVE - 128))
          | (1ULL << (SparqlParser::STRING_LITERAL1 - 128))
          | (1ULL << (SparqlParser::STRING_LITERAL2 - 128))
          | (1ULL << (SparqlParser::STRING_LITERAL_LONG1 - 128))
          | (1ULL << (SparqlParser::STRING_LITERAL_LONG2 - 128))
          | (1ULL << (SparqlParser::OPEN_BRACE - 128))
          | (1ULL << (SparqlParser::PLUS_SIGN - 128))
          | (1ULL << (SparqlParser::MINUS_SIGN - 128))
          | (1ULL << (SparqlParser::ASTERISK - 128))
          | (1ULL << (SparqlParser::NEGATION - 128))
          | (1ULL << (SparqlParser::DIVIDE - 128)))) != 0)) {
          setState(1314);
          expressionList();
        }
        setState(1317);
        match(SparqlParser::CLOSE_BRACE);
        break;
      }

      case SparqlParser::IF: {
        enterOuterAlt(_localctx, 44);
        setState(1318);
        match(SparqlParser::IF);
        setState(1319);
        match(SparqlParser::OPEN_BRACE);
        setState(1320);
        expression(0);
        setState(1321);
        match(SparqlParser::COMMA);
        setState(1322);
        expression(0);
        setState(1323);
        match(SparqlParser::COMMA);
        setState(1324);
        expression(0);
        setState(1325);
        match(SparqlParser::CLOSE_BRACE);
        break;
      }

      case SparqlParser::STRLANG: {
        enterOuterAlt(_localctx, 45);
        setState(1327);
        match(SparqlParser::STRLANG);
        setState(1328);
        match(SparqlParser::OPEN_BRACE);
        setState(1329);
        expression(0);
        setState(1330);
        match(SparqlParser::COMMA);
        setState(1331);
        expression(0);
        setState(1332);
        match(SparqlParser::CLOSE_BRACE);
        break;
      }

      case SparqlParser::STRDT: {
        enterOuterAlt(_localctx, 46);
        setState(1334);
        match(SparqlParser::STRDT);
        setState(1335);
        match(SparqlParser::OPEN_BRACE);
        setState(1336);
        expression(0);
        setState(1337);
        match(SparqlParser::COMMA);
        setState(1338);
        expression(0);
        setState(1339);
        match(SparqlParser::CLOSE_BRACE);
        break;
      }

      case SparqlParser::SAMETERM: {
        enterOuterAlt(_localctx, 47);
        setState(1341);
        match(SparqlParser::SAMETERM);
        setState(1342);
        match(SparqlParser::OPEN_BRACE);
        setState(1343);
        expression(0);
        setState(1344);
        match(SparqlParser::COMMA);
        setState(1345);
        expression(0);
        setState(1346);
        match(SparqlParser::CLOSE_BRACE);
        break;
      }

      case SparqlParser::ISIRI: {
        enterOuterAlt(_localctx, 48);
        setState(1348);
        match(SparqlParser::ISIRI);
        setState(1349);
        match(SparqlParser::OPEN_BRACE);
        setState(1350);
        expression(0);
        setState(1351);
        match(SparqlParser::CLOSE_BRACE);
        break;
      }

      case SparqlParser::ISURI: {
        enterOuterAlt(_localctx, 49);
        setState(1353);
        match(SparqlParser::ISURI);
        setState(1354);
        match(SparqlParser::OPEN_BRACE);
        setState(1355);
        expression(0);
        setState(1356);
        match(SparqlParser::CLOSE_BRACE);
        break;
      }

      case SparqlParser::ISBLANK: {
        enterOuterAlt(_localctx, 50);
        setState(1358);
        match(SparqlParser::ISBLANK);
        setState(1359);
        match(SparqlParser::OPEN_BRACE);
        setState(1360);
        expression(0);
        setState(1361);
        match(SparqlParser::CLOSE_BRACE);
        break;
      }

      case SparqlParser::ISLITERAL: {
        enterOuterAlt(_localctx, 51);
        setState(1363);
        match(SparqlParser::ISLITERAL);
        setState(1364);
        match(SparqlParser::OPEN_BRACE);
        setState(1365);
        expression(0);
        setState(1366);
        match(SparqlParser::CLOSE_BRACE);
        break;
      }

      case SparqlParser::ISNUMERIC: {
        enterOuterAlt(_localctx, 52);
        setState(1368);
        match(SparqlParser::ISNUMERIC);
        setState(1369);
        match(SparqlParser::OPEN_BRACE);
        setState(1370);
        expression(0);
        setState(1371);
        match(SparqlParser::CLOSE_BRACE);
        break;
      }

      case SparqlParser::REGEX: {
        enterOuterAlt(_localctx, 53);
        setState(1373);
        regexExpression();
        break;
      }

      case SparqlParser::EXISTS: {
        enterOuterAlt(_localctx, 54);
        setState(1374);
        existsFunction();
        break;
      }

      case SparqlParser::NOT: {
        enterOuterAlt(_localctx, 55);
        setState(1375);
        notExistsFunction();
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

//----------------- RegexExpressionContext ------------------------------------------------------------------

SparqlParser::RegexExpressionContext::RegexExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::RegexExpressionContext::REGEX() {
  return getToken(SparqlParser::REGEX, 0);
}

tree::TerminalNode* SparqlParser::RegexExpressionContext::OPEN_BRACE() {
  return getToken(SparqlParser::OPEN_BRACE, 0);
}

std::vector<SparqlParser::ExpressionContext *> SparqlParser::RegexExpressionContext::expression() {
  return getRuleContexts<SparqlParser::ExpressionContext>();
}

SparqlParser::ExpressionContext* SparqlParser::RegexExpressionContext::expression(size_t i) {
  return getRuleContext<SparqlParser::ExpressionContext>(i);
}

std::vector<tree::TerminalNode *> SparqlParser::RegexExpressionContext::COMMA() {
  return getTokens(SparqlParser::COMMA);
}

tree::TerminalNode* SparqlParser::RegexExpressionContext::COMMA(size_t i) {
  return getToken(SparqlParser::COMMA, i);
}

tree::TerminalNode* SparqlParser::RegexExpressionContext::CLOSE_BRACE() {
  return getToken(SparqlParser::CLOSE_BRACE, 0);
}


size_t SparqlParser::RegexExpressionContext::getRuleIndex() const {
  return SparqlParser::RuleRegexExpression;
}


antlrcpp::Any SparqlParser::RegexExpressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitRegexExpression(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::RegexExpressionContext* SparqlParser::regexExpression() {
  RegexExpressionContext *_localctx = _tracker.createInstance<RegexExpressionContext>(_ctx, getState());
  enterRule(_localctx, 232, SparqlParser::RuleRegexExpression);
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
    setState(1378);
    match(SparqlParser::REGEX);
    setState(1379);
    match(SparqlParser::OPEN_BRACE);
    setState(1380);
    expression(0);
    setState(1381);
    match(SparqlParser::COMMA);
    setState(1382);
    expression(0);
    setState(1385);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == SparqlParser::COMMA) {
      setState(1383);
      match(SparqlParser::COMMA);
      setState(1384);
      expression(0);
    }
    setState(1387);
    match(SparqlParser::CLOSE_BRACE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SubStringExpressionContext ------------------------------------------------------------------

SparqlParser::SubStringExpressionContext::SubStringExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::SubStringExpressionContext::SUBSTR() {
  return getToken(SparqlParser::SUBSTR, 0);
}

tree::TerminalNode* SparqlParser::SubStringExpressionContext::OPEN_BRACE() {
  return getToken(SparqlParser::OPEN_BRACE, 0);
}

std::vector<SparqlParser::ExpressionContext *> SparqlParser::SubStringExpressionContext::expression() {
  return getRuleContexts<SparqlParser::ExpressionContext>();
}

SparqlParser::ExpressionContext* SparqlParser::SubStringExpressionContext::expression(size_t i) {
  return getRuleContext<SparqlParser::ExpressionContext>(i);
}

std::vector<tree::TerminalNode *> SparqlParser::SubStringExpressionContext::COMMA() {
  return getTokens(SparqlParser::COMMA);
}

tree::TerminalNode* SparqlParser::SubStringExpressionContext::COMMA(size_t i) {
  return getToken(SparqlParser::COMMA, i);
}

tree::TerminalNode* SparqlParser::SubStringExpressionContext::CLOSE_BRACE() {
  return getToken(SparqlParser::CLOSE_BRACE, 0);
}


size_t SparqlParser::SubStringExpressionContext::getRuleIndex() const {
  return SparqlParser::RuleSubStringExpression;
}


antlrcpp::Any SparqlParser::SubStringExpressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitSubStringExpression(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::SubStringExpressionContext* SparqlParser::subStringExpression() {
  SubStringExpressionContext *_localctx = _tracker.createInstance<SubStringExpressionContext>(_ctx, getState());
  enterRule(_localctx, 234, SparqlParser::RuleSubStringExpression);
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
    setState(1389);
    match(SparqlParser::SUBSTR);
    setState(1390);
    match(SparqlParser::OPEN_BRACE);
    setState(1391);
    expression(0);
    setState(1392);
    match(SparqlParser::COMMA);
    setState(1393);
    expression(0);
    setState(1396);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == SparqlParser::COMMA) {
      setState(1394);
      match(SparqlParser::COMMA);
      setState(1395);
      expression(0);
    }
    setState(1398);
    match(SparqlParser::CLOSE_BRACE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StrReplaceExpressionContext ------------------------------------------------------------------

SparqlParser::StrReplaceExpressionContext::StrReplaceExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::StrReplaceExpressionContext::REPLACE() {
  return getToken(SparqlParser::REPLACE, 0);
}

tree::TerminalNode* SparqlParser::StrReplaceExpressionContext::OPEN_BRACE() {
  return getToken(SparqlParser::OPEN_BRACE, 0);
}

std::vector<SparqlParser::ExpressionContext *> SparqlParser::StrReplaceExpressionContext::expression() {
  return getRuleContexts<SparqlParser::ExpressionContext>();
}

SparqlParser::ExpressionContext* SparqlParser::StrReplaceExpressionContext::expression(size_t i) {
  return getRuleContext<SparqlParser::ExpressionContext>(i);
}

std::vector<tree::TerminalNode *> SparqlParser::StrReplaceExpressionContext::COMMA() {
  return getTokens(SparqlParser::COMMA);
}

tree::TerminalNode* SparqlParser::StrReplaceExpressionContext::COMMA(size_t i) {
  return getToken(SparqlParser::COMMA, i);
}

tree::TerminalNode* SparqlParser::StrReplaceExpressionContext::CLOSE_BRACE() {
  return getToken(SparqlParser::CLOSE_BRACE, 0);
}


size_t SparqlParser::StrReplaceExpressionContext::getRuleIndex() const {
  return SparqlParser::RuleStrReplaceExpression;
}


antlrcpp::Any SparqlParser::StrReplaceExpressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitStrReplaceExpression(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::StrReplaceExpressionContext* SparqlParser::strReplaceExpression() {
  StrReplaceExpressionContext *_localctx = _tracker.createInstance<StrReplaceExpressionContext>(_ctx, getState());
  enterRule(_localctx, 236, SparqlParser::RuleStrReplaceExpression);
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
    setState(1400);
    match(SparqlParser::REPLACE);
    setState(1401);
    match(SparqlParser::OPEN_BRACE);
    setState(1402);
    expression(0);
    setState(1403);
    match(SparqlParser::COMMA);
    setState(1404);
    expression(0);
    setState(1405);
    match(SparqlParser::COMMA);
    setState(1406);
    expression(0);
    setState(1409);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == SparqlParser::COMMA) {
      setState(1407);
      match(SparqlParser::COMMA);
      setState(1408);
      expression(0);
    }
    setState(1411);
    match(SparqlParser::CLOSE_BRACE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExistsFunctionContext ------------------------------------------------------------------

SparqlParser::ExistsFunctionContext::ExistsFunctionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::ExistsFunctionContext::EXISTS() {
  return getToken(SparqlParser::EXISTS, 0);
}

SparqlParser::GroupGraphPatternContext* SparqlParser::ExistsFunctionContext::groupGraphPattern() {
  return getRuleContext<SparqlParser::GroupGraphPatternContext>(0);
}


size_t SparqlParser::ExistsFunctionContext::getRuleIndex() const {
  return SparqlParser::RuleExistsFunction;
}


antlrcpp::Any SparqlParser::ExistsFunctionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitExistsFunction(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::ExistsFunctionContext* SparqlParser::existsFunction() {
  ExistsFunctionContext *_localctx = _tracker.createInstance<ExistsFunctionContext>(_ctx, getState());
  enterRule(_localctx, 238, SparqlParser::RuleExistsFunction);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1413);
    match(SparqlParser::EXISTS);
    setState(1414);
    groupGraphPattern();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- NotExistsFunctionContext ------------------------------------------------------------------

SparqlParser::NotExistsFunctionContext::NotExistsFunctionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::NotExistsFunctionContext::NOT() {
  return getToken(SparqlParser::NOT, 0);
}

tree::TerminalNode* SparqlParser::NotExistsFunctionContext::EXISTS() {
  return getToken(SparqlParser::EXISTS, 0);
}

SparqlParser::GroupGraphPatternContext* SparqlParser::NotExistsFunctionContext::groupGraphPattern() {
  return getRuleContext<SparqlParser::GroupGraphPatternContext>(0);
}


size_t SparqlParser::NotExistsFunctionContext::getRuleIndex() const {
  return SparqlParser::RuleNotExistsFunction;
}


antlrcpp::Any SparqlParser::NotExistsFunctionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitNotExistsFunction(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::NotExistsFunctionContext* SparqlParser::notExistsFunction() {
  NotExistsFunctionContext *_localctx = _tracker.createInstance<NotExistsFunctionContext>(_ctx, getState());
  enterRule(_localctx, 240, SparqlParser::RuleNotExistsFunction);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1416);
    match(SparqlParser::NOT);
    setState(1417);
    match(SparqlParser::EXISTS);
    setState(1418);
    groupGraphPattern();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AggregateContext ------------------------------------------------------------------

SparqlParser::AggregateContext::AggregateContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::AggregateContext::COUNT() {
  return getToken(SparqlParser::COUNT, 0);
}

tree::TerminalNode* SparqlParser::AggregateContext::OPEN_BRACE() {
  return getToken(SparqlParser::OPEN_BRACE, 0);
}

tree::TerminalNode* SparqlParser::AggregateContext::CLOSE_BRACE() {
  return getToken(SparqlParser::CLOSE_BRACE, 0);
}

tree::TerminalNode* SparqlParser::AggregateContext::ASTERISK() {
  return getToken(SparqlParser::ASTERISK, 0);
}

SparqlParser::ExpressionContext* SparqlParser::AggregateContext::expression() {
  return getRuleContext<SparqlParser::ExpressionContext>(0);
}

tree::TerminalNode* SparqlParser::AggregateContext::DISTINCT() {
  return getToken(SparqlParser::DISTINCT, 0);
}

tree::TerminalNode* SparqlParser::AggregateContext::SUM() {
  return getToken(SparqlParser::SUM, 0);
}

tree::TerminalNode* SparqlParser::AggregateContext::MIN() {
  return getToken(SparqlParser::MIN, 0);
}

tree::TerminalNode* SparqlParser::AggregateContext::MAX() {
  return getToken(SparqlParser::MAX, 0);
}

tree::TerminalNode* SparqlParser::AggregateContext::AVG() {
  return getToken(SparqlParser::AVG, 0);
}

tree::TerminalNode* SparqlParser::AggregateContext::SAMPLE() {
  return getToken(SparqlParser::SAMPLE, 0);
}

tree::TerminalNode* SparqlParser::AggregateContext::GROUP_CONCAT() {
  return getToken(SparqlParser::GROUP_CONCAT, 0);
}

tree::TerminalNode* SparqlParser::AggregateContext::SEMICOLON() {
  return getToken(SparqlParser::SEMICOLON, 0);
}

tree::TerminalNode* SparqlParser::AggregateContext::SEPARATOR() {
  return getToken(SparqlParser::SEPARATOR, 0);
}

tree::TerminalNode* SparqlParser::AggregateContext::EQUAL() {
  return getToken(SparqlParser::EQUAL, 0);
}

SparqlParser::StringContext* SparqlParser::AggregateContext::string() {
  return getRuleContext<SparqlParser::StringContext>(0);
}


size_t SparqlParser::AggregateContext::getRuleIndex() const {
  return SparqlParser::RuleAggregate;
}


antlrcpp::Any SparqlParser::AggregateContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitAggregate(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::AggregateContext* SparqlParser::aggregate() {
  AggregateContext *_localctx = _tracker.createInstance<AggregateContext>(_ctx, getState());
  enterRule(_localctx, 242, SparqlParser::RuleAggregate);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(1484);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlParser::COUNT: {
        enterOuterAlt(_localctx, 1);
        setState(1420);
        match(SparqlParser::COUNT);
        setState(1421);
        match(SparqlParser::OPEN_BRACE);
        setState(1423);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == SparqlParser::DISTINCT) {
          setState(1422);
          match(SparqlParser::DISTINCT);
        }
        setState(1427);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 133, _ctx)) {
        case 1: {
          setState(1425);
          match(SparqlParser::ASTERISK);
          break;
        }

        case 2: {
          setState(1426);
          expression(0);
          break;
        }

        default:
          break;
        }
        setState(1429);
        match(SparqlParser::CLOSE_BRACE);
        break;
      }

      case SparqlParser::SUM: {
        enterOuterAlt(_localctx, 2);
        setState(1430);
        match(SparqlParser::SUM);
        setState(1431);
        match(SparqlParser::OPEN_BRACE);
        setState(1433);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == SparqlParser::DISTINCT) {
          setState(1432);
          match(SparqlParser::DISTINCT);
        }
        setState(1435);
        expression(0);
        setState(1436);
        match(SparqlParser::CLOSE_BRACE);
        break;
      }

      case SparqlParser::MIN: {
        enterOuterAlt(_localctx, 3);
        setState(1438);
        match(SparqlParser::MIN);
        setState(1439);
        match(SparqlParser::OPEN_BRACE);
        setState(1441);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == SparqlParser::DISTINCT) {
          setState(1440);
          match(SparqlParser::DISTINCT);
        }
        setState(1443);
        expression(0);
        setState(1444);
        match(SparqlParser::CLOSE_BRACE);
        break;
      }

      case SparqlParser::MAX: {
        enterOuterAlt(_localctx, 4);
        setState(1446);
        match(SparqlParser::MAX);
        setState(1447);
        match(SparqlParser::OPEN_BRACE);
        setState(1449);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == SparqlParser::DISTINCT) {
          setState(1448);
          match(SparqlParser::DISTINCT);
        }
        setState(1451);
        expression(0);
        setState(1452);
        match(SparqlParser::CLOSE_BRACE);
        break;
      }

      case SparqlParser::AVG: {
        enterOuterAlt(_localctx, 5);
        setState(1454);
        match(SparqlParser::AVG);
        setState(1455);
        match(SparqlParser::OPEN_BRACE);
        setState(1457);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == SparqlParser::DISTINCT) {
          setState(1456);
          match(SparqlParser::DISTINCT);
        }
        setState(1459);
        expression(0);
        setState(1460);
        match(SparqlParser::CLOSE_BRACE);
        break;
      }

      case SparqlParser::SAMPLE: {
        enterOuterAlt(_localctx, 6);
        setState(1462);
        match(SparqlParser::SAMPLE);
        setState(1463);
        match(SparqlParser::OPEN_BRACE);
        setState(1465);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == SparqlParser::DISTINCT) {
          setState(1464);
          match(SparqlParser::DISTINCT);
        }
        setState(1467);
        expression(0);
        setState(1468);
        match(SparqlParser::CLOSE_BRACE);
        break;
      }

      case SparqlParser::GROUP_CONCAT: {
        enterOuterAlt(_localctx, 7);
        setState(1470);
        match(SparqlParser::GROUP_CONCAT);
        setState(1471);
        match(SparqlParser::OPEN_BRACE);
        setState(1473);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == SparqlParser::DISTINCT) {
          setState(1472);
          match(SparqlParser::DISTINCT);
        }
        setState(1475);
        expression(0);
        setState(1480);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == SparqlParser::SEMICOLON) {
          setState(1476);
          match(SparqlParser::SEMICOLON);
          setState(1477);
          match(SparqlParser::SEPARATOR);
          setState(1478);
          match(SparqlParser::EQUAL);
          setState(1479);
          string();
        }
        setState(1482);
        match(SparqlParser::CLOSE_BRACE);
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

//----------------- IriRefOrFunctionContext ------------------------------------------------------------------

SparqlParser::IriRefOrFunctionContext::IriRefOrFunctionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::IriContext* SparqlParser::IriRefOrFunctionContext::iri() {
  return getRuleContext<SparqlParser::IriContext>(0);
}

SparqlParser::ArgListContext* SparqlParser::IriRefOrFunctionContext::argList() {
  return getRuleContext<SparqlParser::ArgListContext>(0);
}


size_t SparqlParser::IriRefOrFunctionContext::getRuleIndex() const {
  return SparqlParser::RuleIriRefOrFunction;
}


antlrcpp::Any SparqlParser::IriRefOrFunctionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitIriRefOrFunction(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::IriRefOrFunctionContext* SparqlParser::iriRefOrFunction() {
  IriRefOrFunctionContext *_localctx = _tracker.createInstance<IriRefOrFunctionContext>(_ctx, getState());
  enterRule(_localctx, 244, SparqlParser::RuleIriRefOrFunction);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1486);
    iri();
    setState(1488);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 142, _ctx)) {
    case 1: {
      setState(1487);
      argList();
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

//----------------- RdfLiteralContext ------------------------------------------------------------------

SparqlParser::RdfLiteralContext::RdfLiteralContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::StringContext* SparqlParser::RdfLiteralContext::string() {
  return getRuleContext<SparqlParser::StringContext>(0);
}

tree::TerminalNode* SparqlParser::RdfLiteralContext::LANGTAG() {
  return getToken(SparqlParser::LANGTAG, 0);
}

tree::TerminalNode* SparqlParser::RdfLiteralContext::REFERENCE() {
  return getToken(SparqlParser::REFERENCE, 0);
}

SparqlParser::IriContext* SparqlParser::RdfLiteralContext::iri() {
  return getRuleContext<SparqlParser::IriContext>(0);
}


size_t SparqlParser::RdfLiteralContext::getRuleIndex() const {
  return SparqlParser::RuleRdfLiteral;
}


antlrcpp::Any SparqlParser::RdfLiteralContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitRdfLiteral(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::RdfLiteralContext* SparqlParser::rdfLiteral() {
  RdfLiteralContext *_localctx = _tracker.createInstance<RdfLiteralContext>(_ctx, getState());
  enterRule(_localctx, 246, SparqlParser::RuleRdfLiteral);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1490);
    string();
    setState(1494);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 143, _ctx)) {
    case 1: {
      setState(1491);
      match(SparqlParser::LANGTAG);
      break;
    }

    case 2: {
      setState(1492);
      match(SparqlParser::REFERENCE);
      setState(1493);
      iri();
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

//----------------- NumericLiteralContext ------------------------------------------------------------------

SparqlParser::NumericLiteralContext::NumericLiteralContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::NumericLiteralUnsignedContext* SparqlParser::NumericLiteralContext::numericLiteralUnsigned() {
  return getRuleContext<SparqlParser::NumericLiteralUnsignedContext>(0);
}

SparqlParser::NumericLiteralPositiveContext* SparqlParser::NumericLiteralContext::numericLiteralPositive() {
  return getRuleContext<SparqlParser::NumericLiteralPositiveContext>(0);
}

SparqlParser::NumericLiteralNegativeContext* SparqlParser::NumericLiteralContext::numericLiteralNegative() {
  return getRuleContext<SparqlParser::NumericLiteralNegativeContext>(0);
}


size_t SparqlParser::NumericLiteralContext::getRuleIndex() const {
  return SparqlParser::RuleNumericLiteral;
}


antlrcpp::Any SparqlParser::NumericLiteralContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitNumericLiteral(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::NumericLiteralContext* SparqlParser::numericLiteral() {
  NumericLiteralContext *_localctx = _tracker.createInstance<NumericLiteralContext>(_ctx, getState());
  enterRule(_localctx, 248, SparqlParser::RuleNumericLiteral);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(1499);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlParser::INTEGER:
      case SparqlParser::DECIMAL:
      case SparqlParser::DOUBLE: {
        enterOuterAlt(_localctx, 1);
        setState(1496);
        numericLiteralUnsigned();
        break;
      }

      case SparqlParser::INTEGER_POSITIVE:
      case SparqlParser::DECIMAL_POSITIVE:
      case SparqlParser::DOUBLE_POSITIVE: {
        enterOuterAlt(_localctx, 2);
        setState(1497);
        numericLiteralPositive();
        break;
      }

      case SparqlParser::INTEGER_NEGATIVE:
      case SparqlParser::DECIMAL_NEGATIVE:
      case SparqlParser::DOUBLE_NEGATIVE: {
        enterOuterAlt(_localctx, 3);
        setState(1498);
        numericLiteralNegative();
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

//----------------- NumericLiteralUnsignedContext ------------------------------------------------------------------

SparqlParser::NumericLiteralUnsignedContext::NumericLiteralUnsignedContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::NumericLiteralUnsignedContext::INTEGER() {
  return getToken(SparqlParser::INTEGER, 0);
}

tree::TerminalNode* SparqlParser::NumericLiteralUnsignedContext::DECIMAL() {
  return getToken(SparqlParser::DECIMAL, 0);
}

tree::TerminalNode* SparqlParser::NumericLiteralUnsignedContext::DOUBLE() {
  return getToken(SparqlParser::DOUBLE, 0);
}


size_t SparqlParser::NumericLiteralUnsignedContext::getRuleIndex() const {
  return SparqlParser::RuleNumericLiteralUnsigned;
}


antlrcpp::Any SparqlParser::NumericLiteralUnsignedContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitNumericLiteralUnsigned(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::NumericLiteralUnsignedContext* SparqlParser::numericLiteralUnsigned() {
  NumericLiteralUnsignedContext *_localctx = _tracker.createInstance<NumericLiteralUnsignedContext>(_ctx, getState());
  enterRule(_localctx, 250, SparqlParser::RuleNumericLiteralUnsigned);
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
    setState(1501);
    _la = _input->LA(1);
    if (!(((((_la - 122) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 122)) & ((1ULL << (SparqlParser::INTEGER - 122))
      | (1ULL << (SparqlParser::DECIMAL - 122))
      | (1ULL << (SparqlParser::DOUBLE - 122)))) != 0))) {
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

//----------------- NumericLiteralPositiveContext ------------------------------------------------------------------

SparqlParser::NumericLiteralPositiveContext::NumericLiteralPositiveContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::NumericLiteralPositiveContext::INTEGER_POSITIVE() {
  return getToken(SparqlParser::INTEGER_POSITIVE, 0);
}

tree::TerminalNode* SparqlParser::NumericLiteralPositiveContext::DECIMAL_POSITIVE() {
  return getToken(SparqlParser::DECIMAL_POSITIVE, 0);
}

tree::TerminalNode* SparqlParser::NumericLiteralPositiveContext::DOUBLE_POSITIVE() {
  return getToken(SparqlParser::DOUBLE_POSITIVE, 0);
}


size_t SparqlParser::NumericLiteralPositiveContext::getRuleIndex() const {
  return SparqlParser::RuleNumericLiteralPositive;
}


antlrcpp::Any SparqlParser::NumericLiteralPositiveContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitNumericLiteralPositive(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::NumericLiteralPositiveContext* SparqlParser::numericLiteralPositive() {
  NumericLiteralPositiveContext *_localctx = _tracker.createInstance<NumericLiteralPositiveContext>(_ctx, getState());
  enterRule(_localctx, 252, SparqlParser::RuleNumericLiteralPositive);
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
    setState(1503);
    _la = _input->LA(1);
    if (!(((((_la - 125) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 125)) & ((1ULL << (SparqlParser::INTEGER_POSITIVE - 125))
      | (1ULL << (SparqlParser::DECIMAL_POSITIVE - 125))
      | (1ULL << (SparqlParser::DOUBLE_POSITIVE - 125)))) != 0))) {
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

//----------------- NumericLiteralNegativeContext ------------------------------------------------------------------

SparqlParser::NumericLiteralNegativeContext::NumericLiteralNegativeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::NumericLiteralNegativeContext::INTEGER_NEGATIVE() {
  return getToken(SparqlParser::INTEGER_NEGATIVE, 0);
}

tree::TerminalNode* SparqlParser::NumericLiteralNegativeContext::DECIMAL_NEGATIVE() {
  return getToken(SparqlParser::DECIMAL_NEGATIVE, 0);
}

tree::TerminalNode* SparqlParser::NumericLiteralNegativeContext::DOUBLE_NEGATIVE() {
  return getToken(SparqlParser::DOUBLE_NEGATIVE, 0);
}


size_t SparqlParser::NumericLiteralNegativeContext::getRuleIndex() const {
  return SparqlParser::RuleNumericLiteralNegative;
}


antlrcpp::Any SparqlParser::NumericLiteralNegativeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitNumericLiteralNegative(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::NumericLiteralNegativeContext* SparqlParser::numericLiteralNegative() {
  NumericLiteralNegativeContext *_localctx = _tracker.createInstance<NumericLiteralNegativeContext>(_ctx, getState());
  enterRule(_localctx, 254, SparqlParser::RuleNumericLiteralNegative);
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
    setState(1505);
    _la = _input->LA(1);
    if (!(((((_la - 128) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 128)) & ((1ULL << (SparqlParser::INTEGER_NEGATIVE - 128))
      | (1ULL << (SparqlParser::DECIMAL_NEGATIVE - 128))
      | (1ULL << (SparqlParser::DOUBLE_NEGATIVE - 128)))) != 0))) {
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

//----------------- BooleanLiteralContext ------------------------------------------------------------------

SparqlParser::BooleanLiteralContext::BooleanLiteralContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::BooleanLiteralContext::TRUE() {
  return getToken(SparqlParser::TRUE, 0);
}

tree::TerminalNode* SparqlParser::BooleanLiteralContext::FALSE() {
  return getToken(SparqlParser::FALSE, 0);
}


size_t SparqlParser::BooleanLiteralContext::getRuleIndex() const {
  return SparqlParser::RuleBooleanLiteral;
}


antlrcpp::Any SparqlParser::BooleanLiteralContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitBooleanLiteral(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::BooleanLiteralContext* SparqlParser::booleanLiteral() {
  BooleanLiteralContext *_localctx = _tracker.createInstance<BooleanLiteralContext>(_ctx, getState());
  enterRule(_localctx, 256, SparqlParser::RuleBooleanLiteral);
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
    setState(1507);
    _la = _input->LA(1);
    if (!(_la == SparqlParser::TRUE

    || _la == SparqlParser::FALSE)) {
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

//----------------- StringContext ------------------------------------------------------------------

SparqlParser::StringContext::StringContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::StringContext::STRING_LITERAL1() {
  return getToken(SparqlParser::STRING_LITERAL1, 0);
}

tree::TerminalNode* SparqlParser::StringContext::STRING_LITERAL2() {
  return getToken(SparqlParser::STRING_LITERAL2, 0);
}

tree::TerminalNode* SparqlParser::StringContext::STRING_LITERAL_LONG1() {
  return getToken(SparqlParser::STRING_LITERAL_LONG1, 0);
}

tree::TerminalNode* SparqlParser::StringContext::STRING_LITERAL_LONG2() {
  return getToken(SparqlParser::STRING_LITERAL_LONG2, 0);
}


size_t SparqlParser::StringContext::getRuleIndex() const {
  return SparqlParser::RuleString;
}


antlrcpp::Any SparqlParser::StringContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitString(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::StringContext* SparqlParser::string() {
  StringContext *_localctx = _tracker.createInstance<StringContext>(_ctx, getState());
  enterRule(_localctx, 258, SparqlParser::RuleString);
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
    setState(1509);
    _la = _input->LA(1);
    if (!(((((_la - 131) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 131)) & ((1ULL << (SparqlParser::STRING_LITERAL1 - 131))
      | (1ULL << (SparqlParser::STRING_LITERAL2 - 131))
      | (1ULL << (SparqlParser::STRING_LITERAL_LONG1 - 131))
      | (1ULL << (SparqlParser::STRING_LITERAL_LONG2 - 131)))) != 0))) {
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

//----------------- IriContext ------------------------------------------------------------------

SparqlParser::IriContext::IriContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::IriContext::IRIREF() {
  return getToken(SparqlParser::IRIREF, 0);
}

SparqlParser::PrefixedNameContext* SparqlParser::IriContext::prefixedName() {
  return getRuleContext<SparqlParser::PrefixedNameContext>(0);
}


size_t SparqlParser::IriContext::getRuleIndex() const {
  return SparqlParser::RuleIri;
}


antlrcpp::Any SparqlParser::IriContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitIri(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::IriContext* SparqlParser::iri() {
  IriContext *_localctx = _tracker.createInstance<IriContext>(_ctx, getState());
  enterRule(_localctx, 260, SparqlParser::RuleIri);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(1513);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlParser::IRIREF: {
        enterOuterAlt(_localctx, 1);
        setState(1511);
        match(SparqlParser::IRIREF);
        break;
      }

      case SparqlParser::PNAME_NS:
      case SparqlParser::PNAME_LN: {
        enterOuterAlt(_localctx, 2);
        setState(1512);
        prefixedName();
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

//----------------- PrefixedNameContext ------------------------------------------------------------------

SparqlParser::PrefixedNameContext::PrefixedNameContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::PrefixedNameContext::PNAME_LN() {
  return getToken(SparqlParser::PNAME_LN, 0);
}

tree::TerminalNode* SparqlParser::PrefixedNameContext::PNAME_NS() {
  return getToken(SparqlParser::PNAME_NS, 0);
}


size_t SparqlParser::PrefixedNameContext::getRuleIndex() const {
  return SparqlParser::RulePrefixedName;
}


antlrcpp::Any SparqlParser::PrefixedNameContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitPrefixedName(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::PrefixedNameContext* SparqlParser::prefixedName() {
  PrefixedNameContext *_localctx = _tracker.createInstance<PrefixedNameContext>(_ctx, getState());
  enterRule(_localctx, 262, SparqlParser::RulePrefixedName);
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
    setState(1515);
    _la = _input->LA(1);
    if (!(_la == SparqlParser::PNAME_NS

    || _la == SparqlParser::PNAME_LN)) {
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

//----------------- BlankNodeContext ------------------------------------------------------------------

SparqlParser::BlankNodeContext::BlankNodeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::BlankNodeContext::BLANK_NODE_LABEL() {
  return getToken(SparqlParser::BLANK_NODE_LABEL, 0);
}

SparqlParser::AnonContext* SparqlParser::BlankNodeContext::anon() {
  return getRuleContext<SparqlParser::AnonContext>(0);
}


size_t SparqlParser::BlankNodeContext::getRuleIndex() const {
  return SparqlParser::RuleBlankNode;
}


antlrcpp::Any SparqlParser::BlankNodeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitBlankNode(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::BlankNodeContext* SparqlParser::blankNode() {
  BlankNodeContext *_localctx = _tracker.createInstance<BlankNodeContext>(_ctx, getState());
  enterRule(_localctx, 264, SparqlParser::RuleBlankNode);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(1519);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlParser::BLANK_NODE_LABEL: {
        enterOuterAlt(_localctx, 1);
        setState(1517);
        match(SparqlParser::BLANK_NODE_LABEL);
        break;
      }

      case SparqlParser::OPEN_SQUARE_BRACKET: {
        enterOuterAlt(_localctx, 2);
        setState(1518);
        anon();
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

//----------------- AnonContext ------------------------------------------------------------------

SparqlParser::AnonContext::AnonContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::AnonContext::OPEN_SQUARE_BRACKET() {
  return getToken(SparqlParser::OPEN_SQUARE_BRACKET, 0);
}

tree::TerminalNode* SparqlParser::AnonContext::CLOSE_SQUARE_BRACKET() {
  return getToken(SparqlParser::CLOSE_SQUARE_BRACKET, 0);
}


size_t SparqlParser::AnonContext::getRuleIndex() const {
  return SparqlParser::RuleAnon;
}


antlrcpp::Any SparqlParser::AnonContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlParserVisitor*>(visitor))
    return parserVisitor->visitAnon(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::AnonContext* SparqlParser::anon() {
  AnonContext *_localctx = _tracker.createInstance<AnonContext>(_ctx, getState());
  enterRule(_localctx, 266, SparqlParser::RuleAnon);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1521);
    match(SparqlParser::OPEN_SQUARE_BRACKET);
    setState(1522);
    match(SparqlParser::CLOSE_SQUARE_BRACKET);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

bool SparqlParser::sempred(RuleContext *context, size_t ruleIndex, size_t predicateIndex) {
  switch (ruleIndex) {
    case 111: return expressionSempred(antlrcpp::downCast<ExpressionContext *>(context), predicateIndex);

  default:
    break;
  }
  return true;
}

bool SparqlParser::expressionSempred(ExpressionContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 0: return precpred(_ctx, 7);
    case 1: return precpred(_ctx, 6);
    case 2: return precpred(_ctx, 3);
    case 3: return precpred(_ctx, 5);
    case 4: return precpred(_ctx, 4);
    case 5: return precpred(_ctx, 2);
    case 6: return precpred(_ctx, 1);

  default:
    break;
  }
  return true;
}

// Static vars and initialization.
std::vector<dfa::DFA> SparqlParser::_decisionToDFA;
atn::PredictionContextCache SparqlParser::_sharedContextCache;

// We own the ATN which in turn owns the ATN states.
atn::ATN SparqlParser::_atn;
std::vector<uint16_t> SparqlParser::_serializedATN;

std::vector<std::string> SparqlParser::_ruleNames = {
  "query", "prologue", "baseDecl", "prefixDecl", "selectQuery", "subSelect", 
  "selectClause", "selectModifier", "selectVariables", "constructQuery", 
  "describeQuery", "askQuery", "datasetClause", "whereClause", "solutionModifier", 
  "groupClause", "groupCondition", "havingClause", "havingCondition", "orderClause", 
  "orderCondition", "limitOffsetClauses", "limitClause", "offsetClause", 
  "valuesClause", "updateCommand", "update", "load", "clear", "drop", "create", 
  "add", "move", "copy", "insertData", "deleteData", "deleteWhere", "modify", 
  "deleteClause", "insertClause", "usingClause", "graphOrDefault", "graphRef", 
  "graphRefAll", "quadPattern", "quadData", "quads", "quadsDetails", "quadsNotTriples", 
  "triplesTemplate", "groupGraphPattern", "groupGraphPatternSub", "groupGraphPatternSubList", 
  "triplesBlock", "graphPatternNotTriples", "optionalGraphPattern", "graphGraphPattern", 
  "serviceGraphPattern", "bind", "inlineData", "dataBlock", "inlineDataOneVar", 
  "inlineDataFull", "dataBlockValues", "dataBlockValue", "minusGraphPattern", 
  "groupOrUnionGraphPattern", "filter", "constraint", "functionCall", "argList", 
  "expressionList", "constructTemplate", "constructTriples", "triplesSameSubject", 
  "propertyList", "propertyListNotEmpty", "verb", "objectList", "object", 
  "triplesSameSubjectPath", "propertyListPath", "propertyListPathNotEmpty", 
  "propertyListPathNotEmptyList", "verbPath", "verbSimple", "objectListPath", 
  "objectPath", "path", "pathAlternative", "pathSequence", "pathElt", "pathEltOrInverse", 
  "pathMod", "pathPrimary", "pathNegatedPropertySet", "pathOneInPropertySet", 
  "integer", "triplesNode", "blankNodePropertyList", "triplesNodePath", 
  "blankNodePropertyListPath", "collection", "collectionPath", "graphNode", 
  "graphNodePath", "varOrTerm", "varOrIRI", "var", "graphTerm", "nil", "expression", 
  "unaryLiteralExpression", "unaryExpression", "primaryExpression", "builtInCall", 
  "regexExpression", "subStringExpression", "strReplaceExpression", "existsFunction", 
  "notExistsFunction", "aggregate", "iriRefOrFunction", "rdfLiteral", "numericLiteral", 
  "numericLiteralUnsigned", "numericLiteralPositive", "numericLiteralNegative", 
  "booleanLiteral", "string", "iri", "prefixedName", "blankNode", "anon"
};

std::vector<std::string> SparqlParser::_literalNames = {
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
  "", "", "", "", "", "", "'a'", "", "", "", "", "", "", "", "", "", "", 
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
  "", "", "", "", "", "", "", "", "", "", "", "'^^'", "'<='", "'>='", "'!='", 
  "'&&'", "'||'", "'^'", "'('", "')'", "'{'", "'}'", "'['", "']'", "';'", 
  "'.'", "'+'", "'-'", "'*'", "'\u003F'", "','", "'!'", "'/'", "'='", "'<'", 
  "'>'", "'|'"
};

std::vector<std::string> SparqlParser::_symbolicNames = {
  "", "WS", "BASE", "PREFIX", "SELECT", "DISTINCT", "REDUCED", "CONSTRUCT", 
  "DESCRIBE", "ASK", "FROM", "NAMED", "WHERE", "ORDER", "BY", "ASC", "DESC", 
  "LIMIT", "OFFSET", "VALUES", "OPTIONAL", "GRAPH", "UNION", "FILTER", "A", 
  "STR", "LANG", "LANGMATCHES", "DATATYPE", "BOUND", "SAMETERM", "ISIRI", 
  "ISURI", "ISBLANK", "ISLITERAL", "REGEX", "SUBSTR", "TRUE", "FALSE", "LOAD", 
  "CLEAR", "DROP", "ADD", "MOVE", "COPY", "CREATE", "DELETE", "INSERT", 
  "USING", "SILENT", "DEFAULT", "ALL", "DATA", "WITH", "INTO", "TO", "AS", 
  "GROUP", "HAVING", "UNDEF", "BINDINGS", "SERVICE", "BIND", "MINUS", "IRI", 
  "URI", "BNODE", "RAND", "ABS", "CEIL", "FLOOR", "ROUND", "CONCAT", "STRLEN", 
  "UCASE", "LCASE", "ENCODE_FOR_URI", "CONTAINS", "STRSTARTS", "STRENDS", 
  "STRBEFORE", "STRAFTER", "REPLACE", "YEAR", "MONTH", "DAY", "HOURS", "MINUTES", 
  "SECONDS", "TIMEZONE", "TZ", "NOW", "UUID", "STRUUID", "MD5", "SHA1", 
  "SHA256", "SHA384", "SHA512", "COALESCE", "IF", "STRLANG", "STRDT", "ISNUMERIC", 
  "COUNT", "SUM", "MIN", "MAX", "AVG", "SAMPLE", "GROUP_CONCAT", "NOT", 
  "IN", "EXISTS", "SEPARATOR", "IRIREF", "PNAME_NS", "PNAME_LN", "BLANK_NODE_LABEL", 
  "VAR1", "VAR2", "LANGTAG", "INTEGER", "DECIMAL", "DOUBLE", "INTEGER_POSITIVE", 
  "DECIMAL_POSITIVE", "DOUBLE_POSITIVE", "INTEGER_NEGATIVE", "DECIMAL_NEGATIVE", 
  "DOUBLE_NEGATIVE", "STRING_LITERAL1", "STRING_LITERAL2", "STRING_LITERAL_LONG1", 
  "STRING_LITERAL_LONG2", "COMMENT", "REFERENCE", "LESS_EQUAL", "GREATER_EQUAL", 
  "NOT_EQUAL", "AND", "OR", "INVERSE", "OPEN_BRACE", "CLOSE_BRACE", "OPEN_CURLY_BRACE", 
  "CLOSE_CURLY_BRACE", "OPEN_SQUARE_BRACKET", "CLOSE_SQUARE_BRACKET", "SEMICOLON", 
  "DOT", "PLUS_SIGN", "MINUS_SIGN", "ASTERISK", "QUESTION_MARK", "COMMA", 
  "NEGATION", "DIVIDE", "EQUAL", "LESS", "GREATER", "PIPE", "ANY"
};

dfa::Vocabulary SparqlParser::_vocabulary(_literalNames, _symbolicNames);

std::vector<std::string> SparqlParser::_tokenNames;

SparqlParser::Initializer::Initializer() {
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
       0x3, 0xa4, 0x5f7, 0x4, 0x2, 0x9, 0x2, 0x4, 0x3, 0x9, 0x3, 0x4, 0x4, 
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
       0x9, 0x35, 0x4, 0x36, 0x9, 0x36, 0x4, 0x37, 0x9, 0x37, 0x4, 0x38, 
       0x9, 0x38, 0x4, 0x39, 0x9, 0x39, 0x4, 0x3a, 0x9, 0x3a, 0x4, 0x3b, 
       0x9, 0x3b, 0x4, 0x3c, 0x9, 0x3c, 0x4, 0x3d, 0x9, 0x3d, 0x4, 0x3e, 
       0x9, 0x3e, 0x4, 0x3f, 0x9, 0x3f, 0x4, 0x40, 0x9, 0x40, 0x4, 0x41, 
       0x9, 0x41, 0x4, 0x42, 0x9, 0x42, 0x4, 0x43, 0x9, 0x43, 0x4, 0x44, 
       0x9, 0x44, 0x4, 0x45, 0x9, 0x45, 0x4, 0x46, 0x9, 0x46, 0x4, 0x47, 
       0x9, 0x47, 0x4, 0x48, 0x9, 0x48, 0x4, 0x49, 0x9, 0x49, 0x4, 0x4a, 
       0x9, 0x4a, 0x4, 0x4b, 0x9, 0x4b, 0x4, 0x4c, 0x9, 0x4c, 0x4, 0x4d, 
       0x9, 0x4d, 0x4, 0x4e, 0x9, 0x4e, 0x4, 0x4f, 0x9, 0x4f, 0x4, 0x50, 
       0x9, 0x50, 0x4, 0x51, 0x9, 0x51, 0x4, 0x52, 0x9, 0x52, 0x4, 0x53, 
       0x9, 0x53, 0x4, 0x54, 0x9, 0x54, 0x4, 0x55, 0x9, 0x55, 0x4, 0x56, 
       0x9, 0x56, 0x4, 0x57, 0x9, 0x57, 0x4, 0x58, 0x9, 0x58, 0x4, 0x59, 
       0x9, 0x59, 0x4, 0x5a, 0x9, 0x5a, 0x4, 0x5b, 0x9, 0x5b, 0x4, 0x5c, 
       0x9, 0x5c, 0x4, 0x5d, 0x9, 0x5d, 0x4, 0x5e, 0x9, 0x5e, 0x4, 0x5f, 
       0x9, 0x5f, 0x4, 0x60, 0x9, 0x60, 0x4, 0x61, 0x9, 0x61, 0x4, 0x62, 
       0x9, 0x62, 0x4, 0x63, 0x9, 0x63, 0x4, 0x64, 0x9, 0x64, 0x4, 0x65, 
       0x9, 0x65, 0x4, 0x66, 0x9, 0x66, 0x4, 0x67, 0x9, 0x67, 0x4, 0x68, 
       0x9, 0x68, 0x4, 0x69, 0x9, 0x69, 0x4, 0x6a, 0x9, 0x6a, 0x4, 0x6b, 
       0x9, 0x6b, 0x4, 0x6c, 0x9, 0x6c, 0x4, 0x6d, 0x9, 0x6d, 0x4, 0x6e, 
       0x9, 0x6e, 0x4, 0x6f, 0x9, 0x6f, 0x4, 0x70, 0x9, 0x70, 0x4, 0x71, 
       0x9, 0x71, 0x4, 0x72, 0x9, 0x72, 0x4, 0x73, 0x9, 0x73, 0x4, 0x74, 
       0x9, 0x74, 0x4, 0x75, 0x9, 0x75, 0x4, 0x76, 0x9, 0x76, 0x4, 0x77, 
       0x9, 0x77, 0x4, 0x78, 0x9, 0x78, 0x4, 0x79, 0x9, 0x79, 0x4, 0x7a, 
       0x9, 0x7a, 0x4, 0x7b, 0x9, 0x7b, 0x4, 0x7c, 0x9, 0x7c, 0x4, 0x7d, 
       0x9, 0x7d, 0x4, 0x7e, 0x9, 0x7e, 0x4, 0x7f, 0x9, 0x7f, 0x4, 0x80, 
       0x9, 0x80, 0x4, 0x81, 0x9, 0x81, 0x4, 0x82, 0x9, 0x82, 0x4, 0x83, 
       0x9, 0x83, 0x4, 0x84, 0x9, 0x84, 0x4, 0x85, 0x9, 0x85, 0x4, 0x86, 
       0x9, 0x86, 0x4, 0x87, 0x9, 0x87, 0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 
       0x2, 0x3, 0x2, 0x5, 0x2, 0x114, 0xa, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 
       0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 0x5, 0x2, 0x11c, 0xa, 0x2, 0x3, 
       0x3, 0x3, 0x3, 0x7, 0x3, 0x120, 0xa, 0x3, 0xc, 0x3, 0xe, 0x3, 0x123, 
       0xb, 0x3, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x5, 0x3, 0x5, 0x3, 
       0x5, 0x3, 0x5, 0x3, 0x6, 0x3, 0x6, 0x7, 0x6, 0x12e, 0xa, 0x6, 0xc, 
       0x6, 0xe, 0x6, 0x131, 0xb, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 
       0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x8, 0x3, 0x8, 
       0x5, 0x8, 0x13d, 0xa, 0x8, 0x3, 0x8, 0x6, 0x8, 0x140, 0xa, 0x8, 0xd, 
       0x8, 0xe, 0x8, 0x141, 0x3, 0x8, 0x5, 0x8, 0x145, 0xa, 0x8, 0x3, 0x9, 
       0x3, 0x9, 0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 0x3, 
       0xa, 0x3, 0xa, 0x5, 0xa, 0x150, 0xa, 0xa, 0x3, 0xb, 0x3, 0xb, 0x3, 
       0xb, 0x7, 0xb, 0x155, 0xa, 0xb, 0xc, 0xb, 0xe, 0xb, 0x158, 0xb, 0xb, 
       0x3, 0xb, 0x3, 0xb, 0x3, 0xb, 0x3, 0xb, 0x7, 0xb, 0x15e, 0xa, 0xb, 
       0xc, 0xb, 0xe, 0xb, 0x161, 0xb, 0xb, 0x3, 0xb, 0x3, 0xb, 0x3, 0xb, 
       0x5, 0xb, 0x166, 0xa, 0xb, 0x3, 0xb, 0x3, 0xb, 0x5, 0xb, 0x16a, 0xa, 
       0xb, 0x3, 0xc, 0x3, 0xc, 0x6, 0xc, 0x16e, 0xa, 0xc, 0xd, 0xc, 0xe, 
       0xc, 0x16f, 0x3, 0xc, 0x5, 0xc, 0x173, 0xa, 0xc, 0x3, 0xc, 0x7, 0xc, 
       0x176, 0xa, 0xc, 0xc, 0xc, 0xe, 0xc, 0x179, 0xb, 0xc, 0x3, 0xc, 0x5, 
       0xc, 0x17c, 0xa, 0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0xd, 0x3, 0xd, 0x7, 
       0xd, 0x182, 0xa, 0xd, 0xc, 0xd, 0xe, 0xd, 0x185, 0xb, 0xd, 0x3, 0xd, 
       0x3, 0xd, 0x3, 0xd, 0x3, 0xe, 0x3, 0xe, 0x5, 0xe, 0x18c, 0xa, 0xe, 
       0x3, 0xe, 0x3, 0xe, 0x3, 0xf, 0x5, 0xf, 0x191, 0xa, 0xf, 0x3, 0xf, 
       0x3, 0xf, 0x3, 0x10, 0x5, 0x10, 0x196, 0xa, 0x10, 0x3, 0x10, 0x5, 
       0x10, 0x199, 0xa, 0x10, 0x3, 0x10, 0x5, 0x10, 0x19c, 0xa, 0x10, 0x3, 
       0x10, 0x5, 0x10, 0x19f, 0xa, 0x10, 0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 
       0x6, 0x11, 0x1a4, 0xa, 0x11, 0xd, 0x11, 0xe, 0x11, 0x1a5, 0x3, 0x12, 
       0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x5, 0x12, 
       0x1ae, 0xa, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x5, 0x12, 0x1b3, 
       0xa, 0x12, 0x3, 0x13, 0x3, 0x13, 0x6, 0x13, 0x1b7, 0xa, 0x13, 0xd, 
       0x13, 0xe, 0x13, 0x1b8, 0x3, 0x14, 0x3, 0x14, 0x3, 0x15, 0x3, 0x15, 
       0x3, 0x15, 0x6, 0x15, 0x1c0, 0xa, 0x15, 0xd, 0x15, 0xe, 0x15, 0x1c1, 
       0x3, 0x16, 0x3, 0x16, 0x3, 0x16, 0x3, 0x16, 0x3, 0x16, 0x3, 0x16, 
       0x3, 0x16, 0x5, 0x16, 0x1cb, 0xa, 0x16, 0x3, 0x17, 0x3, 0x17, 0x5, 
       0x17, 0x1cf, 0xa, 0x17, 0x3, 0x17, 0x3, 0x17, 0x5, 0x17, 0x1d3, 0xa, 
       0x17, 0x5, 0x17, 0x1d5, 0xa, 0x17, 0x3, 0x18, 0x3, 0x18, 0x3, 0x18, 
       0x3, 0x19, 0x3, 0x19, 0x3, 0x19, 0x3, 0x1a, 0x3, 0x1a, 0x5, 0x1a, 
       0x1df, 0xa, 0x1a, 0x3, 0x1b, 0x3, 0x1b, 0x3, 0x1b, 0x3, 0x1b, 0x3, 
       0x1b, 0x3, 0x1b, 0x7, 0x1b, 0x1e7, 0xa, 0x1b, 0xc, 0x1b, 0xe, 0x1b, 
       0x1ea, 0xb, 0x1b, 0x3, 0x1b, 0x3, 0x1b, 0x5, 0x1b, 0x1ee, 0xa, 0x1b, 
       0x5, 0x1b, 0x1f0, 0xa, 0x1b, 0x3, 0x1c, 0x3, 0x1c, 0x3, 0x1c, 0x3, 
       0x1c, 0x3, 0x1c, 0x3, 0x1c, 0x3, 0x1c, 0x3, 0x1c, 0x3, 0x1c, 0x3, 
       0x1c, 0x3, 0x1c, 0x5, 0x1c, 0x1fd, 0xa, 0x1c, 0x3, 0x1d, 0x3, 0x1d, 
       0x5, 0x1d, 0x201, 0xa, 0x1d, 0x3, 0x1d, 0x3, 0x1d, 0x3, 0x1d, 0x5, 
       0x1d, 0x206, 0xa, 0x1d, 0x3, 0x1e, 0x3, 0x1e, 0x5, 0x1e, 0x20a, 0xa, 
       0x1e, 0x3, 0x1e, 0x3, 0x1e, 0x3, 0x1f, 0x3, 0x1f, 0x5, 0x1f, 0x210, 
       0xa, 0x1f, 0x3, 0x1f, 0x3, 0x1f, 0x3, 0x20, 0x3, 0x20, 0x5, 0x20, 
       0x216, 0xa, 0x20, 0x3, 0x20, 0x3, 0x20, 0x3, 0x21, 0x3, 0x21, 0x5, 
       0x21, 0x21c, 0xa, 0x21, 0x3, 0x21, 0x3, 0x21, 0x3, 0x21, 0x3, 0x21, 
       0x3, 0x22, 0x3, 0x22, 0x5, 0x22, 0x224, 0xa, 0x22, 0x3, 0x22, 0x3, 
       0x22, 0x3, 0x22, 0x3, 0x22, 0x3, 0x23, 0x3, 0x23, 0x5, 0x23, 0x22c, 
       0xa, 0x23, 0x3, 0x23, 0x3, 0x23, 0x3, 0x23, 0x3, 0x23, 0x3, 0x24, 
       0x3, 0x24, 0x3, 0x24, 0x3, 0x24, 0x3, 0x25, 0x3, 0x25, 0x3, 0x25, 
       0x3, 0x25, 0x3, 0x26, 0x3, 0x26, 0x3, 0x26, 0x3, 0x26, 0x3, 0x27, 
       0x3, 0x27, 0x5, 0x27, 0x240, 0xa, 0x27, 0x3, 0x27, 0x3, 0x27, 0x5, 
       0x27, 0x244, 0xa, 0x27, 0x3, 0x27, 0x5, 0x27, 0x247, 0xa, 0x27, 0x3, 
       0x27, 0x7, 0x27, 0x24a, 0xa, 0x27, 0xc, 0x27, 0xe, 0x27, 0x24d, 0xb, 
       0x27, 0x3, 0x27, 0x3, 0x27, 0x3, 0x27, 0x3, 0x28, 0x3, 0x28, 0x3, 
       0x28, 0x3, 0x29, 0x3, 0x29, 0x3, 0x29, 0x3, 0x2a, 0x3, 0x2a, 0x5, 
       0x2a, 0x25a, 0xa, 0x2a, 0x3, 0x2a, 0x3, 0x2a, 0x3, 0x2b, 0x3, 0x2b, 
       0x5, 0x2b, 0x260, 0xa, 0x2b, 0x3, 0x2b, 0x5, 0x2b, 0x263, 0xa, 0x2b, 
       0x3, 0x2c, 0x3, 0x2c, 0x3, 0x2c, 0x3, 0x2d, 0x3, 0x2d, 0x3, 0x2d, 
       0x3, 0x2d, 0x5, 0x2d, 0x26c, 0xa, 0x2d, 0x3, 0x2e, 0x3, 0x2e, 0x3, 
       0x2e, 0x3, 0x2e, 0x3, 0x2f, 0x3, 0x2f, 0x3, 0x2f, 0x3, 0x2f, 0x3, 
       0x30, 0x5, 0x30, 0x277, 0xa, 0x30, 0x3, 0x30, 0x7, 0x30, 0x27a, 0xa, 
       0x30, 0xc, 0x30, 0xe, 0x30, 0x27d, 0xb, 0x30, 0x3, 0x31, 0x3, 0x31, 
       0x5, 0x31, 0x281, 0xa, 0x31, 0x3, 0x31, 0x5, 0x31, 0x284, 0xa, 0x31, 
       0x3, 0x32, 0x3, 0x32, 0x3, 0x32, 0x3, 0x32, 0x5, 0x32, 0x28a, 0xa, 
       0x32, 0x3, 0x32, 0x3, 0x32, 0x3, 0x33, 0x3, 0x33, 0x3, 0x33, 0x5, 
       0x33, 0x291, 0xa, 0x33, 0x7, 0x33, 0x293, 0xa, 0x33, 0xc, 0x33, 0xe, 
       0x33, 0x296, 0xb, 0x33, 0x3, 0x34, 0x3, 0x34, 0x3, 0x34, 0x5, 0x34, 
       0x29b, 0xa, 0x34, 0x3, 0x34, 0x3, 0x34, 0x3, 0x35, 0x5, 0x35, 0x2a0, 
       0xa, 0x35, 0x3, 0x35, 0x7, 0x35, 0x2a3, 0xa, 0x35, 0xc, 0x35, 0xe, 
       0x35, 0x2a6, 0xb, 0x35, 0x3, 0x36, 0x3, 0x36, 0x5, 0x36, 0x2aa, 0xa, 
       0x36, 0x3, 0x36, 0x5, 0x36, 0x2ad, 0xa, 0x36, 0x3, 0x37, 0x3, 0x37, 
       0x3, 0x37, 0x5, 0x37, 0x2b2, 0xa, 0x37, 0x7, 0x37, 0x2b4, 0xa, 0x37, 
       0xc, 0x37, 0xe, 0x37, 0x2b7, 0xb, 0x37, 0x3, 0x38, 0x3, 0x38, 0x3, 
       0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x5, 
       0x38, 0x2c1, 0xa, 0x38, 0x3, 0x39, 0x3, 0x39, 0x3, 0x39, 0x3, 0x3a, 
       0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3b, 0x3, 0x3b, 0x5, 0x3b, 
       0x2cc, 0xa, 0x3b, 0x3, 0x3b, 0x3, 0x3b, 0x3, 0x3b, 0x3, 0x3c, 0x3, 
       0x3c, 0x3, 0x3c, 0x3, 0x3c, 0x3, 0x3c, 0x3, 0x3c, 0x3, 0x3c, 0x3, 
       0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3e, 0x3, 0x3e, 0x5, 0x3e, 0x2dd, 
       0xa, 0x3e, 0x3, 0x3f, 0x3, 0x3f, 0x3, 0x3f, 0x7, 0x3f, 0x2e2, 0xa, 
       0x3f, 0xc, 0x3f, 0xe, 0x3f, 0x2e5, 0xb, 0x3f, 0x3, 0x3f, 0x3, 0x3f, 
       0x3, 0x40, 0x3, 0x40, 0x7, 0x40, 0x2eb, 0xa, 0x40, 0xc, 0x40, 0xe, 
       0x40, 0x2ee, 0xb, 0x40, 0x3, 0x40, 0x3, 0x40, 0x3, 0x40, 0x7, 0x40, 
       0x2f3, 0xa, 0x40, 0xc, 0x40, 0xe, 0x40, 0x2f6, 0xb, 0x40, 0x3, 0x40, 
       0x3, 0x40, 0x3, 0x41, 0x3, 0x41, 0x7, 0x41, 0x2fc, 0xa, 0x41, 0xc, 
       0x41, 0xe, 0x41, 0x2ff, 0xb, 0x41, 0x3, 0x41, 0x3, 0x41, 0x3, 0x42, 
       0x3, 0x42, 0x3, 0x42, 0x3, 0x42, 0x3, 0x42, 0x5, 0x42, 0x308, 0xa, 
       0x42, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x44, 0x3, 0x44, 0x3, 
       0x44, 0x7, 0x44, 0x310, 0xa, 0x44, 0xc, 0x44, 0xe, 0x44, 0x313, 0xb, 
       0x44, 0x3, 0x45, 0x3, 0x45, 0x3, 0x45, 0x3, 0x46, 0x3, 0x46, 0x3, 
       0x46, 0x3, 0x46, 0x3, 0x46, 0x3, 0x46, 0x5, 0x46, 0x31e, 0xa, 0x46, 
       0x3, 0x47, 0x3, 0x47, 0x3, 0x47, 0x3, 0x48, 0x3, 0x48, 0x5, 0x48, 
       0x325, 0xa, 0x48, 0x3, 0x48, 0x3, 0x48, 0x5, 0x48, 0x329, 0xa, 0x48, 
       0x3, 0x48, 0x3, 0x48, 0x3, 0x49, 0x3, 0x49, 0x3, 0x49, 0x7, 0x49, 
       0x330, 0xa, 0x49, 0xc, 0x49, 0xe, 0x49, 0x333, 0xb, 0x49, 0x3, 0x4a, 
       0x3, 0x4a, 0x5, 0x4a, 0x337, 0xa, 0x4a, 0x3, 0x4a, 0x3, 0x4a, 0x3, 
       0x4b, 0x3, 0x4b, 0x3, 0x4b, 0x5, 0x4b, 0x33e, 0xa, 0x4b, 0x7, 0x4b, 
       0x340, 0xa, 0x4b, 0xc, 0x4b, 0xe, 0x4b, 0x343, 0xb, 0x4b, 0x3, 0x4c, 
       0x3, 0x4c, 0x3, 0x4c, 0x3, 0x4c, 0x3, 0x4c, 0x3, 0x4c, 0x5, 0x4c, 
       0x34b, 0xa, 0x4c, 0x3, 0x4d, 0x5, 0x4d, 0x34e, 0xa, 0x4d, 0x3, 0x4e, 
       0x3, 0x4e, 0x3, 0x4e, 0x3, 0x4e, 0x3, 0x4e, 0x3, 0x4e, 0x5, 0x4e, 
       0x356, 0xa, 0x4e, 0x7, 0x4e, 0x358, 0xa, 0x4e, 0xc, 0x4e, 0xe, 0x4e, 
       0x35b, 0xb, 0x4e, 0x3, 0x4f, 0x3, 0x4f, 0x5, 0x4f, 0x35f, 0xa, 0x4f, 
       0x3, 0x50, 0x3, 0x50, 0x3, 0x50, 0x7, 0x50, 0x364, 0xa, 0x50, 0xc, 
       0x50, 0xe, 0x50, 0x367, 0xb, 0x50, 0x3, 0x51, 0x3, 0x51, 0x3, 0x52, 
       0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x5, 0x52, 
       0x371, 0xa, 0x52, 0x3, 0x53, 0x5, 0x53, 0x374, 0xa, 0x53, 0x3, 0x54, 
       0x3, 0x54, 0x5, 0x54, 0x378, 0xa, 0x54, 0x3, 0x54, 0x3, 0x54, 0x3, 
       0x54, 0x5, 0x54, 0x37d, 0xa, 0x54, 0x7, 0x54, 0x37f, 0xa, 0x54, 0xc, 
       0x54, 0xe, 0x54, 0x382, 0xb, 0x54, 0x3, 0x55, 0x3, 0x55, 0x5, 0x55, 
       0x386, 0xa, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x56, 0x3, 0x56, 0x3, 
       0x57, 0x3, 0x57, 0x3, 0x58, 0x3, 0x58, 0x3, 0x58, 0x7, 0x58, 0x391, 
       0xa, 0x58, 0xc, 0x58, 0xe, 0x58, 0x394, 0xb, 0x58, 0x3, 0x59, 0x3, 
       0x59, 0x3, 0x5a, 0x3, 0x5a, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x7, 
       0x5b, 0x39d, 0xa, 0x5b, 0xc, 0x5b, 0xe, 0x5b, 0x3a0, 0xb, 0x5b, 0x3, 
       0x5c, 0x3, 0x5c, 0x3, 0x5c, 0x7, 0x5c, 0x3a5, 0xa, 0x5c, 0xc, 0x5c, 
       0xe, 0x5c, 0x3a8, 0xb, 0x5c, 0x3, 0x5d, 0x3, 0x5d, 0x5, 0x5d, 0x3ac, 
       0xa, 0x5d, 0x3, 0x5e, 0x5, 0x5e, 0x3af, 0xa, 0x5e, 0x3, 0x5e, 0x3, 
       0x5e, 0x3, 0x5f, 0x3, 0x5f, 0x3, 0x60, 0x3, 0x60, 0x3, 0x60, 0x3, 
       0x60, 0x3, 0x60, 0x3, 0x60, 0x3, 0x60, 0x3, 0x60, 0x5, 0x60, 0x3bd, 
       0xa, 0x60, 0x3, 0x61, 0x3, 0x61, 0x3, 0x61, 0x3, 0x61, 0x3, 0x61, 
       0x7, 0x61, 0x3c4, 0xa, 0x61, 0xc, 0x61, 0xe, 0x61, 0x3c7, 0xb, 0x61, 
       0x5, 0x61, 0x3c9, 0xa, 0x61, 0x3, 0x61, 0x5, 0x61, 0x3cc, 0xa, 0x61, 
       0x3, 0x62, 0x5, 0x62, 0x3cf, 0xa, 0x62, 0x3, 0x62, 0x3, 0x62, 0x5, 
       0x62, 0x3d3, 0xa, 0x62, 0x3, 0x63, 0x3, 0x63, 0x3, 0x64, 0x3, 0x64, 
       0x5, 0x64, 0x3d9, 0xa, 0x64, 0x3, 0x65, 0x3, 0x65, 0x3, 0x65, 0x3, 
       0x65, 0x3, 0x66, 0x3, 0x66, 0x5, 0x66, 0x3e1, 0xa, 0x66, 0x3, 0x67, 
       0x3, 0x67, 0x3, 0x67, 0x3, 0x67, 0x3, 0x68, 0x3, 0x68, 0x6, 0x68, 
       0x3e9, 0xa, 0x68, 0xd, 0x68, 0xe, 0x68, 0x3ea, 0x3, 0x68, 0x3, 0x68, 
       0x3, 0x69, 0x3, 0x69, 0x6, 0x69, 0x3f1, 0xa, 0x69, 0xd, 0x69, 0xe, 
       0x69, 0x3f2, 0x3, 0x69, 0x3, 0x69, 0x3, 0x6a, 0x3, 0x6a, 0x5, 0x6a, 
       0x3f9, 0xa, 0x6a, 0x3, 0x6b, 0x3, 0x6b, 0x5, 0x6b, 0x3fd, 0xa, 0x6b, 
       0x3, 0x6c, 0x3, 0x6c, 0x5, 0x6c, 0x401, 0xa, 0x6c, 0x3, 0x6d, 0x3, 
       0x6d, 0x5, 0x6d, 0x405, 0xa, 0x6d, 0x3, 0x6e, 0x3, 0x6e, 0x3, 0x6f, 
       0x3, 0x6f, 0x3, 0x6f, 0x3, 0x6f, 0x3, 0x6f, 0x3, 0x6f, 0x5, 0x6f, 
       0x40f, 0xa, 0x6f, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x71, 0x3, 
       0x71, 0x3, 0x71, 0x3, 0x71, 0x3, 0x71, 0x3, 0x71, 0x3, 0x71, 0x3, 
       0x71, 0x5, 0x71, 0x41c, 0xa, 0x71, 0x3, 0x71, 0x3, 0x71, 0x3, 0x71, 
       0x3, 0x71, 0x3, 0x71, 0x3, 0x71, 0x3, 0x71, 0x3, 0x71, 0x3, 0x71, 
       0x3, 0x71, 0x3, 0x71, 0x3, 0x71, 0x3, 0x71, 0x5, 0x71, 0x42b, 0xa, 
       0x71, 0x3, 0x71, 0x3, 0x71, 0x3, 0x71, 0x5, 0x71, 0x430, 0xa, 0x71, 
       0x3, 0x71, 0x3, 0x71, 0x3, 0x71, 0x3, 0x71, 0x3, 0x71, 0x3, 0x71, 
       0x3, 0x71, 0x7, 0x71, 0x439, 0xa, 0x71, 0xc, 0x71, 0xe, 0x71, 0x43c, 
       0xb, 0x71, 0x3, 0x72, 0x3, 0x72, 0x5, 0x72, 0x440, 0xa, 0x72, 0x3, 
       0x72, 0x3, 0x72, 0x5, 0x72, 0x444, 0xa, 0x72, 0x3, 0x73, 0x5, 0x73, 
       0x447, 0xa, 0x73, 0x3, 0x73, 0x3, 0x73, 0x3, 0x74, 0x3, 0x74, 0x3, 
       0x74, 0x3, 0x74, 0x3, 0x74, 0x3, 0x74, 0x3, 0x74, 0x3, 0x74, 0x3, 
       0x74, 0x3, 0x74, 0x5, 0x74, 0x455, 0xa, 0x74, 0x3, 0x75, 0x3, 0x75, 
       0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 
       0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 
       0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 
       0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 
       0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 
       0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 
       0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x5, 0x75, 0x480, 0xa, 0x75, 0x3, 
       0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 
       0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 
       0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 
       0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 
       0x75, 0x3, 0x75, 0x3, 0x75, 0x5, 0x75, 0x49d, 0xa, 0x75, 0x3, 0x75, 
       0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 
       0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 
       0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 
       0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 
       0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 
       0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 
       0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 
       0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 
       0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 
       0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 
       0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 
       0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 
       0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 
       0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 
       0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 
       0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 
       0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 
       0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 
       0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 
       0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 
       0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 
       0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 
       0x3, 0x75, 0x3, 0x75, 0x5, 0x75, 0x526, 0xa, 0x75, 0x3, 0x75, 0x3, 
       0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 
       0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 
       0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 
       0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 
       0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 
       0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 
       0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 
       0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 
       0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 
       0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x5, 0x75, 0x563, 0xa, 0x75, 
       0x3, 0x76, 0x3, 0x76, 0x3, 0x76, 0x3, 0x76, 0x3, 0x76, 0x3, 0x76, 
       0x3, 0x76, 0x5, 0x76, 0x56c, 0xa, 0x76, 0x3, 0x76, 0x3, 0x76, 0x3, 
       0x77, 0x3, 0x77, 0x3, 0x77, 0x3, 0x77, 0x3, 0x77, 0x3, 0x77, 0x3, 
       0x77, 0x5, 0x77, 0x577, 0xa, 0x77, 0x3, 0x77, 0x3, 0x77, 0x3, 0x78, 
       0x3, 0x78, 0x3, 0x78, 0x3, 0x78, 0x3, 0x78, 0x3, 0x78, 0x3, 0x78, 
       0x3, 0x78, 0x3, 0x78, 0x5, 0x78, 0x584, 0xa, 0x78, 0x3, 0x78, 0x3, 
       0x78, 0x3, 0x79, 0x3, 0x79, 0x3, 0x79, 0x3, 0x7a, 0x3, 0x7a, 0x3, 
       0x7a, 0x3, 0x7a, 0x3, 0x7b, 0x3, 0x7b, 0x3, 0x7b, 0x5, 0x7b, 0x592, 
       0xa, 0x7b, 0x3, 0x7b, 0x3, 0x7b, 0x5, 0x7b, 0x596, 0xa, 0x7b, 0x3, 
       0x7b, 0x3, 0x7b, 0x3, 0x7b, 0x3, 0x7b, 0x5, 0x7b, 0x59c, 0xa, 0x7b, 
       0x3, 0x7b, 0x3, 0x7b, 0x3, 0x7b, 0x3, 0x7b, 0x3, 0x7b, 0x3, 0x7b, 
       0x5, 0x7b, 0x5a4, 0xa, 0x7b, 0x3, 0x7b, 0x3, 0x7b, 0x3, 0x7b, 0x3, 
       0x7b, 0x3, 0x7b, 0x3, 0x7b, 0x5, 0x7b, 0x5ac, 0xa, 0x7b, 0x3, 0x7b, 
       0x3, 0x7b, 0x3, 0x7b, 0x3, 0x7b, 0x3, 0x7b, 0x3, 0x7b, 0x5, 0x7b, 
       0x5b4, 0xa, 0x7b, 0x3, 0x7b, 0x3, 0x7b, 0x3, 0x7b, 0x3, 0x7b, 0x3, 
       0x7b, 0x3, 0x7b, 0x5, 0x7b, 0x5bc, 0xa, 0x7b, 0x3, 0x7b, 0x3, 0x7b, 
       0x3, 0x7b, 0x3, 0x7b, 0x3, 0x7b, 0x3, 0x7b, 0x5, 0x7b, 0x5c4, 0xa, 
       0x7b, 0x3, 0x7b, 0x3, 0x7b, 0x3, 0x7b, 0x3, 0x7b, 0x3, 0x7b, 0x5, 
       0x7b, 0x5cb, 0xa, 0x7b, 0x3, 0x7b, 0x3, 0x7b, 0x5, 0x7b, 0x5cf, 0xa, 
       0x7b, 0x3, 0x7c, 0x3, 0x7c, 0x5, 0x7c, 0x5d3, 0xa, 0x7c, 0x3, 0x7d, 
       0x3, 0x7d, 0x3, 0x7d, 0x3, 0x7d, 0x5, 0x7d, 0x5d9, 0xa, 0x7d, 0x3, 
       0x7e, 0x3, 0x7e, 0x3, 0x7e, 0x5, 0x7e, 0x5de, 0xa, 0x7e, 0x3, 0x7f, 
       0x3, 0x7f, 0x3, 0x80, 0x3, 0x80, 0x3, 0x81, 0x3, 0x81, 0x3, 0x82, 
       0x3, 0x82, 0x3, 0x83, 0x3, 0x83, 0x3, 0x84, 0x3, 0x84, 0x5, 0x84, 
       0x5ec, 0xa, 0x84, 0x3, 0x85, 0x3, 0x85, 0x3, 0x86, 0x3, 0x86, 0x5, 
       0x86, 0x5f2, 0xa, 0x86, 0x3, 0x87, 0x3, 0x87, 0x3, 0x87, 0x3, 0x87, 
       0x2, 0x3, 0xe0, 0x88, 0x2, 0x4, 0x6, 0x8, 0xa, 0xc, 0xe, 0x10, 0x12, 
       0x14, 0x16, 0x18, 0x1a, 0x1c, 0x1e, 0x20, 0x22, 0x24, 0x26, 0x28, 
       0x2a, 0x2c, 0x2e, 0x30, 0x32, 0x34, 0x36, 0x38, 0x3a, 0x3c, 0x3e, 
       0x40, 0x42, 0x44, 0x46, 0x48, 0x4a, 0x4c, 0x4e, 0x50, 0x52, 0x54, 
       0x56, 0x58, 0x5a, 0x5c, 0x5e, 0x60, 0x62, 0x64, 0x66, 0x68, 0x6a, 
       0x6c, 0x6e, 0x70, 0x72, 0x74, 0x76, 0x78, 0x7a, 0x7c, 0x7e, 0x80, 
       0x82, 0x84, 0x86, 0x88, 0x8a, 0x8c, 0x8e, 0x90, 0x92, 0x94, 0x96, 
       0x98, 0x9a, 0x9c, 0x9e, 0xa0, 0xa2, 0xa4, 0xa6, 0xa8, 0xaa, 0xac, 
       0xae, 0xb0, 0xb2, 0xb4, 0xb6, 0xb8, 0xba, 0xbc, 0xbe, 0xc0, 0xc2, 
       0xc4, 0xc6, 0xc8, 0xca, 0xcc, 0xce, 0xd0, 0xd2, 0xd4, 0xd6, 0xd8, 
       0xda, 0xdc, 0xde, 0xe0, 0xe2, 0xe4, 0xe6, 0xe8, 0xea, 0xec, 0xee, 
       0xf0, 0xf2, 0xf4, 0xf6, 0xf8, 0xfa, 0xfc, 0xfe, 0x100, 0x102, 0x104, 
       0x106, 0x108, 0x10a, 0x10c, 0x2, 0x10, 0x3, 0x2, 0x7, 0x8, 0x3, 0x2, 
       0x11, 0x12, 0x4, 0x2, 0x99, 0x99, 0x9b, 0x9c, 0x3, 0x2, 0x79, 0x7a, 
       0x4, 0x2, 0x9b, 0x9b, 0x9f, 0x9f, 0x3, 0x2, 0x99, 0x9a, 0x4, 0x2, 
       0x8b, 0x8d, 0xa0, 0xa2, 0x4, 0x2, 0x99, 0x9a, 0x9e, 0x9e, 0x3, 0x2, 
       0x7c, 0x7e, 0x3, 0x2, 0x7f, 0x81, 0x3, 0x2, 0x82, 0x84, 0x3, 0x2, 
       0x27, 0x28, 0x3, 0x2, 0x85, 0x88, 0x3, 0x2, 0x76, 0x77, 0x2, 0x66c, 
       0x2, 0x11b, 0x3, 0x2, 0x2, 0x2, 0x4, 0x121, 0x3, 0x2, 0x2, 0x2, 0x6, 
       0x124, 0x3, 0x2, 0x2, 0x2, 0x8, 0x127, 0x3, 0x2, 0x2, 0x2, 0xa, 0x12b, 
       0x3, 0x2, 0x2, 0x2, 0xc, 0x135, 0x3, 0x2, 0x2, 0x2, 0xe, 0x13a, 0x3, 
       0x2, 0x2, 0x2, 0x10, 0x146, 0x3, 0x2, 0x2, 0x2, 0x12, 0x14f, 0x3, 
       0x2, 0x2, 0x2, 0x14, 0x151, 0x3, 0x2, 0x2, 0x2, 0x16, 0x16b, 0x3, 
       0x2, 0x2, 0x2, 0x18, 0x17f, 0x3, 0x2, 0x2, 0x2, 0x1a, 0x189, 0x3, 
       0x2, 0x2, 0x2, 0x1c, 0x190, 0x3, 0x2, 0x2, 0x2, 0x1e, 0x195, 0x3, 
       0x2, 0x2, 0x2, 0x20, 0x1a0, 0x3, 0x2, 0x2, 0x2, 0x22, 0x1b2, 0x3, 
       0x2, 0x2, 0x2, 0x24, 0x1b4, 0x3, 0x2, 0x2, 0x2, 0x26, 0x1ba, 0x3, 
       0x2, 0x2, 0x2, 0x28, 0x1bc, 0x3, 0x2, 0x2, 0x2, 0x2a, 0x1ca, 0x3, 
       0x2, 0x2, 0x2, 0x2c, 0x1d4, 0x3, 0x2, 0x2, 0x2, 0x2e, 0x1d6, 0x3, 
       0x2, 0x2, 0x2, 0x30, 0x1d9, 0x3, 0x2, 0x2, 0x2, 0x32, 0x1de, 0x3, 
       0x2, 0x2, 0x2, 0x34, 0x1e0, 0x3, 0x2, 0x2, 0x2, 0x36, 0x1fc, 0x3, 
       0x2, 0x2, 0x2, 0x38, 0x1fe, 0x3, 0x2, 0x2, 0x2, 0x3a, 0x207, 0x3, 
       0x2, 0x2, 0x2, 0x3c, 0x20d, 0x3, 0x2, 0x2, 0x2, 0x3e, 0x213, 0x3, 
       0x2, 0x2, 0x2, 0x40, 0x219, 0x3, 0x2, 0x2, 0x2, 0x42, 0x221, 0x3, 
       0x2, 0x2, 0x2, 0x44, 0x229, 0x3, 0x2, 0x2, 0x2, 0x46, 0x231, 0x3, 
       0x2, 0x2, 0x2, 0x48, 0x235, 0x3, 0x2, 0x2, 0x2, 0x4a, 0x239, 0x3, 
       0x2, 0x2, 0x2, 0x4c, 0x23f, 0x3, 0x2, 0x2, 0x2, 0x4e, 0x251, 0x3, 
       0x2, 0x2, 0x2, 0x50, 0x254, 0x3, 0x2, 0x2, 0x2, 0x52, 0x257, 0x3, 
       0x2, 0x2, 0x2, 0x54, 0x262, 0x3, 0x2, 0x2, 0x2, 0x56, 0x264, 0x3, 
       0x2, 0x2, 0x2, 0x58, 0x26b, 0x3, 0x2, 0x2, 0x2, 0x5a, 0x26d, 0x3, 
       0x2, 0x2, 0x2, 0x5c, 0x271, 0x3, 0x2, 0x2, 0x2, 0x5e, 0x276, 0x3, 
       0x2, 0x2, 0x2, 0x60, 0x27e, 0x3, 0x2, 0x2, 0x2, 0x62, 0x285, 0x3, 
       0x2, 0x2, 0x2, 0x64, 0x28d, 0x3, 0x2, 0x2, 0x2, 0x66, 0x297, 0x3, 
       0x2, 0x2, 0x2, 0x68, 0x29f, 0x3, 0x2, 0x2, 0x2, 0x6a, 0x2a7, 0x3, 
       0x2, 0x2, 0x2, 0x6c, 0x2ae, 0x3, 0x2, 0x2, 0x2, 0x6e, 0x2c0, 0x3, 
       0x2, 0x2, 0x2, 0x70, 0x2c2, 0x3, 0x2, 0x2, 0x2, 0x72, 0x2c5, 0x3, 
       0x2, 0x2, 0x2, 0x74, 0x2c9, 0x3, 0x2, 0x2, 0x2, 0x76, 0x2d0, 0x3, 
       0x2, 0x2, 0x2, 0x78, 0x2d7, 0x3, 0x2, 0x2, 0x2, 0x7a, 0x2dc, 0x3, 
       0x2, 0x2, 0x2, 0x7c, 0x2de, 0x3, 0x2, 0x2, 0x2, 0x7e, 0x2e8, 0x3, 
       0x2, 0x2, 0x2, 0x80, 0x2f9, 0x3, 0x2, 0x2, 0x2, 0x82, 0x307, 0x3, 
       0x2, 0x2, 0x2, 0x84, 0x309, 0x3, 0x2, 0x2, 0x2, 0x86, 0x30c, 0x3, 
       0x2, 0x2, 0x2, 0x88, 0x314, 0x3, 0x2, 0x2, 0x2, 0x8a, 0x31d, 0x3, 
       0x2, 0x2, 0x2, 0x8c, 0x31f, 0x3, 0x2, 0x2, 0x2, 0x8e, 0x322, 0x3, 
       0x2, 0x2, 0x2, 0x90, 0x32c, 0x3, 0x2, 0x2, 0x2, 0x92, 0x334, 0x3, 
       0x2, 0x2, 0x2, 0x94, 0x33a, 0x3, 0x2, 0x2, 0x2, 0x96, 0x34a, 0x3, 
       0x2, 0x2, 0x2, 0x98, 0x34d, 0x3, 0x2, 0x2, 0x2, 0x9a, 0x34f, 0x3, 
       0x2, 0x2, 0x2, 0x9c, 0x35e, 0x3, 0x2, 0x2, 0x2, 0x9e, 0x360, 0x3, 
       0x2, 0x2, 0x2, 0xa0, 0x368, 0x3, 0x2, 0x2, 0x2, 0xa2, 0x370, 0x3, 
       0x2, 0x2, 0x2, 0xa4, 0x373, 0x3, 0x2, 0x2, 0x2, 0xa6, 0x377, 0x3, 
       0x2, 0x2, 0x2, 0xa8, 0x385, 0x3, 0x2, 0x2, 0x2, 0xaa, 0x389, 0x3, 
       0x2, 0x2, 0x2, 0xac, 0x38b, 0x3, 0x2, 0x2, 0x2, 0xae, 0x38d, 0x3, 
       0x2, 0x2, 0x2, 0xb0, 0x395, 0x3, 0x2, 0x2, 0x2, 0xb2, 0x397, 0x3, 
       0x2, 0x2, 0x2, 0xb4, 0x399, 0x3, 0x2, 0x2, 0x2, 0xb6, 0x3a1, 0x3, 
       0x2, 0x2, 0x2, 0xb8, 0x3a9, 0x3, 0x2, 0x2, 0x2, 0xba, 0x3ae, 0x3, 
       0x2, 0x2, 0x2, 0xbc, 0x3b2, 0x3, 0x2, 0x2, 0x2, 0xbe, 0x3bc, 0x3, 
       0x2, 0x2, 0x2, 0xc0, 0x3cb, 0x3, 0x2, 0x2, 0x2, 0xc2, 0x3ce, 0x3, 
       0x2, 0x2, 0x2, 0xc4, 0x3d4, 0x3, 0x2, 0x2, 0x2, 0xc6, 0x3d8, 0x3, 
       0x2, 0x2, 0x2, 0xc8, 0x3da, 0x3, 0x2, 0x2, 0x2, 0xca, 0x3e0, 0x3, 
       0x2, 0x2, 0x2, 0xcc, 0x3e2, 0x3, 0x2, 0x2, 0x2, 0xce, 0x3e6, 0x3, 
       0x2, 0x2, 0x2, 0xd0, 0x3ee, 0x3, 0x2, 0x2, 0x2, 0xd2, 0x3f8, 0x3, 
       0x2, 0x2, 0x2, 0xd4, 0x3fc, 0x3, 0x2, 0x2, 0x2, 0xd6, 0x400, 0x3, 
       0x2, 0x2, 0x2, 0xd8, 0x404, 0x3, 0x2, 0x2, 0x2, 0xda, 0x406, 0x3, 
       0x2, 0x2, 0x2, 0xdc, 0x40e, 0x3, 0x2, 0x2, 0x2, 0xde, 0x410, 0x3, 
       0x2, 0x2, 0x2, 0xe0, 0x41b, 0x3, 0x2, 0x2, 0x2, 0xe2, 0x43f, 0x3, 
       0x2, 0x2, 0x2, 0xe4, 0x446, 0x3, 0x2, 0x2, 0x2, 0xe6, 0x454, 0x3, 
       0x2, 0x2, 0x2, 0xe8, 0x562, 0x3, 0x2, 0x2, 0x2, 0xea, 0x564, 0x3, 
       0x2, 0x2, 0x2, 0xec, 0x56f, 0x3, 0x2, 0x2, 0x2, 0xee, 0x57a, 0x3, 
       0x2, 0x2, 0x2, 0xf0, 0x587, 0x3, 0x2, 0x2, 0x2, 0xf2, 0x58a, 0x3, 
       0x2, 0x2, 0x2, 0xf4, 0x5ce, 0x3, 0x2, 0x2, 0x2, 0xf6, 0x5d0, 0x3, 
       0x2, 0x2, 0x2, 0xf8, 0x5d4, 0x3, 0x2, 0x2, 0x2, 0xfa, 0x5dd, 0x3, 
       0x2, 0x2, 0x2, 0xfc, 0x5df, 0x3, 0x2, 0x2, 0x2, 0xfe, 0x5e1, 0x3, 
       0x2, 0x2, 0x2, 0x100, 0x5e3, 0x3, 0x2, 0x2, 0x2, 0x102, 0x5e5, 0x3, 
       0x2, 0x2, 0x2, 0x104, 0x5e7, 0x3, 0x2, 0x2, 0x2, 0x106, 0x5eb, 0x3, 
       0x2, 0x2, 0x2, 0x108, 0x5ed, 0x3, 0x2, 0x2, 0x2, 0x10a, 0x5f1, 0x3, 
       0x2, 0x2, 0x2, 0x10c, 0x5f3, 0x3, 0x2, 0x2, 0x2, 0x10e, 0x113, 0x5, 
       0x4, 0x3, 0x2, 0x10f, 0x114, 0x5, 0xa, 0x6, 0x2, 0x110, 0x114, 0x5, 
       0x14, 0xb, 0x2, 0x111, 0x114, 0x5, 0x16, 0xc, 0x2, 0x112, 0x114, 
       0x5, 0x18, 0xd, 0x2, 0x113, 0x10f, 0x3, 0x2, 0x2, 0x2, 0x113, 0x110, 
       0x3, 0x2, 0x2, 0x2, 0x113, 0x111, 0x3, 0x2, 0x2, 0x2, 0x113, 0x112, 
       0x3, 0x2, 0x2, 0x2, 0x113, 0x114, 0x3, 0x2, 0x2, 0x2, 0x114, 0x115, 
       0x3, 0x2, 0x2, 0x2, 0x115, 0x116, 0x5, 0x32, 0x1a, 0x2, 0x116, 0x117, 
       0x7, 0x2, 0x2, 0x3, 0x117, 0x11c, 0x3, 0x2, 0x2, 0x2, 0x118, 0x119, 
       0x5, 0x34, 0x1b, 0x2, 0x119, 0x11a, 0x7, 0x2, 0x2, 0x3, 0x11a, 0x11c, 
       0x3, 0x2, 0x2, 0x2, 0x11b, 0x10e, 0x3, 0x2, 0x2, 0x2, 0x11b, 0x118, 
       0x3, 0x2, 0x2, 0x2, 0x11c, 0x3, 0x3, 0x2, 0x2, 0x2, 0x11d, 0x120, 
       0x5, 0x6, 0x4, 0x2, 0x11e, 0x120, 0x5, 0x8, 0x5, 0x2, 0x11f, 0x11d, 
       0x3, 0x2, 0x2, 0x2, 0x11f, 0x11e, 0x3, 0x2, 0x2, 0x2, 0x120, 0x123, 
       0x3, 0x2, 0x2, 0x2, 0x121, 0x11f, 0x3, 0x2, 0x2, 0x2, 0x121, 0x122, 
       0x3, 0x2, 0x2, 0x2, 0x122, 0x5, 0x3, 0x2, 0x2, 0x2, 0x123, 0x121, 
       0x3, 0x2, 0x2, 0x2, 0x124, 0x125, 0x7, 0x4, 0x2, 0x2, 0x125, 0x126, 
       0x7, 0x75, 0x2, 0x2, 0x126, 0x7, 0x3, 0x2, 0x2, 0x2, 0x127, 0x128, 
       0x7, 0x5, 0x2, 0x2, 0x128, 0x129, 0x7, 0x76, 0x2, 0x2, 0x129, 0x12a, 
       0x7, 0x75, 0x2, 0x2, 0x12a, 0x9, 0x3, 0x2, 0x2, 0x2, 0x12b, 0x12f, 
       0x5, 0xe, 0x8, 0x2, 0x12c, 0x12e, 0x5, 0x1a, 0xe, 0x2, 0x12d, 0x12c, 
       0x3, 0x2, 0x2, 0x2, 0x12e, 0x131, 0x3, 0x2, 0x2, 0x2, 0x12f, 0x12d, 
       0x3, 0x2, 0x2, 0x2, 0x12f, 0x130, 0x3, 0x2, 0x2, 0x2, 0x130, 0x132, 
       0x3, 0x2, 0x2, 0x2, 0x131, 0x12f, 0x3, 0x2, 0x2, 0x2, 0x132, 0x133, 
       0x5, 0x1c, 0xf, 0x2, 0x133, 0x134, 0x5, 0x1e, 0x10, 0x2, 0x134, 0xb, 
       0x3, 0x2, 0x2, 0x2, 0x135, 0x136, 0x5, 0xe, 0x8, 0x2, 0x136, 0x137, 
       0x5, 0x1c, 0xf, 0x2, 0x137, 0x138, 0x5, 0x1e, 0x10, 0x2, 0x138, 0x139, 
       0x5, 0x32, 0x1a, 0x2, 0x139, 0xd, 0x3, 0x2, 0x2, 0x2, 0x13a, 0x13c, 
       0x7, 0x6, 0x2, 0x2, 0x13b, 0x13d, 0x5, 0x10, 0x9, 0x2, 0x13c, 0x13b, 
       0x3, 0x2, 0x2, 0x2, 0x13c, 0x13d, 0x3, 0x2, 0x2, 0x2, 0x13d, 0x144, 
       0x3, 0x2, 0x2, 0x2, 0x13e, 0x140, 0x5, 0x12, 0xa, 0x2, 0x13f, 0x13e, 
       0x3, 0x2, 0x2, 0x2, 0x140, 0x141, 0x3, 0x2, 0x2, 0x2, 0x141, 0x13f, 
       0x3, 0x2, 0x2, 0x2, 0x141, 0x142, 0x3, 0x2, 0x2, 0x2, 0x142, 0x145, 
       0x3, 0x2, 0x2, 0x2, 0x143, 0x145, 0x7, 0x9b, 0x2, 0x2, 0x144, 0x13f, 
       0x3, 0x2, 0x2, 0x2, 0x144, 0x143, 0x3, 0x2, 0x2, 0x2, 0x145, 0xf, 
       0x3, 0x2, 0x2, 0x2, 0x146, 0x147, 0x9, 0x2, 0x2, 0x2, 0x147, 0x11, 
       0x3, 0x2, 0x2, 0x2, 0x148, 0x150, 0x5, 0xda, 0x6e, 0x2, 0x149, 0x14a, 
       0x7, 0x91, 0x2, 0x2, 0x14a, 0x14b, 0x5, 0xe0, 0x71, 0x2, 0x14b, 0x14c, 
       0x7, 0x3a, 0x2, 0x2, 0x14c, 0x14d, 0x5, 0xda, 0x6e, 0x2, 0x14d, 0x14e, 
       0x7, 0x92, 0x2, 0x2, 0x14e, 0x150, 0x3, 0x2, 0x2, 0x2, 0x14f, 0x148, 
       0x3, 0x2, 0x2, 0x2, 0x14f, 0x149, 0x3, 0x2, 0x2, 0x2, 0x150, 0x13, 
       0x3, 0x2, 0x2, 0x2, 0x151, 0x169, 0x7, 0x9, 0x2, 0x2, 0x152, 0x156, 
       0x5, 0x92, 0x4a, 0x2, 0x153, 0x155, 0x5, 0x1a, 0xe, 0x2, 0x154, 0x153, 
       0x3, 0x2, 0x2, 0x2, 0x155, 0x158, 0x3, 0x2, 0x2, 0x2, 0x156, 0x154, 
       0x3, 0x2, 0x2, 0x2, 0x156, 0x157, 0x3, 0x2, 0x2, 0x2, 0x157, 0x159, 
       0x3, 0x2, 0x2, 0x2, 0x158, 0x156, 0x3, 0x2, 0x2, 0x2, 0x159, 0x15a, 
       0x5, 0x1c, 0xf, 0x2, 0x15a, 0x15b, 0x5, 0x1e, 0x10, 0x2, 0x15b, 0x16a, 
       0x3, 0x2, 0x2, 0x2, 0x15c, 0x15e, 0x5, 0x1a, 0xe, 0x2, 0x15d, 0x15c, 
       0x3, 0x2, 0x2, 0x2, 0x15e, 0x161, 0x3, 0x2, 0x2, 0x2, 0x15f, 0x15d, 
       0x3, 0x2, 0x2, 0x2, 0x15f, 0x160, 0x3, 0x2, 0x2, 0x2, 0x160, 0x162, 
       0x3, 0x2, 0x2, 0x2, 0x161, 0x15f, 0x3, 0x2, 0x2, 0x2, 0x162, 0x163, 
       0x7, 0xe, 0x2, 0x2, 0x163, 0x165, 0x7, 0x93, 0x2, 0x2, 0x164, 0x166, 
       0x5, 0x64, 0x33, 0x2, 0x165, 0x164, 0x3, 0x2, 0x2, 0x2, 0x165, 0x166, 
       0x3, 0x2, 0x2, 0x2, 0x166, 0x167, 0x3, 0x2, 0x2, 0x2, 0x167, 0x168, 
       0x7, 0x94, 0x2, 0x2, 0x168, 0x16a, 0x5, 0x1e, 0x10, 0x2, 0x169, 0x152, 
       0x3, 0x2, 0x2, 0x2, 0x169, 0x15f, 0x3, 0x2, 0x2, 0x2, 0x16a, 0x15, 
       0x3, 0x2, 0x2, 0x2, 0x16b, 0x172, 0x7, 0xa, 0x2, 0x2, 0x16c, 0x16e, 
       0x5, 0xd8, 0x6d, 0x2, 0x16d, 0x16c, 0x3, 0x2, 0x2, 0x2, 0x16e, 0x16f, 
       0x3, 0x2, 0x2, 0x2, 0x16f, 0x16d, 0x3, 0x2, 0x2, 0x2, 0x16f, 0x170, 
       0x3, 0x2, 0x2, 0x2, 0x170, 0x173, 0x3, 0x2, 0x2, 0x2, 0x171, 0x173, 
       0x7, 0x9b, 0x2, 0x2, 0x172, 0x16d, 0x3, 0x2, 0x2, 0x2, 0x172, 0x171, 
       0x3, 0x2, 0x2, 0x2, 0x173, 0x177, 0x3, 0x2, 0x2, 0x2, 0x174, 0x176, 
       0x5, 0x1a, 0xe, 0x2, 0x175, 0x174, 0x3, 0x2, 0x2, 0x2, 0x176, 0x179, 
       0x3, 0x2, 0x2, 0x2, 0x177, 0x175, 0x3, 0x2, 0x2, 0x2, 0x177, 0x178, 
       0x3, 0x2, 0x2, 0x2, 0x178, 0x17b, 0x3, 0x2, 0x2, 0x2, 0x179, 0x177, 
       0x3, 0x2, 0x2, 0x2, 0x17a, 0x17c, 0x5, 0x1c, 0xf, 0x2, 0x17b, 0x17a, 
       0x3, 0x2, 0x2, 0x2, 0x17b, 0x17c, 0x3, 0x2, 0x2, 0x2, 0x17c, 0x17d, 
       0x3, 0x2, 0x2, 0x2, 0x17d, 0x17e, 0x5, 0x1e, 0x10, 0x2, 0x17e, 0x17, 
       0x3, 0x2, 0x2, 0x2, 0x17f, 0x183, 0x7, 0xb, 0x2, 0x2, 0x180, 0x182, 
       0x5, 0x1a, 0xe, 0x2, 0x181, 0x180, 0x3, 0x2, 0x2, 0x2, 0x182, 0x185, 
       0x3, 0x2, 0x2, 0x2, 0x183, 0x181, 0x3, 0x2, 0x2, 0x2, 0x183, 0x184, 
       0x3, 0x2, 0x2, 0x2, 0x184, 0x186, 0x3, 0x2, 0x2, 0x2, 0x185, 0x183, 
       0x3, 0x2, 0x2, 0x2, 0x186, 0x187, 0x5, 0x1c, 0xf, 0x2, 0x187, 0x188, 
       0x5, 0x1e, 0x10, 0x2, 0x188, 0x19, 0x3, 0x2, 0x2, 0x2, 0x189, 0x18b, 
       0x7, 0xc, 0x2, 0x2, 0x18a, 0x18c, 0x7, 0xd, 0x2, 0x2, 0x18b, 0x18a, 
       0x3, 0x2, 0x2, 0x2, 0x18b, 0x18c, 0x3, 0x2, 0x2, 0x2, 0x18c, 0x18d, 
       0x3, 0x2, 0x2, 0x2, 0x18d, 0x18e, 0x5, 0x106, 0x84, 0x2, 0x18e, 0x1b, 
       0x3, 0x2, 0x2, 0x2, 0x18f, 0x191, 0x7, 0xe, 0x2, 0x2, 0x190, 0x18f, 
       0x3, 0x2, 0x2, 0x2, 0x190, 0x191, 0x3, 0x2, 0x2, 0x2, 0x191, 0x192, 
       0x3, 0x2, 0x2, 0x2, 0x192, 0x193, 0x5, 0x66, 0x34, 0x2, 0x193, 0x1d, 
       0x3, 0x2, 0x2, 0x2, 0x194, 0x196, 0x5, 0x20, 0x11, 0x2, 0x195, 0x194, 
       0x3, 0x2, 0x2, 0x2, 0x195, 0x196, 0x3, 0x2, 0x2, 0x2, 0x196, 0x198, 
       0x3, 0x2, 0x2, 0x2, 0x197, 0x199, 0x5, 0x24, 0x13, 0x2, 0x198, 0x197, 
       0x3, 0x2, 0x2, 0x2, 0x198, 0x199, 0x3, 0x2, 0x2, 0x2, 0x199, 0x19b, 
       0x3, 0x2, 0x2, 0x2, 0x19a, 0x19c, 0x5, 0x28, 0x15, 0x2, 0x19b, 0x19a, 
       0x3, 0x2, 0x2, 0x2, 0x19b, 0x19c, 0x3, 0x2, 0x2, 0x2, 0x19c, 0x19e, 
       0x3, 0x2, 0x2, 0x2, 0x19d, 0x19f, 0x5, 0x2c, 0x17, 0x2, 0x19e, 0x19d, 
       0x3, 0x2, 0x2, 0x2, 0x19e, 0x19f, 0x3, 0x2, 0x2, 0x2, 0x19f, 0x1f, 
       0x3, 0x2, 0x2, 0x2, 0x1a0, 0x1a1, 0x7, 0x3b, 0x2, 0x2, 0x1a1, 0x1a3, 
       0x7, 0x10, 0x2, 0x2, 0x1a2, 0x1a4, 0x5, 0x22, 0x12, 0x2, 0x1a3, 0x1a2, 
       0x3, 0x2, 0x2, 0x2, 0x1a4, 0x1a5, 0x3, 0x2, 0x2, 0x2, 0x1a5, 0x1a3, 
       0x3, 0x2, 0x2, 0x2, 0x1a5, 0x1a6, 0x3, 0x2, 0x2, 0x2, 0x1a6, 0x21, 
       0x3, 0x2, 0x2, 0x2, 0x1a7, 0x1b3, 0x5, 0xe8, 0x75, 0x2, 0x1a8, 0x1b3, 
       0x5, 0x8c, 0x47, 0x2, 0x1a9, 0x1aa, 0x7, 0x91, 0x2, 0x2, 0x1aa, 0x1ad, 
       0x5, 0xe0, 0x71, 0x2, 0x1ab, 0x1ac, 0x7, 0x3a, 0x2, 0x2, 0x1ac, 0x1ae, 
       0x5, 0xda, 0x6e, 0x2, 0x1ad, 0x1ab, 0x3, 0x2, 0x2, 0x2, 0x1ad, 0x1ae, 
       0x3, 0x2, 0x2, 0x2, 0x1ae, 0x1af, 0x3, 0x2, 0x2, 0x2, 0x1af, 0x1b0, 
       0x7, 0x92, 0x2, 0x2, 0x1b0, 0x1b3, 0x3, 0x2, 0x2, 0x2, 0x1b1, 0x1b3, 
       0x5, 0xda, 0x6e, 0x2, 0x1b2, 0x1a7, 0x3, 0x2, 0x2, 0x2, 0x1b2, 0x1a8, 
       0x3, 0x2, 0x2, 0x2, 0x1b2, 0x1a9, 0x3, 0x2, 0x2, 0x2, 0x1b2, 0x1b1, 
       0x3, 0x2, 0x2, 0x2, 0x1b3, 0x23, 0x3, 0x2, 0x2, 0x2, 0x1b4, 0x1b6, 
       0x7, 0x3c, 0x2, 0x2, 0x1b5, 0x1b7, 0x5, 0x26, 0x14, 0x2, 0x1b6, 0x1b5, 
       0x3, 0x2, 0x2, 0x2, 0x1b7, 0x1b8, 0x3, 0x2, 0x2, 0x2, 0x1b8, 0x1b6, 
       0x3, 0x2, 0x2, 0x2, 0x1b8, 0x1b9, 0x3, 0x2, 0x2, 0x2, 0x1b9, 0x25, 
       0x3, 0x2, 0x2, 0x2, 0x1ba, 0x1bb, 0x5, 0x8a, 0x46, 0x2, 0x1bb, 0x27, 
       0x3, 0x2, 0x2, 0x2, 0x1bc, 0x1bd, 0x7, 0xf, 0x2, 0x2, 0x1bd, 0x1bf, 
       0x7, 0x10, 0x2, 0x2, 0x1be, 0x1c0, 0x5, 0x2a, 0x16, 0x2, 0x1bf, 0x1be, 
       0x3, 0x2, 0x2, 0x2, 0x1c0, 0x1c1, 0x3, 0x2, 0x2, 0x2, 0x1c1, 0x1bf, 
       0x3, 0x2, 0x2, 0x2, 0x1c1, 0x1c2, 0x3, 0x2, 0x2, 0x2, 0x1c2, 0x29, 
       0x3, 0x2, 0x2, 0x2, 0x1c3, 0x1c4, 0x9, 0x3, 0x2, 0x2, 0x1c4, 0x1c5, 
       0x7, 0x91, 0x2, 0x2, 0x1c5, 0x1c6, 0x5, 0xe0, 0x71, 0x2, 0x1c6, 0x1c7, 
       0x7, 0x92, 0x2, 0x2, 0x1c7, 0x1cb, 0x3, 0x2, 0x2, 0x2, 0x1c8, 0x1cb, 
       0x5, 0x8a, 0x46, 0x2, 0x1c9, 0x1cb, 0x5, 0xda, 0x6e, 0x2, 0x1ca, 
       0x1c3, 0x3, 0x2, 0x2, 0x2, 0x1ca, 0x1c8, 0x3, 0x2, 0x2, 0x2, 0x1ca, 
       0x1c9, 0x3, 0x2, 0x2, 0x2, 0x1cb, 0x2b, 0x3, 0x2, 0x2, 0x2, 0x1cc, 
       0x1ce, 0x5, 0x2e, 0x18, 0x2, 0x1cd, 0x1cf, 0x5, 0x30, 0x19, 0x2, 
       0x1ce, 0x1cd, 0x3, 0x2, 0x2, 0x2, 0x1ce, 0x1cf, 0x3, 0x2, 0x2, 0x2, 
       0x1cf, 0x1d5, 0x3, 0x2, 0x2, 0x2, 0x1d0, 0x1d2, 0x5, 0x30, 0x19, 
       0x2, 0x1d1, 0x1d3, 0x5, 0x2e, 0x18, 0x2, 0x1d2, 0x1d1, 0x3, 0x2, 
       0x2, 0x2, 0x1d2, 0x1d3, 0x3, 0x2, 0x2, 0x2, 0x1d3, 0x1d5, 0x3, 0x2, 
       0x2, 0x2, 0x1d4, 0x1cc, 0x3, 0x2, 0x2, 0x2, 0x1d4, 0x1d0, 0x3, 0x2, 
       0x2, 0x2, 0x1d5, 0x2d, 0x3, 0x2, 0x2, 0x2, 0x1d6, 0x1d7, 0x7, 0x13, 
       0x2, 0x2, 0x1d7, 0x1d8, 0x7, 0x7c, 0x2, 0x2, 0x1d8, 0x2f, 0x3, 0x2, 
       0x2, 0x2, 0x1d9, 0x1da, 0x7, 0x14, 0x2, 0x2, 0x1da, 0x1db, 0x7, 0x7c, 
       0x2, 0x2, 0x1db, 0x31, 0x3, 0x2, 0x2, 0x2, 0x1dc, 0x1dd, 0x7, 0x15, 
       0x2, 0x2, 0x1dd, 0x1df, 0x5, 0x7a, 0x3e, 0x2, 0x1de, 0x1dc, 0x3, 
       0x2, 0x2, 0x2, 0x1de, 0x1df, 0x3, 0x2, 0x2, 0x2, 0x1df, 0x33, 0x3, 
       0x2, 0x2, 0x2, 0x1e0, 0x1ef, 0x5, 0x4, 0x3, 0x2, 0x1e1, 0x1e8, 0x5, 
       0x36, 0x1c, 0x2, 0x1e2, 0x1e3, 0x7, 0x97, 0x2, 0x2, 0x1e3, 0x1e4, 
       0x5, 0x4, 0x3, 0x2, 0x1e4, 0x1e5, 0x5, 0x36, 0x1c, 0x2, 0x1e5, 0x1e7, 
       0x3, 0x2, 0x2, 0x2, 0x1e6, 0x1e2, 0x3, 0x2, 0x2, 0x2, 0x1e7, 0x1ea, 
       0x3, 0x2, 0x2, 0x2, 0x1e8, 0x1e6, 0x3, 0x2, 0x2, 0x2, 0x1e8, 0x1e9, 
       0x3, 0x2, 0x2, 0x2, 0x1e9, 0x1ed, 0x3, 0x2, 0x2, 0x2, 0x1ea, 0x1e8, 
       0x3, 0x2, 0x2, 0x2, 0x1eb, 0x1ec, 0x7, 0x97, 0x2, 0x2, 0x1ec, 0x1ee, 
       0x5, 0x4, 0x3, 0x2, 0x1ed, 0x1eb, 0x3, 0x2, 0x2, 0x2, 0x1ed, 0x1ee, 
       0x3, 0x2, 0x2, 0x2, 0x1ee, 0x1f0, 0x3, 0x2, 0x2, 0x2, 0x1ef, 0x1e1, 
       0x3, 0x2, 0x2, 0x2, 0x1ef, 0x1f0, 0x3, 0x2, 0x2, 0x2, 0x1f0, 0x35, 
       0x3, 0x2, 0x2, 0x2, 0x1f1, 0x1fd, 0x5, 0x38, 0x1d, 0x2, 0x1f2, 0x1fd, 
       0x5, 0x3a, 0x1e, 0x2, 0x1f3, 0x1fd, 0x5, 0x3c, 0x1f, 0x2, 0x1f4, 
       0x1fd, 0x5, 0x40, 0x21, 0x2, 0x1f5, 0x1fd, 0x5, 0x42, 0x22, 0x2, 
       0x1f6, 0x1fd, 0x5, 0x44, 0x23, 0x2, 0x1f7, 0x1fd, 0x5, 0x3e, 0x20, 
       0x2, 0x1f8, 0x1fd, 0x5, 0x46, 0x24, 0x2, 0x1f9, 0x1fd, 0x5, 0x48, 
       0x25, 0x2, 0x1fa, 0x1fd, 0x5, 0x4a, 0x26, 0x2, 0x1fb, 0x1fd, 0x5, 
       0x4c, 0x27, 0x2, 0x1fc, 0x1f1, 0x3, 0x2, 0x2, 0x2, 0x1fc, 0x1f2, 
       0x3, 0x2, 0x2, 0x2, 0x1fc, 0x1f3, 0x3, 0x2, 0x2, 0x2, 0x1fc, 0x1f4, 
       0x3, 0x2, 0x2, 0x2, 0x1fc, 0x1f5, 0x3, 0x2, 0x2, 0x2, 0x1fc, 0x1f6, 
       0x3, 0x2, 0x2, 0x2, 0x1fc, 0x1f7, 0x3, 0x2, 0x2, 0x2, 0x1fc, 0x1f8, 
       0x3, 0x2, 0x2, 0x2, 0x1fc, 0x1f9, 0x3, 0x2, 0x2, 0x2, 0x1fc, 0x1fa, 
       0x3, 0x2, 0x2, 0x2, 0x1fc, 0x1fb, 0x3, 0x2, 0x2, 0x2, 0x1fd, 0x37, 
       0x3, 0x2, 0x2, 0x2, 0x1fe, 0x200, 0x7, 0x29, 0x2, 0x2, 0x1ff, 0x201, 
       0x7, 0x33, 0x2, 0x2, 0x200, 0x1ff, 0x3, 0x2, 0x2, 0x2, 0x200, 0x201, 
       0x3, 0x2, 0x2, 0x2, 0x201, 0x202, 0x3, 0x2, 0x2, 0x2, 0x202, 0x205, 
       0x5, 0x106, 0x84, 0x2, 0x203, 0x204, 0x7, 0x38, 0x2, 0x2, 0x204, 
       0x206, 0x5, 0x56, 0x2c, 0x2, 0x205, 0x203, 0x3, 0x2, 0x2, 0x2, 0x205, 
       0x206, 0x3, 0x2, 0x2, 0x2, 0x206, 0x39, 0x3, 0x2, 0x2, 0x2, 0x207, 
       0x209, 0x7, 0x2a, 0x2, 0x2, 0x208, 0x20a, 0x7, 0x33, 0x2, 0x2, 0x209, 
       0x208, 0x3, 0x2, 0x2, 0x2, 0x209, 0x20a, 0x3, 0x2, 0x2, 0x2, 0x20a, 
       0x20b, 0x3, 0x2, 0x2, 0x2, 0x20b, 0x20c, 0x5, 0x58, 0x2d, 0x2, 0x20c, 
       0x3b, 0x3, 0x2, 0x2, 0x2, 0x20d, 0x20f, 0x7, 0x2b, 0x2, 0x2, 0x20e, 
       0x210, 0x7, 0x33, 0x2, 0x2, 0x20f, 0x20e, 0x3, 0x2, 0x2, 0x2, 0x20f, 
       0x210, 0x3, 0x2, 0x2, 0x2, 0x210, 0x211, 0x3, 0x2, 0x2, 0x2, 0x211, 
       0x212, 0x5, 0x58, 0x2d, 0x2, 0x212, 0x3d, 0x3, 0x2, 0x2, 0x2, 0x213, 
       0x215, 0x7, 0x2f, 0x2, 0x2, 0x214, 0x216, 0x7, 0x33, 0x2, 0x2, 0x215, 
       0x214, 0x3, 0x2, 0x2, 0x2, 0x215, 0x216, 0x3, 0x2, 0x2, 0x2, 0x216, 
       0x217, 0x3, 0x2, 0x2, 0x2, 0x217, 0x218, 0x5, 0x56, 0x2c, 0x2, 0x218, 
       0x3f, 0x3, 0x2, 0x2, 0x2, 0x219, 0x21b, 0x7, 0x2c, 0x2, 0x2, 0x21a, 
       0x21c, 0x7, 0x33, 0x2, 0x2, 0x21b, 0x21a, 0x3, 0x2, 0x2, 0x2, 0x21b, 
       0x21c, 0x3, 0x2, 0x2, 0x2, 0x21c, 0x21d, 0x3, 0x2, 0x2, 0x2, 0x21d, 
       0x21e, 0x5, 0x54, 0x2b, 0x2, 0x21e, 0x21f, 0x7, 0x39, 0x2, 0x2, 0x21f, 
       0x220, 0x5, 0x54, 0x2b, 0x2, 0x220, 0x41, 0x3, 0x2, 0x2, 0x2, 0x221, 
       0x223, 0x7, 0x2d, 0x2, 0x2, 0x222, 0x224, 0x7, 0x33, 0x2, 0x2, 0x223, 
       0x222, 0x3, 0x2, 0x2, 0x2, 0x223, 0x224, 0x3, 0x2, 0x2, 0x2, 0x224, 
       0x225, 0x3, 0x2, 0x2, 0x2, 0x225, 0x226, 0x5, 0x54, 0x2b, 0x2, 0x226, 
       0x227, 0x7, 0x39, 0x2, 0x2, 0x227, 0x228, 0x5, 0x54, 0x2b, 0x2, 0x228, 
       0x43, 0x3, 0x2, 0x2, 0x2, 0x229, 0x22b, 0x7, 0x2e, 0x2, 0x2, 0x22a, 
       0x22c, 0x7, 0x33, 0x2, 0x2, 0x22b, 0x22a, 0x3, 0x2, 0x2, 0x2, 0x22b, 
       0x22c, 0x3, 0x2, 0x2, 0x2, 0x22c, 0x22d, 0x3, 0x2, 0x2, 0x2, 0x22d, 
       0x22e, 0x5, 0x54, 0x2b, 0x2, 0x22e, 0x22f, 0x7, 0x39, 0x2, 0x2, 0x22f, 
       0x230, 0x5, 0x54, 0x2b, 0x2, 0x230, 0x45, 0x3, 0x2, 0x2, 0x2, 0x231, 
       0x232, 0x7, 0x31, 0x2, 0x2, 0x232, 0x233, 0x7, 0x36, 0x2, 0x2, 0x233, 
       0x234, 0x5, 0x5c, 0x2f, 0x2, 0x234, 0x47, 0x3, 0x2, 0x2, 0x2, 0x235, 
       0x236, 0x7, 0x30, 0x2, 0x2, 0x236, 0x237, 0x7, 0x36, 0x2, 0x2, 0x237, 
       0x238, 0x5, 0x5c, 0x2f, 0x2, 0x238, 0x49, 0x3, 0x2, 0x2, 0x2, 0x239, 
       0x23a, 0x7, 0x30, 0x2, 0x2, 0x23a, 0x23b, 0x7, 0xe, 0x2, 0x2, 0x23b, 
       0x23c, 0x5, 0x5a, 0x2e, 0x2, 0x23c, 0x4b, 0x3, 0x2, 0x2, 0x2, 0x23d, 
       0x23e, 0x7, 0x37, 0x2, 0x2, 0x23e, 0x240, 0x5, 0x106, 0x84, 0x2, 
       0x23f, 0x23d, 0x3, 0x2, 0x2, 0x2, 0x23f, 0x240, 0x3, 0x2, 0x2, 0x2, 
       0x240, 0x246, 0x3, 0x2, 0x2, 0x2, 0x241, 0x243, 0x5, 0x4e, 0x28, 
       0x2, 0x242, 0x244, 0x5, 0x50, 0x29, 0x2, 0x243, 0x242, 0x3, 0x2, 
       0x2, 0x2, 0x243, 0x244, 0x3, 0x2, 0x2, 0x2, 0x244, 0x247, 0x3, 0x2, 
       0x2, 0x2, 0x245, 0x247, 0x5, 0x50, 0x29, 0x2, 0x246, 0x241, 0x3, 
       0x2, 0x2, 0x2, 0x246, 0x245, 0x3, 0x2, 0x2, 0x2, 0x247, 0x24b, 0x3, 
       0x2, 0x2, 0x2, 0x248, 0x24a, 0x5, 0x52, 0x2a, 0x2, 0x249, 0x248, 
       0x3, 0x2, 0x2, 0x2, 0x24a, 0x24d, 0x3, 0x2, 0x2, 0x2, 0x24b, 0x249, 
       0x3, 0x2, 0x2, 0x2, 0x24b, 0x24c, 0x3, 0x2, 0x2, 0x2, 0x24c, 0x24e, 
       0x3, 0x2, 0x2, 0x2, 0x24d, 0x24b, 0x3, 0x2, 0x2, 0x2, 0x24e, 0x24f, 
       0x7, 0xe, 0x2, 0x2, 0x24f, 0x250, 0x5, 0x66, 0x34, 0x2, 0x250, 0x4d, 
       0x3, 0x2, 0x2, 0x2, 0x251, 0x252, 0x7, 0x30, 0x2, 0x2, 0x252, 0x253, 
       0x5, 0x5a, 0x2e, 0x2, 0x253, 0x4f, 0x3, 0x2, 0x2, 0x2, 0x254, 0x255, 
       0x7, 0x31, 0x2, 0x2, 0x255, 0x256, 0x5, 0x5a, 0x2e, 0x2, 0x256, 0x51, 
       0x3, 0x2, 0x2, 0x2, 0x257, 0x259, 0x7, 0x32, 0x2, 0x2, 0x258, 0x25a, 
       0x7, 0xd, 0x2, 0x2, 0x259, 0x258, 0x3, 0x2, 0x2, 0x2, 0x259, 0x25a, 
       0x3, 0x2, 0x2, 0x2, 0x25a, 0x25b, 0x3, 0x2, 0x2, 0x2, 0x25b, 0x25c, 
       0x5, 0x106, 0x84, 0x2, 0x25c, 0x53, 0x3, 0x2, 0x2, 0x2, 0x25d, 0x263, 
       0x7, 0x34, 0x2, 0x2, 0x25e, 0x260, 0x7, 0x17, 0x2, 0x2, 0x25f, 0x25e, 
       0x3, 0x2, 0x2, 0x2, 0x25f, 0x260, 0x3, 0x2, 0x2, 0x2, 0x260, 0x261, 
       0x3, 0x2, 0x2, 0x2, 0x261, 0x263, 0x5, 0x106, 0x84, 0x2, 0x262, 0x25d, 
       0x3, 0x2, 0x2, 0x2, 0x262, 0x25f, 0x3, 0x2, 0x2, 0x2, 0x263, 0x55, 
       0x3, 0x2, 0x2, 0x2, 0x264, 0x265, 0x7, 0x17, 0x2, 0x2, 0x265, 0x266, 
       0x5, 0x106, 0x84, 0x2, 0x266, 0x57, 0x3, 0x2, 0x2, 0x2, 0x267, 0x26c, 
       0x5, 0x56, 0x2c, 0x2, 0x268, 0x26c, 0x7, 0x34, 0x2, 0x2, 0x269, 0x26c, 
       0x7, 0xd, 0x2, 0x2, 0x26a, 0x26c, 0x7, 0x35, 0x2, 0x2, 0x26b, 0x267, 
       0x3, 0x2, 0x2, 0x2, 0x26b, 0x268, 0x3, 0x2, 0x2, 0x2, 0x26b, 0x269, 
       0x3, 0x2, 0x2, 0x2, 0x26b, 0x26a, 0x3, 0x2, 0x2, 0x2, 0x26c, 0x59, 
       0x3, 0x2, 0x2, 0x2, 0x26d, 0x26e, 0x7, 0x93, 0x2, 0x2, 0x26e, 0x26f, 
       0x5, 0x5e, 0x30, 0x2, 0x26f, 0x270, 0x7, 0x94, 0x2, 0x2, 0x270, 0x5b, 
       0x3, 0x2, 0x2, 0x2, 0x271, 0x272, 0x7, 0x93, 0x2, 0x2, 0x272, 0x273, 
       0x5, 0x5e, 0x30, 0x2, 0x273, 0x274, 0x7, 0x94, 0x2, 0x2, 0x274, 0x5d, 
       0x3, 0x2, 0x2, 0x2, 0x275, 0x277, 0x5, 0x64, 0x33, 0x2, 0x276, 0x275, 
       0x3, 0x2, 0x2, 0x2, 0x276, 0x277, 0x3, 0x2, 0x2, 0x2, 0x277, 0x27b, 
       0x3, 0x2, 0x2, 0x2, 0x278, 0x27a, 0x5, 0x60, 0x31, 0x2, 0x279, 0x278, 
       0x3, 0x2, 0x2, 0x2, 0x27a, 0x27d, 0x3, 0x2, 0x2, 0x2, 0x27b, 0x279, 
       0x3, 0x2, 0x2, 0x2, 0x27b, 0x27c, 0x3, 0x2, 0x2, 0x2, 0x27c, 0x5f, 
       0x3, 0x2, 0x2, 0x2, 0x27d, 0x27b, 0x3, 0x2, 0x2, 0x2, 0x27e, 0x280, 
       0x5, 0x62, 0x32, 0x2, 0x27f, 0x281, 0x7, 0x98, 0x2, 0x2, 0x280, 0x27f, 
       0x3, 0x2, 0x2, 0x2, 0x280, 0x281, 0x3, 0x2, 0x2, 0x2, 0x281, 0x283, 
       0x3, 0x2, 0x2, 0x2, 0x282, 0x284, 0x5, 0x64, 0x33, 0x2, 0x283, 0x282, 
       0x3, 0x2, 0x2, 0x2, 0x283, 0x284, 0x3, 0x2, 0x2, 0x2, 0x284, 0x61, 
       0x3, 0x2, 0x2, 0x2, 0x285, 0x286, 0x7, 0x17, 0x2, 0x2, 0x286, 0x287, 
       0x5, 0xd8, 0x6d, 0x2, 0x287, 0x289, 0x7, 0x93, 0x2, 0x2, 0x288, 0x28a, 
       0x5, 0x64, 0x33, 0x2, 0x289, 0x288, 0x3, 0x2, 0x2, 0x2, 0x289, 0x28a, 
       0x3, 0x2, 0x2, 0x2, 0x28a, 0x28b, 0x3, 0x2, 0x2, 0x2, 0x28b, 0x28c, 
       0x7, 0x94, 0x2, 0x2, 0x28c, 0x63, 0x3, 0x2, 0x2, 0x2, 0x28d, 0x294, 
       0x5, 0x96, 0x4c, 0x2, 0x28e, 0x290, 0x7, 0x98, 0x2, 0x2, 0x28f, 0x291, 
       0x5, 0x96, 0x4c, 0x2, 0x290, 0x28f, 0x3, 0x2, 0x2, 0x2, 0x290, 0x291, 
       0x3, 0x2, 0x2, 0x2, 0x291, 0x293, 0x3, 0x2, 0x2, 0x2, 0x292, 0x28e, 
       0x3, 0x2, 0x2, 0x2, 0x293, 0x296, 0x3, 0x2, 0x2, 0x2, 0x294, 0x292, 
       0x3, 0x2, 0x2, 0x2, 0x294, 0x295, 0x3, 0x2, 0x2, 0x2, 0x295, 0x65, 
       0x3, 0x2, 0x2, 0x2, 0x296, 0x294, 0x3, 0x2, 0x2, 0x2, 0x297, 0x29a, 
       0x7, 0x93, 0x2, 0x2, 0x298, 0x29b, 0x5, 0xc, 0x7, 0x2, 0x299, 0x29b, 
       0x5, 0x68, 0x35, 0x2, 0x29a, 0x298, 0x3, 0x2, 0x2, 0x2, 0x29a, 0x299, 
       0x3, 0x2, 0x2, 0x2, 0x29b, 0x29c, 0x3, 0x2, 0x2, 0x2, 0x29c, 0x29d, 
       0x7, 0x94, 0x2, 0x2, 0x29d, 0x67, 0x3, 0x2, 0x2, 0x2, 0x29e, 0x2a0, 
       0x5, 0x6c, 0x37, 0x2, 0x29f, 0x29e, 0x3, 0x2, 0x2, 0x2, 0x29f, 0x2a0, 
       0x3, 0x2, 0x2, 0x2, 0x2a0, 0x2a4, 0x3, 0x2, 0x2, 0x2, 0x2a1, 0x2a3, 
       0x5, 0x6a, 0x36, 0x2, 0x2a2, 0x2a1, 0x3, 0x2, 0x2, 0x2, 0x2a3, 0x2a6, 
       0x3, 0x2, 0x2, 0x2, 0x2a4, 0x2a2, 0x3, 0x2, 0x2, 0x2, 0x2a4, 0x2a5, 
       0x3, 0x2, 0x2, 0x2, 0x2a5, 0x69, 0x3, 0x2, 0x2, 0x2, 0x2a6, 0x2a4, 
       0x3, 0x2, 0x2, 0x2, 0x2a7, 0x2a9, 0x5, 0x6e, 0x38, 0x2, 0x2a8, 0x2aa, 
       0x7, 0x98, 0x2, 0x2, 0x2a9, 0x2a8, 0x3, 0x2, 0x2, 0x2, 0x2a9, 0x2aa, 
       0x3, 0x2, 0x2, 0x2, 0x2aa, 0x2ac, 0x3, 0x2, 0x2, 0x2, 0x2ab, 0x2ad, 
       0x5, 0x6c, 0x37, 0x2, 0x2ac, 0x2ab, 0x3, 0x2, 0x2, 0x2, 0x2ac, 0x2ad, 
       0x3, 0x2, 0x2, 0x2, 0x2ad, 0x6b, 0x3, 0x2, 0x2, 0x2, 0x2ae, 0x2b5, 
       0x5, 0xa2, 0x52, 0x2, 0x2af, 0x2b1, 0x7, 0x98, 0x2, 0x2, 0x2b0, 0x2b2, 
       0x5, 0xa2, 0x52, 0x2, 0x2b1, 0x2b0, 0x3, 0x2, 0x2, 0x2, 0x2b1, 0x2b2, 
       0x3, 0x2, 0x2, 0x2, 0x2b2, 0x2b4, 0x3, 0x2, 0x2, 0x2, 0x2b3, 0x2af, 
       0x3, 0x2, 0x2, 0x2, 0x2b4, 0x2b7, 0x3, 0x2, 0x2, 0x2, 0x2b5, 0x2b3, 
       0x3, 0x2, 0x2, 0x2, 0x2b5, 0x2b6, 0x3, 0x2, 0x2, 0x2, 0x2b6, 0x6d, 
       0x3, 0x2, 0x2, 0x2, 0x2b7, 0x2b5, 0x3, 0x2, 0x2, 0x2, 0x2b8, 0x2c1, 
       0x5, 0x86, 0x44, 0x2, 0x2b9, 0x2c1, 0x5, 0x70, 0x39, 0x2, 0x2ba, 
       0x2c1, 0x5, 0x84, 0x43, 0x2, 0x2bb, 0x2c1, 0x5, 0x72, 0x3a, 0x2, 
       0x2bc, 0x2c1, 0x5, 0x74, 0x3b, 0x2, 0x2bd, 0x2c1, 0x5, 0x88, 0x45, 
       0x2, 0x2be, 0x2c1, 0x5, 0x76, 0x3c, 0x2, 0x2bf, 0x2c1, 0x5, 0x78, 
       0x3d, 0x2, 0x2c0, 0x2b8, 0x3, 0x2, 0x2, 0x2, 0x2c0, 0x2b9, 0x3, 0x2, 
       0x2, 0x2, 0x2c0, 0x2ba, 0x3, 0x2, 0x2, 0x2, 0x2c0, 0x2bb, 0x3, 0x2, 
       0x2, 0x2, 0x2c0, 0x2bc, 0x3, 0x2, 0x2, 0x2, 0x2c0, 0x2bd, 0x3, 0x2, 
       0x2, 0x2, 0x2c0, 0x2be, 0x3, 0x2, 0x2, 0x2, 0x2c0, 0x2bf, 0x3, 0x2, 
       0x2, 0x2, 0x2c1, 0x6f, 0x3, 0x2, 0x2, 0x2, 0x2c2, 0x2c3, 0x7, 0x16, 
       0x2, 0x2, 0x2c3, 0x2c4, 0x5, 0x66, 0x34, 0x2, 0x2c4, 0x71, 0x3, 0x2, 
       0x2, 0x2, 0x2c5, 0x2c6, 0x7, 0x17, 0x2, 0x2, 0x2c6, 0x2c7, 0x5, 0xd8, 
       0x6d, 0x2, 0x2c7, 0x2c8, 0x5, 0x66, 0x34, 0x2, 0x2c8, 0x73, 0x3, 
       0x2, 0x2, 0x2, 0x2c9, 0x2cb, 0x7, 0x3f, 0x2, 0x2, 0x2ca, 0x2cc, 0x7, 
       0x33, 0x2, 0x2, 0x2cb, 0x2ca, 0x3, 0x2, 0x2, 0x2, 0x2cb, 0x2cc, 0x3, 
       0x2, 0x2, 0x2, 0x2cc, 0x2cd, 0x3, 0x2, 0x2, 0x2, 0x2cd, 0x2ce, 0x5, 
       0xd8, 0x6d, 0x2, 0x2ce, 0x2cf, 0x5, 0x66, 0x34, 0x2, 0x2cf, 0x75, 
       0x3, 0x2, 0x2, 0x2, 0x2d0, 0x2d1, 0x7, 0x40, 0x2, 0x2, 0x2d1, 0x2d2, 
       0x7, 0x91, 0x2, 0x2, 0x2d2, 0x2d3, 0x5, 0xe0, 0x71, 0x2, 0x2d3, 0x2d4, 
       0x7, 0x3a, 0x2, 0x2, 0x2d4, 0x2d5, 0x5, 0xda, 0x6e, 0x2, 0x2d5, 0x2d6, 
       0x7, 0x92, 0x2, 0x2, 0x2d6, 0x77, 0x3, 0x2, 0x2, 0x2, 0x2d7, 0x2d8, 
       0x7, 0x15, 0x2, 0x2, 0x2d8, 0x2d9, 0x5, 0x7a, 0x3e, 0x2, 0x2d9, 0x79, 
       0x3, 0x2, 0x2, 0x2, 0x2da, 0x2dd, 0x5, 0x7c, 0x3f, 0x2, 0x2db, 0x2dd, 
       0x5, 0x7e, 0x40, 0x2, 0x2dc, 0x2da, 0x3, 0x2, 0x2, 0x2, 0x2dc, 0x2db, 
       0x3, 0x2, 0x2, 0x2, 0x2dd, 0x7b, 0x3, 0x2, 0x2, 0x2, 0x2de, 0x2df, 
       0x5, 0xda, 0x6e, 0x2, 0x2df, 0x2e3, 0x7, 0x93, 0x2, 0x2, 0x2e0, 0x2e2, 
       0x5, 0x82, 0x42, 0x2, 0x2e1, 0x2e0, 0x3, 0x2, 0x2, 0x2, 0x2e2, 0x2e5, 
       0x3, 0x2, 0x2, 0x2, 0x2e3, 0x2e1, 0x3, 0x2, 0x2, 0x2, 0x2e3, 0x2e4, 
       0x3, 0x2, 0x2, 0x2, 0x2e4, 0x2e6, 0x3, 0x2, 0x2, 0x2, 0x2e5, 0x2e3, 
       0x3, 0x2, 0x2, 0x2, 0x2e6, 0x2e7, 0x7, 0x94, 0x2, 0x2, 0x2e7, 0x7d, 
       0x3, 0x2, 0x2, 0x2, 0x2e8, 0x2ec, 0x7, 0x91, 0x2, 0x2, 0x2e9, 0x2eb, 
       0x5, 0xda, 0x6e, 0x2, 0x2ea, 0x2e9, 0x3, 0x2, 0x2, 0x2, 0x2eb, 0x2ee, 
       0x3, 0x2, 0x2, 0x2, 0x2ec, 0x2ea, 0x3, 0x2, 0x2, 0x2, 0x2ec, 0x2ed, 
       0x3, 0x2, 0x2, 0x2, 0x2ed, 0x2ef, 0x3, 0x2, 0x2, 0x2, 0x2ee, 0x2ec, 
       0x3, 0x2, 0x2, 0x2, 0x2ef, 0x2f0, 0x7, 0x92, 0x2, 0x2, 0x2f0, 0x2f4, 
       0x7, 0x93, 0x2, 0x2, 0x2f1, 0x2f3, 0x5, 0x80, 0x41, 0x2, 0x2f2, 0x2f1, 
       0x3, 0x2, 0x2, 0x2, 0x2f3, 0x2f6, 0x3, 0x2, 0x2, 0x2, 0x2f4, 0x2f2, 
       0x3, 0x2, 0x2, 0x2, 0x2f4, 0x2f5, 0x3, 0x2, 0x2, 0x2, 0x2f5, 0x2f7, 
       0x3, 0x2, 0x2, 0x2, 0x2f6, 0x2f4, 0x3, 0x2, 0x2, 0x2, 0x2f7, 0x2f8, 
       0x7, 0x94, 0x2, 0x2, 0x2f8, 0x7f, 0x3, 0x2, 0x2, 0x2, 0x2f9, 0x2fd, 
       0x7, 0x91, 0x2, 0x2, 0x2fa, 0x2fc, 0x5, 0x82, 0x42, 0x2, 0x2fb, 0x2fa, 
       0x3, 0x2, 0x2, 0x2, 0x2fc, 0x2ff, 0x3, 0x2, 0x2, 0x2, 0x2fd, 0x2fb, 
       0x3, 0x2, 0x2, 0x2, 0x2fd, 0x2fe, 0x3, 0x2, 0x2, 0x2, 0x2fe, 0x300, 
       0x3, 0x2, 0x2, 0x2, 0x2ff, 0x2fd, 0x3, 0x2, 0x2, 0x2, 0x300, 0x301, 
       0x7, 0x92, 0x2, 0x2, 0x301, 0x81, 0x3, 0x2, 0x2, 0x2, 0x302, 0x308, 
       0x5, 0x106, 0x84, 0x2, 0x303, 0x308, 0x5, 0xf8, 0x7d, 0x2, 0x304, 
       0x308, 0x5, 0xfa, 0x7e, 0x2, 0x305, 0x308, 0x5, 0x102, 0x82, 0x2, 
       0x306, 0x308, 0x7, 0x3d, 0x2, 0x2, 0x307, 0x302, 0x3, 0x2, 0x2, 0x2, 
       0x307, 0x303, 0x3, 0x2, 0x2, 0x2, 0x307, 0x304, 0x3, 0x2, 0x2, 0x2, 
       0x307, 0x305, 0x3, 0x2, 0x2, 0x2, 0x307, 0x306, 0x3, 0x2, 0x2, 0x2, 
       0x308, 0x83, 0x3, 0x2, 0x2, 0x2, 0x309, 0x30a, 0x7, 0x41, 0x2, 0x2, 
       0x30a, 0x30b, 0x5, 0x66, 0x34, 0x2, 0x30b, 0x85, 0x3, 0x2, 0x2, 0x2, 
       0x30c, 0x311, 0x5, 0x66, 0x34, 0x2, 0x30d, 0x30e, 0x7, 0x18, 0x2, 
       0x2, 0x30e, 0x310, 0x5, 0x66, 0x34, 0x2, 0x30f, 0x30d, 0x3, 0x2, 
       0x2, 0x2, 0x310, 0x313, 0x3, 0x2, 0x2, 0x2, 0x311, 0x30f, 0x3, 0x2, 
       0x2, 0x2, 0x311, 0x312, 0x3, 0x2, 0x2, 0x2, 0x312, 0x87, 0x3, 0x2, 
       0x2, 0x2, 0x313, 0x311, 0x3, 0x2, 0x2, 0x2, 0x314, 0x315, 0x7, 0x19, 
       0x2, 0x2, 0x315, 0x316, 0x5, 0x8a, 0x46, 0x2, 0x316, 0x89, 0x3, 0x2, 
       0x2, 0x2, 0x317, 0x318, 0x7, 0x91, 0x2, 0x2, 0x318, 0x319, 0x5, 0xe0, 
       0x71, 0x2, 0x319, 0x31a, 0x7, 0x92, 0x2, 0x2, 0x31a, 0x31e, 0x3, 
       0x2, 0x2, 0x2, 0x31b, 0x31e, 0x5, 0xe8, 0x75, 0x2, 0x31c, 0x31e, 
       0x5, 0x8c, 0x47, 0x2, 0x31d, 0x317, 0x3, 0x2, 0x2, 0x2, 0x31d, 0x31b, 
       0x3, 0x2, 0x2, 0x2, 0x31d, 0x31c, 0x3, 0x2, 0x2, 0x2, 0x31e, 0x8b, 
       0x3, 0x2, 0x2, 0x2, 0x31f, 0x320, 0x5, 0x106, 0x84, 0x2, 0x320, 0x321, 
       0x5, 0x8e, 0x48, 0x2, 0x321, 0x8d, 0x3, 0x2, 0x2, 0x2, 0x322, 0x328, 
       0x7, 0x91, 0x2, 0x2, 0x323, 0x325, 0x7, 0x7, 0x2, 0x2, 0x324, 0x323, 
       0x3, 0x2, 0x2, 0x2, 0x324, 0x325, 0x3, 0x2, 0x2, 0x2, 0x325, 0x326, 
       0x3, 0x2, 0x2, 0x2, 0x326, 0x329, 0x5, 0x90, 0x49, 0x2, 0x327, 0x329, 
       0x3, 0x2, 0x2, 0x2, 0x328, 0x324, 0x3, 0x2, 0x2, 0x2, 0x328, 0x327, 
       0x3, 0x2, 0x2, 0x2, 0x329, 0x32a, 0x3, 0x2, 0x2, 0x2, 0x32a, 0x32b, 
       0x7, 0x92, 0x2, 0x2, 0x32b, 0x8f, 0x3, 0x2, 0x2, 0x2, 0x32c, 0x331, 
       0x5, 0xe0, 0x71, 0x2, 0x32d, 0x32e, 0x7, 0x9d, 0x2, 0x2, 0x32e, 0x330, 
       0x5, 0xe0, 0x71, 0x2, 0x32f, 0x32d, 0x3, 0x2, 0x2, 0x2, 0x330, 0x333, 
       0x3, 0x2, 0x2, 0x2, 0x331, 0x32f, 0x3, 0x2, 0x2, 0x2, 0x331, 0x332, 
       0x3, 0x2, 0x2, 0x2, 0x332, 0x91, 0x3, 0x2, 0x2, 0x2, 0x333, 0x331, 
       0x3, 0x2, 0x2, 0x2, 0x334, 0x336, 0x7, 0x93, 0x2, 0x2, 0x335, 0x337, 
       0x5, 0x94, 0x4b, 0x2, 0x336, 0x335, 0x3, 0x2, 0x2, 0x2, 0x336, 0x337, 
       0x3, 0x2, 0x2, 0x2, 0x337, 0x338, 0x3, 0x2, 0x2, 0x2, 0x338, 0x339, 
       0x7, 0x94, 0x2, 0x2, 0x339, 0x93, 0x3, 0x2, 0x2, 0x2, 0x33a, 0x341, 
       0x5, 0x96, 0x4c, 0x2, 0x33b, 0x33d, 0x7, 0x98, 0x2, 0x2, 0x33c, 0x33e, 
       0x5, 0x94, 0x4b, 0x2, 0x33d, 0x33c, 0x3, 0x2, 0x2, 0x2, 0x33d, 0x33e, 
       0x3, 0x2, 0x2, 0x2, 0x33e, 0x340, 0x3, 0x2, 0x2, 0x2, 0x33f, 0x33b, 
       0x3, 0x2, 0x2, 0x2, 0x340, 0x343, 0x3, 0x2, 0x2, 0x2, 0x341, 0x33f, 
       0x3, 0x2, 0x2, 0x2, 0x341, 0x342, 0x3, 0x2, 0x2, 0x2, 0x342, 0x95, 
       0x3, 0x2, 0x2, 0x2, 0x343, 0x341, 0x3, 0x2, 0x2, 0x2, 0x344, 0x345, 
       0x5, 0xd6, 0x6c, 0x2, 0x345, 0x346, 0x5, 0x9a, 0x4e, 0x2, 0x346, 
       0x34b, 0x3, 0x2, 0x2, 0x2, 0x347, 0x348, 0x5, 0xc6, 0x64, 0x2, 0x348, 
       0x349, 0x5, 0x98, 0x4d, 0x2, 0x349, 0x34b, 0x3, 0x2, 0x2, 0x2, 0x34a, 
       0x344, 0x3, 0x2, 0x2, 0x2, 0x34a, 0x347, 0x3, 0x2, 0x2, 0x2, 0x34b, 
       0x97, 0x3, 0x2, 0x2, 0x2, 0x34c, 0x34e, 0x5, 0x9a, 0x4e, 0x2, 0x34d, 
       0x34c, 0x3, 0x2, 0x2, 0x2, 0x34d, 0x34e, 0x3, 0x2, 0x2, 0x2, 0x34e, 
       0x99, 0x3, 0x2, 0x2, 0x2, 0x34f, 0x350, 0x5, 0x9c, 0x4f, 0x2, 0x350, 
       0x359, 0x5, 0x9e, 0x50, 0x2, 0x351, 0x355, 0x7, 0x97, 0x2, 0x2, 0x352, 
       0x353, 0x5, 0x9c, 0x4f, 0x2, 0x353, 0x354, 0x5, 0x9e, 0x50, 0x2, 
       0x354, 0x356, 0x3, 0x2, 0x2, 0x2, 0x355, 0x352, 0x3, 0x2, 0x2, 0x2, 
       0x355, 0x356, 0x3, 0x2, 0x2, 0x2, 0x356, 0x358, 0x3, 0x2, 0x2, 0x2, 
       0x357, 0x351, 0x3, 0x2, 0x2, 0x2, 0x358, 0x35b, 0x3, 0x2, 0x2, 0x2, 
       0x359, 0x357, 0x3, 0x2, 0x2, 0x2, 0x359, 0x35a, 0x3, 0x2, 0x2, 0x2, 
       0x35a, 0x9b, 0x3, 0x2, 0x2, 0x2, 0x35b, 0x359, 0x3, 0x2, 0x2, 0x2, 
       0x35c, 0x35f, 0x5, 0xd8, 0x6d, 0x2, 0x35d, 0x35f, 0x7, 0x1a, 0x2, 
       0x2, 0x35e, 0x35c, 0x3, 0x2, 0x2, 0x2, 0x35e, 0x35d, 0x3, 0x2, 0x2, 
       0x2, 0x35f, 0x9d, 0x3, 0x2, 0x2, 0x2, 0x360, 0x365, 0x5, 0xa0, 0x51, 
       0x2, 0x361, 0x362, 0x7, 0x9d, 0x2, 0x2, 0x362, 0x364, 0x5, 0xa0, 
       0x51, 0x2, 0x363, 0x361, 0x3, 0x2, 0x2, 0x2, 0x364, 0x367, 0x3, 0x2, 
       0x2, 0x2, 0x365, 0x363, 0x3, 0x2, 0x2, 0x2, 0x365, 0x366, 0x3, 0x2, 
       0x2, 0x2, 0x366, 0x9f, 0x3, 0x2, 0x2, 0x2, 0x367, 0x365, 0x3, 0x2, 
       0x2, 0x2, 0x368, 0x369, 0x5, 0xd2, 0x6a, 0x2, 0x369, 0xa1, 0x3, 0x2, 
       0x2, 0x2, 0x36a, 0x36b, 0x5, 0xd6, 0x6c, 0x2, 0x36b, 0x36c, 0x5, 
       0xa6, 0x54, 0x2, 0x36c, 0x371, 0x3, 0x2, 0x2, 0x2, 0x36d, 0x36e, 
       0x5, 0xca, 0x66, 0x2, 0x36e, 0x36f, 0x5, 0xa4, 0x53, 0x2, 0x36f, 
       0x371, 0x3, 0x2, 0x2, 0x2, 0x370, 0x36a, 0x3, 0x2, 0x2, 0x2, 0x370, 
       0x36d, 0x3, 0x2, 0x2, 0x2, 0x371, 0xa3, 0x3, 0x2, 0x2, 0x2, 0x372, 
       0x374, 0x5, 0xa6, 0x54, 0x2, 0x373, 0x372, 0x3, 0x2, 0x2, 0x2, 0x373, 
       0x374, 0x3, 0x2, 0x2, 0x2, 0x374, 0xa5, 0x3, 0x2, 0x2, 0x2, 0x375, 
       0x378, 0x5, 0xaa, 0x56, 0x2, 0x376, 0x378, 0x5, 0xac, 0x57, 0x2, 
       0x377, 0x375, 0x3, 0x2, 0x2, 0x2, 0x377, 0x376, 0x3, 0x2, 0x2, 0x2, 
       0x378, 0x379, 0x3, 0x2, 0x2, 0x2, 0x379, 0x380, 0x5, 0xae, 0x58, 
       0x2, 0x37a, 0x37c, 0x7, 0x97, 0x2, 0x2, 0x37b, 0x37d, 0x5, 0xa8, 
       0x55, 0x2, 0x37c, 0x37b, 0x3, 0x2, 0x2, 0x2, 0x37c, 0x37d, 0x3, 0x2, 
       0x2, 0x2, 0x37d, 0x37f, 0x3, 0x2, 0x2, 0x2, 0x37e, 0x37a, 0x3, 0x2, 
       0x2, 0x2, 0x37f, 0x382, 0x3, 0x2, 0x2, 0x2, 0x380, 0x37e, 0x3, 0x2, 
       0x2, 0x2, 0x380, 0x381, 0x3, 0x2, 0x2, 0x2, 0x381, 0xa7, 0x3, 0x2, 
       0x2, 0x2, 0x382, 0x380, 0x3, 0x2, 0x2, 0x2, 0x383, 0x386, 0x5, 0xaa, 
       0x56, 0x2, 0x384, 0x386, 0x5, 0xac, 0x57, 0x2, 0x385, 0x383, 0x3, 
       0x2, 0x2, 0x2, 0x385, 0x384, 0x3, 0x2, 0x2, 0x2, 0x386, 0x387, 0x3, 
       0x2, 0x2, 0x2, 0x387, 0x388, 0x5, 0x9e, 0x50, 0x2, 0x388, 0xa9, 0x3, 
       0x2, 0x2, 0x2, 0x389, 0x38a, 0x5, 0xb2, 0x5a, 0x2, 0x38a, 0xab, 0x3, 
       0x2, 0x2, 0x2, 0x38b, 0x38c, 0x5, 0xda, 0x6e, 0x2, 0x38c, 0xad, 0x3, 
       0x2, 0x2, 0x2, 0x38d, 0x392, 0x5, 0xb0, 0x59, 0x2, 0x38e, 0x38f, 
       0x7, 0x9d, 0x2, 0x2, 0x38f, 0x391, 0x5, 0xb0, 0x59, 0x2, 0x390, 0x38e, 
       0x3, 0x2, 0x2, 0x2, 0x391, 0x394, 0x3, 0x2, 0x2, 0x2, 0x392, 0x390, 
       0x3, 0x2, 0x2, 0x2, 0x392, 0x393, 0x3, 0x2, 0x2, 0x2, 0x393, 0xaf, 
       0x3, 0x2, 0x2, 0x2, 0x394, 0x392, 0x3, 0x2, 0x2, 0x2, 0x395, 0x396, 
       0x5, 0xd4, 0x6b, 0x2, 0x396, 0xb1, 0x3, 0x2, 0x2, 0x2, 0x397, 0x398, 
       0x5, 0xb4, 0x5b, 0x2, 0x398, 0xb3, 0x3, 0x2, 0x2, 0x2, 0x399, 0x39e, 
       0x5, 0xb6, 0x5c, 0x2, 0x39a, 0x39b, 0x7, 0xa3, 0x2, 0x2, 0x39b, 0x39d, 
       0x5, 0xb6, 0x5c, 0x2, 0x39c, 0x39a, 0x3, 0x2, 0x2, 0x2, 0x39d, 0x3a0, 
       0x3, 0x2, 0x2, 0x2, 0x39e, 0x39c, 0x3, 0x2, 0x2, 0x2, 0x39e, 0x39f, 
       0x3, 0x2, 0x2, 0x2, 0x39f, 0xb5, 0x3, 0x2, 0x2, 0x2, 0x3a0, 0x39e, 
       0x3, 0x2, 0x2, 0x2, 0x3a1, 0x3a6, 0x5, 0xba, 0x5e, 0x2, 0x3a2, 0x3a3, 
       0x7, 0x9f, 0x2, 0x2, 0x3a3, 0x3a5, 0x5, 0xba, 0x5e, 0x2, 0x3a4, 0x3a2, 
       0x3, 0x2, 0x2, 0x2, 0x3a5, 0x3a8, 0x3, 0x2, 0x2, 0x2, 0x3a6, 0x3a4, 
       0x3, 0x2, 0x2, 0x2, 0x3a6, 0x3a7, 0x3, 0x2, 0x2, 0x2, 0x3a7, 0xb7, 
       0x3, 0x2, 0x2, 0x2, 0x3a8, 0x3a6, 0x3, 0x2, 0x2, 0x2, 0x3a9, 0x3ab, 
       0x5, 0xbe, 0x60, 0x2, 0x3aa, 0x3ac, 0x5, 0xbc, 0x5f, 0x2, 0x3ab, 
       0x3aa, 0x3, 0x2, 0x2, 0x2, 0x3ab, 0x3ac, 0x3, 0x2, 0x2, 0x2, 0x3ac, 
       0xb9, 0x3, 0x2, 0x2, 0x2, 0x3ad, 0x3af, 0x7, 0x90, 0x2, 0x2, 0x3ae, 
       0x3ad, 0x3, 0x2, 0x2, 0x2, 0x3ae, 0x3af, 0x3, 0x2, 0x2, 0x2, 0x3af, 
       0x3b0, 0x3, 0x2, 0x2, 0x2, 0x3b0, 0x3b1, 0x5, 0xb8, 0x5d, 0x2, 0x3b1, 
       0xbb, 0x3, 0x2, 0x2, 0x2, 0x3b2, 0x3b3, 0x9, 0x4, 0x2, 0x2, 0x3b3, 
       0xbd, 0x3, 0x2, 0x2, 0x2, 0x3b4, 0x3bd, 0x5, 0x106, 0x84, 0x2, 0x3b5, 
       0x3bd, 0x7, 0x1a, 0x2, 0x2, 0x3b6, 0x3b7, 0x7, 0x9e, 0x2, 0x2, 0x3b7, 
       0x3bd, 0x5, 0xc0, 0x61, 0x2, 0x3b8, 0x3b9, 0x7, 0x91, 0x2, 0x2, 0x3b9, 
       0x3ba, 0x5, 0xb2, 0x5a, 0x2, 0x3ba, 0x3bb, 0x7, 0x92, 0x2, 0x2, 0x3bb, 
       0x3bd, 0x3, 0x2, 0x2, 0x2, 0x3bc, 0x3b4, 0x3, 0x2, 0x2, 0x2, 0x3bc, 
       0x3b5, 0x3, 0x2, 0x2, 0x2, 0x3bc, 0x3b6, 0x3, 0x2, 0x2, 0x2, 0x3bc, 
       0x3b8, 0x3, 0x2, 0x2, 0x2, 0x3bd, 0xbf, 0x3, 0x2, 0x2, 0x2, 0x3be, 
       0x3cc, 0x5, 0xc2, 0x62, 0x2, 0x3bf, 0x3c8, 0x7, 0x91, 0x2, 0x2, 0x3c0, 
       0x3c5, 0x5, 0xc2, 0x62, 0x2, 0x3c1, 0x3c2, 0x7, 0xa3, 0x2, 0x2, 0x3c2, 
       0x3c4, 0x5, 0xc2, 0x62, 0x2, 0x3c3, 0x3c1, 0x3, 0x2, 0x2, 0x2, 0x3c4, 
       0x3c7, 0x3, 0x2, 0x2, 0x2, 0x3c5, 0x3c3, 0x3, 0x2, 0x2, 0x2, 0x3c5, 
       0x3c6, 0x3, 0x2, 0x2, 0x2, 0x3c6, 0x3c9, 0x3, 0x2, 0x2, 0x2, 0x3c7, 
       0x3c5, 0x3, 0x2, 0x2, 0x2, 0x3c8, 0x3c0, 0x3, 0x2, 0x2, 0x2, 0x3c8, 
       0x3c9, 0x3, 0x2, 0x2, 0x2, 0x3c9, 0x3ca, 0x3, 0x2, 0x2, 0x2, 0x3ca, 
       0x3cc, 0x7, 0x92, 0x2, 0x2, 0x3cb, 0x3be, 0x3, 0x2, 0x2, 0x2, 0x3cb, 
       0x3bf, 0x3, 0x2, 0x2, 0x2, 0x3cc, 0xc1, 0x3, 0x2, 0x2, 0x2, 0x3cd, 
       0x3cf, 0x7, 0x90, 0x2, 0x2, 0x3ce, 0x3cd, 0x3, 0x2, 0x2, 0x2, 0x3ce, 
       0x3cf, 0x3, 0x2, 0x2, 0x2, 0x3cf, 0x3d2, 0x3, 0x2, 0x2, 0x2, 0x3d0, 
       0x3d3, 0x5, 0x106, 0x84, 0x2, 0x3d1, 0x3d3, 0x7, 0x1a, 0x2, 0x2, 
       0x3d2, 0x3d0, 0x3, 0x2, 0x2, 0x2, 0x3d2, 0x3d1, 0x3, 0x2, 0x2, 0x2, 
       0x3d3, 0xc3, 0x3, 0x2, 0x2, 0x2, 0x3d4, 0x3d5, 0x7, 0x7c, 0x2, 0x2, 
       0x3d5, 0xc5, 0x3, 0x2, 0x2, 0x2, 0x3d6, 0x3d9, 0x5, 0xce, 0x68, 0x2, 
       0x3d7, 0x3d9, 0x5, 0xc8, 0x65, 0x2, 0x3d8, 0x3d6, 0x3, 0x2, 0x2, 
       0x2, 0x3d8, 0x3d7, 0x3, 0x2, 0x2, 0x2, 0x3d9, 0xc7, 0x3, 0x2, 0x2, 
       0x2, 0x3da, 0x3db, 0x7, 0x95, 0x2, 0x2, 0x3db, 0x3dc, 0x5, 0x9a, 
       0x4e, 0x2, 0x3dc, 0x3dd, 0x7, 0x96, 0x2, 0x2, 0x3dd, 0xc9, 0x3, 0x2, 
       0x2, 0x2, 0x3de, 0x3e1, 0x5, 0xd0, 0x69, 0x2, 0x3df, 0x3e1, 0x5, 
       0xcc, 0x67, 0x2, 0x3e0, 0x3de, 0x3, 0x2, 0x2, 0x2, 0x3e0, 0x3df, 
       0x3, 0x2, 0x2, 0x2, 0x3e1, 0xcb, 0x3, 0x2, 0x2, 0x2, 0x3e2, 0x3e3, 
       0x7, 0x95, 0x2, 0x2, 0x3e3, 0x3e4, 0x5, 0xa6, 0x54, 0x2, 0x3e4, 0x3e5, 
       0x7, 0x96, 0x2, 0x2, 0x3e5, 0xcd, 0x3, 0x2, 0x2, 0x2, 0x3e6, 0x3e8, 
       0x7, 0x91, 0x2, 0x2, 0x3e7, 0x3e9, 0x5, 0xd2, 0x6a, 0x2, 0x3e8, 0x3e7, 
       0x3, 0x2, 0x2, 0x2, 0x3e9, 0x3ea, 0x3, 0x2, 0x2, 0x2, 0x3ea, 0x3e8, 
       0x3, 0x2, 0x2, 0x2, 0x3ea, 0x3eb, 0x3, 0x2, 0x2, 0x2, 0x3eb, 0x3ec, 
       0x3, 0x2, 0x2, 0x2, 0x3ec, 0x3ed, 0x7, 0x92, 0x2, 0x2, 0x3ed, 0xcf, 
       0x3, 0x2, 0x2, 0x2, 0x3ee, 0x3f0, 0x7, 0x91, 0x2, 0x2, 0x3ef, 0x3f1, 
       0x5, 0xd4, 0x6b, 0x2, 0x3f0, 0x3ef, 0x3, 0x2, 0x2, 0x2, 0x3f1, 0x3f2, 
       0x3, 0x2, 0x2, 0x2, 0x3f2, 0x3f0, 0x3, 0x2, 0x2, 0x2, 0x3f2, 0x3f3, 
       0x3, 0x2, 0x2, 0x2, 0x3f3, 0x3f4, 0x3, 0x2, 0x2, 0x2, 0x3f4, 0x3f5, 
       0x7, 0x92, 0x2, 0x2, 0x3f5, 0xd1, 0x3, 0x2, 0x2, 0x2, 0x3f6, 0x3f9, 
       0x5, 0xd6, 0x6c, 0x2, 0x3f7, 0x3f9, 0x5, 0xc6, 0x64, 0x2, 0x3f8, 
       0x3f6, 0x3, 0x2, 0x2, 0x2, 0x3f8, 0x3f7, 0x3, 0x2, 0x2, 0x2, 0x3f9, 
       0xd3, 0x3, 0x2, 0x2, 0x2, 0x3fa, 0x3fd, 0x5, 0xd6, 0x6c, 0x2, 0x3fb, 
       0x3fd, 0x5, 0xca, 0x66, 0x2, 0x3fc, 0x3fa, 0x3, 0x2, 0x2, 0x2, 0x3fc, 
       0x3fb, 0x3, 0x2, 0x2, 0x2, 0x3fd, 0xd5, 0x3, 0x2, 0x2, 0x2, 0x3fe, 
       0x401, 0x5, 0xda, 0x6e, 0x2, 0x3ff, 0x401, 0x5, 0xdc, 0x6f, 0x2, 
       0x400, 0x3fe, 0x3, 0x2, 0x2, 0x2, 0x400, 0x3ff, 0x3, 0x2, 0x2, 0x2, 
       0x401, 0xd7, 0x3, 0x2, 0x2, 0x2, 0x402, 0x405, 0x5, 0xda, 0x6e, 0x2, 
       0x403, 0x405, 0x5, 0x106, 0x84, 0x2, 0x404, 0x402, 0x3, 0x2, 0x2, 
       0x2, 0x404, 0x403, 0x3, 0x2, 0x2, 0x2, 0x405, 0xd9, 0x3, 0x2, 0x2, 
       0x2, 0x406, 0x407, 0x9, 0x5, 0x2, 0x2, 0x407, 0xdb, 0x3, 0x2, 0x2, 
       0x2, 0x408, 0x40f, 0x5, 0x106, 0x84, 0x2, 0x409, 0x40f, 0x5, 0xf8, 
       0x7d, 0x2, 0x40a, 0x40f, 0x5, 0xfa, 0x7e, 0x2, 0x40b, 0x40f, 0x5, 
       0x102, 0x82, 0x2, 0x40c, 0x40f, 0x5, 0x10a, 0x86, 0x2, 0x40d, 0x40f, 
       0x5, 0xde, 0x70, 0x2, 0x40e, 0x408, 0x3, 0x2, 0x2, 0x2, 0x40e, 0x409, 
       0x3, 0x2, 0x2, 0x2, 0x40e, 0x40a, 0x3, 0x2, 0x2, 0x2, 0x40e, 0x40b, 
       0x3, 0x2, 0x2, 0x2, 0x40e, 0x40c, 0x3, 0x2, 0x2, 0x2, 0x40e, 0x40d, 
       0x3, 0x2, 0x2, 0x2, 0x40f, 0xdd, 0x3, 0x2, 0x2, 0x2, 0x410, 0x411, 
       0x7, 0x91, 0x2, 0x2, 0x411, 0x412, 0x7, 0x92, 0x2, 0x2, 0x412, 0xdf, 
       0x3, 0x2, 0x2, 0x2, 0x413, 0x414, 0x8, 0x71, 0x1, 0x2, 0x414, 0x41c, 
       0x5, 0xe6, 0x74, 0x2, 0x415, 0x416, 0x9, 0x6, 0x2, 0x2, 0x416, 0x41c, 
       0x5, 0xe0, 0x71, 0xc, 0x417, 0x418, 0x9, 0x7, 0x2, 0x2, 0x418, 0x41c, 
       0x5, 0xe0, 0x71, 0xb, 0x419, 0x41a, 0x7, 0x9e, 0x2, 0x2, 0x41a, 0x41c, 
       0x5, 0xe0, 0x71, 0xa, 0x41b, 0x413, 0x3, 0x2, 0x2, 0x2, 0x41b, 0x415, 
       0x3, 0x2, 0x2, 0x2, 0x41b, 0x417, 0x3, 0x2, 0x2, 0x2, 0x41b, 0x419, 
       0x3, 0x2, 0x2, 0x2, 0x41c, 0x43a, 0x3, 0x2, 0x2, 0x2, 0x41d, 0x41e, 
       0xc, 0x9, 0x2, 0x2, 0x41e, 0x41f, 0x9, 0x6, 0x2, 0x2, 0x41f, 0x439, 
       0x5, 0xe0, 0x71, 0xa, 0x420, 0x421, 0xc, 0x8, 0x2, 0x2, 0x421, 0x422, 
       0x9, 0x7, 0x2, 0x2, 0x422, 0x439, 0x5, 0xe0, 0x71, 0x9, 0x423, 0x424, 
       0xc, 0x5, 0x2, 0x2, 0x424, 0x425, 0x9, 0x8, 0x2, 0x2, 0x425, 0x439, 
       0x5, 0xe0, 0x71, 0x6, 0x426, 0x427, 0xc, 0x7, 0x2, 0x2, 0x427, 0x439, 
       0x5, 0xe2, 0x72, 0x2, 0x428, 0x42a, 0xc, 0x6, 0x2, 0x2, 0x429, 0x42b, 
       0x7, 0x71, 0x2, 0x2, 0x42a, 0x429, 0x3, 0x2, 0x2, 0x2, 0x42a, 0x42b, 
       0x3, 0x2, 0x2, 0x2, 0x42b, 0x42c, 0x3, 0x2, 0x2, 0x2, 0x42c, 0x42d, 
       0x7, 0x72, 0x2, 0x2, 0x42d, 0x42f, 0x7, 0x91, 0x2, 0x2, 0x42e, 0x430, 
       0x5, 0x90, 0x49, 0x2, 0x42f, 0x42e, 0x3, 0x2, 0x2, 0x2, 0x42f, 0x430, 
       0x3, 0x2, 0x2, 0x2, 0x430, 0x431, 0x3, 0x2, 0x2, 0x2, 0x431, 0x439, 
       0x7, 0x92, 0x2, 0x2, 0x432, 0x433, 0xc, 0x4, 0x2, 0x2, 0x433, 0x434, 
       0x7, 0x8e, 0x2, 0x2, 0x434, 0x439, 0x5, 0xe0, 0x71, 0x2, 0x435, 0x436, 
       0xc, 0x3, 0x2, 0x2, 0x436, 0x437, 0x7, 0x8f, 0x2, 0x2, 0x437, 0x439, 
       0x5, 0xe0, 0x71, 0x2, 0x438, 0x41d, 0x3, 0x2, 0x2, 0x2, 0x438, 0x420, 
       0x3, 0x2, 0x2, 0x2, 0x438, 0x423, 0x3, 0x2, 0x2, 0x2, 0x438, 0x426, 
       0x3, 0x2, 0x2, 0x2, 0x438, 0x428, 0x3, 0x2, 0x2, 0x2, 0x438, 0x432, 
       0x3, 0x2, 0x2, 0x2, 0x438, 0x435, 0x3, 0x2, 0x2, 0x2, 0x439, 0x43c, 
       0x3, 0x2, 0x2, 0x2, 0x43a, 0x438, 0x3, 0x2, 0x2, 0x2, 0x43a, 0x43b, 
       0x3, 0x2, 0x2, 0x2, 0x43b, 0xe1, 0x3, 0x2, 0x2, 0x2, 0x43c, 0x43a, 
       0x3, 0x2, 0x2, 0x2, 0x43d, 0x440, 0x5, 0xfe, 0x80, 0x2, 0x43e, 0x440, 
       0x5, 0x100, 0x81, 0x2, 0x43f, 0x43d, 0x3, 0x2, 0x2, 0x2, 0x43f, 0x43e, 
       0x3, 0x2, 0x2, 0x2, 0x440, 0x443, 0x3, 0x2, 0x2, 0x2, 0x441, 0x442, 
       0x9, 0x6, 0x2, 0x2, 0x442, 0x444, 0x5, 0xe4, 0x73, 0x2, 0x443, 0x441, 
       0x3, 0x2, 0x2, 0x2, 0x443, 0x444, 0x3, 0x2, 0x2, 0x2, 0x444, 0xe3, 
       0x3, 0x2, 0x2, 0x2, 0x445, 0x447, 0x9, 0x9, 0x2, 0x2, 0x446, 0x445, 
       0x3, 0x2, 0x2, 0x2, 0x446, 0x447, 0x3, 0x2, 0x2, 0x2, 0x447, 0x448, 
       0x3, 0x2, 0x2, 0x2, 0x448, 0x449, 0x5, 0xe6, 0x74, 0x2, 0x449, 0xe5, 
       0x3, 0x2, 0x2, 0x2, 0x44a, 0x44b, 0x7, 0x91, 0x2, 0x2, 0x44b, 0x44c, 
       0x5, 0xe0, 0x71, 0x2, 0x44c, 0x44d, 0x7, 0x92, 0x2, 0x2, 0x44d, 0x455, 
       0x3, 0x2, 0x2, 0x2, 0x44e, 0x455, 0x5, 0xe8, 0x75, 0x2, 0x44f, 0x455, 
       0x5, 0xf6, 0x7c, 0x2, 0x450, 0x455, 0x5, 0xf8, 0x7d, 0x2, 0x451, 
       0x455, 0x5, 0xfa, 0x7e, 0x2, 0x452, 0x455, 0x5, 0x102, 0x82, 0x2, 
       0x453, 0x455, 0x5, 0xda, 0x6e, 0x2, 0x454, 0x44a, 0x3, 0x2, 0x2, 
       0x2, 0x454, 0x44e, 0x3, 0x2, 0x2, 0x2, 0x454, 0x44f, 0x3, 0x2, 0x2, 
       0x2, 0x454, 0x450, 0x3, 0x2, 0x2, 0x2, 0x454, 0x451, 0x3, 0x2, 0x2, 
       0x2, 0x454, 0x452, 0x3, 0x2, 0x2, 0x2, 0x454, 0x453, 0x3, 0x2, 0x2, 
       0x2, 0x455, 0xe7, 0x3, 0x2, 0x2, 0x2, 0x456, 0x563, 0x5, 0xf4, 0x7b, 
       0x2, 0x457, 0x458, 0x7, 0x1b, 0x2, 0x2, 0x458, 0x459, 0x7, 0x91, 
       0x2, 0x2, 0x459, 0x45a, 0x5, 0xe0, 0x71, 0x2, 0x45a, 0x45b, 0x7, 
       0x92, 0x2, 0x2, 0x45b, 0x563, 0x3, 0x2, 0x2, 0x2, 0x45c, 0x45d, 0x7, 
       0x1c, 0x2, 0x2, 0x45d, 0x45e, 0x7, 0x91, 0x2, 0x2, 0x45e, 0x45f, 
       0x5, 0xe0, 0x71, 0x2, 0x45f, 0x460, 0x7, 0x92, 0x2, 0x2, 0x460, 0x563, 
       0x3, 0x2, 0x2, 0x2, 0x461, 0x462, 0x7, 0x1d, 0x2, 0x2, 0x462, 0x463, 
       0x7, 0x91, 0x2, 0x2, 0x463, 0x464, 0x5, 0xe0, 0x71, 0x2, 0x464, 0x465, 
       0x7, 0x9d, 0x2, 0x2, 0x465, 0x466, 0x5, 0xe0, 0x71, 0x2, 0x466, 0x467, 
       0x7, 0x92, 0x2, 0x2, 0x467, 0x563, 0x3, 0x2, 0x2, 0x2, 0x468, 0x469, 
       0x7, 0x1e, 0x2, 0x2, 0x469, 0x46a, 0x7, 0x91, 0x2, 0x2, 0x46a, 0x46b, 
       0x5, 0xe0, 0x71, 0x2, 0x46b, 0x46c, 0x7, 0x92, 0x2, 0x2, 0x46c, 0x563, 
       0x3, 0x2, 0x2, 0x2, 0x46d, 0x46e, 0x7, 0x1f, 0x2, 0x2, 0x46e, 0x46f, 
       0x7, 0x91, 0x2, 0x2, 0x46f, 0x470, 0x5, 0xda, 0x6e, 0x2, 0x470, 0x471, 
       0x7, 0x92, 0x2, 0x2, 0x471, 0x563, 0x3, 0x2, 0x2, 0x2, 0x472, 0x473, 
       0x7, 0x42, 0x2, 0x2, 0x473, 0x474, 0x7, 0x91, 0x2, 0x2, 0x474, 0x475, 
       0x5, 0xe0, 0x71, 0x2, 0x475, 0x476, 0x7, 0x92, 0x2, 0x2, 0x476, 0x563, 
       0x3, 0x2, 0x2, 0x2, 0x477, 0x478, 0x7, 0x43, 0x2, 0x2, 0x478, 0x479, 
       0x7, 0x91, 0x2, 0x2, 0x479, 0x47a, 0x5, 0xe0, 0x71, 0x2, 0x47a, 0x47b, 
       0x7, 0x92, 0x2, 0x2, 0x47b, 0x563, 0x3, 0x2, 0x2, 0x2, 0x47c, 0x47d, 
       0x7, 0x44, 0x2, 0x2, 0x47d, 0x47f, 0x7, 0x91, 0x2, 0x2, 0x47e, 0x480, 
       0x5, 0xe0, 0x71, 0x2, 0x47f, 0x47e, 0x3, 0x2, 0x2, 0x2, 0x47f, 0x480, 
       0x3, 0x2, 0x2, 0x2, 0x480, 0x481, 0x3, 0x2, 0x2, 0x2, 0x481, 0x563, 
       0x7, 0x92, 0x2, 0x2, 0x482, 0x483, 0x7, 0x45, 0x2, 0x2, 0x483, 0x484, 
       0x7, 0x91, 0x2, 0x2, 0x484, 0x563, 0x7, 0x92, 0x2, 0x2, 0x485, 0x486, 
       0x7, 0x46, 0x2, 0x2, 0x486, 0x487, 0x7, 0x91, 0x2, 0x2, 0x487, 0x488, 
       0x5, 0xe0, 0x71, 0x2, 0x488, 0x489, 0x7, 0x92, 0x2, 0x2, 0x489, 0x563, 
       0x3, 0x2, 0x2, 0x2, 0x48a, 0x48b, 0x7, 0x47, 0x2, 0x2, 0x48b, 0x48c, 
       0x7, 0x91, 0x2, 0x2, 0x48c, 0x48d, 0x5, 0xe0, 0x71, 0x2, 0x48d, 0x48e, 
       0x7, 0x92, 0x2, 0x2, 0x48e, 0x563, 0x3, 0x2, 0x2, 0x2, 0x48f, 0x490, 
       0x7, 0x48, 0x2, 0x2, 0x490, 0x491, 0x7, 0x91, 0x2, 0x2, 0x491, 0x492, 
       0x5, 0xe0, 0x71, 0x2, 0x492, 0x493, 0x7, 0x92, 0x2, 0x2, 0x493, 0x563, 
       0x3, 0x2, 0x2, 0x2, 0x494, 0x495, 0x7, 0x49, 0x2, 0x2, 0x495, 0x496, 
       0x7, 0x91, 0x2, 0x2, 0x496, 0x497, 0x5, 0xe0, 0x71, 0x2, 0x497, 0x498, 
       0x7, 0x92, 0x2, 0x2, 0x498, 0x563, 0x3, 0x2, 0x2, 0x2, 0x499, 0x49a, 
       0x7, 0x4a, 0x2, 0x2, 0x49a, 0x49c, 0x7, 0x91, 0x2, 0x2, 0x49b, 0x49d, 
       0x5, 0x90, 0x49, 0x2, 0x49c, 0x49b, 0x3, 0x2, 0x2, 0x2, 0x49c, 0x49d, 
       0x3, 0x2, 0x2, 0x2, 0x49d, 0x49e, 0x3, 0x2, 0x2, 0x2, 0x49e, 0x563, 
       0x7, 0x92, 0x2, 0x2, 0x49f, 0x563, 0x5, 0xec, 0x77, 0x2, 0x4a0, 0x4a1, 
       0x7, 0x4b, 0x2, 0x2, 0x4a1, 0x4a2, 0x7, 0x91, 0x2, 0x2, 0x4a2, 0x4a3, 
       0x5, 0xe0, 0x71, 0x2, 0x4a3, 0x4a4, 0x7, 0x92, 0x2, 0x2, 0x4a4, 0x563, 
       0x3, 0x2, 0x2, 0x2, 0x4a5, 0x563, 0x5, 0xee, 0x78, 0x2, 0x4a6, 0x4a7, 
       0x7, 0x4c, 0x2, 0x2, 0x4a7, 0x4a8, 0x7, 0x91, 0x2, 0x2, 0x4a8, 0x4a9, 
       0x5, 0xe0, 0x71, 0x2, 0x4a9, 0x4aa, 0x7, 0x92, 0x2, 0x2, 0x4aa, 0x563, 
       0x3, 0x2, 0x2, 0x2, 0x4ab, 0x4ac, 0x7, 0x4d, 0x2, 0x2, 0x4ac, 0x4ad, 
       0x7, 0x91, 0x2, 0x2, 0x4ad, 0x4ae, 0x5, 0xe0, 0x71, 0x2, 0x4ae, 0x4af, 
       0x7, 0x92, 0x2, 0x2, 0x4af, 0x563, 0x3, 0x2, 0x2, 0x2, 0x4b0, 0x4b1, 
       0x7, 0x4e, 0x2, 0x2, 0x4b1, 0x4b2, 0x7, 0x91, 0x2, 0x2, 0x4b2, 0x4b3, 
       0x5, 0xe0, 0x71, 0x2, 0x4b3, 0x4b4, 0x7, 0x92, 0x2, 0x2, 0x4b4, 0x563, 
       0x3, 0x2, 0x2, 0x2, 0x4b5, 0x4b6, 0x7, 0x4f, 0x2, 0x2, 0x4b6, 0x4b7, 
       0x7, 0x91, 0x2, 0x2, 0x4b7, 0x4b8, 0x5, 0xe0, 0x71, 0x2, 0x4b8, 0x4b9, 
       0x7, 0x9d, 0x2, 0x2, 0x4b9, 0x4ba, 0x5, 0xe0, 0x71, 0x2, 0x4ba, 0x4bb, 
       0x7, 0x92, 0x2, 0x2, 0x4bb, 0x563, 0x3, 0x2, 0x2, 0x2, 0x4bc, 0x4bd, 
       0x7, 0x50, 0x2, 0x2, 0x4bd, 0x4be, 0x7, 0x91, 0x2, 0x2, 0x4be, 0x4bf, 
       0x5, 0xe0, 0x71, 0x2, 0x4bf, 0x4c0, 0x7, 0x9d, 0x2, 0x2, 0x4c0, 0x4c1, 
       0x5, 0xe0, 0x71, 0x2, 0x4c1, 0x4c2, 0x7, 0x92, 0x2, 0x2, 0x4c2, 0x563, 
       0x3, 0x2, 0x2, 0x2, 0x4c3, 0x4c4, 0x7, 0x51, 0x2, 0x2, 0x4c4, 0x4c5, 
       0x7, 0x91, 0x2, 0x2, 0x4c5, 0x4c6, 0x5, 0xe0, 0x71, 0x2, 0x4c6, 0x4c7, 
       0x7, 0x9d, 0x2, 0x2, 0x4c7, 0x4c8, 0x5, 0xe0, 0x71, 0x2, 0x4c8, 0x4c9, 
       0x7, 0x92, 0x2, 0x2, 0x4c9, 0x563, 0x3, 0x2, 0x2, 0x2, 0x4ca, 0x4cb, 
       0x7, 0x52, 0x2, 0x2, 0x4cb, 0x4cc, 0x7, 0x91, 0x2, 0x2, 0x4cc, 0x4cd, 
       0x5, 0xe0, 0x71, 0x2, 0x4cd, 0x4ce, 0x7, 0x9d, 0x2, 0x2, 0x4ce, 0x4cf, 
       0x5, 0xe0, 0x71, 0x2, 0x4cf, 0x4d0, 0x7, 0x92, 0x2, 0x2, 0x4d0, 0x563, 
       0x3, 0x2, 0x2, 0x2, 0x4d1, 0x4d2, 0x7, 0x53, 0x2, 0x2, 0x4d2, 0x4d3, 
       0x7, 0x91, 0x2, 0x2, 0x4d3, 0x4d4, 0x5, 0xe0, 0x71, 0x2, 0x4d4, 0x4d5, 
       0x7, 0x9d, 0x2, 0x2, 0x4d5, 0x4d6, 0x5, 0xe0, 0x71, 0x2, 0x4d6, 0x4d7, 
       0x7, 0x92, 0x2, 0x2, 0x4d7, 0x563, 0x3, 0x2, 0x2, 0x2, 0x4d8, 0x4d9, 
       0x7, 0x55, 0x2, 0x2, 0x4d9, 0x4da, 0x7, 0x91, 0x2, 0x2, 0x4da, 0x4db, 
       0x5, 0xe0, 0x71, 0x2, 0x4db, 0x4dc, 0x7, 0x92, 0x2, 0x2, 0x4dc, 0x563, 
       0x3, 0x2, 0x2, 0x2, 0x4dd, 0x4de, 0x7, 0x56, 0x2, 0x2, 0x4de, 0x4df, 
       0x7, 0x91, 0x2, 0x2, 0x4df, 0x4e0, 0x5, 0xe0, 0x71, 0x2, 0x4e0, 0x4e1, 
       0x7, 0x92, 0x2, 0x2, 0x4e1, 0x563, 0x3, 0x2, 0x2, 0x2, 0x4e2, 0x4e3, 
       0x7, 0x57, 0x2, 0x2, 0x4e3, 0x4e4, 0x7, 0x91, 0x2, 0x2, 0x4e4, 0x4e5, 
       0x5, 0xe0, 0x71, 0x2, 0x4e5, 0x4e6, 0x7, 0x92, 0x2, 0x2, 0x4e6, 0x563, 
       0x3, 0x2, 0x2, 0x2, 0x4e7, 0x4e8, 0x7, 0x58, 0x2, 0x2, 0x4e8, 0x4e9, 
       0x7, 0x91, 0x2, 0x2, 0x4e9, 0x4ea, 0x5, 0xe0, 0x71, 0x2, 0x4ea, 0x4eb, 
       0x7, 0x92, 0x2, 0x2, 0x4eb, 0x563, 0x3, 0x2, 0x2, 0x2, 0x4ec, 0x4ed, 
       0x7, 0x59, 0x2, 0x2, 0x4ed, 0x4ee, 0x7, 0x91, 0x2, 0x2, 0x4ee, 0x4ef, 
       0x5, 0xe0, 0x71, 0x2, 0x4ef, 0x4f0, 0x7, 0x92, 0x2, 0x2, 0x4f0, 0x563, 
       0x3, 0x2, 0x2, 0x2, 0x4f1, 0x4f2, 0x7, 0x5a, 0x2, 0x2, 0x4f2, 0x4f3, 
       0x7, 0x91, 0x2, 0x2, 0x4f3, 0x4f4, 0x5, 0xe0, 0x71, 0x2, 0x4f4, 0x4f5, 
       0x7, 0x92, 0x2, 0x2, 0x4f5, 0x563, 0x3, 0x2, 0x2, 0x2, 0x4f6, 0x4f7, 
       0x7, 0x5b, 0x2, 0x2, 0x4f7, 0x4f8, 0x7, 0x91, 0x2, 0x2, 0x4f8, 0x4f9, 
       0x5, 0xe0, 0x71, 0x2, 0x4f9, 0x4fa, 0x7, 0x92, 0x2, 0x2, 0x4fa, 0x563, 
       0x3, 0x2, 0x2, 0x2, 0x4fb, 0x4fc, 0x7, 0x5c, 0x2, 0x2, 0x4fc, 0x4fd, 
       0x7, 0x91, 0x2, 0x2, 0x4fd, 0x4fe, 0x5, 0xe0, 0x71, 0x2, 0x4fe, 0x4ff, 
       0x7, 0x92, 0x2, 0x2, 0x4ff, 0x563, 0x3, 0x2, 0x2, 0x2, 0x500, 0x501, 
       0x7, 0x5d, 0x2, 0x2, 0x501, 0x502, 0x7, 0x91, 0x2, 0x2, 0x502, 0x563, 
       0x7, 0x92, 0x2, 0x2, 0x503, 0x504, 0x7, 0x5e, 0x2, 0x2, 0x504, 0x505, 
       0x7, 0x91, 0x2, 0x2, 0x505, 0x563, 0x7, 0x92, 0x2, 0x2, 0x506, 0x507, 
       0x7, 0x5f, 0x2, 0x2, 0x507, 0x508, 0x7, 0x91, 0x2, 0x2, 0x508, 0x563, 
       0x7, 0x92, 0x2, 0x2, 0x509, 0x50a, 0x7, 0x60, 0x2, 0x2, 0x50a, 0x50b, 
       0x7, 0x91, 0x2, 0x2, 0x50b, 0x50c, 0x5, 0xe0, 0x71, 0x2, 0x50c, 0x50d, 
       0x7, 0x92, 0x2, 0x2, 0x50d, 0x563, 0x3, 0x2, 0x2, 0x2, 0x50e, 0x50f, 
       0x7, 0x61, 0x2, 0x2, 0x50f, 0x510, 0x7, 0x91, 0x2, 0x2, 0x510, 0x511, 
       0x5, 0xe0, 0x71, 0x2, 0x511, 0x512, 0x7, 0x92, 0x2, 0x2, 0x512, 0x563, 
       0x3, 0x2, 0x2, 0x2, 0x513, 0x514, 0x7, 0x62, 0x2, 0x2, 0x514, 0x515, 
       0x7, 0x91, 0x2, 0x2, 0x515, 0x516, 0x5, 0xe0, 0x71, 0x2, 0x516, 0x517, 
       0x7, 0x92, 0x2, 0x2, 0x517, 0x563, 0x3, 0x2, 0x2, 0x2, 0x518, 0x519, 
       0x7, 0x63, 0x2, 0x2, 0x519, 0x51a, 0x7, 0x91, 0x2, 0x2, 0x51a, 0x51b, 
       0x5, 0xe0, 0x71, 0x2, 0x51b, 0x51c, 0x7, 0x92, 0x2, 0x2, 0x51c, 0x563, 
       0x3, 0x2, 0x2, 0x2, 0x51d, 0x51e, 0x7, 0x64, 0x2, 0x2, 0x51e, 0x51f, 
       0x7, 0x91, 0x2, 0x2, 0x51f, 0x520, 0x5, 0xe0, 0x71, 0x2, 0x520, 0x521, 
       0x7, 0x92, 0x2, 0x2, 0x521, 0x563, 0x3, 0x2, 0x2, 0x2, 0x522, 0x523, 
       0x7, 0x65, 0x2, 0x2, 0x523, 0x525, 0x7, 0x91, 0x2, 0x2, 0x524, 0x526, 
       0x5, 0x90, 0x49, 0x2, 0x525, 0x524, 0x3, 0x2, 0x2, 0x2, 0x525, 0x526, 
       0x3, 0x2, 0x2, 0x2, 0x526, 0x527, 0x3, 0x2, 0x2, 0x2, 0x527, 0x563, 
       0x7, 0x92, 0x2, 0x2, 0x528, 0x529, 0x7, 0x66, 0x2, 0x2, 0x529, 0x52a, 
       0x7, 0x91, 0x2, 0x2, 0x52a, 0x52b, 0x5, 0xe0, 0x71, 0x2, 0x52b, 0x52c, 
       0x7, 0x9d, 0x2, 0x2, 0x52c, 0x52d, 0x5, 0xe0, 0x71, 0x2, 0x52d, 0x52e, 
       0x7, 0x9d, 0x2, 0x2, 0x52e, 0x52f, 0x5, 0xe0, 0x71, 0x2, 0x52f, 0x530, 
       0x7, 0x92, 0x2, 0x2, 0x530, 0x563, 0x3, 0x2, 0x2, 0x2, 0x531, 0x532, 
       0x7, 0x67, 0x2, 0x2, 0x532, 0x533, 0x7, 0x91, 0x2, 0x2, 0x533, 0x534, 
       0x5, 0xe0, 0x71, 0x2, 0x534, 0x535, 0x7, 0x9d, 0x2, 0x2, 0x535, 0x536, 
       0x5, 0xe0, 0x71, 0x2, 0x536, 0x537, 0x7, 0x92, 0x2, 0x2, 0x537, 0x563, 
       0x3, 0x2, 0x2, 0x2, 0x538, 0x539, 0x7, 0x68, 0x2, 0x2, 0x539, 0x53a, 
       0x7, 0x91, 0x2, 0x2, 0x53a, 0x53b, 0x5, 0xe0, 0x71, 0x2, 0x53b, 0x53c, 
       0x7, 0x9d, 0x2, 0x2, 0x53c, 0x53d, 0x5, 0xe0, 0x71, 0x2, 0x53d, 0x53e, 
       0x7, 0x92, 0x2, 0x2, 0x53e, 0x563, 0x3, 0x2, 0x2, 0x2, 0x53f, 0x540, 
       0x7, 0x20, 0x2, 0x2, 0x540, 0x541, 0x7, 0x91, 0x2, 0x2, 0x541, 0x542, 
       0x5, 0xe0, 0x71, 0x2, 0x542, 0x543, 0x7, 0x9d, 0x2, 0x2, 0x543, 0x544, 
       0x5, 0xe0, 0x71, 0x2, 0x544, 0x545, 0x7, 0x92, 0x2, 0x2, 0x545, 0x563, 
       0x3, 0x2, 0x2, 0x2, 0x546, 0x547, 0x7, 0x21, 0x2, 0x2, 0x547, 0x548, 
       0x7, 0x91, 0x2, 0x2, 0x548, 0x549, 0x5, 0xe0, 0x71, 0x2, 0x549, 0x54a, 
       0x7, 0x92, 0x2, 0x2, 0x54a, 0x563, 0x3, 0x2, 0x2, 0x2, 0x54b, 0x54c, 
       0x7, 0x22, 0x2, 0x2, 0x54c, 0x54d, 0x7, 0x91, 0x2, 0x2, 0x54d, 0x54e, 
       0x5, 0xe0, 0x71, 0x2, 0x54e, 0x54f, 0x7, 0x92, 0x2, 0x2, 0x54f, 0x563, 
       0x3, 0x2, 0x2, 0x2, 0x550, 0x551, 0x7, 0x23, 0x2, 0x2, 0x551, 0x552, 
       0x7, 0x91, 0x2, 0x2, 0x552, 0x553, 0x5, 0xe0, 0x71, 0x2, 0x553, 0x554, 
       0x7, 0x92, 0x2, 0x2, 0x554, 0x563, 0x3, 0x2, 0x2, 0x2, 0x555, 0x556, 
       0x7, 0x24, 0x2, 0x2, 0x556, 0x557, 0x7, 0x91, 0x2, 0x2, 0x557, 0x558, 
       0x5, 0xe0, 0x71, 0x2, 0x558, 0x559, 0x7, 0x92, 0x2, 0x2, 0x559, 0x563, 
       0x3, 0x2, 0x2, 0x2, 0x55a, 0x55b, 0x7, 0x69, 0x2, 0x2, 0x55b, 0x55c, 
       0x7, 0x91, 0x2, 0x2, 0x55c, 0x55d, 0x5, 0xe0, 0x71, 0x2, 0x55d, 0x55e, 
       0x7, 0x92, 0x2, 0x2, 0x55e, 0x563, 0x3, 0x2, 0x2, 0x2, 0x55f, 0x563, 
       0x5, 0xea, 0x76, 0x2, 0x560, 0x563, 0x5, 0xf0, 0x79, 0x2, 0x561, 
       0x563, 0x5, 0xf2, 0x7a, 0x2, 0x562, 0x456, 0x3, 0x2, 0x2, 0x2, 0x562, 
       0x457, 0x3, 0x2, 0x2, 0x2, 0x562, 0x45c, 0x3, 0x2, 0x2, 0x2, 0x562, 
       0x461, 0x3, 0x2, 0x2, 0x2, 0x562, 0x468, 0x3, 0x2, 0x2, 0x2, 0x562, 
       0x46d, 0x3, 0x2, 0x2, 0x2, 0x562, 0x472, 0x3, 0x2, 0x2, 0x2, 0x562, 
       0x477, 0x3, 0x2, 0x2, 0x2, 0x562, 0x47c, 0x3, 0x2, 0x2, 0x2, 0x562, 
       0x482, 0x3, 0x2, 0x2, 0x2, 0x562, 0x485, 0x3, 0x2, 0x2, 0x2, 0x562, 
       0x48a, 0x3, 0x2, 0x2, 0x2, 0x562, 0x48f, 0x3, 0x2, 0x2, 0x2, 0x562, 
       0x494, 0x3, 0x2, 0x2, 0x2, 0x562, 0x499, 0x3, 0x2, 0x2, 0x2, 0x562, 
       0x49f, 0x3, 0x2, 0x2, 0x2, 0x562, 0x4a0, 0x3, 0x2, 0x2, 0x2, 0x562, 
       0x4a5, 0x3, 0x2, 0x2, 0x2, 0x562, 0x4a6, 0x3, 0x2, 0x2, 0x2, 0x562, 
       0x4ab, 0x3, 0x2, 0x2, 0x2, 0x562, 0x4b0, 0x3, 0x2, 0x2, 0x2, 0x562, 
       0x4b5, 0x3, 0x2, 0x2, 0x2, 0x562, 0x4bc, 0x3, 0x2, 0x2, 0x2, 0x562, 
       0x4c3, 0x3, 0x2, 0x2, 0x2, 0x562, 0x4ca, 0x3, 0x2, 0x2, 0x2, 0x562, 
       0x4d1, 0x3, 0x2, 0x2, 0x2, 0x562, 0x4d8, 0x3, 0x2, 0x2, 0x2, 0x562, 
       0x4dd, 0x3, 0x2, 0x2, 0x2, 0x562, 0x4e2, 0x3, 0x2, 0x2, 0x2, 0x562, 
       0x4e7, 0x3, 0x2, 0x2, 0x2, 0x562, 0x4ec, 0x3, 0x2, 0x2, 0x2, 0x562, 
       0x4f1, 0x3, 0x2, 0x2, 0x2, 0x562, 0x4f6, 0x3, 0x2, 0x2, 0x2, 0x562, 
       0x4fb, 0x3, 0x2, 0x2, 0x2, 0x562, 0x500, 0x3, 0x2, 0x2, 0x2, 0x562, 
       0x503, 0x3, 0x2, 0x2, 0x2, 0x562, 0x506, 0x3, 0x2, 0x2, 0x2, 0x562, 
       0x509, 0x3, 0x2, 0x2, 0x2, 0x562, 0x50e, 0x3, 0x2, 0x2, 0x2, 0x562, 
       0x513, 0x3, 0x2, 0x2, 0x2, 0x562, 0x518, 0x3, 0x2, 0x2, 0x2, 0x562, 
       0x51d, 0x3, 0x2, 0x2, 0x2, 0x562, 0x522, 0x3, 0x2, 0x2, 0x2, 0x562, 
       0x528, 0x3, 0x2, 0x2, 0x2, 0x562, 0x531, 0x3, 0x2, 0x2, 0x2, 0x562, 
       0x538, 0x3, 0x2, 0x2, 0x2, 0x562, 0x53f, 0x3, 0x2, 0x2, 0x2, 0x562, 
       0x546, 0x3, 0x2, 0x2, 0x2, 0x562, 0x54b, 0x3, 0x2, 0x2, 0x2, 0x562, 
       0x550, 0x3, 0x2, 0x2, 0x2, 0x562, 0x555, 0x3, 0x2, 0x2, 0x2, 0x562, 
       0x55a, 0x3, 0x2, 0x2, 0x2, 0x562, 0x55f, 0x3, 0x2, 0x2, 0x2, 0x562, 
       0x560, 0x3, 0x2, 0x2, 0x2, 0x562, 0x561, 0x3, 0x2, 0x2, 0x2, 0x563, 
       0xe9, 0x3, 0x2, 0x2, 0x2, 0x564, 0x565, 0x7, 0x25, 0x2, 0x2, 0x565, 
       0x566, 0x7, 0x91, 0x2, 0x2, 0x566, 0x567, 0x5, 0xe0, 0x71, 0x2, 0x567, 
       0x568, 0x7, 0x9d, 0x2, 0x2, 0x568, 0x56b, 0x5, 0xe0, 0x71, 0x2, 0x569, 
       0x56a, 0x7, 0x9d, 0x2, 0x2, 0x56a, 0x56c, 0x5, 0xe0, 0x71, 0x2, 0x56b, 
       0x569, 0x3, 0x2, 0x2, 0x2, 0x56b, 0x56c, 0x3, 0x2, 0x2, 0x2, 0x56c, 
       0x56d, 0x3, 0x2, 0x2, 0x2, 0x56d, 0x56e, 0x7, 0x92, 0x2, 0x2, 0x56e, 
       0xeb, 0x3, 0x2, 0x2, 0x2, 0x56f, 0x570, 0x7, 0x26, 0x2, 0x2, 0x570, 
       0x571, 0x7, 0x91, 0x2, 0x2, 0x571, 0x572, 0x5, 0xe0, 0x71, 0x2, 0x572, 
       0x573, 0x7, 0x9d, 0x2, 0x2, 0x573, 0x576, 0x5, 0xe0, 0x71, 0x2, 0x574, 
       0x575, 0x7, 0x9d, 0x2, 0x2, 0x575, 0x577, 0x5, 0xe0, 0x71, 0x2, 0x576, 
       0x574, 0x3, 0x2, 0x2, 0x2, 0x576, 0x577, 0x3, 0x2, 0x2, 0x2, 0x577, 
       0x578, 0x3, 0x2, 0x2, 0x2, 0x578, 0x579, 0x7, 0x92, 0x2, 0x2, 0x579, 
       0xed, 0x3, 0x2, 0x2, 0x2, 0x57a, 0x57b, 0x7, 0x54, 0x2, 0x2, 0x57b, 
       0x57c, 0x7, 0x91, 0x2, 0x2, 0x57c, 0x57d, 0x5, 0xe0, 0x71, 0x2, 0x57d, 
       0x57e, 0x7, 0x9d, 0x2, 0x2, 0x57e, 0x57f, 0x5, 0xe0, 0x71, 0x2, 0x57f, 
       0x580, 0x7, 0x9d, 0x2, 0x2, 0x580, 0x583, 0x5, 0xe0, 0x71, 0x2, 0x581, 
       0x582, 0x7, 0x9d, 0x2, 0x2, 0x582, 0x584, 0x5, 0xe0, 0x71, 0x2, 0x583, 
       0x581, 0x3, 0x2, 0x2, 0x2, 0x583, 0x584, 0x3, 0x2, 0x2, 0x2, 0x584, 
       0x585, 0x3, 0x2, 0x2, 0x2, 0x585, 0x586, 0x7, 0x92, 0x2, 0x2, 0x586, 
       0xef, 0x3, 0x2, 0x2, 0x2, 0x587, 0x588, 0x7, 0x73, 0x2, 0x2, 0x588, 
       0x589, 0x5, 0x66, 0x34, 0x2, 0x589, 0xf1, 0x3, 0x2, 0x2, 0x2, 0x58a, 
       0x58b, 0x7, 0x71, 0x2, 0x2, 0x58b, 0x58c, 0x7, 0x73, 0x2, 0x2, 0x58c, 
       0x58d, 0x5, 0x66, 0x34, 0x2, 0x58d, 0xf3, 0x3, 0x2, 0x2, 0x2, 0x58e, 
       0x58f, 0x7, 0x6a, 0x2, 0x2, 0x58f, 0x591, 0x7, 0x91, 0x2, 0x2, 0x590, 
       0x592, 0x7, 0x7, 0x2, 0x2, 0x591, 0x590, 0x3, 0x2, 0x2, 0x2, 0x591, 
       0x592, 0x3, 0x2, 0x2, 0x2, 0x592, 0x595, 0x3, 0x2, 0x2, 0x2, 0x593, 
       0x596, 0x7, 0x9b, 0x2, 0x2, 0x594, 0x596, 0x5, 0xe0, 0x71, 0x2, 0x595, 
       0x593, 0x3, 0x2, 0x2, 0x2, 0x595, 0x594, 0x3, 0x2, 0x2, 0x2, 0x596, 
       0x597, 0x3, 0x2, 0x2, 0x2, 0x597, 0x5cf, 0x7, 0x92, 0x2, 0x2, 0x598, 
       0x599, 0x7, 0x6b, 0x2, 0x2, 0x599, 0x59b, 0x7, 0x91, 0x2, 0x2, 0x59a, 
       0x59c, 0x7, 0x7, 0x2, 0x2, 0x59b, 0x59a, 0x3, 0x2, 0x2, 0x2, 0x59b, 
       0x59c, 0x3, 0x2, 0x2, 0x2, 0x59c, 0x59d, 0x3, 0x2, 0x2, 0x2, 0x59d, 
       0x59e, 0x5, 0xe0, 0x71, 0x2, 0x59e, 0x59f, 0x7, 0x92, 0x2, 0x2, 0x59f, 
       0x5cf, 0x3, 0x2, 0x2, 0x2, 0x5a0, 0x5a1, 0x7, 0x6c, 0x2, 0x2, 0x5a1, 
       0x5a3, 0x7, 0x91, 0x2, 0x2, 0x5a2, 0x5a4, 0x7, 0x7, 0x2, 0x2, 0x5a3, 
       0x5a2, 0x3, 0x2, 0x2, 0x2, 0x5a3, 0x5a4, 0x3, 0x2, 0x2, 0x2, 0x5a4, 
       0x5a5, 0x3, 0x2, 0x2, 0x2, 0x5a5, 0x5a6, 0x5, 0xe0, 0x71, 0x2, 0x5a6, 
       0x5a7, 0x7, 0x92, 0x2, 0x2, 0x5a7, 0x5cf, 0x3, 0x2, 0x2, 0x2, 0x5a8, 
       0x5a9, 0x7, 0x6d, 0x2, 0x2, 0x5a9, 0x5ab, 0x7, 0x91, 0x2, 0x2, 0x5aa, 
       0x5ac, 0x7, 0x7, 0x2, 0x2, 0x5ab, 0x5aa, 0x3, 0x2, 0x2, 0x2, 0x5ab, 
       0x5ac, 0x3, 0x2, 0x2, 0x2, 0x5ac, 0x5ad, 0x3, 0x2, 0x2, 0x2, 0x5ad, 
       0x5ae, 0x5, 0xe0, 0x71, 0x2, 0x5ae, 0x5af, 0x7, 0x92, 0x2, 0x2, 0x5af, 
       0x5cf, 0x3, 0x2, 0x2, 0x2, 0x5b0, 0x5b1, 0x7, 0x6e, 0x2, 0x2, 0x5b1, 
       0x5b3, 0x7, 0x91, 0x2, 0x2, 0x5b2, 0x5b4, 0x7, 0x7, 0x2, 0x2, 0x5b3, 
       0x5b2, 0x3, 0x2, 0x2, 0x2, 0x5b3, 0x5b4, 0x3, 0x2, 0x2, 0x2, 0x5b4, 
       0x5b5, 0x3, 0x2, 0x2, 0x2, 0x5b5, 0x5b6, 0x5, 0xe0, 0x71, 0x2, 0x5b6, 
       0x5b7, 0x7, 0x92, 0x2, 0x2, 0x5b7, 0x5cf, 0x3, 0x2, 0x2, 0x2, 0x5b8, 
       0x5b9, 0x7, 0x6f, 0x2, 0x2, 0x5b9, 0x5bb, 0x7, 0x91, 0x2, 0x2, 0x5ba, 
       0x5bc, 0x7, 0x7, 0x2, 0x2, 0x5bb, 0x5ba, 0x3, 0x2, 0x2, 0x2, 0x5bb, 
       0x5bc, 0x3, 0x2, 0x2, 0x2, 0x5bc, 0x5bd, 0x3, 0x2, 0x2, 0x2, 0x5bd, 
       0x5be, 0x5, 0xe0, 0x71, 0x2, 0x5be, 0x5bf, 0x7, 0x92, 0x2, 0x2, 0x5bf, 
       0x5cf, 0x3, 0x2, 0x2, 0x2, 0x5c0, 0x5c1, 0x7, 0x70, 0x2, 0x2, 0x5c1, 
       0x5c3, 0x7, 0x91, 0x2, 0x2, 0x5c2, 0x5c4, 0x7, 0x7, 0x2, 0x2, 0x5c3, 
       0x5c2, 0x3, 0x2, 0x2, 0x2, 0x5c3, 0x5c4, 0x3, 0x2, 0x2, 0x2, 0x5c4, 
       0x5c5, 0x3, 0x2, 0x2, 0x2, 0x5c5, 0x5ca, 0x5, 0xe0, 0x71, 0x2, 0x5c6, 
       0x5c7, 0x7, 0x97, 0x2, 0x2, 0x5c7, 0x5c8, 0x7, 0x74, 0x2, 0x2, 0x5c8, 
       0x5c9, 0x7, 0xa0, 0x2, 0x2, 0x5c9, 0x5cb, 0x5, 0x104, 0x83, 0x2, 
       0x5ca, 0x5c6, 0x3, 0x2, 0x2, 0x2, 0x5ca, 0x5cb, 0x3, 0x2, 0x2, 0x2, 
       0x5cb, 0x5cc, 0x3, 0x2, 0x2, 0x2, 0x5cc, 0x5cd, 0x7, 0x92, 0x2, 0x2, 
       0x5cd, 0x5cf, 0x3, 0x2, 0x2, 0x2, 0x5ce, 0x58e, 0x3, 0x2, 0x2, 0x2, 
       0x5ce, 0x598, 0x3, 0x2, 0x2, 0x2, 0x5ce, 0x5a0, 0x3, 0x2, 0x2, 0x2, 
       0x5ce, 0x5a8, 0x3, 0x2, 0x2, 0x2, 0x5ce, 0x5b0, 0x3, 0x2, 0x2, 0x2, 
       0x5ce, 0x5b8, 0x3, 0x2, 0x2, 0x2, 0x5ce, 0x5c0, 0x3, 0x2, 0x2, 0x2, 
       0x5cf, 0xf5, 0x3, 0x2, 0x2, 0x2, 0x5d0, 0x5d2, 0x5, 0x106, 0x84, 
       0x2, 0x5d1, 0x5d3, 0x5, 0x8e, 0x48, 0x2, 0x5d2, 0x5d1, 0x3, 0x2, 
       0x2, 0x2, 0x5d2, 0x5d3, 0x3, 0x2, 0x2, 0x2, 0x5d3, 0xf7, 0x3, 0x2, 
       0x2, 0x2, 0x5d4, 0x5d8, 0x5, 0x104, 0x83, 0x2, 0x5d5, 0x5d9, 0x7, 
       0x7b, 0x2, 0x2, 0x5d6, 0x5d7, 0x7, 0x8a, 0x2, 0x2, 0x5d7, 0x5d9, 
       0x5, 0x106, 0x84, 0x2, 0x5d8, 0x5d5, 0x3, 0x2, 0x2, 0x2, 0x5d8, 0x5d6, 
       0x3, 0x2, 0x2, 0x2, 0x5d8, 0x5d9, 0x3, 0x2, 0x2, 0x2, 0x5d9, 0xf9, 
       0x3, 0x2, 0x2, 0x2, 0x5da, 0x5de, 0x5, 0xfc, 0x7f, 0x2, 0x5db, 0x5de, 
       0x5, 0xfe, 0x80, 0x2, 0x5dc, 0x5de, 0x5, 0x100, 0x81, 0x2, 0x5dd, 
       0x5da, 0x3, 0x2, 0x2, 0x2, 0x5dd, 0x5db, 0x3, 0x2, 0x2, 0x2, 0x5dd, 
       0x5dc, 0x3, 0x2, 0x2, 0x2, 0x5de, 0xfb, 0x3, 0x2, 0x2, 0x2, 0x5df, 
       0x5e0, 0x9, 0xa, 0x2, 0x2, 0x5e0, 0xfd, 0x3, 0x2, 0x2, 0x2, 0x5e1, 
       0x5e2, 0x9, 0xb, 0x2, 0x2, 0x5e2, 0xff, 0x3, 0x2, 0x2, 0x2, 0x5e3, 
       0x5e4, 0x9, 0xc, 0x2, 0x2, 0x5e4, 0x101, 0x3, 0x2, 0x2, 0x2, 0x5e5, 
       0x5e6, 0x9, 0xd, 0x2, 0x2, 0x5e6, 0x103, 0x3, 0x2, 0x2, 0x2, 0x5e7, 
       0x5e8, 0x9, 0xe, 0x2, 0x2, 0x5e8, 0x105, 0x3, 0x2, 0x2, 0x2, 0x5e9, 
       0x5ec, 0x7, 0x75, 0x2, 0x2, 0x5ea, 0x5ec, 0x5, 0x108, 0x85, 0x2, 
       0x5eb, 0x5e9, 0x3, 0x2, 0x2, 0x2, 0x5eb, 0x5ea, 0x3, 0x2, 0x2, 0x2, 
       0x5ec, 0x107, 0x3, 0x2, 0x2, 0x2, 0x5ed, 0x5ee, 0x9, 0xf, 0x2, 0x2, 
       0x5ee, 0x109, 0x3, 0x2, 0x2, 0x2, 0x5ef, 0x5f2, 0x7, 0x78, 0x2, 0x2, 
       0x5f0, 0x5f2, 0x5, 0x10c, 0x87, 0x2, 0x5f1, 0x5ef, 0x3, 0x2, 0x2, 
       0x2, 0x5f1, 0x5f0, 0x3, 0x2, 0x2, 0x2, 0x5f2, 0x10b, 0x3, 0x2, 0x2, 
       0x2, 0x5f3, 0x5f4, 0x7, 0x95, 0x2, 0x2, 0x5f4, 0x5f5, 0x7, 0x96, 
       0x2, 0x2, 0x5f5, 0x10d, 0x3, 0x2, 0x2, 0x2, 0x95, 0x113, 0x11b, 0x11f, 
       0x121, 0x12f, 0x13c, 0x141, 0x144, 0x14f, 0x156, 0x15f, 0x165, 0x169, 
       0x16f, 0x172, 0x177, 0x17b, 0x183, 0x18b, 0x190, 0x195, 0x198, 0x19b, 
       0x19e, 0x1a5, 0x1ad, 0x1b2, 0x1b8, 0x1c1, 0x1ca, 0x1ce, 0x1d2, 0x1d4, 
       0x1de, 0x1e8, 0x1ed, 0x1ef, 0x1fc, 0x200, 0x205, 0x209, 0x20f, 0x215, 
       0x21b, 0x223, 0x22b, 0x23f, 0x243, 0x246, 0x24b, 0x259, 0x25f, 0x262, 
       0x26b, 0x276, 0x27b, 0x280, 0x283, 0x289, 0x290, 0x294, 0x29a, 0x29f, 
       0x2a4, 0x2a9, 0x2ac, 0x2b1, 0x2b5, 0x2c0, 0x2cb, 0x2dc, 0x2e3, 0x2ec, 
       0x2f4, 0x2fd, 0x307, 0x311, 0x31d, 0x324, 0x328, 0x331, 0x336, 0x33d, 
       0x341, 0x34a, 0x34d, 0x355, 0x359, 0x35e, 0x365, 0x370, 0x373, 0x377, 
       0x37c, 0x380, 0x385, 0x392, 0x39e, 0x3a6, 0x3ab, 0x3ae, 0x3bc, 0x3c5, 
       0x3c8, 0x3cb, 0x3ce, 0x3d2, 0x3d8, 0x3e0, 0x3ea, 0x3f2, 0x3f8, 0x3fc, 
       0x400, 0x404, 0x40e, 0x41b, 0x42a, 0x42f, 0x438, 0x43a, 0x43f, 0x443, 
       0x446, 0x454, 0x47f, 0x49c, 0x525, 0x562, 0x56b, 0x576, 0x583, 0x591, 
       0x595, 0x59b, 0x5a3, 0x5ab, 0x5b3, 0x5bb, 0x5c3, 0x5ca, 0x5ce, 0x5d2, 
       0x5d8, 0x5dd, 0x5eb, 0x5f1, 
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

SparqlParser::Initializer SparqlParser::_init;

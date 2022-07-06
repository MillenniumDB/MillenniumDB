// Generated from /home/crojsd/MillenniumDB-Dev/src/parser/query/grammar/MDBParser.g4 by ANTLR 4.9.2
import org.antlr.v4.runtime.atn.*;
import org.antlr.v4.runtime.dfa.DFA;
import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.misc.*;
import org.antlr.v4.runtime.tree.*;
import java.util.List;
import java.util.Iterator;
import java.util.ArrayList;

@SuppressWarnings({"all", "warnings", "unchecked", "unused", "cast"})
public class MDBParser extends Parser {
	static { RuntimeMetaData.checkVersion("4.9.2", RuntimeMetaData.VERSION); }

	protected static final DFA[] _decisionToDFA;
	protected static final PredictionContextCache _sharedContextCache =
		new PredictionContextCache();
	public static final int
		K_ANY=1, K_AND=2, K_AVG=3, K_ALL=4, K_ASC=5, K_BY=6, K_BOOL=7, K_COUNT=8, 
		K_DESCRIBE=9, K_DESC=10, K_DISTINCT=11, K_IS=12, K_INTEGER=13, K_FALSE=14, 
		K_FLOAT=15, K_GROUP=16, K_LIMIT=17, K_MAX=18, K_MATCH=19, K_MIN=20, K_OPTIONAL=21, 
		K_ORDER=22, K_OR=23, K_NOT=24, K_NULL=25, K_SET=26, K_SUM=27, K_STRING=28, 
		K_RETURN=29, K_TRUE=30, K_WHERE=31, TRUE_PROP=32, FALSE_PROP=33, ANON_ID=34, 
		EDGE_ID=35, KEY=36, TYPE=37, TYPE_VAR=38, VARIABLE=39, STRING=40, UNSIGNED_INTEGER=41, 
		UNSIGNED_FLOAT=42, NAME=43, LEQ=44, GEQ=45, EQ=46, NEQ=47, LT=48, GT=49, 
		SINGLE_EQ=50, PATH_SEQUENCE=51, PATH_ALTERNATIVE=52, PATH_NEGATION=53, 
		STAR=54, PERCENT=55, QUESTION_MARK=56, PLUS=57, MINUS=58, L_PAR=59, R_PAR=60, 
		LCURLY_BRACKET=61, RCURLY_BRACKET=62, LSQUARE_BRACKET=63, RSQUARE_BRACKET=64, 
		COMMA=65, COLON=66, WHITE_SPACE=67, SINGLE_LINE_COMMENT=68, UNRECOGNIZED=69;
	public static final int
		RULE_root = 0, RULE_describeStatement = 1, RULE_setStatement = 2, RULE_matchStatement = 3, 
		RULE_whereStatement = 4, RULE_groupByStatement = 5, RULE_orderByStatement = 6, 
		RULE_returnStatement = 7, RULE_setItem = 8, RULE_returnItem = 9, RULE_aggregateFunc = 10, 
		RULE_orderByItem = 11, RULE_groupByItem = 12, RULE_graphPattern = 13, 
		RULE_optionalPattern = 14, RULE_basicPattern = 15, RULE_linearPattern = 16, 
		RULE_path = 17, RULE_pathAlternatives = 18, RULE_pathSequence = 19, RULE_pathAtom = 20, 
		RULE_pathSuffix = 21, RULE_pathType = 22, RULE_node = 23, RULE_fixedNode = 24, 
		RULE_fixedNodeInside = 25, RULE_edge = 26, RULE_edgeInside = 27, RULE_varNode = 28, 
		RULE_properties = 29, RULE_property = 30, RULE_identifier = 31, RULE_boolValue = 32, 
		RULE_numericValue = 33, RULE_value = 34, RULE_conditionalOrExpr = 35, 
		RULE_conditionalAndExpr = 36, RULE_comparisonExpr = 37, RULE_aditiveExpr = 38, 
		RULE_multiplicativeExpr = 39, RULE_unaryExpr = 40, RULE_atomicExpr = 41, 
		RULE_valueExpr = 42, RULE_exprTypename = 43, RULE_keyword = 44;
	private static String[] makeRuleNames() {
		return new String[] {
			"root", "describeStatement", "setStatement", "matchStatement", "whereStatement", 
			"groupByStatement", "orderByStatement", "returnStatement", "setItem", 
			"returnItem", "aggregateFunc", "orderByItem", "groupByItem", "graphPattern", 
			"optionalPattern", "basicPattern", "linearPattern", "path", "pathAlternatives", 
			"pathSequence", "pathAtom", "pathSuffix", "pathType", "node", "fixedNode", 
			"fixedNodeInside", "edge", "edgeInside", "varNode", "properties", "property", 
			"identifier", "boolValue", "numericValue", "value", "conditionalOrExpr", 
			"conditionalAndExpr", "comparisonExpr", "aditiveExpr", "multiplicativeExpr", 
			"unaryExpr", "atomicExpr", "valueExpr", "exprTypename", "keyword"
		};
	}
	public static final String[] ruleNames = makeRuleNames();

	private static String[] makeLiteralNames() {
		return new String[] {
			null, null, null, null, null, null, null, null, null, null, null, null, 
			null, null, "'false'", null, null, null, null, null, null, null, null, 
			null, null, null, null, null, null, null, "'true'", null, null, null, 
			null, null, null, null, null, null, null, null, null, null, "'<='", "'>='", 
			"'=='", "'!='", "'<'", "'>'", "'='", "'/'", "'|'", "'^'", "'*'", "'%'", 
			"'?'", "'+'", "'-'", "'('", "')'", "'{'", "'}'", "'['", "']'", "','", 
			"':'"
		};
	}
	private static final String[] _LITERAL_NAMES = makeLiteralNames();
	private static String[] makeSymbolicNames() {
		return new String[] {
			null, "K_ANY", "K_AND", "K_AVG", "K_ALL", "K_ASC", "K_BY", "K_BOOL", 
			"K_COUNT", "K_DESCRIBE", "K_DESC", "K_DISTINCT", "K_IS", "K_INTEGER", 
			"K_FALSE", "K_FLOAT", "K_GROUP", "K_LIMIT", "K_MAX", "K_MATCH", "K_MIN", 
			"K_OPTIONAL", "K_ORDER", "K_OR", "K_NOT", "K_NULL", "K_SET", "K_SUM", 
			"K_STRING", "K_RETURN", "K_TRUE", "K_WHERE", "TRUE_PROP", "FALSE_PROP", 
			"ANON_ID", "EDGE_ID", "KEY", "TYPE", "TYPE_VAR", "VARIABLE", "STRING", 
			"UNSIGNED_INTEGER", "UNSIGNED_FLOAT", "NAME", "LEQ", "GEQ", "EQ", "NEQ", 
			"LT", "GT", "SINGLE_EQ", "PATH_SEQUENCE", "PATH_ALTERNATIVE", "PATH_NEGATION", 
			"STAR", "PERCENT", "QUESTION_MARK", "PLUS", "MINUS", "L_PAR", "R_PAR", 
			"LCURLY_BRACKET", "RCURLY_BRACKET", "LSQUARE_BRACKET", "RSQUARE_BRACKET", 
			"COMMA", "COLON", "WHITE_SPACE", "SINGLE_LINE_COMMENT", "UNRECOGNIZED"
		};
	}
	private static final String[] _SYMBOLIC_NAMES = makeSymbolicNames();
	public static final Vocabulary VOCABULARY = new VocabularyImpl(_LITERAL_NAMES, _SYMBOLIC_NAMES);

	/**
	 * @deprecated Use {@link #VOCABULARY} instead.
	 */
	@Deprecated
	public static final String[] tokenNames;
	static {
		tokenNames = new String[_SYMBOLIC_NAMES.length];
		for (int i = 0; i < tokenNames.length; i++) {
			tokenNames[i] = VOCABULARY.getLiteralName(i);
			if (tokenNames[i] == null) {
				tokenNames[i] = VOCABULARY.getSymbolicName(i);
			}

			if (tokenNames[i] == null) {
				tokenNames[i] = "<INVALID>";
			}
		}
	}

	@Override
	@Deprecated
	public String[] getTokenNames() {
		return tokenNames;
	}

	@Override

	public Vocabulary getVocabulary() {
		return VOCABULARY;
	}

	@Override
	public String getGrammarFileName() { return "MDBParser.g4"; }

	@Override
	public String[] getRuleNames() { return ruleNames; }

	@Override
	public String getSerializedATN() { return _serializedATN; }

	@Override
	public ATN getATN() { return _ATN; }

	public MDBParser(TokenStream input) {
		super(input);
		_interp = new ParserATNSimulator(this,_ATN,_decisionToDFA,_sharedContextCache);
	}

	public static class RootContext extends ParserRuleContext {
		public MatchStatementContext matchStatement() {
			return getRuleContext(MatchStatementContext.class,0);
		}
		public ReturnStatementContext returnStatement() {
			return getRuleContext(ReturnStatementContext.class,0);
		}
		public TerminalNode EOF() { return getToken(MDBParser.EOF, 0); }
		public SetStatementContext setStatement() {
			return getRuleContext(SetStatementContext.class,0);
		}
		public WhereStatementContext whereStatement() {
			return getRuleContext(WhereStatementContext.class,0);
		}
		public GroupByStatementContext groupByStatement() {
			return getRuleContext(GroupByStatementContext.class,0);
		}
		public OrderByStatementContext orderByStatement() {
			return getRuleContext(OrderByStatementContext.class,0);
		}
		public DescribeStatementContext describeStatement() {
			return getRuleContext(DescribeStatementContext.class,0);
		}
		public RootContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_root; }
	}

	public final RootContext root() throws RecognitionException {
		RootContext _localctx = new RootContext(_ctx, getState());
		enterRule(_localctx, 0, RULE_root);
		int _la;
		try {
			setState(109);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case K_MATCH:
			case K_SET:
				enterOuterAlt(_localctx, 1);
				{
				setState(91);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if (_la==K_SET) {
					{
					setState(90);
					setStatement();
					}
				}

				setState(93);
				matchStatement();
				setState(95);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if (_la==K_WHERE) {
					{
					setState(94);
					whereStatement();
					}
				}

				setState(98);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if (_la==K_GROUP) {
					{
					setState(97);
					groupByStatement();
					}
				}

				setState(101);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if (_la==K_ORDER) {
					{
					setState(100);
					orderByStatement();
					}
				}

				setState(103);
				returnStatement();
				setState(104);
				match(EOF);
				}
				break;
			case K_DESCRIBE:
				enterOuterAlt(_localctx, 2);
				{
				setState(106);
				describeStatement();
				setState(107);
				match(EOF);
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class DescribeStatementContext extends ParserRuleContext {
		public TerminalNode K_DESCRIBE() { return getToken(MDBParser.K_DESCRIBE, 0); }
		public FixedNodeInsideContext fixedNodeInside() {
			return getRuleContext(FixedNodeInsideContext.class,0);
		}
		public DescribeStatementContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_describeStatement; }
	}

	public final DescribeStatementContext describeStatement() throws RecognitionException {
		DescribeStatementContext _localctx = new DescribeStatementContext(_ctx, getState());
		enterRule(_localctx, 2, RULE_describeStatement);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(111);
			match(K_DESCRIBE);
			setState(112);
			fixedNodeInside();
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class SetStatementContext extends ParserRuleContext {
		public TerminalNode K_SET() { return getToken(MDBParser.K_SET, 0); }
		public List<SetItemContext> setItem() {
			return getRuleContexts(SetItemContext.class);
		}
		public SetItemContext setItem(int i) {
			return getRuleContext(SetItemContext.class,i);
		}
		public List<TerminalNode> COMMA() { return getTokens(MDBParser.COMMA); }
		public TerminalNode COMMA(int i) {
			return getToken(MDBParser.COMMA, i);
		}
		public SetStatementContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_setStatement; }
	}

	public final SetStatementContext setStatement() throws RecognitionException {
		SetStatementContext _localctx = new SetStatementContext(_ctx, getState());
		enterRule(_localctx, 4, RULE_setStatement);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(114);
			match(K_SET);
			setState(115);
			setItem();
			setState(120);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==COMMA) {
				{
				{
				setState(116);
				match(COMMA);
				setState(117);
				setItem();
				}
				}
				setState(122);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class MatchStatementContext extends ParserRuleContext {
		public TerminalNode K_MATCH() { return getToken(MDBParser.K_MATCH, 0); }
		public GraphPatternContext graphPattern() {
			return getRuleContext(GraphPatternContext.class,0);
		}
		public MatchStatementContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_matchStatement; }
	}

	public final MatchStatementContext matchStatement() throws RecognitionException {
		MatchStatementContext _localctx = new MatchStatementContext(_ctx, getState());
		enterRule(_localctx, 6, RULE_matchStatement);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(123);
			match(K_MATCH);
			setState(124);
			graphPattern();
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class WhereStatementContext extends ParserRuleContext {
		public TerminalNode K_WHERE() { return getToken(MDBParser.K_WHERE, 0); }
		public ConditionalOrExprContext conditionalOrExpr() {
			return getRuleContext(ConditionalOrExprContext.class,0);
		}
		public WhereStatementContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_whereStatement; }
	}

	public final WhereStatementContext whereStatement() throws RecognitionException {
		WhereStatementContext _localctx = new WhereStatementContext(_ctx, getState());
		enterRule(_localctx, 8, RULE_whereStatement);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(126);
			match(K_WHERE);
			setState(127);
			conditionalOrExpr();
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class GroupByStatementContext extends ParserRuleContext {
		public TerminalNode K_GROUP() { return getToken(MDBParser.K_GROUP, 0); }
		public TerminalNode K_BY() { return getToken(MDBParser.K_BY, 0); }
		public List<GroupByItemContext> groupByItem() {
			return getRuleContexts(GroupByItemContext.class);
		}
		public GroupByItemContext groupByItem(int i) {
			return getRuleContext(GroupByItemContext.class,i);
		}
		public List<TerminalNode> COMMA() { return getTokens(MDBParser.COMMA); }
		public TerminalNode COMMA(int i) {
			return getToken(MDBParser.COMMA, i);
		}
		public GroupByStatementContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_groupByStatement; }
	}

	public final GroupByStatementContext groupByStatement() throws RecognitionException {
		GroupByStatementContext _localctx = new GroupByStatementContext(_ctx, getState());
		enterRule(_localctx, 10, RULE_groupByStatement);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(129);
			match(K_GROUP);
			setState(130);
			match(K_BY);
			setState(131);
			groupByItem();
			setState(136);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==COMMA) {
				{
				{
				setState(132);
				match(COMMA);
				setState(133);
				groupByItem();
				}
				}
				setState(138);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class OrderByStatementContext extends ParserRuleContext {
		public TerminalNode K_ORDER() { return getToken(MDBParser.K_ORDER, 0); }
		public TerminalNode K_BY() { return getToken(MDBParser.K_BY, 0); }
		public List<OrderByItemContext> orderByItem() {
			return getRuleContexts(OrderByItemContext.class);
		}
		public OrderByItemContext orderByItem(int i) {
			return getRuleContext(OrderByItemContext.class,i);
		}
		public List<TerminalNode> COMMA() { return getTokens(MDBParser.COMMA); }
		public TerminalNode COMMA(int i) {
			return getToken(MDBParser.COMMA, i);
		}
		public OrderByStatementContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_orderByStatement; }
	}

	public final OrderByStatementContext orderByStatement() throws RecognitionException {
		OrderByStatementContext _localctx = new OrderByStatementContext(_ctx, getState());
		enterRule(_localctx, 12, RULE_orderByStatement);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(139);
			match(K_ORDER);
			setState(140);
			match(K_BY);
			setState(141);
			orderByItem();
			setState(146);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==COMMA) {
				{
				{
				setState(142);
				match(COMMA);
				setState(143);
				orderByItem();
				}
				}
				setState(148);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class ReturnStatementContext extends ParserRuleContext {
		public ReturnStatementContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_returnStatement; }
	 
		public ReturnStatementContext() { }
		public void copyFrom(ReturnStatementContext ctx) {
			super.copyFrom(ctx);
		}
	}
	public static class ReturnAllContext extends ReturnStatementContext {
		public TerminalNode K_RETURN() { return getToken(MDBParser.K_RETURN, 0); }
		public TerminalNode STAR() { return getToken(MDBParser.STAR, 0); }
		public TerminalNode K_DISTINCT() { return getToken(MDBParser.K_DISTINCT, 0); }
		public TerminalNode K_LIMIT() { return getToken(MDBParser.K_LIMIT, 0); }
		public TerminalNode UNSIGNED_INTEGER() { return getToken(MDBParser.UNSIGNED_INTEGER, 0); }
		public ReturnAllContext(ReturnStatementContext ctx) { copyFrom(ctx); }
	}
	public static class ReturnListContext extends ReturnStatementContext {
		public TerminalNode K_RETURN() { return getToken(MDBParser.K_RETURN, 0); }
		public List<ReturnItemContext> returnItem() {
			return getRuleContexts(ReturnItemContext.class);
		}
		public ReturnItemContext returnItem(int i) {
			return getRuleContext(ReturnItemContext.class,i);
		}
		public TerminalNode K_DISTINCT() { return getToken(MDBParser.K_DISTINCT, 0); }
		public List<TerminalNode> COMMA() { return getTokens(MDBParser.COMMA); }
		public TerminalNode COMMA(int i) {
			return getToken(MDBParser.COMMA, i);
		}
		public TerminalNode K_LIMIT() { return getToken(MDBParser.K_LIMIT, 0); }
		public TerminalNode UNSIGNED_INTEGER() { return getToken(MDBParser.UNSIGNED_INTEGER, 0); }
		public ReturnListContext(ReturnStatementContext ctx) { copyFrom(ctx); }
	}

	public final ReturnStatementContext returnStatement() throws RecognitionException {
		ReturnStatementContext _localctx = new ReturnStatementContext(_ctx, getState());
		enterRule(_localctx, 14, RULE_returnStatement);
		int _la;
		try {
			setState(174);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,13,_ctx) ) {
			case 1:
				_localctx = new ReturnListContext(_localctx);
				enterOuterAlt(_localctx, 1);
				{
				setState(149);
				match(K_RETURN);
				setState(151);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if (_la==K_DISTINCT) {
					{
					setState(150);
					match(K_DISTINCT);
					}
				}

				setState(153);
				returnItem();
				setState(158);
				_errHandler.sync(this);
				_la = _input.LA(1);
				while (_la==COMMA) {
					{
					{
					setState(154);
					match(COMMA);
					setState(155);
					returnItem();
					}
					}
					setState(160);
					_errHandler.sync(this);
					_la = _input.LA(1);
				}
				setState(163);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if (_la==K_LIMIT) {
					{
					setState(161);
					match(K_LIMIT);
					setState(162);
					match(UNSIGNED_INTEGER);
					}
				}

				}
				break;
			case 2:
				_localctx = new ReturnAllContext(_localctx);
				enterOuterAlt(_localctx, 2);
				{
				setState(165);
				match(K_RETURN);
				setState(167);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if (_la==K_DISTINCT) {
					{
					setState(166);
					match(K_DISTINCT);
					}
				}

				setState(169);
				match(STAR);
				setState(172);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if (_la==K_LIMIT) {
					{
					setState(170);
					match(K_LIMIT);
					setState(171);
					match(UNSIGNED_INTEGER);
					}
				}

				}
				break;
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class SetItemContext extends ParserRuleContext {
		public TerminalNode VARIABLE() { return getToken(MDBParser.VARIABLE, 0); }
		public TerminalNode SINGLE_EQ() { return getToken(MDBParser.SINGLE_EQ, 0); }
		public FixedNodeInsideContext fixedNodeInside() {
			return getRuleContext(FixedNodeInsideContext.class,0);
		}
		public SetItemContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_setItem; }
	}

	public final SetItemContext setItem() throws RecognitionException {
		SetItemContext _localctx = new SetItemContext(_ctx, getState());
		enterRule(_localctx, 16, RULE_setItem);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(176);
			match(VARIABLE);
			setState(177);
			match(SINGLE_EQ);
			setState(178);
			fixedNodeInside();
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class ReturnItemContext extends ParserRuleContext {
		public ReturnItemContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_returnItem; }
	 
		public ReturnItemContext() { }
		public void copyFrom(ReturnItemContext ctx) {
			super.copyFrom(ctx);
		}
	}
	public static class ReturnItemVarContext extends ReturnItemContext {
		public TerminalNode VARIABLE() { return getToken(MDBParser.VARIABLE, 0); }
		public TerminalNode KEY() { return getToken(MDBParser.KEY, 0); }
		public ReturnItemVarContext(ReturnItemContext ctx) { copyFrom(ctx); }
	}
	public static class ReturnItemAggContext extends ReturnItemContext {
		public AggregateFuncContext aggregateFunc() {
			return getRuleContext(AggregateFuncContext.class,0);
		}
		public TerminalNode L_PAR() { return getToken(MDBParser.L_PAR, 0); }
		public TerminalNode VARIABLE() { return getToken(MDBParser.VARIABLE, 0); }
		public TerminalNode R_PAR() { return getToken(MDBParser.R_PAR, 0); }
		public TerminalNode KEY() { return getToken(MDBParser.KEY, 0); }
		public ReturnItemAggContext(ReturnItemContext ctx) { copyFrom(ctx); }
	}
	public static class ReturnItemCountContext extends ReturnItemContext {
		public TerminalNode K_COUNT() { return getToken(MDBParser.K_COUNT, 0); }
		public TerminalNode L_PAR() { return getToken(MDBParser.L_PAR, 0); }
		public TerminalNode R_PAR() { return getToken(MDBParser.R_PAR, 0); }
		public TerminalNode VARIABLE() { return getToken(MDBParser.VARIABLE, 0); }
		public TerminalNode STAR() { return getToken(MDBParser.STAR, 0); }
		public TerminalNode K_DISTINCT() { return getToken(MDBParser.K_DISTINCT, 0); }
		public TerminalNode KEY() { return getToken(MDBParser.KEY, 0); }
		public ReturnItemCountContext(ReturnItemContext ctx) { copyFrom(ctx); }
	}

	public final ReturnItemContext returnItem() throws RecognitionException {
		ReturnItemContext _localctx = new ReturnItemContext(_ctx, getState());
		enterRule(_localctx, 18, RULE_returnItem);
		int _la;
		try {
			setState(205);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case VARIABLE:
				_localctx = new ReturnItemVarContext(_localctx);
				enterOuterAlt(_localctx, 1);
				{
				setState(180);
				match(VARIABLE);
				setState(182);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if (_la==KEY) {
					{
					setState(181);
					match(KEY);
					}
				}

				}
				break;
			case K_AVG:
			case K_MAX:
			case K_MIN:
			case K_SUM:
				_localctx = new ReturnItemAggContext(_localctx);
				enterOuterAlt(_localctx, 2);
				{
				setState(184);
				aggregateFunc();
				setState(185);
				match(L_PAR);
				setState(186);
				match(VARIABLE);
				setState(188);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if (_la==KEY) {
					{
					setState(187);
					match(KEY);
					}
				}

				setState(190);
				match(R_PAR);
				}
				break;
			case K_COUNT:
				_localctx = new ReturnItemCountContext(_localctx);
				enterOuterAlt(_localctx, 3);
				{
				setState(192);
				match(K_COUNT);
				setState(193);
				match(L_PAR);
				setState(195);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if (_la==K_DISTINCT) {
					{
					setState(194);
					match(K_DISTINCT);
					}
				}

				setState(202);
				_errHandler.sync(this);
				switch (_input.LA(1)) {
				case VARIABLE:
					{
					setState(197);
					match(VARIABLE);
					setState(199);
					_errHandler.sync(this);
					_la = _input.LA(1);
					if (_la==KEY) {
						{
						setState(198);
						match(KEY);
						}
					}

					}
					break;
				case STAR:
					{
					setState(201);
					match(STAR);
					}
					break;
				default:
					throw new NoViableAltException(this);
				}
				setState(204);
				match(R_PAR);
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class AggregateFuncContext extends ParserRuleContext {
		public TerminalNode K_SUM() { return getToken(MDBParser.K_SUM, 0); }
		public TerminalNode K_MAX() { return getToken(MDBParser.K_MAX, 0); }
		public TerminalNode K_MIN() { return getToken(MDBParser.K_MIN, 0); }
		public TerminalNode K_AVG() { return getToken(MDBParser.K_AVG, 0); }
		public AggregateFuncContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_aggregateFunc; }
	}

	public final AggregateFuncContext aggregateFunc() throws RecognitionException {
		AggregateFuncContext _localctx = new AggregateFuncContext(_ctx, getState());
		enterRule(_localctx, 20, RULE_aggregateFunc);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(207);
			_la = _input.LA(1);
			if ( !((((_la) & ~0x3f) == 0 && ((1L << _la) & ((1L << K_AVG) | (1L << K_MAX) | (1L << K_MIN) | (1L << K_SUM))) != 0)) ) {
			_errHandler.recoverInline(this);
			}
			else {
				if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
				_errHandler.reportMatch(this);
				consume();
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class OrderByItemContext extends ParserRuleContext {
		public OrderByItemContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_orderByItem; }
	 
		public OrderByItemContext() { }
		public void copyFrom(OrderByItemContext ctx) {
			super.copyFrom(ctx);
		}
	}
	public static class OrderByItemCountContext extends OrderByItemContext {
		public TerminalNode K_COUNT() { return getToken(MDBParser.K_COUNT, 0); }
		public TerminalNode L_PAR() { return getToken(MDBParser.L_PAR, 0); }
		public TerminalNode VARIABLE() { return getToken(MDBParser.VARIABLE, 0); }
		public TerminalNode R_PAR() { return getToken(MDBParser.R_PAR, 0); }
		public TerminalNode K_DISTINCT() { return getToken(MDBParser.K_DISTINCT, 0); }
		public TerminalNode KEY() { return getToken(MDBParser.KEY, 0); }
		public TerminalNode K_ASC() { return getToken(MDBParser.K_ASC, 0); }
		public TerminalNode K_DESC() { return getToken(MDBParser.K_DESC, 0); }
		public OrderByItemCountContext(OrderByItemContext ctx) { copyFrom(ctx); }
	}
	public static class OrderByItemVarContext extends OrderByItemContext {
		public TerminalNode VARIABLE() { return getToken(MDBParser.VARIABLE, 0); }
		public TerminalNode KEY() { return getToken(MDBParser.KEY, 0); }
		public TerminalNode K_ASC() { return getToken(MDBParser.K_ASC, 0); }
		public TerminalNode K_DESC() { return getToken(MDBParser.K_DESC, 0); }
		public OrderByItemVarContext(OrderByItemContext ctx) { copyFrom(ctx); }
	}
	public static class OrderByItemAggContext extends OrderByItemContext {
		public AggregateFuncContext aggregateFunc() {
			return getRuleContext(AggregateFuncContext.class,0);
		}
		public TerminalNode L_PAR() { return getToken(MDBParser.L_PAR, 0); }
		public TerminalNode VARIABLE() { return getToken(MDBParser.VARIABLE, 0); }
		public TerminalNode R_PAR() { return getToken(MDBParser.R_PAR, 0); }
		public TerminalNode KEY() { return getToken(MDBParser.KEY, 0); }
		public TerminalNode K_ASC() { return getToken(MDBParser.K_ASC, 0); }
		public TerminalNode K_DESC() { return getToken(MDBParser.K_DESC, 0); }
		public OrderByItemAggContext(OrderByItemContext ctx) { copyFrom(ctx); }
	}

	public final OrderByItemContext orderByItem() throws RecognitionException {
		OrderByItemContext _localctx = new OrderByItemContext(_ctx, getState());
		enterRule(_localctx, 22, RULE_orderByItem);
		int _la;
		try {
			setState(239);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case VARIABLE:
				_localctx = new OrderByItemVarContext(_localctx);
				enterOuterAlt(_localctx, 1);
				{
				setState(209);
				match(VARIABLE);
				setState(211);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if (_la==KEY) {
					{
					setState(210);
					match(KEY);
					}
				}

				setState(214);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if (_la==K_ASC || _la==K_DESC) {
					{
					setState(213);
					_la = _input.LA(1);
					if ( !(_la==K_ASC || _la==K_DESC) ) {
					_errHandler.recoverInline(this);
					}
					else {
						if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
						_errHandler.reportMatch(this);
						consume();
					}
					}
				}

				}
				break;
			case K_AVG:
			case K_MAX:
			case K_MIN:
			case K_SUM:
				_localctx = new OrderByItemAggContext(_localctx);
				enterOuterAlt(_localctx, 2);
				{
				setState(216);
				aggregateFunc();
				setState(217);
				match(L_PAR);
				setState(218);
				match(VARIABLE);
				setState(220);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if (_la==KEY) {
					{
					setState(219);
					match(KEY);
					}
				}

				setState(222);
				match(R_PAR);
				setState(224);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if (_la==K_ASC || _la==K_DESC) {
					{
					setState(223);
					_la = _input.LA(1);
					if ( !(_la==K_ASC || _la==K_DESC) ) {
					_errHandler.recoverInline(this);
					}
					else {
						if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
						_errHandler.reportMatch(this);
						consume();
					}
					}
				}

				}
				break;
			case K_COUNT:
				_localctx = new OrderByItemCountContext(_localctx);
				enterOuterAlt(_localctx, 3);
				{
				setState(226);
				match(K_COUNT);
				setState(227);
				match(L_PAR);
				setState(229);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if (_la==K_DISTINCT) {
					{
					setState(228);
					match(K_DISTINCT);
					}
				}

				setState(231);
				match(VARIABLE);
				setState(233);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if (_la==KEY) {
					{
					setState(232);
					match(KEY);
					}
				}

				setState(235);
				match(R_PAR);
				setState(237);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if (_la==K_ASC || _la==K_DESC) {
					{
					setState(236);
					_la = _input.LA(1);
					if ( !(_la==K_ASC || _la==K_DESC) ) {
					_errHandler.recoverInline(this);
					}
					else {
						if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
						_errHandler.reportMatch(this);
						consume();
					}
					}
				}

				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class GroupByItemContext extends ParserRuleContext {
		public TerminalNode VARIABLE() { return getToken(MDBParser.VARIABLE, 0); }
		public TerminalNode KEY() { return getToken(MDBParser.KEY, 0); }
		public GroupByItemContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_groupByItem; }
	}

	public final GroupByItemContext groupByItem() throws RecognitionException {
		GroupByItemContext _localctx = new GroupByItemContext(_ctx, getState());
		enterRule(_localctx, 24, RULE_groupByItem);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(241);
			match(VARIABLE);
			setState(243);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==KEY) {
				{
				setState(242);
				match(KEY);
				}
			}

			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class GraphPatternContext extends ParserRuleContext {
		public BasicPatternContext basicPattern() {
			return getRuleContext(BasicPatternContext.class,0);
		}
		public List<OptionalPatternContext> optionalPattern() {
			return getRuleContexts(OptionalPatternContext.class);
		}
		public OptionalPatternContext optionalPattern(int i) {
			return getRuleContext(OptionalPatternContext.class,i);
		}
		public GraphPatternContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_graphPattern; }
	}

	public final GraphPatternContext graphPattern() throws RecognitionException {
		GraphPatternContext _localctx = new GraphPatternContext(_ctx, getState());
		enterRule(_localctx, 26, RULE_graphPattern);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(245);
			basicPattern();
			setState(249);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==K_OPTIONAL) {
				{
				{
				setState(246);
				optionalPattern();
				}
				}
				setState(251);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class OptionalPatternContext extends ParserRuleContext {
		public TerminalNode K_OPTIONAL() { return getToken(MDBParser.K_OPTIONAL, 0); }
		public TerminalNode LCURLY_BRACKET() { return getToken(MDBParser.LCURLY_BRACKET, 0); }
		public GraphPatternContext graphPattern() {
			return getRuleContext(GraphPatternContext.class,0);
		}
		public TerminalNode RCURLY_BRACKET() { return getToken(MDBParser.RCURLY_BRACKET, 0); }
		public OptionalPatternContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_optionalPattern; }
	}

	public final OptionalPatternContext optionalPattern() throws RecognitionException {
		OptionalPatternContext _localctx = new OptionalPatternContext(_ctx, getState());
		enterRule(_localctx, 28, RULE_optionalPattern);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(252);
			match(K_OPTIONAL);
			setState(253);
			match(LCURLY_BRACKET);
			setState(254);
			graphPattern();
			setState(255);
			match(RCURLY_BRACKET);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class BasicPatternContext extends ParserRuleContext {
		public List<LinearPatternContext> linearPattern() {
			return getRuleContexts(LinearPatternContext.class);
		}
		public LinearPatternContext linearPattern(int i) {
			return getRuleContext(LinearPatternContext.class,i);
		}
		public List<TerminalNode> COMMA() { return getTokens(MDBParser.COMMA); }
		public TerminalNode COMMA(int i) {
			return getToken(MDBParser.COMMA, i);
		}
		public BasicPatternContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_basicPattern; }
	}

	public final BasicPatternContext basicPattern() throws RecognitionException {
		BasicPatternContext _localctx = new BasicPatternContext(_ctx, getState());
		enterRule(_localctx, 30, RULE_basicPattern);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(257);
			linearPattern();
			setState(262);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==COMMA) {
				{
				{
				setState(258);
				match(COMMA);
				setState(259);
				linearPattern();
				}
				}
				setState(264);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class LinearPatternContext extends ParserRuleContext {
		public List<NodeContext> node() {
			return getRuleContexts(NodeContext.class);
		}
		public NodeContext node(int i) {
			return getRuleContext(NodeContext.class,i);
		}
		public List<EdgeContext> edge() {
			return getRuleContexts(EdgeContext.class);
		}
		public EdgeContext edge(int i) {
			return getRuleContext(EdgeContext.class,i);
		}
		public List<PathContext> path() {
			return getRuleContexts(PathContext.class);
		}
		public PathContext path(int i) {
			return getRuleContext(PathContext.class,i);
		}
		public LinearPatternContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_linearPattern; }
	}

	public final LinearPatternContext linearPattern() throws RecognitionException {
		LinearPatternContext _localctx = new LinearPatternContext(_ctx, getState());
		enterRule(_localctx, 32, RULE_linearPattern);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(265);
			node();
			setState(274);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while ((((_la) & ~0x3f) == 0 && ((1L << _la) & ((1L << LEQ) | (1L << LT) | (1L << SINGLE_EQ) | (1L << MINUS))) != 0)) {
				{
				{
				setState(268);
				_errHandler.sync(this);
				switch (_input.LA(1)) {
				case LT:
				case MINUS:
					{
					setState(266);
					edge();
					}
					break;
				case LEQ:
				case SINGLE_EQ:
					{
					setState(267);
					path();
					}
					break;
				default:
					throw new NoViableAltException(this);
				}
				setState(270);
				node();
				}
				}
				setState(276);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class PathContext extends ParserRuleContext {
		public TerminalNode LEQ() { return getToken(MDBParser.LEQ, 0); }
		public TerminalNode LSQUARE_BRACKET() { return getToken(MDBParser.LSQUARE_BRACKET, 0); }
		public PathAlternativesContext pathAlternatives() {
			return getRuleContext(PathAlternativesContext.class,0);
		}
		public TerminalNode RSQUARE_BRACKET() { return getToken(MDBParser.RSQUARE_BRACKET, 0); }
		public List<TerminalNode> SINGLE_EQ() { return getTokens(MDBParser.SINGLE_EQ); }
		public TerminalNode SINGLE_EQ(int i) {
			return getToken(MDBParser.SINGLE_EQ, i);
		}
		public PathTypeContext pathType() {
			return getRuleContext(PathTypeContext.class,0);
		}
		public TerminalNode VARIABLE() { return getToken(MDBParser.VARIABLE, 0); }
		public TerminalNode GT() { return getToken(MDBParser.GT, 0); }
		public PathContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_path; }
	}

	public final PathContext path() throws RecognitionException {
		PathContext _localctx = new PathContext(_ctx, getState());
		enterRule(_localctx, 34, RULE_path);
		int _la;
		try {
			setState(302);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case LEQ:
				enterOuterAlt(_localctx, 1);
				{
				setState(277);
				match(LEQ);
				setState(278);
				match(LSQUARE_BRACKET);
				setState(280);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if (_la==K_ANY || _la==K_ALL) {
					{
					setState(279);
					pathType();
					}
				}

				setState(283);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if (_la==VARIABLE) {
					{
					setState(282);
					match(VARIABLE);
					}
				}

				setState(285);
				pathAlternatives();
				setState(286);
				match(RSQUARE_BRACKET);
				setState(287);
				match(SINGLE_EQ);
				}
				break;
			case SINGLE_EQ:
				enterOuterAlt(_localctx, 2);
				{
				setState(289);
				match(SINGLE_EQ);
				setState(290);
				match(LSQUARE_BRACKET);
				setState(292);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if (_la==K_ANY || _la==K_ALL) {
					{
					setState(291);
					pathType();
					}
				}

				setState(295);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if (_la==VARIABLE) {
					{
					setState(294);
					match(VARIABLE);
					}
				}

				setState(297);
				pathAlternatives();
				setState(298);
				match(RSQUARE_BRACKET);
				setState(299);
				match(SINGLE_EQ);
				setState(300);
				match(GT);
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class PathAlternativesContext extends ParserRuleContext {
		public List<PathSequenceContext> pathSequence() {
			return getRuleContexts(PathSequenceContext.class);
		}
		public PathSequenceContext pathSequence(int i) {
			return getRuleContext(PathSequenceContext.class,i);
		}
		public List<TerminalNode> PATH_ALTERNATIVE() { return getTokens(MDBParser.PATH_ALTERNATIVE); }
		public TerminalNode PATH_ALTERNATIVE(int i) {
			return getToken(MDBParser.PATH_ALTERNATIVE, i);
		}
		public PathAlternativesContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_pathAlternatives; }
	}

	public final PathAlternativesContext pathAlternatives() throws RecognitionException {
		PathAlternativesContext _localctx = new PathAlternativesContext(_ctx, getState());
		enterRule(_localctx, 36, RULE_pathAlternatives);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(304);
			pathSequence();
			setState(309);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==PATH_ALTERNATIVE) {
				{
				{
				setState(305);
				match(PATH_ALTERNATIVE);
				setState(306);
				pathSequence();
				}
				}
				setState(311);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class PathSequenceContext extends ParserRuleContext {
		public List<PathAtomContext> pathAtom() {
			return getRuleContexts(PathAtomContext.class);
		}
		public PathAtomContext pathAtom(int i) {
			return getRuleContext(PathAtomContext.class,i);
		}
		public List<TerminalNode> PATH_SEQUENCE() { return getTokens(MDBParser.PATH_SEQUENCE); }
		public TerminalNode PATH_SEQUENCE(int i) {
			return getToken(MDBParser.PATH_SEQUENCE, i);
		}
		public PathSequenceContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_pathSequence; }
	}

	public final PathSequenceContext pathSequence() throws RecognitionException {
		PathSequenceContext _localctx = new PathSequenceContext(_ctx, getState());
		enterRule(_localctx, 38, RULE_pathSequence);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(312);
			pathAtom();
			setState(317);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==PATH_SEQUENCE) {
				{
				{
				setState(313);
				match(PATH_SEQUENCE);
				setState(314);
				pathAtom();
				}
				}
				setState(319);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class PathAtomContext extends ParserRuleContext {
		public PathAtomContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_pathAtom; }
	 
		public PathAtomContext() { }
		public void copyFrom(PathAtomContext ctx) {
			super.copyFrom(ctx);
		}
	}
	public static class PathAtomAlternativesContext extends PathAtomContext {
		public TerminalNode L_PAR() { return getToken(MDBParser.L_PAR, 0); }
		public PathAlternativesContext pathAlternatives() {
			return getRuleContext(PathAlternativesContext.class,0);
		}
		public TerminalNode R_PAR() { return getToken(MDBParser.R_PAR, 0); }
		public TerminalNode PATH_NEGATION() { return getToken(MDBParser.PATH_NEGATION, 0); }
		public PathSuffixContext pathSuffix() {
			return getRuleContext(PathSuffixContext.class,0);
		}
		public PathAtomAlternativesContext(PathAtomContext ctx) { copyFrom(ctx); }
	}
	public static class PathAtomSimpleContext extends PathAtomContext {
		public TerminalNode TYPE() { return getToken(MDBParser.TYPE, 0); }
		public TerminalNode PATH_NEGATION() { return getToken(MDBParser.PATH_NEGATION, 0); }
		public PathSuffixContext pathSuffix() {
			return getRuleContext(PathSuffixContext.class,0);
		}
		public PathAtomSimpleContext(PathAtomContext ctx) { copyFrom(ctx); }
	}

	public final PathAtomContext pathAtom() throws RecognitionException {
		PathAtomContext _localctx = new PathAtomContext(_ctx, getState());
		enterRule(_localctx, 40, RULE_pathAtom);
		int _la;
		try {
			setState(336);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,44,_ctx) ) {
			case 1:
				_localctx = new PathAtomSimpleContext(_localctx);
				enterOuterAlt(_localctx, 1);
				{
				setState(321);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if (_la==PATH_NEGATION) {
					{
					setState(320);
					match(PATH_NEGATION);
					}
				}

				setState(323);
				match(TYPE);
				setState(325);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if ((((_la) & ~0x3f) == 0 && ((1L << _la) & ((1L << STAR) | (1L << QUESTION_MARK) | (1L << PLUS) | (1L << LCURLY_BRACKET))) != 0)) {
					{
					setState(324);
					pathSuffix();
					}
				}

				}
				break;
			case 2:
				_localctx = new PathAtomAlternativesContext(_localctx);
				enterOuterAlt(_localctx, 2);
				{
				setState(328);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if (_la==PATH_NEGATION) {
					{
					setState(327);
					match(PATH_NEGATION);
					}
				}

				setState(330);
				match(L_PAR);
				setState(331);
				pathAlternatives();
				setState(332);
				match(R_PAR);
				setState(334);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if ((((_la) & ~0x3f) == 0 && ((1L << _la) & ((1L << STAR) | (1L << QUESTION_MARK) | (1L << PLUS) | (1L << LCURLY_BRACKET))) != 0)) {
					{
					setState(333);
					pathSuffix();
					}
				}

				}
				break;
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class PathSuffixContext extends ParserRuleContext {
		public Token op;
		public Token min;
		public Token max;
		public TerminalNode STAR() { return getToken(MDBParser.STAR, 0); }
		public TerminalNode PLUS() { return getToken(MDBParser.PLUS, 0); }
		public TerminalNode QUESTION_MARK() { return getToken(MDBParser.QUESTION_MARK, 0); }
		public TerminalNode LCURLY_BRACKET() { return getToken(MDBParser.LCURLY_BRACKET, 0); }
		public TerminalNode COMMA() { return getToken(MDBParser.COMMA, 0); }
		public TerminalNode RCURLY_BRACKET() { return getToken(MDBParser.RCURLY_BRACKET, 0); }
		public List<TerminalNode> UNSIGNED_INTEGER() { return getTokens(MDBParser.UNSIGNED_INTEGER); }
		public TerminalNode UNSIGNED_INTEGER(int i) {
			return getToken(MDBParser.UNSIGNED_INTEGER, i);
		}
		public PathSuffixContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_pathSuffix; }
	}

	public final PathSuffixContext pathSuffix() throws RecognitionException {
		PathSuffixContext _localctx = new PathSuffixContext(_ctx, getState());
		enterRule(_localctx, 42, RULE_pathSuffix);
		try {
			setState(346);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case STAR:
				enterOuterAlt(_localctx, 1);
				{
				setState(338);
				((PathSuffixContext)_localctx).op = match(STAR);
				}
				break;
			case PLUS:
				enterOuterAlt(_localctx, 2);
				{
				setState(339);
				((PathSuffixContext)_localctx).op = match(PLUS);
				}
				break;
			case QUESTION_MARK:
				enterOuterAlt(_localctx, 3);
				{
				setState(340);
				((PathSuffixContext)_localctx).op = match(QUESTION_MARK);
				}
				break;
			case LCURLY_BRACKET:
				enterOuterAlt(_localctx, 4);
				{
				setState(341);
				match(LCURLY_BRACKET);
				setState(342);
				((PathSuffixContext)_localctx).min = match(UNSIGNED_INTEGER);
				setState(343);
				match(COMMA);
				setState(344);
				((PathSuffixContext)_localctx).max = match(UNSIGNED_INTEGER);
				setState(345);
				match(RCURLY_BRACKET);
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class PathTypeContext extends ParserRuleContext {
		public TerminalNode K_ANY() { return getToken(MDBParser.K_ANY, 0); }
		public TerminalNode K_ALL() { return getToken(MDBParser.K_ALL, 0); }
		public PathTypeContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_pathType; }
	}

	public final PathTypeContext pathType() throws RecognitionException {
		PathTypeContext _localctx = new PathTypeContext(_ctx, getState());
		enterRule(_localctx, 44, RULE_pathType);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(348);
			_la = _input.LA(1);
			if ( !(_la==K_ANY || _la==K_ALL) ) {
			_errHandler.recoverInline(this);
			}
			else {
				if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
				_errHandler.reportMatch(this);
				consume();
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class NodeContext extends ParserRuleContext {
		public FixedNodeContext fixedNode() {
			return getRuleContext(FixedNodeContext.class,0);
		}
		public VarNodeContext varNode() {
			return getRuleContext(VarNodeContext.class,0);
		}
		public NodeContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_node; }
	}

	public final NodeContext node() throws RecognitionException {
		NodeContext _localctx = new NodeContext(_ctx, getState());
		enterRule(_localctx, 46, RULE_node);
		try {
			setState(352);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,46,_ctx) ) {
			case 1:
				enterOuterAlt(_localctx, 1);
				{
				setState(350);
				fixedNode();
				}
				break;
			case 2:
				enterOuterAlt(_localctx, 2);
				{
				setState(351);
				varNode();
				}
				break;
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class FixedNodeContext extends ParserRuleContext {
		public TerminalNode L_PAR() { return getToken(MDBParser.L_PAR, 0); }
		public FixedNodeInsideContext fixedNodeInside() {
			return getRuleContext(FixedNodeInsideContext.class,0);
		}
		public TerminalNode R_PAR() { return getToken(MDBParser.R_PAR, 0); }
		public FixedNodeContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_fixedNode; }
	}

	public final FixedNodeContext fixedNode() throws RecognitionException {
		FixedNodeContext _localctx = new FixedNodeContext(_ctx, getState());
		enterRule(_localctx, 48, RULE_fixedNode);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(354);
			match(L_PAR);
			setState(355);
			fixedNodeInside();
			setState(356);
			match(R_PAR);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class FixedNodeInsideContext extends ParserRuleContext {
		public IdentifierContext identifier() {
			return getRuleContext(IdentifierContext.class,0);
		}
		public TerminalNode ANON_ID() { return getToken(MDBParser.ANON_ID, 0); }
		public TerminalNode EDGE_ID() { return getToken(MDBParser.EDGE_ID, 0); }
		public ValueContext value() {
			return getRuleContext(ValueContext.class,0);
		}
		public FixedNodeInsideContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_fixedNodeInside; }
	}

	public final FixedNodeInsideContext fixedNodeInside() throws RecognitionException {
		FixedNodeInsideContext _localctx = new FixedNodeInsideContext(_ctx, getState());
		enterRule(_localctx, 50, RULE_fixedNodeInside);
		try {
			setState(362);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case K_AND:
			case K_AVG:
			case K_ALL:
			case K_ASC:
			case K_BY:
			case K_BOOL:
			case K_COUNT:
			case K_DESCRIBE:
			case K_DESC:
			case K_DISTINCT:
			case K_IS:
			case K_INTEGER:
			case K_FLOAT:
			case K_GROUP:
			case K_LIMIT:
			case K_MAX:
			case K_MATCH:
			case K_MIN:
			case K_OPTIONAL:
			case K_ORDER:
			case K_OR:
			case K_NOT:
			case K_NULL:
			case K_SET:
			case K_SUM:
			case K_STRING:
			case K_RETURN:
			case K_WHERE:
			case NAME:
				enterOuterAlt(_localctx, 1);
				{
				setState(358);
				identifier();
				}
				break;
			case ANON_ID:
				enterOuterAlt(_localctx, 2);
				{
				setState(359);
				match(ANON_ID);
				}
				break;
			case EDGE_ID:
				enterOuterAlt(_localctx, 3);
				{
				setState(360);
				match(EDGE_ID);
				}
				break;
			case K_FALSE:
			case K_TRUE:
			case STRING:
			case UNSIGNED_INTEGER:
			case UNSIGNED_FLOAT:
			case PLUS:
			case MINUS:
				enterOuterAlt(_localctx, 4);
				{
				setState(361);
				value();
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class EdgeContext extends ParserRuleContext {
		public TerminalNode LT() { return getToken(MDBParser.LT, 0); }
		public List<TerminalNode> MINUS() { return getTokens(MDBParser.MINUS); }
		public TerminalNode MINUS(int i) {
			return getToken(MDBParser.MINUS, i);
		}
		public EdgeInsideContext edgeInside() {
			return getRuleContext(EdgeInsideContext.class,0);
		}
		public TerminalNode GT() { return getToken(MDBParser.GT, 0); }
		public EdgeContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_edge; }
	}

	public final EdgeContext edge() throws RecognitionException {
		EdgeContext _localctx = new EdgeContext(_ctx, getState());
		enterRule(_localctx, 52, RULE_edge);
		int _la;
		try {
			setState(377);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case LT:
				enterOuterAlt(_localctx, 1);
				{
				setState(364);
				match(LT);
				setState(367);
				_errHandler.sync(this);
				switch ( getInterpreter().adaptivePredict(_input,48,_ctx) ) {
				case 1:
					{
					setState(365);
					match(MINUS);
					setState(366);
					edgeInside();
					}
					break;
				}
				setState(369);
				match(MINUS);
				}
				break;
			case MINUS:
				enterOuterAlt(_localctx, 2);
				{
				setState(370);
				match(MINUS);
				setState(374);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if (_la==LSQUARE_BRACKET) {
					{
					setState(371);
					edgeInside();
					setState(372);
					match(MINUS);
					}
				}

				setState(376);
				match(GT);
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class EdgeInsideContext extends ParserRuleContext {
		public TerminalNode LSQUARE_BRACKET() { return getToken(MDBParser.LSQUARE_BRACKET, 0); }
		public TerminalNode RSQUARE_BRACKET() { return getToken(MDBParser.RSQUARE_BRACKET, 0); }
		public PropertiesContext properties() {
			return getRuleContext(PropertiesContext.class,0);
		}
		public TerminalNode VARIABLE() { return getToken(MDBParser.VARIABLE, 0); }
		public TerminalNode EDGE_ID() { return getToken(MDBParser.EDGE_ID, 0); }
		public TerminalNode TYPE() { return getToken(MDBParser.TYPE, 0); }
		public TerminalNode TYPE_VAR() { return getToken(MDBParser.TYPE_VAR, 0); }
		public EdgeInsideContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_edgeInside; }
	}

	public final EdgeInsideContext edgeInside() throws RecognitionException {
		EdgeInsideContext _localctx = new EdgeInsideContext(_ctx, getState());
		enterRule(_localctx, 54, RULE_edgeInside);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(379);
			match(LSQUARE_BRACKET);
			setState(381);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==EDGE_ID || _la==VARIABLE) {
				{
				setState(380);
				_la = _input.LA(1);
				if ( !(_la==EDGE_ID || _la==VARIABLE) ) {
				_errHandler.recoverInline(this);
				}
				else {
					if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
					_errHandler.reportMatch(this);
					consume();
				}
				}
			}

			setState(384);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==TYPE || _la==TYPE_VAR) {
				{
				setState(383);
				_la = _input.LA(1);
				if ( !(_la==TYPE || _la==TYPE_VAR) ) {
				_errHandler.recoverInline(this);
				}
				else {
					if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
					_errHandler.reportMatch(this);
					consume();
				}
				}
			}

			setState(387);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==LCURLY_BRACKET) {
				{
				setState(386);
				properties();
				}
			}

			setState(389);
			match(RSQUARE_BRACKET);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class VarNodeContext extends ParserRuleContext {
		public TerminalNode L_PAR() { return getToken(MDBParser.L_PAR, 0); }
		public TerminalNode R_PAR() { return getToken(MDBParser.R_PAR, 0); }
		public TerminalNode VARIABLE() { return getToken(MDBParser.VARIABLE, 0); }
		public List<TerminalNode> TYPE() { return getTokens(MDBParser.TYPE); }
		public TerminalNode TYPE(int i) {
			return getToken(MDBParser.TYPE, i);
		}
		public PropertiesContext properties() {
			return getRuleContext(PropertiesContext.class,0);
		}
		public VarNodeContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_varNode; }
	}

	public final VarNodeContext varNode() throws RecognitionException {
		VarNodeContext _localctx = new VarNodeContext(_ctx, getState());
		enterRule(_localctx, 56, RULE_varNode);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(391);
			match(L_PAR);
			setState(393);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==VARIABLE) {
				{
				setState(392);
				match(VARIABLE);
				}
			}

			setState(398);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==TYPE) {
				{
				{
				setState(395);
				match(TYPE);
				}
				}
				setState(400);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(402);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==LCURLY_BRACKET) {
				{
				setState(401);
				properties();
				}
			}

			setState(404);
			match(R_PAR);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class PropertiesContext extends ParserRuleContext {
		public TerminalNode LCURLY_BRACKET() { return getToken(MDBParser.LCURLY_BRACKET, 0); }
		public List<PropertyContext> property() {
			return getRuleContexts(PropertyContext.class);
		}
		public PropertyContext property(int i) {
			return getRuleContext(PropertyContext.class,i);
		}
		public TerminalNode RCURLY_BRACKET() { return getToken(MDBParser.RCURLY_BRACKET, 0); }
		public List<TerminalNode> COMMA() { return getTokens(MDBParser.COMMA); }
		public TerminalNode COMMA(int i) {
			return getToken(MDBParser.COMMA, i);
		}
		public PropertiesContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_properties; }
	}

	public final PropertiesContext properties() throws RecognitionException {
		PropertiesContext _localctx = new PropertiesContext(_ctx, getState());
		enterRule(_localctx, 58, RULE_properties);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(406);
			match(LCURLY_BRACKET);
			setState(407);
			property();
			setState(412);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==COMMA) {
				{
				{
				setState(408);
				match(COMMA);
				setState(409);
				property();
				}
				}
				setState(414);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(415);
			match(RCURLY_BRACKET);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class PropertyContext extends ParserRuleContext {
		public IdentifierContext identifier() {
			return getRuleContext(IdentifierContext.class,0);
		}
		public TerminalNode COLON() { return getToken(MDBParser.COLON, 0); }
		public ValueContext value() {
			return getRuleContext(ValueContext.class,0);
		}
		public TerminalNode TRUE_PROP() { return getToken(MDBParser.TRUE_PROP, 0); }
		public TerminalNode FALSE_PROP() { return getToken(MDBParser.FALSE_PROP, 0); }
		public PropertyContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_property; }
	}

	public final PropertyContext property() throws RecognitionException {
		PropertyContext _localctx = new PropertyContext(_ctx, getState());
		enterRule(_localctx, 60, RULE_property);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(417);
			identifier();
			setState(422);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case COLON:
				{
				setState(418);
				match(COLON);
				setState(419);
				value();
				}
				break;
			case TRUE_PROP:
				{
				setState(420);
				match(TRUE_PROP);
				}
				break;
			case FALSE_PROP:
				{
				setState(421);
				match(FALSE_PROP);
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class IdentifierContext extends ParserRuleContext {
		public TerminalNode NAME() { return getToken(MDBParser.NAME, 0); }
		public KeywordContext keyword() {
			return getRuleContext(KeywordContext.class,0);
		}
		public IdentifierContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_identifier; }
	}

	public final IdentifierContext identifier() throws RecognitionException {
		IdentifierContext _localctx = new IdentifierContext(_ctx, getState());
		enterRule(_localctx, 62, RULE_identifier);
		try {
			setState(426);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case NAME:
				enterOuterAlt(_localctx, 1);
				{
				setState(424);
				match(NAME);
				}
				break;
			case K_AND:
			case K_AVG:
			case K_ALL:
			case K_ASC:
			case K_BY:
			case K_BOOL:
			case K_COUNT:
			case K_DESCRIBE:
			case K_DESC:
			case K_DISTINCT:
			case K_IS:
			case K_INTEGER:
			case K_FLOAT:
			case K_GROUP:
			case K_LIMIT:
			case K_MAX:
			case K_MATCH:
			case K_MIN:
			case K_OPTIONAL:
			case K_ORDER:
			case K_OR:
			case K_NOT:
			case K_NULL:
			case K_SET:
			case K_SUM:
			case K_STRING:
			case K_RETURN:
			case K_WHERE:
				enterOuterAlt(_localctx, 2);
				{
				setState(425);
				keyword();
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class BoolValueContext extends ParserRuleContext {
		public TerminalNode K_TRUE() { return getToken(MDBParser.K_TRUE, 0); }
		public TerminalNode K_FALSE() { return getToken(MDBParser.K_FALSE, 0); }
		public BoolValueContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_boolValue; }
	}

	public final BoolValueContext boolValue() throws RecognitionException {
		BoolValueContext _localctx = new BoolValueContext(_ctx, getState());
		enterRule(_localctx, 64, RULE_boolValue);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(428);
			_la = _input.LA(1);
			if ( !(_la==K_FALSE || _la==K_TRUE) ) {
			_errHandler.recoverInline(this);
			}
			else {
				if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
				_errHandler.reportMatch(this);
				consume();
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class NumericValueContext extends ParserRuleContext {
		public TerminalNode UNSIGNED_INTEGER() { return getToken(MDBParser.UNSIGNED_INTEGER, 0); }
		public TerminalNode UNSIGNED_FLOAT() { return getToken(MDBParser.UNSIGNED_FLOAT, 0); }
		public TerminalNode PLUS() { return getToken(MDBParser.PLUS, 0); }
		public TerminalNode MINUS() { return getToken(MDBParser.MINUS, 0); }
		public NumericValueContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_numericValue; }
	}

	public final NumericValueContext numericValue() throws RecognitionException {
		NumericValueContext _localctx = new NumericValueContext(_ctx, getState());
		enterRule(_localctx, 66, RULE_numericValue);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(431);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==PLUS || _la==MINUS) {
				{
				setState(430);
				_la = _input.LA(1);
				if ( !(_la==PLUS || _la==MINUS) ) {
				_errHandler.recoverInline(this);
				}
				else {
					if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
					_errHandler.reportMatch(this);
					consume();
				}
				}
			}

			setState(433);
			_la = _input.LA(1);
			if ( !(_la==UNSIGNED_INTEGER || _la==UNSIGNED_FLOAT) ) {
			_errHandler.recoverInline(this);
			}
			else {
				if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
				_errHandler.reportMatch(this);
				consume();
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class ValueContext extends ParserRuleContext {
		public NumericValueContext numericValue() {
			return getRuleContext(NumericValueContext.class,0);
		}
		public TerminalNode STRING() { return getToken(MDBParser.STRING, 0); }
		public BoolValueContext boolValue() {
			return getRuleContext(BoolValueContext.class,0);
		}
		public ValueContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_value; }
	}

	public final ValueContext value() throws RecognitionException {
		ValueContext _localctx = new ValueContext(_ctx, getState());
		enterRule(_localctx, 68, RULE_value);
		try {
			setState(438);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case UNSIGNED_INTEGER:
			case UNSIGNED_FLOAT:
			case PLUS:
			case MINUS:
				enterOuterAlt(_localctx, 1);
				{
				setState(435);
				numericValue();
				}
				break;
			case STRING:
				enterOuterAlt(_localctx, 2);
				{
				setState(436);
				match(STRING);
				}
				break;
			case K_FALSE:
			case K_TRUE:
				enterOuterAlt(_localctx, 3);
				{
				setState(437);
				boolValue();
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class ConditionalOrExprContext extends ParserRuleContext {
		public List<ConditionalAndExprContext> conditionalAndExpr() {
			return getRuleContexts(ConditionalAndExprContext.class);
		}
		public ConditionalAndExprContext conditionalAndExpr(int i) {
			return getRuleContext(ConditionalAndExprContext.class,i);
		}
		public List<TerminalNode> K_OR() { return getTokens(MDBParser.K_OR); }
		public TerminalNode K_OR(int i) {
			return getToken(MDBParser.K_OR, i);
		}
		public ConditionalOrExprContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_conditionalOrExpr; }
	}

	public final ConditionalOrExprContext conditionalOrExpr() throws RecognitionException {
		ConditionalOrExprContext _localctx = new ConditionalOrExprContext(_ctx, getState());
		enterRule(_localctx, 70, RULE_conditionalOrExpr);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(440);
			conditionalAndExpr();
			setState(445);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==K_OR) {
				{
				{
				setState(441);
				match(K_OR);
				setState(442);
				conditionalAndExpr();
				}
				}
				setState(447);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class ConditionalAndExprContext extends ParserRuleContext {
		public List<ComparisonExprContext> comparisonExpr() {
			return getRuleContexts(ComparisonExprContext.class);
		}
		public ComparisonExprContext comparisonExpr(int i) {
			return getRuleContext(ComparisonExprContext.class,i);
		}
		public List<TerminalNode> K_AND() { return getTokens(MDBParser.K_AND); }
		public TerminalNode K_AND(int i) {
			return getToken(MDBParser.K_AND, i);
		}
		public ConditionalAndExprContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_conditionalAndExpr; }
	}

	public final ConditionalAndExprContext conditionalAndExpr() throws RecognitionException {
		ConditionalAndExprContext _localctx = new ConditionalAndExprContext(_ctx, getState());
		enterRule(_localctx, 72, RULE_conditionalAndExpr);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(448);
			comparisonExpr();
			setState(453);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==K_AND) {
				{
				{
				setState(449);
				match(K_AND);
				setState(450);
				comparisonExpr();
				}
				}
				setState(455);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class ComparisonExprContext extends ParserRuleContext {
		public ComparisonExprContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_comparisonExpr; }
	 
		public ComparisonExprContext() { }
		public void copyFrom(ComparisonExprContext ctx) {
			super.copyFrom(ctx);
		}
	}
	public static class ComparisonExprIsContext extends ComparisonExprContext {
		public AditiveExprContext aditiveExpr() {
			return getRuleContext(AditiveExprContext.class,0);
		}
		public TerminalNode K_IS() { return getToken(MDBParser.K_IS, 0); }
		public ExprTypenameContext exprTypename() {
			return getRuleContext(ExprTypenameContext.class,0);
		}
		public TerminalNode K_NOT() { return getToken(MDBParser.K_NOT, 0); }
		public ComparisonExprIsContext(ComparisonExprContext ctx) { copyFrom(ctx); }
	}
	public static class ComparisonExprOpContext extends ComparisonExprContext {
		public Token op;
		public List<AditiveExprContext> aditiveExpr() {
			return getRuleContexts(AditiveExprContext.class);
		}
		public AditiveExprContext aditiveExpr(int i) {
			return getRuleContext(AditiveExprContext.class,i);
		}
		public TerminalNode EQ() { return getToken(MDBParser.EQ, 0); }
		public TerminalNode NEQ() { return getToken(MDBParser.NEQ, 0); }
		public TerminalNode LT() { return getToken(MDBParser.LT, 0); }
		public TerminalNode GT() { return getToken(MDBParser.GT, 0); }
		public TerminalNode LEQ() { return getToken(MDBParser.LEQ, 0); }
		public TerminalNode GEQ() { return getToken(MDBParser.GEQ, 0); }
		public ComparisonExprOpContext(ComparisonExprContext ctx) { copyFrom(ctx); }
	}

	public final ComparisonExprContext comparisonExpr() throws RecognitionException {
		ComparisonExprContext _localctx = new ComparisonExprContext(_ctx, getState());
		enterRule(_localctx, 74, RULE_comparisonExpr);
		int _la;
		try {
			setState(468);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,66,_ctx) ) {
			case 1:
				_localctx = new ComparisonExprOpContext(_localctx);
				enterOuterAlt(_localctx, 1);
				{
				setState(456);
				aditiveExpr();
				setState(459);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if ((((_la) & ~0x3f) == 0 && ((1L << _la) & ((1L << LEQ) | (1L << GEQ) | (1L << EQ) | (1L << NEQ) | (1L << LT) | (1L << GT))) != 0)) {
					{
					setState(457);
					((ComparisonExprOpContext)_localctx).op = _input.LT(1);
					_la = _input.LA(1);
					if ( !((((_la) & ~0x3f) == 0 && ((1L << _la) & ((1L << LEQ) | (1L << GEQ) | (1L << EQ) | (1L << NEQ) | (1L << LT) | (1L << GT))) != 0)) ) {
						((ComparisonExprOpContext)_localctx).op = (Token)_errHandler.recoverInline(this);
					}
					else {
						if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
						_errHandler.reportMatch(this);
						consume();
					}
					setState(458);
					aditiveExpr();
					}
				}

				}
				break;
			case 2:
				_localctx = new ComparisonExprIsContext(_localctx);
				enterOuterAlt(_localctx, 2);
				{
				setState(461);
				aditiveExpr();
				setState(462);
				match(K_IS);
				setState(464);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if (_la==K_NOT) {
					{
					setState(463);
					match(K_NOT);
					}
				}

				setState(466);
				exprTypename();
				}
				break;
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class AditiveExprContext extends ParserRuleContext {
		public Token s57;
		public List<Token> op = new ArrayList<Token>();
		public Token s58;
		public Token _tset829;
		public List<MultiplicativeExprContext> multiplicativeExpr() {
			return getRuleContexts(MultiplicativeExprContext.class);
		}
		public MultiplicativeExprContext multiplicativeExpr(int i) {
			return getRuleContext(MultiplicativeExprContext.class,i);
		}
		public List<TerminalNode> PLUS() { return getTokens(MDBParser.PLUS); }
		public TerminalNode PLUS(int i) {
			return getToken(MDBParser.PLUS, i);
		}
		public List<TerminalNode> MINUS() { return getTokens(MDBParser.MINUS); }
		public TerminalNode MINUS(int i) {
			return getToken(MDBParser.MINUS, i);
		}
		public AditiveExprContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_aditiveExpr; }
	}

	public final AditiveExprContext aditiveExpr() throws RecognitionException {
		AditiveExprContext _localctx = new AditiveExprContext(_ctx, getState());
		enterRule(_localctx, 76, RULE_aditiveExpr);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(470);
			multiplicativeExpr();
			setState(475);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==PLUS || _la==MINUS) {
				{
				{
				setState(471);
				((AditiveExprContext)_localctx)._tset829 = _input.LT(1);
				_la = _input.LA(1);
				if ( !(_la==PLUS || _la==MINUS) ) {
					((AditiveExprContext)_localctx)._tset829 = (Token)_errHandler.recoverInline(this);
				}
				else {
					if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
					_errHandler.reportMatch(this);
					consume();
				}
				((AditiveExprContext)_localctx).op.add(((AditiveExprContext)_localctx)._tset829);
				setState(472);
				multiplicativeExpr();
				}
				}
				setState(477);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class MultiplicativeExprContext extends ParserRuleContext {
		public Token s54;
		public List<Token> op = new ArrayList<Token>();
		public Token s51;
		public Token s55;
		public Token _tset848;
		public List<UnaryExprContext> unaryExpr() {
			return getRuleContexts(UnaryExprContext.class);
		}
		public UnaryExprContext unaryExpr(int i) {
			return getRuleContext(UnaryExprContext.class,i);
		}
		public List<TerminalNode> STAR() { return getTokens(MDBParser.STAR); }
		public TerminalNode STAR(int i) {
			return getToken(MDBParser.STAR, i);
		}
		public List<TerminalNode> PATH_SEQUENCE() { return getTokens(MDBParser.PATH_SEQUENCE); }
		public TerminalNode PATH_SEQUENCE(int i) {
			return getToken(MDBParser.PATH_SEQUENCE, i);
		}
		public List<TerminalNode> PERCENT() { return getTokens(MDBParser.PERCENT); }
		public TerminalNode PERCENT(int i) {
			return getToken(MDBParser.PERCENT, i);
		}
		public MultiplicativeExprContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_multiplicativeExpr; }
	}

	public final MultiplicativeExprContext multiplicativeExpr() throws RecognitionException {
		MultiplicativeExprContext _localctx = new MultiplicativeExprContext(_ctx, getState());
		enterRule(_localctx, 78, RULE_multiplicativeExpr);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(478);
			unaryExpr();
			setState(483);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while ((((_la) & ~0x3f) == 0 && ((1L << _la) & ((1L << PATH_SEQUENCE) | (1L << STAR) | (1L << PERCENT))) != 0)) {
				{
				{
				setState(479);
				((MultiplicativeExprContext)_localctx)._tset848 = _input.LT(1);
				_la = _input.LA(1);
				if ( !((((_la) & ~0x3f) == 0 && ((1L << _la) & ((1L << PATH_SEQUENCE) | (1L << STAR) | (1L << PERCENT))) != 0)) ) {
					((MultiplicativeExprContext)_localctx)._tset848 = (Token)_errHandler.recoverInline(this);
				}
				else {
					if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
					_errHandler.reportMatch(this);
					consume();
				}
				((MultiplicativeExprContext)_localctx).op.add(((MultiplicativeExprContext)_localctx)._tset848);
				setState(480);
				unaryExpr();
				}
				}
				setState(485);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class UnaryExprContext extends ParserRuleContext {
		public TerminalNode K_NOT() { return getToken(MDBParser.K_NOT, 0); }
		public UnaryExprContext unaryExpr() {
			return getRuleContext(UnaryExprContext.class,0);
		}
		public TerminalNode PLUS() { return getToken(MDBParser.PLUS, 0); }
		public TerminalNode MINUS() { return getToken(MDBParser.MINUS, 0); }
		public AtomicExprContext atomicExpr() {
			return getRuleContext(AtomicExprContext.class,0);
		}
		public UnaryExprContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_unaryExpr; }
	}

	public final UnaryExprContext unaryExpr() throws RecognitionException {
		UnaryExprContext _localctx = new UnaryExprContext(_ctx, getState());
		enterRule(_localctx, 80, RULE_unaryExpr);
		try {
			setState(493);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case K_NOT:
				enterOuterAlt(_localctx, 1);
				{
				setState(486);
				match(K_NOT);
				setState(487);
				unaryExpr();
				}
				break;
			case PLUS:
				enterOuterAlt(_localctx, 2);
				{
				setState(488);
				match(PLUS);
				setState(489);
				unaryExpr();
				}
				break;
			case MINUS:
				enterOuterAlt(_localctx, 3);
				{
				setState(490);
				match(MINUS);
				setState(491);
				unaryExpr();
				}
				break;
			case K_FALSE:
			case K_TRUE:
			case VARIABLE:
			case STRING:
			case UNSIGNED_INTEGER:
			case UNSIGNED_FLOAT:
			case L_PAR:
				enterOuterAlt(_localctx, 4);
				{
				setState(492);
				atomicExpr();
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class AtomicExprContext extends ParserRuleContext {
		public AtomicExprContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_atomicExpr; }
	 
		public AtomicExprContext() { }
		public void copyFrom(AtomicExprContext ctx) {
			super.copyFrom(ctx);
		}
	}
	public static class ExprValueExprContext extends AtomicExprContext {
		public ValueExprContext valueExpr() {
			return getRuleContext(ValueExprContext.class,0);
		}
		public ExprValueExprContext(AtomicExprContext ctx) { copyFrom(ctx); }
	}
	public static class ExprVarContext extends AtomicExprContext {
		public TerminalNode VARIABLE() { return getToken(MDBParser.VARIABLE, 0); }
		public TerminalNode KEY() { return getToken(MDBParser.KEY, 0); }
		public ExprVarContext(AtomicExprContext ctx) { copyFrom(ctx); }
	}
	public static class ExprParenthesisContext extends AtomicExprContext {
		public TerminalNode L_PAR() { return getToken(MDBParser.L_PAR, 0); }
		public ConditionalOrExprContext conditionalOrExpr() {
			return getRuleContext(ConditionalOrExprContext.class,0);
		}
		public TerminalNode R_PAR() { return getToken(MDBParser.R_PAR, 0); }
		public ExprParenthesisContext(AtomicExprContext ctx) { copyFrom(ctx); }
	}

	public final AtomicExprContext atomicExpr() throws RecognitionException {
		AtomicExprContext _localctx = new AtomicExprContext(_ctx, getState());
		enterRule(_localctx, 82, RULE_atomicExpr);
		int _la;
		try {
			setState(504);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case VARIABLE:
				_localctx = new ExprVarContext(_localctx);
				enterOuterAlt(_localctx, 1);
				{
				setState(495);
				match(VARIABLE);
				setState(497);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if (_la==KEY) {
					{
					setState(496);
					match(KEY);
					}
				}

				}
				break;
			case K_FALSE:
			case K_TRUE:
			case STRING:
			case UNSIGNED_INTEGER:
			case UNSIGNED_FLOAT:
				_localctx = new ExprValueExprContext(_localctx);
				enterOuterAlt(_localctx, 2);
				{
				setState(499);
				valueExpr();
				}
				break;
			case L_PAR:
				_localctx = new ExprParenthesisContext(_localctx);
				enterOuterAlt(_localctx, 3);
				{
				setState(500);
				match(L_PAR);
				setState(501);
				conditionalOrExpr();
				setState(502);
				match(R_PAR);
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class ValueExprContext extends ParserRuleContext {
		public TerminalNode UNSIGNED_INTEGER() { return getToken(MDBParser.UNSIGNED_INTEGER, 0); }
		public TerminalNode UNSIGNED_FLOAT() { return getToken(MDBParser.UNSIGNED_FLOAT, 0); }
		public TerminalNode STRING() { return getToken(MDBParser.STRING, 0); }
		public BoolValueContext boolValue() {
			return getRuleContext(BoolValueContext.class,0);
		}
		public ValueExprContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_valueExpr; }
	}

	public final ValueExprContext valueExpr() throws RecognitionException {
		ValueExprContext _localctx = new ValueExprContext(_ctx, getState());
		enterRule(_localctx, 84, RULE_valueExpr);
		try {
			setState(510);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case UNSIGNED_INTEGER:
				enterOuterAlt(_localctx, 1);
				{
				setState(506);
				match(UNSIGNED_INTEGER);
				}
				break;
			case UNSIGNED_FLOAT:
				enterOuterAlt(_localctx, 2);
				{
				setState(507);
				match(UNSIGNED_FLOAT);
				}
				break;
			case STRING:
				enterOuterAlt(_localctx, 3);
				{
				setState(508);
				match(STRING);
				}
				break;
			case K_FALSE:
			case K_TRUE:
				enterOuterAlt(_localctx, 4);
				{
				setState(509);
				boolValue();
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class ExprTypenameContext extends ParserRuleContext {
		public TerminalNode K_NULL() { return getToken(MDBParser.K_NULL, 0); }
		public TerminalNode K_STRING() { return getToken(MDBParser.K_STRING, 0); }
		public TerminalNode K_BOOL() { return getToken(MDBParser.K_BOOL, 0); }
		public TerminalNode K_INTEGER() { return getToken(MDBParser.K_INTEGER, 0); }
		public TerminalNode K_FLOAT() { return getToken(MDBParser.K_FLOAT, 0); }
		public ExprTypenameContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_exprTypename; }
	}

	public final ExprTypenameContext exprTypename() throws RecognitionException {
		ExprTypenameContext _localctx = new ExprTypenameContext(_ctx, getState());
		enterRule(_localctx, 86, RULE_exprTypename);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(512);
			_la = _input.LA(1);
			if ( !((((_la) & ~0x3f) == 0 && ((1L << _la) & ((1L << K_BOOL) | (1L << K_INTEGER) | (1L << K_FLOAT) | (1L << K_NULL) | (1L << K_STRING))) != 0)) ) {
			_errHandler.recoverInline(this);
			}
			else {
				if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
				_errHandler.reportMatch(this);
				consume();
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class KeywordContext extends ParserRuleContext {
		public TerminalNode K_ALL() { return getToken(MDBParser.K_ALL, 0); }
		public TerminalNode K_AND() { return getToken(MDBParser.K_AND, 0); }
		public TerminalNode K_AVG() { return getToken(MDBParser.K_AVG, 0); }
		public TerminalNode K_ASC() { return getToken(MDBParser.K_ASC, 0); }
		public TerminalNode K_BY() { return getToken(MDBParser.K_BY, 0); }
		public TerminalNode K_BOOL() { return getToken(MDBParser.K_BOOL, 0); }
		public TerminalNode K_COUNT() { return getToken(MDBParser.K_COUNT, 0); }
		public TerminalNode K_DESCRIBE() { return getToken(MDBParser.K_DESCRIBE, 0); }
		public TerminalNode K_DESC() { return getToken(MDBParser.K_DESC, 0); }
		public TerminalNode K_DISTINCT() { return getToken(MDBParser.K_DISTINCT, 0); }
		public TerminalNode K_IS() { return getToken(MDBParser.K_IS, 0); }
		public TerminalNode K_INTEGER() { return getToken(MDBParser.K_INTEGER, 0); }
		public TerminalNode K_FLOAT() { return getToken(MDBParser.K_FLOAT, 0); }
		public TerminalNode K_GROUP() { return getToken(MDBParser.K_GROUP, 0); }
		public TerminalNode K_LIMIT() { return getToken(MDBParser.K_LIMIT, 0); }
		public TerminalNode K_MAX() { return getToken(MDBParser.K_MAX, 0); }
		public TerminalNode K_MATCH() { return getToken(MDBParser.K_MATCH, 0); }
		public TerminalNode K_MIN() { return getToken(MDBParser.K_MIN, 0); }
		public TerminalNode K_OPTIONAL() { return getToken(MDBParser.K_OPTIONAL, 0); }
		public TerminalNode K_ORDER() { return getToken(MDBParser.K_ORDER, 0); }
		public TerminalNode K_OR() { return getToken(MDBParser.K_OR, 0); }
		public TerminalNode K_NOT() { return getToken(MDBParser.K_NOT, 0); }
		public TerminalNode K_NULL() { return getToken(MDBParser.K_NULL, 0); }
		public TerminalNode K_SET() { return getToken(MDBParser.K_SET, 0); }
		public TerminalNode K_SUM() { return getToken(MDBParser.K_SUM, 0); }
		public TerminalNode K_STRING() { return getToken(MDBParser.K_STRING, 0); }
		public TerminalNode K_RETURN() { return getToken(MDBParser.K_RETURN, 0); }
		public TerminalNode K_WHERE() { return getToken(MDBParser.K_WHERE, 0); }
		public KeywordContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_keyword; }
	}

	public final KeywordContext keyword() throws RecognitionException {
		KeywordContext _localctx = new KeywordContext(_ctx, getState());
		enterRule(_localctx, 88, RULE_keyword);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(514);
			_la = _input.LA(1);
			if ( !((((_la) & ~0x3f) == 0 && ((1L << _la) & ((1L << K_AND) | (1L << K_AVG) | (1L << K_ALL) | (1L << K_ASC) | (1L << K_BY) | (1L << K_BOOL) | (1L << K_COUNT) | (1L << K_DESCRIBE) | (1L << K_DESC) | (1L << K_DISTINCT) | (1L << K_IS) | (1L << K_INTEGER) | (1L << K_FLOAT) | (1L << K_GROUP) | (1L << K_LIMIT) | (1L << K_MAX) | (1L << K_MATCH) | (1L << K_MIN) | (1L << K_OPTIONAL) | (1L << K_ORDER) | (1L << K_OR) | (1L << K_NOT) | (1L << K_NULL) | (1L << K_SET) | (1L << K_SUM) | (1L << K_STRING) | (1L << K_RETURN) | (1L << K_WHERE))) != 0)) ) {
			_errHandler.recoverInline(this);
			}
			else {
				if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
				_errHandler.reportMatch(this);
				consume();
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static final String _serializedATN =
		"\3\u608b\ua72a\u8133\ub9ed\u417c\u3be7\u7786\u5964\3G\u0207\4\2\t\2\4"+
		"\3\t\3\4\4\t\4\4\5\t\5\4\6\t\6\4\7\t\7\4\b\t\b\4\t\t\t\4\n\t\n\4\13\t"+
		"\13\4\f\t\f\4\r\t\r\4\16\t\16\4\17\t\17\4\20\t\20\4\21\t\21\4\22\t\22"+
		"\4\23\t\23\4\24\t\24\4\25\t\25\4\26\t\26\4\27\t\27\4\30\t\30\4\31\t\31"+
		"\4\32\t\32\4\33\t\33\4\34\t\34\4\35\t\35\4\36\t\36\4\37\t\37\4 \t \4!"+
		"\t!\4\"\t\"\4#\t#\4$\t$\4%\t%\4&\t&\4\'\t\'\4(\t(\4)\t)\4*\t*\4+\t+\4"+
		",\t,\4-\t-\4.\t.\3\2\5\2^\n\2\3\2\3\2\5\2b\n\2\3\2\5\2e\n\2\3\2\5\2h\n"+
		"\2\3\2\3\2\3\2\3\2\3\2\3\2\5\2p\n\2\3\3\3\3\3\3\3\4\3\4\3\4\3\4\7\4y\n"+
		"\4\f\4\16\4|\13\4\3\5\3\5\3\5\3\6\3\6\3\6\3\7\3\7\3\7\3\7\3\7\7\7\u0089"+
		"\n\7\f\7\16\7\u008c\13\7\3\b\3\b\3\b\3\b\3\b\7\b\u0093\n\b\f\b\16\b\u0096"+
		"\13\b\3\t\3\t\5\t\u009a\n\t\3\t\3\t\3\t\7\t\u009f\n\t\f\t\16\t\u00a2\13"+
		"\t\3\t\3\t\5\t\u00a6\n\t\3\t\3\t\5\t\u00aa\n\t\3\t\3\t\3\t\5\t\u00af\n"+
		"\t\5\t\u00b1\n\t\3\n\3\n\3\n\3\n\3\13\3\13\5\13\u00b9\n\13\3\13\3\13\3"+
		"\13\3\13\5\13\u00bf\n\13\3\13\3\13\3\13\3\13\3\13\5\13\u00c6\n\13\3\13"+
		"\3\13\5\13\u00ca\n\13\3\13\5\13\u00cd\n\13\3\13\5\13\u00d0\n\13\3\f\3"+
		"\f\3\r\3\r\5\r\u00d6\n\r\3\r\5\r\u00d9\n\r\3\r\3\r\3\r\3\r\5\r\u00df\n"+
		"\r\3\r\3\r\5\r\u00e3\n\r\3\r\3\r\3\r\5\r\u00e8\n\r\3\r\3\r\5\r\u00ec\n"+
		"\r\3\r\3\r\5\r\u00f0\n\r\5\r\u00f2\n\r\3\16\3\16\5\16\u00f6\n\16\3\17"+
		"\3\17\7\17\u00fa\n\17\f\17\16\17\u00fd\13\17\3\20\3\20\3\20\3\20\3\20"+
		"\3\21\3\21\3\21\7\21\u0107\n\21\f\21\16\21\u010a\13\21\3\22\3\22\3\22"+
		"\5\22\u010f\n\22\3\22\3\22\7\22\u0113\n\22\f\22\16\22\u0116\13\22\3\23"+
		"\3\23\3\23\5\23\u011b\n\23\3\23\5\23\u011e\n\23\3\23\3\23\3\23\3\23\3"+
		"\23\3\23\3\23\5\23\u0127\n\23\3\23\5\23\u012a\n\23\3\23\3\23\3\23\3\23"+
		"\3\23\5\23\u0131\n\23\3\24\3\24\3\24\7\24\u0136\n\24\f\24\16\24\u0139"+
		"\13\24\3\25\3\25\3\25\7\25\u013e\n\25\f\25\16\25\u0141\13\25\3\26\5\26"+
		"\u0144\n\26\3\26\3\26\5\26\u0148\n\26\3\26\5\26\u014b\n\26\3\26\3\26\3"+
		"\26\3\26\5\26\u0151\n\26\5\26\u0153\n\26\3\27\3\27\3\27\3\27\3\27\3\27"+
		"\3\27\3\27\5\27\u015d\n\27\3\30\3\30\3\31\3\31\5\31\u0163\n\31\3\32\3"+
		"\32\3\32\3\32\3\33\3\33\3\33\3\33\5\33\u016d\n\33\3\34\3\34\3\34\5\34"+
		"\u0172\n\34\3\34\3\34\3\34\3\34\3\34\5\34\u0179\n\34\3\34\5\34\u017c\n"+
		"\34\3\35\3\35\5\35\u0180\n\35\3\35\5\35\u0183\n\35\3\35\5\35\u0186\n\35"+
		"\3\35\3\35\3\36\3\36\5\36\u018c\n\36\3\36\7\36\u018f\n\36\f\36\16\36\u0192"+
		"\13\36\3\36\5\36\u0195\n\36\3\36\3\36\3\37\3\37\3\37\3\37\7\37\u019d\n"+
		"\37\f\37\16\37\u01a0\13\37\3\37\3\37\3 \3 \3 \3 \3 \5 \u01a9\n \3!\3!"+
		"\5!\u01ad\n!\3\"\3\"\3#\5#\u01b2\n#\3#\3#\3$\3$\3$\5$\u01b9\n$\3%\3%\3"+
		"%\7%\u01be\n%\f%\16%\u01c1\13%\3&\3&\3&\7&\u01c6\n&\f&\16&\u01c9\13&\3"+
		"\'\3\'\3\'\5\'\u01ce\n\'\3\'\3\'\3\'\5\'\u01d3\n\'\3\'\3\'\5\'\u01d7\n"+
		"\'\3(\3(\3(\7(\u01dc\n(\f(\16(\u01df\13(\3)\3)\3)\7)\u01e4\n)\f)\16)\u01e7"+
		"\13)\3*\3*\3*\3*\3*\3*\3*\5*\u01f0\n*\3+\3+\5+\u01f4\n+\3+\3+\3+\3+\3"+
		"+\5+\u01fb\n+\3,\3,\3,\3,\5,\u0201\n,\3-\3-\3.\3.\3.\2\2/\2\4\6\b\n\f"+
		"\16\20\22\24\26\30\32\34\36 \"$&(*,.\60\62\64\668:<>@BDFHJLNPRTVXZ\2\16"+
		"\6\2\5\5\24\24\26\26\35\35\4\2\7\7\f\f\4\2\3\3\6\6\4\2%%))\3\2\'(\4\2"+
		"\20\20  \3\2;<\3\2+,\3\2.\63\4\2\65\6589\7\2\t\t\17\17\21\21\33\33\36"+
		"\36\5\2\4\17\21\37!!\2\u022f\2o\3\2\2\2\4q\3\2\2\2\6t\3\2\2\2\b}\3\2\2"+
		"\2\n\u0080\3\2\2\2\f\u0083\3\2\2\2\16\u008d\3\2\2\2\20\u00b0\3\2\2\2\22"+
		"\u00b2\3\2\2\2\24\u00cf\3\2\2\2\26\u00d1\3\2\2\2\30\u00f1\3\2\2\2\32\u00f3"+
		"\3\2\2\2\34\u00f7\3\2\2\2\36\u00fe\3\2\2\2 \u0103\3\2\2\2\"\u010b\3\2"+
		"\2\2$\u0130\3\2\2\2&\u0132\3\2\2\2(\u013a\3\2\2\2*\u0152\3\2\2\2,\u015c"+
		"\3\2\2\2.\u015e\3\2\2\2\60\u0162\3\2\2\2\62\u0164\3\2\2\2\64\u016c\3\2"+
		"\2\2\66\u017b\3\2\2\28\u017d\3\2\2\2:\u0189\3\2\2\2<\u0198\3\2\2\2>\u01a3"+
		"\3\2\2\2@\u01ac\3\2\2\2B\u01ae\3\2\2\2D\u01b1\3\2\2\2F\u01b8\3\2\2\2H"+
		"\u01ba\3\2\2\2J\u01c2\3\2\2\2L\u01d6\3\2\2\2N\u01d8\3\2\2\2P\u01e0\3\2"+
		"\2\2R\u01ef\3\2\2\2T\u01fa\3\2\2\2V\u0200\3\2\2\2X\u0202\3\2\2\2Z\u0204"+
		"\3\2\2\2\\^\5\6\4\2]\\\3\2\2\2]^\3\2\2\2^_\3\2\2\2_a\5\b\5\2`b\5\n\6\2"+
		"a`\3\2\2\2ab\3\2\2\2bd\3\2\2\2ce\5\f\7\2dc\3\2\2\2de\3\2\2\2eg\3\2\2\2"+
		"fh\5\16\b\2gf\3\2\2\2gh\3\2\2\2hi\3\2\2\2ij\5\20\t\2jk\7\2\2\3kp\3\2\2"+
		"\2lm\5\4\3\2mn\7\2\2\3np\3\2\2\2o]\3\2\2\2ol\3\2\2\2p\3\3\2\2\2qr\7\13"+
		"\2\2rs\5\64\33\2s\5\3\2\2\2tu\7\34\2\2uz\5\22\n\2vw\7C\2\2wy\5\22\n\2"+
		"xv\3\2\2\2y|\3\2\2\2zx\3\2\2\2z{\3\2\2\2{\7\3\2\2\2|z\3\2\2\2}~\7\25\2"+
		"\2~\177\5\34\17\2\177\t\3\2\2\2\u0080\u0081\7!\2\2\u0081\u0082\5H%\2\u0082"+
		"\13\3\2\2\2\u0083\u0084\7\22\2\2\u0084\u0085\7\b\2\2\u0085\u008a\5\32"+
		"\16\2\u0086\u0087\7C\2\2\u0087\u0089\5\32\16\2\u0088\u0086\3\2\2\2\u0089"+
		"\u008c\3\2\2\2\u008a\u0088\3\2\2\2\u008a\u008b\3\2\2\2\u008b\r\3\2\2\2"+
		"\u008c\u008a\3\2\2\2\u008d\u008e\7\30\2\2\u008e\u008f\7\b\2\2\u008f\u0094"+
		"\5\30\r\2\u0090\u0091\7C\2\2\u0091\u0093\5\30\r\2\u0092\u0090\3\2\2\2"+
		"\u0093\u0096\3\2\2\2\u0094\u0092\3\2\2\2\u0094\u0095\3\2\2\2\u0095\17"+
		"\3\2\2\2\u0096\u0094\3\2\2\2\u0097\u0099\7\37\2\2\u0098\u009a\7\r\2\2"+
		"\u0099\u0098\3\2\2\2\u0099\u009a\3\2\2\2\u009a\u009b\3\2\2\2\u009b\u00a0"+
		"\5\24\13\2\u009c\u009d\7C\2\2\u009d\u009f\5\24\13\2\u009e\u009c\3\2\2"+
		"\2\u009f\u00a2\3\2\2\2\u00a0\u009e\3\2\2\2\u00a0\u00a1\3\2\2\2\u00a1\u00a5"+
		"\3\2\2\2\u00a2\u00a0\3\2\2\2\u00a3\u00a4\7\23\2\2\u00a4\u00a6\7+\2\2\u00a5"+
		"\u00a3\3\2\2\2\u00a5\u00a6\3\2\2\2\u00a6\u00b1\3\2\2\2\u00a7\u00a9\7\37"+
		"\2\2\u00a8\u00aa\7\r\2\2\u00a9\u00a8\3\2\2\2\u00a9\u00aa\3\2\2\2\u00aa"+
		"\u00ab\3\2\2\2\u00ab\u00ae\78\2\2\u00ac\u00ad\7\23\2\2\u00ad\u00af\7+"+
		"\2\2\u00ae\u00ac\3\2\2\2\u00ae\u00af\3\2\2\2\u00af\u00b1\3\2\2\2\u00b0"+
		"\u0097\3\2\2\2\u00b0\u00a7\3\2\2\2\u00b1\21\3\2\2\2\u00b2\u00b3\7)\2\2"+
		"\u00b3\u00b4\7\64\2\2\u00b4\u00b5\5\64\33\2\u00b5\23\3\2\2\2\u00b6\u00b8"+
		"\7)\2\2\u00b7\u00b9\7&\2\2\u00b8\u00b7\3\2\2\2\u00b8\u00b9\3\2\2\2\u00b9"+
		"\u00d0\3\2\2\2\u00ba\u00bb\5\26\f\2\u00bb\u00bc\7=\2\2\u00bc\u00be\7)"+
		"\2\2\u00bd\u00bf\7&\2\2\u00be\u00bd\3\2\2\2\u00be\u00bf\3\2\2\2\u00bf"+
		"\u00c0\3\2\2\2\u00c0\u00c1\7>\2\2\u00c1\u00d0\3\2\2\2\u00c2\u00c3\7\n"+
		"\2\2\u00c3\u00c5\7=\2\2\u00c4\u00c6\7\r\2\2\u00c5\u00c4\3\2\2\2\u00c5"+
		"\u00c6\3\2\2\2\u00c6\u00cc\3\2\2\2\u00c7\u00c9\7)\2\2\u00c8\u00ca\7&\2"+
		"\2\u00c9\u00c8\3\2\2\2\u00c9\u00ca\3\2\2\2\u00ca\u00cd\3\2\2\2\u00cb\u00cd"+
		"\78\2\2\u00cc\u00c7\3\2\2\2\u00cc\u00cb\3\2\2\2\u00cd\u00ce\3\2\2\2\u00ce"+
		"\u00d0\7>\2\2\u00cf\u00b6\3\2\2\2\u00cf\u00ba\3\2\2\2\u00cf\u00c2\3\2"+
		"\2\2\u00d0\25\3\2\2\2\u00d1\u00d2\t\2\2\2\u00d2\27\3\2\2\2\u00d3\u00d5"+
		"\7)\2\2\u00d4\u00d6\7&\2\2\u00d5\u00d4\3\2\2\2\u00d5\u00d6\3\2\2\2\u00d6"+
		"\u00d8\3\2\2\2\u00d7\u00d9\t\3\2\2\u00d8\u00d7\3\2\2\2\u00d8\u00d9\3\2"+
		"\2\2\u00d9\u00f2\3\2\2\2\u00da\u00db\5\26\f\2\u00db\u00dc\7=\2\2\u00dc"+
		"\u00de\7)\2\2\u00dd\u00df\7&\2\2\u00de\u00dd\3\2\2\2\u00de\u00df\3\2\2"+
		"\2\u00df\u00e0\3\2\2\2\u00e0\u00e2\7>\2\2\u00e1\u00e3\t\3\2\2\u00e2\u00e1"+
		"\3\2\2\2\u00e2\u00e3\3\2\2\2\u00e3\u00f2\3\2\2\2\u00e4\u00e5\7\n\2\2\u00e5"+
		"\u00e7\7=\2\2\u00e6\u00e8\7\r\2\2\u00e7\u00e6\3\2\2\2\u00e7\u00e8\3\2"+
		"\2\2\u00e8\u00e9\3\2\2\2\u00e9\u00eb\7)\2\2\u00ea\u00ec\7&\2\2\u00eb\u00ea"+
		"\3\2\2\2\u00eb\u00ec\3\2\2\2\u00ec\u00ed\3\2\2\2\u00ed\u00ef\7>\2\2\u00ee"+
		"\u00f0\t\3\2\2\u00ef\u00ee\3\2\2\2\u00ef\u00f0\3\2\2\2\u00f0\u00f2\3\2"+
		"\2\2\u00f1\u00d3\3\2\2\2\u00f1\u00da\3\2\2\2\u00f1\u00e4\3\2\2\2\u00f2"+
		"\31\3\2\2\2\u00f3\u00f5\7)\2\2\u00f4\u00f6\7&\2\2\u00f5\u00f4\3\2\2\2"+
		"\u00f5\u00f6\3\2\2\2\u00f6\33\3\2\2\2\u00f7\u00fb\5 \21\2\u00f8\u00fa"+
		"\5\36\20\2\u00f9\u00f8\3\2\2\2\u00fa\u00fd\3\2\2\2\u00fb\u00f9\3\2\2\2"+
		"\u00fb\u00fc\3\2\2\2\u00fc\35\3\2\2\2\u00fd\u00fb\3\2\2\2\u00fe\u00ff"+
		"\7\27\2\2\u00ff\u0100\7?\2\2\u0100\u0101\5\34\17\2\u0101\u0102\7@\2\2"+
		"\u0102\37\3\2\2\2\u0103\u0108\5\"\22\2\u0104\u0105\7C\2\2\u0105\u0107"+
		"\5\"\22\2\u0106\u0104\3\2\2\2\u0107\u010a\3\2\2\2\u0108\u0106\3\2\2\2"+
		"\u0108\u0109\3\2\2\2\u0109!\3\2\2\2\u010a\u0108\3\2\2\2\u010b\u0114\5"+
		"\60\31\2\u010c\u010f\5\66\34\2\u010d\u010f\5$\23\2\u010e\u010c\3\2\2\2"+
		"\u010e\u010d\3\2\2\2\u010f\u0110\3\2\2\2\u0110\u0111\5\60\31\2\u0111\u0113"+
		"\3\2\2\2\u0112\u010e\3\2\2\2\u0113\u0116\3\2\2\2\u0114\u0112\3\2\2\2\u0114"+
		"\u0115\3\2\2\2\u0115#\3\2\2\2\u0116\u0114\3\2\2\2\u0117\u0118\7.\2\2\u0118"+
		"\u011a\7A\2\2\u0119\u011b\5.\30\2\u011a\u0119\3\2\2\2\u011a\u011b\3\2"+
		"\2\2\u011b\u011d\3\2\2\2\u011c\u011e\7)\2\2\u011d\u011c\3\2\2\2\u011d"+
		"\u011e\3\2\2\2\u011e\u011f\3\2\2\2\u011f\u0120\5&\24\2\u0120\u0121\7B"+
		"\2\2\u0121\u0122\7\64\2\2\u0122\u0131\3\2\2\2\u0123\u0124\7\64\2\2\u0124"+
		"\u0126\7A\2\2\u0125\u0127\5.\30\2\u0126\u0125\3\2\2\2\u0126\u0127\3\2"+
		"\2\2\u0127\u0129\3\2\2\2\u0128\u012a\7)\2\2\u0129\u0128\3\2\2\2\u0129"+
		"\u012a\3\2\2\2\u012a\u012b\3\2\2\2\u012b\u012c\5&\24\2\u012c\u012d\7B"+
		"\2\2\u012d\u012e\7\64\2\2\u012e\u012f\7\63\2\2\u012f\u0131\3\2\2\2\u0130"+
		"\u0117\3\2\2\2\u0130\u0123\3\2\2\2\u0131%\3\2\2\2\u0132\u0137\5(\25\2"+
		"\u0133\u0134\7\66\2\2\u0134\u0136\5(\25\2\u0135\u0133\3\2\2\2\u0136\u0139"+
		"\3\2\2\2\u0137\u0135\3\2\2\2\u0137\u0138\3\2\2\2\u0138\'\3\2\2\2\u0139"+
		"\u0137\3\2\2\2\u013a\u013f\5*\26\2\u013b\u013c\7\65\2\2\u013c\u013e\5"+
		"*\26\2\u013d\u013b\3\2\2\2\u013e\u0141\3\2\2\2\u013f\u013d\3\2\2\2\u013f"+
		"\u0140\3\2\2\2\u0140)\3\2\2\2\u0141\u013f\3\2\2\2\u0142\u0144\7\67\2\2"+
		"\u0143\u0142\3\2\2\2\u0143\u0144\3\2\2\2\u0144\u0145\3\2\2\2\u0145\u0147"+
		"\7\'\2\2\u0146\u0148\5,\27\2\u0147\u0146\3\2\2\2\u0147\u0148\3\2\2\2\u0148"+
		"\u0153\3\2\2\2\u0149\u014b\7\67\2\2\u014a\u0149\3\2\2\2\u014a\u014b\3"+
		"\2\2\2\u014b\u014c\3\2\2\2\u014c\u014d\7=\2\2\u014d\u014e\5&\24\2\u014e"+
		"\u0150\7>\2\2\u014f\u0151\5,\27\2\u0150\u014f\3\2\2\2\u0150\u0151\3\2"+
		"\2\2\u0151\u0153\3\2\2\2\u0152\u0143\3\2\2\2\u0152\u014a\3\2\2\2\u0153"+
		"+\3\2\2\2\u0154\u015d\78\2\2\u0155\u015d\7;\2\2\u0156\u015d\7:\2\2\u0157"+
		"\u0158\7?\2\2\u0158\u0159\7+\2\2\u0159\u015a\7C\2\2\u015a\u015b\7+\2\2"+
		"\u015b\u015d\7@\2\2\u015c\u0154\3\2\2\2\u015c\u0155\3\2\2\2\u015c\u0156"+
		"\3\2\2\2\u015c\u0157\3\2\2\2\u015d-\3\2\2\2\u015e\u015f\t\4\2\2\u015f"+
		"/\3\2\2\2\u0160\u0163\5\62\32\2\u0161\u0163\5:\36\2\u0162\u0160\3\2\2"+
		"\2\u0162\u0161\3\2\2\2\u0163\61\3\2\2\2\u0164\u0165\7=\2\2\u0165\u0166"+
		"\5\64\33\2\u0166\u0167\7>\2\2\u0167\63\3\2\2\2\u0168\u016d\5@!\2\u0169"+
		"\u016d\7$\2\2\u016a\u016d\7%\2\2\u016b\u016d\5F$\2\u016c\u0168\3\2\2\2"+
		"\u016c\u0169\3\2\2\2\u016c\u016a\3\2\2\2\u016c\u016b\3\2\2\2\u016d\65"+
		"\3\2\2\2\u016e\u0171\7\62\2\2\u016f\u0170\7<\2\2\u0170\u0172\58\35\2\u0171"+
		"\u016f\3\2\2\2\u0171\u0172\3\2\2\2\u0172\u0173\3\2\2\2\u0173\u017c\7<"+
		"\2\2\u0174\u0178\7<\2\2\u0175\u0176\58\35\2\u0176\u0177\7<\2\2\u0177\u0179"+
		"\3\2\2\2\u0178\u0175\3\2\2\2\u0178\u0179\3\2\2\2\u0179\u017a\3\2\2\2\u017a"+
		"\u017c\7\63\2\2\u017b\u016e\3\2\2\2\u017b\u0174\3\2\2\2\u017c\67\3\2\2"+
		"\2\u017d\u017f\7A\2\2\u017e\u0180\t\5\2\2\u017f\u017e\3\2\2\2\u017f\u0180"+
		"\3\2\2\2\u0180\u0182\3\2\2\2\u0181\u0183\t\6\2\2\u0182\u0181\3\2\2\2\u0182"+
		"\u0183\3\2\2\2\u0183\u0185\3\2\2\2\u0184\u0186\5<\37\2\u0185\u0184\3\2"+
		"\2\2\u0185\u0186\3\2\2\2\u0186\u0187\3\2\2\2\u0187\u0188\7B\2\2\u0188"+
		"9\3\2\2\2\u0189\u018b\7=\2\2\u018a\u018c\7)\2\2\u018b\u018a\3\2\2\2\u018b"+
		"\u018c\3\2\2\2\u018c\u0190\3\2\2\2\u018d\u018f\7\'\2\2\u018e\u018d\3\2"+
		"\2\2\u018f\u0192\3\2\2\2\u0190\u018e\3\2\2\2\u0190\u0191\3\2\2\2\u0191"+
		"\u0194\3\2\2\2\u0192\u0190\3\2\2\2\u0193\u0195\5<\37\2\u0194\u0193\3\2"+
		"\2\2\u0194\u0195\3\2\2\2\u0195\u0196\3\2\2\2\u0196\u0197\7>\2\2\u0197"+
		";\3\2\2\2\u0198\u0199\7?\2\2\u0199\u019e\5> \2\u019a\u019b\7C\2\2\u019b"+
		"\u019d\5> \2\u019c\u019a\3\2\2\2\u019d\u01a0\3\2\2\2\u019e\u019c\3\2\2"+
		"\2\u019e\u019f\3\2\2\2\u019f\u01a1\3\2\2\2\u01a0\u019e\3\2\2\2\u01a1\u01a2"+
		"\7@\2\2\u01a2=\3\2\2\2\u01a3\u01a8\5@!\2\u01a4\u01a5\7D\2\2\u01a5\u01a9"+
		"\5F$\2\u01a6\u01a9\7\"\2\2\u01a7\u01a9\7#\2\2\u01a8\u01a4\3\2\2\2\u01a8"+
		"\u01a6\3\2\2\2\u01a8\u01a7\3\2\2\2\u01a9?\3\2\2\2\u01aa\u01ad\7-\2\2\u01ab"+
		"\u01ad\5Z.\2\u01ac\u01aa\3\2\2\2\u01ac\u01ab\3\2\2\2\u01adA\3\2\2\2\u01ae"+
		"\u01af\t\7\2\2\u01afC\3\2\2\2\u01b0\u01b2\t\b\2\2\u01b1\u01b0\3\2\2\2"+
		"\u01b1\u01b2\3\2\2\2\u01b2\u01b3\3\2\2\2\u01b3\u01b4\t\t\2\2\u01b4E\3"+
		"\2\2\2\u01b5\u01b9\5D#\2\u01b6\u01b9\7*\2\2\u01b7\u01b9\5B\"\2\u01b8\u01b5"+
		"\3\2\2\2\u01b8\u01b6\3\2\2\2\u01b8\u01b7\3\2\2\2\u01b9G\3\2\2\2\u01ba"+
		"\u01bf\5J&\2\u01bb\u01bc\7\31\2\2\u01bc\u01be\5J&\2\u01bd\u01bb\3\2\2"+
		"\2\u01be\u01c1\3\2\2\2\u01bf\u01bd\3\2\2\2\u01bf\u01c0\3\2\2\2\u01c0I"+
		"\3\2\2\2\u01c1\u01bf\3\2\2\2\u01c2\u01c7\5L\'\2\u01c3\u01c4\7\4\2\2\u01c4"+
		"\u01c6\5L\'\2\u01c5\u01c3\3\2\2\2\u01c6\u01c9\3\2\2\2\u01c7\u01c5\3\2"+
		"\2\2\u01c7\u01c8\3\2\2\2\u01c8K\3\2\2\2\u01c9\u01c7\3\2\2\2\u01ca\u01cd"+
		"\5N(\2\u01cb\u01cc\t\n\2\2\u01cc\u01ce\5N(\2\u01cd\u01cb\3\2\2\2\u01cd"+
		"\u01ce\3\2\2\2\u01ce\u01d7\3\2\2\2\u01cf\u01d0\5N(\2\u01d0\u01d2\7\16"+
		"\2\2\u01d1\u01d3\7\32\2\2\u01d2\u01d1\3\2\2\2\u01d2\u01d3\3\2\2\2\u01d3"+
		"\u01d4\3\2\2\2\u01d4\u01d5\5X-\2\u01d5\u01d7\3\2\2\2\u01d6\u01ca\3\2\2"+
		"\2\u01d6\u01cf\3\2\2\2\u01d7M\3\2\2\2\u01d8\u01dd\5P)\2\u01d9\u01da\t"+
		"\b\2\2\u01da\u01dc\5P)\2\u01db\u01d9\3\2\2\2\u01dc\u01df\3\2\2\2\u01dd"+
		"\u01db\3\2\2\2\u01dd\u01de\3\2\2\2\u01deO\3\2\2\2\u01df\u01dd\3\2\2\2"+
		"\u01e0\u01e5\5R*\2\u01e1\u01e2\t\13\2\2\u01e2\u01e4\5R*\2\u01e3\u01e1"+
		"\3\2\2\2\u01e4\u01e7\3\2\2\2\u01e5\u01e3\3\2\2\2\u01e5\u01e6\3\2\2\2\u01e6"+
		"Q\3\2\2\2\u01e7\u01e5\3\2\2\2\u01e8\u01e9\7\32\2\2\u01e9\u01f0\5R*\2\u01ea"+
		"\u01eb\7;\2\2\u01eb\u01f0\5R*\2\u01ec\u01ed\7<\2\2\u01ed\u01f0\5R*\2\u01ee"+
		"\u01f0\5T+\2\u01ef\u01e8\3\2\2\2\u01ef\u01ea\3\2\2\2\u01ef\u01ec\3\2\2"+
		"\2\u01ef\u01ee\3\2\2\2\u01f0S\3\2\2\2\u01f1\u01f3\7)\2\2\u01f2\u01f4\7"+
		"&\2\2\u01f3\u01f2\3\2\2\2\u01f3\u01f4\3\2\2\2\u01f4\u01fb\3\2\2\2\u01f5"+
		"\u01fb\5V,\2\u01f6\u01f7\7=\2\2\u01f7\u01f8\5H%\2\u01f8\u01f9\7>\2\2\u01f9"+
		"\u01fb\3\2\2\2\u01fa\u01f1\3\2\2\2\u01fa\u01f5\3\2\2\2\u01fa\u01f6\3\2"+
		"\2\2\u01fbU\3\2\2\2\u01fc\u0201\7+\2\2\u01fd\u0201\7,\2\2\u01fe\u0201"+
		"\7*\2\2\u01ff\u0201\5B\"\2\u0200\u01fc\3\2\2\2\u0200\u01fd\3\2\2\2\u0200"+
		"\u01fe\3\2\2\2\u0200\u01ff\3\2\2\2\u0201W\3\2\2\2\u0202\u0203\t\f\2\2"+
		"\u0203Y\3\2\2\2\u0204\u0205\t\r\2\2\u0205[\3\2\2\2K]adgoz\u008a\u0094"+
		"\u0099\u00a0\u00a5\u00a9\u00ae\u00b0\u00b8\u00be\u00c5\u00c9\u00cc\u00cf"+
		"\u00d5\u00d8\u00de\u00e2\u00e7\u00eb\u00ef\u00f1\u00f5\u00fb\u0108\u010e"+
		"\u0114\u011a\u011d\u0126\u0129\u0130\u0137\u013f\u0143\u0147\u014a\u0150"+
		"\u0152\u015c\u0162\u016c\u0171\u0178\u017b\u017f\u0182\u0185\u018b\u0190"+
		"\u0194\u019e\u01a8\u01ac\u01b1\u01b8\u01bf\u01c7\u01cd\u01d2\u01d6\u01dd"+
		"\u01e5\u01ef\u01f3\u01fa\u0200";
	public static final ATN _ATN =
		new ATNDeserializer().deserialize(_serializedATN.toCharArray());
	static {
		_decisionToDFA = new DFA[_ATN.getNumberOfDecisions()];
		for (int i = 0; i < _ATN.getNumberOfDecisions(); i++) {
			_decisionToDFA[i] = new DFA(_ATN.getDecisionState(i), i);
		}
	}
}
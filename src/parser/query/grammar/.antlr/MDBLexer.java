// Generated from /home/crojsd/MillenniumDB-Dev/src/parser/query/grammar/MDBLexer.g4 by ANTLR 4.9.2
import org.antlr.v4.runtime.Lexer;
import org.antlr.v4.runtime.CharStream;
import org.antlr.v4.runtime.Token;
import org.antlr.v4.runtime.TokenStream;
import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.atn.*;
import org.antlr.v4.runtime.dfa.DFA;
import org.antlr.v4.runtime.misc.*;

@SuppressWarnings({"all", "warnings", "unchecked", "unused", "cast"})
public class MDBLexer extends Lexer {
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
		WS_CHANNEL=2;
	public static String[] channelNames = {
		"DEFAULT_TOKEN_CHANNEL", "HIDDEN", "WS_CHANNEL"
	};

	public static String[] modeNames = {
		"DEFAULT_MODE"
	};

	private static String[] makeRuleNames() {
		return new String[] {
			"K_ANY", "K_AND", "K_AVG", "K_ALL", "K_ASC", "K_BY", "K_BOOL", "K_COUNT", 
			"K_DESCRIBE", "K_DESC", "K_DISTINCT", "K_IS", "K_INTEGER", "K_FALSE", 
			"K_FLOAT", "K_GROUP", "K_LIMIT", "K_MAX", "K_MATCH", "K_MIN", "K_OPTIONAL", 
			"K_ORDER", "K_OR", "K_NOT", "K_NULL", "K_SET", "K_SUM", "K_STRING", "K_RETURN", 
			"K_TRUE", "K_WHERE", "TRUE_PROP", "FALSE_PROP", "ANON_ID", "EDGE_ID", 
			"KEY", "TYPE", "TYPE_VAR", "VARIABLE", "STRING", "UNSIGNED_INTEGER", 
			"UNSIGNED_FLOAT", "NAME", "LEQ", "GEQ", "EQ", "NEQ", "LT", "GT", "SINGLE_EQ", 
			"PATH_SEQUENCE", "PATH_ALTERNATIVE", "PATH_NEGATION", "STAR", "PERCENT", 
			"QUESTION_MARK", "PLUS", "MINUS", "L_PAR", "R_PAR", "LCURLY_BRACKET", 
			"RCURLY_BRACKET", "LSQUARE_BRACKET", "RSQUARE_BRACKET", "COMMA", "COLON", 
			"WHITE_SPACE", "SINGLE_LINE_COMMENT", "UNRECOGNIZED", "DIGIT", "A", "B", 
			"C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", 
			"Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"
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


	public MDBLexer(CharStream input) {
		super(input);
		_interp = new LexerATNSimulator(this,_ATN,_decisionToDFA,_sharedContextCache);
	}

	@Override
	public String getGrammarFileName() { return "MDBLexer.g4"; }

	@Override
	public String[] getRuleNames() { return ruleNames; }

	@Override
	public String getSerializedATN() { return _serializedATN; }

	@Override
	public String[] getChannelNames() { return channelNames; }

	@Override
	public String[] getModeNames() { return modeNames; }

	@Override
	public ATN getATN() { return _ATN; }

	public static final String _serializedATN =
		"\3\u608b\ua72a\u8133\ub9ed\u417c\u3be7\u7786\u5964\2G\u0250\b\1\4\2\t"+
		"\2\4\3\t\3\4\4\t\4\4\5\t\5\4\6\t\6\4\7\t\7\4\b\t\b\4\t\t\t\4\n\t\n\4\13"+
		"\t\13\4\f\t\f\4\r\t\r\4\16\t\16\4\17\t\17\4\20\t\20\4\21\t\21\4\22\t\22"+
		"\4\23\t\23\4\24\t\24\4\25\t\25\4\26\t\26\4\27\t\27\4\30\t\30\4\31\t\31"+
		"\4\32\t\32\4\33\t\33\4\34\t\34\4\35\t\35\4\36\t\36\4\37\t\37\4 \t \4!"+
		"\t!\4\"\t\"\4#\t#\4$\t$\4%\t%\4&\t&\4\'\t\'\4(\t(\4)\t)\4*\t*\4+\t+\4"+
		",\t,\4-\t-\4.\t.\4/\t/\4\60\t\60\4\61\t\61\4\62\t\62\4\63\t\63\4\64\t"+
		"\64\4\65\t\65\4\66\t\66\4\67\t\67\48\t8\49\t9\4:\t:\4;\t;\4<\t<\4=\t="+
		"\4>\t>\4?\t?\4@\t@\4A\tA\4B\tB\4C\tC\4D\tD\4E\tE\4F\tF\4G\tG\4H\tH\4I"+
		"\tI\4J\tJ\4K\tK\4L\tL\4M\tM\4N\tN\4O\tO\4P\tP\4Q\tQ\4R\tR\4S\tS\4T\tT"+
		"\4U\tU\4V\tV\4W\tW\4X\tX\4Y\tY\4Z\tZ\4[\t[\4\\\t\\\4]\t]\4^\t^\4_\t_\4"+
		"`\t`\4a\ta\3\2\3\2\3\2\3\2\3\3\3\3\3\3\3\3\3\4\3\4\3\4\3\4\3\5\3\5\3\5"+
		"\3\5\3\6\3\6\3\6\3\6\3\7\3\7\3\7\3\b\3\b\3\b\3\b\3\b\3\t\3\t\3\t\3\t\3"+
		"\t\3\t\3\n\3\n\3\n\3\n\3\n\3\n\3\n\3\n\3\n\3\13\3\13\3\13\3\13\3\13\3"+
		"\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\r\3\r\3\r\3\16\3\16\3\16\3\16\3\16"+
		"\3\16\3\16\3\16\3\17\3\17\3\17\3\17\3\17\3\17\3\20\3\20\3\20\3\20\3\20"+
		"\3\20\3\21\3\21\3\21\3\21\3\21\3\21\3\22\3\22\3\22\3\22\3\22\3\22\3\23"+
		"\3\23\3\23\3\23\3\24\3\24\3\24\3\24\3\24\3\24\3\25\3\25\3\25\3\25\3\26"+
		"\3\26\3\26\3\26\3\26\3\26\3\26\3\26\3\26\3\27\3\27\3\27\3\27\3\27\3\27"+
		"\3\30\3\30\3\30\3\31\3\31\3\31\3\31\3\32\3\32\3\32\3\32\3\32\3\33\3\33"+
		"\3\33\3\33\3\34\3\34\3\34\3\34\3\35\3\35\3\35\3\35\3\35\3\35\3\35\3\36"+
		"\3\36\3\36\3\36\3\36\3\36\3\36\3\37\3\37\3\37\3\37\3\37\3 \3 \3 \3 \3"+
		" \3 \3!\3!\3!\3!\3!\3!\3\"\3\"\3\"\3\"\3\"\3\"\3\"\3#\3#\3#\3#\3#\7#\u017c"+
		"\n#\f#\16#\u017f\13#\3$\3$\3$\3$\3$\7$\u0186\n$\f$\16$\u0189\13$\3%\3"+
		"%\3%\7%\u018e\n%\f%\16%\u0191\13%\3&\3&\3&\7&\u0196\n&\f&\16&\u0199\13"+
		"&\3\'\3\'\3\'\3\'\7\'\u019f\n\'\f\'\16\'\u01a2\13\'\3(\3(\3(\7(\u01a7"+
		"\n(\f(\16(\u01aa\13(\3)\3)\3)\3)\7)\u01b0\n)\f)\16)\u01b3\13)\3)\3)\3"+
		"*\6*\u01b8\n*\r*\16*\u01b9\3+\6+\u01bd\n+\r+\16+\u01be\3+\3+\6+\u01c3"+
		"\n+\r+\16+\u01c4\3,\3,\7,\u01c9\n,\f,\16,\u01cc\13,\3-\3-\3-\3.\3.\3."+
		"\3/\3/\3/\3\60\3\60\3\60\3\61\3\61\3\62\3\62\3\63\3\63\3\64\3\64\3\65"+
		"\3\65\3\66\3\66\3\67\3\67\38\38\39\39\3:\3:\3;\3;\3<\3<\3=\3=\3>\3>\3"+
		"?\3?\3@\3@\3A\3A\3B\3B\3C\3C\3D\6D\u0201\nD\rD\16D\u0202\3D\3D\3E\3E\3"+
		"E\3E\7E\u020b\nE\fE\16E\u020e\13E\3E\5E\u0211\nE\3E\3E\5E\u0215\nE\3E"+
		"\3E\3F\3F\3G\3G\3H\3H\3I\3I\3J\3J\3K\3K\3L\3L\3M\3M\3N\3N\3O\3O\3P\3P"+
		"\3Q\3Q\3R\3R\3S\3S\3T\3T\3U\3U\3V\3V\3W\3W\3X\3X\3Y\3Y\3Z\3Z\3[\3[\3\\"+
		"\3\\\3]\3]\3^\3^\3_\3_\3`\3`\3a\3a\2\2b\3\3\5\4\7\5\t\6\13\7\r\b\17\t"+
		"\21\n\23\13\25\f\27\r\31\16\33\17\35\20\37\21!\22#\23%\24\'\25)\26+\27"+
		"-\30/\31\61\32\63\33\65\34\67\359\36;\37= ?!A\"C#E$G%I&K\'M(O)Q*S+U,W"+
		"-Y.[/]\60_\61a\62c\63e\64g\65i\66k\67m8o9q:s;u<w=y>{?}@\177A\u0081B\u0083"+
		"C\u0085D\u0087E\u0089F\u008bG\u008d\2\u008f\2\u0091\2\u0093\2\u0095\2"+
		"\u0097\2\u0099\2\u009b\2\u009d\2\u009f\2\u00a1\2\u00a3\2\u00a5\2\u00a7"+
		"\2\u00a9\2\u00ab\2\u00ad\2\u00af\2\u00b1\2\u00b3\2\u00b5\2\u00b7\2\u00b9"+
		"\2\u00bb\2\u00bd\2\u00bf\2\u00c1\2\3\2#\3\2\63;\3\2\62;\4\2C\\c|\6\2\62"+
		";C\\aac|\3\2$$\5\2\13\f\17\17\"\"\4\2\f\f\17\17\4\2CCcc\4\2DDdd\4\2EE"+
		"ee\4\2FFff\4\2GGgg\4\2HHhh\4\2IIii\4\2JJjj\4\2KKkk\4\2LLll\4\2MMmm\4\2"+
		"NNnn\4\2OOoo\4\2PPpp\4\2QQqq\4\2RRrr\4\2SSss\4\2TTtt\4\2UUuu\4\2VVvv\4"+
		"\2WWww\4\2XXxx\4\2YYyy\4\2ZZzz\4\2[[{{\4\2\\\\||\2\u0244\2\3\3\2\2\2\2"+
		"\5\3\2\2\2\2\7\3\2\2\2\2\t\3\2\2\2\2\13\3\2\2\2\2\r\3\2\2\2\2\17\3\2\2"+
		"\2\2\21\3\2\2\2\2\23\3\2\2\2\2\25\3\2\2\2\2\27\3\2\2\2\2\31\3\2\2\2\2"+
		"\33\3\2\2\2\2\35\3\2\2\2\2\37\3\2\2\2\2!\3\2\2\2\2#\3\2\2\2\2%\3\2\2\2"+
		"\2\'\3\2\2\2\2)\3\2\2\2\2+\3\2\2\2\2-\3\2\2\2\2/\3\2\2\2\2\61\3\2\2\2"+
		"\2\63\3\2\2\2\2\65\3\2\2\2\2\67\3\2\2\2\29\3\2\2\2\2;\3\2\2\2\2=\3\2\2"+
		"\2\2?\3\2\2\2\2A\3\2\2\2\2C\3\2\2\2\2E\3\2\2\2\2G\3\2\2\2\2I\3\2\2\2\2"+
		"K\3\2\2\2\2M\3\2\2\2\2O\3\2\2\2\2Q\3\2\2\2\2S\3\2\2\2\2U\3\2\2\2\2W\3"+
		"\2\2\2\2Y\3\2\2\2\2[\3\2\2\2\2]\3\2\2\2\2_\3\2\2\2\2a\3\2\2\2\2c\3\2\2"+
		"\2\2e\3\2\2\2\2g\3\2\2\2\2i\3\2\2\2\2k\3\2\2\2\2m\3\2\2\2\2o\3\2\2\2\2"+
		"q\3\2\2\2\2s\3\2\2\2\2u\3\2\2\2\2w\3\2\2\2\2y\3\2\2\2\2{\3\2\2\2\2}\3"+
		"\2\2\2\2\177\3\2\2\2\2\u0081\3\2\2\2\2\u0083\3\2\2\2\2\u0085\3\2\2\2\2"+
		"\u0087\3\2\2\2\2\u0089\3\2\2\2\2\u008b\3\2\2\2\3\u00c3\3\2\2\2\5\u00c7"+
		"\3\2\2\2\7\u00cb\3\2\2\2\t\u00cf\3\2\2\2\13\u00d3\3\2\2\2\r\u00d7\3\2"+
		"\2\2\17\u00da\3\2\2\2\21\u00df\3\2\2\2\23\u00e5\3\2\2\2\25\u00ee\3\2\2"+
		"\2\27\u00f3\3\2\2\2\31\u00fc\3\2\2\2\33\u00ff\3\2\2\2\35\u0107\3\2\2\2"+
		"\37\u010d\3\2\2\2!\u0113\3\2\2\2#\u0119\3\2\2\2%\u011f\3\2\2\2\'\u0123"+
		"\3\2\2\2)\u0129\3\2\2\2+\u012d\3\2\2\2-\u0136\3\2\2\2/\u013c\3\2\2\2\61"+
		"\u013f\3\2\2\2\63\u0143\3\2\2\2\65\u0148\3\2\2\2\67\u014c\3\2\2\29\u0150"+
		"\3\2\2\2;\u0157\3\2\2\2=\u015e\3\2\2\2?\u0163\3\2\2\2A\u0169\3\2\2\2C"+
		"\u016f\3\2\2\2E\u0176\3\2\2\2G\u0180\3\2\2\2I\u018a\3\2\2\2K\u0192\3\2"+
		"\2\2M\u019a\3\2\2\2O\u01a3\3\2\2\2Q\u01ab\3\2\2\2S\u01b7\3\2\2\2U\u01bc"+
		"\3\2\2\2W\u01c6\3\2\2\2Y\u01cd\3\2\2\2[\u01d0\3\2\2\2]\u01d3\3\2\2\2_"+
		"\u01d6\3\2\2\2a\u01d9\3\2\2\2c\u01db\3\2\2\2e\u01dd\3\2\2\2g\u01df\3\2"+
		"\2\2i\u01e1\3\2\2\2k\u01e3\3\2\2\2m\u01e5\3\2\2\2o\u01e7\3\2\2\2q\u01e9"+
		"\3\2\2\2s\u01eb\3\2\2\2u\u01ed\3\2\2\2w\u01ef\3\2\2\2y\u01f1\3\2\2\2{"+
		"\u01f3\3\2\2\2}\u01f5\3\2\2\2\177\u01f7\3\2\2\2\u0081\u01f9\3\2\2\2\u0083"+
		"\u01fb\3\2\2\2\u0085\u01fd\3\2\2\2\u0087\u0200\3\2\2\2\u0089\u0206\3\2"+
		"\2\2\u008b\u0218\3\2\2\2\u008d\u021a\3\2\2\2\u008f\u021c\3\2\2\2\u0091"+
		"\u021e\3\2\2\2\u0093\u0220\3\2\2\2\u0095\u0222\3\2\2\2\u0097\u0224\3\2"+
		"\2\2\u0099\u0226\3\2\2\2\u009b\u0228\3\2\2\2\u009d\u022a\3\2\2\2\u009f"+
		"\u022c\3\2\2\2\u00a1\u022e\3\2\2\2\u00a3\u0230\3\2\2\2\u00a5\u0232\3\2"+
		"\2\2\u00a7\u0234\3\2\2\2\u00a9\u0236\3\2\2\2\u00ab\u0238\3\2\2\2\u00ad"+
		"\u023a\3\2\2\2\u00af\u023c\3\2\2\2\u00b1\u023e\3\2\2\2\u00b3\u0240\3\2"+
		"\2\2\u00b5\u0242\3\2\2\2\u00b7\u0244\3\2\2\2\u00b9\u0246\3\2\2\2\u00bb"+
		"\u0248\3\2\2\2\u00bd\u024a\3\2\2\2\u00bf\u024c\3\2\2\2\u00c1\u024e\3\2"+
		"\2\2\u00c3\u00c4\5\u008fH\2\u00c4\u00c5\5\u00a9U\2\u00c5\u00c6\5\u00bf"+
		"`\2\u00c6\4\3\2\2\2\u00c7\u00c8\5\u008fH\2\u00c8\u00c9\5\u00a9U\2\u00c9"+
		"\u00ca\5\u0095K\2\u00ca\6\3\2\2\2\u00cb\u00cc\5\u008fH\2\u00cc\u00cd\5"+
		"\u00b9]\2\u00cd\u00ce\5\u009bN\2\u00ce\b\3\2\2\2\u00cf\u00d0\5\u008fH"+
		"\2\u00d0\u00d1\5\u00a5S\2\u00d1\u00d2\5\u00a5S\2\u00d2\n\3\2\2\2\u00d3"+
		"\u00d4\5\u008fH\2\u00d4\u00d5\5\u00b3Z\2\u00d5\u00d6\5\u0093J\2\u00d6"+
		"\f\3\2\2\2\u00d7\u00d8\5\u0091I\2\u00d8\u00d9\5\u00bf`\2\u00d9\16\3\2"+
		"\2\2\u00da\u00db\5\u0091I\2\u00db\u00dc\5\u00abV\2\u00dc\u00dd\5\u00ab"+
		"V\2\u00dd\u00de\5\u00a5S\2\u00de\20\3\2\2\2\u00df\u00e0\5\u0093J\2\u00e0"+
		"\u00e1\5\u00abV\2\u00e1\u00e2\5\u00b7\\\2\u00e2\u00e3\5\u00a9U\2\u00e3"+
		"\u00e4\5\u00b5[\2\u00e4\22\3\2\2\2\u00e5\u00e6\5\u0095K\2\u00e6\u00e7"+
		"\5\u0097L\2\u00e7\u00e8\5\u00b3Z\2\u00e8\u00e9\5\u0093J\2\u00e9\u00ea"+
		"\5\u00b1Y\2\u00ea\u00eb\5\u009fP\2\u00eb\u00ec\5\u0091I\2\u00ec\u00ed"+
		"\5\u0097L\2\u00ed\24\3\2\2\2\u00ee\u00ef\5\u0095K\2\u00ef\u00f0\5\u0097"+
		"L\2\u00f0\u00f1\5\u00b3Z\2\u00f1\u00f2\5\u0093J\2\u00f2\26\3\2\2\2\u00f3"+
		"\u00f4\5\u0095K\2\u00f4\u00f5\5\u009fP\2\u00f5\u00f6\5\u00b3Z\2\u00f6"+
		"\u00f7\5\u00b5[\2\u00f7\u00f8\5\u009fP\2\u00f8\u00f9\5\u00a9U\2\u00f9"+
		"\u00fa\5\u0093J\2\u00fa\u00fb\5\u00b5[\2\u00fb\30\3\2\2\2\u00fc\u00fd"+
		"\5\u009fP\2\u00fd\u00fe\5\u00b3Z\2\u00fe\32\3\2\2\2\u00ff\u0100\5\u009f"+
		"P\2\u0100\u0101\5\u00a9U\2\u0101\u0102\5\u00b5[\2\u0102\u0103\5\u0097"+
		"L\2\u0103\u0104\5\u009bN\2\u0104\u0105\5\u0097L\2\u0105\u0106\5\u00b1"+
		"Y\2\u0106\34\3\2\2\2\u0107\u0108\7h\2\2\u0108\u0109\7c\2\2\u0109\u010a"+
		"\7n\2\2\u010a\u010b\7u\2\2\u010b\u010c\7g\2\2\u010c\36\3\2\2\2\u010d\u010e"+
		"\5\u0099M\2\u010e\u010f\5\u00a5S\2\u010f\u0110\5\u00abV\2\u0110\u0111"+
		"\5\u008fH\2\u0111\u0112\5\u00b5[\2\u0112 \3\2\2\2\u0113\u0114\5\u009b"+
		"N\2\u0114\u0115\5\u00b1Y\2\u0115\u0116\5\u00abV\2\u0116\u0117\5\u00b7"+
		"\\\2\u0117\u0118\5\u00adW\2\u0118\"\3\2\2\2\u0119\u011a\5\u00a5S\2\u011a"+
		"\u011b\5\u009fP\2\u011b\u011c\5\u00a7T\2\u011c\u011d\5\u009fP\2\u011d"+
		"\u011e\5\u00b5[\2\u011e$\3\2\2\2\u011f\u0120\5\u00a7T\2\u0120\u0121\5"+
		"\u008fH\2\u0121\u0122\5\u00bd_\2\u0122&\3\2\2\2\u0123\u0124\5\u00a7T\2"+
		"\u0124\u0125\5\u008fH\2\u0125\u0126\5\u00b5[\2\u0126\u0127\5\u0093J\2"+
		"\u0127\u0128\5\u009dO\2\u0128(\3\2\2\2\u0129\u012a\5\u00a7T\2\u012a\u012b"+
		"\5\u009fP\2\u012b\u012c\5\u00a9U\2\u012c*\3\2\2\2\u012d\u012e\5\u00ab"+
		"V\2\u012e\u012f\5\u00adW\2\u012f\u0130\5\u00b5[\2\u0130\u0131\5\u009f"+
		"P\2\u0131\u0132\5\u00abV\2\u0132\u0133\5\u00a9U\2\u0133\u0134\5\u008f"+
		"H\2\u0134\u0135\5\u00a5S\2\u0135,\3\2\2\2\u0136\u0137\5\u00abV\2\u0137"+
		"\u0138\5\u00b1Y\2\u0138\u0139\5\u0095K\2\u0139\u013a\5\u0097L\2\u013a"+
		"\u013b\5\u00b1Y\2\u013b.\3\2\2\2\u013c\u013d\5\u00abV\2\u013d\u013e\5"+
		"\u00b1Y\2\u013e\60\3\2\2\2\u013f\u0140\5\u00a9U\2\u0140\u0141\5\u00ab"+
		"V\2\u0141\u0142\5\u00b5[\2\u0142\62\3\2\2\2\u0143\u0144\5\u00a9U\2\u0144"+
		"\u0145\5\u00b7\\\2\u0145\u0146\5\u00a5S\2\u0146\u0147\5\u00a5S\2\u0147"+
		"\64\3\2\2\2\u0148\u0149\5\u00b3Z\2\u0149\u014a\5\u0097L\2\u014a\u014b"+
		"\5\u00b5[\2\u014b\66\3\2\2\2\u014c\u014d\5\u00b3Z\2\u014d\u014e\5\u00b7"+
		"\\\2\u014e\u014f\5\u00a7T\2\u014f8\3\2\2\2\u0150\u0151\5\u00b3Z\2\u0151"+
		"\u0152\5\u00b5[\2\u0152\u0153\5\u00b1Y\2\u0153\u0154\5\u009fP\2\u0154"+
		"\u0155\5\u00a9U\2\u0155\u0156\5\u009bN\2\u0156:\3\2\2\2\u0157\u0158\5"+
		"\u00b1Y\2\u0158\u0159\5\u0097L\2\u0159\u015a\5\u00b5[\2\u015a\u015b\5"+
		"\u00b7\\\2\u015b\u015c\5\u00b1Y\2\u015c\u015d\5\u00a9U\2\u015d<\3\2\2"+
		"\2\u015e\u015f\7v\2\2\u015f\u0160\7t\2\2\u0160\u0161\7w\2\2\u0161\u0162"+
		"\7g\2\2\u0162>\3\2\2\2\u0163\u0164\5\u00bb^\2\u0164\u0165\5\u009dO\2\u0165"+
		"\u0166\5\u0097L\2\u0166\u0167\5\u00b1Y\2\u0167\u0168\5\u0097L\2\u0168"+
		"@\3\2\2\2\u0169\u016a\7<\2\2\u016a\u016b\7v\2\2\u016b\u016c\7t\2\2\u016c"+
		"\u016d\7w\2\2\u016d\u016e\7g\2\2\u016eB\3\2\2\2\u016f\u0170\7<\2\2\u0170"+
		"\u0171\7h\2\2\u0171\u0172\7c\2\2\u0172\u0173\7n\2\2\u0173\u0174\7u\2\2"+
		"\u0174\u0175\7g\2\2\u0175D\3\2\2\2\u0176\u0177\7a\2\2\u0177\u0178\7c\2"+
		"\2\u0178\u0179\3\2\2\2\u0179\u017d\t\2\2\2\u017a\u017c\t\3\2\2\u017b\u017a"+
		"\3\2\2\2\u017c\u017f\3\2\2\2\u017d\u017b\3\2\2\2\u017d\u017e\3\2\2\2\u017e"+
		"F\3\2\2\2\u017f\u017d\3\2\2\2\u0180\u0181\7a\2\2\u0181\u0182\7g\2\2\u0182"+
		"\u0183\3\2\2\2\u0183\u0187\t\2\2\2\u0184\u0186\t\3\2\2\u0185\u0184\3\2"+
		"\2\2\u0186\u0189\3\2\2\2\u0187\u0185\3\2\2\2\u0187\u0188\3\2\2\2\u0188"+
		"H\3\2\2\2\u0189\u0187\3\2\2\2\u018a\u018b\7\60\2\2\u018b\u018f\t\4\2\2"+
		"\u018c\u018e\t\5\2\2\u018d\u018c\3\2\2\2\u018e\u0191\3\2\2\2\u018f\u018d"+
		"\3\2\2\2\u018f\u0190\3\2\2\2\u0190J\3\2\2\2\u0191\u018f\3\2\2\2\u0192"+
		"\u0193\7<\2\2\u0193\u0197\t\4\2\2\u0194\u0196\t\5\2\2\u0195\u0194\3\2"+
		"\2\2\u0196\u0199\3\2\2\2\u0197\u0195\3\2\2\2\u0197\u0198\3\2\2\2\u0198"+
		"L\3\2\2\2\u0199\u0197\3\2\2\2\u019a\u019b\7<\2\2\u019b\u019c\7A\2\2\u019c"+
		"\u01a0\t\4\2\2\u019d\u019f\t\5\2\2\u019e\u019d\3\2\2\2\u019f\u01a2\3\2"+
		"\2\2\u01a0\u019e\3\2\2\2\u01a0\u01a1\3\2\2\2\u01a1N\3\2\2\2\u01a2\u01a0"+
		"\3\2\2\2\u01a3\u01a4\7A\2\2\u01a4\u01a8\t\4\2\2\u01a5\u01a7\t\5\2\2\u01a6"+
		"\u01a5\3\2\2\2\u01a7\u01aa\3\2\2\2\u01a8\u01a6\3\2\2\2\u01a8\u01a9\3\2"+
		"\2\2\u01a9P\3\2\2\2\u01aa\u01a8\3\2\2\2\u01ab\u01b1\7$\2\2\u01ac\u01b0"+
		"\n\6\2\2\u01ad\u01ae\7^\2\2\u01ae\u01b0\7$\2\2\u01af\u01ac\3\2\2\2\u01af"+
		"\u01ad\3\2\2\2\u01b0\u01b3\3\2\2\2\u01b1\u01af\3\2\2\2\u01b1\u01b2\3\2"+
		"\2\2\u01b2\u01b4\3\2\2\2\u01b3\u01b1\3\2\2\2\u01b4\u01b5\7$\2\2\u01b5"+
		"R\3\2\2\2\u01b6\u01b8\5\u008dG\2\u01b7\u01b6\3\2\2\2\u01b8\u01b9\3\2\2"+
		"\2\u01b9\u01b7\3\2\2\2\u01b9\u01ba\3\2\2\2\u01baT\3\2\2\2\u01bb\u01bd"+
		"\5\u008dG\2\u01bc\u01bb\3\2\2\2\u01bd\u01be\3\2\2\2\u01be\u01bc\3\2\2"+
		"\2\u01be\u01bf\3\2\2\2\u01bf\u01c0\3\2\2\2\u01c0\u01c2\7\60\2\2\u01c1"+
		"\u01c3\5\u008dG\2\u01c2\u01c1\3\2\2\2\u01c3\u01c4\3\2\2\2\u01c4\u01c2"+
		"\3\2\2\2\u01c4\u01c5\3\2\2\2\u01c5V\3\2\2\2\u01c6\u01ca\t\4\2\2\u01c7"+
		"\u01c9\t\5\2\2\u01c8\u01c7\3\2\2\2\u01c9\u01cc\3\2\2\2\u01ca\u01c8\3\2"+
		"\2\2\u01ca\u01cb\3\2\2\2\u01cbX\3\2\2\2\u01cc\u01ca\3\2\2\2\u01cd\u01ce"+
		"\7>\2\2\u01ce\u01cf\7?\2\2\u01cfZ\3\2\2\2\u01d0\u01d1\7@\2\2\u01d1\u01d2"+
		"\7?\2\2\u01d2\\\3\2\2\2\u01d3\u01d4\7?\2\2\u01d4\u01d5\7?\2\2\u01d5^\3"+
		"\2\2\2\u01d6\u01d7\7#\2\2\u01d7\u01d8\7?\2\2\u01d8`\3\2\2\2\u01d9\u01da"+
		"\7>\2\2\u01dab\3\2\2\2\u01db\u01dc\7@\2\2\u01dcd\3\2\2\2\u01dd\u01de\7"+
		"?\2\2\u01def\3\2\2\2\u01df\u01e0\7\61\2\2\u01e0h\3\2\2\2\u01e1\u01e2\7"+
		"~\2\2\u01e2j\3\2\2\2\u01e3\u01e4\7`\2\2\u01e4l\3\2\2\2\u01e5\u01e6\7,"+
		"\2\2\u01e6n\3\2\2\2\u01e7\u01e8\7\'\2\2\u01e8p\3\2\2\2\u01e9\u01ea\7A"+
		"\2\2\u01ear\3\2\2\2\u01eb\u01ec\7-\2\2\u01ect\3\2\2\2\u01ed\u01ee\7/\2"+
		"\2\u01eev\3\2\2\2\u01ef\u01f0\7*\2\2\u01f0x\3\2\2\2\u01f1\u01f2\7+\2\2"+
		"\u01f2z\3\2\2\2\u01f3\u01f4\7}\2\2\u01f4|\3\2\2\2\u01f5\u01f6\7\177\2"+
		"\2\u01f6~\3\2\2\2\u01f7\u01f8\7]\2\2\u01f8\u0080\3\2\2\2\u01f9\u01fa\7"+
		"_\2\2\u01fa\u0082\3\2\2\2\u01fb\u01fc\7.\2\2\u01fc\u0084\3\2\2\2\u01fd"+
		"\u01fe\7<\2\2\u01fe\u0086\3\2\2\2\u01ff\u0201\t\7\2\2\u0200\u01ff\3\2"+
		"\2\2\u0201\u0202\3\2\2\2\u0202\u0200\3\2\2\2\u0202\u0203\3\2\2\2\u0203"+
		"\u0204\3\2\2\2\u0204\u0205\bD\2\2\u0205\u0088\3\2\2\2\u0206\u0207\7\61"+
		"\2\2\u0207\u0208\7\61\2\2\u0208\u020c\3\2\2\2\u0209\u020b\n\b\2\2\u020a"+
		"\u0209\3\2\2\2\u020b\u020e\3\2\2\2\u020c\u020a\3\2\2\2\u020c\u020d\3\2"+
		"\2\2\u020d\u0214\3\2\2\2\u020e\u020c\3\2\2\2\u020f\u0211\7\17\2\2\u0210"+
		"\u020f\3\2\2\2\u0210\u0211\3\2\2\2\u0211\u0212\3\2\2\2\u0212\u0215\7\f"+
		"\2\2\u0213\u0215\7\2\2\3\u0214\u0210\3\2\2\2\u0214\u0213\3\2\2\2\u0215"+
		"\u0216\3\2\2\2\u0216\u0217\bE\3\2\u0217\u008a\3\2\2\2\u0218\u0219\13\2"+
		"\2\2\u0219\u008c\3\2\2\2\u021a\u021b\t\3\2\2\u021b\u008e\3\2\2\2\u021c"+
		"\u021d\t\t\2\2\u021d\u0090\3\2\2\2\u021e\u021f\t\n\2\2\u021f\u0092\3\2"+
		"\2\2\u0220\u0221\t\13\2\2\u0221\u0094\3\2\2\2\u0222\u0223\t\f\2\2\u0223"+
		"\u0096\3\2\2\2\u0224\u0225\t\r\2\2\u0225\u0098\3\2\2\2\u0226\u0227\t\16"+
		"\2\2\u0227\u009a\3\2\2\2\u0228\u0229\t\17\2\2\u0229\u009c\3\2\2\2\u022a"+
		"\u022b\t\20\2\2\u022b\u009e\3\2\2\2\u022c\u022d\t\21\2\2\u022d\u00a0\3"+
		"\2\2\2\u022e\u022f\t\22\2\2\u022f\u00a2\3\2\2\2\u0230\u0231\t\23\2\2\u0231"+
		"\u00a4\3\2\2\2\u0232\u0233\t\24\2\2\u0233\u00a6\3\2\2\2\u0234\u0235\t"+
		"\25\2\2\u0235\u00a8\3\2\2\2\u0236\u0237\t\26\2\2\u0237\u00aa\3\2\2\2\u0238"+
		"\u0239\t\27\2\2\u0239\u00ac\3\2\2\2\u023a\u023b\t\30\2\2\u023b\u00ae\3"+
		"\2\2\2\u023c\u023d\t\31\2\2\u023d\u00b0\3\2\2\2\u023e\u023f\t\32\2\2\u023f"+
		"\u00b2\3\2\2\2\u0240\u0241\t\33\2\2\u0241\u00b4\3\2\2\2\u0242\u0243\t"+
		"\34\2\2\u0243\u00b6\3\2\2\2\u0244\u0245\t\35\2\2\u0245\u00b8\3\2\2\2\u0246"+
		"\u0247\t\36\2\2\u0247\u00ba\3\2\2\2\u0248\u0249\t\37\2\2\u0249\u00bc\3"+
		"\2\2\2\u024a\u024b\t \2\2\u024b\u00be\3\2\2\2\u024c\u024d\t!\2\2\u024d"+
		"\u00c0\3\2\2\2\u024e\u024f\t\"\2\2\u024f\u00c2\3\2\2\2\23\2\u017d\u0187"+
		"\u018f\u0197\u01a0\u01a8\u01af\u01b1\u01b9\u01be\u01c4\u01ca\u0202\u020c"+
		"\u0210\u0214\4\2\4\2\2\3\2";
	public static final ATN _ATN =
		new ATNDeserializer().deserialize(_serializedATN.toCharArray());
	static {
		_decisionToDFA = new DFA[_ATN.getNumberOfDecisions()];
		for (int i = 0; i < _ATN.getNumberOfDecisions(); i++) {
			_decisionToDFA[i] = new DFA(_ATN.getDecisionState(i), i);
		}
	}
}
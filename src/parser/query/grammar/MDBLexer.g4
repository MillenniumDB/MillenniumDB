lexer grammar MDBLexer;

channels { WS_CHANNEL }

// KEYWORDS
K_ANY: A N Y;
K_AND: A N D;
K_AVG: A V G;
K_ALL: A L L;
K_ASC: A S C;
K_BY: B Y;
K_BOOL: B O O L;
K_COUNT: C O U N T;
K_DESCRIBE: D E S C R I B E;
K_DESC: D E S C;
K_DISTINCT: D I S T I N C T;
K_IS: I S;
K_INTEGER: I N T E G E R;
K_FALSE: 'false';
K_FLOAT: F L O A T;
K_GROUP: G R O U P;
K_LIMIT: L I M I T;
K_MAX: M A X;
K_MATCH: M A T C H;
K_MIN: M I N;
K_OPTIONAL: O P T I O N A L;
K_ORDER: O R D E R;
K_OR: O R;
K_NOT: N O T;
K_NULL: N U L L;
K_SET: S E T;
K_SUM: S U M;
K_STRING: S T R I N G;
K_RETURN: R E T U R N;
K_TRUE: 'true';
K_WHERE: W H E R E;

// Special cases: true and false are not a valid identifier
TRUE_PROP: ':' 'true';
FALSE_PROP: ':' 'false';

ANON_ID: '_a' [1-9][0-9]*;
EDGE_ID: '_e' [1-9][0-9]*;

KEY: '.' [A-Za-z] [A-Za-z0-9_]*;
TYPE: ':' [A-Za-z] [A-Za-z0-9_]*;
TYPE_VAR: ':' '?' [A-Za-z] [A-Za-z0-9_]*;
VARIABLE: '?' [A-Za-z] [A-Za-z0-9_]*;

STRING: '"' ( ~'"' | '\\"')* '"';

UNSIGNED_INTEGER: DIGIT+;
UNSIGNED_FLOAT: DIGIT+ '.' DIGIT+;

NAME: [A-Za-z] [A-Za-z0-9_]*;

// SYMBOLS
LEQ:                                 '<=';
GEQ:                                 '>=';
EQ:                                  '==';
NEQ:                                 '!=';

LT:                                  '<';
GT:                                  '>';
SINGLE_EQ:                           '=';
PATH_SEQUENCE:                       '/';
PATH_ALTERNATIVE:                    '|';
PATH_NEGATION:                       '^';
STAR:                                '*';
PERCENT:                             '%';
QUESTION_MARK:                       '?';
PLUS:                                '+';
MINUS:                               '-';
L_PAR:                               '(';
R_PAR:                               ')';
LCURLY_BRACKET:                      '{';
RCURLY_BRACKET:                      '}';
LSQUARE_BRACKET:                     '[';
RSQUARE_BRACKET:                     ']';
COMMA:                               ',';
COLON:                               ':';

WHITE_SPACE: [ \t\r\n]+ -> channel(WS_CHANNEL);
SINGLE_LINE_COMMENT: '//' ~[\r\n]* (('\r'? '\n') | EOF) -> channel(HIDDEN);

UNRECOGNIZED: . ;

fragment DIGIT: [0-9];

fragment A: [aA];
fragment B: [bB];
fragment C: [cC];
fragment D: [dD];
fragment E: [eE];
fragment F: [fF];
fragment G: [gG];
fragment H: [hH];
fragment I: [iI];
fragment J: [jJ];
fragment K: [kK];
fragment L: [lL];
fragment M: [mM];
fragment N: [nN];
fragment O: [oO];
fragment P: [pP];
fragment Q: [qQ];
fragment R: [rR];
fragment S: [sS];
fragment T: [tT];
fragment U: [uU];
fragment V: [vV];
fragment W: [wW];
fragment X: [xX];
fragment Y: [yY];
fragment Z: [zZ];

/*
 *    Copyright 2007-2013 The sparkle-g Team
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

/**
 * @author Simone Tripodi   (simone.tripodi)
 * @author Michele Mostarda (michele.mostarda)
 * @author Juergen Pfundt   (Juergen.Pfundt)
 * @version $Id: Sparql.g 523 2012-02-17 23:10:57Z Juergen.Pfundt@gmail.com $
 */
lexer grammar SparqlQueryLexer;

// $<Lexer

WS : (' '| '\t'| EOL)+ -> channel(99);

BASE : ('B'|'b')('A'|'a')('S'|'s')('E'|'e');

PREFIX : ('P'|'p')('R'|'r')('E'|'e')('F'|'f')('I'|'i')('X'|'x');

SELECT : ('S'|'s')('E'|'e')('L'|'l')('E'|'e')('C'|'c')('T'|'t');

DISTINCT : ('D'|'d')('I'|'i')('S'|'s')('T'|'t')('I'|'i')('N'|'n')('C'|'c')('T'|'t');

REDUCED : ('R'|'r')('E'|'e')('D'|'d')('U'|'u')('C'|'c')('E'|'e')('D'|'d');

CONSTRUCT : ('C'|'c')('O'|'o')('N'|'n')('S'|'s')('T'|'t')('R'|'r')('U'|'u')('C'|'c')('T'|'t');

DESCRIBE : ('D'|'d')('E'|'e')('S'|'s')('C'|'c')('R'|'r')('I'|'i')('B'|'b')('E'|'e');

ASK : ('A'|'a')('S'|'s')('K'|'k');

FROM : ('F'|'f')('R'|'r')('O'|'o')('M'|'m');

NAMED : ('N'|'n')('A'|'a')('M'|'m')('E'|'e')('D'|'d');

WHERE : ('W'|'w')('H'|'h')('E'|'e')('R'|'r')('E'|'e');

ORDER : ('O'|'o')('R'|'r')('D'|'d')('E'|'e')('R'|'r');

BY : ('B'|'b')('Y'|'y');

ASC : ('A'|'a')('S'|'s')('C'|'c');

DESC : ('D'|'d')('E'|'e')('S'|'s')('C'|'c');

LIMIT : ('L'|'l')('I'|'i')('M'|'m')('I'|'i')('T'|'t');

OFFSET : ('O'|'o')('F'|'f')('F'|'f')('S'|'s')('E'|'e')('T'|'t');

VALUES : ('V'|'v')('A'|'a')('L'|'l')('U'|'u')('E'|'e')('S'|'s');

OPTIONAL : ('O'|'o')('P'|'p')('T'|'t')('I'|'i')('O'|'o')('N'|'n')('A'|'a')('L'|'l');

GRAPH : ('G'|'g')('R'|'r')('A'|'a')('P'|'p')('H'|'h');

UNION : ('U'|'u')('N'|'n')('I'|'i')('O'|'o')('N'|'n');

FILTER : ('F'|'f')('I'|'i')('L'|'l')('T'|'t')('E'|'e')('R'|'r');

A : 'a';

STR : ('S'|'s')('T'|'t')('R'|'r');

LANG : ('L'|'l')('A'|'a')('N'|'n')('G'|'g');

LANGMATCHES : ('L'|'l')('A'|'a')('N'|'n')('G'|'g')('M'|'m')('A'|'a')('T'|'t')('C'|'c')('H'|'h')('E'|'e')('S'|'s');

DATATYPE : ('D'|'d')('A'|'a')('T'|'t')('A'|'a')('T'|'t')('Y'|'y')('P'|'p')('E'|'e');

BOUND : ('B'|'b')('O'|'o')('U'|'u')('N'|'n')('D'|'d');

SAMETERM : ('S'|'s')('A'|'a')('M'|'m')('E'|'e')('T'|'t')('E'|'e')('R'|'r')('M'|'m');

ISIRI : ('I'|'i')('S'|'s')('I'|'i')('R'|'r')('I'|'i');

ISURI : ('I'|'i')('S'|'s')('U'|'u')('R'|'r')('I'|'i');

ISBLANK : ('I'|'i')('S'|'s')('B'|'b')('L'|'l')('A'|'a')('N'|'n')('K'|'k');

ISLITERAL : ('I'|'i')('S'|'s')('L'|'l')('I'|'i')('T'|'t')('E'|'e')('R'|'r')('A'|'a')('L'|'l');

REGEX : ('R'|'r')('E'|'e')('G'|'g')('E'|'e')('X'|'x');

SUBSTR : ('S'|'s')('U'|'u')('B'|'b')('S'|'s')('T'|'t')('R'|'r');

K_TRUE : ('T'|'t')('R'|'r')('U'|'u')('E'|'e');

K_FALSE : ('F'|'f')('A'|'a')('L'|'l')('S'|'s')('E'|'e');

// LOAD : ('L'|'l')('O'|'o')('A'|'a')('D'|'d');

// CLEAR : ('C'|'c')('L'|'l')('E'|'e')('A'|'a')('R'|'r');

// DROP : ('D'|'d')('R'|'r')('O'|'o')('P'|'p');

// ADD : ('A'|'a')('D'|'d')('D'|'d');

// MOVE : ('M'|'m')('O'|'o')('V'|'v')('E'|'e');

// COPY : ('C'|'c')('O'|'o')('P'|'p')('Y'|'y');

// CREATE 	: ('C'|'c')('R'|'r')('E'|'e')('A'|'a')('T'|'t')('E'|'e');

// DELETE : ('D'|'d')('E'|'e')('L'|'l')('E'|'e')('T'|'t')('E'|'e');

// INSERT : ('I'|'i')('N'|'n')('S'|'s')('E'|'e')('R'|'r')('T'|'t');

// USING : ('U'|'u')('S'|'s')('I'|'i')('N'|'n')('G'|'g');

SILENT : ('S'|'s')('I'|'i')('L'|'l')('E'|'e')('N'|'n')('T'|'t');

// DEFAULT : ('D'|'d')('E'|'e')('F'|'f')('A'|'a')('U'|'u')('L'|'l')('T'|'t');

ALL : ('A'|'a')('L'|'l')('L'|'l');

// DATA : ('D'|'d')('A'|'a')('T'|'t')('A'|'a');

// WITH : ('W'|'w')('I'|'i')('T'|'t')('H'|'h');

// INTO : ('I'|'i')('N'|'n')('T'|'t')('O'|'o');

// TO : ('T'|'t')('O'|'o');

AS : ('A'|'a')('S'|'s');

GROUP : ('G'|'g')('R'|'r')('O'|'o')('U'|'u')('P'|'p');

HAVING : ('H'|'h')('A'|'a')('V'|'v')('I'|'i')('N'|'n')('G'|'g');

UNDEF : ('U'|'u')('N'|'n')('D'|'d')('E'|'e')('F'|'f');

BINDINGS : ('B'|'b')('I'|'i')('N'|'n')('D'|'d')('I'|'i')('N'|'n')('G'|'g')('S'|'s');

SERVICE : ('S'|'s')('E'|'e')('R'|'r')('V'|'v')('I'|'i')('C'|'c')('E'|'e');

BIND : ('B'|'b')('I'|'i')('N'|'n')('D'|'d');

MINUS : ('M'|'m')('I'|'i')('N'|'n')('U'|'u')('S'|'s');

IRI : ('I'|'i')('R'|'r')('I'|'i');

URI : ('U'|'u')('R'|'r')('I'|'i');

BNODE : ('B'|'b')('N'|'n')('O'|'o')('D'|'d')('E'|'e');

RAND : ('R'|'r')('A'|'a')('N'|'n')('D'|'d');

ABS: ('A'|'a')('B'|'b')('S'|'s');

CEIL : ('C'|'c')('E'|'e')('I'|'i')('L'|'l');

FLOOR : ('F'|'f')('L'|'l')('O'|'o')('O'|'o')('R'|'r');

ROUND : ('R'|'r')('O'|'o')('U'|'u')('N'|'n')('D'|'d');

CONCAT 	: ('C'|'c')('O'|'o')('N'|'n')('C'|'c')('A'|'a')('T'|'t');

STRLEN : ('S'|'s')('T'|'t')('R'|'r')('L'|'l')('E'|'e')('N'|'n');

UCASE : ('U'|'u')('C'|'c')('A'|'a')('S'|'s')('E'|'e');

LCASE : ('L'|'l')('C'|'c')('A'|'a')('S'|'s')('E'|'e');

ENCODE_FOR_URI : ('E'|'e')('N'|'n')('C'|'c')('O'|'o')('D'|'d')('E'|'e')'_'('F'|'f')('O'|'o')('R'|'r')'_'('U'|'u')('R'|'r')('I'|'i');

CONTAINS : ('C'|'c')('O'|'o')('N'|'n')('T'|'t')('A'|'a')('I'|'i')('N'|'n')('S'|'s');

STRSTARTS : ('S'|'s')('T'|'t')('R'|'r')('S'|'s')('T'|'t')('A'|'a')('R'|'r')('T'|'t')('S'|'s');

STRENDS : ('S'|'s')('T'|'t')('R'|'r')('E'|'e')('N'|'n')('D'|'d')('S'|'s');

STRBEFORE : ('S'|'s')('T'|'t')('R'|'r')('B'|'b')('E'|'e')('F'|'f')('O'|'o')('R'|'r')('E'|'e');

STRAFTER : ('S'|'s')('T'|'t')('R'|'r')('A'|'a')('F'|'f')('T'|'t')('E'|'e')('R'|'r');

REPLACE : ('R'|'r')('E'|'e')('P'|'p')('L'|'l')('A'|'a')('C'|'c')('E'|'e');

YEAR : ('Y'|'y')('E'|'e')('A'|'a')('R'|'r');

MONTH : ('M'|'m')('O'|'o')('N'|'n')('T'|'t')('H'|'h');

DAY : ('D'|'d')('A'|'a')('Y'|'y');

HOURS : ('H'|'h')('O'|'o')('U'|'u')('R'|'r')('S'|'s');

MINUTES : ('M'|'m')('I'|'i')('N'|'n')('U'|'u')('T'|'t')('E'|'e')('S'|'s');

SECONDS : ('S'|'s')('E'|'e')('C'|'c')('O'|'o')('N'|'n')('D'|'d')('S'|'s');

TIMEZONE :  ('T'|'t')('I'|'i')('M'|'m')('E'|'e')('Z'|'z')('O'|'o')('N'|'n')('E'|'e');

TZ : ('T'|'t')('Z'|'z');

NOW : ('N'|'n')('O'|'o')('W'|'w');

UUID : ('U'|'u')('U'|'u')('I'|'i')('D'|'d');

STRUUID : ('S'|'s')('T'|'t')('R'|'r')('U'|'u')('U'|'u')('I'|'i')('D'|'d');

MD5 : ('M'|'m')('D'|'d')'5';

SHA1 : ('S'|'s')('H'|'h')('A'|'a')'1';

SHA256 : ('S'|'s')('H'|'h')('A'|'a')'256';

SHA384 : ('S'|'s')('H'|'h')('A'|'a')'384';

SHA512 : ('S'|'s')('H'|'h')('A'|'a')'512';

COALESCE : ('C'|'c')('O'|'o')('A'|'a')('L'|'l')('E'|'e')('S'|'s')('C'|'c')('E'|'e');

IF : ('I'|'i')('F'|'f');

STRLANG : ('S'|'s')('T'|'t')('R'|'r')('L'|'l')('A'|'a')('N'|'n')('G'|'g');

STRDT : ('S'|'s')('T'|'t')('R'|'r')('D'|'d')('T'|'t');

ISNUMERIC : ('I'|'i')('S'|'s')('N'|'n')('U'|'u')('M'|'m')('E'|'e')('R'|'r')('I'|'i')('C'|'c');

COUNT : ('C'|'c')('O'|'o')('U'|'u')('N'|'n')('T'|'t');

SUM : ('S'|'s')('U'|'u')('M'|'m');

MIN : ('M'|'m')('I'|'i')('N'|'n');

MAX : ('M'|'m')('A'|'a')('X'|'x');

AVG : ('A'|'a')('V'|'v')('G'|'g');

SAMPLE : ('S'|'s')('A'|'a')('M'|'m')('P'|'p')('L'|'l')('E'|'e');

GROUP_CONCAT  :	 ('G'|'g')('R'|'r')('O'|'o')('U'|'u')('P'|'p') '_' ('C'|'c')('O'|'o')('N'|'n')('C'|'c')('A'|'a')('T'|'t');

NOT : ('N'|'n')('O'|'o')('T'|'t');

IN : ('I'|'i')('N'|'n');

EXISTS : ('E'|'e')('X'|'x')('I'|'i')('S'|'s')('T'|'t')('S'|'s');

SEPARATOR : ('S'|'s')('E'|'e')('P'|'p')('A'|'a')('R'|'r')('A'|'a')('T'|'t')('O'|'o')('R'|'r');

// MDB extension
// Already defined before ALL: ('A'|'a')('L'|'l')('L'|'l');
ANY: ('A'|'a')('N'|'n')('Y'|'y');
SHORTEST: ('S'|'s')('H'|'h')('O'|'o')('R'|'r')('T'|'t')('E'|'e')('S'|'s')('T'|'t');
SIMPLE: ('S'|'s')('I'|'i')('M'|'m')('P'|'p')('L'|'l')('E'|'e');
ACYCLIC: ('A'|'a')('C'|'c')('Y'|'y')('C'|'c')('L'|'l')('I'|'i')('C'|'c');
WALKS: ('W'|'w')('A'|'a')('L'|'l')('K'|'k')('S'|'s');

SHOW: ('S'|'s')('H'|'h')('O'|'o')('W'|'w');

INDEX: ('I'|'i')('N'|'n')('D'|'d')('E'|'e')('X'|'x');

ALPHANUMERIC_IDENTIFIER: [A-Za-z] [A-Za-z0-9_]*;

IRIREF
    : '<' (~('<' | '>' | '"' | '{' | '}' | '|' | '^' | '`' | '\\' | '\u0000'..'\u0020'))* '>'
    ;

PNAME_NS : PN_PREFIX? ':';

PNAME_LN : PNAME_NS PN_LOCAL;

BLANK_NODE_LABEL : '_:' (PN_CHARS_U|DIGIT) ((PN_CHARS|DOT)* PN_CHARS)?;

VAR1 : '?' VARNAME;

VAR2 : '$' VARNAME;

LANGTAG : '@' ('A'..'Z'|'a'..'z')+ (MINUS_SIGN ('A'..'Z'|'a'..'z'|DIGIT)+)*;

INTEGER : DIGIT+;

DECIMAL
    : DIGIT+ DOT DIGIT* /* Allows decimal with no fraction: 1. */
    | DOT DIGIT+
    ;

DOUBLE
    : DIGIT+ DOT DIGIT* EXPONENT
    | DOT DIGIT+ EXPONENT
    | DIGIT+ EXPONENT
    ;

INTEGER_POSITIVE : PLUS_SIGN INTEGER;

DECIMAL_POSITIVE : PLUS_SIGN DECIMAL;

DOUBLE_POSITIVE : PLUS_SIGN DOUBLE;

INTEGER_NEGATIVE : MINUS_SIGN INTEGER;

DECIMAL_NEGATIVE : MINUS_SIGN DECIMAL;

DOUBLE_NEGATIVE : MINUS_SIGN DOUBLE;

fragment
EXPONENT : ('e'|'E') SIGN? DIGIT+;

STRING_LITERAL1 : '\'' (~('\'' | '\\' | '\n' | '\r') | ECHAR)* '\'';

STRING_LITERAL2 : '"' (~('"' | '\\' | '\n' | '\r') | ECHAR)* '"';

STRING_LITERAL_LONG1 : '\'\'\'' (('\'' | '\'\'')? (~('\''|'\\') | ECHAR))* '\'\'\'';

STRING_LITERAL_LONG2 : '"""' (('"' | '""')? (~('"'|'\\') | ECHAR))* '"""';

fragment
ECHAR : '\\' ('t' | 'b' | 'n' | 'r' | 'f' | 'u' | 'U' | '\\' | '"' | '\'');

fragment
PN_CHARS_BASE
    : 'A'..'Z'
    | 'a'..'z'
    | '\u00C0'..'\u00D6'
    | '\u00D8'..'\u00F6'
    | '\u00F8'..'\u02FF'
    | '\u0370'..'\u037D'
    | '\u037F'..'\u1FFF'
    | '\u200C'..'\u200D'
    | '\u2070'..'\u218F'
    | '\u2C00'..'\u2FEF'
    | '\u3001'..'\uD7FF'
    | '\uF900'..'\uFDCF'
    | '\uFDF0'..'\uFFFD'
    //| '\u10000'..'\uEFFFF'
    ;

fragment
PN_CHARS_U : PN_CHARS_BASE | '_';

fragment
VARNAME : (PN_CHARS_U | DIGIT) (PN_CHARS_U | DIGIT | '\u00B7' | '\u0300'..'\u036F' | '\u203F'..'\u2040')*;

fragment
PN_CHARS : PN_CHARS_U | MINUS_SIGN | DIGIT | '\u00B7' | '\u0300'..'\u036F' | '\u203F'..'\u2040';

fragment
PN_PREFIX : PN_CHARS_BASE ((PN_CHARS|DOT)* PN_CHARS)?;

fragment
PN_LOCAL : (PN_CHARS_U|':'|DIGIT|PLX) ((PN_CHARS|DOT|':'|PLX)* (PN_CHARS|':'|PLX))?;

fragment
PLX : PERCENT | PN_LOCAL_ESC;

fragment
PERCENT : '%' HEX HEX;

fragment
HEX : DIGIT | 'A'..'F' | 'a'..'z';

fragment
PN_LOCAL_ESC : '\\' ( '_' | '~' | '.' | '-' | '!' | '$' | '&' | '\'' | '(' | ')' | '*' | '+' | ',' | ';' | '=' | '/' | '?' | '#' | '@' | '%' );

fragment
DIGIT : '0'..'9';

COMMENT : '#' .*? (EOL|EOF) -> channel(99);

fragment
EOL : '\n' | '\r';

REFERENCE : '^^';

LESS_EQUAL : '<=';

GREATER_EQUAL : '>=';

NOT_EQUAL : '!=';

AND : '&&';

OR : '||';

INVERSE : '^';

OPEN_BRACE : '(';

CLOSE_BRACE : ')';

OPEN_CURLY_BRACE : '{';

CLOSE_CURLY_BRACE : '}';

OPEN_SQUARE_BRACKET : '[';

CLOSE_SQUARE_BRACKET : ']';

SEMICOLON : ';';

DOT : '.';

PLUS_SIGN : '+';

MINUS_SIGN : '-';

fragment
SIGN : (PLUS_SIGN|MINUS_SIGN);

ASTERISK : '*';

QUESTION_MARK : '?';

COMMA : ',';

NEGATION : '!';

DIVIDE : '/';

EQUAL : '=';

LESS : '<';

GREATER : '>';

PIPE : '|';

ANY_OTHER : .;

// $>

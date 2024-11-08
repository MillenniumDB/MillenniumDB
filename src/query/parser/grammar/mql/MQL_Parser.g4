
parser grammar MQL_Parser;

options {
	tokenVocab = MQL_Lexer;
}

root: ((setStatement? matchQuery | describeQuery) | insertPatterns) EOF;

matchQuery: matchStatement (projectSimilarity | bruteSimilaritySearch)? whereStatement? groupByStatement? orderByStatement? returnStatement;

insertPatterns: K_INSERT insertLinearPattern (',' insertLinearPattern)*;

insertLinearPattern: insertPlainNode (insertPlainEdge insertPlainNode)*;

insertPlainNode: '(' insertPlainNodeInside? TYPE* properties?')';

insertPlainNodeInside: identifier;

insertPlainEdge: '<' '-' '[' TYPE properties? ']''-'
|                '-' '[' TYPE properties? ']''-' '>'
;

// updateStatements: (insertStatement | deleteStatement | updateSetStatement)+;

// insertStatement: K_INSERT insertElement (',' insertElement)*;

// deleteStatement: K_DELETE deleteElement (',' deleteElement)*;

// deleteElement: (identifier) (TYPE|KEY)?
// |              EDGE_ID KEY?
// |              VARIABLE KEY?
// |              K_LABELS '(' insertObj ')'
// |              K_PROPERTIES '(' insertObj ')'
// ;

// insertObj: identifier|EDGE_ID|VARIABLE;

// updateSetStatement: K_SET setElement (',' setElement)*;

// setElement: K_PROPERTIES '(' insertObj ')' '=' insertProperties
// |           K_LABELS '(' insertObj ')' '=' '{' TYPE (',' TYPE)* '}'
// |           (identifier|VARIABLE) KEY '=' '{' TYPE (',' TYPE)* '}'
// ;

// insertElement: insertNode (insertEdge insertNode)*;

// insertNode: '(' insertObj? TYPE* insertProperties?')';

// insertEdge: '<' '-' '[' TYPE insertProperties? ']''-'
// |           '-' '[' TYPE insertProperties? ']''-' '>'
// ;

// insertProperties: '{' insertProperty (',' insertProperty)* '}';


// TODO: maybe allow expressions as values?
// insertProperty2 is necessary when the property is written without spaces after the colon, example:
// key:date("2001-02-03")
// key        :date
// identifier TYPE  '(' STRING ')';
// insertProperty: identifier (':' value | TRUE_PROP | FALSE_PROP) # insertProperty1
// |               identifier TYPE '(' STRING ')' # insertProperty2
// ;

projectSimilarity:  K_PROJECT_SIMILARITY '(' VARIABLE ',' VARIABLE ',' STRING ',' (fixedNodeInside | tensor) ',' metricType ')';

metricType: K_ANGULAR | K_EUCLIDEAN | K_MANHATTAN;

describeQuery: K_DESCRIBE describeFlag* fixedNodeInside;

describeFlag: ( K_LABELS | K_PROPERTIES| K_OUTGOING | K_INCOMING ) (K_LIMIT UNSIGNED_INTEGER)?;

setStatement: K_SET setItem (',' setItem)*;

matchStatement: K_MATCH graphPattern;

whereStatement: K_WHERE conditionalOrExpr;

groupByStatement: K_GROUP K_BY groupByItem (',' groupByItem)*;

orderByStatement: K_ORDER K_BY orderByItem (',' orderByItem)*;

returnStatement: K_RETURN K_DISTINCT? returnItem (',' returnItem)* limitOffsetClauses? # returnList
|                K_RETURN K_DISTINCT? '*' limitOffsetClauses? # returnAll
;

limitOffsetClauses
    : limitClause offsetClause? | offsetClause limitClause?
    ;

limitClause
    : K_LIMIT UNSIGNED_INTEGER
    ;

offsetClause
    : K_OFFSET UNSIGNED_INTEGER
    ;

setItem: VARIABLE '=' fixedNodeInside;

returnItem: VARIABLE KEY? # returnItemVar
|           aggregateFunc '(' VARIABLE KEY? ')' # returnItemAgg
|           K_COUNT '(' K_DISTINCT? (VARIABLE KEY? | '*') ')' # returnItemCount
;

aggregateFunc: K_SUM
|              K_MAX
|              K_MIN
|              K_AVG
;

orderByItem: VARIABLE KEY? (K_ASC | K_DESC)? # orderByItemVar
|            aggregateFunc '(' VARIABLE KEY? ')' (K_ASC | K_DESC)? # orderByItemAgg
|            K_COUNT '(' K_DISTINCT? VARIABLE KEY? ')' (K_ASC | K_DESC)?  # orderByItemCount
;

groupByItem: VARIABLE KEY?;

graphPattern: basicPattern optionalPattern*;

optionalPattern: K_OPTIONAL '{' graphPattern '}';

similaritySearch: K_SIMILARITY_SEARCH '(' VARIABLE ',' VARIABLE ',' STRING ',' (fixedNodeInside | tensor) ',' ('+' | '-')? UNSIGNED_INTEGER (',' ('+')? UNSIGNED_INTEGER)? ')';

bruteSimilaritySearch: K_BRUTE_SIMILARITY_SEARCH '(' VARIABLE ',' VARIABLE ',' STRING ',' (fixedNodeInside | tensor) ',' metricType ',' ('+')? UNSIGNED_INTEGER ')';

tensor: '[' numericValue (',' numericValue)* ']';

basicPattern: linearPattern (',' linearPattern)*;

linearPattern: (node ((edge | path) node)*) | similaritySearch;

path:'<=' '[' pathType? VARIABLE? pathAlternatives']' '='
|    '=' '[' pathType? VARIABLE? pathAlternatives']' '=' '>'
;

pathAlternatives: pathSequence ('|' pathSequence)*;

pathSequence: pathAtom ('/' pathAtom)*;

pathAtom: '^'? TYPE pathSuffix? # pathAtomSimple
|         '^'? '(' pathAlternatives ')' pathSuffix? # pathAtomAlternatives
;

pathSuffix: op='*'
|           op='+'
|           op='?'
|           '{' min=UNSIGNED_INTEGER ',' max=UNSIGNED_INTEGER '}'
;

pathType: (K_ANY|K_ALL) (K_SHORTEST)? (K_WALKS|K_SIMPLE|K_ACYCLIC|K_TRAILS)?;

node: fixedNode
|     varNode
;

fixedNode: '(' fixedNodeInside ')';

fixedNodeInside: identifier | ANON_ID | EDGE_ID | value;

edge: '<' ('-' edgeInside)? '-'
|     '-' (edgeInside '-')? '>'
;

edgeInside: '[' (VARIABLE | EDGE_ID)? (TYPE | TYPE_VAR)? properties? ']';

varNode: '(' VARIABLE? TYPE* properties? ')';

properties: '{' property (',' property)* '}';

// property2 is necessary when the property is written without spaces after the colon, example:
// key:date("2001-02-03")
// key        :date
// identifier TYPE  '(' STRING ')';
property: identifier (':' value | TRUE_PROP | FALSE_PROP) # property1
|         identifier TYPE '(' STRING ')' # property2
|         identifier K_IS K_NOT? exprTypename (conditionalOrType)*# property3
|         identifier  (op=('=='|'!='|'<'|'>'|'<='|'>=') value)# property4
;


conditionalOrType: K_OR exprTypename;

identifier: NAME | keyword;

boolValue: K_TRUE | K_FALSE;

numericValue: ('+' | '-')? (UNSIGNED_INTEGER | UNSIGNED_FLOAT | UNSIGNED_SCIENTIFIC_NOTATION);

datatypeValue: identifier '(' STRING ')';

value: numericValue | STRING | boolValue | datatypeValue;

conditionalOrExpr: conditionalAndExpr (K_OR conditionalAndExpr)*;

conditionalAndExpr: comparisonExpr (K_AND comparisonExpr)*;

comparisonExpr: additiveExpr (op=('=='|'!='|'<'|'>'|'<='|'>=') additiveExpr)? # comparisonExprOp
|               additiveExpr K_IS K_NOT? exprTypename # comparisonExprIs
;

additiveExpr: multiplicativeExpr (op+=('+'|'-') multiplicativeExpr)*;

multiplicativeExpr: unaryExpr (op+=('*'|'/'|'%') unaryExpr)*;

unaryExpr: K_NOT unaryExpr
|          atomicExpr
|          '+' unaryExpr
|          '-' unaryExpr
;

atomicExpr:  VARIABLE KEY? # exprVar
|            value # exprValue
|            '(' conditionalOrExpr ')' # exprParenthesis
|            function #exprFunction
;

function: regex;

regex: K_REGEX '(' conditionalOrExpr ',' conditionalOrExpr (',' conditionalOrExpr)? ')';

exprTypename: K_NULL
|             K_STRING
|             K_BOOL
|             K_INTEGER
|             K_FLOAT
;

// it excludes keywords true and false
keyword: K_ACYCLIC
| 	     K_AND
|        K_ANGULAR
| 	     K_ANY
|        K_ALL
| 	     K_AVG
| 	     K_ASC
| 	     K_BY
| 	     K_BOOL
| 	     K_COUNT
| 	     K_DELETE
| 	     K_DESCRIBE
| 	     K_DESC
| 	     K_DISTINCT
| 	     K_EDGE
|        K_EUCLIDEAN
| 	     K_INCOMING
| 	     K_INSERT
| 	     K_INTEGER
| 	     K_IS
| 	     K_FLOAT
| 	     K_GROUP
| 	     K_LABELS
| 	     K_LABEL
| 	     K_LIMIT
|        K_MANHATTAN
| 	     K_MATCH
| 	     K_MAX
| 	     K_MIN
| 	     K_OFFSET
| 	     K_OPTIONAL
| 	     K_ORDER
| 	     K_OR
| 	     K_OUTGOING
| 	     K_PROPERTIES
| 	     K_PROPERTY
| 	     K_NOT
| 	     K_NULL
| 	     K_SHORTEST
| 	     K_SIMPLE
| 	     K_RETURN
| 	     K_SET
|		 K_SIMILARITY_SEARCH
| 	     K_SUM
| 	     K_STRING
| 	     K_TRAILS
| 	     K_WALKS
| 	     K_WHERE
;

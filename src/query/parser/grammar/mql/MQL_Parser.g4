
parser grammar MQL_Parser;

options {
	tokenVocab = MQL_Lexer;
}

root: (matchQuery | describeQuery | insertPatterns | createIndexQuery | showQuery) EOF;

matchQuery: primitiveStatementList whereStatement? groupByStatement? orderByStatement?
        returnStatement;

primitiveStatementList: primitiveStatement+;

primitiveStatement: matchStatement | callStatement | letStatement;

insertPatterns: K_INSERT insertLinearPattern (',' insertLinearPattern)*;

insertLinearPattern: insertPlainNode (insertPlainEdge insertPlainNode)*;

insertPlainNode: '(' insertPlainNodeInside? TYPE* properties?')';

insertPlainNodeInside: identifier;

insertPlainEdge: '<' '-' '[' TYPE properties? ']''-'
|                '-' '[' TYPE properties? ']''-' '>'
;

createIndexQuery: K_CREATE identifier K_INDEX STRING K_WITH createIndexOptions;

createIndexOptions:
    '{' (createIndexOption (',' createIndexOption)*)? '}';

createIndexOption:
    STRING ('=' value | boolValue);

showQuery: K_SHOW (
|   identifier K_INDEX
);

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

describeQuery: K_DESCRIBE describeFlag* fixedNodeInside;

describeFlag: ( K_LABELS | K_PROPERTIES| K_OUTGOING | K_INCOMING ) (K_LIMIT UNSIGNED_INTEGER)?;

matchStatement: K_MATCH graphPattern;

letStatement: K_LET letDefinitionList;

letDefinitionList: letDefinition (',' letDefinition)*;

letDefinition: VARIABLE '=' conditionalOrExpr;

whereStatement: K_WHERE conditionalOrExpr;

groupByStatement: K_GROUP K_BY groupByItem (',' groupByItem)*;

orderByStatement: K_ORDER K_BY orderByItem (',' orderByItem)*;

returnStatement: K_RETURN K_DISTINCT? returnItem (',' returnItem)* limitOffsetClauses? # returnList
|                K_RETURN K_DISTINCT? '*' limitOffsetClauses? # returnAll
;

callStatement: K_CALL identifier '(' callArguments? ')' yieldStatement?;

yieldStatement: K_YIELD ('*' | yieldItem (',' yieldItem)*);

yieldItem: VARIABLE alias?;

callArguments: conditionalOrExpr (',' conditionalOrExpr)*;

limitOffsetClauses
    : limitClause offsetClause? | offsetClause limitClause?
    ;

limitClause
    : K_LIMIT UNSIGNED_INTEGER
    ;

offsetClause
    : K_OFFSET UNSIGNED_INTEGER
    ;

returnItem: VARIABLE KEY? # returnItemVar
|           aggregateFunc '(' VARIABLE KEY? ')' (alias)? # returnItemAgg
|           K_COUNT '(' K_DISTINCT? (VARIABLE KEY? | '*') ')' (alias)? # returnItemCount
|           conditionalOrExpr alias #returnItemExpr
;

alias: K_AS VARIABLE;

aggregateFunc: K_SUM
|              K_MAX
|              K_MIN
|              K_AVG
;

orderByItem: VARIABLE KEY? (K_ASC | K_DESC)? # orderByItemVar
|            aggregateFunc '(' VARIABLE KEY? ')' (K_ASC | K_DESC)? # orderByItemAgg
|            K_COUNT '(' K_DISTINCT? VARIABLE KEY? ')' (K_ASC | K_DESC)?  # orderByItemCount
|            conditionalOrExpr (K_ASC | K_DESC)? # orderByItemExpr
;

groupByItem: VARIABLE KEY?;

graphPattern: basicPattern optionalPattern*;

optionalPattern: K_OPTIONAL '{' graphPattern '}';

basicPattern: linearPattern (',' linearPattern)*;

linearPattern: (node ((edge | path) node)*);

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

pathType: (K_ANY|K_ALL) (K_SHORTEST)? (K_WALKS|K_SIMPLE|K_ACYCLIC|K_TRAILS)?
|         (K_SHORTEST) UNSIGNED_INTEGER K_GROUPS? (K_WALKS|K_SIMPLE|K_ACYCLIC|K_TRAILS)?
;

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
|            fixedNodeInside #exprFixedNodeInside
|            '(' conditionalOrExpr ')' # exprParenthesis
|            function #exprFunction
;

function: regex
|         textSearch
|         cosineSimilarity
|         cosineDistance
|         manhattanDistance
|         euclideanDistance;

regex: K_REGEX '(' conditionalOrExpr ',' conditionalOrExpr (',' conditionalOrExpr)? ')';

textSearch: K_TEXT_SEARCH '(' STRING ',' STRING ',' textSearchIndexMode ',' VARIABLE (',' VARIABLE)? ')';

cosineSimilarity:  K_COSINE_SIMILARITY  '(' conditionalOrExpr ',' conditionalOrExpr ')';

cosineDistance:    K_COSINE_DISTANCE    '(' conditionalOrExpr ',' conditionalOrExpr ')';

manhattanDistance: K_MANHATTAN_DISTANCE '(' conditionalOrExpr ',' conditionalOrExpr ')';

euclideanDistance: K_EUCLIDEAN_DISTANCE '(' conditionalOrExpr ',' conditionalOrExpr ')';

textSearchIndexMode: K_PREFIX | K_MATCH;

exprTypename: K_NULL
|             K_STRING
|             K_BOOL
|             K_INTEGER
|             K_FLOAT
;

// it excludes keywords true and false
keyword:
K_ACYCLIC
| K_AND
| K_ANY
| K_AS
| K_AVG
| K_ALL
| K_ASC
| K_BY
| K_BOOL
| K_CALL
| K_COSINE_DISTANCE
| K_COSINE_SIMILARITY
| K_COUNT
| K_CREATE
| K_DELETE
| K_DESCRIBE
| K_DESC
| K_DIMENSIONS
| K_DISTINCT
| K_EDGE
| K_EUCLIDEAN_DISTANCE
| K_FROM
| K_INCOMING
| K_INDEX
| K_INSERT
| K_INTEGER
| K_INTO
| K_IS
| K_FLOAT
| K_GROUP
| K_GROUPS
| K_IDENTITY
| K_LABELS
| K_LABEL
| K_LET
| K_LIMIT
| K_MANHATTAN_DISTANCE
| K_MATCH
| K_MAX
| K_MIN
| K_OBJECTS
| K_OFFSET
| K_OPTIONAL
| K_ORDER
| K_OR
| K_OUTGOING
| K_PREFIX
| K_PROPERTIES
| K_PROPERTY
| K_NOT
| K_NULL
| K_SHORTEST
| K_SHOW
| K_SIMPLE
| K_TEXT_SEARCH
| K_REGEX
| K_RETURN
| K_SUM
| K_STRING
| K_TRAILS
| K_VALUES
| K_WALKS
| K_WITH
| K_WHERE
| K_YIELD
;

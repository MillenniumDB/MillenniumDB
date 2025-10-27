parser grammar MQL_Parser;

options {
	tokenVocab = MQL_Lexer;
}

root: (
		simpleQuery
		| describeQuery
		| createIndexQuery
		| showQuery
		| updateStatement
	) EOF;

updateStatement:
	insertStatement
	| deleteStatement
	| setStatement
	| removeStatement;

insertStatement:
	K_INSERT insertLinearPattern (',' insertLinearPattern)*;

insertLinearPattern: insertNode (insertEdge insertNode)*;

insertNode:
	'(' (identifier | VARIABLE)? TYPE* insertProperties? ')';

insertEdge:
	'<' '-' '[' TYPE insertProperties? ']' '-'
	| '-' '[' TYPE insertProperties? ']' '-' '>';

insertProperties: '{' insertProperty (',' insertProperty)* '}';

// insertProperty2 is necessary when the property is written without spaces after the colon,
// example: key:date("2001-02-03") key :date identifier TYPE '(' STRING ')';
insertProperty:
	identifier (':' value | TRUE_PROP | FALSE_PROP)	# insertProperty1
	| identifier TYPE '(' STRING ')'				# insertProperty2
	| identifier ':' conditionalOrExpr				# insertProperty3;

deleteStatement:
	K_DETACH? K_DELETE (fixedObj | VARIABLE) (
		',' (fixedObj | VARIABLE)
	)*;

setStatement: K_SET setAtom (',' setAtom)*;

setAtom: (fixedObj | VARIABLE) KEY '=' conditionalOrExpr
	| (fixedObj | VARIABLE) insertProperties
	| (fixedObj | VARIABLE) (TYPE)+;

removeStatement: K_REMOVE removeAtom (',' removeAtom)*;

removeAtom: (fixedObj | VARIABLE) KEY
	| (fixedObj | VARIABLE) (TYPE)+;

simpleQuery:
	primitiveStatement+ groupByStatement? havingStatement? orderByStatement? (
		returnStatement
		| updateStatement+
	);

primitiveStatement:
	matchStatement
	| callStatement
	| whereStatement
	| letStatement;

createIndexQuery:
	K_CREATE identifier K_INDEX STRING K_WITH createIndexOptions;

createIndexOptions:
	'{' (createIndexOption (',' createIndexOption)*)? '}';

createIndexOption: STRING ('=' value | boolValue);

showQuery: K_SHOW identifier K_INDEX;

describeQuery: K_DESCRIBE describeFlag* fixedObj;

describeFlag: (K_LABELS | K_PROPERTIES | K_OUTGOING | K_INCOMING) (
		K_LIMIT UNSIGNED_INTEGER
	)?;

matchStatement: K_MATCH graphPattern;

letStatement: K_LET letDefinition (',' letDefinition)*;

letDefinition: VARIABLE '=' conditionalOrExpr;

whereStatement: K_WHERE conditionalOrExpr;

groupByStatement: K_GROUP K_BY groupByItem (',' groupByItem)*;

havingStatement: K_HAVING conditionalOrExpr;

orderByStatement: K_ORDER K_BY orderByItem (',' orderByItem)*;

returnStatement:
	K_RETURN K_DISTINCT? returnItem (',' returnItem)* limitOffsetClauses?	# returnList
	| K_RETURN K_DISTINCT? '*' limitOffsetClauses?							# returnAll;

callStatement:
	K_CALL identifier '(' callArguments? ')' yieldStatement?;

yieldStatement: K_YIELD ('*' | yieldItem (',' yieldItem)*);

yieldItem: VARIABLE alias?;

callArguments: conditionalOrExpr (',' conditionalOrExpr)*;

limitOffsetClauses:
	limitClause offsetClause?
	| offsetClause limitClause?;

limitClause: K_LIMIT UNSIGNED_INTEGER;

offsetClause: K_OFFSET UNSIGNED_INTEGER;

// returnItem:
// 	VARIABLE KEY?												# returnItemVar
// 	| aggregateFunc '(' VARIABLE KEY? ')' alias?				# returnItemAgg
// 	| K_COUNT '(' K_DISTINCT? (VARIABLE KEY? | '*') ')' alias?	# returnItemCount
// 	| conditionalOrExpr alias?									# returnItemExpr;

returnItem: conditionalOrExpr alias?;

alias: K_AS VARIABLE;

aggregateFunc: K_SUM | K_MAX | K_MIN | K_AVG;

// orderByItem: VARIABLE KEY? (K_ASC | K_DESC)? # orderByItemVar | aggregateFunc '(' VARIABLE KEY?
// ')' (K_ASC | K_DESC)? # orderByItemAgg | K_COUNT '(' K_DISTINCT? VARIABLE KEY? ')' (K_ASC |
// K_DESC)? # orderByItemCount | conditionalOrExpr (K_ASC | K_DESC)? # orderByItemExpr;

orderByItem: conditionalOrExpr (K_ASC | K_DESC)?;

groupByItem: VARIABLE KEY?;

graphPattern: basicPattern optionalPattern*;

optionalPattern: K_OPTIONAL '{' graphPattern '}';

basicPattern: linearPattern (',' linearPattern)*;

linearPattern: (node ((edge | path) node)*);

path:
	'<=' '[' pathType? VARIABLE? pathAlternatives ']' '='
	| '=' '[' pathType? VARIABLE? pathAlternatives ']' '=' '>';

pathAlternatives: pathSequence ('|' pathSequence)*;

pathSequence: pathAtom ('/' pathAtom)*;

pathAtom:
	'^'? TYPE pathSuffix?						# pathAtomSimple
	| '^'? '(' pathAlternatives ')' pathSuffix?	# pathAtomAlternatives;

pathSuffix:
	op = '*'
	| op = '+'
	| op = '?'
	| '{' min = UNSIGNED_INTEGER ',' max = UNSIGNED_INTEGER '}';

pathType: (K_ANY | K_ALL) (K_SHORTEST)? (
		K_WALKS
		| K_SIMPLE
		| K_ACYCLIC
		| K_TRAILS
	)?
	| (K_SHORTEST) UNSIGNED_INTEGER K_GROUPS? (
		K_WALKS
		| K_SIMPLE
		| K_ACYCLIC
		| K_TRAILS
	)?;

node: '(' (fixedObj | varNode) ')';

fixedObj: identifier | ANON_ID | EDGE_ID;

varNode: VARIABLE? TYPE* properties?;

edge: '<' ('-' edgeInside)? '-' | '-' (edgeInside '-')? '>';

edgeInside:
	'[' (VARIABLE | EDGE_ID)? (TYPE | TYPE_VAR)? properties? ']';

properties: '{' property (',' property)* '}';

// property2 is necessary when the property is written without spaces after the colon, example:
// key:date("2001-02-03") key :date identifier TYPE '(' STRING ')';
property:
	identifier (':' value | TRUE_PROP | FALSE_PROP)				# property1
	| identifier TYPE '(' STRING ')'							# property2
	| identifier K_IS K_NOT? exprTypename (conditionalOrType)*	# property3
	| identifier (
		op = ('==' | '!=' | '<' | '>' | '<=' | '>=') value
	) # property4;

conditionalOrType: K_OR exprTypename;

identifier: NAME | keyword;

boolValue: K_TRUE | K_FALSE;

numericValue: ('+' | '-')? (
		UNSIGNED_INTEGER
		| UNSIGNED_FLOAT
		| UNSIGNED_SCIENTIFIC_NOTATION
	);

datatypeValue: identifier '(' STRING ')';

value: numericValue | STRING | boolValue | datatypeValue;

conditionalOrExpr:
	conditionalAndExpr (K_OR conditionalAndExpr)*;

conditionalAndExpr: comparisonExpr (K_AND comparisonExpr)*;

comparisonExpr:
	additiveExpr (
		op = ('==' | '!=' | '<' | '>' | '<=' | '>=') additiveExpr
	)?										# comparisonExprOp
	| additiveExpr K_IS K_NOT? exprTypename	# comparisonExprIs;

additiveExpr:
	multiplicativeExpr (op += ('+' | '-') multiplicativeExpr)*;

multiplicativeExpr:
	unaryExpr (op += ('*' | '/' | '%') unaryExpr)*;

unaryExpr:
	K_NOT unaryExpr
	| atomicExpr
	| '+' unaryExpr
	| '-' unaryExpr;

atomicExpr:
	VARIABLE KEY?										# exprVar
	| function											# exprFunction
	| aggregateFunc '(' VARIABLE KEY? ')'				# exprAgg
	| K_COUNT '(' K_DISTINCT? (VARIABLE KEY? | '*') ')'	# exprCount
	| value												# exprValue
	| fixedObj											# exprFixedObj
	| '(' conditionalOrExpr ')'							# exprParenthesis;

function:
	regex
	| cosineSimilarity
	| cosineDistance
	| manhattanDistance
	| euclideanDistance
	| editDistance
	| normalize
	| str
	| labels
	| type
	| propertiesFunction;

regex:
	K_REGEX '(' conditionalOrExpr ',' conditionalOrExpr (
		',' conditionalOrExpr
	)? ')';

cosineSimilarity:
	K_COSINE_SIMILARITY '(' conditionalOrExpr ',' conditionalOrExpr ')';

cosineDistance:
	K_COSINE_DISTANCE '(' conditionalOrExpr ',' conditionalOrExpr ')';

manhattanDistance:
	K_MANHATTAN_DISTANCE '(' conditionalOrExpr ',' conditionalOrExpr ')';

euclideanDistance:
	K_EUCLIDEAN_DISTANCE '(' conditionalOrExpr ',' conditionalOrExpr ')';

editDistance:
	K_EDIT_DISTANCE '(' conditionalOrExpr ',' conditionalOrExpr ')';

normalize: K_NORMALIZE '(' conditionalOrExpr ')';

str: K_STR '(' conditionalOrExpr ')';

labels: K_LABELS '(' conditionalOrExpr ')';

propertiesFunction: K_PROPERTIES '(' conditionalOrExpr ')';

type: K_TYPE '(' conditionalOrExpr ')';

textSearchIndexMode: K_PREFIX | K_MATCH;

exprTypename: K_NULL | K_STRING | K_BOOL | K_INTEGER | K_FLOAT;

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
	| K_EDIT_DISTANCE
	| K_EUCLIDEAN_DISTANCE
	| K_FROM
	| K_HAVING
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
	| K_NORMALIZE
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
	| K_STRING
	| K_STR
	| K_TEXT_SEARCH
	| K_TYPE
	| K_REGEX
	| K_RETURN
	| K_SUM
	| K_TRAILS
	| K_VALUES
	| K_WALKS
	| K_WITH
	| K_WHERE
	| K_YIELD;
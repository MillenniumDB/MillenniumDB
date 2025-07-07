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
parser grammar SparqlQueryParser;

options{
tokenVocab=SparqlQueryLexer;
language=Cpp;
}

// $<Parser

query
    : prologue (selectQuery | constructQuery | describeQuery | askQuery) valuesClause EOF
    // | updateCommand EOF
    | showQuery
    ;

// MDB extension
showQuery: SHOW ALPHANUMERIC_IDENTIFIER INDEX;

prologue
    : (baseDecl | prefixDecl)*
    ;

baseDecl
    : BASE IRIREF
    ;

prefixDecl
    : PREFIX PNAME_NS IRIREF
    ;

selectQuery
    : selectClause datasetClause* whereClause solutionModifier
    ;

subSelect
    : selectClause whereClause solutionModifier valuesClause
    ;

selectClause
    : SELECT selectModifier? (selectVariables+ | '*')
    ;

selectModifier
    : DISTINCT | REDUCED
    ;

selectVariables
    : var                       # selectSingleVariable
    | '(' expression AS var ')' # selectExpressionAsVariable
    ;

constructQuery
    : CONSTRUCT (constructTemplate datasetClause* whereClause solutionModifier | datasetClause* WHERE '{' triplesTemplate? '}' solutionModifier)
    ;

describeQuery
    : DESCRIBE (varOrIRI+ | '*') datasetClause* whereClause? solutionModifier
    ;

askQuery
    : ASK datasetClause* whereClause solutionModifier
    ;

datasetClause
    : FROM NAMED? iri
    ;

whereClause
    : WHERE? groupGraphPattern
    ;

solutionModifier
    : groupClause? havingClause? orderClause? limitOffsetClauses?
    ;

groupClause
    : GROUP BY groupCondition+
    ;

groupCondition
    : builtInCall | functionCall | '(' expression (AS var)? ')' | var
    ;

havingClause
    : HAVING havingCondition+
    ;

havingCondition
    : constraint
    ;

orderClause
    : ORDER BY orderCondition+
    ;

orderCondition
    : (ASC|DESC) '(' expression ')'
    | constraint
    | var
    ;

limitOffsetClauses
    : limitClause offsetClause? | offsetClause limitClause?
    ;

limitClause
    : LIMIT INTEGER
    ;

offsetClause
    : OFFSET INTEGER
    ;

valuesClause
    : (VALUES dataBlock)?
    ;

// updateCommand
//     : prologue (update (';' prologue update)* (';' prologue)?)?
//     ;

// update
//     : load | clear | drop | add | move | copy | create | insertData | deleteData | deleteWhere | modify
//     ;

// load
//     : LOAD SILENT? iri (INTO graphRef)?
//     ;

// clear
//     : CLEAR  SILENT? graphRefAll
//     ;

// drop
//     : DROP SILENT? graphRefAll
//     ;

// create
//     : CREATE SILENT? graphRef
//     ;

// add
//     : ADD SILENT? graphOrDefault TO graphOrDefault
//     ;

// move
//     : MOVE SILENT? graphOrDefault TO graphOrDefault
//     ;

// copy
//     : COPY SILENT? graphOrDefault TO graphOrDefault
//     ;

// insertData
//     : INSERT DATA quadData
//     ;

// deleteData
//     : DELETE DATA quadData
//     ;

// deleteWhere
//     : DELETE WHERE quadPattern
//     ;

// modify
//     : (WITH iri)? (deleteClause insertClause? | insertClause) usingClause* WHERE groupGraphPattern
//     ;

// deleteClause
//     : DELETE quadPattern
//     ;

// insertClause
//     : INSERT quadPattern
//     ;

// usingClause
//     : USING NAMED? iri
//     ;

// graphOrDefault
//     : DEFAULT | GRAPH? iri
//     ;

// graphRef
//     : GRAPH iri
//     ;

// graphRefAll
//     : graphRef | DEFAULT | NAMED | ALL
//     ;

// quadPattern
//     : '{' quads '}'
//     ;

// quadData
//     : '{' quads '}'
//     ;

// quads
//     : triplesTemplate? quadsDetails*
//     ;

// quadsDetails
//     : quadsNotTriples '.'? triplesTemplate?
//     ;

// quadsNotTriples
//     : GRAPH varOrIRI '{' triplesTemplate? '}'
//     ;

triplesTemplate
    : triplesSameSubject ('.' triplesSameSubject?)*
    ;

groupGraphPattern
    : '{' (subSelect | groupGraphPatternSub) '}'
    ;

groupGraphPatternSub
    : triplesBlock?  groupGraphPatternSubList*
    ;

groupGraphPatternSubList
    : graphPatternNotTriples '.'? triplesBlock?
    ;

triplesBlock
    :  triplesSameSubjectPath ('.' triplesSameSubjectPath?)*
    ;

graphPatternNotTriples
    : groupOrUnionGraphPattern
    | optionalGraphPattern
    | minusGraphPattern
    | graphGraphPattern
    | serviceGraphPattern
    | filter
    | bind
    | inlineData
    // MDB extension
    | procedure
    ;

optionalGraphPattern
    : OPTIONAL groupGraphPattern
    ;

graphGraphPattern
    : GRAPH varOrIRI groupGraphPattern
    ;

serviceGraphPattern
    : SERVICE SILENT? varOrIRI groupGraphPattern
    ;

bind
    : BIND '(' expression AS var ')'
    ;

inlineData
    : VALUES dataBlock
    ;

dataBlock
    : inlineDataOneVar | inlineDataFull
    ;

inlineDataOneVar
    : var '{' dataBlockValue* '}'
    ;

inlineDataFull
    : '(' var* ')' '{' dataBlockValues* '}'
    ;

dataBlockValues
    : '(' dataBlockValue* ')'
    ;

dataBlockValue
    : iri | rdfLiteral | numericLiteral | booleanLiteral | UNDEF
    ;

minusGraphPattern
    : MINUS groupGraphPattern
    ;

groupOrUnionGraphPattern
    : groupGraphPattern (UNION groupGraphPattern)*
    ;

filter
    : FILTER constraint
    ;

constraint
    : '(' expression ')' | builtInCall | functionCall
    ;

functionCall
    : iri argList
    ;

argList
    : '(' (DISTINCT? expressionList| /* argList is allowed to be empty */) ')'
    ;

expressionList
    : expression (',' expression)*
    ;

constructTemplate
    : '{' constructTriples? '}'
    ;

constructTriples
    : triplesSameSubject ('.' constructTriples?)*
    ;

triplesSameSubject
    : varOrTerm propertyListNotEmpty | triplesNode propertyList
    ;

propertyList
    : propertyListNotEmpty?
    ;

propertyListNotEmpty
    : verb objectList (';' (verb objectList)?)*
    ;

verb
    : varOrIRI | A
    ;

objectList
    : object (',' object)*
    ;

object
    : graphNode
    ;

triplesSameSubjectPath
    : varOrTerm propertyListPathNotEmpty | triplesNodePath propertyListPath
    ;

propertyListPath
    : propertyListPathNotEmpty?
    ;

propertyListPathNotEmpty
    : (verbPath|verbSimple) objectListPath (';' propertyListPathNotEmptyList?)*
    ;

propertyListPathNotEmptyList
    : (verbPath|verbSimple) objectList
    ;

verbPath
    : path
    // MDB extension
    | (ANY | ALL) (SHORTEST)? (WALKS|SIMPLE|ACYCLIC)? '(' path AS var ')'
    ;

verbSimple
    : var
    ;

objectListPath
    : objectPath (',' objectPath)*
    ;

objectPath
    : graphNodePath
    ;

path
    : pathAlternative
    ;

pathAlternative
    : pathSequence ('|' pathSequence)*
    ;

pathSequence
    : pathEltOrInverse ('/' pathEltOrInverse)*
    ;

pathElt
    : pathPrimary pathMod?
    ;

pathEltOrInverse
    : INVERSE? pathElt
    ;

pathMod
    : '?'| '*' | '+' |  ('{' pathQuantity '}' )
    ;

pathQuantity: pathQuantityExact | pathQuantityRange | pathQuantityMin | pathQuantityMax;

pathQuantityExact: INTEGER;

pathQuantityRange: min=INTEGER ',' max=INTEGER;

pathQuantityMin: min=INTEGER ',';

pathQuantityMax: ',' max=INTEGER;

pathPrimary
    : iri | A | '!' pathNegatedPropertySet | '(' path ')'
    ;

pathNegatedPropertySet
    : pathOneInPropertySet | '(' (pathOneInPropertySet ('|' pathOneInPropertySet)*)? ')'
    ;

pathOneInPropertySet
    : INVERSE? (iri | A)
    ;

integer
    : INTEGER
    ;

triplesNode
    : collection | blankNodePropertyList
    ;

blankNodePropertyList
    : '[' propertyListNotEmpty ']'
    ;

triplesNodePath
    : collectionPath | blankNodePropertyListPath
    ;

blankNodePropertyListPath
    : '[' propertyListPathNotEmpty ']'
    ;

collection
    : '(' graphNode+ ')'
    ;

collectionPath
    : '(' graphNodePath+ ')'
    ;

graphNode
    : varOrTerm | triplesNode
    ;

graphNodePath
    : varOrTerm | triplesNodePath
    ;

varOrTerm
    : var | graphTerm
    ;

varOrIRI
    : var | iri
    ;

var
    : VAR1 | VAR2
    ;

graphTerm
    : iri | rdfLiteral | numericLiteral | booleanLiteral | blankNode | nil
    ;

nil
    : '(' ')'
    ;

expression
    : conditionalOrExpression
    ;

conditionalOrExpression
    : conditionalAndExpression ('||' conditionalAndExpression)*
    ;

conditionalAndExpression
    : relationalExpression ('&&' relationalExpression)*
    ;

relationalExpression
    : additiveExpression (op=('='|'!='|'<'|'>'|'<='|'>=') additiveExpression)?
    | additiveExpression NOT? IN '(' expressionList? ')'
    ;

additiveExpression
    : multiplicativeExpression rhsAdditiveExpression*
    ;

rhsAdditiveExpression
    : rhsAdditiveExpressionSub (op+=('*'|'/') unaryExpression)*
    ;

rhsAdditiveExpressionSub
    : ('+'|'-') multiplicativeExpression
    | numericLiteralPositive
    | numericLiteralNegative
    ;

multiplicativeExpression
    : unaryExpression (op+=('*'|'/') unaryExpression)*
    ;

unaryExpression
    : op=('!'|'+'|'-')? primaryExpression
    ;

primaryExpression
    : '(' expression ')' | builtInCall | iriOrFunction | rdfLiteral | numericLiteral | booleanLiteral | var
    ;

builtInCall
    : aggregate
    | STR '(' expression ')'
    | LANG '(' expression ')'
    | LANGMATCHES '(' expression ',' expression ')'
    | DATATYPE '(' expression ')'
    | BOUND '(' var ')'
    | IRI '(' expression ')'
    | URI '(' expression ')'
    | BNODE '(' expression? ')'
    | RAND '(' ')'
    | ABS '(' expression ')'
    | CEIL '(' expression ')'
    | FLOOR '(' expression ')'
    | ROUND '(' expression ')'
    | CONCAT '(' expressionList? ')'
    | subStringExpression
    | STRLEN '(' expression ')'
    | strReplaceExpression
    | UCASE '(' expression ')'
    | LCASE '(' expression ')'
    | ENCODE_FOR_URI '(' expression ')'
    | CONTAINS '(' expression ',' expression ')'
    | STRSTARTS '(' expression ',' expression ')'
    | STRENDS '(' expression ',' expression ')'
    | STRBEFORE '(' expression ',' expression ')'
    | STRAFTER '(' expression ',' expression ')'
    | YEAR '(' expression ')'
    | MONTH '(' expression ')'
    | DAY '(' expression ')'
    | HOURS '(' expression ')'
    | MINUTES '(' expression ')'
    | SECONDS '(' expression ')'
    | TIMEZONE '(' expression ')'
    | TZ '(' expression ')'
    | NOW '(' ')'
    | UUID '(' ')'
    | STRUUID '(' ')'
    | MD5 '(' expression ')'
    | SHA1 '(' expression ')'
    | SHA256 '(' expression ')'
    | SHA384 '(' expression ')'
    | SHA512 '(' expression ')'
    | COALESCE '(' expressionList? ')'
    | IF '(' expression ',' expression ',' expression ')'
    | STRLANG '(' expression ',' expression ')'
    | STRDT '(' expression ',' expression ')'
    | SAMETERM '(' expression ',' expression ')'
    | ISIRI '(' expression ')'
    | ISURI '(' expression ')'
    | ISBLANK '(' expression ')'
    | ISLITERAL '(' expression ')'
    | ISNUMERIC '(' expression ')'
    | regexExpression
    | existsFunction
    | notExistsFunction
    ;

procedure:          ALPHANUMERIC_IDENTIFIER '(' procedureArguments? ')' AS procedureBindings;
procedureArguments: expression (',' expression)*;
procedureBindings:  var | '(' var (',' var)* ')';

regexExpression
    : REGEX '(' expression ',' expression (',' expression)? ')'
    ;

subStringExpression
    : SUBSTR '(' expression ',' expression (',' expression)? ')'
    ;

strReplaceExpression
    : REPLACE '(' expression ',' expression ',' expression (',' expression)? ')'
    ;

existsFunction
    : EXISTS groupGraphPattern
    ;

notExistsFunction
    : NOT EXISTS groupGraphPattern
    ;

aggregate
    : COUNT '(' DISTINCT? (ASTERISK | expression) ')'
    | SUM '(' DISTINCT? expression ')'
    | MIN '(' DISTINCT? expression ')'
    | MAX '(' DISTINCT? expression ')'
    | AVG '(' DISTINCT? expression ')'
    | SAMPLE '(' DISTINCT? expression ')'
    | GROUP_CONCAT '(' DISTINCT? expression (';' SEPARATOR '=' string)? ')'
    ;

iriOrFunction
    : iri argList?
    ;

rdfLiteral
    : string (LANGTAG | ('^^' iri))?
    ;

numericLiteral
    : numericLiteralUnsigned | numericLiteralPositive | numericLiteralNegative
    ;

numericLiteralUnsigned
    : INTEGER | DECIMAL | DOUBLE
    ;

numericLiteralPositive
    : INTEGER_POSITIVE | DECIMAL_POSITIVE | DOUBLE_POSITIVE
    ;

numericLiteralNegative
    : INTEGER_NEGATIVE | DECIMAL_NEGATIVE | DOUBLE_NEGATIVE
    ;

booleanLiteral
    : K_TRUE | K_FALSE
    ;

string
    : STRING_LITERAL1 | STRING_LITERAL2 | STRING_LITERAL_LONG1 | STRING_LITERAL_LONG2
    ;

iri
    : IRIREF | prefixedName
    ;

prefixedName
    : PNAME_LN | PNAME_NS
    ;

blankNode
    : BLANK_NODE_LABEL | anon
    ;

anon
    : '[' ']'
    ;
// $>


[SPARQL Deviations](../../README.md)
================================================================================
This documents the differences between our implementation and the SPARQL 1.1 Standard.



Table of Contents
================================================================================
- [Case Sensitivity of Language Tags](#case-sensitivity-of-language-tags)
- [Representation of Numeric Types](#representation-of-numeric-types)
- [Representation of Booleans](#representation-of-booleans)
- [Regular Expressions](#regular-expressions)



[Case Sensitivity of Language Tags](#sparql-deviations)
================================================================================
The standard specifies that language tag comparison is case insensitive.\
However our implementation for index scans, joins, etc, is case sensitive.\
The reason for that is that those operations work at the ObjectId level, comparing\
Ids directly for performance reasons. The alternative would be to normalize all\
language tags (to lower-case for example), however in that case we would loose\
information about the case of language tags.\
This means the following strings are different when operating at the ObjectId level:
```
"asdf"@en
"asdf"@eN
"asdf"@En
"asdf"@EN
```
At the expressions level, however, they are equivalent.



[Representation of Numeric Types](#sparql-deviations)
================================================================================
In the SPARQL standard an exact lexical representation of numeric types is used.\
In our implementation we use special encodings for known datatypes and do not store\
the exact strings. That means the following are identical in our implementation:
```
"1"^^xsd:integer
"01"^^xsd:integer
"+1"^^xsd:integer
```
```
"1"^^xsd:double
"01"^^xsd:double
"+1"^^xsd:double
"1.00"^^xsd:double
```
When printing them, applying STR(), etc, the original exact representation is lost.\
Applying `STR()` to any of those values would yield `"1"`.\
Also, for operators such as DISTINCT, those are identical values.\
This happens with all numeric types.



[Representation of Booleans](#sparql-deviations)
================================================================================
In our implementation the following are equivalent:
```
false
"0"^^xsd:boolean
"false"^^xsd:boolean
```
They are all represented as `"false"^^xsd:boolean.`\
The same happens for `true`:
```
true
"1"^^xsd:boolean
"true"^^xsd:boolean
```
They are all represented as `"true"^^xsd:boolean.`



[Regular Expressions](#sparql-deviations)
================================================================================
Our implementation supports `ECMAScript` regular expressions and we only support the `i` flag currently.\
The SPARQL standard uses `Perl` regular expressions.

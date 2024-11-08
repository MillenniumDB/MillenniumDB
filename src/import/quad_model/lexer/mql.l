%option noyywrap
%option nounput
%option noinput
%option nounistd
%{
#include "lex.yy.h"
#include "lexer.h"
%}
COLON         [\:]
L_ARROW       "<-"
R_ARROW       "->"
IMPLICIT      [@]+
TRUE          "true"
FALSE         "false"
STRING        \"([^"\\]|\\.)*\"
TYPED_STRING  [a-zA-Z]+[ \r\t]*\([ \r\t]*\"([^"\\]|\\.)*\"[ \r\t]*\)
IDENTIFIER    [a-zA-Z][a-zA-Z0-9_]*
ANON          "_a"[1-9][0-9]*
INTEGER       [-+]?[0-9]+
FLOAT         [-+]?([0-9]*[.])?[0-9]+([eE][-+]?[0-9]+)?
WHITESPACE    [ \r\t]+
ENDLINE       [\n]
UNRECOGNIZED  .
%%
{COLON}         { return Import::Token::COLON; }
{L_ARROW}       { return Import::Token::L_ARROW; }
{R_ARROW}       { return Import::Token::R_ARROW; }
{IMPLICIT}      { return Import::Token::IMPLICIT; }
{TRUE}          { return Import::Token::TRUE; }
{FALSE}         { return Import::Token::FALSE; }
{STRING}        { return Import::Token::STRING; }
{TYPED_STRING}  { return Import::Token::TYPED_STRING; }
{IDENTIFIER}    { return Import::Token::IDENTIFIER; }
{ANON}          { return Import::Token::ANON; }
{INTEGER}       { return Import::Token::INTEGER; }
{FLOAT}         { return Import::Token::FLOAT; }
{WHITESPACE}    { return Import::Token::WHITESPACE; }
{ENDLINE}       { return Import::Token::ENDLINE; }
{UNRECOGNIZED}  { return Import::Token::UNRECOGNIZED; }
<<EOF>>         { return Import::Token::END_OF_FILE; }
%%

void Import::Lexer::begin(const std::string& file) {
    yyin = fopen(file.c_str(), "r");
}

int Import::Lexer::next_token() {
    int res = yylex();
    str = yytext;
    str_len = yyleng;
    return res;
}

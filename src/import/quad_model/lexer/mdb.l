%option noyywrap
%option nounput
%{
#include "lexer.h"
%}
COLON         [\:]
L_ARROW       "<-"
R_ARROW       "->"
IMPLICIT      [@]+
TRUE          "true"
FALSE         "false"
STRING        \"([^"\\]|\\.)*\"
IDENTIFIER    [a-zA-Z][a-zA-Z0-9_]*
IRI           <([^><\"{}\^\\\|`\x00-\x20])*>
ANON          "_a"[1-9][0-9]*
INTEGER       [-+]?[0-9]+
FLOAT         [-+]?([0-9]*[.])?[0-9]+([eE][-+]?[0-9]+)?
WHITESPACE    [ \r\t]+
ENDLINE       [\n<<EOF>>]
UNRECOGNIZED  .
%%
{COLON}         { return Import::Token::COLON; }
{L_ARROW}       { return Import::Token::L_ARROW; }
{R_ARROW}       { return Import::Token::R_ARROW; }
{IMPLICIT}      { return Import::Token::IMPLICIT; }
{TRUE}          { return Import::Token::TRUE; }
{FALSE}         { return Import::Token::FALSE; }
{STRING}        { return Import::Token::STRING; }
{IDENTIFIER}    { return Import::Token::IDENTIFIER; }
{IRI}           { return Import::Token::IRI; }
{ANON}          { return Import::Token::ANON; }
{INTEGER}       { return Import::Token::INTEGER; }
{FLOAT}         { return Import::Token::FLOAT; }
{WHITESPACE}    { return Import::Token::WHITESPACE; }
{ENDLINE}       { return Import::Token::ENDLINE; }
{UNRECOGNIZED}  { return Import::Token::UNRECOGNIZED; }
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

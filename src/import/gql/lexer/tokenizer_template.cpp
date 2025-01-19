// re2c tokenizer_template.cpp -o tokenizer.cc
#include <cstring>

#include "import/gql/lexer/token.h"
#include "import/gql/lexer/tokenizer.h"
#include "system/string_manager.h"

/*!max:re2c*/
static const size_t GQLTokenizer_SIZE = StringManager::STRING_BLOCK_SIZE;

GQLTokenizer::GQLTokenizer() :
    _str(new char[GQLTokenizer_SIZE + YYMAXFILL]),
    buf(new char[GQLTokenizer_SIZE + YYMAXFILL]),
    lim(buf + GQLTokenizer_SIZE),
    cur(lim),
    mar(lim),
    token_start(lim),
    eof(false)
{ }

GQLTokenizer::~GQLTokenizer()
{
    delete[] buf;
    delete[] str;
}

void GQLTokenizer::begin(MDBIstream& in)
{
    this->in = &in;
}

bool GQLTokenizer::fill(size_t need)
{
    if (eof) {
        return false;
    }
    const size_t free = token_start - buf;
    if (free < need) {
        return false;
    }
    memmove(buf, token_start, lim - token_start);
    lim -= free;
    cur -= free;
    mar -= free;
    token_start -= free;
    // std::cout << "trying to read " << free << " bytes\n";
    auto r = in->read(lim, free);
    // std::cout << "read " << r << " bytes\n";
    lim += r;

    if (lim < buf + GQLTokenizer_SIZE) {
        if (r == 0)
            return false;
        eof = true;
        memset(lim, 0, YYMAXFILL);
        lim += YYMAXFILL - 1;
    }
    return true;
}

Import::GQL::Token GQLTokenizer::get_token()
{
    auto res = _get_token();

    str_len = cur - token_start;

    std::memcpy(_str, token_start, str_len);
    _str[str_len] = '\0';

    str = _str;

    return res;
}

Import::GQL::Token GQLTokenizer::_get_token()
{
    token_start = cur;
    /*!re2c
            re2c:yyfill:enable = 1;
            re2c:define:YYCTYPE = char;
            re2c:define:YYCURSOR = cur;
            re2c:define:YYMARKER = mar;
            re2c:define:YYLIMIT = lim;
            re2c:define:YYFILL = "if (!fill(@@)) return Import::GQL::Token::END_OF_FILE;";
            re2c:define:YYFILL:naked = 1;

            colon        = [:];
            l_arrow      = [<][-];
            r_arrow      = [-][>];
            undirected   = [~];
            true         = "true";
            false        = "false";
            string       = ["] ([^"\\] | [\\][^])* ["];
            typed_string = [a-zA-Z]+ [ \r\t]* [(] [ \r\t]* ["] ([^"\\] | [\\][^])* ["] [ \r\t]* [)];
            identifier   = [a-zA-z][a-zA-Z0-9_]*;
            integer      = [-+]?[0-9]+;
            decimal      = [-+]?([0-9]*[.])?[0-9]+([eE][-+]?[0-9]+)?;
            whitespace   = [ \r\t]+;
            endline      = [\n];

            colon        { return Import::GQL::Token::COLON; }
            l_arrow      { return Import::GQL::Token::L_ARROW; }
            r_arrow      { return Import::GQL::Token::R_ARROW; }
            undirected   { return Import::GQL::Token::UNDIRECTED; }
            true         { return Import::GQL::Token::TRUE; }
            false        { return Import::GQL::Token::FALSE; }
            string       { return Import::GQL::Token::STRING; }
            typed_string { return Import::GQL::Token::TYPED_STRING; }
            identifier   { return Import::GQL::Token::IDENTIFIER; }
            integer      { return Import::GQL::Token::INTEGER; }
            decimal      { return Import::GQL::Token::FLOAT; }
            whitespace   { return Import::GQL::Token::WHITESPACE; }
            endline      { return Import::GQL::Token::ENDLINE; }
            *            { return Import::GQL::Token::UNRECOGNIZED; }
        */
    return Import::GQL::Token::END_OF_FILE;
}

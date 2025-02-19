// re2c tokenizer_template.cpp -o tokenizer.cc
#include <cstring>

#include "import/quad_model/lexer/token.h"
#include "import/quad_model/lexer/tokenizer.h"
#include "system/string_manager.h"

/*!max:re2c*/
constexpr size_t MQLTokenizer_SIZE = StringManager::MAX_STRING_SIZE;

MQLTokenizer::MQLTokenizer() :
    _str(new char[MQLTokenizer_SIZE + YYMAXFILL]),
    buf(new char[MQLTokenizer_SIZE + YYMAXFILL]),
    lim(buf + MQLTokenizer_SIZE),
    cur(lim),
    mar(lim),
    token_start(lim),
    eof(false)
{ }

MQLTokenizer::~MQLTokenizer()
{
    delete[] buf;
    delete[] str;
}

void MQLTokenizer::begin(MDBIstream& in)
{
    this->in = &in;
}

bool MQLTokenizer::fill(size_t need)
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

    if (lim < buf + MQLTokenizer_SIZE) {
        if (r == 0)
            return false;
        eof = true;
        memset(lim, 0, YYMAXFILL);
        lim += YYMAXFILL - 1;
    }
    return true;
}

Import::QuadModel::Token MQLTokenizer::get_token()
{
    auto res = _get_token();

    str_len = cur - token_start;

    std::memcpy(_str, token_start, str_len);
    _str[str_len] = '\0';

    str = _str;

    return res;
}

Import::QuadModel::Token MQLTokenizer::_get_token()
{
    token_start = cur;
    /*!re2c
            re2c:yyfill:enable = 1;
            re2c:define:YYCTYPE = char;
            re2c:define:YYCURSOR = cur;
            re2c:define:YYMARKER = mar;
            re2c:define:YYLIMIT = lim;
            re2c:define:YYFILL = "if (!fill(@@)) return Import::QuadModel::Token::END_OF_FILE;";
            re2c:define:YYFILL:naked = 1;

            colon        = [:];
            l_arrow      = [<][-];
            r_arrow      = [-][>];
            true         = "true";
            false        = "false";
            string       = ["] ([^"\\] | [\\][^])* ["];
            typed_string = [a-zA-Z]+ [ \r\t]* [(] [ \r\t]* ["] ([^"\\] | [\\][^])* ["] [ \r\t]* [)];
            identifier   = [a-zA-z][a-zA-Z0-9_]*;
            integer      = [-+]?[0-9]+;
            decimal      = [-+]?([0-9]*[.])?[0-9]+([eE][-+]?[0-9]+)?;
            whitespace   = [ \r\t]+;
            endline      = [\n];

            colon        { return Import::QuadModel::Token::COLON; }
            l_arrow      { return Import::QuadModel::Token::L_ARROW; }
            r_arrow      { return Import::QuadModel::Token::R_ARROW; }
            true         { return Import::QuadModel::Token::TRUE; }
            false        { return Import::QuadModel::Token::FALSE; }
            string       { return Import::QuadModel::Token::STRING; }
            typed_string { return Import::QuadModel::Token::TYPED_STRING; }
            identifier   { return Import::QuadModel::Token::IDENTIFIER; }
            integer      { return Import::QuadModel::Token::INTEGER; }
            decimal      { return Import::QuadModel::Token::FLOAT; }
            whitespace   { return Import::QuadModel::Token::WHITESPACE; }
            endline      { return Import::QuadModel::Token::ENDLINE; }
            *            { return Import::QuadModel::Token::UNRECOGNIZED; }
        */
    return Import::QuadModel::Token::END_OF_FILE;
}

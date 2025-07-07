// re2c tokenizer_template.cpp -o tokenizer.cc
#include <cstring>

#include "import/gql/csv/lexer/token.h"
#include "import/gql/csv/lexer/tokenizer.h"
#include "system/string_manager.h"

using namespace Import::GQL::CSV;

/*!max:re2c*/
constexpr size_t CSVTokenizer_SIZE = StringManager::MAX_STRING_SIZE;

CSVTokenizer::CSVTokenizer() :
    _str(new char[CSVTokenizer_SIZE + YYMAXFILL]),
    buf(new char[CSVTokenizer_SIZE + YYMAXFILL])
{ }

CSVTokenizer::~CSVTokenizer()
{
    delete[] buf;
    delete[] str;
}

void CSVTokenizer::begin(MDBIstream& in)
{
    this->in = &in;
    this->eof = false;
    lim = buf + CSVTokenizer_SIZE;
    cur = lim;
    mar = lim;
    token_start = lim;
}

bool CSVTokenizer::fill(size_t need)
{
    if (eof)
        return false;

    const size_t free = token_start - buf;
    if (free < need)
        return false;

    if (free + (lim - cur) < need)
        return false;

    memmove(buf, token_start, lim - token_start);
    lim -= free;
    cur -= free;
    mar -= free;
    token_start -= free;

    auto r = in->read(lim, free);
    lim += r;

    if (lim < buf + CSVTokenizer_SIZE) {
        if (r == 0)
            return false;
        eof = true;
        memset(lim, 9, YYMAXFILL);
        lim += YYMAXFILL - 1;
    }
    return true;
}

Token CSVTokenizer::get_token()
{
    auto res = _get_token();
    str_len = cur - token_start;

    std::memcpy(_str, token_start, str_len);
    _str[str_len] = '\0';
    str = _str;
    return res;
}

Token CSVTokenizer::_get_token()
{
    token_start = cur;
    /*!re2c
      re2c:yyfill:enable = 1;
      re2c:define:YYCTYPE = char;
      re2c:define:YYCURSOR = cur;
      re2c:define:YYMARKER = mar;
      re2c:define:YYLIMIT = lim;
      re2c:define:YYFILL = "if (!fill(@@)) return Token::END_OF_FILE;";
      re2c:define:YYFILL:naked = 1;

      sep_column      = [,];
      string          = ["] ([^"\r\n] | ["]["])* ["];
      integer         = [-+]?[0-9]+;
      float           = [-+]?([0-9]*[.])?[0-9]+([eE][-+]?[0-9]+)?;
      endline         = [\r]?[\n];
      unquoted_string = ([^",\r\n])+;

      sep_column      { return Token::SEP_COLUMN; }
      string          { return Token::STRING; }
      integer         { return Token::INTEGER; }
      float           { return Token::FLOAT; }
      endline         { return Token::ENDLINE; }
      unquoted_string { return Token::UNQUOTED_STRING; }
      *               { return Token::UNRECOGNIZED; }
  */
    return Token::END_OF_FILE;
}

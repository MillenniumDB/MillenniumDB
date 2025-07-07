// re2c tokenizer_template.cpp -o tokenizer.cc

#include "tokenizer.h"
#include <cstring>
#include <fstream>


/*!max:re2c*/
constexpr size_t XMLTokenizer_SIZE = 1024 * 1024 * 64;

using namespace Import::Rdf::XML;

XMLTokenizer::XMLTokenizer() :
    _str (new char[XMLTokenizer_SIZE + YYMAXFILL]),
    buf  (new char[XMLTokenizer_SIZE + YYMAXFILL])
{ }

XMLTokenizer::~XMLTokenizer() {
    delete[] str;
    delete[] buf;
}

void XMLTokenizer::begin(MDBIstream& in) {
    this->in = &in;
    eof = false;
    lim = buf + XMLTokenizer_SIZE;
    cur = lim;
    mar = lim;
    token_start = lim;
}

bool XMLTokenizer::fill(size_t need)
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

    if (lim < buf + XMLTokenizer_SIZE) {
        if (r == 0)
            return false;
        eof = true;
        memset(lim, 9, YYMAXFILL);
        lim += YYMAXFILL - 1;
    }
    return true;
}

Token XMLTokenizer::get_token() {
    auto res = _get_token();
    str_len = cur - token_start;

    std::memcpy(_str, token_start, str_len);
    _str[str_len] = '\0';
    str = _str;
    return res;
}

Token XMLTokenizer::_get_token() {
    token_start = cur;
    /*!re2c
        re2c:yyfill:enable = 1;
        re2c:define:YYCTYPE = char;
        re2c:define:YYCURSOR = cur;
        re2c:define:YYMARKER = mar;
        re2c:define:YYLIMIT = lim;
        re2c:define:YYFILL = "if (!fill(@@)) return Token::END_OF_FILE;";
        re2c:define:YYFILL:naked = 1;

        comment          = '<!--' ([^-] | '-' [^->] | '--' [^>])* '-->';
        self_closing_tag = "<" [^>/]+ "/>";
        xml_metadata     = "<?" [^?]* "?>";
        closing_tag      = "</" [^>]+ ">";
        opening_tag      = "<" [^>/]+ [^>]* ">";
        tag_content      = (([^<\n \r\t]+)* [ \r\t]* ([^<\n \r\t]+)+)+;
        whitespace       = [ \r\t]+;
        endline          = [\n];

        comment          { return Token::COMMENT; }
        self_closing_tag { return Token::SELF_CLOSING_TAG; }
        xml_metadata     { return Token::XML_METADATA; }
        closing_tag      { return Token::CLOSING_TAG; }
        opening_tag      { return Token::OPENING_TAG; }
        tag_content      { return Token::TAG_CONTENT; }
        whitespace       { return Token::WHITESPACE; }
        endline          { return Token::ENDLINE; }
        *                { return Token::UNRECOGNIZED; }
    */
    return Token::END_OF_FILE;
}

#pragma once

#include "misc/istream.h"
#include "token.h"

#include <cstdio>
#include <fstream>

namespace Import { namespace Rdf { namespace XML {

class XMLTokenizer {
public:
    char* str;
    size_t str_len;
    MDBIstream* in;

    XMLTokenizer();
    ~XMLTokenizer();

    void begin(MDBIstream& in);
    Token get_token();

private:
    char* const _str;
    char* buf;
    char* lim;
    char* cur;
    char* mar;
    char* token_start;

    bool eof;

    Token _get_token();

    bool fill(size_t need);
};

}}}

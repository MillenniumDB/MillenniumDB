#pragma once

#include "import/gql/lexer/token.h"
#include "misc/istream.h"

class GQLTokenizer {
public:
    char* str;

    size_t str_len;

    MDBIstream* in;

    GQLTokenizer();

    ~GQLTokenizer();

    void begin(MDBIstream& in);

    Import::GQL::Token get_token();

private:
    char* const _str;
    char* buf;
    char* lim;
    char* cur;
    char* mar;
    char* token_start;

    bool eof;

    Import::GQL::Token _get_token();

    bool fill(size_t need);
};

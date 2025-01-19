#pragma once

#include "import/quad_model/lexer/token.h"
#include "misc/istream.h"

class MQLTokenizer {
public:
    char* str;

    size_t str_len;

    MDBIstream* in;

    MQLTokenizer();

    ~MQLTokenizer();

    void begin(MDBIstream& in);

    Import::QuadModel::Token get_token();

private:
    char* const _str;
    char* buf;
    char* lim;
    char* cur;
    char* mar;
    char* token_start;

    bool eof;

    Import::QuadModel::Token _get_token();

    bool fill(size_t need);
};

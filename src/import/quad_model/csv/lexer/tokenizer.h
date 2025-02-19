#pragma once

#include "import/quad_model/csv/lexer/token.h"
#include "misc/istream.h"

namespace Import::QuadModel::CSV {
class CSVTokenizer {
public:
    char* str;
    size_t str_len;
    MDBIstream* in;
    CSVTokenizer();
    ~CSVTokenizer();
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

} // namespace Import::QuadModel::CSV

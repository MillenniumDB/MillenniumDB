#pragma once

#include <cstring>
#include <ostream>
#include <stdint.h>
#include <string>

class LiteralLanguageInlined {
public:
    char     id[6];
    uint16_t language_id;

    LiteralLanguageInlined(const char* _id, uint8_t _language_id) :
        id { _id[0], _id[1], _id[2], _id[3], _id[4], '\0' }, language_id(_language_id) { }
};

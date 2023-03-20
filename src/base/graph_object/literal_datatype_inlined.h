#pragma once

#include <cstring>
#include <ostream>
#include <stdint.h>
#include <string>

class LiteralDatatypeInlined {
public:
    char     id[6];
    uint16_t datatype_id;

    LiteralDatatypeInlined(const char* _id, uint8_t _datatype_id) :
        id { _id[0], _id[1], _id[2], _id[3], _id[4], '\0' }, datatype_id(_datatype_id) { }
};

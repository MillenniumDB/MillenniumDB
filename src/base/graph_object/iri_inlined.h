#pragma once

#include <cstring>
#include <ostream>
#include <string>
#include <stdint.h>

class IriInlined {
public:
    char    id[7];
    uint8_t prefix_id;

    IriInlined(const char* _id, uint8_t _prefix_id) :
        id { _id[0], _id[1], _id[2], _id[3], _id[4], _id[5], '\0' }, prefix_id(_prefix_id) { }
};

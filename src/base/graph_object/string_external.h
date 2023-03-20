#pragma once

#include <cstdint>

class StringExternal {
public:
    uint64_t external_id;

    StringExternal(uint64_t external_id) : external_id(external_id) { }
};

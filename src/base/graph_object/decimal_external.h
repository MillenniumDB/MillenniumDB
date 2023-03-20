#pragma once

#include <cstdint>

class DecimalExternal {
public:
    uint64_t external_id;

    DecimalExternal(uint64_t _external_id) : external_id(_external_id) { }
};

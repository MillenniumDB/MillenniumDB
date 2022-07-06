#pragma once

#include <string>
#include <ostream>

class StringManager {
public:
    static StringManager* instance;

    virtual ~StringManager() = default;

    virtual std::string get_string(uint64_t string_id) const = 0;

    virtual void print_string(std::ostream& os, uint64_t string_id) const = 0;
};

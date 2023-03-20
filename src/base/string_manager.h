#pragma once

#include <string>
#include <ostream>

class StringManager {
public:
    static StringManager* instance;

    virtual ~StringManager() = default;

    virtual std::string get_string(uint64_t string_id) const = 0;

    virtual void print_string(std::ostream& os, uint64_t string_id) const = 0;

    virtual std::string& get_prefix(uint8_t prefix_id) const = 0;

    virtual std::string& get_datatype(uint16_t datatype_id) const = 0;

    virtual std::string& get_language(uint16_t language_id) const = 0;
};

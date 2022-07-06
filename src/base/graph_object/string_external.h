#pragma once

#include "base/string_manager.h"

class StringExternal {
public:
    uint64_t external_id;

    StringExternal(uint64_t external_id) : external_id(external_id) { }

    std::string to_string() const {
        return StringManager::instance->get_string(external_id);
    }

    void print(std::ostream& os) const {
        return StringManager::instance->print_string(os, external_id);
    }

    inline bool operator==(const StringExternal& rhs) const noexcept {
        return this->to_string() == rhs.to_string();
    }

    inline bool operator!=(const StringExternal& rhs) const noexcept {
        return this->to_string() != rhs.to_string();
    }

    inline bool operator<=(const StringExternal& rhs) const noexcept {
        return this->to_string() <= rhs.to_string();
    }

    inline bool operator>=(const StringExternal& rhs) const noexcept {
        return this->to_string() >= rhs.to_string();
    }

    inline bool operator<(const StringExternal& rhs) const noexcept {
        return this->to_string() < rhs.to_string();
    }

    inline bool operator>(const StringExternal& rhs) const noexcept {
        return this->to_string() > rhs.to_string();
    }
};

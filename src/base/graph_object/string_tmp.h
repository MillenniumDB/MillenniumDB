#pragma once

#include <string>

class StringTmp {
public:
    const std::string* str;

    StringTmp(const std::string& str) :
        str (&str) { }

    inline bool operator==(const StringTmp& rhs) const noexcept {
        return this->str == rhs.str;
    }

    inline bool operator!=(const StringTmp& rhs) const noexcept {
        return this->str != rhs.str;
    }

    inline bool operator<=(const StringTmp& rhs) const noexcept {
        return this->str <= rhs.str;
    }

    inline bool operator>=(const StringTmp& rhs) const noexcept {
        return this->str >= rhs.str;
    }

    inline bool operator<(const StringTmp& rhs) const noexcept {
        return this->str < rhs.str;
    }

    inline bool operator>(const StringTmp& rhs) const noexcept {
        return this->str > rhs.str;
    }
};

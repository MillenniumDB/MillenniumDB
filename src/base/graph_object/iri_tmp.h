#pragma once

#include <string>

class IriTmp {
public:
    const std::string* str;

    IriTmp(const std::string& str) :
        str (&str) { }

    inline bool operator==(const IriTmp& rhs) const noexcept {
        return *this->str == *rhs.str;
    }
};

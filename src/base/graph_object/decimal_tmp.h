#pragma once

#include <string>

class DecimalTmp {
public:
    const std::string* str;

    DecimalTmp(const std::string& str) :
        str (&str) { }
};

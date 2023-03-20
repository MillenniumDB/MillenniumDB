#pragma once

#include <string>

class LiteralLanguage {
public:
    std::string str;
    std::string language;

    LiteralLanguage(const std::string str, const std::string language) : str(str), language(language) { }
};
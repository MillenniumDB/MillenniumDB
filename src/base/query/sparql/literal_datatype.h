#pragma once

#include <string>

class LiteralDatatype {
public:
    std::string str;
    std::string datatype;

    LiteralDatatype(const std::string str, const std::string datatype) : str(str), datatype(datatype) { }
};
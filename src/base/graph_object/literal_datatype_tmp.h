#pragma once

#include "base/query/sparql/literal_datatype.h"

class LiteralDatatypeTmp {
public:
    const LiteralDatatype* ld;

    LiteralDatatypeTmp(const LiteralDatatype& ld) :
        ld (&ld) { }
};

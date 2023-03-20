#pragma once

#include "base/query/sparql/literal_language.h"

class LiteralLanguageTmp {
public:
    const LiteralLanguage* ll;

    LiteralLanguageTmp(const LiteralLanguage& ll) :
        ll (&ll) { }
};

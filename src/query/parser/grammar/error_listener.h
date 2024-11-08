#pragma once

#include "antlr4-runtime.h"

#include <sstream>

#include "query/exceptions.h"

namespace antlr4 {

class MyErrorListener : public BaseErrorListener {
public:
    void syntaxError(Recognizer*        /* recognizer */,
                     Token*             /* offendingSymbol */,
                     size_t             line,
                     size_t             charPositionInLine,
                     const std::string& msg,
                     std::exception_ptr /* e */) override
    {
        throw QueryParsingException(msg, line, charPositionInLine);
    }
};

} // namespace antlr4
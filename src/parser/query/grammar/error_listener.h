#include "antlr4-runtime.h"

#include <sstream>

#include "base/exceptions.h"

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
        std::stringstream ss;
        ss << "line " << line << ":" << (charPositionInLine+1) << " " << msg;
        throw QueryParsingException(ss.str());
    }
};

} // namespace antlr4
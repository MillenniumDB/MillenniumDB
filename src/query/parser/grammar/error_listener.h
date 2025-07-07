#pragma once

#include "antlr4-runtime.h"

#include "query/exceptions.h"

namespace antlr4 {

class MyErrorListener : public BaseErrorListener {
public:
    void syntaxError(
        Recognizer* /* recognizer */,
        Token* /* offendingSymbol */,
        size_t line,
        size_t charPositionInLine,
        const std::string& msg,
        std::exception_ptr /* e */
    ) override
    {
        std::string new_msg = msg;

        std::string old_token("K_TRUE");
        std::string new_token("TRUE");

        auto pos = msg.find(old_token);
        if (pos != std::string::npos) {
            new_msg.replace(pos, old_token.size(), new_token);
        }

        old_token = "K_FALSE";
        new_token = "FALSE";

        pos = msg.find(old_token, pos);
        if (pos != std::string::npos) {
            new_msg.replace(pos, old_token.size(), new_token);
        }

        throw QueryParsingException(new_msg, line, charPositionInLine);
    }
};

} // namespace antlr4

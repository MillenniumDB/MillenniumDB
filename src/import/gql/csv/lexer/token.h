#pragma once

#include <string>

namespace Import { namespace GQL { namespace CSV {

// IMPORTANT: must assign consecutive numbers for the automata to work properly
enum Token {
    // END_OF_FILE must be 0
    END_OF_FILE = 0,
    SEP_COLUMN = 1,
    STRING = 2,
    UNQUOTED_STRING = 3,
    INTEGER = 4,
    FLOAT = 5,
    ENDLINE = 6,
    UNRECOGNIZED = 7,
    TOTAL_TOKENS = 8
};

inline std::string token_to_string(Token token)
{
    switch (token) {
    case END_OF_FILE:
        return "END_OF_FILE";
    case SEP_COLUMN:
        return "SEP_COLUMN";
    case STRING:
        return "STRING";
    case UNQUOTED_STRING:
        return "UNQUOTED_STRING";
    case INTEGER:
        return "INTEGER";
    case FLOAT:
        return "FLOAT";
    case ENDLINE:
        return "ENDLINE";
    case UNRECOGNIZED:
        return "UNRECOGNIZED";
    default:
        return "Token is not valid";
    }
}

}}} // namespace Import::GQL::CSV

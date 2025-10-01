#pragma once

#include <string>

namespace Import { namespace GQL {
// IMPORTANT: must assign consecutive numbers for the automata to work properly
enum Token {
    // END_OF_FILE must be 0
    END_OF_FILE = 0,
    COLON = 1,
    L_ARROW = 2,
    R_ARROW = 3,
    UNDIRECTED = 4,
    K_TRUE = 5,
    K_FALSE = 6,
    STRING = 7,
    TYPED_STRING = 8,
    IDENTIFIER = 9,
    INTEGER = 10,
    FLOAT = 11,
    WHITESPACE = 12,
    ENDLINE = 13,
    L_BRACKET = 14,
    R_BRACKET = 15,
    COMMA = 16,
    UNRECOGNIZED = 17,
    TOTAL_TOKENS = 18
};

inline std::string token_to_string(Token token)
{
    switch (token) {
    case END_OF_FILE:
        return "END_OF_FILE";
    case COLON:
        return "COLON";
    case L_ARROW:
        return "L_ARROW";
    case R_ARROW:
        return "R_ARROW";
    case UNDIRECTED:
        return "UNDIRECTED";
    case K_TRUE:
        return "K_TRUE";
    case K_FALSE:
        return "K_FALSE";
    case STRING:
        return "STRING";
    case TYPED_STRING:
        return "TYPED_STRING";
    case IDENTIFIER:
        return "IDENTIFIER";
    case INTEGER:
        return "INTEGER";
    case FLOAT:
        return "FLOAT";
    case WHITESPACE:
        return "WHITESPACE";
    case ENDLINE:
        return "ENDLINE";
    case L_BRACKET:
        return "L_BRACKET";
    case R_BRACKET:
        return "R_BRACKET";
    case UNRECOGNIZED:
        return "UNRECOGNIZED";
    case TOTAL_TOKENS:
        return "TOTAL_TOKENS";
    case COMMA:
        return "COMMA";
    }
    return "Token is not valid";
}

}} // namespace Import::GQL

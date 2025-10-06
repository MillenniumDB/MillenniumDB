#pragma once

namespace Import { namespace QuadModel {
// IMPORTANT: must assign consecutive numbers for the automata to work properly
enum Token {
    // END_OF_FILE must be 0
    END_OF_FILE = 0,
    COLON = 1,
    L_ARROW = 2,
    R_ARROW = 3,
    IMPLICIT = 4,
    K_TRUE = 5,
    K_FALSE = 6,
    STRING = 7,
    TYPED_STRING = 8,
    IDENTIFIER = 9,
    ANON = 10,
    INTEGER = 11,
    FLOAT = 12,
    L_BRACKET = 13,
    R_BRACKET = 14,
    COMMA = 15,
    WHITESPACE = 16,
    ENDLINE = 17,
    UNRECOGNIZED = 18,
    TOTAL_TOKENS = 19
};

}} // namespace Import::QuadModel

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
    TRUE = 5,
    FALSE = 6,
    STRING = 7,
    TYPED_STRING = 8,
    IDENTIFIER = 9,
    ANON = 10,
    INTEGER = 11,
    FLOAT = 12,
    WHITESPACE = 13,
    ENDLINE = 14,
    UNRECOGNIZED = 15,
    TOTAL_TOKENS = 16
};

}} // namespace Import::QuadModel

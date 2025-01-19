#pragma once

namespace Import { namespace GQL {
// IMPORTANT: must assign consecutive numbers for the automata to work properly
enum Token {
    // END_OF_FILE must be 0
    END_OF_FILE = 0,
    COLON = 1,
    L_ARROW = 2,
    R_ARROW = 3,
    UNDIRECTED = 4,
    TRUE = 5,
    FALSE = 6,
    STRING = 7,
    TYPED_STRING = 8,
    IDENTIFIER = 9,
    INTEGER = 10,
    FLOAT = 11,
    WHITESPACE = 12,
    ENDLINE = 13,
    UNRECOGNIZED = 14,
    TOTAL_TOKENS = 15
};

}} // namespace Import::GQL

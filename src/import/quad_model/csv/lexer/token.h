#pragma once

namespace Import { namespace QuadModel { namespace CSV {

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

}}} // namespace Import::QuadModel::CSV

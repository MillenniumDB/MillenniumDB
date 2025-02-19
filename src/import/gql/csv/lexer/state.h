#pragma once

namespace Import { namespace GQL { namespace CSV {

enum State {
    // The parser will read the first column of a nodes file
    START_HEADER_NODES = 0,

    // The parser will read the first column of an edges file
    START_HEADER_EDGES = 1,

    // A new column in the header has been read in a nodes file
    COLUMN_NAME_READ_NODES = 2,

    // A new column in the header has been read in an edges file
    COLUMN_NAME_READ_EDGES = 3,

    // A new line will be read. This is the CSV body of a nodes file
    START_BODY_NODES = 4,

    // The column belongs to a nodes file
    COLUMN_BODY_NODES_READ = 5,

    // A new line will be read. This is the CSV body of an edges file
    START_BODY_EDGES = 6,

    // The column belongs to an edges file
    COLUMN_BODY_EDGES_READ = 7,

    // The line has ended in a nodes file
    END_LINE_NODES = 8,

    // The line has ended in an edges file
    END_LINE_EDGES = 9,

    // Error. Something bad happened
    ERROR = 10,

    TOTAL_STATES = 11
};
}}} // namespace Import::GQL::CSV

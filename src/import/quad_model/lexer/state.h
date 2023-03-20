#pragma once

namespace Import {

enum State {
    // The line is wrong and will be discarded.
    WRONG_LINE = 0,

    // Nothing is read yet.
    LINE_BEGIN = 1,

    // An ID was read. We don't know yet if its a node line or an edge line
    FIRST_ID = 2,

    // We know it's a node line. Labels or properties were read. The line might contain other label or property.
    NODE_DEFINED = 3,

    // ':' was read and we know we are working with a node line, we expect the label
    EXPECT_NODE_LABEL = 4,

    // A key property were read in a node line, we expect the ':'
    EXPECT_NODE_PROP_COLON = 5,

    // A key property and the ':' were read in a node line, we expect the value
    EXPECT_NODE_PROP_VALUE = 6,

    // "@+" was read, we know it's a edge line. We expect a direction ("->" or "<-")
    IMPLICIT_EDGE = 7,

    // "->" or "<-" were read, we know we are in an edge and we expect the second id
    EXPECT_EDGE_SECOND = 8,

    // something like "ID1->ID2" was read, and we expect the colon preceding the edge type
    EXPECT_EDGE_TYPE_COLON = 9,

    // something like "ID1->ID2 :" was read, and we expect the edge type
    EXPECT_EDGE_TYPE = 10,

    // We are in a edge line, it might follow a property or a line terminator.
    EDGE_DEFINED = 11,

    // A key property were read in an edge line, we expect the ':'
    EXPECT_EDGE_PROP_COLON = 12,

    // A key property and the ':' were read in an edge line, we expect the value
    EXPECT_EDGE_PROP_VALUE = 13,

    TOTAL_STATES = 14
};
} // namespace Import

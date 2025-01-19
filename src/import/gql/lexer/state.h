#pragma once

namespace Import { namespace GQL {

// IMPORTANT: must assign consecutive numbers
enum State {
    // The line is wrong and will be discarded.
    WRONG_LINE = 0,

    // Nothing is read yet.
    LINE_BEGIN = 1,

    // An ID was read. We don't know yet if its a node line or an edge line
    FIRST_ID = 2,

    // We know it's a node line. Labels or properties were read.
    // The line might contain other label or property.
    NODE_DEFINED = 3,

    // ':' was read and we know we are working with a node line, we expect the label
    EXPECT_NODE_LABEL = 4,

    // A key property were read in a node line, we expect the ':'
    EXPECT_NODE_PROP_COLON = 5,

    // A key property and the ':' were read in a node line, we expect the value
    EXPECT_NODE_PROP_VALUE = 6,

    // "~", "->" or "<-" were read, we know we are in an edge and we expect the second id
    EXPECT_EDGE_SECOND = 7,

    // We are in a edge line, it might follow a property or a line terminator.
    EDGE_DEFINED = 8,

    // ':' was read and we know we are working with an edge line, we expect the label
    EXPECT_EDGE_LABEL = 9,

    // A key property were read in an edge line, we expect the ':'
    EXPECT_EDGE_PROP_COLON = 10,

    // A key property and the ':' were read in an edge line, we expect the value
    EXPECT_EDGE_PROP_VALUE = 11,

    TOTAL_STATES = 12
};
}} // namespace Import::GQL

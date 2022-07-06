#pragma once

namespace Import {

enum State {
    // La linea esta mal, se descarta la linea entera
    WRONG_LINE = 0,

    // Aún no se lee nada en la linea actual
    LINE_BEGIN = 1,

    // We don't know yet if its a NODE_LINE OR EDGE_LINE
    FIRST_ID = 2,

    // se sabe que es un nodo, se leyeron labels o properties y se volvio atrás (no puede volver a FIRST_ID)
    NODE_DEFINED = 3,

    // se leyó `:` despues de NODE_LINE
    EXPECT_NODE_LABEL = 4,

    // se leyó el key de un property
    EXPECT_NODE_PROP_COLON = 5,

    // se leyó el key y el ':'
    EXPECT_NODE_PROP_VALUE = 6,

    // se leyó @+
    IMPLICIT_EDGE = 7,

    // se leyó -> o <-
    EXPECT_EDGE_SECOND = 8,

    // se espera ':'
    EXPECT_EDGE_TYPE_COLON = 9,

    // se leyó `a->b :`
    EXPECT_EDGE_TYPE = 10,

    // se leyó el edge con el type, tal vez se leyeron properties
    EDGE_DEFINED = 11,

    //
    EXPECT_EDGE_PROP_COLON = 12,

    //
    EXPECT_EDGE_PROP_VALUE = 13,

    TOTAL_STATES = 14
};
} // namespace Import

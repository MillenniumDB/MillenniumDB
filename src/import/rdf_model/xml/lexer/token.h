#pragma once

namespace Import { namespace Rdf { namespace XML {

enum Token {
    END_OF_FILE = 0,
    XML_METADATA,
    OPENING_TAG,
    CLOSING_TAG,
    SELF_CLOSING_TAG,
    TAG_CONTENT,
    COMMENT,
    WHITESPACE,
    ENDLINE,
    UNRECOGNIZED,
    TOTAL_TOKENS
};

}}}


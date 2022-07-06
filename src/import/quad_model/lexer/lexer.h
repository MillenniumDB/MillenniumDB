#pragma once

#include <cstdio>
#include <string>

namespace Import {
enum Token {
  // must skip 0, it represents there are no more tokens
  COLON        = 1,
  L_ARROW      = 2,
  R_ARROW      = 3,
  IMPLICIT     = 4,
  TRUE         = 5,
  FALSE        = 6,
  STRING       = 7,
  IDENTIFIER   = 8,
  IRI          = 9,
  ANON         = 10,
  INTEGER      = 11,
  FLOAT        = 12,
  WHITESPACE   = 13,
  ENDLINE      = 14,
  UNRECOGNIZED = 15,
  TOTAL_TOKENS = 16
};

class Lexer {
public:
    void begin(const std::string& filename);
    int next_token();

    char* str;
    size_t str_len;
};
} // namespace Import

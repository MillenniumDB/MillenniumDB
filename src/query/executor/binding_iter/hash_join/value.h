#pragma once

#include <cstdint>

namespace HashJoin {
/*
Value represents a pair of pointers of a linked list, it is used as value
in hash table. See docs/hash.md to know how values are stored in hash table.
*/
struct Value {
    uint64_t* head;
    uint64_t* tail;

    Value(uint64_t* head, uint64_t* tail) :
        head(head),
        tail(tail) {};
};

} // namespace HashJoin

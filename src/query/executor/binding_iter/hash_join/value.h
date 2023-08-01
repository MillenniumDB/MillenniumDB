#pragma once

#include <memory>
#include <vector>

#include "query/var_id.h"
#include "query/executor/binding_iter.h"
#include "third_party/robin_hood/robin_hood.h"


/*
Value represents a pair of pointers of a linked list, it is used as value
in hash table. See docs/hash.md to know how values are stored in hash table.
*/


namespace HashJoin {
struct Value {
    uint64_t* head;
    uint64_t* tail;

    Value(uint64_t* head, uint64_t* tail) :
        head   (head),
        tail   (tail) { };
};

}

#pragma once

#include <memory>
#include <string>
#include <vector>

#include "base/binding/binding_id.h"
#include "base/ids/object_id.h"
#include "base/ids/var_id.h"
#include "storage/page.h"

class TupleBufferBlock {
public:
    TupleBufferBlock(TmpFileId file_id, uint_fast32_t page_number, const std::vector<VarId>& enumeration_vars);
    ~TupleBufferBlock();

    inline bool is_full()         const noexcept { return *tuple_count == max_tuples; }
    inline auto get_tuple_count() const noexcept { return *tuple_count; }
    inline auto get_page_number() const noexcept { return page.get_page_number(); }
    inline void reset()                 noexcept { *tuple_count = 0; }

    void append_tuple(const std::vector<ObjectId>& tuple);

    // undefined behaviour in case pos is of out-of-bounds, should check before using it
    void assign_to_binding(BindingId& binding, uint_fast32_t pos_in_block);

    static constexpr uint_fast32_t get_max_tuples(const uint_fast32_t tuple_size) {
        if (tuple_size == 0) {
            return UINT32_MAX;
        } else {
            const auto bytes_avalilables = Page::MDB_PAGE_SIZE - sizeof(*tuple_count);
            return bytes_avalilables / (tuple_size * sizeof(uint64_t));
        }
    }

private:
    Page& page;
    const uint_fast32_t tuple_size;
    const std::vector<VarId>& enumeration_vars;

    const uint_fast32_t max_tuples;
    uint64_t* const tuples;
    uint32_t* const tuple_count;
};

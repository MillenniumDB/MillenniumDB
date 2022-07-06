#pragma once

#include <memory>
#include <vector>

#include "base/ids/object_id.h"
#include "base/binding/binding_id.h"
#include "storage/file_id.h"
#include "storage/index/tuple_buffer/tuple_buffer_block.h"

class TupleBuffer {
public:
    TupleBuffer(const std::vector<VarId>& enumeration_vars);
    ~TupleBuffer();

    inline uint_fast32_t get_tuple_count() const noexcept { return tuple_count; }

    // size of vector must be equal to `tuple_size`
    void append_tuple(std::vector<ObjectId>& tuple);

    // undefined behaviour in case pos is of out-of-bounds, must check before using it
    void assign_to_binding(BindingId& binding, uint_fast32_t pos);

    void reset();

private:
    const TmpFileId file_id;
    const std::vector<VarId>& enumeration_vars; // enumeration_vars.size() = tuple_size
    const uint_fast32_t tuple_size;             // WARNING: tuple_size can be 0
    const uint_fast32_t max_tuples;             // How many tuples fit in a page
    uint_fast32_t tuple_count = 0;

    std::unique_ptr<TupleBufferBlock> current_block;
    std::unique_ptr<TupleBufferBlock> last_block;

};

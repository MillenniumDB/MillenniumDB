#include "tuple_buffer.h"

#include "storage/file_manager.h"

using namespace std;

TupleBuffer::TupleBuffer(const vector<VarId>& enumeration_vars) :
    file_id          (file_manager.get_tmp_file_id()),
    enumeration_vars (enumeration_vars),
    tuple_size       (enumeration_vars.size()),
    max_tuples       (TupleBufferBlock::get_max_tuples(tuple_size))
{
    current_block = make_unique<TupleBufferBlock>(file_id, 0, enumeration_vars);
    last_block    = make_unique<TupleBufferBlock>(file_id, 0, enumeration_vars);
}


TupleBuffer::~TupleBuffer() {
    current_block.reset();
    last_block.reset();
    file_manager.remove_tmp(file_id);
}


void TupleBuffer::append_tuple(std::vector<ObjectId>& tuple) {
    if (last_block->is_full()) {
        const auto old_page_number = last_block->get_page_number();
        last_block = make_unique<TupleBufferBlock>(file_id, old_page_number+1, enumeration_vars);
        last_block->reset();
    }
    tuple_count++;
    last_block->append_tuple(tuple);
}


void TupleBuffer::assign_to_binding(BindingId& binding, uint_fast32_t pos) {
    auto block        = pos / max_tuples;
    auto pos_in_block = pos % max_tuples;

    if (current_block->get_page_number() != block) {
        current_block = make_unique<TupleBufferBlock>(file_id, block, enumeration_vars);
    }

    current_block->assign_to_binding(binding, pos_in_block);
}


void TupleBuffer::reset() {
    tuple_count = 0;
    current_block = make_unique<TupleBufferBlock>(file_id, 0, enumeration_vars);
    last_block    = make_unique<TupleBufferBlock>(file_id, 0, enumeration_vars);

    current_block->reset();
}

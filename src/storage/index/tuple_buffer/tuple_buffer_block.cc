#include "tuple_buffer_block.h"

#include <cassert>

#include "storage/buffer_manager.h"

using namespace std;

TupleBufferBlock::TupleBufferBlock(TmpFileId                 file_id,
                                   uint_fast32_t             page_number,
                                   const std::vector<VarId>& _enumeration_vars) :
    page             (buffer_manager.get_tmp_page(file_id, page_number)),
    tuple_size       (_enumeration_vars.size()),
    enumeration_vars (_enumeration_vars),
    max_tuples       (get_max_tuples(tuple_size)),
    tuples           (reinterpret_cast<uint64_t*>(page.get_bytes())),
    tuple_count      (reinterpret_cast<uint32_t*>(page.get_bytes() + max_tuples*tuple_size*sizeof(uint64_t)))
    { }


TupleBufferBlock::~TupleBufferBlock() {
    buffer_manager.unpin(page);
}


void TupleBufferBlock::append_tuple(const std::vector<ObjectId>& tuple) {
    for (std::size_t i = 0; i < tuple.size(); ++i) {
        tuples[ (*tuple_count) * tuple_size + i ] = tuple[i].id;
    }
    ++(*tuple_count);
    page.make_dirty();
}


void TupleBufferBlock::assign_to_binding(BindingId& binding, uint_fast32_t pos_in_block) {
    for (uint_fast32_t i = 0; i < enumeration_vars.size(); i++) {
        binding.add(enumeration_vars[i],
                    ObjectId(tuples[tuple_size*pos_in_block + i]));
    }
}

#include "external_helper.h"

#include "macros/aligned_alloc.h"
#include "misc/fatal_error.h"

using namespace Import;

ExternalHelper::ExternalHelper(
    const std::string& db_folder,
    std::size_t strings_buffer_capacity,
    std::size_t tensors_buffer_capacity
) :
    db_folder(db_folder),
    pending_buffer(reinterpret_cast<char*>(
        MDB_ALIGNED_ALLOC(std::max(StringManager::MAX_STRING_SIZE, TensorManager::MAX_TENSOR_BYTES))
    )),
    buffer_size(strings_buffer_capacity + tensors_buffer_capacity),
    buffer(reinterpret_cast<char*>(MDB_ALIGNED_ALLOC(buffer_size))),
    strings_external_data(
        db_folder + "/" + StringManager::STRINGS_FILENAME,
        db_folder + "/" + ExternalHelper::TMP_PENDING_STRINGS_FILENAME_PREFIX,
        buffer,
        strings_buffer_capacity,
        0, // no offset
        StringManager::MAX_STRING_SIZE,
        StringManager::MIN_PAGE_REMAINING_BYTES,
        StringManager::BLOCK_SIZE
    ),
    tensors_external_data(
        db_folder + "/" + TensorManager::TENSORS_FILENAME,
        db_folder + "/" + ExternalHelper::TMP_PENDING_TENSORS_FILENAME_PREFIX,
        buffer + strings_buffer_capacity,
        tensors_buffer_capacity,
        strings_buffer_capacity, // offset the string_buffer capacity
        TensorManager::MAX_TENSOR_BYTES,
        TensorManager::MIN_PAGE_REMAINING_BYTES,
        TensorManager::BLOCK_SIZE
    )
{
    if (buffer == nullptr || pending_buffer == nullptr) {
        FATAL_ERROR("Could not allocate buffer, try using smaller buffer sizes");
    }

    if (strings_external_data.buffer_capacity < strings_external_data.max_element_size) {
        FATAL_ERROR(
            "Strings buffer capacity must have at least "
            + std::to_string(strings_external_data.max_element_size) + " bytes"
        );
    }

    if (tensors_external_data.buffer_capacity < tensors_external_data.max_element_size) {
        FATAL_ERROR(
            "Tensors buffer capacity must have at least "
            + std::to_string(tensors_external_data.max_element_size) + " bytes"
        );
    }

    ExternalBytes::data = buffer;
    // ExternalBytes::size = num_bytes;
}

ExternalHelper::~ExternalHelper()
{
    MDB_ALIGNED_FREE(buffer);
    MDB_ALIGNED_FREE(pending_buffer);
}

uint64_t ExternalHelper::resolve_id(uint64_t id)
{
    assert(strings_external_data.old_pending_fs != nullptr);
    assert(tensors_external_data.old_pending_fs != nullptr);

    constexpr uint64_t ORIGINAL_DATA_MASK = ObjectId::SUB_TYPE_MASK | ObjectId::MASK_LITERAL_TAG;
    if ((id & ObjectId::MOD_MASK) != ObjectId::MOD_TMP) {
        // nothing to do
        return id;
    }

    const uint64_t mask = id & ORIGINAL_DATA_MASK;
    const uint64_t gen_t = id & ObjectId::GENERIC_TYPE_MASK;

    if (gen_t == ObjectId::MASK_TENSOR) {
        // handle tensor
        tensors_external_data.old_pending_fs->seekg(id & ObjectId::MASK_EXTERNAL_ID);
        const auto num_bytes = BytesEncoder::read_bytes(
            *tensors_external_data.old_pending_fs,
            pending_buffer
        );
        return get_or_create_external_tensor_id(pending_buffer, num_bytes) | mask;
    }

    const auto pos = id & ObjectId::MASK_EXTERNAL_ID;
    strings_external_data.old_pending_fs->seekg(pos);
    const auto str_len = BytesEncoder::read_bytes(*strings_external_data.old_pending_fs, pending_buffer);
    return get_or_create_external_string_id(pending_buffer, str_len) | mask;
}

uint64_t ExternalHelper::get_or_create_external_id(
    const char* bytes,
    std::size_t num_bytes,
    ExternalData& external_data
)
{
    // encode size
    const auto num_bytes_size = BytesEncoder::write_size(
        external_data.buffer + external_data.buffer_end,
        num_bytes
    );
    // encode bytes
    std::memcpy(external_data.buffer + external_data.buffer_end + num_bytes_size, bytes, num_bytes);

    const ExternalBytes eb = external_data.get_external_bytes(external_data.buffer_end);
    const auto found = external_data.external_bytes_set.find(eb);
    if (found != external_data.external_bytes_set.end()) {
        // bytes were already encoded
        return ObjectId::MOD_EXTERNAL | external_data.offset2id(found->offset);
    }

    if (!external_data.full()) {
        // there are space available
        external_data.external_bytes_set.emplace(eb);
        external_data.buffer_end += num_bytes_size + num_bytes;

        const auto remaining_in_block = external_data.remaining_in_block();
        if (remaining_in_block < external_data.min_page_remaining_bytes) {
            // must advance to the next page
            external_data.buffer_end += remaining_in_block;
        }

        return ObjectId::MOD_EXTERNAL | external_data.offset2id(eb.offset);
    }

    const uint64_t pos = external_data.pending_fs->tellp();
    external_data.pending_fs->write(
        external_data.buffer + external_data.buffer_end,
        num_bytes_size + num_bytes
    );
    return ObjectId::MOD_TMP | pos;
}

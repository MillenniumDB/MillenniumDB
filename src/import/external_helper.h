#pragma once

#include <boost/unordered/unordered_flat_set.hpp>
#include <cstddef>

#include "import/external_bytes.h"
#include "import/import_helper.h"
#include "storage/index/hash/strings_hash/strings_hash_bulk_ondisk_import.h"
#include "storage/index/hash/tensors_hash/tensors_hash_bulk_ondisk_import.h"
#include "system/string_manager.h"
#include "system/tensor_manager.h"

namespace Import {

// helper struct for memory/disk manipulation of string and tensor
struct ExternalData {
    static constexpr auto MIN_IMPORT_BUFFER_SIZE = 256 * 1024 * 1024; // 256 MB

    static_assert(
        MIN_IMPORT_BUFFER_SIZE > 2 * StringManager::MAX_STRING_SIZE,
        "Minimum buffer size must be greater than (2 * StringManager::MAX_STRING_SIZE)"
    );
    static_assert(
        MIN_IMPORT_BUFFER_SIZE > 2 * TensorManager::MAX_SIZE_BYTES,
        "Minimum buffer size must be greater than (2 * TensorManager::MAX_SIZE_BYTES)"
    );

    // constants
    const std::size_t max_element_size;
    const std::size_t min_page_remaining_bytes;
    const std::size_t block_size;
    const std::string pending_path_prefix;

    // buffer
    const std::size_t buffer_capacity;
    const std::size_t buffer_start_offset; // number of bytes to shift for getting its ExternalBytes
    std::size_t buffer_end { 0 };
    char* buffer;

    // disk
    const std::string data_path;
    std::size_t disk_offset { 0 };
    std::size_t disk_align_start_offset { 0 };
    std::unique_ptr<std::fstream> data_fs;

    // pending
    std::size_t pending_file_index { 0 };
    std::unique_ptr<std::fstream> pending_fs;
    std::unique_ptr<std::fstream> old_pending_fs;

    boost::unordered_flat_set<ExternalBytes, ExternalBytes::Hasher> external_bytes_set;

    explicit ExternalData(
        const std::string& data_path_,
        const std::string& pending_path_prefix_,
        char* buffer_,
        std::size_t buffer_capacity_,
        std::size_t buffer_start_offset_,
        std::size_t max_element_size_,
        std::size_t min_page_remaining_bytes_,
        std::size_t block_size_
    ) :
        max_element_size { max_element_size_ },
        min_page_remaining_bytes { min_page_remaining_bytes_ },
        block_size { block_size_ },
        pending_path_prefix { pending_path_prefix_ },
        buffer_capacity { buffer_capacity_ },
        buffer_start_offset { buffer_start_offset_ },
        buffer { buffer_ },
        data_path { data_path_ },
        data_fs { get_fstream(data_path) },
        pending_fs { get_fstream(pending_path_prefix_ + std::to_string(pending_file_index)) }
    { }

    // convert the external bytes offset to the actual id by aplying the corresponding shifts
    inline uint64_t offset2id(uint64_t offset) const
    {
        // offset must be:
        //  - shifted right by disk_offset - disk_align_start_offset (current writing position)
        //  - shifted left by buffer_start_offset (tensors buffer are shifted by strings buffer)
        return offset + disk_offset - disk_align_start_offset - buffer_start_offset;
    }

    // get the external bytes given an offset
    // notice that the buffer_start_offset is considered due the shared buffer for strings/tensors
    inline ExternalBytes get_external_bytes(std::size_t offset) const
    {
        return ExternalBytes(buffer_start_offset + offset);
    }

    // write the buffer to disk and update the next offset
    inline void flush_to_disk()
    {
        data_fs->write(buffer + disk_align_start_offset, buffer_end - disk_align_start_offset);
        disk_offset += buffer_end - disk_align_start_offset;
    }

    // store the current pending file as `old_pending_fs`, and update the state for the next process
    inline void advance_pending()
    {
        old_pending_fs = std::move(pending_fs);
        ++pending_file_index;
        pending_fs = get_fstream(pending_path_prefix + std::to_string(pending_file_index));

        external_bytes_set.clear();

        disk_align_start_offset = buffer_end % block_size;
        buffer_end = disk_align_start_offset;
    }

    // clean up the current old pending files
    inline void clean_up_old()
    {
        const auto old_pending_files_index = pending_file_index - 1;
        const auto path = pending_path_prefix + std::to_string(old_pending_files_index);

        old_pending_fs->close();
        std::remove(path.c_str());
    }

    // clean up the disk files and memory allocated by this class
    // this should be called after writing all of data_fs
    inline void clean_up()
    {
        pending_fs->close();

        const auto path = pending_path_prefix + std::to_string(pending_file_index);
        std::remove(path.c_str());

        boost::unordered_flat_set<ExternalBytes, ExternalBytes::Hasher> tmp;
        external_bytes_set.swap(tmp);

        data_fs->close();
    }

    // populates the disk hash with the data_fs
    template<typename DiskHash>
    inline void build_disk_hash(DiskHash& disk_hash, char* pending_buffer)
    {
        data_fs->open(data_path, std::ios::in | std::ios::binary);

        std::size_t current_pos = 0;
        while (current_pos < disk_offset) {
            const std::size_t remaining_in_block = block_size - (current_pos % block_size);

            if (remaining_in_block < min_page_remaining_bytes) {
                current_pos += remaining_in_block;
                data_fs->read(pending_buffer, remaining_in_block);
            }

            const auto num_bytes = BytesEncoder::read_bytes(*data_fs, pending_buffer);

            disk_hash.create_id(pending_buffer, num_bytes, current_pos);
            current_pos = data_fs->tellg();
        }

        data_fs->close();
    }

    inline bool full() const
    {
        return buffer_end + max_element_size > buffer_capacity;
    }

    inline std::size_t remaining_in_block() const
    {
        return block_size - (buffer_end % block_size);
    }
};

// utility class for handling tensor/string buffered write
class ExternalHelper {
public:
    static constexpr char TMP_PENDING_STRINGS_FILENAME_PREFIX[] = "tmp_pending_strings";
    static constexpr char TMP_PENDING_TENSORS_FILENAME_PREFIX[] = "tmp_pending_tensors";

    ExternalHelper() = delete;

    // it is assumed that the `buffer` size equals `strings_buffer_capacity + tensors_buffer_capacity`
    // the buffer is divided by this values: the strings will be written from index zero, while
    // tensors will be written from index `strings_buffer_capacity`
    ExternalHelper(
        const std::string& db_folder,
        std::size_t strings_buffer_capacity,
        std::size_t tensors_buffer_capacity
    );

    ~ExternalHelper();

    uint64_t get_or_create_external_string_id(const char* bytes, std::size_t num_bytes)
    {
        return get_or_create_external_id(bytes, num_bytes, strings_external_data);
    }

    uint64_t get_or_create_external_tensor_id(const char* bytes, std::size_t num_bytes)
    {
        return get_or_create_external_id(bytes, num_bytes, tensors_external_data);
    }

    // Resolves the id by checking if it was already processed. If there is no more buffer available triple pending will be set
    uint64_t resolve_id(uint64_t id);

    void clear_sets()
    {
        strings_external_data.external_bytes_set.clear();
        tensors_external_data.external_bytes_set.clear();
    }

    // resets the state to handle the next batch of pending data. This creates new pending files,
    // but keep the current ones as "old" for resolving ids. It also clears the sets and update offsets
    void advance_pending()
    {
        strings_external_data.advance_pending();
        tensors_external_data.advance_pending();
    }

    // close and remove from disk old pending files
    void clean_up_old()
    {
        strings_external_data.clean_up_old();
        tensors_external_data.clean_up_old();
    }

    // close and remove unused files and free up unused memory
    void clean_up()
    {
        strings_external_data.clean_up();
        tensors_external_data.clean_up();
    }

    void flush_to_disk()
    {
        strings_external_data.flush_to_disk();
        tensors_external_data.flush_to_disk();
    }

    void build_disk_hash()
    {
        {
            StringsHashBulkOnDiskImport strings_hash(db_folder + "/str_hash", buffer, buffer_size);
            strings_external_data.build_disk_hash(strings_hash, pending_buffer);
        }

        {
            TensorsHashBulkOnDiskImport tensors_hash(buffer, buffer_size);
            tensors_external_data.build_disk_hash(tensors_hash, pending_buffer);
        }
    }

    const std::string db_folder;

    // small buffer
    char* pending_buffer;

    uint64_t buffer_size;

    // big buffer (for strings and tensors)
    char* buffer;

private:
    ExternalData strings_external_data;
    ExternalData tensors_external_data;

    // Convert the bytes to a external id if possible or to a temporal that will be resolved afterwards.
    uint64_t get_or_create_external_id(const char* bytes, std::size_t num_bytes, ExternalData& external_data);
};
} // namespace Import

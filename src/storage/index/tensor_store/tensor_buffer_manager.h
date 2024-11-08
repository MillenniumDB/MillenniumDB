#pragma once

#include "storage/file_id.h"
#include "tensor_page.h"
#include "third_party/robin_hood/robin_hood.h"

/*
 * TensorBufferManager contains all the tensor pages in memory and is used to get a page, making transparent if the page
 * is already in memory or needs to be read from disk.
 *
 * Unlike the global buffer_manager, this class manages a single TensorStore per instance.
 *
 * NOTE: We do not need a mutex because we do not support insertions during the server execution
 */
class TensorBufferManager {
public:
    static constexpr uint64_t DEFAULT_TENSOR_PAGES_BUFFER_SIZE = 2ULL * 1024ULL * 1024ULL * 1024ULL; // 2GB

    static_assert(DEFAULT_TENSOR_PAGES_BUFFER_SIZE % TensorPage::SIZE == 0,
                  "DEFAULT_TENSOR_PAGES_BUFFER_SIZE should be multiple of TensorPage::SIZE");

    TensorBufferManager(FileId file_id, uint64_t tensor_page_buffer_pool_size, bool preload_pages);

    ~TensorBufferManager();

    // Get a page. It will search in the shared buffer and if it is not on it, it will read from disk and put in the
    // buffer. Also it will pin the page, so calling buffer_manager.unpin(page) is expected when the caller doesn't need
    // the returned page anymore.
    TensorPage& get_page(uint_fast32_t page_number) noexcept;

    uint_fast32_t count_pages() const;

    // Increases the count of objects using the page. When you get a page using the methods get_page
    // the page is already pinned, so you shouldn't call this method unless you want to pin the page more than once
    void pin(TensorPage& page) {
        page.pin();
    }

    // reduces the count of objects using the page. Should be called when a object using the page is destroyed.
    void unpin(TensorPage& page) {
        page.unpin();
    }

    constexpr auto get_shared_buffer_pool_size() const noexcept {
        return tensor_page_pool_size;
    }

private:
    FileId file_id;
    // Simple clock used to page replacement in the shared buffer
    uint_fast32_t clock_pos;

    // Array of `buffer_pool_size` pages
    TensorPage* const tensor_page_pool;

    // Beginning of the allocated memory for the pages of the shared buffer
    char* const tensor_data;

    // Maximum pages the buffer can have
    const uint_fast32_t tensor_page_pool_size;

    // Used to search the index in the `buffer_pool` of a certain page
    robin_hood::unordered_map<PageId, TensorPage*> pages_map;

    // Returns an unpinned page from the tensor_page_pool
    TensorPage& get_tensor_page_available();
};

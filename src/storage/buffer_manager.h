/*
 * BufferManager contains all pages in memory and is used to get a page, making transparent if the page is
 * already in memory or needs to be readed from disk.
 *
 * `buffer_manager` is a global object and is available when this file is included. Before using it, somebody
 * must call the method BufferManager::init(), usually is the responsability of the model (e.g. QuadModel)
 * to call it.
 *
 * A mutex object is used to prevent conflict between different threads when asking for a page or
 * unpining a page (shared buffer).
 *
 * When asked for a page it can be done with a FileId or a TmpFileId, in the first case, the page returned will be
 * a page from the shared buffer. In the second case, the page will be returned from the private buffer of current thread
 * (that asked for it). Private and shared buffer doesn't need to have the same sizes. All buffers (shared and each private)
 * have the same sistems for pages replacement.
 */

#ifndef STORAGE__BUFFER_MANAGER_H_
#define STORAGE__BUFFER_MANAGER_H_

#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

#include "storage/file_id.h"
#include "storage/page.h"

class Page;

class BufferManager {
public:
    static constexpr uint_fast32_t DEFAULT_SHARED_BUFFER_POOL_SIZE  = 1024 * 256; // 1 GB
    static constexpr uint_fast32_t DEFAULT_PRIVATE_BUFFER_POOL_SIZE = 1024 * 16;  // 64 MB

    ~BufferManager();

    // necesary to be called before first usage
    static void init(uint_fast32_t shared_buffer_pool_size,
                     uint_fast32_t private_buffer_pool_size,
                     uint_fast32_t max_threads);

    // Get a page. It will search in the shared buffer and if it is not on it, it will read from disk and put in the buffer.
    // Also it will pin the page, so calling buffer_manager.unpin(page) is expected when the caller doesn't need
    // the returned page anymore.
    Page& get_page(FileId file_id, uint_fast32_t page_number) noexcept;

    // Get a page from a temp file. It will search in the private buffer and if it is not on it, it will read from disk
    // and put in the buffer.
    // Also it will pin the page, so calling buffer_manager.unpin(page) is expected when the caller doesn't need
    // the returned page anymore.
    Page& get_tmp_page(TmpFileId file_id, uint_fast32_t page_number);

    // Similar to get_page, but the page_number is the greatest number such that page number exist on disk.
    Page& get_last_page(FileId file_id);

    // Similar to get_page, but the page_number is the smallest number such that page number does not exist on disk.
    // The page returned has all its bytes initialized to 0. This operation perform a disk write inmediately
    // so 2 append_page in a row will work as expected.
    Page& append_page(FileId file_id);

    // write all dirty pages to disk
    void flush();

    // reduces the count of objects using the page. Should be called when a object using the page is destroyed.
    void unpin(Page& page);

    // invalidates all pages using `file_id`in shared buffer
    void remove(FileId file_id);

    // invalidates all pages using `tmp_file_id`in private buffer
    void remove_tmp(TmpFileId tmp_file_id);

    constexpr auto get_shared_buffer_pool_size() const noexcept { return shared_buffer_pool_size; }

    uint_fast32_t get_private_buffer_index();

private:
    BufferManager(uint_fast32_t shared_buffer_pool_size,
                  uint_fast32_t private_buffer_pool_size,
                  uint_fast32_t max_threads);

    // maximum pages the buffer can have
    const uint_fast32_t shared_buffer_pool_size;

    // maximum pages for each private buffer
    const uint_fast32_t private_buffer_pool_size;

    // maximum number of private threads
    const uint_fast32_t max_private_buffers;

    // available private positions queue
    std::queue<uint_fast32_t> available_private_positions;

    // used to search the index in the `buffer_pool` of a certain page
    std::unordered_map<PageId, uint_fast32_t, PageIdHasher> pages;

    // map thread id -> private_thread_index
    std::unordered_map<std::thread::id , uint_fast32_t> thread2index;

    // used to search the index in the `private_buffer_pool` of a certain page
    std::vector<std::unordered_map<PageId, uint_fast32_t, PageIdHasher>> private_tmp_pages;

    // array of `buffer_pool_size` pages
    Page* const buffer_pool;

    // private buffer pools
    Page* const private_buffer_pool;

    // begining of the allocated memory for the pages of the shared buffer
    char* const bytes;

    // begining of the allocated memory for the pages of the private buffer
    char* const private_bytes;

    // simple clock used to page replacement in the shared buffer
    uint_fast32_t clock_pos;

    // simple clock used to page replacement in the private buffer
    std::vector<uint_fast32_t> private_clock_pos;

    // to avoid pin/unpin synchronization problems in the shared buffer
    std::mutex pin_mutex;

    // returns the index of an unpined page from shared buffer (`buffer_pool`)
    uint_fast32_t get_buffer_available();

    // returns the index of an unpined page from the private buffer (`private_buffer_pool[thread_number]`)
    uint_fast32_t get_private_buffer_available(uint_fast32_t thread_number);

    // auxiliar method for getting page
    inline Page& get_private_page(uint_fast32_t thread_pos, uint_fast32_t page_pos) const noexcept {
        return private_buffer_pool[thread_pos * private_buffer_pool_size + page_pos];
    }
};

extern BufferManager& buffer_manager; // global object

#endif // STORAGE__BUFFER_MANAGER_H_

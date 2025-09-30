/******************************************************************************

The BufferManager provides pages, making transparent if the page is in memory
or it needs to be read from disk.

We have a global object `buffer_manager` that is available when this header is
included. To use the buffer manager, a `System` object needs to be alive.

We have 2 different type of pages:
Versioned (Page) and Private (PPage)
Each page type has its own buffer.

For concurrency control the system implements MVCC (when using Pages).
PPages doesn't need concurrency control since they are assigned to a single
certain worker.

******************************************************************************/

#pragma once

#include <map>
#include <mutex>
#include <queue>
#include <vector>

#include <boost/unordered/unordered_flat_map.hpp>

#include "storage/file_id.h"
#include "storage/page/private_page.h"
#include "storage/page/page.h"

class BufferManager {
public:
    class VersionScope {
    public:
        const uint64_t start_version;

        const uint64_t string_manager_original_end;

        const uint64_t tensor_manager_original_end;

        const bool is_editable;

        bool commited = false;

        VersionScope(uint64_t start_version, bool is_editable);

        ~VersionScope();
    };

    static constexpr uint64_t DEFAULT_VERSIONED_PAGES_BUFFER_SIZE = 1024 * 1024 * 1024; // 1 GB

    // each worker will have this buffer size
    static constexpr uint64_t DEFAULT_PRIVATE_PAGES_BUFFER_SIZE = 1024 * 1024 * 64; // 64 MB

    static_assert(
        DEFAULT_VERSIONED_PAGES_BUFFER_SIZE % Page::SIZE == 0,
        "DEFAULT_VERSIONED_PAGES_BUFFER_SIZE should be multiple of Page::SIZE"
    );
    static_assert(
        DEFAULT_PRIVATE_PAGES_BUFFER_SIZE % PPage::SIZE == 0,
        "DEFAULT_PRIVATE_PAGES_BUFFER_SIZE should be multiple of PPage::SIZE"
    );

    ~BufferManager();

    // necessary to be called before first usage
    static void init(
        uint64_t versioned_page_buffer_size_in_bytes,
        uint64_t private_page_buffer_pool_size_per_worker_in_bytes,
        uint64_t workers
    );

    // returns true if page is not the result version
    bool need_edit_version(const Page& page);

    // Get a page that won't be edited.
    // Also it will pin the page, so calling buffer_manager.unpin(page) is expected when the
    // caller doesn't need the returned page anymore.
    // It will return the result_version if it exists, otherwise it returns the start_version
    Page& get_page_readonly(FileId file_id, uint64_t page_number) noexcept;

    // Get a page that will be edited.
    // Also it will pin the page, so calling buffer_manager.unpin(page) is expected when the
    // caller doesn't need the returned page anymore.
    // It will return the result_version, creating it if not exists
    Page& get_page_editable(FileId file_id, uint64_t page_number) noexcept;

    // Returns a new page with the result_version, where its the page_number is the smallest
    // number such that page number does not exist on disk.
    // The page returned has all its bytes initialized to 0.
    // This operation does not perform a disk write immediately, but multiple append_page in
    // a row will work as expected.
    Page& append_page(FileId file_id);

    // Get a page from a temp file. It will search in the private buffer and if it is not on it, it will read from disk
    // and put in the buffer.
    // Also it will pin the page, so calling buffer_manager.unpin(page) is expected when the caller doesn't need
    // the returned page anymore.
    PPage& get_ppage(TmpFileId file_id, uint64_t page_number) /*noexcept*/;

    // increases the count of objects using the page. When you get a page using the methods of the buffer manager
    // the page is already pinned, so you shouldn't call this method unless you want to pin the page more than once
    void pin(Page& page)
    {
        page.pin();
    }

    // reduces the count of objects using the page. Should be called when a object using the page is destroyed.
    inline void unpin(Page& page) noexcept
    {
        page.unpin();
    }

    // reduces the count of objects using the page. Should be called when a object using the page is destroyed.
    inline void unpin(PPage& page) noexcept
    {
        page.unpin();
    }

    TmpFileId get_tmp_file_id();

    // invalidates all pages using `tmp_file_id` in private buffer
    void remove_tmp(TmpFileId tmp_file_id);

    std::unique_ptr<VersionScope> init_version_readonly();

    std::unique_ptr<VersionScope> init_version_editable();

    // Returns true if version_number is being used
    bool version_not_being_used(uint64_t version_number);

    bool is_editable(Page& page) const;

    // expected to be called only from main thread, not workers.
    // returns true if there was a pending write
    bool try_flush_page();

private:
    ////////////////////// VERSIONED PAGES BUFFER //////////////////////

    // frames for versioned pages
    // array of size `vp_pool_size`
    Page* const vp_pool;

    // allocated memory for the versioned pages
    char* const vp_data;

    // number of versioned pages the buffer can have
    const uint64_t vp_pool_size;

    // used for page replacement
    uint64_t vp_clock = 0;

    // prevents concurrent modifications in vp_map
    std::mutex vp_mutex;

    // used to search the index in the vp_pool of a certain versioned page
    // it points to the oldest version present in the pool
    boost::unordered_flat_map<PageId, Page*, PageId::Hasher> vp_map;

    // last version that finished its execution
    uint64_t last_stable_version = 0;

    // prevents concurrent modifications in running_version_count
    std::mutex running_version_count_mutex;

    // version -> count, count cannot be 0 (must be deleted when it reaches 0)
    std::map<uint64_t, uint64_t> running_version_count;

    // keeps track of all modifications of the current write transaction
    std::vector<Page*> current_modifications;

    // keeps track of how many appended pages the current update has
    std::map<FileId, unsigned> appended_pages;

    ////////////////////// PRIVATE PAGES BUFFER //////////////////////

    // frames for private pages
    // array of size `pp_pool_size * workers`
    PPage* const pp_pool;

    // allocated memory for the private pages
    char* const pp_data;

    // number of private pages the buffer can have for each worker
    const uint64_t pp_pool_size;

    // used to page replacement
    std::vector<uint64_t> pp_clocks;

    // used to search the index in the `pp_pool` of a certain private page
    std::vector<boost::unordered_flat_map<TmpPageId, PPage*, TmpPageId::Hasher>> pp_map;

    struct TmpFileInfo {
        uint32_t logical_size;
        uint32_t real_size;
        int fd;

        TmpFileInfo() :
            logical_size(0),
            real_size(0),
            fd(-1)
        { }
    };

    // tmp_info[i] is a list with the info of each temporal file from worker i
    // closed files of the worker stays in here until the thread finishes
    std::vector<std::vector<TmpFileInfo>> tmp_info;

    std::mutex pending_write_mutex;

    std::queue<Page*> pending_writes;

    BufferManager(
        uint64_t versioned_page_buffer_pool_size,
        uint64_t private_page_buffer_pool_size_per_worker,
        uint64_t workers
    );

    // returns an unpinned page from vp_pool
    Page& get_vpage_available();

    // returns an unpinned page from the pp_pool
    PPage& get_ppage_available(uint_fast32_t thread_number) noexcept;

    // Only meant to be called by the VersionScope destructor
    void terminate(const VersionScope& version_scope);
};

extern BufferManager& buffer_manager; // global object

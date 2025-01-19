/******************************************************************************

The BufferManager provides pages, making transparent if the page is in memory
or it needs to be read from disk.

We have a global object `buffer_manager` that is available when this header
file is included. Before using it, somebody must call the method init(),
and is the responsibility of the model (e.g. RdfModel/QuadModel) to call it,
and destroy it at the end of the program.

We have 3 different type of pages:
Versioned (VPage), Private (PPage) and Unversioned (UPage)
Each page type has its own buffer.

For concurrency control the system implements MVCC using VPages.
PPages doesn't need concurrency control since they are assigned to a single
certain worker.
UPages don't have concurrency control, since they relay on a higher logic
of the system, where new data doesn't affect old versions, and deletions
are not performed or delayed. For example this is used in the StringHash.

******************************************************************************/

#pragma once

#include <cassert>
#include <map>
#include <mutex>
#include <vector>

#include <boost/unordered/unordered_flat_map.hpp>

// #include "query/query_context.h"
#include "storage/file_id.h"
#include "storage/page/private_page.h"
#include "storage/page/unversioned_page.h"
#include "storage/page/versioned_page.h"
#include "third_party/robin_hood/robin_hood.h"

class BufferManager {
public:
    class VersionScope {
    public:
        uint64_t start_version;

        bool is_editable;

        VersionScope(uint64_t start_version, bool is_editable) :
            start_version(start_version),
            is_editable(is_editable) { }

        ~VersionScope();
    };

    static constexpr uint64_t DEFAULT_VERSIONED_PAGES_BUFFER_SIZE   = 1024 * 1024 * 1024; // 1 GB

    // each worker will have this buffer size
    static constexpr uint64_t DEFAULT_PRIVATE_PAGES_BUFFER_SIZE     = 1024 * 1024 *   64; // 64 MB

    static constexpr uint64_t DEFAULT_UNVERSIONED_PAGES_BUFFER_SIZE = 1024 * 1024 *  128; // 128 MB

    static_assert(DEFAULT_VERSIONED_PAGES_BUFFER_SIZE % VPage::SIZE == 0,
                  "DEFAULT_VERSIONED_PAGES_BUFFER_SIZE should be multiple of VPage::SIZE");
    static_assert(DEFAULT_PRIVATE_PAGES_BUFFER_SIZE % PPage::SIZE == 0,
                  "DEFAULT_PRIVATE_PAGES_BUFFER_SIZE should be multiple of PPage::SIZE");
    static_assert(DEFAULT_UNVERSIONED_PAGES_BUFFER_SIZE % UPage::SIZE == 0,
                  "DEFAULT_UNVERSIONED_PAGES_BUFFER_SIZE should be multiple of UPage::SIZE");

    ~BufferManager();

    // necessary to be called before first usage
    static void init(uint64_t versioned_page_buffer_size_in_bytes,
                     uint64_t private_page_buffer_pool_size_per_worker_in_bytes,
                     uint64_t unversioned_page_buffer_size_in_bytes,
                     uint64_t workers);

    // returns true if page is not the result version
    bool need_edit_version(const VPage& page);

    // Get a page that exists on disk and won't be edited.
    // Also it will pin the page, so calling buffer_manager.unpin(page) is expected when the
    // caller doesn't need the returned page anymore.
    // For pages that don't exist on disk yet use append_vpage
    // It will return the result_version if it exists, otherwise it returns the start_version
    VPage& get_page_readonly(FileId file_id, uint64_t page_number) noexcept;

    // Get a page that exists on disk and will be edited.
    // Also it will pin the page, so calling buffer_manager.unpin(page) is expected when the
    // caller doesn't need the returned page anymore.
    // For pages that don't exist on disk yet, use append_vpage
    // It will return the result_version, creating it if not exists
    VPage& get_page_editable(FileId file_id, uint64_t page_number) noexcept;

    // Returns a new page with the result_version, where its the page_number is the smallest
    // number such that page number does not exist on disk.
    // The page returned has all its bytes initialized to 0.
    // This operation perform a disk write immediately so multiple append_vpage in a row will
    // work as expected.
    VPage& append_vpage(FileId file_id);

    // Get a page from a temp file. It will search in the private buffer and if it is not on it, it will read from disk
    // and put in the buffer.
    // Also it will pin the page, so calling buffer_manager.unpin(page) is expected when the caller doesn't need
    // the returned page anymore.
    PPage& get_ppage(TmpFileId file_id, uint64_t page_number) /*noexcept*/;

    UPage& get_unversioned_page(FileId file_id, uint64_t page_number) noexcept;

    UPage& append_unversioned_page(FileId file_id) noexcept;

    // write all dirty pages to disk
    void flush();

    // increases the count of objects using the page. When you get a page using the methods of the buffer manager
    // the page is already pinned, so you shouldn't call this method unless you want to pin the page more than once
    void pin(VPage& page) {
        page.pin();
    }

    void pin(UPage& page) {
        page.pin();
    }

    // reduces the count of objects using the page. Should be called when a object using the page is destroyed.
    inline void unpin(VPage& page) noexcept {
        page.unpin();
    }

    // reduces the count of objects using the page. Should be called when a object using the page is destroyed.
    inline void unpin(PPage& page) noexcept {
        page.unpin();
    }

     // reduces the count of objects using the page. Should be called when a object using the page is destroyed.
    inline void unpin(UPage& page) noexcept {
        page.unpin();
    }

    TmpFileId get_tmp_file_id();

    // invalidates all pages using `tmp_file_id` in private buffer
    void remove_tmp(TmpFileId tmp_file_id);

    std::unique_ptr<VersionScope> init_version_readonly();

    std::unique_ptr<VersionScope> init_version_editable();

    void upgrade_to_editable(VersionScope&);

    // Returns true if version_number is being used
    bool version_not_being_used(uint64_t version_number);

    bool is_editable(VPage& page) const;

private:
    ////////////////////// VERSIONED PAGES BUFFER //////////////////////

    // frames for versioned pages
    // array of size `vp_pool_size`
    VPage* const vp_pool;

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
    boost::unordered_flat_map<PageId, VPage*, PageId::Hasher> vp_map;

    // last version that finished its execution
    uint64_t last_stable_version = 0;

    // prevents concurrent modifications in running_version_count
    std::mutex running_version_count_mutex;

    // version -> count, count cannot be 0 (must be deleted when it reaches 0)
    std::map<uint64_t, uint64_t> running_version_count;

    // TODO: maybe would be useful to execute delayed deletes
    // version -> count
    // used to know when to delete a version log
    // std::map<uint64_t, uint64_t> remaining_writes;

    // keeps track of all modifications of the current transaction
    // to write to the log
    // TODO: do i need to remember if it is a new page or not?
    // TODO: maybe have the pair <PageId, VPage> to easily get the bytes
    std::vector<PageId> current_modifications;

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
    std::vector<boost::unordered_flat_map<
        TmpPageId,
        PPage*,
        TmpPageId::Hasher
    >> pp_map;

    struct TmpFileInfo {
        uint32_t logical_size;
        uint32_t real_size;
        int fd;

        TmpFileInfo() :
            logical_size (0),
            real_size (0),
            fd (-1) { }
    };

    // tmp_info[i] is a list with the info of each temporal file from worker i
    // closed files of the worker stays in here until the thread finishes
    std::vector<std::vector<TmpFileInfo>> tmp_info;

    ////////////////////// UNVERSIONED PAGES BUFFER //////////////////////

    // frames for unversioned pages
    // array of size `sb_pool_size`
    UPage* const up_pool;

    // allocated memory for the unversioned pages
    char* const up_data;

    // used for page replacement
    uint64_t up_clock = 0;

    const uint64_t up_pool_size;

    // prevents concurrent modifications in up_map
    std::mutex up_mutex;

    // used to search the index in the up_pool of a certain unversioned page
    robin_hood::unordered_flat_map<PageId, UPage*> up_map;


    ////////////////////// PRIVATE METHODS //////////////////////
    BufferManager(uint64_t versioned_page_buffer_pool_size,
                  uint64_t private_page_buffer_pool_size_per_worker,
                  uint64_t unversioned_page_pool_size,
                  uint64_t workers);

    // returns an unpinned page from vp_pool
    VPage& get_vpage_available();

    // returns an unpinned page from the pp_pool
    PPage& get_ppage_available(uint_fast32_t thread_number) noexcept;

    // returns an unpinned page from up_pool
    UPage& get_upage_available();

    // Only meant to be called by the VersionScope destructor
    void terminate(const VersionScope& version_scope);
};

extern BufferManager& buffer_manager; // global object

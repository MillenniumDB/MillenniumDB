/* Page represents the content of a disk block in memory.
 * A page is treated as an array of `MDB_PAGE_SIZE` bytes (pointed by `bytes`).
 * For better performance, `MDB_PAGE_SIZE` should be multiple of the operating system's page size.
 * BufferManager is the only class who can construct a Page object. Other classes must get a Page
 * through BufferManager.
 */
#pragma once

#include <atomic>
#include <cassert>

#include "storage/page_id.h"

class Page {
friend class BufferManager;
friend class FileManager;
public:
    static constexpr size_t MDB_PAGE_SIZE = 4096;

    // contains file_id and page_number of this page
    PageId page_id;

    // mark as dirty so when page is replaced it is written back to disk.
    inline void make_dirty() noexcept { dirty = true; }

    // get the start memory position of `MDB_PAGE_SIZE` allocated bytes
    inline char* get_bytes() const noexcept { return bytes; }

    // get page number
    inline uint_fast32_t get_page_number() const noexcept { return page_id.page_number; };

private:
    // start memory address of the page, of size `MDB_PAGE_SIZE`
    char* bytes;

    // count of objects using this page, modified only by buffer_manager
    std::atomic<uint32_t> pins;

    // used by the replacement policy
    std::atomic<uint32_t> usage;

    // true if data in memory is different from disk
    bool dirty;

    Page() noexcept :
        page_id(FileId(FileId::UNASSIGNED), 0),
        bytes(nullptr),
        pins(0),
        usage(0),
        dirty(false) { }

    void pin() noexcept {
        pins++;
        usage++;
    }

    void unpin() noexcept {
        assert(pins > 0 && "Cannot unpin if pin count is 0");
        pins--;
    }

    // only meant for buffer_manager.remove()
    void reset() noexcept {
        assert(pins == 0 && "Cannot reset page if it is pinned");
        this->bytes   = nullptr;
        this->page_id = PageId(FileId(FileId::UNASSIGNED), 0);
        this->pins    = 0;
        this->usage   = 0;
        this->dirty   = false;
    }

    void reassign(PageId page_id, char* bytes) noexcept {
        assert(!dirty && "Cannot reassign page if it is dirty");
        assert(pins == 0 && "Cannot reassign page if it is pinned");
        assert(usage == 0 && "Should not reassign page if usage is not 0");

        this->page_id = page_id;
        this->bytes   = bytes;
        this->pins    = 1;
        this->usage   = 1;
    }
};

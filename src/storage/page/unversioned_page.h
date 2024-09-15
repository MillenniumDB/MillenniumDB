#pragma once

#include <atomic>
#include <cassert>

#include "storage/page/page_id.h"

// Unversioned Page. Used for pages of structures where updates don't
// generate conflicts with reads. This might imply that the structure
// has a mutex to avoid a read while the page is being updated, but
// after the update, the new data does not affect the result of the
// query, so we don't need to maintain a different version like in
// the case of Versioned Page.
// Also, structures with Unversioned Pages cannot be recovered after a
// crash directly, but they can be reconstructed from scratch from another
// structure that can be recovered after a crash.
class UPage {
friend class BufferManager;
friend class FileManager;
public:
    static constexpr size_t SIZE = 4096;

    // contains file_id and page_number of this page
    PageId page_id;

    // mark as dirty so when page is replaced it is written back to disk.
    inline void make_dirty() noexcept { dirty = true; }

    // get the start memory position of `SIZE` allocated bytes
    inline char* get_bytes() const noexcept { return bytes; }

    // get page number
    inline uint32_t get_page_number() const noexcept { return page_id.page_number; };

private:
    // start memory address of the page
    char* bytes;

    // count of objects using this page, modified only by buffer_manager
    std::atomic<uint32_t> pins;

    // used by the replacement policy
    bool second_chance;

    // true if data in memory is different from disk
    bool dirty;

    UPage() noexcept :
        page_id(FileId(FileId::UNASSIGNED), 0),
        bytes(nullptr),
        pins(0),
        second_chance(false),
        dirty(false) { }

    void pin() noexcept {
        pins++;
        second_chance = true;
    }

    void unpin() noexcept {
        assert(pins > 0 && "Cannot unpin if pin count is 0");
        pins--;
    }

    void set_bytes(char* bytes) {
        this->bytes = bytes;
    }

    void reassign(PageId page_id) noexcept {
        assert(!dirty && "Cannot reassign page if it is dirty");
        assert(pins == 0 && "Cannot reassign page if it is pinned");
        assert(second_chance == false && "Should not reassign page if second_chance is true");

        this->page_id       = page_id;
        this->pins          = 1;
        this->second_chance = true;
    }
};

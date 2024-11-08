#pragma once

#include <cassert>

#include "storage/page/page_id.h"

// Private Page. Used for temporal pages that don't need to be synchronized
// because the only can be used by one thread (worker)
class PPage {
friend class BufferManager;
friend class FileManager;
public:
    static constexpr size_t SIZE = 4096;

    // contains file_id and page_number of this page
    TmpPageId page_id;

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
    uint32_t pins;

    // used by the replacement policy
    bool second_chance;

    // true if data in memory is different from disk
    bool dirty;

    PPage() noexcept :
        page_id(TmpPageId::UNASSIGNED_ID, 0),
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

    // only meant for buffer_manager.remove()
    void reset() noexcept {
        assert(pins == 0 && "Cannot reset page if it is pinned");
        this->page_id       = TmpPageId(TmpPageId::UNASSIGNED_ID, 0);
        this->pins          = 0;
        this->second_chance = 0;
        this->dirty         = false;
    }

    void reassign(TmpPageId page_id) noexcept {
        assert(!dirty && "Cannot reassign page if it is dirty");
        assert(pins == 0 && "Cannot reassign page if it is pinned");
        assert(second_chance == false && "Should not reassign page if second_chance is true");

        this->page_id       = page_id;
        this->pins          = 1;
        this->second_chance = true;
    }
};

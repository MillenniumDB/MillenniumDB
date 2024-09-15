#pragma once

#include <atomic>
#include <cassert>

#include "storage/page/page_id.h"

// Versioned Page. Used for pages that can be used concurrently in different
// places and need different versions (Multi-version concurrency control)
// to avoid conflicts with updates being done at the same time as reads.
// Structures with Versioned Pages can be recovered after a crash if they
// have their Write Ahead Logging (WAL) activated.
class VPage {
friend class BufferManager;
friend class FileManager;
public:
    static constexpr size_t SIZE = 4096;

    // contains file_id and page_number of this page
    PageId page_id;

    // get the start memory position of `SIZE` allocated bytes
    inline char* get_bytes() const noexcept { return bytes; }

    // get page number
    inline uint32_t get_page_number() const noexcept { return page_id.page_number; };

private:
    VPage* next_version;

    VPage* prev_version;

    uint64_t version_number;

    // start memory address of the page
    char* bytes;

    // count of objects using this page
    std::atomic<uint32_t> pins;

    // used by the replacement policy by the buffer_manager
    bool second_chance;

    // true if data in memory is different from disk
    bool dirty;

    VPage() noexcept :
        page_id(FileId(FileId::UNASSIGNED), 0),
        next_version(nullptr),
        prev_version(nullptr),
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

    // only meant for buffer_manager.remove()
    void reset() noexcept {
        assert(pins == 0 && "Cannot reset page if it is pinned");
        this->bytes         = nullptr;
        this->page_id       = PageId(FileId(FileId::UNASSIGNED), 0);
        this->pins          = 0;
        this->second_chance = false;
        this->dirty         = false;
    }

    void set_bytes(char* bytes) {
        this->bytes = bytes;
    }

    void reassign(PageId page_id) noexcept {
        assert(!dirty && "Cannot reassign page if it is dirty");
        assert(pins == 0 && "Cannot reassign page if it is pinned");
        assert(second_chance == false && "Should not reassign page if usage is not 0");

        this->page_id       = page_id;
        this->pins          = 1;
        this->second_chance = true;
    }

    // this doesn't pin the page, necessary for page versioning
    void reassign_page_id(PageId page_id) noexcept {
        assert(!dirty && "Cannot reassign page if it is dirty");
        assert(pins == 0 && "Cannot reassign page if it is pinned");
        assert(second_chance == false && "Should not reassign page if usage is not 0");

        this->page_id = page_id;
    }
};

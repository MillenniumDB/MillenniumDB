#pragma once

#include <cstring>
#include <functional>
#include <type_traits>

#include "storage/file_id.h"

struct PageId {
    FileId file_id;
    uint32_t page_number;

    PageId(FileId file_id, uint32_t page_number) :
        file_id     (file_id),
        page_number (page_number) { }

    // needed to allow std::map having PageId as key
    // bool operator<(const PageId& other) const {
    //     if (this->file_id < other.file_id) {
    //         return true;
    //     } else if (other.file_id < this->file_id) {
    //         return false;
    //     } else {
    //         return this->page_number < other.page_number;
    //     }
    // }

    // needed to allow std::unordered_map having PageId as key
    bool operator==(const PageId& other) const {
        return file_id == other.file_id && page_number == other.page_number;
    }

    struct Hasher {
        uint64_t operator()(const PageId k) const {
            return k.file_id.id | (k.page_number << 6);
        }
    };
};

struct TmpPageId {
    uint32_t id;
    uint32_t page_number;

    static constexpr uint32_t UNASSIGNED_ID = UINT32_MAX;

    TmpPageId(uint32_t id, int32_t page_number) :
        id (id),
        page_number (page_number) { }

    // needed to allow std::unordered_map having PageId as key
    bool operator==(const TmpPageId& other) const {
        return id == other.id && page_number == other.page_number;
    }

    struct Hasher {
        uint64_t operator()(const TmpPageId k) const {
            return k.id ^ (k.page_number << 2);
        }
    };
};

template<>
struct std::hash<PageId> {
    std::size_t operator()(PageId const& k) const noexcept {
        return k.file_id.id | (k.page_number << 6);
    }
};

template<>
struct std::hash<TmpPageId> {
    std::size_t operator()(TmpPageId const& k) const noexcept {
        return k.id ^ (k.page_number << 2);
    }
};

static_assert(std::is_trivially_copyable<PageId>::value);

#ifndef STORAGE__PAGE_ID_H_
#define STORAGE__PAGE_ID_H_

#include "storage/file_id.h"

struct PageId {
    FileId file_id;
    uint_fast32_t page_number;

    PageId(FileId file_id, uint_fast32_t page_number)
        : file_id(file_id), page_number(page_number) { }

    // needed to allow std::map having PageId as key
    bool operator<(const PageId& other) const {
        if (this->file_id < other.file_id) {
            return true;
        } else if (other.file_id < this->file_id) {
            return false;
        } else {
            return this->page_number < other.page_number;
        }
    }

    // needed to allow std::unordered_map having PageId as key
    bool operator==(const PageId& other) const {
        return file_id == other.file_id && page_number == other.page_number;
    }
};

struct PageIdHasher {
    std::size_t operator()(const PageId& k) const {
        return k.file_id.id | (k.page_number << 5);
    }
};

#endif // STORAGE__PAGE_ID_H_
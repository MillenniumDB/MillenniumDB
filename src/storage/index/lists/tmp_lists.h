#pragma once

#include "tmp_lists_page.h"

class TmpLists {
public:
    TmpLists();
    TmpLists(TmpFileId file_id);
    ~TmpLists();

    // returns the offset
    uint64_t insert(const std::vector<ObjectId>& list);

    // writes in out
    void get(std::vector<ObjectId>& out, uint64_t offset);
    static const uint64_t MAX_LIST_SIZE = 255;

    uint32_t get_file_id();

private:
    TmpFileId file_id;
    uint64_t current_page = 0;
    uint64_t total_pages = 0;
    char* buffer;
};

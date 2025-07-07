#pragma once

#include <vector>

#include "graph_models/object_id.h"
#include "query/var_id.h"
#include "storage/file_id.h"
#include "storage/page/private_page.h"

class TmpListsPage {
public:
    TmpListsPage(TmpFileId file_id, uint_fast32_t list_page_number, uint64_t offset = 0);
    ~TmpListsPage();

    // call to insert remaining bytes of a list
    void insert(
        const std::vector<ObjectId>& list,
        uint64_t bytes_left_this_page,
        uint64_t* bytes_left_next_page
    );

    // returns offset
    uint64_t insert(const std::vector<ObjectId>& list, uint64_t* bytes_left_next_page);

    uint64_t get_list_size(uint64_t offset);

    // returns the number of bytes written
    uint64_t get(uint64_t offset, char* buffer, uint64_t buffer_pos, uint64_t* bytes_left_next_page);
    uint64_t
        get(uint64_t offset,
            char* buffer,
            uint64_t buffer_pos,
            uint64_t bytes_left_this_page,
            uint64_t* bytes_left_next_page);

    uint64_t insert_offset;

    static const uint64_t OID_SIZE = sizeof(uint64_t);

private:
    PPage& page;
    char* page_start;
};

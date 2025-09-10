#pragma once

#include <cstdint>

#include "storage/file_id.h"
#include "storage/page/page.h"

// First 4 Bytes are global_depth
// Next 4 Bytes is total_pages
// Then an array of uint32
class StringsHashDir {
public:
    StringsHashDir(FileId file_id);

    ~StringsHashDir();

    // increments total pages
    uint32_t get_new_bucket_number();

    uint32_t global_depth() const;

    uint32_t get_bucket_number(uint32_t pos);

    void set_bucket_number(uint32_t pos, uint32_t bucket_number);

    void duplicate();

private:
    void upgrade_to_editable();

    FileId file_id;

    Page* first_page;
};

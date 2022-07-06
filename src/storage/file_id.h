/*
 * We use a FileId outside of FileManager as an unique identifier to the file we need to use.
 */

#pragma once

#include <cstdint>

class FileId {
public:
    static constexpr int UNASSIGNED = INT32_MAX;

    int id;

    FileId(int id) : id(id) { }

    bool operator<(const FileId other) const {
        return this->id < other.id;
    }

    bool operator==(const FileId other) const {
        return this->id == other.id;
    }
};


class TmpFileId {
public:
    uint32_t private_buffer_pos;
    FileId file_id;

    TmpFileId(uint32_t private_buffer_pos, FileId file_id):
        private_buffer_pos (private_buffer_pos),
        file_id            (file_id) { }

    bool operator<(const TmpFileId other) const {
        // file_id works like a key so it shouldn't be necessary to compare private_buffer_pos
        return this->file_id < other.file_id;
    }

    bool operator==(const TmpFileId other) const {
        // file_id works like a key so it shouldn't be necessary to compare private_buffer_pos
        return this->file_id == other.file_id;
    }
};

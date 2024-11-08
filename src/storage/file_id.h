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
    uint32_t id;

    TmpFileId(uint32_t id) : id (id) { }

    bool operator<(const TmpFileId other) const {
        // it shouldn't be necessary to compare worker
        return this->id < other.id;
    }

    bool operator==(const TmpFileId other) const {
        // it shouldn't be necessary to compare worker
        return this->id == other.id;
    }
};

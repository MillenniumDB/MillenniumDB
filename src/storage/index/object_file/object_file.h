#pragma once

#include <fstream>
#include <memory>
#include <string>

#include "base/ids/object_id.h"
#include "base/string_manager.h"

/*
Object File is designed to contain all strings (from labels, property keys,
property values and named nodes). Strings are meant to be encoded in UTF-8.
Only those that can't be inlined (need more than 7 bytes) are saved here.
All strings are concatenated in memory, separeted by the usual string
terminator: '\0'. The ID is the position in the array where it starts.
Because the ID=0 is special (represents the null object), we need to skip the
first byte when creating the file so the first object will have the ID=1.
*/
class ObjectFile : public StringManager {
public:
    static constexpr auto INITIAL_SIZE = 4096*1024;

    ObjectFile(const std::string& filename);
    ~ObjectFile();

    uint64_t write(const std::string& bytes);
    std::string get_string(uint64_t id) const;
    void print_string(std::ostream& os, uint64_t string_id) const;

private:
    uint64_t current_end;
    uint64_t capacity;
    std::fstream file;
    char* objects; // All objects separated by '\0'
};

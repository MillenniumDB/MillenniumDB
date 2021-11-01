/*
 * Object File is designed to contain all labels, all keys and some values (those that need more than 7 bytes to be
 * encoded). Strings are meant to be encoded in UTF-8.
 *
 * To store an object, the ObjectFile will receive the bytes from the object and it will append to the file the
 * length in bytes followed by the actual bytes. The position where the length was written is returned so
 * the object can be readed later using that position. The 8-byte ID of a graph element that represent an object
 * contains the position encoded in the last 7 bytes (and first byte is used to determine the type).
 *
 * Because the ID=0 is special (represents the null object), we need to write a trash byte when creating the
 * file so the first object will have the ID=1.
 * */

#ifndef STORAGE__OBJECT_FILE_H_
#define STORAGE__OBJECT_FILE_H_

#include <fstream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "base/ids/object_id.h"

struct ObjectFileOutOfBounds : public std::runtime_error {
	ObjectFileOutOfBounds(std::string msg)
		: std::runtime_error(msg) { }
};

class ObjectFile {
public:
    static constexpr auto INITIAL_SIZE = 4096*1024;

    ObjectFile(const std::string& filename);
    ~ObjectFile();

    const char* read(uint64_t id);
    uint64_t write(std::vector<unsigned char>& bytes);

private:
    uint64_t current_end;
    uint64_t capacity;
    std::fstream& file;
    char* objects; // All objects separated by '\0'
};

#endif // STORAGE__OBJECT_FILE_H_

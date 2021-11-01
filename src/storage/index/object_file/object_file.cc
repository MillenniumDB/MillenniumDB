#include "object_file.h"

#include <cassert>
#include <cstring>

#include "storage/file_manager.h"

using namespace std;


ObjectFile::ObjectFile(const string& filename) :
    file (file_manager.get_file(file_manager.get_file_id(filename)))
{
    file.seekg (0, file.end);
    auto end_pos = file.tellg();

    // If the file is empty, write a trash byte to prevent the ID = 0
    if (end_pos == 0) {
        capacity = INITIAL_SIZE;
        objects = new char[INITIAL_SIZE];
        objects[0] = '\0';
        current_end = 1;
    } else {
        capacity = end_pos;
        objects = new char[end_pos];
        file.seekg(0, file.beg);
        file.read(objects, end_pos);
        current_end = end_pos;
    }
}


ObjectFile::~ObjectFile() {
    file.seekg(0, file.beg);
    file.write(objects, current_end);
    delete[] objects;
}


const char* ObjectFile::read(uint64_t id) {
    assert(id > 0);
    assert(objects[id-1] == '\0');
    if (id >= current_end) {
        throw ObjectFileOutOfBounds("OBJECT FILE ERROR: tried to read inexistent object (id: " + std::to_string(id)+ ")");
    }
    return &objects[id];
}


uint64_t ObjectFile::write(vector<unsigned char>& bytes) {
    assert(bytes.size() >= 8); // 7 or less bytes can be inlined

    uint64_t write_pos = current_end;
    // check the is enough space
    while (current_end + bytes.size() + 1 >= capacity) {
        // duplicate buffer
        char* new_objects = new char[capacity*2];
        std::memcpy(
            new_objects,
            objects,
            capacity
        );
        capacity *= 2;

        delete[] objects;
        objects = new_objects;
    }
    // write
    std::memcpy(
        &objects[current_end],
        bytes.data(),
        bytes.size()
    );
    current_end += bytes.size();
    objects[current_end] = '\0';
    ++current_end;

    return write_pos;
}

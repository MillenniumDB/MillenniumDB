#ifndef STORAGE__CATALOG_H_
#define STORAGE__CATALOG_H_

#include <fstream>
#include <string>

class Catalog {
protected:
    Catalog(const std::string& filename);
    ~Catalog() = default;

    bool is_empty();

    // after reading the catalog, check if bytes were readed corerectly.
    // returns true if no error was detected.
    bool check_no_error_flags();

    // should be called before start reading/writing the catalog
    void start_io();

    uint64_t read_uint64();
    uint_fast32_t read_uint32();

    void write_uint64(const uint64_t);
    void write_uint32(const uint_fast32_t);

private:
    std::fstream& file;
};

#endif // STORAGE__CATALOG_H_

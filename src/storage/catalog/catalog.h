#pragma once

#include <cstdint>
#include <fstream>
#include <string>
#include <vector>

#include <boost/unordered/unordered_flat_map.hpp>

/*
Catalog layout in MillenniumDB 1.X.Y:
- 6 bytes: magic number (0x100DECADE5DB)
- 1 byte MDB major version (1)
- 1 byte MDB minor version (X)
- 1 byte MDB patch version (Y)
- 1 byte ModelId (0: QM, 1: RDF, 2: GQL)
- 1 byte Catalog Model major version
- 1 byte Catalog Model minor version
*/
class Catalog {
public:
    enum class ModelID {
        RDF, QUAD, GQL
    };

    static constexpr uint8_t magic_number[] = {0x10, 0x0D, 0xEC, 0xAD, 0xE5, 0xDB};

    // throws if invalid version
    static Catalog::ModelID get_model_id(const std::string& db_dir);

protected:
    Catalog(const std::string& filename);
    ~Catalog();

    bool has_changes = false;

    bool is_empty();

    // throws exception if model_id or major_version is incorrect
    // return value:
    // 0 means same minor version
    // positive means catalog is older
    // negative means catalog is newer (cannot recover, will throw)
    int check_version(std::string model_name, uint8_t model_id, uint8_t catalog_major_ver, uint8_t catalog_minor_ver);

    // must be called before start writing the catalog on disk
    void start_write(uint8_t model_id, uint8_t catalog_major_ver, uint8_t catalog_minor_ver);

    uint8_t read_uint8();
    uint32_t read_uint32();
    uint64_t read_uint64();
    std::string read_string();
    std::vector<std::string> read_strvec();
    boost::unordered_flat_map<uint64_t, uint64_t> read_map();

    void write_uint8(const uint8_t);
    void write_uint32(const uint32_t);
    void write_uint64(const uint64_t);
    void write_string(const std::string&);
    void write_strvec(const std::vector<std::string>& strvec);
    void write_map(const boost::unordered_flat_map<uint64_t, uint64_t>&);

    boost::unordered_flat_map<std::string, uint64_t> convert_strvec_to_map(const std::vector<std::string>& strvec);

private:
    std::fstream file;

    std::string file_path;
};

#include "catalog.h"

#include <stdexcept>

#include "graph_models/exceptions.h"
#include "graph_models/gql/gql_catalog.h"
#include "graph_models/quad_model/quad_catalog.h"
#include "graph_models/rdf_model/rdf_catalog.h"
#include "misc/fatal_error.h"
#include "system/file_manager.h"
#include "system/system.h"

using namespace std;

Catalog::Catalog(const string& filename)
{
    file_path = file_manager.get_file_path(filename);
    file.open(file_path, ios::out | ios::app);
    if (file.fail()) {
        throw std::runtime_error("Could not open file " + filename);
    }
    file.close();
    file.open(file_path, ios::in | ios::out | ios::binary);
}

Catalog::~Catalog()
{
    file.close();
}

bool Catalog::is_empty()
{
    auto old_pos = file.tellg();
    file.seekg(0, file.end);
    bool res = file.tellg() == 0;
    file.seekg(old_pos);
    return res;
}

Catalog::ModelID Catalog::get_model_id(const std::string& db_dir)
{
    auto catalog_path = db_dir + "/catalog.dat";
    std::fstream catalog_fs(catalog_path, std::ios::in | std::ios::binary);
    if (!catalog_fs.is_open()) {
        FATAL_ERROR("Could not open catalog: ", catalog_path);
    }

    uint8_t buf[8];
    catalog_fs.read((char*) buf, sizeof(magic_number));
    uint64_t model_id = 0;

    if (memcmp(buf, magic_number, sizeof(magic_number)) != 0) {
        // MillenniumDB v0
        catalog_fs.seekg(0);
        catalog_fs.read((char*) buf, 8);

        for (int i = 0, shift = 0; i < 8; ++i, shift += 8) {
            model_id |= static_cast<uint64_t>(buf[i]) << shift;
        }
    } else {
        // MillenniumDB v1
        catalog_fs.read((char*) buf, 4);
        // buf[0] is major ver
        // buf[1] is minor ver
        // buf[2] is patch ver
        // buf[3] is model id
        model_id = buf[3];
    }

    if (!catalog_fs.good()) {
        FATAL_ERROR("Error reading catalog: ", catalog_path);
    }

    switch (model_id) {
    case GQLCatalog::MODEL_ID:
        return Catalog::ModelID::GQL;
    case RdfCatalog::MODEL_ID:
        return Catalog::ModelID::RDF;
    case QuadCatalog::MODEL_ID:
        return Catalog::ModelID::QUAD;
    default:
        FATAL_ERROR("Unknown ModelID in catalog: ", model_id);
        return Catalog::ModelID::GQL; // only to avoid warning, program will terminate
    }
}

int Catalog::check_version(
    std::string model_name,
    uint8_t model_id,
    uint8_t runtime_catalog_major_ver,
    uint8_t runtime_catalog_minor_ver
)
{
    file.seekg(0, file.beg);

    uint8_t buf[8];
    file.read((char*) buf, sizeof(magic_number));

    if (memcmp(buf, magic_number, sizeof(magic_number)) != 0) {
        FATAL_ERROR("Unsupported MillenniumDB Version, must create the database again.");
    }

    [[maybe_unused]] auto mdb_major_v = read_uint8();
    [[maybe_unused]] auto mdb_minor_v = read_uint8();
    [[maybe_unused]] auto mdb_patch_v = read_uint8();

    if (auto catalog_model_id = read_uint8(); catalog_model_id != model_id) {
        throw WrongModelException(model_name, model_id, catalog_model_id);
    }
    if (auto catalog_major_version = read_uint8(); catalog_major_version != runtime_catalog_major_ver) {
        throw WrongCatalogVersionException(model_name, runtime_catalog_major_ver, catalog_major_version);
    }

    int catalog_minor_version = read_uint8();

    if (catalog_minor_version > runtime_catalog_minor_ver) {
        FATAL_ERROR("Database catalog was created with a newer version of MillenniumDB");
    }

    return runtime_catalog_minor_ver - catalog_minor_version;
}

void Catalog::start_write(uint8_t model_id, uint8_t catalog_major_ver, uint8_t catalog_minor_ver)
{
    file.seekp(0, file.beg);
    for (size_t i = 0; i < sizeof(magic_number); ++i) {
        write_uint8(magic_number[i]);
    }
    write_uint8(System::MAJOR_VER);
    write_uint8(System::MINOR_VER);
    write_uint8(System::PATCH_VER);
    write_uint8(model_id);
    write_uint8(catalog_major_ver);
    write_uint8(catalog_minor_ver);
}

uint8_t Catalog::read_uint8()
{
    auto res = static_cast<uint8_t>(file.get());

    if (!file.good()) {
        throw std::runtime_error("Error reading uint8 from catalog");
    }

    return res;
}

uint32_t Catalog::read_uint32()
{
    uint32_t res = 0;
    uint8_t buf[4];
    file.read((char*) buf, sizeof(buf));

    for (int i = 0, shift = 0; i < 4; ++i, shift += 8) {
        res |= static_cast<uint32_t>(buf[i]) << shift;
    }

    if (!file.good()) {
        throw std::runtime_error("Error reading uint32 from catalog");
    }
    return res;
}

uint64_t Catalog::read_uint64()
{
    uint64_t res = 0;
    uint8_t buf[8];
    file.read((char*) buf, sizeof(buf));

    for (int i = 0, shift = 0; i < 8; ++i, shift += 8) {
        res |= static_cast<uint64_t>(buf[i]) << shift;
    }

    if (!file.good()) {
        throw std::runtime_error("Error reading uint64 from catalog");
    }

    return res;
}

string Catalog::read_string()
{
    auto len = read_uint32();
    char* buf = new char[len];
    file.read(buf, len);
    string res(buf, len);
    delete[] buf;
    return res;
}

vector<string> Catalog::read_strvec()
{
    vector<string> ret;
    auto size = read_uint32();
    for (size_t i = 0; i < size; ++i) {
        ret.push_back(read_string());
    }
    return ret;
}

boost::unordered_flat_map<uint64_t, uint64_t> Catalog::read_map()
{
    boost::unordered_flat_map<uint64_t, uint64_t> res;
    auto size = read_uint64();
    for (size_t i = 0; i < size; i++) {
        auto k = read_uint64();
        auto v = read_uint64();
        res.insert({ k, v });
    }
    return res;
}

void Catalog::write_uint8(const uint8_t n)
{
    file.put(static_cast<char>(n));
}

void Catalog::write_uint32(const uint32_t n)
{
    uint8_t buf[4];
    for (size_t i = 0, shift = 0; i < sizeof(buf); ++i, shift += 8) {
        buf[i] = (n >> shift) & 0xFF;
    }
    file.write(reinterpret_cast<const char*>(buf), sizeof(buf));
}

void Catalog::write_uint64(const uint64_t n)
{
    uint8_t buf[8];
    for (size_t i = 0, shift = 0; i < sizeof(buf); ++i, shift += 8) {
        buf[i] = (n >> shift) & 0xFF;
    }
    file.write(reinterpret_cast<const char*>(buf), sizeof(buf));
}

void Catalog::write_string(const string& s)
{
    write_uint32(s.size());
    file.write(s.c_str(), s.size());
}

void Catalog::write_strvec(const vector<string>& strvec)
{
    write_uint32(strvec.size());
    for (const auto& str : strvec) {
        write_string(str);
    }
}

void Catalog::write_map(const boost::unordered_flat_map<uint64_t, uint64_t>& map)
{
    write_uint64(map.size());
    for (auto&& [k, v] : map) {
        write_uint64(k);
        write_uint64(v);
    }
}

boost::unordered_flat_map<string, uint64_t> Catalog::convert_strvec_to_map(const vector<string>& strvec)
{
    boost::unordered_flat_map<string, uint64_t> res;

    for (size_t i = 0; i < strvec.size(); ++i) {
        res[strvec[i]] = i;
    }

    return res;
}

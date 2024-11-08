#include "serialization.h"

void Serialization::write_bool(std::fstream& fs, bool value) {
    fs.write(reinterpret_cast<const char*>(&value), sizeof(bool));
}


void Serialization::write_uint8(std::fstream& fs, uint8_t value) {
    fs.write(reinterpret_cast<const char*>(&value), sizeof(uint8_t));
}


void Serialization::write_uint64(std::fstream& fs, uint64_t value) {
    fs.write(reinterpret_cast<const char*>(&value), sizeof(uint64_t));
}


void Serialization::write_float(std::fstream& fs, float value) {
    fs.write(reinterpret_cast<const char*>(&value), sizeof(float));
}


void Serialization::write_string(std::fstream& fs, const std::string& value) {
    write_uint64(fs, value.size());
    fs.write(value.c_str(), value.size());
}


void Serialization::write_uint8_vec(std::fstream& fs, const std::vector<uint8_t>& vec) {
    fs.write(reinterpret_cast<const char*>(vec.data()), sizeof(uint8_t) * vec.size());
}


void Serialization::write_float_vec(std::fstream& fs, const std::vector<float>& vec) {
    fs.write(reinterpret_cast<const char*>(vec.data()), sizeof(float) * vec.size());
}


void Serialization::write_uint64_vec(std::fstream& fs, const std::vector<uint64_t>& vec) {
    fs.write(reinterpret_cast<const char*>(vec.data()), sizeof(uint64_t) * vec.size());
}


void Serialization::write_uint642uint64_unordered_map(std::fstream&                                 fs,
                                                      const std::unordered_map<uint64_t, uint64_t>& map) {
    for (auto&& [key, value] : map) {
        Serialization::write_uint64(fs, key);
        Serialization::write_uint64(fs, value);
    }
}


bool Serialization::read_bool(std::fstream& fs) {
    bool res;
    fs.read(reinterpret_cast<char*>(&res), sizeof(bool));
    return res;
}


uint8_t Serialization::read_uint8(std::fstream& fs) {
    uint8_t res;
    fs.read(reinterpret_cast<char*>(&res), sizeof(uint8_t));
    return res;
}


uint64_t Serialization::read_uint64(std::fstream& fs) {
    uint64_t res;
    fs.read(reinterpret_cast<char*>(&res), sizeof(uint64_t));
    return res;
}


float Serialization::read_float(std::fstream& fs) {
    float res;
    fs.read(reinterpret_cast<char*>(&res), sizeof(float));
    return res;
}


std::string Serialization::read_string(std::fstream& fs) {
    uint64_t    len = read_uint64(fs);
    std::string res;
    res.reserve(len);
    fs.read(&res[0], len);
    return res;
}


std::vector<uint8_t> Serialization::read_uint8_vec(std::fstream& fs, uint64_t size) {
    std::vector<uint8_t> res(size);
    fs.read(reinterpret_cast<char*>(res.data()), sizeof(uint8_t) * size);
    return res;
}


std::vector<float> Serialization::read_float_vec(std::fstream& fs, uint64_t size) {
    std::vector<float> res(size);
    fs.read(reinterpret_cast<char*>(res.data()), sizeof(float) * size);
    return res;
}


std::vector<uint64_t> Serialization::read_uint64_vec(std::fstream& fs, uint64_t size) {
    std::vector<uint64_t> res(size);
    fs.read(reinterpret_cast<char*>(res.data()), sizeof(uint64_t) * size);
    return res;
}


std::unordered_map<uint64_t, uint64_t>
  Serialization::read_uint642uint64_unordered_map(std::fstream& fs, uint64_t size) {
    std::unordered_map<uint64_t, uint64_t> res;
    res.reserve(size);
    for (uint64_t i = 0; i < size; ++i) {
        auto key   = Serialization::read_uint64(fs);
        auto value = Serialization::read_uint64(fs);
        res.insert({ key, value });
    }
    return res;
}

#pragma once

#include <cstdint>
#include <stdexcept>
#include <string>

inline std::string wrong_model_error(const std::string& catalog_name, uint64_t expected, uint64_t received) {
    std::string error_msg = "Wrong MODEL_ID for ";
    error_msg += catalog_name;
    error_msg += " catalog, expected: ";
    error_msg += std::to_string(expected);
    error_msg += ", got: ";
    error_msg += std::to_string(received);
    error_msg += ".\nCatalog file is corrupted.";
    return error_msg;
}

inline std::string wrong_version_error(const std::string& catalog_name, uint64_t expected, uint64_t received) {
    std::string error_msg = "Wrong major version for ";
    error_msg += catalog_name;
    error_msg += " catalog , expected: v";
    error_msg += std::to_string(expected);
    error_msg += ", got: v";
    error_msg += std::to_string(received);
    error_msg += ".\nThe database must be created again to work with this version.";
    return error_msg;
}

struct WrongModelException : public std::runtime_error {
    WrongModelException(const std::string& catalog_name, uint64_t expected, uint64_t received) :
        std::runtime_error(wrong_model_error(catalog_name, expected, received) ) { }
};

struct WrongCatalogVersionException : public std::runtime_error {
    WrongCatalogVersionException(const std::string& catalog_name, uint64_t expected, uint64_t received) :
        std::runtime_error(wrong_version_error(catalog_name, expected, received) ) { }
};

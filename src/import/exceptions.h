#pragma once

#include <stdexcept>
#include <string>

// Used only when importing data
struct ImportException : public std::runtime_error {
    ImportException(std::string msg) :
        std::runtime_error("Error in import: `" + msg + "`.") { }
};

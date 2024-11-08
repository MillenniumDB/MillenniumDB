#pragma once

#include <stdexcept>
#include <string>

struct WrongModelException : public std::runtime_error {
    WrongModelException(std::string msg) :
        std::runtime_error(msg) { }
};

struct WrongCatalogVersionException : public std::runtime_error {
    WrongCatalogVersionException(std::string msg) :
        std::runtime_error(msg) { }
};

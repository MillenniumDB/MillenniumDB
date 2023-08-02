#pragma once

#include <stdexcept>
#include <string>

// Used when the connection with the client is lost
struct ConnectionException : public std::runtime_error {
    ConnectionException(std::string msg) :
        std::runtime_error(msg) { }
};
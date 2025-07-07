#pragma once

#include <stdexcept>
#include <string>

// Used when the connection with the client is lost
struct ConnectionException : public std::runtime_error {
    ConnectionException(std::string msg) :
        std::runtime_error(msg) { }
};

// Used when the protocol is violated. This indicates a clear sign of a client implementation
// error so the connection must be automaticaly closed, because otherwise an undefined behavior
// could occur
struct ProtocolException : public std::runtime_error {
    ProtocolException(std::string msg) :
        std::runtime_error(msg) { }
};

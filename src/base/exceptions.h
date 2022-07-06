#pragma once

#include <stdexcept>
#include <string>

// Used when the connection with the client is lost
struct ConnectionException : public std::runtime_error {
    ConnectionException(std::string msg) :
        std::runtime_error(msg) { }
};

// Used when the query execution has a timeout
struct InterruptedException : public std::runtime_error {
    InterruptedException() : std::runtime_error("Interruption request received.") { }
};

// Used when the query received is not a valid query
struct QueryException : public std::runtime_error {
    QueryException(std::string msg) :
        std::runtime_error(msg) { }
};

// Used when the query received is not a valid query (syntax error)
struct QueryParsingException : public QueryException {
    QueryParsingException(std::string msg) :
        QueryException(msg) { }
};

// Used when the query received is not a valid query (semantic error)
struct QuerySemanticException : public QueryException {
    QuerySemanticException(std::string msg) :
        QueryException("Bad query semantic: `" + msg + "`.") { }
};

// Used when the query is correct but something went wrong during the execution.
// This exception does not mean there is a bug or something wrong with the logic, but
// an expected border-case error. For example being out of available buffers.
struct QueryExecutionException : public std::runtime_error {
    QueryExecutionException(std::string msg) :
        std::runtime_error("Error in query execution: `" + msg + "`.") { }
};

// Very similar to std::logic_error. It reports errors that are a consequence of faulty logic
// within the program such as violating logical preconditions or class invariants.
// It is a clear sign of a bug in the implementation or corrupted data.
struct LogicException : public std::logic_error {
    LogicException(std::string msg) :
        std::logic_error("Logic Error: `" + msg + "`.") { }
};

// Used when the query received needs a feature that is not supported yet, but may be supported
// in the future
struct NotSupportedException : public QueryException {
    NotSupportedException(std::string operation) :
        QueryException("Operation `" + operation + "` not supported yet.") { }
};

// Used only when importing data
struct ImportException : public std::runtime_error {
    ImportException(std::string msg) :
        std::runtime_error("Error in import: `" + msg + "`.") { }
};

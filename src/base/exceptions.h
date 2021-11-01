#ifndef BASE__PARSING_EXCEPTIONS_H_
#define BASE__PARSING_EXCEPTIONS_H_

#include <stdexcept>
#include <string>

struct ConnectionException : public std::runtime_error {
	ConnectionException(std::string msg)
		: std::runtime_error(msg) { }
};

struct InterruptedException : public std::runtime_error {
	InterruptedException()
		: std::runtime_error("Interruption request received") { }
};

struct QueryException : public std::runtime_error {
	QueryException(std::string msg)
		: std::runtime_error(msg) { }
};

struct QueryParsingException : public QueryException {
	QueryParsingException()
		: QueryException("Error parsing query") { }
};

struct QuerySemanticException : public QueryException {
	QuerySemanticException(std::string msg)
		: QueryException("Bad query semantic: " + msg + ".") { }
};

struct NotSupportedException : public QueryException {
	NotSupportedException(std::string operation)
		: QueryException("Operation " + operation + " not supported yet.") { }
};

#endif // BASE__PARSING_EXCEPTIONS_H_

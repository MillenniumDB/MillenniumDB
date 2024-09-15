#pragma once

#include <string>
#include <vector>

namespace TextSearch {

// Function signature used to tokenize the text being indexed
typedef std::vector<std::string> TokenizeFunc(const std::string&);

// Function signature to normalize tokens during indexation, and queries when searching
typedef std::string NormalizeFunc(const std::string&);

// Functions that just pass through without modifying
TokenizeFunc  noop_tokenize;
NormalizeFunc noop_normalize;

// Basic tokenize and normalize functions
TokenizeFunc  std_tokenize;
NormalizeFunc std_normalize;

} // namespace TextSearch

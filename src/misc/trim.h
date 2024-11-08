#include <string>
#include <string_view>

// Removes all leading and trailing white-space characters from the given string.
// The result is valid as long the given string is not modified
inline std::string_view trim_string(const std::string& s) {
    auto start = s.find_first_not_of(" \t\n\r");
    auto end = s.find_last_not_of(" \t\n\r");
    if (start == std::string::npos) start = 0;
    if (end == std::string::npos) end = 0;

    return std::string_view(s).substr(start, 1+end-start);
}

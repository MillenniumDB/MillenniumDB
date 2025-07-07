#pragma once

#include <string>

namespace misc {

// Checks if a string is valid to be used as a part of a path
inline bool is_name_valid_for_path(const std::string& name)
{
    if (name.empty()) {
        return false;
    }

    // first character must be a letter
    if (!std::isalpha(static_cast<unsigned char>(name[0])))
        return false;

    // the rest must be letters, digits or underscore
    for (std::size_t i = 1; i < name.size(); ++i) {
        const char c = name[i];
        if (!std::isalnum(static_cast<unsigned char>(c)) && c != '_') {
            return false;
        }
    }

    return true;
}

} // namespace misc

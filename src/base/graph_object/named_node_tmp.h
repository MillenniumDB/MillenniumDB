#pragma once

#include <string>

class NamedNodeTmp {
public:
    const std::string* name;

    NamedNodeTmp(const std::string& name) :
        name (&name) { }

    inline bool operator==(const NamedNodeTmp& rhs) const noexcept {
        return this->name == rhs.name;
    }

    inline bool operator!=(const NamedNodeTmp& rhs) const noexcept {
        return this->name != rhs.name;
    }

    inline bool operator<=(const NamedNodeTmp& rhs) const noexcept {
        return this->name <= rhs.name;
    }

    inline bool operator>=(const NamedNodeTmp& rhs) const noexcept {
        return this->name >= rhs.name;
    }

    inline bool operator<(const NamedNodeTmp& rhs) const noexcept {
        return this->name < rhs.name;
    }

    inline bool operator>(const NamedNodeTmp& rhs) const noexcept {
        return this->name > rhs.name;
    }
};

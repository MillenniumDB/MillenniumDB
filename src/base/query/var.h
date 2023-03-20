#pragma once

#include <ostream>
#include <string>

class Var {
public:
    std::string name;

    explicit Var(std::string&& name) : name(std::move(name)) { }

    explicit Var(const std::string& name) : name(name) { }

    inline bool operator<(const Var& rhs) const noexcept {
        return name < rhs.name;
    }

    inline bool operator<=(const Var& rhs) const noexcept {
        return name <= rhs.name;
    }

    inline bool operator>(const Var& rhs) const noexcept {
        return name > rhs.name;
    }

    inline bool operator>=(const Var& rhs) const noexcept {
        return name >= rhs.name;
    }

    inline bool operator==(const Var& rhs) const noexcept {
        return name == rhs.name;
    }

    inline bool operator!=(const Var& rhs) const noexcept {
        return name != rhs.name;
    }

    friend std::ostream& operator<<(std::ostream& os, const Var& var) {
        return os << var.name;
    }
};

#ifndef BASE__VAR_H_
#define BASE__VAR_H_

#include <ostream>
#include <string>

class Var {
public:
    std::string name;

    explicit Var(std::string _name) :
        name (std::move(_name)) { }

    ~Var() = default;

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

#endif // BASE__VAR_H_

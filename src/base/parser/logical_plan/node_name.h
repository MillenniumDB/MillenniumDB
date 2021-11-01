#ifndef BASE__NODE_NAME_H_
#define BASE__NODE_NAME_H_

#include <ostream>
#include <string>

class NodeName {
public:
    std::string name;

    explicit NodeName(std::string _name) :
        name (std::move(_name)) { }

    ~NodeName() = default;

    inline bool operator<(const NodeName& rhs) const noexcept {
        return name < rhs.name;
    }

    inline bool operator<=(const NodeName& rhs) const noexcept {
        return name <= rhs.name;
    }

    inline bool operator>(const NodeName& rhs) const noexcept {
        return name > rhs.name;
    }

    inline bool operator>=(const NodeName& rhs) const noexcept {
        return name >= rhs.name;
    }

    inline bool operator==(const NodeName& rhs) const noexcept {
        return name == rhs.name;
    }

    inline bool operator!=(const NodeName& rhs) const noexcept {
        return name != rhs.name;
    }

    friend std::ostream& operator<<(std::ostream& os, const NodeName& node_name) {
        return os << node_name.name;
    }
};

#endif // BASE__NODE_NAME_H_

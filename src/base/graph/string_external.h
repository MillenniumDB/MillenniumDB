#ifndef BASE__STRING_EXTERNAL_H_
#define BASE__STRING_EXTERNAL_H_

#include <cstring>

class StringExternal {
public:
    char* id;

    StringExternal() = delete;
    StringExternal(const char* id) :
        id (const_cast<char*>(id)) { }

    ~StringExternal() = default;

    inline bool operator==(const StringExternal& rhs) const noexcept {
        return strcmp(this->id, rhs.id) == 0;
    }

    inline bool operator!=(const StringExternal& rhs) const noexcept {
        return strcmp(this->id, rhs.id) != 0;
    }

    inline bool operator<=(const StringExternal& rhs) const noexcept {
        return strcmp(this->id, rhs.id) <= 0;
    }

    inline bool operator>=(const StringExternal& rhs) const noexcept {
        return strcmp(this->id, rhs.id) >= 0;
    }

    inline bool operator<(const StringExternal& rhs) const noexcept {
        return strcmp(this->id, rhs.id) < 0;
    }

    inline bool operator>(const StringExternal& rhs) const noexcept {
        return strcmp(this->id, rhs.id) > 0;
    }
};

#endif // BASE__STRING_EXTERNAL_H_

#ifndef BASE__STRING_INLINED_H_
#define BASE__STRING_INLINED_H_

#include <cassert>
#include <cstring>

class StringInlined {
public:
    char id[8];

    StringInlined() = delete;
    StringInlined(const char* _id) :
        id {_id[0], _id[1], _id[2], _id[3], _id[4], _id[5], _id[6], '\0'}
    {
        assert(_id[7] == '\0');
    }

    ~StringInlined() = default;

    inline bool operator==(const StringInlined& rhs) const noexcept {
        return strcmp(this->id, rhs.id) == 0;
    }

    inline bool operator!=(const StringInlined& rhs) const noexcept {
        return strcmp(this->id, rhs.id) != 0;
    }

    inline bool operator<=(const StringInlined& rhs) const noexcept {
        return strcmp(this->id, rhs.id) <= 0;
    }

    inline bool operator>=(const StringInlined& rhs) const noexcept {
        return strcmp(this->id, rhs.id) >= 0;
    }

    inline bool operator<(const StringInlined& rhs) const noexcept {
        return strcmp(this->id, rhs.id) < 0;
    }

    inline bool operator>(const StringInlined& rhs) const noexcept {
        return strcmp(this->id, rhs.id) > 0;
    }
};

#endif // BASE__STRING_INLINED_H_

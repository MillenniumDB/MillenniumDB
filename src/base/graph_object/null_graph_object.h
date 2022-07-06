#pragma once

struct NullGraphObject {
    inline bool operator==(const NullGraphObject&) const noexcept {
        return true;
    }

    inline bool operator!=(const NullGraphObject&) const noexcept {
        return false;
    }

    inline bool operator<=(const NullGraphObject&) const noexcept {
        return true;
    }

    inline bool operator>=(const NullGraphObject&) const noexcept {
        return true;
    }

    inline bool operator<(const NullGraphObject&) const noexcept {
        return false;
    }

    inline bool operator>(const NullGraphObject&) const noexcept {
        return false;
    }
};

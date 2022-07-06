#pragma once

struct NotFoundObject {
    inline bool operator==(const NotFoundObject&) const noexcept {
        return true;
    }

    inline bool operator!=(const NotFoundObject&) const noexcept {
        return false;
    }

    inline bool operator<=(const NotFoundObject&) const noexcept {
        return true;
    }

    inline bool operator>=(const NotFoundObject&) const noexcept {
        return true;
    }

    inline bool operator<(const NotFoundObject&) const noexcept {
        return false;
    }

    inline bool operator>(const NotFoundObject&) const noexcept {
        return false;
    }
};

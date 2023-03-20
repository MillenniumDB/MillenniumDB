#pragma once

class LiteralDatatypeExternal {
public:
    uint64_t external_id;

    LiteralDatatypeExternal(uint64_t external_id) : external_id(external_id) { }

    inline bool operator==(const LiteralDatatypeExternal& rhs) const noexcept {
        return this->external_id == rhs.external_id;
    }

    inline bool operator!=(const LiteralDatatypeExternal& rhs) const noexcept {
        return this->external_id != rhs.external_id;
    }
};

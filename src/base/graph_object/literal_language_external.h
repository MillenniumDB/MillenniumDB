#pragma once

class LiteralLanguageExternal {
public:
    uint64_t external_id;

    LiteralLanguageExternal(uint64_t external_id) : external_id(external_id) { }

    inline bool operator==(const LiteralLanguageExternal& rhs) const noexcept {
        return this->external_id == rhs.external_id;
    }

    inline bool operator!=(const LiteralLanguageExternal& rhs) const noexcept {
        return this->external_id != rhs.external_id;
    }
};

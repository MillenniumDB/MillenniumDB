#pragma once

class IriExternal {
public:
    uint64_t external_id;

    IriExternal(uint64_t external_id) : external_id(external_id) { }

    inline bool operator==(const IriExternal& rhs) const noexcept {
        return this->external_id == rhs.external_id;
    }

    inline bool operator!=(const IriExternal& rhs) const noexcept {
        return this->external_id != rhs.external_id;
    }
};

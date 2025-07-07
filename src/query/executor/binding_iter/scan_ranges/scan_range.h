#pragma once

#include <cstdint>
#include <memory>
#include <ostream>

#include "query/executor/binding.h"
#include "query/id.h"

class ScanRange {
public:
    virtual ~ScanRange() = default;
    virtual uint64_t get_min(Binding& input) = 0;
    virtual uint64_t get_max(Binding& input) = 0;
    virtual void try_assign(Binding& binding, ObjectId) = 0;
    virtual void print(std::ostream& os) const = 0;

    static std::unique_ptr<ScanRange> get(Id id, bool assigned);
    static std::unique_ptr<ScanRange> get(ObjectId id);
};

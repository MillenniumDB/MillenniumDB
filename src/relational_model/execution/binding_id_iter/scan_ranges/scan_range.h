#ifndef RELATIONAL_MODEL__SCAN_RANGE_H_
#define RELATIONAL_MODEL__SCAN_RANGE_H_

#include <cstdint>
#include <memory>
#include <variant>

#include "base/binding/binding_id.h"
#include "base/ids/object_id.h"

using Id = std::variant<VarId, ObjectId>;

class ScanRange {
public:
    virtual ~ScanRange() = default;
    virtual uint64_t get_min(BindingId& input) = 0;
    virtual uint64_t get_max(BindingId& input) = 0;
    virtual void try_assign(BindingId& my_binding, ObjectId) = 0;

    static std::unique_ptr<ScanRange> get(Id id, bool assigned);
};

#endif // RELATIONAL_MODEL__SCAN_RANGE_H_

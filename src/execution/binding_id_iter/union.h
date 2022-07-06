#pragma once

#include <memory>
#include <vector>

#include "base/ids/var_id.h"
#include "base/binding/binding_id_iter.h"

class Union : public BindingIdIter {
public:
    Union(std::vector<std::unique_ptr<BindingIdIter>> iters);

    void analyze(std::ostream& os, int indent = 0) const override;
    void begin(BindingId& parent_binding) override;
    bool next() override;
    void reset() override;
    void assign_nulls() override;

private:
    std::vector<std::unique_ptr<BindingIdIter>> iters;
    uint_fast32_t current_iter;
};

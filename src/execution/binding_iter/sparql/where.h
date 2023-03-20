#pragma once

#include <memory>

#include "base/binding/binding_id.h"
#include "base/binding/binding_id_iter.h"
#include "base/binding/binding_iter.h"

namespace SPARQL {

class Where : public BindingIter {
public:
    Where(std::unique_ptr<BindingIdIter> root, uint_fast32_t binding_size);

    void begin(std::ostream&) override;

    bool next() override;

    GraphObject operator[](VarId var_id) const override;

    void analyze(std::ostream&, int indent = 0) const override;

private:
    std::unique_ptr<BindingIdIter> root;
    BindingId                      binding_id;
};
} // namespace SPARQL
#pragma once

#include <map>
#include <memory>

#include "base/binding/binding_id_iter.h"
#include "base/binding/binding_iter.h"

class Match : public BindingIter {
public:
    Match(std::unique_ptr<BindingIdIter> root,
          uint_fast32_t                  binding_size,
          std::map<VarId, ObjectId>&     fixed_vars);

    void begin(std::ostream&) override;

    bool next() override;

    GraphObject operator[](VarId var_id) const override;

    void analyze(std::ostream&, int indent = 0) const override;

private:
    std::unique_ptr<BindingIdIter> root;

    BindingId binding_id;
};

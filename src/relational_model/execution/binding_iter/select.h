#ifndef RELATIONAL_MODEL__PROJECTION_H_
#define RELATIONAL_MODEL__PROJECTION_H_

#include <memory>
#include <vector>

#include "base/binding/binding_id_iter.h"
#include "base/binding/binding_iter.h"
#include "base/parser/logical_plan/var.h"
#include "relational_model/execution/binding/binding_select.h"

class Select : public BindingIter {

private:
    std::unique_ptr<BindingIter> child_iter;
    uint64_t limit;
    uint64_t count = 0;
    BindingSelect my_binding;

public:
    Select(std::unique_ptr<BindingIter> child_iter,
           std::vector<std::pair<Var, VarId>> projection_vars,
           uint64_t limit);
    ~Select();

    inline Binding& get_binding() noexcept override { return my_binding; }

    void begin() override;
    bool next() override;
    void analyze(std::ostream&, int indent = 0) const override;
};

#endif // RELATIONAL_MODEL__PROJECTION_H_

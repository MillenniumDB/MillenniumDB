#ifndef RELATIONAL_MODEL__WHERE_H_
#define RELATIONAL_MODEL__WHERE_H_

#include <set>
#include <map>
#include <memory>

#include "base/binding/binding_iter.h"
#include "base/binding/binding_id_iter.h"
#include "base/graph/graph_model.h"
#include "base/graph/graph_object.h"
#include "base/ids/var_id.h"
#include "relational_model/execution/binding/binding_where.h"

class Condition;

class Where : public BindingIter {
public:
    Where(const GraphModel& model,
          std::unique_ptr<BindingIter> iter,
          std::unique_ptr<Condition> condition);
    ~Where() = default;

    inline Binding& get_binding() noexcept override { return my_binding; }

    void begin() override;
    bool next() override;
    void analyze(std::ostream&, int indent = 0) const override;

private:
    const GraphModel& model;
    std::unique_ptr<BindingIter> child_iter;
    std::unique_ptr<Condition> condition;
    uint64_t results = 0;
    BindingWhere my_binding;
};

#endif // RELATIONAL_MODEL__WHERE_H_

#ifndef RELATIONAL_MODEL__DISTINCT_ORDERED_H_
#define RELATIONAL_MODEL__DISTINCT_ORDERED_H_

#include <map>
#include <memory>

#include "base/graph/graph_model.h"
#include "base/ids/var_id.h"
#include "relational_model/execution/binding/binding_distinct.h"

class DistinctOrdered : public BindingIter {
public:
    DistinctOrdered(const GraphModel& model,
                    std::unique_ptr<BindingIter> child_iter,
                    std::vector<VarId> projected_vars);

    ~DistinctOrdered() = default;

    inline Binding& get_binding() noexcept override { return child_binding; }

    void begin() override;
    bool next() override;
    void analyze(std::ostream&, int indent = 0) const override;

private:
    const GraphModel& model;
    std::unique_ptr<BindingIter> child_iter;
    Binding& child_binding;
    std::vector<VarId> projected_vars;

    std::vector<GraphObject> current_tuple;
    std::vector<GraphObject> last_tuple;
};

#endif // RELATIONAL_MODEL__DISTINCT_ORDERED_H_

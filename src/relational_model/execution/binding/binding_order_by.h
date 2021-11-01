#ifndef RELATIONAL_MODEL__BINDING_ORDER_BY_H_
#define RELATIONAL_MODEL__BINDING_ORDER_BY_H_

#include <map>
#include <memory>
#include <vector>

#include "base/binding/binding.h"
#include "base/graph/graph_model.h"

class BindingId;

class BindingOrderBy : public Binding {
public:
    BindingOrderBy(const std::map<VarId, uint_fast32_t>& saved_vars);
    ~BindingOrderBy() = default;

    std::ostream& print_to_ostream(std::ostream&) const override;

    GraphObject operator[](const VarId var_id) override;

    void print_header(std::ostream&) const override { }

    void update_binding(std::vector<GraphObject> new_objects);

private:
    // map to know the index in saved_objects of a certain var
    const std::map<VarId, uint_fast32_t>& saved_vars;

    std::vector<GraphObject> saved_objects;
};

#endif // RELATIONAL_MODEL__BINDING_ORDER_BY_H_

#ifndef RELATIONAL_MODEL__BINDING_WHERE_H_
#define RELATIONAL_MODEL__BINDING_WHERE_H_

#include <map>

#include "base/binding/binding.h"
#include "base/graph/graph_model.h"

class BindingWhere : public Binding {
public:
    BindingWhere(const GraphModel& model,
                 Binding& child_binding);
    ~BindingWhere() = default;

    std::ostream& print_to_ostream(std::ostream&) const override;

    void print_header(std::ostream&) const override { }

    GraphObject operator[](const VarId var_id) override;

private:
    const GraphModel& model;
    Binding& child_binding;
};

#endif // RELATIONAL_MODEL__BINDING_WHERE_H_

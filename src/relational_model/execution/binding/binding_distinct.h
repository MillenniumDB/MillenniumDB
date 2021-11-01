#ifndef RELATIONAL_MODEL__BINDING_DISTINCT_H_
#define RELATIONAL_MODEL__BINDING_DISTINCT_H_

#include "base/binding/binding.h"
#include "base/graph/graph_model.h"

class BindingDistinct : public Binding {
public:
    BindingDistinct(GraphModel& model, Binding& child_binding);
    ~BindingDistinct() = default;

    std::ostream& print_to_ostream(std::ostream&) const override;

    void print_header(std::ostream&) const override { }

    GraphObject operator[](const VarId var_id) override;

private:
    GraphModel& model;
    Binding& child_binding;
};

#endif // RELATIONAL_MODEL__BINDING_DISTINCT_H_

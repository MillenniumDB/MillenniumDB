#ifndef RELATIONAL_MODEL__BINDING_MATERIALIZE_ID_H_
#define RELATIONAL_MODEL__BINDING_MATERIALIZE_ID_H_

#include "base/binding/binding.h"
#include "base/graph/graph_model.h"
#include "base/ids/var_id.h"

class BindingId;

class BindingMaterializeId : public Binding {
public:
    BindingMaterializeId(const GraphModel& model, std::size_t binding_size, BindingId& binding_id);
    ~BindingMaterializeId();

    std::ostream& print_to_ostream(std::ostream&) const override;

    GraphObject operator[](const VarId var_id) override;

    void begin(BindingId&);

    void print_header(std::ostream&) const override { }

private:
    const GraphModel& model;
    std::size_t binding_size;
    BindingId& binding_id;
};

#endif // RELATIONAL_MODEL__BINDING_MATERIALIZE_ID_H_

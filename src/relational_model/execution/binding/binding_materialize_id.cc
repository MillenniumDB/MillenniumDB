#include "binding_materialize_id.h"

#include "relational_model/execution/binding_id_iter/index_scan.h"

using namespace std;

BindingMaterializeId::BindingMaterializeId(const GraphModel& model, std::size_t binding_size, BindingId& binding_id) :
    model        (model),
    binding_size (binding_size),
    binding_id   (binding_id) { }


BindingMaterializeId::~BindingMaterializeId() = default;


std::ostream& BindingMaterializeId::print_to_ostream(std::ostream& os) const {
    bool first = true;
    for (uint_fast32_t i = 0; i < binding_size; ++i) {
        if (first) {
            first = false;
        } else {
            os << ", ";
        }
        os << model.get_graph_object(binding_id[VarId(i)]);
    }
    return os;
}


GraphObject BindingMaterializeId::operator[](const VarId var_id) {
    return model.get_graph_object(binding_id[var_id]);
}

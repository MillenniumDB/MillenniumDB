#include "binding_where.h"

using namespace std;

BindingWhere::BindingWhere(const GraphModel& model, Binding& child_binding) :
    model               (model),
    child_binding       (child_binding) { }


std::ostream& BindingWhere::print_to_ostream(std::ostream& os) const {
    os << child_binding;
    return os;
}


GraphObject BindingWhere::operator[](VarId var_id) {
    return child_binding[var_id];
}

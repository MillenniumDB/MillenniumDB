#include "binding_distinct.h"

using namespace std;

BindingDistinct::BindingDistinct(GraphModel& model, Binding& child_binding) :
    model         (model),
    child_binding (child_binding) { }


std::ostream& BindingDistinct::print_to_ostream(std::ostream& os) const {
    return os << child_binding;
}


GraphObject BindingDistinct::operator[](const VarId var) {
    return child_binding[var];
}

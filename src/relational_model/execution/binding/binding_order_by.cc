#include "binding_order_by.h"

#include <cassert>

using namespace std;

BindingOrderBy::BindingOrderBy(const std::map<VarId, uint_fast32_t>& saved_vars) :
    saved_vars (saved_vars){ }


std::ostream& BindingOrderBy::print_to_ostream(std::ostream& os) const {
    return os;
}


GraphObject BindingOrderBy::operator[](const VarId var) {
    auto search = saved_vars.find(var);
    if (search != saved_vars.end()) {
        return saved_objects[search->second];
    } else {
        throw std::logic_error("saved_vars must contain VarId(" + std::to_string(var.id) + ")");
    }
}


void BindingOrderBy::update_binding(std::vector<GraphObject> graph_objects) {
    assert(graph_objects.size() == saved_vars.size());
    saved_objects = move(graph_objects);
}

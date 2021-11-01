#include "binding_select.h"

#include "base/binding/binding.h"

using namespace std;

BindingSelect::BindingSelect(vector<pair<Var, VarId>> projection_vars, Binding& child_binding) :
    projection_vars (move(projection_vars)),
    child_binding   (child_binding) { }


std::ostream& BindingSelect::print_to_ostream(std::ostream& os) const {
    auto it = projection_vars.cbegin();

    if (it != projection_vars.cend()) {
        auto& var_varid_pair = *it;
        os << child_binding[var_varid_pair.second];
        ++it;
    }
    while (it != projection_vars.cend()) {
        auto& var_varid_pair = *it;
        os << "," << child_binding[var_varid_pair.second];
        ++it;
    }
    return os;
}

void BindingSelect::print_header(std::ostream& os) const {
    auto it = projection_vars.cbegin();
    if (it != projection_vars.cend()) {
        auto& var_varid_pair = *it;
        os << var_varid_pair.first;
        ++it;
    }
    while (it != projection_vars.cend()) {
        auto& var_varid_pair = *it;
        os << "," << var_varid_pair.first;
        ++it;
    }
    os << '\n';
}


GraphObject BindingSelect::operator[](const VarId var) {
    return child_binding[var];
}

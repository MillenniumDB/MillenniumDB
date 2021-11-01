#include "select.h"

#include "relational_model/execution/binding_id_iter/property_paths/path_manager.h"

using namespace std;

Select::Select(unique_ptr<BindingIter>  _child_iter,
               vector<pair<Var, VarId>> _projection_vars,
               uint64_t                 _limit) :
    child_iter (move(_child_iter)),
    limit      (_limit),
    my_binding (BindingSelect( move(_projection_vars), child_iter->get_binding() )) { }


Select::~Select() {
    // TODO: We always have the Select operator as the root of our physical query plans.
    // If that changes we might need to call path_manager.clear() somewhere else
    // (it needs to be called always at the destruction of the query and only once)
    path_manager.clear();
}


void Select::begin() {
    child_iter->begin();
}


bool Select::next() {
    if (count < limit && child_iter->next()) {
        count++;
        return true;
    } else {
        return false;
    }
}


void Select::analyze(std::ostream& os, int indent) const {
    os << std::string(indent, ' ');
    os << "Select(";
    for (auto& pair : my_binding.projection_vars) {
        os << " " << pair.first << "(" << pair.second.id << ")";
    }
    os << " )\n";
    child_iter->analyze(os, indent);
}

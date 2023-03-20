#include "where.h"

#include "execution/graph_object/graph_object_factory.h"

using namespace std;

Where::Where(unique_ptr<BindingIter> child_iter,
             unique_ptr<BindingExpr> binding_expr) :
    child_iter   (move(child_iter)),
    binding_expr (move(binding_expr)) { }


void Where::begin(std::ostream& os) {
    child_iter->begin(os);
}


bool Where::next() {
    while (child_iter->next()) {
        checked++;
        if (binding_expr->eval(*child_iter) == GraphObjectFactory::make_bool(true)) {
            ++results;
            return true;
        }
    }
    return false;
}


GraphObject Where::operator[](VarId var_id) const {
    return (*child_iter)[var_id];
}


void Where::analyze(std::ostream& os, int indent) const {
    child_iter->analyze(os, indent);
    os << std::string(indent, ' ');
    os << "Where ( checked: " << checked << ", found: " << results << " )\n";
}

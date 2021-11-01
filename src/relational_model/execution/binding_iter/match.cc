#include "match.h"

using namespace std;

Match::Match(const GraphModel& model, unique_ptr<BindingIdIter> root, size_t binding_size) :
    model      (model),
    root       (move(root)),
    input      (binding_size),
    my_binding (BindingMaterializeId(model, binding_size, input)) { }


void Match::begin() {
    root->begin(input);
}


bool Match::next() {
    return root->next();
}


void Match::analyze(std::ostream& os, int indent) const {
    os << std::string(indent, ' ');
    os << "Match (\n";
    root->analyze(os, indent + 2);
    os << "\n";
    os << std::string(indent, ' ');
    os << ")";
}

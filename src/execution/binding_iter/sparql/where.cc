#include "where.h"

#include "query_optimizer/rdf_model/rdf_model.h"

using namespace std;
using namespace SPARQL;

Where::Where(std::unique_ptr<BindingIdIter> root, uint_fast32_t binding_size) :
    root(move(root)), binding_id(BindingId(binding_size)) { }

void Where::begin(std::ostream&) {
    root->begin(binding_id);
}

bool Where::next() {
    return root->next();
}

GraphObject Where::operator[](VarId var_id) const {
    return rdf_model.get_graph_object(binding_id[var_id]);
}

void Where::analyze(std::ostream& os, int indent) const {
    os << std::string(indent, ' ');
    os << "Where(\n";
    root->analyze(os, indent + 2);
    os << "\n";
    os << std::string(indent, ' ');
    os << ")\n";
}
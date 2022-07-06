#include "match.h"

#include "query_optimizer/quad_model/quad_model.h"

using namespace std;

Match::Match(unique_ptr<BindingIdIter> root,
             uint_fast32_t             binding_size,
             map<VarId, ObjectId>&     fixed_vars) :
    root       (move(root)),
    binding_id (BindingId(binding_size))
{
    for (auto& [var_id, obj_id] : fixed_vars) {
        binding_id.add(var_id, obj_id);
    }
}


void Match::begin(std::ostream&) {
    root->begin(binding_id);
}


bool Match::next() {
    return root->next();
}


GraphObject Match::operator[](VarId var_id) const {
    return quad_model.get_graph_object(binding_id[var_id]);
}


void Match::analyze(std::ostream& os, int indent) const {
    os << std::string(indent, ' ');
    os << "Match(\n";
    root->analyze(os, indent + 2);
    os << "\n";
    os << std::string(indent, ' ');
    os << ")\n";
}

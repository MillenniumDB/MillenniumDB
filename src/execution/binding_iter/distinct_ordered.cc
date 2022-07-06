#include "distinct_ordered.h"

using namespace std;

DistinctOrdered::DistinctOrdered(unique_ptr<BindingIter> _child_iter,
                                 vector<VarId>           _projected_vars) :
    child_iter     (move(_child_iter)),
    projected_vars (_projected_vars) { }


void DistinctOrdered::begin(std::ostream& os) {
    child_iter->begin(os);
    current_tuple = vector<GraphObject>(projected_vars.size());
}


bool DistinctOrdered::next() {
    if (last_tuple.size() == 0) {
        // first tuple
        if (child_iter->next()) {
            // save objects of current tuple for future comparison
            for (const auto& var : projected_vars) {
                last_tuple.push_back( (*child_iter)[var] );
            }
            return true;
        } else {
            return false;
        }
    } else {
        // not first tuple
        while (child_iter->next()) {
            // save objects of current tuple
            for (size_t i = 0; i < projected_vars.size(); i++) {
                current_tuple[i] = (*child_iter)[projected_vars[i]];
            }
            if (current_tuple != last_tuple) {
                last_tuple = current_tuple;
                return true;
            }
        }
        return false;
    }
}


GraphObject DistinctOrdered::operator[](VarId var_id) const {
    return (*child_iter)[var_id];
}


void DistinctOrdered::analyze(std::ostream& os, int indent) const {
    os << std::string(indent, ' ');
    child_iter->analyze(os, indent);
    os << "DistinctOrdered(";
    for (auto& var_id : projected_vars) {
        os << " VarId(" << var_id.id << ")";
    }
    os << " )\n";
}

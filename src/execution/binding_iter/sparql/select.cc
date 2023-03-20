#include "select.h"

using namespace std;
using namespace SPARQL;

Select::Select(std::unique_ptr<BindingIter>         child_iter,
               std::vector<std::pair<Var, VarId>>&& projection_vars,
               uint64_t                             limit,
               uint64_t                             offset) :
    child_iter         (move(child_iter)),
    projection_vars    (move(projection_vars)),
    limit              (limit),
    offset             (offset) { }

void Select::begin(std::ostream& _os) {
    os = &_os;
    // print header
    auto it = projection_vars.cbegin();
    if (it != projection_vars.cend()) {
        auto& var_varid_pair = *it;
        (*os) << var_varid_pair.first;
        ++it;
    }
    while (it != projection_vars.cend()) {
        auto& var_varid_pair = *it;
        (*os) << "," << var_varid_pair.first;
        ++it;
    }
    (*os) << '\n'
          << "---------------------------------------\n";

    child_iter->begin(_os);
}

bool Select::next() {
    while (offset > 0) {
        if (child_iter->next()) {
            --offset;
        }
        else {
            return false;
        }
    }
    if (count < limit && child_iter->next()) {
        auto it = projection_vars.cbegin();

        if (it != projection_vars.cend()) {
            auto& var_varid_pair = *it;
            (*os) << (*child_iter)[var_varid_pair.second];
            ++it;
        }
        while (it != projection_vars.cend()) {
            auto& var_varid_pair = *it;
            (*os) << "," << (*child_iter)[var_varid_pair.second];
            ++it;
        }
        (*os) << "\n";
        count++;
        return true;
    } else {
        return false;
    }
}

GraphObject Select::operator[](VarId var) const {
    return (*child_iter)[var];
}

void Select::analyze(std::ostream& os, int indent) const {
    child_iter->analyze(os, indent);
    os << std::string(indent, ' ');
    os << "Select(";
    for (auto& pair : projection_vars) {
        os << " " << pair.first << "(" << pair.second.id << ")";
    }
    if (limit != OpSelect::DEFAULT_LIMIT) {
        os << " LIMIT " << limit;
    }
    os << " )\n";
}
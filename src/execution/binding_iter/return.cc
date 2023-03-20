#include "return.h"

#include "execution/binding_id_iter/paths/path_manager.h"
#include "parser/query/op/mdb/op_return.h"
#include "query_optimizer/quad_model/quad_model.h"

using namespace MDB;
using namespace std;

Return::Return(unique_ptr<BindingIter>    child_iter,
               vector<pair<Var, VarId>>&& projection_vars,
               uint64_t                   limit) :
    child_iter      (move(child_iter)),
    projection_vars (move(projection_vars)),
    limit           (limit) { }


Return::~Return() {
    // TODO: We always have the Return operator as the root of our physical query plans.
    // If that changes we might need to call path_manager.clear() somewhere else
    // (it needs to be called always at the destruction of the query and only once)
    path_manager.clear();

    // Clean index cache if mode is NAIVE
    if (quad_model.index_mode == IndexMode::NAIVE) {
        quad_model.idx_map.clear();
        quad_model.idx_inv_map.clear();
    }

    // Jump to next line in Trie statistics file
    fstream stats_file;
    stats_file.open("trie_stats.csv", ios::app);
    stats_file << endl;
    stats_file.close();
}


void Return::begin(std::ostream& _os) {
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


bool Return::next() {
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


GraphObject Return::operator[](VarId var) const {
    return (*child_iter)[var];
}


void Return::analyze(std::ostream& os, int indent) const {
    child_iter->analyze(os, indent);
    os << std::string(indent, ' ');
    os << "Return(";
    for (auto& pair : projection_vars) {
        os << " " << pair.first << "(" << pair.second.id << ")";
    }
    if (limit != OpReturn::DEFAULT_LIMIT) {
        os << " LIMIT " << limit;
    }
    os << " )\n";
}

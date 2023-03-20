#include "binding_iter_visitor.h"

#include "execution/binding_id_iter/distinct_id_hash.h"
#include "execution/binding_iter/distinct_hash.h"
#include "execution/binding_iter/sparql/select.h"
#include "execution/binding_iter/sparql/where.h"

using namespace std;
using namespace SPARQL;

BindingIterVisitor::BindingIterVisitor(std::set<Var> vars, ThreadInfo* thread_info) :
    thread_info (thread_info),
    var2var_id  (construct_var2var_id(vars)) { }

map<Var, VarId> BindingIterVisitor::construct_var2var_id(std::set<Var>& vars) {
    map <Var, VarId> res;
    uint_fast32_t i = 0;
    for (auto& var : vars) {
        res.insert({ var, VarId(i++) });
    }
    return res;
}


VarId BindingIterVisitor::get_var_id(const Var& var) const {
    auto search = var2var_id.find(var);
    if (search != var2var_id.end()) {
        return (*search).second;
    } else {
        throw LogicException("var " + var.name + " not present in var2var_id");
    }
}

void BindingIterVisitor::visit(OpSelect& op_select) {
    for (const auto& var : op_select.vars) {
        auto var_id = get_var_id(var);
        projection_vars.push_back({ var, var_id });
    }

    // OpWhere may change this value when accepting visitor
    distinct_into_id = op_select.distinct;

    op_select.op->accept_visitor(*this);

    // TODO: Handle this cases of distinct
    // if (op_select.distinct) {
        // std::vector<VarId> projected_var_ids;
        // for (const auto& [var, var_id] : projection_vars) {
        //     projected_var_ids.push_back(var_id);
        // }
        // TODO: Use DistinctOrdered when possible
        // if (distinct_ordered_possible) {
        //     tmp = make_unique<DistinctOrdered>(move(tmp), move(projected_var_ids));
        // } else {
        // TODO: This would never happen with the current operators
        // if (!distinct_into_id) {
        //     tmp = make_unique<DistinctHash>(move(tmp), move(projected_var_ids));
        // }
        // else DistinctIdHash was created when visiting OpWhere
        // }
    // }

    tmp = make_unique<Select>(move(tmp), move(projection_vars), op_select.limit, op_select.offset);
}

void BindingIterVisitor::visit(OpWhere& op_where) {
    BindingIdIterVisitor id_visitor(thread_info, var2var_id);
    op_where.op->accept_visitor(id_visitor);

    unique_ptr<BindingIdIter> binding_id_iter_current_root = move(id_visitor.tmp);

    const auto binding_size = var2var_id.size();

    // TODO: Check aggs.size() == 0 when implemented
    if (distinct_into_id) {
        std::vector<VarId> projected_var_ids;
        for (const auto& [var, var_id] : projection_vars) {
            projected_var_ids.push_back(var_id);
        }
        binding_id_iter_current_root = make_unique<DistinctIdHash>(move(binding_id_iter_current_root), move(projected_var_ids));
    }

    tmp = make_unique<Where>(move(binding_id_iter_current_root), binding_size);
}

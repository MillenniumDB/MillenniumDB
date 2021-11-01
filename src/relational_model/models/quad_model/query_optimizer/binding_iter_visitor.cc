#include "binding_iter_visitor.h"

#include "base/parser/logical_plan/op/op_distinct.h"
#include "base/parser/logical_plan/op/op_group_by.h"
#include "base/parser/logical_plan/op/op_match.h"
#include "base/parser/logical_plan/op/op_optional.h"
#include "base/parser/logical_plan/op/op_order_by.h"
#include "base/parser/logical_plan/op/op_select.h"
#include "base/parser/logical_plan/op/op_where.h"
#include "base/parser/logical_plan/op/visitors/formula_to_condition.h"
#include "base/parser/logical_plan/op/visitors/get_formula_property_vars.h"
#include "relational_model/execution/binding_id_iter/distinct_id_hash.h"
#include "relational_model/execution/binding_id_iter/optional_node.h"
#include "relational_model/execution/binding_id_iter/property_paths/path_manager.h"
#include "relational_model/execution/binding_iter/match.h"
#include "relational_model/execution/binding_iter/order_by.h"
#include "relational_model/execution/binding_iter/select.h"
#include "relational_model/execution/binding_iter/where.h"
#include "relational_model/execution/binding_iter/distinct_ordered.h"
#include "relational_model/execution/binding_iter/distinct_hash.h"
#include "relational_model/models/quad_model/query_optimizer/binding_id_iter_visitor.h"

using namespace std;

BindingIterVisitor::BindingIterVisitor(const QuadModel& model, std::set<Var> vars, ThreadInfo* thread_info) :
    model       (model),
    var2var_id  (construct_var2var_id(vars)),
    thread_info (thread_info) { }


map<Var, VarId> BindingIterVisitor::construct_var2var_id(std::set<Var>& vars) {
    map<Var, VarId> res;
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
        throw std::logic_error("var " + var.name + " not present in var2var_id");
    }
}


std::unique_ptr<BindingIter> BindingIterVisitor::exec(OpSelect& op_select) {
    op_select.accept_visitor(*this);
    return move(tmp);
}


void BindingIterVisitor::visit(OpSelect& op_select) {
    // need to save the select items to be able to push optional properties from select to match in visit(OpMatch&)
    for (const auto& select_item : op_select.select_items) {
        string var_name = select_item.var;

        if (select_item.key) {
            var_properties.insert( make_pair(Var(select_item.var), select_item.key.get()) );
            var_name += '.' + select_item.key.get();
        }

        Var var(var_name);
        auto var_id = get_var_id(var);
        projection_vars.push_back(make_pair(var, var_id));
    }

    if (projection_vars.empty()) { // SELECT *
        for (auto&& [k, v] : var2var_id) {
            // TODO: here we may want to project anonymous variables too
            if (k.name[1] != '_') { // k.name looks like '?x' or '?_1'
                projection_vars.push_back(make_pair(k, v));
            }
        }
    }

    op_select.op->accept_visitor(*this);
    tmp = make_unique<Select>(move(tmp), move(projection_vars), op_select.limit);
}


void BindingIterVisitor::visit(OpWhere& op_where) {
    distinct_into_id = false;

    // add corresponding var_properties
    GetFormulaPropertyVars get_property_vars_visitor;
    for (auto& pair : get_property_vars_visitor(op_where.formula_disjunction)) {
        cout << pair.first.name << " " << pair.second << "\n";
        var_properties.insert(pair);
    }

    op_where.op->accept_visitor(*this);

    Formula2ConditionVisitor visitor(model, var2var_id);
    auto condition = visitor(op_where.formula_disjunction);

    tmp = make_unique<Where>(
        model,
        move(tmp),
        move(condition)
    );
}


void BindingIterVisitor::visit(OpMatch& op_match) {
    BindingIdIterVisitor id_visitor(model, var2var_id, thread_info);
    op_match.op->accept_visitor(id_visitor);

    unique_ptr<BindingIdIter> binding_id_iter_current_root = move(id_visitor.tmp);

    const auto binding_size = var2var_id.size();

    // TODO: set need_materialize_paths where needed
    path_manager.begin(binding_size, need_materialize_paths);

    vector<unique_ptr<BindingIdIter>> optional_children;

    // Push properties from Select/Where/OrderBy into MATCH as optional children
    for (const auto& [var, prop_name] : var_properties) {
        auto obj_var_id = get_var_id(var);
        auto value_var  = get_var_id(Var(var.name + '.' + prop_name));
        auto key_id     = model.get_object_id(GraphObject::make_string(prop_name));

        array<unique_ptr<ScanRange>, 3> ranges {
            ScanRange::get(obj_var_id, true),
            ScanRange::get(key_id, true),
            ScanRange::get(value_var, false)
        };
        auto index_scan = make_unique<IndexScan<3>>(*model.object_key_value, thread_info, move(ranges));
        optional_children.push_back(move(index_scan));
    }
    if (optional_children.size() > 0) {
        binding_id_iter_current_root = make_unique<OptionalNode>(move(binding_id_iter_current_root),
                                                                 move(optional_children));
    }
    if (distinct_into_id) {
        std::vector<VarId> projected_var_ids;
        for (const auto& [var, var_id] : projection_vars) {
            projected_var_ids.push_back(var_id);
        }

        binding_id_iter_current_root = make_unique<DistinctIdHash>(move(binding_id_iter_current_root), move(projected_var_ids));
    }

    tmp = make_unique<Match>(model, move(binding_id_iter_current_root), binding_size);
}


void BindingIterVisitor::visit(OpOrderBy& op_order_by) {
    std::vector<VarId> order_vars;
    std::set<VarId> saved_vars;

    for (const auto& order_item : op_order_by.items) {
        string var_name = order_item.var;
        if (order_item.key) {
            var_name += '.' + order_item.key.get();

            // add properties to var_properties to use at visiting OpMatch
            var_properties.insert( make_pair(Var(order_item.var), order_item.key.get()) );
        }
        const auto var_id = get_var_id(Var(var_name));
        order_vars.push_back(var_id);
        saved_vars.insert(var_id);
    }

    for (auto& [var, var_id] : projection_vars) {
        saved_vars.insert(var_id);
    }

    // TODO: we could set distinct_ordered_possible=true if the projection vars are in the begining
    // e.g. if we have SELECT DISTINCT ?x, ?y ... ORDER BY ?x, ?z, ?y we can't use DistinctOrdered
    // distinct_ordered_possible = true;

    op_order_by.op->accept_visitor(*this);
    tmp = make_unique<OrderBy>(thread_info, move(tmp), saved_vars, order_vars, op_order_by.ascending_order);
}


void BindingIterVisitor::visit(OpGroupBy& op_group_by) {
    // TODO: implement this when GroupBy are supported
    op_group_by.op->accept_visitor(*this);
}


void BindingIterVisitor::visit(OpDistinct& op_distinct) {
    std::vector<VarId> projected_var_ids;
    for (const auto& [var, var_id] : projection_vars) {
        projected_var_ids.push_back(var_id);
    }
    distinct_into_id = true;  // OpWhere may change this value when accepting visitor
    op_distinct.op->accept_visitor(*this);

    if (distinct_ordered_possible) {
        tmp = make_unique<DistinctOrdered>(model, move(tmp), move(projected_var_ids));
    } else {
        if (!distinct_into_id) {
            tmp = make_unique<DistinctHash>(move(tmp), move(projected_var_ids));
        }
        // else DistinctIdHash was created when visiting OpMatch
    }
}

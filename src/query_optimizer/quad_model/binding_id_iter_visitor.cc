#include "binding_id_iter_visitor.h"

#include <cassert>
#include <iostream>

#include "execution/binding_id_iter/optional_node.h"
#include "execution/binding_id_iter/empty_binding_id_iter.h"
#include "execution/binding_id_iter/single_result_binding_id_iter.h"
#include "query_optimizer/quad_model/quad_model.h"
#include "query_optimizer/quad_model/plan/basic/edge_plan.h"
#include "query_optimizer/quad_model/plan/basic/label_plan.h"
#include "query_optimizer/quad_model/plan/basic/path_plan.h"
#include "query_optimizer/quad_model/plan/basic/property_plan.h"
#include "query_optimizer/quad_model/plan/basic/unjoint_object_plan.h"
// #include "query_optimizer/quad_model/plan/join/hash_join_plan.h"
#include "query_optimizer/quad_model/join_order/greedy_optimizer.h"
#include "query_optimizer/quad_model/join_order/leapfrog_optimizer.h"
#include "query_optimizer/quad_model/join_order/selinger_optimizer.h"

using namespace std;

constexpr auto MAX_SELINGER_PLANS = 0;

BindingIdIterVisitor::BindingIdIterVisitor(ThreadInfo* thread_info,
                                           const map<Var, VarId>& var2var_id,
                                           map<VarId, ObjectId>& fixed_vars,
                                           const vector<tuple<Var, string, QueryElement>>& where_properties) :
    var2var_id       (var2var_id),
    fixed_vars       (fixed_vars),
    where_properties (where_properties),
    thread_info      (thread_info) { }


VarId BindingIdIterVisitor::get_var_id(const Var& var) const {
    auto search = var2var_id.find(var);
    if (search != var2var_id.end()) {
        return (*search).second;
    } else {
        throw LogicException("variable " + var.name + " not present in var_name2var_id");
    }
}


void BindingIdIterVisitor::visit(OpBasicGraphPattern& op_basic_graph_pattern) {
    // Process Isolated Terms
    // if a term is not found we can asume the MATCH result is empty
    for (auto& isolated_term : op_basic_graph_pattern.isolated_terms) {
        if ( !term_exists(quad_model.get_object_id(isolated_term.term.to_graph_object())) ) {
            tmp = make_unique<EmptyBindingIdIter>();
            return;
        }
    }

    // Push equalities from where into the basic graph pattern
    std::set<Var> where_vars;
    for (auto& where_property : where_properties) {
        auto var    = std::get<0>(where_property);
        auto key    = std::get<1>(where_property);
        auto value  = std::get<2>(where_property);

        where_vars.insert(var);
        op_basic_graph_pattern.add_property(OpProperty(var, key, value));
    }

    vector<unique_ptr<Plan>> base_plans;

    // Process Isolated Vars
    for (auto& isolated_var : op_basic_graph_pattern.isolated_vars) {
        auto var_id = get_var_id(isolated_var.var);
        auto fixed_var_found = fixed_vars.find(var_id);
        if (fixed_var_found != fixed_vars.end()) {
            if ( !term_exists(fixed_var_found->second) ) {
                tmp = make_unique<EmptyBindingIdIter>();
                return;
            }
        } else {
            // we could have something like: MATCH (?x) WHERE ?x.age == 1.
            // ?x is not really an isolated var
            if (where_vars.find(isolated_var.var) == where_vars.end()) {
                base_plans.push_back(
                    make_unique<UnjointObjectPlan>(var_id)
                );
            }
        }
    }

    // Process Labels
    for (auto& op_label : op_basic_graph_pattern.labels) {
        auto label_id = quad_model.get_object_id(GraphObject::make_string(op_label.label));
        auto node_id  = get_id(op_label.node);

        base_plans.push_back(
            make_unique<LabelPlan>(node_id, label_id)
        );
    }

    // Process properties (value is fixed)
    for (auto& op_property : op_basic_graph_pattern.properties) {
        auto obj_id   = get_id(op_property.node);
        auto key_id   = quad_model.get_object_id(GraphObject::make_string(op_property.key));
        auto value_id = quad_model.get_object_id(op_property.value.to_graph_object());

        if (op_property.node.is_var()) {
            VarId value_var = get_var_id(Var(op_property.node.to_var().name + '.' + op_property.key) );
            fixed_vars.insert({value_var, value_id});
        }

        base_plans.push_back(
            make_unique<PropertyPlan>(obj_id, key_id, value_id)
        );
    }

    // Process connections
    for (auto& op_edge : op_basic_graph_pattern.edges) {
        auto from_id = get_id(op_edge.from);
        auto to_id   = get_id(op_edge.to);
        auto edge_id = get_id(op_edge.edge);
        auto type_id = get_id(op_edge.type);

        base_plans.push_back(
            make_unique<EdgePlan>(from_id, to_id, type_id, edge_id)
        );
    }

    // Process property paths
    for (auto& path : op_basic_graph_pattern.paths) {
        auto from_id = get_id(path.from);
        auto to_id   = get_id(path.to);

        VarId path_var = get_var_id(path.var);
        base_plans.push_back(
            make_unique<PathPlan>(path_var, from_id, to_id, *path.path, path.semantic)
        );
    }

    assert(tmp == nullptr);

    // construct var names
    vector<string> var_names;
    const auto binding_size = var2var_id.size();
    var_names.resize(binding_size);
    for (auto&& [var, var_id] : var2var_id) {
        var_names[var_id.id] = var.name;
    }

    if (base_plans.size() == 0) {
        tmp = make_unique<SingleResultBindingIdIter>();
        return;
    }

    // Set input vars
    for (auto& plan : base_plans) {
        plan->set_input_vars(assigned_vars);
    }

    // try to use leapfrog if there is a join
    if (base_plans.size() > 1) {
        tmp = LeapfrogOptimizer::try_get_iter(thread_info, base_plans, var_names, binding_size);
    }

    if (tmp == nullptr) {
        unique_ptr<Plan> root_plan = nullptr;
        if (base_plans.size() <= MAX_SELINGER_PLANS) {
            SelingerOptimizer selinger_optimizer(base_plans, var_names);
            root_plan = selinger_optimizer.get_plan();
        } else {
            root_plan = GreedyOptimizer::get_plan(base_plans, var_names);
        }

        std::cout << "\nPlan Generated:\n";
        root_plan->print(std::cout, true, var_names);
        std::cout << "\nestimated cost: " << root_plan->estimate_cost() << "\n";

        tmp = root_plan->get_binding_id_iter(thread_info);
    }

    // insert new assigned_vars
    for (auto& plan : base_plans) {
        for (auto var : plan->get_vars()) {
            assigned_vars.insert(var);
        }
    }
}


void BindingIdIterVisitor::visit(OpOptional& op_optional) {
    op_optional.op->accept_visitor(*this);
    unique_ptr<BindingIdIter> binding_id_iter = move(tmp);

    vector<unique_ptr<BindingIdIter>> optional_children;
    // TODO: its not necessary to remember assigned_vars and reassign them after visiting a child because we only
    // support well designed patterns. If we want to support non well designed patterns this could change
    // auto current_scope_assigned_vars = assigned_vars;

    for (auto& optional : op_optional.optionals) {
        optional->accept_visitor(*this);
        optional_children.push_back(move(tmp));
        // assigned_vars = current_scope_assigned_vars;
    }

    assert(tmp == nullptr);
    tmp = make_unique<OptionalNode>(move(binding_id_iter), move(optional_children));
}


Id BindingIdIterVisitor::get_id(const QueryElement& query_element) const {
    if (query_element.is_var()) {
        auto var_id = get_var_id(query_element.to_var());
        auto fixed_var_found = fixed_vars.find(var_id);
        if (fixed_var_found != fixed_vars.end()) {
            return fixed_var_found->second;
        } else {
            return var_id;
        }
    } else {
        return quad_model.get_object_id(query_element.to_graph_object());
    }
}


bool BindingIdIterVisitor::term_exists(ObjectId term) const {
    if (term.is_not_found()) {
        return false;
    } else if ((term.id & ObjectId::TYPE_MASK) == ObjectId::ANONYMOUS_NODE_MASK) {
        auto anon_id = term.id & ObjectId::VALUE_MASK;
        return anon_id <= quad_model.catalog().anonymous_nodes_count;
    } else if ((term.id & ObjectId::TYPE_MASK) == ObjectId::CONNECTION_MASK) {
        auto conn_id = term.id & ObjectId::VALUE_MASK;
        return conn_id <= quad_model.catalog().connections_count;
    } else {
        // search in nodes
        auto r = RecordFactory::get(term.id);
        bool interruption_requested = false;
        auto it = quad_model.nodes->get_range(&interruption_requested, r, r);
        return it->next() != nullptr;
    }
}

#include "binding_id_iter_visitor.h"

#include <cassert>
#include <iostream>

#include "base/parser/logical_plan/op/op_basic_graph_pattern.h"
#include "base/parser/logical_plan/op/op_optional.h"
#include "base/parser/logical_plan/op/op_path.h"
#include "base/parser/logical_plan/op/op_path_alternatives.h"
#include "base/parser/logical_plan/op/op_path_atom.h"
#include "base/parser/logical_plan/op/op_path_sequence.h"
#include "base/parser/logical_plan/op/op_path_kleene_star.h"
#include "base/parser/logical_plan/op/op_path_optional.h"

#include "relational_model/execution/binding_id_iter/optional_node.h"
#include "relational_model/execution/binding_id_iter/empty_binding_id_iter.h"
#include "relational_model/execution/binding_id_iter/single_result_binding_id_iter.h"

#include "relational_model/models/quad_model/query_optimizer/plan/basic/connection_plan.h"
#include "relational_model/models/quad_model/query_optimizer/plan/basic/label_plan.h"
#include "relational_model/models/quad_model/query_optimizer/plan/basic/property_plan.h"
#include "relational_model/models/quad_model/query_optimizer/plan/basic/property_path_plan.h"
#include "relational_model/models/quad_model/query_optimizer/plan/basic/unjoint_object_plan.h"
// #include "relational_model/models/quad_model/query_optimizer/plan/join/hash_join_plan.h"

#include "relational_model/models/quad_model/query_optimizer/join_order/greedy_optimizer.h"
#include "relational_model/models/quad_model/query_optimizer/join_order/leapfrog_optimizer.h"
#include "relational_model/models/quad_model/query_optimizer/join_order/selinger_optimizer.h"

using namespace std;

constexpr auto MAX_SELINGER_PLANS = 0;

BindingIdIterVisitor::BindingIdIterVisitor(const QuadModel& model,
                                           const map<Var, VarId>& var2var_id,
                                           ThreadInfo* thread_info) :
    model       (model),
    var2var_id  (var2var_id),
    thread_info (thread_info) { }


VarId BindingIdIterVisitor::get_var_id(const Var& var) {
    auto search = var2var_id.find(var);
    if (search != var2var_id.end()) {
        return (*search).second;
    } else {
        throw std::logic_error("variable " + var.name + " not present in var_name2var_id");
    }
}


void BindingIdIterVisitor::visit(OpBasicGraphPattern& op_basic_graph_pattern) {
    // Process Isolated Terms
    // if a term is not found we can asume the MATCH result is empty
    for (auto& isolated_term : op_basic_graph_pattern.isolated_terms) {
        ObjectId term = model.get_object_id(isolated_term.term.to_graph_object());
        if (term.is_not_found()) {
            tmp = make_unique<EmptyBindingIdIter>();
            return;
        } else if ((term.id & GraphModel::TYPE_MASK) == GraphModel::ANONYMOUS_NODE_MASK) {
            auto anon_id = term.id & GraphModel::VALUE_MASK;
            if (anon_id > model.catalog().anonymous_nodes_count) {
                tmp = make_unique<EmptyBindingIdIter>();
                return;
            } else {
                tmp = make_unique<SingleResultBindingIdIter>();
            }
        } else if ((term.id & GraphModel::TYPE_MASK) == GraphModel::CONNECTION_MASK) {
            auto conn_id = term.id & GraphModel::VALUE_MASK;
            if (conn_id > model.catalog().connections_count) {
                tmp = make_unique<EmptyBindingIdIter>();
                return;
            } else {
                tmp = make_unique<SingleResultBindingIdIter>();
            }
        } else {
            // search in nodes
            auto r = RecordFactory::get(term.id);
            bool interruption_requested = false;
            auto it = model.nodes->get_range(&interruption_requested, r, r);
            if (it->next() == nullptr) {
                tmp = make_unique<EmptyBindingIdIter>();
                return;
            }
        }
    }
    vector<unique_ptr<Plan>> base_plans;

    // Process Isolated Vars
    for (auto& isolated_var : op_basic_graph_pattern.isolated_vars) {
        base_plans.push_back(
            make_unique<UnjointObjectPlan>(model, get_var_id(isolated_var.var))
        );
    }

    // Process Labels
    for (auto& op_label : op_basic_graph_pattern.labels) {
        auto label_id = model.get_object_id(GraphObject::make_string(op_label.label));

        if (op_label.node_id.is_var()) {
            auto node_var_id = get_var_id(op_label.node_id.to_var());
            base_plans.push_back(
                make_unique<LabelPlan>(model, node_var_id, label_id)
            );
        } else {
            auto node_id = model.get_object_id(op_label.node_id.to_graph_object());
            base_plans.push_back(
                make_unique<LabelPlan>(model, node_id, label_id)
            );
        }
    }

    // Process properties from Match
    for (auto& op_property : op_basic_graph_pattern.properties) {
        auto key_id   = model.get_object_id(GraphObject::make_string(op_property.key));
        auto value_id = model.get_value_id(op_property.value);

        if (op_property.node_id.is_var()) {
            auto obj_var_id = get_var_id(op_property.node_id.to_var());

            base_plans.push_back(
                make_unique<PropertyPlan>(model, obj_var_id, key_id, value_id)
            );
        } else {
            auto obj_id = model.get_object_id(op_property.node_id.to_graph_object());
            base_plans.push_back(
                make_unique<PropertyPlan>(model, obj_id, key_id, value_id)
            );
        }
    }

    // Process connections
    for (auto& op_connection : op_basic_graph_pattern.connections) {
        auto from_id = op_connection.from.is_var()
                        ? (Id) get_var_id(op_connection.from.to_var())
                        : (Id) model.get_object_id(op_connection.from.to_graph_object());

        auto to_id   = op_connection.to.is_var()
                        ? (Id) get_var_id(op_connection.to.to_var())
                        : (Id) model.get_object_id(op_connection.to.to_graph_object());

        auto edge_id = op_connection.edge.is_var()
                        ? (Id) get_var_id(op_connection.edge.to_var())
                        : (Id) model.get_object_id(op_connection.edge.to_graph_object());

        if (op_connection.types.empty()) {
            // Type not mentioned, creating anonymous variable for type
            // Important: this name must be consistent with generated at OpBasicGraphPattern
            auto tmp_str = op_connection.edge.to_string();
            if (tmp_str[0] == '?') {
                tmp_str.erase(0, 1);
            }
            auto type_var_id = get_var_id(Var("?_typeof_" + tmp_str));
            base_plans.push_back(
                make_unique<ConnectionPlan>(model, from_id, to_id, type_var_id, edge_id));
        }
        else if (op_connection.types.size() == 1) {
            if (op_connection.types[0].is_var()) {
                // Type is an explicit variable
                auto type_var_id = get_var_id(Var(op_connection.types[0].to_var()));
                base_plans.push_back(
                    make_unique<ConnectionPlan>(model, from_id, to_id, type_var_id, edge_id));
            } else {
                // Type is an IdentifiebleNode
                auto type_obj_id = model.get_object_id(op_connection.types[0].to_graph_object());
                base_plans.push_back(
                    make_unique<ConnectionPlan>(model, from_id, to_id, type_obj_id, edge_id)
                );
            }
        }
        else {
            throw QuerySemanticException("Connections can't have multiple types when using QuadModel");
        }
    }

    for (auto& property_path : op_basic_graph_pattern.property_paths) {
        auto from_id = property_path.from.is_var()
                    ? (Id) get_var_id(property_path.from.to_var())
                    : (Id) model.get_object_id(property_path.from.to_graph_object());

        auto to_id   = property_path.to.is_var()
                    ? (Id) get_var_id(property_path.to.to_var())
                    : (Id) model.get_object_id(property_path.to.to_graph_object());


        VarId path_var = get_var_id(property_path.var);
        base_plans.push_back(
            make_unique<PropertyPathPlan>(model, path_var, from_id, to_id, *property_path.path)
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
            SelingerOptimizer selinger_optimizer(move(base_plans), var_names);
            root_plan = selinger_optimizer.get_plan();
        } else {
            root_plan = GreedyOptimizer::get_plan(move(base_plans), var_names);
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

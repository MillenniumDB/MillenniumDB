#include "optimize_tree.h"

#include "base/exceptions.h"
#include "base/parser/logical_plan/op/op_basic_graph_pattern.h"
#include "base/parser/logical_plan/op/op_distinct.h"
#include "base/parser/logical_plan/op/op_group_by.h"
#include "base/parser/logical_plan/op/op_match.h"
#include "base/parser/logical_plan/op/op_optional.h"
#include "base/parser/logical_plan/op/op_order_by.h"
#include "base/parser/logical_plan/op/op_select.h"
#include "base/parser/logical_plan/op/op_where.h"

using namespace std;

void OptimizeTree::visit(OpOptional& op_optional) {
    op_optional.op->accept_visitor(*this);
    bool current_move_children_up = false;
    if (delete_current) {
        current_move_children_up = true;
    }
    delete_current = false;
    move_children_up = false;
    optional_to_match = false;
    for (auto it = op_optional.optionals.begin(); it != op_optional.optionals.end(); ) {
        (*it)->accept_visitor(*this);
        if (move_children_up || optional_to_match) {
            it = op_optional.optionals.erase(it);
            it = op_optional.optionals.insert(it,
                                              std::make_move_iterator(optionals.begin()),
                                              std::make_move_iterator(optionals.end()));
            advance(it, optionals.size());
            optionals.clear();

            // reset
            move_children_up = false;
            optional_to_match = false;

        }
        else if (delete_current) {
            it = op_optional.optionals.erase(it);
            delete_current = false;
        } else {
            ++it;
        }
    }

    if (op_optional.optionals.size() == 0) {
        if (current_move_children_up) {
            move_children_up = false;
            delete_current = true;
        } else {
            move_children_up = false;
            delete_current= false;
            optional_to_match = true;
            optionals.emplace_back(move(op_optional.op));
        }

    } else if(current_move_children_up) {
        move_children_up = true;
        delete_current = false;
        for (auto it=op_optional.optionals.begin(); it!=op_optional.optionals.end(); ) {
            optionals.emplace_back(move(*it));
            it = op_optional.optionals.erase(it);
        }
    }
}


void OptimizeTree::visit(OpBasicGraphPattern& op_basic_graph_pattern) {
    // delete already assigned properties
    for (auto it = op_basic_graph_pattern.properties.begin(); it != op_basic_graph_pattern.properties.end(); ) {
        auto op_property = *it;
        auto property_search = global_properties_set.find(op_property);
        if (property_search != global_properties_set.end()) {
            auto found_property = *property_search;
            if (op_property.value != found_property.value) {
                it = op_basic_graph_pattern.properties.erase(it);
            } else {
                ++it;
            }
        }
        global_properties_set.insert(op_property);
    }

    // delete already assigned labels
    for (auto it = op_basic_graph_pattern.labels.begin(); it != op_basic_graph_pattern.labels.end(); ) {
        auto op_label = *it;
        auto label_search = global_label_set.find(op_label);
        if (label_search != global_label_set.end()) {
            it = op_basic_graph_pattern.labels.erase(it);
        } else {
            ++it;
        }
        global_label_set.insert(op_label);
    }

    // delete already assigned isolated vars
    for (auto it = op_basic_graph_pattern.isolated_vars.begin(); it != op_basic_graph_pattern.isolated_vars.end(); ) {
        auto op_unjoint_object = *it;

        if (global_vars.find(op_unjoint_object.var) != global_vars.end()) {
            it = op_basic_graph_pattern.isolated_vars.erase(it);
        } else {
            ++it;
        }
    }

    // if nothing to match, will be deleted
    if (   op_basic_graph_pattern.connections.empty()
        && op_basic_graph_pattern.labels.empty()
        && op_basic_graph_pattern.properties.empty()
        && op_basic_graph_pattern.property_paths.empty()
        && op_basic_graph_pattern.isolated_vars.empty())
    {
        delete_current = true;
    }

    auto match_vars = op_basic_graph_pattern.get_vars();
    for (auto& var_name : match_vars) {
        global_vars.insert(var_name);
    }
}


void OptimizeTree::visit(OpMatch& op_match) {
    optional_to_match = false;
    op_match.op->accept_visitor(*this);
    if (optional_to_match) {
        op_match.op = move(optionals[0]);
    }
}


void OptimizeTree::visit(OpSelect& op_select) {
    op_select.op->accept_visitor(*this);
}


void OptimizeTree::visit(OpWhere& op_where) {
    op_where.op->accept_visitor(*this);
}


void OptimizeTree::visit(OpGroupBy& op_group_by) {
    op_group_by.op->accept_visitor(*this);
}


void OptimizeTree::visit(OpOrderBy& op_order_by) {
    op_order_by.op->accept_visitor(*this);
}


void OptimizeTree::visit(OpDistinct& op_distinct) {
    op_distinct.op->accept_visitor(*this);
}

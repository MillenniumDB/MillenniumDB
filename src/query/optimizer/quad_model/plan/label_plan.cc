#include "label_plan.h"

#include <cassert>

#include "graph_models/quad_model/quad_model.h"
#include "query/executor/binding_iter/index_scan.h"
#include "storage/index/leapfrog/leapfrog_bpt_iter.h"

using namespace std;

LabelPlan::LabelPlan(Id node, Id label) :
    node           (node),
    label          (label),
    node_assigned  (node.is_OID()),
    label_assigned (label.is_OID()) { }


void LabelPlan::print(std::ostream& os, int indent) const {
    for (int i = 0; i < indent; ++i) {
        os << ' ';
    }
    os << "Label(";
    os << "node: " << node;
    os << ", label: " << label;
    os << ")";

    os << ",\n";
    for (int i = 0; i < indent; ++i) {
        os << ' ';
    }
    os << "  ↳ Estimated factor: " << estimate_output_size();
}


double LabelPlan::estimate_cost() const {
    return /*100.0 +*/ estimate_output_size();
}


double LabelPlan::estimate_output_size() const {
    const auto total_nodes = static_cast<double>(quad_model.catalog().identifiable_nodes_count
                                                 + quad_model.catalog().anonymous_nodes_count);

    auto total_labels = static_cast<double>(quad_model.catalog().label_count);

    if (total_nodes == 0) { // to avoid division by 0
        return 0;
    }

    if (label_assigned) {
        // nodes with label `label_id`
        double label_count = 0;
        if (label.is_OID()) {
            auto it = quad_model.catalog().label2total_count.find(label.get_OID().id);
            if (it != quad_model.catalog().label2total_count.end()) {
                label_count = static_cast<double>(it->second);
            }
        } else {
            // TODO: this case (label is an assigned variable) is not possible yet, but we may need to cover it in the future
            return 0;
        }

        if (node_assigned) {
            return label_count / total_nodes;
        } else {
            return label_count;
        }
    } else {
        if (node_assigned) {
            return total_labels / total_nodes;
        } else {
            return total_nodes;
        }
    }
}


void LabelPlan::set_input_vars(const std::set<VarId>& input_vars) {
    set_input_var(input_vars, node, &node_assigned);
    set_input_var(input_vars, label, &label_assigned);
}


// Must be consistent with the index scan returned in get_binding_iter()
std::set<VarId> LabelPlan::get_vars() const {
    std::set<VarId> result;
    if (node.is_var() && !node_assigned) {
        result.insert(node.get_var());
    }
    if (label.is_var() && !label_assigned) {
        result.insert(label.get_var());
    }

    return result;
}


/**
 * ╔═╦═══════════════╦═════════════════╦═════════╗
 * ║ ║ Node Assigned ║  Label Assigned ║  Index  ║
 * ╠═╬═══════════════╬═════════════════╬═════════╣
 * ║1║       yes     ║       yes       ║    NL   ║
 * ║2║       yes     ║       no        ║    NL   ║
 * ║3║       no      ║       yes       ║    LN   ║
 * ║4║       no      ║       no        ║    LN   ║
 * ╚═╩═══════════════╩═════════════════╩═════════╝
 */
unique_ptr<BindingIter> LabelPlan::get_binding_iter() const {
    array<unique_ptr<ScanRange>, 2> ranges;
    if (node_assigned) {
        ranges[0] = ScanRange::get(node, node_assigned);
        ranges[1] = ScanRange::get(label, label_assigned);
        return make_unique<IndexScan<2>>(*quad_model.node_label, std::move(ranges));
    } else {
        ranges[0] = ScanRange::get(label, label_assigned);
        ranges[1] = ScanRange::get(node, node_assigned);
        return make_unique<IndexScan<2>>(*quad_model.label_node, std::move(ranges));
    }
}


bool LabelPlan::get_leapfrog_iter(std::vector<std::unique_ptr<LeapfrogIter>>& leapfrog_iters,
                                  vector<VarId>&                              var_order,
                                  uint_fast32_t&                              enumeration_level) const
{
    vector<unique_ptr<ScanRange>> initial_ranges;
    vector<VarId> intersection_vars;
    vector<VarId> enumeration_vars;

    // index = INT32_MAX means enumeration, index = -1 means term
    int_fast32_t node_index, label_index;

    // Assign node_index
    if (node.is_OID() || node_assigned) {
        node_index = -1;
    } else {
        node_index = INT32_MAX;
    }

    // Assign label_index
    if (label.is_OID() || label_assigned) {
        label_index = -1;
    } else {
        label_index = INT32_MAX;
    }

    // search for vars marked as enumeration (INT32_MAX) that are intersection
    // and assign them the correct index
    for (size_t i = 0; i < enumeration_level; i++) {
        if (node_index == INT32_MAX && node.get_var() == var_order[i]) {
            node_index = i;
        }
        if (label_index == INT32_MAX && label.get_var() == var_order[i]) {
            label_index = i;
        }
    }

    auto assign = [&initial_ranges, &enumeration_vars, &intersection_vars]
                  (int_fast32_t& index, Id id)
                  -> void
    {
        if (index == -1) {
            initial_ranges.push_back(ScanRange::get(id, true));
        } else if (index == INT32_MAX) {
            enumeration_vars.push_back(id.get_var());
        } else {
            intersection_vars.push_back(id.get_var());
        }
    };

    // node_label
    if (node_index <= label_index) {
        assign(node_index,  node);
        assign(label_index, label);

        leapfrog_iters.push_back(make_unique<LeapfrogBptIter<2>>(
            &get_query_ctx().thread_info.interruption_requested,
            *quad_model.node_label,
            std::move(initial_ranges),
            std::move(intersection_vars),
            std::move(enumeration_vars)
        ));
        return true;
    }
    // to_type_from_edge
    else {
        assign(label_index, label);
        assign(node_index,  node);

        leapfrog_iters.push_back(make_unique<LeapfrogBptIter<2>>(
            &get_query_ctx().thread_info.interruption_requested,
            *quad_model.label_node,
            std::move(initial_ranges),
            std::move(intersection_vars),
            std::move(enumeration_vars)
        ));
        return true;
    }
}

#include "edge_plan.h"

#include "graph_models/quad_model/quad_model.h"
#include "query/executor/binding_iter/edge_table_lookup.h"
#include "query/executor/binding_iter/index_scan.h"
#include "storage/index/leapfrog/leapfrog_bpt_iter.h"
#include "storage/index/leapfrog/leapfrog_edge_table_iter.h"

using namespace std;

EdgePlan::EdgePlan(Id from, Id to, Id type, Id edge) :
    from(from),
    to(to),
    type(type),
    edge(edge),
    from_assigned(from.is_OID()),
    to_assigned(to.is_OID()),
    type_assigned(type.is_OID()),
    edge_assigned(edge.is_OID()) { }


void EdgePlan::print(std::ostream& os, int indent) const {
    for (int i = 0; i < indent; ++i) {
        os << ' ';
    }
    os << "Edge(";
    os << "from: " << from;
    os << ", to: " << to;
    os << ", type: " << type;
    os << ", edge: " << edge;
    os << ")";

    os << ",\n";
    for (int i = 0; i < indent; ++i) {
        os << ' ';
    }
    os << "  ↳ Estimated factor: " << estimate_output_size();
}


double EdgePlan::estimate_cost() const {
    // return 1 + estimate_output_size();
    // d: cost of traveling down the B+Tree;
    // t: cost per tuple;
    // n: estimated output size
    // return d + t*n;
    return /*100.0 +*/ estimate_output_size();
}


double EdgePlan::estimate_output_size() const {
    const auto total_connections = static_cast<double>(quad_model.catalog.edge_count);

    double heuristic_divisor = 1.0;

    if (from_assigned)
        heuristic_divisor += 1.0;
    if (to_assigned)
        heuristic_divisor += 1.0;
    if (type_assigned)
        heuristic_divisor += 2.0;

    if (edge_assigned) {
        return 1.0 / heuristic_divisor;
    }

    // check for special cases
    if (from == to) {
        if (from == type) {
            // from == to == type
            if (type.is_OID()) {
                double count = quad_model.catalog.equal_from_to_type_with_type(type.get_OID().id);
                return count;
            } else {
                double count = quad_model.catalog.equal_from_to_type_count;
                return count / heuristic_divisor;
            }
        } else {
            // from == to
            if (type.is_OID()) {
                double count = quad_model.catalog.equal_from_to_with_type(type.get_OID().id);
                return count / heuristic_divisor;
            } else {
                double count = quad_model.catalog.equal_from_to_count;
                return count / heuristic_divisor;
            }
        }
    } else if (from == type) {
        if (type.is_OID()) {
            double count = quad_model.catalog.equal_from_type_with_type(type.get_OID().id);
            return count / heuristic_divisor;
        } else {
            double count = quad_model.catalog.equal_from_type_count;
            return count / heuristic_divisor;
        }
    } else if (to == type) {
        if (type.is_OID()) {
            double count = quad_model.catalog.equal_to_type_with_type(type.get_OID().id);
            return count / heuristic_divisor;
        } else {
            double count = quad_model.catalog.equal_to_type_count;
            return count / heuristic_divisor;
        }
    } else if (type_assigned) { // end special cases
        if (type.is_OID()) {
            double count = quad_model.catalog.connections_with_type(type.get_OID().id);
            return count / heuristic_divisor;
        } else {
            return total_connections / heuristic_divisor;
        }
    } else {
        return total_connections / heuristic_divisor;
    }
}


void EdgePlan::set_input_vars(const std::set<VarId>& input_vars) {
    set_input_var(input_vars, from, &from_assigned);
    set_input_var(input_vars, to, &to_assigned);
    set_input_var(input_vars, type, &type_assigned);
    set_input_var(input_vars, edge, &edge_assigned);
}


std::set<VarId> EdgePlan::get_vars() const {
    std::set<VarId> result;
    if (from.is_var() && !from_assigned) {
        result.insert(from.get_var());
    }
    if (to.is_var() && !to_assigned) {
        result.insert(to.get_var());
    }
    if (type.is_var() && !type_assigned) {
        result.insert(type.get_var());
    }
    if (edge.is_var() && !edge_assigned) {
        result.insert(edge.get_var());
    }

    return result;
}


/** FTYE | TYFE | YFTE
 * ╔═╦══════════════╦════════════╦═══════════════╦═════════════════╦══════════╗
 * ║ ║ FromAssigned ║ ToAssigned ║ tYpeAssigned  ║  EdgeAssigned   ║  index   ║
 * ╠═╬══════════════╬════════════╬═══════════════╬═════════════════╬══════════╣
 * ║1║      yes     ║     yes    ║      yes      ║      no         ║  YFTE    ║
 * ║2║      yes     ║     yes    ║      no       ║      no         ║  FTYE    ║
 * ║3║      yes     ║     no     ║      yes      ║      no         ║  YFTE    ║
 * ║4║      yes     ║     no     ║      no       ║      no         ║  FTYE    ║
 * ║5║      no      ║     yes    ║      yes      ║      no         ║  TYFE    ║
 * ║6║      no      ║     yes    ║      no       ║      no         ║  TYFE    ║
 * ║7║      no      ║     no     ║      yes      ║      no         ║  YFTE    ║
 * ║8║      no      ║     no     ║      no       ║      no         ║  YFTE    ║
 * ║9║      *       ║     *      ║      *        ║      yes        ║  table   ║
 * ╚═╩══════════════╩════════════╩═══════════════╩═════════════════╩══════════╝
 */
unique_ptr<BindingIter> EdgePlan::get_binding_iter() const {
    if (edge_assigned) {
        return make_unique<EdgeTableLookup>(
            *quad_model.edge_table,
            edge,
            from,
            to,
            type,
            from_assigned,
            to_assigned,
            type_assigned
        );
    }
    // check for special cases
    if (from == to) {
        if (from == type) {
            // equal_from_to_type
            array<unique_ptr<ScanRange>, 2> ranges;
            ranges[0] = ScanRange::get(from, from_assigned);
            ranges[1] = ScanRange::get(edge, edge_assigned);
            return make_unique<IndexScan<2>>(*quad_model.equal_from_to_type, std::move(ranges));
        } else {
            // equal_from_to
            array<unique_ptr<ScanRange>, 3> ranges;
            ranges[2] = ScanRange::get(edge, edge_assigned);
            if (type_assigned) {
                ranges[0] = ScanRange::get(type, type_assigned);
                ranges[1] = ScanRange::get(from, from_assigned);
                return make_unique<IndexScan<3>>(*quad_model.equal_from_to_inverted, std::move(ranges));
            } else {
                ranges[0] = ScanRange::get(from, from_assigned);
                ranges[1] = ScanRange::get(type, type_assigned);
                return make_unique<IndexScan<3>>(*quad_model.equal_from_to, std::move(ranges));
            }
        }
    } else if (to == type) {
        // equal_to_type
        array<unique_ptr<ScanRange>, 3> ranges;
        ranges[2] = ScanRange::get(edge, edge_assigned);
        if (from_assigned) {
            ranges[0] = ScanRange::get(from, from_assigned);
            ranges[1] = ScanRange::get(to, to_assigned);
            return make_unique<IndexScan<3>>(*quad_model.equal_to_type_inverted, std::move(ranges));
        } else {
            ranges[0] = ScanRange::get(to, to_assigned);
            ranges[1] = ScanRange::get(from, from_assigned);
            return make_unique<IndexScan<3>>(*quad_model.equal_to_type, std::move(ranges));
        }
    } else if (from == type) {
        // equal_from_type
        array<unique_ptr<ScanRange>, 3> ranges;
        ranges[2] = ScanRange::get(edge, edge_assigned);
        if (to_assigned) {
            ranges[0] = ScanRange::get(to, to_assigned);
            ranges[1] = ScanRange::get(from, from_assigned);
            return make_unique<IndexScan<3>>(*quad_model.equal_from_type_inverted, std::move(ranges));
        } else {
            ranges[0] = ScanRange::get(from, from_assigned);
            ranges[1] = ScanRange::get(to, to_assigned);
            return make_unique<IndexScan<3>>(*quad_model.equal_from_type, std::move(ranges));
        }
    } else {
        // No special case
        array<unique_ptr<ScanRange>, 4> ranges;
        ranges[3] = ScanRange::get(edge, edge_assigned);

        if (from_assigned) {
            if (type_assigned) { // CASES 1 and 3 => YFTE
                // cout << "using type from to edge\n";
                ranges[0] = ScanRange::get(type, type_assigned);
                ranges[1] = ScanRange::get(from, from_assigned);
                ranges[2] = ScanRange::get(to, to_assigned);

                return make_unique<IndexScan<4>>(*quad_model.type_from_to_edge, std::move(ranges));
            } else { // CASES 2 and 4 => FTYE
                // cout << "using from to type edge\n";
                ranges[0] = ScanRange::get(from, from_assigned);
                ranges[1] = ScanRange::get(to, to_assigned);
                ranges[2] = ScanRange::get(type, type_assigned);

                return make_unique<IndexScan<4>>(*quad_model.from_to_type_edge, std::move(ranges));
            }
        } else {
            if (to_assigned) { // CASES 5 and 6 => TYFE
                // cout << "using to type from edge\n";
                ranges[0] = ScanRange::get(to, to_assigned);
                ranges[1] = ScanRange::get(type, type_assigned);
                ranges[2] = ScanRange::get(from, from_assigned);

                return make_unique<IndexScan<4>>(*quad_model.to_type_from_edge, std::move(ranges));
            } else { // CASES 7 and 8 => YFTE
                // cout << "using type from to edge\n";
                ranges[0] = ScanRange::get(type, type_assigned);
                ranges[1] = ScanRange::get(from, from_assigned);
                ranges[2] = ScanRange::get(to, to_assigned);

                return make_unique<IndexScan<4>>(*quad_model.type_from_to_edge, std::move(ranges));
            }
        }
    }
}


bool EdgePlan::get_leapfrog_iter(
    std::vector<std::unique_ptr<LeapfrogIter>>& leapfrog_iters,
    vector<VarId>& var_order,
    uint_fast32_t& enumeration_level
) const {
    // TODO: support special cases
    if ((from.is_var() && from == to) || (from.is_var() && from == type)
        || (to.is_var() && to == type)
        // TODO: these cases are trivially empty
        || (from.is_var() && from == edge) || (to.is_var() && to == edge) || (type.is_var() && type == edge))
    {
        return false;
    }

    vector<unique_ptr<ScanRange>> initial_ranges;
    vector<VarId> intersection_vars;
    vector<VarId> enumeration_vars;

    // index = INT32_MAX means enumeration, index = -1 means term or assigned_var
    int_fast32_t from_index, to_index, type_index, edge_index;

    auto assign_index = [](int_fast32_t& index, const Id& id, bool assigned) -> void {
        if (!id.is_var() || assigned) {
            index = -1;
        } else {
            index = INT32_MAX;
        }
    };

    assign_index(from_index, from, from_assigned);
    assign_index(to_index, to, to_assigned);
    assign_index(type_index, type, type_assigned);
    assign_index(edge_index, edge, edge_assigned);

    // search for vars marked as enumeration (INT32_MAX) that are intersection
    // and assign them the correct index
    for (size_t i = 0; i < enumeration_level; i++) {
        if (from_index == INT32_MAX && from.get_var() == var_order[i]) {
            from_index = i;
        }
        if (to_index == INT32_MAX && to.get_var() == var_order[i]) {
            to_index = i;
        }
        if (type_index == INT32_MAX && type.get_var() == var_order[i]) {
            type_index = i;
        }
        if (edge_index == INT32_MAX && edge.get_var() == var_order[i]) {
            edge_index = i;
        }
    }

    auto assign =
        [&initial_ranges, &enumeration_vars, &intersection_vars](int_fast32_t& index, Id id) -> void {
        if (index == -1) {
            initial_ranges.push_back(ScanRange::get(id, true));
        } else if (index == INT32_MAX) {
            enumeration_vars.push_back(id.get_var());
        } else {
            intersection_vars.push_back(id.get_var());
        }
    };

    // Using the corresponding indexes from the edge table
    const uint_fast32_t FROM = 0;
    const uint_fast32_t TO = 1;
    const uint_fast32_t TYPE = 2;

    // check if we can use the table
    if (edge_index <= from_index && edge_index <= to_index && edge_index <= type_index) {
        assign(edge_index, edge);
        std::array<uint_fast32_t, 3> permutation;

        // from to type
        if (from_index <= to_index && to_index <= type_index) {
            assign(from_index, from);
            assign(to_index, to);
            assign(type_index, type);
            permutation = { FROM, TO, TYPE };
        }
        // from type to
        else if (from_index <= type_index && type_index <= to_index)
        {
            assign(from_index, from);
            assign(type_index, type);
            assign(to_index, to);
            permutation = { FROM, TYPE, TO };
        }

        // to from type
        else if (to_index <= from_index && from_index <= type_index)
        {
            assign(to_index, to);
            assign(from_index, from);
            assign(type_index, type);
            permutation = { TO, FROM, TYPE };
        }

        // to type from
        else if (to_index <= type_index && type_index <= from_index)
        {
            assign(to_index, to);
            assign(type_index, type);
            assign(from_index, from);
            permutation = { TO, TYPE, FROM };
        }

        // type from to
        else if (type_index <= from_index && from_index <= to_index)
        {
            assign(type_index, type);
            assign(from_index, from);
            assign(to_index, to);
            permutation = { TYPE, FROM, TO };
        }

        // type to from
        else /*if (type_index <= to_index && to_index <= from_index)*/
        {
            assign(type_index, type);
            assign(to_index, to);
            assign(from_index, from);
            permutation = { TYPE, TO, FROM };
        }

        leapfrog_iters.push_back(make_unique<LeapfrogEdgeTableIter>(
            &get_query_ctx().thread_info.interruption_requested,
            *quad_model.edge_table,
            std::move(initial_ranges),
            std::move(intersection_vars),
            std::move(enumeration_vars),
            std::move(permutation)
        ));
        return true;
    }

    auto get_iter_from_quad = [&initial_ranges, &enumeration_vars, &intersection_vars](BPlusTree<4>& bpt
                              ) -> unique_ptr<LeapfrogIter> {
        return make_unique<LeapfrogBptIter<4>>(
            &get_query_ctx().thread_info.interruption_requested,
            bpt,
            std::move(initial_ranges),
            std::move(intersection_vars),
            std::move(enumeration_vars)
        );
    };

    // from_to_type_edge
    if (from_index <= to_index && to_index <= type_index && type_index <= edge_index) {
        assign(from_index, from);
        assign(to_index, to);
        assign(type_index, type);
        assign(edge_index, edge);
        leapfrog_iters.push_back(get_iter_from_quad(*quad_model.from_to_type_edge));
        return true;
    }
    // to_type_from_edge
    else if (to_index <= type_index && type_index <= from_index && from_index <= edge_index)
    {
        assign(to_index, to);
        assign(type_index, type);
        assign(from_index, from);
        assign(edge_index, edge);
        leapfrog_iters.push_back(get_iter_from_quad(*quad_model.to_type_from_edge));
        return true;
    }
    // type_from_to_edge
    else if (type_index <= from_index && from_index <= to_index && to_index <= edge_index)
    {
        assign(type_index, type);
        assign(from_index, from);
        assign(to_index, to);
        assign(edge_index, edge);
        leapfrog_iters.push_back(get_iter_from_quad(*quad_model.type_from_to_edge));
        return true;
    }
    // type_to_from_edge
    else if (type_index <= to_index && to_index <= from_index && from_index <= edge_index)
    {
        assign(type_index, type);
        assign(to_index, to);
        assign(from_index, from);
        assign(edge_index, edge);
        leapfrog_iters.push_back(get_iter_from_quad(*quad_model.type_to_from_edge));
        return true;
        // }
        // edge is intersection (and can't go as first)
        // else if (edge_index != INT32_MAX) {
        //     cout << "from_index: " << from_index << "\n";
        //     cout << "to_index: " << to_index << "\n";
        //     cout << "type_index: " << type_index << "\n";
        //     cout << "edge_index: " << edge_index << "\n";
        //     if (from_index < edge_index) {
        //         cout << "Usando nuevo indice from_edge\n";
        //         // FROM/EDGE
        //         assign(from_index, from);
        //         assign(edge_index, edge);
        //         leapfrog_iters.push_back(make_unique<LeapfrogBptIter<2>>(
        //             &thread_info.interruption_requested,
        //             *quad_model.from_edge,
        //             move(initial_ranges),
        //             move(intersection_vars),
        //             move(enumeration_vars)
        //         ));
        //         initial_ranges.clear();
        //         intersection_vars.clear();
        //         enumeration_vars.clear();
        //         if (!initial_ranges.empty() || !intersection_vars.empty() || !enumeration_vars.empty()) {
        //             cout << "Suposicion erronea con move vector\n";
        //         }
        //     } else {
        //         // EDGE/FROM
        //         assign(edge_index, edge);
        //         assign(from_index, from);
        //         leapfrog_iters.push_back(make_unique<LeapfrogEdgeTableIter>(
        //             &thread_info.interruption_requested,
        //             *quad_model.edge_table,
        //             move(initial_ranges),
        //             move(intersection_vars),
        //             move(enumeration_vars),
        //             std::array<uint_fast32_t, 3> { FROM, FROM, FROM }
        //         ));
        //     }
        //     if (to_index < edge_index) {
        //         cout << "Usando nuevo indice to_edge\n";
        //         // TO/EDGE
        //         assign(to_index, to);
        //         assign(edge_index, edge);
        //         leapfrog_iters.push_back(make_unique<LeapfrogBptIter<2>>(
        //             &thread_info.interruption_requested,
        //             *quad_model.to_edge,
        //             move(initial_ranges),
        //             move(intersection_vars),
        //             move(enumeration_vars)
        //         ));
        //         initial_ranges.clear();
        //         intersection_vars.clear();
        //         enumeration_vars.clear();
        //         if (!initial_ranges.empty() || !intersection_vars.empty() || !enumeration_vars.empty()) {
        //             cout << "Suposicion erronea con move vector\n";
        //         }
        //     } else {
        //         // EDGE/TO
        //         assign(edge_index, edge);
        //         assign(to_index, to);

        //         leapfrog_iters.push_back(make_unique<LeapfrogEdgeTableIter>(
        //             &thread_info.interruption_requested,
        //             *quad_model.edge_table,
        //             move(initial_ranges),
        //             move(intersection_vars),
        //             move(enumeration_vars),
        //             std::array<uint_fast32_t, 3> { TO, TO, TO }
        //         ));
        //     }
        //     if (type_index < edge_index) {
        //         // TYPE/EDGE
        //         cout << "Usando nuevo indice type_edge\n";
        //         assign(type_index, type);
        //         assign(edge_index, edge);
        //         leapfrog_iters.push_back(make_unique<LeapfrogBptIter<2>>(
        //             &thread_info.interruption_requested,
        //             *quad_model.type_edge,
        //             move(initial_ranges),
        //             move(intersection_vars),
        //             move(enumeration_vars)
        //         ));
        //         initial_ranges.clear();
        //         intersection_vars.clear();
        //         enumeration_vars.clear();
        //         if (!initial_ranges.empty() || !intersection_vars.empty() || !enumeration_vars.empty()) {
        //             cout << "Suposicion erronea con move vector\n";
        //         }
        //     } else {
        //         // EDGE/TYPE
        //         assign(edge_index, edge);
        //         assign(type_index, type);

        //         leapfrog_iters.push_back(make_unique<LeapfrogEdgeTableIter>(
        //             &thread_info.interruption_requested,
        //             *quad_model.edge_table,
        //             move(initial_ranges),
        //             move(intersection_vars),
        //             move(enumeration_vars),
        //             std::array<uint_fast32_t, 3> { TYPE, TYPE, TYPE }
        //         ));
        //     }
        //     var_order.push_back(std::get<VarId>(edge));
        //     return true;
    } else {
        // cout << "no order for leapfrog\n"
        //      << "from: " << from_index
        //      << ", to: " << to_index
        //      << ", type: " << type_index
        //      << ", edge: " << edge_index
        //      << "\n";
        return false;
    }
}

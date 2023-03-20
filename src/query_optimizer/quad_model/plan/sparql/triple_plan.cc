#include "triple_plan.h"

#include <cassert>

#include "execution/binding_id_iter/index_scan.h"
#include "storage/index/leapfrog/leapfrog_bpt_iter.h"
#include "query_optimizer/rdf_model/rdf_model.h"

using namespace std;

TriplePlan::TriplePlan(Id subject, Id predicate, Id object) :
    subject            (subject),
    predicate          (predicate),
    object             (object),
    subject_assigned   (std::holds_alternative<ObjectId>(subject)),
    predicate_assigned (std::holds_alternative<ObjectId>(predicate)),
    object_assigned    (std::holds_alternative<ObjectId>(object)) { }


void TriplePlan::print(std::ostream& os, int indent, const std::vector<std::string>& var_names) const {
    for (int i = 0; i < indent; ++i) {
        os << ' ';
    }
    os << "Triple(";
    if (std::holds_alternative<ObjectId>(subject)) {
        os << "subject: " << rdf_model.get_graph_object(std::get<ObjectId>(subject));
    } else {
        os << "subject: " << var_names[std::get<VarId>(subject).id];
    }

    if (std::holds_alternative<ObjectId>(predicate)) {
        os << ", predicate: " << rdf_model.get_graph_object(std::get<ObjectId>(predicate));
    } else {
        os << ", predicate: " << var_names[std::get<VarId>(predicate).id];
    }

    if (std::holds_alternative<ObjectId>(object)) {
        os << ", object: " << rdf_model.get_graph_object(std::get<ObjectId>(object));
    } else {
        os << ", object: " << var_names[std::get<VarId>(object).id];
    }

    os << ")";

    os << ",\n";
    for (int i = 0; i < indent; ++i) {
        os << ' ';
    }
    os << "  ↳ Estimated factor: " << estimate_output_size();
}


double TriplePlan::estimate_cost() const {
    return /*100.0 +*/ estimate_output_size();
}


double TriplePlan::estimate_output_size() const {
    const auto total_triples       = static_cast<double>(rdf_model.catalog().triples_count);
    const auto distinct_subjects   = static_cast<double>(rdf_model.catalog().distinct_subjects);
    const auto distinct_predicates = static_cast<double>(rdf_model.catalog().distinct_predicates);
    const auto distinct_objects    = static_cast<double>(rdf_model.catalog().distinct_objects);

    // Avoid division by zero
    if (distinct_subjects == 0 || distinct_predicates == 0 || distinct_objects == 0) {
        return 0.0;
        // All elements assigned
    } else if (subject_assigned && predicate_assigned && object_assigned) {
        double predicate_count =
          static_cast<double>(rdf_model.catalog().predicate2total_count[std::get<ObjectId>(predicate).id]);
        return predicate_count / (distinct_subjects * distinct_objects);
        // Two elements assigned
    } else if (subject_assigned && predicate_assigned) {
        double predicate_count =
          static_cast<double>(rdf_model.catalog().predicate2total_count[std::get<ObjectId>(predicate).id]);
        return predicate_count / distinct_subjects;
    } else if (subject_assigned && object_assigned) {
        return total_triples / (distinct_subjects * distinct_objects);
    } else if (predicate_assigned && object_assigned) {
        double predicate_count =
          static_cast<double>(rdf_model.catalog().predicate2total_count[std::get<ObjectId>(predicate).id]);
        return predicate_count / distinct_objects;
        // One element assigned
    } else if (subject_assigned) {
        return total_triples / distinct_subjects;
    } else if (predicate_assigned) {
        double predicate_count =
          static_cast<double>(rdf_model.catalog().predicate2total_count[std::get<ObjectId>(predicate).id]);
        return predicate_count;
    } else if (object_assigned) {
        return total_triples / distinct_objects;
        // Any element assigned
    } else {
        return total_triples;
    }
}


void TriplePlan::set_input_vars(const std::set<VarId>& input_vars) {
    set_input_var(input_vars, subject,   &subject_assigned);
    set_input_var(input_vars, predicate, &predicate_assigned);
    set_input_var(input_vars, object,    &object_assigned);
}


std::set<VarId> TriplePlan::get_vars() const {
    std::set<VarId> result;
    if (std::holds_alternative<VarId>(subject) && !subject_assigned) {
        result.insert(std::get<VarId>(subject));
    }
    if (std::holds_alternative<VarId>(predicate) && !predicate_assigned) {
        result.insert(std::get<VarId>(predicate));
    }
    if (std::holds_alternative<VarId>(object) && !object_assigned) {
        result.insert(std::get<VarId>(object));
    }

    return result;
}


/**
 * ╔═╦══════════════════╦═══════════════════╦══════════════════╦═════════╗
 * ║ ║  SubjectAssigned ║ PredicateAssigned ║  ObjectAssigned  ║  Index  ║
 * ╠═╬══════════════════╬═══════════════════╬══════════════════╬═════════╣
 * ║1║       yes        ║        yes        ║        yes       ║   SPO   ║
 * ║2║       yes        ║        yes        ║        no        ║   SPO   ║
 * ║3║       yes        ║        no         ║        yes       ║   OSP   ║
 * ║4║       yes        ║        no         ║        no        ║   SPO   ║
 * ║5║       no         ║        yes        ║        yes       ║   POS   ║
 * ║6║       no         ║        yes        ║        no        ║   POS   ║
 * ║7║       no         ║        no         ║        yes       ║   OSP   ║
 * ║8║       no         ║        no         ║        no        ║   SPO   ║
 * ╚═╩══════════════════╩═══════════════════╩══════════════════╩═════════╝
 */
unique_ptr<BindingIdIter> TriplePlan::get_binding_id_iter(ThreadInfo* thread_info) const {
    array<unique_ptr<ScanRange>, 3> ranges;

    // Check for special cases
    if (subject == predicate) {
        if (subject == object) {
            // equal_spo
            array<unique_ptr<ScanRange>, 1> ranges;
            ranges[0] = ScanRange::get(subject, subject_assigned);
            return make_unique<IndexScan<1>>(*rdf_model.equal_spo, thread_info, move(ranges));
        } else {
            // equal_sp
            array<unique_ptr<ScanRange>, 2> ranges;
            if (object_assigned) {
                ranges[0] = ScanRange::get(object, object_assigned);
                ranges[1] = ScanRange::get(subject, subject_assigned);
                return make_unique<IndexScan<2>>(*rdf_model.equal_sp_inverted, thread_info, move(ranges));
            } else {
                ranges[0] = ScanRange::get(subject, subject_assigned);
                ranges[1] = ScanRange::get(object, object_assigned);
                return make_unique<IndexScan<2>>(*rdf_model.equal_sp, thread_info, move(ranges));
            }
        }
    } else if (subject == object) {
        // equal_so
        array<unique_ptr<ScanRange>, 2> ranges;
        if (predicate_assigned) {
            ranges[0] = ScanRange::get(predicate, predicate_assigned);
            ranges[1] = ScanRange::get(subject, subject_assigned);
            return make_unique<IndexScan<2>>(*rdf_model.equal_so_inverted, thread_info, move(ranges));
        } else {
            ranges[0] = ScanRange::get(subject, subject_assigned);
            ranges[1] = ScanRange::get(predicate, predicate_assigned);
            return make_unique<IndexScan<2>>(*rdf_model.equal_so, thread_info, move(ranges));
        }
    } else if (predicate == object) {
        // equal_po
        array<unique_ptr<ScanRange>, 2> ranges;
        if (subject_assigned) {
            ranges[0] = ScanRange::get(subject, subject_assigned);
            ranges[1] = ScanRange::get(predicate, predicate_assigned);
            return make_unique<IndexScan<2>>(*rdf_model.equal_po_inverted, thread_info, move(ranges));
        } else {
            ranges[0] = ScanRange::get(predicate, predicate_assigned);
            ranges[1] = ScanRange::get(subject, subject_assigned);
            return make_unique<IndexScan<2>>(*rdf_model.equal_po, thread_info, move(ranges));
        }
    } else {
        // No special case
        if (subject_assigned) {
            // 1-4
            if (predicate_assigned) {
                // 1-2
                ranges[0] = ScanRange::get(subject, subject_assigned);
                ranges[1] = ScanRange::get(predicate, predicate_assigned);
                ranges[2] = ScanRange::get(object, object_assigned);
                return make_unique<IndexScan<3>>(*rdf_model.spo, thread_info, move(ranges));
            } else {
                // 3-4
                if (object_assigned) {
                    // 3
                    ranges[0] = ScanRange::get(object, object_assigned);
                    ranges[1] = ScanRange::get(subject, subject_assigned);
                    ranges[2] = ScanRange::get(predicate, predicate_assigned);
                    return make_unique<IndexScan<3>>(*rdf_model.osp, thread_info, move(ranges));
                } else {
                    // 4
                    ranges[0] = ScanRange::get(subject, subject_assigned);
                    ranges[1] = ScanRange::get(predicate, predicate_assigned);
                    ranges[2] = ScanRange::get(object, object_assigned);
                    return make_unique<IndexScan<3>>(*rdf_model.spo, thread_info, move(ranges));
                }
            }
        } else {
            // 5-8
            if (predicate_assigned) {
                // 5-6
                ranges[0] = ScanRange::get(predicate, predicate_assigned);
                ranges[1] = ScanRange::get(object, object_assigned);
                ranges[2] = ScanRange::get(subject, subject_assigned);
                return make_unique<IndexScan<3>>(*rdf_model.pos, thread_info, move(ranges));
            } else {
                // 7-8
                if (object_assigned) {
                    // 7
                    ranges[0] = ScanRange::get(object, object_assigned);
                    ranges[1] = ScanRange::get(subject, subject_assigned);
                    ranges[2] = ScanRange::get(predicate, predicate_assigned);
                    return make_unique<IndexScan<3>>(*rdf_model.osp, thread_info, move(ranges));
                } else {
                    // 8
                    ranges[0] = ScanRange::get(subject, subject_assigned);
                    ranges[1] = ScanRange::get(predicate, predicate_assigned);
                    ranges[2] = ScanRange::get(object, object_assigned);
                    return make_unique<IndexScan<3>>(*rdf_model.spo, thread_info, move(ranges));
                }
            }
        }
    }
    return nullptr;
}


bool TriplePlan::get_leapfrog_iter(ThreadInfo*                       thread_info,
                                   vector<unique_ptr<LeapfrogIter>>& leapfrog_iters,
                                   vector<VarId>&                    var_order,
                                   uint_fast32_t&                    enumeration_level) const
{
    // TODO: support special cases
    if (   (std::holds_alternative<VarId>(subject)  && subject   == predicate)
        || (std::holds_alternative<VarId>(subject)  && subject   == object)
        || (std::holds_alternative<VarId>(predicate) && predicate == object))
    {
        return false;
    }

    vector<unique_ptr<ScanRange>> initial_ranges;
    vector<VarId> intersection_vars;
    vector<VarId> enumeration_vars;

    int_fast32_t subject_index, predicate_index, object_index;

    auto assign_index = [] (int_fast32_t& index, const Id& id, bool assigned) -> void {
        if (std::holds_alternative<ObjectId>(id) || assigned) {
            index = -1;
        } else {
            index = INT32_MAX;
        }
    };

    assign_index(subject_index,   subject,   subject_assigned);
    assign_index(predicate_index, predicate, predicate_assigned);
    assign_index(object_index,    object,    object_assigned);

    // search for vars marked as enumeraion (INT32_MAX) that are intersection
    // and assign them the correct index
    for (size_t i = 0; i < enumeration_level; i++) {
        if (subject_index == INT32_MAX && std::get<VarId>(subject) == var_order[i]) {
            subject_index = i;
        }
        if (predicate_index == INT32_MAX && std::get<VarId>(predicate) == var_order[i]) {
            predicate_index = i;
        }
        if (object_index == INT32_MAX && std::get<VarId>(object) == var_order[i]) {
            object_index = i;
        }
    }

    auto assign = [&initial_ranges, &enumeration_vars, &intersection_vars]
                  (int_fast32_t& index, Id id)
                  -> void
    {
        if (index == -1) {
            initial_ranges.push_back(ScanRange::get(id, true));
        } else if (index == INT32_MAX) {
            enumeration_vars.push_back(std::get<VarId>(id));
        } else {
            intersection_vars.push_back(std::get<VarId>(id));
        }
    };

    auto get_iter_from_triple = [&thread_info, &initial_ranges, &enumeration_vars, &intersection_vars]
                              (BPlusTree<3>& bpt) -> unique_ptr<LeapfrogIter>
    {
        return make_unique<LeapfrogBptIter<3>>(
            &thread_info->interruption_requested,
            bpt,
            move(initial_ranges),
            move(intersection_vars),
            move(enumeration_vars)
        );
    };


    // pos
    if (predicate_index <= object_index && object_index <= subject_index) {
        assign(predicate_index, predicate);
        assign(object_index,    object);
        assign(subject_index,   subject);
        leapfrog_iters.push_back(get_iter_from_triple(*rdf_model.pos));
        return true;
    }
    // pso
    else if (predicate_index <= subject_index && subject_index <= object_index) {
        assign(predicate_index, predicate);
        assign(subject_index,   subject);
        assign(object_index,    object);
        leapfrog_iters.push_back(get_iter_from_triple(*rdf_model.pso));
        return true;
    }
    // spo
    else if (subject_index <= predicate_index && predicate_index <= object_index) {
        assign(subject_index,   subject);
        assign(predicate_index, predicate);
        assign(object_index,    object);
        leapfrog_iters.push_back(get_iter_from_triple(*rdf_model.spo));
        return true;
    }
    // osp
    else if (object_index <= subject_index && subject_index <= predicate_index) {
        assign(object_index,    object);
        assign(subject_index,   subject);
        assign(predicate_index, predicate);
        leapfrog_iters.push_back(get_iter_from_triple(*rdf_model.osp));
        return true;
    }
    else {
        return false;
    }
}
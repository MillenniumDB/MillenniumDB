#include "triple_plan.h"

#include "graph_models/rdf_model/rdf_model.h"
#include "query/executor/binding_iter/index_scan.h"
#include "query/query_context.h"
#include "storage/index/leapfrog/leapfrog_bpt_iter.h"

using namespace SPARQL;

TriplePlan::TriplePlan(Id subject, Id predicate, Id object) :
    subject            (subject),
    predicate          (predicate),
    object             (object),
    subject_assigned   (subject.is_OID()),
    predicate_assigned (predicate.is_OID()),
    object_assigned    (object.is_OID()) { }


void TriplePlan::print(std::ostream& os, int indent) const {
    for (int i = 0; i < indent; ++i) {
        os << ' ';
    }
    os << "Triple(subject: " << subject;
    os << ", predicate: " << predicate;
    os << ", object: " << object;
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
    }

    // Check for special cases
    if (subject == object) {
        if (subject == predicate) {
            // S == P == O
            if (subject.is_var()) {
                return rdf_model.catalog().equal_spo_count;
            } else {
                if (subject_assigned) {
                    return rdf_model.catalog().equal_spo_count /
                           std::max<double>(distinct_subjects, std::max<double>(distinct_predicates, distinct_objects));
                } else {
                    return 1.0;
                }
            }
        } else {
            // S == O
            if (predicate.is_var()) {
                return rdf_model.catalog().equal_so_count;
            } else {
                return rdf_model.catalog().equal_so_count / distinct_predicates;
            }
        }
    } else if (subject == predicate) {
        // S == P
        if (object.is_var()) {
            return rdf_model.catalog().equal_sp_count;
        } else {
            return rdf_model.catalog().equal_sp_count / distinct_objects;
        }

    } else if (predicate == object) {
        // P == O
        if (subject.is_var()) {
            return rdf_model.catalog().equal_po_count;
        } else {
            return rdf_model.catalog().equal_po_count / distinct_subjects;
        }
    }

    // All elements assigned
    if (subject_assigned && predicate_assigned && object_assigned) {
        if (predicate.is_OID()) {
            double predicate_count = static_cast<double>(rdf_model.catalog().predicate2total_count[predicate.get_OID().id]);
            return predicate_count / (distinct_subjects * distinct_objects);
        } else {
            return 0.5;
        }
        // Two elements assigned
    } else if (subject_assigned && predicate_assigned) {
        if (predicate.is_OID()) {
            double predicate_count =
            static_cast<double>(rdf_model.catalog().predicate2total_count[predicate.get_OID().id]);
            return predicate_count / distinct_subjects;
        } else {
            return total_triples - std::max(distinct_subjects, distinct_predicates) + 1;
        }
    } else if (subject_assigned && object_assigned) {
        return total_triples / (distinct_subjects * distinct_objects);
    } else if (predicate_assigned && object_assigned) {
        if (predicate.is_OID()) {
            double predicate_count = static_cast<double>(
                rdf_model.catalog().predicate2total_count[predicate.get_OID().id]
            );
            return predicate_count / distinct_objects;
        } else {
            return total_triples - std::max(distinct_subjects, distinct_objects) + 1;
        }
        // One element assigned
    } else if (subject_assigned) {
        return total_triples / distinct_subjects;
    } else if (predicate_assigned) {
        if (predicate.is_OID()) {
            double predicate_count = static_cast<double>(
                rdf_model.catalog().predicate2total_count[predicate.get_OID().id]
            );
            return predicate_count;
        } else {
            return total_triples - distinct_predicates + 2;;
        }
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
    if (subject.is_var() && !subject_assigned) {
        result.insert(subject.get_var());
    }
    if (predicate.is_var() && !predicate_assigned) {
        result.insert(predicate.get_var());
    }
    if (object.is_var() && !object_assigned) {
        result.insert(object.get_var());
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
std::unique_ptr<BindingIter> TriplePlan::get_binding_iter() const {
    std::array<std::unique_ptr<ScanRange>, 3> ranges;

    // Check for special cases
    if (subject == predicate) {
        if (subject == object) {
            // equal_spo
            std::array<std::unique_ptr<ScanRange>, 1> ranges;
            ranges[0] = ScanRange::get(subject, subject_assigned);
            return std::make_unique<IndexScan<1>>(*rdf_model.equal_spo, std::move(ranges));
        } else {
            // equal_sp
            std::array<std::unique_ptr<ScanRange>, 2> ranges;
            if (object_assigned) {
                ranges[0] = ScanRange::get(object, object_assigned);
                ranges[1] = ScanRange::get(subject, subject_assigned);
                return std::make_unique<IndexScan<2>>(*rdf_model.equal_sp_inverted, std::move(ranges));
            } else {
                ranges[0] = ScanRange::get(subject, subject_assigned);
                ranges[1] = ScanRange::get(object, object_assigned);
                return std::make_unique<IndexScan<2>>(*rdf_model.equal_sp, std::move(ranges));
            }
        }
    } else if (subject == object) {
        // equal_so
        std::array<std::unique_ptr<ScanRange>, 2> ranges;
        if (predicate_assigned) {
            ranges[0] = ScanRange::get(predicate, predicate_assigned);
            ranges[1] = ScanRange::get(subject, subject_assigned);
            return std::make_unique<IndexScan<2>>(*rdf_model.equal_so_inverted, std::move(ranges));
        } else {
            ranges[0] = ScanRange::get(subject, subject_assigned);
            ranges[1] = ScanRange::get(predicate, predicate_assigned);
            return std::make_unique<IndexScan<2>>(*rdf_model.equal_so, std::move(ranges));
        }
    } else if (predicate == object) {
        // equal_po
        std::array<std::unique_ptr<ScanRange>, 2> ranges;
        if (subject_assigned) {
            ranges[0] = ScanRange::get(subject, subject_assigned);
            ranges[1] = ScanRange::get(predicate, predicate_assigned);
            return std::make_unique<IndexScan<2>>(*rdf_model.equal_po_inverted, std::move(ranges));
        } else {
            ranges[0] = ScanRange::get(predicate, predicate_assigned);
            ranges[1] = ScanRange::get(subject, subject_assigned);
            return std::make_unique<IndexScan<2>>(*rdf_model.equal_po, std::move(ranges));
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
                return std::make_unique<IndexScan<3>>(*rdf_model.spo, std::move(ranges));
            } else {
                // 3-4
                if (object_assigned) {
                    // 3
                    ranges[0] = ScanRange::get(object, object_assigned);
                    ranges[1] = ScanRange::get(subject, subject_assigned);
                    ranges[2] = ScanRange::get(predicate, predicate_assigned);
                    return std::make_unique<IndexScan<3>>(*rdf_model.osp, std::move(ranges));
                } else {
                    // 4
                    ranges[0] = ScanRange::get(subject, subject_assigned);
                    ranges[1] = ScanRange::get(predicate, predicate_assigned);
                    ranges[2] = ScanRange::get(object, object_assigned);
                    return std::make_unique<IndexScan<3>>(*rdf_model.spo, std::move(ranges));
                }
            }
        } else {
            // 5-8
            if (predicate_assigned) {
                // 5-6
                ranges[0] = ScanRange::get(predicate, predicate_assigned);
                ranges[1] = ScanRange::get(object, object_assigned);
                ranges[2] = ScanRange::get(subject, subject_assigned);
                return std::make_unique<IndexScan<3>>(*rdf_model.pos, std::move(ranges));
            } else {
                // 7-8
                if (object_assigned) {
                    // 7
                    ranges[0] = ScanRange::get(object, object_assigned);
                    ranges[1] = ScanRange::get(subject, subject_assigned);
                    ranges[2] = ScanRange::get(predicate, predicate_assigned);
                    return std::make_unique<IndexScan<3>>(*rdf_model.osp, std::move(ranges));
                } else {
                    // 8
                    ranges[0] = ScanRange::get(subject, subject_assigned);
                    ranges[1] = ScanRange::get(predicate, predicate_assigned);
                    ranges[2] = ScanRange::get(object, object_assigned);
                    return std::make_unique<IndexScan<3>>(*rdf_model.spo, std::move(ranges));
                }
            }
        }
    }
    return nullptr;
}


bool TriplePlan::get_leapfrog_iter(std::vector<std::unique_ptr<LeapfrogIter>>& leapfrog_iters,
                                   std::vector<VarId>&                         var_order,
                                   uint_fast32_t&                              enumeration_level) const
{
    // TODO: support special cases
    if (   (subject.is_var()   && subject   == predicate)
        || (subject.is_var()   && subject   == object)
        || (predicate.is_var() && predicate == object))
    {
        return false;
    }

    std::vector<std::unique_ptr<ScanRange>> initial_ranges;
    std::vector<VarId> intersection_vars;
    std::vector<VarId> enumeration_vars;

    int_fast32_t subject_index, predicate_index, object_index;

    auto assign_index = [] (int_fast32_t& index, const Id& id, bool assigned) -> void {
        if (id.is_OID() || assigned) {
            index = -1;
        } else {
            index = INT32_MAX;
        }
    };

    assign_index(subject_index,   subject,   subject_assigned);
    assign_index(predicate_index, predicate, predicate_assigned);
    assign_index(object_index,    object,    object_assigned);

    // search for vars marked as enumeration (INT32_MAX) that are intersection
    // and assign them the correct index
    for (uint_fast32_t i = 0; i < enumeration_level; i++) {
        if (subject_index == INT32_MAX && subject.get_var() == var_order[i]) {
            subject_index = i;
        }
        if (predicate_index == INT32_MAX && predicate.get_var() == var_order[i]) {
            predicate_index = i;
        }
        if (object_index == INT32_MAX && object.get_var() == var_order[i]) {
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
            enumeration_vars.push_back(id.get_var());
        } else {
            intersection_vars.push_back(id.get_var());
        }
    };

    auto get_iter_from_triple = [&initial_ranges, &enumeration_vars, &intersection_vars]
                              (BPlusTree<3>& bpt) -> std::unique_ptr<LeapfrogIter>
    {
        return std::make_unique<LeapfrogBptIter<3>>(
            &get_query_ctx().thread_info.interruption_requested,
            bpt,
            std::move(initial_ranges),
            std::move(intersection_vars),
            std::move(enumeration_vars)
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

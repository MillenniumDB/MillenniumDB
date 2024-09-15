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
    object_assigned    (object.is_OID())
{
    if (subject == predicate) {
        if (subject == object) {
            index = Index::EQUAL_SPO;
        } else {
            index = Index::EQUAL_SP;
        }
    } else if (subject == object) {
        index = Index::EQUAL_SO;
    } else if (predicate == object) {
        index = Index::EQUAL_PO;
    } else {
        index = Index::NORMAL;
    }
}


void TriplePlan::print(std::ostream& os, int indent) const {
    for (int i = 0; i < indent; ++i) {
        os << ' ';
    }
    os << "Triple(" << subject;
    os << ", " << predicate;
    os << ", " << object;
    os << ")";
    os << ", estimated cost: " << estimate_cost() << '\n';
}


double TriplePlan::estimate_cost() const {
    return estimate_output_size();
}


double TriplePlan::estimate_output_size() const {
    if (!cached_output_estimation_is_valid) {
        cached_output_estimation = _estimate_output_size();
        cached_output_estimation_is_valid = true;
    }

    return cached_output_estimation;
}

/* Cases
3 terms, 0 assigned, 0 non-assigned:
    search in the BPT, 1 if triple exist, 0 if not
2 terms, 1 assigned, 0 non-assigned:
    BPT estimation (forgetting the assigned) divided by H1
2 terms, 0 assigned, 1 non-assigned:
    BPT estimation
1 terms, 2 assigned, 0 non-assigned:
    - if Term is Predicate: catalog[P] divided by H2
    - else: BPT estimation (forgetting the assigned) divided by H2
1 terms, 1 assigned, 1 non-assigned:
    - if Term is Predicate: catalog[P] divided by H1
    - else: BPT estimation (forgetting the assigned) divided by H1
1 terms, 0 assigned, 2 non-assigned:
    - if Term is Predicate: catalog[P]
    - else: BPT estimation
0 terms, 3 assigned, 0 non-assigned:
    H3
0 terms, 2 assigned, 1 non-assigned:
    catalog count divided by H2
0 terms, 1 assigned, 2 non-assigned:
    catalog count divided by H1
0 terms, 0 assigned, 3 non-assigned:
    catalog count

*/
double TriplePlan::_estimate_output_size() const {
    constexpr double H1 = 0.01;      // heuristic probability for 1 assigned var
    constexpr double H2 = 0.00001;   // heuristic probability for 2 assigned vars
    constexpr double H3 = 0.0000001; // heuristic probability for 3 assigned vars

    int terms = 0;
    int assigned_vars = 0;

    terms += static_cast<int>(subject.is_OID());
    terms += static_cast<int>(predicate.is_OID());
    terms += static_cast<int>(object.is_OID());

    assigned_vars += static_cast<int>(subject.is_var() && subject_assigned);
    assigned_vars += static_cast<int>(predicate.is_var() && predicate_assigned);
    assigned_vars += static_cast<int>(object.is_var() && object_assigned);

    const auto& catalog = rdf_model.catalog();

    switch (terms) {
    case 3: {
        Record<3> min = {
            subject.get_OID().id,
            predicate.get_OID().id,
            object.get_OID().id
        };

        Record<3> max = min;

        bool interruption_requested = false;
        auto it = rdf_model.spo->get_range(&interruption_requested, min, max);
        return it.next() == nullptr ? 0 : 1;
    }
    case 2: {
        auto bpt_estimation = estimate_with_bpt();
        if (assigned_vars == 1) { // not_assigned_vars == 0
            return bpt_estimation * H1;
        } else { // assigned_vars == 0, not_assigned_vars == 1
            return bpt_estimation;
        }
    }
    case 1: {
        double predicate_count;
        if (predicate.is_OID()) {
            // auto it = catalog.predicate2total_count.find(predicate.get_OID().id);
            // if (it != catalog.predicate2total_count.end()) {
            //     predicate_count = static_cast<double>(it->second);
            // } else {
            //     predicate_count = 0;
            // }
            predicate_count = catalog.get_predicate_count(predicate.get_OID().id);
        } else {
            predicate_count = estimate_with_bpt();
        }
        switch (assigned_vars) {
        case 2:
            return predicate_count * H2;
        case 1:
            return predicate_count * H1;
        default: // 0
            return predicate_count;
        }
    }
    case 0: {
        double total_count;
        switch (index) {
        case Index::EQUAL_SPO:
            total_count = catalog.get_equal_spo_count();
            break;
        case Index::EQUAL_SP:
            total_count = catalog.get_equal_sp_count();
            break;
        case Index::EQUAL_SO:
            total_count = catalog.get_equal_so_count();
            break;
        case Index::EQUAL_PO:
            total_count = catalog.get_equal_po_count();
            break;
        default: // using default instead of Index::NORMAL to avoid gcc warning of uninitialized total_count
            total_count = catalog.get_triples_count();
            break;
        }
        switch (assigned_vars) {
        case 3:
            return H3;
        case 2:
            return total_count * H2;
        case 1:
            return total_count * H1;
        default: // case 0
            return total_count;
        }
    }
    }

    assert(false);
    return 0; // should never reach here
}


void TriplePlan::set_input_vars(const std::set<VarId>& input_vars) {
    auto previous_assigned_count = static_cast<int>(subject_assigned)
                                 + static_cast<int>(predicate_assigned)
                                 + static_cast<int>(object_assigned);

    set_input_var(input_vars, subject,   &subject_assigned);
    set_input_var(input_vars, predicate, &predicate_assigned);
    set_input_var(input_vars, object,    &object_assigned);

    auto after_assigned_count = static_cast<int>(subject_assigned)
                              + static_cast<int>(predicate_assigned)
                              + static_cast<int>(object_assigned);

    if (previous_assigned_count != after_assigned_count) {
        cached_output_estimation_is_valid = false;
    }
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


// helper method for estimate_with_bpt()
inline uint64_t get_oid(Id id, uint64_t default_value) {
    if (id.is_OID()) {
        return id.get_OID().id;
    } else {
        return default_value;
    }
}


double TriplePlan::estimate_with_bpt() const {
    Record<2> min2;
    Record<2> max2;

    BPlusTree<2>* index2_used = nullptr;

    switch (index) {
    case Index::EQUAL_SP:
        if (subject.is_OID()) {
            min2[0] = get_oid(subject, 0);
            min2[1] = get_oid(object,  0);

            max2[0] = get_oid(subject, UINT64_MAX);
            max2[1] = get_oid(object,  UINT64_MAX);
            index2_used = rdf_model.equal_sp.get();
        } else {
            min2[0] = get_oid(object,  0);
            min2[1] = get_oid(subject, 0);

            max2[0] = get_oid(object,  UINT64_MAX);
            max2[1] = get_oid(subject, UINT64_MAX);
            index2_used = rdf_model.equal_sp_inverted.get();
        }
        break;
    case Index::EQUAL_SO:
        if (subject.is_OID()) {
            min2[0] = get_oid(subject,   0);
            min2[1] = get_oid(predicate, 0);

            max2[0] = get_oid(subject,   UINT64_MAX);
            max2[1] = get_oid(predicate, UINT64_MAX);
            index2_used = rdf_model.equal_so.get();
        } else {
            min2[0] = get_oid(predicate, 0);
            min2[1] = get_oid(subject,   0);

            max2[0] = get_oid(predicate, UINT64_MAX);
            max2[1] = get_oid(subject,   UINT64_MAX);
            index2_used = rdf_model.equal_so_inverted.get();
        }
        break;
    case Index::EQUAL_PO:
        if (predicate.is_OID()) {
            min2[0] = get_oid(predicate, 0);
            min2[1] = get_oid(subject,   0);

            max2[0] = get_oid(predicate, UINT64_MAX);
            max2[1] = get_oid(subject,   UINT64_MAX);
            index2_used = rdf_model.equal_po.get();
        } else {
            min2[0] = get_oid(subject,   0);
            min2[1] = get_oid(predicate, 0);

            max2[0] = get_oid(subject,   UINT64_MAX);
            max2[1] = get_oid(predicate, UINT64_MAX);
            index2_used = rdf_model.equal_po_inverted.get();
        }
        break;
    case Index::NORMAL:
        break;
    case Index::EQUAL_SPO: // throw logic error?
        break;
    }

    if (index2_used != nullptr) {
        return index2_used->estimate_records(min2, max2);
    }

    Record<3> min;
    Record<3> max;

    BPlusTree<3>* index_used = nullptr;
    if (subject.is_OID()) {
        // 1-4
        if (predicate.is_OID()) {
            // 1-2: SPO
            min[0] = get_oid(subject,   0);
            min[1] = get_oid(predicate, 0);
            min[2] = get_oid(object,    0);

            max[0] = get_oid(subject,   UINT64_MAX);
            max[1] = get_oid(predicate, UINT64_MAX);
            max[2] = get_oid(object,    UINT64_MAX);
            index_used = rdf_model.spo.get();
        } else {
            // 3-4
            if (object.is_OID()) {
                // 3: OSP
                min[0] = get_oid(object,    0);
                min[1] = get_oid(subject,   0);
                min[2] = get_oid(predicate, 0);

                max[0] = get_oid(object,    UINT64_MAX);
                max[1] = get_oid(subject,   UINT64_MAX);
                max[2] = get_oid(predicate, UINT64_MAX);
                index_used = rdf_model.osp.get();
            } else {
                // 4: SPO
                min[0] = get_oid(subject,   0);
                min[1] = get_oid(predicate, 0);
                min[2] = get_oid(object,    0);

                max[0] = get_oid(subject,   UINT64_MAX);
                max[1] = get_oid(predicate, UINT64_MAX);
                max[2] = get_oid(object,    UINT64_MAX);
                index_used = rdf_model.spo.get();
            }
        }
    } else {
        // 5-8
        if (predicate.is_OID()) {
            // 5-6: POS
            min[0] = get_oid(predicate, 0);
            min[1] = get_oid(object,    0);
            min[2] = get_oid(subject,   0);

            max[0] = get_oid(predicate, UINT64_MAX);
            max[1] = get_oid(object,    UINT64_MAX);
            max[2] = get_oid(subject,   UINT64_MAX);
            index_used = rdf_model.pos.get();
        } else {
            // 7-8:
            if (object.is_OID()) {
                // 7: OSP
                min[0] = get_oid(object,    0);
                min[1] = get_oid(subject,   0);
                min[2] = get_oid(predicate, 0);

                max[0] = get_oid(object,    UINT64_MAX);
                max[1] = get_oid(subject,   UINT64_MAX);
                max[2] = get_oid(predicate, UINT64_MAX);
                index_used = rdf_model.osp.get();
            } else {
                // 8: SPO
                min[0] = get_oid(subject,   0);
                min[1] = get_oid(predicate, 0);
                min[2] = get_oid(object,    0);

                max[0] = get_oid(subject,   UINT64_MAX);
                max[1] = get_oid(predicate, UINT64_MAX);
                max[2] = get_oid(object,    UINT64_MAX);
                index_used = rdf_model.spo.get();
            }
        }
    }
    return index_used->estimate_records(min, max);
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


bool TriplePlan::get_leapfrog_iter(
    std::vector<std::unique_ptr<LeapfrogIter>>& leapfrog_iters,
    std::vector<VarId>& var_order,
    uint_fast32_t& enumeration_level) const
{
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

    auto get_iter2_from_triple = [&initial_ranges, &enumeration_vars, &intersection_vars]
                              (BPlusTree<2>& bpt) -> std::unique_ptr<LeapfrogIter>
    {
        return std::make_unique<LeapfrogBptIter<2>>(
            &get_query_ctx().thread_info.interruption_requested,
            bpt,
            std::move(initial_ranges),
            std::move(intersection_vars),
            std::move(enumeration_vars)
        );
    };

    switch (index) {
    case Index::NORMAL:
        break;
    case Index::EQUAL_SPO:
        assign(subject_index, subject);
        leapfrog_iters.push_back(
            std::make_unique<LeapfrogBptIter<1>>(
                &get_query_ctx().thread_info.interruption_requested,
                *rdf_model.equal_spo,
                std::move(initial_ranges),
                std::move(intersection_vars),
                std::move(enumeration_vars)
            )
        );
        return true;
    case Index::EQUAL_SP: {
        if (subject_index <= object_index) {
            assign(subject_index, subject);
            assign(object_index,  object);
            leapfrog_iters.push_back(get_iter2_from_triple(*rdf_model.equal_sp));
        } else {
            assign(object_index,  object);
            assign(subject_index, subject);
            leapfrog_iters.push_back(get_iter2_from_triple(*rdf_model.equal_sp_inverted));
        }
        return true;
    }
    case Index::EQUAL_SO: {
        if (subject_index <= predicate_index) {
            assign(subject_index,   subject);
            assign(predicate_index, predicate);
            leapfrog_iters.push_back(get_iter2_from_triple(*rdf_model.equal_so));
        } else {
            assign(predicate_index, predicate);
            assign(subject_index,   subject);
            leapfrog_iters.push_back(get_iter2_from_triple(*rdf_model.equal_so_inverted));
        }
        return true;
    }
    case Index::EQUAL_PO: {
        if (predicate_index <= subject_index) {
            assign(predicate_index, predicate);
            assign(subject_index,   subject);
            leapfrog_iters.push_back(get_iter2_from_triple(*rdf_model.equal_po));
        } else {
            assign(subject_index,   subject);
            assign(predicate_index, predicate);
            leapfrog_iters.push_back(get_iter2_from_triple(*rdf_model.equal_po_inverted));
        }
        return true;
    }
    }

    // spo
    if (subject_index <= predicate_index && predicate_index <= object_index) {
        assign(subject_index,   subject);
        assign(predicate_index, predicate);
        assign(object_index,    object);
        leapfrog_iters.push_back(get_iter_from_triple(*rdf_model.spo));
        return true;
    }
    // pos
    else if (predicate_index <= object_index && object_index <= subject_index) {
        assign(predicate_index, predicate);
        assign(object_index,    object);
        assign(subject_index,   subject);
        leapfrog_iters.push_back(get_iter_from_triple(*rdf_model.pos));
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
    // pso
    else if (rdf_model.pso != nullptr && predicate_index <= subject_index && subject_index <= object_index) {
        assign(predicate_index, predicate);
        assign(subject_index,   subject);
        assign(object_index,    object);
        leapfrog_iters.push_back(get_iter_from_triple(*rdf_model.pso));
        return true;
    }
    // sop
    else if (rdf_model.sop != nullptr && subject_index <= object_index && object_index <= predicate_index) {
        assign(subject_index,   subject);
        assign(object_index,    object);
        assign(predicate_index, predicate);
        leapfrog_iters.push_back(get_iter_from_triple(*rdf_model.sop));
        return true;
    }
    // ops
    else if (rdf_model.ops != nullptr && object_index <= predicate_index && predicate_index <= subject_index) {
        assign(object_index,    object);
        assign(predicate_index, predicate);
        assign(subject_index,   subject);
        leapfrog_iters.push_back(get_iter_from_triple(*rdf_model.ops));
        return true;
    }
    else {
        return false;
    }
}

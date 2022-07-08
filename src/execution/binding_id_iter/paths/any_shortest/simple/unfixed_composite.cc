#include "unfixed_composite.h"

#include <cassert>

#include "base/ids/var_id.h"
#include "query_optimizer/quad_model/quad_model.h"
#include "storage/index/record.h"

using namespace std;
using namespace Paths::AnyShortest;

UnfixedComposite::UnfixedComposite(ThreadInfo*  thread_info,
                                   VarId        path_var,
                                   VarId        start,
                                   VarId        end,
                                   RPQAutomaton automaton) :
    thread_info       (thread_info),
    path_var          (path_var),
    start             (start),
    end               (end),
    automaton         (automaton) { }


void UnfixedComposite::begin(BindingId& _parent_binding) {
    parent_binding = &_parent_binding;

    start_iter = quad_model.type_from_to_edge->get_range(
      &thread_info->interruption_requested,
      RecordFactory::get(automaton.from_to_connections[0][current_start_transition].type_id.id, 0, 0, 0),
      RecordFactory::get(automaton.from_to_connections[0][current_start_transition].type_id.id,
                         UINT64_MAX,
                         UINT64_MAX,
                         UINT64_MAX));
    bpt_searches++;

    current_start_record = start_iter->next();
    if (current_start_record != nullptr) {
        parent_binding->add(start, ObjectId(current_start_record->ids[1]));
        visited.emplace(current_start_record->ids[1]);
        path_enum = make_unique<BFSIterEnum>(thread_info,
                                             path_var,
                                             start,
                                             end,
                                             automaton);
        path_enum->begin(*parent_binding);
    }
}


bool UnfixedComposite::next() {
    while (true) {
        if (path_enum != nullptr && path_enum->next()) {
            results_found++;
            return true;
        } else {
            auto new_start_record = start_iter->next();
            while (new_start_record != nullptr) {
                if (new_start_record->ids[1] != current_start_record->ids[1]) {
                    break;
                } else {
                    new_start_record = start_iter->next();
                }
            }

            current_start_record = move(new_start_record);

            if (current_start_record != nullptr) {
                auto has_not_been_visited = visited.emplace(current_start_record->ids[1]).second;
                if (has_not_been_visited) {
                    parent_binding->add(start, ObjectId(current_start_record->ids[1]));
                    path_enum->reset();
                    continue;
                }
            } else {
                current_start_transition++;
                if (current_start_transition < automaton.from_to_connections[0].size()) {
                    start_iter = quad_model.type_from_to_edge->get_range(
                      &thread_info->interruption_requested,
                      RecordFactory::get(automaton.from_to_connections[0][current_start_transition].type_id.id, 0, 0, 0),
                      RecordFactory::get(automaton.from_to_connections[0][current_start_transition].type_id.id,
                                         UINT64_MAX,
                                         UINT64_MAX,
                                         UINT64_MAX));
                    bpt_searches++;
                    current_start_record = start_iter->next();
                    if (current_start_record != nullptr) {
                        auto has_not_been_visited = visited.emplace(current_start_record->ids[1]).second;
                        if (has_not_been_visited) {
                            // The initial node is fixed so that the path enum checks all transitions and thus
                            // guarantee optimal path. Fixing the transition might result in a non-optimal path,
                            // because the optimal path might be in a later transition.
                            parent_binding->add(start, ObjectId(current_start_record->ids[1]));
                            path_enum->reset();
                            continue;
                        }
                    }
                } else {
                    return false;
                }
            }
        }
    }
}


void UnfixedComposite::reset() {
    current_start_transition = 0;
    results_found            = 0;
    bpt_searches             = 0;
    start_iter               = quad_model.type_from_to_edge->get_range(
      &thread_info->interruption_requested,
      RecordFactory::get(automaton.from_to_connections[0][current_start_transition].type_id.id, 0, 0, 0),
      RecordFactory::get(automaton.from_to_connections[0][current_start_transition].type_id.id,
                         UINT64_MAX,
                         UINT64_MAX,
                         UINT64_MAX));

    current_start_record = start_iter->next();
    path_enum            = nullptr;
    if (current_start_record != nullptr) {
        parent_binding->add(start, ObjectId(current_start_record->ids[1]));
        path_enum = make_unique<BFSIterEnum>(thread_info,
                                             path_var,
                                             start,
                                             end,
                                             automaton);
        path_enum->begin(*parent_binding);
    }
}


void UnfixedComposite::assign_nulls() {
    parent_binding->add(end, ObjectId::get_null());
}


void UnfixedComposite::analyze(std::ostream& os, int indent) const {
    os << std::string(indent, ' ');
    os << "Paths::AnyShortest::UnfixedComposite(bpt_searches: " << bpt_searches << ", found: " << results_found << ")";
}

#include "path_plan.h"

#include "base/exceptions.h"
#include "execution/binding_id_iter/paths/all_shortest/bfs_check.h"
#include "execution/binding_id_iter/paths/all_shortest/bfs_enum.h"
#include "execution/binding_id_iter/paths/all_shortest_count/bfs_check.h"
#include "execution/binding_id_iter/paths/all_shortest_count/bfs_enum.h"
#include "execution/binding_id_iter/paths/all_simple/bfs_check.h"
#include "execution/binding_id_iter/paths/all_simple/bfs_enum.h"
#include "execution/binding_id_iter/paths/all_simple/dfs_check.h"
#include "execution/binding_id_iter/paths/all_simple/dfs_enum.h"
#include "execution/binding_id_iter/paths/all_trails/bfs_check.h"
#include "execution/binding_id_iter/paths/all_trails/bfs_enum.h"
#include "execution/binding_id_iter/paths/all_trails/dfs_check.h"
#include "execution/binding_id_iter/paths/all_trails/dfs_enum.h"
#include "execution/binding_id_iter/paths/any/dfs_check.h"
#include "execution/binding_id_iter/paths/any/dfs_enum.h"
#include "execution/binding_id_iter/paths/any_shortest/bfs_check.h"
#include "execution/binding_id_iter/paths/any_shortest/bfs_enum.h"
#include "execution/binding_id_iter/paths/any_shortest/unfixed_composite.h"
#include "execution/binding_id_iter/paths/any_shortest/experimental/a_star_iter_enum.h"
#include "execution/binding_id_iter/paths/any_simple/bfs_check.h"
#include "execution/binding_id_iter/paths/any_simple/bfs_enum.h"
#include "execution/binding_id_iter/paths/any_simple/dfs_check.h"
#include "execution/binding_id_iter/paths/any_simple/dfs_enum.h"
#include "execution/binding_id_iter/paths/any_trails/bfs_check.h"
#include "execution/binding_id_iter/paths/any_trails/bfs_enum.h"
#include "execution/binding_id_iter/paths/any_trails/dfs_check.h"
#include "execution/binding_id_iter/paths/any_trails/dfs_enum.h"
#include "execution/binding_id_iter/paths/path_manager.h"
#include "query_optimizer/quad_model/quad_model.h"

using namespace std;

PathPlan::PathPlan(VarId path_var, Id from, Id to, IPath& path, PathSemantic path_semantic) :
    path_var      (path_var),
    from          (from),
    to            (to),
    path          (path),
    from_assigned (std::holds_alternative<ObjectId>(from)),
    to_assigned   (std::holds_alternative<ObjectId>(to)),
    path_semantic (path_semantic) { }


double PathPlan::estimate_cost() const {
    // TODO: find a better estimation
    if (!to_assigned && !from_assigned) {
        return std::numeric_limits<double>::max();
    }
    return /*100.0 +*/ estimate_output_size();
}


void PathPlan::print(std::ostream& os, int indent, const std::vector<std::string>& var_names) const {
    for (int i = 0; i < indent; ++i) {
        os << ' ';
    }
    os << "PathPlan(";
    if (std::holds_alternative<ObjectId>(from)) {
        os << "from: " << quad_model.get_graph_object(std::get<ObjectId>(from));
    } else {
        os << "from: " << var_names[std::get<VarId>(from).id];
    }
    if (std::holds_alternative<ObjectId>(to)) {
        os << ", to: " << quad_model.get_graph_object(std::get<ObjectId>(to));
    } else {
        os << ", to: " << var_names[std::get<VarId>(to).id];
    }
    os << ", path: " << var_names[path_var.id] << ": " << path.to_string();
    os << ")";

    os << ",\n";
    for (int i = 0; i < indent; ++i) {
        os << ' ';
    }
    os << "  ↳ Estimated factor: " << estimate_output_size();
}


double PathPlan::estimate_output_size() const {
    // TODO: find a better estimation
    const auto total_connections = static_cast<double>(quad_model.catalog().connections_count);
    return total_connections * total_connections;
}


std::set<VarId> PathPlan::get_vars() const {
    std::set<VarId> result;
    if (std::holds_alternative<VarId>(from) && !from_assigned) {
        result.insert(std::get<VarId>(from));
    }
    if (std::holds_alternative<VarId>(to) && !to_assigned) {
        result.insert(std::get<VarId>(to));
    }

    return result;
}


void PathPlan::set_input_vars(const std::set<VarId>& input_vars) {
    set_input_var(input_vars, from, &from_assigned);
    set_input_var(input_vars, to, &to_assigned);
}


unique_ptr<Paths::IndexProvider> PathPlan::get_provider(RPQ_DFA automaton, ThreadInfo* thread_info) const {
    auto t_info = unordered_map<uint64_t, Paths::IndexType>();
    auto t_inv_info = unordered_map<uint64_t, Paths::IndexType>();
    for (size_t state = 0; state < automaton.from_to_connections.size(); state++) {
        for (auto& transition : automaton.from_to_connections[state]) {
            if (transition.inverse) {
                // Avoid transitions that are already stored
                if (t_inv_info.find(transition.type_id.id) != t_inv_info.end()) {
                    continue;
                }

                // Use default index type if needed
                if (quad_model.use_default_index) {
                    if (path_semantic == PathSemantic::TRAILS || path_semantic == PathSemantic::ANY_TRAILS) {
                        t_inv_info.insert({transition.type_id.id, quad_model.default_trails_index_type});
                    } else {
                        t_inv_info.insert({transition.type_id.id, quad_model.default_index_type});
                    }
                    continue;
                }

                // TODO: Catalog method, when not using default index type, consider also trails with an IF
                /*
                auto count = quad_model.catalog().type2total_count.find(transition.type_id.id);
                if (count != quad_model.catalog().type2total_count.end()) {
                    if (count->second > 10) {
                        t_inv_info.insert({transition.type_id.id, Paths::IndexType::BTREE});
                    }
                }
                */
                if (path_semantic == PathSemantic::TRAILS || path_semantic == PathSemantic::ANY_TRAILS) {
                    t_inv_info.insert({transition.type_id.id, Paths::IndexType::BTREE});
                } else {
                    t_inv_info.insert({transition.type_id.id, Paths::IndexType::BTREE});
                }
            } else {
                // Avoid transitions that are already stored
                if (t_info.find(transition.type_id.id) != t_info.end()) {
                    continue;
                }

                // Use default index type if needed
                if (quad_model.use_default_index) {
                    if (path_semantic == PathSemantic::TRAILS || path_semantic == PathSemantic::ANY_TRAILS) {
                        t_info.insert({transition.type_id.id, quad_model.default_trails_index_type});
                    } else {
                        t_info.insert({transition.type_id.id, quad_model.default_index_type});
                    }
                    continue;
                }

                // TODO: Catalog method, when not using default index type, consider also trails with an IF
                /*
                auto count = quad_model.catalog().type2total_count.find(transition.type_id.id);
                if (count != quad_model.catalog().type2total_count.end()) {
                    if (count->second > 10) {
                        t_inv_info.insert({transition.type_id.id, Paths::IndexType::BTREE});
                    }
                }
                */
                if (path_semantic == PathSemantic::TRAILS || path_semantic == PathSemantic::ANY_TRAILS) {
                    t_info.insert({transition.type_id.id, Paths::IndexType::BTREE});
                } else {
                    t_info.insert({transition.type_id.id, Paths::IndexType::BTREE});
                }
            }
        }
    }
    return make_unique<Paths::QuadModelIndexProvider>(
        move(t_info), move(t_inv_info), &thread_info->interruption_requested);
}


unique_ptr<BindingIdIter> PathPlan::get_binding_id_iter(ThreadInfo* thread_info) const {
    std::function<ObjectId(const std::string&)> str_to_object_id_f = [](const std::string& str) {
        return quad_model.get_object_id(QueryElement(NamedNode(str)));
    };

    switch (path_semantic) {
    case PathSemantic::ANY : {
        if (from_assigned) {
            auto automaton = path.get_rpq_automaton(str_to_object_id_f);
            if (to_assigned) {
                // bool case
                auto provider = get_provider(automaton, thread_info);
                if (quad_model.path_mode == PathMode::BFS) {
                    return make_unique<Paths::AnyShortest::BFSCheck>(thread_info,
                                                                     path_var,
                                                                     from,
                                                                     to,
                                                                     automaton,
                                                                     move(provider));
                } else {
                    return make_unique<Paths::Any::DFSCheck>(thread_info,
                                                             path_var,
                                                             from,
                                                             to,
                                                             automaton,
                                                             move(provider));
                }
            } else {
                // enum starting on from
                auto provider = get_provider(automaton, thread_info);
                if (quad_model.path_mode == PathMode::BFS) {
                    if (automaton.total_final_states > 1) {
                        return make_unique<Paths::AnyShortest::BFSEnum<true>>(thread_info,
                                                                              path_var,
                                                                              from,
                                                                              std::get<VarId>(to),
                                                                              automaton,
                                                                              move(provider));
                    } else {
                        return make_unique<Paths::AnyShortest::BFSEnum<false>>(thread_info,
                                                                               path_var,
                                                                               from,
                                                                               std::get<VarId>(to),
                                                                               automaton,
                                                                               move(provider));
                    }
                } else {
                    if (automaton.total_final_states > 1) {
                        return make_unique<Paths::Any::DFSEnum<true>>(thread_info,
                                                                      path_var,
                                                                      from,
                                                                      std::get<VarId>(to),
                                                                      automaton,
                                                                      move(provider));
                    } else {
                        return make_unique<Paths::Any::DFSEnum<false>>(thread_info,
                                                                       path_var,
                                                                       from,
                                                                       std::get<VarId>(to),
                                                                       automaton,
                                                                       move(provider));
                    }
                }
            }
        } else {
            if (to_assigned) {
                // enum starting on to
                auto inverted_path = path.invert();
                auto automaton = inverted_path->get_rpq_automaton(str_to_object_id_f);
                auto provider = get_provider(automaton, thread_info);
                if (quad_model.path_mode == PathMode::BFS) {
                    if (automaton.total_final_states > 1) {
                        return make_unique<Paths::AnyShortest::BFSEnum<true>>(thread_info,
                                                                              path_var,
                                                                              to,
                                                                              std::get<VarId>(from),
                                                                              automaton,
                                                                              move(provider));
                    } else {
                        return make_unique<Paths::AnyShortest::BFSEnum<false>>(thread_info,
                                                                               path_var,
                                                                               to,
                                                                               std::get<VarId>(from),
                                                                               automaton,
                                                                               move(provider));
                    }
                } else {
                    if (automaton.total_final_states > 1) {
                        return make_unique<Paths::Any::DFSEnum<true>>(thread_info,
                                                                      path_var,
                                                                      to,
                                                                      std::get<VarId>(from),
                                                                      automaton,
                                                                      move(provider));
                    } else {
                        return make_unique<Paths::Any::DFSEnum<false>>(thread_info,
                                                                       path_var,
                                                                       to,
                                                                       std::get<VarId>(from),
                                                                       automaton,
                                                                       move(provider));
                    }
                }
            } else {
                if (path.nullable()) {
                    throw QuerySemanticException("Nullable property paths must have at least 1 node fixed");
                }
                auto automaton = path.get_rpq_automaton(str_to_object_id_f);
                auto provider = get_provider(automaton, thread_info);
                // TODO: Use template for multiple final states in automaton
                return make_unique<Paths::AnyShortest::UnfixedComposite>(thread_info,
                                                                         path_var,
                                                                         std::get<VarId>(from),
                                                                         std::get<VarId>(to),
                                                                         automaton,
                                                                         move(provider));
            }
        }
    }
    case PathSemantic::ALL_SHORTEST : {
        if (from_assigned) {
            auto automaton = path.get_rpq_automaton(str_to_object_id_f);
            if (to_assigned) {
                // bool case
                auto provider = get_provider(automaton, thread_info);
                return make_unique<Paths::AllShortest::BFSCheck>(thread_info,
                                                                 path_var,
                                                                 from,
                                                                 to,
                                                                 automaton,
                                                                 move(provider));
            } else {
                // enum starting on from
                auto provider = get_provider(automaton, thread_info);
                if (automaton.total_final_states > 1) {
                    return make_unique<Paths::AllShortest::BFSEnum<true>>(thread_info,
                                                                          path_var,
                                                                          from,
                                                                          std::get<VarId>(to),
                                                                          automaton,
                                                                          move(provider));
                } else {
                    return make_unique<Paths::AllShortest::BFSEnum<false>>(thread_info,
                                                                           path_var,
                                                                           from,
                                                                           std::get<VarId>(to),
                                                                           automaton,
                                                                           move(provider));
                }
            }
        } else {
            if (to_assigned) {
                // enum starting on to
                auto inverted_path = path.invert();
                auto automaton = inverted_path->get_rpq_automaton(str_to_object_id_f);
                auto provider = get_provider(automaton, thread_info);
                if (automaton.total_final_states > 1) {
                    return make_unique<Paths::AllShortest::BFSEnum<true>>(thread_info,
                                                                          path_var,
                                                                          to,
                                                                          std::get<VarId>(from),
                                                                          automaton,
                                                                          move(provider));
                } else {
                    return make_unique<Paths::AllShortest::BFSEnum<false>>(thread_info,
                                                                          path_var,
                                                                          to,
                                                                          std::get<VarId>(from),
                                                                          automaton,
                                                                          move(provider));
                }
            } else {
                // TODO: allow no-nullable unfixed paths
                throw QuerySemanticException("property paths with ALL_SHORTEST semantics must have at least 1 node fixed.");
            }
        }
    }
    case PathSemantic::ALL_COUNT : {
        if (from_assigned) {
            auto automaton = path.get_rpq_automaton(str_to_object_id_f);
            if (to_assigned) {
                // bool case
                auto provider = get_provider(automaton, thread_info);
                return make_unique<Paths::AllShortestCount::BFSCheck>(thread_info,
                                                                      path_var,
                                                                      from,
                                                                      to,
                                                                      automaton,
                                                                      move(provider));
            } else {
                // enum starting on from
                auto provider = get_provider(automaton, thread_info);
                if (automaton.total_final_states > 1) {
                    return make_unique<Paths::AllShortestCount::BFSEnum<true>>(thread_info,
                                                                               path_var,
                                                                               from,
                                                                               std::get<VarId>(to),
                                                                               automaton,
                                                                               move(provider));
                } else {
                    return make_unique<Paths::AllShortestCount::BFSEnum<false>>(thread_info,
                                                                                path_var,
                                                                                from,
                                                                                std::get<VarId>(to),
                                                                                automaton,
                                                                                move(provider));
                }
            }
        } else {
            if (to_assigned) {
                // enum starting on to
                auto inverted_path = path.invert();
                auto automaton = inverted_path->get_rpq_automaton(str_to_object_id_f);
                auto provider = get_provider(automaton, thread_info);
                if (automaton.total_final_states > 1) {
                    return make_unique<Paths::AllShortestCount::BFSEnum<true>>(thread_info,
                                                                               path_var,
                                                                               to,
                                                                               std::get<VarId>(from),
                                                                               automaton,
                                                                               move(provider));
                } else {
                    return make_unique<Paths::AllShortestCount::BFSEnum<false>>(thread_info,
                                                                                path_var,
                                                                                to,
                                                                                std::get<VarId>(from),
                                                                                automaton,
                                                                                move(provider));
                }
            } else {
                // TODO: allow no-nullable unfixed paths
                throw QuerySemanticException("property paths with ALL_COUNT semantics must have at least 1 node fixed.");
            }
        }
    }
    case PathSemantic::ANY_SIMPLE : {
        if (from_assigned) {
            auto automaton = path.get_rpq_automaton(str_to_object_id_f);
            if (to_assigned) {
                // bool case
                auto provider = get_provider(automaton, thread_info);
                if (quad_model.path_mode == PathMode::BFS) {
                    return make_unique<Paths::AnySimple::BFSCheck>(thread_info,
                                                                   path_var,
                                                                   from,
                                                                   to,
                                                                   automaton,
                                                                   move(provider));
                } else {
                    return make_unique<Paths::AnySimple::DFSCheck>(thread_info,
                                                                   path_var,
                                                                   from,
                                                                   to,
                                                                   automaton,
                                                                   move(provider));
                }
            } else {
                // enum starting on from
                auto provider = get_provider(automaton, thread_info);
                if (quad_model.path_mode == PathMode::BFS) {
                    return make_unique<Paths::AnySimple::BFSEnum>(thread_info,
                                                                  path_var,
                                                                  from,
                                                                  std::get<VarId>(to),
                                                                  automaton,
                                                                  move(provider));
                } else {
                    return make_unique<Paths::AnySimple::DFSEnum>(thread_info,
                                                                  path_var,
                                                                  from,
                                                                  std::get<VarId>(to),
                                                                  automaton,
                                                                  move(provider));

                }
            }
        } else {
            if (to_assigned) {
                // enum starting on to
                auto inverted_path = path.invert();
                auto automaton = inverted_path->get_rpq_automaton(str_to_object_id_f);
                auto provider = get_provider(automaton, thread_info);
                if (quad_model.path_mode == PathMode::BFS) {
                    return make_unique<Paths::AnySimple::BFSEnum>(thread_info,
                                                                  path_var,
                                                                  to,
                                                                  std::get<VarId>(from),
                                                                  automaton,
                                                                  move(provider));
                } else {
                    return make_unique<Paths::AnySimple::DFSEnum>(thread_info,
                                                                  path_var,
                                                                  to,
                                                                  std::get<VarId>(from),
                                                                  automaton,
                                                                  move(provider));
                }
            } else {
                // TODO: allow no-nullable unfixed paths
                throw QuerySemanticException("property paths with ANY_SIMPLE semantics must have at least 1 node fixed.");
            }
        }
    }
    case PathSemantic::SIMPLE : {
        if (from_assigned) {
            auto automaton = path.get_rpq_automaton(str_to_object_id_f);
            if (to_assigned) {
                // bool case
                auto provider = get_provider(automaton, thread_info);
                if (quad_model.path_mode == PathMode::BFS) {
                    return make_unique<Paths::AllSimple::BFSCheck>(thread_info,
                                                                   path_var,
                                                                   from,
                                                                   to,
                                                                   automaton,
                                                                   move(provider));
                } else {
                    return make_unique<Paths::AllSimple::DFSCheck>(thread_info,
                                                                   path_var,
                                                                   from,
                                                                   to,
                                                                   automaton,
                                                                   move(provider));
                }
            } else {
                // enum starting on from
                auto provider = get_provider(automaton, thread_info);
                if (quad_model.path_mode == PathMode::BFS) {
                    return make_unique<Paths::AllSimple::BFSEnum>(thread_info,
                                                                  path_var,
                                                                  from,
                                                                  std::get<VarId>(to),
                                                                  automaton,
                                                                  move(provider));
                } else {
                    return make_unique<Paths::AllSimple::DFSEnum>(thread_info,
                                                                  path_var,
                                                                  from,
                                                                  std::get<VarId>(to),
                                                                  automaton,
                                                                  move(provider));

                }
            }
        } else {
            if (to_assigned) {
                // enum starting on to
                auto inverted_path = path.invert();
                auto automaton = inverted_path->get_rpq_automaton(str_to_object_id_f);
                auto provider = get_provider(automaton, thread_info);
                if (quad_model.path_mode == PathMode::BFS) {
                    return make_unique<Paths::AllSimple::BFSEnum>(thread_info,
                                                                  path_var,
                                                                  to,
                                                                  std::get<VarId>(from),
                                                                  automaton,
                                                                  move(provider));
                } else {
                    return make_unique<Paths::AllSimple::DFSEnum>(thread_info,
                                                                  path_var,
                                                                  to,
                                                                  std::get<VarId>(from),
                                                                  automaton,
                                                                  move(provider));
                }
            } else {
                // TODO: allow no-nullable unfixed paths
                throw QuerySemanticException("property paths with SIMPLE semantics must have at least 1 node fixed.");
            }
        }
    }
    case PathSemantic::ANY_TRAILS : {
        if (from_assigned) {
            auto automaton = path.get_rpq_automaton(str_to_object_id_f);
            if (to_assigned) {
                // bool case
                auto provider = get_provider(automaton, thread_info);
                if (quad_model.path_mode == PathMode::BFS) {
                    return make_unique<Paths::AnyTrails::BFSCheck>(thread_info,
                                                                   path_var,
                                                                   from,
                                                                   to,
                                                                   automaton,
                                                                   move(provider));
                } else {
                    return make_unique<Paths::AnyTrails::DFSCheck>(thread_info,
                                                                   path_var,
                                                                   from,
                                                                   to,
                                                                   automaton,
                                                                   move(provider));
                }
            } else {
                // enum starting on from
                auto provider = get_provider(automaton, thread_info);
                if (quad_model.path_mode == PathMode::BFS) {
                    return make_unique<Paths::AnyTrails::BFSEnum>(thread_info,
                                                                  path_var,
                                                                  from,
                                                                  std::get<VarId>(to),
                                                                  automaton,
                                                                  move(provider));
                } else {
                    return make_unique<Paths::AnyTrails::DFSEnum>(thread_info,
                                                                  path_var,
                                                                  from,
                                                                  std::get<VarId>(to),
                                                                  automaton,
                                                                  move(provider));
                }
            }
        } else {
            if (to_assigned) {
                // enum starting on to
                auto inverted_path = path.invert();
                auto automaton = inverted_path->get_rpq_automaton(str_to_object_id_f);
                auto provider = get_provider(automaton, thread_info);
                if (quad_model.path_mode == PathMode::BFS) {
                    return make_unique<Paths::AnyTrails::BFSEnum>(thread_info,
                                                                  path_var,
                                                                  to,
                                                                  std::get<VarId>(from),
                                                                  automaton,
                                                                  move(provider));
                } else {
                    return make_unique<Paths::AnyTrails::DFSEnum>(thread_info,
                                                                  path_var,
                                                                  to,
                                                                  std::get<VarId>(from),
                                                                  automaton,
                                                                  move(provider));
                }
            } else {
                // TODO: allow no-nullable unfixed paths
                throw QuerySemanticException("property paths with ANY_TRAILS semantics must have at least 1 node fixed.");
            }
        }
    }
    case PathSemantic::TRAILS : {
        if (from_assigned) {
            auto automaton = path.get_rpq_automaton(str_to_object_id_f);
            if (to_assigned) {
                // bool case
                auto provider = get_provider(automaton, thread_info);
                if (quad_model.path_mode == PathMode::BFS) {
                    return make_unique<Paths::AllTrails::BFSCheck>(thread_info,
                                                                   path_var,
                                                                   from,
                                                                   to,
                                                                   automaton,
                                                                   move(provider));
                } else {
                    return make_unique<Paths::AllTrails::DFSCheck>(thread_info,
                                                                   path_var,
                                                                   from,
                                                                   to,
                                                                   automaton,
                                                                   move(provider));
                }
            } else {
                // enum starting on from
                auto provider = get_provider(automaton, thread_info);
                if (quad_model.path_mode == PathMode::BFS) {
                    return make_unique<Paths::AllTrails::BFSEnum>(thread_info,
                                                                  path_var,
                                                                  from,
                                                                  std::get<VarId>(to),
                                                                  automaton,
                                                                  move(provider));
                } else {
                    return make_unique<Paths::AllTrails::DFSEnum>(thread_info,
                                                                  path_var,
                                                                  from,
                                                                  std::get<VarId>(to),
                                                                  automaton,
                                                                  move(provider));
                }
            }
        } else {
            if (to_assigned) {
                // enum starting on to
                auto inverted_path = path.invert();
                auto automaton = inverted_path->get_rpq_automaton(str_to_object_id_f);
                auto provider = get_provider(automaton, thread_info);
                if (quad_model.path_mode == PathMode::BFS) {
                    return make_unique<Paths::AllTrails::BFSEnum>(thread_info,
                                                                  path_var,
                                                                  to,
                                                                  std::get<VarId>(from),
                                                                  automaton,
                                                                  move(provider));
                } else {
                    return make_unique<Paths::AllTrails::DFSEnum>(thread_info,
                                                                  path_var,
                                                                  to,
                                                                  std::get<VarId>(from),
                                                                  automaton,
                                                                  move(provider));
                }
            } else {
                // TODO: allow no-nullable unfixed paths
                throw QuerySemanticException("property paths with TRAILS semantics must have at least 1 node fixed.");
            }
        }
    }
    }
    return nullptr;
}

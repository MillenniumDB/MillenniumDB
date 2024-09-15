#include "construct_executor.h"

#include "graph_models/rdf_model/conversions.h"
#include "query/executor/query_executor/sparql/ttl_writer.h"

using namespace SPARQL;

ConstructExecutor::ConstructExecutor(
    std::unique_ptr<BindingIter> child_iter,
    std::vector<Triple>          triples
) :
    child_iter       (std::move(child_iter)),
    triples          (std::move(triples)),
    extendable_table (DistinctBindingHash(3)),
    current_triple   (std::vector<ObjectId>(3)) { }


bool ConstructExecutor::process_element(Id id, Idx idx, BlankNodeMap& blank_nodes) {
    if (!id.is_var()) {
        current_triple[static_cast<size_t>(idx)] = id.get_OID();
    } else {
        auto var = id.get_var();
        auto& var_name = get_query_ctx().get_var_name(var);

        if (var_name.size() > 2 && var_name[0] == '_' && var_name[1] == ':') {
            auto oid_it = blank_nodes.find(var_name);
            if (oid_it == blank_nodes.end()) {
                auto oid = Conversions::pack_blank_tmp(get_query_ctx().get_new_blank_node());
                blank_nodes.insert({ var_name, oid });
                current_triple[static_cast<size_t>(idx)] = oid;
            } else {
                current_triple[static_cast<size_t>(idx)] = oid_it->second;
            }
        } else {
            current_triple[static_cast<size_t>(idx)] = (*child_binding)[var];
        }
    }
    if (current_triple[static_cast<size_t>(idx)].is_null()) {
        return false;
    }
    return true;
}


uint64_t ConstructExecutor::execute(std::ostream& os) {
    // Number of triples in the graph returned
    triple_count = 0;

    child_binding = std::make_unique<Binding>(get_query_ctx().get_var_size());
    child_iter->begin(*child_binding);

    BlankNodeMap blank_nodes;
    while (child_iter->next()) {
        for (auto triple : triples) {
            if (!process_element(triple.subject,   Idx::subject,   blank_nodes)) continue;
            if (!process_element(triple.predicate, Idx::predicate, blank_nodes)) continue;
            if (!process_element(triple.object,    Idx::object,    blank_nodes)) continue;

            if (current_triple_distinct()) {
                triple_count++;
                write_and_escape_ttl(os, current_triple[0]);
                os << ' ';
                write_and_escape_ttl(os, current_triple[1]);
                os << ' ';
                write_and_escape_ttl(os, current_triple[2]);
                os << " .\n";
            }
        }
        blank_nodes.clear();
    }

    return triple_count;
}


bool ConstructExecutor::current_triple_distinct() {
    bool is_new_tuple = !extendable_table.is_in_or_insert(current_triple);
    return is_new_tuple;
}


void ConstructExecutor::analyze(std::ostream & os, bool print_stats, int indent) const {
    os << std::string(indent, ' ') << "ConstructExecutor(";

    os << triple_count << " triples; ";

    // auto first = true;
    // for (auto& var : vars) {
    //     if (!first) {
    //         os << ", ";
    //     } else {
    //         first = false;
    //     }
    //     os << "?" << var_manager->get(var);
    // }

    // for (auto& iri : iris) {
    //     if (!first) {
    //         os << ", ";
    //     } else {
    //         first = false;
    //     }
    //     os << Conversions::to_human_str(iri);
    // }

    os << ")\n";
    if (child_iter) {
        BindingIterPrinter printer(os, print_stats, indent + 2);
        child_iter->accept_visitor(printer);
    }
}
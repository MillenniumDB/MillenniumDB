#include "describe_executor.h"

#include "graph_models/rdf_model/conversions.h"
#include "query/executor/query_executor/sparql/ttl_writer.h"
#include "query/optimizer/rdf_model/plan/triple_plan.h"

using namespace SPARQL;

DescribeExecutor::DescribeExecutor(
    std::unique_ptr<BindingIter> child_iter,
    std::vector<VarId>             vars,
    std::vector<ObjectId>          iris
) :
    child_iter       (std::move(child_iter)),
    vars             (std::move(vars)),
    iris             (std::move(iris)),
    extendable_table (DistinctBindingHash<ObjectId>(3)),
    current_triple   (std::vector<ObjectId>(3)) { }


uint64_t DescribeExecutor::execute(std::ostream& os) {
    auto var0 = VarId(0);
    auto var1 = VarId(1);

    // Number of triples in the graph returned
    triple_count = 0;

    if (child_iter) {
        auto child_binding = std::make_unique<Binding>(get_query_ctx().get_var_size());
        child_iter->begin(*child_binding);

        while(child_iter->next()) {
            for (auto var: vars) {
                auto oid = (*child_binding)[var];

                auto gen_type = oid.get_generic_type();
                // We only describe blank nodes and iris, if we want to describe literals
                // this check has to be changed.
                if (gen_type == ObjectId::MASK_ANON || gen_type == ObjectId::MASK_IRI) {
                    triple_count += output_triples(os, oid, var0, var1);
                    triple_count += output_triples(os, var0, var1, oid);
                }
            }
        }
    }

    for (auto iri: iris) {
        if (!iri.is_iri()) {
            continue;
        }
        triple_count += output_triples(os, iri, var0, var1);
        triple_count += output_triples(os, var0, var1, iri);
    }

    return triple_count;
}


uint64_t DescribeExecutor::output_triples(std::ostream& os, Id subject, Id predicate, Id object) {
    auto describe_binding = std::make_unique<Binding>(2);

    auto plan = std::make_unique<TriplePlan>(subject, predicate, object);
    auto describe_iter = plan->get_binding_iter();

    describe_iter->begin(*describe_binding);

    uint64_t result_count = 0;

    std::string subject_str;
    std::string predicate_str;
    std::string object_str;

    // We only describe blank nodes and IRIs.
    // If we want to describe something else we have to update these checks.
    if (!subject.is_var()) {
        auto oid = subject.get_OID();
        if (!oid.is_iri() && !oid.is_bnode()) {
            return 0;
        }
        current_triple[0] = oid;
    }
    if (!predicate.is_var()) {
        auto oid = predicate.get_OID();
        if (!oid.is_iri() && !oid.is_bnode()) {
            return 0;
        }
        current_triple[1] = oid;
    }
    if (!object.is_var()) {
        auto oid = object.get_OID();
        if (!oid.is_iri() && !oid.is_bnode()) {
            return 0;
        }
        current_triple[2] = oid;
    }

    while (describe_iter->next()) {
        result_count++;

        if (subject.is_var()) {
            auto var_id = subject.get_var();
            auto oid = (*describe_binding)[var_id];
            current_triple[0] = oid;
        }
        if (predicate.is_var()) {
            auto var_id = predicate.get_var();
            auto oid = (*describe_binding)[var_id];
            current_triple[1] = oid;
        }
        if (object.is_var()) {
            auto var_id = object.get_var();
            auto oid = (*describe_binding)[var_id];
            current_triple[2] = oid;
        }

        if (current_triple_distinct()) {
            write_and_escape_ttl(os, current_triple[0]);
            os << ' ';
            write_and_escape_ttl(os, current_triple[1]);
            os << ' ';
            write_and_escape_ttl(os, current_triple[2]);
            os << " .\n";
        }
    }

    return result_count;
}


bool DescribeExecutor::current_triple_distinct() {
    bool is_new_tuple = !extendable_table.is_in_or_insert(current_triple);
    return is_new_tuple;
}


void DescribeExecutor::analyze(std::ostream& os, int indent) const {
    os << std::string(indent, ' ') << "DescribeExecutor(";

    os << triple_count << " triples; ";

    auto first = true;
    for (auto& var : vars) {
        if (!first) {
            os << ", ";
        } else {
            first = false;
        }
        os << "?" << get_query_ctx().get_var_name(var);
    }

    for (auto& iri : iris) {
        if (!first) {
            os << ", ";
        } else {
            first = false;
        }
        os << iri;
    }

    os << ")\n";
    if (child_iter) {
        child_iter->analyze(os, indent + 2);
    }
}

#include "sparql_service.h"

#include <string>

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/range/combine.hpp>

#include "graph_models/rdf_model/rdf_model.h"

void SparqlService::begin(Binding& _parent_binding) {
    first_next = true;
    failed = false;
    parent_binding = &_parent_binding;
}


bool SparqlService::next() {
    if (failed) return false;
    try {
        if (first_next) {
            first_next = false;
            network_requests++;
            executions++;
            response_parser.begin(*parent_binding);
            auto result = response_parser.next();
            if (result) {
                result_count++;
            }
            return result;
        } else {
            auto result = response_parser.next();
            if (result) {
                result_count++;
            }
            return result;
        }
    } catch(std::exception const& e) { // Catches boost/asio, property_tree and tokenizer errors
        if (silent) {
            assign_nulls(); // Set all null to avoid propagating the error
            failed = true;
            return false;
        } else {
            throw QueryExecutionException(e.what());
        }
    }
}


void SparqlService::analyze(std::ostream& os, int indent) const {

    os << std::string(indent, ' ');
    os << "SparqlService(";
    if (silent) os << "SILENT ";
    if (std::holds_alternative<VarId>(response_parser.var_or_iri)) {
        auto service_id = std::get<VarId>(response_parser.var_or_iri);
        os << '?' << get_query_ctx().get_var_name(service_id);
    } else {
        os << '<' << std::get<std::string>(response_parser.var_or_iri) << '>';
    }

    os << " scope:";
    auto first = true;
    for (auto var : scope_vars) {
        if (first) first = false; else os << ",";
        os << "?" << get_query_ctx().get_var_name(var);
    }

    first = true;
    os << "; fixed:";
    for (auto var : fixed_vars) {
        if (first) first = false; else os << ",";
        os << "?" << get_query_ctx().get_var_name(var);
    }

    first = true;
    os << "; fixed_join:";
    for (auto var : fixed_join_vars) {
        if (first) first = false; else os << ",";
        os << "?" << get_query_ctx().get_var_name(var);
    }
    os << "; ";

    for (auto& cha: response_parser.query) {
        if (cha != '\r') os << cha;
        if (cha == '\n') os << std::string(indent, ' ');
    }

    os << '\n' << std::string(indent + 2, ' ') << "Network request: " << network_requests << ", Results: " << result_count << ".";

    os << '\n' << std::string(indent + 2, ' ')
       << "Total Request Time: " << response_parser.request_duration.count() << " ms, "
       << "Total Parse Time: " << response_parser.parse_duration.count() << " ms.";

    os << '\n' << std::string(indent, ' ') << ")\n";
}


void SparqlService::reset() {
    if (fixed_join_vars.size() != 0) { // If there are fixed join vars we have to consume the API in next
        first_next = true;
    } else {
        try {
            executions++;
            response_parser.reset(); // Else reset, response_parser.service is an iri so does not consume again
        }
        catch (std::exception const& e) {
            if (silent)
                failed = true;
            else
                throw QueryExecutionException(e.what());
        }
    }
}


void SparqlService::assign_nulls(){
    for (auto var_id : response_parser.scope_vars) {
        parent_binding->add(var_id, ObjectId::get_null());
    }
}

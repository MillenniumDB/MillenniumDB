#include "sparql_service.h"

#include <string>

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/range/combine.hpp>

#include "graph_models/rdf_model/rdf_model.h"

void SparqlService::_begin(Binding& _parent_binding) {
    first_next = true;
    failed = false;
    parent_binding = &_parent_binding;
}


bool SparqlService::_next() {
    if (failed) return false;
    try {
        if (first_next) {
            first_next = false;
            network_requests++;
            response_parser.begin(*parent_binding);
            auto result = response_parser.next();
            return result;
        } else {
            auto result = response_parser.next();
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


void SparqlService::accept_visitor(BindingIterVisitor& visitor) {
    visitor.visit(*this);
}


void SparqlService::_reset() {
    if (fixed_join_vars.size() != 0) { // If there are fixed join vars we have to consume the API in next
        first_next = true;
    } else {
        try {
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

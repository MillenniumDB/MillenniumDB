#include "sparql_service.h"
#include "query/exceptions.h"

#include <string>

void SparqlService::_begin(Binding& _parent_binding)
{
    first_next = true;
    failed = false;
    parent_binding = &_parent_binding;
}

bool SparqlService::_next()
{
    if (failed)
        return false;
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
    } catch (const std::exception& e) { // Catches boost/asio, property_tree and tokenizer errors
        if (silent) {
            assign_nulls(); // Set all null to avoid propagating the error
            failed = true;
            return false;
        } else {
            throw QueryExecutionException(e.what());
        }
    }
}

void SparqlService::_reset()
{
    // if there are fixed join vars we have to consume the API in next
    if (fixed_join_vars.size() != 0) {
        first_next = true;
    } else {
        try {
            // else reset, response_parser.service is an iri so does not consume again
            response_parser.reset();
        } catch (std::exception const& e) {
            if (silent)
                failed = true;
            else
                throw QueryExecutionException(e.what());
        }
    }
}

void SparqlService::assign_nulls()
{
    for (auto var_id : response_parser.scope_vars) {
        parent_binding->add(var_id, ObjectId::get_null());
    }
}

void SparqlService::print(std::ostream& os, int indent, bool stats) const
{
    if (stats) {
        os << std::string(indent, ' ') << "[begin: " << stat_begin << " next: " << stat_next
           << " reset: " << stat_reset << " results: " << results << "requests: " << network_requests
           << ", total_request_time: " << response_parser.request_duration.count()
           << " ms, total_parse_time: " << response_parser.parse_duration.count() << " ms]";
    }

    os << std::string(indent, ' ') << "SparqlService(";
    if (silent)
        os << "SILENT ";
    if (std::holds_alternative<VarId>(response_parser.var_or_iri)) {
        auto service_id = std::get<VarId>(response_parser.var_or_iri);
        os << service_id;
    } else {
        os << '<' << std::get<std::string>(response_parser.var_or_iri) << '>';
    }

    if (scope_vars.size() > 0) {
        os << ", scope: ";
        auto first = true;
        for (auto var : scope_vars) {
            if (first)
                first = false;
            else
                os << ", ";
            os << var;
        }
    }

    if (fixed_vars.size() > 0) {
        os << ", fixed:";
        auto first = true;
        for (auto var : fixed_vars) {
            if (first)
                first = false;
            else
                os << ", ";
            os << var;
        }
    }

    if (fixed_join_vars.size() > 0) {
        os << ", fixed_join:";
        auto first = true;
        for (auto var : fixed_join_vars) {
            if (first)
                first = false;
            else
                os << ", ";
            os << var;
        }
    }

    os << ")\n";

    os << std::string(indent, ' ');

    auto last_char_newline = false;
    for (auto& cha : response_parser.query) {
        if (cha != '\r')
            os << cha;
        if (cha == '\n') {
            os << std::string(indent - 4, ' ');
            last_char_newline = true;
        } else {
            last_char_newline = false;
        }
    }

    if (!last_char_newline) {
        os << '\n';
    }
}

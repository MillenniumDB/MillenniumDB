#include "ask_executor.h"

using namespace SPARQL;

uint64_t AskExecutor::execute(std::ostream& os) {
    binding = std::make_unique<Binding>(get_query_ctx().get_var_size());
    root->begin(*binding);

    auto has_result = root->next() ? "true" : "false";

    switch (response_type) {
        case SPARQL::ResponseType::CSV: {
            os << "_askResult\n" << has_result << "\n";
            break;
        }
        case SPARQL::ResponseType::TSV: {
            os << "?_askResult\n" << has_result << "\n";
            break;
        }
        case SPARQL::ResponseType::JSON: {
            os << "{\"head\":{},\"boolean\":" << has_result << "}";
            break;
        }
        case SPARQL::ResponseType::XML: {
            os << "<?xml version=\"1.0\"?><sparql xmlns=\"http://www.w3.org/2005/sparql-results#\"><head></head><boolean>"
               << has_result << "</boolean></sparql>";
            break;
        }
        default: throw NotSupportedException("ASK response type: " + SPARQL::response_type_to_string(response_type));
    }
    return 1;
}


void AskExecutor::analyze(std::ostream& os, bool print_stats, int indent) const {
    os << std::string(indent, ' ') << "AskExecutor(";
    switch (response_type) {
    case SPARQL::ResponseType::JSON:{ os << "JSON"; break; }
    case SPARQL::ResponseType::CSV: { os << "CSV" ; break; }
    case SPARQL::ResponseType::TSV: { os << "TSV" ; break; }
    case SPARQL::ResponseType::XML: { os << "XML" ; break; }
    default: throw NotSupportedException("ASK response type: " + SPARQL::response_type_to_string(response_type));
    }
    os << ")\n";

    root->print(os, indent + 2, print_stats);
}

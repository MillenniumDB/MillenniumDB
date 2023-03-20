#pragma once

#include <string>

#include "execution/graph_object/graph_object_manager.h"

struct SparqlElementToString {
    std::string operator()(const Var& v) {
        return v.name;
    }

    std::string operator()(const Iri& i) {
        return '<' + i.name + '>';
    }

    std::string operator()(const Literal& l) {
        return '"' + l.str + '"';
    }

    std::string operator()(const LiteralDatatype& ld) {
        return '"' + ld.str + "\"^^<" + ld.datatype + '>';
    }

    std::string operator()(const LiteralLanguage& ll) {
        return '"' + ll.str + "\"@" + ll.language;
    }

    std::string operator()(const DateTime& dt) {
        return '"' + dt.get_value_string() + "\"^^<http://www.w3.org/2001/XMLSchema#dateTime>";
    }

    std::string operator()(const Decimal& d) {
        return '"' + d.str + "\"^^<http://www.w3.org/2001/XMLSchema#decimal>";
    }

    std::string operator()(bool b) {
        return b ? "\"true\"^^<http://www.w3.org/2001/XMLSchema#boolean>" : "\"false\"^^<http://www.w3.org/2001/XMLSchema#boolean>";
    }

    std::string operator()(const std::unique_ptr<SPARQL::IPath>& /*p*/) {
        // TODO: implement this
        return "\"Path2String not implemented\"";
    }
};

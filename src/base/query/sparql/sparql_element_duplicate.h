#pragma once

#include <memory>
#include <string>

#include "base/graph_object/datetime.h"
#include "base/query/sparql/decimal.h"
#include "base/query/sparql/iri.h"
#include "base/query/sparql/literal.h"
#include "base/query/sparql/literal_datatype.h"
#include "base/query/sparql/literal_language.h"
#include "base/query/sparql/path.h"
#include "base/query/var.h"

struct SparqlElementDuplicate {
    SparqlElement operator()(const Var& v) {
        return SparqlElement(v);
    }

    SparqlElement operator()(const Iri& i) {
        return SparqlElement(i);
    }

    SparqlElement operator()(const Literal& l) {
        return SparqlElement(l);
    }

    SparqlElement operator()(const LiteralDatatype& ld) {
        return SparqlElement(ld);
    }

    SparqlElement operator()(const LiteralLanguage& ll) {
        return SparqlElement(ll);
    }

    SparqlElement operator()(const DateTime& dt) {
        return SparqlElement(dt);
    }

    SparqlElement operator()(const Decimal& d) {
        return SparqlElement(d);
    }

    SparqlElement operator()(bool b) {
        return SparqlElement(b);
    }

    SparqlElement operator()(const std::unique_ptr<SPARQL::IPath>& p) {
        return SparqlElement(p->duplicate());
    }
};

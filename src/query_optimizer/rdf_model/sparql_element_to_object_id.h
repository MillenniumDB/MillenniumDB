#pragma once

#include "base/ids/object_id.h"
#include "base/query/sparql/sparql_element.h"

struct SparqlElementToObjectId {
    SparqlElementToObjectId(bool create_if_not_exists = false) : create_if_not_exists(create_if_not_exists) { }

    ObjectId operator()(const SparqlElement&);

    ObjectId operator()(const Var&);
    ObjectId operator()(const Iri&);
    ObjectId operator()(const Literal&);
    ObjectId operator()(const LiteralDatatype&);
    ObjectId operator()(const LiteralLanguage&);
    ObjectId operator()(DateTime);
    ObjectId operator()(Decimal);
    ObjectId operator()(bool);
    ObjectId operator()(const std::unique_ptr<SPARQL::IPath>&);
    // ObjectId operator()(int64_t);
    // ObjectId operator()(float);

private:
    bool create_if_not_exists;

    ObjectId get_string_id(const std::string&, uint64_t mask_inlined, uint64_t mask_external);
};

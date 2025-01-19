#pragma once

#include <cstdint>

#include "graph_models/model_destroyer.h"
#include "query/parser/op/op.h"
#include "query/parser/paths/regular_path_expr.h"
#include "graph_models/rdf_model/rdf_catalog.h"

template <std::size_t N> class BPlusTree;

class SparqlElement;

class RdfModel {
public:
    std::unique_ptr<BPlusTree<3>> spo; // (subject,    predicate, object)
    std::unique_ptr<BPlusTree<3>> pos; // (predicate, object,     subject)
    std::unique_ptr<BPlusTree<3>> osp; // (object,    subject,    predicate)
    std::unique_ptr<BPlusTree<3>> pso; // (predicate, subject,    object)

    std::unique_ptr<BPlusTree<3>> sop;
    std::unique_ptr<BPlusTree<3>> ops;

    // Special cases
    std::unique_ptr<BPlusTree<1>> equal_spo; // (subject=predicate=object)
    std::unique_ptr<BPlusTree<2>> equal_sp;  // (subject=predicate, object)
    std::unique_ptr<BPlusTree<2>> equal_so;  // (subject=object,    predicate)
    std::unique_ptr<BPlusTree<2>> equal_po;  // (predicate=object,  subject)

    std::unique_ptr<BPlusTree<2>> equal_sp_inverted; // (object,    subject=predicate)
    std::unique_ptr<BPlusTree<2>> equal_so_inverted; // (predicate, subject=object)
    std::unique_ptr<BPlusTree<2>> equal_po_inverted; // (subject,   predicate=object)

    RdfCatalog catalog;

    uint64_t MAX_LIMIT = Op::DEFAULT_LIMIT;

    // Path mode to use
    PathSearchMode path_mode = PathSearchMode::BFS;

    // list of {alias, prefix}
    // These are common prefixes used for queries, in case the user has not defined them in the query.
    // This way IRIs such as xsd:string can be used in queries even if xsd is not explicitly defined.
    std::vector<std::pair<std::string, std::string>> default_query_prefixes = {
        { "bd", "http://www.bigdata.com/rdf#" },
        { "cc", "http://creativecommons.org/ns#" },
        { "dct", "http://purl.org/dc/terms/" },
        { "geo", "http://www.opengis.net/ont/geosparql#" },
        { "hint", "http://www.bigdata.com/queryHints#" },
        { "ontolex", "http://www.w3.org/ns/lemon/ontolex#" },
        { "owl", "http://www.w3.org/2002/07/owl#" },
        { "prov", "http://www.w3.org/ns/prov#" },
        { "rdf", "http://www.w3.org/1999/02/22-rdf-syntax-ns#" },
        { "rdfs", "http://www.w3.org/2000/01/rdf-schema#" },
        { "schema", "http://schema.org/" },
        { "skos", "http://www.w3.org/2004/02/skos/core#" },
        { "xsd", "http://www.w3.org/2001/XMLSchema#" },
        { "p", "http://www.wikidata.org/prop/" },
        { "pq", "http://www.wikidata.org/prop/qualifier/" },
        { "pqn", "http://www.wikidata.org/prop/qualifier/value-normalized/" },
        { "pqv", "http://www.wikidata.org/prop/qualifier/value/" },
        { "pr", "http://www.wikidata.org/prop/reference/" },
        { "prn", "http://www.wikidata.org/prop/reference/value-normalized/" },
        { "prv", "http://www.wikidata.org/prop/reference/value/" },
        { "psv", "http://www.wikidata.org/prop/statement/value/" },
        { "ps", "http://www.wikidata.org/prop/statement/" },
        { "psn", "http://www.wikidata.org/prop/statement/value-normalized/" },
        { "wd", "http://www.wikidata.org/entity/" },
        { "wdata", "http://www.wikidata.org/wiki/Special:EntityData/" },
        { "wdno", "http://www.wikidata.org/prop/novalue/" },
        { "wdref", "http://www.wikidata.org/reference/" },
        { "wds", "http://www.wikidata.org/entity/statement/" },
        { "wdt", "http://www.wikidata.org/prop/direct/" },
        { "wdtn", "http://www.wikidata.org/prop/direct-normalized/" },
        { "wdv", "http://www.wikidata.org/value/" },
        { "wikibase", "http://wikiba.se/ontology#" },
        { "mdbfn", "http://millenniumdb.com/function/" },
    };

    // Default compression IRI prefixes.
    // These are added to the prefixes used for compression if there are still IDs available (max 256 prefixes).
    static inline const std::vector<std::string> default_compression_prefixes = {
        "http://",
        "https://",
        "http://www.",
        "https://www.",
    };

    // necessary to be called before first usage
    static std::unique_ptr<ModelDestroyer> init();

private:
    RdfModel();
};

extern RdfModel& rdf_model; // global object

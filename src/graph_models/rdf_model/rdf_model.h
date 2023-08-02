#pragma once

#include <cstdint>
#include <type_traits>

#include "graph_models/object_id.h"
#include "network/sparql/response_type.h"
#include "query/parser/op/op.h"
#include "query/parser/paths/regular_path_expr.h"
#include "graph_models/rdf_model/rdf_catalog.h"

template <std::size_t N> class BPlusTree;

class SparqlElement;

class RdfModel {
    class Destroyer {
        friend class RdfModel;

    private:
        Destroyer() = default;

    public:
        ~Destroyer();
    };

public:
    std::unique_ptr<BPlusTree<3>> spo; // (subject,    predicate, object)
    std::unique_ptr<BPlusTree<3>> pos; // (predicate, object,     subject)
    std::unique_ptr<BPlusTree<3>> osp; // (object,    subject,    predicate)
    std::unique_ptr<BPlusTree<3>> pso; // (predicate, subject,    object)

    // Special cases
    std::unique_ptr<BPlusTree<1>> equal_spo; // (subject=predicate=object)
    std::unique_ptr<BPlusTree<2>> equal_sp;  // (subject=predicate, object)
    std::unique_ptr<BPlusTree<2>> equal_so;  // (subject=object,    predicate)
    std::unique_ptr<BPlusTree<2>> equal_po;  // (predicate=object,  subject)

    std::unique_ptr<BPlusTree<2>> equal_sp_inverted; // (object,    subject=predicate)
    std::unique_ptr<BPlusTree<2>> equal_so_inverted; // (predicate, subject=object)
    std::unique_ptr<BPlusTree<2>> equal_po_inverted; // (subject,   predicate=object)

    uint64_t MAX_LIMIT = UINT64_MAX;

    // Path mode to use
    PathMode path_mode = PathMode::BFS;

    // TODO: make empty for default, read from config file?
    // list of {alias, prefix}
    std::vector<std::pair<std::string, std::string>> default_prefixes = {
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
    };

    // necessary to be called before first usage
    static RdfModel::Destroyer init(const std::string& db_folder,
                                    uint_fast32_t      str_max_initial_populate_size,
                                    uint_fast32_t      shared_buffer_pool_size,
                                    uint_fast32_t      private_buffer_pool_size,
                                    uint_fast32_t      max_threads);

    // void exec_inserts(const SPARQL::OpInsert&);

    inline RdfCatalog& catalog() const noexcept {
        return const_cast<RdfCatalog&>(reinterpret_cast<const RdfCatalog&>(catalog_buf));
    }

private:
    typename std::aligned_storage<sizeof(RdfCatalog), alignof(RdfCatalog)>::type catalog_buf;

    RdfModel(const std::string& db_folder,
             uint_fast32_t      str_max_initial_populate_size,
             uint_fast32_t      shared_buffer_pool_size,
             uint_fast32_t      private_buffer_pool_size,
             uint_fast32_t      max_threads);

    ~RdfModel();
};

extern RdfModel& rdf_model; // global object

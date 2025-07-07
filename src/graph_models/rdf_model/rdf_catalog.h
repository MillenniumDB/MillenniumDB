#pragma once

#include <cstdint>
#include <ostream>
#include <string>

#include "graph_models/rdf_model/iri_prefixes.h"
#include "storage/catalog/catalog.h"
#include "storage/index/hnsw/hnsw_index_manager.h"
#include "storage/index/text_search/text_index_manager.h"

class RdfCatalog : public Catalog {
public:
    static constexpr uint8_t MODEL_ID = 1;

    static constexpr uint8_t MAJOR_VERSION = 1;
    static constexpr uint8_t MINOR_VERSION = 0;

    // The database can handle more than MAX_LANG_AND_DTT languages and datatypes,
    // but the catalog can save up to this this many
    static constexpr uint64_t MAX_LANG_AND_DTT = 4095;

    // Constructor for existing catalog
    RdfCatalog(const std::string& filename);

    // Constructor for new catalog
    RdfCatalog(const std::string& filename, size_t permutations);

    ~RdfCatalog();

    void print(std::ostream&);
    void save();

    inline uint64_t get_triples_count()   const { return triples_count; }
    inline uint64_t get_equal_spo_count() const { return equal_spo_count; }
    inline uint64_t get_equal_sp_count()  const { return equal_sp_count; }
    inline uint64_t get_equal_so_count()  const { return equal_so_count; }
    inline uint64_t get_equal_po_count()  const { return equal_po_count; }

    inline void set_triples_count(uint64_t count) {
        triples_count = count;
    }

    inline void set_equal_spo_count(uint64_t count) {
        equal_spo_count = count;
    }

    inline void set_equal_sp_count(uint64_t count) {
        equal_sp_count = count;
    }

    inline void set_equal_so_count(uint64_t count) {
        equal_so_count = count;
    }

    inline void set_equal_po_count(uint64_t count) {
        equal_po_count = count;
    }

    inline void set_blank_node_count(uint64_t count) {
        blank_node_count = count;
    }

    void set_predicate_stats(boost::unordered_flat_map<uint64_t, uint64_t>&& predicate_stats) {
        predicate2total_count = std::move(predicate_stats);
    }

    uint64_t get_predicate_count(uint64_t predicate_id) const {
        auto it = predicate2total_count.find(predicate_id);
        if (it != predicate2total_count.end()) {
            return it->second;
        } else {
            return 0;
        }
    }

    uint64_t get_new_blank_node() {
        return blank_node_count++;
    }

    void insert_triple(uint64_t s, uint64_t p, uint64_t o);

    void delete_triple(uint64_t s, uint64_t p, uint64_t o);

    IriPrefixes prefixes;
    std::vector<std::string> datatypes;
    std::vector<std::string> languages;

    // Possible values for permutations:
    // 3: SPO, POS, OSP
    // 4: SPO, POS, OSP, PSO
    // 6: SPO, POS, OSP, PSO, SOP, OPS
    uint64_t permutations;

    TextSearch::TextIndexManager text_index_manager;

    HNSW::HNSWIndexManager hnsw_index_manager;

private:
    uint64_t blank_node_count;

    uint64_t triples_count;

    uint64_t equal_spo_count;
    uint64_t equal_sp_count;
    uint64_t equal_so_count;
    uint64_t equal_po_count;

    boost::unordered_flat_map<uint64_t, uint64_t> predicate2total_count;
};

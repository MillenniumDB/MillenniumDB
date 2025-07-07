#include "rdf_catalog.h"

#include <cassert>

#include "query/exceptions.h"
#include "storage/index/text_search/rdf.h"

using namespace std;

// Constructor for existing catalog
RdfCatalog::RdfCatalog(const std::string& filename) :
    Catalog(filename)
{
    assert(!is_empty());

    auto diff_minor_version = check_version("RDF", MODEL_ID, MAJOR_VERSION, MINOR_VERSION);

    if (diff_minor_version != 0) {
        throw LogicException("Undefined catalog recovery");
    }

    permutations = read_uint64();
    blank_node_count = read_uint64();
    triples_count = read_uint64();

    equal_spo_count = read_uint64();
    equal_sp_count = read_uint64();
    equal_so_count = read_uint64();
    equal_po_count = read_uint64();

    prefixes.init(read_strvec());
    datatypes = read_strvec();
    languages = read_strvec();

    predicate2total_count = read_map();

    text_index_manager.init(
        TextSearch::RDF::index_predicate,
        TextSearch::RDF::index_single,
        TextSearch::RDF::remove_single,
        TextSearch::RDF::oid_to_string
    );
    const auto text_index_name2metadata_size = read_uint64();
    for (uint_fast32_t i = 0; i < text_index_name2metadata_size; ++i) {
        const auto name = read_string();
        TextSearch::TextIndexManager::TextIndexMetadata metadata;
        metadata.normalization_type = static_cast<TextSearch::NORMALIZE_TYPE>(read_uint8());
        metadata.tokenization_type = static_cast<TextSearch::TOKENIZE_TYPE>(read_uint8());
        metadata.predicate_id = ObjectId(read_uint64());
        metadata.predicate = read_string();
        text_index_manager.load_text_index(name, metadata);
    }

    hnsw_index_manager.init();
    const auto hnsw_index_name2metadata_size = read_uint64();
    for (uint_fast32_t i = 0; i < hnsw_index_name2metadata_size; ++i) {
        const auto name = read_string();
        HNSW::HNSWIndexManager::HNSWIndexMetadata metadata;
        metadata.metric_type = static_cast<HNSW::MetricType>(read_uint8());
        metadata.predicate = read_string();
        hnsw_index_manager.load_hnsw_index(name, metadata);
    }
}

// Constructor for new empty catalog
RdfCatalog::RdfCatalog(const std::string& filename, size_t permutations) :
    Catalog(filename),
    permutations(permutations)
{
    assert(is_empty());
    blank_node_count = 0;
    triples_count = 0;

    equal_spo_count = 0;
    equal_sp_count = 0;
    equal_so_count = 0;
    equal_po_count = 0;

    has_changes = true;
}

RdfCatalog::~RdfCatalog()
{
    if (has_changes || text_index_manager.has_changes() || hnsw_index_manager.has_changes()) {
        save();
    }
}

void RdfCatalog::save()
{
    start_write(MODEL_ID, MAJOR_VERSION, MINOR_VERSION);

    write_uint64(permutations);
    write_uint64(blank_node_count);
    write_uint64(triples_count);

    write_uint64(equal_spo_count);
    write_uint64(equal_sp_count);
    write_uint64(equal_so_count);
    write_uint64(equal_po_count);

    write_strvec(prefixes.get_prefix_list());
    write_strvec(datatypes);
    write_strvec(languages);

    write_map(predicate2total_count);

    const auto text_index_name2metadata = text_index_manager.get_name2metadata();
    write_uint64(text_index_name2metadata.size());
    for (const auto& [name, metadata] : text_index_name2metadata) {
        write_string(name);
        write_uint8(static_cast<uint8_t>(metadata.normalization_type));
        write_uint8(static_cast<uint8_t>(metadata.tokenization_type));
        write_uint64(metadata.predicate_id.id);
        write_string(metadata.predicate);
    }

    const auto hnsw_index_name2metadata = hnsw_index_manager.get_name2metadata();
    write_uint64(hnsw_index_name2metadata.size());
    for (const auto& [name, metadata] : hnsw_index_name2metadata) {
        write_string(name);
        write_uint8(static_cast<uint8_t>(metadata.metric_type));
        write_string(metadata.predicate);
    }
}

void RdfCatalog::print(std::ostream& os)
{
    os << "-------------------------------------\n";
    os << "Catalog:\n";
    os << "  triples:                " << triples_count << "\n";
    os << "  distinct predicates:    " << predicate2total_count.size() << "\n";

    os << "  blank nodes count:      " << blank_node_count << "\n";

    os << "  triples with S = P = O: " << equal_spo_count << "\n";
    os << "  triples with S = P:     " << equal_sp_count << "\n";
    os << "  triples with S = O:     " << equal_so_count << "\n";
    os << "  triples with P = O:     " << equal_po_count << "\n";

    os << "  Index permutations: ";
    switch (permutations) {
    case 3:
        os << "SPO, POS, OSP\n";
        break;
    case 4:
        os << "SPO, POS, OSP, PSO\n";
        break;
    case 6:
        os << "SPO, POS, OSP, PSO, SOP, OPS\n";
        break;
    default:
        break;
    }

    const auto text_index_name2metadata = text_index_manager.get_name2metadata();
    if (!text_index_name2metadata.empty()) {
        os << "  Text Indexes (" << text_index_name2metadata.size() << "):\n";
        for (const auto& [name, metadata] : text_index_name2metadata) {
            os << "    " << name << ": " << metadata << "\n";
        }
    }

    const auto hnsw_index_name2metadata = hnsw_index_manager.get_name2metadata();
    if (!hnsw_index_name2metadata.empty()) {
        os << "  HNSW Indexes (" << hnsw_index_name2metadata.size() << "):\n";
        for (const auto& [name, metadata] : hnsw_index_name2metadata) {
            os << "    " << name << ": " << metadata << "\n";
        }
    }

    os << "-------------------------------------\n";
}

void RdfCatalog::insert_triple(uint64_t s, uint64_t p, uint64_t o)
{
    triples_count++;

    if (s == p) {
        equal_sp_count++;
        if (p == o) {
            equal_spo_count++;
        }
    }
    if (s == o) {
        equal_so_count++;
    }
    if (p == o) {
        equal_po_count++;
    }

    predicate2total_count[p]++;
    has_changes = true;
}

void RdfCatalog::delete_triple(uint64_t s, uint64_t p, uint64_t o)
{
    triples_count--;

    if (s == p) {
        equal_sp_count--;
        if (p == o) {
            equal_spo_count--;
        }
    }
    if (s == o) {
        equal_so_count--;
    }
    if (p == o) {
        equal_po_count--;
    }

    auto it = predicate2total_count.find(p);
    if (it != predicate2total_count.end()) {
        it->second--;
        if (it->second == 0) {
            predicate2total_count.erase(it);
        }
    } else {
        assert(false);
    }
    has_changes = true;
}

#include "rdf_catalog.h"

#include <cassert>

#include "graph_models/exceptions.h"

using namespace std;

// Constructor for existing catalog
RdfCatalog::RdfCatalog(const std::string& filename) : Catalog(filename) {
    assert(!is_empty());

    start_io();
    auto model_id = read_uint64();
    if (model_id != MODEL_ID) {
        std::string error_msg = "Wrong MODEL_ID for RdfCatalog, expected: ";
        error_msg += std::to_string(MODEL_ID);
        error_msg += ", got: ";
        error_msg += std::to_string(model_id);
        throw WrongModelException(error_msg);
    }
    auto version_id = read_uint64();
    if (version_id != VERSION) {
        std::string error_msg = "Wrong version for RdfCatalog, expected: v";
        error_msg += std::to_string(VERSION);
        error_msg += ", got: v";
        error_msg += std::to_string(version_id);
        throw WrongCatalogVersionException(error_msg);
    }
    permutations     = read_uint64();
    blank_node_count = read_uint64();
    triples_count    = read_uint64();

    equal_spo_count = read_uint64();
    equal_sp_count  = read_uint64();
    equal_so_count  = read_uint64();
    equal_po_count  = read_uint64();

    prefixes.init(read_strvec());
    datatypes = read_strvec();
    languages = read_strvec();


    auto distinct_predicates = read_uint64();
    for (uint_fast32_t i = 0; i < distinct_predicates; i++) {
        auto predicate_id          = read_uint64();
        auto predicate_total_count = read_uint64();
        predicate2total_count.insert({ predicate_id, predicate_total_count });
    }
}


// Constructor for new empty catalog
RdfCatalog::RdfCatalog(const std::string& filename, size_t permutations) :
    Catalog(filename), permutations(permutations)
{
    assert(is_empty());
    blank_node_count = 0;
    triples_count    = 0;

    equal_spo_count = 0;
    equal_sp_count  = 0;
    equal_so_count  = 0;
    equal_po_count  = 0;

    has_changes = true;
}


RdfCatalog::~RdfCatalog() {
    if (has_changes) {
        save();
    }
}


void RdfCatalog::save() {
    start_io();

    write_uint64(MODEL_ID);
    write_uint64(VERSION);

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

    write_uint64(predicate2total_count.size());
    for (auto&&[k, v] : predicate2total_count) {
        write_uint64(k);
        write_uint64(v);
    }
}


void RdfCatalog::print(std::ostream& os) {
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

    os << "-------------------------------------\n";
}


void RdfCatalog::insert_triple(uint64_t s, uint64_t p, uint64_t o) {
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


void RdfCatalog::delete_triple(uint64_t s, uint64_t p,uint64_t o) {
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

#include "rdf_catalog.h"

using namespace std;

RdfCatalog::RdfCatalog(const std::string& filename) : Catalog(filename) {
    if (is_empty()) {
        blank_nodes_count = 0;
        triples_count     = 0;

        distinct_subjects   = 0;
        distinct_predicates = 0;
        distinct_objects    = 0;

        equal_spo_count = 0;
        equal_sp_count  = 0;
        equal_so_count  = 0;
        equal_po_count  = 0;
    } else {
        start_io();
        blank_nodes_count = read_uint64();
        triples_count     = read_uint64();

        distinct_subjects   = read_uint64();
        distinct_predicates = read_uint64();
        distinct_objects    = read_uint64();

        equal_spo_count = read_uint64();
        equal_sp_count  = read_uint64();
        equal_so_count  = read_uint64();
        equal_po_count  = read_uint64();

        prefixes  = read_strvec();
        datatypes = read_strvec();
        languages = read_strvec();

        for (uint_fast32_t i = 0; i < distinct_predicates; i++) {
            auto predicate_id          = read_uint64();
            auto predicate_total_count = read_uint64();
            predicate2total_count.insert({ predicate_id, predicate_total_count });
        }
    }
}


void RdfCatalog::save_changes() {
    start_io();

    write_uint64(blank_nodes_count);
    write_uint64(triples_count);

    write_uint64(distinct_subjects);
    write_uint64(distinct_predicates);
    write_uint64(distinct_objects);

    write_uint64(equal_spo_count);
    write_uint64(equal_sp_count);
    write_uint64(equal_so_count);
    write_uint64(equal_po_count);

    write_strvec(prefixes);
    write_strvec(datatypes);
    write_strvec(languages);

    for (auto&&[k, v] : predicate2total_count) {
        write_uint64(k);
        write_uint64(v);
    }
}

void RdfCatalog::print() {
    cout << "-------------------------------------\n";
    cout << "Catalog:\n";
    cout << "  blank nodes count:   " << blank_nodes_count << "\n";
    cout << "  triples count:       " << triples_count << "\n";

    cout << "  distinct subjects:   " << distinct_subjects << "\n";
    cout << "  distinct predicates: " << distinct_predicates << "\n";
    cout << "  distinct objects:    " << distinct_objects << "\n";

    cout << "  equal_spo_count:     " << equal_spo_count << "\n";
    cout << "  equal_sp_count:      " << equal_sp_count << "\n";
    cout << "  equal_so_count:      " << equal_so_count << "\n";
    cout << "  equal_po_count:      " << equal_po_count << "\n";
    cout << "-------------------------------------\n";
}
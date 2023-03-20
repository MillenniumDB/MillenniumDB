#pragma once

#include <iostream>
#include <stdint.h>
#include <string>

#include "storage/catalog/catalog.h"
#include "third_party/robin_hood/robin_hood.h"

class RdfCatalog : public Catalog {
public:
    RdfCatalog(const std::string& filename);

    void print();
    void save_changes();

    uint64_t blank_nodes_count;
    uint64_t triples_count;

    uint64_t distinct_subjects;
    uint64_t distinct_predicates;
    uint64_t distinct_objects;

    uint64_t equal_spo_count;
    uint64_t equal_sp_count;
    uint64_t equal_so_count;
    uint64_t equal_po_count;

    std::vector<std::string> prefixes;
    std::vector<std::string> datatypes;
    std::vector<std::string> languages;

    robin_hood::unordered_map<uint64_t, uint64_t> predicate2total_count;
};
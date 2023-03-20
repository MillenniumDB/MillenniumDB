#include "quad_catalog.h"

#include <cassert>
#include <iostream>

using namespace std;

QuadCatalog::QuadCatalog(const std::string& filename) :
    Catalog (filename)
{
    if (is_empty()) {
        identifiable_nodes_count = 0;
        anonymous_nodes_count    = 0;
        connections_count        = 0;

        label_count              = 0;
        properties_count         = 0;

        distinct_labels          = 0;
        distinct_keys            = 0;
        distinct_from            = 0;
        distinct_to              = 0;
        distinct_type            = 0;

        equal_from_to_count      = 0;
        equal_from_type_count    = 0;
        equal_to_type_count      = 0;
        equal_from_to_type_count = 0;
    }
    else {
        start_io();
        identifiable_nodes_count = read_uint64();
        anonymous_nodes_count    = read_uint64();
        connections_count        = read_uint64();

        label_count              = read_uint64();
        properties_count         = read_uint64();

        distinct_labels          = read_uint64();
        distinct_from            = read_uint64();
        distinct_to              = read_uint64();
        distinct_type            = read_uint64();
        distinct_keys            = read_uint64();

        equal_from_to_count      = read_uint64();
        equal_from_type_count    = read_uint64();
        equal_to_type_count      = read_uint64();
        equal_from_to_type_count = read_uint64();

        for (uint_fast32_t i = 0; i < distinct_labels; i++) {
            auto label_id          = read_uint64();
            auto label_total_count = read_uint64();
            label2total_count.insert({ label_id, label_total_count });
        }

        for (uint_fast32_t i = 0; i < distinct_type; i++) {
            auto type_id          = read_uint64();
            auto type_total_count = read_uint64();
            type2total_count.insert({ type_id, type_total_count });
        }

        for (uint_fast32_t i = 0; i < distinct_keys; i++) {
            auto key_id           = read_uint64();
            auto key_total_count  = read_uint64();
            key2total_count.insert({ key_id, key_total_count });
        }

        for (uint_fast32_t i = 0; i < distinct_keys; i++) {
            auto key_id             = read_uint64();
            auto key_distinct_count = read_uint64();
            key2distinct.insert({ key_id, key_distinct_count });
        }

        const auto type2equal_from_to_type_count_size = read_uint64();
        for (uint_fast32_t i = 0; i < type2equal_from_to_type_count_size; i++) {
            auto type  = read_uint64();
            auto count = read_uint64();
            type2equal_from_to_type_count.insert({ type, count });
        }

        const auto type2equal_from_to_count_size = read_uint64();
        for (uint_fast32_t i = 0; i < type2equal_from_to_count_size; i++) {
            auto type  = read_uint64();
            auto count = read_uint64();
            type2equal_from_to_count.insert({ type, count });
        }

        const auto type2equal_from_type_count_size = read_uint64();
        for (uint_fast32_t i = 0; i < type2equal_from_type_count_size; i++) {
            auto type  = read_uint64();
            auto count = read_uint64();
            type2equal_from_type_count.insert({ type, count });
        }

        const auto type2equal_to_type_count_size = read_uint64();
        for (uint_fast32_t i = 0; i < type2equal_to_type_count_size; i++) {
            auto type  = read_uint64();
            auto count = read_uint64();
            type2equal_to_type_count.insert({ type, count });
        }
    }
}


void QuadCatalog::save_changes() {
    start_io();

    write_uint64(identifiable_nodes_count);
    write_uint64(anonymous_nodes_count);
    write_uint64(connections_count);

    write_uint64(label_count);
    write_uint64(properties_count);

    write_uint64(distinct_labels);
    write_uint64(distinct_from);
    write_uint64(distinct_to);
    write_uint64(distinct_type);
    write_uint64(distinct_keys);

    write_uint64(equal_from_to_count);
    write_uint64(equal_from_type_count);
    write_uint64(equal_to_type_count);
    write_uint64(equal_from_to_type_count);

    assert(distinct_labels == label2total_count.size());
    assert(distinct_type   == type2total_count.size());
    assert(distinct_keys   == key2total_count.size());
    assert(distinct_keys   == key2distinct.size());

    for (auto&&[k, v] : label2total_count) {
        write_uint64(k);
        write_uint64(v);
    }

    for (auto&&[k, v] : type2total_count) {
        write_uint64(k);
        write_uint64(v);
    }

    for (auto&&[k, v] : key2total_count) {
        write_uint64(k);
        write_uint64(v);
    }

    for (auto&&[k, v] : key2distinct) {
        write_uint64(k);
        write_uint64(v);
    }

    write_uint64(type2equal_from_to_type_count.size());
    for (auto&&[k, v] : type2equal_from_to_type_count) {
        write_uint64(k);
        write_uint64(v);
    }

    write_uint64(type2equal_from_to_count.size());
    for (auto&&[k, v] : type2equal_from_to_count) {
        write_uint64(k);
        write_uint64(v);
    }

    write_uint64(type2equal_from_type_count.size());
    for (auto&&[k, v] : type2equal_from_type_count) {
        write_uint64(k);
        write_uint64(v);
    }

    write_uint64(type2equal_to_type_count.size());
    for (auto&&[k, v] : type2equal_to_type_count) {
        write_uint64(k);
        write_uint64(v);
    }
}


void QuadCatalog::print() {
    cout << "-------------------------------------\n";
    cout << "Catalog:\n";
    cout << "  identifiable nodes count: " << identifiable_nodes_count << "\n";
    cout << "  anonymous nodes count:    " << anonymous_nodes_count    << "\n";
    cout << "  connections count:        " << connections_count        << "\n";

    cout << "  label count:              " << label_count              << "\n";
    cout << "  properties count:         " << properties_count         << "\n";

    cout << "  distinct labels:          " << distinct_labels          << "\n";
    cout << "  distinct from's:          " << distinct_from            << "\n";
    cout << "  distinct to's:            " << distinct_to              << "\n";
    cout << "  distinct type's:          " << distinct_type            << "\n";
    cout << "  distinct keys:            " << distinct_keys            << "\n";

    cout << "  equal_from_to_count:      " << equal_from_to_count      << "\n";
    cout << "  equal_from_type_count:    " << equal_from_type_count    << "\n";
    cout << "  equal_to_type_count:      " << equal_to_type_count      << "\n";
    cout << "  equal_from_to_type_count: " << equal_from_to_type_count << "\n";
    cout << "-------------------------------------\n";
}


uint64_t QuadCatalog::connections_with_type(uint64_t type_id) {
    // NOT using type2total_count[type_id] because it would insert a 0 in the map if the key is no found
    auto search = type2total_count.find(type_id);
    if (search == type2total_count.end()) {
        return 0;
    } else {
        return search->second;
    }
}


uint64_t QuadCatalog::equal_from_to_type_with_type(uint64_t type_id) {
    auto search = type2equal_from_to_type_count.find(type_id);
    if (search == type2equal_from_to_type_count.end()) {
        return 0;
    } else {
        return search->second;
    }
}


uint64_t QuadCatalog::equal_from_to_with_type(uint64_t type_id) {
    auto search = type2equal_from_to_count.find(type_id);
    if (search == type2equal_from_to_count.end()) {
        return 0;
    } else {
        return search->second;
    }
}


uint64_t QuadCatalog::equal_from_type_with_type(uint64_t type_id) {
    auto search = type2equal_from_type_count.find(type_id);
    if (search == type2equal_from_type_count.end()) {
        return 0;
    } else {
        return search->second;
    }
}


uint64_t QuadCatalog::equal_to_type_with_type(uint64_t type_id) {
    auto search = type2equal_to_type_count.find(type_id);
    if (search == type2equal_to_type_count.end()) {
        return 0;
    } else {
        return search->second;
    }
}

#include "quad_catalog.h"

#include "storage/index/text_search/quad.h"

#include <cassert>

using namespace std;

QuadCatalog::QuadCatalog(const std::string& filename) :
    Catalog(filename)
{
    if (is_empty()) {
        identifiable_nodes_count = 0;
        anonymous_nodes_count = 0;

        edge_count = 0;
        label_count = 0;
        properties_count = 0;

        equal_from_to_count = 0;
        equal_from_type_count = 0;
        equal_to_type_count = 0;
        equal_from_to_type_count = 0;

        has_changes = true;
    } else {
        start_io();
        if (read_uint64() != MODEL_ID) {
            throw runtime_error("QuadCatalog: wrong MODEL_ID");
        }
        if (read_uint64() != VERSION) {
            throw runtime_error("QuadCatalog: wrong VERSION");
        }
        identifiable_nodes_count = read_uint64();
        anonymous_nodes_count = read_uint64();

        edge_count = read_uint64();
        label_count = read_uint64();
        properties_count = read_uint64();

        equal_from_to_count = read_uint64();
        equal_to_type_count = read_uint64();
        equal_from_type_count = read_uint64();
        equal_from_to_type_count = read_uint64();

        const auto distinct_labels = read_uint64();
        for (uint_fast32_t i = 0; i < distinct_labels; i++) {
            auto label_id = read_uint64();
            auto label_total_count = read_uint64();
            label2total_count.insert({ label_id, label_total_count });
        }

        const auto distinct_keys = read_uint64();
        for (uint_fast32_t i = 0; i < distinct_keys; i++) {
            auto key_id = read_uint64();
            auto key_total_count = read_uint64();
            key2total_count.insert({ key_id, key_total_count });
        }

        const auto distinct_types = read_uint64();
        for (uint_fast32_t i = 0; i < distinct_types; i++) {
            auto type_id = read_uint64();
            auto type_total_count = read_uint64();
            type2total_count.insert({ type_id, type_total_count });
        }

        const auto type2equal_from_to_type_count_size = read_uint64();
        for (uint_fast32_t i = 0; i < type2equal_from_to_type_count_size; i++) {
            auto type = read_uint64();
            auto count = read_uint64();
            type2equal_from_to_type_count.insert({ type, count });
        }

        const auto type2equal_from_to_count_size = read_uint64();
        for (uint_fast32_t i = 0; i < type2equal_from_to_count_size; i++) {
            auto type = read_uint64();
            auto count = read_uint64();
            type2equal_from_to_count.insert({ type, count });
        }

        const auto type2equal_from_type_count_size = read_uint64();
        for (uint_fast32_t i = 0; i < type2equal_from_type_count_size; i++) {
            auto type = read_uint64();
            auto count = read_uint64();
            type2equal_from_type_count.insert({ type, count });
        }

        const auto type2equal_to_type_count_size = read_uint64();
        for (uint_fast32_t i = 0; i < type2equal_to_type_count_size; i++) {
            auto type = read_uint64();
            auto count = read_uint64();
            type2equal_to_type_count.insert({ type, count });
        }

        text_search_index_manager.init(
            TextSearch::Quad::index_predicate,
            TextSearch::Quad::index_single,
            TextSearch::Quad::remove_single,
            TextSearch::Quad::oid_to_string
        );
        const auto name2metadata = read_uint64();
        for (uint_fast32_t i = 0; i < name2metadata; i++) {
            const auto name = read_string();
            TextSearch::TextSearchIndexManager::TextSearchIndexMetadata metadata;
            metadata.normalization_type = static_cast<TextSearch::NORMALIZE_TYPE>(read_uint8());
            metadata.tokenization_type = static_cast<TextSearch::TOKENIZE_TYPE>(read_uint8());
            metadata.predicate = read_string();
            text_search_index_manager.load_text_search_index(name, metadata);
        }
    }
}

QuadCatalog::~QuadCatalog()
{
    if (has_changes || text_search_index_manager.has_changes()) {
        save();
    }
}

void QuadCatalog::save()
{
    start_io();

    write_uint64(MODEL_ID);
    write_uint64(VERSION);

    write_uint64(identifiable_nodes_count);
    write_uint64(anonymous_nodes_count);

    write_uint64(edge_count);
    write_uint64(label_count);
    write_uint64(properties_count);

    write_uint64(equal_from_to_count);
    write_uint64(equal_to_type_count);
    write_uint64(equal_from_type_count);
    write_uint64(equal_from_to_type_count);

    write_uint64(label2total_count.size());
    for (auto&& [k, v] : label2total_count) {
        write_uint64(k);
        write_uint64(v);
    }

    write_uint64(key2total_count.size());
    for (auto&& [k, v] : key2total_count) {
        write_uint64(k);
        write_uint64(v);
    }

    write_uint64(type2total_count.size());
    for (auto&& [k, v] : type2total_count) {
        write_uint64(k);
        write_uint64(v);
    }

    write_uint64(type2equal_from_to_type_count.size());
    for (auto&& [k, v] : type2equal_from_to_type_count) {
        write_uint64(k);
        write_uint64(v);
    }

    write_uint64(type2equal_from_to_count.size());
    for (auto&& [k, v] : type2equal_from_to_count) {
        write_uint64(k);
        write_uint64(v);
    }

    write_uint64(type2equal_from_type_count.size());
    for (auto&& [k, v] : type2equal_from_type_count) {
        write_uint64(k);
        write_uint64(v);
    }

    write_uint64(type2equal_to_type_count.size());
    for (auto&& [k, v] : type2equal_to_type_count) {
        write_uint64(k);
        write_uint64(v);
    }

    const auto& name2metadata = text_search_index_manager.get_name2metadata();
    write_uint64(name2metadata.size());
    for (const auto& [name, metadata] : name2metadata) {
        write_string(name);
        write_uint8(static_cast<uint8_t>(metadata.normalization_type));
        write_uint8(static_cast<uint8_t>(metadata.tokenization_type));
        write_string(metadata.predicate);
    }
}

void QuadCatalog::print(std::ostream& os)
{
    os << "-------------------------------------\n";
    os << "Catalog:\n";
    os << "  identifiable nodes count: " << identifiable_nodes_count << "\n";
    // os << "  anonymous nodes count:    " << anonymous_nodes_count << "\n";
    os << "  edges count:              " << edge_count << "\n";

    os << "  label count:              " << label_count << "\n";
    os << "  properties count:         " << properties_count << "\n";

    os << "  distinct labels:          " << label2total_count.size() << "\n";
    os << "  distinct keys:            " << key2total_count.size() << "\n";
    os << "  distinct type's:          " << type2total_count.size() << "\n";

    os << "  equal_from_to_count:      " << equal_from_to_count << "\n";
    os << "  equal_to_type_count:      " << equal_to_type_count << "\n";
    os << "  equal_from_type_count:    " << equal_from_type_count << "\n";
    os << "  equal_from_to_type_count: " << equal_from_to_type_count << "\n";
    os << "-------------------------------------\n";
}

uint64_t QuadCatalog::connections_with_type(uint64_t type_id) const
{
    auto search = type2total_count.find(type_id);
    if (search == type2total_count.end()) {
        return 0;
    } else {
        return search->second;
    }
}

uint64_t QuadCatalog::equal_from_to_type_with_type(uint64_t type_id) const
{
    auto search = type2equal_from_to_type_count.find(type_id);
    if (search == type2equal_from_to_type_count.end()) {
        return 0;
    } else {
        return search->second;
    }
}

uint64_t QuadCatalog::equal_from_to_with_type(uint64_t type_id) const
{
    auto search = type2equal_from_to_count.find(type_id);
    if (search == type2equal_from_to_count.end()) {
        return 0;
    } else {
        return search->second;
    }
}

uint64_t QuadCatalog::equal_from_type_with_type(uint64_t type_id) const
{
    auto search = type2equal_from_type_count.find(type_id);
    if (search == type2equal_from_type_count.end()) {
        return 0;
    } else {
        return search->second;
    }
}

uint64_t QuadCatalog::equal_to_type_with_type(uint64_t type_id) const
{
    auto search = type2equal_to_type_count.find(type_id);
    if (search == type2equal_to_type_count.end()) {
        return 0;
    } else {
        return search->second;
    }
}

uint64_t QuadCatalog::insert_new_edge(uint64_t from, uint64_t to, uint64_t type)
{
    auto new_edge_id = ++edge_count;

    type2total_count[type]++;
    if (from == to) {
        type2equal_from_to_count[type]++;
        equal_from_to_count++;
        if (from == type) {
            type2equal_from_to_type_count[type]++;
            equal_from_to_type_count++;
        }
    }
    if (from == type) {
        type2equal_from_type_count[type]++;
        equal_from_type_count++;
    }
    if (to == type) {
        type2equal_to_type_count[type]++;
        equal_to_type_count++;
    }

    has_changes = true;
    return new_edge_id;
}

void QuadCatalog::insert_property(uint64_t key)
{
    has_changes = true;
    properties_count++;
    key2total_count[key]++;
}

void QuadCatalog::insert_label(uint64_t label)
{
    has_changes = true;
    label_count++;
    label2total_count[label]++;
}

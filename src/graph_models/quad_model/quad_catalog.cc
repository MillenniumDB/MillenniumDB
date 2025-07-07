#include "quad_catalog.h"

#include "query/exceptions.h"
#include "storage/index/text_search/quad.h"

using namespace std;

QuadCatalog::QuadCatalog(const std::string& filename) :
    Catalog(filename)
{
    if (is_empty()) {
        nodes_count = 0;

        edge_count = 0;
        label_count = 0;
        properties_count = 0;

        equal_from_to_count = 0;
        equal_from_type_count = 0;
        equal_to_type_count = 0;
        equal_from_to_type_count = 0;

        has_changes = true;
    } else {
        auto diff_minor_version = check_version("Quad", MODEL_ID, MAJOR_VERSION, MINOR_VERSION);

        if (diff_minor_version != 0) {
            throw LogicException("Undefined catalog recovery");
        }

        nodes_count = read_uint64();

        [[maybe_unused]]
        auto unused = read_uint64(); // because old layout

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

        text_index_manager.init(
            TextSearch::Quad::index_predicate,
            TextSearch::Quad::index_single,
            TextSearch::Quad::remove_single,
            TextSearch::Quad::oid_to_string
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
}

QuadCatalog::~QuadCatalog()
{
    if (has_changes || text_index_manager.has_changes() || hnsw_index_manager.has_changes()) {
        save();
    }
}

void QuadCatalog::save()
{
    start_write(MODEL_ID, MAJOR_VERSION, MINOR_VERSION);

    write_uint64(nodes_count);
    write_uint64(0); // because old

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

void QuadCatalog::print(std::ostream& os)
{
    os << "-------------------------------------\n";
    os << "Catalog:\n";
    os << "  nodes count:              " << nodes_count << "\n";
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

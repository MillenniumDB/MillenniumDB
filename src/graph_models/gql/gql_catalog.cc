#include "gql_catalog.h"

#include "query/exceptions.h"

GQLCatalog::GQLCatalog(const std::string& filename) :
    Catalog(filename)
{
    if (!is_empty()) {
        auto diff_minor_version = check_version("GQL", MODEL_ID, MAJOR_VERSION, MINOR_VERSION);

        if (diff_minor_version != 0) {
            throw LogicException("Undefined catalog recovery");
        }

        nodes_count = read_uint64();
        directed_edges_count = read_uint64();
        undirected_edges_count = read_uint64();
        node_labels_count = read_uint64();
        edge_labels_count = read_uint64();
        node_properties_count = read_uint64();
        edge_properties_count = read_uint64();
        equal_directed_edges_count = read_uint64();
        equal_undirected_edges_count = read_uint64();

        node_label2total_count = read_map();
        edge_label2total_count = read_map();
        node_key2total_count = read_map();
        edge_key2total_count = read_map();

        node_labels_str = read_strvec();
        edge_labels_str = read_strvec();

        node_labels2id = convert_strvec_to_map(node_labels_str);
        edge_labels2id = convert_strvec_to_map(edge_labels_str);

        node_keys_str = read_strvec();
        edge_keys_str = read_strvec();

        node_keys2id = convert_strvec_to_map(node_keys_str);
        edge_keys2id = convert_strvec_to_map(edge_keys_str);
    } else {
        has_changes = true;
    }
}

GQLCatalog::~GQLCatalog()
{
    if (has_changes) {
        save();
    }
}

void GQLCatalog::print(std::ostream& os)
{
    os << "-------------------------------------\n";
    os << "GQL Catalog:\n";
    os << "  Nodes:                " << nodes_count << "\n";
    os << "  Edges:                " << undirected_edges_count + directed_edges_count << "\n";
    os << "     Directed:          " << directed_edges_count << "\n";
    os << "     Undirected:        " << undirected_edges_count << "\n";
    os << "  Node Labels:          " << node_labels_count << "\n";
    os << "  Edge Labels:          " << edge_labels_count << "\n";
    os << "  Node Properties:      " << node_properties_count << "\n";
    os << "  Edge Properties:      " << edge_properties_count << "\n";
    os << "  Distinct Node Labels: " << node_label2total_count.size() << "\n";
    os << "  Distinct Edge Labels: " << edge_label2total_count.size() << "\n";
    os << "  Distinct Node Keys:   " << node_key2total_count.size() << "\n";
    os << "  Distinct Edge Keys:   " << edge_key2total_count.size() << "\n";
    os << "-------------------------------------\n";
}

void GQLCatalog::save()
{
    start_write(MODEL_ID, MAJOR_VERSION, MINOR_VERSION);

    write_uint64(nodes_count);
    write_uint64(directed_edges_count);
    write_uint64(undirected_edges_count);
    write_uint64(node_labels_count);
    write_uint64(edge_labels_count);

    write_uint64(node_properties_count);
    write_uint64(edge_properties_count);
    write_uint64(equal_directed_edges_count);
    write_uint64(equal_undirected_edges_count);

    write_map(node_label2total_count);
    write_map(edge_label2total_count);
    write_map(node_key2total_count);
    write_map(edge_key2total_count);

    write_strvec(node_labels_str);
    write_strvec(edge_labels_str);

    write_strvec(node_keys_str);
    write_strvec(edge_keys_str);
}

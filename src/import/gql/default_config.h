#pragma once

#include <fstream>
#include <thread>

#include "import/default_config.h"
#include "misc/total_ram.h"
#include "network/server/protocol.h"
#include "system/buffer_manager.h"
#include "system/string_manager.h"
#include "system/tensor_manager.h"

namespace Import { namespace GQL {

inline void create_default_params(const std::string& db_folder)
{
    auto target_ram = (get_total_ram() * 3) / 4;
    auto workers = std::thread::hardware_concurrency();
    if (workers > 8) {
        workers = 8;
    } else if (workers > 2) {
        workers -= 1;
    } else {
        workers = 1;
    }

    uint64_t ideal_strings_static_buffer_bytes = get_file_size(db_folder + "/strings.dat");
    uint64_t ideal_tensors_static_buffer_bytes = get_file_size(db_folder + "/tensors.dat");

    uint64_t ideal_versioned_pages_buffer_bytes = get_file_size(db_folder + "/d_edges.table")
                                                + get_file_size(db_folder + "/edge_key_value.dir")
                                                + get_file_size(db_folder + "/edge_key_value.leaf")
                                                + get_file_size(db_folder + "/edge_label.dir")
                                                + get_file_size(db_folder + "/edge_label.leaf")
                                                + get_file_size(db_folder + "/from_to_edge.dir")
                                                + get_file_size(db_folder + "/from_to_edge.leaf")
                                                + get_file_size(db_folder + "/key_value_edge.dir")
                                                + get_file_size(db_folder + "/key_value_edge.leaf")
                                                + get_file_size(db_folder + "/key_value_node.dir")
                                                + get_file_size(db_folder + "/key_value_node.leaf")
                                                + get_file_size(db_folder + "/label_edge.dir")
                                                + get_file_size(db_folder + "/label_edge.leaf")
                                                + get_file_size(db_folder + "/label_node.dir")
                                                + get_file_size(db_folder + "/label_node.leaf")
                                                + get_file_size(db_folder + "/node_key_value.dir")
                                                + get_file_size(db_folder + "/node_key_value.leaf")
                                                + get_file_size(db_folder + "/node_label.dir")
                                                + get_file_size(db_folder + "/node_label.leaf")
                                                + get_file_size(db_folder + "/to_from_edge.dir")
                                                + get_file_size(db_folder + "/to_from_edge.leaf")
                                                + get_file_size(db_folder + "/u_edge.dir")
                                                + get_file_size(db_folder + "/u_edge.leaf")
                                                + get_file_size(db_folder + "/u_edges.table");

    uint64_t ideal_unversioned_pages_buffer_bytes = get_file_size(db_folder + "/str_hash.dat")
                                                  + get_file_size(db_folder + "/tensor_hash.dat");

    if (ideal_strings_static_buffer_bytes < StringManager::DEFAULT_STATIC_BUFFER) {
        ideal_strings_static_buffer_bytes = StringManager::DEFAULT_STATIC_BUFFER;
    }
    if (ideal_tensors_static_buffer_bytes < TensorManager::DEFAULT_STATIC_BUFFER) {
        ideal_tensors_static_buffer_bytes = TensorManager::DEFAULT_STATIC_BUFFER;
    }
    if (ideal_versioned_pages_buffer_bytes < BufferManager::DEFAULT_VERSIONED_PAGES_BUFFER_SIZE) {
        ideal_versioned_pages_buffer_bytes = BufferManager::DEFAULT_VERSIONED_PAGES_BUFFER_SIZE;
    }
    if (ideal_unversioned_pages_buffer_bytes < BufferManager::DEFAULT_UNVERSIONED_PAGES_BUFFER_SIZE) {
        ideal_unversioned_pages_buffer_bytes = BufferManager::DEFAULT_UNVERSIONED_PAGES_BUFFER_SIZE;
    }

    make_multiple(ideal_strings_static_buffer_bytes, StringManager::MAX_STRING_SIZE);
    make_multiple(ideal_tensors_static_buffer_bytes, TensorManager::MAX_TENSOR_BYTES);
    make_multiple(ideal_versioned_pages_buffer_bytes, VPage::SIZE);
    make_multiple(ideal_unversioned_pages_buffer_bytes, UPage::SIZE);

    uint64_t strings_static_buffer = StringManager::DEFAULT_STATIC_BUFFER;
    uint64_t strings_dynamic_buffer = StringManager::DEFAULT_DYNAMIC_BUFFER;

    uint64_t tensors_static_buffer = TensorManager::DEFAULT_STATIC_BUFFER;
    uint64_t tensors_dynamic_buffer = TensorManager::DEFAULT_DYNAMIC_BUFFER;

    uint64_t private_pages_buffer = BufferManager::DEFAULT_PRIVATE_PAGES_BUFFER_SIZE;
    uint64_t versioned_pages_buffer = BufferManager::DEFAULT_VERSIONED_PAGES_BUFFER_SIZE;
    uint64_t unversioned_pages_buffer = BufferManager::DEFAULT_UNVERSIONED_PAGES_BUFFER_SIZE;

    // case 1: everything fits in target_ram
    if (ideal_strings_static_buffer_bytes + ideal_tensors_static_buffer_bytes
            + ideal_versioned_pages_buffer_bytes + ideal_unversioned_pages_buffer_bytes
        <= target_ram)
    {
        strings_static_buffer = ideal_strings_static_buffer_bytes;
        tensors_static_buffer = ideal_tensors_static_buffer_bytes;
        versioned_pages_buffer = ideal_versioned_pages_buffer_bytes;
        unversioned_pages_buffer = ideal_unversioned_pages_buffer_bytes;

        strings_dynamic_buffer /= 4;
        tensors_dynamic_buffer /= 4;
    }

    // case 2: almost everything fits in target_ram (all + 1/3 of B+trees)
    else if (ideal_strings_static_buffer_bytes + ideal_tensors_static_buffer_bytes
                 + (ideal_versioned_pages_buffer_bytes / 3) + ideal_unversioned_pages_buffer_bytes
             <= target_ram)
    {
        strings_static_buffer = ideal_strings_static_buffer_bytes;
        tensors_static_buffer = ideal_tensors_static_buffer_bytes;
        versioned_pages_buffer = ideal_versioned_pages_buffer_bytes / 3;
        unversioned_pages_buffer = ideal_unversioned_pages_buffer_bytes;

        strings_dynamic_buffer /= 4;
        tensors_dynamic_buffer /= 4;
    }

    std::string strings_static_buffer_str = get_suffix(strings_static_buffer);
    std::string strings_dynamic_buffer_str = get_suffix(strings_dynamic_buffer);
    std::string private_pages_buffer_str = get_suffix(private_pages_buffer);
    std::string versioned_pages_buffer_str = get_suffix(versioned_pages_buffer);
    std::string unversioned_pages_buffer_str = get_suffix(unversioned_pages_buffer);
    std::string tensors_static_buffer_str = get_suffix(tensors_static_buffer);
    std::string tensors_dynamic_buffer_str = get_suffix(tensors_dynamic_buffer);

    std::fstream f(db_folder + "/db.config", std::ios::out);
    f << "# server options:\n"
      << "port = " << MDBServer::Protocol::DEFAULT_PORT << "\n"
      << "browser-port = " << MDBServer::Protocol::DEFAULT_BROWSER_PORT << "\n"
      << "threads = " << workers << " # worker threads\n"
      << "timeout = " << MDBServer::Protocol::DEFAULT_QUERY_TIMEOUT_SECONDS.count()
      << " # timeout in seconds\n"
      << "\n"
      << "# uncomment to set a hard limit in every query\n"
      << "# limit =\n"
      << "\n"
      << "# if admin_user is enabled, updates require authentication\n"
      << "# admin_user =\n"
      << "# admin_password =\n"
      << "\n"
      << "# buffer sizes:\n"
      << "strings-static = " << strings_static_buffer_str << "\n"
      << "strings-dynamic = " << strings_dynamic_buffer_str << "\n"
      << "private-buffer = " << private_pages_buffer_str << "\n"
      << "versioned-buffer = " << versioned_pages_buffer_str << "\n"
      << "unversioned-buffer = " << unversioned_pages_buffer_str << "\n"
      << "tensors-static = " << tensors_static_buffer_str << "\n"
      << "tensors-dynamic = " << tensors_dynamic_buffer_str << "\n";
}
}} // namespace Import::GQL

#pragma once

#include <cstdint>
#include <string>

class System {
public:
    System(
        const std::string& db_folder,
        uint64_t str_static_size,
        uint64_t str_dynamic_size,
        uint64_t shared_buffer_size,
        uint64_t private_buffer_size,
        uint64_t str_hash_buffer_size,
        uint64_t tensor_buffer_size_bytes,
        uint64_t workers
    );

    ~System();
};

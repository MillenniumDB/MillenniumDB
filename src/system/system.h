#pragma once

#include <cstdint>
#include <string>

class System {
public:
    System(
        const std::string& db_folder,
        uint64_t str_initial_populate_size,
        uint64_t shared_buffer_size,
        uint64_t private_buffer_size,
        uint64_t str_hash_buffer_size,
        uint64_t workers
    );

    ~System();
};

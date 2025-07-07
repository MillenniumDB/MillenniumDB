#pragma once

#include <cstdint>
#include <string>

class System {
public:
    static constexpr uint8_t MAJOR_VER = 1;
    static constexpr uint8_t MINOR_VER = 0;
    static constexpr uint8_t PATCH_VER = 0;

    System(
        const std::string& db_folder,
        uint64_t str_static_size,
        uint64_t str_dynamic_size,
        uint64_t shared_buffer_size,
        uint64_t private_buffer_size,
        uint64_t str_hash_buffer_size,
        uint64_t tensor_static_size,
        uint64_t tensor_dynamic_size,
        uint64_t workers
    );

    ~System();
};

#pragma once

#include <cstring>

#include "misc/bytes_encoder.h"
#include "third_party/hashes/hash_function_wrapper.h"

namespace Import {

// Storage for in-memory byte data. It holds an static array of bytes that is used for storing tensor and string dat
class ExternalBytes {
public:
    static char* data;

    uint64_t offset;

    ExternalBytes(uint64_t offset) :
        offset(offset)
    { }

    bool operator==(const ExternalBytes& other) const
    {
        auto ptr1 = Import::ExternalBytes::data + offset;
        auto ptr2 = Import::ExternalBytes::data + other.offset;

        const auto [num_bytes1, num_bytes_read1] = BytesEncoder::read_size(ptr1);
        const auto [num_bytes2, num_bytes_read2] = BytesEncoder::read_size(ptr2);

        if (num_bytes1 != num_bytes2) {
            return false;
        }

        ptr1 += num_bytes_read1;
        ptr2 += num_bytes_read2;

        return memcmp(ptr1, ptr2, num_bytes1) == 0;
    }

    struct Hasher {
        std::size_t operator()(const Import::ExternalBytes& external_bytes) const
        {
            auto ptr = Import::ExternalBytes::data + external_bytes.offset;
            const auto [num_bytes, num_bytes_read] = BytesEncoder::read_size(ptr);
            ptr += num_bytes_read;

            return HashFunctionWrapper(ptr, num_bytes);
        }
    };
};

} // namespace Import

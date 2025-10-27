#pragma once

#include "graph_models/object_id.h"
#include "misc/bytes_encoder.h"
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

class ListEncoder {
public:
    // writes the encoded list to the buffer and returns the size
    static uint64_t encode(std::vector<ObjectId>& list, char* buffer)
    {
        if (list.empty()) {
            *buffer = 0;
            return 1;
        }

        uint64_t size_len = BytesEncoder::write_size(buffer, list.size());
        buffer += size_len;

        for (auto oid : list) {
            memcpy(buffer, &(oid.id), sizeof(ObjectId));
            buffer += sizeof(ObjectId);
        }
        return size_len + list.size() * sizeof(ObjectId);
    }

    static std::vector<ObjectId> decode(char* buffer)
    {
        uint64_t list_size;
        if (*buffer == 0) {
            list_size = 0;
            buffer++;
        } else {
            auto encoder_res = BytesEncoder::read_size(buffer);
            list_size = encoder_res.first;
            buffer += encoder_res.second;
        }

        std::vector<ObjectId> result;

        for (uint64_t i = 0; i < list_size; ++i) {
            ObjectId elem;
            memcpy(&elem, buffer, sizeof(ObjectId));
            buffer += sizeof(ObjectId);
            result.push_back(elem);
        }
        return result;
    }
};

#pragma once

#include "dictionary.h"

class DictionaryEncoder {
private:
    std::unique_ptr<DictionaryItem> tmp;

public:
    std::unique_ptr<Dictionary> decode(char* buffer)
    {
        char type = *buffer;
        buffer++;

        switch (type) {
        case DictionaryItem::TYPE_LITERAL:
            decode_literal(buffer);
            break;
        case DictionaryItem::TYPE_ARRAY:
            decode_array(buffer);
            break;
        case DictionaryItem::TYPE_OBJECT:
            decode_object(buffer);
            break;
        }
        return std::make_unique<Dictionary>(std::move(tmp));
    }

    uint64_t encode(const std::unique_ptr<Dictionary>& item, char* buffer)
    {
        char* initial_buffer = buffer;
        buffer = item->to_external_string(buffer);
        return buffer - initial_buffer;
    }

private:
    uint64_t decode_object(char* buffer)
    {
        char* initial_buffer = buffer;
        auto [size, size_bytes] = BytesEncoder::read_size(buffer);
        buffer += size_bytes;

        std::map<ObjectId, std::unique_ptr<DictionaryItem>> oid2item;

        for (uint64_t i = 0; i < size; i++) {
            ObjectId key;
            memcpy(&key, buffer, sizeof(ObjectId));
            buffer += sizeof(ObjectId);

            char type = *buffer;
            buffer++;

            uint64_t bytes_read;
            switch (type) {
            case DictionaryItem::TYPE_LITERAL:
                bytes_read = decode_literal(buffer);
                break;
            case DictionaryItem::TYPE_ARRAY:
                bytes_read = decode_array(buffer);
                break;
            case DictionaryItem::TYPE_OBJECT:
                bytes_read = decode_object(buffer);
                break;
            }
            buffer += bytes_read;
            oid2item.emplace(key, std::move(tmp));
        }

        tmp = std::make_unique<DictionaryObject>(std::move(oid2item));
        return buffer - initial_buffer;
    }

    uint64_t decode_array(char* buffer)
    {
        char* initial_buffer = buffer;
        auto [size, size_bytes] = BytesEncoder::read_size(buffer);
        buffer += size_bytes;

        std::vector<std::unique_ptr<DictionaryItem>> array;

        for (uint64_t i = 0; i < size; i++) {
            char type = *buffer;
            buffer++;

            uint64_t bytes_read;
            switch (type) {
            case DictionaryItem::TYPE_LITERAL:
                bytes_read = decode_literal(buffer);
                break;
            case DictionaryItem::TYPE_ARRAY:
                bytes_read = decode_array(buffer);
                break;
            case DictionaryItem::TYPE_OBJECT:
                bytes_read = decode_object(buffer);
                break;
            }
            buffer += bytes_read;
            array.push_back(std::move(tmp));
        }

        tmp = std::make_unique<DictionaryArray>(std::move(array));
        return buffer - initial_buffer;
    }

    uint64_t decode_literal(char* buffer)
    {
        ObjectId literal;
        memcpy(&literal, buffer, sizeof(ObjectId));
        tmp = std::make_unique<DictionaryLiteral>(literal);
        return sizeof(ObjectId);
    }
};

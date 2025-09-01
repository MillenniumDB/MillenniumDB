#pragma once

#include <map>
#include <memory>
#include <ostream>
#include <vector>

#include "misc/bytes_encoder.h"
#include "query/query_context.h"

class DictionaryItem {
public:
    enum type {
        TYPE_LITERAL,
        TYPE_ARRAY,
        TYPE_OBJECT
    };

    virtual ~DictionaryItem() = default;
    virtual void to_string(std::ostream& os) const = 0;
    virtual char* to_external_string(char* buffer) const = 0;
};

class DictionaryLiteral : public DictionaryItem {
public:
    ObjectId object_id;

    DictionaryLiteral(ObjectId object) :
        object_id(object)
    { }

    void to_string(std::ostream& os) const override
    {
        os << object_id;
    }

    char* to_external_string(char* buffer) const override
    {
        *buffer = static_cast<char>(TYPE_LITERAL);
        buffer++;
        memcpy(buffer, &(object_id.id), sizeof(ObjectId));
        buffer += sizeof(ObjectId);
        return buffer;
    }
};

class DictionaryArray : public DictionaryItem {
public:
    std::vector<std::unique_ptr<DictionaryItem>> values;

    DictionaryArray() = default;

    DictionaryArray(std::vector<std::unique_ptr<DictionaryItem>> values) :
        values(std::move(values))
    { }

    void add(std::unique_ptr<DictionaryItem> item)
    {
        values.push_back(std::move(item));
    }

protected:
    void to_string(std::ostream& os) const override
    {
        os << "[";

        auto it = values.begin();
        (*it)->to_string(os);
        it++;

        for (; it != values.end(); ++it) {
            os << ",";
            (*it)->to_string(os);
        }
        os << "]";
    }

    char* to_external_string(char* buffer) const override
    {
        *buffer = static_cast<char>(TYPE_ARRAY);
        buffer++;
        uint64_t size = values.size();
        uint64_t size_bytes = BytesEncoder::write_size(buffer, size);
        buffer += size_bytes;

        for (auto& item : values) {
            buffer = item->to_external_string(buffer);
        }

        return buffer;
    }
};

class DictionaryObject : public DictionaryItem {
public:
    std::map<ObjectId, std::unique_ptr<DictionaryItem>> keys;

    DictionaryObject() = default;

    DictionaryObject(std::map<ObjectId, std::unique_ptr<DictionaryItem>> keys) :
        keys(std::move(keys))
    { }

    void add(ObjectId key, std::unique_ptr<DictionaryItem> item)
    {
        keys[key] = std::move(item);
    }

    void to_string(std::ostream& os) const override
    {
        os << "{";

        auto it = keys.begin();
        os << it->first << ":";
        it->second->to_string(os);
        it++;

        for (; it != keys.end(); ++it) {
            os << "," << it->first << ":";
            it->second->to_string(os);
        }
        os << "}";
    }

    char* to_external_string(char* buffer) const override
    {
        *buffer = static_cast<char>(TYPE_OBJECT);
        buffer++;
        uint64_t size = keys.size();
        uint64_t size_bytes = BytesEncoder::write_size(buffer, size);
        buffer += size_bytes;

        for (auto&& [k, v] : keys) {
            memcpy(buffer, &(k.id), sizeof(ObjectId));
            buffer += sizeof(ObjectId);

            buffer = v->to_external_string(buffer);
        }

        return buffer;
    }
};

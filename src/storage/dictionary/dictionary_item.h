#pragma once

#include <map>
#include <memory>
#include <ostream>
#include <vector>

#include "misc/bytes_encoder.h"
#include "query/query_context.h"

class DictionaryItem {
public:
    enum Type {
        TYPE_LITERAL,
        TYPE_ARRAY,
        TYPE_OBJECT
    };

    virtual ~DictionaryItem() = default;
    virtual void to_string(std::ostream& os) const = 0;
    virtual char* to_external_string(char* buffer) const = 0;
    virtual bool is_equal(const std::unique_ptr<DictionaryItem>& other) const = 0;
};

class DictionaryLiteral : public DictionaryItem {
public:
    ObjectId object_id;

    DictionaryLiteral(ObjectId object) :
        object_id(object)
    { }

    bool is_equal(const std::unique_ptr<DictionaryItem>& other) const override
    {
        auto other_ptr = dynamic_cast<DictionaryLiteral*>(other.get());
        return other_ptr != nullptr && object_id == other_ptr->object_id;
    }

protected:
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

    bool is_equal(const std::unique_ptr<DictionaryItem>& other) const override
    {
        auto other_ptr = dynamic_cast<DictionaryArray*>(other.get());

        if (other_ptr == nullptr || other_ptr->values.size() != values.size()) {
            return false;
        }

        for (uint64_t i = 0; i < values.size(); ++i) {
            if (!values[i]->is_equal(other_ptr->values[i])) {
                return false;
            }
        }
        return true;
    }

protected:
    void to_string(std::ostream& os) const override
    {
        os << "[";
        for (auto it = values.begin(); it != values.end(); ++it) {
            if (it != values.begin()) {
                os << ",";
            }
            (*it)->to_string(os);
        }
        os << "]";
    }

    char* to_external_string(char* buffer) const override
    {
        *buffer = static_cast<char>(TYPE_ARRAY);
        buffer++;
        if (values.empty()) {
            *buffer++ = 0;
            return buffer;
        }

        uint64_t size_bytes = BytesEncoder::write_size(buffer, values.size());
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

    bool is_equal(const std::unique_ptr<DictionaryItem>& other) const override
    {
        auto other_ptr = dynamic_cast<DictionaryObject*>(other.get());
        if (other_ptr == nullptr || keys.size() != other_ptr->keys.size()) {
            return false;
        }

        auto comp = [](const std::pair<const ObjectId, std::unique_ptr<DictionaryItem>>& pair1,
                       const std::pair<const ObjectId, std::unique_ptr<DictionaryItem>>& pair2) {
            return pair1.first == pair2.first && pair1.second->is_equal(pair2.second);
        };
        return std::equal(keys.begin(), keys.end(), other_ptr->keys.begin(), other_ptr->keys.end(), comp);
    }

protected:
    void to_string(std::ostream& os) const override
    {
        os << "{";

        for (auto it = keys.begin(); it != keys.end(); ++it) {
            if (it != keys.begin()) {
                os << ",";
            }
            os << it->first << ":";
            it->second->to_string(os);
        }
        os << "}";
    }

    char* to_external_string(char* buffer) const override
    {
        *buffer = static_cast<char>(TYPE_OBJECT);
        buffer++;
        if (keys.empty()) {
            *buffer++ = 0;
            return buffer;
        }

        uint64_t size_bytes = BytesEncoder::write_size(buffer, keys.size());
        buffer += size_bytes;

        for (auto&& [k, v] : keys) {
            memcpy(buffer, &(k.id), sizeof(ObjectId));
            buffer += sizeof(ObjectId);

            buffer = v->to_external_string(buffer);
        }

        return buffer;
    }
};

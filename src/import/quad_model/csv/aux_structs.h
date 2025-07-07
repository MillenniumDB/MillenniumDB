#pragma once

#include <cstdint>
#include <string>

namespace Import { namespace QuadModel { namespace CSV {
enum CSVType {
    UNDEFINED,
    ID,
    START_ID,
    END_ID,
    TYPE,
    LABEL,
    STR,
    INT,
    DECIMAL,
    DATE,
    DATETIME
};

struct CSVColumn {
    CSVType type;
    std::string name;
    uint64_t key_id;
    char* value_str;
    size_t value_size;

    CSVColumn(CSVType type, std::string name, uint64_t key_id) :
        type(type),
        name(name),
        key_id(key_id)
    {
        value_str = new char[64 * 1024 * 1024];
        value_size = 0;
    }

    // Not allowed to generate copies
    CSVColumn(const CSVColumn& other) = delete;

    CSVColumn(CSVColumn&& other)
    {
        this->value_str = other.value_str;
        other.value_str = nullptr;
        this->type = other.type;
        this->name = other.name;
        this->key_id = other.key_id;
        this->value_size = other.value_size;
    }

    ~CSVColumn()
    {
        if (value_str)
            delete[] value_str;
    }
};
}}} // namespace Import::QuadModel::CSV
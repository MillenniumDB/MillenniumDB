#pragma once

#include <string>

#include "storage/file_id.h"

// class StringPage;

class CharIter {
public:
    virtual ~CharIter() = default;

    // Once '\0' is returned, calling again next_char() is undefined behaviour.
    virtual char next_char() = 0;
};


class StringTmpIter : public CharIter {
    std::string::const_iterator current;
    std::string::const_iterator end;

public:
    StringTmpIter(const std::string& str) :
        current (str.begin()),
        end     (str.end()) { }

    char next_char() override {
        if (current == end) {
            return '\0';
        } else {
            auto res = *current;
            ++current;
            return res;
        }
    }
};


class StringInlineIter : public CharIter {
    int64_t encoded_value;
    char* current;

public:
    StringInlineIter(int64_t _encoded_value) :
        encoded_value (_encoded_value)
    {
        current = reinterpret_cast<char*>(&encoded_value);
    }

    char next_char() override {
        auto res = *current;
        ++current;
        return res;
    }
};


class StringExternalIter : public CharIter {
    const char* current_block;

    uint_fast32_t bytes_left;
    uint_fast32_t current_page_offset;
    uint_fast32_t current_block_number;

public:
    StringExternalIter(uint64_t string_start, size_t str_len);
    // StringExternalIter(char* ptr);
    ~StringExternalIter();

    char next_char() override;
};

#pragma once

#include <cassert>
#include <cstdint>
#include <memory>
#include <string>

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


class BytesIter : public CharIter {
    const char* current;
    uint64_t remaining;

public:
    BytesIter(const char* bytes, uint64_t size) :
        current (bytes),
        remaining (size) { }

    char next_char() override {
        if (remaining == 0) {
            return '\0';
        } else {
            auto res = *current;
            ++current;
            --remaining;
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
    StringExternalIter(uint64_t string_start, uint64_t str_len);

    char next_char() override;
};


class IriInlineIter : public CharIter {
    StringTmpIter prefix_iter;
    const char* current;
    int64_t encoded_value;
    bool iter_prefix = true;

public:
    IriInlineIter(const std::string& prefix, int64_t _encoded_value) :
        prefix_iter(prefix), encoded_value(_encoded_value)
    {
        current = reinterpret_cast<char*>(&encoded_value);
        assert(current[7] == '\0');
    }

    char next_char() override;
};


class IriExternalIter : public CharIter {
    StringTmpIter prefix_iter;
    std::unique_ptr<CharIter> suffix_iter;
    bool iter_prefix = true;
public:
    IriExternalIter(const std::string& prefix, uint64_t iri_id);

    char next_char() override;
};


class IriTmpIter : public CharIter {
    StringTmpIter prefix_iter;
    std::unique_ptr<CharIter> suffix_iter;
    bool iter_prefix = true;
public:
    IriTmpIter(const std::string& prefix, uint64_t iri_id);

    char next_char() override;
};


class LiteralLanguageInlineIter : public CharIter {
    StringTmpIter language_iter;
    const char* current;
public:
    LiteralLanguageInlineIter(const char prefix[], const std::string& language) :
        language_iter(language)
    {
        current = &prefix[0];
    }

    char next_char() override;
};


class LiteralLanguageExternalIter : public CharIter {
    std::unique_ptr<CharIter> prefix_iter;
    StringTmpIter language_iter;

public:
    LiteralLanguageExternalIter(uint64_t literal_id, const std::string& language);

    char next_char() override;
};


class LiteralDatatypeInlineIter : public CharIter {
    StringTmpIter datatype_iter;
    const char* current;
    bool iter_datatype = true;

public:
    LiteralDatatypeInlineIter(const char prefix[], const std::string& datatype) :
        datatype_iter(datatype)
    {
        current = &prefix[0];
    }

    char next_char() override;
};


class LiteralDatatypeExternalIter : public CharIter {
    std::unique_ptr<CharIter> literal_iter;
    StringTmpIter datatype_iter;
    bool iter_datatype = true;

public:
    LiteralDatatypeExternalIter(uint64_t literal_id, const std::string& datatype);

    char next_char() override;
};

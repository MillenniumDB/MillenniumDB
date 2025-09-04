#pragma once

#include "dictionary_item.h"

class Dictionary {
public:
    std::unique_ptr<DictionaryItem> dictionary;

    Dictionary(std::unique_ptr<DictionaryItem> dictionary) :
        dictionary(std::move(dictionary))
    { }

    Dictionary(std::map<ObjectId, std::unique_ptr<DictionaryItem>> dictionary) :
        dictionary(std::make_unique<DictionaryObject>(std::move(dictionary)))
    { }

    char* to_external_string(char* buffer)
    {
        return dictionary->to_external_string(buffer);
    }

    void to_string(std::ostream& os) const
    {
        dictionary->to_string(os);
    }

    bool operator==(const Dictionary& other)
    {
        return dictionary->is_equal(other.dictionary);
    }
};

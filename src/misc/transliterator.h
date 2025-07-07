#pragma once

#include <mutex>
#include <string>

#include <unicode/translit.h>
#include <unicode/utypes.h>
#include <unicode/locid.h>

/**
 * Singleton class that holds transliterators and transformers for text
 *
 * See: https://unicode-org.github.io/icu/userguide/transforms/general
 * See: https://en.wikipedia.org/wiki/Unicode_character_property
 */
class Transliterator {
public:
    Transliterator(const Transliterator&) = delete;
    Transliterator& operator=(const Transliterator&) = delete;
    Transliterator(Transliterator&&) = delete;
    Transliterator& operator=(Transliterator&&) = delete;

    static const Transliterator* get_instance();

    std::string nfkd_casefold(const std::string& input) const;

    static std::string uppercase(const std::string& input);

    static std::string lowercase(const std::string& input);

    static std::string substr(const std::string& str, size_t start, size_t length);

private:
    Transliterator();

    static Transliterator* instance;

    static std::mutex instance_mutex;

    UErrorCode ec { U_ZERO_ERROR };

    icu::Transliterator* nfkd_casefold_transliterator;
};

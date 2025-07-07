#include "transliterator.h"

#include <stdexcept>

#include <unicode/uchar.h>
#include <unicode/unistr.h>
#include <unicode/uchar.h>

Transliterator* Transliterator::instance = nullptr;
std::mutex Transliterator::instance_mutex;

const Transliterator* Transliterator::get_instance()
{
    std::lock_guard<std::mutex> lck(instance_mutex);
    if (instance == nullptr) {
        instance = new Transliterator();
    }
    return instance;
}

Transliterator::Transliterator()
{
    nfkd_casefold_transliterator = icu::Transliterator::createInstance(
        // NFKD, then remove Mark (nonspacing), then lowercase
        "NFKD; [:Mn:] Remove; Lower",
        UTRANS_FORWARD,
        ec
    );
    if (U_FAILURE(ec)) {
        throw std::runtime_error(
            "Could not create NFKD casefold transliterator. ICU Error code: " + std::to_string(ec)
        );
    }
}

std::string Transliterator::nfkd_casefold(const std::string& input) const
{
    icu::UnicodeString icu_input(input.c_str(), "UTF-8");
    nfkd_casefold_transliterator->transliterate(icu_input);

    std::string res;
    icu_input.toUTF8String(res);
    return res;
}

std::string Transliterator::lowercase(const std::string& input)
{
    icu::UnicodeString unicode_str = icu::UnicodeString::fromUTF8(input);
    unicode_str.toLower();

    std::string res;
    unicode_str.toUTF8String(res);
    return res;
}

std::string Transliterator::uppercase(const std::string& input)
{
    icu::UnicodeString unicode_str = icu::UnicodeString::fromUTF8(input);
    unicode_str.toUpper();

    std::string res;
    unicode_str.toUTF8String(res);
    return res;
}

std::string Transliterator::substr(const std::string& str, size_t start, size_t length)
{
    icu::UnicodeString unicode_str = icu::UnicodeString::fromUTF8(str);

    int32_t count = unicode_str.countChar32();

    // Adjust for out-of-bounds requests
    if (str.empty() || start >= static_cast<size_t>(count) || length == 0) {
        return "";
    }

    if (length == std::string::npos || start + length > static_cast<size_t>(count)) {
        length = count - start;
    }

    icu::UnicodeString substring;
    int32_t pos = unicode_str.moveIndex32(0, start);
    size_t found = 0;

    while (found < length) {
        UChar32 c = unicode_str.char32At(pos);
        substring.append(c);
        pos = unicode_str.moveIndex32(pos, 1);
        found++;
    }

    std::string res;
    substring.toUTF8String(res);

    return res;
}

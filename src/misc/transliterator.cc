#include "transliterator.h"

#include <stdexcept>

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

std::string Transliterator::lowercase(const std::string& input) const {
    icu::UnicodeString icu_input(input.c_str(), "UTF-8");
    icu::Locale null_loc { NULL };
    icu_input.toLower(loc);

    std::string res;
    icu_input.toUTF8String(res);
    return res;
}

std::string Transliterator::uppercase(const std::string& input) const {
    icu::UnicodeString icu_input(input.c_str(), "UTF-8");
    icu::Locale null_loc { NULL };
    icu_input.toUpper(loc);

    std::string res;
    icu_input.toUTF8String(res);
    return res;
}
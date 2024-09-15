#include <iostream>
#include <vector>

#include "storage/scsu.h"

typedef bool TestFunction();

bool equal(const char* a, size_t a_count, const char* b, size_t b_count) {
    if (a_count != b_count) {
        return false;
    }
    auto end = a + a_count;
    while (a < end) {
        if (*a++ != *b++) {
            return false;
        }
    }
    return true;
}


bool round_trip() {
    std::vector<std::string> tests = {
        "🎅🏿",
        "比利时王国",
        "बेल्जियम",
        "a z A Z",
        "コンピュータ",
        "日本語の表記体系",
        "にほんごのひょうきたいけい",
        "0日.本語の表記体系!",
        ".日.本.語.の.表.記.体.系.",
        "。日。本。語。の。表。記。体。系。",
        ".に.ほ.ん.ご.の.ひ.ょ.う.き.た.い.け.い",
        "\t\n\r",
        "جرج واشنگتن",
        "Вашингтон, Джордж",
        "ᔫᕐᒋ ᕚᓯᖕᑐᓐ",
        "ජෝර්ජ් වොෂිංටන්",
        "比利时王国बेल्जियमa z A Zコンピュータ日本語の表記体系にほんごのひょうきたいけい0日." // NOLINT(bugprone-suspicious-missing-comma)
        "本語の表記体系!.日.本.語.の.表.記.体.系.。日。本。語。""の。表。記。体。系。.に.ほ." // NOLINT(bugprone-suspicious-missing-comma)
        "ん.ご.の.ひ.ょ.う.き.た.い.け.い\t\n\rجرج واشنگتنВашингтон, Джорджᔫᕐᒋ ᕚᓯᖕᑐᓐ🎅🏿ජෝර්ජ් වොෂිංටන්",
    };

    // All ASCII characters
    std::string ascii;
    for (unsigned i = 0x00; i <= 0x7F; i++) {
        ascii.push_back(static_cast<char>(i));
    }
    tests.push_back(std::move(ascii));


    auto error = false;
    char compressed[1024*1024];
    char decompressed[1024*1024];


    for (auto& string : tests) {
        auto original       = string.c_str();
        auto original_count = string.size();

        auto compressed_count   = SCSU::compress(original, original_count, compressed);
        auto decompressed_count = SCSU::decompress(compressed, compressed_count, decompressed);

        // std::cout << "String: " << string << "\nOriginal size: " << string.size() << ", Compressed size: " << com_count << "\n\n";

        if (!equal(original, original_count, decompressed, decompressed_count)) {
            error = true;
            auto decompressed_string = std::string(decompressed, decompressed + decompressed_count);
            std::cerr << "Original: \"" << string << "\" Decompressed: \"" << decompressed_string << "\"\n";
        }
    }

    return error;
}


int main() {
    std::vector<TestFunction*> tests;

    tests.push_back(&round_trip);

    auto error = false;

    for (auto& test_func : tests) {
        if (test_func()) {
            error = true;
        }
    }

    return error;
}

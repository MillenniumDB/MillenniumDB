#include "tokenize_normalize.h"
#include "misc/locale.h"

#include <clocale>
#include <codecvt>
#include <cuchar>

#include "misc/locale.h"

namespace TextSearch {


std::vector<std::string> noop_tokenize(const std::string& string) {
    return { string };
}


std::string noop_normalize(const std::string& string) {
    return string;
}


std::vector<std::string> std_tokenize(const std::string& string) {
    std::vector<std::string> vec;
    std::string str;

    bool previous_whitespace = false;

    for (auto c : string) {
        switch (c) {
            case '!':
            case '?':
            case '\'':
            case '"':
            case ':':
            case ';':
            case ',':
            case '.':
            case ' ':
            case '\t': {
                if (previous_whitespace) {
                    continue;
                }

                vec.push_back(std::move(str));
                str.clear();
                previous_whitespace = true;
                continue;
            }
            default: {
                previous_whitespace = false;
                str += c;
            }
        }
    }

    if (!str.empty()) {
        vec.push_back(std::move(str));
    }

    return vec;
}


std::string std_normalize(const std::string& str) {
    auto locale = misc::get_locale();
    std::wstring_convert<std::codecvt_utf8<wchar_t>> str_conv;
    auto wstr = str_conv.from_bytes(str);

    std::wstring w_normalized;

    for (auto wc : wstr) {
        // To lower case
        if (wc >= 'A' && wc <= 'Z') {
            wc = wc - 'A' + 'a';
        } else {
            switch(wc) {
            case L'ñ':
            case L'Ñ':
                wc = L'ñ'; break;
            case L'å':
            case L'Å':
            case L'ä':
            case L'Ä':
            case L'á':
            case L'Á':
                wc = L'a'; break;
            case L'ë':
            case L'Ë':
            case L'é':
            case L'É':
                wc = L'e'; break;
            case L'ï':
            case L'Ï':
            case L'í':
            case L'Í':
                wc = L'i'; break;
            case L'ø':
            case L'Ø':
            case L'ö':
            case L'Ö':
            case L'ó':
            case L'Ó':
                wc = L'o'; break;
            case L'ü':
            case L'Ü':
            case L'ú':
            case L'Ú':
                wc = L'u'; break;
            case L'ç':
            case L'Ç':
                wc = L'c'; break;
            default:
                break;
            }
        }
        w_normalized += wc;
    }

    return str_conv.to_bytes(w_normalized);
}


// std::string std_normalize(const std::string& string) {
//     std::setlocale(LC_ALL, "en_US.UTF-8");

//     // Includes space for null pointer
//     auto byte_count = string.size();
//     const char* src = string.c_str();
//     const char* src_end = src + byte_count;

//     auto wstr = new char32_t[byte_count];
//     auto wstr_copy = wstr;
//     auto norm = new char[byte_count];

//     std::mbstate_t state{};

//     // Convert string into char32_t and store in wstr
//     while (src < src_end) {
//         auto count = std::mbrtoc32(wstr_copy, src, src_end - src, &state);
//         if (count == 0) {
//             break;
//         } else if (count > 0) {
//             wstr_copy++;
//             src += count;
//         } else {
//             continue;
//         }
//     }

//     size_t wstr_len = wstr_copy - wstr;

//     auto norm_copy = norm;
//     state =  std::mbstate_t();
//     for (size_t i = 0; i < wstr_len; i++) {
//         auto w = wstr[i];

//         // To lower case
//         if (w >= 'A' && w <= 'Z') {
//             w = w - 'A' + 'a';
//         } else {
//             switch(wstr[i]) {
//             case L'ñ':
//             case L'Ñ': w = L'ñ'; break;
//             case L'á':
//             case L'Á': w = L'a'; break;
//             case L'é':
//             case L'É': w = L'e'; break;
//             case L'í':
//             case L'Í': w = L'i'; break;
//             case L'ó':
//             case L'Ó': w = L'o'; break;
//             case L'ú':
//             case L'Ú': w = L'i'; break;
//             }
//         }
//         norm_copy += std::c32rtomb(norm_copy, w, &state);
//     }

//     std::string res(norm, norm_copy);
//     delete [] wstr;
//     delete [] norm;
//     return res;
// }

} // namespace TextSearch

#pragma once

#include <cstdint>
#include <cstring>
#include <string>

namespace UnicodeEscape {
inline bool is_hex(char byte)
{
    return (('a' <= byte && byte <= 'f') || ('A' <= byte && byte <= 'F') || ('0' <= byte && byte <= '9'));
}

constexpr uint8_t replacement_char[] = { 0xEF, 0xBF, 0xBD };

inline uint8_t read_codepoint(char* str_ptr, char* write_ptr, char* end_ptr, uint8_t unicode_length)
{
    // skip u character
    str_ptr++;

    if (str_ptr + unicode_length > end_ptr) {
        return 0;
    }

    uint8_t buffer[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    for (unsigned i = 0; i < unicode_length; ++i) {
        if (is_hex(*str_ptr)) {
            buffer[i] = *str_ptr;
            str_ptr++;
        } else {
            return 0;
        }
    }

    const uint32_t code = strtoul((const char*) buffer, NULL, 16);

    uint8_t size = 0;
    if (code < 0x0080) {
        size = 1;
    } else if (code < 0x0800) {
        size = 2;
    } else if (code < 0x00010000) {
        size = 3;
    } else if (code < 0x00110000) {
        size = 4;
    } else {
        memcpy(write_ptr, replacement_char, 3);
        return 3;
    }

    uint32_t c = code;
    switch (size) {
    case 4:
        write_ptr[3] = (uint8_t) (0x80u | (c & 0x3Fu));
        c >>= 6;
        c |= (16 << 12); // set bit 4
        [[fallthrough]];
    case 3:
        write_ptr[2] = (uint8_t) (0x80u | (c & 0x3Fu));
        c >>= 6;
        c |= (32 << 6); // set bit 5
        [[fallthrough]];
    case 2:
        write_ptr[1] = (uint8_t) (0x80u | (c & 0x3Fu));
        c >>= 6;
        c |= 0xC0; // set bits 6 and 7
        [[fallthrough]];
    case 1:
        write_ptr[0] = (uint8_t) c;
        [[fallthrough]];
    default:
        break;
    }
    return size;
}

// str_ptr is supposed to point to the 'u' or 'U', example "u00E9"
inline std::string read_codepoint(const char* str_ptr, const char* end_ptr, uint8_t unicode_length)
{
    // skip u character
    str_ptr++;

    if (str_ptr + unicode_length > end_ptr) {
        return "";
    }

    uint8_t buffer[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    for (unsigned i = 0; i < unicode_length; ++i) {
        if (is_hex(*str_ptr)) {
            buffer[i] = *str_ptr;
            str_ptr++;
        } else {
            return "";
        }
    }

    const uint32_t code = strtoul((const char*) buffer, NULL, 16);

    uint8_t size = 0;
    if (code < 0x0080) {
        size = 1;
    } else if (code < 0x0800) {
        size = 2;
    } else if (code < 0x00010000) {
        size = 3;
    } else if (code < 0x00110000) {
        size = 4;
    } else {
        return std::string(reinterpret_cast<const char*>(replacement_char), 3);
    }

    uint32_t c = code;
    uint8_t write_ptr[5] = { 0, 0, 0, 0, 0 };
    switch (size) {
    case 4:
        write_ptr[3] = (uint8_t) (0x80u | (c & 0x3Fu));
        c >>= 6;
        c |= (16 << 12); // set bit 4
        [[fallthrough]];
    case 3:
        write_ptr[2] = (uint8_t) (0x80u | (c & 0x3Fu));
        c >>= 6;
        c |= (32 << 6); // set bit 5
        [[fallthrough]];
    case 2:
        write_ptr[1] = (uint8_t) (0x80u | (c & 0x3Fu));
        c >>= 6;
        c |= 0xC0; // set bits 6 and 7
        [[fallthrough]];
    case 1:
        write_ptr[0] = (uint8_t) c;
        [[fallthrough]];
    default:
        break;
    }
    return std::string(reinterpret_cast<const char*>(write_ptr), size);
}

inline std::string normalize_string(const std::string& str)
{
    std::string res;
    res.reserve(str.size());

    const char* chars = str.data();
    const char* end = chars + str.size();

    while (chars < end - 1) {
        if (*chars != '\\') {
            res += *chars;
            chars++;
            continue;
        }
        chars++;
        switch (*chars) {
        case 't': {
            res += '\t';
            break;
        }
        case 'b': {
            res += '\b';
            break;
        }
        case 'n': {
            res += '\n';
            break;
        }
        case 'r': {
            res += '\r';
            break;
        }
        case 'f': {
            res += '\f';
            break;
        }
        case '\\': {
            res += '\\';
            break;
        }
        case '"': {
            res += '"';
            break;
        }
        case '\'': {
            res += '\'';
            break;
        }
        case 'u': {
            auto codepoint = UnicodeEscape::read_codepoint(chars, end, 4);
            if (!codepoint.empty()) {
                res += codepoint;
                chars += 4;
            } else {
                res += "\\u";
            }
            break;
        }
        case 'U': {
            auto codepoint = UnicodeEscape::read_codepoint(chars, end, 8);
            if (!codepoint.empty()) {
                res += codepoint;
                chars += 8;
            } else {
                res += "\\U";
            }
            break;
        }
        default: {
            res += '\\';
            res += *chars;
        }
        }
        chars++;
    }

    if (chars < end) {
        res += *chars;
    }

    return res;
}

inline void normalize_string(char* read_ptr, char* write_ptr, char* end, size_t& str_len) {
    while (read_ptr < end) {
        if (*read_ptr != '\\') {
            *write_ptr = *read_ptr;
            read_ptr++;
            write_ptr++;
            continue;
        }
        read_ptr++;
        str_len--;
        switch (*read_ptr) {
            case '"':
                *write_ptr = '"';
                break;
            case '\\':
                *write_ptr = '\\';
                break;
            case 'n':
                *write_ptr = '\n';
                break;
            case 't':
                *write_ptr = '\t';
                break;
            case 'r':
                *write_ptr = '\r';
                break;
            case '/':
                *write_ptr = '/';
                break;
            case 'u': {
                int size = read_codepoint(read_ptr, write_ptr, end, 4);
                if (size != 0) {
                    write_ptr += size - 1;
                    read_ptr += 4;
                    // 5 because of the u and the 4 hex characters
                    str_len += size - 5;
                } else {
                    write_ptr[0] = '\\';
                    write_ptr[1] = 'u';
                    write_ptr++;
                    str_len++;
                }
                break;
            }
            case 'U': {
                int size = read_codepoint(read_ptr, write_ptr, end, 8);
                if (size != 0) {
                    write_ptr += size - 1;
                    read_ptr += 8;
                    // 9 because of the u and the 8 hex characters
                    str_len += size - 9;
                } else {
                    write_ptr[0] = '\\';
                    write_ptr[1] = 'u';
                    write_ptr++;
                    str_len++;
                }
                break;
            }
            default:
                *write_ptr = '\\';
                write_ptr++;
                *write_ptr = *read_ptr;
                str_len++;
                break;
        }
        read_ptr++;
        write_ptr++;
    }
    *write_ptr = '\0';
}
} // namespace UnicodeEscape

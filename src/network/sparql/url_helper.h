#pragma once

#include <string>

class UrlHelper {
    static inline unsigned char from_hex(unsigned char ch) {
        if (ch <= '9' && ch >= '0')
            ch -= '0';
        else if (ch <= 'f' && ch >= 'a')
            ch -= 'a' - 10;
        else if (ch <= 'F' && ch >= 'A')
            ch -= 'A' - 10;
        else
            ch = 0;
        return ch;
    }

public:
    static const std::string decode(const std::string& str) {
        std::string res;
        for (size_t i = 0; i < str.size(); ++i) {
            if (str[i] == '+') {
                res += ' ';
            } else if (str[i] == '%' && str.size() > i+2) {
                const unsigned char ch1 = from_hex(str[i+1]);
                const unsigned char ch2 = from_hex(str[i+2]);
                const unsigned char ch = (ch1 << 4) | ch2;
                res += ch;
                i += 2;
            } else {
                res += str[i];
            }
        }
        return res;
    }
};

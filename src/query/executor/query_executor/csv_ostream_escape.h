#pragma once

#include <sstream>

#include "query/exceptions.h"

class CSVOstreamEscape : public std::stringbuf {
public:
    CSVOstreamEscape(std::ostream& os) :
        os (os) { }

protected:
    int overflow(int i) override {
        char c = static_cast<char>(i);

        switch (c) {
            case '"':
                os << '"';
                os << '"';
                break;
            case '\\':
                os << '\\';
                os << '\\';
                break;
            // case '/':
            //     os << '\\';
            //     os << '/';
            //     break;
            case '\b':
                os << '\\';
                os << 'b';
                break;
            case '\f':
                os << '\\';
                os << 'f';
                break;
            case '\n':
                os << '\\';
                os << 'n';
                break;
            case '\r':
                os << '\\';
                os << 'r';
                break;
            case '\t':
                os << '\\';
                os << 't';
                break;
            default:
                os << c;
        }

        return i;
    }

    int sync() override {
        os.flush();
        return 0;
    }

private:
    std::ostream& os;
};

#pragma once

#include <sstream>

class XMLOstreamEscape : public std::stringbuf {
public:
    XMLOstreamEscape(std::ostream& os) :
        os (os) { }

protected:
    int overflow(int i) override {
        char c = static_cast<char>(i);

        switch (c) {
            case '\t':
                os << "&#09;";
                break;
            case '\n':
                os << "&#10;";
                break;
            case '\r':
                os << "&#13;";
                break;
            case ' ':
                os << "&#32;";
                break;
            case '"':
                os << "&quot;";
                break;
            // Is not necessary to escape single quotes when the surrounding quotes are double quotes
            // case '\'':
            //     os << "&apos;";
            //     break;
            case '<':
                os << "&lt;";
                break;
            case '>':
                os << "&gt;";
                break;
            case '&':
                os << "&amp;";
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

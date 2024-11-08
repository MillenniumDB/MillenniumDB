/*
******************************************************************************

UNICODE, INC. LICENSE AGREEMENT - DATA FILES AND SOFTWARE

Unicode Data Files include all data files under the directories
http://www.unicode.org/Public/ and http://www.unicode.org/reports/.
Unicode Software includes any source code under the directories
http://www.unicode.org/Public/ and http://www.unicode.org/reports/.

NOTICE TO USER: Carefully read the following legal agreement. BY DOWNLOADING,
INSTALLING, COPYING OR OTHERWISE USING UNICODE INC.'S DATA FILES ("DATA FILES"),
AND/OR SOFTWARE ("SOFTWARE"), YOU UNEQUIVOCALLY ACCEPT, AND AGREE TO BE BOUND
BY, ALL OF THE TERMS AND CONDITIONS OF THIS AGREEMENT. IF YOU DO NOT AGREE,
DO NOT DOWNLOAD, INSTALL, COPY, DISTRIBUTE OR USE THE DATA FILES OR SOFTWARE.

COPYRIGHT AND PERMISSION NOTICE

Copyright (C) 2003-2004, Unicode, Inc. and International Business Machines Corporation.
All Rights Reserved.
Distributed under the Terms of Use in http://www.unicode.org/copyright.html.

Permission is hereby granted, free of charge, to any person obtaining a copy of
the Unicode data files and associated documentation (the "Data Files") or
Unicode software and associated documentation (the "Software") to deal in the
Data Files or Software without restriction, including without limitation the
rights to use, copy, modify, merge, publish, distribute, and/or sell copies of
the Data Files or Software, and to permit persons to whom the Data Files or
Software are furnished to do so, provided that (a) the above copyright notice(s)
and this permission notice appear in all copies of the Data Files or Software,
(b) both the above copyright notice(s) and this permission notice appear in
associated documentation, and (c) there is clear notice in each modified Data
File or in the Software as well as in the documentation associated with the Data
File(s) or Software that the data or software has been modified.

THE DATA FILES AND SOFTWARE ARE PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT OF THIRD
PARTY RIGHTS. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR HOLDERS INCLUDED IN THIS
NOTICE BE LIABLE FOR ANY CLAIM, OR ANY SPECIAL INDIRECT OR CONSEQUENTIAL
DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING
OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THE DATA FILES OR
SOFTWARE.

Except as contained in this notice, the name of a copyright holder shall not be
used in advertising or otherwise to promote the sale, use or other dealings in
these Data Files or Software without prior written authorization of the
copyright holder.

******************************************************************************
*   file name:  scsumini.c
*   encoding:   US-ASCII
*   tab size:   8 (not used)
*   indentation:4
*
*   created on: 2003jun24
*   created by: Markus W. Scherer
*
*   Minimal SCSU encoder.
*   This SCSU encoder is sufficient and small but not very efficient.
*   While it could be used as is, it is intended to show that a simple, small
*   yet functioning SCSU encoder is feasible.
*
*   This file will by itself compile on many platforms into a standalone
*   command-line program. A user can type Unicode code points and the program
*   will output the SCSU bytes from the encoder.
*
*   Features:
*   - Encoder state: 4 bits (boolean mode and 3-bit window number)
*   - Uses very small set of SCSU features; does not define dynamic windows
*     but uses predefined ones.
*   - Generates conformant SCSU output.
*   - Generates XML-suitable output.
*   - Encodes U+FEFF as recommended for SCSU signatures.
*   - Encodes US-ASCII and ISO-8859-1 text as itself.
*   - Encodes scripts in compact form for which SCSU has predefined dynamic windows.
*   - Encodes many texts with no more bytes than either UTF-8 or UTF-16.
*   - Encodes some texts with more bytes than UTF-8/16, especially texts in
*     many small scripts:
*     This encoder will use Unicode mode for them (2 bytes per character)
*     and for each space character switch to single byte mode, emit the space,
*     and switch back to Unicode mode. (3 bytes per space)
*/

#include "scsu.h"


namespace SCSU {

/* driver for sample code */
// extern void out(int byte) {
//     printf("--> %02X\n", byte & 0xff);
// }


/* SCSU command byte values */
enum {
    SQ0 = 0x01, /* Quote from window pair 0 */
    SQU = 0x0E, /* Quote a single Unicode character */
    SCU = 0x0F, /* Change to Unicode mode */
    SC0 = 0x10, /* Select window 0 */

    UC0 = 0xE0, /* Select window 0 */
    UQU = 0xF0  /* Quote a single Unicode character */
};


/* constant offsets for the 8 static and 8 dynamic windows */
static const uint16_t offsets[16] = {
    /* initial offsets for the 8 dynamic (sliding) windows */
    0x0080, /* Latin-1 */
    0x00C0, /* Latin Extended A */
    0x0400, /* Cyrillic */
    0x0600, /* Arabic */
    0x0900, /* Devanagari */
    0x3040, /* Hiragana */
    0x30A0, /* Katakana */
    0xFF00, /* Fullwidth ASCII */

    /* offsets for the 8 static windows */
    0x0000, /* ASCII for quoted tags */
    0x0080, /* Latin - 1 Supplement (for access to punctuation) */
    0x0100, /* Latin Extended-A */
    0x0300, /* Combining Diacritical Marks */
    0x2000, /* General Punctuation */
    0x2080, /* Currency Symbols */
    0x2100, /* Letter-like Symbols and Number Forms */
    0x3000  /* CJK Symbols and punctuation */
};


// utf-8 prefixes and masks
constexpr unsigned char UTF_CONT_PREFIX      = 0b1000'0000;
constexpr unsigned char UTF_CONT_PREFIX_MASK = 0b1100'0000;
constexpr unsigned char UTF_CONT_SUFFIX_MASK = 0b0011'1111;
constexpr unsigned char UTF_CONT_SHIFT       = 6;

constexpr unsigned char UTF_1_PREFIX      = 0b0000'0000;
constexpr unsigned char UTF_1_PREFIX_MASK = 0b1000'0000;
constexpr unsigned char UTF_1_SUFFIX_MASK = 0b0111'1111;
constexpr uint32_t      UTF_1_MAX         = (1ULL << 7) - 1;

constexpr unsigned char UTF_2_PREFIX      = 0b1100'0000;
constexpr unsigned char UTF_2_PREFIX_MASK = 0b1110'0000;
constexpr unsigned char UTF_2_SUFFIX_MASK = 0b0001'1111;
constexpr uint32_t      UTF_2_MAX         = (1ULL << (5 + UTF_CONT_SHIFT)) - 1;

constexpr unsigned char UTF_3_PREFIX      = 0b1110'0000;
constexpr unsigned char UTF_3_PREFIX_MASK = 0b1111'0000;
constexpr unsigned char UTF_3_SUFFIX_MASK = 0b0000'1111;
constexpr uint32_t      UTF_3_MAX         = (1ULL << (4 + 2 * UTF_CONT_SHIFT)) - 1;

constexpr unsigned char UTF_4_PREFIX      = 0b1111'0000;
constexpr unsigned char UTF_4_PREFIX_MASK = 0b1111'1000;
constexpr unsigned char UTF_4_SUFFIX_MASK = 0b0000'0111;
constexpr uint32_t      UTF_4_MAX         = 0x10'FFFF;

constexpr uint32_t UTF_REPLACEMENT      = 0xFFFD;
constexpr uint32_t UTF_SURROGATE_BEG    = 0xD800;
constexpr uint32_t UTF_SURROGATE_END    = 0xDFFF;
constexpr uint32_t UTF_SURROGATE_HIGH   = 0xD800;
constexpr uint32_t UTF_SURROGATE_LOW    = 0xDC00;
constexpr uint32_t UTF_SURROGATE_OFFSET = 0x10000;

uint32_t code_point_from_utf8(const unsigned char*& begin, const unsigned char*& end) {
    if (begin >= end) {
        return -1;
    }

    if ((*begin & UTF_1_PREFIX_MASK) == UTF_1_PREFIX) {
        uint32_t code_point = *begin & UTF_1_SUFFIX_MASK;
        begin++;
        return code_point;
    }
    else if ((*begin & UTF_2_PREFIX_MASK) == UTF_2_PREFIX) {
        if (begin + 1 >= end) {
            begin = end;
            return UTF_REPLACEMENT;
        }
        if ((*(begin+1) & UTF_CONT_PREFIX_MASK) != UTF_CONT_PREFIX) {
            begin++;
            return UTF_REPLACEMENT;
        }

        uint32_t code_point = (*begin & UTF_2_SUFFIX_MASK);
        begin++;
        code_point <<= UTF_CONT_SHIFT;
        code_point |= *begin & UTF_CONT_SUFFIX_MASK;
        begin++;

        return code_point;
    }
    else if ((*begin & UTF_3_PREFIX_MASK) == UTF_3_PREFIX) {
        if (begin + 2 >= end) {
            begin = end;
            return UTF_REPLACEMENT;
        }
        if ((*(begin+1) & UTF_CONT_PREFIX_MASK) != UTF_CONT_PREFIX
            || (*(begin+2) & UTF_CONT_PREFIX_MASK) != UTF_CONT_PREFIX)
        {
            begin++;
            return UTF_REPLACEMENT;
        }

        uint32_t code_point = (*begin & UTF_3_SUFFIX_MASK);
        begin++;
        code_point <<= UTF_CONT_SHIFT;
        code_point |= *begin & UTF_CONT_SUFFIX_MASK;
        begin++;
        code_point <<= UTF_CONT_SHIFT;
        code_point |= *begin & UTF_CONT_SUFFIX_MASK;
        begin++;

        // Check for Surrogate code point
        if (code_point >= UTF_SURROGATE_BEG && code_point <= UTF_SURROGATE_END) {
            return UTF_REPLACEMENT;
        }

        return code_point;
    }
    else if ((*begin & UTF_4_PREFIX_MASK) == UTF_4_PREFIX) {
        if (begin + 3 >= end) {
            begin = end;
            return UTF_REPLACEMENT;
        }
        if ((*(begin+1) & UTF_CONT_PREFIX_MASK) != UTF_CONT_PREFIX
            || (*(begin+2) & UTF_CONT_PREFIX_MASK) != UTF_CONT_PREFIX
            || (*(begin+3) & UTF_CONT_PREFIX_MASK) != UTF_CONT_PREFIX)
        {
            begin++;
            return UTF_REPLACEMENT;
        }

        uint32_t code_point = (*begin & UTF_4_SUFFIX_MASK);
        begin++;
        code_point <<= UTF_CONT_SHIFT;
        code_point |= *begin & UTF_CONT_SUFFIX_MASK;
        begin++;
        code_point <<= UTF_CONT_SHIFT;
        code_point |= *begin & UTF_CONT_SUFFIX_MASK;
        begin++;
        code_point <<= UTF_CONT_SHIFT;
        code_point |= *begin & UTF_CONT_SUFFIX_MASK;
        begin++;

        return code_point;
    }
    else {
        begin++;
        return UTF_REPLACEMENT;
    }
}


struct State {
    bool    isUnicodeMode;
    uint8_t window;
};


static /* inline */ char isInWindow(uint32_t offset, uint32_t c) {
    return (char) (offset <= c && c <= (offset + 0x7f));
}


/* get the index of the static/dynamic window that contains c; -1 if none */
static int getWindow(uint32_t c) {
    int i;

    for (i = 0; i < 16; ++i) {
        if (isInWindow(offsets[i], c)) {
            return i;
        }
    }
    return -1;
}


/**
 * Function for minimal SCSU encoding.
 * Calls out(byte) to output SCSU bytes.
 *
 * @param c Input Unicode code point or UTF-16 code unit.
 *        The output will be the same whether the function is called with
 *        a sequence of code points or an equivalent sequence of UTF-16 code
 *        units, or an equivalent mixture of them.
 *        It must be 0<=c<=0x10ffff.
 * @param pState Pointer to SCSUMiniState struct.
 *        Indicates whether the encoder is currently in Unicode mode,
 *        and which is the current dynamic window for single-byte mode.
 *        All fields must initially be set to 0 (single-byte mode, window 0).
 */
void encode(State* state, uint32_t c, unsigned char*& out) { // NOLINT(misc-no-recursion)
    int w; /* result of getWindow(), -1..7 */

    /*
     * Check arguments:
     * - Is c a valid code point? (0<=c<=0x10ffff)
     * - Is pState a valid pointer to a state struct with valid values?
     *   (isUnicodeMode=0..1  and window=0..7)
     */
    if (c > 0x10ffff || (state->window & ~7) != 0) {
        /* illegal argument error */
        return;
    }

    if (c > 0xffff) {
        /* encode a supplementary code point as a surrogate pair */
        c -= UTF_SURROGATE_OFFSET;
        encode(state, UTF_SURROGATE_HIGH + (c >> 10), out);
        encode(state, UTF_SURROGATE_LOW + (c & 0x3ff), out);
        return;
    }

    if (!state->isUnicodeMode) {
        /* single-byte mode */
        if (c < 0x20) {
            /*
             * Encode C0 control code:
             * Check the code point against the bit mask 0010 0110 0000 0001
             * which contains 1-bits at the bit positions corresponding to
             * code points 0D 0A 09 00 (CR LF TAB NUL)
             * which are encoded directly.
             * All other C0 control codes are quoted with SQ0.
             */
            if (((1 << c) & 0x2601) == 0) {
                *out++ = SQ0;
            }
            *out++ = c;
        } else if (c <= 0x7f) {
            /* encode US-ASCII directly */
            *out++ = c;
        } else if (isInWindow(offsets[state->window], c)) {
            /* use the current dynamic window */
            *out++ = 0x80 + (c - offsets[state->window]);
        } else if ((w = getWindow(c)) >= 0) {
            if (w <= 7) {
                /* switch to a dynamic window */
                *out++ = SC0 + w;
                *out++ = 0x80 + (c - offsets[w]);
                state->window = w;
            } else {
                /* quote from a static window */
                *out++ = SQ0 + (w - 8);
                *out++ = c - offsets[w];
            }
        } else if (c == 0xfeff) {
            /* encode the signature character U+FEFF with SQU */
            *out++ = SQU;
            *out++ = 0xfe;
            *out++ = 0xff;
        } else {
            /* switch to Unicode mode */
            *out++ = SCU;
            state->isUnicodeMode = true;
            encode(state, c, out);
        }
    } else {
        /* Unicode mode */
        if (c <= 0x7f) {
            /* US-ASCII: switch to single-byte mode with the previous dynamic window */
            state->isUnicodeMode = false;
            *out++ = UC0 + state->window;
            encode(state, c, out);
        } else if ((w = getWindow(c)) >= 0 && w <= 7) {
            /* switch to single-byte mode with a matching dynamic window */
            *out++ = UC0 + w;
            state->window        = w;
            state->isUnicodeMode = false;
            encode(state, c, out);
        } else {
            if (0xe000 <= c && c <= 0xf2ff) {
                *out++ = (UQU);
            }
            *out++ = c >> 8;
            *out++ = c & 0xFF;
        }
    }
}

void write_utf8(uint32_t code_point, unsigned char*& out) { // NOLINT(misc-no-recursion)
    if (code_point <= UTF_1_MAX) {
        *out++ = UTF_1_PREFIX | code_point;
    }
    else if (code_point <= UTF_2_MAX) {
        *(out    ) = UTF_2_PREFIX    | ((code_point >>     UTF_CONT_SHIFT) & UTF_2_SUFFIX_MASK);
        *(out + 1) = UTF_CONT_PREFIX | ((code_point                      ) & UTF_CONT_SUFFIX_MASK);
        out += 2;
    }
    else if (code_point <= UTF_3_MAX) {
        if (code_point >= UTF_SURROGATE_BEG && code_point <= UTF_SURROGATE_END) {
            write_utf8(UTF_REPLACEMENT, out);
            return;
        }
        *(out    ) = UTF_3_PREFIX    | ((code_point >> 2 * UTF_CONT_SHIFT) & UTF_3_SUFFIX_MASK);
        *(out + 1) = UTF_CONT_PREFIX | ((code_point >>     UTF_CONT_SHIFT) & UTF_CONT_SUFFIX_MASK);
        *(out + 2) = UTF_CONT_PREFIX | ((code_point                      ) & UTF_CONT_SUFFIX_MASK);
        out += 3;
    } else if (code_point <= UTF_4_MAX) {
        *(out    ) = UTF_4_PREFIX    | ((code_point >> 3 * UTF_CONT_SHIFT) & UTF_4_SUFFIX_MASK);
        *(out + 1) = UTF_CONT_PREFIX | ((code_point >> 2 * UTF_CONT_SHIFT) & UTF_CONT_SUFFIX_MASK);
        *(out + 2) = UTF_CONT_PREFIX | ((code_point >>     UTF_CONT_SHIFT) & UTF_CONT_SUFFIX_MASK);
        *(out + 3) = UTF_CONT_PREFIX | ((code_point                      ) & UTF_CONT_SUFFIX_MASK);
        out += 4;
    } else {
        write_utf8(UTF_REPLACEMENT, out);
    }
}


uint_fast32_t compress(const char* in_signed, uint_fast32_t count, char* out_signed) {
    auto in = reinterpret_cast<const unsigned char*>(in_signed);
    auto out = reinterpret_cast<unsigned char*>(out_signed);

    State state {false, 0};

    auto in_end  = in + count;
    auto out_beg = out;

    while (in < in_end) {
        auto code_point = code_point_from_utf8(in, in_end);
        encode(&state, code_point, out);
    }

    return out - out_beg;
}


uint_fast32_t decompress(const char* in_signed, uint_fast32_t count, char* out_signed) {
    auto in = reinterpret_cast<const unsigned char*>(in_signed);
    auto out = reinterpret_cast<unsigned char*>(out_signed);

    bool isUnicodeMode = false;
    auto window = 0;

    auto in_end = in + count;
    auto out_beg = out;

    while (in < in_end) {
        if (!isUnicodeMode) {
            // single-byte mode
            if (*in >= SQ0 && *in < SQ0 + 8) {
                // quote from window
                auto q_window = *in++ - SQ0;
                if (*in < 0x80) {
                    // static window
                    auto code_point = *in++ + offsets[q_window + 8];
                    write_utf8(code_point, out);
                } else {
                    // dynamic window
                    auto code_point = *in++ + offsets[q_window];
                    write_utf8(code_point, out);
                }
            } else if (*in == SQU) {
                // quote unicode from BMP
                in++;
                auto code_point = static_cast<uint32_t>(*in++) << 8;
                code_point |= *in++;
                write_utf8(code_point, out);
            } else if (*in == SCU) {
                // change to unicode mode
                isUnicodeMode = true;
                in++;
            } else if (*in >= SC0 && *in < SC0 + 8) {
                // change dynamic window
                window = *in++ - SC0;
            } else if (*in < 0x80) {
                // static window 0 (offset 0, ASCII)
                write_utf8(*in++, out);
            } else {
                // dynamic window
                auto code_point = *in++ + offsets[window] - 0x80;
                write_utf8(code_point, out);
            }
        } else {
            // Unicode mode
            if (*in == UQU) {
                // quoted code point (because it collides with tag byte)
                in++;
                auto code_point = *in++ << 8;
                code_point |= *in++;
                write_utf8(code_point, out);
            } else if (*in >= UC0 && *in < UC0 + 8) {
                // change to single byte mode and change dynamic window
                isUnicodeMode = false;
                window = *in++ - UC0;
            } else if (*in >= UTF_SURROGATE_HIGH >> 8 && *in < UTF_SURROGATE_LOW >> 8) {
                // Surrogate pair
                auto high = static_cast<uint32_t>(*in++) << 8;
                high |= *in++;

                auto low = static_cast<uint32_t>(*in++) << 8;
                low |= *in++;

                high -= UTF_SURROGATE_HIGH;
                low -= UTF_SURROGATE_LOW;

                auto code_point = (high << 10) | low;
                code_point += UTF_SURROGATE_OFFSET;
                write_utf8(code_point, out);
            } else {
                // code point encoded in two bytes
                auto code_point = *in++ << 8;
                code_point |= *in++;
                write_utf8(code_point, out);
            }
        }
    }
    return out - out_beg;
}

} // namespace SCSU

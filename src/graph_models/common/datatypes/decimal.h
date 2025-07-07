#pragma once

#include <cassert>
#include <cmath>
#include <cstdint>
#include <ostream>
#include <sstream>
#include <string>

#include "third_party/dragonbox/dragonbox.h"

class Decimal {
private:
    uint64_t significand;
    int8_t exponent;
    bool negative;

    constexpr Decimal(uint64_t significand, int8_t exponent, bool negative) :
        significand(significand),
        exponent(exponent),
        negative(negative)
    { }

public:
    static constexpr size_t MAX_DIGIT_PRECISION = 19;
    static constexpr auto EXTERN_BUFFER_SIZE = 10;

    // Initializes a zero
    constexpr Decimal() :
        significand(0),
        exponent(0),
        negative(false)
    { }

    explicit Decimal(int64_t i) :
        significand(i >= 0 ? i : -i),
        exponent(0),
        negative(i < 0)
    {
        normalize();
    }

    explicit Decimal(float f)
    {
        assert(std::isnormal(f)); // assumes not zero, infinity, nan

        auto v = jkj::dragonbox::to_decimal(f);

        this->exponent = v.exponent;
        this->negative = v.is_negative;
        this->significand = v.significand;

        normalize();
    }

    explicit Decimal(double d)
    {
        assert(std::isnormal(d)); // assumes not zero, infinity, nan

        auto v = jkj::dragonbox::to_decimal(d);

        this->exponent = v.exponent;
        this->negative = v.is_negative;
        this->significand = v.significand;

        normalize();
    }

    // may lose precision if more than 18 significant digits are needed
    Decimal(std::string_view sv, bool* error)
    {
        if (sv.empty()) {
            *error = true;
            return;
        }

        *error = false;

        this->negative = sv[0] == '-';

        auto [whole, fractional] = get_parts(sv, error);

        if (*error) {
            return;
        }

        int ws = whole.size();
        int fs = fractional.size();

        int exp = 0;

        uint8_t digits_arr[MAX_DIGIT_PRECISION + 1];
        size_t digits_size = 0;

        if (ws > 0 && fs > 0) {
            exp = -fs;
            digits_size = ws + fs;
            if (digits_size > MAX_DIGIT_PRECISION) {
                exp += digits_size - MAX_DIGIT_PRECISION;
                digits_size = MAX_DIGIT_PRECISION;
            }

            for (size_t i = 0; i < size_t(ws) && i < digits_size; i++) {
                digits_arr[i] = char_to_digit(whole[i]);
            }

            for (size_t i = ws; i < digits_size; i++) {
                digits_arr[i] = char_to_digit(fractional[i - ws]);
            }
        } else if (ws > 0) {
            auto start = whole.find_first_not_of('0'); // leading zeros
            auto last = whole.find_last_not_of('0'); // trailing zeros
            exp = ws - last - 1; // number of zeros
            digits_size = 1 + last - start;
            if (digits_size > MAX_DIGIT_PRECISION) {
                exp += digits_size - MAX_DIGIT_PRECISION;
                digits_size = MAX_DIGIT_PRECISION;
            }
            for (size_t i = 0; i < digits_size; i++) {
                digits_arr[i] = char_to_digit(whole[start + i]);
            }
        } else if (fs > 0) {
            auto first = fractional.find_first_not_of('0');
            exp = -fs;
            fractional = fractional.substr(first);
            fs = fractional.size();
            digits_size = fs;
            if (digits_size > MAX_DIGIT_PRECISION) {
                exp += digits_size - MAX_DIGIT_PRECISION;
                digits_size = MAX_DIGIT_PRECISION;
            }
            for (size_t i = 0; i < digits_size; i++) {
                digits_arr[i] = char_to_digit(fractional[i]);
            }
        }

        if (exp < INT8_MIN || exp > INT8_MAX) {
            *error = true;
            return;
        }

        this->exponent = exp;

        uint64_t mult = 1;
        significand = 0;
        for (size_t i = 0; i < digits_size; i++) {
            significand += mult * digits_arr[digits_size - 1 - i];
            mult *= 10;
        }
        normalize();
    }

    static Decimal from_external(const char* buffer)
    {
        auto bytes = reinterpret_cast<const uint8_t*>(buffer);

        uint64_t significand = 0;
        for (int i = 0, shift = 0; i < 8; ++i, shift += 8) {
            significand |= static_cast<uint64_t>(bytes[i]) << shift;
        }

        auto res_significand = significand;
        auto res_exponent = reinterpret_cast<const int8_t*>(bytes)[8];
        auto res_negative = bytes[9] == 1;

        return Decimal(res_significand, res_exponent, res_negative);
    }

    void serialize_extern(char* buffer) const
    {
        auto bytes = reinterpret_cast<uint8_t*>(buffer);

        for (size_t i = 0, shift = 0; i < 8; ++i, shift += 8) {
            bytes[i] = (significand >> shift) & 0xFF;
        }

        reinterpret_cast<int8_t*>(bytes)[8] = exponent;
        bytes[9] = negative ? 1 : 0;
    }

    static Decimal from_inlined(uint64_t i)
    {
        auto bytes = reinterpret_cast<uint8_t*>(&i);

        uint64_t significand = 0;
        significand += static_cast<uint64_t>(bytes[0]);
        significand += static_cast<uint64_t>(bytes[1]) << 8;
        significand += static_cast<uint64_t>(bytes[2]) << 16;
        significand += static_cast<uint64_t>(bytes[3]) << 24;
        significand += static_cast<uint64_t>(bytes[4]) << 32;
        significand += (static_cast<uint64_t>(bytes[5]) & 0x7F) << 40;

        uint64_t res_significand = significand;
        int8_t res_exponent = (i >> 48) & 0xFF;
        bool res_negative = ((1ULL << 47) & i) != 0;

        return Decimal(res_significand, res_exponent, res_negative);
    }

    constexpr bool can_inline() const
    {
        return (significand & 0xFF'FF'80'00'00'00'00'00) == 0;
    }

    uint64_t serialize_inlined() const
    {
        uint64_t res = static_cast<uint8_t>(exponent);
        res = res << 48;
        if (negative) {
            res |= (1ULL << 47);
        }
        return res | significand;
    }

    bool operator==(const Decimal& rhs) const
    {
        // border case: comparison between +0 and -0
        if (this->significand == 0) {
            return rhs.significand == 0;
        }
        return significand == rhs.significand && exponent == rhs.exponent && negative == rhs.negative;
    }

    bool operator!=(const Decimal& rhs) const
    {
        return !(rhs == *this);
    }

    bool operator<(const Decimal& rhs) const
    {
        if (this->negative != rhs.negative)
            return this->negative;

        // Handle zero cases
        if (significand == 0 && rhs.significand == 0)
            return false;
        if (significand == 0)
            return !negative;
        if (rhs.significand == 0)
            return negative;

        int lhs_exponent = this->exponent;
        auto lhs_significand = this->significand;

        int rhs_exponent = rhs.exponent;
        auto rhs_significand = rhs.significand;

        while (lhs_exponent > rhs_exponent) {
            if (lhs_significand < 100000000000000000) {
                lhs_significand *= 10;
                lhs_exponent -= 1;
            } else {
                rhs_exponent += 1;
                rhs_significand /= 10;
            }
        }

        while (lhs_exponent < rhs_exponent) {
            if (rhs_significand < 100000000000000000) {
                rhs_significand *= 10;
                rhs_exponent -= 1;
            } else {
                lhs_exponent += 1;
                lhs_significand /= 10;
            }
        }

        if (lhs_significand < rhs_significand)
            return !negative;
        if (lhs_significand > rhs_significand)
            return negative;
        return false; // equal
    }

    bool operator>(const Decimal& rhs) const
    {
        return rhs < *this;
    }

    bool operator<=(const Decimal& rhs) const
    {
        return !(rhs < *this);
    }

    bool operator>=(const Decimal& rhs) const
    {
        return !(*this < rhs);
    }

    bool is_zero() const
    {
        return significand == 0;
    }

    Decimal operator-() const
    {
        Decimal dec(*this);
        dec.negative = !dec.negative;
        return dec;
    }

    Decimal abs() const
    {
        Decimal res(*this);
        res.negative = false;
        return res;
    }

    Decimal floor() const
    {
        Decimal res(*this);

        if (exponent >= 0) {
            return res;
        }

        for (int i = 0; i < -exponent; i++) {
            res.significand /= 10;
        }

        int adjustment = res.negative ? 1 : 0;
        res.significand += adjustment;
        res.exponent = 0;

        res.normalize();
        return res;
    }

    Decimal ceil() const
    {
        Decimal res(*this);

        if (exponent >= 0) {
            return res;
        }

        for (int i = 0; i < -exponent; i++) {
            res.significand /= 10;
        }

        int adjustment = res.negative ? 0 : 1;
        res.significand += adjustment;
        res.exponent = 0;

        res.normalize();
        return res;
    }

    Decimal round() const
    {
        Decimal res(*this);

        if (this->exponent >= 0) {
            return res;
        }

        for (int i = 0; i < -this->exponent - 1; i++) {
            res.significand /= 10;
        }

        // first digit after .
        auto first_decimal_digit = res.significand % 10;

        int adjustment = 0;

        if (this->negative) {
            // for negative decimals ROUND(-X.5) = (-X)
            if (first_decimal_digit > 5) {
                adjustment = 1;
            } else if (first_decimal_digit == 5 && this->exponent < -1) {
                // assumes this is normalized
                // ROUND(-1.50001) = (-2)
                adjustment = 1;
            }
        } else {
            if (first_decimal_digit >= 5) {
                adjustment = 1;
            }
        }

        res.significand /= 10;
        res.significand += adjustment;
        res.exponent = 0;

        res.normalize();
        return res;
    }

    friend std::ostream& operator<<(std::ostream& os, const Decimal& dec)
    {
        if (dec.negative)
            os << '-';

        if (dec.exponent >= 0) {
            os << dec.significand;
            for (int i = 0; i < dec.exponent; i++) {
                os << '0';
            }
            os << ".0";
        } else {
            char buf[MAX_DIGIT_PRECISION + 1];

            auto digits_significand = snprintf(
                buf,
                MAX_DIGIT_PRECISION + 1,
                "%llu",
                static_cast<unsigned long long>(dec.significand)
            );

            if (digits_significand <= -dec.exponent) {
                os << "0.";

                auto zeros_after_comma = -dec.exponent - digits_significand;
                for (int i = 0; i < zeros_after_comma; i++) {
                    os << '0';
                }
                for (int i = 0; i < digits_significand; i++) {
                    os << buf[i];
                }
            } else {
                auto digits_before_comma = digits_significand + dec.exponent;

                for (int i = 0; i < digits_before_comma; i++) {
                    os << buf[i];
                }
                os << '.';
                for (int i = digits_before_comma; i < digits_significand; i++) {
                    os << buf[i];
                }
            }
        }

        return os;
    }

    Decimal operator+(const Decimal& other) const
    {
        int lhs_exponent = this->exponent;
        auto lhs_significand = this->significand;

        int rhs_exponent = other.exponent;
        auto rhs_significand = other.significand;

        while (lhs_exponent > rhs_exponent) {
            if (lhs_significand < 100000000000000000) {
                lhs_significand *= 10;
                lhs_exponent -= 1;
            } else {
                rhs_exponent += 1;
                rhs_significand /= 10;
            }
        }

        while (lhs_exponent < rhs_exponent) {
            if (rhs_significand < 100000000000000000) {
                rhs_significand *= 10;
                rhs_exponent -= 1;
            } else {
                lhs_exponent += 1;
                lhs_significand /= 10;
            }
        }

        Decimal res;
        res.exponent = lhs_exponent;
        if (this->negative == other.negative) {
            res.negative = this->negative;
            res.significand = lhs_significand + rhs_significand;

            if (res.significand >= 1000000000000000000) {
                res.exponent += 1;
                res.significand /= 10;
            }
        } else {
            if (lhs_significand >= rhs_significand) {
                res.negative = this->negative;
                res.significand = lhs_significand - rhs_significand;
            } else {
                res.negative = other.negative;
                res.significand = rhs_significand - lhs_significand;
            }
        }

        res.normalize();
        return res;
    }

    Decimal operator-(const Decimal& rhs) const
    {
        return *this + (-rhs);
    }

    Decimal operator*(const Decimal& rhs) const
    {
        Decimal res;

        // TODO: check for possible overflow
        res.significand = this->significand * rhs.significand;
        res.negative = this->negative != rhs.negative;
        res.exponent = this->exponent + rhs.exponent;
        res.normalize();
        return res;
    }

    Decimal operator/(const Decimal& rhs) const
    {
        if (rhs.significand == 0) {
            throw std::runtime_error("Division by zero");
        }

        int lhs_exponent = this->exponent;
        auto lhs_significand = this->significand;

        int rhs_exponent = rhs.exponent;
        auto rhs_significand = rhs.significand;

        while (lhs_significand % rhs_significand != 0 && lhs_significand < 100000000000000000) {
            lhs_significand *= 10;
            lhs_exponent--;
        }

        int result_exponent = lhs_exponent - rhs_exponent;

        if (result_exponent < INT8_MIN || result_exponent > INT8_MAX) {
            throw std::runtime_error("Unrepresentable decimal");
        }

        Decimal res;
        res.significand = lhs_significand / rhs_significand;
        res.exponent = result_exponent;
        res.negative = this->negative != rhs.negative;

        res.normalize();
        return res;
    }

    float to_float() const
    {
        return to_double();
    }

    double to_double() const
    {
        double res = significand;
        res *= std::pow(10, exponent);
        if (negative) {
            res *= -1;
        }
        return res;
    }

    // Converts the Decimal to an int64_t, truncation occurs.
    // If the result does not fit into an int64_t an error is returned.
    int64_t to_int(bool* error) const
    {
        // TODO: set error if result does not fit into an int64_t an error is returned.
        *error = false;
        int64_t res = significand;

        if (exponent >= 0) {
            for (int i = 0; i < exponent; i++) {
                res *= 10;
            }
        } else {
            for (int i = 0; i < -exponent; i++) {
                res /= 10;
            }
        }

        if (negative) {
            res *= -1;
        }
        return res;
    }

    std::string to_string() const
    {
        std::stringstream ss;
        ss << *this;
        return ss.str();
    }

private:
    void normalize()
    {
        if (significand == 0) {
            exponent = 0;
            negative = false;
            return;
        }

        while (significand % 10 == 0) {
            significand /= 10;
            exponent++;
        }
    }

    static inline uint8_t char_to_digit(char c)
    {
        assert(c >= '0' && c <= '9');
        return c - '0';
    }

    static std::pair<std::string_view, std::string_view> get_parts(std::string_view sv, bool* error)
    {
        *error = false;
        // Check for invalid characters
        if (sv.find_first_not_of("+-.0123456789") != std::string::npos) {
            *error = true;
            return {};
        }

        // Check for at least one digit
        if (sv.find_first_of("0123456789") == std::string::npos) {
            *error = true;
            return {};
        }

        // Check for signs
        auto first_sign = sv.find_first_of("+-");
        if (first_sign != std::string::npos) {
            // Make sure sign is at beginning
            if (first_sign != 0) {
                *error = true;
                return {};
            }
            // Make sure we do not have multiple signs
            if (sv.find_last_of("+-") != first_sign) {
                *error = true;
                return {};
            }
        }

        // Remove leading zeros and any sign
        auto start = sv.find_first_not_of("+-0");
        if (start == std::string::npos) {
            // All zeros (e.g. 00000000)
            return {};
        }
        sv = sv.substr(start);

        std::string_view whole; // Whole part (if any), leading zeros removed
        std::string_view fractional; // Fractional part (if any), trailing zeros removed

        auto sep = sv.find('.');
        if (sep == std::string::npos) {
            // No fractional part (eg 12300)
            whole = sv;
        } else if (sep == sv.size() - 1) {
            // Empty fractional part (eg 12300.)
            whole = sv.substr(0, sep);
        } else if (sep == 0) {
            // No whole part (eg .000123000)
            auto end = sv.find_last_not_of('0');
            if (sep == end) {
                // No fractional part (eg .00000)
                return {};
            }
            // Fractional part (eg .00123000)
            // Remove sep and trailing zeroes from fractional part
            fractional = sv.substr(1, end);
        } else {
            // Whole and fractional part (eg 123000.000123000)
            whole = sv.substr(0, sep);

            auto end = sv.find_last_not_of('0');
            if (sep == end) {
                // Empty fractional part (eg 123000.00000)
            } else {
                // Fractional part (eg 123000.000123000)
                fractional = sv.substr(sep + 1, end - sep);
            }
        }
        return { whole, fractional };
    }
};

static_assert(std::is_trivially_copyable<Decimal>::value, "Decimal should be trivially copyable");

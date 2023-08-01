#include "decimal.h"

#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <exception>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>

#include "query/exceptions.h"
#include "graph_models/object_id.h"

using namespace std::literals::string_literals;

unsigned Decimal::MAX_SIGNIFICANT_FIGURES = 30;

/**
 *  @brief Converts a digit from ASCII to it's numeric value.
 *  @param c The digit encoded in ASCII.
 *  @return An uint8_t with the numeric value of the digit.
 */
static uint8_t atoi(char c) {
    assert(c >= '0' && c <= '9');
    return c - '0';
}

/**
 *  @brief Converts the numeric value of a digit to it's ASCII representation.
 *  @param c The numeric value of the digit.
 *  @return A char containing the ASCII representation of the digit.
 */
static char itoa(uint8_t i) {
    assert(i >= 0 && i <= 9);
    return '0' + i;
}

Decimal::Decimal() {
    // 0
}

/**
 *  @brief Creates a Decimal from an int64_t.
 *  @param i The int64_t to convert to Decimal.
 *  @return A Decimal containing the value of the int64_t.
 */
Decimal::Decimal(int64_t i) {
    if (i == 0) {
        return;
    }

    auto str = std::to_string(i);
    std::string_view sv(str);

    if (sv[0] == '-') {
        sign = true;
        sv   = sv.substr(1);
    }

    auto svs  = sv.size();
    auto last = sv.find_last_not_of('0');

    exponent = svs - last - 1;               // number of zeros
    sv       = sv.substr(0, svs - exponent); // remove trailing zeros
    svs      = sv.size();
    digits   = std::vector<uint8_t>(svs);
    for (unsigned i = 0; i < svs; i++) {
        digits[i] = atoi(sv[svs - i - 1]);
    }
    return;
}

/**
 *  @brief Creates a Decimal from the human representation of a Decimal (eg "-123.034")
 *  @param sv The string representing the Decimal.
 *  @return A Decimal with the value of the string representation.
 */
Decimal::Decimal(std::string_view sv, bool* error) {
    *error = false;

    if (sv[0] == '-') {
        sign = true;
    }

    auto [whole, fractional] = Decimal::get_parts(sv, error);

    if (*error) {
        return;
    }

    int ws = whole.size();
    int fs = fractional.size();

    if (ws > 0 && fs > 0) {
        exponent = -fs;
        digits   = std::vector<uint8_t>(ws + fs);
        for (auto i = 0; i < fs; i++) {
            digits[i] = atoi(fractional[fs - i - 1]);
        }
        for (auto i = fs; i < ws + fs; i++) {
            digits[i] = atoi(whole[ws - (i - fs) - 1]);
        }
        return;
    } else if (ws > 0) {
        auto last = whole.find_last_not_of('0');
        exponent  = ws - last - 1;                  // number of zeros
        whole     = whole.substr(0, ws - exponent); // remove trailing zeros
        ws        = whole.size();
        digits    = std::vector<uint8_t>(ws);
        for (auto i = 0; i < ws; i++) {
            digits[i] = atoi(whole[ws - i - 1]);
        }
        return;
    } else if (fs > 0) {
        auto first = fractional.find_first_not_of('0');
        exponent   = -fs;
        fractional = fractional.substr(first);
        fs         = fractional.size();
        digits     = std::vector<uint8_t>(fs);
        for (auto i = 0; i < fs; i++) {
            digits[i] = atoi(fractional[fs - i - 1]);
        }
        return;
    } else {
        // 0
        return;
    }
}

/**
 *  @brief Deserializes a Decimal.
 *  @param vec The vector containing the serialized form of the Decimal.
 *  @return A Decimal with the value of the deserialization.
 */
Decimal::Decimal(std::vector<uint8_t> vec) {
    //[1 bit: sign][7 bits: number bytes for digits][int8_t: exponent, base 10][digits, 2 per byte, little endian]
    uint8_t b = vec[0];
    sign      = vec[0] >> 7;
    exponent  = vec[1];

    digits.reserve((vec.size() - 2) * 2);
    auto vs = vec.size();

    for (unsigned i = 2; i < vs; i++) {
        b = vec[i];
        digits.push_back(b & 0x0F);
        b = b >> 4;
        if (i < vs - 1 || b != 0) // If last digit is zero, ignore it. This happens when the number of digits is uneven.
        {
            digits.push_back(b);
        }
    }
}

/**
 *  @brief Sets this Decimal equal to the deserialized Decimal from external string.
 *         Used as an adapter for now, ideally Decimal(std::vector<uint8_t>) should be used.
 *  @param sv The string_view representing the Decimal from which to deserialize.
 */
Decimal Decimal::from_external(std::string_view sv) {
    // TODO: performance, avoid allocating and copying
    auto bytes = std::vector<uint8_t>(sv.begin(), sv.end());
    return Decimal(bytes);
}

/**
 *  @brief Splits a string_view containing a decimal number into it's whole and fractional parts (possibly empty).
 *  @param sv The string representation of the number to split.
 *  @return A pair of string_views representing the whole and fractional part of the number.
 */
std::pair<std::string_view, std::string_view> Decimal::get_parts(std::string_view sv, bool* error) {
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

    std::string_view whole;      // Whole part (if any), leading zeros removed
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

/**
 *  @brief Trims any leading and trailing zeros in digits, adjusts the exponent as necessary.
 */
void Decimal::trim_zeros() {
    if (this->digits.size() == 0) {
        // Zero
        this->exponent = 0;
        this->sign     = false;
        return;
    }

    size_t left_zeros  = 0;
    size_t right_zeros = 0;

    for (size_t i = 0; i < this->digits.size() && this->digits[i] == 0; i++) {
        left_zeros++;
    }

    for (auto i = static_cast<int_fast64_t>(this->digits.size()) - 1; i >= 0 && this->digits[i] == 0; i--) {
        right_zeros++;
    }

    if (left_zeros == 0 && right_zeros == 0) {
        // No trimming necessary
        return;
    }

    if (left_zeros == this->digits.size()) {
        // All zeros
        this->digits.clear();
        this->exponent = 0;
        this->sign     = false;
        return;
    }

    auto new_size = this->digits.size() - left_zeros - right_zeros;
    std::memmove(this->digits.data(), this->digits.data() + left_zeros, new_size);
    this->digits.resize(new_size);
    this->exponent += left_zeros;
}

/**
 *  @brief Converts the Decimal a float, loss of precision can occur.
 *  @return A float best representing the Decimal.
 */
float Decimal::to_float() const {
    return this->to_double();
}

/**
 *  @brief Converts the Decimal a double, loss of precision can occur.
 *  @return A double best representing the Decimal.
 */
double Decimal::to_double() const {
    double res  = 0;
    double mult = std::pow(10, exponent);
    for (unsigned i = 0; i < digits.size(); i++) {
        res += digits[i] * mult;
        mult *= 10;
    }
    if (sign) {
        res *= -1;
    }
    return res;
}

/**
 *  @brief Converts the Decimal to an int64_t, truncation occurs. If the result does not fit into an int64_t an error is returned.
 *  @return An int64_t representing the whole part of the Decimal.
 */
int64_t Decimal::to_int(bool* error) const {
    *error = false;
    int64_t res  = 0;
    // Result does not fit into int64_t
    if (exponent > 18) {
        *error = true;
        return 0;
    }

    int64_t mult = std::pow(10, exponent);
    mult = std::max(static_cast<int64_t>(1), mult);

    int64_t start = std::max(static_cast<int64_t>(0), -static_cast<int64_t>(exponent));

    for (unsigned i = start; i < digits.size(); i++) {
        int64_t next_sum = digits[i] * mult;

        // Check for overflow
        if (next_sum / mult != digits[i]) {
            *error = true;
            return 0;
        }

        // Check for overflow
        if (INT64_MAX - next_sum < res) {
            *error = true;
            return 0;
        }
        res += next_sum;

        mult *= 10;
    }
    if (sign) {
        // Check for overflow
        if (res < INT64_MAX * -1) {
            *error = true;
            return 0;
        }
        res *= -1;
    }
    return res;
}

/**
 *  @brief Converts the Decimal into a human readable representation.
 *         Should be used for things like printing.
 *  @return A string containing the human readable representation.
 */
std::string Decimal::to_string() const {
    std::stringstream ss;
    if (digits.size() == 0) {
        return "0.0"s;
    }

    if (sign) {
        ss << "-";
    }

    if (exponent >= 0) {
        // Append zero or more zeros to the end.
        // 12345 -> 12345 | 12345000

        for (int i = digits.size() - 1; i >= 0; i--) {
            ss << itoa(digits[i]);
        }
        ss << std::string(exponent, '0');
        ss << ".0";
    } else if (-exponent < static_cast<int>(digits.size())) {
        // The separator is somewhere between the digits.
        // 12345 -> 1.2345 | 123.45 , etc

        // Digits before the separator
        for (int i = digits.size() - 1; i >= (-exponent); i--) {
            ss << itoa(digits[i]);
        }

        ss << '.';

        // Digits after the separator.
        for (int i = -exponent - 1; i >= 0; i--) {
            ss << itoa(digits[i]);
        }
    } else {
        // All the digits are after the separator.
        // Zero or more zeros have to be inserted between the separator and the digits.
        // 12345 -> 0.12345 | 0.00012345 , etc

        ss << "0.";

        // Fill with necessary zeros.
        auto zeros = (-exponent) - digits.size();
        if (zeros > 0) {
            ss << std::string(zeros, '0');
        }

        // Append all the digits.
        for (int i = digits.size() - 1; i >= 0; i--) {
            ss << itoa(digits[i]);
        }
    }

    return ss.str();
}

/**
 *  @brief Serializes Decimal into bytes.
 *  @return A vector containing the bytes of the of the serialized Decimal format.
 */
std::vector<uint8_t> Decimal::to_bytes() const {
    //[1 bit: sign][7 bits: number bytes for digits][int8_t: exponent, base 10][digits, 2 per byte, little endian]
    auto bytes_for_digits = (digits.size() + 1) / 2;
    if (bytes_for_digits > 0b0111'1111) {
        throw QueryExecutionException("Decimal is too big");
    }
    std::vector<uint8_t> data(1 + 1 + bytes_for_digits, 0);
    uint8_t              b = bytes_for_digits;
    b |= (static_cast<uint8_t>(sign) << 7);
    data[0] = b;
    data[1] = exponent;

    auto ds = digits.size();
    for (unsigned i = 0; i < bytes_for_digits; i++) {
        b = digits[2 * i];
        if (2 * i + 1 < ds) // Handle "missing" last digits when number of digits is uneven.
        {
            b |= digits[2 * i + 1] << 4;
        }
        data[i + 2] = b; // +1 for [sign number of bytes] and +1 for [exponent]
    }
    return data;
}

/**
 *  @brief Serializes Decimal into bytes. Used as an adapter for now, ideally to_bytes should be used.
 *  @return A string containing the bytes of the serialized Decimal format.
 */
std::string Decimal::to_external() const {
    auto bytes = this->to_bytes();
    // TODO: performance, avoid allocating and copying
    return std::string(bytes.begin(), bytes.end());
}

/**
 *  @brief Serializes Decimal into decimal inlined. If id does not fit ObjectId::NULL_ID is returned.
 *  @return A uint64_t containing the decimal if it fits, ObjectId::NULL_ID otherwise.
 */
uint64_t Decimal::to_internal() const {
    auto sep = std::abs(std::min(int8_t(0), exponent));
    // Check if the separator fits into 4 bits
    if (sep > 0xF) {
        return ObjectId::NULL_ID;
    }
    uint64_t num = 0;
    if (exponent > 19) {
        // Overflow
        return ObjectId::NULL_ID;
    }

    uint64_t exp = std::max(int8_t(0), exponent);
    uint64_t mult = std::pow(uint64_t(10), exp);
    for (uint64_t i = 0; i < digits.size(); i++) {
        auto next = mult * digits[i];
        if (next / mult != digits[i]) {
            // Overflow;
            return ObjectId::NULL_ID;
        }

        auto prev_num = num;
        num += next;
        if (num - next != prev_num) {
            // Overflow;
            return ObjectId::NULL_ID;
        }

        auto prev_mult = mult;
        mult *= 10;
        if (i + 1 < digits.size() && mult / 10 != prev_mult) {
            // Overflow;
            return ObjectId::NULL_ID;
        }
    }

    if (num > MAX_INLINE_NUM) {
        // Does not fit into inline decimal
        return ObjectId::NULL_ID;
    }
    uint64_t ret = 0;

    ret |= static_cast<uint8_t>(sign); ret <<= 51;
    ret |= num; ret <<= 4;
    ret |= sep;
    ret |= ObjectId::MASK_DECIMAL_INLINED;

    return ret;
}

Decimal Decimal::ceil() const {
    if (this->exponent >= 0) {
        return *this;
    }

    if (static_cast<size_t>(-this->exponent) >= this->digits.size()) {
        if (this->sign) {
            return Decimal();
        } else {
            return Decimal(1);
        }
    }

    auto decimals = -this->exponent;
    Decimal d;
    d.digits = std::vector<uint8_t>(this->digits.begin() + decimals, this->digits.end());

    if (this->sign) {
        d.sign = true;
    } else {
        d = d + Decimal(1);
    }

    return d;
}

Decimal Decimal::floor() const {
    if (this->exponent >= 0) {
        return *this;
    }

    if (static_cast<size_t>(-this->exponent) >= this->digits.size()) {
        if (this->sign) {
            return Decimal(-1);
        } else {
            return Decimal();
        }
    }

    auto decimals = -this->exponent;
    Decimal d;
    d.digits   = std::vector<uint8_t>(this->digits.begin() + decimals, this->digits.end());

    if (this->sign) {
        d.sign = true;
        d = d - Decimal(1);
    }
    return d;
}

Decimal Decimal::round() const {
    if (this->exponent >= 0) {
        return *this;
    }

    size_t decimals = -this->exponent;

    if (decimals > this->digits.size()) {
        return Decimal();
    }

    Decimal d;
    d.digits   = std::vector<uint8_t>(this->digits.begin() + decimals, this->digits.end());
    d.exponent = 0;
    d.sign     = this->sign;

    uint8_t decider = 0;
    if (this->digits.size() >= decimals) {
        decider = this->digits[decimals-1];
    }

    if (this->sign && (decider > 5 || (decider == 5 && decimals >= 2))) {
        d = d - Decimal(1);
    } else if (!this->sign && decider >= 5) {
        d = d + Decimal(1);
    }

    return d;
}

Decimal Decimal::operator-() const {
    Decimal dec(*this);
    dec.sign = !dec.sign;
    return dec;
}

bool Decimal::operator==(const Decimal& rhs) const {
    if (this->digits.size() == 0) {
        return rhs.digits.size() == 0;
    }
    return this->sign == rhs.sign && this->exponent == rhs.exponent && this->digits == rhs.digits;
}

Decimal Decimal::operator*(const Decimal& rhs) const {
    // Size (digit count)
    const auto lhs_s = this->digits.size();
    const auto rhs_s = rhs.digits.size();

    Decimal res;

    for (size_t r = 0; r < rhs_s; r++) {
        Decimal sum;
        sum.digits    = std::vector<uint8_t>(lhs_s + 1, 0); // +1 for potential carry
        sum.exponent  = this->exponent + rhs.exponent + r;
        uint8_t carry = 0;
        for (size_t l = 0; l < lhs_s; l++) {
            uint8_t digit = this->digits[l] * rhs.digits[r] + carry;
            carry         = digit / 10;
            digit         = digit % 10;
            sum.digits[l] = digit;
        }
        sum.digits[lhs_s] = carry;
        sum.trim_zeros();
        // TODO: implement and use +=
        res = res + sum;
    }

    if (this->sign != rhs.sign) {
        res.sign = true;
    }
    return res;
}

/**
 *  @brief Divides this Decimal by a divisor, the divisor must not be 0.
 *  @param rhs the divisor of the division, must not be 0.
 *  @return The result of the division.
 */
Decimal Decimal::operator/(const Decimal& rhs) const {
    if (rhs == Decimal(0)) {
        throw LogicException("Division by zero");
    }
    if (*this == Decimal(0)) {
        return *this;
    }
    // This function implements division using the long division algorithm.
    auto res_exponent = 0;
    auto res_sign     = false;

    Decimal dividend(*this);
    // Ensure dividend exponent is >= 0.
    // If the exponent is negative, set to zero and adjust result exponent.
    if (dividend.exponent < 0) {
        res_exponent += dividend.exponent;
        dividend.exponent = 0;
    }
    // Make dividend positive, adjusting result sign.
    res_sign ^= dividend.sign;
    dividend.sign = false;

    Decimal divisor(rhs);
    // Ensure divisor exponent is >= 0.
    // If the exponent is negative, set to zero and adjust result exponent.
    if (divisor.exponent < 0) {
        res_exponent -= divisor.exponent;
        divisor.exponent = 0;
    }
    // Make divisor positive, adjusting result sign.
    res_sign ^= divisor.sign;
    divisor.sign = false;

    // From this point onwards both dividend and divisor should be positive and have exponent >= 0.
    assert(dividend.exponent >= 0);
    assert(divisor.exponent >= 0);
    assert(!dividend.sign);
    assert(!divisor.sign);

    // Vector of digits, big endian, to facilitate calculations.
    std::vector<uint8_t> res_digits;

    // Calculate effective divisor and dividend digit count (digits + trailing zeros)
    auto divisor_s = divisor.digits.size() + divisor.exponent;
    auto dividend_s = dividend.digits.size() + dividend.exponent;

    // Ensure dividend has more or equal effective digits than divisor.
    // If necessary increase dividend exponent and adjust result exponent.
    if (dividend_s < divisor_s) {
        auto diff = divisor_s - dividend_s;
        dividend.exponent += diff;
        res_exponent -= diff;
    }

    // Ensure dividend is greater than divisor.
    // If necessary increase dividend exponent by one and adjust result exponent.
    if (dividend < divisor) {
        dividend.exponent++;
        res_exponent--;
    }

    // From this point on, dividend >= divisor
    assert(dividend >= divisor);

    // Update effective digit count.
    // May have changed due to adjusting exponents.
    divisor_s = divisor.digits.size() + divisor.exponent;
    dividend_s = dividend.digits.size() + dividend.exponent;

    // Offset is used to align dividend and divisor digits.
    auto offset    = dividend_s - divisor_s;

    // Take the first n leading most significant digits of the dividend,
    // where n is the number of effective digits of the divisor.
    auto remainder = dividend.sub_digits(offset, dividend_s);

    // Ensure divisor goes into remainder at least once,
    // If not take one more digit from the dividend.
    if (remainder < divisor) {
        offset--;
        remainder = dividend.sub_digits(offset, dividend_s);
    }

    // Do the initial division.
    auto q_r      = Decimal::quotient_remainder(remainder, divisor);
    auto quotient = q_r.first;
    remainder     = q_r.second;
    res_digits.push_back(quotient);

    // Do the remaining divisions consuming digits from the dividend.
    // Stop if we have reached MAX_SIGNIFICANT_FIGURES + 1.
    while (offset > 0 && res_digits.size() < MAX_SIGNIFICANT_FIGURES + 1) {
        offset--;
        uint8_t next_digit = 0;
        if (offset >= static_cast<size_t>(dividend.exponent)) {
            next_digit = dividend.digits[offset - dividend.exponent];
        }
        remainder.insert_digit(next_digit);

        auto q_r  = Decimal::quotient_remainder(remainder, divisor);
        quotient  = q_r.first;
        remainder = q_r.second;
        res_digits.push_back(quotient);
    }

    // If we still have a remainder and not yet reached MAX_SIGNIFICANT_FIGURES + 1
    // append 0s and continue dividing.
    // We divide until MAX_SIGNIFICANT_FIGURES + 1 instead of MAX_SIGNIFICANT_FIGURES
    // so we can round afterwards.
    while (remainder > Decimal(0) && res_digits.size() < MAX_SIGNIFICANT_FIGURES + 1) {
        remainder.insert_digit(0);
        res_exponent--;

        auto q_r  = Decimal::quotient_remainder(remainder, divisor);
        quotient  = q_r.first;
        remainder = q_r.second;
        res_digits.push_back(quotient);
    }

    // Initialize result decimal, transform digits into little endian.
    Decimal res;
    res.digits = std::vector<uint8_t>(res_digits.rbegin(), res_digits.rend());
    res.exponent = res_exponent;
    res.sign = res_sign;


    // Round if necessary.
    if (res.digits.size() == MAX_SIGNIFICANT_FIGURES + 1) {
        if (res.digits[0] >= 5) {
            Decimal d(1);
            d.exponent = res.exponent + 1;
            res        = res + d;
        }
        // TODO: avoid unnecessary allocation
        res.digits = std::vector<uint8_t>(res.digits.begin() + 1, res.digits.end());
        res.exponent++;
    }

    res.trim_zeros();
    return res;
}

bool Decimal::operator<(const Decimal& rhs) const {
    if (this->digits.size() == 0 and rhs.digits.size() == 0) {
        return false;
    }

    if (this->digits.size() == 0) {
        return !rhs.sign;
    }

    if (rhs.digits.size() == 0) {
        return this->sign;
    }

    if (this->sign && !rhs.sign) {
        return true;
    }

    if (!this->sign && rhs.sign) {
        return false;
    }

    auto lhs_msp = static_cast<int>(this->digits.size()) + this->exponent; // Most significant position
    auto rhs_msp = static_cast<int>(rhs.digits.size()) + rhs.exponent;     // Most significant position

    if (!this->sign && !rhs.sign) {
        if (lhs_msp != rhs_msp) {
            return lhs_msp < rhs_msp;
        }
        auto lhs_s = this->digits.size();
        auto rhs_s = rhs.digits.size();
        for (unsigned i = 0; i < std::min(lhs_s, rhs_s); i++) {
            auto ld = this->digits[lhs_s - i - 1];
            auto rd = rhs.digits[rhs_s - i - 1];
            if (ld != rd) {
                return ld < rd;
            }
        }
        return lhs_s < rhs_s;
    }

    if (this->sign && rhs.sign) {
        if (lhs_msp != rhs_msp) {
            return lhs_msp > rhs_msp;
        }
        return this->digits > rhs.digits;
    }

    throw LogicException("This should never be reached");
}

Decimal Decimal::operator+(const Decimal& rhs) const {
    Decimal res;

    if (this->sign && rhs.sign) {
        res.sign = true;
    } else if (this->sign) {
        Decimal dec(*this);
        dec.sign = false;
        return rhs - dec;
    } else if (rhs.sign) {
        Decimal dec(rhs);
        dec.sign = false;
        return *this - dec;
    }
    // Both decimals can be treated as positive at this point.

    // least significant position
    int lsp = std::min(this->exponent, rhs.exponent);
    // most significant position
    const int lhs_s = this->digits.size();
    const int rhs_s = rhs.digits.size();
    const int msp   = std::max(this->exponent + lhs_s, rhs.exponent + rhs_s);

    int res_size; // number of digits in res, +1 for carry
    if (lsp * msp <= 0) {
        res_size = std::abs(msp) + std::abs(lsp) + 1;
    } else {
        res_size = std::abs(msp - lsp) + 2;
    }

    res.exponent = lsp;
    res.digits   = std::vector<uint8_t>(res_size, 0);
    auto& res_d  = res.digits;

    int lhs_off = lsp - this->exponent; // offset used to align lhs
    int rhs_off = lsp - rhs.exponent;   // offset used to align rhs

    uint8_t carry = 0;
    for (int p = 0; p < res_size; p++) {
        uint8_t d = 0; // digit

        if (p + lhs_off >= 0 && p + lhs_off < static_cast<int>(this->digits.size())) {
            d += this->digits[p + lhs_off];
        }

        if (p + rhs_off >= 0 && p + rhs_off < static_cast<int>(rhs.digits.size())) {
            d += rhs.digits[p + rhs_off];
        }

        d += carry;
        if (d > 9) {
            carry = 1;
            d -= 10;
        } else {
            carry = 0;
        }

        res_d[p] = d;
    }
    res.trim_zeros();
    return res;
}

Decimal Decimal::operator-(const Decimal& rhs_r) const {
    Decimal res;

    const Decimal* lhs = this;
    const Decimal* rhs = &rhs_r;


    if (lhs->sign && rhs->sign) {
        res.sign = true;
        // We want abs(lhs) > abs(rhs)
        // To avoid creating copies and changing signs we turn the check into
        // if lhs > rhs and swap instead of (-lhs) < (-rhs).
        if (*lhs > *rhs) {
            std::swap(lhs, rhs);
            res.sign = !res.sign;
        }
    } else if (lhs->sign) {
        Decimal dec(*rhs);
        dec.sign = true;
        return *lhs + dec;
    } else if (rhs->sign) {
        Decimal dec(*rhs);
        dec.sign = false;
        return *lhs + dec;
    } else {
        // Both positive
        if (*lhs < *rhs) {
            std::swap(lhs, rhs);
            res.sign = !res.sign;
        }
    }

    // At this point both decimals can be treated as positive and lhs >= rhs.

    // least significant position
    int lsp = std::min(lhs->exponent, rhs->exponent);
    // most significant position
    const int lhs_s = lhs->digits.size();
    const int rhs_s = rhs->digits.size();
    const int msp   = std::max(lhs->exponent + lhs_s, rhs->exponent + rhs_s);

    int res_size; // number of digits in res
    if (lsp * msp <= 0) {
        res_size = std::abs(msp) + std::abs(lsp);
    } else {
        res_size = std::abs(msp - lsp) + 1;
    }

    res.exponent = lsp;
    res.digits   = std::vector<uint8_t>(res_size, 0);

    int lhs_off = lsp - lhs->exponent; // offset used to align lhs
    int rhs_off = lsp - rhs->exponent; // offset used to align rhs

    uint8_t carry = 0;
    for (int p = 0; p < res_size; p++) {
        int8_t d = 0; // digit

        if (p + lhs_off >= 0 && p + lhs_off < static_cast<int>(lhs->digits.size())) {
            d += lhs->digits[p + lhs_off];
        }

        if (p + rhs_off >= 0 && p + rhs_off < static_cast<int>(rhs->digits.size())) {
            d -= rhs->digits[p + rhs_off];
        }

        d -= carry;
        if (d < 0) {
            carry = 1;
            d += 10;
        } else {
            carry = 0;
        }

        res.digits[p] = d;
    }
    res.trim_zeros();
    return res;
}

/**
 *  @brief Calculates the quotient and remainder of dividend / divisor. Used as a helper function to implement operator/.
 *  @param dividend The dividend of the division, has to have exponent >= 0.
 *  @param divisor The divisor of the division, has to have exponent >= 0.
 *  @return the quotient and remainder of the division as a std::pair.
 */
std::pair<uint8_t, Decimal> Decimal::quotient_remainder(const Decimal& dividend, const Decimal& divisor) {
    assert(dividend.exponent >= 0);
    assert(divisor.exponent >= 0);

    if (dividend.digits.size() == 0) {
        return {0, Decimal()};
    }

    if (divisor.digits.size() == 0) {
        throw LogicException("division by zero");
    }

    if (divisor > dividend) {
        return {0, dividend};
    }

    if (divisor == dividend) {
        return {1, Decimal()};
    }

    auto dividend_s = dividend.digits.size() + dividend.exponent;
    auto divisor_s = divisor.digits.size() + divisor.exponent;

    if (dividend_s == divisor_s) {
        assert(dividend.digits.size() >= 1);
        auto q = dividend.digits[dividend.digits.size() - 1] / divisor.digits[divisor.digits.size() - 1];

        Decimal quotient(q);
        auto    remainder = dividend - quotient * divisor;
        if (remainder < Decimal(0)) {
            q--;
            quotient  = Decimal(q);
            remainder = dividend - quotient * divisor;
        }
        remainder.trim_zeros();
        return { q, remainder };
    } else if (dividend_s == divisor_s + 1) {
        auto s = dividend.digits.size();
        auto two_digits = dividend.digits[s - 1] * 10;
        if (s >= 2) {
            two_digits += dividend.digits[s - 2];
        }
        auto q          = two_digits / divisor.digits[divisor.digits.size() - 1];

        Decimal quotient(q);
        auto    remainder = dividend - quotient * divisor;

        while (remainder < Decimal(0)) {
            q--;
            quotient  = Decimal(q);
            remainder = dividend - quotient * divisor;
        }
        remainder.trim_zeros();
        return { q, remainder };
    } else {
        throw NotSupportedException("");
    }
}

/**
 *  @brief Constructs a Decimal containing a sub-slice of effective digits of the Decimal.
 *         Effective digits include trailing zeros represented by the exponent.
 *  @param start the starting index of the slice to return, inclusive.
 *  @param end the ending index of the slice to return, exclusive.
 *  @return a Decimal representing the sub-slice of effective digits.
 */
Decimal Decimal::sub_digits(size_t start, size_t end) const {
    assert(this->exponent >= 0);
    assert(start <= this->digits.size() + this->exponent);
    assert(end >= static_cast<size_t>(this->exponent) && end <= this->digits.size() + this->exponent);
    if (start >= end) {
        return Decimal();
    }
    Decimal sub;
    if (start <= static_cast<size_t>(this->exponent)) {
        auto diff = this->exponent - start;
        sub.exponent += diff;
        start = 0;
    } else {
        start -= this->exponent;
    }
    end -= this->exponent;
    sub.digits = std::vector<uint8_t>(this->digits.begin() + start, this->digits.begin() + end);
    return sub;
}

/**
 *  @brief Inserts a digit behind the least siginificant digit.
 *         Inserting 0 only adjusts the exponent and does not perform an actual insertion.
 *  @param d the digit to insert.
 *  @return a new Decimal representing the result of inserting the digit.
 */
void Decimal::insert_digit(uint8_t d) {
    assert(this->exponent >= 0);
    // Optimization to avoid inserting 0 and later trimming zeros.
    if (d == 0) {
        this->exponent++;
        return;
    }

    for (auto i = 0; i < this->exponent; i++) {
        this->digits.insert(this->digits.begin(), 0);
    }
    this->exponent = 0;
    this->digits.insert(this->digits.begin(), d);
}

std::ostream& operator<<(std::ostream& os, const Decimal& dec) {
    os << dec.to_string();
    return os;
}

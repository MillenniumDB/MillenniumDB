#pragma once

#include <cstdint>
#include <ostream>
#include <string>
#include <vector>

class Decimal {
public:
    static unsigned MAX_SIGNIFICANT_FIGURES;
    static constexpr uint64_t MAX_INLINE_NUM = 0x0007'FFFF'FFFF'FFFFULL;

    std::vector<uint8_t> digits;
    int8_t               exponent = 0;
    bool                 sign     = false;

    Decimal();
    Decimal(int64_t);
    Decimal(std::string_view, bool* error);
    Decimal(std::vector<uint8_t>);

    static Decimal from_external(std::string_view);

    // assumes std::fpclassify is FP_NORMAL
    static Decimal from_float(float);

    // assumes std::fpclassify is FP_NORMAL
    static Decimal from_double(double);

    void trim_zeros();

    bool operator==(const Decimal&) const;
    bool operator<(const Decimal&) const;

    inline bool operator!=(const Decimal& rhs) const {
        return !(*this == rhs);
    }

    inline bool operator>(const Decimal& rhs) const {
        return !(*this < rhs) && *this != rhs;
    }

    inline bool operator>=(const Decimal& rhs) const {
        return *this > rhs || *this == rhs;
    }

    inline bool operator<=(const Decimal& rhs) const {
        return *this < rhs || *this == rhs;
    }

    inline Decimal operator+() const {
        return *this;
    };

    Decimal operator-() const;

    Decimal operator+(const Decimal&) const;
    Decimal operator-(const Decimal&) const;
    Decimal operator*(const Decimal&) const;
    Decimal operator/(const Decimal&) const;
    Decimal operator%(const Decimal&) const;

    Decimal ceil() const;
    Decimal floor() const;
    Decimal round() const;
    Decimal abs() const;
    Decimal log(const Decimal&) const;
    Decimal log10() const;
    Decimal ln() const;
    Decimal exp() const;
    Decimal pow(const Decimal&) const;
    Decimal sqrt() const;

    Decimal sin() const;
    Decimal cos() const;
    Decimal tan() const;
    Decimal cot() const;
    Decimal sinh() const;
    Decimal cosh() const;
    Decimal tanh() const;
    Decimal asin() const;
    Decimal acos() const;
    Decimal atan() const;

    float                to_float() const;
    double               to_double() const;
    int64_t              to_int(bool* error) const;
    std::string          to_string() const;
    std::vector<uint8_t> to_bytes() const;
    std::string          to_external() const;
    uint64_t             to_internal() const;

    // Only public for testing during operator/ development.
    // Maybe make private at some point?
    static std::pair<uint8_t, Decimal> quotient_remainder(const Decimal& lhs, const Decimal& rhs);

private:
    Decimal sub_digits(size_t start, size_t end) const;
    void    insert_digit(uint8_t);

    static std::pair<std::string_view, std::string_view> get_parts(std::string_view, bool* error);
};

std::ostream& operator<<(std::ostream& os, const Decimal& dec);

#include "graph_models/common/datatypes/decimal.h"

#include <cassert>
#include <cmath>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

using std::cout;
using std::pair;
using std::string;
using std::vector;

typedef int TestFunction();

int absolute_value_tests() {
    vector<std::tuple<string, string>> tests {
        std::make_tuple("0", "0"),
        std::make_tuple("1", "1"),
        std::make_tuple("-1", "1"),
        std::make_tuple("20", "20"),
        std::make_tuple("-20", "20"),
        std::make_tuple("0.23", "0.23"),
        std::make_tuple("-0.678", "0.678"),
        std::make_tuple("-1.456", "1.456"),
        std::make_tuple("20.854", "20.854"),
        std::make_tuple("-0.0006", "0.0006"),
    };

    cout << "absolute_value_tests\n";
    for (auto& [in, want] : tests) {
        bool error;
        auto in_d = Decimal(in, &error);
        assert(!error);
        auto want_d = Decimal(want, &error);
        assert(!error);

        auto got = in_d.abs();
        if (got != want_d) {
            cout << "abs(" << in << ") got " << got << ", want " << want << "\n";
            return 1;
        }
    }
    return 0;
}

int modulo_tests() {
    Decimal::MAX_SIGNIFICANT_FIGURES = 100;

    vector<std::tuple<string, string, string>> tests {
        std::make_tuple("8", "4", "0"),
        std::make_tuple("8", "3", "2"),
        std::make_tuple("10", "6", "4"),
        std::make_tuple("100", "25", "0"),
        std::make_tuple("888", "77", "41"),
        std::make_tuple("13123242", "99", "0"),
        std::make_tuple("1000000", "3", "1"),
        std::make_tuple("254262", "46500", "21762"),
        std::make_tuple("0.7125", "0.3", "0.1125"),
        std::make_tuple("0.0011", "0.0003", "0.0002"),
        std::make_tuple("1", "0.3333333", "0.0000001"),
        std::make_tuple("0.9", "0.7", "0.2"),
        std::make_tuple("-8", "3", "-2"),
        std::make_tuple("8", "-3", "2"),
        std::make_tuple("-8", "-3", "-2"),
        std::make_tuple("0", "5", "0"),
        std::make_tuple("100", "1", "0"),
        std::make_tuple("999999999999999999999999999999", "333333333333333333333333333333", "0"),
        std::make_tuple("999999999999999999999999999998", "333333333333333333333333333333", "333333333333333333333333333332")
    };

    cout << "modulo_tests\n";
    for (auto& [dividend, divisor, want] : tests) {
        bool error;
        Decimal dividend_d(dividend, &error);
        assert(!error);
        Decimal divisor_d(divisor, &error);
        assert(!error);
        Decimal want_d(want, &error);
        assert(!error);

        auto got = dividend_d % divisor_d;

        if (got != want_d) {
            cout << "(" << dividend_d << " % " << divisor_d << ") got " << got << ", want " << want_d << "\n";
            return 1;
        }
    }
    return 0;
}

int log_tests() {
    Decimal::MAX_SIGNIFICANT_FIGURES = 100;

    vector<std::tuple<string, string, string>> tests{
        {"8", "2", "3"},        // log2(8) = 3
        {"27", "3", "3"},       // log3(27) = 3
        {"1000", "10", "3"},    // log10(1000) = 3
        {"16", "4", "2"},       // log4(16) = 2
        {"1", "5", "0"},         // log5(1) = 0
        {"60", "5", "2.543959"}
    };

    cout << "log_tests\n";
    for (const auto& [arg, base, want] : tests) {
        bool error;
        Decimal arg_d(arg, &error);
        Decimal base_d(base, &error);
        Decimal want_d(want, &error);
        Decimal got = arg_d.log(base_d);
        if (got != want_d) {
            cout << "log(" << arg_d << ", " << base_d << ") got " << got << ", want " << want_d << "\n";
            return 1;
        }
    }
    return 0;
}


int log10_tests() {
    Decimal::MAX_SIGNIFICANT_FIGURES = 100;

    vector<std::tuple<string, string>> tests{
        {"1000", "3"},          // log10(1000) = 3
        {"0.1", "-1"},          // log10(0.1) = -1
        {"1", "0"},             // log10(1) = 0
        {"60", "1.778151"}
    };

    cout << "log10_tests\n";
    for (const auto& [arg, want] : tests) {
        bool error;
        Decimal arg_d(arg, &error);
        assert(!error);
        Decimal want_d(want, &error);
        assert(!error);
        Decimal got = arg_d.log10();

        if (got != want_d) {
            cout << "log10(" << arg_d << ") got " << got << ", want " << want_d << "\n";
            return 1;
        }
    }
    return 0;
}

int ln_tests() {
    Decimal::MAX_SIGNIFICANT_FIGURES = 100;

    vector<std::tuple<string, string>> tests{
        {"1", "0"},             // ln(1) = 0
        {"2.718281828", "1"},   // ln(e) = 1
        {"7.389", "1.999992"},          // ln(e^2) = 2
        {"10", "2.302585"}
    };

    cout << "ln_tests\n";
    for (const auto& [arg, want] : tests) {
        bool error;
        Decimal arg_d(arg, &error);
        assert(!error);
        Decimal want_d(want, &error);
        assert(!error);
        Decimal got = arg_d.ln();

        if (got != want_d) {
            cout << "ln(" << arg_d << ") got " << got << ", want " << want_d << "\n";
            return 1;
        }
    }
    return 0;
}

int exp_tests() {
    Decimal::MAX_SIGNIFICANT_FIGURES = 100;

    vector<std::tuple<string, string>> tests{
        {"0", "1"},             // e^0 = 1
        {"1", "2.718282"},   // e^1 = e
        {"2", "7.389056"},      // e^2 = ~7.389
        {"0.5", "1.648721"}
    };

    cout << "exp_tests\n";
    for (const auto& [arg, want] : tests) {
        bool error;
        Decimal arg_d(arg, &error);
        assert(!error);
        Decimal want_d(want, &error);
        assert(!error);

        Decimal got = arg_d.exp();
        if (got != want_d) {
            cout << "exp(" << arg_d << ") got " << got << ", want " << want_d << "\n";
            return 1;
        }
    }
    return 0;
}

int pow_tests() {
    Decimal::MAX_SIGNIFICANT_FIGURES = 100;

    vector<std::tuple<string, string, string>> tests{
        {"2", "3", "8"},        // 2^3 = 8
        {"10", "2", "100"},     // 10^2 = 100
        {"4", "0.5", "2"},      // 4^0.5 = 2
        {"1", "100", "1"},      // 1^100 = 1
        {"9", "0.5", "3"},       // 9^0.5 = 3
        {"1.6", "5", "10.48576"}
    };

    cout << "pow_tests\n";
    for (const auto& [base, exponent, want] : tests) {
        bool error;
        Decimal base_d(base, &error);
        assert(!error);
        Decimal exponent_d(exponent, &error);
        assert(!error);
        Decimal want_d(want, &error);
        assert(!error);

        Decimal got = base_d.pow(exponent_d);
        if (got != want_d) {
            cout << "pow(" << base_d << ", " << exponent_d << ") got " << got << ", want " << want_d << "\n";
            return 1;
        }
    }
    return 0;
}

int sqrt_tests() {
    Decimal::MAX_SIGNIFICANT_FIGURES = 100;

    vector<std::tuple<string, string>> tests{
        {"4", "2"},             // sqrt(4) = 2
        {"9", "3"},             // sqrt(9) = 3
        {"0.25", "0.5"},        // sqrt(0.25) = 0.5
        {"1", "1"},             // sqrt(1) = 1
        {"0", "0"},              // sqrt(0) = 0
        {"1.6", "1.264911"}
    };

    cout << "sqrt_tests\n";
    for (const auto& [arg, want] : tests) {
        bool error;
        Decimal arg_d(arg, &error);
        assert(!error);
        Decimal want_d(want, &error);
        assert(!error);

        Decimal got = arg_d.sqrt();
        if (got != want_d) {
            cout << "sqrt(" << arg_d << ") got " << got << ", want " << want_d << "\n";
            return 1;
        }
    }
    return 0;
}


int sin_tests() {
    Decimal::MAX_SIGNIFICANT_FIGURES = 100;

    vector<std::tuple<string, string>> tests{
        {"0", "0"},                     // sin(0) = 0
        {"1.57079632679", "1"},         // sin(π/2) = 1
        {"3.14159265359", "0"},         // sin(π) = 0
        {"-1.57079632679", "-1"},       // sin(-π/2) = -1
        {"6.28318530718", "0"},          // sin(2π) = 0
        {"10", "-0.544021"}
    };

    cout << "sin_tests\n";
    for (auto& [arg, want] : tests) {
        bool error;
        Decimal arg_d(arg, &error);
        assert(!error);
        Decimal want_d(want, &error);
        assert(!error);

        auto got = arg_d.sin();
        if (got != want_d) {
            cout << "sin(" << arg_d << ") got " << got << ", want " << want_d << "\n";
            return 1;
        }
    }
    return 0;
}

int cos_tests() {
    Decimal::MAX_SIGNIFICANT_FIGURES = 100;

    vector<std::tuple<string, string>> tests{
        {"0", "1"},                     // cos(0) = 1
        {"1.57079632679", "0"},         // cos(π/2) = 0
        {"3.14159265359", "-1"},        // cos(π) = -1
        {"-1.57079632679", "0"},        // cos(-π/2) = 0
        {"6.28318530718", "1"},          // cos(2π) = 1
        {"10", "-0.839072"}
    };

    cout << "cos_tests\n";
    for (auto& [arg, want] : tests) {
        bool error;
        Decimal arg_d(arg, &error);
        assert(!error);
        Decimal want_d(want, &error);
        assert(!error);

        auto got = arg_d.cos();
        if (got != want_d) {
            cout << "cos(" << arg_d << ") got " << got << ", want " << want_d << "\n";
            return 1;
        }
    }
    return 0;
}

int tan_tests() {
    Decimal::MAX_SIGNIFICANT_FIGURES = 100;

    vector<std::tuple<string, string>> tests{
        {"0", "0"},                     // tan(0) = 0
        {"0.78539816339", "1"},         // tan(π/4) = 1
        {"-0.78539816339", "-1"},       // tan(-π/4) = -1
        {"3.14159265359", "0"},         // tan(π) = 0
        {"-3.14159265359", "0"},         // tan(-π) = 0
        {"10", "0.648361"}
    };

    cout << "tan_tests\n";
    for (auto& [arg, want] : tests) {
        bool error;
        Decimal arg_d(arg, &error);
        assert(!error);
        Decimal want_d(want, &error);
        assert(!error);

        auto got = arg_d.tan();
        if (got != want_d) {
            cout << "tan(" << arg_d << ") got " << got << ", want " << want_d << "\n";
            return 1;
        }
    }
    return 0;
}

int cot_tests() {
    Decimal::MAX_SIGNIFICANT_FIGURES = 100;

    vector<std::tuple<string, string>> tests{
        {"0.78539816339", "1"},         // cot(π/4) = 1
        {"-0.78539816339", "-1"},       // cot(-π/4) = -1
        {"10", "1.542351"}
    };

    cout << "cot_tests\n";
    for (auto& [arg, want] : tests) {
        bool error;
        Decimal arg_d(arg, &error);
        assert(!error);
        Decimal want_d(want, &error);
        assert(!error);

        auto got = arg_d.cot();
        if (got != want_d) {
            cout << "cot(" << arg_d << ") got " << got << ", want " << want_d << "\n";
            return 1;
        }
    }
    return 0;
}

int sinh_tests() {
    Decimal::MAX_SIGNIFICANT_FIGURES = 100;

    vector<std::tuple<string, string>> tests{
        {"0", "0"},                     // sinh(0) = 0
        {"1", "1.175201"},         // sinh(1) ≈ 1.1752
        {"-1", "-1.175201"},       // sinh(-1) ≈ -1.1752
        {"2", "3.626860"}          // sinh(2) ≈ 3.62686
    };

    cout << "sinh_tests\n";
    for (auto& [arg, want] : tests) {
        bool error;
        Decimal arg_d(arg, &error);
        assert(!error);
        Decimal want_d(want, &error);
        assert(!error);

        auto got = arg_d.sinh();
        if (got != want_d) {
            cout << "sinh(" << arg_d << ") got " << got << ", want " << want_d << "\n";
            return 1;
        }
    }
    return 0;
}

int cosh_tests() {
    Decimal::MAX_SIGNIFICANT_FIGURES = 100;

    vector<std::tuple<string, string>> tests{
        {"0", "1"},                     // cosh(0) = 1
        {"1", "1.543081"},         // cosh(1) ≈ 1.54308
        {"-1", "1.543081"},        // cosh(-1) ≈ 1.54308
        {"2", "3.762196"}          // cosh(2) ≈ 3.76219
    };

    cout << "cosh_tests\n";
    for (auto& [arg, want] : tests) {
        bool error;
        Decimal arg_d(arg, &error);
        assert(!error);
        Decimal want_d(want, &error);
        assert(!error);

        auto got = arg_d.cosh();
        if (got != want_d) {
            cout << "cosh(" << arg_d << ") got " << got << ", want " << want_d << "\n";
            return 1;
        }
    }
    return 0;
}

int tanh_tests() {
    Decimal::MAX_SIGNIFICANT_FIGURES = 100;

    vector<std::tuple<string, string>> tests{
        {"0", "0"},                     // tanh(0) = 0
        {"1", "0.761594"},         // tanh(1) ≈ 0.7616
        {"-1", "-0.761594"},       // tanh(-1) ≈ -0.7616
        {"2", "0.964028"},         // tanh(2) ≈ 0.9640
        {"-2", "-0.964028"}        // tanh(-2) ≈ -0.9640
    };

    cout << "tanh_tests\n";
    for (auto& [arg, want] : tests) {
        bool error;
        Decimal arg_d(arg, &error);
        assert(!error);
        Decimal want_d(want, &error);
        assert(!error);

        auto got = arg_d.tanh();
        if (got != want_d) {
            cout << "tanh(" << arg_d << ") got " << got << ", want " << want_d << "\n";
            return 1;
        }
    }
    return 0;
}

int asin_tests() {
    Decimal::MAX_SIGNIFICANT_FIGURES = 100;

    vector<std::tuple<string, string>> tests{
        {"0", "0"},                     // asin(0) = 0
        {"0.5", "0.523599"},       // asin(0.5) ≈ π/6 ≈ 0.5236
        {"1", "1.570796"},         // asin(1) = π/2
        {"-0.5", "-0.523599"},     // asin(-0.5) ≈ -π/6
        {"-1", "-1.570796"}        // asin(-1) = -π/2
    };

    cout << "asin_tests\n";
    for (auto& [arg, want] : tests) {
        bool error;
        Decimal arg_d(arg, &error);
        assert(!error);
        Decimal want_d(want, &error);
        assert(!error);

        auto got = arg_d.asin();
        if (got != want_d) {
            cout << "asin(" << arg_d << ") got " << got << ", want " << want_d << "\n";
            return 1;
        }
    }
    return 0;
}

int acos_tests() {
    Decimal::MAX_SIGNIFICANT_FIGURES = 100;

    vector<std::tuple<string, string>> tests{
        {"1", "0"},                     // acos(1) = 0
        {"0.5", "1.047198"},        // acos(0.5) ≈ π/3 ≈ 1.0472
        {"0", "1.570796"},         // acos(0) = π/2
        {"-0.5", "2.094395"},      // acos(-0.5) ≈ 2π/3 ≈ 2.0944
        {"-1", "3.141593"}         // acos(-1) = π
    };

    cout << "acos_tests\n";
    for (auto& [arg, want] : tests) {
        bool error;
        Decimal arg_d(arg, &error);
        assert(!error);
        Decimal want_d(want, &error);
        assert(!error);

        auto got = arg_d.acos();
        if (got != want_d) {
            cout << "acos(" << arg_d << ") got " << got << ", want " << want_d << "\n";
            return 1;
        }
    }
    return 0;
}

int atan_tests() {
    Decimal::MAX_SIGNIFICANT_FIGURES = 100;

    vector<std::tuple<string, string>> tests{
        {"0", "0"},                     // atan(0) = 0
        {"1", "0.785398"},         // atan(1) = π/4 ≈ 0.7854
        {"-1", "-0.785398"},       // atan(-1) ≈ -π/4 ≈ -0.7854
        {"10", "1.471128"},         // atan(10) ≈ 1.4711
        {"-10", "-1.471128"}        // atan(-10) ≈ -1.4711
    };

    cout << "atan_tests\n";
    for (auto& [arg, want] : tests) {
        bool error;
        Decimal arg_d(arg, &error);
        assert(!error);
        Decimal want_d(want, &error);
        assert(!error);

        auto got = arg_d.atan();
        if (got != want_d) {
            cout << "atan(" << arg_d << ") got " << got << ", want " << want_d << "\n";
            return 1;
        }
    }
    return 0;
}

int main() {
    cout.precision(20);

    vector<TestFunction*> tests;

    tests.push_back(&absolute_value_tests);
    tests.push_back(&modulo_tests);
    tests.push_back(&log10_tests);
    tests.push_back(&log_tests);
    tests.push_back(&ln_tests);
    tests.push_back(&exp_tests);
    tests.push_back(&pow_tests);
    tests.push_back(&sqrt_tests);
    tests.push_back(&sin_tests);
    tests.push_back(&cos_tests);
    tests.push_back(&tan_tests);
    tests.push_back(&cot_tests);
    tests.push_back(&sinh_tests);
    tests.push_back(&cosh_tests);
    tests.push_back(&tanh_tests);
    tests.push_back(&asin_tests);
    tests.push_back(&acos_tests);
    tests.push_back(&atan_tests);

    for (auto& test_func : tests) {
        if (test_func()) {
            return 1;
        }
    }
    return 0;
}
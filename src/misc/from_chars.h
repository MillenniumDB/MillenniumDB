#include <charconv>
#include <type_traits>

namespace SPARQL {

template<typename T>
inline std::from_chars_result from_chars(const char* first, const char* last, T& value, int base = 10)
{
    // Skip leading '+' if it exists and there's characters after it
    if (first != last && *first == '+') {
        ++first;
    }

    if constexpr (std::is_floating_point_v<T>) {
        // Floating point from_chars does not take a base
        return std::from_chars(first, last, value);
    } else {
        // Integer versions take the base
        return std::from_chars(first, last, value, base);
    }
}

} // namespace SPARQL
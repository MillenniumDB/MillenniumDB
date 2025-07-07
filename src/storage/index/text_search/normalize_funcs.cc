#include "normalize_funcs.h"

#include "misc/transliterator.h"

#include <cassert>

namespace TextSearch {

std::string noop_normalize(const std::string& token)
{
    return token;
}

std::string nfkd_casefold_normalize(const std::string& token)
{
    return Transliterator::get_instance()->nfkd_casefold(token);
}
} // namespace TextSearch

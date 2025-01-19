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
    const auto* transliterator = Transliterator::get_instance();
    return transliterator->nfkd_casefold(token);
}
} // namespace TextSearch

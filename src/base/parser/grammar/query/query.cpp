#include "query_def.h"
#include "query_config.h"

namespace query { namespace parser {
    BOOST_SPIRIT_INSTANTIATE(query_type, iterator_type, context_type);
}}

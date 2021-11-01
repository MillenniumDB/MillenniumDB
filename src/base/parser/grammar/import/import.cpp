#include "import_def.h"
#include "import_config.h"

namespace import { namespace parser {
    BOOST_SPIRIT_INSTANTIATE(import_type, iterator_type, context_type);
}}

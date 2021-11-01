#include "manual_plan_def.h"
#include "manual_plan_config.h"

namespace manual_plan { namespace parser {
    BOOST_SPIRIT_INSTANTIATE(manual_plan_type, iterator_type, context_type);
}}

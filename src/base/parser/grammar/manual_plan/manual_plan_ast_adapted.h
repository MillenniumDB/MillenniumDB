#ifndef BASE__MANUAL_PLAN_AST_ADAPTED_H_
#define BASE__MANUAL_PLAN_QUERY_AST_ADAPTED_H_

#include <boost/fusion/include/adapt_struct.hpp>

#include "base/parser/grammar/manual_plan/manual_plan_ast.h"

BOOST_FUSION_ADAPT_STRUCT(manual_plan::ast::ManualRoot,
    select, relations, where, group_by, order_by, limit
)

BOOST_FUSION_ADAPT_STRUCT(manual_plan::ast::NodeLabel,
    var_or_id, label
)

BOOST_FUSION_ADAPT_STRUCT(manual_plan::ast::ObjectProperty,
    var_or_id, key, value
)

BOOST_FUSION_ADAPT_STRUCT(manual_plan::ast::TypedConnection,
    type, from, to, edge
)

#endif // BASE__QUERY_AST_ADAPTED_H_

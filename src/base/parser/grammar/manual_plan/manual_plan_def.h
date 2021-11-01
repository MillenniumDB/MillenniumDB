#ifndef BASE__MANUAL_PLAN_DEF_H_
#define BASE__MANUAL_PLAN_DEF_H_

#include <boost/spirit/home/x3.hpp>

#include "manual_plan_ast.h"
#include "manual_plan_ast_adapted.h"
#include "manual_plan.h"
// #include "base/parser/grammar/common/common_def.h"
#include "base/parser/grammar/query/query_def.h"

namespace manual_plan {
    namespace parser {
        namespace x3 = boost::spirit::x3;
        // using namespace query::parser;
        using namespace common::parser;

        using x3::uint32;

        // Declare rules
        x3::rule<class manual_root, ast::ManualRoot>
            manual_root = "manual_root";
        x3::rule<class select_item, ast::NodeLabel>
            node_label = "node_label";
        x3::rule<class select_item, ast::ObjectProperty>
            object_property = "object_property";
        x3::rule<class select_item, ast::TypedConnection>
            typed_connection = "typed_connection";

        auto const node_label_def =
            lexeme["NodeLabel"] >> "(" >> (var | node_name) >> "," >> label >> ")";

        auto const object_property_def =
            lexeme["Property"] >> "(" >> (var | node_name) >> "," >> key >> "," >> (value | node_name) >> ")";

        auto const typed_connection_def =
            lexeme["TypedConnection"]
            >> "(" >> (var | node_name)
            >> "," >> (var | node_name)
            >> "," >> (var | node_name)
            >> "," >> var
            >> ")";

        auto const manual_root_def =
            query::parser::select_statement
            >> ((node_label | object_property | typed_connection) % (','))
            >> -(query::parser::where_statement)
            >> -(query::parser::group_by_statement)
            >> -(query::parser::order_by_statement)
            >> -(query::parser::limit_statement);

        BOOST_SPIRIT_DEFINE(
            manual_root,
            node_label,
            object_property,
            typed_connection
        )
    }

    parser::manual_plan_type query() {
        return parser::manual_root;
    }
}

#endif // BASE__MANUAL_PLAN_DEF_H_

#ifndef BASE__MANUAL_PLAN_AST_H_
#define BASE__MANUAL_PLAN_AST_H_

#include <string>

#include <boost/variant.hpp>
#include <boost/optional.hpp>
#include <boost/fusion/include/io.hpp>
#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>

#include "base/parser/grammar/query/query_ast.h"

namespace manual_plan { namespace ast {
    namespace x3 = boost::spirit::x3;
    using namespace common::ast;

    struct NodeLabel {
        std::string var_or_id;
        std::string label;
    };

    struct ObjectProperty {
        std::string var_or_id;
        std::string key;
        ast::Value value;
    };

    // struct Connection {
    //     std::string from;
    //     std::string to;
    //     std::string edge;
    // };

    // struct ConnectionType {
    //     std::string type;
    //     std::string edge;
    // };

    struct TypedConnection {
        std::string from;
        std::string to;
        std::string type;
        std::string edge;
    };

    using Relation = boost::variant<NodeLabel, ObjectProperty, TypedConnection>;

    struct ManualRoot {
        query::ast::SelectStatement                                  select;
        std::vector<Relation>                                        relations;
        boost::optional<query::ast::FormulaDisjunction>              where;
        boost::optional<std::vector<query::ast::OrderedSelectItem>>  group_by;
        boost::optional<std::vector<query::ast::OrderedSelectItem>>  order_by;
        boost::optional<uint_fast32_t>                               limit;
    };
}}

#endif // BASE__MANUAL_PLAN_AST_H_

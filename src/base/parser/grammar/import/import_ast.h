#ifndef BASE__IMPORT_AST_H_
#define BASE__IMPORT_AST_H_

#include <iostream>
#include <string>

#include <boost/variant.hpp>
#include <boost/optional.hpp>
#include <boost/fusion/include/io.hpp>
#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>

#include "base/parser/grammar/common/common_ast.h"

namespace import { namespace ast {

    namespace x3 = boost::spirit::x3;
    using namespace common::ast;

    struct Node {
        std::string name;
        std::vector<std::string> labels;
        std::vector<Property> properties;

        bool anonymous() const {
            return name[0] == '_';
        }
    };


    struct Edge {
        EdgeDirection direction;

        boost::variant<std::string, bool, int64_t, float> lhs_id;
        boost::variant<std::string, bool, int64_t, float> rhs_id;

        std::vector<std::string> labels;
        std::vector<Property> properties;

        bool left_anonymous() const {
            return lhs_id.type() == typeid(std::string)
                && boost::get<std::string>(lhs_id)[0] == '_';
        }

        bool right_anonymous() const {
            return rhs_id.type() == typeid(std::string)
                && boost::get<std::string>(rhs_id)[0] == '_';
        }
    };


    struct ImplicitEdge {
        EdgeDirection direction;

        // nesting_level = 1: left is the implicit node
        // nesting_level = 2: left is the edge
        // nesting_level = 3: left is the edge of the edge
        // and so on
        std::size_t nesting;
        boost::variant<std::string, bool, int64_t, float> rhs_id;

        std::vector<std::string> labels;
        std::vector<Property> properties;

        std::size_t nesting_level() const {
            return nesting;
        }

        bool right_anonymous() const {
            return rhs_id.type() == typeid(std::string)
                && boost::get<std::string>(rhs_id)[0] == '_';
        }
    };

    using ImportLine = boost::variant<Node, Edge, ImplicitEdge>;
}}

#endif // BASE__IMPORT_AST_H_

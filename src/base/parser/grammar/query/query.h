#ifndef BASE__QUERY_H_
#define BASE__QUERY_H_

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/home/x3.hpp>

#include "query_ast.h"

namespace query {

    namespace parser {
        namespace x3 = boost::spirit::x3;
        using query_type = x3::rule<class root, ast::Root>;
        BOOST_SPIRIT_DECLARE(query_type)
    }

    parser::query_type query();
}

#endif // BASE__QUERY_H_

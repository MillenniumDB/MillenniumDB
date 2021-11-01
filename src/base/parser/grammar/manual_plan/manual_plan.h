#ifndef BASE__MANUAL_PLAN_H_
#define BASE__MANUAL_PLAN_H_

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/home/x3.hpp>

#include "manual_plan_ast.h"

namespace manual_plan {

    namespace parser {
        namespace x3 = boost::spirit::x3;
        using manual_plan_type = x3::rule<class manual_root, ast::ManualRoot>;
        BOOST_SPIRIT_DECLARE(manual_plan_type)
    }

    parser::manual_plan_type manual_plan();
}

#endif // BASE__MANUAL_PLAN_H_

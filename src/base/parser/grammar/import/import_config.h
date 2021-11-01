#ifndef BASE__IMPORT_CONFIG_H_
#define BASE__IMPORT_CONFIG_H_

#include <boost/spirit/home/x3.hpp>

namespace import { namespace parser {
    namespace x3 = boost::spirit::x3;

    using iterator_type = std::string::const_iterator;
    using context_type = x3::phrase_parse_context<x3::space_type>::type;
}}

#endif // BASE__IMPORT_CONFIG_H_

#ifndef BASE__COMMON_DEF_H_
#define BASE__COMMON_DEF_H_

#include <string>
#include <boost/spirit/home/x3.hpp>

#include "base/parser/grammar/common/common_ast.h"
#include "base/parser/grammar/common/common_ast_adapted.h"

namespace common {
    namespace parser {
        namespace x3 = boost::spirit::x3;

        using x3::attr;
        using x3::char_;
        using x3::eol;
        using x3::eoi;
        using x3::int64;
        using x3::lexeme;
        using x3::lit;
        using x3::no_case;
        using x3::space;

        x3::real_parser<float, x3::strict_real_policies<float>> const float_ = { };

        auto const line_skipper = "//" >> *(char_ - eol) >> (eol | eoi);
        auto const skipper = space | line_skipper;

        // Rules
        x3::rule<class var, std::string>
            var = "var";
        x3::rule<class key, std::string>
            key = "key";
        x3::rule<class label, std::string>
            label = "label";
        x3::rule<class value, ast::Value>
            value = "value";
        x3::rule<class property, ast::Property>
            property = "property";
        x3::rule<class node_name, std::string>
            node_name = "node_name";
        x3::rule<class node_id_str, std::string>
            node_id_str = "node_id_str";

        // Grammar
        auto const boolean =
            lexeme[no_case["true"]]  >> attr(true) |
            lexeme[no_case["false"]] >> attr(false);

        auto const escaped =
            '\\' >> (
                "n"  >> attr('\n') |
                "t"  >> attr('\t') |
                "b"  >> attr('\b') |
                "f"  >> attr('\f') |
                "r"  >> attr('\r') |
                "/"  >> attr('/')  |
                "\\" >> attr('\\') |
                "\"" >> attr('\"')
            );

        auto const string =
            lexeme['"' >> *(escaped | ~char_('"')) >> '"'];

        // string_quoted is different from common::parser::string, it mantains the quotation marks ("")
        auto const string_quoted =
            lexeme[char_('"') >> *(escaped | ~char_('"')) >> char_('"')];

        auto const node_name_def =
            lexeme[char_("A-Za-z") >> *char_("A-Za-z0-9_")];

        auto const anon_or_connection =
            lexeme[char_("_") >> char_("AaCc") >> char_("1-9") >> *char_("0-9")];

        auto const connection =
            lexeme[char_("_") >> char_("Cc") >> char_("1-9") >> *char_("0-9")];

        auto const node_id_str_def =
            string_quoted | node_name | var | anon_or_connection | x3::eps;

        auto const node_id =
            boolean | float_ | int64 | node_id_str;

        auto const var_def =
            lexeme[char_('?') >> char_("A-Za-z") >> *char_("A-Za-z0-9_")];

        auto const key_def =
            lexeme[char_("A-Za-z") >> *char_("A-Za-z0-9_")];

        auto const label_def =
            lexeme[':' >> char_("A-Za-z") >> *char_("A-Za-z0-9_")];

        auto const value_def =
            string | float_ | int64 | boolean;

        auto const property_def =
            key >> ':' >> value;

        BOOST_SPIRIT_DEFINE(
            var,
            key,
            label,
            value,
            property,
            node_name,
            node_id_str
        )
    }
}

#endif // BASE__COMMON_DEF_H_

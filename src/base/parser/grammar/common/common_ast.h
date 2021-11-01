#ifndef BASE__COMMON_AST_H_
#define BASE__COMMON_AST_H_

#include <string>

#include <boost/variant.hpp>
#include <boost/optional.hpp>
#include <boost/fusion/include/io.hpp>
#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>

namespace common { namespace ast {

    namespace x3 = boost::spirit::x3;
    using Value = boost::variant<std::string, int64_t, float, bool>;

    struct Property {
        std::string key;
        Value value;
    };

    enum class EdgeDirection {
        right,
        left
    };
}} // namespace common::ast

#endif // BASE__COMMON_AST_H_

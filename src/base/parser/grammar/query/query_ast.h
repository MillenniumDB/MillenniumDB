#ifndef BASE__QUERY_AST_H_
#define BASE__QUERY_AST_H_

#include <string>

#include <boost/variant.hpp>
#include <boost/optional.hpp>
#include <boost/fusion/include/io.hpp>
#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>

#include "base/parser/grammar/common/common_ast.h"

namespace query { namespace ast {
    namespace x3 = boost::spirit::x3;
    using namespace common::ast;

    enum class Order {
        Ascending,
        Descending
    };

    struct SelectItem {
        std::string var;
        boost::optional<std::string> key;
    };

    struct OrderedSelectItem {
        SelectItem item;
        Order order;
    };

    enum class Comparator {
        EQ, // ==
        NE, // !=
        GT, // >
        LT, // <
        GE, // >=
        LE, // <=
    };

    enum class PropertyPathSuffix {
        NONE,
        ZERO_OR_MORE, // *
        ONE_OR_MORE,  // +
        ZERO_OR_ONE   // ?
    };

    struct PropertyPathBoundSuffix {
        uint64_t min;
        uint64_t max;
    };

    struct PropertyPathAtom;

    struct PropertyPathSequence {
        std::vector<PropertyPathAtom> atoms;
    };

    struct PropertyPathAlternatives {
        std::vector<PropertyPathSequence> alternatives;
    };

    struct PropertyPathAtom {
        bool inverse = false;
        boost::variant<PropertyPathSuffix, PropertyPathBoundSuffix> suffix;

        boost::variant<
            std::string,
            boost::recursive_wrapper<PropertyPathAlternatives>
        > atom;
    };

    struct PropertyPath {
        std::string var; // may be a variable or an empty string
        EdgeDirection direction;
        PropertyPathAlternatives path_alternatives;
    };

    struct Edge {
        std::string id; // Can a variable, empty or a connection (e.g. "_c123")
        std::vector<std::string> types;
        std::vector<Property> properties;
        EdgeDirection direction;
    };

    struct Node {
        boost::variant<std::string, bool, int64_t, float> id;
        std::vector<std::string> labels;
        std::vector<Property> properties;
    };

    struct LinearPatternStep {
        boost::variant<Edge, PropertyPath> path;
        Node node;
    };

    struct LinearPattern {
        Node root;
        std::vector<LinearPatternStep> path;
    };

    struct GraphPattern {
        std::vector<LinearPattern> pattern;
        std::vector<boost::recursive_wrapper<GraphPattern>> optionals;
    };

    struct Statement {
        SelectItem lhs;
        Comparator comparator;
        boost::variant<SelectItem, ast::Value> rhs;
    };

    struct FormulaDisjunction;

    struct AtomicFormula {
        bool negation = false;
        boost::variant <Statement, boost::recursive_wrapper<FormulaDisjunction>> content;
    };

    // ANDs
    struct FormulaConjunction {
        std::vector<AtomicFormula> formulas;
    };

    // ORs
    struct FormulaDisjunction {
        std::vector<FormulaConjunction> formula_conjunctions;
    };

    struct SelectStatement {
        bool distinct;
        std::vector<SelectItem> selection;
    };

    struct Root {
        bool                                             explain;
        SelectStatement                                  select;
        GraphPattern                                     graph_pattern;
        boost::optional<FormulaDisjunction>              where;
        boost::optional<std::vector<OrderedSelectItem>>  group_by;
        boost::optional<std::vector<OrderedSelectItem>>  order_by;
        boost::optional<uint64_t>                        limit;
    };
}}

#endif // BASE__QUERY_AST_H_

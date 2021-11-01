#ifndef BASE__GRAMMAR_H_
#define BASE__GRAMMAR_H_

#include <boost/spirit/home/x3.hpp>

#include "query_ast.h"
#include "query_ast_adapted.h"
#include "query.h"
#include "base/parser/grammar/common/common_def.h"

namespace query {
    namespace parser {
        namespace x3 = boost::spirit::x3;
        using namespace common::parser;

        using x3::uint64;

        // Declare rules
        x3::rule<class root, ast::Root>
            root = "root";

        x3::rule<class select_item, ast::SelectItem>
            select_item = "select_item";
        x3::rule<class select_items, std::vector<ast::SelectItem>>
            select_items = "select_items";
        x3::rule<class select_statement, ast::SelectStatement>
            select_statement = "select_statement";

        x3::rule<class graph_pattern, ast::GraphPattern>
            graph_pattern = "graph_pattern";
        x3::rule<class optional_pattern, ast::GraphPattern>
            optional_pattern = "optional_pattern";
        x3::rule<class linear_pattern, ast::LinearPattern>
            linear_pattern = "linear_pattern";

        x3::rule<class node, ast::Node>
            node = "node";
        x3::rule<class edge, ast::Edge>
            edge = "edge";

        // PROPERTY PATHS
        x3::rule<class property_path, ast::PropertyPath>
            property_path = "property_path";
        x3::rule<class property_path_atom, ast::PropertyPathAtom>
            property_path_atom = "property_path_atom";
        x3::rule<class property_path_alternatives, ast::PropertyPathAlternatives>
            property_path_alternatives = "property_path_alternatives";
        x3::rule<class property_path_sequence, ast::PropertyPathSequence>
            property_path_sequence = "property_path_sequence";
        x3::rule<class property_path_bound_suffix, ast::PropertyPathBoundSuffix>
            property_path_bound_suffix = "property_path_bound_suffix";

        // WHERE CONDITIONS
        x3::rule<class statement, ast::Statement>
            statement = "statement";
        x3::rule<class atomic_formula, ast::AtomicFormula>
            atomic_formula = "atomic_formula";
        x3::rule<class formula_conjunction, ast::FormulaConjunction>
            formula_conjunction = "formula_conjunction";
        x3::rule<class formula_disjunction, ast::FormulaDisjunction>
            formula_disjunction = "formula_disjunction";

        x3::rule<class order, ast::Order>
            order = "order";
        x3::rule<class ordered_select_item, ast::OrderedSelectItem>
            ordered_select_item = "ordered_select_item";
        x3::rule<class ordered_select_items, std::vector<ast::OrderedSelectItem>>
            ordered_select_items = "ordered_select_items";


        auto const comparator =
            lit("==") >> attr(ast::Comparator::EQ) |
            lit("<=") >> attr(ast::Comparator::LE) |
            lit(">=") >> attr(ast::Comparator::GE) |
            lit("!=") >> attr(ast::Comparator::NE) |
            lit('<')  >> attr(ast::Comparator::LT) |
            lit('>')  >> attr(ast::Comparator::GT);

        auto const node_inside =
            node_id >> *label >> -("{" >> -(property % ',') >> "}");

        auto const type =
            lexeme[no_case[":TYPE"]] >> '(' >> (var | node_name) >> ')';

        auto const connection_id =
            var | connection | x3::eps;

        auto const edge_inside =
            connection_id >> *(type | node_name) >> -("{" >> -(property % ',') >> "}");

        auto const node_def =
            '(' >> node_inside >> ")";

        auto const edge_def =
            (-("-[" >> edge_inside >> ']') >> "->" >> attr(ast::EdgeDirection::right)) |
            ("<-" >> -('[' >> edge_inside >> "]-") >> attr(ast::EdgeDirection::left));

        auto const property_path_alternatives_def =
            property_path_sequence % "|";

        auto const property_path_sequence_def =
            property_path_atom % "/";

        auto const property_path_def =
            "=["  >> -var >> property_path_alternatives >> "]=>" >> attr(ast::EdgeDirection::right) |
            "<=[" >> -var >> property_path_alternatives >> "]="  >> attr(ast::EdgeDirection::left);

        auto const property_path_bound_suffix_def =
            "{" >> uint64 >> "," >> uint64 >> "}";

        auto const property_path_suffix =
            property_path_bound_suffix                              |
            lit("*") >> attr(ast::PropertyPathSuffix::ZERO_OR_MORE) |
            lit('+') >> attr(ast::PropertyPathSuffix::ONE_OR_MORE)  |
            lit('?') >> attr(ast::PropertyPathSuffix::ZERO_OR_ONE)  |
            attr(ast::PropertyPathSuffix::NONE) ;

        auto const property_path_atom_def =
            ( ("^" >> attr(true)) | attr(false) )
            >> ( node_name | ('(' >> property_path_alternatives >> ')') )
            >> property_path_suffix;

        auto const linear_pattern_def =
            node >> *((edge | property_path) >> node);

        auto const statement_def =
            select_item >> comparator >> (select_item | value);

        auto optional_not =
            no_case["NOT"] >> attr(true) |
            attr(false);

        auto const atomic_formula_def =
            optional_not >>
            (
                statement |
                '(' >> formula_disjunction >> ')'
            );

        auto const formula_disjunction_def =
            formula_conjunction % no_case["OR"];

        auto const formula_conjunction_def =
            atomic_formula % no_case["AND"];

        auto const formula_def =
            formula_conjunction;

        auto explain_statement =
            no_case["explain"] >> attr(true) |
            attr(false);

        auto distinct_statement =
            no_case["distinct"] >> attr(true) |
            attr(false);

        auto const select_item_def =
            var >> -("." >> key);

        auto const select_items_def =
            ( lit('*') >> attr(std::vector<ast::SelectItem>()) ) |
            select_item % ',';

        auto const select_statement_def =
            no_case["select"] >> distinct_statement >> select_items;

        auto const match_statement =
            no_case["match"] >> graph_pattern;

        auto const linear_pattern_list =
            linear_pattern % ',';

        auto const optional_pattern_def =
            no_case["optional"] >> '{' >> graph_pattern >> '}';

        auto const graph_pattern_def =
            linear_pattern_list >> *optional_pattern;

        auto const where_statement =
            no_case["where"] >> formula_disjunction;

        auto const order_def =
            no_case["asc"]  >> -no_case["ending"] >> attr(ast::Order::Ascending)  |
            no_case["desc"] >> -no_case["ending"] >> attr(ast::Order::Descending) |
            attr(ast::Order::Ascending) ;

        auto const ordered_select_item_def =
            select_item >> order;

        auto const ordered_select_items_def =
            ordered_select_item % ',';

        auto const group_by_statement =
            no_case["group by"] >> ordered_select_items;

        auto const order_by_statement =
            no_case["order by"] >> ordered_select_items;

        auto const limit_statement =
            no_case["limit"] >> uint64;

        auto const root_def =
            explain_statement
            >> select_statement
            >> match_statement
            >> -(where_statement)
            >> -(group_by_statement)
            >> -(order_by_statement)
            >> -(limit_statement);

        BOOST_SPIRIT_DEFINE(
            root,
            select_item,
            select_items,
            select_statement,
            node,
            edge,

            property_path,
            property_path_atom,
            property_path_alternatives,
            property_path_sequence,
            property_path_bound_suffix,

            graph_pattern,
            optional_pattern,
            linear_pattern,

            statement,
            atomic_formula,
            formula_conjunction,
            formula_disjunction,
            order,
            ordered_select_item,
            ordered_select_items
        )
    }

    parser::query_type query() {
        return parser::root;
    }
}

#endif // BASE__GRAMMAR_H_

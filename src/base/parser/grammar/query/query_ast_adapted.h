#ifndef BASE__QUERY_AST_ADAPTED_H_
#define BASE__QUERY_AST_ADAPTED_H_

#include <boost/fusion/include/adapt_struct.hpp>

#include "base/parser/grammar/query/query_ast.h"

BOOST_FUSION_ADAPT_STRUCT(query::ast::Root,
    explain, select, graph_pattern, where, group_by, order_by, limit
)

BOOST_FUSION_ADAPT_STRUCT(query::ast::GraphPattern,
    pattern, optionals
)

BOOST_FUSION_ADAPT_STRUCT(query::ast::LinearPattern,
    root, path
)

BOOST_FUSION_ADAPT_STRUCT(query::ast::LinearPatternStep,
    path, node
)

BOOST_FUSION_ADAPT_STRUCT(query::ast::Node,
    id, labels, properties
)

BOOST_FUSION_ADAPT_STRUCT(query::ast::Edge,
    id, types, properties, direction
)

BOOST_FUSION_ADAPT_STRUCT(query::ast::PropertyPath,
    var, path_alternatives, direction
)

BOOST_FUSION_ADAPT_STRUCT(query::ast::PropertyPathAtom,
    inverse, atom, suffix
)

BOOST_FUSION_ADAPT_STRUCT(query::ast::PropertyPathSequence,
    atoms
)

BOOST_FUSION_ADAPT_STRUCT(query::ast::PropertyPathAlternatives,
    alternatives
)

BOOST_FUSION_ADAPT_STRUCT(query::ast::PropertyPathBoundSuffix,
    min, max
)

BOOST_FUSION_ADAPT_STRUCT(query::ast::SelectItem,
    var, key
)

BOOST_FUSION_ADAPT_STRUCT(query::ast::OrderedSelectItem,
    item, order
)

BOOST_FUSION_ADAPT_STRUCT(query::ast::Statement,
    lhs, comparator, rhs
)

BOOST_FUSION_ADAPT_STRUCT(query::ast::AtomicFormula,
    negation, content
)

BOOST_FUSION_ADAPT_STRUCT(query::ast::FormulaDisjunction,
    formula_conjunctions
)

BOOST_FUSION_ADAPT_STRUCT(query::ast::FormulaConjunction,
    formulas
)

BOOST_FUSION_ADAPT_STRUCT(query::ast::SelectStatement,
    distinct, selection
)


#endif // BASE__QUERY_AST_ADAPTED_H_

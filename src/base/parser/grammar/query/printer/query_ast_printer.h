#ifndef BASE__QUERY_AST_PRINTER_H_
#define BASE__QUERY_AST_PRINTER_H__

#include <boost/optional.hpp>
#include <boost/variant.hpp>

#include "base/parser/grammar/query/query_ast.h"

// Prints the AST generated at parsing to a given stream.
class QueryAstPrinter : public boost::static_visitor<void> {
private:
    std::ostream& out;
    const int_fast32_t base_indent;
    static constexpr auto tab_size = 2;

public:
    QueryAstPrinter(std::ostream& out, int_fast32_t base_indent);
    QueryAstPrinter(std::ostream& out);

    void indent() const;
    void indent(std::string str) const;
    void indent(std::string str, int_fast32_t extra_indent) const;

    void operator() (query::ast::Root const&) const;
    void operator() (std::vector<query::ast::SelectItem> const&) const;
    void operator() (std::vector<query::ast::OrderedSelectItem> const&) const;
    void operator() (std::vector<query::ast::LinearPattern> const&) const;
    void operator() (query::ast::GraphPattern const&) const;
    void operator() (query::ast::LinearPattern const&) const;
    void operator() (query::ast::Node const&) const;
    void operator() (query::ast::Edge const&) const;
    void operator() (query::ast::PropertyPath const&) const;
    void operator() (query::ast::PropertyPathAlternatives const&) const;
    void operator() (query::ast::PropertyPathSequence const&) const;
    void operator() (query::ast::PropertyPathAtom const&) const;
    void operator() (query::ast::PropertyPathSuffix const&) const;
    void operator() (query::ast::PropertyPathBoundSuffix const&) const;
    void operator() (query::ast::LinearPatternStep const&) const;
    void operator() (query::ast::Statement const&) const;
    void operator() (query::ast::FormulaDisjunction const&) const;
    void operator() (query::ast::FormulaConjunction const&) const;
    void operator() (query::ast::AtomicFormula const&) const;
    void operator() (query::ast::Value const&) const;
    void operator() (query::ast::SelectItem const&) const;

    void operator() (int64_t         const&) const;
    void operator() (float           const&) const;
    void operator() (bool            const&) const;
    void operator() (std::string     const&) const;

    void operator() (query::ast::Comparator const&) const;
};

#endif  // BASE__QUERY_AST_PRINTER_H_

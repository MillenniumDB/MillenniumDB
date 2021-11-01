#ifndef BASE__FORMULA_CHECK_VARS_H_
#define BASE__FORMULA_CHECK_VARS_H_

#include <boost/optional.hpp>
#include <boost/variant.hpp>
#include <set>

#include "base/parser/grammar/query/query_ast.h"
#include "base/parser/logical_plan/var.h"

// Checks every var used in the WHERE clause is declared in the MATCH clause
class CheckVarNamesFormula : public boost::static_visitor<void> {
public:
    const std::set<Var>& declared_vars;

    CheckVarNamesFormula(const std::set<Var>& declared_vars);

    void operator()(boost::optional<query::ast::FormulaDisjunction> const&) const;
    void operator()(query::ast::AtomicFormula const&) const;
    void operator()(query::ast::FormulaConjunction const&) const;
    void operator()(query::ast::FormulaDisjunction const&) const;
    void operator()(query::ast::Statement const&) const;
    void operator()(query::ast::SelectItem const&) const;
    void operator()(common::ast::Value const&) const;
};

#endif // BASE__FORMULA_CHECK_VARS_H_

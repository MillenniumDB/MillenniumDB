#ifndef BASE__GET_FORMULA_VARS_H_
#define BASE__GET_FORMULA_VARS_H_

#include <boost/optional.hpp>
#include <boost/variant.hpp>
#include <set>

#include "base/parser/grammar/query/query_ast.h"
#include "base/parser/logical_plan/var.h"

// Return every var used in the WHERE (including vars with properties like ?x.key)
class GetFormulaVars : public boost::static_visitor<std::set<Var>> {
public:
    GetFormulaVars() { }

    std::set<Var> operator()(query::ast::AtomicFormula const&);
    std::set<Var> operator()(query::ast::FormulaConjunction const&);
    std::set<Var> operator()(query::ast::FormulaDisjunction const&);
    std::set<Var> operator()(query::ast::Statement const&);

private:
    std::set<Var> get_vars(boost::variant<query::ast::SelectItem, common::ast::Value> item);
};

#endif // BASE__GET_FORMULA_VARS_H_
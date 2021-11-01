#ifndef BASE__GET_FORMULA_PROPERTY_VARS_H_
#define BASE__GET_FORMULA_PROPERTY_VARS_H_

#include <boost/optional.hpp>
#include <boost/variant.hpp>
#include <set>

#include "base/parser/grammar/query/query_ast.h"
#include "base/parser/logical_plan/var.h"

// Return properties var used in the WHERE (like ?x.key)
class GetFormulaPropertyVars : public boost::static_visitor<std::set<std::pair<Var, std::string>>> {
public:
    GetFormulaPropertyVars() { }

    std::set<std::pair<Var, std::string>> operator()(query::ast::AtomicFormula const&);
    std::set<std::pair<Var, std::string>> operator()(query::ast::FormulaConjunction const&);
    std::set<std::pair<Var, std::string>> operator()(query::ast::FormulaDisjunction const&);
    std::set<std::pair<Var, std::string>> operator()(query::ast::Statement const&);

private:
    std::set<std::pair<Var, std::string>> get_vars(boost::variant<query::ast::SelectItem, common::ast::Value> item);
};

#endif // BASE__GET_FORMULA_PROPERTY_VARS_H_
#include "get_formula_vars.h"

using namespace query::ast;

std::set<Var> GetFormulaVars::operator()(AtomicFormula const& atomic_formula) {
    return boost::apply_visitor(*this, atomic_formula.content);
}


std::set<Var> GetFormulaVars::operator()(FormulaDisjunction const& formula_disjunction) {
    std::set<Var> res;
    for (auto& f : formula_disjunction.formula_conjunctions) {
        for (auto& var : (*this)(f)) {
            res.insert(var);
        }
    }
    return res;
}


std::set<Var> GetFormulaVars::operator()(FormulaConjunction const& formula_conjunction) {
    std::set<Var> res;
    for (auto& f : formula_conjunction.formulas) {
        for (auto& var : (*this)(f)) {
            res.insert(var);
        }
    }
    return res;
}


std::set<Var> GetFormulaVars::get_vars(boost::variant<SelectItem, common::ast::Value> item) {
    std::set<Var> res;
    if (item.type() == typeid(SelectItem)) {
        auto select_item = boost::get<SelectItem>(item);
        res.emplace(select_item.var);

        if (select_item.key) {
            res.emplace(select_item.var + "." + select_item.key.get());
        }
    }
    return res;
}


std::set<Var> GetFormulaVars::operator()(Statement const& statement) {
    auto res = get_vars(statement.lhs);
    for (auto& var : get_vars(statement.rhs)) {
        res.insert(var);
    }
    return res;
}

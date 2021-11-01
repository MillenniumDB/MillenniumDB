#include "get_formula_property_vars.h"

using namespace query::ast;

std::set<std::pair<Var, std::string>> GetFormulaPropertyVars::operator()(AtomicFormula const& atomic_formula) {
    return boost::apply_visitor(*this, atomic_formula.content);
}


std::set<std::pair<Var, std::string>> GetFormulaPropertyVars::operator()(FormulaDisjunction const& formula_disjunction) {
    std::set<std::pair<Var, std::string>> res;
    for (auto& f : formula_disjunction.formula_conjunctions) {
        for (auto& var : (*this)(f)) {
            res.insert(var);
        }
    }
    return res;
}


std::set<std::pair<Var, std::string>> GetFormulaPropertyVars::operator()(FormulaConjunction const& formula_conjunction) {
    std::set<std::pair<Var, std::string>> res;
    for (auto& f : formula_conjunction.formulas) {
        for (auto& var : (*this)(f)) {
            res.insert(var);
        }
    }
    return res;
}


std::set<std::pair<Var, std::string>> GetFormulaPropertyVars::get_vars(boost::variant<SelectItem, common::ast::Value> item) {
    std::set<std::pair<Var, std::string>> res;
    if (item.type() == typeid(SelectItem)) {
        auto select_item = boost::get<SelectItem>(item);

        if (select_item.key) {
            res.insert(std::make_pair(Var(select_item.var), select_item.key.get()));
        }
    }
    return res;
}


std::set<std::pair<Var, std::string>> GetFormulaPropertyVars::operator()(Statement const& statement) {
    auto res = get_vars(statement.lhs);
    for (auto& var : get_vars(statement.rhs)) {
        res.insert(var);
    }
    return res;
}

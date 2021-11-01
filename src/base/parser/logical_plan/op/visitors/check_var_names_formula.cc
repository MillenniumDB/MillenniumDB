#include "check_var_names_formula.h"

#include "base/exceptions.h"

CheckVarNamesFormula::CheckVarNamesFormula(const std::set<Var>& declared_vars) :
    declared_vars (declared_vars) { }


void CheckVarNamesFormula::operator()(query::ast::AtomicFormula const& atomic_formula) const {
    boost::apply_visitor(*this, atomic_formula.content);
}


void CheckVarNamesFormula::operator()(boost::optional<query::ast::FormulaDisjunction> const& optional_formula) const {
    if (optional_formula) {
        auto formula = static_cast<query::ast::FormulaDisjunction>(optional_formula.get());
        (*this)(formula);
    }
}


void CheckVarNamesFormula::operator()(query::ast::FormulaDisjunction const& disjunction) const {
    for (auto& f : disjunction.formula_conjunctions) {
        (*this)(f);
    }
}


void CheckVarNamesFormula::operator()(query::ast::FormulaConjunction const& conjunction) const {
    for (auto& f : conjunction.formulas) {
        (*this)(f);
    }
}


void CheckVarNamesFormula::operator()(query::ast::Statement const& statement) const {
    (*this)(statement.lhs);
    boost::apply_visitor(*this, statement.rhs);
}


void CheckVarNamesFormula::operator()(query::ast::SelectItem const& select_item) const {
    if (declared_vars.find(Var(select_item.var)) == declared_vars.cend()){
        throw QuerySemanticException("Variable \"" + select_item.var +
            "\" used in WHERE is not declared in MATCH");
    }
}

void CheckVarNamesFormula::operator()(common::ast::Value const&) const { }

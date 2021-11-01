#include "formula_to_condition.h"

#include "base/graph/condition/value_assign/value_assign_variable.h"
#include "base/graph/condition/value_assign/value_assign_constant.h"
#include "base/parser/grammar/common/value_visitor.h"

using namespace query::ast;

Formula2ConditionVisitor::Formula2ConditionVisitor(const GraphModel& model, const std::map<Var, VarId>& var2var_ids) :
    model       (model),
    var2var_ids (var2var_ids) { }


std::unique_ptr<Condition> Formula2ConditionVisitor::operator()(AtomicFormula const& atomic_formula) {
    if (atomic_formula.negation) {
        auto condition = boost::apply_visitor(*this, atomic_formula.content);
        return std::make_unique<Negation>(std::move(condition));
    } else {
        return boost::apply_visitor(*this, atomic_formula.content);
    }
}


std::unique_ptr<Condition> Formula2ConditionVisitor::operator()(FormulaDisjunction const& formula_disjunction) {
    if (formula_disjunction.formula_conjunctions.size() > 1) {
        std::vector<std::unique_ptr<Condition>> conditions;
        for (auto& f : formula_disjunction.formula_conjunctions) {
            conditions.push_back( (*this)(f) );
        }
        return std::make_unique<Disjunction>(std::move(conditions));
    } else {
        // simplify vector of size 1
        return (*this)(formula_disjunction.formula_conjunctions[0]);
    }
}


std::unique_ptr<Condition> Formula2ConditionVisitor::operator()(FormulaConjunction const& formula_conjunction) {
    if (formula_conjunction.formulas.size() > 1) {
        std::vector<std::unique_ptr<Condition>> conditions;
        for (auto& f : formula_conjunction.formulas) {
            conditions.push_back( (*this)(f) );
        }
        return std::make_unique<Conjunction>(std::move(conditions));
    } else {
        // simplify vector of size 1
        return (*this)(formula_conjunction.formulas[0]);
    }
}


std::unique_ptr<ValueAssign> Formula2ConditionVisitor::get_value_assignator(
    boost::variant<SelectItem, common::ast::Value> item)
{
    if (item.type() == typeid(SelectItem)) {
        auto select_item = boost::get<SelectItem>(item);

        if (select_item.key) {
            auto find_var_id = var2var_ids.find(Var(select_item.var + "." + select_item.key.get()));
            assert(find_var_id != var2var_ids.end()
                    && "Variable names inside WHERE need to be checked before processing conditions");
            return std::make_unique<ValueAssignVariable>(find_var_id->second);
        } else {
            auto find_var_id = var2var_ids.find(Var(select_item.var));
            assert(find_var_id != var2var_ids.end()
                    && "Variable names inside WHERE need to be checked before processing conditions");
            return std::make_unique<ValueAssignVariable>(find_var_id->second);
        }
    } else {
        auto casted_value = boost::get<Value>(item);
        if (casted_value.type() == typeid(std::string)) {
            // strings can be destroyed after `casted_value` is visited, so it needs to be handled differently
            auto str_ptr = std::make_unique<std::string>(boost::get<std::string>(casted_value));
            auto str_graph_object = GraphObject::make_string(str_ptr->c_str());
            return std::make_unique<ValueAssignConstant>(str_graph_object, move(str_ptr));
        } else {
            auto visitor = ValueVisitor();
            auto value = visitor(casted_value);
            return std::make_unique<ValueAssignConstant>(value, nullptr);
        }
    }
}


std::unique_ptr<Condition> Formula2ConditionVisitor::operator()(Statement const& statement) {
    std::unique_ptr<ValueAssign> lhs = get_value_assignator(statement.lhs);
    std::unique_ptr<ValueAssign> rhs = get_value_assignator(statement.rhs);

    switch (statement.comparator) {
        case Comparator::EQ:
            return std::make_unique<Equals>(std::move(lhs), std::move(rhs));

        case Comparator::NE:
            return std::make_unique<NotEquals>(std::move(lhs), std::move(rhs));

        case Comparator::LE:
            return std::make_unique<LessOrEquals>(std::move(lhs), std::move(rhs));

        case Comparator::GE:
            return std::make_unique<GreaterOrEquals>(std::move(lhs), std::move(rhs));

        case Comparator::GT:
            return std::make_unique<GreaterThan>(std::move(lhs), std::move(rhs));

        case Comparator::LT:
            return std::make_unique<LessThan>(std::move(lhs), std::move(rhs));

        default:
            return nullptr;
    }
}

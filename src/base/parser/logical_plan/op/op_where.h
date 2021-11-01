#ifndef BASE__OP_WHERE_H_
#define BASE__OP_WHERE_H_

#include <memory>

#include "base/parser/logical_plan/op/op.h"
#include "base/parser/logical_plan/op/visitors/get_formula_vars.h"
#include "base/parser/grammar/query/printer/query_ast_printer.h"

class OpWhere : public Op {
public:
    std::unique_ptr<Op> op;
    const query::ast::FormulaDisjunction formula_disjunction;

    OpWhere(std::unique_ptr<Op> op, const query::ast::FormulaDisjunction formula_disjunction) :
        op                  (std::move(op)),
        formula_disjunction (formula_disjunction) { }


    void accept_visitor(OpVisitor& visitor) override {
        visitor.visit(*this);
    }


    std::ostream& print_to_ostream(std::ostream& os, int indent=0) const override {
        os << std::string(indent, ' ');
        os << "OpWhere(";
        QueryAstPrinter printer(os, 0);
        printer(formula_disjunction);
        os << ")\n";
        return op->print_to_ostream(os, indent + 2);
    }


    std::set<Var> get_vars() const override {
        GetFormulaVars visitor;
        auto res = visitor(formula_disjunction);
        for (auto& var : op->get_vars()) {
            res.insert(var);
        }
        return res;
    }
};

#endif // BASE__OP_WHERE_H_

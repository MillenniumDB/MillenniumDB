#pragma once

#include <string>
#include <vector>

#include "query/executor/binding_iter.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

class BindingExprPrinter {
public:
    std::ostream& os;

    const size_t base_indentation;

    const bool stats;

    BindingExprPrinter(std::ostream& os, size_t base_indentation = 0, bool stats = false) :
        os(os),
        base_indentation(base_indentation),
        stats(stats)
    { }

    ~BindingExprPrinter()
    {
        for (size_t i = 0; i < ops.size(); i++) {
            os << std::string(base_indentation + 2, ' ') << "_Op_" << i << "_:\n";
            ops[i]->print(os, base_indentation + 4, stats);
        }
    }

    void print(const BindingExpr& binding_expr)
    {
        binding_expr.print(os, ops);
    }

    std::vector<BindingIter*> ops;
};

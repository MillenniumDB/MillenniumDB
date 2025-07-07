
#pragma once

#include "query/executor/binding_iter/binding_expr/binding_expr.h"
#include "query/var_id.h"

class UAgg {
public:
    virtual ~UAgg() = default;

    UAgg(VarId var_id, BindingExpr* expr) :
        expr(expr),
        var_id(var_id)
    { }

    virtual void set_binding(Binding& _binding)
    {
        binding = &_binding;
    }

    virtual size_t get_offset() const = 0;

    virtual void begin(void* state_data) = 0;

    // evals the expression with the binding, changing the state_data
    virtual void process(void* state_data) = 0;

    // gets the final result of the aggregation
    virtual ObjectId get(void* state_data) = 0;

    virtual std::ostream& print_to_ostream(std::ostream& os) const = 0;

    friend std::ostream& operator<<(std::ostream& os, const UAgg& a)
    {
        return a.print_to_ostream(os);
    }

protected:
    Binding* binding;
    BindingExpr* expr;
    VarId var_id;
};

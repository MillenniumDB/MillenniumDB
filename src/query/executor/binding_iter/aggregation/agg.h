#pragma once

#include <memory>

#include "query/executor/binding_iter/aggregation/unordered_agg.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"
#include "query/var_id.h"

class Agg {
public:
    virtual ~Agg() = default;

    Agg(VarId var_id, std::unique_ptr<BindingExpr> expr) :
        expr(std::move(expr)),
        var_id(var_id)
    { }

    virtual void set_binding(Binding& _binding)
    {
        binding = &_binding;
    }

    virtual void begin() = 0;

    virtual void process() = 0;

    virtual ObjectId get() = 0;

    virtual bool is_pipelineble() const
    {
        return false;
    }

    virtual std::ostream& print_to_ostream(std::ostream& os) const = 0;

    friend std::ostream& operator<<(std::ostream& os, const Agg& a)
    {
        return a.print_to_ostream(os);
    }

    // must be overriden if is_pipelineble() returns true
    virtual std::unique_ptr<UAgg> get_uagg()
    {
        return nullptr;
    }

protected:
    Binding* binding;
    std::unique_ptr<BindingExpr> expr;
    VarId var_id;
};

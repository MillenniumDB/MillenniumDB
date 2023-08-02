#pragma once

#include <ostream>

#include "query/executor/binding.h"
#include "graph_models/object_id.h"

// Abstract class
class BindingExpr {
public:
    virtual ~BindingExpr() = default;

    virtual ObjectId eval(const Binding&) = 0;

    virtual std::ostream& print_to_ostream(std::ostream& os) const = 0;

    friend std::ostream& operator<<(std::ostream& os, const BindingExpr& e) {
        return e.print_to_ostream(os);
    }
};

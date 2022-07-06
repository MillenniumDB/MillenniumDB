#pragma once

#include <set>

#include "base/query/var.h"
#include "parser/query/return_item/return_item_visitor.h"

class ReturnItem {
public:
    virtual ~ReturnItem() = default;

    virtual void accept_visitor(ReturnItemVisitor&) = 0;

    virtual Var get_var() const = 0;

    // In the case of something like COUNT(?x), we have 2 variables: `count(?x) and ?x`
    virtual std::set<Var> get_vars() const = 0;

    virtual std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const = 0;

    friend std::ostream& operator<<(std::ostream& os, const ReturnItem& b) {
        return b.print_to_ostream(os);
    }
};

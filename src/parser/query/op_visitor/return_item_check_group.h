#pragma once

#include <set>

#include "base/query/var.h"
#include "parser/query/return_item/return_item_visitor.h"

/*
Checks the use of the variable is consistent with the group
*/
class ReturnItemCheckGroup : public ReturnItemVisitor {
public:
    ReturnItemCheckGroup(const std::set<std::string>& group_var_names) :
        group_var_names (group_var_names) { }

    void visit(ReturnItemAgg&)   override;
    void visit(ReturnItemCount&) override;
    void visit(ReturnItemVar&)   override;

private:
    // Variables declared in GROUP BY statement, empty if not present
    const std::set<std::string>& group_var_names;
};

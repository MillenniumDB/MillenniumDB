#include "return_item_check_group.h"

#include "base/exceptions.h"
#include "parser/query/return_item/return_items.h"

using namespace MDB;

void ReturnItemCheckGroup::visit(ReturnItemAgg& return_item) {
    // AGG_FUNC(?x) possible only if ?x is NOT in GROUP BY
    // AGG_FUNC(?x.key) possible only if ?x and ?x.key are NOT in GROUP BY
    if (group_var_names.size() != 0) {
        auto pos = return_item.inside_var.find('.');
        if (pos != std::string::npos) {
            auto var_without_property = return_item.inside_var.substr(0, pos);
            if (group_var_names.find(return_item.inside_var) != group_var_names.end()
                || group_var_names.find(var_without_property) != group_var_names.end())
            {
                throw QuerySemanticException("Variable '" + return_item.inside_var
                                           + "' can't be aggregated because is in GROUP BY");
            }
        } else {
            if (group_var_names.find(return_item.inside_var) != group_var_names.end()) {
                throw QuerySemanticException("Variable '" + return_item.inside_var
                                           + "' can't be aggregated because is in GROUP BY");
            }
        }
    }
}


void ReturnItemCheckGroup::visit(ReturnItemCount& return_item) {
    // COUNT(*) always possible
    // COUNT(?x) possible only if ?x is NOT in GROUP BY
    // COUNT(?x.key) possible only if ?x and ?x.key are NOT in GROUP BY
    if (return_item.inside_var == "*") {
        return;
    }

    if (group_var_names.size() != 0) {
        auto pos = return_item.inside_var.find('.');
        if (pos != std::string::npos) {
            auto var_without_property = return_item.inside_var.substr(0, pos);
            if (group_var_names.find(return_item.inside_var) != group_var_names.end()
                || group_var_names.find(var_without_property) != group_var_names.end())
            {
                throw QuerySemanticException("Variable '" + return_item.inside_var
                                           + "' can't be aggregated because is in GROUP BY");
            }
        } else {
            if (group_var_names.find(return_item.inside_var) != group_var_names.end()) {
                throw QuerySemanticException("Variable '" + return_item.inside_var
                                           + "' can't be aggregated because is in GROUP BY");
            }
        }
    }
}


void ReturnItemCheckGroup::visit(ReturnItemVar& return_item) {
    // ?x possible only if ?x is in GROUP BY
    // ?x.key possible only if ?x or ?x.key is in GROUP BY
    if (group_var_names.size() != 0) {
        auto pos = return_item.var.name.find('.');
        if (pos != std::string::npos) {
            auto var_without_property = return_item.var.name.substr(0, pos);
            if (group_var_names.find(return_item.var.name) == group_var_names.end()
                && group_var_names.find(var_without_property) == group_var_names.end())
            {
                throw QuerySemanticException("Variable '" + return_item.var.name
                                           + "' can't be returned because is not in GROUP BY");
            }
        } else {
            if (group_var_names.find(return_item.var.name) == group_var_names.end()) {
                throw QuerySemanticException("Variable '" + return_item.var.name
                                           + "' can't be returned because is not in GROUP BY");
            }
        }
    }
}

#pragma once

#include <set>

#include "base/query/var.h"
#include "parser/query/return_item/return_item_visitor.h"

namespace MDB {
/*
Checks the variable is declared in MATCH statement.
Prevents using properties from variables that are PropertyPaths.
Prevents using DISTINCT with path variables (not supported yet).
*/
class ReturnItemCheckVarName : public ReturnItemVisitor {
public:
    ReturnItemCheckVarName(std::set<Var>& declared_vars,
                           std::set<Var>& declared_path_vars,
                           std::string    operation_name,
                           bool           op_return_distinct = false) :
        declared_vars(declared_vars),
        declared_path_vars(declared_path_vars),
        operation_name(operation_name),
        op_return_distinct(op_return_distinct) { }

    void visit(ReturnItemAgg&) override;
    void visit(ReturnItemCount&) override;
    void visit(ReturnItemVar&) override;

private:
    // Variables declared in MATCH statement
    std::set<Var>& declared_vars;

    // Variables declared in MATCH statement which are property paths
    std::set<Var>& declared_path_vars;

    // May be "RETURN", "ORDER BY", "GROUP BY"
    std::string operation_name;

    bool op_return_distinct;

    void validate_var(const std::string& var_name, bool distinct) const;
};
} // namespace MDB
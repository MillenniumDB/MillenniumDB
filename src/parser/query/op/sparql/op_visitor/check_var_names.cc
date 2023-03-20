#include "check_var_names.h"

#include <set>

#include "base/exceptions.h"
#include "base/query/var.h"
#include "parser/query/op/sparql/ops.h"

using namespace SPARQL;

void CheckVarNames::visit(OpSelect& op_select) {
    std::set<Var> where_vars = op_select.get_vars();
    for (auto& var : op_select.vars) {
        if (where_vars.find(var) == where_vars.end()) {
            throw QuerySemanticException("Variable \"" + var.name + "\" is not declared");
        }
    }
}
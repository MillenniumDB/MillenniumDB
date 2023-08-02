#pragma once

#include <set>
#include <map>

#include "query/var_id.h"
#include "query/parser/op/op_visitor.h"

namespace SPARQL {
/*
   According to (Schmidt, 2010) safe vars are recursively defined as:
   safeVars([[t]]_D) = vars(t)
   safeVars(A1 JOIN A2) = safeVars(A1) U safeVars(A2)
   safeVars(A1 UNION A2) = safeVars(A1) ∩ safeVars(A2)
   safeVars(A1 \ A2) = safeVars(A1)
   safeVars(A1 OPT A2) = safeVars(A1)
   safeVars(Select(A1)) = safeVars(A1) ∩ select.vars
   safeVars(Filter(A1)) = safeVars(A1)

   This class is meant to be used on an op anywhere on the tree
   and then the safe_vars variable is checked afterwards to
   determine what where the safe vars on the tree.
*/
class ObtainVarCount : public OpVisitor {
public:
    /// Invariant: safe_vars is always the safe vars of the current node
    /// after leaving visit.
    std::map<VarId, int> var_count;

    void visit(OpConstruct&)            override;
    void visit(OpDescribe&)             override;
    void visit(OpBasicGraphPattern&)    override;
    void visit(OpEmpty&)                override;
    void visit(OpFilter&)               override;
    void visit(OpJoin&)                 override;
    void visit(OpMinus&)                override;
    void visit(OpNotExists&)            override;
    void visit(OpSemiJoin&)             override;
    void visit(OpUnion&)                override;
    void visit(OpOptional&)             override;
    void visit(OpOrderBy&)              override;
    void visit(OpGroupBy&)              override;
    void visit(OpHaving&)               override;
    void visit(OpSelect&)               override;
    void visit(OpAsk&)                  override;
    void visit(OpService&)              override;
    void visit(OpWhere&)                override;
    void visit(OpBind&)                 override;
    void visit(OpUnitTable&)            override;
    void visit(OpValues&)               override;
};
} // namespace SPARQL

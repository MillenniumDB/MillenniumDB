#pragma once

#include <map>
#include <set>

#include "base/binding/binding_id_iter.h"
#include "base/exceptions.h"
#include "base/ids/id.h"
#include "base/query/sparql/sparql_element.h"
#include "base/query/var.h"
#include "base/thread/thread_info.h"
#include "parser/query/op/sparql/ops.h"

namespace SPARQL {

class BindingIdIterVisitor : public OpVisitor {
public:
    BindingIdIterVisitor(ThreadInfo* thread_info, const std::map<Var, VarId>& var2var_id);

    const std::map<Var, VarId>& var2var_id;

    // Variables that are assigned when evaluating the triple, but the optimizer don't know
    // the value (e.g. OPTIONALS)
    std::set<VarId> assigned_vars;

    ThreadInfo* thread_info;

    // After visiting an Op, the result must be written into tmp
    std::unique_ptr<BindingIdIter> tmp;

    VarId get_var_id(const Var& var) const;

    void visit(OpTriples&) override;
    void visit(OpOptional&) override;
    // BindingIdIterVisitor manages these ops
    void visit(OpSelect&) override {};
    void visit(OpWhere&) override {};

private:
    Id get_id(const SparqlElement&) const;
};
} // namespace SPARQL
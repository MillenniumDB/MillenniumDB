#pragma once

#include <map>
#include <memory>
#include <set>
#include <vector>

#include "base/binding/binding_id_iter.h"
#include "base/binding/binding_iter.h"
#include "base/exceptions.h"
#include "base/ids/var_id.h"
#include "base/query/var.h"
#include "base/thread/thread_info.h"
#include "parser/query/op/sparql/ops.h"
#include "parser/query/op/op_visitor.h"
#include "query_optimizer/rdf_model/binding_id_iter_visitor.h"

namespace SPARQL {

class BindingIterVisitor : public OpVisitor {
public:
    // each visitor will set its corresponding BindingIter here
    std::unique_ptr<BindingIter> tmp;

    ThreadInfo* thread_info;

    const std::map<Var, VarId> var2var_id;

    std::vector<std::pair<Var, VarId>> projection_vars;

    // When true, DistinctIdHash will be applied in visit(OpWhere&) to remove duplicates
    bool distinct_into_id = false;

    bool distinct_ordered_possible = false;

    BindingIterVisitor(std::set<Var> var_names, ThreadInfo* thread_info);

    VarId get_var_id(const Var& var_name) const;

    static std::map<Var, VarId> construct_var2var_id(std::set<Var>& var);

    void visit(OpSelect&) override;
    void visit(OpWhere&) override;
    // BindingIdIterVisitor manages these ops
    void visit(OpTriples&) override {};
    void visit(OpOptional&) override {};
};
} // namespace SPARQL
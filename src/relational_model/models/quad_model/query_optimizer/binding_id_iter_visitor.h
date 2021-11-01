#ifndef RELATIONAL_MODEL__BINDING_ID_ITER_VISITOR_H_
#define RELATIONAL_MODEL__BINDING_ID_ITER_VISITOR_H_

#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

#include "base/graph/graph_object.h"
#include "base/ids/object_id.h"
#include "base/ids/var_id.h"
#include "base/parser/grammar/manual_plan/manual_plan_ast.h"
#include "base/parser/grammar/query/query_ast.h"
#include "base/parser/logical_plan/var.h"
#include "base/parser/logical_plan/op/visitors/op_visitor.h"
#include "base/thread/thread_info.h"
#include "relational_model/models/quad_model/quad_model.h"
#include "relational_model/models/quad_model/query_optimizer/plan/plan.h"

class BindingIdIter;

class BindingIdIterVisitor : public OpVisitor {
public:
    BindingIdIterVisitor(const QuadModel& model, const std::map<Var, VarId>& var2var_id, ThreadInfo* thread_info);
    ~BindingIdIterVisitor() = default;

    const QuadModel& model;
    const std::vector<query::ast::SelectItem> select_items;
    const std::map<Var, VarId>& var2var_id;

    std::set<VarId> assigned_vars;
    ThreadInfo* thread_info;

    // After visiting an Op, the result must be written into tmp
    std::unique_ptr<BindingIdIter> tmp;

    std::unique_ptr<BindingIdIter> exec(OpSelect&);
    std::unique_ptr<BindingIdIter> exec(manual_plan::ast::ManualRoot&);

    VarId get_var_id(const Var& var);

    /* This visitor only process 2 Ops */
    void visit(OpBasicGraphPattern&) override;
    void visit(OpOptional&)          override;

    /* These are processed in Binding */
    void visit(OpConnection&)       override { }
    void visit(OpDistinct&)         override { }
    void visit(OpGroupBy&)          override { }
    void visit(OpWhere&)           override { }
    void visit(OpMatch&) override { }
    void visit(OpIsolatedTerm&)     override { }
    void visit(OpIsolatedVar&)      override { }
    void visit(OpLabel&)            override { }
    void visit(OpOrderBy&)          override { }
    void visit(OpPath&)             override { }
    void visit(OpPathAlternatives&) override { }
    void visit(OpPathAtom&)         override { }
    void visit(OpPathSequence&)     override { }
    void visit(OpPathKleeneStar&)   override { }
    void visit(OpPathOptional&)     override { }
    void visit(OpProperty&)         override { }
    void visit(OpPropertyPath&)     override { }
    void visit(OpSelect&)           override { }
};

#endif // RELATIONAL_MODEL__BINDING_ID_ITER_VISITOR_H_

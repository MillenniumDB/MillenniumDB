#pragma once

#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

#include "base/graph_object/graph_object.h"
#include "base/ids/id.h"
#include "base/query/var.h"
#include "base/thread/thread_info.h"
#include "parser/query/op/ops.h"

class BindingIdIter;
class NodeId;

class BindingIdIterVisitor : public OpVisitor {
public:
    BindingIdIterVisitor(ThreadInfo* thread_info,
                         const std::map<Var, VarId>& var2var_id,
                         std::map<VarId, ObjectId>& fixed_vars,
                         const std::vector<std::tuple<Var, std::string, QueryElement>>& where_properties);

    // const std::vector<query::ast::SelectItem> select_items;
    const std::map<Var, VarId>& var2var_id;

    // Variables that are assigned when evaluating the basic graph pattern, and the optimizer know the value.
    // May come from OpProperty or the Where clause
    std::map<VarId, ObjectId>& fixed_vars;

    // Properties from a where clause that are mandatory (so they are pushed into the basic graph pattern)
    const std::vector<std::tuple<Var, std::string, QueryElement>>& where_properties;

    // Variables that are assigned when evaluating the basic graph pattern, but the optimizer don't know
    // the value (e.g. OPTIONALS)
    std::set<VarId> assigned_vars;

    ThreadInfo* thread_info;

    // After visiting an Op, the result must be written into tmp
    std::unique_ptr<BindingIdIter> tmp;

    VarId get_var_id(const Var& var) const;

    /* This visitor only process these 2 Ops */
    void visit(OpBasicGraphPattern&) override;
    void visit(OpOptional&)          override;

    /* These are processed in BindingIterVisitor */
    void visit(OpDescribe&) override { }
    void visit(OpGroupBy&)  override { }
    void visit(OpMatch&)    override { }
    void visit(OpOrderBy&)  override { }
    void visit(OpReturn&)   override { }
    void visit(OpSet&)      override { }
    void visit(OpWhere&)    override { }

    /* These are processed inside OpBasicGraphPattern */
    void visit(OpEdge&)         override { }
    void visit(OpIsolatedTerm&) override { }
    void visit(OpIsolatedVar&)  override { }
    void visit(OpLabel&)        override { }
    void visit(OpPath&)         override { }
    void visit(OpProperty&)     override { }

private:
    Id get_id(const QueryElement&) const;

    bool term_exists(ObjectId) const;
};

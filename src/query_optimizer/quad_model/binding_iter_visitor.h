#pragma once

#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

#include "base/binding/binding_iter.h"
#include "base/ids/var_id.h"
#include "base/ids/object_id.h"
#include "base/query/query_element.h"
#include "base/query/var.h"
#include "base/graph_object/graph_object.h"
#include "base/thread/thread_info.h"
#include "execution/binding_iter/aggregation/agg.h"
#include "parser/query/op/mdb/ops.h"

class BindingIterVisitor : public OpVisitor {
public:
    // each visitor will set its corresponding BindingIter here
    std::unique_ptr<BindingIter> tmp;

    ThreadInfo* thread_info;

    const std::map<Var, VarId> var2var_id;

    // properties used in RETURN and ORDER BY. We need to remember them to add optional children in the OpMatch
    std::set<std::pair<Var, std::string>> var_properties;

    std::vector<std::pair<Var, VarId>> projection_vars;

    std::vector<VarId> group_vars;

    std::set<VarId> group_saved_vars;

    // Contains mandatory equalities of variables with constants (to push them from WHERE into the BindingId phase).
    std::map<VarId, ObjectId> fixed_vars;

    // Aggregates from RETURN and ORDER BY
    std::map<VarId, std::unique_ptr<Agg>> aggs;

    // Contains mandatory equalities of properties with constants (to push them from WHERE into the BindingId phase).
    // We use QueryElement instead of GraphObject because it is what OpProperty has as value
    std::vector<std::tuple<Var, std::string, QueryElement>> where_properties;

    // When true, DistinctIdHash will be applied in visit(OpMatch&) to remove duplicates
    bool distinct_into_id = false;

    bool need_materialize_paths = false;

    bool distinct_ordered_possible = false;

    // True if query contains a group by
    bool group = false;

    BindingIterVisitor(std::set<Var> var_names, ThreadInfo* thread_info);

    VarId get_var_id(const Var& var_name) const;

    static std::map<Var, VarId> construct_var2var_id(std::set<Var>& var_names);

    void visit(MDB::OpDescribe&) override;
    void visit(MDB::OpGroupBy&)  override;
    void visit(MDB::OpMatch&)    override;
    void visit(MDB::OpOrderBy&)  override;
    void visit(MDB::OpReturn&)   override;
    void visit(MDB::OpSet&)      override;
    void visit(MDB::OpWhere&)    override;

    /* BindingIdIterVisitor manages these ops */
    void visit(MDB::OpBasicGraphPattern&) override { }
    void visit(MDB::OpEdge&)              override { }
    void visit(MDB::OpIsolatedTerm&)      override { }
    void visit(MDB::OpIsolatedVar&)       override { }
    void visit(MDB::OpLabel&)             override { }
    void visit(MDB::OpOptional&)          override { }
    void visit(MDB::OpPath&)              override { }
    void visit(MDB::OpProperty&)          override { }
};

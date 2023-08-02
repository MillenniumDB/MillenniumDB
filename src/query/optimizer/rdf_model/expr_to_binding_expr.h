#pragma once

#include <chrono>
#include <cstdint>
#include <map>
#include <memory>
#include <optional>
#include <random>
#include <unordered_map>

#include <boost/uuid/uuid_generators.hpp>

#include "graph_models/rdf_model/rdf_model.h"
#include "query/executor/binding_iter/aggregation/agg.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"
#include "query/executor/binding_iter/projection_order_exprs.h"
#include "query/parser/expr/expr.h"
#include "query/parser/expr/expr_visitor.h"
#include "query/query_context.h"
#include "query/var_id.h"

namespace SPARQL {

enum class GroupingMode {
    Grouping,        // Expression must have aggregation or group variable
    NoGrouping,      // Expression must not have aggregation or group variable
    OptionalGrouping // Expression may have aggregation or group variable
};

// This visitor returns nullptr if condition is pushed outside
class ExprToBindingExpr : public ExprVisitor {
public:
    // Current time, used for NOW() and as seed for RAND()
    const std::chrono::system_clock::time_point current_time = std::chrono::system_clock::now();

    // Random number generator for RAND()
    std::default_random_engine rand_generator = std::default_random_engine(current_time.time_since_epoch().count());
    std::uniform_real_distribution<double> distribution = std::uniform_real_distribution<double>(0.0, 1.0);

    // UUID generator for UUID() and STRUUID()
    boost::uuids::random_generator uuid_generator;

    const std::set<VarId>                     safe_assigned_vars;
    const std::set<VarId>                     possible_assigned_vars;

    const std::set<VarId>                     group_vars;
    std::map<VarId, std::unique_ptr<Agg>>*  aggregations;
    const std::vector<ProjectionOrderExpr>*   projection_order_exprs;

    std::unique_ptr<BindingExpr> current_binding_expr;

    // This expressions is part of (<expressions> AS <var>), as_var is <var>
    const std::optional<VarId> as_var;

    const GroupingMode grouping_mode;

    bool visited_aggregation = false; // An aggregation has been visited previously
    bool at_root             = true;  // The visitor is currently at the root node

    // This constructor is used to visit expressions that must not have aggregations or group variables.
    ExprToBindingExpr(
        const std::set<VarId>& safe_assigned_vars,
        const std::set<VarId>& possible_assigned_vars
    ) :
        safe_assigned_vars       (safe_assigned_vars),
        possible_assigned_vars   (possible_assigned_vars),
        group_vars               ({}),
        aggregations             (nullptr),
        projection_order_exprs   (nullptr),
        as_var                   ({}),
        grouping_mode            (GroupingMode::NoGrouping) { }

    // This constructor is used to visit expressions that can have aggregations or group variables.
    ExprToBindingExpr(
        const std::set<VarId>&                   safe_assigned_vars,
        const std::set<VarId>&                   possible_assigned_vars,
        const std::set<VarId>&                   group_vars,
        std::map<VarId, std::unique_ptr<Agg>>& aggregations,
        const std::vector<ProjectionOrderExpr>&  projection_order_exprs,
        std::optional<VarId>                     as_var,
        GroupingMode                             grouping_mode
    ) :
        safe_assigned_vars      (safe_assigned_vars),
        possible_assigned_vars  (possible_assigned_vars),
        group_vars              (group_vars),
        aggregations            (&aggregations),
        projection_order_exprs  (&projection_order_exprs),
        as_var                  (as_var),
        grouping_mode           (grouping_mode) { }

    template<typename AggType> AggType* check_and_make_aggregate(Expr*);

    void visit(SPARQL::ExprVar&)            override;
    void visit(SPARQL::ExprObjectId&)       override;
    void visit(SPARQL::ExprTerm&)           override;
    void visit(SPARQL::ExprEqual&)          override;
    void visit(SPARQL::ExprNotEqual&)       override;

    void visit(SPARQL::ExprNot&)            override;
    void visit(SPARQL::ExprUnaryMinus&)     override;
    void visit(SPARQL::ExprUnaryPlus&)      override;
    void visit(SPARQL::ExprMultiplication&) override;
    void visit(SPARQL::ExprDivision&)       override;
    void visit(SPARQL::ExprAddition&)       override;
    void visit(SPARQL::ExprSubtraction&)    override;
    void visit(SPARQL::ExprAnd&)            override;
    void visit(SPARQL::ExprOr&)             override;

    void visit(SPARQL::ExprLess&)           override;
    void visit(SPARQL::ExprGreater&)        override;
    void visit(SPARQL::ExprLessOrEqual&)    override;
    void visit(SPARQL::ExprGreaterOrEqual&) override;

    void visit(SPARQL::ExprIn&)             override;
    void visit(SPARQL::ExprNotIn&)          override;

    void visit(SPARQL::ExprExists&)         override;
    void visit(SPARQL::ExprNotExists&)      override;

    void visit(SPARQL::ExprAggAvg&)         override;
    void visit(SPARQL::ExprAggCount&)       override;
    void visit(SPARQL::ExprAggCountAll&)    override;
    void visit(SPARQL::ExprAggGroupConcat&) override;
    void visit(SPARQL::ExprAggMax&)         override;
    void visit(SPARQL::ExprAggMin&)         override;
    void visit(SPARQL::ExprAggSample&)      override;
    void visit(SPARQL::ExprAggSum&)         override;

    void visit(SPARQL::ExprAbs&)            override;
    void visit(SPARQL::ExprBNode&)          override;
    void visit(SPARQL::ExprBound&)          override;
    void visit(SPARQL::ExprCeil&)           override;
    void visit(SPARQL::ExprCoalesce&)       override;
    void visit(SPARQL::ExprConcat&)         override;
    void visit(SPARQL::ExprContains&)       override;
    void visit(SPARQL::ExprDatatype&)       override;
    void visit(SPARQL::ExprDay&)            override;
    void visit(SPARQL::ExprEncodeForUri&)   override;
    void visit(SPARQL::ExprFloor&)          override;
    void visit(SPARQL::ExprHours&)          override;
    void visit(SPARQL::ExprIf&)             override;
    void visit(SPARQL::ExprIRI&)            override;
    void visit(SPARQL::ExprIsBlank&)        override;
    void visit(SPARQL::ExprIsIRI&)          override;
    void visit(SPARQL::ExprIsLiteral&)      override;
    void visit(SPARQL::ExprIsNumeric&)      override;
    void visit(SPARQL::ExprIsURI&)          override;
    void visit(SPARQL::ExprLang&)           override;
    void visit(SPARQL::ExprLangMatches&)    override;
    void visit(SPARQL::ExprLCase&)          override;
    void visit(SPARQL::ExprMD5&)            override;
    void visit(SPARQL::ExprMinutes&)        override;
    void visit(SPARQL::ExprMonth&)          override;
    void visit(SPARQL::ExprNow&)            override;
    void visit(SPARQL::ExprRand&)           override;
    void visit(SPARQL::ExprRegex&)          override;
    void visit(SPARQL::ExprReplace&)        override;
    void visit(SPARQL::ExprRound&)          override;
    void visit(SPARQL::ExprSameTerm&)       override;
    void visit(SPARQL::ExprSeconds&)        override;
    void visit(SPARQL::ExprSHA1&)           override;
    void visit(SPARQL::ExprSHA256&)         override;
    void visit(SPARQL::ExprSHA384&)         override;
    void visit(SPARQL::ExprSHA512&)         override;
    void visit(SPARQL::ExprStrAfter&)       override;
    void visit(SPARQL::ExprStrBefore&)      override;
    void visit(SPARQL::ExprStrDT&)          override;
    void visit(SPARQL::ExprStrEnds&)        override;
    void visit(SPARQL::ExprStrLang&)        override;
    void visit(SPARQL::ExprStrLen&)         override;
    void visit(SPARQL::ExprStrStarts&)      override;
    void visit(SPARQL::ExprStrUUID&)        override;
    void visit(SPARQL::ExprStr&)            override;
    void visit(SPARQL::ExprSubStr&)         override;
    void visit(SPARQL::ExprTimezone&)       override;
    void visit(SPARQL::ExprTZ&)             override;
    void visit(SPARQL::ExprUCase&)          override;
    void visit(SPARQL::ExprURI&)            override;
    void visit(SPARQL::ExprUUID&)           override;
    void visit(SPARQL::ExprYear&)           override;
    void visit(SPARQL::ExprCast&)           override;
};
} // namespace SPARQL

#include "query_visitor.h"

#include <cassert>

#include "graph_models/common/datatypes/datetime.h"
#include "graph_models/quad_model/quad_object_id.h"
#include "query/parser/expr/mql_exprs.h"
#include "query/parser/paths/path_alternatives.h"
#include "query/parser/paths/path_atom.h"
#include "query/parser/paths/path_kleene_plus.h"
#include "query/parser/paths/path_kleene_star.h"
#include "query/parser/paths/path_optional.h"
#include "query/parser/paths/path_sequence.h"
#include "query/query_context.h"
#include "storage/index/tensor_store/lsh/metric.h"
#include "storage/index/tensor_store/tensor_store.h"

using namespace MQL;
using antlrcpp::Any;

Any QueryVisitor::visitDescribeQuery(MQL_Parser::DescribeQueryContext* ctx)
{
    visitChildren(ctx);

    assert(last_node.is_OID());

    // Fast path for DESCRIBE without flags
    if (ctx->describeFlag().empty()) {
        current_op = std::make_unique<OpDescribe>(
            last_node.get_OID(),
            OpDescribe::DEFAULT_LIMIT,
            OpDescribe::DEFAULT_LIMIT,
            OpDescribe::DEFAULT_LIMIT,
            OpDescribe::DEFAULT_LIMIT
        );
        return 0;
    }

    uint64_t labels_limit = 0;
    uint64_t properties_limit = 0;
    uint64_t outgoing_limit = 0;
    uint64_t incoming_limit = 0;

    bool labels_limit_seen = false;
    bool properties_limit_seen = false;
    bool outgoing_limit_seen = false;
    bool incoming_limit_seen = false;

    for (auto& flag : ctx->describeFlag()) {
        // Get limit
        uint64_t curr_limit = UINT64_MAX;
        if (flag->K_LIMIT())
            curr_limit = std::stoull(flag->UNSIGNED_INTEGER()->getText());
        // Set limit
        if (flag->K_LABELS() != nullptr) {
            if (labels_limit_seen)
                throw std::runtime_error("Duplicate DESCRIBE flag: LABELS");
            labels_limit = curr_limit;
            labels_limit_seen = true;
        } else if (flag->K_PROPERTIES() != nullptr) {
            if (properties_limit_seen)
                throw std::runtime_error("Duplicate DESCRIBE flag: PROPERTIES");
            properties_limit = curr_limit;
            properties_limit_seen = true;
        } else if (flag->K_OUTGOING() != nullptr) {
            if (outgoing_limit_seen)
                throw std::runtime_error("Duplicate DESCRIBE flag: OUTGOING");
            outgoing_limit = curr_limit;
            outgoing_limit_seen = true;
        } else if (flag->K_INCOMING() != nullptr) {
            if (incoming_limit_seen)
                throw std::runtime_error("Duplicate DESCRIBE flag: INCOMING");
            incoming_limit = curr_limit;
            incoming_limit_seen = true;
        }
    }
    current_op = std::make_unique<OpDescribe>(
        last_node.get_OID(),
        labels_limit,
        properties_limit,
        outgoing_limit,
        incoming_limit
    );

    return 0;
}


// Any QueryVisitor::visitInsertQuery(MQL_Parser::InsertQueryContext* ctx) {
//     visitChildren(ctx);
//     current_op = std::make_unique<OpInsert>(
//         std::move(insert_labels),
//         std::move(insert_properties),
//         std::move(insert_edges)
//     );
//     return 0;
// }


Any QueryVisitor::visitMatchQuery(MQL_Parser::MatchQueryContext* ctx)
{
    visitChildren(ctx);

    // Process used_var_properties
    std::vector<OpProperty> optional_properties;
    for (auto& property : used_var_properties) {
        if (match_var_properties.find(property) == match_var_properties.end()) {
            optional_properties.push_back(property);
        }
    }

    current_op = std::make_unique<OpMatch>(std::move(current_op), std::move(optional_properties));

    if (!project_similarity_info.tensor_store_name.empty()) {
        current_op = std::make_unique<OpProjectSimilarity>(
            std::move(current_op),
            project_similarity_info.object_var,
            project_similarity_info.similarity_var,
            std::move(project_similarity_info.tensor_store_name),
            std::move(project_similarity_info.query_tensor),
            project_similarity_info.query_object,
            project_similarity_info.metric_type
        );
    } else if (!brute_similarity_search_info.tensor_store_name.empty()) {
        current_op = std::make_unique<OpBruteSimilaritySearch>(
            std::move(current_op),
            brute_similarity_search_info.object_var,
            brute_similarity_search_info.similarity_var,
            std::move(brute_similarity_search_info.tensor_store_name),
            std::move(brute_similarity_search_info.query_tensor),
            brute_similarity_search_info.query_object,
            brute_similarity_search_info.metric_type,
            brute_similarity_search_info.k
        );
    }

    if (current_expr != nullptr) {
        current_op = std::make_unique<OpWhere>(std::move(current_op), std::move(current_expr));
    }
    if (group_by_vars.size() > 0) {
        current_op = std::make_unique<OpGroupBy>(std::move(current_op), std::move(group_by_vars));
    }

    if (order_by_info.items.size() > 0) {
        current_op = std::make_unique<OpOrderBy>(
            std::move(current_op),
            std::move(order_by_info.items),
            std::move(order_by_info.ascending_order)
        );
    }

    // RETURN *
    if (return_info.items.size() == 0) {
        auto vars = current_op->get_all_vars();
        for (auto& var : vars) {
            // filter anonymous vars
            if (!get_query_ctx().is_internal(var)) {
                return_info.items.push_back({ var, nullptr });
            }
        }
    }

    current_op = std::make_unique<OpReturn>(
        std::move(current_op),
        std::move(return_info.items),
        return_info.distinct,
        return_info.limit,
        return_info.offset
    );

    if (set_items.size() > 0) {
        current_op = std::make_unique<OpSet>(std::move(current_op), std::move(set_items));
    }
    return 0;
}


Any QueryVisitor::visitMatchStatement(MQL_Parser::MatchStatementContext* ctx)
{
    visitChildren(ctx);
    return 0;
}


Any QueryVisitor::visitSetItem(MQL_Parser::SetItemContext* ctx)
{
    auto var_name = ctx->VARIABLE()->getText();
    var_name.erase(0, 1); // remove leading '?'
    set_items.push_back({ get_query_ctx().get_or_create_var(var_name),
                          QuadObjectId::get_fixed_node_inside(ctx->fixedNodeInside()->getText()) });
    return 0;
}


Any QueryVisitor::visitInsertPatterns(MQL_Parser::InsertPatternsContext* ctx)
{
    current_basic_graph_pattern = std::make_unique<OpBasicGraphPattern>();
    visitChildren(ctx);
    current_op = std::make_unique<OpInsert>(std::move(current_basic_graph_pattern));
    return 0;
}


Any QueryVisitor::visitInsertLinearPattern(MQL_Parser::InsertLinearPatternContext* ctx)
{
    first_element_disjoint = ctx->children.size() == 1;
    ctx->children[0]->accept(this);
    saved_node = last_node;
    for (size_t i = 2; i < ctx->children.size(); i += 2) {
        ctx->children[i]->accept(this); // accept node
        ctx->children[i - 1]->accept(this); // accept edge
        saved_node = last_node;
    }
    return 0;
}


Any QueryVisitor::visitInsertPlainNode(MQL_Parser::InsertPlainNodeContext* ctx)
{
    last_node = QuadObjectId::get_fixed_node_inside(ctx->insertPlainNodeInside()->getText());

    // Process Labels
    for (auto& label : ctx->TYPE()) {
        auto label_str = label->getText();
        label_str.erase(0, 1); // remove leading ':'
        auto label_id = QuadObjectId::get_string(label_str);
        current_basic_graph_pattern->add_label(OpLabel(last_node, label_id));
    }

    auto properties = ctx->properties();
    if (properties != nullptr) {
        saved_property_obj = last_node;
        for (auto property : properties->property()) {
            property->accept(this);
        }
    }

    // necessary to insert even if not disjoint
    current_basic_graph_pattern->add_disjoint_term(last_node.get_OID());

    return 0;
}


Any QueryVisitor::visitInsertPlainEdge(MQL_Parser::InsertPlainEdgeContext* ctx)
{
    auto edge = get_query_ctx().get_internal_var();

    auto type_str = ctx->TYPE()->getText();
    type_str.erase(0, 1); // remove leading ':'
    auto type_id = QuadObjectId::get_named_node(type_str);

    auto properties = ctx->properties();
    if (properties != nullptr) {
        saved_property_obj = edge;
        for (auto property : properties->property()) {
            property->accept(this);
        }
    }

    if (ctx->GT() != nullptr) {
        // right direction
        current_basic_graph_pattern->add_edge(OpEdge(saved_node, last_node, type_id, edge));
    } else {
        // left direction
        current_basic_graph_pattern->add_edge(OpEdge(last_node, saved_node, type_id, edge));
    }
    return 0;
}

// Any QueryVisitor::visitInsertLabelElement(MQL_Parser::InsertLabelElementContext* ctx) {
//     auto label_str = ctx->STRING()->getText();
//     // remove surrounding double quotes
//     label_str = label_str.substr(1, label_str.size() - 2);

//     ObjectId label_id = QuadObjectId::get_string(label_str);

//     ObjectId node_id = (ctx->identifier() == nullptr)
//         ? QuadObjectId::get_fixed_node_inside(ctx->ANON_ID()->getText())
//         : QuadObjectId::get_fixed_node_inside(ctx->identifier()->getText());

//     insert_labels.emplace_back(node_id, label_id);
//     return 0;
// }


// Any QueryVisitor::visitInsertPropertyElement(MQL_Parser::InsertPropertyElementContext* ctx) {
//     auto obj = ctx->fixedNodeInside()->getText();

//     auto key = ctx->STRING()->getText();
//     key = key.substr(1, key.size() - 2); // remove surrounding double quotes

//     insert_properties.emplace_back(
//         QuadObjectId::get_fixed_node_inside(obj),
//         QuadObjectId::get_string(key),
//         QuadObjectId::get_value(ctx->value()->getText())
//     );
//     return 0;
// }


// Any QueryVisitor::visitInsertEdgeElement(MQL_Parser::InsertEdgeElementContext* ctx) {
//     auto from = QuadObjectId::get_fixed_node_inside(ctx->fixedNodeInside()[0]->getText());
//     auto to   = QuadObjectId::get_fixed_node_inside(ctx->fixedNodeInside()[1]->getText());
//     auto type = QuadObjectId::get_fixed_node_inside(ctx->identifier()->getText());
//     insert_edges.emplace_back(
//         from,
//         to,
//         type,
//         ObjectId::get_null() // edge won't be used on inserts, pass anything
//     );
//     return 0;
// }


Any QueryVisitor::visitReturnAll(MQL_Parser::ReturnAllContext* ctx)
{
    if (ctx->limitOffsetClauses() != nullptr) {
        auto limit_clause = ctx->limitOffsetClauses()->limitClause();
        auto offset_clause = ctx->limitOffsetClauses()->offsetClause();

        if (limit_clause) {
            return_info.limit = stoll(limit_clause->UNSIGNED_INTEGER()->getText());
        }
        if (offset_clause) {
            return_info.offset = stoll(offset_clause->UNSIGNED_INTEGER()->getText());
        }
    }
    return_info.distinct = ctx->K_DISTINCT() != nullptr;
    return 0;
}


Any QueryVisitor::visitReturnList(MQL_Parser::ReturnListContext* ctx)
{
    for (auto& return_expr : ctx->returnItem()) {
        return_expr->accept(this);
    }

    if (ctx->limitOffsetClauses() != nullptr) {
        auto limit_clause = ctx->limitOffsetClauses()->limitClause();
        auto offset_clause = ctx->limitOffsetClauses()->offsetClause();

        if (limit_clause) {
            return_info.limit = stoll(limit_clause->UNSIGNED_INTEGER()->getText());
        }
        if (offset_clause) {
            return_info.offset = stoll(offset_clause->UNSIGNED_INTEGER()->getText());
        }
    }
    return_info.distinct = ctx->K_DISTINCT() != nullptr;
    return 0;
}


Any QueryVisitor::visitReturnItemVar(MQL_Parser::ReturnItemVarContext* ctx)
{
    auto var_name = ctx->VARIABLE()->getText();
    var_name.erase(0, 1); // remove leading '?'
    auto var = get_query_ctx().get_or_create_var(var_name);

    if (ctx->KEY() != nullptr) {
        auto key_name = ctx->KEY()->getText();
        auto property_var_name = var_name + key_name;
        key_name.erase(0, 1); // remove leading '.'

        auto property_var = get_query_ctx().get_or_create_var(property_var_name);

        auto key_id = QuadObjectId::get_string(key_name);
        used_var_properties.emplace(var, key_id, property_var);
        return_info.items.push_back({ property_var, nullptr });
    } else {
        return_info.items.push_back({ var, nullptr });
    }
    return 0;
}


Any QueryVisitor::visitReturnItemAgg(MQL_Parser::ReturnItemAggContext* ctx)
{
    auto var_name = ctx->VARIABLE()->getText();
    var_name.erase(0, 1); // remove leading '?'
    auto var = get_query_ctx().get_or_create_var(var_name);
    std::string agg_inside_var_name = var_name;

    std::unique_ptr<Expr> inner_expr;
    if (ctx->KEY() != nullptr) {
        auto key_name = ctx->KEY()->getText();
        agg_inside_var_name += key_name;
        key_name.erase(0, 1); // remove leading '.'

        auto property_var = get_query_ctx().get_or_create_var(agg_inside_var_name);

        auto key_id = QuadObjectId::get_string(key_name);
        used_var_properties.emplace(var, key_id, property_var);
        inner_expr = std::make_unique<ExprVarProperty>(var, key_id, property_var);
    } else {
        inner_expr = std::make_unique<ExprVar>(var);
    }
    std::string agg_var_name;
    std::unique_ptr<Expr> expr;
    if (ctx->aggregateFunc()->K_AVG()) {
        expr = std::make_unique<ExprAggAvg>(std::move(inner_expr));
        agg_var_name = ".AVG";
    } else if (ctx->aggregateFunc()->K_MAX()) {
        expr = std::make_unique<ExprAggMax>(std::move(inner_expr));
        agg_var_name = ".MAX";
    } else if (ctx->aggregateFunc()->K_MIN()) {
        expr = std::make_unique<ExprAggMin>(std::move(inner_expr));
        agg_var_name = ".MIN";
    } else if (ctx->aggregateFunc()->K_SUM()) {
        expr = std::make_unique<ExprAggSum>(std::move(inner_expr));
        agg_var_name = ".SUM";
    } else {
        throw std::runtime_error("Unmanaged aggregateFunc");
    }
    agg_var_name += "(";
    agg_var_name += agg_inside_var_name;
    agg_var_name += ")";
    return_info.items.push_back({ get_query_ctx().get_or_create_var(agg_var_name), std::move(expr) });
    return 0;
}


Any QueryVisitor::visitReturnItemCount(MQL_Parser::ReturnItemCountContext* ctx)
{
    bool distinct = ctx->K_DISTINCT() != nullptr;
    if (ctx->VARIABLE() != nullptr) {
        auto var_name = ctx->VARIABLE()->getText();
        var_name.erase(0, 1); // remove leading '?'
        auto var = get_query_ctx().get_or_create_var(var_name);

        if (ctx->KEY() != nullptr) {
            auto key_name = ctx->KEY()->getText();
            auto property_var_name = var_name + key_name;
            key_name.erase(0, 1); // remove leading '.'

            auto property_var = get_query_ctx().get_or_create_var(property_var_name);
            auto key_id = QuadObjectId::get_string(key_name);
            used_var_properties.emplace(var, key_id, property_var);

            std::string expr_var = ".COUNT(";
            expr_var += (distinct ? "DISTINCT " : "");
            expr_var += property_var_name + ")";
            return_info.items.push_back({ get_query_ctx().get_or_create_var(expr_var),
                                          std::make_unique<ExprAggCount>(
                                              std::make_unique<ExprVarProperty>(var, key_id, property_var),
                                              distinct
                                          ) });
        } else {
            std::string expr_var = ".COUNT(";
            expr_var += (distinct ? "DISTINCT " : "");
            expr_var += var_name + ")";
            return_info.items.push_back(
                { get_query_ctx().get_or_create_var(expr_var),
                  std::make_unique<ExprAggCount>(std::make_unique<ExprVar>(var), distinct) }
            );
        }
    } else {
        std::string expr_var = ".COUNT(";
        expr_var += (distinct ? "DISTINCT " : "");
        expr_var += "*)";

        return_info.items.push_back({ get_query_ctx().get_or_create_var(expr_var),
                                      std::make_unique<ExprAggCountAll>(distinct) });
    }

    return 0;
}


Any QueryVisitor::visitOrderByStatement(MQL_Parser::OrderByStatementContext* ctx)
{
    for (auto order_by_item : ctx->orderByItem()) {
        order_by_item->accept(this);
    }

    return 0;
}


Any QueryVisitor::visitOrderByItemVar(MQL_Parser::OrderByItemVarContext* ctx)
{
    auto var_name = ctx->VARIABLE()->getText();
    var_name.erase(0, 1); // remove leading '?'
    auto var = get_query_ctx().get_or_create_var(var_name);

    if (ctx->KEY() != nullptr) {
        auto key_name = ctx->KEY()->getText();
        auto property_var_name = var_name + key_name;
        key_name.erase(0, 1); // remove leading '.'

        auto property_var = get_query_ctx().get_or_create_var(property_var_name);

        auto key_id = QuadObjectId::get_string(key_name);
        used_var_properties.emplace(var, key_id, property_var);
        order_by_info.items.push_back({ property_var, nullptr });
    } else {
        order_by_info.items.push_back({ var, nullptr });
    }
    order_by_info.ascending_order.push_back(ctx->K_DESC() == nullptr);
    return 0;
}


Any QueryVisitor::visitOrderByItemAgg(MQL_Parser::OrderByItemAggContext* ctx)
{
    auto var_name = ctx->VARIABLE()->getText();
    var_name.erase(0, 1); // remove leading '?'
    auto var = get_query_ctx().get_or_create_var(var_name);
    std::string agg_inside_var_name = var_name;

    std::unique_ptr<Expr> expr;
    if (ctx->KEY() != nullptr) {
        auto key_name = ctx->KEY()->getText();
        agg_inside_var_name += key_name;
        key_name.erase(0, 1); // remove leading '.'

        auto property_var = get_query_ctx().get_or_create_var(agg_inside_var_name);

        auto key_id = QuadObjectId::get_string(key_name);
        used_var_properties.emplace(var, key_id, property_var);
        expr = std::make_unique<ExprVarProperty>(var, key_id, property_var);
    } else {
        expr = std::make_unique<ExprVar>(var);
    }

    std::string agg_var_name;
    if (ctx->aggregateFunc()->K_AVG()) {
        expr = std::make_unique<ExprAggAvg>(std::move(expr));
        agg_var_name = ".AVG";
    } else if (ctx->aggregateFunc()->K_MAX()) {
        expr = std::make_unique<ExprAggMax>(std::move(expr));
        agg_var_name = ".MAX";
    } else if (ctx->aggregateFunc()->K_MIN()) {
        expr = std::make_unique<ExprAggMin>(std::move(expr));
        agg_var_name = ".MIN";
    } else if (ctx->aggregateFunc()->K_SUM()) {
        expr = std::make_unique<ExprAggSum>(std::move(expr));
        agg_var_name = ".SUM";
    } else {
        throw std::runtime_error("Unmanaged aggregateFunc");
    }
    agg_var_name += "(";
    agg_var_name += agg_inside_var_name;
    agg_var_name += ")";

    order_by_info.items.push_back({ get_query_ctx().get_or_create_var(agg_var_name), std::move(expr) });
    order_by_info.ascending_order.push_back(ctx->K_DESC() == nullptr);
    return 0;
}


Any QueryVisitor::visitOrderByItemCount(MQL_Parser::OrderByItemCountContext* ctx)
{
    bool distinct = ctx->K_DISTINCT() != nullptr;
    if (ctx->VARIABLE() != nullptr) {
        auto var_name = ctx->VARIABLE()->getText();
        var_name.erase(0, 1); // remove leading '?'
        auto var = get_query_ctx().get_or_create_var(var_name);

        if (ctx->KEY() != nullptr) {
            auto key_name = ctx->KEY()->getText();
            auto property_var_name = var_name + key_name;
            key_name.erase(0, 1); // remove leading '.'

            auto property_var = get_query_ctx().get_or_create_var(property_var_name);
            auto key_id = QuadObjectId::get_string(key_name);
            used_var_properties.emplace(var, key_id, property_var);

            std::string expr_var = ".COUNT(";
            expr_var += (distinct ? "DISTINCT " : "");
            expr_var += property_var_name + ")";
            order_by_info.items.push_back({ get_query_ctx().get_or_create_var(expr_var),
                                            std::make_unique<ExprAggCount>(
                                                std::make_unique<ExprVarProperty>(var, key_id, property_var),
                                                distinct
                                            ) });
        } else {
            std::string expr_var = ".COUNT(";
            expr_var += (distinct ? "DISTINCT " : "");
            expr_var += var_name + ")";
            order_by_info.items.push_back(
                { get_query_ctx().get_or_create_var(expr_var),
                  std::make_unique<ExprAggCount>(std::make_unique<ExprVar>(var), distinct) }
            );
        }
    } else {
        std::string expr_var = ".COUNT(";
        expr_var += (distinct ? "DISTINCT " : "");
        expr_var += "*)";

        order_by_info.items.push_back({ get_query_ctx().get_or_create_var(expr_var),
                                        std::make_unique<ExprAggCountAll>(distinct) });
    }
    order_by_info.ascending_order.push_back(ctx->K_DESC() == nullptr);

    return 0;
}


Any QueryVisitor::visitGroupByStatement(MQL_Parser::GroupByStatementContext* ctx)
{
    for (auto group_by_item : ctx->groupByItem()) {
        group_by_item->accept(this);
    }

    return 0;
}


Any QueryVisitor::visitGroupByItem(MQL_Parser::GroupByItemContext* ctx)
{
    auto var_name = ctx->VARIABLE()->getText();
    var_name.erase(0, 1); // remove leading '?'
    auto var = get_query_ctx().get_or_create_var(var_name);

    if (ctx->KEY() != nullptr) {
        auto key_name = ctx->KEY()->getText();
        auto property_var_name = var_name + key_name;
        key_name.erase(0, 1); // remove leading '.'

        auto property_var = get_query_ctx().get_or_create_var(property_var_name);

        auto key_id = QuadObjectId::get_string(key_name);
        used_var_properties.emplace(var, key_id, property_var);
        group_by_vars.push_back(property_var);
    } else {
        group_by_vars.push_back(var);
    }
    return 0;
}


Any QueryVisitor::visitGraphPattern(MQL_Parser::GraphPatternContext* ctx)
{
    if (ctx->basicPattern() != nullptr) {
        ctx->basicPattern()->accept(this);
        auto parent = std::move(current_basic_graph_pattern);
        if (ctx->optionalPattern().size() > 0) {
            std::vector<std::unique_ptr<Op>> optional_children;
            for (auto& opt : ctx->optionalPattern()) {
                opt->accept(this);
                optional_children.push_back(std::move(current_op));
            }
            current_op = std::make_unique<OpOptional>(std::move(parent), std::move(optional_children));
        } else {
            current_op = std::move(parent);
        }
    }

    return 0;
}


Any QueryVisitor::visitBasicPattern(MQL_Parser::BasicPatternContext* ctx)
{
    current_basic_graph_pattern = std::make_unique<OpBasicGraphPattern>();
    visitChildren(ctx);
    for (auto& pending_disjoint_var : possible_disjoint_vars) {
        current_basic_graph_pattern->try_add_possible_disjoint_var(pending_disjoint_var);
    }

    // Properties ANDs queries (property3 and property4)
    if (!property_expr.empty()) {
        if (property_expr.size() == 1) {
            current_expr = std::move(property_expr[0]);
        } else {
            current_expr = std::make_unique<MQL::ExprAnd>(std::move(property_expr));
        }
        property_expr.clear();
    }

    return 0;
}


Any QueryVisitor::visitLinearPattern(MQL_Parser::LinearPatternContext* ctx)
{
    if (ctx->similaritySearch() != nullptr) {
        ctx->similaritySearch()->accept(this);
    } else {
        first_element_disjoint = ctx->children.size() == 1;
        ctx->children[0]->accept(this);
        saved_node = last_node;
        for (size_t i = 2; i < ctx->children.size(); i += 2) {
            ctx->children[i]->accept(this); // accept node
            ctx->children[i - 1]->accept(this); // accept edge or path
            saved_node = last_node;
        }
    }

    return 0;
}


Any QueryVisitor::visitFixedNodeInside(MQL_Parser::FixedNodeInsideContext* ctx)
{
    last_node = QuadObjectId::get_fixed_node_inside(ctx->getText());
    if (first_element_disjoint) {
        current_basic_graph_pattern->add_disjoint_term(last_node.get_OID());
    }
    return 0;
}


Any QueryVisitor::visitProperty1(MQL_Parser::Property1Context* property)
{
    auto key_str = property->identifier()->getText();
    auto key_id = QuadObjectId::get_string(key_str);

    ObjectId value_id;
    if (property->value() == nullptr) {
        if (property->FALSE_PROP() != nullptr) {
            value_id = ObjectId(ObjectId::BOOL_FALSE);
        }
        if (property->TRUE_PROP() != nullptr) {
            value_id = ObjectId(ObjectId::BOOL_TRUE);
        }
    } else if (property->value()->datatypeValue() != nullptr) {
        auto dtt_value = property->value()->datatypeValue();
        std::string datatype = dtt_value->identifier()->getText();
        std::string value = dtt_value->STRING()->getText();
        // remove surrounding double quotes
        value = value.substr(1, value.size() - 2);

        if (datatype == "date") {
            value_id = ObjectId(DateTime::from_date(value));
            if (value_id.is_null()) {
                throw QueryException("Invalid date value: " + value);
            }
        } else if (datatype == "dateTime") {
            value_id = ObjectId(DateTime::from_dateTime(value));
            if (value_id.is_null()) {
                throw QueryException("Invalid dateTime value: " + value);
            }
        } else if (datatype == "dateTimeStamp") {
            value_id = ObjectId(DateTime::from_dateTimeStamp(value));
            if (value_id.is_null()) {
                throw QueryException("Invalid dateTimeStamp value: " + value);
            }
        } else if (datatype == "time") {
            value_id = ObjectId(DateTime::from_time(value));
            if (value_id.is_null()) {
                throw QueryException("Invalid time value: " + value);
            }
        } else {
            throw QueryException("Unrecognized datatype: " + datatype);
        }
    } else {
        std::string value = property->value()->getText();
        value_id = QuadObjectId::get_value(value);
    }
    OpProperty op_property(saved_property_obj, key_id, value_id);
    match_var_properties.insert(op_property);
    current_basic_graph_pattern->add_property(op_property);
    return 0;
}

Any QueryVisitor::visitProperty2(MQL_Parser::Property2Context* property)
{
    auto key_str = property->identifier()->getText();
    auto key_id = QuadObjectId::get_string(key_str);

    ObjectId value_id;

    std::string datatype = property->TYPE()->getText();
    // remove leading ':'
    datatype.erase(0, 1);

    std::string value = property->STRING()->getText();
    // remove surrounding double quotes
    value = value.substr(1, value.size() - 2);

    if (datatype == "date") {
        value_id = ObjectId(DateTime::from_date(value));
        if (value_id.is_null()) {
            throw QueryException("Invalid date value: " + value);
        }
    } else if (datatype == "dateTime") {
        value_id = ObjectId(DateTime::from_dateTime(value));
        if (value_id.is_null()) {
            throw QueryException("Invalid dateTime value: " + value);
        }
    } else if (datatype == "dateTimeStamp") {
        value_id = ObjectId(DateTime::from_dateTimeStamp(value));
        if (value_id.is_null()) {
            throw QueryException("Invalid dateTimeStamp value: " + value);
        }
    } else if (datatype == "time") {
        value_id = ObjectId(DateTime::from_time(value));
        if (value_id.is_null()) {
            throw QueryException("Invalid time value: " + value);
        }
    } else {
        throw QueryException("Unrecognized datatype: " + datatype);
    }

    OpProperty op_property(saved_property_obj, key_id, value_id);
    match_var_properties.insert(op_property);
    current_basic_graph_pattern->add_property(op_property);
    return 0;
}

Any QueryVisitor::visitProperty3(MQL_Parser::Property3Context* property)
{ // (?x :Person {name IS STRING})
    auto key_str = property->identifier()->getText();
    auto key_id = QuadObjectId::get_string(key_str);

    assert(saved_property_obj.is_var());
    auto var_name = get_query_ctx().get_var_name(saved_property_obj.get_var());
    auto property_var = get_query_ctx().get_or_create_var(var_name + '.' + key_str);

    uint64_t propertyTypeBitmap = 0;
    std::vector<std::unique_ptr<Expr>> datatypes_is_exprs;
    bool negation = property->K_NOT() != nullptr;


    for (std::size_t i = 0; i < property->conditionalOrType().size() + 1; i++) {
        MQL_Parser::ExprTypenameContext* data_type =
            (i == 0) ? property->exprTypename() : property->conditionalOrType()[i - 1]->exprTypename();

        auto expr_var_property =
            std::make_unique<MQL::ExprVarProperty>(saved_property_obj.get_var(), key_id, property_var);
        ExprIs::TypeName type = ExprIs::TypeName::NULL_; // assign to avoid warning

        if (data_type->K_BOOL()) {
            type = ExprIs::TypeName::BOOL;
            propertyTypeBitmap = propertyTypeBitmap | static_cast<uint64_t>(PropertyType::TYPE_BOOL);
        } else if (data_type->K_FLOAT()) {
            type = ExprIs::TypeName::FLOAT;
            propertyTypeBitmap = propertyTypeBitmap | static_cast<uint64_t>(PropertyType::TYPE_FLOAT);
        } else if (data_type->K_INTEGER()) {
            type = ExprIs::TypeName::INTEGER;
            propertyTypeBitmap = propertyTypeBitmap | static_cast<uint64_t>(PropertyType::TYPE_INTEGER);
        } else if (data_type->K_NULL()) {
            propertyTypeBitmap = propertyTypeBitmap | static_cast<uint64_t>(PropertyType::TYPE_NULL);
            type = ExprIs::TypeName::NULL_;
        } else if (data_type->K_STRING()) {
            type = ExprIs::TypeName::STRING;
            propertyTypeBitmap = propertyTypeBitmap | static_cast<uint64_t>(PropertyType::TYPE_STRING);
        }

        if (negation) {
            propertyTypeBitmap = ~propertyTypeBitmap;
        }
        datatypes_is_exprs.push_back(
            std::make_unique<MQL::ExprIs>(negation, std::move(expr_var_property), type, propertyTypeBitmap)
        );
    }
    property_expr.push_back(std::make_unique<MQL::ExprOr>(std::move(datatypes_is_exprs)));
    used_var_properties.emplace(saved_property_obj.get_var(), key_id, property_var);
    return 0;
}

Any QueryVisitor::visitProperty4(MQL_Parser::Property4Context* property)
{ // (?x :Person {value > 5})
    auto key_str = property->identifier()->getText();
    auto key_id = QuadObjectId::get_string(key_str);

    auto var_name = get_query_ctx().get_var_name(saved_property_obj.get_var());
    auto property_var = get_query_ctx().get_or_create_var(var_name + "." + key_str);

    auto op = property->op->getText();
    auto expr_var_property =
        std::make_unique<MQL::ExprVarProperty>(saved_property_obj.get_var(), key_id, property_var);

    ObjectId oid;
    if (property->value()->datatypeValue() != nullptr) {
        auto dtt_value = property->value()->datatypeValue();
        std::string datatype = dtt_value->identifier()->getText();
        std::string value = dtt_value->STRING()->getText();
        // remove surrounding double quotes
        value = value.substr(1, value.size() - 2);

        if (datatype == "date") {
            oid = ObjectId(DateTime::from_date(value));
            if (oid.is_null()) {
                throw QueryException("Invalid date value: " + value);
            }
        } else if (datatype == "dateTime") {
            oid = ObjectId(DateTime::from_dateTime(value));
            if (oid.is_null()) {
                throw QueryException("Invalid dateTime value: " + value);
            }
        } else if (datatype == "dateTimeStamp") {
            oid = ObjectId(DateTime::from_dateTimeStamp(value));
            if (oid.is_null()) {
                throw QueryException("Invalid dateTimeStamp value: " + value);
            }
        } else if (datatype == "time") {
            oid = ObjectId(DateTime::from_time(value));
            if (oid.is_null()) {
                throw QueryException("Invalid time value: " + value);
            }
        } else {
            throw QueryException("Unrecognized datatype: " + datatype);
        }
    } else {
        oid = QuadObjectId::get_value(property->value()->getText());
    }

    auto value_constant = std::make_unique<ExprConstant>(oid);
    used_var_properties.emplace(saved_property_obj.get_var(), key_id, property_var);
    std::tuple<VarId, ObjectId, ObjectId, ObjectId>
        property_operation_tuple(saved_property_obj.get_var(), key_id, oid, QuadObjectId::get_value(op));

    if (op == "==") {
        property_expr.push_back(
            std::make_unique<ExprEquals>(std::move(expr_var_property), std::move(value_constant))
        );
    } else if (op == "!=") {
        property_expr.push_back(
            std::make_unique<ExprNotEquals>(std::move(expr_var_property), std::move(value_constant))
        );
    } else if (op == "<") {
        property_expr.push_back(
            std::make_unique<ExprLess>(std::move(expr_var_property), std::move(value_constant))
        );
    } else if (op == "<=") {
        property_expr.push_back(
            std::make_unique<ExprLessOrEquals>(std::move(expr_var_property), std::move(value_constant))
        );
    } else if (op == ">=") {
        property_expr.push_back(
            std::make_unique<ExprGreaterOrEquals>(std::move(expr_var_property), std::move(value_constant))
        );
    } else if (op == ">") {
        property_expr.push_back(
            std::make_unique<ExprGreater>(std::move(expr_var_property), std::move(value_constant))
        );
    } else {
        throw std::invalid_argument(op + " not recognized as a valid ComparisonExpr operator");
    }
    return 0;
}

Any QueryVisitor::visitVarNode(MQL_Parser::VarNodeContext* ctx)
{
    std::string var_name;
    VarId var(0); // Will be overwritten

    // process var
    if (ctx->VARIABLE() != nullptr) {
        var_name = ctx->VARIABLE()->getText();
        var_name.erase(0, 1); // remove leading '?'
        var = get_query_ctx().get_or_create_var(var_name);
    } else {
        var = get_query_ctx().get_internal_var();
    }

    // Process Labels
    for (auto& label : ctx->TYPE()) {
        auto label_str = label->getText();
        label_str.erase(0, 1); // remove leading ':'
        auto label_id = QuadObjectId::get_string(label_str);
        current_basic_graph_pattern->add_label(OpLabel(var, label_id));
    }

    // Process Properties
    auto properties = ctx->properties();
    if (properties != nullptr) {
        saved_property_obj = var;
        for (auto property : properties->property()) {
            property->accept(this);
        }
    }

    if (first_element_disjoint && ctx->TYPE().empty() && current_basic_graph_pattern->properties.empty()) {
        current_basic_graph_pattern->add_disjoint_var(OpDisjointVar(var));
    }

    last_node = var;
    return 0;
}


Any QueryVisitor::visitEdge(MQL_Parser::EdgeContext* ctx)
{
    if (ctx->edgeInside() == nullptr) {
        saved_edge = get_query_ctx().get_internal_var();
        saved_type = get_query_ctx().get_internal_var();
    }
    visitChildren(ctx);
    if (ctx->GT() != nullptr) {
        // right direction
        current_basic_graph_pattern->add_edge(OpEdge(saved_node, last_node, saved_type, saved_edge));
    } else {
        // left direction
        current_basic_graph_pattern->add_edge(OpEdge(last_node, saved_node, saved_type, saved_edge));
    }
    return 0;
}


Any QueryVisitor::visitEdgeInside(MQL_Parser::EdgeInsideContext* ctx)
{
    if (ctx->VARIABLE() != nullptr) {
        auto edge_var_name = ctx->VARIABLE()->getText();
        edge_var_name.erase(0, 1); // remove leading '?'
        saved_edge = get_query_ctx().get_or_create_var(edge_var_name);
    } else if (ctx->EDGE_ID()) {
        saved_edge = QuadObjectId::get_edge(ctx->EDGE_ID()->getText());
    } else {
        saved_edge = get_query_ctx().get_internal_var();
    }

    if (ctx->TYPE_VAR() != nullptr) {
        auto type_var_name = ctx->TYPE_VAR()->getText();
        type_var_name.erase(0, 2); // remove leading ':?'
        auto type_var = get_query_ctx().get_or_create_var(type_var_name);
        saved_type = type_var;
    } else if (ctx->TYPE() != nullptr) {
        auto type_str = ctx->TYPE()->getText();
        type_str.erase(0, 1); // remove leading ':'
        saved_type = QuadObjectId::get_named_node(type_str);
    } else {
        auto type_var = get_query_ctx().get_internal_var();
        saved_type = type_var;
    }

    auto properties = ctx->properties();
    if (properties != nullptr) {
        saved_property_obj = saved_edge;
        for (auto property : properties->property()) {
            property->accept(this);
        }
    }
    return 0;
}


Any QueryVisitor::visitPath(MQL_Parser::PathContext* ctx)
{
    VarId path_var(0); // Will be overwritten
    if (ctx->VARIABLE() == nullptr) {
        path_var = get_query_ctx().get_internal_var();
    } else {
        auto path_var_name = ctx->VARIABLE()->getText();
        path_var_name.erase(0, 1); // remove leading '?'
        path_var = get_query_ctx().get_or_create_var(path_var_name);
    }

    PathSemantic semantic = PathSemantic::DEFAULT;
    if (ctx->pathType() != nullptr) {
        if (ctx->pathType()->K_ALL()) {
            if (ctx->pathType()->K_SHORTEST()) {
                if (ctx->pathType()->K_ACYCLIC()) {
                    semantic = PathSemantic::ALL_SHORTEST_ACYCLIC;
                } else if (ctx->pathType()->K_SIMPLE()) {
                    semantic = PathSemantic::ALL_SHORTEST_SIMPLE;
                } else if (ctx->pathType()->K_TRAILS()) {
                    semantic = PathSemantic::ALL_SHORTEST_TRAILS;
                } else { // WALKS by default
                    semantic = PathSemantic::ALL_SHORTEST_WALKS;
                }
            } else {
                if (ctx->pathType()->K_ACYCLIC()) {
                    semantic = PathSemantic::ALL_ACYCLIC;
                } else if (ctx->pathType()->K_SIMPLE()) {
                    semantic = PathSemantic::ALL_SIMPLE;
                } else if (ctx->pathType()->K_TRAILS()) {
                    semantic = PathSemantic::ALL_TRAILS;
                } else { // WALKS by default
                    throw QueryException("ALL WALKS path semantic not allowed");
                }
            }
        } else { // K_ANY
            if (ctx->pathType()->K_SHORTEST()) {
                if (ctx->pathType()->K_ACYCLIC()) {
                    semantic = PathSemantic::ANY_SHORTEST_ACYCLIC;
                } else if (ctx->pathType()->K_SIMPLE()) {
                    semantic = PathSemantic::ANY_SHORTEST_SIMPLE;
                } else if (ctx->pathType()->K_TRAILS()) {
                    semantic = PathSemantic::ANY_SHORTEST_TRAILS;
                } else { // WALKS by default
                    semantic = PathSemantic::ANY_SHORTEST_WALKS;
                }
            } else {
                if (ctx->pathType()->K_ACYCLIC()) {
                    semantic = PathSemantic::ANY_ACYCLIC;
                } else if (ctx->pathType()->K_SIMPLE()) {
                    semantic = PathSemantic::ANY_SIMPLE;
                } else if (ctx->pathType()->K_TRAILS()) {
                    semantic = PathSemantic::ANY_TRAILS;
                } else { // WALKS by default
                    semantic = PathSemantic::ANY_WALKS;
                }
            }
        }
    }

    current_path_inverse = false;
    ctx->pathAlternatives()->accept(this);
    if (ctx->GT() != nullptr) {
        // right direction
        current_basic_graph_pattern->add_path(OpPath(
            path_var,
            saved_node,
            last_node,
            semantic,
            OpPath::Direction::LEFT_TO_RIGHT,
            std::move(current_path)
        ));
    } else {
        // left direction
        current_basic_graph_pattern->add_path(OpPath(
            path_var,
            last_node,
            saved_node,
            semantic,
            OpPath::Direction::RIGHT_TO_LEFT,
            std::move(current_path)
        ));
    }
    return 0;
}


Any QueryVisitor::visitPathAlternatives(MQL_Parser::PathAlternativesContext* ctx)
{
    if (ctx->pathSequence().size() > 1) {
        std::vector<std::unique_ptr<RegularPathExpr>> alternatives;
        for (auto s : ctx->pathSequence()) {
            s->accept(this);
            alternatives.push_back(std::move(current_path));
        }
        current_path = std::make_unique<PathAlternatives>(std::move(alternatives));
    } else {
        ctx->pathSequence()[0]->accept(this);
    }
    return 0;
}


Any QueryVisitor::visitPathSequence(MQL_Parser::PathSequenceContext* ctx)
{
    if (ctx->pathAtom().size() > 1) {
        std::vector<std::unique_ptr<RegularPathExpr>> sequence;
        if (current_path_inverse) {
            for (int i = ctx->pathAtom().size() - 1; i >= 0; i--) {
                ctx->pathAtom()[i]->accept(this);
                sequence.push_back(std::move(current_path));
            }
        } else {
            for (auto a : ctx->pathAtom()) {
                a->accept(this);
                sequence.push_back(std::move(current_path));
            }
        }
        current_path = std::make_unique<PathSequence>(std::move(sequence));
    } else {
        ctx->pathAtom()[0]->accept(this);
    }
    return 0;
}


Any QueryVisitor::visitPathAtomSimple(MQL_Parser::PathAtomSimpleContext* ctx)
{
    std::string type = ctx->TYPE()->getText();
    type.erase(0, 1); // remove ':'

    bool inverse = (ctx->children[0]->getText() == "^") ^ current_path_inverse;

    current_path = std::make_unique<PathAtom>(type, inverse);

    auto suffix = ctx->pathSuffix();
    if (suffix == nullptr) {
        // no suffix
        return 0;
    } else if (suffix->op == nullptr) {
        // {min, max}
        std::vector<std::unique_ptr<RegularPathExpr>> seq;
        unsigned min = std::stoul(suffix->min->getText());
        unsigned max = std::stoul(suffix->max->getText());
        unsigned i = 0;
        for (; i < min; i++) {
            seq.push_back(current_path->clone());
        }
        for (; i < max; i++) {
            seq.push_back(std::make_unique<PathOptional>(current_path->clone()));
        }
        current_path = std::make_unique<PathSequence>(std::move(seq));
    } else if (suffix->op->getText() == "*") {
        current_path = std::make_unique<PathKleeneStar>(std::move(current_path));
    } else if (suffix->op->getText() == "+") {
        current_path = std::make_unique<PathKleenePlus>(std::move(current_path));
    } else if (suffix->op->getText() == "?") {
        if (!current_path->nullable()) {
            current_path = std::make_unique<PathOptional>(std::move(current_path));
        }
        // else we avoid a redundant PathOptional, current_path stays the same
    }
    return 0;
}


Any QueryVisitor::visitPathAtomAlternatives(MQL_Parser::PathAtomAlternativesContext* ctx)
{
    auto previous_current_path_inverse = current_path_inverse;

    current_path_inverse = (ctx->children[0]->getText() == "^") ^ current_path_inverse;
    ctx->pathAlternatives()->accept(this);
    current_path_inverse = previous_current_path_inverse;

    auto suffix = ctx->pathSuffix();
    if (suffix == nullptr) {
        // no suffix
    } else if (suffix->op == nullptr) {
        // {min, max}
        std::vector<std::unique_ptr<RegularPathExpr>> seq;
        unsigned min = std::stoul(suffix->min->getText());
        unsigned max = std::stoul(suffix->max->getText());
        unsigned i = 0;
        for (; i < min; i++) {
            seq.push_back(current_path->clone());
        }
        for (; i < max; i++) {
            seq.push_back(std::make_unique<PathOptional>(current_path->clone()));
        }
        current_path = std::make_unique<PathSequence>(std::move(seq));
    } else if (suffix->op->getText() == "*") {
        current_path = std::make_unique<PathKleeneStar>(std::move(current_path));
    } else if (suffix->op->getText() == "+") {
        current_path = std::make_unique<PathKleenePlus>(std::move(current_path));
    } else if (suffix->op->getText() == "?") {
        if (!current_path->nullable()) {
            current_path = std::make_unique<PathOptional>(std::move(current_path));
        }
        // else we avoid a redundant PathOptional, current_path stays the same
    }
    return 0;
}


Any QueryVisitor::visitWhereStatement(MQL_Parser::WhereStatementContext* ctx)
{
    auto current_old_expr = std::move(current_expr);
    ctx->conditionalOrExpr()->accept(this);
    if (current_old_expr != nullptr) {
        std::vector<std::unique_ptr<Expr>> and_list;
        and_list.push_back(std::move(current_expr));
        and_list.push_back(std::move(current_old_expr));
        current_expr = std::make_unique<ExprAnd>(std::move(and_list));
    }
    return 0;
}


Any QueryVisitor::visitExprVar(MQL_Parser::ExprVarContext* ctx)
{
    auto var_name = ctx->VARIABLE()->getText();
    var_name.erase(0, 1); // remove leading '?'
    auto var = get_query_ctx().get_or_create_var(var_name);

    if (ctx->KEY() != nullptr) {
        auto key_name = ctx->KEY()->getText();
        auto property_var_name = var_name + key_name;
        key_name.erase(0, 1); // remove leading '.'

        auto property_var = get_query_ctx().get_or_create_var(property_var_name);
        auto key_id = QuadObjectId::get_string(key_name);

        current_expr = std::make_unique<ExprVarProperty>(var, key_id, property_var);
        used_var_properties.emplace(var, key_id, property_var);
    } else {
        current_expr = std::make_unique<ExprVar>(var);
    }
    return 0;
}


Any QueryVisitor::visitExprValue(MQL_Parser::ExprValueContext* ctx)
{
    if (ctx->value()->datatypeValue()) {
        auto dtt_value = ctx->value()->datatypeValue();
        std::string datatype = dtt_value->identifier()->getText();
        std::string value = dtt_value->STRING()->getText();
        // remove surrounding double quotes
        value = value.substr(1, value.size() - 2);
        ObjectId datetime_id;
        if (datatype == "date") {
            datetime_id = ObjectId(DateTime::from_date(value));
            if (datetime_id.is_null()) {
                throw QueryException("Invalid date value: " + value);
            }
        } else if (datatype == "dateTime") {
            datetime_id = ObjectId(DateTime::from_dateTime(value));
            if (datetime_id.is_null()) {
                throw QueryException("Invalid dateTime value: " + value);
            }
        } else if (datatype == "dateTimeStamp") {
            datetime_id = ObjectId(DateTime::from_dateTimeStamp(value));
            if (datetime_id.is_null()) {
                throw QueryException("Invalid dateTimeStamp value: " + value);
            }
        } else if (datatype == "time") {
            datetime_id = ObjectId(DateTime::from_time(value));
            if (datetime_id.is_null()) {
                throw QueryException("Invalid time value: " + value);
            }
        } else {
            throw QueryException("Unrecognized datatype: " + datatype);
        }
        current_expr = std::make_unique<ExprConstant>(datetime_id);
    } else {
        auto oid = QuadObjectId::get_value(ctx->getText());
        current_expr = std::make_unique<ExprConstant>(oid);
    }
    return 0;
}


Any QueryVisitor::visitConditionalOrExpr(MQL_Parser::ConditionalOrExprContext* ctx)
{
    ctx->conditionalAndExpr()[0]->accept(this);

    if (ctx->conditionalAndExpr().size() > 1) {
        std::vector<std::unique_ptr<Expr>> or_list;
        or_list.push_back(std::move(current_expr));
        for (std::size_t i = 1; i < ctx->conditionalAndExpr().size(); i++) {
            ctx->conditionalAndExpr()[i]->accept(this);
            or_list.push_back(std::move(current_expr));
        }
        current_expr = std::make_unique<ExprOr>(std::move(or_list));
    }
    return 0;
}


Any QueryVisitor::visitConditionalAndExpr(MQL_Parser::ConditionalAndExprContext* ctx)
{
    ctx->comparisonExpr()[0]->accept(this);

    if (ctx->comparisonExpr().size() > 1) {
        std::vector<std::unique_ptr<Expr>> and_list;
        and_list.push_back(std::move(current_expr));
        for (std::size_t i = 1; i < ctx->comparisonExpr().size(); i++) {
            ctx->comparisonExpr()[i]->accept(this);
            and_list.push_back(std::move(current_expr));
        }
        current_expr = std::make_unique<ExprAnd>(std::move(and_list));
    }
    return 0;
}


Any QueryVisitor::visitComparisonExprOp(MQL_Parser::ComparisonExprOpContext* ctx)
{
    ctx->additiveExpr()[0]->accept(this);
    if (ctx->additiveExpr().size() > 1) {
        auto saved_lhs = std::move(current_expr);
        ctx->additiveExpr()[1]->accept(this);
        auto op = ctx->op->getText();
        if (op == "==") {
            current_expr = std::make_unique<ExprEquals>(std::move(saved_lhs), std::move(current_expr));
        } else if (op == "!=") {
            current_expr = std::make_unique<ExprNotEquals>(std::move(saved_lhs), std::move(current_expr));
        } else if (op == "<") {
            current_expr = std::make_unique<ExprLess>(std::move(saved_lhs), std::move(current_expr));
        } else if (op == "<=") {
            current_expr = std::make_unique<ExprLessOrEquals>(std::move(saved_lhs), std::move(current_expr));
        } else if (op == ">=") {
            current_expr =
                std::make_unique<ExprGreaterOrEquals>(std::move(saved_lhs), std::move(current_expr));
        } else if (op == ">") {
            current_expr = std::make_unique<ExprGreater>(std::move(saved_lhs), std::move(current_expr));
        } else {
            throw std::invalid_argument(op + " not recognized as a valid ComparisonExpr operator");
        }
    }
    return 0;
}


Any QueryVisitor::visitComparisonExprIs(MQL_Parser::ComparisonExprIsContext* ctx)
{
    ctx->additiveExpr()->accept(this);
    ExprIs::TypeName type;

    uint64_t propertyTypeBitmap = 0;
    if (ctx->exprTypename()->K_BOOL()) {
        type = ExprIs::TypeName::BOOL;
        propertyTypeBitmap = propertyTypeBitmap | static_cast<uint64_t>(PropertyType::TYPE_BOOL);
    } else if (ctx->exprTypename()->K_FLOAT()) {
        type = ExprIs::TypeName::FLOAT;
        propertyTypeBitmap = propertyTypeBitmap | static_cast<uint64_t>(PropertyType::TYPE_FLOAT);
    } else if (ctx->exprTypename()->K_INTEGER()) {
        type = ExprIs::TypeName::INTEGER;
        propertyTypeBitmap = propertyTypeBitmap | static_cast<uint64_t>(PropertyType::TYPE_INTEGER);
    } else if (ctx->exprTypename()->K_NULL()) {
        type = ExprIs::TypeName::NULL_;
        propertyTypeBitmap = propertyTypeBitmap | static_cast<uint64_t>(PropertyType::TYPE_NULL);
    } else if (ctx->exprTypename()->K_STRING()) {
        type = ExprIs::TypeName::STRING;
        propertyTypeBitmap = propertyTypeBitmap | static_cast<uint64_t>(PropertyType::TYPE_STRING);
    } else {
        throw std::invalid_argument(
            "exprTypename '" + ctx->exprTypename()->toString() + "' not recognized in ComparisonExprIs"
        );
    }

    if (ctx->K_NOT() != nullptr) {
        propertyTypeBitmap = ~propertyTypeBitmap;
    }
    current_expr =
        std::make_unique<ExprIs>(ctx->K_NOT() != nullptr, std::move(current_expr), type, propertyTypeBitmap);

    return 0;
}


Any QueryVisitor::visitAdditiveExpr(MQL_Parser::AdditiveExprContext* ctx)
{
    auto multiplicativeExprs = ctx->multiplicativeExpr();
    multiplicativeExprs[0]->accept(this);
    for (std::size_t i = 1; i < multiplicativeExprs.size(); i++) {
        auto saved_lhs = std::move(current_expr);
        multiplicativeExprs[i]->accept(this);
        auto op = ctx->op[i - 1]->getText();
        if (op == "+") {
            current_expr = std::make_unique<ExprAddition>(std::move(saved_lhs), std::move(current_expr));
        } else if (op == "-") {
            current_expr = std::make_unique<ExprSubtraction>(std::move(saved_lhs), std::move(current_expr));
        } else {
            throw std::invalid_argument(op + " not recognized as a valid AdditiveExpr operator");
        }
    }
    return 0;
}


Any QueryVisitor::visitMultiplicativeExpr(MQL_Parser::MultiplicativeExprContext* ctx)
{
    auto unaryExprs = ctx->unaryExpr();
    unaryExprs[0]->accept(this);
    for (std::size_t i = 1; i < unaryExprs.size(); i++) {
        auto saved_lhs = std::move(current_expr);
        unaryExprs[i]->accept(this);
        auto op = ctx->op[i - 1]->getText();
        if (op == "*") {
            current_expr =
                std::make_unique<ExprMultiplication>(std::move(saved_lhs), std::move(current_expr));
        } else if (op == "/") {
            current_expr = std::make_unique<ExprDivision>(std::move(saved_lhs), std::move(current_expr));
        } else if (op == "|") {
            current_expr = std::make_unique<ExprModulo>(std::move(saved_lhs), std::move(current_expr));
        } else {
            throw std::invalid_argument(op + " not recognized as a valid MultiplicativeExpr operator");
        }
    }
    return 0;
}


Any QueryVisitor::visitUnaryExpr(MQL_Parser::UnaryExprContext* ctx)
{
    if (ctx->unaryExpr() != nullptr) {
        ctx->unaryExpr()->accept(this);
        if (ctx->K_NOT() != nullptr) {
            current_expr = std::make_unique<ExprNot>(std::move(current_expr));
        } else if (ctx->children[0]->getText() == "+") {
            current_expr = std::make_unique<ExprUnaryPlus>(std::move(current_expr));
        } else if (ctx->children[0]->getText() == "-") {
            current_expr = std::make_unique<ExprUnaryMinus>(std::move(current_expr));
        } else {
            throw std::invalid_argument(
                ctx->children[0]->getText() + " not recognized as a valid UnaryExpr operator"
            );
        }
    } else {
        ctx->atomicExpr()->accept(this);
    }
    return 0;
}


Any QueryVisitor::visitFunction(MQL_Parser::FunctionContext* ctx)
{
    visitChildren(ctx);

    return 0;
}


Any QueryVisitor::visitRegex(MQL_Parser::RegexContext* ctx)
{
    visit(ctx->conditionalOrExpr(0));
    auto expr0 = std::move(current_expr);

    visit(ctx->conditionalOrExpr(1));
    auto expr1 = std::move(current_expr);

    if (ctx->conditionalOrExpr().size() == 3) {
        visit(ctx->conditionalOrExpr(2));
        auto expr2 = std::move(current_expr);
        current_expr = std::make_unique<ExprRegex>(std::move(expr0), std::move(expr1), std::move(expr2));
    } else {
        current_expr = std::make_unique<ExprRegex>(std::move(expr0), std::move(expr1));
    }

    return 0;
}


Any QueryVisitor::visitSimilaritySearch(MQL_Parser::SimilaritySearchContext* ctx)
{
    auto tensor_store_name = ctx->STRING()->getText();
    // remove surrounding double quotes
    tensor_store_name = tensor_store_name.substr(1, tensor_store_name.size() - 2);

    // Set variables
    auto object_var_name = ctx->VARIABLE(0)->getText();
    object_var_name.erase(0, 1); // remove leading '?'
    auto object_var = get_query_ctx().get_or_create_var(object_var_name);

    auto similarity_var_name = ctx->VARIABLE(1)->getText();
    similarity_var_name.erase(0, 1); // remove leading '?'
    auto similarity_var = get_query_ctx().get_or_create_var(similarity_var_name);

    // Set the maximum number of neighbors. Negative means no limit
    int64_t k = std::stoll(ctx->UNSIGNED_INTEGER(0)->getText());
    if (k == 0)
        throw std::invalid_argument("k cannot be zero");
    if (ctx->MINUS() != nullptr)
        k = -k;

    // Set the search_k. Negative means no limit
    int64_t search_k;
    if (ctx->UNSIGNED_INTEGER(1)) {
        // Check that search_k is a valid value zero
        search_k = std::stoll(ctx->UNSIGNED_INTEGER(1)->getText());
        if (search_k == 0) {
            throw std::invalid_argument("search_k cannot be zero");
        }

        if (k < 0) {
            throw std::invalid_argument("k cannot be negative when search_k is specified");
        }
    } else {
        // The user did not specify a search_k, we identify the default value as -1
        search_k = -1;
    }

    if (ctx->fixedNodeInside() != nullptr) {
        // Get tensor from tensor store with the given object
        auto oid = QuadObjectId::get_fixed_node_inside(ctx->fixedNodeInside()->getText());
        current_basic_graph_pattern->add_similarity_search(OpSimilaritySearch(
            object_var,
            similarity_var,
            std::move(tensor_store_name),
            k,
            search_k,
            std::vector<float> {},
            oid
        ));
    } else {
        // Get tensor from parser, no object involved
        visit(ctx->tensor());
        current_basic_graph_pattern->add_similarity_search(OpSimilaritySearch(
            object_var,
            similarity_var,
            std::move(tensor_store_name),
            k,
            search_k,
            std::move(current_tensor),
            ObjectId::get_null()
        ));
    }

    return 0;
}


antlrcpp::Any QueryVisitor::visitProjectSimilarity(MQL_Parser::ProjectSimilarityContext* ctx)
{
    auto tensor_store_name = ctx->STRING()->getText();
    // remove surrounding double quotes
    tensor_store_name = tensor_store_name.substr(1, tensor_store_name.size() - 2);

    // Set variables
    auto object_var_name = ctx->VARIABLE(0)->getText();
    object_var_name.erase(0, 1); // remove leading '?'
    auto object_var = get_query_ctx().get_or_create_var(object_var_name);

    auto similarity_var_name = ctx->VARIABLE(1)->getText();
    similarity_var_name.erase(0, 1); // remove leading '?'
    auto similarity_var = get_query_ctx().get_or_create_var(similarity_var_name);

    project_similarity_info.object_var = object_var;
    project_similarity_info.similarity_var = similarity_var;
    project_similarity_info.tensor_store_name = std::move(tensor_store_name);
    if (ctx->fixedNodeInside() != nullptr) {
        // Get tensor from tensor store with the given object
        const auto oid = QuadObjectId::get_fixed_node_inside(ctx->fixedNodeInside()->getText());
        project_similarity_info.query_tensor = std::vector<float> {};
        project_similarity_info.query_object = oid;
    } else {
        // Get tensor from parser, no object involved
        visit(ctx->tensor());
        project_similarity_info.query_tensor = std::move(current_tensor);
        project_similarity_info.query_object = ObjectId::get_null();
    }
    auto metricType = ctx->metricType();
    if (metricType->K_ANGULAR()) {
        project_similarity_info.metric_type = LSH::MetricType::ANGULAR;
    } else if (metricType->K_EUCLIDEAN()) {
        project_similarity_info.metric_type = LSH::MetricType::EUCLIDEAN;
    } else { // metricType->K_MANHATTAN()
        project_similarity_info.metric_type = LSH::MetricType::MANHATTAN;
    }

    return 0;
}


std::any QueryVisitor::visitBruteSimilaritySearch(MQL_Parser::BruteSimilaritySearchContext* ctx)
{
    auto tensor_store_name = ctx->STRING()->getText();
    // remove surrounding double quotes
    tensor_store_name = tensor_store_name.substr(1, tensor_store_name.size() - 2);

    // Set variables
    auto object_var_name = ctx->VARIABLE(0)->getText();
    object_var_name.erase(0, 1); // remove leading '?'
    auto object_var = get_query_ctx().get_or_create_var(object_var_name);

    auto similarity_var_name = ctx->VARIABLE(1)->getText();
    similarity_var_name.erase(0, 1); // remove leading '?'
    auto similarity_var = get_query_ctx().get_or_create_var(similarity_var_name);

    // Set the maximum number of neighbors
    brute_similarity_search_info.k = std::stoll(ctx->UNSIGNED_INTEGER()->getText());
    brute_similarity_search_info.object_var = object_var;
    brute_similarity_search_info.similarity_var = similarity_var;
    brute_similarity_search_info.tensor_store_name = std::move(tensor_store_name);
    if (ctx->fixedNodeInside() != nullptr) {
        // Get tensor from tensor store with the given object
        const auto oid = QuadObjectId::get_fixed_node_inside(ctx->fixedNodeInside()->getText());
        brute_similarity_search_info.query_tensor = std::vector<float> {};
        brute_similarity_search_info.query_object = oid;
    } else {
        // Get tensor from parser, no object involved
        visit(ctx->tensor());
        brute_similarity_search_info.query_tensor = std::move(current_tensor);
        brute_similarity_search_info.query_object = ObjectId::get_null();
    }

    const auto metricType = ctx->metricType();
    if (metricType->K_ANGULAR()) {
        brute_similarity_search_info.metric_type = LSH::MetricType::ANGULAR;
    } else if (metricType->K_EUCLIDEAN()) {
        brute_similarity_search_info.metric_type = LSH::MetricType::EUCLIDEAN;
    } else { // metricType->K_MANHATTAN()
        brute_similarity_search_info.metric_type = LSH::MetricType::MANHATTAN;
    }

    return 0;
}


antlrcpp::Any QueryVisitor::visitTensor(MQL_Parser::TensorContext* ctx)
{
    const auto numericValues = ctx->numericValue();
    const auto size = numericValues.size();

    current_tensor.resize(size);

    for (auto i = 0u; i < size; ++i) {
        current_tensor[i] = std::stof(numericValues[i]->getText(), nullptr);
    }

    assert(!current_tensor.empty());

    return 0;
}

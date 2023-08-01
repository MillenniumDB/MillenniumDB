#include "query_visitor.h"

#include <cassert>

#include "graph_models/quad_model/quad_object_id.h"
#include "query/parser/expr/mql_exprs.h"
#include "query/parser/paths/path_alternatives.h"
#include "query/parser/paths/path_atom.h"
#include "query/parser/paths/path_kleene_star.h"
#include "query/parser/paths/path_optional.h"
#include "query/parser/paths/path_sequence.h"
#include "query/query_context.h"

using namespace MQL;
using antlrcpp::Any;

Any QueryVisitor::visitDescribeQuery(MQL_Parser::DescribeQueryContext* ctx) {
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

    uint64_t labels_limit     = 0;
    uint64_t properties_limit = 0;
    uint64_t outgoing_limit   = 0;
    uint64_t incoming_limit   = 0;

    bool labels_limit_seen     = false;
    bool properties_limit_seen = false;
    bool outgoing_limit_seen   = false;
    bool incoming_limit_seen   = false;

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


Any QueryVisitor::visitInsertQuery(MQL_Parser::InsertQueryContext* ctx) {
    visitChildren(ctx);
    current_op = std::make_unique<OpInsert>(
        std::move(insert_labels),
        std::move(insert_properties),
        std::move(insert_edges)
    );
    return 0;
}


Any QueryVisitor::visitMatchQuery(MQL_Parser::MatchQueryContext* ctx) {
    visitChildren(ctx);

    // Process used_var_properties
    std::vector<OpProperty> optional_properties;
    for (auto& property : used_var_properties) {
        if (match_var_properties.find(property) == match_var_properties.end()) {
            optional_properties.push_back(property);
        }
    }

    current_op = std::make_unique<OpMatch>(
        std::move(current_op),
        std::move(optional_properties)
    );

    if (current_expr != nullptr) {
        current_op = std::make_unique<OpWhere>(
            std::move(current_op),
            std::move(current_expr)
        );
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
                return_info.items.push_back({var, nullptr});
            }
        }
    }

    current_op = std::make_unique<OpReturn>(
        std::move(current_op),
        std::move(return_info.items),
        return_info.distinct,
        return_info.limit
    );

    if (set_items.size() > 0) {
        current_op = std::make_unique<OpSet>(
            std::move(current_op),
            std::move(set_items)
        );
    }
    return 0;
}


Any QueryVisitor::visitMatchStatement(MQL_Parser::MatchStatementContext* ctx) {
    visitChildren(ctx);
    return 0;
}


Any QueryVisitor::visitSetItem(MQL_Parser::SetItemContext* ctx) {
    auto var_name = ctx->VARIABLE()->getText();
    var_name.erase(0, 1); // remove leading '?'
    set_items.push_back({
        get_query_ctx().get_or_create_var(var_name),
        QuadObjectId::get_fixed_node_inside(ctx->fixedNodeInside()->getText())
    });
    return 0;
}


Any QueryVisitor::visitInsertLabelElement(MQL_Parser::InsertLabelElementContext* ctx) {
    auto label_str = ctx->STRING()->getText();
    // remove surrounding double quotes
    label_str = label_str.substr(1, label_str.size() - 2);

    ObjectId label_id = QuadObjectId::get_string(label_str);

    ObjectId node_id = (ctx->identifier() == nullptr)
        ? QuadObjectId::get_fixed_node_inside(ctx->ANON_ID()->getText())
        : QuadObjectId::get_fixed_node_inside(ctx->identifier()->getText());

    insert_labels.emplace_back(node_id, label_id);
    return 0;
}


Any QueryVisitor::visitInsertPropertyElement(MQL_Parser::InsertPropertyElementContext* ctx) {
    auto obj = ctx->fixedNodeInside()->getText();

    auto key = ctx->STRING()->getText();
    key = key.substr(1, key.size() - 2); // remove surrounding double quotes

    insert_properties.emplace_back(
        QuadObjectId::get_fixed_node_inside(obj),
        QuadObjectId::get_string(key),
        QuadObjectId::get_value(ctx->value()->getText())
    );
    return 0;
}


Any QueryVisitor::visitInsertEdgeElement(MQL_Parser::InsertEdgeElementContext* ctx) {
    auto from = QuadObjectId::get_fixed_node_inside(ctx->fixedNodeInside()[0]->getText());
    auto to   = QuadObjectId::get_fixed_node_inside(ctx->fixedNodeInside()[1]->getText());
    auto type = QuadObjectId::get_fixed_node_inside(ctx->identifier()->getText());
    insert_edges.emplace_back(
        from,
        to,
        type,
        ObjectId::get_null() // edge won't be used on inserts, pass anything
    );
    return 0;
}


Any QueryVisitor::visitReturnAll(MQL_Parser::ReturnAllContext* ctx) {
    if (ctx->UNSIGNED_INTEGER() != nullptr) {
        return_info.limit = stoll(ctx->UNSIGNED_INTEGER()->getText());
    } else {
        return_info.limit = OpReturn::DEFAULT_LIMIT;
    }
    return_info.distinct = ctx->K_DISTINCT() != nullptr;

    return 0;
}


Any QueryVisitor::visitReturnList(MQL_Parser::ReturnListContext* ctx) {
    for (auto& return_expr : ctx->returnItem()) {
        return_expr->accept(this);
    }

    if (ctx->UNSIGNED_INTEGER() != nullptr) {
        return_info.limit = stoll(ctx->UNSIGNED_INTEGER()->getText());
    } else {
        return_info.limit = OpReturn::DEFAULT_LIMIT;
    }
    return_info.distinct = ctx->K_DISTINCT() != nullptr;
    return 0;
}


Any QueryVisitor::visitReturnItemVar(MQL_Parser::ReturnItemVarContext* ctx) {
    auto var_name = ctx->VARIABLE()->getText();
    var_name.erase(0, 1); // remove leading '?'
    auto var = get_query_ctx().get_or_create_var(var_name);

    if (ctx->KEY() != nullptr) {
        auto key_name = ctx->KEY()->getText();
        auto property_var_name = var_name + key_name;
        key_name.erase(0, 1); // remove leading '.'

        auto property_var = get_query_ctx().get_or_create_var(property_var_name);

        auto key_id = QuadObjectId::get_string(key_name);
        used_var_properties.emplace(
            var, key_id, property_var
        );
        return_info.items.push_back({property_var, nullptr});
    } else {
        return_info.items.push_back({var, nullptr});
    }
    return 0;
}


Any QueryVisitor::visitReturnItemAgg(MQL_Parser::ReturnItemAggContext* ctx) {
    // TODO:
    // auto var_name = ctx->VARIABLE()->getText();
    // var_name.erase(0, 1); // remove leading '?'
    // if (ctx->KEY() != nullptr) {
    //     var += ctx->KEY()->getText();
    // }
    // return_items.push_back(
    //     std::make_unique<ReturnItemAgg>(
    //         ascii_str_to_lower(ctx->aggregateFunc()->getText()),
    //         std::move(var)
    //     )
    // );
    return 0;
}


Any QueryVisitor::visitReturnItemCount(MQL_Parser::ReturnItemCountContext* ctx) {
    // TODO:
    // std::string inside_var;
    // if (ctx->VARIABLE() != nullptr) {
    //     inside_var = ctx->VARIABLE()->getText();
    //     if (ctx->KEY() != nullptr) {
    //         inside_var += ctx->KEY()->getText();
    //     }
    // } else {
    //     inside_var = "*";
    // }
    // return_items.push_back(std::make_unique<ReturnItemCount>(ctx->K_DISTINCT() != nullptr,
    //                                                             std::move(inside_var)));
    return 0;
}


Any QueryVisitor::visitOrderByStatement(MQL_Parser::OrderByStatementContext* ctx) {
    for (auto order_by_item : ctx->orderByItem()) {
        order_by_item->accept(this);
    }

    return 0;
}


Any QueryVisitor::visitOrderByItemVar(MQL_Parser::OrderByItemVarContext* ctx) {
    auto var_name = ctx->VARIABLE()->getText();
    var_name.erase(0, 1); // remove leading '?'
    auto var = get_query_ctx().get_or_create_var(var_name);

    if (ctx->KEY() != nullptr) {
        auto key_name = ctx->KEY()->getText();
        auto property_var_name = var_name + key_name;
        key_name.erase(0, 1); // remove leading '.'

        auto property_var = get_query_ctx().get_or_create_var(property_var_name);

        auto key_id = QuadObjectId::get_string(key_name);
        used_var_properties.emplace(
            var, key_id, property_var
        );
        order_by_info.items.push_back(property_var);
    } else {
        order_by_info.items.push_back(var);
    }

    order_by_info.ascending_order.push_back(ctx->K_DESC() == nullptr);
    return 0;
}


Any QueryVisitor::visitOrderByItemAgg(MQL_Parser::OrderByItemAggContext* ctx) {
    // auto var = ctx->VARIABLE()->getText();
    // if (ctx->KEY() != nullptr) {
    //     var += ctx->KEY()->getText();
    // }
    // order_by_items.push_back(std::make_unique<ReturnItemAgg>(ascii_str_to_lower(ctx->aggregateFunc()->getText()), std::move(var)));
    // order_by_ascending_order.push_back(ctx->K_DESC() == nullptr);
    return 0;
}


Any QueryVisitor::visitOrderByItemCount(MQL_Parser::OrderByItemCountContext* ctx) {
    // auto var = ctx->VARIABLE()->getText();
    // if (ctx->KEY() != nullptr) {
    //     var += ctx->KEY()->getText();
    // }
    // order_by_items.push_back(std::make_unique<ReturnItemCount>(ctx->K_DISTINCT() != nullptr, std::move(var)));
    // order_by_ascending_order.push_back(ctx->K_DESC() == nullptr);
    return 0;
}


Any QueryVisitor::visitGroupByStatement(MQL_Parser::GroupByStatementContext* ctx) {
    // for (auto group_by_item : ctx->groupByItem()) {
    //     group_by_item->accept(this);
    // }

    // current_op = std::make_unique<OpGroupBy>(std::move(current_op),
    //                                             std::move(group_by_vars));
    return 0;
}


Any QueryVisitor::visitGroupByItem(MQL_Parser::GroupByItemContext* ctx) {
    // auto var = ctx->VARIABLE()->getText();
    // if (ctx->KEY() != nullptr) {
    //     var += ctx->KEY()->getText();
    // }
    // group_by_vars.emplace_back(Var(var));
    return 0;
}


Any QueryVisitor::visitGraphPattern(MQL_Parser::GraphPatternContext* ctx) {
    ctx->basicPattern()->accept(this);
    auto parent = std::move(current_basic_graph_pattern);
    if (ctx->optionalPattern().size() > 0) {
        std::vector<std::unique_ptr<Op>> optional_children;
        for (auto& opt : ctx->optionalPattern()) {
            opt->accept(this);
            optional_children.push_back(std::move(current_op));
        }
        current_op = std::make_unique<OpOptional>(
            std::move(parent),
            std::move(optional_children)
        );
    } else {
        current_op = std::move(parent);
    }
    return 0;
}


Any QueryVisitor::visitBasicPattern(MQL_Parser::BasicPatternContext* ctx) {
    current_basic_graph_pattern = std::make_unique<OpBasicGraphPattern>();
    visitChildren(ctx);
    for (auto& pending_disjoint_var : possible_disjoint_vars) {
        current_basic_graph_pattern->try_add_possible_disjoint_var(pending_disjoint_var);
    }
    return 0;
}


Any QueryVisitor::visitLinearPattern(MQL_Parser::LinearPatternContext* ctx) {
    first_element_disjoint = ctx->children.size() == 1;
    ctx->children[0]->accept(this);
    saved_node = last_node;
    for (size_t i = 2; i < ctx->children.size(); i += 2) {
        ctx->children[i]->accept(this);     // accept node
        ctx->children[i - 1]->accept(this); // accept edge or path
        saved_node = last_node;
    }
    return 0;
}


Any QueryVisitor::visitFixedNodeInside(MQL_Parser::FixedNodeInsideContext* ctx) {
    last_node = QuadObjectId::get_fixed_node_inside(ctx->getText());
    if (first_element_disjoint) {
        current_basic_graph_pattern->add_disjoint_term(last_node.get_OID());
    }
    return 0;
}


Any QueryVisitor::visitVarNode(MQL_Parser::VarNodeContext* ctx) {
    std::string var_name;
    VarId var(0); // Will be overwritten

    // process var
    if (ctx->VARIABLE() != nullptr) {
        var_name = ctx->VARIABLE()->getText();
        var_name.erase(0, 1); // remove leading '?'
        var = get_query_ctx().get_or_create_var(var_name);
        if (first_element_disjoint && ctx->TYPE().empty() && ctx->properties() == nullptr) {
            possible_disjoint_vars.insert(var);
        }
    } else {
        var = get_query_ctx().get_internal_var();
        if (first_element_disjoint && ctx->TYPE().empty() && ctx->properties() == nullptr) {
            // we know for sure is an disjoint var
            current_basic_graph_pattern->add_disjoint_var(OpDisjointVar(var));
        }
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
        for (auto property : properties->property()) {
            auto key_str = property->identifier()->getText();
            auto key_id = QuadObjectId::get_string(key_str);

            std::string value;
            if (property->value() == nullptr) {
                if (property->FALSE_PROP() != nullptr) {
                    value = "false";
                }
                if (property->TRUE_PROP() != nullptr) {
                    value = "true";
                }
            } else {
                value = property->value()->getText();
            }
            auto value_id = QuadObjectId::get_value(value);
            OpProperty op_property(var, key_id, value_id);
            match_var_properties.insert(op_property);
            current_basic_graph_pattern->add_property(op_property);
        }
    }

    last_node = var;
    return 0;
}


Any QueryVisitor::visitEdge(MQL_Parser::EdgeContext* ctx) {
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

Any QueryVisitor::visitEdgeInside(MQL_Parser::EdgeInsideContext* ctx) {
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
        type_str.erase(0, 1);// remove leading ':'
        saved_type = QuadObjectId::get_named_node(type_str);
    } else {
        auto type_var = get_query_ctx().get_internal_var();
        saved_type = type_var;
    }

    auto properties = ctx->properties();
    if (properties != nullptr) {
        for (auto property : properties->property()) {
            auto key_str = property->identifier()->getText();
            auto key_id = QuadObjectId::get_string(key_str);

            std::string value;
            if (property->value() == nullptr) {
                if (property->FALSE_PROP() != nullptr) {
                    value = "false";
                }
                if (property->TRUE_PROP() != nullptr) {
                    value = "true";
                }
            } else {
                value = property->value()->getText();
            }
            auto value_id = QuadObjectId::get_value(value);
            OpProperty op_property(saved_edge, key_id, value_id);
            match_var_properties.insert(op_property);
            current_basic_graph_pattern->add_property(
                op_property
            );
        }
    }
    return visitChildren(ctx);
}


Any QueryVisitor::visitPath(MQL_Parser::PathContext* ctx) {
    VarId path_var(0); // Will be overwritten
    if (ctx->VARIABLE() == nullptr) {
        path_var = get_query_ctx().get_internal_var();
    } else {
        auto path_var_name = ctx->VARIABLE()->getText();
        path_var_name.erase(0, 1); // remove leading '?'
        path_var = get_query_ctx().get_or_create_var(path_var_name);
    }

    PathSemantic semantic = PathSemantic::ALL_SHORTEST_WALKS;
    if (ctx->pathType() == nullptr) {
        semantic = PathSemantic::ANY_SHORTEST_WALKS;
    } else {
        auto semantic_str = ctx->pathType()->getText();
        std::transform(semantic_str.begin(), semantic_str.end(), semantic_str.begin(), ascii_to_lower);
        if (semantic_str == "any") {
            semantic = PathSemantic::ANY_SHORTEST_WALKS;
        }
        // TODO: include other semantics
    }

    current_path_inverse = false;
    ctx->pathAlternatives()->accept(this);
    if (ctx->GT() != nullptr) {
        // right direction
        current_basic_graph_pattern->add_path(
            OpPath(
                path_var,
                saved_node,
                last_node,
                semantic,
                std::move(current_path)
            )
        );
    } else {
        // left direction
        current_basic_graph_pattern->add_path(
            OpPath(
                path_var,
                last_node,
                saved_node,
                semantic,
                std::move(current_path))
            );
    }
    return 0;
}


Any QueryVisitor::visitPathAlternatives(MQL_Parser::PathAlternativesContext* ctx) {
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


Any QueryVisitor::visitPathSequence(MQL_Parser::PathSequenceContext* ctx) {
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


Any QueryVisitor::visitPathAtomSimple(MQL_Parser::PathAtomSimpleContext* ctx) {
    std::string type = ctx->TYPE()->getText();
    type.erase(0, 1);// remove ':'

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
        // kleene star
        current_path = std::make_unique<PathKleeneStar>(std::move(current_path));

    } else if (suffix->op->getText() == "+") {
        // A+ => A / A*
        auto kleene_star = std::make_unique<PathKleeneStar>(current_path->clone());
        std::vector<std::unique_ptr<RegularPathExpr>> seq;
        seq.push_back(std::move(current_path));
        seq.push_back(std::move(kleene_star));
        current_path = std::make_unique<PathSequence>(std::move(seq));

    } else if (suffix->op->getText() == "?") {
        if (!current_path->nullable()) {
            current_path = std::make_unique<PathOptional>(std::move(current_path));
        }
        // else we avoid a redundant PathOptional, current_path stays the same
    }
    return 0;
}


Any QueryVisitor::visitPathAtomAlternatives(MQL_Parser::PathAtomAlternativesContext* ctx) {
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
        // kleene star
        current_path = std::make_unique<PathKleeneStar>(std::move(current_path));

    } else if (suffix->op->getText() == "+") {
        // A+ => A / A*
        auto kleene_star = std::make_unique<PathKleeneStar>(current_path->clone());
        std::vector<std::unique_ptr<RegularPathExpr>> seq;
        seq.push_back(std::move(current_path));
        seq.push_back(std::move(kleene_star));
        current_path = std::make_unique<PathSequence>(std::move(seq));

    } else if (suffix->op->getText() == "?") {
        if (!current_path->nullable()) {
            current_path = std::make_unique<PathOptional>(std::move(current_path));
        }
        // else we avoid a redundant PathOptional, current_path stays the same
    }
    return 0;
}


Any QueryVisitor::visitWhereStatement(MQL_Parser::WhereStatementContext* ctx) {
    ctx->conditionalOrExpr()->accept(this);
    return 0;
}


Any QueryVisitor::visitExprVar(MQL_Parser::ExprVarContext* ctx) {
    auto var_name = ctx->VARIABLE()->getText();
    var_name.erase(0, 1); // remove leading '?'
    auto var = get_query_ctx().get_or_create_var(var_name);

    if (ctx->KEY() != nullptr) {
        auto key_name = ctx->KEY()->getText();
        auto property_var_name = var_name + key_name;
        key_name.erase(0, 1); // remove leading '.'

        auto property_var = get_query_ctx().get_or_create_var(property_var_name);
        auto key_id = QuadObjectId::get_string(key_name);

        current_expr = std::make_unique<ExprVar>(property_var);
        used_var_properties.emplace(var, key_id, property_var);
    } else {
        current_expr = std::make_unique<ExprVar>(var);
    }
    return 0;
}


Any QueryVisitor::visitExprValueExpr(MQL_Parser::ExprValueExprContext* ctx) {
    auto oid = QuadObjectId::get_value(ctx->getText());
    current_expr = std::make_unique<ExprConstant>(oid);
    return 0;
}


Any QueryVisitor::visitConditionalOrExpr(MQL_Parser::ConditionalOrExprContext* ctx) {
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


Any QueryVisitor::visitConditionalAndExpr(MQL_Parser::ConditionalAndExprContext* ctx) {
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


Any QueryVisitor::visitComparisonExprOp(MQL_Parser::ComparisonExprOpContext* ctx) {
    ctx->additiveExpr()[0]->accept(this);
    if (ctx->additiveExpr().size() > 1) {
        auto saved_lhs = std::move(current_expr);
        ctx->additiveExpr()[1]->accept(this);
        auto op = ctx->op->getText();
        if (op == "==") {
            current_expr = std::make_unique<ExprEquals>(std::move(saved_lhs), std::move(current_expr));
        } else if (op == "!=") {
            current_expr = std::make_unique<ExprNotEquals>(std::move(saved_lhs), std::move(current_expr));
        }  else if (op == "<") {
            current_expr = std::make_unique<ExprLess>(std::move(saved_lhs), std::move(current_expr));
        } else if (op == "<=") {
            current_expr = std::make_unique<ExprLessOrEquals>(std::move(saved_lhs), std::move(current_expr));
        } else if (op == ">=") {
            current_expr = std::make_unique<ExprGreaterOrEquals>(std::move(saved_lhs), std::move(current_expr));
        } else if (op == ">") {
            current_expr = std::make_unique<ExprGreater>(std::move(saved_lhs), std::move(current_expr));
        } else {
            throw std::invalid_argument(op + " not recognized as a valid ComparisonExpr operator");

        }
    }
    return 0;
}


Any QueryVisitor::visitComparisonExprIs(MQL_Parser::ComparisonExprIsContext* ctx) {
    ctx->additiveExpr()->accept(this);
    current_expr = std::make_unique<ExprIs>(ctx->K_NOT() != nullptr,
                                            std::move(current_expr),
                                            ascii_str_to_lower(ctx->exprTypename()->getText()));
    return 0;
}


Any QueryVisitor::visitAdditiveExpr(MQL_Parser::AdditiveExprContext* ctx) {
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
        }  else {
            throw std::invalid_argument(op + " not recognized as a valid AdditiveExpr operator");
        }
    }
    return 0;
}


Any QueryVisitor::visitMultiplicativeExpr(MQL_Parser::MultiplicativeExprContext* ctx) {
    auto unaryExprs = ctx->unaryExpr();
    unaryExprs[0]->accept(this);
    for (std::size_t i = 1; i < unaryExprs.size(); i++) {
        auto saved_lhs = std::move(current_expr);
        unaryExprs[i]->accept(this);
        auto op = ctx->op[i - 1]->getText();
        if (op == "*") {
            current_expr = std::make_unique<ExprMultiplication>(std::move(saved_lhs), std::move(current_expr));
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


Any QueryVisitor::visitUnaryExpr(MQL_Parser::UnaryExprContext* ctx) {
    if (ctx->unaryExpr() != nullptr) {
        ctx->unaryExpr()->accept(this);
        if (ctx->K_NOT() != nullptr) {
            current_expr = std::make_unique<ExprNot>(std::move(current_expr));
        } else if (ctx->children[0]->getText() == "+") {
            current_expr = std::make_unique<ExprUnaryPlus>(std::move(current_expr));
        } else if (ctx->children[0]->getText() == "-") {
            current_expr = std::make_unique<ExprUnaryMinus>(std::move(current_expr));
        } else {
            throw std::invalid_argument(ctx->children[0]->getText() + " not recognized as a valid UnaryExpr operator");
        }
    } else {
        ctx->atomicExpr()->accept(this);
    }
    return 0;
}
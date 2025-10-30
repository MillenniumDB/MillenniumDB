#include "query_visitor.h"

#include <cassert>
#include <cstdlib>

#include "graph_models/common/datatypes/datetime.h"
#include "graph_models/common/datatypes/tensor/tensor.h"
#include "graph_models/quad_model/conversions.h"
#include "graph_models/quad_model/quad_model.h"
#include "graph_models/quad_model/quad_object_id.h"
#include "misc/is_name_valid_for_path.h"
#include "misc/transliterator.h"
#include "query/parser/expr/mql/exprs.h"
#include "query/parser/op/mql/ops.h"
#include "query/parser/paths/path_alternatives.h"
#include "query/parser/paths/path_atom.h"
#include "query/parser/paths/path_kleene_plus.h"
#include "query/parser/paths/path_kleene_star.h"
#include "query/parser/paths/path_optional.h"
#include "query/parser/paths/path_sequence.h"
#include "query/query_context.h"
#include "query/update/mql/hnsw_index_options.h"
#include "query/update/mql/text_index_options.h"

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
                throw QueryException("Duplicate DESCRIBE flag: LABELS");
            labels_limit = curr_limit;
            labels_limit_seen = true;
        } else if (flag->K_PROPERTIES() != nullptr) {
            if (properties_limit_seen)
                throw QueryException("Duplicate DESCRIBE flag: PROPERTIES");
            properties_limit = curr_limit;
            properties_limit_seen = true;
        } else if (flag->K_OUTGOING() != nullptr) {
            if (outgoing_limit_seen)
                throw QueryException("Duplicate DESCRIBE flag: OUTGOING");
            outgoing_limit = curr_limit;
            outgoing_limit_seen = true;
        } else if (flag->K_INCOMING() != nullptr) {
            if (incoming_limit_seen)
                throw QueryException("Duplicate DESCRIBE flag: INCOMING");
            incoming_limit = curr_limit;
            incoming_limit_seen = true;
        }
    }

    const auto object_id = last_node.get_OID();
    const bool is_edge = object_id.get_type() & ObjectId::MASK_EDGE;
    if (is_edge && (labels_limit_seen || outgoing_limit_seen || incoming_limit_seen)) {
        throw QueryException("Invalid flag to describe edge");
    }

    current_op = std::make_unique<OpDescribe>(
        object_id,
        labels_limit,
        properties_limit,
        outgoing_limit,
        incoming_limit
    );

    return 0;
}

Any QueryVisitor::visitShowQuery(MQL_Parser::ShowQueryContext* ctx)
{
    const auto index_type = ctx->identifier()->getText();
    const auto index_type_lowercased = Transliterator::lowercase(index_type);
    if (index_type_lowercased == "hnsw") {
        current_op = std::make_unique<OpShow>(OpShow::Type::HNSW_INDEX);
    } else if (index_type_lowercased == "text") {
        current_op = std::make_unique<OpShow>(OpShow::Type::TEXT_INDEX);
    } else {
        throw QueryException("Unknown index type: \"" + index_type + '"');
    }

    return 0;
}

Any QueryVisitor::visitSimpleQuery(MQL_Parser::SimpleQueryContext* ctx)
{
    const auto primitiveStatements = ctx->primitiveStatement();

    std::vector<std::unique_ptr<Op>> sequence;

    for (auto& primitiveStatement : primitiveStatements) {
        visit(primitiveStatement);
        if (auto where_stmt = primitiveStatement->whereStatement()) {
            assert(current_expr != nullptr);
            if (current_expr->has_aggregation()) {
                throw QueryException("Cannot have aggregations inside WHERE, use HAVING INSTEAD");
            }
            if (sequence.empty()) {
                throw QueryException("Invalid WHERE placement");
            }
            sequence.back() = std::make_unique<OpWhere>(std::move(sequence.back()), std::move(current_expr));
        } else {
            assert(current_op != nullptr);
            if (current_expr) {
                if (current_expr->has_aggregation()) {
                    throw QueryException("Cannot have aggregations inside WHERE, use HAVING INSTEAD");
                }
                current_op = std::make_unique<OpWhere>(std::move(current_op), std::move(current_expr));
            }
            sequence.emplace_back(std::move(current_op));
        }
    }
    if (sequence.size() > 1) {
        current_op = std::make_unique<OpSequence>(std::move(sequence));
    } else {
        assert(sequence.size() > 0);
        current_op = std::move(sequence[0]);
    }

    if (auto group_stmt = ctx->groupByStatement()) {
        group_stmt->accept(this);
        assert(group_by_exprs.size() > 0);
        current_op = std::make_unique<OpGroupBy>(std::move(current_op), std::move(group_by_exprs));
    }
    if (auto having_stmt = ctx->havingStatement()) {
        assert(current_expr == nullptr);
        having_stmt->accept(this);
        assert(current_expr != nullptr);
        current_op = std::make_unique<OpHaving>(std::move(current_op), std::move(current_expr));
    }
    if (auto order_stmt = ctx->orderByStatement()) {
        order_stmt->accept(this);

        assert(order_by_info.items.size() > 0);
        current_op = std::make_unique<OpOrderBy>(
            std::move(current_op),
            std::move(order_by_info.items),
            std::move(order_by_info.ascending_order)
        );
    }

    if (auto return_stmt = ctx->returnStatement()) {
        return_stmt->accept(this);
        current_op = std::make_unique<OpReturn>(
            std::move(current_op),
            std::move(return_info.items),
            return_info.distinct,
            return_info.limit,
            return_info.offset
        );
    } else {
        update_info.update_ctx = std::make_unique<UpdateContext>();
        for (auto& update : ctx->updateStatement()) {
            update->accept(this);
        }
        current_op = std::make_unique<OpUpdate>(
            std::move(current_op),
            std::move(update_info.update_ctx),
            std::move(update_info.update_actions)
        );
    }

    return 0;
}

Any QueryVisitor::visitUpdateStatement(MQL_Parser::UpdateStatementContext* ctx)
{
    // curent_op is not null when root is a simpleQuery
    // curent_op is null when root is a updateStatement
    if (current_op != nullptr) {
        visitChildren(ctx);
        return 0;
    }

    update_info.update_ctx = std::make_unique<UpdateContext>();
    visitChildren(ctx);
    current_op = std::make_unique<OpUpdate>(
        std::make_unique<OpUnitTable>(),
        std::move(update_info.update_ctx),
        std::move(update_info.update_actions)
    );

    return 0;
}

Any QueryVisitor::visitDeleteStatement(MQL_Parser::DeleteStatementContext* ctx)
{
    bool detach = ctx->K_DETACH() != nullptr;
    auto obj_list = ctx->fixedObj();
    auto var_list = ctx->VARIABLE();

    for (auto obj : obj_list) {
        auto oid = QuadObjectId::get_fixed_node_inside(obj->getText());
        update_info.update_actions.push_back(std::make_unique<DeleteObject>(oid, detach));
    }

    for (auto var : var_list) {
        auto var_name = var->getText();
        var_name.erase(0, 1); // remove leading '?'
        auto var_id = get_query_ctx().get_or_create_var(var_name);
        update_info.update_actions.push_back(std::make_unique<DeleteObject>(var_id, detach));
    }

    return 0;
}

Any QueryVisitor::visitSetAtom(MQL_Parser::SetAtomContext* ctx)
{
    Id obj = ObjectId();
    if (auto fixed_obj = ctx->fixedObj()) {
        obj = QuadObjectId::get_fixed_node_inside(fixed_obj->getText());
    } else if (auto variable = ctx->VARIABLE()) {
        auto var_name = variable->getText();
        var_name.erase(0, 1); // remove leading '?'
        obj = get_query_ctx().get_or_create_var(var_name);
    }

    if (auto key_terminal = ctx->KEY()) {
        auto key_name = key_terminal->getText();

        key_name.erase(0, 1); // remove leading '.'
        auto key_id = QuadObjectId::get_string(key_name);

        ctx->conditionalOrExpr()->accept(this);
        update_info.update_actions.push_back(
            std::make_unique<InsertPropertyExpr>(obj, key_id, std::move(current_expr))
        );
    } else if (auto insert_properties = ctx->insertProperties()) {
        saved_property_obj = obj;
        for (auto property : insert_properties->insertProperty()) {
            property->accept(this);
        }
    } else {
        for (auto& label : ctx->TYPE()) {
            auto label_str = label->getText();
            label_str.erase(0, 1); // remove leading ':'
            auto label_id = QuadObjectId::get_string(label_str);
            update_info.update_actions.push_back(std::make_unique<SetLabelOrType>(obj, label_id));
        }
    }

    return 0;
}

Any QueryVisitor::visitRemoveAtom(MQL_Parser::RemoveAtomContext* ctx)
{
    Id obj = ObjectId();
    if (auto fixed_obj = ctx->fixedObj()) {
        obj = QuadObjectId::get_fixed_node_inside(fixed_obj->getText());
    } else if (auto variable = ctx->VARIABLE()) {
        auto var_name = variable->getText();
        var_name.erase(0, 1); // remove leading '?'
        obj = get_query_ctx().get_or_create_var(var_name);
    }

    if (auto key_terminal = ctx->KEY()) {
        auto key_name = key_terminal->getText();

        key_name.erase(0, 1); // remove leading '.'
        auto key_id = QuadObjectId::get_string(key_name);

        update_info.update_actions.push_back(std::make_unique<DeleteProperty>(obj, key_id));
    } else {
        for (auto& label : ctx->TYPE()) {
            auto label_str = label->getText();
            label_str.erase(0, 1); // remove leading ':'
            auto label_id = QuadObjectId::get_string(label_str);
            update_info.update_actions.push_back(std::make_unique<DeleteLabel>(obj, label_id));
        }
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

Any QueryVisitor::visitInsertLinearPattern(MQL_Parser::InsertLinearPatternContext* ctx)
{
    ctx->children[0]->accept(this);
    saved_node = last_node;
    for (size_t i = 2; i < ctx->children.size(); i += 2) {
        ctx->children[i]->accept(this); // accept node
        ctx->children[i - 1]->accept(this); // accept edge
        saved_node = last_node;
    }

    return 0;
}

Any QueryVisitor::visitInsertNode(MQL_Parser::InsertNodeContext* ctx)
{
    if (auto variable = ctx->VARIABLE()) {
        auto var_name = variable->getText();
        var_name.erase(0, 1); // remove leading '?'
        last_node = get_query_ctx().get_or_create_var(var_name);
    } else if (auto identifier = ctx->identifier()) {
        last_node = QuadObjectId::get_fixed_node_inside(identifier->getText());
    } else {
        last_node = update_info.update_ctx->get_anon_id();
    }
    update_info.update_actions.push_back(std::make_unique<InsertNode>(last_node));

    // Process Labels
    for (auto& label : ctx->TYPE()) {
        auto label_str = label->getText();
        label_str.erase(0, 1); // remove leading ':'
        auto label_id = QuadObjectId::get_string(label_str);

        update_info.update_actions.push_back(std::make_unique<InsertLabel>(last_node, label_id));
    }

    if (auto properties = ctx->insertProperties()) {
        saved_property_obj = last_node;
        for (auto property : properties->insertProperty()) {
            property->accept(this);
        }
    }

    return 0;
}

Any QueryVisitor::visitInsertEdge(MQL_Parser::InsertEdgeContext* ctx)
{
    auto edge = get_query_ctx().get_internal_var();

    auto type_str = ctx->TYPE()->getText();
    type_str.erase(0, 1); // remove leading ':'
    auto type_id = QuadObjectId::get_named_node(type_str);

    // InsertEdge must go before because it assigns the var
    if (ctx->GT() != nullptr) {
        // right direction
        update_info.update_actions.push_back(
            std::make_unique<InsertEdge>(saved_node, last_node, type_id, edge)
        );
    } else {
        // left direction
        update_info.update_actions.push_back(
            std::make_unique<InsertEdge>(last_node, saved_node, type_id, edge)
        );
    }

    if (auto properties = ctx->insertProperties()) {
        saved_property_obj = edge;
        for (auto property : properties->insertProperty()) {
            property->accept(this);
        }
    }

    return 0;
}

Any QueryVisitor::visitInsertProperty1(MQL_Parser::InsertProperty1Context* ctx)
{
    auto key_str = ctx->identifier()->getText();
    auto key_id = QuadObjectId::get_string(key_str);

    ObjectId value_id;

    if (auto value = ctx->value()) {
        visitValue(value);
        value_id = current_value_oid;
    } else {
        if (ctx->FALSE_PROP() != nullptr) {
            value_id = ObjectId(ObjectId::BOOL_FALSE);
        } else {
            assert(ctx->TRUE_PROP() != nullptr);
            value_id = ObjectId(ObjectId::BOOL_TRUE);
        }
    }

    update_info.update_actions.push_back(
        std::make_unique<InsertProperty>(saved_property_obj, key_id, value_id)
    );
    return 0;
}

Any QueryVisitor::visitInsertProperty2(MQL_Parser::InsertProperty2Context* ctx)
{
    auto key_str = ctx->identifier()->getText();
    auto key_id = QuadObjectId::get_string(key_str);

    std::string datatype = ctx->TYPE()->getText();
    // remove leading ':'
    datatype.erase(0, 1);

    std::string str = ctx->STRING()->getText();
    // remove surrounding double quotes
    str = str.substr(1, str.size() - 2);

    parse_datatype_value(datatype, str); // will set current_value_oid

    update_info.update_actions.push_back(
        std::make_unique<InsertProperty>(saved_property_obj, key_id, current_value_oid)
    );
    return 0;
}

Any QueryVisitor::visitInsertProperty3(MQL_Parser::InsertProperty3Context* ctx)
{
    auto key_str = ctx->identifier()->getText();
    auto key_id = QuadObjectId::get_string(key_str);

    ctx->conditionalOrExpr()->accept(this);
    assert(current_expr != nullptr);

    update_info.update_actions.push_back(
        std::make_unique<InsertPropertyExpr>(saved_property_obj, key_id, std::move(current_expr))
    );
    return 0;
}

Any QueryVisitor::visitCallStatement(MQL_Parser::CallStatementContext* ctx)
{
    const auto procedure_name = ctx->identifier()->getText();
    const auto procedure_name_lowercased = Transliterator::lowercase(procedure_name);

    OpCall::ProcedureType procedure_type;

    if (procedure_name_lowercased == "hnsw_top_k") {
        procedure_type = OpCall::ProcedureType::HNSW_TOP_K;
    } else if (procedure_name_lowercased == "hnsw_scan") {
        procedure_type = OpCall::ProcedureType::HNSW_SCAN;
    } else if (procedure_name_lowercased == "text_search") {
        procedure_type = OpCall::ProcedureType::TEXT_SEARCH;
    } else {
        throw QueryException("Invalid CALL statement procedure: \"" + procedure_name + "\"");
    }
    visitChildren(ctx);

    // validate arguments
    switch (procedure_type) {
    case OpCall::ProcedureType::HNSW_TOP_K:
    case OpCall::ProcedureType::HNSW_SCAN: {
        if (current_call_argument_exprs.size() != 4) {
            throw QueryException(
                OpCall::get_procedure_string(procedure_type) + " expects exactly 4 arguments"
            );
        }
        break;
    }
    case OpCall::ProcedureType::TEXT_SEARCH: {
        if (current_call_argument_exprs.size() == 2) {
            // set default arguments
            current_call_argument_exprs.emplace_back(
                std::make_unique<ExprConstant>(QuadObjectId::get_string("prefix"))
            );
        } else if (current_call_argument_exprs.size() != 3) {
            throw QueryException(
                OpCall::get_procedure_string(procedure_type) + " expects between 2 and 3 arguments"
            );
        }
        break;
    }
    default:
        throw NotSupportedException("Unhandled CALL statement procedure: \"" + procedure_name + "\"");
    }

    // validate yield statement
    auto available_yield_var_names = OpCall::get_procedure_available_yield_variable_names(procedure_type);

    auto check_yield_var = [&](const std::string& var_name) -> void {
        std::string var_names_str = "{ ";
        var_names_str += available_yield_var_names[0];
        for (std::size_t i = 1; i < available_yield_var_names.size(); ++i) {
            var_names_str += ", " + available_yield_var_names[i];
        }
        var_names_str += " }";

        if (std::find(available_yield_var_names.begin(), available_yield_var_names.end(), var_name)
            == available_yield_var_names.end())
        {
            throw QueryException(
                OpCall::get_procedure_string(procedure_type) + " has an unexpected yield variable: \""
                + var_name + "\". The valid yield variables are the following: " + var_names_str
            );
        }
    };

    for (const auto& [yield_var, _] : current_call_yield_var2alias) {
        check_yield_var(yield_var);
    }

    std::vector<VarId> yield_vars;
    if (current_call_yield_var2alias.empty()) {
        // create all implicit variables
        for (const auto& available_var_name : available_yield_var_names) {
            const auto implicit_var = get_query_ctx().get_or_create_var(available_var_name);
            yield_vars.emplace_back(implicit_var);
        }
    } else {
        // map procedure variable names to its aliases
        for (const auto& available_var_name : available_yield_var_names) {
            const auto it = current_call_yield_var2alias.find(available_var_name);
            if (it != current_call_yield_var2alias.end()) {
                // variable declared by user
                yield_vars.emplace_back(it->second);
            } else {
                // use an internal var
                yield_vars.emplace_back(get_query_ctx().get_internal_var());
            }
        }
    }

    current_op = std::make_unique<OpCall>(
        procedure_type,
        std::move(current_call_argument_exprs),
        std::move(yield_vars)
    );

    return 0;
}

Any QueryVisitor::visitCallArguments(MQL_Parser::CallArgumentsContext* ctx)
{
    const auto conditionalAndExprs = ctx->conditionalOrExpr();

    std::vector<std::unique_ptr<Expr>> call_argument_exprs;
    call_argument_exprs.reserve(conditionalAndExprs.size());

    for (const auto& conditionalOrExpr : conditionalAndExprs) {
        visit(conditionalOrExpr);
        call_argument_exprs.emplace_back(std::move(current_expr));
    }

    current_call_argument_exprs = std::move(call_argument_exprs);

    return 0;
}

Any QueryVisitor::visitYieldStatement(MQL_Parser::YieldStatementContext* ctx)
{
    if (ctx->STAR()) {
        return 0;
    }

    const auto yieldItems = ctx->yieldItem();
    std::map<std::string, VarId> yield_var2alias;
    for (const auto& yieldItem : yieldItems) {
        auto var_name = yieldItem->VARIABLE()->getText();
        var_name.erase(0, 1); // remove leading '?'
        if (yieldItem->alias()) {
            // ?x as ?y -> { "x", ?y }
            auto alias_var_name = yieldItem->alias()->VARIABLE()->getText();
            alias_var_name.erase(0, 1); // remove leading '?'
            const auto alias_var = get_query_ctx().get_or_create_var(alias_var_name);
            if (!yield_var2alias.insert({ var_name, alias_var }).second) {
                throw QueryException("Duplicate procedure variable: \"" + var_name + '"');
            }
        } else {
            // ?x -> { "x", ?x }
            const auto var = get_query_ctx().get_or_create_var(var_name);
            if (!yield_var2alias.insert({ var_name, var }).second) {
                throw QueryException("Duplicate procedure variable: \"" + var_name + '"');
            }
        }
    }

    current_call_yield_var2alias = std::move(yield_var2alias);

    return 0;
}

Any QueryVisitor::visitLetStatement(MQL_Parser::LetStatementContext* ctx) {
    OpLet::VarExprVecType var_expr;

    const auto letDefinitionList = ctx->letDefinition();
    for (auto& definition : letDefinitionList) {
        auto var_name = definition->VARIABLE()->getText();
        var_name.erase(0, 1); // remove leading '?'

        visit(definition->conditionalOrExpr());

        var_expr.emplace_back(get_query_ctx().get_or_create_var(var_name), std::move(current_expr));
    }

    current_op = std::make_unique<OpLet>(std::move(var_expr));

    return 0;
}

Any QueryVisitor::visitReturnAll(MQL_Parser::ReturnAllContext* ctx)
{
    if (auto limit_offset_clause = ctx->limitOffsetClauses()) {
        auto limit_clause = limit_offset_clause->limitClause();
        auto offset_clause = limit_offset_clause->offsetClause();

        if (limit_clause) {
            return_info.limit = stoll(limit_clause->UNSIGNED_INTEGER()->getText());
        }
        if (offset_clause) {
            return_info.offset = stoll(offset_clause->UNSIGNED_INTEGER()->getText());
        }
    }
    return_info.distinct = ctx->K_DISTINCT() != nullptr;

    auto vars = current_op->get_all_vars();
    for (auto& var : vars) {
        // filter anonymous vars
        if (!get_query_ctx().is_internal(var)) {
            auto& var_name = get_query_ctx().get_var_name(var);
            if (auto pos = var_name.find('.'); pos != std::string::npos) {
                auto var_without_property = var_name.substr(0, pos);
                auto key_name = var_name.substr(pos + 1);
                auto var_without_property_id = get_query_ctx().get_or_create_var(var_without_property);
                auto key_id = QuadObjectId::get_string(key_name);

                return_info.items.emplace_back(
                    std::make_unique<ExprVarProperty>(var_without_property_id, key_id, var),
                    var
                );
            } else {
                return_info.items.emplace_back(std::make_unique<ExprVar>(var), var);
            }
        }
    }
    return 0;
}

Any QueryVisitor::visitReturnList(MQL_Parser::ReturnListContext* ctx)
{
    for (auto& return_expr : ctx->returnItem()) {
        return_expr->accept(this);
    }

    if (auto limit_offset_clause = ctx->limitOffsetClauses()) {
        auto limit_clause = limit_offset_clause->limitClause();
        auto offset_clause = limit_offset_clause->offsetClause();

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

Any QueryVisitor::visitReturnItem(MQL_Parser::ReturnItemContext* ctx)
{
    visit(ctx->conditionalOrExpr());
    assert(current_expr != nullptr);

    VarId var(0);
    if (auto alias = ctx->alias()) {
        auto var_name = alias->VARIABLE()->getText();
        var_name.erase(0, 1); // remove leading '?'
        var = get_query_ctx().get_or_create_var(var_name);
    } else if (auto expr_var = current_expr->get_var(); expr_var.has_value()) {
        var = expr_var.value();
    } else {
        std::stringstream ss;
        ss << '.' << *current_expr;
        var = get_query_ctx().get_or_create_var(ss.str());
    }

    return_info.items.emplace_back(std::move(current_expr), var);
    return 0;
}

Any QueryVisitor::visitOrderByItem(MQL_Parser::OrderByItemContext* ctx)
{
    visit(ctx->conditionalOrExpr());

    assert(current_expr != nullptr);
    order_by_info.items.push_back(std::move(current_expr));
    order_by_info.ascending_order.push_back(ctx->K_DESC() == nullptr);

    return 0;
}

Any QueryVisitor::visitGroupByItem(MQL_Parser::GroupByItemContext* ctx)
{
    auto var_name = ctx->VARIABLE()->getText();
    var_name.erase(0, 1); // remove leading '?'
    auto var = get_query_ctx().get_or_create_var(var_name);

    if (auto key_terminal = ctx->KEY()) {
        auto key_name = key_terminal->getText();
        auto property_var_name = var_name + key_name;
        key_name.erase(0, 1); // remove leading '.'

        auto property_var = get_query_ctx().get_or_create_var(property_var_name);

        auto key_id = QuadObjectId::get_string(key_name);
        group_by_exprs.push_back(std::make_unique<ExprVarProperty>(var, key_id, property_var));
    } else {
        group_by_exprs.push_back(std::make_unique<ExprVar>(var));
    }
    return 0;
}

Any QueryVisitor::visitGraphPattern(MQL_Parser::GraphPatternContext* ctx)
{
    assert(ctx->basicPattern() != nullptr);
    ctx->basicPattern()->accept(this);
    auto parent = std::move(current_bgp);
    if (auto optional_ctx = ctx->optionalPattern(); optional_ctx.size() > 0) {
        std::vector<std::unique_ptr<Op>> optional_children;
        for (auto& opt : optional_ctx) {
            auto old_expr = std::move(current_expr);
            opt->accept(this);
            if (current_expr != nullptr) {
                current_op = std::make_unique<OpWhere>(std::move(current_op), std::move(current_expr));
            }
            current_expr = std::move(old_expr);

            optional_children.push_back(std::move(current_op));
        }
        current_op = std::make_unique<OpOptional>(std::move(parent), std::move(optional_children));
    } else {
        current_op = std::move(parent);
    }

    return 0;
}

Any QueryVisitor::visitBasicPattern(MQL_Parser::BasicPatternContext* ctx)
{
    current_bgp = std::make_unique<OpBasicGraphPattern>();
    visitChildren(ctx);
    for (auto& pending_disjoint_var : possible_disjoint_vars) {
        current_bgp->try_add_possible_disjoint_var(pending_disjoint_var);
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
    first_element_disjoint = ctx->children.size() == 1;
    ctx->children[0]->accept(this);
    saved_node = last_node;
    for (size_t i = 2; i < ctx->children.size(); i += 2) {
        ctx->children[i]->accept(this); // accept node
        ctx->children[i - 1]->accept(this); // accept edge or path
        saved_node = last_node;
    }

    return 0;
}

Any QueryVisitor::visitFixedObj(MQL_Parser::FixedObjContext* ctx)
{
    last_node = QuadObjectId::get_fixed_node_inside(ctx->getText());
    if (first_element_disjoint) {
        current_bgp->add_disjoint_term(last_node.get_OID());
    }
    return 0;
}

Any QueryVisitor::visitProperty1(MQL_Parser::Property1Context* property)
{
    auto key_str = property->identifier()->getText();
    auto key_id = QuadObjectId::get_string(key_str);

    ObjectId value_id;

    if (property->value() != nullptr) {
        visitValue(property->value());
        value_id = current_value_oid;
    } else {
        if (property->FALSE_PROP() != nullptr) {
            value_id = ObjectId(ObjectId::BOOL_FALSE);
        } else {
            assert(property->TRUE_PROP() != nullptr);
            value_id = ObjectId(ObjectId::BOOL_TRUE);
        }
    }

    current_bgp->add_property(saved_property_obj, key_id, value_id);
    return 0;
}

Any QueryVisitor::visitProperty2(MQL_Parser::Property2Context* property)
{
    auto key_str = property->identifier()->getText();
    auto key_id = QuadObjectId::get_string(key_str);

    std::string datatype = property->TYPE()->getText();
    // remove leading ':'
    datatype.erase(0, 1);

    std::string str = property->STRING()->getText();
    // remove surrounding double quotes
    str = str.substr(1, str.size() - 2);

    parse_datatype_value(datatype, str); // will set current_value_oid

    current_bgp->add_property(saved_property_obj, key_id, current_value_oid);
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
    std::vector<std::unique_ptr<Expr>> datatypes_is_exprs_where;
    bool negation = property->K_NOT() != nullptr;

    for (std::size_t i = 0; i < property->conditionalOrType().size() + 1; i++) {
        MQL_Parser::ExprTypenameContext* data_type = (i == 0)
                                                       ? property->exprTypename()
                                                       : property->conditionalOrType()[i - 1]->exprTypename();

        auto expr_var_property = std::make_unique<MQL::ExprVarProperty>(
            saved_property_obj.get_var(),
            key_id,
            property_var
        );
        ExprIs::TypeName type = ExprIs::TypeName::NULL_; // assign to avoid compilation warning

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
        auto expr_var_property_where = expr_var_property->clone();
        datatypes_is_exprs.push_back(
            std::make_unique<MQL::ExprIs>(negation, std::move(expr_var_property), type, propertyTypeBitmap)
        );
        datatypes_is_exprs_where.push_back(std::make_unique<MQL::ExprIs>(
            negation,
            std::move(expr_var_property_where),
            type,
            propertyTypeBitmap
        ));
    }
    property_expr.push_back(std::make_unique<MQL::ExprOr>(std::move(datatypes_is_exprs)));
    return 0;
}

Any QueryVisitor::visitProperty4(MQL_Parser::Property4Context* property)
{ // (?x :Person {value > 5})
    auto key_str = property->identifier()->getText();
    auto key_id = QuadObjectId::get_string(key_str);

    auto var_name = get_query_ctx().get_var_name(saved_property_obj.get_var());
    auto property_var = get_query_ctx().get_or_create_var(var_name + "." + key_str);

    auto op = property->op->getText();
    auto expr_var_property = std::make_unique<MQL::ExprVarProperty>(
        saved_property_obj.get_var(),
        key_id,
        property_var
    );

    ObjectId oid;
    visit(property->value());
    oid = current_value_oid;

    auto value_constant = std::make_unique<ExprConstant>(oid);
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
    if (auto variable = ctx->VARIABLE()) {
        var_name = variable->getText();
        var_name.erase(0, 1); // remove leading '?'
        var = get_query_ctx().get_or_create_var(var_name);
    } else {
        var = get_query_ctx().get_internal_var();
    }

    // Process Labels
    auto labels = ctx->TYPE();
    for (auto& label : labels) {
        auto label_str = label->getText();
        label_str.erase(0, 1); // remove leading ':'
        auto label_id = QuadObjectId::get_string(label_str);
        current_bgp->add_label(var, label_id);
    }

    // Process Properties
    auto properties = ctx->properties();
    if (properties != nullptr) {
        saved_property_obj = var;
        for (auto property : properties->property()) {
            property->accept(this);
        }
    }

    if (first_element_disjoint && labels.empty() && current_bgp->properties.empty()) {
        current_bgp->add_disjoint_var(var);
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
        current_bgp->add_edge(saved_node, last_node, saved_type, saved_edge);
    } else {
        // left direction
        current_bgp->add_edge(last_node, saved_node, saved_type, saved_edge);
    }
    return 0;
}

Any QueryVisitor::visitEdgeInside(MQL_Parser::EdgeInsideContext* ctx)
{
    if (auto variable = ctx->VARIABLE()) {
        auto edge_var_name = variable->getText();
        edge_var_name.erase(0, 1); // remove leading '?'
        saved_edge = get_query_ctx().get_or_create_var(edge_var_name);
    } else if (auto edge = ctx->EDGE_ID()) {
        saved_edge = QuadObjectId::get_edge(edge->getText());
    } else {
        saved_edge = get_query_ctx().get_internal_var();
    }

    if (auto type_var = ctx->TYPE_VAR()) {
        auto type_var_name = type_var->getText();
        type_var_name.erase(0, 2); // remove leading ':?'
        saved_type = get_query_ctx().get_or_create_var(type_var_name);
    } else if (auto type = ctx->TYPE()) {
        auto type_str = type->getText();
        type_str.erase(0, 1); // remove leading ':'
        saved_type = QuadObjectId::get_named_node(type_str);
    } else {
        saved_type = get_query_ctx().get_internal_var();
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
    if (auto variable = ctx->VARIABLE()) {
        auto path_var_name = variable->getText();
        path_var_name.erase(0, 1); // remove leading '?'
        path_var = get_query_ctx().get_or_create_var(path_var_name);
    } else {
        path_var = get_query_ctx().get_internal_var();
    }
    uint64_t K = 0;

    PathSemantic semantic = PathSemantic::DEFAULT;
    if (auto path_type_ctx = ctx->pathType()) {
        if (path_type_ctx->UNSIGNED_INTEGER()) {
            K = std::stoull(path_type_ctx->UNSIGNED_INTEGER()->getText());
            if (path_type_ctx->K_GROUPS()) {
                if (path_type_ctx->K_ACYCLIC()) {
                    semantic = PathSemantic::SHORTEST_K_GROUPS_ACYCLIC;
                } else if (path_type_ctx->K_SIMPLE()) {
                    semantic = PathSemantic::SHORTEST_K_GROUPS_SIMPLE;
                } else if (path_type_ctx->K_TRAILS()) {
                    semantic = PathSemantic::SHORTEST_K_GROUPS_TRAILS;
                } else { // WALKS by default
                    semantic = PathSemantic::SHORTEST_K_GROUPS_WALKS;
                }
            } else {
                if (path_type_ctx->K_ACYCLIC()) {
                    semantic = PathSemantic::SHORTEST_K_ACYCLIC;
                } else if (path_type_ctx->K_SIMPLE()) {
                    semantic = PathSemantic::SHORTEST_K_SIMPLE;
                } else if (path_type_ctx->K_TRAILS()) {
                    semantic = PathSemantic::SHORTEST_K_TRAILS;
                } else { // WALKS by default
                    semantic = PathSemantic::SHORTEST_K_WALKS;
                }
            }
        } else if (path_type_ctx->K_ALL()) {
            if (path_type_ctx->K_SHORTEST()) {
                if (path_type_ctx->K_ACYCLIC()) {
                    semantic = PathSemantic::ALL_SHORTEST_ACYCLIC;
                } else if (path_type_ctx->K_SIMPLE()) {
                    semantic = PathSemantic::ALL_SHORTEST_SIMPLE;
                } else if (path_type_ctx->K_TRAILS()) {
                    semantic = PathSemantic::ALL_SHORTEST_TRAILS;
                } else { // WALKS by default
                    semantic = PathSemantic::ALL_SHORTEST_WALKS;
                }
            } else {
                if (path_type_ctx->K_ACYCLIC()) {
                    semantic = PathSemantic::ALL_ACYCLIC;
                } else if (path_type_ctx->K_SIMPLE()) {
                    semantic = PathSemantic::ALL_SIMPLE;
                } else if (path_type_ctx->K_TRAILS()) {
                    semantic = PathSemantic::ALL_TRAILS;
                } else { // WALKS by default
                    throw QueryException("ALL WALKS path semantic not allowed");
                }
            }
        } else { // K_ANY
            if (path_type_ctx->K_SHORTEST()) {
                if (path_type_ctx->K_ACYCLIC()) {
                    semantic = PathSemantic::ANY_SHORTEST_ACYCLIC;
                } else if (path_type_ctx->K_SIMPLE()) {
                    semantic = PathSemantic::ANY_SHORTEST_SIMPLE;
                } else if (path_type_ctx->K_TRAILS()) {
                    semantic = PathSemantic::ANY_SHORTEST_TRAILS;
                } else { // WALKS by default
                    semantic = PathSemantic::ANY_SHORTEST_WALKS;
                }
            } else {
                if (path_type_ctx->K_ACYCLIC()) {
                    semantic = PathSemantic::ANY_ACYCLIC;
                } else if (path_type_ctx->K_SIMPLE()) {
                    semantic = PathSemantic::ANY_SIMPLE;
                } else if (path_type_ctx->K_TRAILS()) {
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
        current_bgp->add_path(
            path_var,
            saved_node,
            last_node,
            semantic,
            Path::Direction::LEFT_TO_RIGHT,
            K,
            std::move(current_path)
        );
    } else {
        // left direction
        current_bgp->add_path(
            path_var,
            last_node,
            saved_node,
            semantic,
            Path::Direction::RIGHT_TO_LEFT,
            K,
            std::move(current_path)
        );
    }
    return 0;
}

Any QueryVisitor::visitPathAlternatives(MQL_Parser::PathAlternativesContext* ctx)
{
    auto path_sequence = ctx->pathSequence();
    if (path_sequence.size() > 1) {
        std::vector<std::unique_ptr<RegularPathExpr>> alternatives;
        for (auto s : path_sequence) {
            s->accept(this);
            alternatives.push_back(std::move(current_path));
        }
        current_path = std::make_unique<PathAlternatives>(std::move(alternatives));
    } else {
        path_sequence[0]->accept(this);
    }
    return 0;
}

Any QueryVisitor::visitPathSequence(MQL_Parser::PathSequenceContext* ctx)
{
    auto path_atom = ctx->pathAtom();
    if (path_atom.size() > 1) {
        std::vector<std::unique_ptr<RegularPathExpr>> sequence;
        if (current_path_inverse) {
            for (int i = path_atom.size() - 1; i >= 0; i--) {
                path_atom[i]->accept(this);
                sequence.push_back(std::move(current_path));
            }
        } else {
            for (auto a : path_atom) {
                a->accept(this);
                sequence.push_back(std::move(current_path));
            }
        }
        current_path = std::make_unique<PathSequence>(std::move(sequence));
    } else {
        path_atom[0]->accept(this);
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

Any QueryVisitor::visitExprVar(MQL_Parser::ExprVarContext* ctx)
{
    auto var_name = ctx->VARIABLE()->getText();
    var_name.erase(0, 1); // remove leading '?'
    auto var = get_query_ctx().get_or_create_var(var_name);

    if (auto key_terminal = ctx->KEY()) {
        auto key_name = key_terminal->getText();
        auto property_var_name = var_name + key_name;
        key_name.erase(0, 1); // remove leading '.'

        auto property_var = get_query_ctx().get_or_create_var(property_var_name);
        auto key_id = QuadObjectId::get_string(key_name);

        current_expr = std::make_unique<ExprVarProperty>(var, key_id, property_var);
    } else {
        current_expr = std::make_unique<ExprVar>(var);
    }
    return 0;
}

Any QueryVisitor::visitExprFixedObj(MQL_Parser::ExprFixedObjContext* ctx)
{
    auto oid = QuadObjectId::get_fixed_node_inside(ctx->getText());
    current_expr = std::make_unique<ExprConstant>(oid);

    return 0;
}

Any QueryVisitor::visitExprValue(MQL_Parser::ExprValueContext* ctx)
{
    visit(ctx->value());
    current_expr = std::make_unique<ExprConstant>(current_value_oid);

    return 0;
}

Any QueryVisitor::visitExprCount(MQL_Parser::ExprCountContext* ctx)
{
    bool distinct = ctx->K_DISTINCT() != nullptr;
    if (auto variable = ctx->VARIABLE()) {
        auto var_name = variable->getText();
        var_name.erase(0, 1); // remove leading '?'
        auto var = get_query_ctx().get_or_create_var(var_name);

        if (auto key_terminal = ctx->KEY()) {
            auto key_name = key_terminal->getText();
            auto property_var_name = var_name + key_name;
            key_name.erase(0, 1); // remove leading '.'

            auto property_var = get_query_ctx().get_or_create_var(property_var_name);
            auto key_id = QuadObjectId::get_string(key_name);

            current_expr = std::make_unique<ExprAggCount>(
                std::make_unique<ExprVarProperty>(var, key_id, property_var),
                distinct
            );
        } else {
            current_expr = std::make_unique<ExprAggCount>(std::make_unique<ExprVar>(var), distinct);
        }
    } else {
        current_expr = std::make_unique<ExprAggCountAll>(distinct);
    }
    return 0;
}

Any QueryVisitor::visitExprAgg(MQL_Parser::ExprAggContext* ctx)
{
    auto var_name = ctx->VARIABLE()->getText();
    var_name.erase(0, 1); // remove leading '?'
    auto var = get_query_ctx().get_or_create_var(var_name);
    std::string agg_inside_var_name = var_name;
    std::unique_ptr<Expr> inner_expr;
    if (auto key_terminal = ctx->KEY()) {
        auto key_name = key_terminal->getText();
        agg_inside_var_name += key_name;
        key_name.erase(0, 1); // remove leading '.'

        auto property_var = get_query_ctx().get_or_create_var(agg_inside_var_name);

        auto key_id = QuadObjectId::get_string(key_name);
        inner_expr = std::make_unique<ExprVarProperty>(var, key_id, property_var);
    } else {
        inner_expr = std::make_unique<ExprVar>(var);
    }

    auto agg_func = ctx->aggregateFunc();
    if (agg_func->K_AVG()) {
        current_expr = std::make_unique<ExprAggAvg>(std::move(inner_expr));
    } else if (agg_func->K_MAX()) {
        current_expr = std::make_unique<ExprAggMax>(std::move(inner_expr));
    } else if (agg_func->K_MIN()) {
        current_expr = std::make_unique<ExprAggMin>(std::move(inner_expr));
    } else if (agg_func->K_SUM()) {
        current_expr = std::make_unique<ExprAggSum>(std::move(inner_expr));
    } else {
        throw std::runtime_error("Unmanaged aggregateFunc");
    }
    return 0;
}

Any QueryVisitor::visitConditionalOrExpr(MQL_Parser::ConditionalOrExprContext* ctx)
{
    auto conditional_ands = ctx->conditionalAndExpr();
    conditional_ands[0]->accept(this);

    if (conditional_ands.size() > 1) {
        std::vector<std::unique_ptr<Expr>> or_list;
        or_list.push_back(std::move(current_expr));
        for (std::size_t i = 1; i < conditional_ands.size(); i++) {
            conditional_ands[i]->accept(this);
            or_list.push_back(std::move(current_expr));
        }
        current_expr = std::make_unique<ExprOr>(std::move(or_list));
    }

    return 0;
}

Any QueryVisitor::visitConditionalAndExpr(MQL_Parser::ConditionalAndExprContext* ctx)
{
    auto comparisons = ctx->comparisonExpr();
    comparisons[0]->accept(this);

    if (comparisons.size() > 1) {
        std::vector<std::unique_ptr<Expr>> and_list;
        and_list.push_back(std::move(current_expr));
        for (std::size_t i = 1; i < comparisons.size(); i++) {
            comparisons[i]->accept(this);
            and_list.push_back(std::move(current_expr));
        }
        current_expr = std::make_unique<ExprAnd>(std::move(and_list));
    }
    return 0;
}

Any QueryVisitor::visitComparisonExprOp(MQL_Parser::ComparisonExprOpContext* ctx)
{
    auto additive_exprs = ctx->additiveExpr();
    additive_exprs[0]->accept(this);
    if (additive_exprs.size() > 1) {
        auto saved_lhs = std::move(current_expr);
        additive_exprs[1]->accept(this);
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
            current_expr = std::make_unique<ExprGreaterOrEquals>(
                std::move(saved_lhs),
                std::move(current_expr)
            );
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
    auto expr_typename = ctx->exprTypename();
    if (expr_typename->K_BOOL()) {
        type = ExprIs::TypeName::BOOL;
        propertyTypeBitmap = propertyTypeBitmap | static_cast<uint64_t>(PropertyType::TYPE_BOOL);
    } else if (expr_typename->K_FLOAT()) {
        type = ExprIs::TypeName::FLOAT;
        propertyTypeBitmap = propertyTypeBitmap | static_cast<uint64_t>(PropertyType::TYPE_FLOAT);
    } else if (expr_typename->K_INTEGER()) {
        type = ExprIs::TypeName::INTEGER;
        propertyTypeBitmap = propertyTypeBitmap | static_cast<uint64_t>(PropertyType::TYPE_INTEGER);
    } else if (expr_typename->K_NULL()) {
        type = ExprIs::TypeName::NULL_;
        propertyTypeBitmap = propertyTypeBitmap | static_cast<uint64_t>(PropertyType::TYPE_NULL);
    } else if (expr_typename->K_STRING()) {
        type = ExprIs::TypeName::STRING;
        propertyTypeBitmap = propertyTypeBitmap | static_cast<uint64_t>(PropertyType::TYPE_STRING);
    } else {
        throw std::invalid_argument(
            "exprTypename '" + expr_typename->toString() + "' not recognized in ComparisonExprIs"
        );
    }

    bool not = ctx->K_NOT() != nullptr;
    if (not ) {
        propertyTypeBitmap = ~propertyTypeBitmap;
    }
    current_expr = std::make_unique<ExprIs>(not, std::move(current_expr), type, propertyTypeBitmap);

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
            current_expr = std::make_unique<ExprMultiplication>(
                std::move(saved_lhs),
                std::move(current_expr)
            );
        } else if (op == "/") {
            current_expr = std::make_unique<ExprDivision>(std::move(saved_lhs), std::move(current_expr));
        } else if (op == "%") {
            current_expr = std::make_unique<ExprModulo>(std::move(saved_lhs), std::move(current_expr));
        } else {
            throw std::invalid_argument(op + " not recognized as a valid MultiplicativeExpr operator");
        }
    }
    return 0;
}

Any QueryVisitor::visitUnaryExpr(MQL_Parser::UnaryExprContext* ctx)
{
    if (auto unary_expr = ctx->unaryExpr()) {
        unary_expr->accept(this);
        auto unary_op = ctx->children[0]->getText();
        if (ctx->K_NOT() != nullptr) {
            current_expr = std::make_unique<ExprNot>(std::move(current_expr));
        } else if (unary_op == "+") {
            current_expr = std::make_unique<ExprUnaryPlus>(std::move(current_expr));
        } else if (unary_op == "-") {
            current_expr = std::make_unique<ExprUnaryMinus>(std::move(current_expr));
        } else {
            throw std::invalid_argument(unary_op + " not recognized as a valid UnaryExpr operator");
        }
    } else {
        ctx->atomicExpr()->accept(this);
    }
    return 0;
}

Any QueryVisitor::visitRegex(MQL_Parser::RegexContext* ctx)
{
    auto conditional_ors = ctx->conditionalOrExpr();
    visit(conditional_ors[0]);
    auto expr0 = std::move(current_expr);

    visit(conditional_ors[1]);
    auto expr1 = std::move(current_expr);

    if (conditional_ors.size() == 3) {
        visit(conditional_ors[2]);
        auto expr2 = std::move(current_expr);
        current_expr = std::make_unique<ExprRegex>(std::move(expr0), std::move(expr1), std::move(expr2));
    } else {
        current_expr = std::make_unique<ExprRegex>(std::move(expr0), std::move(expr1));
    }

    return 0;
}

Any QueryVisitor::visitCosineSimilarity(MQL_Parser::CosineSimilarityContext* ctx)
{
    auto conditional_ors = ctx->conditionalOrExpr();
    visit(conditional_ors[0]);
    auto expr0 = std::move(current_expr);

    visit(conditional_ors[1]);
    auto expr1 = std::move(current_expr);

    current_expr = std::make_unique<ExprCosineSimilarity>(std::move(expr0), std::move(expr1));

    return 0;
}

Any QueryVisitor::visitCosineDistance(MQL_Parser::CosineDistanceContext* ctx)
{
    auto conditional_ors = ctx->conditionalOrExpr();
    visit(conditional_ors[0]);
    auto expr0 = std::move(current_expr);

    visit(conditional_ors[1]);
    auto expr1 = std::move(current_expr);

    current_expr = std::make_unique<ExprCosineDistance>(std::move(expr0), std::move(expr1));

    return 0;
}

Any QueryVisitor::visitManhattanDistance(MQL_Parser::ManhattanDistanceContext* ctx)
{
    auto conditional_ors = ctx->conditionalOrExpr();
    visit(conditional_ors[0]);
    auto expr0 = std::move(current_expr);

    visit(conditional_ors[1]);
    auto expr1 = std::move(current_expr);

    current_expr = std::make_unique<ExprManhattanDistance>(std::move(expr0), std::move(expr1));

    return 0;
}

Any QueryVisitor::visitEuclideanDistance(MQL_Parser::EuclideanDistanceContext* ctx)
{
    auto conditional_ors = ctx->conditionalOrExpr();
    visit(conditional_ors[0]);
    auto expr0 = std::move(current_expr);

    visit(conditional_ors[1]);
    auto expr1 = std::move(current_expr);

    current_expr = std::make_unique<ExprEuclideanDistance>(std::move(expr0), std::move(expr1));

    return 0;
}

Any QueryVisitor::visitEditDistance(MQL_Parser::EditDistanceContext* ctx)
{
    visit(ctx->conditionalOrExpr(0));
    auto expr0 = std::move(current_expr);

    visit(ctx->conditionalOrExpr(1));
    auto expr1 = std::move(current_expr);

    current_expr = std::make_unique<ExprEditDistance>(std::move(expr0), std::move(expr1));

    return 0;
}

Any QueryVisitor::visitNormalize(MQL_Parser::NormalizeContext* ctx)
{
    visit(ctx->conditionalOrExpr());

    current_expr = std::make_unique<ExprNormalize>(std::move(current_expr));

    return 0;
}

Any QueryVisitor::visitStr(MQL_Parser::StrContext* ctx)
{
    visit(ctx->conditionalOrExpr());

    current_expr = std::make_unique<ExprStr>(std::move(current_expr));

    return 0;
}

Any QueryVisitor::visitLabels(MQL_Parser::LabelsContext* ctx)
{
    visit(ctx->conditionalOrExpr());

    current_expr = std::make_unique<ExprLabels>(std::move(current_expr));

    return 0;
}

Any QueryVisitor::visitType(MQL_Parser::TypeContext* ctx)
{
    visit(ctx->conditionalOrExpr());

    current_expr = std::make_unique<ExprType>(std::move(current_expr));

    return 0;
}

Any QueryVisitor::visitPropertiesFunction(MQL_Parser::PropertiesFunctionContext* ctx)
{
    visit(ctx->conditionalOrExpr());

    current_expr = std::make_unique<ExprProperties>(std::move(current_expr));

    return 0;
}

template<typename IndexOptions, typename OptionHandlerFunc>
void QueryVisitor::parse_index_options(
    MQL_Parser::CreateIndexOptionsContext* ctx,
    IndexOptions& index_opts,
    OptionHandlerFunc option_handler_func
)
{
    assert(ctx != nullptr);
    for (const auto& opt : ctx->createIndexOption()) {
        std::string key = opt->STRING()->getText();
        key = key.substr(1, key.size() - 2);

        if (!index_opts.opt2seen.contains(key)) {
            const auto valid_options = index_opts.valid_options();
            std::string valid_options_str;
            valid_options_str += valid_options[0];
            for (std::size_t i = 1; i < valid_options.size(); ++i) {
                valid_options_str += ", " + valid_options[i];
            }
            throw QueryException(
                "Unexpected index option: \"" + key + "\". Expected one of the following: { "
                + valid_options_str + " }"
            );
        }

        if (index_opts.opt2seen[key] == true) {
            throw QueryException("Duplicate index option: \"" + key + "\"");
        }

        option_handler_func(key, opt->value());

        index_opts.opt2seen[key] = true;
    }

    index_opts.validate();
}

Any QueryVisitor::visitCreateIndexQuery(MQL_Parser::CreateIndexQueryContext* ctx)
{
    update_info.update_ctx = std::make_unique<UpdateContext>();

    std::string index_name = ctx->STRING()->getText();
    index_name = index_name.substr(1, index_name.size() - 2);

    if (!misc::is_name_valid_for_path(index_name)) {
        throw QueryException("Invalid index name: \"" + index_name + "\"");
    }

    const std::string index_type = ctx->identifier()->getText();
    const std::string index_type_lowercased = Transliterator::lowercase(index_type);

    // converts a value context to the contents of its string, throws if not possible
    auto value_to_str = [&](MQL_Parser::ValueContext* ctx, const std::string& key) -> std::string {
        visit(ctx);
        const auto gen_sub_t = current_value_oid.id & ObjectId::SUB_TYPE_MASK;
        switch (gen_sub_t) {
        case ObjectId::MASK_STRING_SIMPLE:
            return MQL::Conversions::unpack_string(current_value_oid);
        default:
            break;
        }

        throw QueryException("index option \"" + key + "\" is expected to be a string");
    };

    // converts a value context to a uint64_t, throws if not possible
    auto value_to_uint64 = [&](MQL_Parser::ValueContext* ctx, const std::string& key) -> uint64_t {
        visit(ctx);
        const auto gen_sub_t = current_value_oid.id & ObjectId::SUB_TYPE_MASK;
        switch (gen_sub_t) {
        case ObjectId::MASK_INT:
            return MQL::Conversions::unpack_int(current_value_oid);
        default:
            break;
        }

        throw QueryException("index option \"" + key + "\" is expected to be an unsigned integer");
    };

    // Check if index name is used
    if (quad_model.catalog.index_name_exists(index_name)) {
        throw QueryException("Index \"" + index_name + "\" already exists");
    }

    if (index_type_lowercased == "text") {
        TextIndexOptions text_index_opts;

        parse_index_options(
            ctx->createIndexOptions(),
            text_index_opts,
            [&](const std::string& key, MQL_Parser::ValueContext* valueCtx) {
                if (key == "property") {
                    text_index_opts.property = value_to_str(valueCtx, key);
                } else if (key == "normalization") {
                    text_index_opts.normalization = value_to_str(valueCtx, key);
                } else if (key == "tokenization") {
                    text_index_opts.tokenization = value_to_str(valueCtx, key);
                } else {
                    assert(false && "unhandled text index option");
                }
            }
        );

        TextSearch::NORMALIZE_TYPE normalize_type;
        if (text_index_opts.normalization == "nfkdCasefold") {
            normalize_type = TextSearch::NORMALIZE_TYPE::NFKD_CASEFOLD;
        } else {
            assert(text_index_opts.normalization == "identity");
            normalize_type = TextSearch::NORMALIZE_TYPE::IDENTITY;
        }

        TextSearch::TOKENIZE_TYPE tokenize_type;
        if (text_index_opts.tokenization == "wsSplitPunc") {
            tokenize_type = TextSearch::TOKENIZE_TYPE::WHITESPACE_SPLIT_PUNCTUATION;
            // identity, wsSplitPunc, wsRmPunc, wsKeepPunc
        } else if (text_index_opts.tokenization == "wsRmPunc") {
            tokenize_type = TextSearch::TOKENIZE_TYPE::WHITESPACE_REMOVE_PUNCTUATION;
        } else if (text_index_opts.tokenization == "wsKeepPunc") {
            tokenize_type = TextSearch::TOKENIZE_TYPE::WHITESPACE_KEEP_PUNCTUATION;
        } else {
            assert(text_index_opts.tokenization == "identity");
            tokenize_type = TextSearch::TOKENIZE_TYPE::IDENTITY;
        }

        update_info.update_actions.push_back(std::make_unique<CreateTextIndex>(
            std::move(index_name),
            std::move(text_index_opts.property),
            normalize_type,
            tokenize_type
        ));

    } else if (index_type_lowercased == "hnsw") {
        HNSWIndexOptions hnsw_index_opts;

        parse_index_options(
            ctx->createIndexOptions(),
            hnsw_index_opts,
            [&](const std::string& key, MQL_Parser::ValueContext* valueCtx) {
                if (key == "property") {
                    hnsw_index_opts.property = value_to_str(valueCtx, key);
                } else if (key == "metric") {
                    hnsw_index_opts.metric = value_to_str(valueCtx, key);
                } else if (key == "dimension") {
                    hnsw_index_opts.dimension = value_to_uint64(valueCtx, key);
                } else if (key == "maxEdges") {
                    hnsw_index_opts.max_edges = value_to_uint64(valueCtx, key);
                } else if (key == "maxCandidates") {
                    hnsw_index_opts.max_candidates = value_to_uint64(valueCtx, key);
                } else {
                    assert(false && "unhandled hnsw index option");
                }
            }
        );

        HNSW::MetricType metric_type;
        if (hnsw_index_opts.metric == "cosineDistance") {
            metric_type = HNSW::MetricType::COSINE_DISTANCE;
        } else if (hnsw_index_opts.metric == "manhattanDistance") {
            metric_type = HNSW::MetricType::MANHATTAN_DISTANCE;
        } else {
            assert(hnsw_index_opts.metric == "euclideanDistance");
            metric_type = HNSW::MetricType::EUCLIDEAN_DISTANCE;
        }

        update_info.update_actions.push_back(std::make_unique<CreateHNSWIndex>(
            std::move(index_name),
            std::move(hnsw_index_opts.property),
            hnsw_index_opts.dimension,
            hnsw_index_opts.max_edges,
            hnsw_index_opts.max_candidates,
            metric_type
        ));
    } else {
        throw QueryException("Invalid index type \"" + index_type + "\"");
    }

    current_op = std::make_unique<OpUpdate>(
        std::make_unique<OpUnitTable>(),
        std::move(update_info.update_ctx),
        std::move(update_info.update_actions)
    );

    return 0;
}

Any QueryVisitor::visitValue(MQL_Parser::ValueContext* ctx)
{
    if (auto datatype_value = ctx->datatypeValue()) {
        auto datatype = datatype_value->identifier()->getText();
        auto str = datatype_value->STRING()->getText();
        str = str.substr(1, str.size() - 2); // remove double quotes

        parse_datatype_value(datatype, str); // will set current_value_oid
    } else {
        current_value_oid = QuadObjectId::get_value(ctx->getText());
    }

    return 0;
}

void QueryVisitor::parse_datatype_value(const std::string& datatype, const std::string& str)
{
    if (datatype == "date") {
        current_value_oid = ObjectId(DateTime::from_date(str));
        if (current_value_oid.is_null()) {
            throw QueryException("Invalid date value: " + str);
        }
    } else if (datatype == "dateTime") {
        current_value_oid = ObjectId(DateTime::from_dateTime(str));
        if (current_value_oid.is_null()) {
            throw QueryException("Invalid dateTime value: " + str);
        }
    } else if (datatype == "dateTimeStamp") {
        current_value_oid = ObjectId(DateTime::from_dateTimeStamp(str));
        if (current_value_oid.is_null()) {
            throw QueryException("Invalid dateTimeStamp value: " + str);
        }
    } else if (datatype == "time") {
        current_value_oid = ObjectId(DateTime::from_time(str));
        if (current_value_oid.is_null()) {
            throw QueryException("Invalid time value: " + str);
        }
    } else if (datatype == "tensorFloat") {
        bool error;
        const auto tensor = tensor::Tensor<float>::from_literal(str, &error);
        if (error) {
            throw QueryException("Invalid tensor float value: " + str);
        }
        current_value_oid = Common::Conversions::pack_tensor(tensor);
    } else if (datatype == "tensorDouble") {
        bool error;
        const auto tensor = tensor::Tensor<double>::from_literal(str, &error);
        if (error) {
            throw QueryException("Invalid tensor double value: " + str);
        }
        current_value_oid = Common::Conversions::pack_tensor(tensor);
    } else {
        throw QueryException("Unrecognized datatype: " + datatype);
    }
}

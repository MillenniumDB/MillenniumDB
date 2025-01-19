#include "query_visitor.h"

#include <memory>

#include "graph_models/common/conversions.h"
#include "graph_models/gql/conversions.h"
#include "graph_models/rdf_model/conversions.h"
#include "query/parser/expr/gql_exprs.h"
#include "query/parser/op/gql/ops.h"

// #define DEBUG_GQL_QUERY_VISITOR

#ifdef DEBUG_GQL_QUERY_VISITOR

struct VisitorLogger {
    std::string context;
    static uint64_t indentation_level;

    VisitorLogger(std::string context) :
        context(context)
    {
        std::cout << std::string(indentation_level * 2, ' ') << "> " << context << std::endl;
        indentation_level++;
    }

    ~VisitorLogger()
    {
        indentation_level--;
        std::cout << std::string(indentation_level * 2, ' ') << "< " << std::endl;
    }
};
uint64_t VisitorLogger::indentation_level = 0;

#define LOG_VISITOR VisitorLogger logger_instance(__func__);
#define LOG_INFO(arg) std::cout << std::string(VisitorLogger::indentation_level * 2, ' ') << arg << std::endl

#else
#define LOG_VISITOR
#define LOG_INFO(arg)
#endif

using namespace GQL;
using antlrcpp::Any;

std::any QueryVisitor::visitPrimitiveQueryStatement(GQLParser::PrimitiveQueryStatementContext* ctx)
{
    LOG_VISITOR
    if (ctx->matchStatement()){
        ctx->matchStatement()->accept(this);
    }
    if (ctx->orderByAndPageStatement()){
        ctx->orderByAndPageStatement()->accept(this);
        current_op = std::make_unique<OpOrderBy>(
            std::move(current_op),
            std::move(order_by_items),
            std::move(order_by_ascending),
            std::move(order_nulls)
        );
    }
    return 0;
}

std::any QueryVisitor::visitMatchStatement(GQLParser::MatchStatementContext* ctx)
{
    LOG_VISITOR
    visitChildren(ctx);
    return 0;
}

std::any QueryVisitor::visitPathPatternList(GQLParser::PathPatternListContext* ctx)
{
    LOG_VISITOR
    std::vector<std::unique_ptr<Op>> basic_graph_patterns;

    for (auto pattern : ctx->pathPattern()) {
        visit(pattern);
        basic_graph_patterns.push_back(std::move(current_op));
    }

    if (basic_graph_patterns.size() == 1) {
        current_op = std::move(basic_graph_patterns[0]);
    } else {
        current_op = std::make_unique<OpGraphPatternList>(std::move(basic_graph_patterns));
    }

    return 0;
}

std::any QueryVisitor::visitPathPatternUnion(GQLParser::PathPatternUnionContext* ctx)
{
    LOG_VISITOR
    std::vector<std::unique_ptr<Op>> basic_graph_patterns;

    for (auto pattern : ctx->pathTerm()) {
        visit(pattern);
        basic_graph_patterns.push_back(std::move(current_op));
    }

    if (basic_graph_patterns.size() == 1) {
        current_op = std::move(basic_graph_patterns[0]);
    } else {
        current_op = std::make_unique<OpPathUnion>(std::move(basic_graph_patterns));
    }

    current_pattern = Other;
    return 0;
}

std::any QueryVisitor::visitReturnStatementBody(GQLParser::ReturnStatementBodyContext* ctx)
{
    LOG_VISITOR
    // No bindings
    if (ctx->NO()) {
        current_op = std::make_unique<OpReturn>(std::move(current_op), std::vector<OpReturn::Item> {});
        return 0;
    }

    if (ctx->setQuantifier()) {
    }

    if (ctx->ASTERISK()) {
        std::vector<OpReturn::Item> return_items;

        auto vars = current_op->get_all_vars();
        for (auto& var : vars) {
            std::string var_name = get_query_ctx().get_var_name(var);

            // filter anonymous vars and vars with properties
            if (!get_query_ctx().is_internal(var) && var_name.find('.') == var_name.npos) {
                return_items.emplace_back(std::make_unique<ExprVar>(var), std::nullopt);
            }
        }
        current_op = std::make_unique<OpReturn>(std::move(current_op), std::move(return_items));
    }

    if (ctx->returnItemList()) {
        visit(ctx->returnItemList());
    }

    return 0;
}

std::any QueryVisitor::visitReturnItemList(GQLParser::ReturnItemListContext* ctx)
{
    LOG_VISITOR
    std::vector<OpReturn::Item> return_items;

    for (auto& item : ctx->returnItem()) {
        visit(item);

        std::optional<VarId> alias = {};
        if (item->returnItemAlias() != nullptr) {
            // TODO: throw an exception if the alias is already defined
            std::string alias_str = item->returnItemAlias()->identifier()->getText();
            alias = get_query_ctx().get_or_create_var(alias_str);
        }
        return_items.emplace_back(std::move(current_expr), alias);
    }

    current_op = std::make_unique<OpReturn>(std::move(current_op), std::move(return_items));
    return 0;
}

OpEdge QueryVisitor::create_edge(VarId left_node, VarId right_node, VarId edge_id, EdgeType type)
{
    switch (type) {
    case EdgePointingLeft: {
        return OpEdge(right_node, left_node, edge_id, OpEdge::Directed);
    }
    case EdgeUndirected: {
        return OpEdge(left_node, right_node, edge_id, OpEdge::Undirected);
    }
    case EdgePointingRight: {
        return OpEdge(left_node, right_node, edge_id, OpEdge::Directed);
    }
    case EdgeLeftOrUndirected: {
        return OpEdge(right_node, left_node, edge_id, OpEdge::UndirectedOrDirectedTo);
    }
    case EdgeUndirectedOrRight: {
        return OpEdge(left_node, right_node, edge_id, OpEdge::UndirectedOrDirectedTo);
    }
    case EdgeLeftOrRight: {
        return OpEdge(left_node, right_node, edge_id, OpEdge::DirectedLeftOrRight);
    }
    case EdgeAnyDirection:
        return OpEdge(left_node, right_node, edge_id, OpEdge::AnyDirection);
    }
}

std::any QueryVisitor::visitPathPattern(GQLParser::PathPatternContext* ctx)
{
    LOG_VISITOR
    visit(ctx->pathPatternExpression());

    if (ctx->pathPatternPrefix()) {
        visit(ctx->pathPatternPrefix());
    } else {
        path_mode = PathMode();
    }

    if (ctx->pathVariableDeclaration()) {
        visit(ctx->pathVariableDeclaration());
        current_op = std::make_unique<OpGraphPattern>(std::move(current_op), path_mode, *current_id);
    } else {
        current_op = std::make_unique<OpGraphPattern>(std::move(current_op), path_mode);
    }
    return 0;
}

std::any QueryVisitor::visitParenthesizedPathPatternExpression(
    GQLParser::ParenthesizedPathPatternExpressionContext* ctx
)
{
    LOG_VISITOR
    visit(ctx->pathPatternExpression());

    if (ctx->pathModePrefix()) {
        visit(ctx->pathModePrefix());
    } else {
        path_mode = PathMode();
    }

    if (ctx->subpathVariableDeclaration()) {
        visit(ctx->subpathVariableDeclaration());
        current_op = std::make_unique<OpGraphPattern>(std::move(current_op), path_mode, *current_id);
    } else {
        current_op = std::make_unique<OpGraphPattern>(std::move(current_op), path_mode);
    }

    if (ctx->parenthesizedPathPatternWhereClause()) {
        visit(ctx->parenthesizedPathPatternWhereClause());
        current_expr_list.push_back(std::move(current_expr));
        current_op = std::make_unique<OpFilter>(std::move(current_op), std::move(current_expr_list));
    }

    current_pattern = Subpath;
    return 0;
}

std::any QueryVisitor::visitAllPathSearch(GQLParser::AllPathSearchContext* ctx)
{
    LOG_VISITOR
    path_mode.prefix = PathMode::ALL;
    if (ctx->pathMode()) {
        visit(ctx->pathMode());
    }
    return 0;
}

std::any QueryVisitor::visitAnyPathSearch(GQLParser::AnyPathSearchContext* ctx)
{
    LOG_VISITOR
    path_mode.prefix = PathMode::ANY;

    if (ctx->numberOfPaths()) {
        path_mode.path_count = std::stoull(ctx->numberOfPaths()->getText());
    }

    if (ctx->pathMode()) {
        visit(ctx->pathMode());
    }

    return 0;
}

std::any QueryVisitor::visitShortestPathSearch(GQLParser::ShortestPathSearchContext* ctx)
{
    LOG_VISITOR
    if (ctx->allShortestPathSearch()) {
        path_mode.prefix = PathMode::ALL;
        if (ctx->allShortestPathSearch()->pathMode()) {
            visit(ctx->allShortestPathSearch()->pathMode());
        }

    } else if (ctx->anyShortestPathSearch()) {
        path_mode.prefix = PathMode::ANY;
        if (ctx->anyShortestPathSearch()->pathMode()) {
            visit(ctx->anyShortestPathSearch()->pathMode());
        }

    } else if (ctx->countedShortestPathSearch()) {
        path_mode.prefix = PathMode::SHORTEST_PATH_COUNT;
        if (ctx->countedShortestPathSearch()->pathMode()) {
            visit(ctx->countedShortestPathSearch()->pathMode());
        }

        path_mode.path_count = std::stoull(ctx->countedShortestPathSearch()->numberOfPaths()->getText());

    } else if (ctx->countedShortestGroupSearch()) {
        path_mode.prefix = PathMode::SHORTEST_GROUP_COUNT;
        if (ctx->countedShortestGroupSearch()->pathMode()) {
            visit(ctx->countedShortestGroupSearch()->pathMode());
        }

        path_mode.path_count = std::stoull(ctx->countedShortestGroupSearch()->numberOfGroups()->getText());
    }

    return 0;
}

std::any QueryVisitor::visitPathMode(GQLParser::PathModeContext* ctx)
{
    LOG_VISITOR
    if (ctx->WALK()) {
        path_mode.mode = PathMode::WALK;
    } else if (ctx->TRAIL()) {
        path_mode.mode = PathMode::TRAIL;
    } else if (ctx->SIMPLE()) {
        path_mode.mode = PathMode::SIMPLE;
    } else if (ctx->ACYCLIC()) {
        path_mode.mode = PathMode::ACYCLIC;
    }

    return 0;
}

std::any QueryVisitor::visitPathVariableDeclaration(GQLParser::PathVariableDeclarationContext* ctx)
{
    LOG_VISITOR
    auto var_name = get_query_ctx().get_or_create_var(ctx->pathVariable()->getText());
    current_id = std::make_unique<VarId>(var_name);
    return 0;
}

std::any QueryVisitor::visitSubpathVariableDeclaration(GQLParser::SubpathVariableDeclarationContext* ctx)
{
    LOG_VISITOR
    auto var_name = get_query_ctx().get_or_create_var(ctx->subpathVariable()->getText());
    current_id = std::make_unique<VarId>(var_name);
    return 0;
}

std::any QueryVisitor::visitPathTerm(GQLParser::PathTermContext* ctx)
{
    LOG_VISITOR
    auto parent_bgp = std::move(current_basic_graph_pattern);
    current_basic_graph_pattern = std::make_unique<OpBasicGraphPattern>();

    std::vector<std::unique_ptr<Expr>> parent_expr_list = std::move(current_expr_list);

    std::vector<PatternType> pattern_types(ctx->children.size());
    std::vector<EdgeType> edge_types(ctx->children.size());
    std::vector<VarId> pattern_ids(ctx->children.size(), VarId(0));

    std::vector<std::unique_ptr<Repetition>> edge_repetitions(ctx->children.size());

    // visit all children and create nodes and subpaths
    for (size_t i = 0; i < ctx->children.size(); i++) {
        current_repetition = nullptr;
        visit(ctx->children[i]);

        switch (current_pattern) {
        case Node: {
            if (current_repetition) {
                throw QuerySemanticException("A repeated pattern should have path length greater than 0.");
            }
            auto node = OpNode(*current_id);
            current_basic_graph_pattern->add_pattern(std::make_unique<OpNode>(node));

            pattern_ids[i] = *current_id;
            pattern_types[i] = Node;
            singleton_types[*current_id] = VarType::Node;
            break;
        }
        case Edge: {
            pattern_ids[i] = *current_id;
            pattern_types[i] = Edge;
            edge_types[i] = edge_type;
            singleton_types[*current_id] = VarType::Edge;
            if (current_repetition) {
                edge_repetitions[i] = std::move(current_repetition);
            }
            break;
        }
        case Subpath: {
            if (current_repetition) {
                current_basic_graph_pattern->add_pattern(
                    std::make_unique<OpRepetition>(std::move(current_op), std::move(current_repetition))
                );
            } else {
                current_basic_graph_pattern->add_pattern(std::move(current_op));
            }
            pattern_types[i] = Subpath;
            break;
        }
        case Other: {
        }
        }
    }

    VarId left_node(0);
    VarId right_node(0);

    for (size_t i = 0; i < ctx->children.size(); i++) {
        if (pattern_types[i] == Edge) {
            // store the edge id
            VarId edge_id = pattern_ids[i];

            // find the left node id
            if (i > 0 && pattern_types[i - 1] == Node) {
                left_node = pattern_ids[i - 1];
            } else {
                auto node = OpNode(get_query_ctx().get_internal_var());
                current_basic_graph_pattern->add_pattern(std::make_unique<OpNode>(node));
                left_node = node.id;
            }

            // find the right node id
            if (i + 1 < ctx->children.size() && pattern_types[i + 1] == Node) {
                right_node = pattern_ids[i + 1];
            } else {
                auto node = OpNode(get_query_ctx().get_internal_var());
                current_basic_graph_pattern->add_pattern(std::make_unique<OpNode>(node));
                right_node = node.id;

                // set the node as previous pattern
                pattern_ids[i] = node.id;
                pattern_types[i] = Node;
            }

            // create the edge
            OpEdge edge = create_edge(left_node, right_node, edge_id, edge_types[i]);
            if (edge_repetitions[i]) {
                current_basic_graph_pattern->add_pattern(std::make_unique<OpRepetition>(
                    std::make_unique<OpEdge>(edge),
                    std::move(edge_repetitions[i])
                ));
            } else {
                current_basic_graph_pattern->add_pattern(std::make_unique<OpEdge>(edge));
            }
        }
    }

    if (current_expr_list.empty()) {
        current_op = std::move(current_basic_graph_pattern);
    } else {
        current_op = std::make_unique<OpFilter>(
            std::move(current_basic_graph_pattern),
            std::move(current_expr_list)
        );
    }

    current_basic_graph_pattern = std::move(parent_bgp);
    current_expr_list = std::move(parent_expr_list);

    return 0;
}

std::any QueryVisitor::visitNodePattern(GQLParser::NodePatternContext* ctx)
{
    LOG_VISITOR
    current_pattern = Node;
    visitChildren(ctx);
    return 0;
}

std::any QueryVisitor::visitEdgePattern(GQLParser::EdgePatternContext* ctx)
{
    LOG_VISITOR
    current_pattern = Edge;
    if (ctx->fullEdgePattern()) {
        visitChildren(ctx);
        auto pattern_ctx = ctx->fullEdgePattern();

        if (pattern_ctx->fullEdgePointingLeft()) {
            edge_type = EdgePointingLeft;
        } else if (pattern_ctx->fullEdgeUndirected()) {
            edge_type = EdgeUndirected;
        } else if (pattern_ctx->fullEdgePointingRight()) {
            edge_type = EdgePointingRight;
        } else if (pattern_ctx->fullEdgeLeftOrUndirected()) {
            edge_type = EdgeLeftOrUndirected;
        } else if (pattern_ctx->fullEdgeUndirectedOrRight()) {
            edge_type = EdgeUndirectedOrRight;
        } else if (pattern_ctx->fullEdgeLeftOrRight()) {
            edge_type = EdgeLeftOrRight;
        } else if (pattern_ctx->fullEdgeAnyDirection()) {
            edge_type = EdgeAnyDirection;
        }
    } else if (ctx->abbreviatedEdgePattern()) {
        current_id = std::make_unique<VarId>(get_query_ctx().get_internal_var());

        std::string pattern_text = ctx->abbreviatedEdgePattern()->getText();

        if (pattern_text == "<-") {
            edge_type = EdgePointingLeft;
        } else if (pattern_text == "~") {
            edge_type = EdgeUndirected;
        } else if (pattern_text == "->") {
            edge_type = EdgePointingRight;
        } else if (pattern_text == "<~") {
            edge_type = EdgeLeftOrUndirected;
        } else if (pattern_text == "~>") {
            edge_type = EdgeUndirectedOrRight;
        } else if (pattern_text == "<->") {
            edge_type = EdgeLeftOrRight;
        } else if (pattern_text == "-") {
            edge_type = EdgeAnyDirection;
        }
    }

    return 0;
}

std::any QueryVisitor::visitElementPatternFiller(GQLParser::ElementPatternFillerContext* ctx)
{
    LOG_VISITOR
    if (ctx->elementVariableDeclaration()) {
        visit(ctx->elementVariableDeclaration());
    } else {
        current_id = std::make_unique<VarId>(get_query_ctx().get_internal_var());
    }

    // set the type before visiting the label expression and the where clause
    if (current_pattern == Node) {
        singleton_types[*current_id] = VarType::Node;
    } else {
        singleton_types[*current_id] = VarType::Edge;
    }

    if (ctx->isLabelExpression()) {
        current_label_var_id = std::make_unique<VarId>(*current_id);
        visit(ctx->isLabelExpression());
        current_expr_list.push_back(std::move(current_expr));
    } else {
    }

    if (ctx->elementPatternPredicate()) {
        visit(ctx->elementPatternPredicate());
        current_expr_list.push_back(std::move(current_expr));
    }

    return 0;
}

std::any QueryVisitor::visitQuestionedPathPrimary(GQLParser::QuestionedPathPrimaryContext* ctx)
{
    LOG_VISITOR
    visit(ctx->pathPrimary());
    current_repetition = std::make_unique<Repetition>(0, 1);
    return 0;
}

std::any QueryVisitor::visitGraphPatternQuantifier(GQLParser::GraphPatternQuantifierContext* ctx)
{
    LOG_VISITOR
    if (ctx->ASTERISK()) {
        current_repetition = std::make_unique<Repetition>(0, std::nullopt);
    } else if (ctx->PLUS_SIGN()) {
        current_repetition = std::make_unique<Repetition>(1, std::nullopt);
    } else if (ctx->fixedQuantifier()) {
        std::string integer_str = ctx->fixedQuantifier()->UNSIGNED_DECIMAL_INTEGER()->getText();

        integer_str.erase(std::remove(integer_str.begin(), integer_str.end(), '_'), integer_str.end());
        uint64_t integer = std::stoull(integer_str);

        current_repetition = std::make_unique<Repetition>(integer, integer);

    } else if (ctx->generalQuantifier()) {
        uint64_t lower = 0;
        std::optional<uint64_t> upper = std::nullopt;

        if (ctx->generalQuantifier()->lowerBound()) {
            std::string integer_str = ctx->generalQuantifier()->lowerBound()->getText();

            integer_str.erase(std::remove(integer_str.begin(), integer_str.end(), '_'), integer_str.end());
            lower = std::stoull(integer_str);
        }
        if (ctx->generalQuantifier()->upperBound()) {
            std::string integer_str = ctx->generalQuantifier()->upperBound()->getText();

            integer_str.erase(std::remove(integer_str.begin(), integer_str.end(), '_'), integer_str.end());
            upper = std::stoull(integer_str);
        }

        if (upper.has_value() && lower > upper) {
            throw QuerySemanticException("The lower bound in the quantifier is greater than the upper bound."
            );
        }

        current_repetition = std::make_unique<Repetition>(lower, upper);
    }

    return 0;
}

std::any QueryVisitor::visitGraphPatternWhereClause(GQLParser::GraphPatternWhereClauseContext* ctx)
{
    LOG_VISITOR
    visitChildren(ctx);
    current_expr_list.push_back(std::move(current_expr));
    current_op = std::make_unique<OpFilter>(std::move(current_op), std::move(current_expr_list));
    return 0;
}

std::any QueryVisitor::visitParenthesizedPathPatternWhereClause(
    GQLParser::ParenthesizedPathPatternWhereClauseContext* ctx
)
{
    LOG_VISITOR
    visitChildren(ctx);
    // current_expr_list.push_back(std::move(current_expr));
    // current_op = std::make_unique<OpFilter>(std::move(current_op), std::move(current_expr_list));
    return 0;
}

std::any QueryVisitor::visitElementPatternWhereClause(GQLParser::ElementPatternWhereClauseContext* ctx)
{
    LOG_VISITOR
    visit(ctx->expression());
    return 0;
}

// Child of elementPropertySpecification
std::any QueryVisitor::visitPropertyKeyValuePairList(GQLParser::PropertyKeyValuePairListContext* ctx)
{
    LOG_VISITOR
    std::vector<std::unique_ptr<Expr>> and_expressions;

    for (auto& factor : ctx->propertyKeyValuePair()) {
        visit(factor);
        and_expressions.push_back(std::move(current_expr));
    }
    if (and_expressions.size() == 1) {
        current_expr = std::move(and_expressions[0]);
    } else {
        current_expr = std::make_unique<ExprAnd>(std::move(and_expressions));
    }

    return 0;
}

std::any QueryVisitor::visitPropertyKeyValuePair(GQLParser::PropertyKeyValuePairContext* ctx)
{
    LOG_VISITOR
    visit(ctx->expression());
    auto key = ctx->propertyName()->getText();

    std::string var_name = get_query_ctx().get_var_name(*current_id);
    VarId property_var = get_query_ctx().get_or_create_var(var_name + "." + key);

    switch (current_pattern) {
    case Node:
        current_expr = std::make_unique<ExprEquals>(
            std::make_unique<ExprProperty>(
                *current_id,
                Conversions::pack_node_property(key),
                property_var,
                VarType::Node
            ),
            std::move(current_expr)
        );
        break;
    case Edge:
        current_expr = std::make_unique<ExprEquals>(
            std::make_unique<ExprProperty>(
                *current_id,
                Conversions::pack_edge_property(key),
                property_var,
                VarType::Edge
            ),
            std::move(current_expr)
        );
        break;
    case Other:
    case Subpath:
        break;
    }

    return 0;
}

std::any QueryVisitor::visitElementVariableDeclaration(GQLParser::ElementVariableDeclarationContext* ctx)
{
    LOG_VISITOR
    if (ctx->TEMP()) {
    }

    std::string var_name = ctx->elementVariable()->getText();
    auto new_var = get_query_ctx().get_or_create_var(var_name);
    current_id = std::make_unique<VarId>(new_var);
    return 0;
}

std::any QueryVisitor::visitIsLabelExpression(GQLParser::IsLabelExpressionContext* ctx)
{
    LOG_VISITOR
    if (ctx->isOrColon()->COLON()) {
    } else if (ctx->isOrColon()->IS()) {
    }

    visit(ctx->labelExpression());
    return 0;
}

std::any QueryVisitor::visitLabelExpression(GQLParser::LabelExpressionContext* ctx)
{
    LOG_VISITOR
    std::vector<std::unique_ptr<Expr>> or_expressions;
    for (auto& term : ctx->labelTerm()) {
        visit(term);
        or_expressions.push_back(std::move(current_expr));
    }

    if (or_expressions.size() == 1) {
        current_expr = std::move(or_expressions[0]);
    } else {
        current_expr = std::make_unique<ExprOr>(std::move(or_expressions));
    }
    return 0;
}

std::any QueryVisitor::visitLabelTerm(GQLParser::LabelTermContext* ctx)
{
    LOG_VISITOR
    std::vector<std::unique_ptr<Expr>> and_expressions;

    for (auto& factor : ctx->labelFactor()) {
        visit(factor);
        and_expressions.push_back(std::move(current_expr));
    }
    if (and_expressions.size() == 1) {
        current_expr = std::move(and_expressions[0]);
    } else {
        current_expr = std::make_unique<ExprAnd>(std::move(and_expressions));
    }
    return 0;
}

std::any QueryVisitor::visitLabelFactor(GQLParser::LabelFactorContext* ctx)
{
    LOG_VISITOR
    visit(ctx->labelPrimary());

    if (ctx->EXCLAMATION_MARK()) {
        current_expr = std::make_unique<GQL::ExprNot>(std::move(current_expr));
    }

    return 0;
}

std::any QueryVisitor::visitLabelPrimary(GQLParser::LabelPrimaryContext* ctx)
{
    LOG_VISITOR
    if (ctx->parenthesizedLabelExpression()) {
        visit(ctx->parenthesizedLabelExpression());
        return 0;
    }

    if (current_pattern == Node) {
        current_expr = std::make_unique<ExprHasNodeLabel>(
            *current_label_var_id,
            Conversions::pack_node_label(ctx->getText())
        );
    } else {
        current_expr = std::make_unique<ExprHasEdgeLabel>(
            *current_label_var_id,
            Conversions::pack_edge_label(ctx->getText())
        );
    }

    return 0;
}

// expression
std::any QueryVisitor::visitGqlNotExpression(GQLParser::GqlNotExpressionContext* ctx)
{
    LOG_VISITOR
    visit(ctx->expression());

    current_expr = std::make_unique<ExprNot>(std::move(current_expr));
    return 0;
}

std::any QueryVisitor::visitGqlLogicalOrExpression(GQLParser::GqlLogicalOrExpressionContext* ctx)
{
    LOG_VISITOR
    std::vector<std::unique_ptr<Expr>> or_expressions;

    for (auto& term : ctx->expression()) {
        visit(term);
        or_expressions.push_back(std::move(current_expr));
    }
    current_expr = std::make_unique<ExprOr>(std::move(or_expressions));
    return 0;
}

std::any QueryVisitor::visitGqlLogicalAndExpression(GQLParser::GqlLogicalAndExpressionContext* ctx)
{
    LOG_VISITOR
    std::vector<std::unique_ptr<Expr>> and_expressions;

    for (auto& term : ctx->expression()) {
        visit(term);
        and_expressions.push_back(std::move(current_expr));
    }
    current_expr = std::make_unique<ExprAnd>(std::move(and_expressions));
    return 0;
}

std::any QueryVisitor::visitGqlLogicalXorExpression(GQLParser::GqlLogicalXorExpressionContext* ctx)
{
    LOG_VISITOR
    visit(ctx->lhs);
    auto lhs_expr = std::move(current_expr);
    visit(ctx->rhs);
    auto rhs_expr = std::move(current_expr);

    current_expr = std::make_unique<ExprXor>(std::move(lhs_expr), std::move(rhs_expr));
    return 0;
}

// expression predicate
std::any QueryVisitor::visitGqlBooleanTestExpression(GQLParser::GqlBooleanTestExpressionContext* ctx)
{
    LOG_VISITOR
    visit(ctx->expressionPredicate());

    ObjectId value_oid;

    if (ctx->truthValue()->TRUE()) {
        value_oid = Common::Conversions::pack_bool(true);
    } else if (ctx->truthValue()->FALSE()) {
        value_oid = Common::Conversions::pack_bool(false);
    } else {
        value_oid = ObjectId::get_null();
    }

    if (ctx->NOT()) {
        current_expr = std::make_unique<ExprNot>(
            std::make_unique<ExprIs>(std::move(current_expr), std::make_unique<ExprTerm>(value_oid))
        );
    } else {
        current_expr = std::make_unique<ExprIs>(
            std::move(current_expr),
            std::make_unique<ExprTerm>(value_oid)
        );
    }
    return 0;
}

std::any QueryVisitor::visitGqlComparisonExpression(GQLParser::GqlComparisonExpressionContext* ctx)
{
    LOG_VISITOR
    visit(ctx->lhs);
    auto lhs_expr = std::move(current_expr);
    visit(ctx->rhs);
    auto rhs_expr = std::move(current_expr);

    if (ctx->compOp()->EQUALS_OPERATOR()) {
        current_expr = std::make_unique<ExprEquals>(std::move(lhs_expr), std::move(rhs_expr));
    } else if (ctx->compOp()->NOT_EQUALS_OPERATOR()) {
        current_expr = std::make_unique<ExprNotEquals>(std::move(lhs_expr), std::move(rhs_expr));
    } else if (ctx->compOp()->LEFT_ANGLE_BRACKET()) {
        current_expr = std::make_unique<ExprLess>(std::move(lhs_expr), std::move(rhs_expr));
    } else if (ctx->compOp()->RIGHT_ANGLE_BRACKET()) {
        current_expr = std::make_unique<ExprGreater>(std::move(lhs_expr), std::move(rhs_expr));
    } else if (ctx->compOp()->LESS_THAN_OR_EQUALS_OPERATOR()) {
        current_expr = std::make_unique<ExprLessOrEquals>(std::move(lhs_expr), std::move(rhs_expr));
    } else if (ctx->compOp()->GREATER_THAN_OR_EQUALS_OPERATOR()) {
        current_expr = std::make_unique<ExprGreaterOrEquals>(std::move(lhs_expr), std::move(rhs_expr));
    }

    return 0;
}

std::any QueryVisitor::visitGqlLowArithmeticExpression(GQLParser::GqlLowArithmeticExpressionContext* ctx)
{
    LOG_VISITOR
    visit(ctx->lhs);
    auto lhs_expr = std::move(current_expr);
    visit(ctx->rhs);
    auto rhs_expr = std::move(current_expr);

    if (ctx->PLUS_SIGN()) {
        current_expr = std::make_unique<ExprAddition>(std::move(lhs_expr), std::move(rhs_expr));
    } else if (ctx->MINUS_SIGN()) {
        current_expr = std::make_unique<ExprSubtraction>(std::move(lhs_expr), std::move(rhs_expr));
    }

    return 0;
}

std::any QueryVisitor::visitGqlHighArithmeticExpression(GQLParser::GqlHighArithmeticExpressionContext* ctx)
{
    LOG_VISITOR
    visit(ctx->lhs);
    auto lhs_expr = std::move(current_expr);
    visit(ctx->rhs);
    auto rhs_expr = std::move(current_expr);

    if (ctx->ASTERISK()) {
        current_expr = std::make_unique<ExprMultiplication>(std::move(lhs_expr), std::move(rhs_expr));
    } else if (ctx->SOLIDUS()) {
        current_expr = std::make_unique<ExprDivision>(std::move(lhs_expr), std::move(rhs_expr));
    }

    return 0;
}

std::any QueryVisitor::visitGqlNullExpression(GQLParser::GqlNullExpressionContext* ctx)
{
    LOG_VISITOR
    visit(ctx->expressionPredicate());

    if (ctx->nullPredicateCond()->NOT()) {
        current_expr = std::make_unique<ExprNot>(std::make_unique<ExprIs>(
            std::move(current_expr),
            std::make_unique<ExprTerm>(ObjectId::get_null())
        ));
    } else {
        current_expr = std::make_unique<ExprIs>(
            std::move(current_expr),
            std::make_unique<ExprTerm>(ObjectId::get_null())
        );
    }

    return 0;
}

// expression atom
std::any QueryVisitor::visitGqlParenthesizedExpression(GQLParser::GqlParenthesizedExpressionContext* ctx)
{
    LOG_VISITOR
    // check if this function is needed
    visitChildren(ctx);
    return 0;
}

// Child of ExpressionAtom
std::any QueryVisitor::visitGqlPropertyReference(GQLParser::GqlPropertyReferenceContext* ctx)
{
    LOG_VISITOR
    // assumes a variable
    std::string var_name = ctx->expressionAtom()->getText();
    std::string property_name = ctx->propertyName()->getText();

    VarId var_id = get_query_ctx().get_or_create_var(var_name);
    VarId property_var = get_query_ctx().get_or_create_var(var_name + "." + property_name);

    if (!singleton_types.count(var_id)) {
        throw QuerySemanticException(
            "Condition expression contains the variable \"" + var_name
            + "\" which does not appear in the pattern expression."
        );
    }

    if (singleton_types[var_id] == VarType::Node) {
        current_expr = std::make_unique<ExprProperty>(
            var_id,
            Conversions::pack_node_property(property_name),
            property_var,
            VarType::Node
        );
    } else {
        current_expr = std::make_unique<ExprProperty>(
            var_id,
            Conversions::pack_edge_property(property_name),
            property_var,
            VarType::Edge
        );
    }

    return 0;
}

std::any QueryVisitor::visitGqlLabeledExpression(GQLParser::GqlLabeledExpressionContext* ctx)
{
    LOG_VISITOR
    // assumes a variable
    std::string var_name = ctx->elementVariableReference()->getText();
    bool found;
    VarId var_id = get_query_ctx().get_var(var_name, &found);

    if (!found || !singleton_types.count(var_id)) {
        throw QuerySemanticException(
            "Condition expression contains the variable \"" + var_name
            + "\" which does not appear in the pattern expression."
        );
    }
    current_label_var_id = std::make_unique<VarId>(var_id);

    visit(ctx->labeledPredicateCond()->labelExpression());
    if (ctx->labeledPredicateCond()->NOT()) {
        current_expr = std::make_unique<ExprNot>(std::move(current_expr));
    }
    return 0;
}

std::any QueryVisitor::visitGqlConcatenationExpression(GQLParser::GqlConcatenationExpressionContext* ctx)
{
    LOG_VISITOR
    visit(ctx->expressionAtom()[0]);
    auto lhs_expr = std::move(current_expr);
    visit(ctx->expressionAtom()[1]);
    auto rhs_expr = std::move(current_expr);

    current_expr = std::make_unique<ExprConcat>(std::move(lhs_expr), std::move(rhs_expr));
    return 0;
}

std::any QueryVisitor::visitSingleQuotedCharacterSequence(GQLParser::SingleQuotedCharacterSequenceContext* ctx
)
{
    LOG_VISITOR
    std::vector<std::unique_ptr<Expr>> str_expressions;

    for (auto& term : ctx->unbrokenSingleQuotedCharacterSequence()) {
        std::string raw_string = term->getText();

        size_t start_pos = 1;
        if (raw_string[0] == '@') {
            // do something else
            start_pos++;
        }
        raw_string = std::string(raw_string, start_pos, raw_string.size() - 2);

        ObjectId oid = SPARQL::Conversions::pack_string_simple(raw_string);
        str_expressions.push_back(std::make_unique<ExprTerm>(oid));
    }

    if (str_expressions.size() == 1) {
        current_expr = std::move(str_expressions[0]);
    } else {
        current_expr = std::make_unique<ExprOr>(std::move(str_expressions));
    }
    return 0;
}

std::any QueryVisitor::visitDoubleQuotedCharacterSequence(GQLParser::DoubleQuotedCharacterSequenceContext* ctx
)
{
    LOG_VISITOR
    std::vector<std::unique_ptr<Expr>> str_expressions;

    for (auto& term : ctx->unbrokenDoubleQuotedCharacterSequence()) {
        std::string raw_string = term->getText();

        size_t start_pos = 1;
        if (raw_string[0] == '@') {
            // do something else
            start_pos++;
        }
        raw_string = std::string(raw_string, start_pos, raw_string.size() - 2);

        ObjectId oid = SPARQL::Conversions::pack_string_simple(raw_string);
        str_expressions.push_back(std::make_unique<ExprTerm>(oid));
    }

    if (str_expressions.size() == 1) {
        current_expr = std::move(str_expressions[0]);
    } else {
        current_expr = std::make_unique<ExprOr>(std::move(str_expressions));
    }
    return 0;
}

std::any QueryVisitor::visitIntegerLiteral(GQLParser::IntegerLiteralContext* ctx)
{
    LOG_VISITOR
    std::string integer_str = ctx->getText();

    integer_str.erase(std::remove(integer_str.begin(), integer_str.end(), '_'), integer_str.end());

    if (integer_str.size() > 2) {
        std::string integer_prefix = integer_str.substr(0, 2);
        std::string integer_suffix = integer_str.substr(2);

        if (integer_prefix == "0b") {
            uint64_t integer = std::stoull(integer_suffix, nullptr, 2);
            ObjectId oid = Common::Conversions::pack_int(integer);
            current_expr = std::make_unique<ExprTerm>(oid);
            return 0;
        } else if (integer_prefix == "0o") {
            uint64_t integer = std::stoull(integer_suffix, nullptr, 8);
            ObjectId oid = Common::Conversions::pack_int(integer);
            current_expr = std::make_unique<ExprTerm>(oid);
            return 0;
        } else if (integer_prefix == "0x") {
            uint64_t integer = std::stoull(integer_suffix, nullptr, 16);
            ObjectId oid = Common::Conversions::pack_int(integer);
            current_expr = std::make_unique<ExprTerm>(oid);
            return 0;
        }
    }

    uint64_t integer = std::stoull(integer_str);
    ObjectId oid = Common::Conversions::pack_int(integer);
    current_expr = std::make_unique<ExprTerm>(oid);
    return 0;
}

std::any QueryVisitor::visitFloatLiteral(GQLParser::FloatLiteralContext* ctx)
{
    LOG_VISITOR
    float float_literal = std::stof(ctx->getText());
    ObjectId oid = Common::Conversions::pack_float(float_literal);
    current_expr = std::make_unique<ExprTerm>(oid);
    return 0;
}

std::any QueryVisitor::visitBooleanLiteral(GQLParser::BooleanLiteralContext* ctx)
{
    LOG_VISITOR
    if (ctx->TRUE()) {
        ObjectId oid = Common::Conversions::pack_bool(true);
        current_expr = std::make_unique<ExprTerm>(oid);
    } else if (ctx->FALSE()) {
        ObjectId oid = Common::Conversions::pack_bool(false);
        current_expr = std::make_unique<ExprTerm>(oid);
    } else {
        current_expr = std::make_unique<ExprTerm>(ObjectId::get_null());
    }
    return 0;
}

std::any QueryVisitor::visitGqlVariableExpression(GQLParser::GqlVariableExpressionContext* ctx)
{
    LOG_VISITOR
    std::string var_name = ctx->getText();
    VarId var_id = get_query_ctx().get_or_create_var(var_name);
    current_expr = std::make_unique<ExprVar>(var_id);
    return 0;
}

std::any QueryVisitor::visitOrderByAndPageStatement(GQLParser::OrderByAndPageStatementContext* ctx)
{
    LOG_VISITOR
    if (ctx->orderByClause()) {
        ctx->orderByClause()->accept(this);
    }
    return 0;
}

std::any QueryVisitor::visitOrderByClause(GQLParser::OrderByClauseContext* ctx) {
    LOG_VISITOR
    for (auto& oc : ctx->sortSpecificationList()->sortSpecification()) {
        if (oc->sortKey()) {
            visit(oc->sortKey());
            order_by_items.emplace_back(std::move(current_expr));
        } else {
            // it should not enter here unless grammar is modified
            throw QuerySemanticException("Unsupported ORDER BY condition: '" + oc->getText() + "'");
        }
        if (oc->orderingSpecification()){
            if (oc->orderingSpecification()->DESC()||oc->orderingSpecification()->DESCENDING()) {
                order_by_ascending.push_back(false);
                if (oc->nullOrdering()){
                    auto null_order = oc->nullOrdering()->getText();
                    if (null_order == "NULLSFIRST") {
                        order_nulls.push_back(false);
                    } else {
                        order_nulls.push_back(true);
                    }
                } else {
                    order_nulls.push_back(true);
                }
            } else {
                order_by_ascending.push_back(true);
                if (oc->nullOrdering()){
                    auto null_order = oc->nullOrdering()->getText();
                    if (null_order == "NULLSFIRST") {
                        order_nulls.push_back(true);
                    } else {
                        order_nulls.push_back(false);
                    }
                } else {
                    order_nulls.push_back(false);
                }
            }
        } else {
            order_by_ascending.push_back(true);
            if (oc->nullOrdering()){
                auto null_order = oc->nullOrdering()->getText();
                if (null_order == "NULLSFIRST") {
                    order_nulls.push_back(true);
                } else {
                    order_nulls.push_back(false);
                }
            } else {
                order_nulls.push_back(false);
            }
        }
    }
    return 0;
}

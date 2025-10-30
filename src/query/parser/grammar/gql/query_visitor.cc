#include "query_visitor.h"

#include <iomanip>
#include <list>
#include <memory>

#include "graph_models/common/conversions.h"
#include "graph_models/gql/conversions.h"
#include "graph_models/rdf_model/conversions.h"
#include "query/parser/expr/gql/exprs.h"
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

std::any QueryVisitor::visitLinearDataModifyingStatementBody(
    GQLParser::LinearDataModifyingStatementBodyContext* ctx
)
{
    LOG_VISITOR
    std::vector<std::unique_ptr<Op>> query_statements;

    // for now we visit only if there is a return statement
    auto primitiveResultStatement = ctx->primitiveResultStatement();
    if (!primitiveResultStatement) {
        current_op = std::make_unique<OpReturn>(
            std::make_unique<OpEmpty>(),
            std::vector<OpReturn::Item> {},
            false
        );
        return 0;
    }

    for (auto& child : ctx->simpleDataAccessingStatement()) {
        visit(child);
        query_statements.push_back(std::move(current_op));
    }
    current_op = std::make_unique<OpQueryStatements>(std::move(query_statements));

    visit(primitiveResultStatement);
    return 0;
}

std::any QueryVisitor::visitPrimitiveQueryStatement(GQLParser::PrimitiveQueryStatementContext* ctx)
{
    LOG_VISITOR

    auto matchStatement = ctx->matchStatement();
    auto letStatement = ctx->letStatement();
    auto orderByAndPageStatement = ctx->orderByAndPageStatement();
    auto filterStatement = ctx->filterStatement();

    if (matchStatement) {
        matchStatement->accept(this);
    } else if (letStatement) {
        letStatement->accept(this);
    } else if (orderByAndPageStatement) {
        orderByAndPageStatement->accept(this);
    } else if (filterStatement) {
        filterStatement->accept(this);
        current_op = std::make_unique<OpFilter>(std::move(filter_items));
    }
    return 0;
}

std::any QueryVisitor::visitMatchStatement(GQLParser::MatchStatementContext* ctx)
{
    LOG_VISITOR
    visitChildren(ctx);
    return 0;
}

std::any QueryVisitor::visitLetStatement(GQLParser::LetStatementContext* ctx)
{
    LOG_VISITOR
    visitChildren(ctx);
    current_op = std::make_unique<OpLet>(std::move(let_items));
    return 0;
}

std::any QueryVisitor::visitLetVariableDefinitionList(GQLParser::LetVariableDefinitionListContext* ctx)
{
    LOG_VISITOR
    for (auto child : ctx->letVariableDefinition()) {
        visit(child);
        if (let_item.has_value()) {
            let_items.push_back(std::move(*let_item));
            let_item = std::nullopt;
        }
    }
    return 0;
}

std::any QueryVisitor::visitLetVariableDefinition(GQLParser::LetVariableDefinitionContext* ctx)
{
    LOG_VISITOR
    if (ctx->EQUALS_OPERATOR()) {
        visit(ctx->expression());
        VarId var_id = get_query_ctx().get_or_create_var(ctx->valueVariable()->getText());
        let_item = { var_id, std::move(current_expr) };
    }

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

    // We create this OP, even if the size of the vector is 1. This OP means that there is a match statement.
    current_op = std::make_unique<OpGraphPatternList>(std::move(basic_graph_patterns));
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

std::any QueryVisitor::visitPrimitiveResultStatement(GQLParser::PrimitiveResultStatementContext* ctx)
{
    LOG_VISITOR
    visit(ctx->returnStatement());

    auto orderByAndPageStatement = ctx->orderByAndPageStatement();

    if (orderByAndPageStatement) {
        auto op_return = std::move(current_op);
        visit(orderByAndPageStatement);

        // we set the op order by as separate child of the op return
        auto op_return_ptr = dynamic_cast<OpReturn*>(op_return.get());
        current_op = std::make_unique<OpReturn>(
            std::move(op_return_ptr->op),
            std::move(op_return_ptr->return_items),
            op_return_ptr->distinct,
            std::move(current_op)
        );
    }
    return 0;
}

std::any QueryVisitor::visitReturnStatementBody(GQLParser::ReturnStatementBodyContext* ctx)
{
    LOG_VISITOR
    if (current_op == nullptr) {
        current_op = std::make_unique<OpUnitTable>();
    }

    distinct = false;
    auto setQuantifier = ctx->setQuantifier();
    if (setQuantifier) {
        if (setQuantifier->DISTINCT()) {
            distinct = true;
        }
    }

    auto returnItemList = ctx->returnItemList();
    if (ctx->ASTERISK()) {
        if (ctx->groupByClause()) {
            throw QuerySemanticException("A query that contains an asterisk (*) in the RETURN statement "
                                         "cannot contain the GROUP BY clause");
        }

        auto vars = current_op->get_all_vars();
        for (auto& var : vars) {
            std::string var_name = get_query_ctx().get_var_name(var);

            // filter anonymous vars and vars with properties
            if (!get_query_ctx().is_internal(var) && var_name.find('.') == var_name.npos) {
                return_items.emplace_back(std::make_unique<ExprVar>(var), std::nullopt);
            }
        }
    } else if (returnItemList) {
        visit(returnItemList);
    }

    auto groupByClause = ctx->groupByClause();
    if (groupByClause) {
        visit(groupByClause->groupingElementList());
        if (!current_expr_list.empty()) {
            current_op = std::make_unique<OpGroupBy>(std::move(current_op), std::move(current_expr_list));
        }
    }

    current_op = std::make_unique<OpReturn>(std::move(current_op), std::move(return_items), distinct);
    return 0;
}

std::any QueryVisitor::visitReturnItemList(GQLParser::ReturnItemListContext* ctx)
{
    LOG_VISITOR
    for (auto& item : ctx->returnItem()) {
        visit(item);

        auto returnItemAlias = item->returnItemAlias();

        std::optional<VarId> alias = {};
        if (returnItemAlias != nullptr) {
            // TODO: throw an exception if the alias is already defined
            std::string alias_str = returnItemAlias->identifier()->getText();
            alias = get_query_ctx().get_or_create_var(alias_str);
        } else {
            std::string alias_str = item->expression()->getText();
            alias = get_query_ctx().get_or_create_var(alias_str);
        }
        return_items.emplace_back(std::move(current_expr), alias);
    }
    return 0;
}

std::any QueryVisitor::visitGroupingElementList(GQLParser::GroupingElementListContext* ctx)
{
    LOG_VISITOR
    current_expr_list.clear();
    for (auto& elem : ctx->groupingElement()) {
        VarId group_var = get_query_ctx().get_or_create_var(elem->getText());
        current_expr_list.push_back(std::make_unique<ExprVar>(group_var));
    }
    return 0;
}

OpEdge QueryVisitor::create_edge(
    VarId left_node,
    VarId right_node,
    VarId edge_id,
    VarId direction_var,
    EdgeType type
)
{
    switch (type) {
    case EdgePointingLeft: {
        return OpEdge(
            right_node,
            left_node,
            edge_id,
            direction_var,
            OpEdge::Directed,
            ObjectId(ObjectId::DIRECTION_LEFT)
        );
    }
    case EdgeUndirected: {
        return OpEdge(left_node, right_node, edge_id, direction_var, OpEdge::Undirected);
    }
    case EdgePointingRight: {
        return OpEdge(
            left_node,
            right_node,
            edge_id,
            direction_var,
            OpEdge::Directed,
            ObjectId(ObjectId::DIRECTION_RIGHT)
        );
    }
    case EdgeLeftOrUndirected: {
        return OpEdge(
            right_node,
            left_node,
            edge_id,
            direction_var,
            OpEdge::UndirectedOrDirectedTo,
            ObjectId(ObjectId::DIRECTION_LEFT)
        );
    }
    case EdgeUndirectedOrRight: {
        return OpEdge(
            left_node,
            right_node,
            edge_id,
            direction_var,
            OpEdge::UndirectedOrDirectedTo,
            ObjectId(ObjectId::DIRECTION_RIGHT)
        );
    }
    case EdgeLeftOrRight: {
        return OpEdge(left_node, right_node, edge_id, direction_var, OpEdge::DirectedLeftOrRight);
    }
    case EdgeAnyDirection: {
        return OpEdge(left_node, right_node, edge_id, direction_var, OpEdge::AnyDirection);
    }
    }
    throw QuerySemanticException("Unrecognized edge type");
}

std::any QueryVisitor::visitPathPattern(GQLParser::PathPatternContext* ctx)
{
    LOG_VISITOR

    auto pathPatternPrefix = ctx->pathPatternPrefix();
    if (pathPatternPrefix) {
        visit(pathPatternPrefix);
    } else {
        path_mode = PathMode();
    }

    // we copy the path mode, so we can remember it even if a child pattern overrides it
    PathMode this_path_mode = path_mode;

    visit(ctx->pathPatternExpression());

    auto pathVariableDeclaration = ctx->pathVariableDeclaration();
    if (pathVariableDeclaration) {
        visit(pathVariableDeclaration);
        current_op = std::make_unique<OpGraphPattern>(std::move(current_op), this_path_mode, *current_id);
    } else {
        current_op = std::make_unique<OpGraphPattern>(std::move(current_op), this_path_mode);
    }
    return 0;
}

std::any QueryVisitor::visitParenthesizedPathPatternExpression(
    GQLParser::ParenthesizedPathPatternExpressionContext* ctx
)
{
    LOG_VISITOR
    // if the subpath does not contain a path mode, we use the path mode of the parent gp
    auto pathModePrefix = ctx->pathModePrefix();
    if (pathModePrefix) {
        visit(pathModePrefix);
    }

    // we copy the path mode, so we can remember it even if a child pattern overrides it
    PathMode this_path_mode = path_mode;

    visit(ctx->pathPatternExpression());

    auto subpathVariableDeclaration = ctx->subpathVariableDeclaration();
    if (subpathVariableDeclaration) {
        visit(subpathVariableDeclaration);
        current_op = std::make_unique<OpGraphPattern>(std::move(current_op), this_path_mode, *current_id);
    } else {
        current_op = std::make_unique<OpGraphPattern>(std::move(current_op), this_path_mode);
    }

    auto parenthesizedPathPatternWhereClause = ctx->parenthesizedPathPatternWhereClause();
    if (parenthesizedPathPatternWhereClause) {
        visit(parenthesizedPathPatternWhereClause);
        std::vector<std::unique_ptr<Expr>> expr_list;
        expr_list.push_back(std::move(current_expr));
        current_op = std::make_unique<OpWhere>(std::move(current_op), std::move(expr_list));
    }

    current_pattern = Subpath;
    return 0;
}

std::any QueryVisitor::visitAllPathSearch(GQLParser::AllPathSearchContext* ctx)
{
    LOG_VISITOR
    path_mode.selector = PathMode::ALL;
    auto pathMode = ctx->pathMode();
    if (pathMode) {
        visit(pathMode);
    }
    return 0;
}

std::any QueryVisitor::visitAnyPathSearch(GQLParser::AnyPathSearchContext* ctx)
{
    LOG_VISITOR
    path_mode.selector = PathMode::ANY;

    auto numberOfPaths = ctx->numberOfPaths();
    if (numberOfPaths) {
        path_mode.path_count = std::stoull(numberOfPaths->getText());
    }

    auto pathMode = ctx->pathMode();
    if (pathMode) {
        visit(pathMode);
    }

    return 0;
}

std::any QueryVisitor::visitShortestPathSearch(GQLParser::ShortestPathSearchContext* ctx)
{
    LOG_VISITOR

    auto allShortestPathSearch = ctx->allShortestPathSearch();
    auto anyShortestPathSearch = ctx->anyShortestPathSearch();
    auto countedShortestPathSearch = ctx->countedShortestPathSearch();
    auto countedShortestGroupSearch = ctx->countedShortestGroupSearch();

    if (allShortestPathSearch) {
        path_mode.selector = PathMode::ALL_SHORTEST;

        auto allShortestPathSearch_pathMode = allShortestPathSearch->pathMode();
        if (allShortestPathSearch_pathMode) {
            visit(allShortestPathSearch_pathMode);
        }

    } else if (anyShortestPathSearch) {
        path_mode.selector = PathMode::ANY_SHORTEST;

        auto anyShortestPathSearch_pathMode = anyShortestPathSearch->pathMode();
        if (anyShortestPathSearch_pathMode) {
            visit(anyShortestPathSearch_pathMode);
        }

    } else if (countedShortestPathSearch) {
        path_mode.selector = PathMode::SHORTEST_PATH_COUNT;

        auto countedShortestPathSearch_pathMode = countedShortestPathSearch->pathMode();
        if (countedShortestPathSearch_pathMode) {
            visit(countedShortestPathSearch_pathMode);
        }

        path_mode.path_count = std::stoull(countedShortestPathSearch->numberOfPaths()->getText());

    } else if (countedShortestGroupSearch) {
        path_mode.selector = PathMode::SHORTEST_GROUP_COUNT;

        auto countedShortestGroupSearch_pathMode = countedShortestGroupSearch->pathMode();

        if (countedShortestGroupSearch_pathMode) {
            visit(countedShortestGroupSearch_pathMode);
        }

        path_mode.path_count = std::stoull(countedShortestGroupSearch->numberOfGroups()->getText());
    }

    return 0;
}

std::any QueryVisitor::visitPathMode(GQLParser::PathModeContext* ctx)
{
    LOG_VISITOR
    if (ctx->WALK()) {
        path_mode.restrictor = PathMode::WALK;
    } else if (ctx->TRAIL()) {
        path_mode.restrictor = PathMode::TRAIL;
    } else if (ctx->SIMPLE()) {
        path_mode.restrictor = PathMode::SIMPLE;
    } else if (ctx->ACYCLIC()) {
        path_mode.restrictor = PathMode::ACYCLIC;
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
    std::vector<std::unique_ptr<Expr>> parent_expr_list = std::move(current_expr_list);
    std::list<PatternInfo> bgp_patterns;
    std::vector<VarId> direction_vars;

    // visit all children and create nodes and subpaths
    for (size_t i = 0; i < ctx->children.size(); i++) {
        current_repetition = nullptr;
        visit(ctx->children[i]);

        switch (current_pattern) {
        case Node: {
            if (current_repetition) {
                throw QuerySemanticException("A repeated pattern should have path length greater than 0.");
            }
            bgp_patterns.emplace_back(
                std::make_unique<OpNode>(*current_id),
                current_pattern,
                *current_id,
                EdgeAnyDirection
            );
            singleton_types[*current_id] = VarType::Node;
            break;
        }
        case Edge: {
            bgp_patterns.emplace_back(nullptr, current_pattern, *current_id, edge_type);
            singleton_types[*current_id] = VarType::Edge;

            VarId direction_var = get_query_ctx().get_or_create_var(
                ".dir_" + get_query_ctx().get_var_name(*current_id)
            );
            direction_vars.push_back(direction_var);

            if (current_repetition) {
                bgp_patterns.back().repetition = std::move(current_repetition);
            }
            break;
        }
        case Subpath: {
            if (current_repetition) {
                bgp_patterns.emplace_back(
                    std::make_unique<OpRepetition>(std::move(current_op), std::move(current_repetition)),
                    Subpath,
                    *current_id,
                    EdgeAnyDirection
                );
            } else {
                bgp_patterns
                    .emplace_back(std::move(current_op), current_pattern, *current_id, EdgeAnyDirection);
            }
            break;
        }
        case Other: {
        }
        }
    }

    VarId left_node(0);
    VarId right_node(0);

    auto it_bgp_patterns = bgp_patterns.begin();
    auto it_direction_vars = direction_vars.begin();

    // create the edges and add the element patterns to the bgp in order
    while (it_bgp_patterns != bgp_patterns.end()) {
        if (it_bgp_patterns->type != Edge) {
            it_bgp_patterns++;
            continue;
        }

        // store the edge id
        VarId edge_id = it_bgp_patterns->id;

        if (it_bgp_patterns->repetition != nullptr) {
            // if the edge has a quantifier, then we add 2 internal nodes
            std::vector<std::unique_ptr<Op>> rep_patterns;
            auto left_node_op = std::make_unique<OpNode>(get_query_ctx().get_internal_var());
            auto right_node_op = std::make_unique<OpNode>(get_query_ctx().get_internal_var());

            OpEdge edge = create_edge(
                left_node_op->id,
                right_node_op->id,
                edge_id,
                *it_direction_vars,
                it_bgp_patterns->edge_type
            );
            rep_patterns.push_back(std::move(left_node_op));
            rep_patterns.push_back(std::make_unique<OpEdge>(edge));
            rep_patterns.push_back(std::move(right_node_op));

            auto bgp = std::make_unique<OpBasicGraphPattern>(std::move(rep_patterns));

            PatternInfo pattern_info(
                std::make_unique<OpRepetition>(std::move(bgp), std::move(it_bgp_patterns->repetition)),
                Other,
                edge_id,
                EdgeAnyDirection
            );

            (*it_bgp_patterns) = std::move(pattern_info);
        } else {
            // find the left node id
            auto it_last = it_bgp_patterns;
            if (it_bgp_patterns != bgp_patterns.begin() && (--it_last)->type == Node) {
                left_node = it_last->id;
            } else {
                auto node_id = get_query_ctx().get_internal_var();
                PatternInfo pattern_info(std::make_unique<OpNode>(node_id), Node, node_id, EdgeAnyDirection);
                bgp_patterns.insert(it_bgp_patterns, std::move(pattern_info));
                left_node = node_id;
            }

            // find the right node id
            auto it_next = it_bgp_patterns;
            it_next++;
            if (it_next != bgp_patterns.end() && it_next->type == Node) {
                right_node = it_next->id;
            } else {
                auto node_id = get_query_ctx().get_internal_var();
                PatternInfo pattern_info(std::make_unique<OpNode>(node_id), Node, node_id, EdgeAnyDirection);
                bgp_patterns.insert(it_next, std::move(pattern_info));
                right_node = node_id;
            }
            OpEdge edge = create_edge(
                left_node,
                right_node,
                edge_id,
                *it_direction_vars,
                it_bgp_patterns->edge_type
            );
            it_bgp_patterns->pattern = std::make_unique<OpEdge>(edge);
        }

        it_bgp_patterns++;
        it_direction_vars++;
    }

    auto current_basic_graph_pattern = std::make_unique<OpBasicGraphPattern>();
    for (auto& pattern : bgp_patterns) {
        current_basic_graph_pattern->add_pattern(std::move(pattern.pattern));
    }

    if (current_expr_list.empty()) {
        current_op = std::move(current_basic_graph_pattern);
    } else {
        current_op = std::make_unique<OpWhere>(
            std::move(current_basic_graph_pattern),
            std::move(current_expr_list)
        );
    }

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

    auto fullEdgePattern = ctx->fullEdgePattern();
    auto abbreviatedEdgePattern = ctx->abbreviatedEdgePattern();

    if (fullEdgePattern) {
        visitChildren(ctx);

        if (fullEdgePattern->fullEdgePointingLeft()) {
            edge_type = EdgePointingLeft;
        } else if (fullEdgePattern->fullEdgeUndirected()) {
            edge_type = EdgeUndirected;
        } else if (fullEdgePattern->fullEdgePointingRight()) {
            edge_type = EdgePointingRight;
        } else if (fullEdgePattern->fullEdgeLeftOrUndirected()) {
            edge_type = EdgeLeftOrUndirected;
        } else if (fullEdgePattern->fullEdgeUndirectedOrRight()) {
            edge_type = EdgeUndirectedOrRight;
        } else if (fullEdgePattern->fullEdgeLeftOrRight()) {
            edge_type = EdgeLeftOrRight;
        } else if (fullEdgePattern->fullEdgeAnyDirection()) {
            edge_type = EdgeAnyDirection;
        }
    } else if (abbreviatedEdgePattern) {
        current_id = std::make_unique<VarId>(get_query_ctx().get_internal_var());

        std::string pattern_text = abbreviatedEdgePattern->getText();

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
    auto elementVariableDeclaration = ctx->elementVariableDeclaration();
    if (elementVariableDeclaration) {
        visit(elementVariableDeclaration);
    } else {
        current_id = std::make_unique<VarId>(get_query_ctx().get_internal_var());
    }

    // set the type before visiting the label expression and the where clause
    if (current_pattern == Node) {
        singleton_types[*current_id] = VarType::Node;
    } else {
        singleton_types[*current_id] = VarType::Edge;
    }

    auto isLabelExpression = ctx->isLabelExpression();
    if (isLabelExpression) {
        current_label_var_id = std::make_unique<VarId>(*current_id);
        visit(isLabelExpression);
        current_expr_list.push_back(std::move(current_expr));
    } else {
    }

    auto elementPatternPredicate = ctx->elementPatternPredicate();
    if (elementPatternPredicate) {
        visit(elementPatternPredicate);
        current_expr_list.push_back(std::move(current_expr));
    }

    return 0;
}

std::any QueryVisitor::visitQuantifiedPathPrimary(GQLParser::QuantifiedPathPrimaryContext* ctx)
{
    LOG_VISITOR
    visit(ctx->pathPrimary());
    visit(ctx->graphPatternQuantifier());
    return 0;
}

std::any QueryVisitor::visitQuestionedPathPrimary(GQLParser::QuestionedPathPrimaryContext* ctx)
{
    LOG_VISITOR
    visit(ctx->pathPrimary());
    current_repetition = std::make_unique<OpRepetition::Repetition>(0, 1);
    return 0;
}

std::any QueryVisitor::visitGraphPatternQuantifier(GQLParser::GraphPatternQuantifierContext* ctx)
{
    LOG_VISITOR

    auto fixedQuantifier = ctx->fixedQuantifier();
    auto generalQuantifier = ctx->generalQuantifier();

    if (ctx->ASTERISK()) {
        current_repetition = std::make_unique<OpRepetition::Repetition>(0, std::nullopt);
    } else if (ctx->PLUS_SIGN()) {
        current_repetition = std::make_unique<OpRepetition::Repetition>(1, std::nullopt);
    } else if (fixedQuantifier) {
        std::string integer_str = fixedQuantifier->UNSIGNED_DECIMAL_INTEGER()->getText();

        integer_str.erase(std::remove(integer_str.begin(), integer_str.end(), '_'), integer_str.end());
        uint64_t integer = std::stoull(integer_str);

        current_repetition = std::make_unique<OpRepetition::Repetition>(integer, integer);

    } else if (generalQuantifier) {
        uint64_t lower = 0;
        std::optional<uint64_t> upper = std::nullopt;

        auto generalQuantifier_lowerBound = generalQuantifier->lowerBound();
        auto generalQuantifier_upperBound = generalQuantifier->upperBound();

        if (generalQuantifier_lowerBound) {
            std::string integer_str = generalQuantifier_lowerBound->getText();

            integer_str.erase(std::remove(integer_str.begin(), integer_str.end(), '_'), integer_str.end());
            lower = std::stoull(integer_str);
        }
        if (generalQuantifier_upperBound) {
            std::string integer_str = generalQuantifier_upperBound->getText();

            integer_str.erase(std::remove(integer_str.begin(), integer_str.end(), '_'), integer_str.end());
            upper = std::stoull(integer_str);
        }

        if (upper.has_value() && lower > upper) {
            throw QuerySemanticException("The lower bound in the quantifier is greater than the upper bound."
            );
        }

        current_repetition = std::make_unique<OpRepetition::Repetition>(lower, upper);
    }

    return 0;
}

std::any QueryVisitor::visitGraphPatternWhereClause(GQLParser::GraphPatternWhereClauseContext* ctx)
{
    LOG_VISITOR
    visitChildren(ctx);
    std::vector<std::unique_ptr<Expr>> expr_list;
    expr_list.push_back(std::move(current_expr));
    current_op = std::make_unique<OpWhere>(std::move(current_op), std::move(expr_list));
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
    auto parenthesizedLabelExpression = ctx->parenthesizedLabelExpression();
    if (parenthesizedLabelExpression) {
        visit(parenthesizedLabelExpression);
        return 0;
    }

    if (ctx->wildcardLabel()) {
        VarType::Type type = current_pattern == Node ? VarType::Node : VarType::Edge;
        current_expr = std::make_unique<ExprWildcardLabel>(*current_label_var_id, type);
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

    if (ctx->truthValue()->K_TRUE()) {
        value_oid = Common::Conversions::pack_bool(true);
    } else if (ctx->truthValue()->K_FALSE()) {
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

std::any QueryVisitor::visitGqlInExpression(GQLParser::GqlInExpressionContext* ctx)
{
    visit(ctx->lhs);
    auto lhs_expr = std::move(current_expr);

    visit(ctx->rhs);
    auto rhs_expr = std::move(current_expr);

    current_expr = std::make_unique<ExprIn>(std::move(lhs_expr), std::move(rhs_expr));
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

std::any QueryVisitor::visitGqlOneArgScalarFunction(GQLParser::GqlOneArgScalarFunctionContext* ctx)
{
    LOG_VISITOR
    if (auto unsignedLiteral = ctx->functionParameter()->unsignedLiteral()) {
        visit(unsignedLiteral);
    } else if (auto variable = ctx->functionParameter()->variable()) {
        visit(variable);
    } else if (auto propertyReference = ctx->functionParameter()->propertyReference()) {
        visit(propertyReference);
    } else if (auto functionCall = ctx->functionParameter()->functionCall()) {
        visit(functionCall);
    } else if (auto expression = ctx->functionParameter()->expression()) {
        visit(expression);
    }

    auto expr = std::move(current_expr);

    if (ctx->oneArgNumericFunctionName()->ABS()) {
        current_expr = std::make_unique<ExprAbs>(std::move(expr));
    } else if (ctx->oneArgNumericFunctionName()->SIN()) {
        current_expr = std::make_unique<ExprSin>(std::move(expr));
    } else if (ctx->oneArgNumericFunctionName()->COS()) {
        current_expr = std::make_unique<ExprCos>(std::move(expr));
    } else if (ctx->oneArgNumericFunctionName()->TAN()) {
        current_expr = std::make_unique<ExprTan>(std::move(expr));
    } else if (ctx->oneArgNumericFunctionName()->COT()) {
        current_expr = std::make_unique<ExprCot>(std::move(expr));
    } else if (ctx->oneArgNumericFunctionName()->SINH()) {
        current_expr = std::make_unique<ExprSinh>(std::move(expr));
    } else if (ctx->oneArgNumericFunctionName()->COSH()) {
        current_expr = std::make_unique<ExprCosh>(std::move(expr));
    } else if (ctx->oneArgNumericFunctionName()->TANH()) {
        current_expr = std::make_unique<ExprTanh>(std::move(expr));
    } else if (ctx->oneArgNumericFunctionName()->ASIN()) {
        current_expr = std::make_unique<ExprAsin>(std::move(expr));
    } else if (ctx->oneArgNumericFunctionName()->ACOS()) {
        current_expr = std::make_unique<ExprAcos>(std::move(expr));
    } else if (ctx->oneArgNumericFunctionName()->ATAN()) {
        current_expr = std::make_unique<ExprAtan>(std::move(expr));
    } else if (ctx->oneArgNumericFunctionName()->DEGREES()) {
        current_expr = std::make_unique<ExprDegrees>(std::move(expr));
    } else if (ctx->oneArgNumericFunctionName()->RADIANS()) {
        current_expr = std::make_unique<ExprRadians>(std::move(expr));
    } else if (ctx->oneArgNumericFunctionName()->LOG10()) {
        current_expr = std::make_unique<ExprLog10>(std::move(expr));
    } else if (ctx->oneArgNumericFunctionName()->LN()) {
        current_expr = std::make_unique<ExprLn>(std::move(expr));
    } else if (ctx->oneArgNumericFunctionName()->EXP()) {
        current_expr = std::make_unique<ExprExp>(std::move(expr));
    } else if (ctx->oneArgNumericFunctionName()->SQRT()) {
        current_expr = std::make_unique<ExprSqrt>(std::move(expr));
    } else if (ctx->oneArgNumericFunctionName()->FLOOR()) {
        current_expr = std::make_unique<ExprFloor>(std::move(expr));
    } else if (ctx->oneArgNumericFunctionName()->CEIL()) {
        current_expr = std::make_unique<ExprCeil>(std::move(expr));
    } else if (ctx->oneArgNumericFunctionName()->CHAR_LENGTH()
               || ctx->oneArgNumericFunctionName()->CHARACTER_LENGTH())
    {
        current_expr = std::make_unique<ExprLength>(std::move(expr));
    }
    return 0;
}

std::any QueryVisitor::visitGqlTwoArgScalarFunction(GQLParser::GqlTwoArgScalarFunctionContext* ctx)
{
    LOG_VISITOR
    std::vector<std::unique_ptr<Expr>> expressions;
    for (auto& oc : ctx->functionParameter()) {
        if (auto unsignedLiteral = oc->unsignedLiteral()) {
            visit(unsignedLiteral);
        } else if (auto variable = oc->variable()) {
            visit(variable);
        } else if (auto propertyReference = oc->propertyReference()) {
            visit(propertyReference);
        } else if (auto functionCall = oc->functionCall()) {
            visit(functionCall);
        } else if (auto expression = oc->expression()) {
            visit(expression);
        }
        expressions.push_back(std::move(current_expr));
    }

    if (ctx->twoArgNumericFunctionName()->MOD()) {
        current_expr = std::make_unique<ExprModulo>(std::move(expressions[0]), std::move(expressions[1]));
    } else if (ctx->twoArgNumericFunctionName()->LOG()) {
        current_expr = std::make_unique<ExprLog>(std::move(expressions[0]), std::move(expressions[1]));
    } else if (ctx->twoArgNumericFunctionName()->POWER()) {
        current_expr = std::make_unique<ExprPower>(std::move(expressions[0]), std::move(expressions[1]));
    }
    return 0;
}

std::any QueryVisitor::visitGqlSubstringFunction(GQLParser::GqlSubstringFunctionContext* ctx)
{
    LOG_VISITOR
    std::vector<std::unique_ptr<Expr>> expressions;
    visit(ctx->str);
    expressions.push_back(std::move(current_expr));
    visit(ctx->strLen);
    expressions.push_back(std::move(current_expr));
    bool left;
    if (ctx->RIGHT()) {
        left = false;
    } else {
        left = true;
    }

    current_expr = std::make_unique<ExprSubStr>(
        std::move(expressions[0]),
        std::move(expressions[1]),
        std::move(left)
    );
    return 0;
}

std::any QueryVisitor::visitGqlFoldStringFunction(GQLParser::GqlFoldStringFunctionContext* ctx)
{
    LOG_VISITOR
    visit(ctx->expressionAtom());
    bool upper;
    if (ctx->UPPER()) {
        upper = true;
    } else {
        upper = false;
    }

    current_expr = std::make_unique<ExprFold>(std::move(current_expr), std::move(upper));
    return 0;
}

std::any QueryVisitor::visitGqlSingleTrimStringFunction(GQLParser::GqlSingleTrimStringFunctionContext* ctx)
{
    LOG_VISITOR
    std::vector<std::unique_ptr<Expr>> expressions;
    for (auto& oc : ctx->expressionAtom()) {
        visit(oc);
        expressions.insert(expressions.begin(), std::move(current_expr));
    }
    if (expressions.size() == 1) {
        expressions.push_back(nullptr);
    }
    std::string specification;
    auto trimSpecification = ctx->trimSpecification();
    if (trimSpecification) {
        if (trimSpecification->LEADING()) {
            specification = "LEADING";
        } else if (trimSpecification->TRAILING()) {
            specification = "TRAILING";
        } else {
            specification = "BOTH";
        }
    } else {
        specification = "BOTH";
    }

    current_expr = std::make_unique<ExprSingleTrim>(
        std::move(expressions[0]),
        std::move(expressions[1]),
        std::move(specification)
    );
    return 0;
}

std::any QueryVisitor::visitGqlMultiTrimStringFunction(GQLParser::GqlMultiTrimStringFunctionContext* ctx)
{
    LOG_VISITOR
    std::vector<std::unique_ptr<Expr>> expressions;
    visit(ctx->trimSrc);
    expressions.push_back(std::move(current_expr));

    auto delChar = ctx->delChar;
    if (delChar) {
        visit(delChar);
        expressions.push_back(std::move(current_expr));
    } else {
        expressions.push_back(nullptr);
    }
    std::string specification;
    if (ctx->BTRIM()) {
        specification = "BTRIM";
    } else if (ctx->LTRIM()) {
        specification = "LTRIM";
    } else if (ctx->RTRIM()) {
        specification = "RTRIM";
    } else {
        specification = "BTRIM";
    }

    current_expr = std::make_unique<ExprMultiTrim>(
        std::move(expressions[0]),
        std::move(expressions[1]),
        std::move(specification)
    );
    return 0;
}

std::any QueryVisitor::visitGqlNormStringFunction(GQLParser::GqlNormStringFunctionContext* ctx)
{
    LOG_VISITOR
    visit(ctx->expressionAtom());

    std::string form;
    auto normalForm = ctx->normalForm();
    if (normalForm) {
        if (normalForm->NFC()) {
            form = "NFC";
        } else if (normalForm->NFD()) {
            form = "NFD";
        } else if (normalForm->NFKC()) {
            form = "NFKC";
        } else {
            form = "NFKD";
        }
    } else {
        form = "NFC";
    }

    current_expr = std::make_unique<ExprNormalize>(std::move(current_expr), std::move(form));
    return 0;
}

std::any QueryVisitor::visitGqlNullIfCaseFunction(GQLParser::GqlNullIfCaseFunctionContext* ctx)
{
    LOG_VISITOR
    visit(ctx->lhs);
    auto lhs_expr = std::move(current_expr);
    visit(ctx->rhs);
    auto rhs_expr = std::move(current_expr);

    current_expr = std::make_unique<ExprNullIf>(std::move(lhs_expr), std::move(rhs_expr));
    return 0;
}

std::any QueryVisitor::visitGqlCoalesceCaseFunction(GQLParser::GqlCoalesceCaseFunctionContext* ctx)
{
    LOG_VISITOR
    std::vector<std::unique_ptr<Expr>> expressions;

    for (auto expr_ctx : ctx->expression()) {
        visit(expr_ctx);
        expressions.push_back(std::move(current_expr));
    }

    current_expr = std::make_unique<ExprCoalesce>(std::move(expressions));
    return 0;
}

std::any QueryVisitor::visitGqlSimpleCaseFunction(GQLParser::GqlSimpleCaseFunctionContext* ctx)
{
    LOG_VISITOR
    visit(ctx->expressionAtom());
    auto case_operand = std::move(current_expr);

    std::vector<std::pair<std::pair<std::string, std::vector<std::unique_ptr<Expr>>>, std::unique_ptr<Expr>>>
        when_clauses;
    for (auto when_clause : ctx->simpleWhenClause()) {
        std::vector<std::unique_ptr<Expr>> when_operands;
        std::string comp_operand;
        for (auto when_operand : when_clause->whenOperand()) {
            auto whenOperand_expressionAtom = when_operand->expressionAtom();
            auto whenOperand_comparisonPredicateCond = when_operand->comparisonPredicateCond();

            if (whenOperand_expressionAtom) {
                visit(whenOperand_expressionAtom);
                comp_operand = "=";
                when_operands.push_back(std::move(current_expr));
            } else if (whenOperand_comparisonPredicateCond) {
                auto comparisonPredicateCond_compOp = whenOperand_comparisonPredicateCond->compOp();

                if (comparisonPredicateCond_compOp->EQUALS_OPERATOR()) {
                    comp_operand = "=";
                } else if (comparisonPredicateCond_compOp->NOT_EQUALS_OPERATOR()) {
                    comp_operand = "!=";
                } else if (comparisonPredicateCond_compOp->LEFT_ANGLE_BRACKET()) {
                    comp_operand = "<";
                } else if (comparisonPredicateCond_compOp->RIGHT_ANGLE_BRACKET()) {
                    comp_operand = ">";
                } else if (comparisonPredicateCond_compOp->LESS_THAN_OR_EQUALS_OPERATOR()) {
                    comp_operand = "<=";
                } else if (comparisonPredicateCond_compOp->GREATER_THAN_OR_EQUALS_OPERATOR()) {
                    comp_operand = ">=";
                }
                visit(whenOperand_comparisonPredicateCond->expression());
                when_operands.push_back(std::move(current_expr));
            }
        }
        std::pair operand = { comp_operand, std::move(when_operands) };
        visit(when_clause->expression());
        std::pair clause = { std::move(operand), std::move(current_expr) };
        when_clauses.push_back(std::move(clause));
    }

    std::unique_ptr<Expr> else_clause;
    if (auto elseClause = ctx->elseClause()) {
        visit(elseClause);
        else_clause = std::move(current_expr);
    } else {
        else_clause = nullptr;
    }

    current_expr = std::make_unique<ExprSimpleCase>(
        std::move(case_operand),
        std::move(when_clauses),
        std::move(else_clause)
    );
    return 0;
}

std::any QueryVisitor::visitGqlSearchedCaseFunction(GQLParser::GqlSearchedCaseFunctionContext* ctx)
{
    LOG_VISITOR
    std::vector<std::pair<std::unique_ptr<Expr>, std::unique_ptr<Expr>>> when_clauses;

    for (auto when_clause : ctx->searchedWhenClause()) {
        auto when_clause_expression = when_clause->expression();
        visit(when_clause_expression[0]);
        auto condition = std::move(current_expr);
        visit(when_clause_expression[1]);
        auto result = std::move(current_expr);
        when_clauses.emplace_back(std::make_pair(std::move(condition), std::move(result)));
    }

    std::unique_ptr<Expr> else_clause;
    if (auto elseClause = ctx->elseClause()) {
        visit(elseClause);
        else_clause = std::move(current_expr);
    } else {
        else_clause = nullptr;
    }

    current_expr = std::make_unique<ExprSearchedCase>(std::move(when_clauses), std::move(else_clause));
    return 0;
}

std::any QueryVisitor::visitCastFunction(GQLParser::CastFunctionContext* ctx)
{
    // TODO: visitor currently only considers predefined types when processing the CastFunction.
    //       However, valueType can include various other types, such as path types, list types,
    //       record types, and dynamic union types.
    LOG_VISITOR;
    visit(ctx->expression());
    GQL_OID::GenericType target = GQL_OID::GenericType::NULL_ID; // to avoid warnings
    if (auto predefTypeCtx = dynamic_cast<GQLParser::PredefTypeContext*>(ctx->valueType())) {
        auto predefinedTypeCtx = dynamic_cast<GQLParser::PredefinedTypeContext*>(predefTypeCtx->children[0]);

        if (predefinedTypeCtx) {
            for (auto child : predefinedTypeCtx->children) {
                if (dynamic_cast<GQLParser::BooleanTypeContext*>(child)) {
                    target = GQL_OID::GenericType::BOOL;
                    break;
                } else if (dynamic_cast<GQLParser::CharacterStringTypeContext*>(child)) {
                    target = GQL_OID::GenericType::STRING;
                    break;
                } else if (dynamic_cast<GQLParser::NumericTypeContext*>(child)) {
                    target = GQL_OID::GenericType::NUMERIC;
                    break;
                } else if (dynamic_cast<GQLParser::TemporalTypeContext*>(child)) {
                    target = GQL_OID::GenericType::DATE;
                    break;
                }
            }
        }
    }

    current_expr = std::make_unique<ExprCast>(std::move(current_expr), std::move(target));
    return 0;
}

std::any QueryVisitor::visitGqlCountAllFunction(GQLParser::GqlCountAllFunctionContext*)
{
    LOG_VISITOR
    current_expr = std::make_unique<ExprAggCountAll>(get_query_ctx().get_internal_var());
    return 0;
}

std::any QueryVisitor::visitGqlGeneralSetFunction(GQLParser::GqlGeneralSetFunctionContext* ctx)
{
    LOG_VISITOR
    visit(ctx->expression());
    distinct = false;
    auto setQuantifier = ctx->setQuantifier();
    if (setQuantifier) {
        if (setQuantifier->DISTINCT()) {
            distinct = true;
        }
    }

    VarId agg_var = get_query_ctx().get_internal_var();

    if (ctx->generalSetFunctionType()->COUNT()) {
        current_expr = std::make_unique<ExprAggCount>(std::move(current_expr), distinct, agg_var);
    } else if (ctx->generalSetFunctionType()->AVG()) {
        current_expr = std::make_unique<ExprAggAvg>(std::move(current_expr), distinct, agg_var);
    } else if (ctx->generalSetFunctionType()->MAX()) {
        current_expr = std::make_unique<ExprAggMax>(std::move(current_expr), distinct, agg_var);
    } else if (ctx->generalSetFunctionType()->MIN()) {
        current_expr = std::make_unique<ExprAggMin>(std::move(current_expr), distinct, agg_var);
    } else if (ctx->generalSetFunctionType()->SUM()) {
        current_expr = std::make_unique<ExprAggSum>(std::move(current_expr), distinct, agg_var);
    } else if (ctx->generalSetFunctionType()->STDDEV_POP()) {
        current_expr = std::make_unique<ExprAggStddevPop>(std::move(current_expr), distinct, agg_var);
    } else if (ctx->generalSetFunctionType()->STDDEV_SAMP()) {
        current_expr = std::make_unique<ExprAggStddevSamp>(std::move(current_expr), distinct, agg_var);
    } else if (ctx->generalSetFunctionType()->COLLECT()) {
        current_expr = std::make_unique<ExprAggCollect>(std::move(current_expr), distinct, agg_var);
    }
    return 0;
}

std::any QueryVisitor::visitGqlBinarySetFunction(GQLParser::GqlBinarySetFunctionContext* ctx)
{
    LOG_VISITOR
    visit(ctx->lhs);
    auto expr = std::move(current_expr);
    visit(ctx->rhs);
    auto percentile = std::move(current_expr);

    distinct = false;
    auto setQuantifier = ctx->setQuantifier();
    if (setQuantifier) {
        if (setQuantifier->DISTINCT()) {
            distinct = true;
        }
    }

    VarId agg_var = get_query_ctx().get_internal_var();

    if (ctx->binarySetFunctionType()->PERCENTILE_CONT()) {
        current_expr = std::make_unique<ExprAggPercentileCont>(
            std::move(expr),
            std::move(percentile),
            distinct,
            agg_var
        );
    } else if (ctx->binarySetFunctionType()->PERCENTILE_DISC()) {
        current_expr = std::make_unique<ExprAggPercentileDisc>(
            std::move(expr),
            std::move(percentile),
            distinct,
            agg_var
        );
    }
    return 0;
}

std::any QueryVisitor::visitPropertyReference(GQLParser::PropertyReferenceContext* ctx)
{
    LOG_VISITOR
    std::string var_name = ctx->variable()->getText();
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

    auto labeledPredicateCond = ctx->labeledPredicateCond();
    visit(labeledPredicateCond->labelExpression());
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
    uint64_t integer = get_unsigned_integer(integer_str);
    ObjectId oid = Common::Conversions::pack_int(integer);
    current_expr = std::make_unique<ExprTerm>(oid);
    return 0;
}

std::any QueryVisitor::visitGqlUnaryExpression(GQLParser::GqlUnaryExpressionContext* ctx)
{
    LOG_VISITOR
    std::string integer_str = ctx->expressionAtom()->getText();

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
    ObjectId oid;
    uint64_t integer = std::stoull(integer_str);
    if (ctx->unaryOperator()->MINUS_SIGN()) {
        oid = Common::Conversions::pack_int(-integer);
    } else {
        oid = Common::Conversions::pack_int(integer);
    }
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
    if (ctx->K_TRUE()) {
        ObjectId oid = Common::Conversions::pack_bool(true);
        current_expr = std::make_unique<ExprTerm>(oid);
    } else if (ctx->K_FALSE()) {
        ObjectId oid = Common::Conversions::pack_bool(false);
        current_expr = std::make_unique<ExprTerm>(oid);
    } else {
        current_expr = std::make_unique<ExprTerm>(ObjectId::get_null());
    }
    return 0;
}

std::any QueryVisitor::visitListValueConstructor(GQLParser::ListValueConstructorContext* ctx)
{
    std::vector<ObjectId> list;

    for (auto& expr : ctx->expression()) {
        visit(expr);
        if (current_expr != nullptr) {
            if (auto expr_term = dynamic_cast<ExprTerm*>(current_expr.get())) {
                list.push_back(expr_term->term);
            }
        }
    }
    ObjectId list_oid = Conversions::pack_list(list);
    current_expr = std::make_unique<ExprTerm>(list_oid);
    return 0;
}

std::any QueryVisitor::visitListLiteral(GQLParser::ListLiteralContext* ctx)
{
    std::vector<ObjectId> list;

    for (auto& expr : ctx->generalLiteral()) {
        visit(expr);
        if (current_expr != nullptr) {
            if (auto expr_term = dynamic_cast<ExprTerm*>(current_expr.get())) {
                list.push_back(expr_term->term);
            }
        }
    }
    ObjectId list_oid = Conversions::pack_list(list);
    current_expr = std::make_unique<ExprTerm>(list_oid);
    return 0;
}

std::any QueryVisitor::visitDateFunction(GQLParser::DateFunctionContext* ctx)
{
    // DATE
    auto dateFunctionParameters = ctx->dateFunctionParameters();
    if (dateFunctionParameters) {
        if (dateFunctionParameters->dateString()) {
            std::string date_str = dateFunctionParameters->getText();
            date_str = date_str.substr(1, date_str.size() - 2);
            auto date = DateTime::from_date(date_str);
            current_expr = std::make_unique<ExprTerm>(Conversions::pack_date(date));
        }
    }
    // CURRENT_DATE
    else
    {
        std::time_t t = std::chrono::system_clock::to_time_t(get_query_ctx().thread_info.time_start);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&t), "%Y-%m-%d");
        ObjectId oid(DateTime::from_date(ss.str()));
        current_expr = std::make_unique<ExprTerm>(oid);
    }
    return 0;
}

std::any QueryVisitor::visitTimeFunction(GQLParser::TimeFunctionContext* ctx)
{
    // ZONED_TIME
    auto timeFunctionParameters = ctx->timeFunctionParameters();
    if (timeFunctionParameters) {
        std::string date_str = timeFunctionParameters->getText();
        date_str = date_str.substr(1, date_str.size() - 2);
        auto date = DateTime::from_zoned_time(date_str);
        current_expr = std::make_unique<ExprTerm>(Conversions::pack_date(date));
    }
    // CURRENT_TIME
    else
    {
        std::time_t t = std::chrono::system_clock::to_time_t(get_query_ctx().thread_info.time_start);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&t), "%H:%M:%S");
        ObjectId oid(DateTime::from_zoned_time(ss.str()));
        current_expr = std::make_unique<ExprTerm>(oid);
    }
    return 0;
}

std::any QueryVisitor::visitLocalTimeFunction(GQLParser::LocalTimeFunctionContext* ctx)
{
    // LOCAL_TIME
    auto timeFunctionParameters = ctx->timeFunctionParameters();
    if (timeFunctionParameters) {
        std::string date_str = timeFunctionParameters->getText();
        date_str = date_str.substr(1, date_str.size() - 2);
        auto date = DateTime::from_local_time(date_str);
        current_expr = std::make_unique<ExprTerm>(Conversions::pack_date(date));
    } else {
        std::time_t t = std::chrono::system_clock::to_time_t(get_query_ctx().thread_info.time_start);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&t), "%H:%M:%S");
        ObjectId oid(DateTime::from_local_time(ss.str()));
        current_expr = std::make_unique<ExprTerm>(oid);
    }
    return 0;
}

std::any QueryVisitor::visitDatetimeFunction(GQLParser::DatetimeFunctionContext* ctx)
{
    // ZONED_DATETIME
    auto datetimeFunctionParameters = ctx->datetimeFunctionParameters();
    if (datetimeFunctionParameters) {
        std::string date_str = datetimeFunctionParameters->getText();
        date_str = date_str.substr(1, date_str.size() - 2);
        auto date = DateTime::from_zoned_datetime(date_str);
        current_expr = std::make_unique<ExprTerm>(Conversions::pack_date(date));
    }
    // CURRENT_TIMESTAMP
    else
    {
        std::time_t t = std::chrono::system_clock::to_time_t(get_query_ctx().thread_info.time_start);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&t), "%Y-%m-%dT%H:%M:%SZ");
        std::cout << ss.str() << std::endl;
        ObjectId oid(DateTime::from_zoned_datetime(ss.str()));
        current_expr = std::make_unique<ExprTerm>(oid);
    }
    return 0;
}

std::any QueryVisitor::visitLocalDatetimeFunction(GQLParser::LocalDatetimeFunctionContext* ctx)
{
    // LOCAL_DATETIME
    auto datetimeFunctionParameters = ctx->datetimeFunctionParameters();
    if (datetimeFunctionParameters) {
        std::string date_str = datetimeFunctionParameters->getText();
        date_str = date_str.substr(1, date_str.size() - 2);
        auto date = DateTime::from_local_datetime(date_str);
        current_expr = std::make_unique<ExprTerm>(Conversions::pack_date(date));
    }
    // LOCAL_TIMESTAMP
    else
    {
        std::time_t t = std::chrono::system_clock::to_time_t(get_query_ctx().thread_info.time_start);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&t), "%Y-%m-%dT%H:%M:%S");
        ObjectId oid(DateTime::from_local_datetime(ss.str()));
        current_expr = std::make_unique<ExprTerm>(oid);
    }
    return 0;
}

std::any QueryVisitor::visitLabelsFunction(GQLParser::LabelsFunctionContext* ctx)
{
    LOG_VISITOR
    VarId var = get_query_ctx().get_or_create_var(ctx->variable()->getText());
    VarType::Type type = singleton_types[var];
    current_expr = std::make_unique<ExprLabels>(var, type);
    return 0;
}

std::any QueryVisitor::visitPropertiesFunction(GQLParser::PropertiesFunctionContext* ctx)
{
    LOG_VISITOR
    VarId var = get_query_ctx().get_or_create_var(ctx->variable()->getText());
    VarType::Type type = singleton_types[var];
    current_expr = std::make_unique<ExprProperties>(var, type);
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
    if (auto orderByClause = ctx->orderByClause()) {
        orderByClause->accept(this);
    }

    uint64_t offset = Op::DEFAULT_OFFSET;
    uint64_t limit = Op::DEFAULT_LIMIT;
    if (auto offsetClause = ctx->offsetClause()) {
        std::string offset_str = offsetClause->unsignedIntegerSpecification()->getText();
        offset = get_unsigned_integer(offset_str);
    }
    if (auto limitClause = ctx->limitClause()) {
        std::string limit_str = limitClause->unsignedIntegerSpecification()->getText();
        limit = get_unsigned_integer(limit_str);
    }

    current_op = std::make_unique<OpOrderBy>(
        std::move(order_by_items),
        std::move(order_by_ascending),
        std::move(order_nulls),
        offset,
        limit
    );
    return 0;
}

std::any QueryVisitor::visitOrderByClause(GQLParser::OrderByClauseContext* ctx)
{
    LOG_VISITOR
    for (auto& oc : ctx->sortSpecificationList()->sortSpecification()) {
        auto nullOrdering = oc->nullOrdering();
        if (auto sortKey = oc->sortKey()) {
            visit(sortKey);
            order_by_items.emplace_back(std::move(current_expr));
        } else {
            // it should not enter here unless grammar is modified
            throw QuerySemanticException("Unsupported ORDER BY condition: '" + oc->getText() + "'");
        }
        if (auto orderingSpecification = oc->orderingSpecification()) {
            if (orderingSpecification->DESC() || orderingSpecification->DESCENDING()) {
                order_by_ascending.push_back(false);
                if (nullOrdering) {
                    auto null_order = nullOrdering->getText();
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
                if (nullOrdering) {
                    auto null_order = nullOrdering->getText();
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
            if (nullOrdering) {
                auto null_order = nullOrdering->getText();
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

std::any QueryVisitor::visitOffsetClause(GQLParser::OffsetClauseContext* ctx)
{
    LOG_VISITOR
    std::string offset_str = ctx->unsignedIntegerSpecification()->getText();
    offset = get_unsigned_integer(offset_str);
    return 0;
}

std::any QueryVisitor::visitLimitClause(GQLParser::LimitClauseContext* ctx)
{
    LOG_VISITOR
    std::string limit_str = ctx->unsignedIntegerSpecification()->getText();
    limit = get_unsigned_integer(limit_str);
    return 0;
}

std::any QueryVisitor::visitFilterStatement(GQLParser::FilterStatementContext* ctx)
{
    LOG_VISITOR
    if (auto whereClause = ctx->whereClause()) {
        visit(whereClause->expression());
        filter_items.emplace_back(std::move(current_expr));
    }
    if (auto filterExpression = ctx->expression()) {
        visit(filterExpression);
        filter_items.emplace_back(std::move(current_expr));
    }
    return 0;
}

uint64_t QueryVisitor::get_unsigned_integer(std::string& integer_str)
{
    integer_str.erase(std::remove(integer_str.begin(), integer_str.end(), '_'), integer_str.end());

    if (integer_str.size() > 2) {
        std::string integer_prefix = integer_str.substr(0, 2);
        std::string integer_suffix = integer_str.substr(2);

        if (integer_prefix == "0b") {
            return std::stoull(integer_suffix, nullptr, 2);
        } else if (integer_prefix == "0o") {
            return std::stoull(integer_suffix, nullptr, 8);
        } else if (integer_prefix == "0x") {
            return std::stoull(integer_suffix, nullptr, 16);
        }
    }
    return std::stoull(integer_str);
}

std::any QueryVisitor::visitSessionCloseCommand(GQLParser::SessionCloseCommandContext*)
{
    throw NotSupportedException("Session");
}

std::any QueryVisitor::visitSessionActivityCommand(GQLParser::SessionActivityCommandContext*)
{
    throw NotSupportedException("Session");
}

std::any QueryVisitor::visitStartTransactionCommand(GQLParser::StartTransactionCommandContext*)
{
    throw NotSupportedException("Transaction");
}

std::any QueryVisitor::visitEndTransactionCommand(GQLParser::EndTransactionCommandContext*)
{
    throw NotSupportedException("Transaction");
}

std::any
    QueryVisitor::visitLinearCatalogModifyingStatement(GQLParser::LinearCatalogModifyingStatementContext*)
{
    throw NotSupportedException("Create/Drop");
}

std::any
    QueryVisitor::visitPrimitiveDataModifyingStatement(GQLParser::PrimitiveDataModifyingStatementContext*)
{
    throw NotSupportedException("Update");
}

std::any QueryVisitor::visitBindingVariableDefinitionBlock(GQLParser::BindingVariableDefinitionBlockContext*)
{
    throw NotSupportedException("Variable definition block");
}

std::any QueryVisitor::visitCallQueryStatement(GQLParser::CallQueryStatementContext*)
{
    throw NotSupportedException("Call");
}

std::any QueryVisitor::visitForStatement(GQLParser::ForStatementContext*)
{
    throw NotSupportedException("For");
}

std::any QueryVisitor::visitQueryConjunction(GQLParser::QueryConjunctionContext*)
{
    throw NotSupportedException("Composite query");
}

std::any QueryVisitor::visitNestedDataModifyingProcedure(GQLParser::NestedDataModifyingProcedureContext*)
{
    throw NotSupportedException("Nested procedure");
}

std::any QueryVisitor::visitNextStatement(GQLParser::NextStatementContext*)
{
    throw NotSupportedException("Composite query");
}

std::any QueryVisitor::visitOptionalOperand(GQLParser::OptionalOperandContext*)
{
    throw NotSupportedException("Optional match");
}

std::any QueryVisitor::visitGraphPatternYieldClause(GQLParser::GraphPatternYieldClauseContext*)
{
    throw NotSupportedException("Yield");
}

std::any QueryVisitor::visitNestedProcedureSpecification(GQLParser::NestedProcedureSpecificationContext*)
{
    throw NotSupportedException("Nested procedure");
}

std::any QueryVisitor::visitGraphExpression(GQLParser::GraphExpressionContext*)
{
    throw NotSupportedException("Graph expression");
}

std::any QueryVisitor::visitBindingTableExpression(GQLParser::BindingTableExpressionContext*)
{
    throw NotSupportedException("Binding table expression");
}


#include "query_visitor.h"

#include <sstream>
#include <variant>

#include "graph_models/common/datatypes/datetime.h"
#include "graph_models/object_id.h"
#include "graph_models/rdf_model/conversions.h"
#include "graph_models/rdf_model/rdf_model.h"
#include "graph_models/rdf_model/rdf_object_id.h"
#include "misc/transliterator.h"
#include "misc/unicode_escape.h"
#include "query/exceptions.h"
#include "query/id.h"
#include "query/parser/expr/sparql/exprs.h"
#include "query/parser/grammar/sparql/mdb_extensions.h"
#include "query/parser/grammar/sparql/sparql_parser_context_traits.h"
#include "query/parser/op/sparql/op_show.h"
#include "query/parser/op/sparql/op_visitor.h"
#include "query/parser/op/sparql/ops.h"
#include "query/parser/paths/path_alternatives.h"
#include "query/parser/paths/path_atom.h"
#include "query/parser/paths/path_kleene_plus.h"
#include "query/parser/paths/path_kleene_star.h"
#include "query/parser/paths/path_negated_set.h"
#include "query/parser/paths/path_optional.h"
#include "query/parser/paths/path_sequence.h"
#include "query/parser/paths/regular_path_expr.h"
#include "query/query_context.h"

using namespace SPARQL;
using antlrcpp::Any;

// #define DEBUG_QUERY_VISITORS

#ifdef DEBUG_QUERY_VISITORS

struct VisitorLogger {
    std::string context;
    static uint64_t indentation_level;

    VisitorLogger(std::string context) : context(context) {
        std::cout << std::string(indentation_level * 2, ' ') << "> " << context << std::endl;
        indentation_level++;
    }

    ~VisitorLogger() {
        indentation_level--;
        std::cout << std::string(indentation_level * 2, ' ') << "< " << context << std::endl;
    }
};
uint64_t VisitorLogger::indentation_level = 0;

#define LOG_VISITOR VisitorLogger logger_instance(__func__);
#define LOG_INFO(arg) std::cout << std::string(VisitorLogger::indentation_level * 2, ' ') << arg << std::endl

#else
#define LOG_VISITOR
#define LOG_INFO(arg)
#endif

Any QueryVisitor::visitConstructQuery(SparqlParser::ConstructQueryContext* ctx) {
    LOG_VISITOR
    if (ctx->triplesTemplate()) {
        visit(ctx->triplesTemplate());
        current_op = std::make_unique<OpBasicGraphPattern>(current_triples, std::vector<OpPath>());

        if (ctx->solutionModifier()) {
            visit(ctx->solutionModifier());
        }

        current_op = std::make_unique<OpConstruct>(
            std::move(current_triples),
            std::move(current_op),
            offset,
            limit
        );
    } else if (ctx->constructTemplate()) {
        visit(ctx->constructTemplate());
        auto triples_template = std::move(current_triples);

        visit(ctx->whereClause());

        if (ctx->solutionModifier()) {
            visit(ctx->solutionModifier());
        }

        current_op = std::make_unique<OpConstruct>(
            std::move(triples_template),
            std::move(current_op),
            offset,
            limit
        );
    }
    return 0;
}

Any QueryVisitor::visitDescribeQuery(SparqlParser::DescribeQueryContext* ctx) {
    LOG_VISITOR
    if (ctx->whereClause()) {
        visit(ctx->whereClause());
    }
    if (ctx->solutionModifier()) {
        visit(ctx->solutionModifier());
    }

    std::vector<VarId> vars;
    std::vector<std::string> iris;

    auto var_iri_contexts = ctx->varOrIRI();
    for (auto& var_iri : var_iri_contexts) {
        if (auto var_ctx = var_iri->var()) {
            auto var_name = var_ctx->getText().substr(1);
            vars.push_back(get_query_ctx().get_or_create_var(var_name));
        } else if (auto iri_ctx = var_iri->iri()) {
            iris.push_back(iriCtxToString(iri_ctx));
        }
    }
    current_op = std::make_unique<OpDescribe>(
        std::move(current_op),
        std::move(vars),
        std::move(iris),
        offset,
        limit
    );

    // TODO: consider datasetClause
    return 0;
}

Any QueryVisitor::visitAskQuery(SparqlParser::AskQueryContext* ctx) {
    LOG_VISITOR
    visitChildren(ctx);
    current_op = std::make_unique<OpAsk>(std::move(current_op));
    return 0;
}

Any QueryVisitor::visitShowQuery(SparqlParser::ShowQueryContext* ctx) {
    LOG_VISITOR

    const auto index_type = ctx->ALPHANUMERIC_IDENTIFIER()->getText();
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

// Root parser rule
Any QueryVisitor::visitQuery(SparqlParser::QueryContext* ctx) {
    LOG_VISITOR
    visitChildren(ctx);
    if (current_op == nullptr) {
        throw QueryParsingException("Empty query", 0, 0);
    }
    return 0;
}

Any QueryVisitor::visitPrologue(SparqlParser::PrologueContext* ctx) {
    LOG_VISITOR
    visitChildren(ctx);
    for (auto&& [alias, iri_prefix] : rdf_model.default_query_prefixes) {
        if (global_info.iri_prefix_map.find(alias) == global_info.iri_prefix_map.end()) {
            global_info.iri_prefix_map.insert({alias, iri_prefix});
        }
    }
    return 0;
}


Any QueryVisitor::visitBaseDecl(SparqlParser::BaseDeclContext* ctx) {
    LOG_VISITOR
    std::string base_iri = ctx->IRIREF()->getText();
    global_info.base_iri = base_iri.substr(1, base_iri.size() - 2); // remove '<' ... '>'
    return 0;
}


Any QueryVisitor::visitPrefixDecl(SparqlParser::PrefixDeclContext* ctx) {
    LOG_VISITOR
    std::string alias = ctx->PNAME_NS()->getText();
    alias = alias.substr(0, alias.size() - 1); // remove ':'

    std::string iri_prefix = ctx->IRIREF()->getText();
    iri_prefix = iri_prefix.substr(1, iri_prefix.size() - 2); // remove '<' ... '>'

    global_info.iri_prefix_map[alias] = iri_prefix;
    return 0;
}


Any QueryVisitor::visitSolutionModifier(SparqlParser::SolutionModifierContext* ctx) {
    LOG_VISITOR
    // LIMIT and OFFSET
    auto limit_offset_clauses = ctx->limitOffsetClauses();
    if (limit_offset_clauses) {
        auto limit_clause = limit_offset_clauses->limitClause();
        if (limit_clause) {
            limit = std::stoull(limit_clause->INTEGER()->getText());
        }
        auto offset_clause = limit_offset_clauses->offsetClause();
        if (offset_clause) {
            offset = std::stoull(offset_clause->INTEGER()->getText());
        }
    }
    // GROUP BY
    auto group_clause = ctx->groupClause();
    if (group_clause) {
        group_by_present = true;
        visit(group_clause);
        current_op = std::make_unique<OpGroupBy>(
            std::move(current_op),
            std::move(group_by_items)
        );
    }

    // HAVING
    auto having_clause = ctx->havingClause();
    if (having_clause) {
        visit(having_clause);
        current_op = std::make_unique<OpHaving>(
            std::move(current_op),
            std::move(having_expressions)
        );
    }

    // ORDER BY
    auto order_clause = ctx->orderClause();
    if (order_clause) {
        visit(order_clause);
        current_op = std::make_unique<OpOrderBy>(
            std::move(current_op),
            std::move(order_by_items),
            std::move(order_by_ascending)
        );
    }
    return 0;
}


Any QueryVisitor::visitOrderClause(SparqlParser::OrderClauseContext* ctx) {
    LOG_VISITOR
    for (auto& oc : ctx->orderCondition()) {
        if (oc->var()) {
            auto var_name = oc->var()->getText().substr(1);
            auto var = get_query_ctx().get_or_create_var(var_name);
            order_by_items.emplace_back(var);
        } else if (oc->expression()) {
            visit(oc->expression());
            order_by_items.emplace_back(std::move(current_expr));
        } else if (oc->constraint()) {
            visit(oc->constraint());
            order_by_items.emplace_back(std::move(current_expr));
        } else {
            // it should not enter here unless grammar is modified
            throw QuerySemanticException("Unsupported ORDER BY condition: '" + oc->getText() + "'");
        }

        if (oc->DESC()) {
            order_by_ascending.push_back(false);
        } else {
            order_by_ascending.push_back(true);
        }
    }
    return 0;
}


Any QueryVisitor::visitSelectQuery(SparqlParser::SelectQueryContext* ctx) {
    LOG_VISITOR
    visit(ctx->whereClause());
    visit(ctx->solutionModifier());
    visit(ctx->selectClause());


    const auto datasetClauses = ctx->datasetClause();
    if (!datasetClauses.empty()){
        std::vector<std::string> from_graphs;
        std::vector<std::string> from_named_graphs;

        for (std::size_t i = 0; i < datasetClauses.size(); ++i) {
            if (datasetClauses[i]->NAMED()) {
                from_named_graphs.push_back(iriCtxToString(datasetClauses[i]->iri()));
            } else {
                from_graphs.push_back(iriCtxToString(datasetClauses[i]->iri()));
            }
        }
        current_op = std::make_unique<OpFrom>(from_graphs, from_named_graphs, std::move(current_op));
    }

    current_op = std::make_unique<OpSelect>(
        std::move(current_op),
        std::move(select_variables),
        std::move(select_variables_expressions),
        ctx->selectClause()->selectModifier() != nullptr &&
        ctx->selectClause()->selectModifier()->DISTINCT() != nullptr,
        limit,
        offset,
        false // is not sub select
    );

    // TODO: consider datasetClause
    return 0;
}

Any QueryVisitor::visitSubSelect(SparqlParser::SubSelectContext* ctx) {
    LOG_VISITOR

    QueryVisitor visitor(global_info);
    visitor.visit(ctx->whereClause());
    visitor.visit(ctx->solutionModifier());
    visitor.visit(ctx->selectClause());

    visitor.current_op = std::make_unique<OpSelect>(
        std::move(visitor.current_op),
        std::move(visitor.select_variables),
        std::move(visitor.select_variables_expressions),
        ctx->selectClause()->selectModifier() != nullptr &&
        ctx->selectClause()->selectModifier()->DISTINCT() != nullptr,
        visitor.limit,
        visitor.offset,
        true // is sub select
    );
    visitor.visit(ctx->valuesClause());
    // TODO: consider datasetClause
    current_op = std::move(visitor.current_op);
    return 0;
}


Any QueryVisitor::visitSelectClause(SparqlParser::SelectClauseContext* ctx) {
    LOG_VISITOR
    visitChildren(ctx);
    if (ctx->ASTERISK() != nullptr) {
        if (group_by_present) {
            throw QuerySemanticException("SELECT * not legal with GROUP BY");
        }

        for (auto& var : current_op->get_scope_vars()) {
            // Prevent storing blank nodes and internal vars
            if (get_query_ctx().get_var_name(var).find("_:") != 0
                && !get_query_ctx().is_internal(var))
            {
                select_variables.push_back(var);
                select_variables_expressions.push_back(nullptr);
            }
        }
    }
    return 0;
}


Any QueryVisitor::visitSelectSingleVariable(SparqlParser::SelectSingleVariableContext* ctx) {
    LOG_VISITOR
    auto var_name = ctx->var()->getText().substr(1);
    select_variables.push_back(get_query_ctx().get_or_create_var(var_name));
    select_variables_expressions.push_back(nullptr);
    return 0;
}

Any QueryVisitor::visitSelectExpressionAsVariable(SparqlParser::SelectExpressionAsVariableContext* ctx) {
    LOG_VISITOR
    auto var_name = ctx->var()->getText().substr(1);
    select_variables.push_back(get_query_ctx().get_or_create_var(var_name));
    visit(ctx->expression());
    select_variables_expressions.push_back(std::move(current_expr));
    return 0;
}

Any QueryVisitor::visitWhereClause(SparqlParser::WhereClauseContext* ctx) {
    LOG_VISITOR
    visit(ctx->groupGraphPattern());
    assert(current_op != nullptr);
    return 0;
}


Any QueryVisitor::visitGroupGraphPatternSub(SparqlParser::GroupGraphPatternSubContext* ctx) {
    LOG_VISITOR
    // 1. Visit the outermost graph pattern
    if (ctx->triplesBlock()) {
        visit(ctx->triplesBlock());
    } else {
        current_op = std::make_unique<OpUnitTable>();
    }
    // add empty list to the stack
    current_filters.push({});

    // 2. Visit special scopes (OPTIONAL, UNION, FILTER, etc.) and other basic graph patterns
    for (auto group_graph_pattern_sub_list : ctx->groupGraphPatternSubList()) {
        visit(group_graph_pattern_sub_list->graphPatternNotTriples());

        if (group_graph_pattern_sub_list->triplesBlock()) {
            std::unique_ptr<Op> lhs_op = std::move(current_op);
            visit(group_graph_pattern_sub_list->triplesBlock());
            // If lhs_op is not an OpUnitTable, we need to join it with the current_op
            if (dynamic_cast<OpUnitTable*>(lhs_op.get()) == nullptr) {
                current_op = std::make_unique<OpJoin>(std::move(lhs_op), std::move(current_op));
            }
        }
    }

    std::vector<std::unique_ptr<Expr>>& filters = current_filters.top();
    if (filters.size() > 0) {
        current_op = std::make_unique<OpFilter>(std::move(current_op), std::move(filters));
    }
    current_filters.pop();
    return 0;
}


Any QueryVisitor::visitGroupOrUnionGraphPattern(SparqlParser::GroupOrUnionGraphPatternContext* ctx) {
    LOG_VISITOR
    std::unique_ptr<Op> lhs_op = std::move(current_op);

    std::vector<std::unique_ptr<Op>> union_ops;
    for (auto& ggp_item : ctx->groupGraphPattern()) {
        visit(ggp_item);
        union_ops.push_back(std::move(current_op));
    }
    // If the union has only one element, we don't need to create a union operator
    if (union_ops.size() == 1) {
        current_op = std::move(union_ops[0]);
    } else {
        current_op = std::make_unique<OpUnion>(std::move(union_ops));
    }
    // If lhs_op is not an OpUnitTable, we need to join it with the current_op
    if (dynamic_cast<OpUnitTable*>(lhs_op.get()) == nullptr) {
        current_op = std::make_unique<OpJoin>(std::move(lhs_op), std::move(current_op));
    }
    return 0;
}


Any QueryVisitor::visitOptionalGraphPattern(SparqlParser::OptionalGraphPatternContext* ctx) {
    LOG_VISITOR
    std::unique_ptr<Op> lhs_op = std::move(current_op);

    visit(ctx->groupGraphPattern());
    current_op = std::make_unique<OpOptional>(std::move(lhs_op), std::move(current_op));
    return 0;
}


Any QueryVisitor::visitMinusGraphPattern(SparqlParser::MinusGraphPatternContext* ctx) {
    LOG_VISITOR
    std::unique_ptr<Op> lhs_op = std::move(current_op);

    visit(ctx->groupGraphPattern());
    current_op = std::make_unique<OpMinus>(std::move(lhs_op), std::move(current_op));
    return 0;
 }


Any QueryVisitor::visitServiceGraphPattern(SparqlParser::ServiceGraphPatternContext* ctx) {
    LOG_VISITOR
    auto lhs_op = std::move(current_op);
    visit(ctx->groupGraphPattern());

    auto start_idx = ctx->groupGraphPattern()->start->getStartIndex();
    auto stop_idx  = ctx->groupGraphPattern()->stop->getStopIndex();
    antlr4::misc::Interval interval(start_idx, stop_idx);
    auto query = ctx->groupGraphPattern()->start->getInputStream()->getText(interval);

    std::string prefixes;
    for (const auto& [alias, iri_prefix]: global_info.iri_prefix_map) { // sends at least the query prefixes
        prefixes += "PREFIX " + alias + ": <" + iri_prefix + ">\n";
    }

    std::optional<std::variant<VarId, std::string>> var_or_iri;
    auto var_or_iri_ctx = ctx->varOrIRI();

    if (var_or_iri_ctx->iri()) {
        var_or_iri = iriCtxToString(var_or_iri_ctx->iri());
    } else {
        auto var_name = var_or_iri_ctx->var()->getText().substr(1);
        var_or_iri = get_query_ctx().get_or_create_var(var_name);
    }

    current_op = std::make_unique<OpService>(
        ctx->SILENT() != nullptr,
        var_or_iri.value(),
        query, prefixes,
        std::move(current_op)
    );

    // If lhs_op is not an OpUnitTable, we need to join it with the current_op
    if (dynamic_cast<OpUnitTable*>(lhs_op.get()) == nullptr) {
        current_op = std::make_unique<OpJoin>(std::move(lhs_op), std::move(current_op));
    }

    return 0;
}


Any QueryVisitor::visitGraphGraphPattern(SparqlParser::GraphGraphPatternContext* ctx) {
    LOG_VISITOR
    std::unique_ptr<Op> lhs_op = std::move(current_op);

    visit(ctx->groupGraphPattern());
    if (ctx->varOrIRI()->iri()) {
        // It's an IRI
        current_op = std::make_unique<OpGraph>(
            iriCtxToString(ctx->varOrIRI()->iri()),
            std::move(current_op)
        );
    } else if (ctx->varOrIRI()->var()){
        // It's a variable
        auto var_name = ctx->varOrIRI()->var()->getText().substr(1);
        auto var = get_query_ctx().get_or_create_var(var_name);
        current_op = std::make_unique<OpGraph>(var, std::move(current_op));
    }
    // If lhs_op is not an OpUnitTable, we need to join it with the current_op
    if (dynamic_cast<OpUnitTable*>(lhs_op.get()) == nullptr) {
        current_op = std::make_unique<OpJoin>(std::move(lhs_op), std::move(current_op));
    }
    return 0;
}


Any QueryVisitor::visitFilter(SparqlParser::FilterContext* ctx) {
    LOG_VISITOR
    visit(ctx->constraint());
    current_filters.top().push_back(std::move(current_expr));
    return 0;
}

Any QueryVisitor::visitProcedure(SparqlParser::ProcedureContext* ctx)
{
    LOG_VISITOR

    const auto procedure_name = ctx->ALPHANUMERIC_IDENTIFIER()->getText();
    const auto procedure_name_lowercased = Transliterator::lowercase(procedure_name);

    OpProcedure::ProcedureType procedure_type;

    if (procedure_name_lowercased == "hnsw_top_k") {
        procedure_type = OpProcedure::ProcedureType::HNSW_TOP_K;
    } else if (procedure_name_lowercased == "hnsw_scan") {
        procedure_type = OpProcedure::ProcedureType::HNSW_SCAN;
    } else if (procedure_name_lowercased == "text_search") {
        procedure_type = OpProcedure::ProcedureType::TEXT_SEARCH;
    } else {
        throw QueryException("Invalid procedure: \"" + procedure_name + "\"");
    }

    std::vector<std::unique_ptr<Expr>> procedure_args;
    auto procedureArguments = ctx->procedureArguments();
    if (procedureArguments) {
        for (const auto& procedureArgument : procedureArguments->expression()){
            visit(procedureArgument);
            procedure_args.emplace_back(std::move(current_expr));
        }
    }

    std::vector<VarId> procedure_bindings;
    auto procedureBindings = ctx->procedureBindings();
    for (const auto& procedureBinding : procedureBindings->var()) {
        const auto var_name = procedureBinding->getText().substr(1);
        const auto var = get_query_ctx().get_or_create_var(var_name);
        procedure_bindings.emplace_back(var);
    }

    assert(!procedure_bindings.empty());

    // validate arguments and variables
    auto validate_args_exact = [&](std::size_t num_args) {
        if (procedure_args.size() != num_args) {
            throw QueryException(
                OpProcedure::get_procedure_string(procedure_type) + " expects exactly "
                + std::to_string(num_args) + " arguments"
            );
        }
    };

    auto validate_args_range = [&](std::size_t min_args, std::size_t max_args) {
        if (procedure_args.size() < min_args || procedure_args.size() > max_args) {
            throw QueryException(
                OpProcedure::get_procedure_string(procedure_type) + " expects "
                + std::to_string(min_args) + "-" + std::to_string(max_args) + " arguments"
            );
        }
    };

    auto validate_bindings = [&](std::size_t max_bindings) {
        while (procedure_bindings.size() < max_bindings) {
            procedure_bindings.emplace_back(get_query_ctx().get_internal_var());
        }

        if (procedure_bindings.size() > max_bindings) {
            throw QueryException(
                OpProcedure::get_procedure_string(procedure_type) + " expects at most "
                + std::to_string(max_bindings) + " bindings"
            );
        }
    };

    switch (procedure_type) {
    case OpProcedure::ProcedureType::HNSW_TOP_K:
    case OpProcedure::ProcedureType::HNSW_SCAN: {
        validate_args_exact(4);
        validate_bindings(2);
        break;
    }
    case OpProcedure::ProcedureType::TEXT_SEARCH: {
        if (procedure_args.size() == 2) {
            // set default arguments
            procedure_args.emplace_back(std::make_unique<ExprTerm>(Conversions::pack_string_simple("prefix"))
            );
        }
        validate_args_range(2, 3);
        validate_bindings(2);
        break;
    }
    default:
        throw NotSupportedException("Unhandled procedure: \"" + procedure_name + "\"");
    }

    std::unique_ptr<Op> lhs_op = std::move(current_op);

    current_op = std::make_unique<OpProcedure>(
        procedure_type,
        std::move(procedure_args),
        std::move(procedure_bindings)
    );

    // If lhs_op is not an OpUnitTable, we need to join it with the current_op
    if (dynamic_cast<OpUnitTable*>(lhs_op.get()) == nullptr) {
        current_op = std::make_unique<OpJoin>(std::move(lhs_op), std::move(current_op));
    }

    return 0;
}

Any QueryVisitor::visitBind(SparqlParser::BindContext* ctx) {
    LOG_VISITOR
    visit(ctx->expression());
    auto expr = std::move(current_expr);

    visit(ctx->var());
    auto var_name = ctx->var()->getText().substr(1);
    auto var = get_query_ctx().get_or_create_var(var_name);

    current_op = std::make_unique<OpBind>(std::move(current_op), var, std::move(expr));
    return 0;
}


Any QueryVisitor::visitValuesClause(SparqlParser::ValuesClauseContext* ctx) {
    LOG_VISITOR

    if (ctx->VALUES() == nullptr) {
        return 0;
    }

    auto op = std::move(current_op);

    visitChildren(ctx);

    if (auto select = dynamic_cast<OpSelect*>(op.get())) {
        select->op = std::make_unique<OpJoin>(std::move(current_op), std::move(select->op));
    } else if (auto ask = dynamic_cast<OpAsk*>(op.get())) {
        ask->op = std::make_unique<OpJoin>(std::move(current_op), std::move(ask->op));
    } else if (auto describe = dynamic_cast<OpDescribe*>(op.get())) {
        describe->op = std::make_unique<OpJoin>(std::move(current_op), std::move(describe->op));
    } else if (auto construct = dynamic_cast<OpConstruct*>(op.get())) {
        construct->op = std::make_unique<OpJoin>(std::move(current_op), std::move(construct->op));
    } else {
        assert(false);
    }

    current_op = std::move(op);
    return 0;
}


Any QueryVisitor::visitInlineDataOneVar(SparqlParser::InlineDataOneVarContext* ctx) {
    LOG_VISITOR
    std::vector<ObjectId> values;
    for (auto block : ctx->dataBlockValue()) {
        if (block->UNDEF()) {
            current_sparql_element = ObjectId::get_null();
        } else {
            visit(block);
        }
        values.push_back(current_sparql_element.get_OID());
    }
    visit(ctx->var());
    std::vector<VarId> vars = { current_sparql_element.get_var() };

    auto values_op = std::make_unique<OpValues>(std::move(vars), std::move(values));
    if (current_op != nullptr && dynamic_cast<OpUnitTable*>(current_op.get()) == nullptr) {
        current_op = std::make_unique<OpJoin>(std::move(current_op), std::move(values_op));
    } else {
        current_op = std::move(values_op);
    }

    return 0;
}


Any QueryVisitor::visitInlineDataFull(SparqlParser::InlineDataFullContext* ctx) {
    LOG_VISITOR
    std::vector<VarId> vars;
    for (auto var : ctx->var()) {
        visit(var);
        vars.push_back(current_sparql_element.get_var());
    }

    auto vars_count = vars.size();

    std::vector<ObjectId> values;
    for (auto blocks : ctx->dataBlockValues()) {
        size_t count = 0;
        for (auto block : blocks->dataBlockValue()) {
            if (block->UNDEF()) {
                current_sparql_element = ObjectId::get_null();
            } else {
                visit(block);
            }
            values.push_back(current_sparql_element.get_OID());
            count++;
        }
        if (count != vars_count) {
            std::stringstream ss;
            ss << "Each VALUES tuple should have " << vars_count << " values, but one has " << count << " values";
            throw QuerySemanticException(ss.str());
        }
    }

    auto values_op = std::make_unique<OpValues>(std::move(vars), std::move(values));
    if (current_op != nullptr && dynamic_cast<OpUnitTable*>(current_op.get()) == nullptr) {
        current_op = std::make_unique<OpJoin>(std::move(current_op), std::move(values_op));
    } else {
        current_op = std::move(values_op);
    }

    return 0;
}


Any QueryVisitor::visitMultiplicativeExpression(SparqlParser::MultiplicativeExpressionContext* ctx) {
    LOG_VISITOR
    visit(ctx->unaryExpression(0));
    assert(current_expr != nullptr);

    const auto unaryExpressions = ctx->unaryExpression();
    assert(ctx->op.size() == unaryExpressions.size() - 1);
    for (size_t i = 1; i < unaryExpressions.size(); ++i) {
        std::unique_ptr<Expr> lhs = std::move(current_expr);
        visit(unaryExpressions[i]);
        assert(current_expr != nullptr);

        switch(ctx->op[i-1]->getType()) {
            case SparqlParser::ASTERISK:
                current_expr = std::make_unique<ExprMultiplication>(std::move(lhs), std::move(current_expr));
                break;
            case SparqlParser::DIVIDE:
                current_expr = std::make_unique<ExprDivision>(std::move(lhs), std::move(current_expr));
                break;
            default:
                // it should not enter here unless grammar is modified
                throw QuerySemanticException("Unhandled multiplicative expression");
        }
    }
    return 0;
}


Any QueryVisitor::visitPrimaryExpression(SparqlParser::PrimaryExpressionContext* ctx) {
    LOG_VISITOR
    if (ctx->expression()) {
        visit(ctx->expression());
    } else if (ctx->builtInCall()) {
        visit(ctx->builtInCall());
    } else if (ctx->iriOrFunction()) {
        auto iri = ctx->iriOrFunction()->iri();
        auto argList = ctx->iriOrFunction()->argList();
        if (argList) {
            handle_function_call(iri, argList);
        } else {
            const auto iri_str = iriCtxToString(iri);
            current_expr = std::make_unique<ExprTerm>(Conversions::pack_iri(iri_str));
        }
    } else if (ctx->rdfLiteral()) {
        visit(ctx->rdfLiteral());
        current_expr = std::make_unique<ExprTerm>(current_sparql_element.get_OID());
    } else if (ctx->numericLiteral()) {
        visit(ctx->numericLiteral());
        current_expr = std::make_unique<ExprTerm>(current_sparql_element.get_OID());
    } else if (ctx->booleanLiteral()) {
        current_expr = std::make_unique<ExprTerm>(
            Conversions::pack_bool(ctx->booleanLiteral()->K_TRUE() != nullptr)
        );
    } else if (ctx->var()) {
        auto var_name = ctx->var()->getText().substr(1);
        auto var = get_query_ctx().get_or_create_var(var_name);
        current_expr = std::make_unique<ExprVar>(var);
    }
    else {
        // it should not enter here unless grammar is modified
        throw QuerySemanticException("Unhandled primary expression");
    }
    return 0;
}


Any QueryVisitor::visitUnaryExpression(SparqlParser::UnaryExpressionContext* ctx) {
    LOG_VISITOR
    visit(ctx->primaryExpression());
    assert(current_expr != nullptr);
    if (ctx->op != nullptr) {
        switch(ctx->op->getType()) {
            case SparqlParser::PLUS_SIGN:
                current_expr = std::make_unique<ExprUnaryPlus>(std::move(current_expr));
                break;
            case SparqlParser::MINUS_SIGN:
                current_expr = std::make_unique<ExprUnaryMinus>(std::move(current_expr));
                break;
            case SparqlParser::NEGATION:
                current_expr = std::make_unique<ExprNot>(std::move(current_expr));
                break;
            default:
                // it should not enter here unless grammar is modified
                throw QuerySemanticException("Unhandled unary expression");
        }
    }
    return 0;
}


Any QueryVisitor::visitAdditiveExpression(SparqlParser::AdditiveExpressionContext* ctx) {
    LOG_VISITOR
    visit(ctx->multiplicativeExpression());
    assert(current_expr != nullptr);

    const auto rhsAdditiveExpressions = ctx->rhsAdditiveExpression();
    for (size_t i = 0; i < rhsAdditiveExpressions.size(); ++i) {
        std::unique_ptr<Expr> additive_lhs = std::move(current_expr);

        auto current_additive_expr = rhsAdditiveExpressions[i];
        auto current_additive_sub_expr = current_additive_expr->rhsAdditiveExpressionSub();

        bool minus = false;
        std::unique_ptr<Expr> multiplicative_rhs;
        if (current_additive_sub_expr->numericLiteralPositive()) {
            visit(current_additive_sub_expr->numericLiteralPositive());
            multiplicative_rhs = std::make_unique<ExprTerm>(current_sparql_element.get_OID());
        } else if (current_additive_sub_expr->numericLiteralNegative()) {
            // minus sign is in the number
            visit(current_additive_sub_expr->numericLiteralNegative());
            multiplicative_rhs = std::make_unique<ExprTerm>(current_sparql_element.get_OID());
        } else if (current_additive_sub_expr->PLUS_SIGN()) {
            visit(current_additive_sub_expr->multiplicativeExpression());
            assert(current_expr != nullptr);
            multiplicative_rhs = std::move(current_expr);
        } else if (current_additive_sub_expr->MINUS_SIGN()) {
            minus = true;
            visit(current_additive_sub_expr->multiplicativeExpression());
            assert(current_expr != nullptr);
            multiplicative_rhs = std::move(current_expr);
        }

        const auto unaryExpressions = current_additive_expr->unaryExpression();
        for (size_t j = 0; unaryExpressions.size(); ++j) {
            visit(unaryExpressions[j]);
            assert(current_expr != nullptr);

            if (current_additive_expr->op[j]->getType() == SparqlParser::ASTERISK) {
                multiplicative_rhs = std::make_unique<ExprMultiplication>(std::move(multiplicative_rhs), std::move(current_expr));
            } else {
                multiplicative_rhs = std::make_unique<ExprDivision>(std::move(multiplicative_rhs), std::move(current_expr));
            }
        }

        if (minus) {
            current_expr = std::make_unique<ExprSubtraction>(std::move(additive_lhs), std::move(multiplicative_rhs));
        } else {
            current_expr = std::make_unique<ExprAddition>(std::move(additive_lhs), std::move(multiplicative_rhs));
        }
    }
    return 0;
}


Any QueryVisitor::visitRelationalExpression(SparqlParser::RelationalExpressionContext* ctx) {
    LOG_VISITOR
    visit(ctx->additiveExpression(0));
    assert(current_expr != nullptr);

    if (ctx->IN() != nullptr) {
        visit(ctx->additiveExpression()[0]);
        auto lhs_expr = std::move(current_expr);
        // IN or NOT IN
        std::vector<std::unique_ptr<Expr>> expr_list;

        if (ctx->expressionList()) {
            for (auto expr_ctx : ctx->expressionList()->expression()) {
                visit(expr_ctx);
                expr_list.push_back(std::move(current_expr));
            }
        }

        if (ctx->NOT() != nullptr) {
            current_expr = std::make_unique<ExprNotIn>(std::move(lhs_expr), std::move(expr_list));
        } else {
            current_expr = std::make_unique<ExprIn>(std::move(lhs_expr), std::move(expr_list));
        }
    } else if (ctx->op != nullptr) {
        std::unique_ptr<Expr> lhs = std::move(current_expr);
        visit(ctx->additiveExpression(1));
        assert(current_expr != nullptr);
        // op=('='|'!='|'<'|'>'|'<='|'>=')
        switch(ctx->op->getType()) {
            case SparqlParser::EQUAL:
                current_expr = std::make_unique<ExprEqual>(std::move(lhs), std::move(current_expr));
                break;
            case SparqlParser::NOT_EQUAL:
                current_expr = std::make_unique<ExprNotEqual>(std::move(lhs), std::move(current_expr));
                break;
            case SparqlParser::LESS:
                current_expr = std::make_unique<ExprLess>(std::move(lhs), std::move(current_expr));
                break;
            case SparqlParser::GREATER:
                current_expr = std::make_unique<ExprGreater>(std::move(lhs), std::move(current_expr));
                break;
            case SparqlParser::LESS_EQUAL:
                current_expr = std::make_unique<ExprLessOrEqual>(std::move(lhs), std::move(current_expr));
                break;
            case SparqlParser::GREATER_EQUAL:
                current_expr = std::make_unique<ExprGreaterOrEqual>(std::move(lhs), std::move(current_expr));
                break;
            default:
                // it should not enter here unless grammar is modified
                throw QuerySemanticException("Unhandled relational expression");
        }
    }
    return 0;
}


Any QueryVisitor::visitConditionalAndExpression(SparqlParser::ConditionalAndExpressionContext* ctx) {
    LOG_VISITOR

    const auto relationalExpressions = ctx->relationalExpression();

    visit(relationalExpressions[0]);
    assert(current_expr != nullptr);

    for (size_t i = 1; i < relationalExpressions.size(); ++i) {
        std::unique_ptr<Expr> lhs = std::move(current_expr);
        visit(relationalExpressions[i]);
        assert(current_expr != nullptr);
        current_expr = std::make_unique<ExprAnd>(std::move(lhs), std::move(current_expr));
    }
    return 0;
}


Any QueryVisitor::visitConditionalOrExpression(SparqlParser::ConditionalOrExpressionContext* ctx) {
    LOG_VISITOR

    const auto conditionalAndExpressions = ctx->conditionalAndExpression();

    visit(conditionalAndExpressions[0]);
    assert(current_expr != nullptr);

    for (size_t i = 1; i < conditionalAndExpressions.size(); ++i) {
        std::unique_ptr<Expr> lhs = std::move(current_expr);
        visit(conditionalAndExpressions[i]);
        assert(current_expr != nullptr);
        current_expr = std::make_unique<ExprOr>(std::move(lhs), std::move(current_expr));
    }
    return 0;
}


Any QueryVisitor::visitBuiltInCall(SparqlParser::BuiltInCallContext* ctx) {
    LOG_VISITOR
    if (ctx->aggregate()) {
        visit(ctx->aggregate());
    }
    else if (ctx->STR()) {
        visit(ctx->expression(0));
        current_expr = std::make_unique<ExprStr>(std::move(current_expr));
    }
    else if (ctx->LANG()) {
        visit(ctx->expression(0));
        current_expr = std::make_unique<ExprLang>(std::move(current_expr));
    }
    else if (ctx->LANGMATCHES()) {
        visit(ctx->expression(0));
        auto expr0 = std::move(current_expr);
        visit(ctx->expression(1));
        auto expr1 = std::move(current_expr);
        current_expr = std::make_unique<ExprLangMatches>(std::move(expr0), std::move(expr1));
    }
    else if (ctx->DATATYPE()) {
        visit(ctx->expression(0));
        current_expr = std::make_unique<ExprDatatype>(std::move(current_expr));
    }
    else if (ctx->BOUND()) {
        auto var_name = ctx->var()->getText().substr(1);
        auto var = get_query_ctx().get_or_create_var(var_name);
        current_expr = std::make_unique<ExprBound>(var);
    }
    else if (ctx->IRI()) {
        visit(ctx->expression(0));
        // IRI needs to know the base IRI of the query
        current_expr = std::make_unique<ExprIRI>(std::move(current_expr), global_info.base_iri);
    }
    else if (ctx->URI()) {
        visit(ctx->expression(0));
        // URI needs to know the base IRI of the query
        current_expr = std::make_unique<ExprURI>(std::move(current_expr), global_info.base_iri);
    }
    else if (ctx->BNODE()) {
        auto expr = ctx->expression(0);
        if (expr) {
            visit(expr);
            current_expr = std::make_unique<ExprBNode>(std::move(current_expr));
        } else {
            current_expr = std::make_unique<ExprBNode>(nullptr);
        }
    }
    else if (ctx->RAND()) {
        current_expr = std::make_unique<ExprRand>();
    }
    else if (ctx->ABS()) {
        visit(ctx->expression(0));
        current_expr = std::make_unique<ExprAbs>(std::move(current_expr));
    }
    else if (ctx->CEIL()) {
        visit(ctx->expression(0));
        current_expr = std::make_unique<ExprCeil>(std::move(current_expr));
    }
    else if (ctx->FLOOR()) {
        visit(ctx->expression(0));
        current_expr = std::make_unique<ExprFloor>(std::move(current_expr));
    }
    else if (ctx->ROUND()) {
        visit(ctx->expression(0));
        current_expr = std::make_unique<ExprRound>(std::move(current_expr));
    }
    else if (ctx->CONCAT()) {
        std::vector<std::unique_ptr<Expr>> expr_list;
        if (ctx->expressionList()) {
            for (auto expr_ctx : ctx->expressionList()->expression()) {
                visit(expr_ctx);
                expr_list.push_back(std::move(current_expr));
            }
        }
        current_expr = std::make_unique<ExprConcat>(std::move(expr_list));
    }
    else if (ctx->subStringExpression()) {
        visit(ctx->subStringExpression());
    }
    else if (ctx->STRLEN()) {
        visit(ctx->expression(0));
        current_expr = std::make_unique<ExprStrLen>(std::move(current_expr));
    }
    else if (ctx->strReplaceExpression()) {
        visit(ctx->strReplaceExpression());
    }
    else if (ctx->UCASE()) {
        visit(ctx->expression(0));
        current_expr = std::make_unique<ExprUCase>(std::move(current_expr));
    }
    else if (ctx->LCASE()) {
        visit(ctx->expression(0));
        current_expr = std::make_unique<ExprLCase>(std::move(current_expr));
    }
    else if (ctx->ENCODE_FOR_URI()) {
        visit(ctx->expression(0));
        current_expr = std::make_unique<ExprEncodeForUri>(std::move(current_expr));
    }
    else if (ctx->CONTAINS()) {
        visit(ctx->expression(0));
        auto expr0 = std::move(current_expr);
        visit(ctx->expression(1));
        auto expr1 = std::move(current_expr);
        current_expr = std::make_unique<ExprContains>(std::move(expr0), std::move(expr1));
    }
    else if (ctx->STRSTARTS()) {
        visit(ctx->expression(0));
        auto expr0 = std::move(current_expr);
        visit(ctx->expression(1));
        auto expr1 = std::move(current_expr);
        current_expr = std::make_unique<ExprStrStarts>(std::move(expr0), std::move(expr1));
    }
    else if (ctx->STRENDS()) {
        visit(ctx->expression(0));
        auto expr0 = std::move(current_expr);
        visit(ctx->expression(1));
        auto expr1 = std::move(current_expr);
        current_expr = std::make_unique<ExprStrEnds>(std::move(expr0), std::move(expr1));
    }
    else if (ctx->STRBEFORE()) {
        visit(ctx->expression(0));
        auto expr0 = std::move(current_expr);
        visit(ctx->expression(1));
        auto expr1 = std::move(current_expr);
        current_expr = std::make_unique<ExprStrBefore>(std::move(expr0), std::move(expr1));
    }
    else if (ctx->STRAFTER()) {
        visit(ctx->expression(0));
        auto expr0 = std::move(current_expr);
        visit(ctx->expression(1));
        auto expr1 = std::move(current_expr);
        current_expr = std::make_unique<ExprStrAfter>(std::move(expr0), std::move(expr1));
    }
    else if (ctx->YEAR()) {
        visit(ctx->expression(0));
        current_expr = std::make_unique<ExprYear>(std::move(current_expr));
    }
    else if (ctx->MONTH()) {
        visit(ctx->expression(0));
        current_expr = std::make_unique<ExprMonth>(std::move(current_expr));
    }
    else if (ctx->DAY()) {
        visit(ctx->expression(0));
        current_expr = std::make_unique<ExprDay>(std::move(current_expr));
    }
    else if (ctx->HOURS()) {
        visit(ctx->expression(0));
        current_expr = std::make_unique<ExprHours>(std::move(current_expr));
    }
    else if (ctx->MINUTES()) {
        visit(ctx->expression(0));
        current_expr = std::make_unique<ExprMinutes>(std::move(current_expr));
    }
    else if (ctx->SECONDS()) {
        visit(ctx->expression(0));
        current_expr = std::make_unique<ExprSeconds>(std::move(current_expr));
    }
    else if (ctx->TIMEZONE()) {
        visit(ctx->expression(0));
        current_expr = std::make_unique<ExprTimezone>(std::move(current_expr));
    }
    else if (ctx->TZ()) {
        visit(ctx->expression(0));
        current_expr = std::make_unique<ExprTZ>(std::move(current_expr));
    }
    else if (ctx->NOW()) {
        current_expr = std::make_unique<ExprNow>();
    }
    else if (ctx->UUID()) {
        current_expr = std::make_unique<ExprUUID>();
    }
    else if (ctx->STRUUID()) {
        current_expr = std::make_unique<ExprStrUUID>();
    }
    else if (ctx->MD5()) {
        visit(ctx->expression(0));
        current_expr = std::make_unique<ExprMD5>(std::move(current_expr));
    }
    else if (ctx->SHA1()) {
        visit(ctx->expression(0));
        current_expr = std::make_unique<ExprSHA1>(std::move(current_expr));
    }
    else if (ctx->SHA256()) {
        visit(ctx->expression(0));
        current_expr = std::make_unique<ExprSHA256>(std::move(current_expr));
    }
    else if (ctx->SHA384()) {
        visit(ctx->expression(0));
        current_expr = std::make_unique<ExprSHA384>(std::move(current_expr));
    }
    else if (ctx->SHA512()) {
        visit(ctx->expression(0));
        current_expr = std::make_unique<ExprSHA512>(std::move(current_expr));
    }
    else if (ctx->COALESCE()) {
        std::vector<std::unique_ptr<Expr>> expr_list;
        if (ctx->expressionList()) {
            for (auto expr_ctx : ctx->expressionList()->expression()) {
                visit(expr_ctx);
                expr_list.push_back(std::move(current_expr));
            }
        }
        current_expr = std::make_unique<ExprCoalesce>(std::move(expr_list));
    }
    else if (ctx->IF()) {
        visit(ctx->expression(0));
        auto expr0 = std::move(current_expr);
        visit(ctx->expression(1));
        auto expr1 = std::move(current_expr);
        visit(ctx->expression(2));
        auto expr2 = std::move(current_expr);
        current_expr = std::make_unique<ExprIf>(std::move(expr0),
                                                std::move(expr1),
                                                std::move(expr2));
    }
    else if (ctx->STRLANG()) {
        visit(ctx->expression(0));
        auto expr0 = std::move(current_expr);
        visit(ctx->expression(1));
        auto expr1 = std::move(current_expr);
        current_expr = std::make_unique<ExprStrLang>(std::move(expr0), std::move(expr1));
    }
    else if (ctx->STRDT()) {
        visit(ctx->expression(0));
        auto expr0 = std::move(current_expr);
        visit(ctx->expression(1));
        auto expr1 = std::move(current_expr);
        current_expr = std::make_unique<ExprStrDT>(std::move(expr0), std::move(expr1));
    }
    else if (ctx->SAMETERM()) {
        visit(ctx->expression(0));
        auto expr0 = std::move(current_expr);
        visit(ctx->expression(1));
        auto expr1 = std::move(current_expr);
        current_expr = std::make_unique<ExprSameTerm>(std::move(expr0), std::move(expr1));
    }
    else if (ctx->ISIRI()) {
        visit(ctx->expression(0));
        current_expr = std::make_unique<ExprIsIRI>(std::move(current_expr));
    }
    else if (ctx->ISURI()) {
        visit(ctx->expression(0));
        current_expr = std::make_unique<ExprIsURI>(std::move(current_expr));
    }
    else if (ctx->ISBLANK()) {
        visit(ctx->expression(0));
        current_expr = std::make_unique<ExprIsBlank>(std::move(current_expr));
    }
    else if (ctx->ISLITERAL()) {
        visit(ctx->expression(0));
        current_expr = std::make_unique<ExprIsLiteral>(std::move(current_expr));
    }
    else if (ctx->ISNUMERIC()) {
        visit(ctx->expression(0));
        current_expr = std::make_unique<ExprIsNumeric>(std::move(current_expr));
    }
    else if (ctx->regexExpression()) {
        visit(ctx->regexExpression());
    }
    else if (ctx->existsFunction()) {
        visit(ctx->existsFunction());
    }
    else if (ctx->notExistsFunction()) {
        visit(ctx->notExistsFunction());
    }
    else {
        // it should not enter here unless grammar is modified
        throw QuerySemanticException("Unhandled built-in call: \"" + ctx->getText() + '"');
    }
    return 0;
}


Any QueryVisitor::visitAggregate(SparqlParser::AggregateContext* ctx) {
    LOG_VISITOR
    bool distinct = ctx->DISTINCT() != nullptr;
    if (ctx->expression()) {
        // only COUNT(*) doesn't have an expression
        visit(ctx->expression());
    }
    if (ctx->COUNT()) {
        if (ctx->ASTERISK()) {
            current_expr = std::make_unique<ExprAggCountAll>(distinct);
        } else {
            current_expr = std::make_unique<ExprAggCount>(std::move(current_expr), distinct);
        }
    } else if (ctx->SUM()) {
        current_expr = std::make_unique<ExprAggSum>(std::move(current_expr), distinct);
    } else if (ctx->MIN()) {
        current_expr = std::make_unique<ExprAggMin>(std::move(current_expr), distinct);
    } else if (ctx->MAX()) {
        current_expr = std::make_unique<ExprAggMax>(std::move(current_expr), distinct);
    } else if (ctx->AVG()) {
        current_expr = std::make_unique<ExprAggAvg>(std::move(current_expr), distinct);
    } else if (ctx->SAMPLE()) {
        current_expr = std::make_unique<ExprAggSample>(std::move(current_expr), distinct);
    } else if (ctx->GROUP_CONCAT()) {
        std::string separator = " "; // default separator
        if (ctx->SEPARATOR()) {
            separator =  ctx->string()->getText();
            separator = separator.substr(1, separator.size() - 2); // delete quotes
        }
        current_expr = std::make_unique<ExprAggGroupConcat>(std::move(current_expr), separator, distinct);
    } else {
        // it should not enter here unless grammar is modified
        throw QuerySemanticException("Unhandled aggregate: \"" + ctx->getText() + '"');
    }
    return 0;
}


Any QueryVisitor::visitSubStringExpression(SparqlParser::SubStringExpressionContext* ctx) {
    LOG_VISITOR

    const auto expressions = ctx->expression();

    visit(expressions[0]);
    auto expr0 = std::move(current_expr);
    visit(expressions[1]);
    auto expr1 = std::move(current_expr);

    if (expressions.size() == 3) {
        visit(expressions[2]);
        auto expr2 = std::move(current_expr);
        current_expr = std::make_unique<ExprSubStr>(std::move(expr0),
                                                    std::move(expr1),
                                                    std::move(expr2));
    } else {
        current_expr = std::make_unique<ExprSubStr>(std::move(expr0),
                                                    std::move(expr1));
    }
    return 0;
}


Any QueryVisitor::visitStrReplaceExpression(SparqlParser::StrReplaceExpressionContext* ctx) {
    LOG_VISITOR

    const auto expressions = ctx->expression();

    visit(expressions[0]);
    auto expr0 = std::move(current_expr);
    visit(expressions[1]);
    auto expr1 = std::move(current_expr);
    visit(expressions[2]);
    auto expr2 = std::move(current_expr);

    if (expressions.size() == 4) {
        visit(expressions[3]);
        auto expr3 = std::move(current_expr);
        current_expr = std::make_unique<ExprReplace>(std::move(expr0),
                                                     std::move(expr1),
                                                     std::move(expr2),
                                                     std::move(expr3));
    } else {
        current_expr = std::make_unique<ExprReplace>(std::move(expr0),
                                                     std::move(expr1),
                                                     std::move(expr2));
    }
    return 0;
}


Any QueryVisitor::visitRegexExpression(SparqlParser::RegexExpressionContext* ctx) {
    LOG_VISITOR

    const auto expressions = ctx->expression();

    visit(expressions[0]);
    auto expr0 = std::move(current_expr);
    visit(expressions[1]);
    auto expr1 = std::move(current_expr);

    if (expressions.size() == 3) {
        visit(expressions[2]);
        auto expr2 = std::move(current_expr);
        current_expr = std::make_unique<ExprRegex>(std::move(expr0),
                                                   std::move(expr1),
                                                   std::move(expr2));
    } else {
        current_expr = std::make_unique<ExprRegex>(std::move(expr0),
                                                   std::move(expr1));
    }
    return 0;
}


Any QueryVisitor::visitExistsFunction(SparqlParser::ExistsFunctionContext* ctx) {
    LOG_VISITOR
    auto previous_op = std::move(current_op);
    visit(ctx->groupGraphPattern());
    assert(current_op != nullptr);
    current_expr = std::make_unique<ExprExists>(std::move(current_op));
    current_op = std::move(previous_op);
    return 0;
}


Any QueryVisitor::visitNotExistsFunction(SparqlParser::NotExistsFunctionContext* ctx) {
    LOG_VISITOR
    auto previous_op = std::move(current_op);
    visit(ctx->groupGraphPattern());
    assert(current_op != nullptr);
    current_expr = std::make_unique<ExprNotExists>(std::move(current_op));
    current_op = std::move(previous_op);
    return 0;
}

Any QueryVisitor::visitFunctionCall(SparqlParser::FunctionCallContext* ctx) {
    LOG_VISITOR

    handle_function_call(ctx->iri(), ctx->argList());
    return 0;
}


Any QueryVisitor::visitTriplesBlock(SparqlParser::TriplesBlockContext* ctx) {
    LOG_VISITOR
    for (auto& triples_same_subject_path : ctx->triplesSameSubjectPath()) {
        visit(triples_same_subject_path);
    }
    current_op = std::make_unique<OpBasicGraphPattern>(
        std::move(current_triples),
        std::move(current_paths)
    );
    return 0;
}


Any QueryVisitor::visitTriplesSameSubjectPath(SparqlParser::TriplesSameSubjectPathContext* ctx) {
    LOG_VISITOR
    if (ctx->varOrTerm()) {
        // 1. Visit the subject
        visit(ctx->varOrTerm());
        subject_stack.push(current_sparql_element.get_ID());
        // 2. Visit the predicate object list
        visit(ctx->propertyListPathNotEmpty());
        subject_stack.pop();
    }
    else {
        // Create new blank node
        LOG_INFO("Creating blank node in:" << "TriplesSameSubjectPathContext");
        auto new_blank_node = get_new_blank_node_var();

        subject_stack.push(std::move(new_blank_node));
        // Visit the inner predicates/object lists
        visit(ctx->triplesNodePath());
        // Visit the outer predicate/object list
        visit(ctx->propertyListPath());
        subject_stack.pop();
    }
    return 0;
}


Any QueryVisitor::visitTriplesSameSubject(SparqlParser::TriplesSameSubjectContext* ctx) {
    LOG_VISITOR
    if (ctx->varOrTerm()) {
        // 1. Visit the subject
        visit(ctx->varOrTerm());
        subject_stack.push(current_sparql_element.get_ID());
        // 2. Visit the predicate object list
        visit(ctx->propertyListNotEmpty());
        subject_stack.pop();
    }
    else {
        // Create new blank node
        LOG_INFO("Creating blank node in:" << "TriplesSameSubjectContext");
        auto new_blank_node = get_new_blank_node_var();

        subject_stack.push(std::move(new_blank_node));
        // Visit the inner predicates/object lists
        visit(ctx->triplesNode());
        // Visit the outer predicate/object list
        visit(ctx->propertyList());
        subject_stack.pop();
    }
    return 0;
}


Any QueryVisitor::visitBlankNodePropertyListPath(SparqlParser::BlankNodePropertyListPathContext* ctx) {
    LOG_VISITOR

    // Visit the propertyListNotEmpty
    visit(ctx->propertyListPathNotEmpty());

    return 0;
}

Any QueryVisitor::visitPropertyListPathNotEmpty(SparqlParser::PropertyListPathNotEmptyContext* ctx)
{
    LOG_VISITOR

    // 1. Visit the first predicate
    if (ctx->verbPath()) {
        visit(ctx->verbPath());
    } else {
        visit(ctx->verbSimple());
    }
    predicate_stack.push(std::move(current_sparql_element));

    // 2. Visit the first object list
    for (auto& objectPath : ctx->objectListPath()->objectPath()) {
        visit(objectPath);
    }
    predicate_stack.pop();

    // Handle the rest of the predicate and object list pairs
    for (auto& propertyListPathNotEmptyList : ctx->propertyListPathNotEmptyList()) {
        // 1. Visit the predicate
        if (propertyListPathNotEmptyList->verbPath()) {
            visit(propertyListPathNotEmptyList->verbPath());
        } else {
            visit(propertyListPathNotEmptyList->verbSimple());
        }
        predicate_stack.push(std::move(current_sparql_element));

        // 2. Visit the object list
        for (auto& object : propertyListPathNotEmptyList->objectList()->object()) {
            visit(object);
        }
        predicate_stack.pop();
    }

    return 0;
}

Any QueryVisitor::visitPropertyListNotEmpty(SparqlParser::PropertyListNotEmptyContext* ctx) {
    LOG_VISITOR

    const auto verbs = ctx->verb();
    for (size_t i = 0; i < verbs.size(); ++i) {
        // Visit the predicate
        visit(verbs[i]);

        predicate_stack.push(std::move(current_sparql_element));
        for (auto& object : ctx->objectList(i)->object()) {
            visit(object);
        }
        predicate_stack.pop();
    }
    return 0;
}


Any QueryVisitor::visitObjectPath(SparqlParser::ObjectPathContext* ctx) {
    LOG_VISITOR
    auto gnp = ctx->graphNodePath();
    if (gnp->varOrTerm()) {
        visit(gnp->varOrTerm());

        if (predicate_stack.top().is_path()) {
            auto path_var = current_path_var_is_fresh
                          ? current_path_var
                          : get_query_ctx().get_internal_var();
            current_path_var_is_fresh = false;

            current_paths.emplace_back(
                path_var,
                subject_stack.top(),
                current_sparql_element.get_ID(),
                current_path_semantic,
                predicate_stack.top().get_path()
            );
        } else {
            LOG_INFO("triples emplace back in: ObjectPathContext varOrTerm");
            current_triples.emplace_back(
                subject_stack.top(),
                predicate_stack.top().get_ID(),
                current_sparql_element.get_ID()
            );
        }
    }
    else {
        LOG_INFO("Creating blank node in:" << "ObjectPathContext");
        auto new_blank_node = get_new_blank_node_var();

        subject_stack.push(new_blank_node);
        visit(gnp->triplesNodePath());
        subject_stack.pop();

        if (predicate_stack.top().is_path()) {
            auto path_var = current_path_var_is_fresh
                          ? current_path_var
                          : get_query_ctx().get_internal_var();
            current_path_var_is_fresh = false;

            current_paths.emplace_back(
                path_var,
                subject_stack.top(),
                new_blank_node,
                current_path_semantic,
                predicate_stack.top().get_path()
            );
        } else {
            LOG_INFO("triples emplace back in: ObjectPathContext not varOrTerm");
            current_triples.emplace_back(
                subject_stack.top(),
                predicate_stack.top().get_ID(),
                new_blank_node
            );
        }
    }
    return 0;
}


Any QueryVisitor::visitObject(SparqlParser::ObjectContext* ctx) {
    LOG_VISITOR
    auto gn = ctx->graphNode();
    if (gn->varOrTerm()) {
        visit(gn->varOrTerm());

        if (predicate_stack.top().is_path()) {
            auto path_var = current_path_var_is_fresh
                          ? current_path_var
                          : get_query_ctx().get_internal_var();
            current_path_var_is_fresh = false;

            current_paths.emplace_back(
                path_var,
                subject_stack.top(),
                current_sparql_element.get_ID(),
                current_path_semantic,
                predicate_stack.top().get_path()
            );
        } else {
            LOG_INFO("triples emplace back in: ObjectContext");
            current_triples.emplace_back(
                subject_stack.top(),
                predicate_stack.top().get_ID(),
                current_sparql_element.get_ID()
            );
        }
    }
    else {
        LOG_INFO("Creating blank node in: ObjectContext");
        auto new_blank_node = get_new_blank_node_var();

        subject_stack.push(new_blank_node);
        visit(gn->triplesNode());
        subject_stack.pop();

        if (predicate_stack.top().is_path()) {
            auto path_var = current_path_var_is_fresh
                          ? current_path_var
                          : get_query_ctx().get_internal_var();
            current_path_var_is_fresh = false;

            current_paths.emplace_back(
                path_var,
                subject_stack.top(),
                new_blank_node,
                current_path_semantic,
                predicate_stack.top().get_path()
            );
        } else {
            LOG_INFO("triples emplace back in: ObjectContext");
            current_triples.emplace_back(
                subject_stack.top(),
                predicate_stack.top().get_ID(),
                new_blank_node
            );
        }
    }
    return 0;
}


Any QueryVisitor::visitCollectionPath(SparqlParser::CollectionPathContext* ctx) {
    LOG_VISITOR
    Id representative_bnode = subject_stack.top();

    // prev_bnode will be the subject of the first triple that will be created.
    Id prev_bnode = subject_stack.top();

    const auto graphNodePaths = ctx->graphNodePath();
    for (size_t i = 0; i < graphNodePaths.size(); ++i) {
        LOG_INFO("Creating blank node in: CollectionPathContext");
        subject_stack.push(get_new_blank_node_var());
        LOG_INFO("Entering CollectionPathContext node: " << i);
        auto previous_amount_of_triples = current_triples.size();
        visit(graphNodePaths[i]);
        LOG_INFO("reentered CollectionPathContext node: " << i);
        // Case 1. Created a new triple
        if (previous_amount_of_triples < current_triples.size()) {
            current_sparql_element = current_triples[previous_amount_of_triples].subject;
        }
        // Case 2. A sparql element was created, default case:
        IdOrPath rdf_node = current_sparql_element.clone();

        Id predicate = Conversions::pack_iri("http://www.w3.org/1999/02/22-rdf-syntax-ns#first");
        LOG_INFO("triples emplace back in: CollectionPathContext");
        current_triples.emplace_back(
            prev_bnode,
            predicate,
            rdf_node.get_ID()
        );

        if (i < graphNodePaths.size() - 1) {
            Id next_bnode = subject_stack.top();

            predicate = Conversions::pack_iri("http://www.w3.org/1999/02/22-rdf-syntax-ns#rest");

            LOG_INFO("triples emplace back in: CollectionPathContext");
            current_triples.emplace_back(
                prev_bnode,
                predicate,
                next_bnode
            );
            prev_bnode = std::move(next_bnode);
        } else {
            predicate = Conversions::pack_iri("http://www.w3.org/1999/02/22-rdf-syntax-ns#rest");
            LOG_INFO("triples emplace back in: CollectionPathContext");
            current_triples.emplace_back(
                prev_bnode,
                predicate,
                Conversions::pack_iri("http://www.w3.org/1999/02/22-rdf-syntax-ns#nil")
            );
        }
        subject_stack.pop();
    }
    current_sparql_element = std::move(representative_bnode);

    return 0;
}


Any QueryVisitor::visitCollection(SparqlParser::CollectionContext* ctx) {
    LOG_VISITOR
    Id representative_bnode = subject_stack.top();
    // I assume this prev_bnode is the blank node that will be added, the
    // reason for this is because in the ObjectPath visitation the blank node
    // is added into the subject_stack, not because it is a subject though.
    Id prev_bnode = subject_stack.top();
    subject_stack.push(prev_bnode);

    const auto graphNodes = ctx->graphNode();
    for (size_t i = 0; i < graphNodes.size(); ++i) {
        LOG_INFO("Creating blank node in: CollectionContext");
        subject_stack.push(get_new_blank_node_var());
        LOG_INFO("Entering CollectionContext node: " << i);
        auto previous_amount_of_triples = current_triples.size();
        visit(graphNodes[i]);
        LOG_INFO("reentered CollectionContext node: " << i);
        // Case 1. Created a new triple
        if (previous_amount_of_triples < current_triples.size()) {
            current_sparql_element = current_triples[previous_amount_of_triples].subject;
        }
        // Case 2. A sparql element was created, default case:
        Id rdf_node = current_sparql_element.get_ID();

        Id predicate = Conversions::pack_iri("http://www.w3.org/1999/02/22-rdf-syntax-ns#first");
        LOG_INFO("triples emplace back in: CollectionContext");
        current_triples.emplace_back(
            prev_bnode,
            predicate,
            rdf_node
        );

        if (i < graphNodes.size() - 1) {
            Id next_bnode = subject_stack.top();

            predicate = Conversions::pack_iri("http://www.w3.org/1999/02/22-rdf-syntax-ns#rest");

            LOG_INFO("triples emplace back in: CollectionContext");
            current_triples.emplace_back(
                prev_bnode,
                predicate,
                next_bnode
            );
            prev_bnode = std::move(next_bnode);
        } else {
            predicate = Conversions::pack_iri("http://www.w3.org/1999/02/22-rdf-syntax-ns#rest");
            LOG_INFO("triples emplace back in: CollectionContext");
            current_triples.emplace_back(
                prev_bnode,
                predicate,
                Conversions::pack_iri("http://www.w3.org/1999/02/22-rdf-syntax-ns#nil")
            );
        }
        subject_stack.pop();
    }
    subject_stack.pop();
    current_sparql_element = std::move(representative_bnode);

    return 0;
}

Any QueryVisitor::visitGroupCondition(SparqlParser::GroupConditionContext* ctx)
{
    LOG_VISITOR
    visitChildren(ctx);

    std::optional<VarId> var;
    std::unique_ptr<Expr> expr;

    if (ctx->var()) {
        auto var_name = ctx->var()->getText().substr(1);
        if (ctx->AS()) {
            var = get_query_ctx().get_or_create_var(var_name);
        } else {
            auto expr_var = get_query_ctx().get_or_create_var(var_name);
            expr = std::make_unique<ExprVar>(expr_var);
        }
    }

    if (ctx->expression()) {
        expr = std::move(current_expr);
    } else if (ctx->builtInCall()) {
        expr = std::move(current_expr);
    } else if (ctx->functionCall()) {
        expr = std::move(current_expr);
    }

    group_by_items.push_back({ std::move(expr), var });
    return 0;
}

Any QueryVisitor::visitHavingCondition(SparqlParser::HavingConditionContext *ctx) {
    LOG_VISITOR
    visitChildren(ctx);
    auto constraint = ctx->constraint();

    if (constraint->expression()) {
        having_expressions.push_back(std::move(current_expr));
    } else {
        throw NotSupportedException("Only expressions are supported in HAVING");
    }

    return 0;
}


Any QueryVisitor::visitVar(SparqlParser::VarContext* ctx) {
    LOG_VISITOR
    auto var_name = ctx->getText().substr(1);
    auto var = get_query_ctx().get_or_create_var(var_name);
    current_sparql_element = var;
    return 0;
}


Any QueryVisitor::visitIri(SparqlParser::IriContext* ctx) {
    LOG_VISITOR
    current_sparql_element = Conversions::pack_iri(iriCtxToString(ctx));
    return 0;
}


Any QueryVisitor::visitRdfLiteral(SparqlParser::RdfLiteralContext* ctx) {
    LOG_VISITOR
    std::string str = stringCtxToString(ctx->string());
    if (ctx->iri()) {
        std::string iri = iriCtxToString(ctx->iri());
        current_sparql_element = Conversions::try_pack_string_datatype(iri, str);
    }
    else if (ctx->LANGTAG()) {
        current_sparql_element = Conversions::pack_string_lang(ctx->LANGTAG()->getText().substr(1), str);
    } else {
        current_sparql_element = Conversions::pack_string_simple(str);
    }
    return 0;
}


Any QueryVisitor::visitNumericLiteralUnsigned(SparqlParser::NumericLiteralUnsignedContext* ctx) {
    LOG_VISITOR
    if (ctx->INTEGER()) {
        current_sparql_element = handleIntegerString(ctx->getText(), "http://www.w3.org/2001/XMLSchema#integer");
    } else if (ctx->DECIMAL()) {
        bool error;
        Decimal dec(ctx->getText(), &error);
        if (error) {
            throw QueryException("Invalid decimal value: " + ctx->getText());
        }
        current_sparql_element = Conversions::pack_decimal(dec);
    } else {
        // Double
         try {
            current_sparql_element = Conversions::pack_double(std::stod(ctx->getText()));
        } catch (const std::out_of_range& e) {
            current_sparql_element = Conversions::pack_string_datatype(
                "http://www.w3.org/2001/XMLSchema#double",
                ctx->getText());
        } catch (const std::invalid_argument& e) {
            current_sparql_element = Conversions::pack_string_datatype(
                "http://www.w3.org/2001/XMLSchema#double",
                ctx->getText());
        }
    }
    return 0;
}


Any QueryVisitor::visitNumericLiteralPositive(SparqlParser::NumericLiteralPositiveContext* ctx) {
    LOG_VISITOR
    if (ctx->INTEGER_POSITIVE()) {
        current_sparql_element = handleIntegerString(ctx->getText(), "http://www.w3.org/2001/XMLSchema#positiveInteger");
    } else if (ctx->DECIMAL_POSITIVE()) {
        bool error;
        Decimal dec(ctx->getText(), &error);
        if (error) {
            throw QueryException("Invalid decimal value: " + ctx->getText());
        }
        current_sparql_element = Conversions::pack_decimal(dec);
    } else {
        // Double
        try {
            current_sparql_element = Conversions::pack_double(std::stod(ctx->getText()));
        } catch (const std::out_of_range& e) {
            current_sparql_element = Conversions::pack_string_datatype(
                "http://www.w3.org/2001/XMLSchema#double",
                ctx->getText());
        } catch (const std::invalid_argument& e) {
            current_sparql_element = Conversions::pack_string_datatype(
                "http://www.w3.org/2001/XMLSchema#double",
                ctx->getText());
        }
    }
    return 0;
}


Any QueryVisitor::visitNumericLiteralNegative(SparqlParser::NumericLiteralNegativeContext* ctx) {
    LOG_VISITOR
    if (ctx->INTEGER_NEGATIVE()) {
        current_sparql_element = handleIntegerString(ctx->getText(), "http://www.w3.org/2001/XMLSchema#negativeInteger");
    } else if (ctx->DECIMAL_NEGATIVE()) {
        bool error;
        Decimal dec(ctx->getText(), &error);
        if (error) {
            throw QueryException("Invalid decimal value: " + ctx->getText());
        }
        current_sparql_element = Conversions::pack_decimal(dec);
    } else {
        // Double
        try {
            current_sparql_element = Conversions::pack_double(std::stod(ctx->getText()));
        } catch (const std::out_of_range& e) {
            current_sparql_element = Conversions::pack_string_datatype(
                "http://www.w3.org/2001/XMLSchema#double",
                ctx->getText());
        } catch (const std::invalid_argument& e) {
            current_sparql_element = Conversions::pack_string_datatype(
                "http://www.w3.org/2001/XMLSchema#double",
                ctx->getText());
        }
    }
    return 0;
}


Any QueryVisitor::visitBooleanLiteral(SparqlParser::BooleanLiteralContext* ctx) {
    LOG_VISITOR
    current_sparql_element = Conversions::pack_bool(ctx->K_TRUE() != nullptr);
    return 0;
}


Any QueryVisitor::visitBlankNode(SparqlParser::BlankNodeContext* ctx) {
    LOG_VISITOR
    if (ctx->BLANK_NODE_LABEL()) {
        auto var_name = ctx->getText();
        auto var = get_query_ctx().get_or_create_var(var_name);
        current_sparql_element = var;
    }
    else {
        LOG_INFO("Creating blank node in: BlankNodeContext");
        current_sparql_element = get_new_blank_node_var();
    }
    return 0;
}


Any QueryVisitor::visitNil(SparqlParser::NilContext*) {
    LOG_VISITOR
    current_sparql_element = Conversions::pack_iri("http://www.w3.org/1999/02/22-rdf-syntax-ns#nil");
    return 0;
}


Any QueryVisitor::visitVerbPath(SparqlParser::VerbPathContext* ctx) {
    LOG_VISITOR
    // Set current_path
    current_path_inverse = false;
    visit(ctx->path());
    // default semantic, may be overridden
    current_path_semantic = PathSemantic::DEFAULT;

    // MillenniumDB's path extension
    if (ctx->AS()) {
        // Path semantic
        if (ctx->ALL()) {
            if (ctx->SHORTEST()) {
                if (ctx->SIMPLE()) {
                    current_path_semantic = PathSemantic::ALL_SHORTEST_SIMPLE;
                } else if (ctx->ACYCLIC()) {
                    current_path_semantic = PathSemantic::ALL_SHORTEST_ACYCLIC;
                } else {
                    current_path_semantic = PathSemantic::ALL_SHORTEST_WALKS;
                }
            } else {
                if (ctx->SIMPLE()) {
                    current_path_semantic = PathSemantic::ALL_SIMPLE;
                } else if (ctx->ACYCLIC()) {
                    current_path_semantic = PathSemantic::ALL_ACYCLIC;
                } else {
                    throw QueryException("ALL WALKS path semantic not allowed");
                }
            }
        }
        else { //if (ctx->ANY())
            if (ctx->SHORTEST()) {
                if (ctx->SIMPLE()) {
                    current_path_semantic = PathSemantic::ANY_SHORTEST_SIMPLE;
                } else if (ctx->ACYCLIC()) {
                    current_path_semantic = PathSemantic::ANY_SHORTEST_ACYCLIC;
                } else {
                    current_path_semantic = PathSemantic::ANY_SHORTEST_WALKS;
                }
            } else {
                if (ctx->SIMPLE()) {
                    current_path_semantic = PathSemantic::ANY_SIMPLE;
                } else if (ctx->ACYCLIC()) {
                    current_path_semantic = PathSemantic::ANY_ACYCLIC;
                } else {
                    current_path_semantic = PathSemantic::ANY_WALKS;
                }
            }
        }
        current_path_var = get_query_ctx().get_or_create_var(ctx->var()->getText().substr(1));
    }
    // Default SPARQL path
    else {
        auto casted_atom = dynamic_cast<PathAtom*>(current_path.get());
        // if current path is an atom we have a triple instead of a path
        if (casted_atom && !casted_atom->inverse) {
            current_sparql_element = Conversions::pack_iri(casted_atom->atom);
            return 0;
        } else {
            current_path_var = get_query_ctx().get_internal_var();
        }
    }
    current_path_var_is_fresh = true;
    current_sparql_element = std::move(current_path);
    return 0;
}


Any QueryVisitor::visitPathAlternative(SparqlParser::PathAlternativeContext* ctx) {
    LOG_VISITOR

    const auto pathSequences = ctx->pathSequence();

    if (pathSequences.size() > 1) {
        std::vector<std::unique_ptr<RegularPathExpr>> alternatives;
        for (auto& ps_item : pathSequences) {
            visit(ps_item);
            alternatives.push_back(std::move(current_path));
        }
        current_path = std::make_unique<PathAlternatives>(std::move(alternatives));
    } else {
        visit(pathSequences[0]);
    }
    return 0;
}


Any QueryVisitor::visitPathSequence(SparqlParser::PathSequenceContext* ctx) {
    LOG_VISITOR

    const auto pathEltOrInverses = ctx->pathEltOrInverse();

    if (pathEltOrInverses.size() > 1) {
        std::vector<std::unique_ptr<RegularPathExpr>> sequence;
        if (current_path_inverse) {
            for (int i = pathEltOrInverses.size() - 1; i >= 0; i--) {
                visit(pathEltOrInverses[i]);
                sequence.push_back(std::move(current_path));
            }
        } else {
            for (auto& pe_item : pathEltOrInverses) {
                visit(pe_item);
                sequence.push_back(std::move(current_path));
            }
        }
        current_path = std::make_unique<PathSequence>(std::move(sequence));
    } else {
        visit(pathEltOrInverses[0]);
    }
    return 0;
}


Any QueryVisitor::visitPathEltOrInverse(SparqlParser::PathEltOrInverseContext* ctx) {
    LOG_VISITOR
    auto pe = ctx->pathElt();
    auto pp = pe->pathPrimary();
    auto mod = pe->pathMod();

    bool previous_current_path_inverse = current_path_inverse;
    current_path_inverse = (ctx->INVERSE() != nullptr) ^ current_path_inverse;
    if (pp->path()) {
        visit(pp->path()->pathAlternative());
    } else if (pp->iri()) {
        std::string iri = iriCtxToString(pp->iri());
        current_path = std::make_unique<PathAtom>(std::move(iri), current_path_inverse);
    } else if (pp->A()) {
        current_path = std::make_unique<PathAtom>("http://www.w3.org/1999/02/22-rdf-syntax-ns#type",
                                                  current_path_inverse);
    } else {
        std::vector<PathAtom> negated_set;
        for (auto& path_one : pp->pathNegatedPropertySet()->pathOneInPropertySet()) {
            std::string iri;
            if (path_one->A()) {
                iri = "http://www.w3.org/1999/02/22-rdf-syntax-ns#type";
            } else {
                iri = iriCtxToString(path_one->iri());
            }
            negated_set.push_back(PathAtom(std::move(iri), path_one->INVERSE() != nullptr));
        }
        current_path = std::make_unique<PathNegatedSet>(std::move(negated_set));
        throw NotSupportedException("PathNegatedSet");
    }

    current_path_inverse = previous_current_path_inverse;

    if (mod) {
        if (mod->pathQuantity()) {
            std::vector<std::unique_ptr<RegularPathExpr>> seq;
            if (mod->pathQuantity()->pathQuantityExact()) {
                unsigned exact = std::stoul(mod->pathQuantity()->pathQuantityExact()->INTEGER()->getText());
                for (unsigned i = 0; i < exact; i++) {
                    seq.push_back(current_path->clone());
                }
            }
            else if (mod->pathQuantity()->pathQuantityRange()) {
                unsigned min = std::stoul(mod->pathQuantity()->pathQuantityRange()->min->getText());
                unsigned max = std::stoul(mod->pathQuantity()->pathQuantityRange()->max->getText());
                unsigned i = 0;
                for (; i < min; i++) {
                    seq.push_back(current_path->clone());
                }
                for (; i < max; i++) {
                    seq.push_back(std::make_unique<PathOptional>(current_path->clone()));
                }
            }
            else if (mod->pathQuantity()->pathQuantityMax()) {
                unsigned max = std::stoul(mod->pathQuantity()->pathQuantityMax()->max->getText());
                for (unsigned i = 0; i < max; i++) {
                    seq.push_back(std::make_unique<PathOptional>(current_path->clone()));
                }
            }
            else if (mod->pathQuantity()->pathQuantityMin()) {
                unsigned min = std::stoul(mod->pathQuantity()->pathQuantityMin()->min->getText());
                for (unsigned i = 0; i < min; i++) {
                    seq.push_back(current_path->clone());
                }
                seq.push_back(std::make_unique<PathKleeneStar>(current_path->clone()));
            }
            current_path = std::make_unique<PathSequence>(std::move(seq));
        } else {
            switch(mod->getText()[0]) {
            case '*':
                current_path = std::make_unique<PathKleeneStar>(std::move(current_path));
                break;
            case '?':
                if (!current_path->nullable()) {
                    current_path = std::make_unique<PathOptional>(std::move(current_path));
                }
                // else we avoid a redundant PathOptional, current_path stays the same
                break;
            case '+':
                current_path = std::make_unique<PathKleenePlus>(std::move(current_path));
                break;
            }
        }
    }
    return 0;
}


Any QueryVisitor::visitVerb(SparqlParser::VerbContext* ctx) {
    LOG_VISITOR
    if (ctx->A()) {
        current_sparql_element = Conversions::pack_iri("http://www.w3.org/1999/02/22-rdf-syntax-ns#type");
    } else {
        visit(ctx->varOrIRI());
    }
    return 0;
}


std::string QueryVisitor::iriCtxToString(SparqlParser::IriContext* ctx) {
    std::string iri;
    if (ctx->IRIREF()) {
        iri = ctx->IRIREF()->getText();
        iri = iri.substr(1, iri.size() - 2);
    }
    else {
        std::string prefixedName = ctx->prefixedName()->getText();
        auto pos = prefixedName.find(':');
        auto prefix = prefixedName.substr(0, pos);
        auto suffix = prefixedName.substr(pos + 1);

        if (global_info.iri_prefix_map.find(prefix) == global_info.iri_prefix_map.end()) {
            throw QuerySemanticException("The prefix '" + prefix + "' is not defined");
        }
        iri = global_info.iri_prefix_map[prefix] + suffix;
    }
    // Check if the IRI is absolute or not
    // If it is not absolute, it needs to be expanded with the base IRI
    auto pos = iri.find(':');
    if (pos == std::string::npos) {
        if (global_info.base_iri.empty()) {
            throw QuerySemanticException("The IRI '" + iri + "' is not absolute and the base IRI is not defined");
        }
        iri = global_info.base_iri + iri;
    }
    return iri;
}


std::string QueryVisitor::stringCtxToString(SparqlParser::StringContext* ctx) {
    std::string str = ctx->getText();
    if (ctx->STRING_LITERAL1() || ctx->STRING_LITERAL2()) {
        // One quote per side
        str = str.substr(1, str.size() - 2);
    } else {
        // Three quotes per side
        str = str.substr(3, str.size() - 6);
    }
    return UnicodeEscape::normalize_string(str);
}

/**
 * @brief Handle a string representing an integer, setting its ObjectId
 *
 * @param str the string representing the integer
 */
ObjectId QueryVisitor::handleIntegerString(const std::string& str, const std::string& iri) {
    try {
        size_t pos;
        int64_t n = std::stoll(str, &pos);
        // Check if the whole string was parsed
        if (pos != str.size())
            return Conversions::pack_string_datatype(iri, str);
        return Conversions::pack_int(n);
    } catch (std::out_of_range& e) {
        // The integer is too big, we use a Decimal
        bool error;
        Decimal dec(str, &error);
        if (error) {
            return Conversions::pack_string_datatype(iri, str);
        }
        return Conversions::pack_decimal(dec);
    } catch (std::invalid_argument& e) {
        // The string is not a valid integer
        return Conversions::pack_string_datatype(iri, str);
    }
}

void QueryVisitor::handle_function_call(
    SparqlQueryParser::IriContext* iriCtx,
    SparqlQueryParser::ArgListContext* argListCtx
)
{
    assert(argListCtx != nullptr && "Expected a function call");

    if (argListCtx->DISTINCT() != nullptr) {
        // Only custom aggregate functions use the DISTINCT keyword in a function call. We have not implemented custom
        // aggregate functions yet.
        throw NotSupportedException("DISTINCT inside function call argument list is not supported yet");
    }

    const auto iri = iriCtxToString(iriCtx);

    // XSD Cast
    const std::string_view XML_SCHEMA = "http://www.w3.org/2001/XMLSchema#";
    if (iri.size() > XML_SCHEMA.size() && iri.rfind(XML_SCHEMA, 0) == 0) {
        const auto expressionList = argListCtx->expressionList();
        if (expressionList == nullptr) {
            throw QuerySemanticException("Cast functions must have one argument");
        }
        const auto expressions = expressionList->expression();
        if (expressions.size() != 1) {
            throw QuerySemanticException("Cast functions must have one argument");
        }
        visit(expressions[0]);
        const auto xsd_suffix = iri.substr(XML_SCHEMA.size());
        // xsd:boolean
        if (xsd_suffix == "boolean") {
            current_expr = std::make_unique<ExprCast>(CastType::xsd_boolean, std::move(current_expr));
        }
        // xsd:double
        else if (xsd_suffix == "double")
        {
            current_expr = std::make_unique<ExprCast>(CastType::xsd_double, std::move(current_expr));
        }
        // xsd:float
        else if (xsd_suffix == "float")
        {
            current_expr = std::make_unique<ExprCast>(CastType::xsd_float, std::move(current_expr));
        }
        // xsd:decimal
        else if (xsd_suffix == "decimal")
        {
            current_expr = std::make_unique<ExprCast>(CastType::xsd_decimal, std::move(current_expr));
        }
        // xsd:integer
        else if (xsd_suffix == "integer")
        {
            current_expr = std::make_unique<ExprCast>(CastType::xsd_integer, std::move(current_expr));
        }
        // xsd:dateTime
        else if (xsd_suffix == "dateTime")
        {
            current_expr = std::make_unique<ExprCast>(CastType::xsd_dateTime, std::move(current_expr));
        }
        // xsd:string
        else if (xsd_suffix == "string")
        {
            current_expr = std::make_unique<ExprCast>(CastType::xsd_string, std::move(current_expr));
        } else {
            throw NotSupportedException("Unsupported xsd cast function: " + iri);
        }
        return;
    }

    // MDB Cast
    namespace MDBType = MDBExtensions::Type;
    if (iri.size() > MDBType::TYPE_PREFIX_IRI.size() && iri.rfind(MDBType::TYPE_PREFIX_IRI, 0) == 0) {
        const auto expressionList = argListCtx->expressionList();
        if (expressionList == nullptr) {
            throw QuerySemanticException("Cast functions must have one argument");
        }
        const auto expressions = expressionList->expression();
        if (expressions.size() != 1) {
            throw QuerySemanticException("Cast functions must have one argument");
        }
        visit(expressions[0]);

        const auto mdbtype_suffix = iri.substr(MDBType::TYPE_PREFIX_IRI.size());
        // mdbtype:tensorFloat
        if (mdbtype_suffix == MDBType::TENSOR_FLOAT_SUFFIX_IRI) {
            current_expr = std::make_unique<ExprCast>(CastType::mdbtype_tensorFloat, std::move(current_expr));
        }
        // mdbtype:tensorDouble
        else if (mdbtype_suffix == MDBType::TENSOR_DOUBLE_SUFFIX_IRI)
        {
            current_expr = std::make_unique<ExprCast>(
                CastType::mdbtype_tensorDouble,
                std::move(current_expr)
            );
        } else {
            throw NotSupportedException("Unsupported mdbtype cast function: " + iri);
        }
        return;
    }

    // MDB Function
    namespace MDBFn = MDBExtensions::Function;
    if (iri.size() > MDBFn::FUNCTION_PREFIX_IRI.size() && iri.rfind(MDBFn::FUNCTION_PREFIX_IRI, 0) == 0) {
        const auto mdbfn_suffix = iri.substr(MDBFn::FUNCTION_PREFIX_IRI.size());
        const auto expressionList = argListCtx->expressionList();
        if (expressionList == nullptr) {
            // NOTE: Here non-argument functions could be handled
            throw QuerySemanticException("Cast functions must have one argument");
        }
        const auto expressions = expressionList->expression();

        // mdbfn:sqrt
        if (mdbfn_suffix == MDBFn::SQRT_SUFFIX_IRI) {
            if (expressions.size() != 1) {
                throw QuerySemanticException(iri + " function expects 1 argument");
            }
            visit(expressions[0]);
            current_expr = std::make_unique<ExprSqrt>(std::move(current_expr));
        } else if (mdbfn_suffix == MDBFn::POW_SUFFIX_IRI) {
            if (expressions.size() != 2) {
                throw QuerySemanticException(iri + " function expects 2 arguments");
            }
            visit(expressions[0]);
            auto lhs = std::move(current_expr);
            visit(expressions[1]);
            current_expr = std::make_unique<ExprPow>(std::move(lhs), std::move(current_expr));
        } else if (mdbfn_suffix == MDBFn::DOT_SUFFIX_IRI) {
            if (expressions.size() != 2) {
                throw QuerySemanticException(iri + " function expects 2 arguments");
            }
            visit(expressions[0]);
            auto lhs = std::move(current_expr);
            visit(expressions[1]);
            current_expr = std::make_unique<ExprDot>(std::move(lhs), std::move(current_expr));
        } else if (mdbfn_suffix == MDBFn::SUM_SUFFIX_IRI) {
            if (expressions.size() != 1) {
                throw QuerySemanticException(iri + " function expects 1 argument");
            }
            visit(expressions[0]);
            current_expr = std::make_unique<ExprSum>(std::move(current_expr));
        } else if (mdbfn_suffix == MDBFn::COSINE_SIMILARITY_SUFFIX_IRI) {
            if (expressions.size() != 2) {
                throw QuerySemanticException(iri + " function expects 2 arguments");
            }
            visit(expressions[0]);
            auto lhs = std::move(current_expr);
            visit(expressions[1]);
            current_expr = std::make_unique<ExprCosineSimilarity>(std::move(lhs), std::move(current_expr));
        } else if (mdbfn_suffix == MDBFn::EUCLIDEAN_DISTANCE_SUFFIX_IRI) {
            if (expressions.size() != 2) {
                throw QuerySemanticException(iri + " function expects 2 arguments");
            }
            visit(expressions[0]);
            auto lhs = std::move(current_expr);
            visit(expressions[1]);
            current_expr = std::make_unique<ExprEuclideanDistance>(std::move(lhs), std::move(current_expr));
        } else if (mdbfn_suffix == MDBFn::COSINE_DISTANCE_SUFFIX_IRI){
            if (expressions.size() != 2) {
                throw QuerySemanticException(iri + " function expects 2 arguments");
            }
            visit(expressions[0]);
            auto lhs = std::move(current_expr);
            visit(expressions[1]);
            current_expr = std::make_unique<ExprCosineDistance>(std::move(lhs), std::move(current_expr));
        } else if (mdbfn_suffix == MDBFn::MANHATTAN_DISTANCE_SUFFIX_IRI) {
            if (expressions.size() != 2) {
                throw QuerySemanticException(iri + " function expects 2 arguments");
            }
            visit(expressions[0]);
            auto lhs = std::move(current_expr);
            visit(expressions[1]);
            current_expr = std::make_unique<ExprManhattanDistance>(std::move(lhs), std::move(current_expr));
        } else {
            throw NotSupportedException("Unsupported mdbfn function: " + iri);
        }
        return;
    }

    throw NotSupportedException("Unsupported function call: " + iri);
}

#include "executor_constructor.h"

#include <memory>

#include "graph_models/rdf_model/conversions.h"
#include "query/exceptions.h"
#include "query/executor/query_executor/sparql/show_executor.h"
#include "system/path_manager.h"
#include "query/executor/query_executor/sparql/ask_executor.h"
#include "query/executor/query_executor/sparql/construct_executor.h"
#include "query/executor/query_executor/sparql/csv_select_executor.h"
#include "query/executor/query_executor/sparql/describe_executor.h"
#include "query/executor/query_executor/sparql/json_select_executor.h"
#include "query/executor/query_executor/sparql/tsv_select_executor.h"
#include "query/executor/query_executor/sparql/xml_select_executor.h"
#include "query/optimizer/rdf_model/binding_iter_constructor.h"
#include "query/parser/op/op_visitor.h"
#include "query/parser/op/sparql/op_select.h"

using namespace SPARQL;

void ExecutorConstructor::visit(OpDescribe& op_describe) {
    response_type = SPARQL::ResponseType::TURTLE;
    // if (response_type != SPARQL::ResponseType::TURTLE) {
    //     throw NotSupportedException("DESCRIBE response type: " + SPARQL::response_type_to_string(response_type));
    // }

    std::unique_ptr<BindingIter> child_iter;

    if (op_describe.op) {
        BindingIterConstructor visitor;
        op_describe.accept_visitor(visitor);
        child_iter = std::move(visitor.tmp);
        path_manager.begin(std::move(visitor.begin_at_left));
    }

    std::vector<ObjectId> iris;
    iris.reserve(op_describe.iris.size());

    for (auto & iri : op_describe.iris) {
        auto oid = SPARQL::Conversions::pack_iri(iri);
        iris.push_back(oid);
    }

    executor = std::make_unique<DescribeExecutor>(
        std::move(child_iter),
        std::move(op_describe.vars),
        std::move(iris)
    );
}


void ExecutorConstructor::visit(OpConstruct& op_construct) {
    response_type = SPARQL::ResponseType::TURTLE;
    // if (response_type != SPARQL::ResponseType::TURTLE) {
    //     throw NotSupportedException("CONSTRUCT response type: " + SPARQL::response_type_to_string(response_type));
    // }

    BindingIterConstructor visitor;
    op_construct.accept_visitor(visitor);

    std::vector<Triple> triples;
    for (auto& op_triple : op_construct.triples) {
        triples.push_back(Triple(op_triple.subject, op_triple.predicate, op_triple.object));
    }

    path_manager.begin(std::move(visitor.begin_at_left));
    executor = std::make_unique<ConstructExecutor>(
        std::move(visitor.tmp),
        std::move(triples)
    );
}


void ExecutorConstructor::visit(OpSelect& op_select) {
    BindingIterConstructor visitor;
    op_select.accept_visitor(visitor);
    std::unique_ptr<BindingIter> iter_root = std::move(visitor.tmp);

    path_manager.begin(std::move(visitor.begin_at_left));
    auto projection_vars = op_select.vars;

    switch (response_type) {
    case SPARQL::ResponseType::CSV:
        executor = std::make_unique<CSVSelectExecutor>(
            std::move(iter_root),
            std::move(projection_vars)
        );
        break;
    case SPARQL::ResponseType::TSV:
        executor = std::make_unique<TSVSelectExecutor>(
            std::move(iter_root),
            std::move(projection_vars)
        );
        break;
    case SPARQL::ResponseType::JSON:
        executor = std::make_unique<JsonSelectExecutor>(
            std::move(iter_root),
            std::move(projection_vars)
        );
        break;
    case SPARQL::ResponseType::XML:
        executor = std::make_unique<XMLSelectExecutor>(
            std::move(iter_root),
            std::move(projection_vars));
        break;
    default: throw NotSupportedException("SELECT response type: " + SPARQL::response_type_to_string(response_type));
    }
}


void ExecutorConstructor::visit(OpAsk& op_ask) {
    BindingIterConstructor visitor;

    op_ask.op->accept_visitor(visitor);

    std::unique_ptr<BindingIter> binding_iter = std::move(visitor.tmp);

    path_manager.begin(std::move(visitor.begin_at_left));

    executor = std::make_unique<AskExecutor>(
        std::move(binding_iter),
        response_type
    );
}

void ExecutorConstructor::visit(OpShow& op_show)
{
    switch (response_type) {
    case SPARQL::ResponseType::CSV:
        if (op_show.type == OpShow::Type::TEXT_SEARCH_INDEX) {
            executor = std::make_unique<
                SPARQL::ShowExecutor<SPARQL::ResponseType::CSV, OpShow::Type::TEXT_SEARCH_INDEX>>();
            break;
        }

    case SPARQL::ResponseType::TSV:
        if (op_show.type == OpShow::Type::TEXT_SEARCH_INDEX) {
            executor = std::make_unique<
                SPARQL::ShowExecutor<SPARQL::ResponseType::TSV, OpShow::Type::TEXT_SEARCH_INDEX>>();
            break;
        }
    default:
        throw std::runtime_error("Unhandled SHOW");
    }
}
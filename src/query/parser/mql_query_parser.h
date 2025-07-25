#pragma once

#include "misc/logger.h"
#include "query/parser/grammar/error_listener.h"
#include "query/parser/grammar/mql/autogenerated/MQL_Lexer.h"
#include "query/parser/grammar/mql/autogenerated/MQL_Parser.h"
#include "query/parser/grammar/mql/query_visitor.h"
#include "query/parser/op/mql/op.h"
#include "query/rewriter/mql/op/check_var_names.h"

namespace MQL {

class QueryParser {
public:
    static std::unique_ptr<Op> get_query_plan(const std::string& query)
    {
        antlr4::ANTLRInputStream input(query);
        MQL_Lexer lexer(&input);
        antlr4::CommonTokenStream tokens(&lexer);
        MQL_Parser parser(&tokens);

        parser.getInterpreter<antlr4::atn::ParserATNSimulator>()->setPredictionMode(
            antlr4::atn::PredictionMode::SLL
        );

        antlr4::MyErrorListener error_listener;
        parser.removeErrorListeners();
        parser.addErrorListener(&error_listener);

        MQL_Parser::RootContext* tree = parser.root();
        QueryVisitor visitor;
        visitor.visitRoot(tree);

        auto res = std::move(visitor.current_op);

        logger(Category::LogicalPlan) << "Initial logical plan:\n" << *res;

        CheckVarNames check_var_names;
        res->accept_visitor(check_var_names);

        // TODO:
        // CheckWellDesigned check_well_designed;
        // res->accept_visitor(check_well_designed);

        // TODO:
        // OptimizeOptionalTree optimize_optional_tree;
        // res->accept_visitor(optimize_optional_tree);

        // TODO: push negation inside, simplify constant expressions
        // PushWhere push_where;
        // res->accept_visitor(push_where);

        logger(Category::LogicalPlan) << *res;

        return res;
    }
};
} // namespace MQL

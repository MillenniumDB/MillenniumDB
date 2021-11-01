#ifndef BASE__QUERY_PARSER_H_
#define BASE__QUERY_PARSER_H_

#include "base/parser/grammar/manual_plan/manual_plan_ast.h"
#include "base/parser/grammar/query/query_ast.h"
#include "base/parser/logical_plan/op/op_select.h"
#include "base/parser/logical_plan/op/visitors/op_visitor.h"

class QueryParser {
public:
    static std::unique_ptr<OpSelect>    get_query_plan(std::string& query);
    static manual_plan::ast::ManualRoot get_manual_plan(std::string& query);

private:
    static std::unique_ptr<OpSelect> get_query_plan(query::ast::Root& ast);
    static void check_query_plan(OpSelect& op_select);
};

#endif // BASE__QUERY_PARSER_H_

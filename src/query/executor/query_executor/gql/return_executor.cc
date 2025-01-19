#include "return_executor.h"
#include "graph_models/gql/conversions.h"
#include "graph_models/gql/gql_model.h"
#include "query/executor/query_executor/csv_ostream_escape.h"
#include "query/executor/query_executor/tsv_ostream_escape.h"

using namespace GQL;

uint64_t ReturnExecutor::execute(std::ostream& os)
{
    auto it = projection_vars.cbegin();
    if (it != projection_vars.cend()) {
        std::string var_name = get_query_ctx().get_var_name(*it);
        if (var_name[0] == '.') {
            var_name.erase(0, 1);
        }
        os << var_name;
        ++it;
    }
    while (it != projection_vars.cend()) {
        if (ret == ReturnType::CSV) {
            os << ',';
        } else if (ret == ReturnType::TSV) {
            os << '\t';
        }
        std::string var_name = get_query_ctx().get_var_name(*it);
        if (var_name[0] == '.') {
            var_name.erase(0, 1);
        }
        os << var_name;
        ++it;
    }
    os << "\n";

    uint64_t result_count = 0;
    Binding binding(get_query_ctx().get_var_size());
    for (auto&& [var, value] : set_vars) {
        binding.add(var, value);
    }

    iter->begin(binding);

    std::unique_ptr<std::ostream> escaped_os;
    std::unique_ptr<std::stringbuf> stream_escaper;
    if (ret == ReturnType::CSV) {
        stream_escaper = std::make_unique<CSVOstreamEscape>(os);
    } else {
        stream_escaper = std::make_unique<TSVOstreamEscape>(os);
    }
    escaped_os = std::make_unique<std::ostream>(stream_escaper.get());

    while (iter->next()) {
        result_count++;
        auto it = projection_vars.cbegin();

        if (it != projection_vars.cend()) {
            auto value = binding[*it];
            print(os, *escaped_os, value);
            ++it;
        }
        while (it != projection_vars.cend()) {
            auto value = binding[*it];
            if (ret == ReturnType::CSV) {
                os << ',';
            } else {
                os << '\t';
            }
            print(os, *escaped_os, value);
            ++it;
        }
        os << "\n";
    }

    return result_count;
}

void ReturnExecutor::print(std::ostream& os, std::ostream& /* escaped_os */, ObjectId oid)
{
    Conversions::debug_print(os, oid);
}

void ReturnExecutor::analyze(std::ostream& os, bool print_stats, int indent) const
{
    BindingIterPrinter printer(os, print_stats, indent);
    iter->accept_visitor(printer);
}

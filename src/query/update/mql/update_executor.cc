#include "update_executor.h"

using namespace MQL;

uint64_t UpdateExecutor::execute()
{
    Binding binding(get_query_ctx().get_var_size());

    iter->begin(binding);

    while (iter->next()) {
        for (auto& action : update_actions) {
            action->process(binding, *update_context);
        }
    }
    // TODO: update catalog

    return 0;
}

void UpdateExecutor::analyze(std::ostream& os, bool print_stats, int indent) const
{
    // TODO:
}

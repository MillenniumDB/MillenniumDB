#include "query_context.h"

std::ostream& (*QueryContext::_debug_print)(std::ostream&, ObjectId);

thread_local QueryContext* QueryContext::_query_ctx = nullptr;
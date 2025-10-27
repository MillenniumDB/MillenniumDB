#include "id.h"

#include "query/query_context.h"

std::ostream& operator<<(std::ostream& os, ObjectId oid)
{
    return get_query_ctx()._debug_print(os, oid);
}

std::ostream& operator<<(std::ostream& os, VarId var)
{
    return os << '?' << get_query_ctx().get_var_name(var);
}

std::ostream& operator<<(std::ostream& os, Id id)
{
    if (id.is_OID()) {
        get_query_ctx()._debug_print(os, id.get_OID());
    } else {
        os << '?' << get_query_ctx().get_var_name(id.get_var());
    }
    return os;
}

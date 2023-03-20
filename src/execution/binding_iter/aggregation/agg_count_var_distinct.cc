#include "agg_count_var_distinct.h"

void AggCountVarDistinct::process() {
    tuple[0] = (*binding_iter)[var_id];
    if (tuple[0] != GraphObjectFactory::make_null()) {
        if (!extendable_table->is_in_or_insert(tuple)) {
            count++;
        }
    }
}

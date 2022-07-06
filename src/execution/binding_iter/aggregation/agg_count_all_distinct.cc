#include "agg_count_all_distinct.h"

void AggCountAllDistinct::process() {
    for (uint_fast32_t i = 0; i < var_ids.size(); i++) {
        tuple[i] = (*binding_iter)[var_ids[i]];
    }
    if (tuple[0] != GraphObject::make_null() ) {
        if (!extendable_table->is_in_or_insert(tuple)) {
            count++;
        }
    }
}

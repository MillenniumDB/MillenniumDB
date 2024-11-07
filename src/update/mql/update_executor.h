#pragma once

#include <map>

#include "query/parser/op/op.h"

namespace MQL {

class UpdateExecutor : public OpVisitor {
public:
    ~UpdateExecutor();

    void visit(OpInsert&) override;

    void execute(Op& op);
    void print_stats(std::ostream& os);

private:
    std::map<ObjectId, ObjectId> created_ids;

    // returns true if oid was transformed
    bool transform_if_tmp(ObjectId& oid);

    uint64_t stat_new_labels = 0;
    uint64_t stat_new_properties = 0;
    uint64_t stat_new_edges = 0;
};
} // namespace MQL

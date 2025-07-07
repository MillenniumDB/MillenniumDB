#pragma once

#include "query/executor/binding_iter.h"
#include "query/executor/query_executor/query_executor.h"

namespace GQL {

enum ReturnType {
    CSV,
    TSV
};

class ReturnExecutor : public QueryExecutor {
public:
    ReturnExecutor(
        std::unique_ptr<BindingIter> iter,
        std::map<VarId, ObjectId>&& set_vars,
        std::vector<VarId>&& projection_vars,
        ReturnType return_type
    ) :
        iter(std::move(iter)),
        set_vars(std::move(set_vars)),
        projection_vars(std::move(projection_vars)),
        ret(return_type)
    { }

    ReturnExecutor(
        std::unique_ptr<BindingIter> iter,
        std::vector<VarId>&& projection_vars,
        ReturnType return_type
    ) :
        iter(std::move(iter)),
        projection_vars(std::move(projection_vars)),
        ret(return_type)
    { }

    // returns how many results were obtained
    uint64_t execute(std::ostream& os) override;

    void analyze(std::ostream& os, bool print_stats = false, int indent = 0) const override;

    static void print(std::ostream& os, std::ostream& escaped_os, ObjectId oid);

private:
    std::unique_ptr<BindingIter> iter;

    std::map<VarId, ObjectId> set_vars;

    std::vector<VarId> projection_vars;

    ReturnType ret = CSV;
};
} // namespace GQL

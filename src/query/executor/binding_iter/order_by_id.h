#pragma once

#include <map>
#include <memory>
#include <set>
#include <vector>

#include "query/executor/binding_iter.h"
#include "graph_models/object_id.h"
#include "storage/file_id.h"
#include "storage/tuple_collection/tuple_id_collection.h"

class OrderById : public BindingIter {
public:
    OrderById(
        std::unique_ptr<BindingIter> child_iter,
        const std::set<VarId>&         saved_vars,
        std::vector<VarId>&&           order_vars,
        std::vector<bool>&&            ascending,
        int64_t(*_compare)(ObjectId, ObjectId));

    ~OrderById();

    void begin(Binding&) override;

    void reset() override;

    bool next() override;

    void assign_nulls() override;

    void analyze(std::ostream&, int indent = 0) const override;

private:
    std::unique_ptr<BindingIter> child_iter;

    std::map<VarId, uint_fast32_t> saved_vars;

    const std::vector<VarId> order_vars;
    const std::vector<bool> ascending;

    const TmpFileId first_file_id;
    const TmpFileId second_file_id;

    std::unique_ptr<TupleIdCollection> run;
    const TmpFileId* output_file_id;
    uint_fast32_t total_pages = 0;
    uint_fast32_t current_page = 0;
    uint64_t page_position = 0;

    const std::vector<ObjectId> saved_objects;
    Binding* parent_binding;

    int64_t(*compare)(ObjectId, ObjectId);

    std::unique_ptr<TupleIdCollection> get_run(Page& run_page);

    void merge_sort(const std::vector<VarId>& order_var_ids);
};

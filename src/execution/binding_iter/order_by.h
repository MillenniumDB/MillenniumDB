#pragma once

#include <map>
#include <memory>
#include <set>
#include <vector>

#include "base/binding/binding_id.h"
#include "base/binding/binding_iter.h"
#include "base/thread/thread_info.h"
#include "storage/file_id.h"
#include "storage/tuple_collection/tuple_collection.h"

class OrderBy : public BindingIter {
public:
    OrderBy(ThreadInfo*                  thread_info,
            std::unique_ptr<BindingIter> child,
            const std::set<VarId>&       saved_vars,
            std::vector<VarId>           order_vars,
            std::vector<bool>            ascending);
    ~OrderBy();

    void begin(std::ostream&) override;

    bool next() override;

    GraphObject operator[](VarId var_id) const override;

    void analyze(std::ostream&, int indent = 0) const override;

private:
    ThreadInfo* thread_info;
    std::unique_ptr<BindingIter> child;

    std::map<VarId, uint_fast32_t> saved_vars;

    std::vector<VarId> order_vars;
    std::vector<bool> ascending;

    TmpFileId first_file_id;
    TmpFileId second_file_id;

    std::unique_ptr<TupleCollection> run;
    TmpFileId* output_file_id;
    uint_fast32_t total_pages = 0;
    uint_fast32_t current_page = 0;
    uint64_t page_position = 0;

    std::vector<GraphObject> saved_objects;

    std::unique_ptr<TupleCollection> get_run(Page& run_page);
    void merge_sort(const std::vector<VarId>& order_var_ids);
};

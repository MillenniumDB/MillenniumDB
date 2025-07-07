#pragma once

#include "misc/min_max_heap.h"
#include "query/executor/binding_iter.h"
#include "query/var_id.h"
#include "storage/index/text_search/text_index.h"
#include "storage/index/text_search/text_index_manager.h"

namespace Procedure {

// Gives the results sorted by score by querying all text indexes (if index_name is "*")
// Otherwise, query a single text index
class TextSearchMultiScan : public BindingIter {
public:
    using UnpackStringFuncType = std::string(ObjectId);

    struct HeapItem {
        double score;
        std::size_t pos;

        HeapItem() = default;

        explicit HeapItem(double score_, std::size_t pos_) :
            score { score_ },
            pos { std::move(pos_) }
        { }
    };

    struct HeapItemLess {
        bool operator()(const HeapItem& lhs, const HeapItem& rhs)
        {
            return lhs.score < rhs.score;
        }
    };

    TextSearchMultiScan(
        std::vector<std::unique_ptr<BindingExpr>>&& argument_binding_exprs_,
        std::vector<VarId>&& yield_vars_,
        TextSearch::TextIndexManager& text_index_manager_,
        UnpackStringFuncType* unpack_string_func_ptr_
    ) :
        argument_binding_exprs { std::move(argument_binding_exprs_) },
        yield_vars { std::move(yield_vars_) },
        text_index_manager { text_index_manager_ },
        unpack_string_func_ptr { unpack_string_func_ptr_ }
    {
        assert(argument_binding_exprs.size() == 3);
        assert(yield_vars.size() == 2);
    }

    void print(std::ostream& os, int indent, bool stats) const override;

    void _begin(Binding& parent_binding) override;
    bool _next() override;
    void _reset() override;
    void assign_nulls() override;

private:
    const std::vector<std::unique_ptr<BindingExpr>> argument_binding_exprs;
    const std::vector<VarId> yield_vars;
    TextSearch::TextIndexManager& text_index_manager;
    UnpackStringFuncType* unpack_string_func_ptr;

    Binding* parent_binding;

    // evaluated arguments
    std::string index_name;
    std::string query;
    TextSearch::SearchType search_type;

    // whether the argument evaluation is valid for this procedure
    bool valid_arguments;

    // changes on each begin/reset call
    std::vector<std::string> text_index_names;
    std::vector<std::unique_ptr<TextSearch::TextSearchIter>> text_search_iters;
    MinMaxHeap<HeapItem, HeapItemLess> score_heap;

    // sets valid_arguments to true if the argument evaluation is valid for
    // this procedure, false otherwise
    void eval_arguments();

    void set_iters();

    inline std::unique_ptr<TextSearch::TextSearchIter> get_iter(const TextSearch::TextIndex& text_search_index
    ) const
    {
        switch (search_type) {
        case TextSearch::SearchType::MATCH:
            return text_search_index.search<TextSearch::SearchType::MATCH, false>(query);
        case TextSearch::SearchType::PREFIX:
            return text_search_index.search<TextSearch::SearchType::PREFIX, false>(query);
        default:
            throw QueryExecutionException(
                "Unhandled search type: " + std::to_string(static_cast<uint8_t>(search_type))
            );
        }
    }
};
} // namespace Procedure

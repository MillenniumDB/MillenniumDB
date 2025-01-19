#pragma once

#include <memory>

#include "query/executor/binding_iter.h"
#include "storage/index/text_search/text_search_index.h"

class TextSearchIndexScan : public BindingIter {
public:
    TextSearchIndexScan(
        TextSearch::TextSearchIndex& text_search_index_,
        const std::string& query_,
        TextSearch::SearchType search_type_,
        VarId object_var_,
        VarId match_var_
    ) :
        text_search_index { text_search_index_ },
        object_var { object_var_ },
        match_var { match_var_ },
        search_type { search_type_ },
        query { query_ }
    { }

    void accept_visitor(BindingIterVisitor& visitor) override;
    void _begin(Binding& parent_binding) override;
    bool _next() override;
    void _reset() override;
    void assign_nulls() override;

private:
    TextSearch::TextSearchIndex& text_search_index;

    const VarId object_var;
    const VarId match_var;
    const TextSearch::SearchType search_type;
    const std::string query;

    Binding* parent_binding;

    std::unique_ptr<TextSearch::TextSearchIter> text_search_iter;

    void set_text_search_iter();
};

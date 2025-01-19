#pragma once

#include <memory>

#include "graph_models/quad_model/quad_model.h"
#include "query/parser/expr/expr.h"
#include "query/query_context.h"
#include "query/var_id.h"
#include "storage/index/text_search/search_type.h"
#include "storage/index/text_search/text_search_index.h"

namespace MQL {

class ExprTextSearch : public Expr {
public:
    VarId object_var;
    VarId match_var;

    std::string text_search_index_name;
    std::string query;

    TextSearch::TextSearchIndex* text_search_index_ptr;

    TextSearch::SearchType search_type;

    ExprTextSearch(
        std::string&& text_search_index_name_,
        std::string&& query_,
        TextSearch::SearchType search_type_,
        VarId object_var_,
        VarId match_var_
    ) :
        object_var { object_var_ },
        match_var { match_var_ },
        text_search_index_name { std::move(text_search_index_name_) },
        query { std::move(query_) },
        search_type { search_type_ }
    {
        const bool text_search_index_found = quad_model.catalog.text_search_index_manager
                                                 .get_text_search_index(
                                                     text_search_index_name,
                                                     &text_search_index_ptr
                                                 );
        if (!text_search_index_found) {
            throw QuerySemanticException("TextSearchIndex not found: \"" + text_search_index_name + "\"");
        }
    }

    std::unique_ptr<Expr> clone() const override
    {
        auto text_search_index_name_clone = text_search_index_name;
        auto query_clone = query;

        return std::make_unique<ExprTextSearch>(
            std::move(text_search_index_name_clone),
            std::move(query_clone),
            search_type,
            object_var,
            match_var
        );
    }

    void accept_visitor(ExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override
    {
        return { object_var, match_var };
    }

    bool has_aggregation() const override
    {
        return false;
    }

    // Used when pushed to OpBasicGraphPattern (sort in std::set)
    bool operator<(const ExprTextSearch& other) const
    {
        if (other.object_var < object_var) {
            return true;
        } else if (object_var < other.object_var) {
            return false;
        } else if (other.match_var < match_var) {
            return true;
        } else if (match_var < other.match_var) {
            return false;
        } else if (other.search_type < search_type) {
            return true;
        } else if (search_type < other.search_type) {
            return false;
        } else if (other.text_search_index_ptr < text_search_index_ptr) {
            return true;
        } else if (text_search_index_ptr < other.text_search_index_ptr) {
            return false;
        } else if (other.query < query) {
            return true;
        } else {
            return query < other.query;
        }
    }

    // Used when pushed to OpBasicGraphPattern (Op printing)
    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const
    {
        os << std::string(indent, ' ');
        os << "ExprTextSearch(\"";
        os << text_search_index_name << "\", \"";
        os << query << "\", ";
        os << search_type << ", ";
        os << get_query_ctx().get_var_name(object_var) << ", ";
        os << get_query_ctx().get_var_name(match_var);
        return os << ")\n";
    }
};
} // namespace MQL

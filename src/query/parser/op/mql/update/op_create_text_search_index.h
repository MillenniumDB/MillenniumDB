#pragma once

#include <string>

#include "query/parser/op/op.h"
#include "storage/index/text_search/normalize_funcs.h"
#include "storage/index/text_search/tokenize_funcs.h"

namespace MQL {

class OpCreateTextSearchIndex : public Op {
public:
    const std::string text_search_index_name;
    const std::string property;
    const TextSearch::NORMALIZE_TYPE normalize_type;
    const TextSearch::TOKENIZE_TYPE tokenize_type;

    OpCreateTextSearchIndex(
        std::string&& text_search_index_name,
        std::string&& property,
        TextSearch::NORMALIZE_TYPE normalize_type,
        TextSearch::TOKENIZE_TYPE tokenize_type
    ) :
        text_search_index_name { std::move(text_search_index_name) },
        property { std::move(property) },
        normalize_type { normalize_type },
        tokenize_type { tokenize_type }
    { }

    std::unique_ptr<Op> clone() const override
    {
        auto text_search_index_name_clone = text_search_index_name;
        auto property_clone = property;
        return std::make_unique<OpCreateTextSearchIndex>(
            std::move(text_search_index_name_clone),
            std::move(property_clone),
            normalize_type,
            tokenize_type
        );
    }

    bool read_only() const override
    {
        return false;
    }

    void accept_visitor(OpVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override
    {
        return {};
    }

    std::set<VarId> get_scope_vars() const override
    {
        return {};
    }

    std::set<VarId> get_safe_vars() const override
    {
        return {};
    }

    std::set<VarId> get_fixable_vars() const override
    {
        return {};
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override
    {
        os << std::string(indent, ' ');
        os << "OpCreateTextSearchIndex(text_search_index_name: " << text_search_index_name
           << ", property: " << property << ", normalize_type: " << normalize_type
           << ", tokenize_type: " << tokenize_type << ")\n";
        return os;
    }
};
} // namespace MQL
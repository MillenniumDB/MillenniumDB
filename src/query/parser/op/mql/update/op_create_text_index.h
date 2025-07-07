#pragma once

#include <string>

#include "query/parser/op/mql/op.h"
#include "storage/index/text_search/normalize_funcs.h"
#include "storage/index/text_search/tokenize_funcs.h"

namespace MQL {

class OpCreateTextIndex : public Op {
public:
    const std::string index_name;
    const std::string property;
    const TextSearch::NORMALIZE_TYPE normalize_type;
    const TextSearch::TOKENIZE_TYPE tokenize_type;

    OpCreateTextIndex(
        std::string&& index_name,
        std::string&& property,
        TextSearch::NORMALIZE_TYPE normalize_type,
        TextSearch::TOKENIZE_TYPE tokenize_type
    ) :
        index_name { std::move(index_name) },
        property { std::move(property) },
        normalize_type { normalize_type },
        tokenize_type { tokenize_type }
    { }

    std::unique_ptr<Op> clone() const override
    {
        auto text_search_index_name_clone = index_name;
        auto property_clone = property;
        return std::make_unique<OpCreateTextIndex>(
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

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override
    {
        os << std::string(indent, ' ');
        os << "OpCreateTextIndex(index_name: " << index_name
           << ", property: " << property << ", normalize_type: " << normalize_type
           << ", tokenize_type: " << tokenize_type << ")\n";
        return os;
    }
};
} // namespace MQL

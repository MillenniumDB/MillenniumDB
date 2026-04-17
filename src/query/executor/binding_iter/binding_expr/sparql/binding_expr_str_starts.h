#pragma once

#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

#include <memory>

namespace SPARQL {
class BindingExprStrStarts : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> lhs;
    std::unique_ptr<BindingExpr> rhs;

    BindingExprStrStarts(std::unique_ptr<BindingExpr> lhs, std::unique_ptr<BindingExpr> rhs) :
        lhs(std::move(lhs)),
        rhs(std::move(rhs))
    { }

    ObjectId eval(const Binding& binding) override
    {
        auto lhs_oid = lhs->eval(binding);
        auto rhs_oid = rhs->eval(binding);

        auto lhs_sub = lhs_oid.subtype();
        auto rhs_sub = rhs_oid.subtype();

        if (lhs_sub != ObjectSubType::String && lhs_sub != ObjectSubType::StringXsd
            && lhs_sub != ObjectSubType::StringLang)
        {
            return ObjectId::get_null();
        }

        if (rhs_sub != ObjectSubType::String && rhs_sub != ObjectSubType::StringXsd
            && rhs_sub != ObjectSubType::StringLang)
        {
            return ObjectId::get_null();
        }

        if (lhs_sub != ObjectSubType::StringLang && rhs_sub == ObjectSubType::StringLang) {
            return ObjectId::get_null();
        }

        std::string lhs_str;
        std::string rhs_str;

        if (lhs_sub == ObjectSubType::StringLang && rhs_sub == ObjectSubType::StringLang) {
            auto [lhs_l, lhs_s] = Conversions::unpack_string_lang(lhs_oid);
            auto [rhs_l, rhs_s] = Conversions::unpack_string_lang(rhs_oid);
            if (lhs_l != rhs_l) {
                return ObjectId::get_null();
            }
            lhs_str = std::move(lhs_s);
            rhs_str = std::move(rhs_s);
        } else if (lhs_sub == ObjectSubType::String || lhs_sub == ObjectSubType::StringXsd) {
            lhs_str = Conversions::to_lexical_str(lhs_oid);
            rhs_str = Conversions::to_lexical_str(rhs_oid);
        } else {
            auto [lhs_l, lhs_s] = Conversions::unpack_string_lang(lhs_oid);
            lhs_str = lhs_s;
            rhs_str = Conversions::to_lexical_str(rhs_oid);
        }

        auto lhs_size = lhs_str.size();
        auto rhs_size = rhs_str.size();

        if (rhs_size == 0) {
            return Conversions::pack_bool(true);
        }
        if (lhs_size == 0) {
            return Conversions::pack_bool(false);
        }
        if (lhs_size < rhs_size) {
            return Conversions::pack_bool(false);
        }

        auto prefix = lhs_str.substr(0, rhs_size);
        return Conversions::pack_bool(prefix == rhs_str);
    }

    void accept_visitor(BindingExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    void print(std::ostream& os, std::vector<BindingIter*>& ops) const override
    {
        os << "STRSTARTS(";
        lhs->print(os, ops);
        os << ", ";
        rhs->print(os, ops);
        os << ')';
    }
};
} // namespace SPARQL

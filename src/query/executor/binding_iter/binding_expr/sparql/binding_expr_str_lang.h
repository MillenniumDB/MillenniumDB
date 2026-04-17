#pragma once

#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

#include <memory>

namespace SPARQL {
class BindingExprStrLang : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> lhs;
    std::unique_ptr<BindingExpr> rhs;

    BindingExprStrLang(std::unique_ptr<BindingExpr> lhs, std::unique_ptr<BindingExpr> rhs) :
        lhs(std::move(lhs)),
        rhs(std::move(rhs))
    { }

    ObjectId eval(const Binding& binding) override
    {
        auto lhs_oid = lhs->eval(binding);
        auto rhs_oid = rhs->eval(binding);

        auto expr1_subtype = lhs_oid.subtype();
        if (expr1_subtype != ObjectSubType::String && expr1_subtype != ObjectSubType::StringXsd) {
            return ObjectId::get_null();
        }

        auto expr2_subtype = rhs_oid.subtype();
        if (expr2_subtype != ObjectSubType::String) {
            return ObjectId::get_null();
        }

        auto str = Conversions::to_lexical_str(lhs_oid);
        auto lang = Conversions::unpack_string(rhs_oid);
        return Conversions::pack_string_lang(lang, str);
    }

    void accept_visitor(BindingExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    void print(std::ostream& os, std::vector<BindingIter*>& ops) const override
    {
        os << "STRLANG(";
        lhs->print(os, ops);
        os << ", ";
        rhs->print(os, ops);
        os << ')';
    }
};
} // namespace SPARQL

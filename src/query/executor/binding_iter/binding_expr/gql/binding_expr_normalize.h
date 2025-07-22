#pragma once

#include <cassert>
#include <memory>
#include <unicode/normalizer2.h>
#include <unicode/unistr.h>
#include <unicode/utypes.h>

#include "graph_models/gql/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace GQL {
class BindingExprNormalize : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> expr;
    std::string form;

    BindingExprNormalize(std::unique_ptr<BindingExpr> expr, std::string form) :
        expr(std::move(expr)),
        form(std::move(form))
    { }

    ObjectId eval(const Binding& binding) override
    {
        auto expr_oid = expr->eval(binding);
        auto expr_generic_type = GQL_OID::get_generic_type(expr_oid);

        if (expr_generic_type == GQL_OID::GenericType::STRING) {
            auto str = GQL::Conversions::unpack_string(expr_oid);
            icu::UnicodeString uInput = icu::UnicodeString::fromUTF8(str);
            icu::UnicodeString uOutput;
            const icu::Normalizer2* normalizer = nullptr;
            UErrorCode status = U_ZERO_ERROR;
            if (form == "NFC") {
                normalizer = icu::Normalizer2::getInstance(nullptr, "nfc", UNORM2_COMPOSE, status);
            } else if (form == "NFD") {
                normalizer = icu::Normalizer2::getInstance(nullptr, "nfc", UNORM2_DECOMPOSE, status);
            } else if (form == "NFKC") {
                normalizer = icu::Normalizer2::getInstance(nullptr, "nfkc", UNORM2_COMPOSE, status);
            } else {
                normalizer = icu::Normalizer2::getInstance(nullptr, "nfkc", UNORM2_DECOMPOSE, status);
            }
            normalizer->normalize(uInput, uOutput, status);
            std::string out;
            uOutput.toUTF8String(out);
            return GQL::Conversions::pack_string_simple(out);
        } else {
            return ObjectId::get_null();
        }
    }

    void accept_visitor(BindingExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    void print(std::ostream& os, std::vector<BindingIter*> ops) const override
    {
        os << "NORMALIZE(";
        expr->print(os, ops);
        os << ",";
        os << form;
        os << ")";
    }
};
} // namespace GQL

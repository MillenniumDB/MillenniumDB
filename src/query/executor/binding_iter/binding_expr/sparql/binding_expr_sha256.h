#pragma once

#include <cstdio>
#include <memory>

#include <openssl/sha.h>

#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace SPARQL {
class BindingExprSHA256 : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> expr;

    BindingExprSHA256(std::unique_ptr<BindingExpr> expr) :
        expr(std::move(expr))
    { }

    ObjectId eval(const Binding& binding) override
    {
        auto expr_oid = expr->eval(binding);

        switch (RDF_OID::get_generic_sub_type(expr_oid)) {
        case RDF_OID::GenericSubType::STRING_SIMPLE: {
            auto str = Conversions::unpack_string(expr_oid);
            unsigned char hash_bytes_buffer[SHA256_DIGEST_LENGTH + 1];
            auto hash_bytes = SHA256((const unsigned char*) str.data(), str.size(), hash_bytes_buffer);

            char hash_bytes_str[2 * SHA256_DIGEST_LENGTH + 1];

            for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
                snprintf(hash_bytes_str + i * 2, 3, "%02x", hash_bytes[i]);
            }

            std::string hash(hash_bytes_str, SHA256_DIGEST_LENGTH * 2);
            return Conversions::pack_string_simple(hash);
        }
        default:
            return ObjectId::get_null();
        }
    }

    void accept_visitor(BindingExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    void print(std::ostream& os, std::vector<BindingIter*> ops) const override
    {
        os << "SHA256(";
        expr->print(os, ops);
        os << ')';
    }
};
} // namespace SPARQL

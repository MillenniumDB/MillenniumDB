#pragma once

#include <cstdio>
#include <memory>

#include <openssl/evp.h>
#include <openssl/md5.h>

#include "graph_models/object_id.h"
#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

#ifdef OPENSSL_VERSION_MAJOR
    #if OPENSSL_VERSION_MAJOR == 3
        #define MDB_USE_MD5_EVP_Q_DIGEST
    #endif
#endif

namespace SPARQL {
class BindingExprMD5 : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> expr;

    BindingExprMD5(std::unique_ptr<BindingExpr> expr) :
        expr (std::move(expr)) { }

    ObjectId eval(const Binding& binding) override {
        auto expr_oid = expr->eval(binding);

        switch (RDF_OID::get_generic_sub_type(expr_oid)) {
        case RDF_OID::GenericSubType::STRING_SIMPLE: {
            auto str = Conversions::unpack_string(expr_oid);

            #ifdef MDB_USE_MD5_EVP_Q_DIGEST
            unsigned char hash_bytes[MD5_DIGEST_LENGTH+1];
            EVP_Q_digest(nullptr, "MD5", nullptr, (const unsigned char*)str.data(), str.size(), hash_bytes, nullptr);
            #else
            unsigned char hash_bytes_buf[MD5_DIGEST_LENGTH+1];
            auto hash_bytes = MD5((const unsigned char*)str.data(), str.size(), hash_bytes_buf);
            #endif

            char hash_bytes_str[2*MD5_DIGEST_LENGTH+1];

            for (int i = 0; i < MD5_DIGEST_LENGTH; i++) {
                snprintf(hash_bytes_str+i*2, 3, "%02x", hash_bytes[i]);
            }

            std::string hash(hash_bytes_str, MD5_DIGEST_LENGTH*2);
            return Conversions::pack_string_simple(hash);
        }
        default:
            return ObjectId::get_null();
        }
    }

    void accept_visitor(BindingExprVisitor& visitor) override {
        visitor.visit(*this);
    }
};
} // namespace SPARQL

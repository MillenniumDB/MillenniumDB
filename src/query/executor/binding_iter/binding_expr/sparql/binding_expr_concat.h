#pragma once

#include <memory>
#include <vector>

#include "graph_models/object_id.h"
#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"


namespace SPARQL {
class BindingExprConcat : public BindingExpr {
public:
    std::vector<std::unique_ptr<BindingExpr>> exprs;

    BindingExprConcat(std::vector<std::unique_ptr<BindingExpr>> exprs) :
        exprs(std::move(exprs)) { }

    ObjectId eval(const Binding& binding) override {
        int32_t     type = -3; // -3 unset, -2 xsd:string, -1 simple, >=0 lang
        std::string res;

        for (auto& expr : exprs) {
            auto expr_oid = expr->eval(binding);
            switch (expr_oid.get_sub_type()) {
            case ObjectId::MASK_STRING_SIMPLE: {
                auto str = Conversions::unpack_string_simple(expr_oid);
                res += str;
                type = -1;
                break;
            }
            case ObjectId::MASK_STRING_XSD: {
                auto str = Conversions::unpack_string_xsd(expr_oid);
                res += str;
                if (type == -3) {
                    type = -2;
                } else if (type != -2) {
                    type = -1;
                }
                break;
            }
            case ObjectId::MASK_STRING_LANG: {
                auto [lang, str] = Conversions::unpack_string_lang(expr_oid);
                res += str;
                if (type == -3) {
                    type = lang;
                } else if (type != lang) {
                    type = -1;
                }
                break;
            }
            default: {
            }
                return ObjectId::get_null();
            }
        }

        if (exprs.size() == 0) {
            type = -1;
        }

        assert(type >= -2);
        if (type == -1) {
            return Conversions::pack_string_simple(res);
        } else if (type == -2) {
            return Conversions::pack_string_xsd(res);
        } else {
            return Conversions::pack_string_lang(type, res);
        }
    }

    std::ostream& print_to_ostream(std::ostream& os) const override {
        os << "CONCAT(";

        for (size_t i = 0; i < exprs.size(); i++) {
            if (i != 0) {
                os << ", ";
            }
            os << *exprs[i];
        }
        os << ")";
        return os;
    }
};
} // namespace SPARQL

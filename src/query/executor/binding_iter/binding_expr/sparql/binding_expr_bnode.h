#pragma once

#include <memory>

#include "graph_models/object_id.h"
#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"
#include "query/query_context.h"

namespace SPARQL {
class BindingExprBNode : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> expr;

    BindingExprBNode(std::unique_ptr<BindingExpr> expr) :
        expr (std::move(expr)) { }

    ObjectId eval(const Binding& binding) override {
        auto oid = expr->eval(binding);
        switch(oid.get_sub_type()) {
        case ObjectId::MASK_STRING_SIMPLE: {
            auto str = Conversions::unpack_string_simple(oid);
            auto it = get_query_ctx().blank_node_ids.find(str);
            if (it == get_query_ctx().blank_node_ids.end()) {
                auto new_bnode = get_query_ctx().get_new_blank_node();
                get_query_ctx().blank_node_ids.insert({str, new_bnode});
                auto ret = ObjectId(ObjectId::MASK_ANON_TMP | new_bnode);
                return ret;
            } else {
                return ObjectId(ObjectId::MASK_ANON_TMP | it->second);
            }
        }
        default:
            return ObjectId::get_null();
        }
    }

    std::ostream& print_to_ostream(std::ostream& os) const override {
        os << "BNODE(" << *expr << ")";
        return os;
    }
};
} // namespace SPARQL

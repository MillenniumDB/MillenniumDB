#pragma once

#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"
#include "query/query_context.h"

#include <memory>

namespace SPARQL {

class BindingExprBNode : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> expr;

    BindingExprBNode(std::unique_ptr<BindingExpr> expr) :
        expr(std::move(expr))
    { }

    ObjectId eval(const Binding& binding) override
    {
        auto oid = expr->eval(binding);
        switch (oid.subtype()) {
        case ObjectSubType::String:
        case ObjectSubType::StringXsd: {
            auto str = Conversions::unpack_string(oid);
            auto it = get_query_ctx().blank_node_ids.find(str);
            if (it == get_query_ctx().blank_node_ids.end()) {
                auto new_bnode = get_query_ctx().get_new_blank_node();
                get_query_ctx().blank_node_ids.insert({ str, new_bnode });
                return Conversions::pack_blank_tmp(new_bnode);
            } else {
                return Conversions::pack_blank_tmp(it->second);
            }
        }
        default:
            return ObjectId::get_null();
        }
    }

    void accept_visitor(BindingExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    void print(std::ostream& os, std::vector<BindingIter*>& ops) const override
    {
        os << "BNODE(";
        if (expr != nullptr) {
            expr->print(os, ops);
        }
        os << ')';
    }
};
} // namespace SPARQL

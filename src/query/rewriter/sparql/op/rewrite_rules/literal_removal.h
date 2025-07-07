#pragma once

#include <cassert>
#include <memory>
#include <set>

#include "graph_models/object_id.h"
#include "graph_models/rdf_model/rdf_object_id.h"
#include "query/parser/op/sparql/op_visitor.h"
#include "query/parser/op/sparql/ops.h"
#include "rewrite_rule.h"
#include "query/rewriter/sparql/expr/expr_rewrite_rule_visitor.h"

namespace SPARQL {
/**
  Filter[..., True, ...](op) = Filter[..., ...](op)
  Filter[..., False, ...](op) = Empty
 */
class LiteralRemoval : public RewriteRule {
private:
    ExprRewriteRuleVisitor expr_visitor;

public:
    bool is_possible_to_regroup(std::unique_ptr<Op>& unknown_op) override {
        if (!is_castable_to<OpFilter>(unknown_op)) {
            return false;
        }
        bool has_rewritten = false;
        auto op_filter = dynamic_cast<OpFilter*>(unknown_op.get());
        std::vector<std::unique_ptr<Expr>> new_filters;
        for (auto &expr : op_filter->filters) {
            if (is_bool(expr)) {
                has_rewritten = true;
                auto expr_object_id = dynamic_cast<ExprTerm*>(expr.get());
                bool val = get_bool(expr_object_id->term);
                //std::cout << "Removing a bool: " << val << std::endl;
                if (val == true)
                    continue;
                else {
                    op_filter->op = std::make_unique<OpEmpty>(op_filter->op);
                    new_filters.clear();
                    break;
                }
            }
            new_filters.push_back(std::move(expr));
        }
        op_filter->filters = std::move(new_filters);
        return has_rewritten;
    }

    std::unique_ptr<Op> regroup(std::unique_ptr<Op> unknown_op) override {
        // All work is done in is_possible_to_regroup, this is because
        // is_possible_to_regroup marks whether a change will be made
        // here. This is the only way to use the visitor interface for
        // expr's.
        return unknown_op;
    }

private:
    bool is_bool(std::unique_ptr<Expr>& literal) {
        auto expr = dynamic_cast<ExprTerm*>(literal.get());
        if (expr == nullptr) {
            return false;
        }
        return RDF_OID::get_generic_type(expr->term) == RDF_OID::GenericType::BOOL;
    }

    bool get_bool(ObjectId oid) {
        if (oid.is_true()) {
            return true;
        } else {
            assert(oid.is_false());
            return false;
        }
    }

};
} // namespace SPARQL

#include "expr_to_binding_expr.h"


#include "query/optimizer/quad_model/binding_iter_constructor.h"
#include "query/optimizer/quad_model/expr_property_types_visitor.h"
#include "query/parser/expr/mql_exprs.h"

using namespace MQL;

void ExprPropertyTypes::visit(ExprIs& expr)
{
    auto potential_expr_var_property = dynamic_cast<ExprVarProperty*>(expr.expr.get());

    if (potential_expr_var_property != nullptr) {
        propId_and_bitmap = std::make_unique<std::unordered_map<uint64_t, uint64_t>>();
        propId_and_bitmap->insert({ potential_expr_var_property->var_with_property.id, expr.type_bitmap });

        auto info_collected = false;
        for (auto& p : bic->properties_types_queries) {
            if (p.var_without_property.id == potential_expr_var_property->var_with_property.id) {
                info_collected = true;
            }
        }

        if (!info_collected) {
            auto propertyType = PropertyTypeConstraint(
                potential_expr_var_property->var_without_property,
                potential_expr_var_property->key,
                potential_expr_var_property->var_with_property,
                expr.type_bitmap
            );
            bic->properties_types_queries.push_back(propertyType);
        }
    }
}

void ExprPropertyTypes::visit(ExprAnd& expr)
{
    std::unordered_map<uint64_t, uint64_t> new_propId;
    for (auto& e : expr.and_list) {
        auto potential_or_expr = dynamic_cast<ExprOr*>(e.get());
        auto potential_is_expr = dynamic_cast<ExprIs*>(e.get());
        if (potential_or_expr != nullptr || potential_is_expr != nullptr) {
            e->accept_visitor(*this);
            for (const auto& pair : *propId_and_bitmap) {
                auto key = pair.first;
                auto it = new_propId.find(key);

                if (it != new_propId.end()) { // ya existe
                    it->second &= pair.second;
                } else {
                    new_propId.insert({ key, pair.second });
                }
            }
        }
    }
    propId_and_bitmap = std::make_unique<std::unordered_map<uint64_t, uint64_t>>(std::move(new_propId));
}

void ExprPropertyTypes::visit(ExprOr& expr)
{
    std::unordered_map<uint64_t, uint64_t> new_propId;
    for (auto& e : expr.or_list) {
        auto potential_and_expr = dynamic_cast<ExprAnd*>(e.get());
        auto potential_is_expr = dynamic_cast<ExprIs*>(e.get());
        if (potential_and_expr != nullptr || potential_is_expr != nullptr) {
            e->accept_visitor(*this);
            for (const auto& pair : *propId_and_bitmap) {
                auto key = pair.first;
                auto it = new_propId.find(key);

                if (it != new_propId.end()) {
                    it->second |= pair.second;
                } else {
                    new_propId.insert({ key, pair.second });
                }
            }
        }
    }
    propId_and_bitmap = std::make_unique<std::unordered_map<uint64_t, uint64_t>>(std::move(new_propId));
}

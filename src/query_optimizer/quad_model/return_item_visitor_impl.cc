#include "return_item_visitor_impl.h"

#include "execution/binding_iter/aggregation/aggs.h"
#include "parser/query/return_item/return_items.h"

using namespace MDB;

void ReturnItemVisitorImpl::visit(ReturnItemAgg& return_item) {
    auto pos = return_item.inside_var.find('.');
    if (pos != std::string::npos) {
        // we split something like "?x1.key1" into "?x" and "key1"
        auto var_without_property = return_item.inside_var.substr(0, pos);
        auto var_key              = return_item.inside_var.substr(pos + 1);
        binding_iter_visitor.var_properties.insert({ Var(var_without_property), var_key });
    }

    Var var = return_item.get_var();
    auto var_id = binding_iter_visitor.get_var_id(var);

    Var inside_var(return_item.inside_var);
    auto inside_var_id = binding_iter_visitor.get_var_id(inside_var);

    if (return_item.aggregate_func == "avg") {
        binding_iter_visitor.aggs.insert({var_id , std::make_unique<AggAvg>(inside_var_id)});
    } else if (return_item.aggregate_func == "max") {
        binding_iter_visitor.aggs.insert({var_id , std::make_unique<AggMax>(inside_var_id)});
    } else if (return_item.aggregate_func == "min") {
        binding_iter_visitor.aggs.insert({var_id , std::make_unique<AggMin>(inside_var_id)});
    } else if (return_item.aggregate_func == "sum") {
        binding_iter_visitor.aggs.insert({var_id , std::make_unique<AggSum>(inside_var_id)});
    }
    binding_iter_visitor.group_saved_vars.insert(inside_var_id);
}


void ReturnItemVisitorImpl::visit(ReturnItemCount& return_item) {
    if (return_item.inside_var == "*") {
        Var var = return_item.get_var();
        auto var_id = binding_iter_visitor.get_var_id(var);
        if (return_item.distinct) {
            std::vector<VarId> var_ids;
            for (auto&& [var, var_id] : binding_iter_visitor.var2var_id) {
                if (var.name[0] == '?' && var.name[1] != '_') {
                    var_ids.push_back(var_id);
                    binding_iter_visitor.group_saved_vars.insert(var_id);
                }
            }
            binding_iter_visitor.aggs.insert({var_id , std::make_unique<AggCountAllDistinct>(std::move(var_ids))});
        } else {
            binding_iter_visitor.aggs.insert({var_id , std::make_unique<AggCountAll>()});
        }
    } else {
        auto pos = return_item.inside_var.find('.');
        if (pos != std::string::npos) {
            // we split something like "?x1.key1" into "?x" and "key1"
            auto var_without_property = return_item.inside_var.substr(0, pos);
            auto var_key              = return_item.inside_var.substr(pos + 1);
            binding_iter_visitor.var_properties.insert({ Var(var_without_property), var_key });
        }
        Var var = return_item.get_var();
        auto var_id = binding_iter_visitor.get_var_id(var);

        Var inside_var(return_item.inside_var);
        auto inside_var_id = binding_iter_visitor.get_var_id(inside_var);
        if (return_item.distinct) {
            binding_iter_visitor.aggs.insert({var_id , std::make_unique<AggCountVarDistinct>(inside_var_id)});
        } else {
            binding_iter_visitor.aggs.insert({var_id , std::make_unique<AggCountVar>(inside_var_id)});
        }
        binding_iter_visitor.group_saved_vars.insert(inside_var_id);
    }
}


void ReturnItemVisitorImpl::visit(ReturnItemVar& return_item) {
    std::string& var_str = return_item.var.name;
    auto pos = var_str.find('.');
    if (pos != std::string::npos) {
        // we split something like "?x1.key1" into "?x" and "key1"
        auto var_without_property = var_str.substr(0, pos);
        auto var_key              = var_str.substr(pos + 1);
        binding_iter_visitor.var_properties.insert({ Var(var_without_property), var_key });
    }

    auto var_id = binding_iter_visitor.get_var_id(return_item.var);
    binding_iter_visitor.group_saved_vars.insert(var_id);
}

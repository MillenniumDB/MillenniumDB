#include "text_search_multiscan.h"

#include "misc/transliterator.h"
#include "query/executor/binding_iter/binding_expr/binding_expr_printer.h"

using namespace Procedure;

void TextSearchMultiScan::_begin(Binding& parent_binding)
{
    this->parent_binding = &parent_binding;

    _reset();
}

bool TextSearchMultiScan::_next()
{
    if (!valid_arguments) {
        return false;
    }

    if (score_heap.empty()) {
        return false;
    }

    // extract highest score
    HeapItem heap_item = score_heap.get_max();
    score_heap.pop_max();

    // get both index and iter
    const auto* text_search_index = text_index_manager.get_text_index(text_index_names[heap_item.pos]);
    const auto& text_search_iter = text_search_iters[heap_item.pos];

    // get row
    const uint64_t table_pointer = text_search_iter->get_table_pointer();
    const std::vector<uint64_t> table_row = text_search_index->table->get(table_pointer);

    // add bindings
    parent_binding->add(yield_vars[0], ObjectId(table_row[0]));
    parent_binding->add(yield_vars[1], ObjectId(table_row[1]));

    if (text_search_iter->next()) {
        // there are more results in this iter, update its priority
        const double next_score = text_search_iter->get_score();
        heap_item.score = next_score; // reuse the heap item
        score_heap.emplace(std::move(heap_item));
    }

    return true;
}

void TextSearchMultiScan::_reset()
{
    eval_arguments();

    if (valid_arguments) {
        set_iters();
    }
}

void TextSearchMultiScan::assign_nulls()
{
    for (const auto& yield_var : yield_vars) {
        parent_binding->add(yield_var, ObjectId::get_null());
    }
}

void TextSearchMultiScan::print(std::ostream& os, int indent, bool stats) const
{
    if (stats) {
        print_generic_stats(os, indent);
    }
    os << std::string(indent, ' ') << "TextSearchMultiScan(";
    {
        BindingExprPrinter printer(os, indent, stats);
        if (!argument_binding_exprs.empty()) {
            printer.print(*argument_binding_exprs[0]);
        }
        for (std::size_t i = 1; i < argument_binding_exprs.size(); ++i) {
            os << ", ";
            printer.print(*argument_binding_exprs[i]);
        }

        os << ") -> (";

        os << yield_vars[0];
        for (std::size_t i = 1; i < yield_vars.size(); ++i) {
            os << ", ";
            os << yield_vars[i];
        }
    }
    os << ")\n";
}

void TextSearchMultiScan::eval_arguments()
{
    valid_arguments = false;
    const auto name_oid = argument_binding_exprs[0]->eval(*parent_binding);
    const auto name_gen_t = name_oid.id & ObjectId::GENERIC_TYPE_MASK;
    if (name_gen_t != ObjectId::MASK_STRING) {
        return;
    }
    index_name = unpack_string_func_ptr(name_oid);

    const auto query_oid = argument_binding_exprs[1]->eval(*parent_binding);
    const auto query_gen_t = query_oid.id & ObjectId::GENERIC_TYPE_MASK;
    if (query_gen_t != ObjectId::MASK_STRING) {
        return;
    }
    query = unpack_string_func_ptr(query_oid);

    const auto search_type_oid = argument_binding_exprs[2]->eval(*parent_binding);
    const auto search_type_gen_t = search_type_oid.id & ObjectId::GENERIC_TYPE_MASK;
    if (search_type_gen_t != ObjectId::MASK_STRING) {
        return;
    }
    const std::string search_type_str = unpack_string_func_ptr(search_type_oid);
    const std::string search_type_str_lowercased = Transliterator::lowercase(search_type_str);

    if (search_type_str_lowercased == "match") {
        search_type = TextSearch::SearchType::MATCH;
    } else if (search_type_str_lowercased == "prefix") {
        search_type = TextSearch::SearchType::PREFIX;
    } else {
        return;
    }

    valid_arguments = true;
}

void TextSearchMultiScan::set_iters()
{
    text_index_names.clear();
    text_search_iters.clear();
    score_heap.clear();

    if (index_name != "*") {
        // search in a single index
        const auto* text_index_ptr = text_index_manager.get_text_index(index_name);

        if (text_index_ptr == nullptr) {
            // index did not exist
            valid_arguments = false;
            return;
        }

        auto text_search_iter = get_iter(*text_index_ptr);
        if (text_search_iter->next()) {
            const double score = text_search_iter->get_score();
            text_index_names.emplace_back(index_name);
            text_search_iters.emplace_back(std::move(text_search_iter));
            score_heap.emplace(score, 0);
        }

        return;
    }

    // search over all indexes
    const auto index_names = text_index_manager.get_index_names();
    for (const auto& name : index_names) {
        auto text_index = text_index_manager.get_text_index(name);
        auto text_search_iter = get_iter(*text_index);

        if (text_search_iter->next()) {
            const double score = text_search_iter->get_score();
            const std::size_t i = text_search_iters.size();
            text_index_names.emplace_back(name);
            text_search_iters.emplace_back(std::move(text_search_iter));
            score_heap.emplace(score, i);
        }
    }
}

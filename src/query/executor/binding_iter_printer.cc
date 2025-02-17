#include "binding_iter_printer.h"

#include "query/executor/binding_iter.h"
#include "query/executor/binding_iters.h"
#include "query/parser/op/op.h"
#include "storage/index/tensor_store/lsh/binding_iters/forest_index_top_k.h"
#include "storage/index/tensor_store/lsh/binding_iters/forest_index_top_all.h"
#include "storage/index/tensor_store/lsh/binding_iters/project_tensor_similarity.h"
#include "storage/index/tensor_store/lsh/binding_iters/brute_similarity_search.h"
#include "storage/index/tensor_store/lsh/metric.h"
#include "storage/index/tensor_store/tensor_store.h"

class BindingIterPrinterHelper {
    BindingIterPrinter& printer;

public:
    BindingIterPrinterHelper(
        const std::string&  name,
        BindingIterPrinter& printer,
        BindingIter&        binding_iter,
        const std::string   stats = ""
    ) :
        printer(printer)
    {
        if (printer.print_stats) {
            printer.os << std::string(printer.indent, ' ');
            printer.os << "[begin: " << binding_iter.stat_begin;
            printer.os << ", next: " << binding_iter.stat_next;
            printer.os << ", reset: " << binding_iter.stat_reset;
            printer.os << ", results: " << binding_iter.results;
            if (stats != "") {
                printer.os << ", " << stats;
            }
            printer.os << "]\n";
        }

        printer.os << std::string(printer.indent, ' ') << name << '(';

        printer.indent += 2;
    }

    ~BindingIterPrinterHelper() {
        printer.indent -= 2;
    }
};


void BindingIterPrinter::print_join(
        const std::vector<VarId>&           parent_safe_vars,
        const std::vector<VarId>&           lhs_only_vars,
        const std::vector<VarId>&           safe_join_vars,
        const std::vector<VarId>&           unsafe_join_vars,
        const std::vector<VarId>&           rhs_only_vars,
        const std::unique_ptr<BindingIter>& lhs,
        const std::unique_ptr<BindingIter>& rhs
    ) {

    os << "variables: ";

    auto first = true;
    if (parent_safe_vars.size() > 0) {
        for (auto& var: parent_safe_vars) {
            if (first) first = false; else os << ", ";
            os << "[?" << get_query_ctx().get_var_name(var) << "]";
        }
    }

    if (!first) os << " ";
    os << "< ";

    first = true;
    for (auto& var: lhs_only_vars) {
        if (first) first = false; else os << ", ";
        os << "?" << get_query_ctx().get_var_name(var);
    }

    if (!first) os << " ";
    os << "| ";

    first = true;
    for (auto& var: safe_join_vars) {
        if (first) first = false; else os << ", ";
        os << "[?" << get_query_ctx().get_var_name(var) << "]";
    }

    for (auto& var: unsafe_join_vars) {
        if (first) first = false; else os << ", ";
        os << "?" << get_query_ctx().get_var_name(var);
    }

    if (!first) os << " ";
    os << "| ";

    first = true;
    for (auto& var: rhs_only_vars) {
        if (first) first = false; else os << ", ";
        os << "?" << get_query_ctx().get_var_name(var);
    }

    if (!first) os << " ";
    os << ">)\n";

    lhs->accept_visitor(*this);
    rhs->accept_visitor(*this);
}


void BindingIterPrinter::visit(Aggregation& binding_iter) {
    auto helper = BindingIterPrinterHelper("Aggregation", *this, binding_iter);

    auto first = true;

    if (binding_iter.group_vars.size() > 0) {
        os << "group_vars: ";

        for (auto var : binding_iter.group_vars) {
            if (first) first = false; else os << ", ";
            os << '?' << get_query_ctx().get_var_name(var);
        }
    }

    if (binding_iter.aggregations.size() > 0) {
        if (!first) os << ", ";
        os << "aggregations: ";

        first = true;
        for (auto& [var, agg] : binding_iter.aggregations) {
            if (first) first = false; else os << ", ";
            os << '?' << get_query_ctx().get_var_name(var) << '=' << *agg;
        }
    }

    os << ")\n";
    binding_iter.child->accept_visitor(*this);
}


void BindingIterPrinter::visit(Bind& binding_iter) {
    auto helper = BindingIterPrinterHelper("Bind", *this, binding_iter);

    auto printer = get_query_ctx().create_binding_expr_printer(os);

    os << '?' << get_query_ctx().get_var_name(binding_iter.var) << '=';
    binding_iter.expr->accept_visitor(*printer);
    os << ")\n";

    indent += 2;
    for (size_t i = 0; i < printer->ops.size(); i++) {
        os << std::string(indent - 2, ' ') << "_Op_" << i << "_:\n";
        printer->ops[i]->accept_visitor(*this);
    }
    indent -= 2;

    binding_iter.child_iter->accept_visitor(*this);
}


void BindingIterPrinter::visit(DistinctHash& binding_iter) {
    std::stringstream ss;
    ss << "processed: " << binding_iter.processed;
    auto helper = BindingIterPrinterHelper("DistinctHash", *this, binding_iter, ss.str());

    os << ")\n";
    binding_iter.child_iter->accept_visitor(*this);
}


void BindingIterPrinter::visit(EdgeTableLookup& binding_iter) {
    std::stringstream ss;
    ss << "lookups: " << binding_iter.lookups;
    auto helper = BindingIterPrinterHelper("EdgeTableLookup", *this, binding_iter, ss.str());

    os << ")\n";
}


void BindingIterPrinter::visit(EmptyBindingIter& binding_iter) {
    auto helper = BindingIterPrinterHelper("EmptyBindingIter", *this, binding_iter);
    os << ")\n";
}


void BindingIterPrinter::visit(Filter& binding_iter) {
    std::stringstream ss;
    ss << "filtered: " << binding_iter.filtered_results;
    auto helper = BindingIterPrinterHelper("Filter", *this, binding_iter, ss.str());

    os << "\n";

    auto printer = get_query_ctx().create_binding_expr_printer(os);

    for (auto& filter : binding_iter.filters) {
        os << std::string(indent, ' ');
        filter->accept_visitor(*printer);
        os << "\n";
    }

    indent += 2;
    for (size_t i = 0; i < printer->ops.size(); i++) {
        os << std::string(indent - 2, ' ') << "_Op_" << i << "_:\n";
        printer->ops[i]->accept_visitor(*this);
    }
    indent -= 2;

    os << std::string(indent - 2, ' ') <<")\n";
    binding_iter.child_iter->accept_visitor(*this);
}


void BindingIterPrinter::visit(ExprEvaluator& binding_iter) {
    auto helper = BindingIterPrinterHelper("ExprEvaluator", *this, binding_iter);

    os << "expressions: ";

    auto printer = get_query_ctx().create_binding_expr_printer(os);

    auto first = true;
    for (auto& [var, expr] : binding_iter.exprs) {
        if (first) first = false; else os << ", ";

        os << '?' << get_query_ctx().get_var_name(var);
        if (expr) {
            os << '=';
            expr->accept_visitor(*printer);
        }
    }

    if (printer->ops.size() == 0) {
        os << ")\n";
    } else {
        os << "\n";

        indent += 2;
        for (size_t i = 0; i < printer->ops.size(); i++) {
            os << std::string(indent - 2, ' ') << "_Op_" << i << "_:\n";
            printer->ops[i]->accept_visitor(*this);
        }
        indent -= 2;
        os << std::string(indent - 2, ' ') <<")\n";
    }

    binding_iter.child_iter->accept_visitor(*this);
}


void BindingIterPrinter::visit(IndexLeftOuterJoin& binding_iter) {
    auto helper = BindingIterPrinterHelper("IndexLeftOuterJoin", *this, binding_iter);
    os << ")\n";
    binding_iter.lhs->accept_visitor(*this);
    binding_iter.original_rhs->accept_visitor(*this);
}


void BindingIterPrinter::visit(IndexNestedLoopJoin& binding_iter) {
    auto helper = BindingIterPrinterHelper("IndexNestedLoopJoin", *this, binding_iter);
    os << ")\n";
    binding_iter.lhs->accept_visitor(*this);
    binding_iter.original_rhs->accept_visitor(*this);
}

template <size_t N>
void BindingIterPrinter::print_index_scan(IndexScan<N>& binding_iter) {
    std::stringstream ss;
    ss << "bpt_searches: " << binding_iter.bpt_searches;
    auto helper = BindingIterPrinterHelper("IndexScan", *this, binding_iter, ss.str());

    os  << "ranges:";

    for (auto& range : binding_iter.ranges) {
        os << " ";
        range->print(os);
    }
    os << ")\n";
}

void BindingIterPrinter::visit(IndexScan<1>& binding_iter) {
    print_index_scan(binding_iter);
}


void BindingIterPrinter::visit(IndexScan<2>& binding_iter) {
    print_index_scan(binding_iter);
}


void BindingIterPrinter::visit(IndexScan<3>& binding_iter) {
    print_index_scan(binding_iter);
}


void BindingIterPrinter::visit(IndexScan<4>& binding_iter) {
    print_index_scan(binding_iter);
}


void BindingIterPrinter::visit(LeapfrogJoin& binding_iter) {
    std::stringstream ss;
    ss << "seeks: " << binding_iter.seeks;
    auto helper = BindingIterPrinterHelper("LeapfrogJoin", *this, binding_iter, ss.str());

    if (binding_iter.enumeration_level > 0) {
        os << "?" << get_query_ctx().get_var_name(binding_iter.var_order[0]);
    }
    for (int i = 1; i < binding_iter.enumeration_level; i++) {
        os << ", ?" << get_query_ctx().get_var_name(binding_iter.var_order[i]);
    }

    os << ")\n";

    for (auto& iter : binding_iter.leapfrog_iters) {
        os << std::string(indent, ' ') << iter->get_iter_name() << "(ranges:";
        for (auto& range : iter->initial_ranges) {
            os << " ";
            range->print(os);
        }
        for (auto& var : iter->get_intersection_vars()) {
            os << " ?" << get_query_ctx().get_var_name(var);
        }
        for (auto& var : iter->get_enumeration_vars()) {
            os << " ?" << get_query_ctx().get_var_name(var);
        }
        os << ")\n";
    }
}


void BindingIterPrinter::visit(NestedLoopAntiJoin<false>& binding_iter) {
    auto helper = BindingIterPrinterHelper("NestedLoopAntiJoin<false>", *this, binding_iter);
    print_join(binding_iter.parent_safe_vars,
               binding_iter.lhs_only_vars,
               binding_iter.safe_join_vars,
               binding_iter.unsafe_join_vars,
               binding_iter.rhs_only_vars,
               binding_iter.lhs,
               binding_iter.rhs);
}


void BindingIterPrinter::visit(NestedLoopAntiJoin<true>& binding_iter) {
    auto helper = BindingIterPrinterHelper("NestedLoopAntiJoin<true>", *this, binding_iter);
    print_join(binding_iter.parent_safe_vars,
               binding_iter.lhs_only_vars,
               binding_iter.safe_join_vars,
               binding_iter.unsafe_join_vars,
               binding_iter.rhs_only_vars,
               binding_iter.lhs,
               binding_iter.rhs);
}


void BindingIterPrinter::visit(NestedLoopJoin& binding_iter) {
    auto helper = BindingIterPrinterHelper("NestedLoopJoin", *this, binding_iter);
    print_join(binding_iter.parent_safe_vars,
               binding_iter.lhs_only_vars,
               binding_iter.safe_join_vars,
               binding_iter.unsafe_join_vars,
               binding_iter.rhs_only_vars,
               binding_iter.lhs,
               binding_iter.original_rhs);
}


void BindingIterPrinter::visit(NestedLoopLeftJoin& binding_iter) {
    auto helper = BindingIterPrinterHelper("NestedLoopLeftJoin", *this, binding_iter);
    print_join(binding_iter.parent_safe_vars,
               binding_iter.lhs_only_vars,
               binding_iter.safe_join_vars,
               binding_iter.unsafe_join_vars,
               binding_iter.rhs_only_vars,
               binding_iter.lhs,
               binding_iter.original_rhs);
}


void BindingIterPrinter::visit(NestedLoopSemiJoin& binding_iter) {
    auto helper = BindingIterPrinterHelper("NestedLoopSemiJoin", *this, binding_iter);
    print_join(binding_iter.parent_safe_vars,
               binding_iter.lhs_only_vars,
               binding_iter.safe_join_vars,
               binding_iter.unsafe_join_vars,
               binding_iter.rhs_only_vars,
               binding_iter.lhs,
               binding_iter.original_rhs);
}


void BindingIterPrinter::visit(NoFreeVariableMinus& binding_iter) {
    auto helper = BindingIterPrinterHelper("NoFreeVariableMinus", *this, binding_iter);
    os << ")\n";
    binding_iter.lhs->accept_visitor(*this);
    binding_iter.rhs->accept_visitor(*this);
}


void BindingIterPrinter::visit(ObjectEnum& binding_iter) {
    auto helper = BindingIterPrinterHelper("ObjectEnum", *this, binding_iter);
    os << "var: " << get_query_ctx().get_var_name(binding_iter.var);
    os << ", max_count: " << binding_iter.max_count;
    os << ")\n";

}


void BindingIterPrinter::visit(OrderBy& binding_iter) {
    auto helper = BindingIterPrinterHelper("OrderBy", *this, binding_iter);

    os << "order_vars: ";
    auto first = true;
    for (size_t i = 0; i < binding_iter.order_vars.size(); i++) {
        if (first) first = false; else os << ", ";
        if (binding_iter.ascending[i]) os << "ASC " ; else os << "DESC ";
        os << '?' << get_query_ctx().get_var_name(binding_iter.order_vars[i]);
    }

    os << ", saved_vars: ";
    first = true;
    for (auto [var, _] : binding_iter.saved_vars) {
        if (first) first = false; else os << ", ";
        os << '?' << get_query_ctx().get_var_name(var);
    }

    os << ")\n";
    binding_iter.child_iter->accept_visitor(*this);
}


void BindingIterPrinter::visit(SingleResultBindingIter& binding_iter) {
    auto helper = BindingIterPrinterHelper("SingleResultBindingIter", *this, binding_iter);
    os << ")\n";
}


void BindingIterPrinter::visit(Slice& binding_iter) {
    auto helper = BindingIterPrinterHelper("Slice", *this, binding_iter);

    auto printed = false;

    if (binding_iter.offset != Op::DEFAULT_OFFSET) {
        printed = true;
        os << "offset: " << binding_iter.offset;
    }

    if (binding_iter.limit != Op::DEFAULT_LIMIT) {
        if (printed) os << ", ";
        printed = true;
        os << "limit: " << binding_iter.limit;
    }

    os << ")\n";
    binding_iter.child_iter->accept_visitor(*this);
}


void BindingIterPrinter::visit(SparqlService& binding_iter) {
    std::stringstream ss;
    ss << "requests: " << binding_iter.network_requests << ", "
       << "total_request_time: " << binding_iter.response_parser.request_duration.count() << " ms, "
       << "total_parse_time: " << binding_iter.response_parser.parse_duration.count() << " ms.";

    auto helper = BindingIterPrinterHelper("SparqlService", *this, binding_iter, ss.str());

    if (binding_iter.silent) os << "SILENT ";
    if (std::holds_alternative<VarId>(binding_iter.response_parser.var_or_iri)) {
        auto service_id = std::get<VarId>(binding_iter.response_parser.var_or_iri);
        os << '?' << get_query_ctx().get_var_name(service_id);
    } else {
        os << '<' << std::get<std::string>(binding_iter.response_parser.var_or_iri) << '>';
    }

    if (binding_iter.scope_vars.size() > 0) {
        os << ", scope:";
        auto first = true;
        for (auto var : binding_iter.scope_vars) {
            if (first) first = false; else os << ", ";
            os << "?" << get_query_ctx().get_var_name(var);
        }
    }

    if (binding_iter.fixed_vars.size() > 0) {
        os << ", fixed:";
        auto first = true;
        for (auto var : binding_iter.fixed_vars) {
            if (first) first = false; else os << ", ";
            os << "?" << get_query_ctx().get_var_name(var);
        }
    }

    if (binding_iter.fixed_join_vars.size() > 0) {
        os << ", fixed_join:";
        auto first = true;
        for (auto var : binding_iter.fixed_join_vars) {
            if (first) first = false; else os << ", ";
            os << "?" << get_query_ctx().get_var_name(var);
        }
    }

    os << ")\n";

    os << std::string(indent, ' ');

    auto last_char_newline = false;
    for (auto& cha: binding_iter.response_parser.query) {
        if (cha != '\r') os << cha;
        if (cha == '\n') {
            os << std::string(indent - 4, ' ');
            last_char_newline = true;
        } else {
            last_char_newline = false;
        }
    }

    if (!last_char_newline) {
        os << '\n';
    }
}


void BindingIterPrinter::visit(SubSelect& binding_iter) {
    auto helper = BindingIterPrinterHelper("SubSelect", *this, binding_iter);

    auto printed = false;

    if (binding_iter.projection_vars.size() > 0) {
        printed = true;
        os << "proj_vars: ";
        auto first = true;
        for (auto var : binding_iter.projection_vars) {
            if (first) first = false; else os << ", ";
            os << "?" << get_query_ctx().get_var_name(var);
        }
    }

    if (binding_iter.safe_assigned_vars.size() > 0) {
        if (printed) os << ", ";
        printed = true;
        os << "fixed_vars: ";
        auto first = true;
        for (auto var : binding_iter.safe_assigned_vars) {
            if (first) first = false; else os << ", ";
            os << "?" << get_query_ctx().get_var_name(var);
        }
    }

    os << ")\n";
    binding_iter.child_iter->accept_visitor(*this);
}


void BindingIterPrinter::visit(Union& binding_iter) {
    auto helper = BindingIterPrinterHelper("Union", *this, binding_iter);
    os << ")\n";
    for (const auto& iter : binding_iter.iters) {
        iter->accept_visitor(*this);
    }
}


void BindingIterPrinter::visit(Values& binding_iter) {
    auto helper = BindingIterPrinterHelper("Values", *this, binding_iter);

    os << "vars: ";

    auto first = true;
    for (auto& [var, fixed] : binding_iter.vars) {
        if (first) first = false; else os << ", ";
        if (fixed) os << "[";
        os << "?" << get_query_ctx().get_var_name(var);
        if (fixed) os << "]";
    }

    os << "\n";

    for (size_t i = 0; i < binding_iter.values.size(); i += binding_iter.vars.size()) {
        os << std::string(indent, ' ') << '(';
        for (size_t j = 0; j < binding_iter.vars.size(); j++) {
            if (j != 0) os << ", ";
            os << binding_iter.values[i + j];
        }
        os << ")\n";
    }

    os << std::string(indent - 2, ' ') << ")\n";
}


void BindingIterPrinter::visit(CrossProduct& binding_iter) {
    auto helper = BindingIterPrinterHelper("CrossProduct", *this, binding_iter);
    os << ")\n";
    binding_iter.lhs->accept_visitor(*this);
    binding_iter.rhs->accept_visitor(*this);
}


void BindingIterPrinter::visit(HashJoin::MaterializeIter& binding_iter) {
    auto helper = BindingIterPrinterHelper("HashJoin::MaterializeIter", *this, binding_iter);
    os << ")\n";
}


void BindingIterPrinter::visit(HashJoin::BGP::Hybrid::Join1Var& binding_iter) {
    auto helper = BindingIterPrinterHelper("HashJoin::BGP::Hybrid::Join1Var", *this, binding_iter);
    os << "(\n";
    binding_iter.lhs->accept_visitor(*this);
    binding_iter.rhs->accept_visitor(*this);
}


void BindingIterPrinter::visit(HashJoin::BGP::Hybrid::Join<2>& binding_iter) {
    auto helper = BindingIterPrinterHelper("HashJoin::BGP::Hybrid::Join<2>", *this, binding_iter);
    os << ")\n";
    binding_iter.lhs->accept_visitor(*this);
    binding_iter.rhs->accept_visitor(*this);

}


void BindingIterPrinter::visit(HashJoin::BGP::Hybrid::Join<3>& binding_iter) {
    auto helper = BindingIterPrinterHelper("HashJoin::BGP::Hybrid::Join<3>", *this, binding_iter);
    os << ")\n";
    binding_iter.lhs->accept_visitor(*this);
    binding_iter.rhs->accept_visitor(*this);
}


void BindingIterPrinter::visit(HashJoin::BGP::Hybrid::Join<4>& binding_iter) {
    auto helper = BindingIterPrinterHelper("HashJoin::BGP::Hybrid::Join<4>", *this, binding_iter);
    os << ")\n";
    binding_iter.lhs->accept_visitor(*this);
    binding_iter.rhs->accept_visitor(*this);
}


void BindingIterPrinter::visit(HashJoin::BGP::InMemory::Join1Var& binding_iter) {
    auto helper = BindingIterPrinterHelper("HashJoin::BGP::InMemory::Join1Var", *this, binding_iter);
    os << ")\n";
    binding_iter.build_rel->accept_visitor(*this);
    binding_iter.probe_rel->accept_visitor(*this);
}


void BindingIterPrinter::visit(HashJoin::BGP::InMemory::Join<2>& binding_iter) {
    auto helper = BindingIterPrinterHelper("HashJoin::BGP::InMemory::Join<2>", *this, binding_iter);
    os << ")\n";
    binding_iter.build_rel->accept_visitor(*this);
    binding_iter.probe_rel->accept_visitor(*this);
}


void BindingIterPrinter::visit(HashJoin::BGP::InMemory::Join<3>& binding_iter) {
    auto helper = BindingIterPrinterHelper("HashJoin::BGP::InMemory::Join<3>", *this, binding_iter);
    os << ")\n";
    binding_iter.build_rel->accept_visitor(*this);
    binding_iter.probe_rel->accept_visitor(*this);
}


void BindingIterPrinter::visit(HashJoin::BGP::InMemory::Join<4>& binding_iter) {
    auto helper = BindingIterPrinterHelper("HashJoin::BGP::InMemory::Join<4>", *this, binding_iter);
    os << ")\n";
    binding_iter.build_rel->accept_visitor(*this);
    binding_iter.probe_rel->accept_visitor(*this);
}


void BindingIterPrinter::visit(HashJoin::Generic::Hybrid::AntiJoin& binding_iter) {
    auto helper = BindingIterPrinterHelper("HashJoin::Generic::Hybrid::AntiJoin", *this, binding_iter);
    os << ")\n";
    binding_iter.original_lhs->accept_visitor(*this);
    binding_iter.original_rhs->accept_visitor(*this);
}


void BindingIterPrinter::visit(HashJoin::Generic::Hybrid::Join& binding_iter) {
    auto helper = BindingIterPrinterHelper("HashJoin::Generic::Hybrid::Join", *this, binding_iter);
    os << ")\n";
    binding_iter.lhs->accept_visitor(*this);
    binding_iter.rhs->accept_visitor(*this);
}


void BindingIterPrinter::visit(HashJoin::Generic::Hybrid::LeftJoin& binding_iter) {
    std::stringstream ss;
    ss << "not nulls: " << binding_iter.found_not_nulls
       << ", nulls: " << binding_iter.found_nulls;
    auto helper = BindingIterPrinterHelper("HashJoin::Generic::Hybrid::LeftJoin", *this, binding_iter, ss.str());
    os << ")\n";
    binding_iter.original_lhs->accept_visitor(*this);
    binding_iter.original_rhs->accept_visitor(*this);
}


void BindingIterPrinter::visit(HashJoin::Generic::Hybrid::SemiJoin& binding_iter) {
    auto helper = BindingIterPrinterHelper("HashJoin::Generic::Hybrid::SemiJoin", *this, binding_iter);
    os << ")\n";
    binding_iter.original_lhs->accept_visitor(*this);
    binding_iter.original_rhs->accept_visitor(*this);
}


void BindingIterPrinter::visit(HashJoin::Generic::InMemory::AntiJoin& binding_iter) {
    auto helper = BindingIterPrinterHelper("HashJoin::Generic::InMemory::AntiJoin", *this, binding_iter);
    os << ")\n";
    binding_iter.lhs->accept_visitor(*this);
    binding_iter.rhs->accept_visitor(*this);
}


void BindingIterPrinter::visit(HashJoin::Generic::InMemory::Join& binding_iter) {
    auto helper = BindingIterPrinterHelper("HashJoin::Generic::InMemory::Join", *this, binding_iter);
    os << ")\n";
    binding_iter.lhs->accept_visitor(*this);
    binding_iter.rhs->accept_visitor(*this);
}


void BindingIterPrinter::visit(HashJoin::Generic::InMemory::LeftJoin& binding_iter) {
    std::stringstream ss;
    ss << "not nulls: " << binding_iter.found_not_nulls
       << ", nulls: " << binding_iter.found_nulls;
    auto helper = BindingIterPrinterHelper("HashJoin::Generic::InMemory::LeftJoin", *this, binding_iter, ss.str());
    os << ")\n";
    binding_iter.lhs->accept_visitor(*this);
    binding_iter.rhs->accept_visitor(*this);
}


void BindingIterPrinter::visit(HashJoin::Generic::InMemory::SemiJoin& binding_iter) {
    auto helper = BindingIterPrinterHelper("HashJoin::Generic::InMemory::SemiJoin", *this, binding_iter);
    os << ")\n";
    binding_iter.lhs->accept_visitor(*this);
    binding_iter.rhs->accept_visitor(*this);
}


void BindingIterPrinter::visit(Paths::UnfixedComposite& binding_iter) {
    std::stringstream ss;
    ss << "idx_searches: " << binding_iter.idx_searches;
    auto helper = BindingIterPrinterHelper("Paths::UnfixedComposite", *this, binding_iter, ss.str());
    os << ")\n";
    binding_iter.child_iter->accept_visitor(*this);
}


void BindingIterPrinter::visit(Paths::AllShortest::BFSCheck& binding_iter) {
    std::stringstream ss;
    ss << "idx_searches: " << binding_iter.idx_searches;
    auto helper = BindingIterPrinterHelper("Paths::AllShortest::BFSCheck", *this, binding_iter, ss.str());
    os << ")\n";
}


void BindingIterPrinter::visit(Paths::AllShortest::BFSEnum<false>& binding_iter) {
    std::stringstream ss;
    ss << "idx_searches: " << binding_iter.idx_searches;
    auto helper = BindingIterPrinterHelper("Paths::AllShortest::BFSEnum<false>", *this, binding_iter, ss.str());
    os << ")\n";
}


void BindingIterPrinter::visit(Paths::AllShortest::BFSEnum<true>& binding_iter) {
    std::stringstream ss;
    ss << "idx_searches: " << binding_iter.idx_searches;
    auto helper = BindingIterPrinterHelper("Paths::AllShortest::BFSEnum<true>", *this, binding_iter, ss.str());
    os << ")\n";
}


void BindingIterPrinter::visit(Paths::AllShortestCount::BFSCheck& binding_iter) {
    std::stringstream ss;
    ss << "idx_searches: " << binding_iter.idx_searches
       << ", paths_found: " << binding_iter.paths_found;
    auto helper = BindingIterPrinterHelper("Paths::AllShortestCount::BFSCheck", *this, binding_iter, ss.str());
    os << ")\n";
}


void BindingIterPrinter::visit(Paths::AllShortestCount::BFSEnum<false>& binding_iter) {
    std::stringstream ss;
    ss << "idx_searches: " << binding_iter.idx_searches
       << ", paths_found: " << binding_iter.paths_found;
    auto helper = BindingIterPrinterHelper("Paths::AllShortestCount::BFSEnum<false>", *this, binding_iter, ss.str());
    os << ")\n";
}


void BindingIterPrinter::visit(Paths::AllShortestCount::BFSEnum<true>& binding_iter) {
    std::stringstream ss;
    ss << "idx_searches: " << binding_iter.idx_searches
       << ", paths_found: " << binding_iter.paths_found;
    auto helper = BindingIterPrinterHelper("Paths::AllShortestCount::BFSEnum<true>", *this, binding_iter, ss.str());
    os << ")\n";
}


void BindingIterPrinter::visit(Paths::AllShortestSimple::BFSCheck<false>& binding_iter) {
    std::stringstream ss;
    ss << "idx_searches: " << binding_iter.idx_searches;
    auto helper = BindingIterPrinterHelper("Paths::AllShortestSimple::BFSCheck<ACYCLIC>", *this, binding_iter, ss.str());
    os << ")\n";
}


void BindingIterPrinter::visit(Paths::AllShortestSimple::BFSCheck<true>& binding_iter) {
    std::stringstream ss;
    ss << "idx_searches: " << binding_iter.idx_searches;
    auto helper = BindingIterPrinterHelper("Paths::AllShortestSimple::BFSCheck<SIMPLE>", *this, binding_iter, ss.str());
    os << ")\n";
}


void BindingIterPrinter::visit(Paths::AllShortestSimple::BFSEnum<false>& binding_iter) {
    std::stringstream ss;
    ss << "idx_searches: " << binding_iter.idx_searches;
    auto helper = BindingIterPrinterHelper("Paths::AllShortestSimple::BFSEnum<ACYCLIC>", *this, binding_iter, ss.str());
    os << ")\n";
}


void BindingIterPrinter::visit(Paths::AllShortestSimple::BFSEnum<true>& binding_iter) {
    std::stringstream ss;
    ss << "idx_searches: " << binding_iter.idx_searches;
    auto helper = BindingIterPrinterHelper("Paths::AllShortestSimple::BFSEnum<SIMPLE>", *this, binding_iter, ss.str());
    os << ")\n";
}


void BindingIterPrinter::visit(Paths::AllShortestTrails::BFSCheck& binding_iter) {
    std::stringstream ss;
    ss << "idx_searches: " << binding_iter.idx_searches;
    auto helper = BindingIterPrinterHelper("Paths::AllShortestTrails::BFSCheck", *this, binding_iter, ss.str());
    os << ")\n";
}


void BindingIterPrinter::visit(Paths::AllShortestTrails::BFSEnum& binding_iter) {
    std::stringstream ss;
    ss << "idx_searches: " << binding_iter.idx_searches;
    auto helper = BindingIterPrinterHelper("Paths::AllShortestTrails::BFSEnum", *this, binding_iter, ss.str());
    os << ")\n";
}


void BindingIterPrinter::visit(Paths::AllSimple::BFSCheck<false>& binding_iter) {
    std::stringstream ss;
    ss << "idx_searches: " << binding_iter.idx_searches;
    auto helper = BindingIterPrinterHelper("Paths::AllSimple::BFSCheck<ACYCLIC>", *this, binding_iter, ss.str());
    os << ")\n";
}


void BindingIterPrinter::visit(Paths::AllSimple::BFSCheck<true>& binding_iter) {
    std::stringstream ss;
    ss << "idx_searches: " << binding_iter.idx_searches;
    auto helper = BindingIterPrinterHelper("Paths::AllSimple::BFSCheck<SIMPLE>", *this, binding_iter, ss.str());
    os << ")\n";
}


void BindingIterPrinter::visit(Paths::AllSimple::BFSEnum<false>& binding_iter) {
    std::stringstream ss;
    ss << "idx_searches: " << binding_iter.idx_searches;
    auto helper = BindingIterPrinterHelper("Paths::AllSimple::BFSEnum<ACYCLIC>", *this, binding_iter, ss.str());
    os << ")\n";
}


void BindingIterPrinter::visit(Paths::AllSimple::BFSEnum<true>& binding_iter) {
    std::stringstream ss;
    ss << "idx_searches: " << binding_iter.idx_searches;
    auto helper = BindingIterPrinterHelper("Paths::AllSimple::BFSEnum<SIMPLE>", *this, binding_iter, ss.str());
    os << ")\n";
}


void BindingIterPrinter::visit(Paths::AllSimple::DFSCheck<false>& binding_iter) {
    std::stringstream ss;
    ss << "idx_searches: " << binding_iter.idx_searches;
    auto helper = BindingIterPrinterHelper("Paths::AllSimple::DFSCheck<ACYCLIC>", *this, binding_iter, ss.str());
    os << ")\n";
}


void BindingIterPrinter::visit(Paths::AllSimple::DFSCheck<true>& binding_iter) {
    std::stringstream ss;
    ss << "idx_searches: " << binding_iter.idx_searches;
    auto helper = BindingIterPrinterHelper("Paths::AllSimple::DFSCheck<SIMPLE>", *this, binding_iter, ss.str());
    os << ")\n";
}


void BindingIterPrinter::visit(Paths::AllSimple::DFSEnum<false>& binding_iter) {
    std::stringstream ss;
    ss << "idx_searches: " << binding_iter.idx_searches;
    auto helper = BindingIterPrinterHelper("Paths::AllSimple::DFSEnum<ACYCLIC>", *this, binding_iter, ss.str());
    os << ")\n";
}


void BindingIterPrinter::visit(Paths::AllSimple::DFSEnum<true>& binding_iter) {
    std::stringstream ss;
    ss << "idx_searches: " << binding_iter.idx_searches;
    auto helper = BindingIterPrinterHelper("Paths::AllSimple::DFSEnum<SIMPLE>", *this, binding_iter, ss.str());
    os << ")\n";
}


void BindingIterPrinter::visit(Paths::AllTrails::BFSCheck& binding_iter) {
    std::stringstream ss;
    ss << "idx_searches: " << binding_iter.idx_searches;
    auto helper = BindingIterPrinterHelper("Paths::AllTrails::BFSCheck", *this, binding_iter, ss.str());
    os << ")\n";
}


void BindingIterPrinter::visit(Paths::AllTrails::BFSEnum& binding_iter) {
    std::stringstream ss;
    ss << "idx_searches: " << binding_iter.idx_searches;
    auto helper = BindingIterPrinterHelper("Paths::AllTrails::BFSEnum", *this, binding_iter, ss.str());
    os << ")\n";
}


void BindingIterPrinter::visit(Paths::AllTrails::DFSCheck& binding_iter) {
    std::stringstream ss;
    ss << "idx_searches: " << binding_iter.idx_searches;
    auto helper = BindingIterPrinterHelper("Paths::AllTrails::DFSCheck", *this, binding_iter, ss.str());
    os << ")\n";
}


void BindingIterPrinter::visit(Paths::AllTrails::DFSEnum& binding_iter) {
    std::stringstream ss;
    ss << "idx_searches: " << binding_iter.idx_searches;
    auto helper = BindingIterPrinterHelper("Paths::AllTrails::DFSEnum", *this, binding_iter, ss.str());
    os << ")\n";
}


void BindingIterPrinter::visit(Paths::Any::DFSCheck& binding_iter) {
    std::stringstream ss;
    ss << "idx_searches: " << binding_iter.idx_searches;
    auto helper = BindingIterPrinterHelper("Paths::Any::DFSCheck", *this, binding_iter, ss.str());
    os << ")\n";
}


void BindingIterPrinter::visit(Paths::Any::DFSEnum<false>& binding_iter) {
    std::stringstream ss;
    ss << "idx_searches: " << binding_iter.idx_searches;
    auto helper = BindingIterPrinterHelper("Paths::Any::DFSEnum<false>", *this, binding_iter, ss.str());
    os << ")\n";
}


void BindingIterPrinter::visit(Paths::Any::DFSEnum<true>& binding_iter) {
    std::stringstream ss;
    ss << "idx_searches: " << binding_iter.idx_searches;
    auto helper = BindingIterPrinterHelper("Paths::Any::DFSEnum<true>", *this, binding_iter, ss.str());
    os << ")\n";
}


void BindingIterPrinter::visit(Paths::Any::BFS_RDPQCheck& binding_iter) {
    std::stringstream ss;
    ss << "idx_searches: " << binding_iter.idx_searches;
    auto helper = BindingIterPrinterHelper("Paths::Any::BFS_RDPQCheck", *this, binding_iter, ss.str());
    os << ")\n";
}


void BindingIterPrinter::visit(Paths::Any::BFS_RDPQEnum& binding_iter) {
    std::stringstream ss;
    ss << "idx_searches: " << binding_iter.idx_searches;
    auto helper = BindingIterPrinterHelper("Paths::Any::BFS_RDPQEnum", *this, binding_iter, ss.str());
    os << ")\n";
}


void BindingIterPrinter::visit(Paths::Any::DijkstraCheck& binding_iter) {
    std::stringstream ss;
    ss << "idx_searches: " << binding_iter.idx_searches;
    auto helper = BindingIterPrinterHelper("Paths::Any::DijkstraCheck", *this, binding_iter, ss.str());
    os << ")\n";
}


void BindingIterPrinter::visit(Paths::Any::DijkstraEnum& binding_iter) {
    std::stringstream ss;
    ss << "idx_searches: " << binding_iter.idx_searches;
    auto helper = BindingIterPrinterHelper("Paths::Any::DijkstraEnum", *this, binding_iter, ss.str());
    os << ")\n";
}


void BindingIterPrinter::visit(Paths::Any::BFSCheck& binding_iter) {
    std::stringstream ss;
    ss << "idx_searches: " << binding_iter.idx_searches;
    auto helper = BindingIterPrinterHelper("Paths::Any::BFSCheck", *this, binding_iter, ss.str());
    os << ")\n";
}


void BindingIterPrinter::visit(Paths::Any::BFSEnum<false>& binding_iter) {
    std::stringstream ss;
    ss << "idx_searches: " << binding_iter.idx_searches;
    auto helper = BindingIterPrinterHelper("Paths::Any::BFSEnum<false>", *this, binding_iter, ss.str());
    os << ")\n";
}


void BindingIterPrinter::visit(Paths::Any::BFSEnum<true>& binding_iter) {
    std::stringstream ss;
    ss << "idx_searches: " << binding_iter.idx_searches;
    auto helper = BindingIterPrinterHelper("Paths::Any::BFSEnum<true>", *this, binding_iter, ss.str());
    os << ")\n";
}

void BindingIterPrinter::visit(Paths::Any::BFSMultipleStarts<false>& binding_iter) {
    std::stringstream ss;
    ss << "idx_searches: " << binding_iter.idx_searches;
    auto helper = BindingIterPrinterHelper("Paths::Any::BFSMultipleStarts<false>", *this, binding_iter, ss.str());
    os << ")\n";
}


void BindingIterPrinter::visit(Paths::Any::BFSMultipleStartsNaive<true>& binding_iter) {
    std::stringstream ss;
    ss << "idx_searches: " << binding_iter.idx_searches;
    auto helper = BindingIterPrinterHelper("Paths::Any::BFSMultipleStartsNaive<true>", *this, binding_iter, ss.str());
    os << ")\n";
}

void BindingIterPrinter::visit(Paths::Any::BFSMultipleStartsNaive<false>& binding_iter) {
    std::stringstream ss;
    ss << "idx_searches: " << binding_iter.idx_searches;
    auto helper = BindingIterPrinterHelper("Paths::Any::BFSMultipleStartsNaive<false>", *this, binding_iter, ss.str());
    os << ")\n";
}


void BindingIterPrinter::visit(Paths::Any::BFSMultipleStarts<true>& binding_iter) {
    std::stringstream ss;
    ss << "idx_searches: " << binding_iter.idx_searches;
    auto helper = BindingIterPrinterHelper("Paths::Any::BFSMultipleStarts<true>", *this, binding_iter, ss.str());
    os << ")\n";
}
void BindingIterPrinter::visit(Paths::AnySimple::BFSCheck<false>& binding_iter) {
    std::stringstream ss;
    ss << "idx_searches: " << binding_iter.idx_searches;
    auto helper = BindingIterPrinterHelper("Paths::AnySimple::BFSCheck<ACYCLIC>", *this, binding_iter, ss.str());
    os << ")\n";
}


void BindingIterPrinter::visit(Paths::AnySimple::BFSCheck<true>& binding_iter) {
    std::stringstream ss;
    ss << "idx_searches: " << binding_iter.idx_searches;
    auto helper = BindingIterPrinterHelper("Paths::AnySimple::BFSCheck<SIMPLE>", *this, binding_iter, ss.str());
    os << ")\n";
}


void BindingIterPrinter::visit(Paths::AnySimple::BFSEnum<false>& binding_iter) {
    std::stringstream ss;
    ss << "idx_searches: " << binding_iter.idx_searches;
    auto helper = BindingIterPrinterHelper("Paths::AnySimple::BFSEnum<ACYCLIC>", *this, binding_iter, ss.str());
    os << ")\n";
}


void BindingIterPrinter::visit(Paths::AnySimple::BFSEnum<true>& binding_iter) {
    std::stringstream ss;
    ss << "idx_searches: " << binding_iter.idx_searches;
    auto helper = BindingIterPrinterHelper("Paths::AnySimple::BFSEnum<SIMPLE>", *this, binding_iter, ss.str());
    os << ")\n";
}


void BindingIterPrinter::visit(Paths::AnySimple::DFSCheck<false>& binding_iter) {
    std::stringstream ss;
    ss << "idx_searches: " << binding_iter.idx_searches;
    auto helper = BindingIterPrinterHelper("Paths::AnySimple::DFSCheck<ACYCLIC>", *this, binding_iter, ss.str());
    os << ")\n";
}


void BindingIterPrinter::visit(Paths::AnySimple::DFSCheck<true>& binding_iter) {
    std::stringstream ss;
    ss << "idx_searches: " << binding_iter.idx_searches;
    auto helper = BindingIterPrinterHelper("Paths::AnySimple::DFSCheck<SIMPLE>", *this, binding_iter, ss.str());
    os << ")\n";
}


void BindingIterPrinter::visit(Paths::AnySimple::DFSEnum<false>& binding_iter) {
    std::stringstream ss;
    ss << "idx_searches: " << binding_iter.idx_searches;
    auto helper = BindingIterPrinterHelper("Paths::AnySimple::DFSEnum<ACYCLIC>", *this, binding_iter, ss.str());
    os << ")\n";
}


void BindingIterPrinter::visit(Paths::AnySimple::DFSEnum<true>& binding_iter) {
    std::stringstream ss;
    ss << "idx_searches: " << binding_iter.idx_searches;
    auto helper = BindingIterPrinterHelper("Paths::AnySimple::DFSEnum<SIMPLE>", *this, binding_iter, ss.str());
    os << ")\n";
}


void BindingIterPrinter::visit(Paths::AnyTrails::BFSCheck& binding_iter) {
    std::stringstream ss;
    ss << "idx_searches: " << binding_iter.idx_searches;
    auto helper = BindingIterPrinterHelper("Paths::AnyTrails::BFSCheck", *this, binding_iter, ss.str());
    os << ")\n";
}


void BindingIterPrinter::visit(Paths::AnyTrails::BFSEnum& binding_iter) {
    std::stringstream ss;
    ss << "idx_searches: " << binding_iter.idx_searches;
    auto helper = BindingIterPrinterHelper("Paths::AnyTrails::BFSEnum", *this, binding_iter, ss.str());
    os << ")\n";
}


void BindingIterPrinter::visit(Paths::AnyTrails::DFSCheck& binding_iter) {
    std::stringstream ss;
    ss << "idx_searches: " << binding_iter.idx_searches;
    auto helper = BindingIterPrinterHelper("Paths::AnyTrails::DFSCheck", *this, binding_iter, ss.str());
    os << ")\n";
}


void BindingIterPrinter::visit(Paths::AnyTrails::DFSEnum& binding_iter) {
    std::stringstream ss;
    ss << "idx_searches: " << binding_iter.idx_searches;
    auto helper = BindingIterPrinterHelper("Paths::AnyTrails::DFSEnum", *this, binding_iter, ss.str());
    os << ")\n";
}


void BindingIterPrinter::visit(LSH::ForestIndexTopK& binding_iter) {
    auto helper = BindingIterPrinterHelper("ForestIndexTopK", *this, binding_iter);
    os << "object: ?" << get_query_ctx().get_var_name(binding_iter.object_var);
    os << ", similarity: ?" << get_query_ctx().get_var_name(binding_iter.similarity_var);
    os << ", query_tensor.size(): " << binding_iter.query_tensor.size();
    os << ", k: " << binding_iter.k;
    os << ", search_k" << binding_iter.search_k;
    os << ", tensor_store: " << binding_iter.tensor_store.name;
    os << ")\n";
}


void BindingIterPrinter::visit(LSH::ForestIndexTopAll& binding_iter) {
    auto helper = BindingIterPrinterHelper("ForestIndexTopAll", *this, binding_iter);
    os << "object: ?" << get_query_ctx().get_var_name(binding_iter.object_var);
    os << ", similarity: ?" << get_query_ctx().get_var_name(binding_iter.similarity_var);
    os << ")\n";
}


void BindingIterPrinter::visit(LSH::ProjectTensorSimilarity& binding_iter) {
    auto helper = BindingIterPrinterHelper("ProjectTensorSimilarity", *this, binding_iter);
    os << "object: ?" << get_query_ctx().get_var_name(binding_iter.object_var);
    os << ", similarity: ?" << get_query_ctx().get_var_name(binding_iter.similarity_var);
    os << ", tensor_store: " << binding_iter.tensor_store.name;
    os << ", metric: ";
    switch (binding_iter.metric_type) {
    case LSH::MetricType::ANGULAR:
        os << "Angular";
        break;
    case LSH::MetricType::EUCLIDEAN:
        os << "Euclidean";
        break;
    case LSH::MetricType::MANHATTAN:
        os << "Manhattan";
        break;
    default:
        os << "unknown";
        break;
    }
    os << ")\n";
    binding_iter.child_iter->accept_visitor(*this);
}


void BindingIterPrinter::visit(LSH::BruteSimilaritySearch& binding_iter) {
    auto helper = BindingIterPrinterHelper("BruteSimilaritySearch", *this, binding_iter);
    os << "object: ?" << get_query_ctx().get_var_name(binding_iter.object_var);
    os << ", similarity: ?" << get_query_ctx().get_var_name(binding_iter.similarity_var);
    os << ", tensor_store: " << binding_iter.tensor_store.name;
    os << ", metric: ";
    switch (binding_iter.metric_type) {
    case LSH::MetricType::ANGULAR:
        os << "Angular";
        break;
    case LSH::MetricType::EUCLIDEAN:
        os << "Euclidean";
        break;
    case LSH::MetricType::MANHATTAN:
        os << "Manhattan";
        break;
    default:
        os << "unknown";
        break;
    }
    os << ", k: " << binding_iter.k;
    os << ")\n";
    binding_iter.child_iter->accept_visitor(*this);
}
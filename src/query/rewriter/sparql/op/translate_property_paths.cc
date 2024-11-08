#include "translate_property_paths.h"

#include "graph_models/rdf_model/conversions.h"
#include "query/parser/op/sparql/ops.h"
#include "query/parser/paths/path_atom.h"
#include "query/parser/paths/path_sequence.h"
#include "query/parser/paths/regular_path_expr.h"

#include <memory>

using namespace SPARQL;

void TranslatePropertyPaths::translate(OpPath&                op_path,
                                       std::vector<OpTriple>& new_triples,
                                       std::vector<OpPath>&   new_paths) {
    // Stack containing tuples { from, to, path }
    std::vector<std::tuple<Id, Id, std::unique_ptr<RegularPathExpr>, VarId>> stack;
    stack.emplace_back(op_path.subject, op_path.object, std::move(op_path.path), op_path.var);

    while (!stack.empty()) {
        auto current = std::move(stack.back());
        stack.pop_back();
        switch (std::get<2>(current)->type()) {
        case PathType::PATH_ATOM: {
            auto tmp = dynamic_cast<PathAtom*>(std::get<2>(current).get());
            auto iri = Conversions::pack_iri(tmp->atom);
            if (tmp->inverse) {
                // X inv(iri) Y => Y iri X
                new_triples.emplace_back(std::get<1>(current), iri, std::get<0>(current));
            } else {
                // X link(iri) Y => X iri Y
                new_triples.emplace_back(std::get<0>(current), iri, std::get<1>(current));
            }
            break;
        }
        case PathType::PATH_SEQUENCE: {
            if (!TRANSFORM_SEQUENCE) {
                new_paths.emplace_back(op_path.var,
                                       std::get<0>(current),
                                       std::get<1>(current),
                                       op_path.semantic,
                                       std::move(std::get<2>(current)));
                break;
            }
            // X seq(P, Q) Y => X P ?_V . ?_V Q Y .
            // Optimized as X seq(A1, A2, ..., An) Y => X A1 ?_V1 . ?_V1 A2 ?_V2 . ... ?_Vn-1 An Y .
            auto tmp = dynamic_cast<PathSequence*>(std::get<2>(current).get());
            // X A1 ?_V1 .
            Id from = std::get<0>(current);
            Id to   = get_query_ctx().get_internal_var();
            stack.emplace_back(from, to, std::move(tmp->sequence.front()), std::get<3>(current));
            // ?_V1 A2 ?_V2 . ...
            for (size_t i = 1; i < tmp->sequence.size() - 1; ++i) {
                from = to;
                to   = get_query_ctx().get_internal_var();
                stack.emplace_back(from, to, std::move(tmp->sequence[i]), get_query_ctx().get_internal_var());
            }
            // ?_Vn-1 An Y .
            from = to;
            to   = std::get<1>(current);
            stack.emplace_back(from, to, std::move(tmp->sequence.back()), get_query_ctx().get_internal_var());
            break;
        }
        default: {
            // X P Y => Path(X, P, Y) .
            new_paths.emplace_back(std::get<3>(current),
                                   std::get<0>(current),
                                   std::get<1>(current),
                                   op_path.semantic,
                                   std::move(std::get<2>(current)));
            break;
        }
        }
    }
}

void TranslatePropertyPaths::visit(OpAsk& op_ask) {
    op_ask.op->accept_visitor(*this);
}

void TranslatePropertyPaths::visit(OpBasicGraphPattern& op_basic_graph_pattern) {
    auto it = op_basic_graph_pattern.paths.begin();

    std::vector<OpTriple> new_triples;
    std::vector<OpPath>   new_paths;
    while (it != op_basic_graph_pattern.paths.end()) {
        // Apply translation rules only when there is no path capture variable and it has the default semantic
        if (get_query_ctx().is_internal((*it).var) && (*it).semantic == PathSemantic::DEFAULT) {
            translate((*it), new_triples, new_paths);
            it = op_basic_graph_pattern.paths.erase(it);
        } else {
            ++it;
        }
    }
    // Add the new triples and paths
    op_basic_graph_pattern.triples.insert(op_basic_graph_pattern.triples.end(),
                                          std::make_move_iterator(new_triples.begin()),
                                          std::make_move_iterator(new_triples.end()));
    op_basic_graph_pattern.paths.insert(op_basic_graph_pattern.paths.end(),
                                        std::make_move_iterator(new_paths.begin()),
                                        std::make_move_iterator(new_paths.end()));
}

void TranslatePropertyPaths::visit(OpBind& op_bind) {
    op_bind.op->accept_visitor(*this);
}

void TranslatePropertyPaths::visit(OpConstruct& op_construct) {
    op_construct.op->accept_visitor(*this);
}

void TranslatePropertyPaths::visit(OpDescribe& op_describe) {
    if (op_describe.op) {
        op_describe.op->accept_visitor(*this);
    }
}

void TranslatePropertyPaths::visit(OpEmpty& op_empty) {
    if (op_empty.deleted_op.has_value()) {
        op_empty.deleted_op.value()->accept_visitor(*this);
    }
}

void TranslatePropertyPaths::visit(OpFilter& op_filter) {
    op_filter.op->accept_visitor(*this);

    TranslatePropertyPathsExpr expr_visitor(*this);
    for (auto& expr : op_filter.filters) {
        expr->accept_visitor(expr_visitor);
    }
}

void TranslatePropertyPaths::visit(OpGraph& op_graph) {
    op_graph.op->accept_visitor(*this);
}

void TranslatePropertyPaths::visit(OpGroupBy& op_group_by) {
    op_group_by.op->accept_visitor(*this);
}

void TranslatePropertyPaths::visit(OpHaving& op_having) {
    op_having.op->accept_visitor(*this);
}

void TranslatePropertyPaths::visit(OpJoin& op_join) {
    op_join.lhs->accept_visitor(*this);
    op_join.rhs->accept_visitor(*this);
}

void TranslatePropertyPaths::visit(OpMinus& op_minus) {
    op_minus.lhs->accept_visitor(*this);
    op_minus.rhs->accept_visitor(*this);
}


void TranslatePropertyPaths::visit(OpNotExists& op_not_exists) {
    op_not_exists.lhs->accept_visitor(*this);
    op_not_exists.rhs->accept_visitor(*this);
}

void TranslatePropertyPaths::visit(OpOptional& op_optional) {
    op_optional.lhs->accept_visitor(*this);
    op_optional.rhs->accept_visitor(*this);
}

void TranslatePropertyPaths::visit(OpOrderBy& op_order_by) {
    op_order_by.op->accept_visitor(*this);
}

void TranslatePropertyPaths::visit(OpSelect& op_select) {
    op_select.op->accept_visitor(*this);
}

void TranslatePropertyPaths::visit(OpSemiJoin& op_semi_join) {
    op_semi_join.lhs->accept_visitor(*this);
    op_semi_join.rhs->accept_visitor(*this);
}

void TranslatePropertyPaths::visit(OpSequence& op_sequence) {
    for (auto& op : op_sequence.ops) {
        op->accept_visitor(*this);
    }
}

void TranslatePropertyPaths::visit(OpService& op_service) {
    op_service.op->accept_visitor(*this);
}

void TranslatePropertyPaths::visit(OpUnion& op_union) {
    for (auto& child : op_union.unions) {
        child->accept_visitor(*this);
    }
}

///////////////////// TranslatePropertyPathsExpr /////////////////////

void TranslatePropertyPathsExpr::visit(ExprNotExists& e) {
    e.op->accept_visitor(op_visitor);
}

void TranslatePropertyPathsExpr::visit(ExprExists& e) {
    e.op->accept_visitor(op_visitor);
}

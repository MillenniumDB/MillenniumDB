#include "change_join_to_sequence.h"

#include <cassert>

#include "graph_models/rdf_model/rdf_model.h"
#include "query/parser/op/op_visitor.h"
#include "query/parser/op/sparql/op_join.h"
#include "query/parser/op/sparql/ops.h"

using namespace SPARQL;

void ChangeJoinToSequence::visit(OpBasicGraphPattern&) { }
void ChangeJoinToSequence::visit(OpEmpty&) { }
void ChangeJoinToSequence::visit(OpService&) { }
void ChangeJoinToSequence::visit(OpUnitTable&) { }
void ChangeJoinToSequence::visit(OpValues&) { }


void ChangeJoinToSequence::visit(OpOptional& op_optional)
{
    transform_child_if_necessary(op_optional.lhs);
    transform_child_if_necessary(op_optional.rhs);

    assert(!is_castable_to<OpJoin>(op_optional.lhs));
    assert(!is_castable_to<OpJoin>(op_optional.rhs));
}


void ChangeJoinToSequence::visit(OpFilter& op_filter)
{
    transform_child_if_necessary(op_filter.op);
    assert(!is_castable_to<OpJoin>(op_filter.op));
}


void ChangeJoinToSequence::visit(OpSelect& op_select)
{
    transform_child_if_necessary(op_select.op);
    assert(!is_castable_to<OpJoin>(op_select.op));
}


void ChangeJoinToSequence::visit(OpDescribe& op_describe)
{
    if (op_describe.op) {
        transform_child_if_necessary(op_describe.op);
        assert(!is_castable_to<OpJoin>(op_describe.op));
    }
}


void ChangeJoinToSequence::visit(OpConstruct& op_construct)
{
    if (op_construct.op) {
        transform_child_if_necessary(op_construct.op);
        assert(!is_castable_to<OpJoin>(op_construct.op));
    }
}


void ChangeJoinToSequence::visit(OpAsk& op_ask)
{
    transform_child_if_necessary(op_ask.op);
    assert(!is_castable_to<OpJoin>(op_ask.op));
}


void ChangeJoinToSequence::visit(OpOrderBy& op_order_by)
{
    transform_child_if_necessary(op_order_by.op);
    assert(!is_castable_to<OpJoin>(op_order_by.op));
}


void ChangeJoinToSequence::visit(OpGroupBy& op_group_by)
{
    transform_child_if_necessary(op_group_by.op);
    assert(!is_castable_to<OpJoin>(op_group_by.op));
}


void ChangeJoinToSequence::visit(OpHaving& op_having)
{
    op_having.op->accept_visitor(*this);
}


void ChangeJoinToSequence::visit(OpJoin& op_join)
{
    // potential_sequences.back().is_from_join = true;
    op_join.lhs->accept_visitor(*this);
    op_join.rhs->accept_visitor(*this);
    add_op_to_sequence_information(std::move(op_join.lhs));
    add_op_to_sequence_information(std::move(op_join.rhs));
}


void ChangeJoinToSequence::visit(OpSemiJoin& op_semi_join)
{
    transform_child_if_necessary(op_semi_join.lhs);
    transform_child_if_necessary(op_semi_join.rhs);
    assert(!is_castable_to<OpJoin>(op_semi_join.lhs));
    assert(!is_castable_to<OpJoin>(op_semi_join.rhs));
}


void ChangeJoinToSequence::visit(OpMinus& op_minus)
{
    transform_child_if_necessary(op_minus.lhs);
    transform_child_if_necessary(op_minus.rhs);
    assert(!is_castable_to<OpJoin>(op_minus.lhs));
    assert(!is_castable_to<OpJoin>(op_minus.rhs));
}


void ChangeJoinToSequence::visit(OpNotExists& op_not_exists)
{
    transform_child_if_necessary(op_not_exists.lhs);
    transform_child_if_necessary(op_not_exists.rhs);
    assert(!is_castable_to<OpJoin>(op_not_exists.lhs));
    assert(!is_castable_to<OpJoin>(op_not_exists.rhs));
}


void ChangeJoinToSequence::visit(OpUnion& op_union)
{
    for (auto& child : op_union.unions) {
        transform_child_if_necessary(child);
    }
}


void ChangeJoinToSequence::transform_child_if_necessary(std::unique_ptr<Op>& op)
{
    potential_sequences.push_back({});
    op->accept_visitor(*this);
    auto opt_sequence = get_pertinent_sequence();
    if (opt_sequence.has_value()) {
        op = std::move(opt_sequence.value());
    }
}


void ChangeJoinToSequence::add_op_to_sequence_information(std::unique_ptr<Op> op)
{
    auto& sequence_info = potential_sequences.back();
    if (is_castable_to<OpBasicGraphPattern>(op)) {
        auto bgp = cast_to<OpBasicGraphPattern>(std::move(op));
        auto& sequence_info = potential_sequences.back();
        if (sequence_info.bgp.has_value()) {
            auto& original_bgp = sequence_info.bgp.value();
            original_bgp->triples.insert(
                original_bgp->triples.end(),
                make_move_iterator(bgp->triples.begin()),
                make_move_iterator(bgp->triples.end())
            );
            original_bgp->paths.insert(
                original_bgp->paths.end(),
                make_move_iterator(bgp->paths.begin()),
                make_move_iterator(bgp->paths.end())
            );
        } else {
            sequence_info.bgp = std::move(bgp);
        }
    } else if (is_castable_to<OpService>(op)) {
        sequence_info.services.push_back(cast_to<OpService>(std::move(op)));
    } else if (is_castable_to<OpBind>(op)) {
        sequence_info.op_binds.push_back(cast_to<OpBind>(std::move(op)));
    } else if (is_castable_to<OpValues>(op)) {
        sequence_info.op_values.push_back(cast_to<OpValues>(std::move(op)));
    } else if (!is_castable_to<OpJoin>(op)) {
        sequence_info.other_ops.push_back(std::move(op));
    }
}


void ChangeJoinToSequence::visit(OpBind& op_bind)
{
    transform_child_if_necessary(op_bind.op);
    assert(!is_castable_to<OpJoin>(op_bind.op));

    auto& last_potential_sequence = potential_sequences.back();

    if (last_potential_sequence.bgp.has_value()) {
        auto last_bgp = std::move(last_potential_sequence.bgp.value());
        last_potential_sequence.bgp.reset();
        last_potential_sequence.other_ops.push_back(std::move(last_bgp));
    }

    auto& other_ops = last_potential_sequence.other_ops;
    other_ops.insert(
        other_ops.end(),
        make_move_iterator(last_potential_sequence.services.begin()),
        make_move_iterator(last_potential_sequence.services.end())
    );

    last_potential_sequence.services.clear();
}


std::optional<std::unique_ptr<Op>> ChangeJoinToSequence::get_pertinent_sequence()
{
    SequenceInformation potential_sequence = std::move(potential_sequences.back());
    potential_sequences.pop_back();

    std::vector<std::unique_ptr<Op>> sequence;

    for (auto& op : potential_sequence.op_binds) {
        sequence.push_back(std::move(op));
    }

    // there is a border case where the optimization of ReplaceSingleValues
    // in a empty database with nullable property paths would give an incorrect number of results
    if (rdf_model.catalog.get_triples_count() > 0) {
        for (auto& op : potential_sequence.op_values) {
            sequence.push_back(std::move(op));
        }

        if (potential_sequence.bgp.has_value()) {
            sequence.push_back(std::move(potential_sequence.bgp.value()));
        }
    } else {
        if (potential_sequence.bgp.has_value()) {
            sequence.push_back(std::move(potential_sequence.bgp.value()));
        }

        for (auto& op : potential_sequence.op_values) {
            sequence.push_back(std::move(op));
        }
    }

    for (auto& op : potential_sequence.other_ops) {
        sequence.push_back(std::move(op));
    }

    for (auto& op : potential_sequence.services) {
        sequence.push_back(std::move(op));
    }

    if (sequence.empty()) {
        return {};
    } else if (sequence.size() == 1) {
        return std::move(sequence[0]);
    } else {
        return std::make_unique<OpSequence>(std::move(sequence));
    }
}

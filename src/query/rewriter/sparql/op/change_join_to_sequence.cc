#include "change_join_to_sequence.h"

#include <cassert>

#include "query/parser/op/sparql/op_join.h"
#include "query/parser/op/sparql/op_visitor.h"
#include "query/parser/op/sparql/ops.h"

using namespace SPARQL;

void ChangeJoinToSequence::visit(OpBasicGraphPattern&) { }
void ChangeJoinToSequence::visit(OpEmpty&) { }
void ChangeJoinToSequence::visit(OpProcedure&) { }
void ChangeJoinToSequence::visit(OpService&) { }
void ChangeJoinToSequence::visit(OpUnitTable&) { }
void ChangeJoinToSequence::visit(OpValues&) { }
void ChangeJoinToSequence::visit(OpShow&) { }

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

void ChangeJoinToSequence::visit(OpFrom& op_from)
{
    transform_child_if_necessary(op_from.op);
    assert(!is_castable_to<OpJoin>(op_from.op));
}

void ChangeJoinToSequence::visit(OpGraph& op_graph)
{
    transform_child_if_necessary(op_graph.op);
    assert(!is_castable_to<OpJoin>(op_graph.op));
}

void ChangeJoinToSequence::visit(OpHaving& op_having)
{
    transform_child_if_necessary(op_having.op);
    assert(!is_castable_to<OpJoin>(op_having.op));
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
        assert(!is_castable_to<OpJoin>(child));
    }
}

void ChangeJoinToSequence::visit(OpBind& op_bind)
{
    transform_child_if_necessary(op_bind.op);
    assert(!is_castable_to<OpJoin>(op_bind.op));
}

void ChangeJoinToSequence::visit(OpJoin&)
{
    assert(false);
}

void ChangeJoinToSequence::transform_child_if_necessary(std::unique_ptr<Op>& op)
{
    if (is_castable_to<OpJoin>(op)) {
        auto join = cast_to<OpJoin>(std::move(op));
        transform_child_if_necessary(join->lhs);
        transform_child_if_necessary(join->rhs);

        std::vector<std::unique_ptr<Op>> sequence;

        try_merge(sequence, std::move(join->lhs));
        try_merge(sequence, std::move(join->rhs));

        if (sequence.size() > 1) {
            op = std::make_unique<OpSequence>(std::move(sequence));
        } else if (sequence.size() == 1) {
            op = std::move(sequence[0]);
        } else {
            assert(false);
            op = std::make_unique<OpUnitTable>();
        }
    } else {
        op->accept_visitor(*this);
    }
}
void ChangeJoinToSequence::try_merge(std::vector<std::unique_ptr<Op>>& sequence, std::unique_ptr<Op> op)
{
    if (is_castable_to<OpSequence>(op)) {
        auto rhs_sequence = cast_to<OpSequence>(std::move(op));

        for (auto& e : rhs_sequence->ops) {
            try_merge(sequence, std::move(e));
        }
    } else if (is_castable_to<OpBasicGraphPattern>(op)) {
        auto rhs_bgp = cast_to<OpBasicGraphPattern>(std::move(op));
        if (sequence.empty()) {
            sequence.push_back(std::move(rhs_bgp));
            return;
        }

        if (auto last_bgp = dynamic_cast<OpBasicGraphPattern*>(sequence.back().get())) {
            last_bgp->merge(std::move(rhs_bgp));
        } else {
            sequence.push_back(std::move(rhs_bgp));
        }
    } else {
        sequence.push_back(std::move(op));
    }
}

#pragma once

#include <memory>
#include <optional>
#include <set>
#include <vector>

#include "query/parser/op/op.h"
#include "query/parser/op/op_visitor.h"

namespace SPARQL {
/*
  Create sequence objects that have the structure for Op:
    BGP? Op* service*
  and rewrite all BGP JOIN BGP as BGP, concatenating all of their
  children.
*/
class ChangeJoinToSequence : public OpVisitor {
private:
    struct SequenceInformation {
        std::optional<std::unique_ptr<OpBasicGraphPattern>> last_bgp;
        std::vector<std::unique_ptr<Op>>                    ops;
        std::vector<std::unique_ptr<OpService>>             services;
        bool                                                is_from_join = false;
        bool                                                visited_bind = false;
    };
    std::vector<SequenceInformation> potential_sequences;

// IMPORTANT:
// If you create a new visit functions for a new OP you probably need to use
// transform_child_if_necessary instead of op.child.visit(*this)

public:
    void visit(OpConstruct&)            override;
    void visit(OpDescribe&)             override;
    void visit(OpBasicGraphPattern&)    override;
    void visit(OpFilter&)               override;
    void visit(OpJoin&)                 override;
    void visit(OpSemiJoin&)             override;
    void visit(OpEmpty&)                override;
    void visit(OpMinus&)                override;
    void visit(OpNotExists&)            override;
    void visit(OpUnion&)                override;
    void visit(OpOptional&)             override;
    void visit(OpOrderBy&)              override;
    void visit(OpGroupBy&)              override;
    void visit(OpHaving&)               override;
    void visit(OpSelect&)               override;
    void visit(OpAsk&)                  override;
    void visit(OpService&)              override;
    void visit(OpWhere&)                override;
    void visit(OpBind&)                 override;
    void visit(OpUnitTable&)            override;
    void visit(OpValues&)               override;

private:
    template<typename T>
    bool is_castable_to(std::unique_ptr<Op>& unknown_op) {
        if (dynamic_cast<T*>(unknown_op.get()) != nullptr) {
            return true;
        }
        return false;
    }

    template<typename T>
    std::unique_ptr<T> cast_to(std::unique_ptr<Op> unknown_op) {
        std::unique_ptr<T> casted_optional(dynamic_cast<T*>(unknown_op.release()));
        return casted_optional;
    }

    void add_op_to_sequence_information(std::unique_ptr<Op> op);
    void add_basic_graph_pattern(std::unique_ptr<OpBasicGraphPattern> op);
    void transform_child_if_necessary(std::unique_ptr<Op>& op);

    std::optional<std::unique_ptr<Op>> get_pertinent_sequence();
};
} // namespace SPARQL

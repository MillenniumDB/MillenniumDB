#pragma once

#include <memory>
#include <optional>
#include <vector>

#include "query/parser/op/op.h"
#include "query/parser/op/op_visitor.h"

namespace SPARQL {
/*
  Create sequence objects that have the structure for Op:
    BIND* VALUE* BGP? Op* service*
  and rewrite all BGP JOIN BGP as BGP, concatenating all of their
  children.
*/
class ChangeJoinToSequence : public OpVisitor {
private:
    struct SequenceInformation {
        std::vector<std::unique_ptr<OpBind>> op_binds;

        std::vector<std::unique_ptr<OpValues>> op_values;

        std::optional<std::unique_ptr<OpBasicGraphPattern>> bgp;

        std::vector<std::unique_ptr<Op>> other_ops;

        std::vector<std::unique_ptr<OpService>> services;
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
    void visit(OpFrom&)                 override;
    void visit(OpGraph&)                override;
    void visit(OpGroupBy&)              override;
    void visit(OpHaving&)               override;
    void visit(OpSelect&)               override;
    void visit(OpAsk&)                  override;
    void visit(OpService&)              override;
    void visit(OpBind&)                 override;
    void visit(OpUnitTable&)            override;
    void visit(OpValues&)               override;
    void visit(OpShow&)                 override;

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

    void transform_child_if_necessary(std::unique_ptr<Op>& op);

    std::optional<std::unique_ptr<Op>> get_pertinent_sequence();
};
} // namespace SPARQL

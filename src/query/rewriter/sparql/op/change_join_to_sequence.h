#pragma once

#include <memory>

#include "query/parser/op/sparql/op.h"
#include "query/parser/op/sparql/op_visitor.h"

namespace SPARQL {

// Rewrite all Op1 JOIN Op2 as sequences, concatenating their basic graph patterns if possible.
// IMPORTANT:
// If you create a new visit functions for a new OP you probably need to use
// transform_child_if_necessary instead of accept_visitor
class ChangeJoinToSequence : public OpVisitor {
public:
    void visit(OpConstruct&) override;
    void visit(OpDescribe&) override;
    void visit(OpBasicGraphPattern&) override;
    void visit(OpProcedure&) override;
    void visit(OpFilter&) override;
    void visit(OpJoin&) override;
    void visit(OpSemiJoin&) override;
    void visit(OpEmpty&) override;
    void visit(OpMinus&) override;
    void visit(OpNotExists&) override;
    void visit(OpUnion&) override;
    void visit(OpOptional&) override;
    void visit(OpOrderBy&) override;
    void visit(OpFrom&) override;
    void visit(OpGraph&) override;
    void visit(OpGroupBy&) override;
    void visit(OpHaving&) override;
    void visit(OpSelect&) override;
    void visit(OpAsk&) override;
    void visit(OpService&) override;
    void visit(OpBind&) override;
    void visit(OpUnitTable&) override;
    void visit(OpValues&) override;
    void visit(OpShow&) override;

    void visit(OpPath&) override { }
    void visit(OpTriple&) override { }
    void visit(OpSequence&) override { }

    void visit(OpCreateHNSWIndex&) override { }
    void visit(OpCreateTextIndex&) override { }
    void visit(OpDeleteData&) override { }
    void visit(OpInsertData&) override { }
    void visit(OpUpdate&) override { }

private:
    template<typename T>
    static bool is_castable_to(std::unique_ptr<Op>& unknown_op)
    {
        if (dynamic_cast<T*>(unknown_op.get()) != nullptr) {
            return true;
        }
        return false;
    }

    template<typename T>
    static std::unique_ptr<T> cast_to(std::unique_ptr<Op> unknown_op)
    {
        std::unique_ptr<T> casted_optional(dynamic_cast<T*>(unknown_op.release()));
        return casted_optional;
    }

    void transform_child_if_necessary(std::unique_ptr<Op>& op);

    static void try_merge(std::vector<std::unique_ptr<Op>>& sequence, std::unique_ptr<Op> op);
};
} // namespace SPARQL

#pragma once

#include <map>

#include "graph_models/object_id.h"
#include "query/parser/op/gql/op_visitor.h"
#include "query/var_id.h"

namespace GQL {
class ReplaceParameters : public OpVisitor {
public:
    ReplaceParameters(const std::map<VarId, ObjectId>& parameters_) :
        parameters { parameters_ }
    { }

    void visit(OpGraphPattern& op) override;
    void visit(OpBasicGraphPattern& op) override;
    void visit(OpGraphPatternList& op) override;
    void visit(OpNode& op) override;
    void visit(OpEdge& op) override;
    void visit(OpWhere& op) override;
    void visit(OpReturn& op) override;
    void visit(OpPathUnion& op) override;
    void visit(OpRepetition& op) override;
    void visit(OpLinearPattern& op) override;
    void visit(OpFilter& op) override;
    void visit(OpLet& op) override;
    void visit(OpOrderBy& op) override;
    void visit(OpQueryStatements& op) override;
    void visit(OpGroupBy& op) override;
    void visit(OpUnitTable&) override;
    void visit(OpEmpty&) override;

private:
    const std::map<VarId, ObjectId>& parameters;
};

} // namespace GQL

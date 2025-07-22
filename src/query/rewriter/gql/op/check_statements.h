#pragma once

#include "query/parser/op/gql/ops.h"

namespace GQL {

class CheckStatements : public OpVisitor {
private:
    bool has_match_or_let = false;

public:
    void visit(OpQueryStatements&) override;
    void visit(OpReturn&) override;
    void visit(OpFilter&) override;
    void visit(OpFilterStatement&) override;
    void visit(OpGraphPatternList&) override;
    void visit(OpLet&) override;
    void visit(OpOrderBy&) override;
    void visit(OpOrderByStatement&) override;
};
} // namespace GQL

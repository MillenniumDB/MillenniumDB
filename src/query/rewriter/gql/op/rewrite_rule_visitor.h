#pragma once

#include <memory>

#include "query/parser/op/op_visitor.h"
#include "query/rewriter/gql/op/rewrite_rules/rewrite_rule.h"

namespace GQL {

class RewriteRuleVisitor : public OpVisitor {
private:
    std::vector<std::unique_ptr<RewriteRule>> rules;
    bool has_rewritten = false;

public:
    RewriteRuleVisitor() = default;

    RewriteRuleVisitor(std::vector<std::unique_ptr<RewriteRule>>&& rules) :
        rules(std::move(rules))
    { }

    template<typename Rule>
    void add_rule()
    {
        rules.push_back(std::make_unique<Rule>());
    }

    void begin_visit(std::unique_ptr<Op>& op)
    {
        do {
            has_rewritten = false;
            for (auto& rule : rules) {
                if (rule->is_possible_to_regroup(op)) {
                    op = rule->regroup(std::move(op));
                    has_rewritten = true;
                }
            }
            op->accept_visitor(*this);
        } while (has_rewritten);
    }

    void remove_rules()
    {
        rules.clear();
    }

    void visit(OpGraphPattern&) override;
    void visit(OpGraphPatternList&) override;
    void visit(OpBasicGraphPattern&) override;
    void visit(OpRepetition&) override;
    void visit(OpReturn&) override;
    void visit(OpOrderBy&) override;

    void visit(OpFilter&) override;
    void visit(OpOptProperties&) override;
    void visit(OpPathUnion&) override;
    void visit(OpNode&) override { }
    void visit(OpEdge&) override { }
    void visit(OpNodeLabel&) override { }
    void visit(OpEdgeLabel&) override { }
    void visit(OpProperty&) override { }
};
} // namespace GQL

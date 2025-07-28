#pragma once

#include <memory>

#include "query/parser/op/sparql/op_visitor.h"
#include "query/parser/op/sparql/ops.h"
#include "query/rewriter/sparql/op/rewrite_rules/rewrite_rule.h"

namespace SPARQL {
/**
    * This visitor uses a rewrite rule that implements
    * is_possible_to_regroup and regroup and uses these
    * functions to implement the rule. The visitor checks
    * every rule in rules at each position sequentially,
    * if the idea is to do the rewrite rules one at a time
    * without this logic, it is necessary to pass the visitors
    * one after the other. It is assumed that the rewrite
    * rules do not create infinite loops.
    *
    * Begin the visitation with the begin_visit() function.
    */
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
        /*       _\|/_
             (o o)
     +----oOO-{_}-OOo---------------------------------------------------+
     | To prevent errors always use begin_visit to start the rewriting, |
     | do not use op->accept_visitor directly                           |
     +-----------------------------------------------------------------*/
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

    void visit(OpConstruct&) override;
    void visit(OpDescribe&) override;
    void visit(OpFilter&) override;
    void visit(OpJoin&) override;
    void visit(OpSemiJoin&) override;
    void visit(OpMinus&) override;
    void visit(OpNotExists&) override;
    void visit(OpSequence&) override;
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

    void visit(OpBasicGraphPattern&) override { }
    void visit(OpEmpty&) override { }
    void visit(OpPath&) override { }
    void visit(OpProcedure&) override { }
    void visit(OpShow&) override { }
    void visit(OpTriple&) override { }
    void visit(OpUnitTable&) override { }
    void visit(OpValues&) override { }

    void visit(OpCreateHNSWIndex&) override { }
    void visit(OpCreateTextIndex&) override { }
    void visit(OpDeleteData&) override { }
    void visit(OpInsertData&) override { }
    void visit(OpUpdate&) override { }
};
} // namespace SPARQL

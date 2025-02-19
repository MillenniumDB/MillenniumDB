#pragma once

#include <set>
#include <memory>

#include "query/query_context.h"
#include "query/parser/expr/expr_visitor.h"
#include "query/parser/expr/sparql_exprs.h"
#include "query/rewriter/sparql/expr/rewrite_rules/expr_rewrite_rule.h"

namespace SPARQL {
   /**
    * This visitor uses a rewrite rule that implements
    * is_possible_to_regroup and regroup and uses these
    * functions to implement the rule. The visitor checks
    * every rule in rules at each position sequentially,
    * if the idea is to do the rewrite rules one at a time
    * without this logic, it is necessary to pass the visitors
    * one after the other. The visitor only passes once,
    * to pass multiple times the bool value has_rewritten
    * is checked with reset_and_check_if_has_rewritten_a_rule
    */
class ExprRewriteRuleVisitor : public ExprVisitor {

private:
    std::vector<std::unique_ptr<ExprRewriteRule>> rules;
    bool has_rewritten = false;
    // bool has_rewritten_father = false;

public:
    template <typename Rule>
    void add_rule() {
        rules.push_back(std::make_unique<Rule>());
    }

    void remove_rules() {
        rules.clear();
    }

    bool reset_and_check_if_has_rewritten_a_rule() {
        auto out = has_rewritten;
        has_rewritten = false;
        return out;
    }

    /************************************************************************
     * It is necessary to always start visitation from start_visit, because *
     * that way the first expr container is modified.\ Not is used because  *
     * it only contains an expr, it is removed after this visitation.       *
     ************************************************************************/
    void start_visit(std::unique_ptr<Expr>& expr) {
        auto temp_expr_container = std::make_unique<ExprNot>(std::move(expr));
        temp_expr_container->accept_visitor(*this);
        expr = std::move(temp_expr_container->expr);
    }

    // {}
    void visit(ExprVar&)         override {}
    void visit(ExprAggCountAll&) override {}
    void visit(ExprTerm&)        override {}
    void visit(ExprBound&)       override {}
    void visit(ExprStrUUID&)     override {}
    void visit(ExprUUID&)        override {}
    void visit(ExprNow&)         override {}
    void visit(ExprRand&)        override {}

    // expr
    void visit(ExprNot&            expr) override { visit_expr_with_one_expr<ExprNot>(expr);            }
    void visit(ExprUnaryMinus&     expr) override { visit_expr_with_one_expr<ExprUnaryMinus>(expr);     }
    void visit(ExprUnaryPlus&      expr) override { visit_expr_with_one_expr<ExprUnaryPlus>(expr);      }
    void visit(ExprAggAvg&         expr) override { visit_expr_with_one_expr<ExprAggAvg>(expr);         }
    void visit(ExprAggCount&       expr) override { visit_expr_with_one_expr<ExprAggCount>(expr);       }
    void visit(ExprAggGroupConcat& expr) override { visit_expr_with_one_expr<ExprAggGroupConcat>(expr); }
    void visit(ExprAggMax&         expr) override { visit_expr_with_one_expr<ExprAggMax>(expr);         }
    void visit(ExprAggMin&         expr) override { visit_expr_with_one_expr<ExprAggMin>(expr);         }
    void visit(ExprAggSample&      expr) override { visit_expr_with_one_expr<ExprAggSample>(expr);      }
    void visit(ExprAggSum&         expr) override { visit_expr_with_one_expr<ExprAggSum>(expr);         }
    void visit(ExprAbs&            expr) override { visit_expr_with_one_expr<ExprAbs>(expr);            }
    void visit(ExprBNode&          expr) override { visit_expr_with_one_expr<ExprBNode>(expr);          }
    void visit(ExprCeil&           expr) override { visit_expr_with_one_expr<ExprCeil>(expr);           }
    void visit(ExprDatatype&       expr) override { visit_expr_with_one_expr<ExprDatatype>(expr);       }
    void visit(ExprFloor&          expr) override { visit_expr_with_one_expr<ExprFloor>(expr);          }
    void visit(ExprHours&          expr) override { visit_expr_with_one_expr<ExprHours>(expr);          }
    void visit(ExprDay&            expr) override { visit_expr_with_one_expr<ExprDay>(expr);            }
    void visit(ExprEncodeForUri&   expr) override { visit_expr_with_one_expr<ExprEncodeForUri>(expr);   }
    void visit(ExprIRI&            expr) override { visit_expr_with_one_expr<ExprIRI>(expr);            }
    void visit(ExprIsBlank&        expr) override { visit_expr_with_one_expr<ExprIsBlank>(expr);        }
    void visit(ExprIsIRI&          expr) override { visit_expr_with_one_expr<ExprIsIRI>(expr);          }
    void visit(ExprIsLiteral&      expr) override { visit_expr_with_one_expr<ExprIsLiteral>(expr);      }
    void visit(ExprIsNumeric&      expr) override { visit_expr_with_one_expr<ExprIsNumeric>(expr);      }
    void visit(ExprIsURI&          expr) override { visit_expr_with_one_expr<ExprIsURI>(expr);          }
    void visit(ExprLang&           expr) override { visit_expr_with_one_expr<ExprLang>(expr);           }
    void visit(ExprLCase&          expr) override { visit_expr_with_one_expr<ExprLCase>(expr);          }
    void visit(ExprMD5&            expr) override { visit_expr_with_one_expr<ExprMD5>(expr);            }
    void visit(ExprMinutes&        expr) override { visit_expr_with_one_expr<ExprMinutes>(expr);        }
    void visit(ExprMonth&          expr) override { visit_expr_with_one_expr<ExprMonth>(expr);          }
    void visit(ExprRound&          expr) override { visit_expr_with_one_expr<ExprRound>(expr);          }
    void visit(ExprSeconds&        expr) override { visit_expr_with_one_expr<ExprSeconds>(expr);        }
    void visit(ExprSHA1&           expr) override { visit_expr_with_one_expr<ExprSHA1>(expr);           }
    void visit(ExprSHA256&         expr) override { visit_expr_with_one_expr<ExprSHA256>(expr);         }
    void visit(ExprSHA384&         expr) override { visit_expr_with_one_expr<ExprSHA384>(expr);         }
    void visit(ExprSHA512&         expr) override { visit_expr_with_one_expr<ExprSHA512>(expr);         }
    void visit(ExprStr&            expr) override { visit_expr_with_one_expr<ExprStr>(expr);            }
    void visit(ExprTimezone&       expr) override { visit_expr_with_one_expr<ExprTimezone>(expr);       }
    void visit(ExprTZ&             expr) override { visit_expr_with_one_expr<ExprTZ>(expr);             }
    void visit(ExprUCase&          expr) override { visit_expr_with_one_expr<ExprUCase>(expr);          }
    void visit(ExprURI&            expr) override { visit_expr_with_one_expr<ExprURI>(expr);            }
    void visit(ExprYear&           expr) override { visit_expr_with_one_expr<ExprYear>(expr);           }
    void visit(ExprStrLen&         expr) override { visit_expr_with_one_expr<ExprStrLen>(expr);         }
    void visit(ExprCast&           expr) override { visit_expr_with_one_expr<ExprCast>(expr);           }
    void visit(ExprSqrt&           expr) override { visit_expr_with_one_expr<ExprSqrt>(expr);           }
    void visit(ExprSum&            expr) override { visit_expr_with_one_expr<ExprSum>(expr);            }

    // lhs rhs
    void visit(ExprMultiplication& expr) override { visit_expr_with_lhs_and_rhs<ExprMultiplication>(expr); }
    void visit(ExprDivision&       expr) override { visit_expr_with_lhs_and_rhs<ExprDivision>(expr);       }
    void visit(ExprAddition&       expr) override { visit_expr_with_lhs_and_rhs<ExprAddition>(expr);       }
    void visit(ExprSubtraction&    expr) override { visit_expr_with_lhs_and_rhs<ExprSubtraction>(expr);    }
    void visit(ExprAnd&            expr) override { visit_expr_with_lhs_and_rhs<ExprAnd>(expr);            }
    void visit(ExprOr&             expr) override { visit_expr_with_lhs_and_rhs<ExprOr>(expr);             }
    void visit(ExprLess&           expr) override { visit_expr_with_lhs_and_rhs<ExprLess>(expr);           }
    void visit(ExprGreater&        expr) override { visit_expr_with_lhs_and_rhs<ExprGreater>(expr);        }
    void visit(ExprEqual&          expr) override { visit_expr_with_lhs_and_rhs<ExprEqual>(expr);          }
    void visit(ExprNotEqual&       expr) override { visit_expr_with_lhs_and_rhs<ExprNotEqual>(expr);       }
    void visit(ExprLessOrEqual&    expr) override { visit_expr_with_lhs_and_rhs<ExprLessOrEqual>(expr);    }
    void visit(ExprGreaterOrEqual& expr) override { visit_expr_with_lhs_and_rhs<ExprGreaterOrEqual>(expr); }
    void visit(ExprContains&       expr) override { visit_expr_with_lhs_and_rhs<ExprContains>(expr);       }
    void visit(ExprSameTerm&       expr) override { visit_expr_with_lhs_and_rhs<ExprSameTerm>(expr);       }
    void visit(ExprStrAfter&       expr) override { visit_expr_with_lhs_and_rhs<ExprStrAfter>(expr);       }
    void visit(ExprStrBefore&      expr) override { visit_expr_with_lhs_and_rhs<ExprStrBefore>(expr);      }
    void visit(ExprDot&            expr) override { visit_expr_with_lhs_and_rhs<ExprDot>(expr);            }
    void visit(ExprPow&            expr) override { visit_expr_with_lhs_and_rhs<ExprPow>(expr);            }

    // lhs_expr and exprs
    void visit(ExprIn&    expr) override { visit_expr_with_lhs_expr_and_exprs<ExprIn>(expr);    }
    void visit(ExprNotIn& expr) override { visit_expr_with_lhs_expr_and_exprs<ExprNotIn>(expr); }

    // exprs
    void visit(ExprCoalesce& expr) override { visit_expr_with_exprs<ExprCoalesce>(expr); }
    void visit(ExprConcat&   expr) override { visit_expr_with_exprs<ExprConcat>(expr); }

    // Contains an op
    void visit(ExprExists&)         override {}
    void visit(ExprNotExists&)      override {}

    // expr1 and expr2
    void visit(ExprLangMatches& expr) override { visit_expr_with_expr1_and_expr2<ExprLangMatches>(expr); }
    void visit(ExprStrDT&       expr) override { visit_expr_with_expr1_and_expr2<ExprStrDT>(expr);       }
    void visit(ExprStrEnds&     expr) override { visit_expr_with_expr1_and_expr2<ExprStrEnds>(expr);     }
    void visit(ExprStrLang&     expr) override { visit_expr_with_expr1_and_expr2<ExprStrLang>(expr);     }
    void visit(ExprStrStarts&   expr) override { visit_expr_with_expr1_and_expr2<ExprStrStarts>(expr);   }

    // expr1 expr2 expr3
    void visit(ExprIf&      expr) override { visit_expr_with_expr1_and_expr2_and_expr3<ExprIf>(expr); }
    void visit(ExprRegex&   expr) override { visit_expr_with_expr1_and_expr2_and_expr3<ExprRegex>(expr);       }
    void visit(ExprSubStr&  expr) override { visit_expr_with_expr1_and_expr2_and_expr3<ExprSubStr>(expr);      }

    // expr1 expr2 expr3 expr4
    void visit(ExprReplace& expr) override { visit_expr_with_expr1_and_expr2_and_expr3_and_expr4<ExprReplace>(expr); }

private:
    template <typename T>
    void visit_expr_with_one_expr(T& expr) {
        for (auto& rule : rules) {
            if (rule->is_possible_to_regroup(expr.expr)) {
                expr.expr = rule->regroup(std::move(expr.expr));
                has_rewritten = true;
            }
        }
        if (expr.expr != nullptr)
            expr.expr->accept_visitor(*this);
    }

    template <typename T>
    void visit_expr_with_exprs(T& expr) {
        for (auto& rule : rules) {
            for (auto& child : expr.exprs) {
                if (rule->is_possible_to_regroup(child)) {
                    child = rule->regroup(std::move(child));
                    has_rewritten = true;
                }
            }
        }
        for (auto& child : expr.exprs) {
            if (child != nullptr) {
                child->accept_visitor(*this);
            }
        }
    }

    template <typename T>
    void visit_expr_with_expr1_and_expr2(T& expr) {
        for (auto& rule : rules) {
            if (rule->is_possible_to_regroup(expr.expr1)) {
                expr.expr1 = rule->regroup(std::move(expr.expr1));
                has_rewritten = true;
            }
            if (rule->is_possible_to_regroup(expr.expr2)) {
                expr.expr2 = rule->regroup(std::move(expr.expr2));
                has_rewritten = true;
            }
        }
        if (expr.expr1 != nullptr)
            expr.expr1->accept_visitor(*this);
        if (expr.expr2 != nullptr)
            expr.expr2->accept_visitor(*this);
    }

    template <typename T>
    void visit_expr_with_lhs(T& expr) {
        for (auto& rule : rules) {
            if (rule->is_possible_to_regroup(expr.lhs)) {
                expr.lhs = rule->regroup(std::move(expr.lhs));
                has_rewritten = true;
            }
        }
        if (expr.lhs != nullptr)
            expr.lhs->accept_visitor(*this);
    }

    template <typename T>
    void visit_expr_with_rhs(T& expr) {
        for (auto& rule : rules) {
            if (rule->is_possible_to_regroup(expr.rhs)) {
                expr.rhs = rule->regroup(std::move(expr.rhs));
                has_rewritten = true;
            }
        }
        if (expr.rhs != nullptr)
            expr.rhs->accept_visitor(*this);
    }

    template <typename T>
    void visit_expr_with_lhs_and_rhs(T& expr) {
        visit_expr_with_lhs<T>(expr);
        visit_expr_with_rhs<T>(expr);
    }


    template <typename T>
    void visit_expr_with_expr1_and_expr2_and_expr3(T& expr) {
        visit_expr_with_expr1_and_expr2<T>(expr);
        if (expr.expr3 != nullptr) {
            for (auto& rule : rules) {
                if (rule->is_possible_to_regroup(expr.expr3)) {
                    expr.expr3 = rule->regroup(std::move(expr.expr3));
                    has_rewritten = true;
                }
            }
            expr.expr3->accept_visitor(*this);
        }
    }

    template <typename T>
    void visit_expr_with_expr1_and_expr2_and_expr3_and_expr4(T& expr) {
        visit_expr_with_expr1_and_expr2_and_expr3<T>(expr);
        for (auto& rule : rules) {
            if (rule->is_possible_to_regroup(expr.expr4)) {
                expr.expr4 = rule->regroup(std::move(expr.expr4));
                has_rewritten = true;
            }
        }
        if (expr.expr4 != nullptr)
            expr.expr4->accept_visitor(*this);
    }

    template <typename T>
    void visit_expr_with_lhs_expr_and_exprs(T& expr) {
        for (auto& rule : rules) {
            if (rule->is_possible_to_regroup(expr.lhs_expr)) {
                expr.lhs_expr = rule->regroup(std::move(expr.lhs_expr));
                has_rewritten = true;
            }
        }
        if (expr.lhs_expr != nullptr)
            expr.lhs_expr->accept_visitor(*this);
        visit_expr_with_exprs<T>(expr);
    }
};
} // namespace SPARQL

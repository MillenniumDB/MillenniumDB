#include "expr_to_binding_expr.h"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "query/executor/binding_iter/aggregation/sparql/aggs.h"
#include "query/executor/binding_iter/binding_expr/binding_expr_term.h"
#include "query/executor/binding_iter/binding_expr/binding_expr_var.h"
#include "query/executor/binding_iter/binding_expr/sparql_binding_exprs.h"
#include "query/parser/expr/sparql/exprs.h"
#include "query/optimizer/rdf_model/binding_iter_constructor.h"

using namespace std;
using namespace SPARQL;

void ExprToBindingExpr::visit(ExprVar& expr_var) {
    at_root = false;
    tmp = make_unique<BindingExprVar>(expr_var.var);
    if (bic == nullptr) return;

    bic->group_saved_vars.insert(expr_var.var);

    if (inside_aggregation) {
        if (!bic->grouping) {
            std::string msg = "Var \""
                    + get_query_ctx().get_var_name(expr_var.var)
                    + "\" cannot be used in aggregation without grouping";
            throw QuerySemanticException(msg);
        }
    } else { // not inside aggregation
        if (bic->grouping) {
            // It's fine to use a var previously defined as an expression
            // Example:
            // SELECT (2*sum(?o) as ?x) (-1*?x AS ?y)
            // WHERE { ?s :p ?o }
            // GROUP BY ?s
            // In that case ?x is not grouped but can be used
            for (const auto& [var, _] : bic->projection_order_exprs) {
                if (var == expr_var.var) {
                    return;
                }
            }
            if (bic->group_vars.find(expr_var.var) == bic->group_vars.end()) {
                std::string msg = "Var \""
                    + get_query_ctx().get_var_name(expr_var.var)
                    + "\" must be grouped";
                throw QuerySemanticException(msg);
            }
        }
    }
}


void ExprToBindingExpr::visit(ExprTerm& expr_term) {
    at_root = false;
    tmp = make_unique<BindingExprTerm>(expr_term.term);
}


void ExprToBindingExpr::visit(ExprBNode& expr_bnode) {
    at_root = false;
    if (expr_bnode.expr) {
        expr_bnode.expr->accept_visitor(*this);
        tmp = make_unique<BindingExprBNode>(
            std::move(tmp)
        );
    } else {
        tmp = make_unique<BindingExprTerm>(
            Conversions::pack_blank_tmp(get_query_ctx().get_new_blank_node())
        );
    }
}


void ExprToBindingExpr::visit(ExprEqual& expr_equal) {
    at_root = false;
    expr_equal.lhs->accept_visitor(*this);
    auto lhs = std::move(tmp);

    expr_equal.rhs->accept_visitor(*this);
    auto rhs = std::move(tmp);

    tmp = make_unique<BindingExprEquals>(std::move(lhs), std::move(rhs));
}


void ExprToBindingExpr::visit(ExprNotEqual& expr_not_equal) {
    at_root = false;
    expr_not_equal.lhs->accept_visitor(*this);
    auto lhs = std::move(tmp);

    expr_not_equal.rhs->accept_visitor(*this);
    auto rhs = std::move(tmp);

    // (A != B) === NOT(A = B)
    tmp = make_unique<BindingExprEquals>(std::move(lhs), std::move(rhs));
    tmp = make_unique<BindingExprNot>(std::move(tmp));
}


void ExprToBindingExpr::visit(ExprNot& expr_not) {
    at_root = false;
    expr_not.expr->accept_visitor(*this);
    auto expr = std::move(tmp);

    tmp = make_unique<BindingExprNot>(std::move(expr));
}


void ExprToBindingExpr::visit(ExprUnaryMinus& expr_unary_minus) {
    at_root = false;
    expr_unary_minus.expr->accept_visitor(*this);
    auto expr = std::move(tmp);

    tmp = make_unique<BindingExprUnaryMinus>(std::move(expr));
}

void ExprToBindingExpr::visit(ExprUnaryPlus& expr_unary_plus) {
    at_root = false;
    expr_unary_plus.expr->accept_visitor(*this);
    auto expr = std::move(tmp);

    tmp = make_unique<BindingExprUnaryPlus>(std::move(expr));
}

void ExprToBindingExpr::visit(ExprMultiplication& expr_multiplication) {
    at_root = false;
    expr_multiplication.lhs->accept_visitor(*this);
    auto lhs = std::move(tmp);

    expr_multiplication.rhs->accept_visitor(*this);
    auto rhs = std::move(tmp);

    tmp = make_unique<BindingExprMultiplication>(std::move(lhs), std::move(rhs));
}

void ExprToBindingExpr::visit(ExprDivision& expr_division) {
    at_root = false;
    expr_division.lhs->accept_visitor(*this);
    auto lhs = std::move(tmp);

    expr_division.rhs->accept_visitor(*this);
    auto rhs = std::move(tmp);

    tmp = make_unique<BindingExprDivision>(std::move(lhs), std::move(rhs));
}

void ExprToBindingExpr::visit(ExprAddition& expr_addition) {
    at_root = false;
    expr_addition.lhs->accept_visitor(*this);
    auto lhs = std::move(tmp);

    expr_addition.rhs->accept_visitor(*this);
    auto rhs = std::move(tmp);

    tmp = make_unique<BindingExprAddition>(std::move(lhs), std::move(rhs));
}

void ExprToBindingExpr::visit(ExprSubtraction& expr_subtraction) {
    at_root = false;
    expr_subtraction.lhs->accept_visitor(*this);
    auto lhs = std::move(tmp);

    expr_subtraction.rhs->accept_visitor(*this);
    auto rhs = std::move(tmp);

    tmp = make_unique<BindingExprSubtraction>(std::move(lhs), std::move(rhs));
}

void ExprToBindingExpr::visit(ExprAnd& expr_and) {
    at_root = false;
    expr_and.lhs->accept_visitor(*this);
    auto lhs = std::move(tmp);

    expr_and.rhs->accept_visitor(*this);
    auto rhs = std::move(tmp);

    tmp = make_unique<BindingExprAnd>(std::move(lhs), std::move(rhs));
}

void ExprToBindingExpr::visit(ExprOr& expr_or) {
    at_root = false;
    expr_or.lhs->accept_visitor(*this);
    auto lhs = std::move(tmp);

    expr_or.rhs->accept_visitor(*this);
    auto rhs = std::move(tmp);

    tmp = make_unique<BindingExprOr>(std::move(lhs), std::move(rhs));
}

void ExprToBindingExpr::visit(ExprLess& expr_less) {
    at_root = false;
    expr_less.lhs->accept_visitor(*this);
    auto lhs = std::move(tmp);

    expr_less.rhs->accept_visitor(*this);
    auto rhs = std::move(tmp);

    tmp = make_unique<BindingExprLess>(std::move(lhs), std::move(rhs));
}

void ExprToBindingExpr::visit(ExprGreater& expr_greater) {
    at_root = false;
    expr_greater.lhs->accept_visitor(*this);
    auto lhs = std::move(tmp);

    expr_greater.rhs->accept_visitor(*this);
    auto rhs = std::move(tmp);

    tmp = make_unique<BindingExprGreater>(std::move(lhs), std::move(rhs));
}

void ExprToBindingExpr::visit(ExprLessOrEqual& expr_less_or_equal) {
    at_root = false;
    expr_less_or_equal.lhs->accept_visitor(*this);
    auto lhs = std::move(tmp);

    expr_less_or_equal.rhs->accept_visitor(*this);
    auto rhs = std::move(tmp);

    tmp = make_unique<BindingExprLessOrEqual>(std::move(lhs), std::move(rhs));
}

void ExprToBindingExpr::visit(ExprGreaterOrEqual& expr_greater_or_equal) {
    at_root = false;
    expr_greater_or_equal.lhs->accept_visitor(*this);
    auto lhs = std::move(tmp);

    expr_greater_or_equal.rhs->accept_visitor(*this);
    auto rhs = std::move(tmp);

    tmp = make_unique<BindingExprGreaterOrEqual>(std::move(lhs), std::move(rhs));
}

void ExprToBindingExpr::visit(ExprNotExists& expr_not_exists) {
    at_root = false;
    BindingIterConstructor binding_iter_visitor;
    if (bic != nullptr) {
        binding_iter_visitor.safe_assigned_vars = bic->safe_assigned_vars;
    }

    expr_not_exists.op->accept_visitor(binding_iter_visitor);

    tmp = make_unique<BindingExprNotExists>(
        std::move(binding_iter_visitor.tmp),
        expr_not_exists.get_all_vars()
    );
}


void ExprToBindingExpr::visit(ExprExists& expr_exists) {
    at_root = false;

    BindingIterConstructor binding_iter_visitor;
    if (bic != nullptr) {
        binding_iter_visitor.safe_assigned_vars = bic->safe_assigned_vars;
    }

    expr_exists.op->accept_visitor(binding_iter_visitor);

    tmp = make_unique<BindingExprExists>(
        std::move(binding_iter_visitor.tmp),
        expr_exists.get_all_vars()
    );
}


void ExprToBindingExpr::visit(ExprIn& expr_in) {
    at_root = false;

    std::vector<std::unique_ptr<BindingExpr>> exprs;
    exprs.reserve(expr_in.exprs.size());

    for (auto& expr : expr_in.exprs) {
        expr->accept_visitor(*this);
        exprs.push_back(std::move(tmp));
    }

    expr_in.lhs_expr->accept_visitor(*this);

    tmp = make_unique<BindingExprIn>(std::move(tmp), std::move(exprs));
}


void ExprToBindingExpr::visit(ExprNotIn& expr_not_in) {
    at_root = false;

    std::vector<std::unique_ptr<BindingExpr>> exprs;
    exprs.reserve(expr_not_in.exprs.size());

    for (auto& expr : expr_not_in.exprs) {
        expr->accept_visitor(*this);
        exprs.push_back(std::move(tmp));
    }

    expr_not_in.lhs_expr->accept_visitor(*this);

    // (A NOT IN (x,y,z)) == ! (A IN (x,y,z))
    tmp = make_unique<BindingExprIn>(std::move(tmp), std::move(exprs));
    tmp = make_unique<BindingExprNot>(std::move(tmp));
}


void ExprToBindingExpr::visit(ExprAbs& expr_abs) {
    at_root = false;
    expr_abs.expr->accept_visitor(*this);
    auto expr = std::move(tmp);

    tmp = make_unique<BindingExprAbs>(std::move(expr));
}

void ExprToBindingExpr::visit(ExprBound& expr_bound) {
    at_root = false;
    tmp = make_unique<BindingExprBound>(expr_bound.var);
}

void ExprToBindingExpr::visit(ExprCeil& expr_ceil) {
    at_root = false;
    expr_ceil.expr->accept_visitor(*this);
    auto expr = std::move(tmp);

    tmp = make_unique<BindingExprCeil>(std::move(expr));
}

void ExprToBindingExpr::visit(ExprReplace& expr_replace) {
    at_root = false;
    expr_replace.expr1->accept_visitor(*this);
    auto expr1 = std::move(tmp);

    expr_replace.expr2->accept_visitor(*this);
    auto expr2 = std::move(tmp);

    expr_replace.expr3->accept_visitor(*this);
    auto expr3 = std::move(tmp);

    if (expr_replace.expr4) {
        expr_replace.expr4->accept_visitor(*this);
        auto expr4 = std::move(tmp);
        tmp =
          make_unique<BindingExprReplace>(std::move(expr1), std::move(expr2), std::move(expr3), std::move(expr4));
    } else {
        tmp =
          make_unique<BindingExprReplace>(std::move(expr1), std::move(expr2), std::move(expr3), nullptr);
    }
}

void ExprToBindingExpr::visit(ExprRegex& expr_regex) {
    at_root = false;
    expr_regex.expr1->accept_visitor(*this);
    auto expr1 = std::move(tmp);

    expr_regex.expr2->accept_visitor(*this);
    auto expr2 = std::move(tmp);

    if (expr_regex.expr3) {
        expr_regex.expr3->accept_visitor(*this);
        auto expr3 = std::move(tmp);
        tmp = make_unique<BindingExprRegex>(std::move(expr1), std::move(expr2), std::move(expr3));
    } else {
        tmp = make_unique<BindingExprRegex>(std::move(expr1), std::move(expr2), nullptr);
    }
}

void ExprToBindingExpr::visit(ExprRound& expr_round) {
    at_root = false;
    expr_round.expr->accept_visitor(*this);
    auto expr = std::move(tmp);

    tmp = make_unique<BindingExprRound>(std::move(expr));
}

void ExprToBindingExpr::visit(ExprFloor& expr_floor) {
    at_root = false;
    expr_floor.expr->accept_visitor(*this);
    auto expr = std::move(tmp);

    tmp = make_unique<BindingExprFloor>(std::move(expr));
}

void ExprToBindingExpr::visit(ExprCoalesce& expr_coalesce) {
    at_root = false;
    std::vector<std::unique_ptr<BindingExpr>> expr_list;
    for (auto& expr : expr_coalesce.exprs) {
        expr->accept_visitor(*this);
        expr_list.push_back(std::move(tmp));
    }

    tmp = make_unique<BindingExprCoalesce>(std::move(expr_list));
}

void ExprToBindingExpr::visit(ExprConcat& expr_concat) {
    at_root = false;
     std::vector<std::unique_ptr<BindingExpr>> expr_list;
    for (auto& expr : expr_concat.exprs) {
        expr->accept_visitor(*this);
        expr_list.push_back(std::move(tmp));
    }

    tmp = make_unique<BindingExprConcat>(std::move(expr_list));
}

void ExprToBindingExpr::visit(ExprContains& expr_contains) {
    at_root = false;
    expr_contains.lhs->accept_visitor(*this);
    auto lhs = std::move(tmp);

    expr_contains.rhs->accept_visitor(*this);
    auto rhs = std::move(tmp);

    tmp = make_unique<BindingExprContains>(std::move(lhs), std::move(rhs));
}

void ExprToBindingExpr::visit(ExprDatatype& expr_datatype) {
    at_root = false;
    expr_datatype.expr->accept_visitor(*this);
    auto expr = std::move(tmp);

    tmp = make_unique<BindingExprDatatype>(std::move(expr));
}

void ExprToBindingExpr::visit(ExprDay& expr_month) {
    at_root = false;
    expr_month.expr->accept_visitor(*this);

    auto expr = std::move(tmp);

    tmp = make_unique<BindingExprDay>(std::move(expr));
}

void ExprToBindingExpr::visit(ExprEncodeForUri& expr_encode_for_uri) {
    at_root = false;
    expr_encode_for_uri.expr->accept_visitor(*this);
    auto expr = std::move(tmp);

    tmp = make_unique<BindingExprEncodeForUri>(std::move(expr));
}

void ExprToBindingExpr::visit(ExprHours& expr_hours) {
    at_root = false;
    expr_hours.expr->accept_visitor(*this);

    auto expr = std::move(tmp);

    tmp = make_unique<BindingExprHours>(std::move(expr));
}

void ExprToBindingExpr::visit(ExprIf& expr_if) {
    at_root = false;
    expr_if.expr1->accept_visitor(*this);
    auto expr_cond = std::move(tmp);

    expr_if.expr2->accept_visitor(*this);
    auto expr_then = std::move(tmp);

    expr_if.expr3->accept_visitor(*this);
    auto expr_else = std::move(tmp);

    tmp = make_unique<BindingExprIf>(std::move(expr_cond), std::move(expr_then), std::move(expr_else));
}

void ExprToBindingExpr::visit(ExprIRI& expr_iri) {
    at_root = false;
    expr_iri.expr->accept_visitor(*this);

    auto expr = std::move(tmp);

    tmp = make_unique<BindingExprIRI>(std::move(expr), expr_iri.base_iri);
}

void ExprToBindingExpr::visit(ExprIsBlank& expr_is_blank) {
    at_root = false;
    expr_is_blank.expr->accept_visitor(*this);
    auto expr = std::move(tmp);

    tmp = make_unique<BindingExprIsBlank>(std::move(expr));
}

void ExprToBindingExpr::visit(ExprIsIRI& expr_is_iri) {
    at_root = false;
    expr_is_iri.expr->accept_visitor(*this);
    auto expr = std::move(tmp);

    tmp = make_unique<BindingExprIsIRI>(std::move(expr));
}

void ExprToBindingExpr::visit(ExprIsLiteral& expr_is_literal) {
    at_root = false;
    expr_is_literal.expr->accept_visitor(*this);
    auto expr = std::move(tmp);

    tmp = make_unique<BindingExprIsLiteral>(std::move(expr));
}

void ExprToBindingExpr::visit(ExprIsNumeric& expr_is_numeric) {
    at_root = false;
    expr_is_numeric.expr->accept_visitor(*this);
    auto expr = std::move(tmp);

    tmp = make_unique<BindingExprIsNumeric>(std::move(expr));
}

void ExprToBindingExpr::visit(ExprIsURI& expr_is_uri) {
    at_root = false;
    expr_is_uri.expr->accept_visitor(*this);
    auto expr = std::move(tmp);

    // isURI is an alternate spelling for the isIRI function
    tmp = make_unique<BindingExprIsIRI>(std::move(expr));
}

void ExprToBindingExpr::visit(ExprLang& expr_lang) {
    at_root = false;
    expr_lang.expr->accept_visitor(*this);
    auto expr = std::move(tmp);

    tmp = make_unique<BindingExprLang>(std::move(expr));
}

void ExprToBindingExpr::visit(ExprLangMatches& expr_lang_matches) {
    at_root = false;
    expr_lang_matches.expr1->accept_visitor(*this);
    auto expr1 = std::move(tmp);

    expr_lang_matches.expr2->accept_visitor(*this);
    auto expr2 = std::move(tmp);

    tmp = make_unique<BindingExprLangMatches>(std::move(expr1), std::move(expr2));
}

void ExprToBindingExpr::visit(ExprLCase& expr_lcase) {
    at_root = false;
    expr_lcase.expr->accept_visitor(*this);

    auto expr = std::move(tmp);

    tmp = make_unique<BindingExprLCase>(std::move(expr));
}

void ExprToBindingExpr::visit(ExprMD5& expr_md5) {
    at_root = false;
    expr_md5.expr->accept_visitor(*this);

    auto expr = std::move(tmp);

    tmp = make_unique<BindingExprMD5>(std::move(expr));
}

void ExprToBindingExpr::visit(ExprMinutes& expr_minutes) {
    at_root = false;
    expr_minutes.expr->accept_visitor(*this);

    auto expr = std::move(tmp);

    tmp = make_unique<BindingExprMinutes>(std::move(expr));
}

void ExprToBindingExpr::visit(ExprMonth& expr_month) {
    at_root = false;
    expr_month.expr->accept_visitor(*this);

    auto expr = std::move(tmp);

    tmp = make_unique<BindingExprMonth>(std::move(expr));
}

void ExprToBindingExpr::visit(ExprNow&) {
    at_root = false;
    // Each invocation of the NOW function returns exactly the same value.
    std::time_t t = std::chrono::system_clock::to_time_t(get_query_ctx().thread_info.time_start);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&t), "%Y-%m-%dT%H:%M:%SZ");
    // Pack the datetime into an ObjectId
    ObjectId oid(DateTime::from_dateTime(ss.str()));

    tmp = make_unique<BindingExprTerm>(oid);
}

void ExprToBindingExpr::visit(ExprRand&) {
    at_root = false;
    // Each invocation of the RAND function returns a new value.
    double dbl = get_query_ctx().get_rand();

    tmp = make_unique<BindingExprTerm>(Conversions::pack_double(dbl));
}

void ExprToBindingExpr::visit(ExprSameTerm& expr_same_term) {
    at_root = false;
    expr_same_term.lhs->accept_visitor(*this);
    auto lhs = std::move(tmp);

    expr_same_term.rhs->accept_visitor(*this);
    auto rhs = std::move(tmp);

    tmp = make_unique<BindingExprSameTerm>(std::move(lhs), std::move(rhs));
}

void ExprToBindingExpr::visit(ExprSeconds& expr_seconds) {
    at_root = false;
    expr_seconds.expr->accept_visitor(*this);

    auto expr = std::move(tmp);

    tmp = make_unique<BindingExprSeconds>(std::move(expr));
}

void ExprToBindingExpr::visit(ExprSHA1& expr_sha1) {
    at_root = false;
    expr_sha1.expr->accept_visitor(*this);

    auto expr = std::move(tmp);

    tmp = make_unique<BindingExprSHA1>(std::move(expr));
}

void ExprToBindingExpr::visit(ExprSHA256& expr_sha256) {
    at_root = false;
    expr_sha256.expr->accept_visitor(*this);

    auto expr = std::move(tmp);

    tmp = make_unique<BindingExprSHA256>(std::move(expr));
}

void ExprToBindingExpr::visit(ExprSHA384& expr_sha384) {
    at_root = false;
    expr_sha384.expr->accept_visitor(*this);

    auto expr = std::move(tmp);

    tmp = make_unique<BindingExprSHA384>(std::move(expr));
}

void ExprToBindingExpr::visit(ExprSHA512& expr_sha512) {
    at_root = false;
    expr_sha512.expr->accept_visitor(*this);

    auto expr = std::move(tmp);

    tmp = make_unique<BindingExprSHA512>(std::move(expr));
}

void ExprToBindingExpr::visit(ExprStrAfter& expr_str_after) {
    at_root = false;
    expr_str_after.lhs->accept_visitor(*this);
    auto lhs = std::move(tmp);

    expr_str_after.rhs->accept_visitor(*this);
    auto rhs = std::move(tmp);

    tmp = make_unique<BindingExprStrAfter>(std::move(lhs), std::move(rhs));
}

void ExprToBindingExpr::visit(ExprStrBefore& expr_str_before) {
    at_root = false;
    expr_str_before.lhs->accept_visitor(*this);
    auto lhs = std::move(tmp);

    expr_str_before.rhs->accept_visitor(*this);
    auto rhs = std::move(tmp);

    tmp = make_unique<BindingExprStrBefore>(std::move(lhs), std::move(rhs));
}

void ExprToBindingExpr::visit(ExprStrDT& expr_str_dt) {
    at_root = false;
    expr_str_dt.expr1->accept_visitor(*this);
    auto expr1 = std::move(tmp);

    expr_str_dt.expr2->accept_visitor(*this);
    auto expr2 = std::move(tmp);

    tmp = make_unique<BindingExprStrDT>(std::move(expr1), std::move(expr2));
}

void ExprToBindingExpr::visit(ExprStrEnds& expr_str_ends) {
    at_root = false;
    expr_str_ends.expr1->accept_visitor(*this);
    auto lhs = std::move(tmp);

    expr_str_ends.expr2->accept_visitor(*this);
    auto rhs = std::move(tmp);

    tmp = make_unique<BindingExprStrEnds>(std::move(lhs), std::move(rhs));
}

void ExprToBindingExpr::visit(ExprStrLang& expr_str_lang) {
    at_root = false;
    expr_str_lang.expr1->accept_visitor(*this);
    auto expr1 = std::move(tmp);

    expr_str_lang.expr2->accept_visitor(*this);
    auto expr2 = std::move(tmp);

    tmp = make_unique<BindingExprStrLang>(std::move(expr1), std::move(expr2));
}

void ExprToBindingExpr::visit(ExprStrLen& expr_str_len) {
    at_root = false;
    expr_str_len.expr->accept_visitor(*this);
    auto expr = std::move(tmp);

    tmp = make_unique<BindingExprStrLen>(std::move(expr));
}

void ExprToBindingExpr::visit(ExprStrStarts& expr_str_starts) {
    at_root = false;
    expr_str_starts.expr1->accept_visitor(*this);
    auto lhs = std::move(tmp);

    expr_str_starts.expr2->accept_visitor(*this);
    auto rhs = std::move(tmp);

    tmp = make_unique<BindingExprStrStarts>(std::move(lhs), std::move(rhs));
}

void ExprToBindingExpr::visit(ExprStrUUID&) {
    at_root = false;
    // Each invocation of the STRUUID function returns a new value.
    boost::uuids::random_generator uuid_generator;
    boost::uuids::uuid uuid = uuid_generator();
    std::string uuid_str = boost::uuids::to_string(uuid);

    tmp = make_unique<BindingExprTerm>(Conversions::pack_string_simple(uuid_str));
}

void ExprToBindingExpr::visit(ExprStr& expr_str) {
    at_root = false;
    expr_str.expr->accept_visitor(*this);
    auto expr = std::move(tmp);

    tmp = make_unique<BindingExprStr>(std::move(expr));
}

void ExprToBindingExpr::visit(ExprURI& expr_uri) {
    at_root = false;
    expr_uri.expr->accept_visitor(*this);

    auto expr = std::move(tmp);

    // URI is an alternate spelling for the IRI function
    tmp = make_unique<BindingExprIRI>(std::move(expr), expr_uri.base_iri);
}

void ExprToBindingExpr::visit(ExprSubStr& expr_substr) {
    at_root = false;
    expr_substr.expr1->accept_visitor(*this);
    auto expr_str = std::move(tmp);

    expr_substr.expr2->accept_visitor(*this);
    auto expr_start = std::move(tmp);

    if (expr_substr.expr3 == nullptr) {
        tmp = make_unique<BindingExprSubStr>(std::move(expr_str), std::move(expr_start));
    } else {
        expr_substr.expr3->accept_visitor(*this);
        auto expr_length = std::move(tmp);
        tmp = make_unique<BindingExprSubStr>(std::move(expr_str), std::move(expr_start), std::move(expr_length));
    }
}

void ExprToBindingExpr::visit(ExprTimezone& expr_timezone) {
    at_root = false;
    expr_timezone.expr->accept_visitor(*this);

    auto expr = std::move(tmp);

    tmp = make_unique<BindingExprTimezone>(std::move(expr));
}

void ExprToBindingExpr::visit(ExprTZ& expr_tz) {
    at_root = false;
    expr_tz.expr->accept_visitor(*this);

    auto expr = std::move(tmp);

    tmp = make_unique<BindingExprTZ>(std::move(expr));
}

void ExprToBindingExpr::visit(ExprUCase& expr_ucase) {
    at_root = false;
    expr_ucase.expr->accept_visitor(*this);

    auto expr = std::move(tmp);

    tmp = make_unique<BindingExprUCase>(std::move(expr));
}

void ExprToBindingExpr::visit(ExprUUID&) {
    at_root = false;
    // Each invocation of the UUID function returns a new value.
    boost::uuids::random_generator uuid_generator;
    boost::uuids::uuid uuid = uuid_generator();
    std::string uuid_str = "urn:uuid:" + boost::uuids::to_string(uuid);

    tmp = make_unique<BindingExprTerm>(Conversions::pack_iri(uuid_str));
}

void ExprToBindingExpr::visit(ExprYear& expr_year) {
    at_root = false;
    expr_year.expr->accept_visitor(*this);

    auto expr = std::move(tmp);

    tmp = make_unique<BindingExprYear>(std::move(expr));
}

void ExprToBindingExpr::visit(ExprCast& expr_cast) {
    at_root = false;
    expr_cast.expr->accept_visitor(*this);

    auto expr = std::move(tmp);

    tmp = make_unique<BindingExprCast>(expr_cast.cast_type, std::move(expr));
}

void ExprToBindingExpr::visit(ExprCosineDistance& expr_cosine_distance)
{
    at_root = false;
    expr_cosine_distance.lhs->accept_visitor(*this);
    auto lhs = std::move(tmp);

    expr_cosine_distance.rhs->accept_visitor(*this);
    auto rhs = std::move(tmp);

    tmp = make_unique<BindingExprCosineDistance>(std::move(lhs), std::move(rhs));
}


void ExprToBindingExpr::visit(ExprCosineSimilarity& expr_cosine_similarity)
{
    at_root = false;
    expr_cosine_similarity.lhs->accept_visitor(*this);
    auto lhs = std::move(tmp);

    expr_cosine_similarity.rhs->accept_visitor(*this);
    auto rhs = std::move(tmp);

    tmp = make_unique<BindingExprCosineSimilarity>(std::move(lhs), std::move(rhs));
}

void ExprToBindingExpr::visit(ExprDot& expr_dot)
{
    at_root = false;
    expr_dot.lhs->accept_visitor(*this);
    auto lhs = std::move(tmp);

    expr_dot.rhs->accept_visitor(*this);
    auto rhs = std::move(tmp);

    tmp = make_unique<BindingExprDot>(std::move(lhs), std::move(rhs));
}

void ExprToBindingExpr::visit(ExprEuclideanDistance& expr_euclidean_distance)
{
    at_root = false;
    expr_euclidean_distance.lhs->accept_visitor(*this);
    auto lhs = std::move(tmp);

    expr_euclidean_distance.rhs->accept_visitor(*this);
    auto rhs = std::move(tmp);

    tmp = make_unique<BindingExprEuclideanDistance>(std::move(lhs), std::move(rhs));
}

void ExprToBindingExpr::visit(ExprManhattanDistance& expr_manhattan_distance)
{
    at_root = false;
    expr_manhattan_distance.lhs->accept_visitor(*this);
    auto lhs = std::move(tmp);

    expr_manhattan_distance.rhs->accept_visitor(*this);
    auto rhs = std::move(tmp);

    tmp = make_unique<BindingExprManhattanDistance>(std::move(lhs), std::move(rhs));
}

void ExprToBindingExpr::visit(ExprPow& expr_pow)
{
    at_root = false;
    expr_pow.lhs->accept_visitor(*this);
    auto lhs = std::move(tmp);

    expr_pow.rhs->accept_visitor(*this);
    auto rhs = std::move(tmp);

    tmp = make_unique<BindingExprPow>(std::move(lhs), std::move(rhs));
}

void ExprToBindingExpr::visit(ExprSqrt& expr_sqrt)
{
    at_root = false;
    expr_sqrt.expr->accept_visitor(*this);

    auto expr = std::move(tmp);

    tmp = make_unique<BindingExprSqrt>(std::move(expr));
}

void ExprToBindingExpr::visit(ExprSum& expr_sum)
{
    at_root = false;
    expr_sum.expr->accept_visitor(*this);

    auto expr = std::move(tmp);

    tmp = make_unique<BindingExprSum>(std::move(expr));
}

void ExprToBindingExpr::visit(ExprAggSum& expr) {
    if (expr.distinct) {
        check_and_make_aggregate<AggSumDistinct>(expr.expr.get());
    } else {
        check_and_make_aggregate<AggSum>(expr.expr.get());
    }
}

void ExprToBindingExpr::visit(ExprAggMin& expr) {
    check_and_make_aggregate<AggMin>(expr.expr.get());
}

void ExprToBindingExpr::visit(ExprAggMax& expr) {
    check_and_make_aggregate<AggMax>(expr.expr.get());
}

void ExprToBindingExpr::visit(ExprAggAvg& expr) {
    if (expr.distinct) {
        check_and_make_aggregate<AggAvgDistinct>(expr.expr.get());
    } else {
        check_and_make_aggregate<AggAvg>(expr.expr.get());
    }
}

void ExprToBindingExpr::visit(ExprAggCount& expr) {
    if (expr.distinct) {
        check_and_make_aggregate<AggCountDistinct>(expr.expr.get());
    } else {
        check_and_make_aggregate<AggCount>(expr.expr.get());
    }
}

void ExprToBindingExpr::visit(ExprAggCountAll& expr) {
    if (expr.distinct) {
        std::vector<VarId> basic_vars;

        for (auto var : get_query_ctx().get_non_internal_vars()) {
            basic_vars.push_back(var);
        }

        check_and_make_aggregate<AggCountAllDistinct>(nullptr, std::move(basic_vars));
    } else {
        check_and_make_aggregate<AggCountAll>(nullptr);
    }
}

void ExprToBindingExpr::visit(ExprAggSample& expr) {
    check_and_make_aggregate<AggSample>(expr.expr.get());
}

void ExprToBindingExpr::visit(ExprAggGroupConcat& expr) {
    if (expr.distinct) {
        check_and_make_aggregate<AggGroupConcatDistinct>(expr.expr.get(), expr.separator);
    } else {
        check_and_make_aggregate<AggGroupConcat>(expr.expr.get(), expr.separator);
    }
}

template<typename AggType, class ... Args>
void ExprToBindingExpr::check_and_make_aggregate(Expr* expr, Args&&... args) {
    if (bic == nullptr) {
        throw QuerySemanticException("Aggregation where it is not allowed");
    }
    if (!bic->grouping) {
        throw QuerySemanticException("Aggregation where it is not allowed");
    }
    if (inside_aggregation) {
        throw QuerySemanticException("Nested aggregations are not allowed");
    }

    VarId var(0);
    if (at_root && as_var.has_value()) {
        // If we are at the root of the expression, and the expressions is associated with a variable
        // (<expr> AS <var>), then we can avoid creating an unnecessary internal variable.
        var = as_var.value();
    } else {
        // Otherwise we have to create an internal variable to associated with the aggregation.
        var = get_query_ctx().get_internal_var();
    }

    at_root = false;
    inside_aggregation = true;

    std::unique_ptr<AggType> agg;
    if (expr) {
        expr->accept_visitor(*this);
        agg = make_unique<AggType>(var, std::move(tmp), std::forward<Args>(args)...);
    } else {
        agg = make_unique<AggType>(var, nullptr, std::forward<Args>(args)...);
    }

    bic->aggregations.insert({var, std::move(agg)});

    tmp = make_unique<BindingExprVar>(var);

    // We are done with this branch of the expressions tree.
    // This aggregation does not count for any other branches, which have their own aggregations.
    inside_aggregation = false;
}

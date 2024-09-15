#include "sparql_binding_expr_printer.h"


namespace SPARQL {


void BindingExprPrinter::visit(BindingExprNot& expr) {
    os << "NOT(";
    expr.expr->accept_visitor(*this);
    os << ")";
}


void BindingExprPrinter::visit(BindingExprUnaryMinus& expr) {
    os << "-(";
    expr.expr->accept_visitor(*this);
    os << ')';
}


void BindingExprPrinter::visit(BindingExprUnaryPlus& expr) {
    os << "+(";
    expr.expr->accept_visitor(*this);
    os << ')';
}


void BindingExprPrinter::visit(BindingExprMultiplication& expr) {
    os << '(';
    expr.lhs->accept_visitor(*this);
    os << " * ";
    expr.rhs->accept_visitor(*this);
    os << ')';
}


void BindingExprPrinter::visit(BindingExprDivision& expr) {
    os << '(';
    expr.lhs->accept_visitor(*this);
    os << " / ";
    expr.rhs->accept_visitor(*this);
    os << ')';
}


void BindingExprPrinter::visit(BindingExprAddition& expr) {
    os << '(';
    expr.lhs->accept_visitor(*this);
    os << " + ";
    expr.rhs->accept_visitor(*this);
    os << ')';
}


void BindingExprPrinter::visit(BindingExprSubtraction& expr) {
    os << '(';
    expr.lhs->accept_visitor(*this);
    os << " - ";
    expr.rhs->accept_visitor(*this);
    os << ')';
}


void BindingExprPrinter::visit(BindingExprAnd& expr) {
    os << '(';
    expr.lhs->accept_visitor(*this);
    os << " AND ";
    expr.rhs->accept_visitor(*this);
    os << ')';
}


void BindingExprPrinter::visit(BindingExprOr& expr) {
    os << '(';
    expr.lhs->accept_visitor(*this);
    os << " OR ";
    expr.rhs->accept_visitor(*this);
    os << ')';
}


void BindingExprPrinter::visit(BindingExprLess& expr) {
    os << '(';
    expr.lhs->accept_visitor(*this);
    os << " < ";
    expr.rhs->accept_visitor(*this);
    os << ')';
}


void BindingExprPrinter::visit(BindingExprGreater& expr) {
    os << '(';
    expr.lhs->accept_visitor(*this);
    os << " > ";
    expr.rhs->accept_visitor(*this);
    os << ')';
}


void BindingExprPrinter::visit(BindingExprEquals& expr) {
    os << '(';
    expr.lhs->accept_visitor(*this);
    os << " = ";
    expr.rhs->accept_visitor(*this);
    os << ')';
}


void BindingExprPrinter::visit(BindingExprLessOrEqual& expr) {
    os << '(';
    expr.lhs->accept_visitor(*this);
    os << " <= ";
    expr.rhs->accept_visitor(*this);
    os << ')';
}


void BindingExprPrinter::visit(BindingExprGreaterOrEqual& expr) {
    os << '(';
    expr.lhs->accept_visitor(*this);
    os << " >= ";
    expr.rhs->accept_visitor(*this);
    os << ')';
}


void BindingExprPrinter::visit(BindingExprIn& expr) {
    os << '(';
    expr.lhs_expr->accept_visitor(*this);
    os << " IN (";
    auto first = true;
    for (auto& e : expr.exprs) {
        if (first) first = false; else os << ", ";
        e->accept_visitor(*this);
    }
    os << "))";
}


void BindingExprPrinter::visit(BindingExprAbs& expr) {
    os << "ABS(";
    expr.expr->accept_visitor(*this);
    os << ')';
}


void BindingExprPrinter::visit(BindingExprBNode& expr) {
    os << "BNODE(";
    if (expr.expr != nullptr) {
        expr.expr->accept_visitor(*this);
    }
    os << ')';
}


void BindingExprPrinter::visit(BindingExprBound& expr) {
    os << "BOUND(?" << get_query_ctx().get_var_name(expr.var_id) << ')';

}


void BindingExprPrinter::visit(BindingExprCeil& expr) {
    os << "CEIL(";
    expr.expr->accept_visitor(*this);
    os << ')';
}


void BindingExprPrinter::visit(BindingExprCoalesce& expr) {
    os << "COALESCE(";
    auto first = true;
    for (auto& expr : expr.exprs) {
        if (first) first = false; else os << ", ";
        expr->accept_visitor(*this);
    }
    os << ')';
}


void BindingExprPrinter::visit(BindingExprConcat& expr) {
    os << "CONCAT(";
    bool first = true;
    for (auto& expr : expr.exprs) {
        if (first) first = false; else os << ", ";
        expr->accept_visitor(*this);
    }
    os << ')';
}


void BindingExprPrinter::visit(BindingExprContains& expr) {
    os << "CONTAINS(";
    expr.lhs->accept_visitor(*this);
    os << ", ";
    expr.rhs->accept_visitor(*this);
    os << ')';
}


void BindingExprPrinter::visit(BindingExprDatatype& expr) {
    os  << "DATATYPE(";
    expr.expr->accept_visitor(*this);
    os << ')';
}


void BindingExprPrinter::visit(BindingExprDay& expr) {
    os  << "DAY(";
    expr.expr->accept_visitor(*this);
    os << ')';
}


void BindingExprPrinter::visit(BindingExprEncodeForUri& expr) {
    os  << "ENCODE_FOR_URI(";
    expr.expr->accept_visitor(*this);
    os << ')';
}


void BindingExprPrinter::visit(BindingExprExists& expr) {
    os << "EXISTS(_Op_" << ops.size() << "_)";
    ops.push_back(expr.op_iter.get());
}


void BindingExprPrinter::visit(BindingExprFloor& expr) {
    os  << "FLOOR(";
    expr.expr->accept_visitor(*this);
    os << ')';
}


void BindingExprPrinter::visit(BindingExprHours& expr) {
    os  << "HOURS(";
    expr.expr->accept_visitor(*this);
    os << ')';
}


void BindingExprPrinter::visit(BindingExprIf& expr) {
    os << "IF(";
    expr.expr_cond->accept_visitor(*this);
    os << ", ";
    expr.expr_then->accept_visitor(*this);
    os << ", ";
    expr.expr_else->accept_visitor(*this);
    os << ')';
}

void BindingExprPrinter::visit(BindingExprIRI& expr) {
    os  << "IRI(";
    expr.expr->accept_visitor(*this);
    os << ')';
}


void BindingExprPrinter::visit(BindingExprIsBlank& expr) {
    os  << "isBLANK(";
    expr.expr->accept_visitor(*this);
    os << ')';
}


void BindingExprPrinter::visit(BindingExprIsIRI& expr) {
    os  << "isIRI(";
    expr.expr->accept_visitor(*this);
    os << ')';
}


void BindingExprPrinter::visit(BindingExprIsLiteral& expr) {
    os  << "isLITERAL(";
    expr.expr->accept_visitor(*this);
    os << ')';
}


void BindingExprPrinter::visit(BindingExprIsNumeric& expr) {
    os  << "isNUMERIC(";
    expr.expr->accept_visitor(*this);
    os << ')';
}


void BindingExprPrinter::visit(BindingExprLang& expr) {
    os  << "LANG(";
    expr.expr->accept_visitor(*this);
    os << ')';
}


void BindingExprPrinter::visit(BindingExprLangMatches& expr) {
    os << "LANGMATCHES(";
    expr.lhs->accept_visitor(*this);
    os << ", ";
    expr.rhs->accept_visitor(*this);
    os << ')';
}


void BindingExprPrinter::visit(BindingExprLCase& expr) {
    os  << "LCASE(";
    expr.expr->accept_visitor(*this);
    os << ')';
}


void BindingExprPrinter::visit(BindingExprMD5& expr) {
    os  << "MD5(";
    expr.expr->accept_visitor(*this);
    os << ')';
}


void BindingExprPrinter::visit(BindingExprMinutes& expr) {
    os  << "MINUTES(";
    expr.expr->accept_visitor(*this);
    os << ')';
}


void BindingExprPrinter::visit(BindingExprMonth& expr) {
    os  << "MONTH(";
    expr.expr->accept_visitor(*this);
    os << ')';
}


void BindingExprPrinter::visit(BindingExprNotExists& expr) {
    os << "NOT EXISTS(_Op_" << ops.size() << "_)";
    ops.push_back(expr.op_iter.get());
}



void BindingExprPrinter::visit(BindingExprRegex& expr) {
    os << "REGEX(";
    expr.expr1->accept_visitor(*this);
    os << ", ";
    expr.expr2->accept_visitor(*this);
    if (expr.expr3 != nullptr) {
        os << ", ";
        expr.expr3->accept_visitor(*this);
    }
    os << ')';
}


void BindingExprPrinter::visit(BindingExprReplace& expr) {
    os << "REPLACE(";
    expr.expr1->accept_visitor(*this);
    os << ", ";
    expr.expr2->accept_visitor(*this);
    os << ", ";
    expr.expr3->accept_visitor(*this);
    if (expr.expr4 != nullptr) {
        os << ", ";
        expr.expr4->accept_visitor(*this);
    }
    os << ')';
}


void BindingExprPrinter::visit(BindingExprRound& expr) {
    os  << "ROUND(";
    expr.expr->accept_visitor(*this);
    os << ')';
}


void BindingExprPrinter::visit(BindingExprSameTerm& expr) {
    os << "sameTERM(";
    expr.lhs->accept_visitor(*this);
    os << ", ";
    expr.rhs->accept_visitor(*this);
    os << ')';
}


void BindingExprPrinter::visit(BindingExprSeconds& expr) {
    os  << "SECONDS(";
    expr.expr->accept_visitor(*this);
    os << ')';
}


void BindingExprPrinter::visit(BindingExprSHA1& expr) {
    os  << "SHA1(";
    expr.expr->accept_visitor(*this);
    os << ')';
}


void BindingExprPrinter::visit(BindingExprSHA256& expr) {
    os  << "SHA256(";
    expr.expr->accept_visitor(*this);
    os << ')';
}


void BindingExprPrinter::visit(BindingExprSHA384& expr) {
    os  << "SHA384(";
    expr.expr->accept_visitor(*this);
    os << ')';
}


void BindingExprPrinter::visit(BindingExprSHA512& expr) {
    os  << "SHA521(";
    expr.expr->accept_visitor(*this);
    os << ')';
}


void BindingExprPrinter::visit(BindingExprStrAfter& expr) {
    os << "STRAFTER(";
    expr.lhs->accept_visitor(*this);
    os << ", ";
    expr.rhs->accept_visitor(*this);
    os << ')';
}


void BindingExprPrinter::visit(BindingExprStrBefore& expr) {
    os << "STRBEFORE(";
    expr.lhs->accept_visitor(*this);
    os << ", ";
    expr.rhs->accept_visitor(*this);
    os << ')';
}


void BindingExprPrinter::visit(BindingExprStrDT& expr) {
    os << "STRDT(";
    expr.expr1->accept_visitor(*this);
    os << ", ";
    expr.expr2->accept_visitor(*this);
    os << ')';
}


void BindingExprPrinter::visit(BindingExprStrEnds& expr) {
    os << "STRENDS(";
    expr.lhs->accept_visitor(*this);
    os << ", ";
    expr.rhs->accept_visitor(*this);
    os << ')';
}


void BindingExprPrinter::visit(BindingExprStrLang& expr) {
    os << "STRLANG(";
    expr.expr1->accept_visitor(*this);
    os << ", ";
    expr.expr2->accept_visitor(*this);
    os << ')';
}


void BindingExprPrinter::visit(BindingExprStrLen& expr) {
    os  << "STRLEN(";
    expr.expr->accept_visitor(*this);
    os << ')';
}


void BindingExprPrinter::visit(BindingExprStrStarts& expr) {
    os << "STRSTARTS(";
    expr.lhs->accept_visitor(*this);
    os << ", ";
    expr.rhs->accept_visitor(*this);
    os << ')';
}


void BindingExprPrinter::visit(BindingExprStr& expr) {
    os  << "STR(";
    expr.expr->accept_visitor(*this);
    os << ')';
}


void BindingExprPrinter::visit(BindingExprSubStr& expr) {
    os << "SUBSTR(";
    expr.expr_str->accept_visitor(*this);
    os << ", ";
    expr.expr_start->accept_visitor(*this);
    if (expr.expr_length != nullptr) {
        os << ", ";
        expr.expr_length->accept_visitor(*this);
    }
    os << ')';
}


void BindingExprPrinter::visit(BindingExprTimezone& expr) {
    os  << "TIMEZONE(";
    expr.expr->accept_visitor(*this);
    os << ')';
}


void BindingExprPrinter::visit(BindingExprTZ& expr) {
    os  << "TZ(";
    expr.expr->accept_visitor(*this);
    os << ')';
}


void BindingExprPrinter::visit(BindingExprUCase& expr) {
    os  << "UCASE(";
    expr.expr->accept_visitor(*this);
    os << ')';
}


void BindingExprPrinter::visit(BindingExprYear& expr) {
    os  << "YEAR(";
    expr.expr->accept_visitor(*this);
    os << ')';
}


void BindingExprPrinter::visit(BindingExprCast& expr) {
    os << cast_type_to_string(expr.cast_type) << '(';
    expr.expr->accept_visitor(*this);
    os << ')';
}


} // namespace SPARQL
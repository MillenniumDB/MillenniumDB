#include "query_ast_printer.h"

using namespace query::ast;

QueryAstPrinter::QueryAstPrinter(std::ostream& out) :
    out(out),
    base_indent(0) { }


QueryAstPrinter::QueryAstPrinter(std::ostream& out, int_fast32_t base_indent) :
    out(out),
    base_indent(base_indent) { }


void QueryAstPrinter::indent(std::string str) const {
    int_fast32_t spaces = base_indent * tab_size;
    for (int_fast32_t i = 0; i < spaces; i++) {
        out << " ";
    }
    out << str;
}


void QueryAstPrinter::indent() const {
    int_fast32_t spaces = base_indent * tab_size;
    for (int_fast32_t i = 0; i < spaces; i++) {
        out << " ";
    }
}


void QueryAstPrinter::indent(std::string str, int_fast32_t extra_indent) const {
    int_fast32_t spaces = (base_indent + extra_indent) * tab_size;
    for (int_fast32_t i = 0; i < spaces; i++) {
        out << " ";
    }
    out << str;
}


void QueryAstPrinter::operator()(Root const& r) const {
    indent("{\n");
    auto printer = QueryAstPrinter(out, base_indent+1);
    auto printer2 = QueryAstPrinter(out, base_indent+2);

    if (r.select.distinct) {
        printer.indent("\"SELECT DISTINCT\": [\n");
    } else {
        printer.indent("\"SELECT\": \"");
    }
    printer2(r.select.selection);
    out << "\",\n";

    printer.indent("\"MATCH\": [\n");
    printer2(r.graph_pattern);
    printer.indent("],\n");

    if (r.where) {
        printer.indent("\"WHERE\": \"");
        printer(r.where.get());
        out << "\",\n";
    }

    if (r.group_by) {
        printer.indent("\"GROUP BY\": ");
        auto item_list = r.group_by.get();
        printer(item_list);
        out << ",\n";
    }

    if (r.order_by) {
        printer.indent("\"ORDER BY\": ");
        auto item_list = r.order_by.get();
        printer(item_list);
        out << ",\n";
    }

    if (r.limit) {
        printer.indent("\"LIMIT\": ");
        out << r.limit.get();
    } else {
        printer.indent("\"LIMIT\": null");
    }
    indent("\n}\n");
}


void QueryAstPrinter::operator()(SelectItem const& select_item) const {
    out << select_item.var;
    if (select_item.key) {
        out << "." << select_item.key.get();
    }
}


void QueryAstPrinter::operator()(std::vector<SelectItem> const& select_items) const {
    auto it = select_items.begin();
    while (it != select_items.end()) {
        (*this)(*it);
        ++it;
        if (it != select_items.end()) {
            out << ", ";
        }
    }
}


void QueryAstPrinter::operator()(std::vector<query::ast::OrderedSelectItem> const& ordered_select_items) const {
    out << "\"";
    auto it = ordered_select_items.begin();
    while (it != ordered_select_items.end()) {
        (*this)(it->item);
        if (it->order == query::ast::Order::Ascending) {
            out << " ASC";
        } else {
            out << " DESC";
        }
        ++it;
        if (it != ordered_select_items.end()) {
            out << ", ";
        }
    }
    out << "\"";
}


void QueryAstPrinter::operator() (std::vector<LinearPattern> const& graph_pattern) const {
    auto printer = QueryAstPrinter(out, base_indent+1);
    auto it = graph_pattern.begin();
    while (it != graph_pattern.end()) {
        printer.indent();
        printer(*it);
        ++it;
        if (it != graph_pattern.end()) {
            out << ",\n";
        }
        else {
            out << "\n";
        }
    }
}


void QueryAstPrinter::operator() (GraphPattern const& graph_pattern) const {
    indent("\"MainPattern\": [\n");
    (*this)(graph_pattern.pattern);
    indent("],\n");

    auto printer = QueryAstPrinter(out, base_indent+1);
    auto printer2 = QueryAstPrinter(out, base_indent+2);
    if (graph_pattern.optionals.size() > 0) {
        indent("\"OptionalPatterns\": [\n");\
        for (auto const& optional_pattern : graph_pattern.optionals) {
            printer.indent("{\n");
            printer2(optional_pattern.get());
            printer.indent("}\n");
        }
        indent("]\n");
    } else {
        indent("\"OptionalPatterns\": [ ]\n");
    }
}


void QueryAstPrinter::operator() (LinearPattern const& linear_pattern) const {
    out << "{\n";
    auto printer = QueryAstPrinter(out, base_indent+1);
    printer.indent();
    printer(linear_pattern.root);
    for (auto const& step_path : linear_pattern.path) {
        out << ",\n";
        printer.indent();
        printer(step_path);
    }
    out << "\n";
    indent("}");
}


void QueryAstPrinter::operator() (LinearPatternStep const& step) const {
    boost::apply_visitor(*this, step.path);
    out << ",\n";
    indent();
    (*this)(step.node);
}


void QueryAstPrinter::operator() (Node const& node) const {
    out << "\"NODE\": {\n";
    indent("\"VAR_OR_ID\": ", 1);
    // out << "\"" << node.var_or_id << "\",\n"; TODO:
    indent("\"LABELS\": [", 1);
    auto label_iter = node.labels.begin();
    while (label_iter != node.labels.end()) {
        out << "\"" << *label_iter << "\"";
        ++label_iter;
        if (label_iter != node.labels.end()) {
            out << ", ";
        }
    }
    out << "],\n";
    if (node.properties.size() > 0) {
        indent("\"PROPERTIES\": [\n", 1);
        auto prop_iter = node.properties.begin();
        while (prop_iter != node.properties.end()) {
            indent("{ \"KEY\": ", 2);
            out << "\"" << (*prop_iter).key << "\"";
            out << ", \"VALUE\": ";
            boost::apply_visitor(*this, (*prop_iter).value);
            ++prop_iter;
            if (prop_iter != node.properties.end()) {
                out << " },\n";
            }
            else {
                out << " }\n";
            }
        }
        indent("]\n", 1);
    }
    else {
        indent("\"PROPERTIES\": []\n", 1);
    }
    indent("}");
}


void QueryAstPrinter::operator() (Edge const& edge) const {
    out << "\"EDGE\": {\n";
    indent("\"DIRECTION\": ", 1);
    if (edge.direction == EdgeDirection::right) {
        out << "\"RIGHT\",\n";
    } else {
        out << "\"LEFT\",\n";
    }
    indent("\"ID\": ", 1);
    out << "\"" << edge.id << "\",\n";
    indent("\"TYPES\": [", 1);
    auto label_iter = edge.types.begin();
    while (label_iter != edge.types.end()) {
        out << "\"" << *label_iter << "\"";
        ++label_iter;
        if (label_iter != edge.types.end()) {
            out << ", ";
        }
    }
    out << "],\n";
    if (edge.properties.size() > 0) {
        indent("\"PROPERTIES\": [\n", 1);
        auto prop_iter = edge.properties.begin();
        while (prop_iter != edge.properties.end()) {
            indent("{ \"KEY\": ", 2);
            out << "\"" << (*prop_iter).key << "\"";
            out << ", \"VALUE\": ";
            boost::apply_visitor(*this, (*prop_iter).value);
            ++prop_iter;
            if (prop_iter != edge.properties.end()) {
                out << " },\n";
            }
            else {
                out << " }\n";
            }
        }
        indent("]\n", 1);
    }
    else {
        indent("\"PROPERTIES\": []\n", 1);
    }
    indent("}");
}


void QueryAstPrinter::operator() (PropertyPath const& property_path) const {
    out << "\"PROPERTY_PATH\": {\n";
    indent("\"DIRECTION\": ", 1);
    if (property_path.direction == EdgeDirection::right) {
        out << "\"RIGHT\",\n";
    } else {
        out << "\"LEFT\",\n";
    }
    indent("\"PATH\": \"", 1);
    (*this)(property_path.path_alternatives);
    out << "\"\n";
    indent("}");
}


void QueryAstPrinter::operator() (PropertyPathAlternatives const& path) const {
    if (path.alternatives.size() > 1)
        out << "(";
    (*this)(path.alternatives[0]);
    for (unsigned i = 1; i < path.alternatives.size(); i++) {
        out << " | ";
        (*this)(path.alternatives[i]);
    }
    if (path.alternatives.size() > 1)
        out << ")";
}


void QueryAstPrinter::operator() (PropertyPathSequence const& path) const {
    if (path.atoms.size() > 1)
        out << "(";
    (*this)(path.atoms[0]);
    for (unsigned i = 1; i < path.atoms.size(); i++) {
        out << " / ";
        (*this)(path.atoms[i]);
    }
    if (path.atoms.size() > 1)
        out << ")";
}


void QueryAstPrinter::operator() (PropertyPathAtom const& atom) const {
    if (atom.inverse) {
        out << "^";
    }
    boost::apply_visitor(*this, atom.atom);
    boost::apply_visitor(*this, atom.suffix);
}


void QueryAstPrinter::operator() (PropertyPathSuffix const& suffix) const {
    switch (suffix) {
    case PropertyPathSuffix::ONE_OR_MORE :
        out << "+";
        break;
    case PropertyPathSuffix::ZERO_OR_ONE :
        out << "?";
        break;
    case PropertyPathSuffix::ZERO_OR_MORE :
        out << "*";
        break;
    case PropertyPathSuffix::NONE :
        break;
    }
}


void QueryAstPrinter::operator() (PropertyPathBoundSuffix const& suffix) const{
    out << "{" << suffix.min << ", " << suffix.max << "}";
}


void QueryAstPrinter::operator()(Statement const& statement) const {
    (*this)(statement.lhs);
    (*this)(statement.comparator);
    boost::apply_visitor(*this, statement.rhs);
}


void QueryAstPrinter::operator()(FormulaDisjunction const& f) const {
    if (f.formula_conjunctions.size() == 1) {
        (*this)(f.formula_conjunctions[0]);
    } else {
        out << "(";
        (*this)(f.formula_conjunctions[0]);
        for (unsigned i = 1; i < f.formula_conjunctions.size(); i++) {
            out << " OR ";
            (*this)(f.formula_conjunctions[i]);
        }
        out << ")";
    }
}


void QueryAstPrinter::operator()(FormulaConjunction const& f) const {
    if (f.formulas.size() == 1) {
        (*this)(f.formulas[0]);
    } else {
        out << "(";
        (*this)(f.formulas[0]);
        for (unsigned i = 1; i < f.formulas.size(); i++) {
            out << " AND ";
            (*this)(f.formulas[i]);
        }
        out << ")";
    }
}


void QueryAstPrinter::operator()(AtomicFormula const& f) const {
    if (f.negation) {
        out << "NOT ";
    }
    boost::apply_visitor(*this, f.content);
}


void QueryAstPrinter::operator()(Value const& v) const {
    boost::apply_visitor(*this, v);
}


void QueryAstPrinter::operator()(std::string const& text) const {
    out << "\"" << text << "\"";
}

void QueryAstPrinter::operator() (Comparator const& c) const {
    switch(c) {
        case Comparator::EQ :
            out << " == ";
            break;
        case Comparator::NE :
            out << " != ";
            break;
        case Comparator::GT :
            out << " > ";
            break;
        case Comparator::GE :
            out << " >= ";
            break;
        case Comparator::LT :
            out << " < ";
            break;
        case Comparator::LE :
            out << " <= ";
            break;
    };
}


void QueryAstPrinter::operator() (int64_t const& n) const {out << n; }
void QueryAstPrinter::operator() (float const& n)   const {out << n; }

void QueryAstPrinter::operator() (bool const& b) const {
    if (b)
        out << "true";
    else
        out << "false";
}

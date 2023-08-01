#include "no_free_variable_minus.h"

void NoFreeVariableMinus::begin(Binding& parent_binding) {
    rhs->begin(parent_binding);
    lhs->begin(parent_binding);
    has_result = rhs->next();
}


bool NoFreeVariableMinus::next() {
    if (!has_result) {
        return lhs->next();
    }
    return false;
}


void NoFreeVariableMinus::reset() {
    rhs->reset();
    if (rhs->next()) {
        has_result = true;
    }
    else {
        has_result = false;
        lhs->reset();
    }
}


void NoFreeVariableMinus::assign_nulls() {
    lhs->assign_nulls();
    // Rhs is not in the current scope, so we don't assign nulls here.
}


void NoFreeVariableMinus::analyze(std::ostream& os, int indent) const {
    os << std::string(indent, ' ');
    os << "NoFreeVariableMinus(";
        os << "\n";
        lhs->analyze(os, indent + 2);
        rhs->analyze(os, indent + 2);
    os << "\n" << std::string(indent, ' ') << ")";
}

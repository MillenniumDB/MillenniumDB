#include "filter.h"

#include "graph_models/rdf_model/conversions.h"

void Filter::begin(Binding& _parent_binding) {
    parent_binding = &_parent_binding;
    child->begin(_parent_binding);
}

// TODO: this filter is designed for SPARQL, for MQL we should create a new one?
// or maybe just inject the behaviour to evaluate
// `!Conversions::to_boolean(evaluation).is_true()`
bool Filter::next() {
    while (child->next()) {
        bool pass_filters = true;
        for (auto& filter : filters) {
            auto evaluation = filter->eval(*parent_binding);
            if (!SPARQL::Conversions::to_boolean(evaluation).is_true()) {
                pass_filters = false;
                break;
            }
        }
        if (pass_filters) {
            passed_results++;
            return true;
        } else {
            filtered_results++;
        }
    }
    return false;
}


void Filter::reset() {
    child->reset();
}


void Filter::assign_nulls() {
    child->assign_nulls();
}


void Filter::analyze(std::ostream& os, int indent) const {
    os << std::string(indent, ' ');
    os << "Filter(filtered: " << filtered_results << ", passed: " << passed_results;

    for (auto& filter : filters) {
        os << ", " << *filter;
    }

    os << ")\n";
    child->analyze(os, indent+2);
}

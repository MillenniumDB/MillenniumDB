#include "sparql_element.h"

#include "sparql_element_duplicate.h"

SparqlElement SparqlElement::duplicate() const {
    return std::visit(SparqlElementDuplicate(), value);
}

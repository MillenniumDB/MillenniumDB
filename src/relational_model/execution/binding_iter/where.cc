#include "where.h"

#include "base/binding/binding.h"
#include "base/graph/condition/condition.h"

using namespace std;

Where::Where(const GraphModel& model,
             unique_ptr<BindingIter> _child_iter,
             unique_ptr<Condition> condition) :
    model      (model),
    child_iter (move(_child_iter)),
    condition  (move(condition)),
    my_binding (BindingWhere(model, child_iter->get_binding())) { }


void Where::begin() {
    child_iter->begin();
}


bool Where::next() {
    while (child_iter->next()) {
        // my_binding.clear_cache();
        if (condition->eval(my_binding)) {
            ++results;
            return true;
        }
    }
    return false;
}


void Where::analyze(std::ostream& os, int indent) const {
    os << std::string(indent, ' ');
    os << "Where(found: " << results << ")\n";
    child_iter->analyze(os, indent);
}

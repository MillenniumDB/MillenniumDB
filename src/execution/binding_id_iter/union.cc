#include "union.h"

using namespace std;

Union::Union(std::vector<std::unique_ptr<BindingIdIter>> iters) :
    iters(move(iters)) { }


void Union::begin(BindingId& parent_binding) {
    current_iter = 0;
    for (auto& iter : iters) {
        iter->begin(parent_binding);
    }
}


bool Union::next() {
    while (current_iter < iters.size()) {
        if (iters[current_iter]->next()) {
            return true;
        } else {
            ++current_iter;
        }
    }
    return false;
}


void Union::reset() {
    current_iter = 0;
    for (auto& iter : iters) {
        iter->reset();
    }
}


void Union::assign_nulls() {
    iters[current_iter]->assign_nulls();
}


void Union::analyze(std::ostream& os, int indent) const {
    os << std::string(indent, ' ');
    os << "Union(";
    for (const auto& iter : iters) {
        os << "\n";
        iter->analyze(os, indent + 2);
    }
    os << "\n" << std::string(indent, ' ') << ")";
}

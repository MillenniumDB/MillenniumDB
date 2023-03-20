#include "optimize_optional_tree.h"

#include "base/exceptions.h"
#include "parser/query/op/sparql/ops.h"

using namespace std;
using namespace SPARQL;

void OptimizeOptionalTree::visit(OpOptional& op_optional) {
    op_optional.op->accept_visitor(*this);
    bool current_move_children_up = false;
    if (delete_current) {
        current_move_children_up = true;
    }
    delete_current    = false;
    move_children_up  = false;
    optional_to_match = false;
    for (auto it = op_optional.optionals.begin(); it != op_optional.optionals.end(); ) {
        (*it)->accept_visitor(*this);
        if (move_children_up || optional_to_match) {
            it = op_optional.optionals.erase(it);
            it = op_optional.optionals.insert(it,
                                              std::make_move_iterator(optionals.begin()),
                                              std::make_move_iterator(optionals.end()));
            advance(it, optionals.size());
            optionals.clear();
            // reset
            move_children_up = false;
            optional_to_match = false;
        }
        else if (delete_current) {
            it = op_optional.optionals.erase(it);
            delete_current = false;
        }
        else {
            ++it;
        }
    }

    if (op_optional.optionals.size() == 0) {
        if (current_move_children_up) {
            move_children_up = false;
            delete_current   = true;
        } else {
            move_children_up  = false;
            delete_current    = false;
            optional_to_match = true;
            optionals.emplace_back(move(op_optional.op));
        }

    } else if(current_move_children_up) {
        move_children_up = true;
        delete_current   = false;
        for (auto it=op_optional.optionals.begin(); it!=op_optional.optionals.end(); ) {
            optionals.emplace_back(move(*it));
            it = op_optional.optionals.erase(it);
        }
    }
}

void OptimizeOptionalTree::visit(OpTriples& op_triples) {
    if (op_triples.triples.empty()) {
        delete_current = true;
    }
    for (auto& var : op_triples.get_vars()) {
        global_vars.insert(var);
    }
}

void OptimizeOptionalTree::visit(OpSelect& op_select) {
    op_select.op->accept_visitor(*this);
}

void OptimizeOptionalTree::visit(OpWhere& op_where) {
    optional_to_match = false;
    op_where.op->accept_visitor(*this);
    if (optional_to_match) {
        op_where.op = move(optionals[0]);
    }
}
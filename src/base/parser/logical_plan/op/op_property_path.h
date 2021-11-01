#ifndef BASE__OP_PROPERTY_PATH_H_
#define BASE__OP_PROPERTY_PATH_H_

#include <string>
#include <memory>

#include "base/parser/logical_plan/op/op.h"
#include "base/parser/logical_plan/op/op_path.h"
#include "base/parser/logical_plan/op/property_paths/path_automaton.h"

class OpPropertyPath : public Op {
public:
    const Var var;
    const NodeId from;
    const NodeId to;

    std::unique_ptr<OpPath> path;

    OpPropertyPath(Var var, NodeId from, NodeId to, std::unique_ptr<OpPath> path) :
        var  (std::move(var)),
        from (std::move(from)),
        to   (std::move(to)),
        path (std::move(path)) { }

    void accept_visitor(OpVisitor& visitor) override {
        visitor.visit(*this);
    }

    bool operator<(const OpPropertyPath& other) const {
        if (from < other.from) {
            return true;
        } else if (to < other.to) {
            return true;
        } else if (*path < *other.path) {
            return true;
        }
        return false;
    }

    std::set<Var> get_vars() const override {
        std::set<Var> res;
        if (from.is_var()) {
            res.insert(from.to_var());
        }
        if (to.is_var()) {
            res.insert(to.to_var());
        }
        res.insert(var);
        return res;
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent=0) const override {
        os << std::string(indent, ' ');
        os << "OpPropertyPath(" << from << "=>" << to <<")\n";
        os << *path << "\n";
        return os;
    };
};

#endif // BASE__OP_PROPERTY_PATH_H_

#pragma once

#include <string>

#include "base/query/query_element.h"
#include "parser/query/op/op.h"

namespace MDB {

class OpDescribe : public Op {
public:
    QueryElement node;

    OpDescribe(const std::string& node_str) : node(QueryElement::deduce(node_str)) { }

    void accept_visitor(OpVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::set<Var> get_vars() const override {
        std::set<Var> res;
        return res;
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        os << std::string(indent, ' ');
        os << "OpDescribe(" << node << ")\n";
        return os;
    }
};
} // namespace MDB
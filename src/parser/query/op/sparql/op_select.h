#pragma once

#include <memory>
#include <vector>

#include "base/query/var.h"
#include "parser/query/op/op.h"

namespace SPARQL {

class OpSelect : public Op {
public:
    static constexpr uint64_t DEFAULT_LIMIT  = UINT64_MAX;
    static constexpr uint64_t DEFAULT_OFFSET = 0;

    std::unique_ptr<Op> op;
    std::vector<Var>    vars;

    const bool distinct;

    const uint64_t limit;
    const uint64_t offset;

    OpSelect(std::unique_ptr<Op> op, std::vector<Var> vars, bool distinct, uint64_t limit, uint64_t offset) :
        op(std::move(op)), vars(std::move(vars)), distinct(distinct), limit(limit), offset(offset) { }

    void accept_visitor(OpVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::set<Var> get_vars() const override {
        return op->get_vars();
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        os << std::string(indent, ' ');
        os << "OpSelect(";
        if (distinct) {
            os << "DISTINCT ";
        }
        bool first = true;
        for (auto& var : vars) {
            if (!first) {
                os << ", ";
            }
            first = false;
            os << var;
        }
        if (limit != DEFAULT_LIMIT) {
            os << "; LIMIT " << limit;
        }
        if (offset != DEFAULT_OFFSET) {
            os << "; OFFSET " << offset;
        }
        os << ")\n";
        return op->print_to_ostream(os, indent + 2);
    };
};
} // namespace SPARQL
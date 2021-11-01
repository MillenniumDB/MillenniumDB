#ifndef BASE__OP_OPTIONAL_H_
#define BASE__OP_OPTIONAL_H_

#include <map>
#include <string>
#include <vector>

#include "base/parser/grammar/query/query_ast.h"
#include "base/parser/logical_plan/op/op_basic_graph_pattern.h"
#include "base/parser/logical_plan/op/op.h"


class OpOptional : public Op {
public:
    std::unique_ptr<Op> op;
    std::vector<std::unique_ptr<Op>> optionals;

    OpOptional(OpOptional&& op_optional) :
        op        (std::move(op_optional.op)),
        optionals (std::move(op_optional.optionals)) { }

    OpOptional(const query::ast::GraphPattern& graph_pattern, uint_fast32_t* anon_count) :
        op ( std::make_unique<OpBasicGraphPattern>(graph_pattern.pattern, anon_count) )
    {
        for (auto& optional : graph_pattern.optionals) {
            if (optional.get().optionals.size() == 0) {
                optionals.emplace_back( std::make_unique<OpBasicGraphPattern>(optional.get().pattern, anon_count) );
            } else {
                optionals.emplace_back( std::make_unique<OpOptional>(optional.get(), anon_count) );
            }
        }
    }

    ~OpOptional() = default;


    void accept_visitor(OpVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent=0) const override{
        os << std::string(indent, ' ');
        os << "OpOptional()\n";
        os << std::string(indent + 2, ' ') << "main pattern:\n";
        op->print_to_ostream(os, indent + 4);
        for (auto& optional : optionals) {
            os << std::string(indent + 2, ' ') << "child:\n";
            optional->print_to_ostream(os, indent + 4);
        }
        return os;
    }

    std::set<Var> get_vars() const override {
        auto res = op->get_vars();
        for (auto& child : optionals) {
            for (auto& child_var : child->get_vars()) {
                res.insert(child_var);
            }
        }
        return res;
    }
};
#endif // BASE__OP_OPTIONAL_H_

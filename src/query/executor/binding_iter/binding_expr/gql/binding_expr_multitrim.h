#pragma once

#include "graph_models/gql/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

#include <cassert>
#include <memory>

namespace GQL {
class BindingExprMultiTrim : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> trim_src;
    std::unique_ptr<BindingExpr> delim_str;
    std::string specification;

    BindingExprMultiTrim(
        std::unique_ptr<BindingExpr> trim_src,
        std::unique_ptr<BindingExpr> delim_str,
        std::string specification
    ) :
        trim_src(std::move(trim_src)),
        delim_str(std::move(delim_str)),
        specification(std::move(specification))
    { }

    ObjectId eval(const Binding& binding) override
    {
        auto src_oid = trim_src->eval(binding);
        auto src_generic_type = src_oid.generic_type();

        if (src_generic_type == ObjectGenType::String) {
            auto str = GQL::Conversions::unpack_string(src_oid);
            if (delim_str != nullptr) {
                auto delim_oid = delim_str->eval(binding);
                auto delim_generic_type = delim_oid.generic_type();
                if (delim_generic_type == ObjectGenType::String) {
                    auto delim = GQL::Conversions::unpack_string(delim_oid);
                    if (specification == "LTRIM") {
                        size_t start = str.find_first_not_of(delim);
                        return GQL::Conversions::pack_string(
                            (start == std::string::npos) ? "" : str.substr(start)
                        );
                    } else if (specification == "RTRIM") {
                        size_t end = str.find_last_not_of(delim);
                        return GQL::Conversions::pack_string(
                            (end == std::string::npos) ? "" : str.substr(0, end + 1)
                        );
                    } else {
                        size_t start = str.find_first_not_of(delim);
                        auto tmp = (start == std::string::npos) ? "" : str.substr(start);
                        size_t end = tmp.find_last_not_of(delim);
                        return GQL::Conversions::pack_string(
                            (end == std::string::npos) ? "" : tmp.substr(0, end + 1)
                        );
                    }
                }
                return ObjectId::get_null();
            } else if (delim_str == nullptr) {
                auto delim = " ";
                if (specification == "LTRIM") {
                    size_t start = str.find_first_not_of(delim);
                    return GQL::Conversions::pack_string(
                        (start == std::string::npos) ? "" : str.substr(start)
                    );
                } else if (specification == "RTRIM") {
                    size_t end = str.find_last_not_of(delim);
                    return GQL::Conversions::pack_string(
                        (end == std::string::npos) ? "" : str.substr(0, end + 1)
                    );
                } else {
                    size_t start = str.find_first_not_of(delim);
                    auto tmp = (start == std::string::npos) ? "" : str.substr(start);
                    size_t end = tmp.find_last_not_of(delim);
                    return GQL::Conversions::pack_string(
                        (end == std::string::npos) ? "" : tmp.substr(0, end + 1)
                    );
                }
            }
            return ObjectId::get_null();
        } else {
            return ObjectId::get_null();
        }
    }

    void accept_visitor(BindingExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    void print(std::ostream& os, std::vector<BindingIter*>& ops) const override
    {
        os << specification;
        os << "(";
        trim_src->print(os, ops);
        os << ",";
        if (delim_str == nullptr) {
            os << " ";
        } else {
            delim_str->print(os, ops);
        }
        os << ")";
    }
};
} // namespace GQL

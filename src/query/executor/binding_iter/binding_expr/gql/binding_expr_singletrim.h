#pragma once

#include <cassert>
#include <memory>

#include "query/exceptions.h"
#include "graph_models/gql/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace GQL {
class BindingExprSingleTrim : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> str;
    std::unique_ptr<BindingExpr> single_char;
    std::string specification;

    BindingExprSingleTrim(std::unique_ptr<BindingExpr> str, std::unique_ptr<BindingExpr> single_char, std::string specification) :
        str(std::move(str)),
        single_char(std::move(single_char)),
        specification(std::move(specification))
    { }

    ObjectId eval(const Binding& binding) override
    {
        auto str_oid = str->eval(binding);
        auto str_generic_type = GQL_OID::get_generic_type(str_oid);

        if (str_generic_type == GQL_OID::GenericType::STRING) {
            auto str = GQL::Conversions::unpack_string(str_oid);
            if (single_char != nullptr) {
                auto char_oid = single_char->eval(binding);
                auto char_generic_type = GQL_OID::get_generic_type(char_oid);
                if (char_generic_type == GQL_OID::GenericType::STRING) {
                    auto character = GQL::Conversions::unpack_string(char_oid);
                    if (character.size() <= 1) {
                        if (specification == "LEADING") {
                            size_t start = str.find_first_not_of(character);
                            return GQL::Conversions::pack_string_simple((start == std::string::npos) ? "" : str.substr(start));
                        } else if (specification == "TRAILING") {
                            size_t end = str.find_last_not_of(character);
                            return GQL::Conversions::pack_string_simple((end == std::string::npos) ? "" : str.substr(0, end + 1));
                        } else {
                            size_t start = str.find_first_not_of(character);
                            auto tmp = (start == std::string::npos) ? "" : str.substr(start);
                            size_t end = tmp.find_last_not_of(character);
                            return GQL::Conversions::pack_string_simple((end == std::string::npos) ? "" : tmp.substr(0, end + 1));
                        }
                    }
                    throw QueryExecutionException("data exception — multicharacter trim specification for single trim expression");
                }
                return ObjectId::get_null();
            } else if (single_char == nullptr) {
                auto character = " ";
                if (specification == "LEADING") {
                    size_t start = str.find_first_not_of(character);
                    return GQL::Conversions::pack_string_simple((start == std::string::npos) ? "" : str.substr(start));
                } else if (specification == "TRAILING") {
                    size_t end = str.find_last_not_of(character);
                    return GQL::Conversions::pack_string_simple((end == std::string::npos) ? "" : str.substr(0, end + 1));
                } else {
                    size_t start = str.find_first_not_of(character);
                    auto tmp = (start == std::string::npos) ? "" : str.substr(start);
                    size_t end = tmp.find_last_not_of(character);
                    return GQL::Conversions::pack_string_simple((end == std::string::npos) ? "" : tmp.substr(0, end + 1));
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
};
} // namespace GQL
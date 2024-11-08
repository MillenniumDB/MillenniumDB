#pragma once

#include "query/parser/op/op.h"

namespace MQL {

class OpSimilaritySearch : public Op {
public:
    VarId              object_var;
    VarId              similarity_var;
    std::string        tensor_store_name;
    int64_t            k;
    int64_t            search_k;
    std::vector<float> query_tensor;
    ObjectId           query_object;

    OpSimilaritySearch(VarId                object_var,
                       VarId                similarity_var,
                       std::string&&        tensor_store_name_,
                       int64_t              k_,
                       int64_t              search_k_,
                       std::vector<float>&& query_tensor_,
                       ObjectId             query_object_) :
        object_var        (object_var),
        similarity_var    (similarity_var),
        tensor_store_name (std::move(tensor_store_name_)),
        k                 (k_),
        search_k          (search_k_),
        query_tensor      (std::move(query_tensor_)),
        query_object      (query_object_) {
        assert(((query_tensor.size() == 0) ^ (query_object.id == ObjectId::NULL_ID))
               && "Exactly one of the following must be non-null: query_tensor, query_object");
        };


    bool operator<(const OpSimilaritySearch& other) const {
        if (object_var < other.object_var) {
            return true;
        } else if (other.object_var < object_var) {
            return false;
        } else if (similarity_var < other.similarity_var) {
            return true;
        } else if (other.similarity_var < similarity_var) {
            return false;
        } else if (tensor_store_name < other.tensor_store_name) {
            return true;
        } else if (other.tensor_store_name < tensor_store_name) {
            return false;
        } else if (k < other.k) {
            return true;
        } else if (other.k < k) {
            return false;
        } else if (search_k < other.search_k) {
            return true;
        } else if (other.search_k < search_k) {
            return false;
        } else if (query_object < other.query_object) {
            return true;
        } else if (other.query_object < query_object) {
            return false;
        } else {
            return query_tensor < other.query_tensor;
        }
    }

    std::unique_ptr<Op> clone() const override {
        std::string        tensor_store_name_clone = tensor_store_name;
        std::vector<float> query_tensor_clone      = query_tensor;
        return std::make_unique<OpSimilaritySearch>(object_var,
                                                    similarity_var,
                                                    std::move(tensor_store_name_clone),
                                                    k,
                                                    search_k,
                                                    std::move(query_tensor_clone),
                                                    query_object);
    }

    void accept_visitor(OpVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override {
        return { object_var, similarity_var };
    }

    std::set<VarId> get_scope_vars() const override {
        return get_all_vars();
    }

    std::set<VarId> get_safe_vars() const override {
        return get_all_vars();
    }

    std::set<VarId> get_fixable_vars() const override {
        return {};
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        os << std::string(indent, ' ');
        os << "OpSimilaritySearch(";
        os << object_var << ", ";
        os << similarity_var << ", ";
        os << tensor_store_name << ", ";
        if (query_tensor.empty()) {
            os << query_object << ", ";
        } else {
            os << "query_tensor(" << query_tensor.size() << "), ";
        }
        os << k << ", ";
        os << search_k << ")\n";
        return os;
    }
};
} // namespace MQL

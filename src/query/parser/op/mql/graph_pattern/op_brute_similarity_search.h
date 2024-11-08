#pragma once

#include "query/parser/op/op.h"
#include "storage/index/tensor_store/lsh/metric.h"

namespace MQL {

class OpBruteSimilaritySearch : public Op {
public:
    std::unique_ptr<Op> op;
    VarId               object_var;
    VarId               similarity_var;
    std::string         tensor_store_name;
    std::vector<float>  query_tensor;
    ObjectId            query_object;
    LSH::MetricType     metric_type;
    uint64_t            k;

    OpBruteSimilaritySearch(std::unique_ptr<Op>  op_,
                            VarId                object_var_,
                            VarId                similarity_var_,
                            std::string&&        tensor_store_name_,
                            std::vector<float>&& query_tensor_,
                            ObjectId             query_object_,
                            LSH::MetricType      metric_type_,
                            uint64_t             k_) :
        op                (std::move(op_)),
        object_var        (object_var_),
        similarity_var    (similarity_var_),
        tensor_store_name (std::move(tensor_store_name_)),
        query_tensor      (std::move(query_tensor_)),
        query_object      (query_object_),
        metric_type       (metric_type_),
        k                 (k_) {
        assert(((query_tensor.size() == 0) ^ (query_object.id == ObjectId::NULL_ID))
               && "Exactly one of the following must be non-null: query_tensor, query_object");
        };


    bool operator<(const OpBruteSimilaritySearch& other) const {
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
        } else if (query_object < other.query_object) {
            return true;
        } else if (other.query_object < query_object) {
            return false;
        } else if (k < other.k) {
            return true;
        } else if (other.k < k) {
            return false;
        } else {
            return query_tensor < other.query_tensor;
        }
    }

    std::unique_ptr<Op> clone() const override {
        std::string        tensor_store_name_clone = tensor_store_name;
        std::vector<float> query_tensor_clone      = query_tensor;
        return std::make_unique<OpBruteSimilaritySearch>(op->clone(),
                                                         object_var,
                                                         similarity_var,
                                                         std::move(tensor_store_name_clone),
                                                         std::move(query_tensor_clone),
                                                         query_object,
                                                         metric_type,
                                                         k);
    }

    void accept_visitor(OpVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override {
        std::set<VarId> res = op->get_all_vars();
        res.insert(object_var);
        res.insert(similarity_var);
        return res;
    }

    std::set<VarId> get_scope_vars() const override {
        std::set<VarId> res = op->get_scope_vars();
        res.insert(object_var);
        res.insert(similarity_var);
        return res;
    }

    std::set<VarId> get_safe_vars() const override {
        std::set<VarId> res = op->get_safe_vars();
        res.insert(object_var);
        res.insert(similarity_var);
        return res;
    }

    std::set<VarId> get_fixable_vars() const override {
        return op->get_fixable_vars();
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        os << std::string(indent, ' ');
        os << "OpBruteSimilaritySearch(";
        os << object_var << ", ";
        os << similarity_var << ", ";
        os << tensor_store_name << ", ";
        if (query_tensor.empty()) {
            os << query_object << ", ";
        } else {
            os << "query_tensor(" << query_tensor.size() << "), ";
        }
        os << k << ")\n";
        return op->print_to_ostream(os, indent + 2);
    }
};
} // namespace MQL

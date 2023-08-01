#pragma once

#include "graph_models/rdf_model/conversions.h"
#include "graph_models/rdf_model/datatypes/decimal.h"
#include "query/executor/binding_iter/aggregation/agg.h"
#include "storage/index/hash/distinct_binding_hash/distinct_binding_hash.h"

namespace SPARQL {
class AggSumDistinct : public Agg {
public:
    using Agg::Agg;
    void begin() override {
        hash_table = std::make_unique<DistinctBindingHash<ObjectId>>(1);

        sum_integer = 0;
        sum_decimal = Decimal(0);
        sum_float = 0;
        sum_double = 0;

        type = Conversions::OPTYPE_INTEGER;
    }

    void process() override {
        if (type == Conversions::OPTYPE_INVALID) {
            return;
        }

        auto oid = expr->eval(*binding);
        auto op_type = Conversions::calculate_optype(oid);

        if (op_type == Conversions::OPTYPE_INVALID) {
            type = Conversions::OPTYPE_INVALID;
            return;
        }

        oid_vec[0] = oid;
        if (hash_table->is_in_or_insert(oid_vec)) {
            return;
        }

        if (op_type > type) {
            if (op_type == Conversions::OPTYPE_DECIMAL) {
                sum_decimal = Decimal(sum_integer);
                type = Conversions::OPTYPE_DECIMAL;
            } else if (op_type == Conversions::OPTYPE_FLOAT) {
                if (type == Conversions::OPTYPE_INTEGER) {
                    sum_float = sum_integer;
                } else {
                    sum_float = sum_decimal.to_float();
                }
                type = Conversions::OPTYPE_FLOAT;
            } else {
                if (type == Conversions::OPTYPE_INTEGER) {
                    sum_double = sum_integer;
                } else if (type == Conversions::OPTYPE_DECIMAL) {
                    sum_double = sum_decimal.to_double();
                } else {
                    sum_double = sum_float;
                }
                type = Conversions::OPTYPE_DOUBLE;
            }
        }

        if (type == Conversions::OPTYPE_INTEGER) {
            sum_integer += Conversions::unpack_int(oid);
        } else if (type == Conversions::OPTYPE_DECIMAL) {
            sum_decimal = sum_decimal + Conversions::to_decimal(oid);
        } else if (type == Conversions::OPTYPE_FLOAT) {
            sum_float += Conversions::to_float(oid);
        } else {
            sum_double += Conversions::to_double(oid);
        }
    }

    // indicates the end of a group
    ObjectId get() override {
        if (type == Conversions::OPTYPE_INTEGER) {
            return Conversions::pack_int(sum_integer);
        } else if (type == Conversions::OPTYPE_DECIMAL) {
            return Conversions::pack_decimal(sum_decimal);
        } else if (type == Conversions::OPTYPE_FLOAT) {
            return Conversions::pack_float(sum_float);
        } else if (type == Conversions::OPTYPE_DOUBLE) {
            return Conversions::pack_double(sum_double);
        } else {
            return ObjectId::get_null();
        }
    }

    std::ostream& print_to_ostream(std::ostream& os) const override {
        os << "SUM( DISTINCT" << *expr << ")";
        return os;
    }

private:
    uint_fast8_t type;

    int64_t sum_integer;
    Decimal sum_decimal;
    float   sum_float;
    double  sum_double;

    std::unique_ptr<DistinctBindingHash<ObjectId>> hash_table;

    // Vector to pass oid to the hash table
    std::vector<ObjectId> oid_vec{1};
};
} // namespace SPARQL

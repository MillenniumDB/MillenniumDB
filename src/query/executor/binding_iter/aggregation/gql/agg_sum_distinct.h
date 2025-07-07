#pragma once

#include "graph_models/common/datatypes/decimal.h"
#include "graph_models/gql/conversions.h"
#include "query/executor/binding_iter/aggregation/agg.h"
#include "query/executor/binding_iter/binding_expr/gql_binding_expr_printer.h"
#include "storage/index/hash/distinct_binding_hash/distinct_binding_hash.h"

namespace GQL {
class AggSumDistinct : public Agg {
public:
    using Agg::Agg;

    AggSumDistinct(VarId var_id, std::unique_ptr<BindingExpr> expr) :
        Agg(var_id, std::move(expr)),
        hash_table(1)
    { }

    void begin() override
    {
        hash_table.reset();

        sum_integer = 0;
        sum_decimal = Decimal();
        sum_float = 0;
        sum_double = 0;

        type = Conversions::OpType::INTEGER;
    }

    void process() override
    {
        if (type == Conversions::OpType::INVALID) {
            return;
        }

        auto oid = expr->eval(*binding);
        auto op_type = Conversions::calculate_optype(oid);

        if (op_type == Conversions::OpType::INVALID) {
            type = Conversions::OpType::INVALID;
            return;
        }

        oid_vec[0] = oid;
        if (hash_table.is_in_or_insert(oid_vec)) {
            return;
        }

        if (op_type > type) {
            if (op_type == Conversions::OpType::DECIMAL) {
                sum_decimal = Decimal(sum_integer);
                type = Conversions::OpType::DECIMAL;
            } else if (op_type == Conversions::OpType::FLOAT) {
                if (type == Conversions::OpType::INTEGER) {
                    sum_float = sum_integer;
                } else {
                    sum_float = sum_decimal.to_float();
                }
                type = Conversions::OpType::FLOAT;
            } else {
                if (type == Conversions::OpType::INTEGER) {
                    sum_double = sum_integer;
                } else if (type == Conversions::OpType::DECIMAL) {
                    sum_double = sum_decimal.to_double();
                } else {
                    sum_double = sum_float;
                }
                type = Conversions::OpType::DOUBLE;
            }
        }

        if (type == Conversions::OpType::INTEGER) {
            sum_integer += Conversions::unpack_int(oid);
        } else if (type == Conversions::OpType::DECIMAL) {
            sum_decimal = sum_decimal + Conversions::to_decimal(oid);
        } else if (type == Conversions::OpType::FLOAT) {
            sum_float += Conversions::to_float(oid);
        } else {
            sum_double += Conversions::to_double(oid);
        }
    }

    // indicates the end of a group
    ObjectId get() override
    {
        if (type == Conversions::OpType::INTEGER) {
            return Conversions::pack_int(sum_integer);
        } else if (type == Conversions::OpType::DECIMAL) {
            return Conversions::pack_decimal(sum_decimal);
        } else if (type == Conversions::OpType::FLOAT) {
            return Conversions::pack_float(sum_float);
        } else if (type == Conversions::OpType::DOUBLE) {
            return Conversions::pack_double(sum_double);
        } else {
            return ObjectId::get_null();
        }
    }

    std::ostream& print_to_ostream(std::ostream& os) const override
    {
        os << "SUM( DISTINCT";
        BindingExprPrinter printer(os);
        expr->accept_visitor(printer);
        os << ")";
        return os;
    }

private:
    Conversions::OpType type;

    int64_t sum_integer;
    Decimal sum_decimal;
    float sum_float;
    double sum_double;

    DistinctBindingHash hash_table;

    // Vector to pass oid to the hash table
    std::vector<ObjectId> oid_vec { 1 };
};
} // namespace GQL

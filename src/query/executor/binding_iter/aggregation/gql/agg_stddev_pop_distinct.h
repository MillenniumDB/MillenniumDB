#pragma once

#include <cmath>

#include "graph_models/gql/conversions.h"
#include "query/executor/binding_iter/aggregation/agg.h"
#include "query/executor/binding_iter/binding_expr/gql_binding_expr_printer.h"
#include "storage/index/hash/distinct_binding_hash/distinct_binding_hash.h"

namespace GQL {
class AggStddevPopDistinct : public Agg {
public:
    using Agg::Agg;

    AggStddevPopDistinct(VarId var_id, std::unique_ptr<BindingExpr> expr) :
        Agg (var_id, std::move(expr)),
        hash_table (1) { }

    void begin() override {
        hash_table.reset();

        sum_integer = 0;
        sum_decimal = Decimal(0);
        sum_float = 0;
        sum_double = 0;

        sum_squares_integer = 0;
        sum_squares_decimal = Decimal(0);
        sum_squares_float = 0;
        sum_squares_double = 0;

        type = Conversions::OPTYPE_INTEGER;
        count = 0;
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
        if (hash_table.is_in_or_insert(oid_vec)) {
            return;
        }

        if (op_type > type) {
            if (op_type == Conversions::OPTYPE_DECIMAL) {
                sum_decimal = Decimal(sum_integer);
                sum_squares_decimal = Decimal(sum_squares_integer);
                type = Conversions::OPTYPE_DECIMAL;
            } else if (op_type == Conversions::OPTYPE_FLOAT) {
                if (type == Conversions::OPTYPE_INTEGER) {
                    sum_float = sum_integer;
                    sum_squares_float = sum_squares_integer;
                } else {
                    sum_float = sum_decimal.to_float();
                    sum_squares_float = sum_squares_decimal.to_float();
                }
                type = Conversions::OPTYPE_FLOAT;
            } else {
                if (type == Conversions::OPTYPE_INTEGER) {
                    sum_double = sum_integer;
                    sum_squares_double = sum_squares_integer;
                } else if (type == Conversions::OPTYPE_DECIMAL) {
                    sum_double = sum_decimal.to_double();
                    sum_squares_double = sum_squares_decimal.to_double();
                } else {
                    sum_double = sum_float;
                    sum_squares_double = sum_squares_float;
                }
                type = Conversions::OPTYPE_DOUBLE;
            }
        }

        if (type == Conversions::OPTYPE_INTEGER) {
            auto num = Conversions::unpack_int(oid);
            sum_integer += num;
            sum_squares_integer += pow(num,2);
        } else if (type == Conversions::OPTYPE_DECIMAL) {
            sum_decimal = sum_decimal + Conversions::to_decimal(oid);
            sum_squares_decimal = sum_squares_decimal + Conversions::to_decimal(oid).pow(Decimal(2));
        } else if (type == Conversions::OPTYPE_FLOAT) {
            auto num = Conversions::to_float(oid);
            sum_float += num;
            sum_squares_float += pow(num,2);
        } else {
            auto num = Conversions::to_double(oid);
            sum_double += num;
            sum_squares_double += pow(num,2);
        }
        count++;
    }

    // indicates the end of a group
    ObjectId get() override {
        if (count == 0) {
            return Conversions::pack_int(0);
        }

         if (type == Conversions::OPTYPE_INTEGER) {
            Decimal dev_pop_squared = (Decimal(sum_squares_integer)-Decimal(sum_integer)*Decimal(sum_integer)/Decimal(count))/Decimal((count));
            Decimal dev_pop = dev_pop_squared.sqrt();
            return Conversions::pack_decimal(dev_pop);
        } else if (type == Conversions::OPTYPE_DECIMAL) {
            Decimal dev_pop_squared = (Decimal(sum_squares_decimal)-Decimal(sum_decimal)*Decimal(sum_decimal)/Decimal(count))/Decimal((count));
            Decimal dev_pop = dev_pop_squared.sqrt();
            return Conversions::pack_decimal(dev_pop);
        } else if (type == Conversions::OPTYPE_FLOAT) {
            auto dev_pop_squared = (sum_squares_float-sum_float*sum_float/count)/count;
            auto dev_pop = sqrt(dev_pop_squared);
            return Conversions::pack_float(dev_pop);
        } else if (type == Conversions::OPTYPE_DOUBLE) {
            auto dev_pop_squared = (sum_squares_double-sum_double*sum_double/count)/count;
            auto dev_pop = sqrt(dev_pop_squared);
            return Conversions::pack_float(dev_pop);
        } else {
            return ObjectId::get_null();
        }
    }

    std::ostream& print_to_ostream(std::ostream& os) const override {
        os << "STDDEV_POP(DISTINCT ";
        BindingExprPrinter printer(os);
        expr->accept_visitor(printer);
        os << ")";
        return os;
    }

private:
    uint_fast8_t type;

    int64_t sum_integer;
    Decimal sum_decimal;
    float   sum_float;
    double  sum_double;

    int64_t sum_squares_integer;
    Decimal sum_squares_decimal;
    float   sum_squares_float;
    double  sum_squares_double;

    uint64_t count = 0;

    DistinctBindingHash hash_table;

    // Vector to pass oid to the hash table
    std::vector<ObjectId> oid_vec{1};
};
} // namespace GQL

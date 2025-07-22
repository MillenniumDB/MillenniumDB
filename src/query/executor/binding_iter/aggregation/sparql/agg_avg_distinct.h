#pragma once

#include "graph_models/common/datatypes/decimal.h"
#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/aggregation/agg.h"
#include "query/executor/binding_iter/binding_expr/binding_expr_printer.h"
#include "storage/index/hash/distinct_binding_hash/distinct_binding_hash.h"

namespace SPARQL {
class AggAvgDistinct : public Agg {
public:
    using Agg::Agg;

    AggAvgDistinct(VarId var_id, std::unique_ptr<BindingExpr> expr) :
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
        count = 0;
    }

    void process() override
    {
        if (type == Conversions::OpType::INVALID) {
            return;
        }

        auto oid = expr->eval(*binding);
        auto op_type = Conversions::calculate_optype(oid);

        if (op_type > Conversions::OpType::DOUBLE) {
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
        count++;
    }

    // indicates the end of a group
    ObjectId get() override
    {
        if (type == Conversions::OpType::INVALID) {
            return ObjectId::get_null();
            ;
        }
        if (count == 0) {
            return Conversions::pack_int(0);
        }

        if (type == Conversions::OpType::INTEGER) {
            Decimal avg;
            if (sum_integer % count == 0) {
                // Optimization to avoid expensive Decimal divide
                avg = Decimal(sum_integer / count);
            } else {
                avg = Decimal(sum_integer) / Decimal(count);
            }
            return Conversions::pack_decimal(avg);
        } else if (type == Conversions::OpType::DECIMAL) {
            auto avg = sum_decimal / Decimal(count);
            return Conversions::pack_decimal(avg);
        } else if (type == Conversions::OpType::FLOAT) {
            auto avg = sum_float / count;
            return Conversions::pack_float(avg);
        } else if (type == Conversions::OpType::DOUBLE) {
            auto avg = sum_double / count;
            return Conversions::pack_double(avg);
        } else {
            return ObjectId::get_null();
        }
    }

    std::ostream& print(std::ostream& os) const override
    {
        os << "AVG(DISTINCT ";
        BindingExprPrinter printer(os);
        printer.print(*expr);
        os << ")";
        return os;
    }

private:
    Conversions::OpType type;

    int64_t sum_integer;
    Decimal sum_decimal;
    float sum_float;
    double sum_double;

    int64_t count = 0;

    DistinctBindingHash hash_table;

    // Vector to pass oid to the hash table
    std::vector<ObjectId> oid_vec { 1 };
};
} // namespace SPARQL

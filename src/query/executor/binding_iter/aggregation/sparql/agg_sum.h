#pragma once

#include "graph_models/common/datatypes/decimal.h"
#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/aggregation/agg.h"
#include "query/executor/binding_iter/aggregation/sparql/uagg_sum.h"
#include "query/executor/binding_iter/binding_expr/binding_expr_printer.h"

namespace SPARQL {
class AggSum : public Agg {
public:
    using Agg::Agg;
    void begin() override
    {
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

        if (op_type > Conversions::OpType::DOUBLE) {
            type = Conversions::OpType::INVALID;
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
        switch (type) {
        case Common::Conversions::OpType::INTEGER:
            return Conversions::pack_int(sum_integer);
        case Common::Conversions::OpType::DECIMAL:
            return Conversions::pack_decimal(sum_decimal);
        case Common::Conversions::OpType::FLOAT:
            return Conversions::pack_float(sum_float);
        case Common::Conversions::OpType::DOUBLE:
            return Conversions::pack_double(sum_double);
        case Common::Conversions::OpType::TENSOR_FLOAT:
        case Common::Conversions::OpType::TENSOR_DOUBLE:
        case Common::Conversions::OpType::INVALID:
        default:
            return ObjectId::get_null();
        }
    }

    std::ostream& print(std::ostream& os) const override
    {
        os << "SUM(";
        BindingExprPrinter printer(os);
        printer.print(*expr);
        os << ")";
        return os;
    }

    bool is_pipelineble() const override
    {
        return true;
    }

    std::unique_ptr<UAgg> get_uagg() override
    {
        return std::make_unique<UAggSum>(var_id, expr.get());
    }

private:
    Conversions::OpType type;

    int64_t sum_integer;
    Decimal sum_decimal;
    float sum_float;
    double sum_double;
};
} // namespace SPARQL

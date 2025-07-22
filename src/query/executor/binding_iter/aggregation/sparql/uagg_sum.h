#pragma once

#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/aggregation/unordered_agg.h"
#include "query/executor/binding_iter/binding_expr/binding_expr_printer.h"

namespace SPARQL {
class UAggSum : public UAgg {
public:
    union DataSum {
        int64_t sum_integer;
        float sum_float;
        double sum_double;
        Decimal sum_decimal;
    };

    struct Data {
        Conversions::OpType sum_type;
        DataSum sum;
    };

    using UAgg::UAgg;

    virtual size_t get_offset() const override
    {
        return sizeof(Data);
    }

    void begin(void* vdata) override
    {
        Data* data = reinterpret_cast<Data*>(vdata);
        data->sum_type = Conversions::OpType::INTEGER;
        data->sum.sum_decimal = Decimal();
    }

    void process(void* vdata) override
    {
        Data* data = reinterpret_cast<Data*>(vdata);

        if (data->sum_type == Conversions::OpType::INVALID) {
            return;
        }

        auto oid = expr->eval(*binding);
        auto op_type = Conversions::calculate_optype(oid);

        if (op_type > Conversions::OpType::DOUBLE) {
            data->sum_type = Conversions::OpType::INVALID;
            return;
        }

        if (op_type > data->sum_type) {
            if (op_type == Conversions::OpType::DECIMAL) {
                data->sum.sum_decimal = Decimal(data->sum.sum_integer);
                data->sum_type = Conversions::OpType::DECIMAL;
            } else if (op_type == Conversions::OpType::FLOAT) {
                if (data->sum_type == Conversions::OpType::INTEGER) {
                    data->sum.sum_float = data->sum.sum_integer;
                } else {
                    data->sum.sum_float = data->sum.sum_decimal.to_float();
                }
                data->sum_type = Conversions::OpType::FLOAT;
            } else {
                if (data->sum_type == Conversions::OpType::INTEGER) {
                    data->sum.sum_double = data->sum.sum_integer;
                } else if (data->sum_type == Conversions::OpType::DECIMAL) {
                    data->sum.sum_double = data->sum.sum_decimal.to_double();
                } else {
                    data->sum.sum_double = data->sum.sum_float;
                }
                data->sum_type = Conversions::OpType::DOUBLE;
            }
        }

        if (data->sum_type == Conversions::OpType::INTEGER) {
            data->sum.sum_integer += Conversions::unpack_int(oid);
        } else if (data->sum_type == Conversions::OpType::DECIMAL) {
            data->sum.sum_decimal = data->sum.sum_decimal + Conversions::to_decimal(oid);
        } else if (data->sum_type == Conversions::OpType::FLOAT) {
            data->sum.sum_float += Conversions::to_float(oid);
        } else {
            data->sum.sum_double += Conversions::to_double(oid);
        }
    }

    // indicates the end of a group
    ObjectId get(void* vdata) override
    {
        Data* data = reinterpret_cast<Data*>(vdata);
        switch (data->sum_type) {
        case Common::Conversions::OpType::INTEGER:
            return Conversions::pack_int(data->sum.sum_integer);
        case Common::Conversions::OpType::DECIMAL:
            return Conversions::pack_decimal(data->sum.sum_decimal);
        case Common::Conversions::OpType::FLOAT:
            return Conversions::pack_float(data->sum.sum_float);
        case Common::Conversions::OpType::DOUBLE:
            return Conversions::pack_double(data->sum.sum_double);
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
};
} // namespace SPARQL

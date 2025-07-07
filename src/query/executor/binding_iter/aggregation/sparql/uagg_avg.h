#pragma once

#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/aggregation/unordered_agg.h"
#include "query/executor/binding_iter/binding_expr/sparql_binding_expr_printer.h"

namespace SPARQL {
class UAggAvg: public UAgg {
public:
    union DataSum {
        int64_t sum_integer;
        float sum_float;
        double sum_double;
        Decimal sum_decimal;
    };

    struct Data {
        int64_t count;
        DataSum sum;
        Conversions::OpType sum_type;
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
        data->count = 0;
        data->sum.sum_integer = 0;
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
        data->count++;
    }

    // indicates the end of a group
    ObjectId get(void* vdata) override
    {
        Data* data = reinterpret_cast<Data*>(vdata);
        if (data->sum_type == Conversions::OpType::INVALID) {
            return ObjectId::get_null();
        }

        if (data->count == 0) {
            return Conversions::pack_int(0);
        }

        if (data->sum_type == Conversions::OpType::INTEGER) {
            Decimal avg;
            if (data->sum.sum_integer % data->count == 0) {
                // Optimization to avoid expensive Decimal divide
                avg = Decimal(data->sum.sum_integer / data->count);
            } else {
                avg = Decimal(data->sum.sum_integer) / Decimal(data->count);
            }
            return Conversions::pack_decimal(avg);
        } else if (data->sum_type == Conversions::OpType::DECIMAL) {
            auto avg = data->sum.sum_decimal / Decimal(data->count);
            return Conversions::pack_decimal(avg);
        } else if (data->sum_type == Conversions::OpType::FLOAT) {
            auto avg = data->sum.sum_float / data->count;
            return Conversions::pack_float(avg);
        } else if (data->sum_type == Conversions::OpType::DOUBLE) {
            auto avg = data->sum.sum_double / data->count;
            return Conversions::pack_double(avg);
        } else {
            return ObjectId::get_null();
        }
    }

    std::ostream& print_to_ostream(std::ostream& os) const override
    {
        os << "AVG(";
        BindingExprPrinter printer(os);
        expr->accept_visitor(printer);
        os << ")";
        return os;
    }
};
} // namespace SPARQL

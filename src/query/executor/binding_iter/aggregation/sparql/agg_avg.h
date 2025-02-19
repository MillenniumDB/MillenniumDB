#pragma once

#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/aggregation/agg.h"
#include "query/executor/binding_iter/binding_expr/sparql_binding_expr_printer.h"

namespace SPARQL {
class AggAvg : public Agg {
public:
    using Agg::Agg;
    void begin() override {
        sum_integer = 0;
        sum_decimal = Decimal(0);
        sum_float = 0;
        sum_double = 0;

        type = Conversions::OPTYPE_INTEGER;
        count = 0;
    }

    void process() override {
        if (type == Conversions::OPTYPE_INVALID) {
            return;
        }

        auto oid = expr->eval(*binding);
        auto op_type = Conversions::calculate_optype(oid);

        if (op_type > Conversions::OPTYPE_DOUBLE) {
            type = Conversions::OPTYPE_INVALID;
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
        count++;
    }

    // indicates the end of a group
    ObjectId get() override {
        if (type == Conversions::OPTYPE_INVALID) {
            return ObjectId::get_null();;
        }

        if (count == 0) {
            return Conversions::pack_int(0);
        }

         if (type == Conversions::OPTYPE_INTEGER) {
            Decimal avg;
            if (sum_integer % count == 0) {
                // Optimization to avoid expensive Decimal divide
                avg = Decimal(sum_integer/count);
            } else {
                avg = Decimal(sum_integer) / Decimal(count);
            }
            return Conversions::pack_decimal(avg);
        } else if (type == Conversions::OPTYPE_DECIMAL) {
            auto avg = sum_decimal / Decimal(count);
            return Conversions::pack_decimal(avg);
        } else if (type == Conversions::OPTYPE_FLOAT) {
            auto avg = sum_float / count;
            return Conversions::pack_float(avg);
        } else if (type == Conversions::OPTYPE_DOUBLE) {
            auto avg = sum_double / count;
            return Conversions::pack_double(avg);
        } else {
            return ObjectId::get_null();
        }
    }

    std::ostream& print_to_ostream(std::ostream& os) const override {
        os << "AVG(";
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

    uint64_t count = 0;
};
} // namespace SPARQL

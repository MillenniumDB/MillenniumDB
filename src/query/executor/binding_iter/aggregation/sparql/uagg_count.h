#pragma once

#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/aggregation/unordered_agg.h"
#include "query/executor/binding_iter/binding_expr/sparql_binding_expr_printer.h"

namespace SPARQL {
class UAggCount : public UAgg {
public:
    struct Data {
        uint64_t count = 0;
    };

    using UAgg::UAgg;

    virtual size_t get_offset() const override
    {
        return sizeof(Data);
    }

    void begin(void* vdata) override
    {
        Data* data = reinterpret_cast<Data*>(vdata);
        data->count = 0;
    }

    void process(void* vdata) override
    {
        auto oid = expr->eval(*binding);
        if (oid.is_valid()) {
            Data* data = reinterpret_cast<Data*>(vdata);
            data->count++;
        }
    }

    // indicates the end of a group
    ObjectId get(void* vdata) override
    {
        Data* data = reinterpret_cast<Data*>(vdata);
        return Conversions::pack_int(data->count);
    }

    std::ostream& print_to_ostream(std::ostream& os) const override
    {
        os << "COUNT(";
        BindingExprPrinter printer(os);
        expr->accept_visitor(printer);
        os << ")";
        return os;
    }
};
} // namespace SPARQL

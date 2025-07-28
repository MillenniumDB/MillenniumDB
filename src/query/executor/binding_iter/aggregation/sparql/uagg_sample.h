
#pragma once

#include "query/executor/binding_iter/aggregation/unordered_agg.h"
#include "query/executor/binding_iter/binding_expr/binding_expr_printer.h"

namespace SPARQL {
class UAggSample : public UAgg {
public:
    struct Data {
        ObjectId sample;
    };

    using UAgg::UAgg;

    virtual size_t get_offset() const override
    {
        return sizeof(Data);
    }

    void begin(void* vdata) override
    {
        Data* data = reinterpret_cast<Data*>(vdata);
        data->sample = ObjectId::get_null();
    }

    void process(void* vdata) override
    {
        Data* data = reinterpret_cast<Data*>(vdata);
        if (!data->sample.is_null()) {
            return;
        }
        data->sample = expr->eval(*binding);
    }

    // indicates the end of a group
    ObjectId get(void* vdata) override
    {
        Data* data = reinterpret_cast<Data*>(vdata);
        return data->sample;
    }

    std::ostream& print(std::ostream& os) const override
    {
        os << "COUNT(";
        BindingExprPrinter printer(os);
        printer.print(*expr);
        os << ")";
        return os;
    }
};
} // namespace SPARQL

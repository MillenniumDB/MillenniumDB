
#pragma once

#include "graph_models/rdf_model/comparisons.h"
#include "query/executor/binding_iter/aggregation/unordered_agg.h"
#include "query/executor/binding_iter/binding_expr/sparql_binding_expr_printer.h"

namespace SPARQL {
class UAggMin : public UAgg {
public:
    struct Data {
        ObjectId min;
    };

    using UAgg::UAgg;

    virtual size_t get_offset() const override
    {
        return sizeof(Data);
    }

    void begin(void* vdata) override
    {
        Data* data = reinterpret_cast<Data*>(vdata);
        data->min = ObjectId::get_null();
    }

    void process(void* vdata) override
    {
        auto oid = expr->eval(*binding);
        Data* data = reinterpret_cast<Data*>(vdata);
        if (data->min.is_null()) {
            data->min = oid;
        } else if (oid.is_valid()) {
            auto cmp = SPARQL::Comparisons::compare(oid, data->min);
            if (cmp < 0) {
                data->min = oid;
            }
        }
    }

    // indicates the end of a group
    ObjectId get(void* vdata) override
    {
        Data* data = reinterpret_cast<Data*>(vdata);
        return data->min;
    }

    std::ostream& print_to_ostream(std::ostream& os) const override
    {
        os << "MIN(";
        BindingExprPrinter printer(os);
        expr->accept_visitor(printer);
        os << ")";
        return os;
    }
};
} // namespace SPARQL

#pragma once

#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/aggregation/agg.h"

namespace SPARQL {
class AggCountAll : public Agg {
public:
    using Agg::Agg;
    void begin() override {
        count = 0;
    }

    void process() override {
        count++;
    }

    // indicates the end of a group
    ObjectId get() override {
        return Conversions::pack_int(count);
    }

    std::ostream& print_to_ostream(std::ostream& os) const override {
        os << "COUNT(*)";
        return os;
    }

private:
    uint64_t count = 0;
};
} // namespace SPARQL
